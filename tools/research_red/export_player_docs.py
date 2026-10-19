#!/usr/bin/env python3
"""Research Red: player-facing documentation, generated from the game data.

Writes docs/player/ (and a copy to the workspace's player-docs/ folder for
reading on a phone):
  README.md       what's here
  ENCOUNTERS.md   wild Pokémon per area, day and night, with levels and %
  TRAINERS.md     every trainer's team on the way to Brock
  ITEMS.md        item balls and hidden items per area
and docs/BALANCE.md for development: levels per area, aces, level caps.

Everything comes from the same files the game is built from, so the docs
can't drift: run this after changing encounters, trainers or maps.
Usage: python3 tools/research_red/export_player_docs.py [--check]
"""
import json
import os
import re
import shutil
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(REPO, "docs/player")
BALANCE = os.path.join(REPO, "docs/BALANCE.md")
WORKSPACE_COPY = "/mnt/e/Workspace/romhacks/research-red/player-docs"

# The demo's areas, in story order: (title, map constant, map folder prefixes)
AREAS = [
    ("Pallet Town", "MAP_PALLET_TOWN", ["PalletTown"]),
    ("Route 1", "MAP_ROUTE1", ["Route1"]),
    ("Viridian City", "MAP_VIRIDIAN_CITY", ["ViridianCity"]),
    ("Route 22", "MAP_ROUTE22", ["Route22"]),
    ("Route 2", "MAP_ROUTE2", ["Route2"]),
    ("Viridian Forest", "MAP_VIRIDIAN_FOREST", ["ViridianForest"]),
    ("Pewter City", "MAP_PEWTER_CITY", ["PewterCity"]),
]
SLOT_RATES = {
    "land_mons": [20, 20, 10, 10, 10, 10, 5, 5, 4, 4, 1, 1],
    "water_mons": [60, 30, 5, 4, 1],
    "rock_smash_mons": [60, 30, 5, 4, 1],
    "fishing_mons": [70, 30, 60, 20, 20, 40, 40, 15, 4, 1],
}
SECTIONS = [("land_mons", "Grass", None), ("water_mons", "Surfing", None), ("rock_smash_mons", "Rock Smash", None),
            ("fishing_mons", "Old Rod", (0, 2)), ("fishing_mons", "Good Rod", (2, 5)), ("fishing_mons", "Super Rod", (5, 10))]
# Level caps (src/caps.c): the cap before each badge
LEVEL_CAPS = [("Brock", 14), ("Misty", 21), ("Lt. Surge", 24), ("Erika", 29), ("Koga", 43), ("Sabrina", 43),
              ("Blaine", 47), ("Giovanni", 50)]
# Story trainers in the demo that aren't on a map script (scene battles)
STORY_TRAINERS = [
    ("Oak's lab", "TRAINER_RR_ELM_LAB_", "TRAINER_RR_JENNY_LAB_", "Elm & Jenny (2v2, their lab starters)"),
    ("Route 22", "TRAINER_RR_ELM_R22_", "TRAINER_RR_JENNY_R22_", "Elm & Jenny (6v6 double)"),
    ("Pewter City", "TRAINER_RR_ELM_PEWTER_", "TRAINER_RR_JENNY_PEWTER_", "Elm & Jenny after Brock (scene not built yet)"),
]
# The starter chain: your pick -> (Elm's, Jenny's)
# Maps inside these areas that belong to later in the game
EXCLUDE_MAPS = {"ViridianCity_Gym"}
STARTERS = ["BULBASAUR", "CHARMANDER", "SQUIRTLE", "CHIKORITA", "CYNDAQUIL", "TOTODILE"]


def chain(pick):
    r, t = divmod(pick, 3)
    return STARTERS[(1 - r) * 3 + (t + 1) % 3], STARTERS[(1 - r) * 3 + (t + 2) % 3]


