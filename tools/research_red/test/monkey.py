#!/usr/bin/env python3
"""Research Red: random-button stress test (a "monkey test", #18).

From a few starting points, presses random buttons for a while (walking,
menus, battles, the bag, the Pokédex...) and watches for what a player
mashing buttons could break: the blue error screen, a frozen screen, a
softlock. It uses the same tester as smoke.py (tester.py), so the report
looks the same.

The presses come from a seed: the same seed on the same build presses the
very same buttons, so anything it finds can be played again and looked at.
The debug menu is off limits: if the random presses open it, the runner
backs out with B straight away (rr_run "guard"), so no debug warps or gifts.

Usage:
  python3 tools/research_red/test/monkey.py                     # every start, 10 game minutes each, a new seed
  python3 tools/research_red/test/monkey.py --seed 1234 pewter  # replay one start with that seed
  python3 tools/research_red/test/monkey.py --minutes 30 --seeds 3
  python3 tools/research_red/test/monkey.py --list

Output: <workspace>/test-runs/monkey/<date>_<version>_<commit>_seed<N>/
  REPORT.md, a contact sheet per start, screenshots every 10 seconds
It runs at low priority; run it when you like, not on every build.
"""
import datetime
import os
import random
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import smoke  # noqa: E402  (Research Red's scenes: the intro saves @bedroom, the debug skips)
import tester as T  # noqa: E402

FPS = 60
SHOT_EVERY = 600        # frames between screenshots (10 seconds)

# Where the monkey starts: (setup lines after @bedroom, description).
# Each is a debug skip, so it starts with a team and the story where it says.
STARTS = {
    "bedroom": (lambda: [], "New game in the bedroom: breakfast and the first scenes are reachable"),
    "pallet": (lambda: smoke.debug_script(smoke.SCRIPT_AFTER_LAB), "Pallet after the lab, with a starter"),
    "route22": (lambda: smoke.debug_script(smoke.SCRIPT_ROUTE22), "Route 22 scene skip"),
    "lunch": (lambda: smoke.debug_script(smoke.SCRIPT_LUNCH), "Lunch scene skip (Viridian)"),
    "pewter": (lambda: smoke.debug_script(smoke.SCRIPT_AFTER_BROCK), "After Brock in Pewter: Ranch Pager, Fly, rematches"),
    "wild": (lambda: smoke.debug_script(smoke.SCRIPT_AFTER_LAB) + smoke.debug_script(smoke.SCRIPT_CATCH),
             "A wild battle with balls in the bag"),
}

# (weight, kind) of each random action
ACTIONS = [(40, "walk"), (20, "A"), (14, "B"), (6, "START"), (2, "SELECT"), (4, "L"), (4, "R"), (10, "wait")]


def debug_menu_open():
    """Any active task running code from src/debug.o (the overworld debug
    menu). A finished task keeps its old function, so isActive counts too."""
    lo, hi = debug_text_range()
    base, size = T.G.sym("gTasks"), T.G.const("sizeof(struct Task)")
    active = T.G.const("offsetof(struct Task, isActive)")
    groups = [f"{base + i * size:#x}:4 >= {lo:#x} && {base + i * size:#x}:4 < {hi:#x}"
              f" && {base + i * size + active:#x}:1 != 0" for i in range(16)]
    return " || ".join(groups)


def debug_text_range():
    """The debug menu's code, from the linker map next to the ROM."""
    text = open(os.path.join(smoke.REPO, "pokefirered.map")).read()
    m = re.search(r"^ \.text\s+(0x[0-9a-f]+)\s+(0x[0-9a-f]+) \S*src/debug\.o$", text, re.M)
    if not m:
        sys.exit("src/debug.o's code not found in pokefirered.map: build first")
    start = int(m[1], 16)
    return start, start + int(m[2], 16)


def monkey_lines(rng, minutes):
    """Random presses for `minutes` of game time, a screenshot every 10 s."""
    lines, frames, next_shot, shot = [], 0, 0, 0
    kinds = [k for w, k in ACTIONS for _ in range(w)]
    while frames < minutes * 60 * FPS:
        if frames >= next_shot:
            lines.append(f"shot m{shot:04d}")
            shot += 1
            next_shot += SHOT_EVERY
        kind = rng.choice(kinds)
        if kind == "walk":
            n = rng.choice([4, 8, 16, 16, 32, 48])
            lines.append(f"hold {rng.choice(['UP', 'DOWN', 'LEFT', 'RIGHT'])} {n}")
            frames += n
        elif kind == "wait":
            n = rng.choice([10, 30, 60, 120])
            lines.append(f"wait {n}")
            frames += n
        else:
            lines.append(f"press {kind}")
            frames += 8
    return lines + ["wait 120", "shot m9999"]


def scenario(name, seed, minutes):
    def lines():
        rng = random.Random(f"{seed}-{name}")
        setup = STARTS[name][0]()
        return (["load @bedroom"] + setup + ["wait 60", f"guard B {debug_menu_open()}"]
                + monkey_lines(rng, minutes))
    return lines


def main():
    argv = sys.argv[1:]

    def option(flag, default):
        if flag in argv:
            i = argv.index(flag)
            value = argv[i + 1]
            del argv[i:i + 2]
            return int(value)
        return default
    seed = option("--seed", None)
    minutes = option("--minutes", 10)
    seeds = option("--seeds", 1)
    if "--list" in argv:
        for k, (_, desc) in STARTS.items():
            print(f"{k:10} {desc}")
        return
    if seed is None:
        seed = int(datetime.datetime.now().strftime("%m%d%H%M"))
    wanted = [a for a in argv if not a.startswith("--")] or list(STARTS)
    for w in wanted:
        if w not in STARTS:
            sys.exit(f"unknown start '{w}' (see --list)")

    scenarios = {"intro": smoke.SCENARIOS["intro"]}
    for s in range(seed, seed + seeds):
        for w in wanted:
            key = w if seeds == 1 else f"{w}_{s}"
            scenarios[key] = (scenario(w, s, minutes), f"{STARTS[w][1]} (seed {s}, {minutes} min)")
    version, commit = smoke.version_and_commit()
    base = os.path.join(smoke.WORKSPACE, "test-runs") if os.path.isdir(smoke.WORKSPACE) \
        else os.path.join(smoke.REPO, "build/test-runs")
    stamp = f"{datetime.datetime.now():%Y-%m-%d_%H%M}_{version}_{commit}_seed{seed}"
    T.main(T.Suite(
        title=f"Monkey test: {version} ({commit}), seed {seed}, {minutes} game minutes per start",
        repo=smoke.REPO,
        rom=os.path.join(smoke.REPO, "pokefirered.gba"),
        scenarios=scenarios,
        out_dir=os.path.join(base, "monkey", stamp),
        runner=os.path.join(smoke.TOOLS, "rr_run"),
        first="intro",
        progress_var=smoke.STORY,
        map_names=smoke.MAP_NAMES,
        timeout=max(1200, minutes * 120),
        setup_hint="run tools/research_red/test/setup_mgba.sh first",
    ), [k for k in scenarios if k != "intro"])


if __name__ == "__main__":
    main()
