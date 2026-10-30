#!/usr/bin/env python3
"""Research Red: the proper-case sweep (#11).

FireRed writes names, menus and many lines in ALL CAPS ("POKéMON", "SAVE",
"PALLET TOWN", "YOUNGSTER"). This rewrites ALL-CAPS words in the game's text
to proper case ("Pokémon", "Save", "Pallet Town", "Youngster"):

  - only inside text strings: _("..."), COMPOUND_STRING("..."), .string "..."
    in data/ and src/, the trainers' Name: lines, map and item names
  - never inside {CODES} (colours, {PLAYER}, {PAUSE 15} ...)
  - real abbreviations stay (PC, HP, PP, TM, HM, OK, TV, ID ...), and a few
    words have their own spelling (POKéMON -> Pokémon, POKéNAV -> PokéNav)

  python3 tools/research_red/proper_case.py            # rewrite the files
  python3 tools/research_red/proper_case.py --check    # count what's left, change nothing

Safe to run again: proper-case text is left alone.
"""
import glob
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

KEEP = {"PC", "HP", "PP", "TM", "HM", "TMS", "HMS", "OK", "TV", "ID", "KO", "EV", "IV", "EVS", "IVS",
        "VS", "NPC", "UFO", "DNA", "CD", "SS", "PKMN", "PK", "MN", "AM", "PM", "IQ", "UV", "EX", "GX",
        "TNT", "ABC", "XYZ", "RNG", "AI", "CPU", "LCD", "LED", "USA", "UK", "OS", "MC", "DJ", "OT"}
SPECIAL = {
    "POKéMON": "Pokémon", "POKéDEX": "Pokédex", "POKé": "Poké", "POKéNAV": "PokéNav",
    "POKéBLOCK": "Pokéblock", "POKéBLOCKS": "Pokéblocks", "POKéFLUTE": "Poké Flute",
    "TMS": "TMs", "HMS": "HMs", "EVS": "EVs", "IVS": "IVs",
}
# Words never to touch: placeholders spelled out as text in some tables
SKIP_STRINGS = re.compile(r"^[A-Z0-9]*_[A-Z0-9_]*$")   # e.g. "SPECIES_X" style ids used as strings

WORD = re.compile(r"[A-Za-zé]+(?:['’][A-Za-zé]+)?")
STRING = re.compile(r'(_\(|COMPOUND_STRING\(|ITEM_NAME\(|\.string\s+)"((?:[^"\\]|\\.)*)"')
PARTY_NAME = re.compile(r"^(Name: )(.+)$", re.M)


def is_caps(word):
    word = re.split(r"['’]", word)[0]   # "OAK's" counts as caps: look before the apostrophe
    letters = [c for c in word if c.isalpha() and c != "é"]
    return len(letters) >= 2 and all(c.isupper() for c in letters)


def fix_word(word):
    if not is_caps(word):
        return word
    base, _, tail = word.partition("'") if "'" in word else word.partition("’")
    sep = "'" if "'" in word else ("’" if "’" in word else "")
    if base in SPECIAL:
        out = SPECIAL[base]
    elif base in KEEP:
        out = base
    else:
        out = base[0] + base[1:].lower()
    if sep:
        out += sep + tail.lower()
    return out


def fix_text(text):
    """Proper-case the words of one string, leaving {CODES} alone."""
    if SKIP_STRINGS.match(text):
        return text
    parts = re.split(r"(\{[^}]*\}|\\.)", text)
    for i in range(0, len(parts), 2):
        parts[i] = WORD.sub(lambda m: fix_word(m.group(0)), parts[i])
    return "".join(parts)


def files():
    pats = ["data/maps/*/scripts.inc", "data/maps/*/text.inc", "data/text/*.inc", "data/scripts/*.inc",
            "src/*.c", "src/data/*.h", "src/data/text/*.h", "src/data/*/*.h"]
    out = set()
    for p in pats:
        out.update(glob.glob(os.path.join(REPO, p)))
    # our own scenes are written in proper case already; the debug menu is ours too
    return sorted(f for f in out if "/research_red/" not in f and not os.path.basename(f).startswith("rr_"))


def sweep(check):
    changed_files, changed_words = 0, 0
    for f in files() + [os.path.join(REPO, "src/data/trainers.party"),
                        os.path.join(REPO, "src/data/region_map/region_map_sections.json")]:
        s = open(f, encoding="utf-8").read()
        new = STRING.sub(lambda m: m.group(1) + '"' + fix_text(m.group(2)) + '"', s)
        if f.endswith(".party"):
            new = PARTY_NAME.sub(lambda m: m.group(1) + fix_text(m.group(2)), new)
        if f.endswith(".json"):
            new = re.sub(r'("name": ")([^"]*)(")', lambda m: m.group(1) + fix_text(m.group(2)) + m.group(3), new)
        if new != s:
            n = sum(1 for a, b in zip(WORD.findall(s), WORD.findall(new)) if a != b)
            changed_files += 1
            changed_words += n
            if not check:
                open(f, "w", encoding="utf-8").write(new)
    verb = "would change" if check else "changed"
    if not check or changed_words:
        print(f"{verb} {changed_words} words in {changed_files} files"
              + (": ALL-CAPS text is back, run tools/research_red/proper_case.py (#11)" if check else ""))
    return changed_words


if __name__ == "__main__":
    sys.exit(1 if sweep("--check" in sys.argv) and "--check" in sys.argv else 0)
