#!/usr/bin/env python3
"""Research Red: AI-vs-AI battle simulations for balance (#18).

Plays key story fights many times in the real game (headless, like the
smoke tests) with the AI choosing moves for both sides (FLAG_RR_AI_VS_AI,
the flag behind test mode, but without test mode's Lv 30 team). Your side gets
a basic trainer's AI settings, like Brock's (see careful_player). Each run
puts a different seed into the game's random numbers before the fight, so
the runs differ the way real attempts would (damage rolls, crits, misses,
AI choices). Then it reads who won, how many turns it took and how many
Pokémon fainted on each side.

The player's team is a guess at a typical player at that point: the team the
debug skip hands out (data/scripts/debug.inc). Change that to change the guess.

Usage:
  python3 tools/research_red/test/balance.py              # every fight, 50 runs each
  python3 tools/research_red/test/balance.py brock --runs 20
  python3 tools/research_red/test/balance.py --list

Writes data/research_red/battle_sims.json (the numbers), then reruns
export_player_docs.py, which turns it into the "Simulated fights" section of
docs/BALANCE.md. The run's screenshots go to <workspace>/test-runs/balance/.
"""
import datetime
import json
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import smoke  # noqa: E402  (the intro's @bedroom, the debug skips, step())
import tester as T  # noqa: E402
from smoke import debug_script, step  # noqa: E402
from tester import play_until, ready  # noqa: E402

RESULTS = os.path.join(smoke.REPO, "data/research_red/battle_sims.json")
CHUNK = 10   # runs per runner process; the chunks of all fights run in parallel


def brock_setup():
    # Debug 10 is after Brock: take the win back (badge, Brock's flags) so the
    # cap is 14 again, and mark the Camper beaten so he doesn't spot us
    return (debug_script(smoke.SCRIPT_AFTER_BROCK)
            + T.clear_flag("FLAG_DEFEATED_BROCK") + T.clear_flag("FLAG_BADGE01_GET")
            + T.clear_flag("TRAINER_FLAGS_START + TRAINER_LEADER_BROCK")
            + T.set_flag("TRAINER_FLAGS_START + TRAINER_CAMPER_LIAM")
            # the path smoke.py's brock_rematch takes, up to facing Brock
            + step("RIGHT", 5) + step("UP", 14) + step("LEFT", 14) + step("DOWN", 5) + step("RIGHT", 7) + step("UP")
            + ready("gym") + step("UP", 16) + ["press UP 6", "wait 20"])


# name: (setup up to just before the fight, what starts it, description, the team guess)
FIGHTS = {
    "lab": (lambda: debug_script(smoke.SCRIPT_LAB),
            lambda: step("UP"),
            "Lab: you and Ivy vs Elm and Jenny (double)",
            "your starter + Ivy's, Lv 5 (the scene's own)"),
    "route22": (lambda: debug_script(smoke.SCRIPT_ROUTE22),
                lambda: step("LEFT"),
                "Route 22: the 6v6 double with Ivy vs Elm and Jenny",
                "debug 4 team: starter, Bulbasaur, Spearow, Rattata, Mankey, Nidoran M, all Lv 12"),
    "pewter": (lambda: debug_script(smoke.SCRIPT_AFTER_BROCK) + step("LEFT") + ["wait 20", "press DOWN 6", "wait 20"],
               lambda: ["press A", "wait 30"],
               "Pewter: Elm and Jenny's rematch outside the Pokémon Center",
               "debug 10 team, all Lv 14 (the cap before Brock; 1 badge)"),
    "brock": (brock_setup,
              lambda: ["press A", "wait 30"],
              "Pewter Gym: Brock",
              "debug 10 team, all Lv 14 (no badge, at the cap)"),
}


def seed_rng(seed):
    """Four words of the game's random number state, from the seed."""
    base, x, lines = T.G.sym("gRngValue"), seed * 2654435761 % 2**32 or 1, []
    for i in range(4):
        x = (x * 1103515245 + 12345) % 2**32
        lines.append(f"poke {base + i * 4:#x}:4 {x:#x}")
    return lines


def careful_player():
    """In an AI-vs-AI battle the player's side takes the AI settings of a
    battle partner, and with none it gets no settings: it picks moves almost
    at random (Tail Whip into a super effective Water Gun). Right after the
    battle starts (the settings are made once, at the start), give both of
    the player's battlers a basic trainer's: no useless moves, go for KOs,
    pick the strongest move. Roughly a careful player; Brock has the same."""
    ptr = T.G.sym("gAiThinkingStruct")
    off = T.G.const("offsetof(struct AiThinkingStruct, aiFlags)")
    flags = T.G.const("AI_FLAG_BASIC_TRAINER")
    lines = []
    for battler in (0, 2):
        at = off + battler * 8
        lines += [f"poke *{ptr:#x}+{at:#x}:4 {flags & 0xFFFFFFFF:#x}", f"poke *{ptr:#x}+{at + 4:#x}:4 {flags >> 32:#x}"]
    return lines


