#!/usr/bin/env python3
"""Research Red: the anime/lore reference database (not game data).

Source of truth: docs/reference/kanto_anime.csv (edit it in Excel or a text
editor). This script writes docs/reference/KANTO_ANIME.md (grouped by place)
and copies the sheet to the workspace (E:) with an Excel-friendly encoding.

Columns: Place, Order (story order of the place), Source, Episode, What
happens, Characters, Pokémon, Idea for Research Red, Status (idea / used / no), Checked (wiki = read
from Bulbapedia; memory = details from Claude's memory, to verify).

Usage: python3 tools/research_red/build_reference.py
"""
import csv
import os
import shutil

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
CSV = os.path.join(REPO, "docs/reference/kanto_anime.csv")
MD = os.path.join(REPO, "docs/reference/KANTO_ANIME.md")
WORKSPACE = "/mnt/e/Workspace/romhacks/research-red/reference"


def main():
    rows = list(csv.DictReader(open(CSV, encoding="utf-8")))
    rows.sort(key=lambda r: (int(r["Order"]), r["Place"]))
    out = ["# Kanto in the anime: places, characters, set pieces",
           "",
           "_Generated from `kanto_anime.csv` by `tools/research_red/build_reference.py`. "
           "Edit the CSV (it opens in Excel), then rerun._",
           "",
           "Ideas for dialogue, side quests, ambient Pokémon (#64) and homages. Nothing here is "
           "decided; Status says whether an idea is used. Summarised from Bulbapedia "
           "(location pages, read 2026-10-20). Episode titles are the English dub's. "
           "Checked: wiki = from the wiki pages; memory = details from Claude's memory, check before using.",
           ""]
    place = None
    for r in rows:
        if r["Place"] != place:
            place = r["Place"]
            out += ["", f"## {place}", "",
                    "| Source | Episode | What happens | Characters | Pokémon | Idea | Status | Checked |",
                    "|---|---|---|---|---|---|---|---|"]
        cells = [r[k].replace("|", "/") for k in
                 ("Source", "Episode", "What happens", "Characters", "Pokémon", "Idea for Research Red", "Status", "Checked")]
        out.append("| " + " | ".join(cells) + " |")
    open(MD, "w", encoding="utf-8").write("\n".join(out) + "\n")
    if os.path.isdir(os.path.dirname(WORKSPACE)):
        os.makedirs(WORKSPACE, exist_ok=True)
        with open(os.path.join(WORKSPACE, "kanto_anime.csv"), "w", encoding="utf-8-sig") as f:
            f.write(open(CSV, encoding="utf-8").read())
        shutil.copy(MD, WORKSPACE)
    print(f"{len(rows)} rows -> {os.path.relpath(MD, REPO)}")


if __name__ == "__main__":
    main()
