#!/usr/bin/env python3
"""Research Red: checks that run on EVERY smoke-test scene (and the playthrough).

smoke.py hands these to the tester (Suite.always_start / always_end / judges),
so no scene has to ask for them:

  dialogue    each "Name:" line shows that speaker's portrait; narration shows
              none; no text box closes while its text is still printing
              (dialogue.py; #120, #76)
  leftovers   when the scene ends, nothing a battle or a scene switches on for
              a while is still on: the AI playing your side (outside test
              mode), "no whiteout" (#106-style leaks)
  brightness  how bright every screenshot is (mean, 0-255), saved as
              brightness.json in the scene's folder; a scene can demand a range
              with bright(shot, lo, hi) (day/night, #108)
  wild        a wild_probe's battle is a wild Pokemon from that map (#106)
  battles     every battle's start (battles.json); curated fights stay
              doubles (#128); two separate teams are flagged (#107)
  scripted    a scripted fight shows its opening and lines (#105)
  coverage    which event scripts ran (coverage.json; coverage.py adds up a
              whole run into COVERAGE.md: what no test reaches)

Each check reads game memory or the pictures, never the screenshots' timing.
"""
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import dialogue  # noqa: E402
from contact_sheet import read_png  # noqa: E402

TEST_MODE_MARK = "# rr: test-mode"   # smoke.py writes it when a scene turns on test mode (debug 9)

# Flags that must be off once a scene hands control back (outside test mode)
LEFTOVER_FLAGS = {
    "FLAG_RR_NO_WHITEOUT": "'no whiteout' is still on after the battle (it's set around story battles only)",
    "FLAG_RR_AI_VS_AI": "the AI is still playing your side after the scene (outside test mode)",
}


# --- dialogue -----------------------------------------------------------------
def dialogue_start(G):
    return dialogue.watch_lines(G)


def dialogue_judge(out, sdir, script):
    return dialogue.judge(out)


# --- leftovers ----------------------------------------------------------------
def leftovers_end(G):
    # judged only if the scene ended with no script running (some scenes stop mid-battle on purpose)
    lines = [f"peek leftover_script {G.sym('sGlobalScriptContextStatus'):#x}",
             f"# leftover-idle {G.const('script_shutdown')}"]
    for flag in LEFTOVER_FLAGS:
        addr, bit = G.flag_addr(flag)
        lines.append(f"peek leftover_{flag} {addr}")
        lines.append(f"# leftover-bit {flag} {bit}")
    return lines


def leftovers_judge(out, sdir, script):
    fails = []
    idle = re.search(r"^# leftover-idle (\d+)$", script, re.M)
    status = re.search(r"^PEEK leftover_script (\d+)$", out, re.M)
    if not (idle and status and status[1] == idle[1]):
        return [], []
    bits = dict(re.findall(r"^# leftover-bit (\S+) (\d+)$", script, re.M))
    for flag, why in LEFTOVER_FLAGS.items():
        if flag == "FLAG_RR_AI_VS_AI" and TEST_MODE_MARK in script:
            continue
        m = re.search(rf"^PEEK leftover_{flag} (\d+)$", out, re.M)
        if m and flag in bits and int(m[1]) & int(bits[flag]):
            fails.append(why)
    return fails, []


# --- brightness ---------------------------------------------------------------
def luma(path):
    """Mean brightness of a screenshot, 0 (black) to 255 (white)."""
    w, h, px = read_png(path)
    vals = [0.299 * c[0] + 0.587 * c[1] + 0.114 * c[2] for row in px[::4] for c in row[::4]]
    return sum(vals) / len(vals)


def bright(shot, lo, hi):
    """Script lines: FAIL unless screenshot `shot` (taken earlier) has a mean
    brightness between lo and hi (0-255)."""
    return [f"# expect-bright {shot} {lo} {hi}"]


def same_bright(shots, tolerance=6):
    """Script lines: FAIL unless the screenshots (same place, taken earlier)
    are within `tolerance` of each other in brightness (one light level)."""
    return [f"# expect-same-bright {tolerance} {' '.join(shots)}"]