def load_names():
    species = {}
    for root, _, files in os.walk(os.path.join(REPO, "src/data/pokemon/species_info")):
        for f in files:
            text = open(os.path.join(root, f), encoding="utf-8").read()
            for m in re.finditer(r"\[(SPECIES_\w+)\]\s*=\s*\{.*?\.speciesName\s*=\s*_\(\"(.*?)\"\)", text, re.S):
                species.setdefault(m.group(1), m.group(2))
    items = {}
    text = open(os.path.join(REPO, "src/data/items.h"), encoding="utf-8").read()
    for m in re.finditer(r"\[(ITEM_\w+)\]\s*=\s*\{\s*\.name\s*=\s*ITEM_NAME\(\"(.*?)\"\)", text, re.S):
        items[m.group(1)] = m.group(2)
    return species, items


def pretty(const, names):
    return names.get(const) or const.split("_", 1)[1].replace("_", " ").title()


def parse_party():
    trainers, cur = {}, None
    for line in open(os.path.join(REPO, "src/data/trainers.party"), encoding="utf-8"):
        line = line.rstrip("\n")
        m = re.match(r"=== (TRAINER_\w+) ===", line)
        if m:
            cur = {"id": m.group(1), "fields": {}, "mons": []}
            trainers[cur["id"]] = cur
            continue
        if cur is None or not line.strip() or line.startswith("/*"):
            continue
        if not cur["mons"] and re.match(r"^[A-Z][\w ]*: ", line) and not line.startswith("Level:"):
            k, v = line.split(": ", 1)
            cur["fields"][k] = v
        elif line.startswith("Level: "):
            cur["mons"][-1]["level"] = int(line.split(": ")[1].split()[0])
        elif line.startswith("- "):
            cur["mons"][-1]["moves"].append(line[2:])
        elif re.match(r"^\w+: ", line) and cur["mons"]:
            pass  # IVs, abilities etc.
        else:
            m = re.match(r"^(?:(.+?) \((.+?)\)|(.+?))(?: @ (.+))?$", line)
            nick, sp = (m.group(1), m.group(2)) if m.group(2) else (None, m.group(3))
            cur["mons"].append({"species": sp.strip(), "nick": nick, "level": None, "moves": [], "item": m.group(4)})
    return trainers


def map_folders(prefixes):
    folders = []
    for d in sorted(os.listdir(os.path.join(REPO, "data/maps"))):
        for p in prefixes:
            if (d == p or d.startswith(p + "_")) and d not in EXCLUDE_MAPS:
                folders.append(d)
    return folders


def area_trainers(prefixes):
    found = []
    for d in map_folders(prefixes):
        path = os.path.join(REPO, "data/maps", d, "scripts.inc")
        if os.path.exists(path):
            for t in re.findall(r"trainerbattle\w*\s+(TRAINER_\w+)", open(path, encoding="utf-8").read()):
                if t not in found and not t.startswith("TRAINER_RIVAL"):
                    found.append(t)
    return found


def area_items(prefixes, item_names):
    scripts = {}
    for path in [os.path.join(REPO, "data/scripts/item_ball_scripts.inc")]:
        for m in re.finditer(r"^(\w+)::\s*\n\s*finditem (ITEM_\w+)", open(path, encoding="utf-8").read(), re.M):
            scripts[m.group(1)] = m.group(2)
    rows = []
    for d in map_folders(prefixes):
        m = json.load(open(os.path.join(REPO, "data/maps", d, "map.json"), encoding="utf-8"))
        place = re.sub(r"(?<!^)(?=[A-Z0-9])", " ", d.replace("_", " · ")).replace("  ", " ")
        for o in m.get("object_events", []):
            if o.get("graphics_id") == "OBJ_EVENT_GFX_ITEM_BALL" and o.get("script") in scripts:
                rows.append((place, pretty(scripts[o["script"]], item_names), f"({o['x']}, {o['y']})", "Poké Ball on the ground"))
        for b in m.get("bg_events", []):
            if b.get("type") == "hidden_item":
                rows.append((place, pretty(b["item"], item_names), f"({b['x']}, {b['y']})", "hidden (use the Itemfinder)"))
    return rows