def fight_scenario(name, first_run, runs):
    def lines():
        setup, start = FIGHTS[name][0], FIGHTS[name][1]
        state = f"@{name}_{first_run}"
        cb2 = T.G.sym("gMain") + T.G.const("main_callback2")
        battle_over = f"{cb2:#x}:4 != {T.G.sym('BattleMainCB2') | 1:#x}"
        results = T.G.sym("gBattleResults")
        out = ["load @bedroom"] + setup() + T.set_flag("FLAG_RR_AI_VS_AI") + ["wait 30", f"save {state}"]
        for run in range(first_run, first_run + runs):
            out += ([f"load {state}"] + seed_rng(run) + start()
                    + [f"until start{run} 40000 A {T.G.in_battle()}"] + careful_player()
                    + ["wait 200", f"shot b{run:03d}a"]
                    + [f"until end{run} 60000 A {battle_over}", f"shot b{run:03d}b"]
                    + [f"peek outcome{run} {T.G.sym('gBattleOutcome'):#x}:1",
                       f"peek turns{run} {results + T.G.const('offsetof(struct BattleResults, battleTurnCounter)'):#x}:1",
                       f"peek mine{run} {results:#x}:1", f"peek theirs{run} {results + 1:#x}:1"])
        return out
    return lines


def collect(out_dir, wanted, runs, meta):
    """Read every chunk's runner.log into battle_sims.json."""
    data = json.load(open(RESULTS)) if os.path.exists(RESULTS) else {"fights": {}}
    for name in wanted:
        rows = []
        for first in range(0, runs, CHUNK):
            log = os.path.join(out_dir, f"{name}_{first}", "runner.log")
            peeks, ended = {}, set()
            for line in open(log).read().splitlines() if os.path.exists(log) else []:
                p = line.split()
                if p[0] == "PEEK":
                    peeks[p[1]] = int(p[2])
                elif p[0] == "UNTIL" and p[1].startswith("end") and p[2] == "ok":
                    ended.add(int(p[1][3:]))
            for run in range(first, min(first + CHUNK, runs)):
                if run not in ended:
                    rows.append({"seed": run, "outcome": "unfinished"})
                    continue
                outcome = {1: "won", 2: "lost", 3: "draw"}.get(peeks.get(f"outcome{run}"), "other")
                rows.append({"seed": run, "outcome": outcome, "turns": peeks.get(f"turns{run}"),
                             "your_fainted": peeks.get(f"mine{run}"), "their_fainted": peeks.get(f"theirs{run}")})
        done = [r for r in rows if r["outcome"] != "unfinished"]
        avg = (lambda k: round(sum(r[k] for r in done) / len(done), 1) if done else None)
        data["fights"][name] = {
            "fight": FIGHTS[name][2], "team": FIGHTS[name][3], **meta,
            "runs": len(rows), "won": sum(r["outcome"] == "won" for r in rows),
            "lost": sum(r["outcome"] == "lost" for r in rows),
            "unfinished": len(rows) - len(done),
            "avg_turns": avg("turns"), "avg_your_fainted": avg("your_fainted"), "avg_their_fainted": avg("their_fainted"),
            "results": rows,
        }
    data["fights"] = {k: data["fights"][k] for k in FIGHTS if k in data["fights"]}   # story order
    with open(RESULTS, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=1, ensure_ascii=False)
        f.write("\n")
    for name in wanted:
        d = data["fights"][name]
        print(f"  {name}: won {d['won']}/{d['runs']}, {d['avg_turns']} turns, "
              f"{d['avg_your_fainted']} of yours fainted on average"
              + (f", {d['unfinished']} unfinished" if d["unfinished"] else ""))
    subprocess.run([sys.executable, os.path.join(smoke.REPO, "tools/research_red/export_player_docs.py")], check=True)


def main():
    argv = sys.argv[1:]
    runs = 50
    if "--runs" in argv:
        i = argv.index("--runs")
        runs = int(argv[i + 1])
        del argv[i:i + 2]
    if "--list" in argv:
        for k, v in FIGHTS.items():
            print(f"{k:8} {v[2]}  (your team: {v[3]})")
        return
    wanted = [a for a in argv if not a.startswith("--")] or list(FIGHTS)
    for w in wanted:
        if w not in FIGHTS:
            sys.exit(f"unknown fight '{w}' (see --list)")
    scenarios = {"intro": smoke.SCENARIOS["intro"]}
    for name in wanted:
        for first in range(0, runs, CHUNK):
            scenarios[f"{name}_{first}"] = (fight_scenario(name, first, min(CHUNK, runs - first)),
                                            f"{FIGHTS[name][2]}, runs {first}-{min(first + CHUNK, runs) - 1}")
    version, commit = smoke.version_and_commit()
    base = os.path.join(smoke.WORKSPACE, "test-runs") if os.path.isdir(smoke.WORKSPACE) \
        else os.path.join(smoke.REPO, "build/test-runs")
    out_dir = os.path.join(base, "balance", f"{datetime.datetime.now():%Y-%m-%d_%H%M}_{version}_{commit}")
    meta = {"build": f"{version} ({commit})", "date": f"{datetime.date.today()}"}
    T.main(T.Suite(
        title=f"Battle simulations: {version} ({commit}), {runs} runs per fight",
        repo=smoke.REPO,
        rom=os.path.join(smoke.REPO, "pokefirered.gba"),
        scenarios=scenarios,
        out_dir=out_dir,
        runner=os.path.join(smoke.TOOLS, "rr_run"),
        first="intro",
        progress_var=smoke.STORY,
        map_names=smoke.MAP_NAMES,
        timeout=3600,
        setup_hint="run tools/research_red/test/setup_mgba.sh first",
        after_run=lambda d: collect(d, wanted, runs, meta),
    ), [k for k in scenarios if k != "intro"])


if __name__ == "__main__":
    main()
