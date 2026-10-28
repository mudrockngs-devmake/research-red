#!/usr/bin/env python3
"""Research Red: which story scripts the tests reach (script coverage).

Every scene records where the event script engine is, each frame (rr_run
"cover"; rr_checks.coverage_judge turns the addresses into script labels,
coverage.json in the scene's folder). This adds up one or more test runs:

  python3 tools/research_red/test/coverage.py RUN_DIR [RUN_DIR ...]

and writes COVERAGE.md in the first RUN_DIR: for every file in
data/scripts/research_red/, the labels a test reached and the ones none did.

Only labels where a script stops to wait (text, a movement, a delay, a
battle...) are counted: the engine is sampled once a frame, so a label that
only sets a flag and jumps on finishes between two samples. Those are listed
as "not measured".
"""
import bisect
import glob
import json
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))
SCRIPTS = os.path.join(REPO, "data", "scripts", "research_red")
WAITS = re.compile(r"^\s*(msgbox|message|waitmessage|waitbuttonpress|waitmovement|delay|trainerbattle\w*|"
                   r"dofieldeffect|waitstate|yesnobox|multichoice\w*|waitfanfare|fadescreen\w*|"
                   r"rr_\w+|playmoncry|waitmoncry|special\w*|warp\w*)\b", re.M)
_symbols = None


def symbols():
    """(sorted addresses, names) of the ROM's data labels."""
    global _symbols
    if _symbols is None:
        out = subprocess.run(["arm-none-eabi-nm", "-n", os.path.join(REPO, "pokefirered.elf")],
                             capture_output=True, text=True, check=True).stdout
        pairs = [(int(a, 16), n) for a, t, n in (l.split()[:3] for l in out.splitlines() if len(l.split()) >= 3)
                 if t in "Dd" and a.startswith("08")]
        _symbols = ([a for a, _ in pairs], [n for _, n in pairs])
    return _symbols


def labels_hit(values):
    """The script labels the engine was inside of, from cover addresses."""
    addrs, names = symbols()
    hit = set()
    for v in values:
        i = bisect.bisect_right(addrs, v) - 1
        if i >= 0 and v - addrs[i] < 0x1000:
            hit.add(names[i])
    return hit


def labels_by_file():
    """{file: [(label, waits)]} for every label in data/scripts/research_red/."""
    out = {}
    for path in sorted(glob.glob(os.path.join(SCRIPTS, "*.inc"))):
        text = open(path).read()
        parts = re.split(r"^(\w+)::?\s*$", text, flags=re.M)
        labels = []
        for name, body in zip(parts[1::2], parts[2::2]):
            if "_Text_" in name or "_Movement_" in name:
                continue
            labels.append((name, bool(WAITS.search(body))))
        out[os.path.basename(path)] = labels
    return out


def main(run_dirs):
    hit = set()
    for d in run_dirs:
        for f in glob.glob(os.path.join(d, "*", "coverage.json")):
            hit |= set(json.load(open(f)))
    rows, missed_all, total, reached = [], [], 0, 0
    for fname, labels in labels_by_file().items():
        measured = [n for n, w in labels if w]
        if not measured:
            continue
        got = [n for n in measured if n in hit]
        missed = [n for n in measured if n not in hit]
        total += len(measured)
        reached += len(got)
        rows.append(f"| {fname} | {len(got)}/{len(measured)} | {', '.join(missed) or '-'} |")
        missed_all += missed
    pct = 100 * reached / total if total else 0
    text = ("# Script coverage\n\n"
            f"Runs: {', '.join(os.path.basename(d.rstrip('/')) for d in run_dirs)}\n\n"
            f"**{reached}/{total} story script labels reached ({pct:.0f}%).** Labels that only set "
            "flags and jump on are not measured (they finish between two frames).\n\n"
            "| File | Reached | Never reached |\n|---|---|---|\n" + "\n".join(rows) + "\n")
    out = os.path.join(run_dirs[0], "COVERAGE.md")
    open(out, "w").write(text)
    print(f"{reached}/{total} labels reached ({pct:.0f}%). {out}")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    main(sys.argv[1:])
