#!/usr/bin/env python3
"""Research Red: release notes for one version, from docs/CHANGELOG.md.

Prints the version's changelog section plus how to patch, for GitHub Releases
(used by .github/workflows/build-bps.yml when a version tag is pushed).
Usage: python3 tools/research_red/release_notes.py v0.0.14
"""
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BASE_SHA1 = "dd5945db9b930750cb39d00c84da8571feebf417"


def main():
    version = sys.argv[1]
    text = open(os.path.join(REPO, "docs/CHANGELOG.md"), encoding="utf-8").read()
    m = re.search(rf"^## {re.escape(version)}\s*$(.*?)(?=^## |\Z)", text, re.M | re.S)
    body = m.group(1).strip() if m else "(No changelog entry for this version.)"
    print(f"""{body}

## How to play
1. Get **Pokémon FireRed (USA) Rev 1** (v1.1) as a `.gba` file you dumped yourself.
   SHA1 `{BASE_SHA1}`. Other versions (Rev 0, Europe, LeafGreen) won't work.
2. Apply the `.bps` patch below with [Rom Patcher JS](https://www.marcrobledo.com/RomPatcher.js/) or Floating IPS.
3. Play in mGBA (recommended) or another accurate GBA emulator.

Test build: saves may not carry over between builds before v0.1 (see the Save line above).
Report problems on the Issues tab (Bug report form).""")


if __name__ == "__main__":
    main()
