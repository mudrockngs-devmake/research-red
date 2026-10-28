#!/usr/bin/env python3
"""Research Red: the settings matrix (#128, #107).

Plays every curated fight (Route 22, the Pewter rematch, Oak + Daisy, Delia,
Brock's rematch) once with each Research Rules setting a player can pick:

  default     as a new game: trainer battles doubles, wild battles singles
  singles     "trainers as singles" (FLAG_RR_FORCE_DOUBLES off)
  wilddbl     wild doubles on (FLAG_RR_FORCE_DOUBLE_WILD)

The battle recorder (rr_checks.py, on every scene) writes battles.json and
FAILs a curated fight that starts as a single (#128); two separate opponent
teams are a WARN for #107 until that's built. The wild probe after Oak +
Daisy checks the leak from #106 under each setting too.

  python3 tools/research_red/test/matrix.py [scene ...] [--list]

Output: test-runs/matrix/<date>_<version>_<commit>/ (REPORT.md, MATRIX.md).
Slow (each fight is a full AI battle): run before a build, not every change.
"""
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import nav  # noqa: E402
import smoke  # noqa: E402
import tester as T  # noqa: E402

FIGHTS = {
    "route22": smoke.sc_route22,
    "pewter": smoke.sc_pewter,
    "oak": smoke.sc_oak_rematch,
    "delia": smoke.sc_delia_battle,
    "brock": smoke.sc_brock_rematch,
}
SETTINGS = {
    "default": lambda: [],
    # trainers as singles; battle style Set so a KO doesn't ask "Will you switch?"
    # (nobody answers it while the AI plays your side: a test-mode limit)
    "singles": lambda: T.clear_flag("FLAG_RR_FORCE_DOUBLES") + smoke.options_bits(2),
    "wilddbl": lambda: T.set_flag("FLAG_RR_FORCE_DOUBLE_WILD"),
}


def with_setting(fight, setting):
    """The fight's scene with the setting poked in once the debug skips are
    done (just before its first wait-for-something, which starts the walk)."""
    def scene():
        lines = FIGHTS[fight]()
        i = next(n for n, ln in enumerate(lines) if ln.startswith("autoshot") or ln.startswith("until"))
        lines = lines[:i] + SETTINGS[setting]() + [f"# matrix {fight} {setting}"] + lines[i:]
        if fight == "oak":   # then the #106 wild probe, as a player would walk out
            lines += nav.wild_probe(T.G, ("MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", 6, 4), "MAP_ROUTE1")
        return lines
    return scene


SCENES = {"intro": smoke.SCENARIOS["intro"]}
for f in FIGHTS:
    for st in SETTINGS:
        SCENES[f"{f}_{st}"] = (with_setting(f, st), f"{f} with the '{st}' Research Rules setting")


def summary(out_dir):
    """MATRIX.md: one row per fight, one column per setting."""
    rows = ["| Fight | " + " | ".join(SETTINGS) + " |", "|---|" + "---|" * len(SETTINGS)]
    for f in FIGHTS:
        cells = []
        for st in SETTINGS:
            path = os.path.join(out_dir, f"{f}_{st}", "battles.json")
            if not os.path.exists(path):
                cells.append("not run")
                continue
            parts = []
            for b in json.load(open(path)):
                if b["curated"]:
                    kind = "double" if b["double"] else "**single**"
                    teams = "2 teams" if b["two_opponents"] else "1 team"
                    parts.append(f"{b['opponent_a'].removeprefix('TRAINER_')}: {kind}, {teams}, {b['enemy_count']} mons")
            cells.append("<br>".join(parts) or "no curated battle")
        rows.append(f"| {f} | " + " | ".join(cells) + " |")
    open(os.path.join(out_dir, "MATRIX.md"), "w").write(
        "# Settings matrix\n\nHow each curated fight started under each Research Rules setting "
        "(#128: always a double; #107: one team of 6).\n\n" + "\n".join(rows) + "\n")


if __name__ == "__main__":
    smoke.main(SCENES, kind="matrix", title="Settings matrix",
               argv=sys.argv[1:] + ["--no-compare"], after_run=summary)
