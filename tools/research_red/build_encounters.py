#!/usr/bin/env python3
"""Research Red: wild Pokemon tables from easy-to-edit text files.

Each file in data/research_red/encounters/ describes one area:

    map: MAP_ROUTE1
    [grass]                 how often: rate 21 (FireRed's usual), optional
    Pidgey      2-4   20    species, levels, percent (each section adds to 100)
    ...
    [grass night]           optional; used at night (the plain table otherwise)

Sections: grass, surf, rock smash, old rod, good rod, super rod; add " night"
for a night-only version. The game picks from fixed slots (grass has 12 slots
worth 20/20/10/10/10/10/5/5/4/4/1/1 percent), so the tool finds the slots that
add up to each percent, and says so if a percent can't be made exactly.

Writes src/data/wild_encounters.json (both FireRed and LeafGreen tables of the
map; the night tables get a "_Night" label). The in-game Field Guide (Start
menu) reads the same data, so it always shows the real rates.

Usage: python3 tools/research_red/build_encounters.py [--check]
"""
import glob
import json
import os
import re
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC_DIR = os.path.join(REPO, "data/research_red/encounters")
JSON = os.path.join(REPO, "src/data/wild_encounters.json")
SPECIES_H = os.path.join(REPO, "include/constants/species.h")

SLOTS = {
    "grass": ("land_mons", [20, 20, 10, 10, 10, 10, 5, 5, 4, 4, 1, 1], 21),
    "surf": ("water_mons", [60, 30, 5, 4, 1], 2),
    "rock smash": ("rock_smash_mons", [60, 30, 5, 4, 1], 50),
    "old rod": ("fishing_mons", [70, 30], 10),
    "good rod": ("fishing_mons", [60, 20, 20], 10),
    "super rod": ("fishing_mons", [40, 40, 15, 4, 1], 10),
}
ROD_ORDER = ["old rod", "good rod", "super rod"]


def species_constant(name, known):
    c = "SPECIES_" + re.sub(r"[^A-Z0-9]+", "_", name.upper().replace("♂", "_M").replace("♀", "_F")
                              .replace("'", "").replace(".", "")).strip("_")
    if c not in known:
        sys.exit(f"unknown Pokemon '{name}' (looked for {c})")
    return c


def assign(rates, wanted):
    """Split the slots between the species so each gets its percent. Returns a
    species index per slot, exact if possible, otherwise closest."""
    n = len(rates)
    order = sorted(range(len(wanted)), key=lambda i: -wanted[i])
    best = [None, None]

    def dfs(slot, left, picks):
        if slot == n:
            err = sum(abs(x) for x in left)
            if best[0] is None or err < best[0]:
                best[0], best[1] = err, picks[:]
            return best[0] == 0
        for i in order:
            if left[i] >= rates[slot] or (best[0] is None and i == order[-1]):
                left[i] -= rates[slot]
                picks.append(i)
                if dfs(slot + 1, left, picks):
                    return True
                picks.pop()
                left[i] += rates[slot]
        return False

    if not dfs(0, list(wanted), []):
        # no exact split: give each slot to whoever is furthest below their percent
        left = list(wanted)
        picks = []
        for r in rates:
            i = max(range(len(left)), key=lambda k: left[k])
            left[i] -= r
            picks.append(i)
        best[1] = picks
    return best[1]


def parse(path, known):
    area = {"map": None, "sections": {}}
    current = None
    for n, raw in enumerate(open(path, encoding="utf-8"), 1):
        line = raw.split("#")[0].strip()
        if not line:
            continue
        where = f"{os.path.basename(path)}:{n}"
        if line.lower().startswith("map:"):
            area["map"] = line.split(":", 1)[1].strip()
        elif line.startswith("["):
            current = line.strip("[]").strip().lower()
            base = current.replace(" night", "")
            if base not in SLOTS:
                sys.exit(f"{where}: unknown section [{current}]")
            area["sections"][current] = {"rate": SLOTS[base][2], "mons": []}
        elif line.lower().startswith("rate"):
            area["sections"][current]["rate"] = int(line.split()[1])
        else:
            m = re.match(r"(.+?)\s+(\d+)(?:-(\d+))?\s+(\d+)%?$", line)
            if not m or current is None:
                sys.exit(f"{where}: expected 'Name  minLv-maxLv  percent'")
            lo = int(m.group(2))
            hi = int(m.group(3) or lo)
            area["sections"][current]["mons"].append((species_constant(m.group(1).strip(), known), lo, hi, int(m.group(4))))
    if not area["map"]:
        sys.exit(f"{path}: missing 'map: MAP_...'")
    return area