def brightness_judge(out, sdir, script):
    fails, values = [], {}
    for f in sorted(os.listdir(sdir)):
        if f.endswith(".png"):
            values[f[:-4]] = round(luma(os.path.join(sdir, f)), 1)
    json.dump(values, open(os.path.join(sdir, "brightness.json"), "w"), indent=0)
    for shot, lo, hi in re.findall(r"^# expect-bright (\S+) (\S+) (\S+)$", script, re.M):
        v = values.get(shot)
        if v is None:
            fails.append(f"{shot}.png missing (brightness check)")
        elif not float(lo) <= v <= float(hi):
            fails.append(f"{shot}.png brightness {v} is outside {lo}-{hi}")
    for tol, shots in re.findall(r"^# expect-same-bright (\S+) (.+)$", script, re.M):
        vals = {sh: values.get(sh) for sh in shots.split()}
        known = [v for v in vals.values() if v is not None]
        if known and max(known) - min(known) > float(tol):
            fails.append("not one light level: " + ", ".join(f"{k} {v}" for k, v in vals.items()))
    return fails, []


# --- wild probe (nav.wild_probe, #106) -----------------------------------------
def wild_judge(out, sdir, script):
    """A wild_probe in the scene: what appeared must be a wild Pokemon from
    that map's encounter table, in a wild battle (not a trainer's team)."""
    m = re.search(r"^# wild-map (\S+)$", script, re.M)
    if not m:
        return [], []
    allowed = set(re.search(r"^# wild-allowed (.*)$", script, re.M)[1].split())
    trainer_bit = int(re.search(r"^# wild-trainer-bit (\d+)$", script, re.M)[1])
    peeks = dict(re.findall(r"^PEEK (wild_\w+) (\d+)$", out, re.M))
    guarded = re.search(r"^# wild-map-id (\d+)$", script, re.M)
    if guarded:
        # the playthrough plays wild battles out as they come: judge the
        # battles recorded on that map after the probe started
        mark = re.search(r"^MARK wild_probe (\d+)$", out, re.M)
        start = int(mark[1]) if mark else 0
        found = [b for b in battle_list(out, script) if b["map"] == int(guarded[1]) and b["frame"] >= start
                 and not b["curated"]]
        if not found:
            return [f"wild probe on {m[1]}: no wild battle started"], []
        b = found[0]
        peeks = {"wild_species_1": str(b["species_1"]), "wild_species_3": str(b["species_3"] if b["double"] else 0),
                 "wild_trainer_flags": str(trainer_bit if b["trainer"] else 0)}
    elif "wild_species_1" not in peeks or re.search(r"^UNTIL wild_battle timeout", out, re.M):
        return [f"wild probe on {m[1]}: no wild battle started"], []
    fails = []
    if int(peeks["wild_trainer_flags"]) & trainer_bit:
        fails.append(f"wild probe on {m[1]}: the battle is a trainer battle")
    for slot in ("wild_species_1", "wild_species_3"):
        sp = peeks.get(slot, "0")
        if slot == "wild_species_3" and sp == "0":
            continue      # a single battle
        if sp not in allowed:
            fails.append(f"wild probe on {m[1]}: species {sp} isn't in its encounter table (a leftover team? #106)")
    return fails, []


# --- battles (#128, #107) ---------------------------------------------------------
# Every battle's start is recorded (battles.json in the scene's folder). Curated
# fights (Research Red's own trainers and the Gym leaders) keep their teams
# whatever the Research Rules say; with "trainers as singles" they may be
# singles (#128, owner 2026-10-28). A pair (Elm and Jenny, Oak and Daisy) is
# one trainer with one team and a combined picture (#107): a curated fight
# against two trainers with two separate teams is a failure. A curated fight
# you win must have beaten every one of their Pokémon (#139: all six come out).
def battles_start(G):
    cb2 = G.sym("gMain") + G.const("main_callback2")
    params = G.sym("gTrainerBattleParameter")
    opp_a = params + G.const("offsetof(TrainerBattleParameter, params.opponentA)")
    opp_b = params + G.const("offsetof(TrainerBattleParameter, params.opponentB)")
    curated = [G.const(n) for n in curated_trainers()]
    mons, size = G.sym("gBattleMons"), G.const("sizeof(struct BattlePokemon)")
    sp = G.const("offsetof(struct BattlePokemon, species)")
    party, psize = G.sym("gPlayerParty"), G.const("sizeof(struct Pokemon)")
    hp = G.const("offsetof(struct Pokemon, hp)")
    able = [f"watch b_hp{i} {party + i * psize + hp:#x}:2" for i in range(6)]   # who can fight
    able.append(f"watch b_pcount {G.sym('gPlayerPartyCount'):#x}:1")
    foes = G.sym("gEnemyParty")
    able += [f"watch b_ehp{i} {foes + i * psize + hp:#x}:2" for i in range(6)]   # who they've lost
    return able + [f"watch b_map {G.map_addr()}", f"watch b_sp1 {mons + size + sp:#x}:2",
            f"watch b_sp3 {mons + 3 * size + sp:#x}:2",
            f"watch b_cb2 {cb2:#x}:4", f"watch b_flags {G.sym('gBattleTypeFlags'):#x}:4",
            f"watch b_count {G.sym('gEnemyPartyCount'):#x}:1",
            f"watch b_oppa {opp_a:#x}:2", f"watch b_oppb {opp_b:#x}:2",
            f"# battle-main {G.sym('BattleMainCB2') | 1}",
            f"# battle-bits {G.const('BATTLE_TYPE_DOUBLE')} {G.const('BATTLE_TYPE_TRAINER')} "
            f"{G.const('BATTLE_TYPE_TWO_OPPONENTS')}",
            f"# battle-curated {' '.join(map(str, curated))}",
            f"# battle-names " + " ".join(f"{G.const(n)}={n}" for n in curated_trainers() + list(SCRIPTED_FIGHT_LINES))]


