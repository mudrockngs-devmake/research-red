#!/usr/bin/env python3
"""Research Red (#90): TM Case descriptions for TM51-TM100.

The TM items TM51-TM100 are numbered (ITEM_TM51 ...), but the move each one
teaches comes from the list in include/constants/tms_hms.h. When that list
changes, the item's description has to follow, or the TM Case shows the old
move's text. This writes each TM's description into src/data/items.h from
data/research_red/tm_descriptions.txt (one line per move).

  python3 tools/research_red/build_tm_descriptions.py           # write items.h
  python3 tools/research_red/build_tm_descriptions.py --check   # fail if stale
"""
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TMS_HMS = os.path.join(REPO, "include/constants/tms_hms.h")
ITEMS = os.path.join(REPO, "src/data/items.h")
SOURCE = os.path.join(REPO, "data/research_red/tm_descriptions.txt")
FIRST, LAST = 51, 100
MAX_CHARS, MAX_LINES = 22, 3   # what fits the TM Case's description box


def tm_moves():
    text = open(TMS_HMS).read().split("#define FOREACH_HM")[0]
    return re.findall(r"F\(([A-Z_0-9]+)\)", text)


def descriptions(problems):
    out = {}
    for n, line in enumerate(open(SOURCE, encoding="utf-8"), 1):
        line = line.split("#")[0].strip()
        if not line:
            continue
        move, _, text = (p.strip() for p in line.partition("|"))
        lines = [l.strip() for l in text.split("/")]
        if len(lines) > MAX_LINES or any(len(l) > MAX_CHARS for l in lines):
            problems.append(f"tm_descriptions.txt:{n}: {move}: at most {MAX_LINES} lines of {MAX_CHARS} characters")
        out[move] = lines
    return out


def entry(lines):
    body = "\n".join(f'            "{l}\\n"' for l in lines[:-1])
    last = f'            "{lines[-1]}"), // Research Red (#90, tools/research_red/build_tm_descriptions.py)'
    return "COMPOUND_STRING(\n" + (body + "\n" if body else "") + last


def build(problems):
    moves = tm_moves()
    desc = descriptions(problems)
    items = open(ITEMS, encoding="utf-8").read()
    for n in range(FIRST, LAST + 1):
        move = moves[n - 1]
        if move not in desc:
            problems.append(f"TM{n} ({move}) has no line in data/research_red/tm_descriptions.txt")
            continue
        pat = re.compile(r"(\[ITEM_TM%d\] =\s*\{.*?\.description = )COMPOUND_STRING\(.*?\),[^\n]*" % n, re.S)
        items, count = pat.subn(lambda m: m.group(1) + entry(desc[move]), items, count=1)
        if not count:
            problems.append(f"ITEM_TM{n} not found in src/data/items.h")
    return items


def main():
    check = "--check" in sys.argv
    problems = []
    new = build(problems)
    for p in problems:
        print(p)
    if problems:
        return 1
    old = open(ITEMS, encoding="utf-8").read()
    if check:
        if new != old:
            print("TM descriptions are stale: run tools/research_red/build_tm_descriptions.py (#90)")
            return 1
        return 0
    if new != old:
        open(ITEMS, "w", encoding="utf-8").write(new)
    print(f"TM{FIRST}-TM{LAST} descriptions -> src/data/items.h")
    return 0


if __name__ == "__main__":
    sys.exit(main())