def build_table(area, night):
    """Returns {json type: {"encounter_rate", "mons"}} for one time of day."""
    out = {}
    report = []
    for section, (jtype, rates, _) in SLOTS.items():
        key = section + (" night" if night else "")
        data = area["sections"].get(key)
        if data is None and night:
            data = area["sections"].get(section)   # night falls back to the day list
        if data is None:
            continue
        mons = data["mons"]
        total = sum(m[3] for m in mons)
        if total != 100:
            sys.exit(f"{area['map']} [{key}]: percents add up to {total}, not 100")
        picks = assign(rates, [m[3] for m in mons])
        slots = [{"min_level": mons[i][1], "max_level": mons[i][2], "species": mons[i][0]} for i in picks]
        got = {}
        for r, i in zip(rates, picks):
            got[i] = got.get(i, 0) + r
        for i, m in enumerate(mons):
            if got.get(i, 0) != m[3]:
                report.append(f"  {area['map']} [{key}] {m[0]}: wanted {m[3]}%, got {got.get(i, 0)}%")
        if jtype == "fishing_mons":
            rods = out.setdefault("fishing_mons", {"encounter_rate": data["rate"], "rods": {}})
            rods["rods"][section] = slots
        else:
            out[jtype] = {"encounter_rate": data["rate"], "mons": slots}
    if "fishing_mons" in out:
        rods = out["fishing_mons"]["rods"]
        if set(rods) != set(ROD_ORDER):
            sys.exit(f"{area['map']}: give all three rods ([old rod], [good rod], [super rod]) or none")
        out["fishing_mons"] = {"encounter_rate": out["fishing_mons"]["encounter_rate"],
                               "mons": [s for r in ROD_ORDER for s in rods[r]]}
    return out, report


def main():
    known = set(re.findall(r"\b(SPECIES_\w+)\s*=", open(SPECIES_H).read()))
    data = json.load(open(JSON))
    group = data["wild_encounter_groups"][0]
    reports = []
    for path in sorted(glob.glob(os.path.join(SRC_DIR, "*.txt"))):
        area = parse(path, known)
        has_night = any(k.endswith(" night") for k in area["sections"])
        day, rep = build_table(area, False)
        reports += rep
        night, rep = build_table(area, True) if has_night else (None, [])
        reports += rep
        existing = [e for e in group["encounters"] if e["map"] == area["map"] and not e["base_label"].endswith("_Night")]
        if not existing:
            sys.exit(f"{area['map']}: no table in wild_encounters.json to replace")
        group["encounters"] = [e for e in group["encounters"]
                               if not (e["map"] == area["map"] and e["base_label"].endswith("_Night"))]
        for e in existing:
            for k in [k for k in e if k.endswith("_mons")]:
                del e[k]
            e.update(day)
            if night:
                idx = group["encounters"].index(e)
                group["encounters"].insert(idx + 1, dict({"map": area["map"], "base_label": e["base_label"] + "_Night"}, **night))
    text = json.dumps(data, indent=2, ensure_ascii=False) + "\n"
    if "--check" in sys.argv:
        if open(JSON).read() != text:
            sys.exit("wild_encounters.json is out of date: run tools/research_red/build_encounters.py")
        return
    open(JSON, "w").write(text)
    print("encounter tables written")
    if reports:
        print("Percents that couldn't be made exactly from the slots:")
        print("\n".join(reports))


if __name__ == "__main__":
    main()