def curated_trainers():
    """TRAINER_RR_* (except Oak's lab fights and the scripted fights, #105) and the Gym leaders."""
    text = open(os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(HERE))),
                             "include/constants/opponents.h")).read()
    names = re.findall(r"^\s+(TRAINER_(?:RR_\w+|LEADER_\w+)),", text, re.M)
    return [n for n in names if "OAK_LAB" not in n and n not in SCRIPTED_FIGHT_LINES]


def battle_list(out, script):
    """Every battle's start from the watch lines: frame, map (as the game
    stores it), single/double, trainers, team size, the first foes' species."""
    main = int(re.search(r"^# battle-main (\d+)$", script, re.M)[1])
    dbl, trainer, two = map(int, re.search(r"^# battle-bits (\d+) (\d+) (\d+)$", script, re.M).groups())
    curated = set(map(int, re.search(r"^# battle-curated(.*)$", script, re.M)[1].split()))
    names = dict(kv.split("=") for kv in re.search(r"^# battle-names(.*)$", script, re.M)[1].split())
    now, battles = {}, []
    for f, k, v in sorted(((int(p[2]), p[1], int(p[3])) for p in (l.split() for l in out.splitlines())
                           if p and p[0] == "WATCH" and p[1].startswith("b_")), key=lambda e: e[0]):
        was_in = now.get("b_cb2") == main
        before = now.get(k, 0)
        now[k] = v
        if k.startswith("b_ehp") and was_in and battles and before > 0 and v == 0:
            battles[-1]["beaten"][int(k[-1])] = f
        if k == "b_cb2" and was_in and v != main and battles:
            # the game clears what's left of their team at the end of a battle (several at
            # once, same frame): only one-at-a-time faints count
            at = list(battles[-1]["beaten"].values())
            battles[-1]["foes_beaten"] = sum(1 for t in at if at.count(t) == 1)
            battles[-1]["player_able_end"] = sum(1 for i in range(min(6, now.get("b_pcount", 6)))
                                                 if now.get(f"b_hp{i}", 0) > 0)
        if k in ("b_sp1", "b_sp3") and battles and battles[-1].get("fresh_" + k) and now.get("b_cb2") == main:
            battles[-1]["species_" + k[-1]] = v        # the foes are loaded just after the start
            battles[-1]["fresh_" + k] = v == 0
        if k == "b_cb2" and v == main and not was_in:
            flags = now.get("b_flags", 0)
            battles.append({"frame": f, "map": now.get("b_map", 0), "species_1": now.get("b_sp1", 0),
                            "species_3": now.get("b_sp3", 0), "fresh_b_sp1": True, "fresh_b_sp3": True,
                            "beaten": {},
                            "trainer": bool(flags & trainer), "double": bool(flags & dbl),
                            "two_opponents": bool(flags & two), "enemy_count": now.get("b_count", 0),
                            "player_able": sum(1 for i in range(min(6, now.get("b_pcount", 6)))
                                               if now.get(f"b_hp{i}", 0) > 0),
                            # (the trainer numbers stay from the last trainer battle: wild ones get none)
                            "opponent_a": names.get(str(now.get("b_oppa")), now.get("b_oppa")) if flags & trainer else None,
                            "opponent_b": names.get(str(now.get("b_oppb")), now.get("b_oppb")) if flags & trainer else None,
                            "curated": bool(flags & trainer) and now.get("b_oppa") in curated})
    for b in battles:
        b.pop("fresh_b_sp1", None)
        b.pop("fresh_b_sp3", None)
        b.pop("beaten", None)
    return battles