def encounter_tables(map_const, species):
    data = json.load(open(os.path.join(REPO, "src/data/wild_encounters.json"), encoding="utf-8"))
    tables = {}
    for e in data["wild_encounter_groups"][0]["encounters"]:
        if e["map"] != map_const or "LeafGreen" in e["base_label"]:
            continue
        tables["night" if e["base_label"].endswith("_Night") else "day"] = e
    return tables


def rows_for(table, key, span):
    if key not in table:
        return []
    mons = table[key]["mons"]
    rates = SLOT_RATES[key]
    lo, hi = span or (0, len(mons))
    merged = {}
    for i in range(lo, hi):
        m = mons[i]
        r = merged.setdefault(m["species"], [0, m["min_level"], m["max_level"]])
        r[0] += rates[i]
        r[1] = min(r[1], m["min_level"])
        r[2] = max(r[2], m["max_level"])
    return sorted(merged.items(), key=lambda kv: -kv[1][0])


def write(path, text, outputs):
    outputs[path] = text


def build():
    species, items = load_names()
    trainers = parse_party()
    outputs = {}
    note = "_Generated by `tools/research_red/export_player_docs.py` from the game's data. Don't edit by hand._\n\n"

    # ENCOUNTERS
    lines = ["# Wild Pokémon", "", note.strip(), "",
             "Chances are per encounter. \"Day\" covers morning, day and evening; \"Night\" is the game's night "
             "(the same clock that tints the screen). In game: Start > Guide.", ""]
    balance_rows = []
    for title, const, prefixes in AREAS:
        tables = encounter_tables(const, species)
        if not tables:
            balance_rows.append((title, None))
            continue
        lines += [f"## {title}", ""]
        wild_levels = []
        for key, label, span in SECTIONS:
            day = rows_for(tables.get("day", {}), key, span)
            night = rows_for(tables.get("night", {}), key, span) if "night" in tables else []
            if not day:
                continue
            for tlabel, rows in (("Day", day), ("Night", night)) if night and night != day else (("Any time", day),):
                lines += [f"**{label} ({tlabel})**", "", "| Pokémon | Levels | Chance |", "|---|---|---|"]
                for sp, (pct, lo, hi) in rows:
                    lines.append(f"| {pretty(sp, species)} | {lo}-{hi} | {pct}% |" if lo != hi else f"| {pretty(sp, species)} | {lo} | {pct}% |")
                    wild_levels += [lo, hi]
                lines.append("")
        balance_rows.append((title, (min(wild_levels), max(wild_levels)) if wild_levels else None))
    write(os.path.join(OUT, "ENCOUNTERS.md"), "\n".join(lines), outputs)

    # TRAINERS
    def team_table(tid):
        t = trainers.get(tid)
        if not t:
            return [f"_({tid} not found)_"], []
        out = ["| Pokémon | Level | Moves |", "|---|---|---|"]
        lv = []
        for m in t["mons"]:
            name = pretty("SPECIES_" + re.sub(r"\W+", "_", m["species"].upper()).strip("_"), species)
            if m["nick"]:
                name = f"{m['nick']} ({name})"
            out.append(f"| {name} | {m['level']} | {', '.join(m['moves']) or 'level-up moves'} |")
            lv.append(m["level"] or 0)
        return out, lv

    lines = ["# Trainers on the way to Brock", "", note.strip(), "",
             "Every trainer battle is a double (you and Ivy). Change that in Start > Rules after the Viridian lunch.", ""]
    trainer_levels = {}
    for title, _, prefixes in AREAS:
        ids = area_trainers(prefixes)
        story = [s for s in STORY_TRAINERS if s[0] == title or (title == "Pallet Town" and s[0] == "Oak's lab")]
        if not ids and not story:
            continue
        lines += [f"## {title}", ""]
        for where, elm_prefix, jenny_prefix, desc in story:
            lines += [f"### {desc}", "", "Their starters depend on yours:", "",
                      "| Your starter | Elm | Jenny |", "|---|---|---|"]
            for i, st in enumerate(STARTERS):
                e, j = chain(i)
                lines.append(f"| {pretty('SPECIES_' + st, species)} | {pretty('SPECIES_' + e, species)} | {pretty('SPECIES_' + j, species)} |")
            lines.append("")
            for who, prefix, first in (("Elm", elm_prefix, "CYNDAQUIL"), ("Jenny", jenny_prefix, "TOTODILE")):
                tbl, lv = team_table(prefix + first)
                lines += [f"**{who}** (shown with {pretty('SPECIES_' + first, species)}; the starter slot changes with yours)", ""] + tbl + [""]
                trainer_levels.setdefault(title, []).extend(lv)
        for tid in ids:
            t = trainers.get(tid)
            if not t:
                continue
            tbl, lv = team_table(tid)
            lines += [f"### {t['fields'].get('Class', '')} {t['fields'].get('Name', '')}".rstrip(), ""] + tbl + [""]
            trainer_levels.setdefault(title, []).extend(lv)
    write(os.path.join(OUT, "TRAINERS.md"), "\n".join(lines), outputs)

    # ITEMS
    lines = ["# Items on the ground", "", note.strip(), "",
             "Item balls and hidden items (coordinates are map tiles, x then y). Gifts from people aren't listed yet.", ""]
    for title, _, prefixes in AREAS:
        rows = area_items(prefixes, items)
        if not rows:
            continue
        lines += [f"## {title}", "", "| Where | Item | Tile | How |", "|---|---|---|---|"]
        lines += [f"| {p} | {i} | {xy} | {how} |" for p, i, xy, how in rows]
        lines.append("")
    write(os.path.join(OUT, "ITEMS.md"), "\n".join(lines), outputs)

    write(os.path.join(OUT, "README.md"), "\n".join([
        "# Research Red player docs", "", note.strip(), "",
        "- [Wild Pokémon](ENCOUNTERS.md): every area, day and night, with levels and chances",
        "- [Trainers](TRAINERS.md): every team on the way to Brock, including Elm and Jenny for each starter",
        "- [Items](ITEMS.md): item balls and hidden items",
        "", "Covers Pallet Town to Pewter City (the demo).", ""]), outputs)

    # BALANCE (development)
    lines = ["# Balance overview", "", note.strip(), "",
             "Levels you'll meet in each area, in story order, next to the level cap. Use it to spot spikes and dips.", "",
             "| Area | Wild levels | Trainer levels (lowest-highest) | Strongest trainer Pokémon |", "|---|---|---|---|"]
    for title, wl in balance_rows:
        tl = trainer_levels.get(title, [])
        lines.append(f"| {title} | {f'{wl[0]}-{wl[1]}' if wl else '-'} | {f'{min(tl)}-{max(tl)}' if tl else '-'} | {max(tl) if tl else '-'} |")
    lines += ["", "## Level caps (src/caps.c)", "", "| Before | Cap |", "|---|---|"]
    lines += [f"| {who} | {cap} |" for who, cap in LEVEL_CAPS]
    lines += ["", "Caps are soft by default (little EXP over the cap), hard or off in Start > Rules.",
              "", "## Notes", "",
              "- The debug skips (Start > Debug > Scripts) raise the team to Lv 12 for Route 22 and the lunch.",
              "- Elm and Jenny's starters are in the starter slot of their teams; see TRAINERS.md for the chain.", ""]
    outputs[BALANCE] = "\n".join(lines)
    return outputs


def main():
    outputs = build()
    if "--check" in sys.argv:
        stale = [p for p, t in outputs.items() if not os.path.exists(p) or open(p, encoding="utf-8").read() != t]
        if stale:
            sys.exit("player docs are out of date: run tools/research_red/export_player_docs.py")
        return
    os.makedirs(OUT, exist_ok=True)
    for p, t in outputs.items():
        open(p, "w", encoding="utf-8").write(t)
    if os.path.isdir(os.path.dirname(WORKSPACE_COPY)):
        shutil.rmtree(WORKSPACE_COPY, ignore_errors=True)
        shutil.copytree(OUT, WORKSPACE_COPY)
        shutil.copy(BALANCE, WORKSPACE_COPY)
    print(f"wrote {len(outputs)} files to docs/player/ and docs/BALANCE.md")


if __name__ == "__main__":
    main()