def battles_judge(out, sdir, script):
    battles = battle_list(out, script)
    json.dump(battles, open(os.path.join(sdir, "battles.json"), "w"), indent=1)
    fails, warns = [], []
    # the playthrough marks its Research Rules setting (# trainers doubles/singles)
    mode = re.search(r"^# trainers (doubles|singles)$", script, re.M)
    for b in battles:
        if mode and b["trainer"]:
            who = b["opponent_a"] or "a trainer"
            if mode[1] == "doubles" and not b["double"]:
                fails.append(f"{who}: a single battle with trainers set to doubles (#128)")
            elif mode[1] == "singles" and b["double"]:
                warns.append(f"{who}: a double with trainers set to singles (fine only for FireRed's own doubles, #128)")
        if not b["curated"]:
            continue
        who = b["opponent_a"] + (f" + {b['opponent_b']}" if b["two_opponents"] else "")
        if b["two_opponents"]:
            fails.append(f"{who}: two separate teams ({b['enemy_count']} Pokémon), not one team under one AI (#107)")
        # a win (you still have someone standing) must have beaten their whole team (#139)
        if b.get("player_able_end") and b.get("foes_beaten", 0) < b["enemy_count"]:
            fails.append(f"{who}: the battle ended with {b['foes_beaten']} of their {b['enemy_count']} "
                         f"Pokémon beaten (#139)")
    return fails, warns


# --- scripted fights (#105) ----------------------------------------------------------
# A scripted fight (src/rr_scripted_fight.c) shows its own opening and its
# lines, in that order; its trainer number says which fight ran.
SCRIPTED_FIGHT_LINES = {
    "TRAINER_RR_OAK_WATCH": ["Elm and Jenny challenge Prof. Oak!", "Elm: ", "Oak: "],
    "TRAINER_RR_BROCK_WATCH": ["Elm and Jenny challenge Brock!", "Brock: ", "Elm: "],
}


def battle_texts(out):
    """The battle message box's texts in order (each once in a row)."""
    texts = []
    for p in (l.split() for l in out.splitlines()):
        if len(p) > 3 and p[0] == "WATCHTEXT" and p[1] == "btext":
            t = dialogue.decode(p[3])
            if not texts or texts[-1] != t:
                texts.append(t)
    return texts


def scripted_judge(out, sdir, script):
    fights = [b["opponent_a"] for b in battle_list(out, script) if b["opponent_a"] in SCRIPTED_FIGHT_LINES]
    if not fights:
        return [], []
    texts = battle_texts(out)
    fails = []
    for name in sorted(set(fights)):
        at = 0
        for want in SCRIPTED_FIGHT_LINES[name]:
            hit = next((i for i in range(at, len(texts)) if texts[i].startswith(want)), None)
            if hit is None:
                fails.append(f"{name}: the scripted fight never showed \"{want}\" (#105)")
                break
            at = hit + 1
    return fails, []


# --- script coverage (coverage.py) -------------------------------------------------
def coverage_start(G):
    """Record where the event script engine is, every frame."""
    ctx = G.sym("sGlobalScriptContext")
    ptr = ctx + G.const("offsetof(struct ScriptContext, scriptPtr)")
    # text waits inside FireRed's shared message scripts (Std_MsgboxDefault...):
    # the story script that called them is on the call stack (return addresses)
    stack = ctx + G.const("offsetof(struct ScriptContext, stack)")
    return [f"cover {ptr:#x}:4"] + [f"cover {stack + 4 * i:#x}:4" for i in range(3)]


def coverage_judge(out, sdir, script):
    import coverage
    values = [int(v) for v in re.findall(r"^COVER (\d+)$", out, re.M)]
    json.dump(sorted(coverage.labels_hit(values)), open(os.path.join(sdir, "coverage.json"), "w"), indent=0)
    return [], []


ALWAYS_START = [dialogue_start, battles_start, coverage_start]
def position_end(G):
    """Where the scene left the player (PEEK where_map/x/y in runner.log; x, y
    carry the 7-tile border: subtract 7 for Porymap's numbers)."""
    import nav
    return nav.where(G)


ALWAYS_END = [leftovers_end, position_end]
JUDGES = [dialogue_judge, leftovers_judge, brightness_judge, wild_judge, battles_judge, scripted_judge,
          coverage_judge]
