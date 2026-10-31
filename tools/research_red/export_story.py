#!/usr/bin/env python3
"""Research Red: export story data to readable Markdown for writing on the go.

Writes docs/story/:
  README.md                       index
  scenes/<scene>.md               each Research Red cutscene as a script (lines in order)
  <Area>/<Map>.md                 per map: people (with all their lines), trainers and
                                  teams, signs, scene triggers, wild Pokemon, exits
  trainers.md                     every Research Red trainer and team
  FULL_SCRIPT.md                  every Research Red line in story order (intro call,
                                  scenes, battle lines, Pokédex banter) for proofreading

Usage: python3 tools/research_red/export_story.py [--sync]
  --sync  also copy docs/story to E:\\Workspace\\romhacks\\research-red\\story\\
Regenerate after any story change; the files are generated, edit copies and
send them back rather than editing these in place.
"""
import glob
import json
import os
import re
import shutil
import sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(REPO, "docs/story")
SYNC = "/mnt/e/Workspace/romhacks/research-red/story"

# Demo region (through Brock), in story order
AREAS = [
    ("Pallet Town", "PalletTown"),
    ("Route 1", "Route1"),
    ("Viridian City", "ViridianCity"),
    ("Route 22", "Route22"),
    ("Route 2", "Route2"),
    ("Viridian Forest", "ViridianForest"),
    ("Pewter City", "PewterCity"),
]
JUMPS = re.compile(r"^\s*(?:goto|call|goto_if_\w+|call_if_\w+|case)\b.*?,?\s*([A-Za-z_][A-Za-z0-9_]*)\s*$")
TEXTCMD = re.compile(r"^\s*(?:msgbox|message|trainerbattle_\w+|textcolor)\b(.*)$")


def nice(sym, prefix):
    s = sym[len(prefix):] if sym.startswith(prefix) else sym
    if s.isupper() or "_" in s and s.replace("_", "").isupper():
        return s.replace("_", " ").title()
    # CamelCase map folder names: PalletTown_PlayersHouse_1F -> Pallet Town - Players House 1F
    s = re.sub(r"(?<=[a-z])(?=[A-Z0-9])", " ", s)
    return s.replace("_", " - ")


def load_texts():
    """label -> text with \\n/\\l as newlines and \\p as a new box."""
    texts = {}
    files = glob.glob(os.path.join(REPO, "data/maps/*/*.inc")) + glob.glob(os.path.join(REPO, "data/scripts/**/*.inc"), recursive=True) \
        + glob.glob(os.path.join(REPO, "data/text/*.inc"))
    for f in files:
        label = None
        for line in open(f, errors="replace"):
            m = re.match(r"^([A-Za-z0-9_]+)::?\s*$", line.strip())
            if m:
                label = m.group(1)
                continue
            m = re.match(r'^\s*\.string\s+"(.*)"\s*$', line)
            if m and label:
                texts[label] = texts.get(label, "") + m.group(1)
            elif line.strip() and not line.strip().startswith("@"):
                label = None
    for k, v in texts.items():
        texts[k] = v.replace("\\p", "\n\n").replace("\\n", "\n").replace("\\l", "\n").replace("$", "").strip()
    return texts


def load_scripts():
    """label -> list of lines (until the next label)."""
    scripts = {}
    files = glob.glob(os.path.join(REPO, "data/maps/*/scripts.inc")) + glob.glob(os.path.join(REPO, "data/scripts/**/*.inc"), recursive=True)
    for f in files:
        cur = None
        for line in open(f, errors="replace"):
            m = re.match(r"^([A-Za-z0-9_]+)::?\s*$", line.strip())
            if m:
                cur = m.group(1)
                scripts[cur] = []
            elif cur:
                scripts[cur].append(line.rstrip("\n"))
    return scripts


def load_trainers():
    """TRAINER_X -> dict(name, cls, double, mons=[(species, level, moves)])."""
    trainers, cur = {}, None
    for line in open(os.path.join(REPO, "src/data/trainers.party")):
        line = line.rstrip()
        m = re.match(r"^=== (TRAINER_\w+) ===", line)
        if m:
            cur = {"id": m.group(1), "name": "", "cls": "", "double": False, "mons": []}
            trainers[m.group(1)] = cur
            continue
        if cur is None:
            continue
        if line.startswith("Name:"):
            cur["name"] = line[5:].strip()
        elif line.startswith("Class:"):
            cur["cls"] = line[6:].strip()
        elif line.startswith("Double Battle:"):
            cur["double"] = "Yes" in line
        elif line.startswith("Level:") and cur["mons"]:
            cur["mons"][-1][1] = line[6:].strip()
        elif line.startswith("- ") and cur["mons"]:
            cur["mons"][-1][2].append(line[2:].strip())
        elif line and not re.match(r"^(Pic|Gender|Music|AI|IVs|EVs|Ability|Nature|Items|Mugshot|Starting Status|Party Size|Pool Rules|Copy Pool|Tera Type|Ball|Shiny|Happiness|Dynamax|Gigantamax|Tags|/\*)", line) and ":" not in line:
            species = line.split("@")[0].split("(")[0].strip()
            cur["mons"].append([species, "?", []])
    return trainers


TEXTS = {}


def walk(label, scripts, seen, depth=0):
    """Text labels and trainer ids reached from a script label, in order."""
    if label in seen or label not in scripts or depth > 12:
        return []
    seen.add(label)
    found = []
    for line in scripts[label]:
        s = line.strip()
        for tid in re.findall(r"\bTRAINER_\w+", s):
            if not tid.startswith("TRAINER_BATTLE") and not tid.startswith("TRAINER_TYPE"):
                found.append(("trainer", tid))
        for word in re.findall(r"\b[A-Za-z_][A-Za-z0-9_]*\b", s):
            if word in TEXTS:
                found.append(("text", word))
            elif word in scripts and re.match(r"^\s*(goto|call|case|goto_if|call_if|trainerbattle)", s):
                found += walk(word, scripts, seen, depth + 1)
        if re.match(r"^\s*(end|return)\s*$", s):
            break
    return found


def fmt_text(t):
    return "\n".join("> " + l if l else ">" for l in t.split("\n"))


def wild_tables():
    w = json.load(open(os.path.join(REPO, "src/data/wild_encounters.json")))
    group = w["wild_encounter_groups"][0]
    rates = {f["type"]: f["encounter_rates"] for f in group["fields"]}
    tables = {}
    for e in group["encounters"]:
        if e.get("base_label", "").endswith("_LeafGreen"):
            continue
        tables[e["map"]] = (e, rates)
    return tables


def map_doc(mapdir, texts, scripts, trainers, wild):
    m = json.load(open(os.path.join(REPO, "data/maps", mapdir, "map.json")))
    out = [f"# {nice(mapdir, '')}", "", f"Map `{m['id']}` · music `{m.get('music','')}` · weather `{m.get('weather','')}`", ""]

    scene_hooks = [c for c in m.get("coord_events", []) if c.get("script", "").startswith("RR_")]
    hooks = sorted({(c["script"], c.get("var", ""), c.get("var_value", "")) for c in scene_hooks})
    if hooks:
        out += ["## Story scene triggers", ""]
        for script, var, val in hooks:
            out.append(f"- `{script}` when `{var}` = {val} (see scenes/)")
        out.append("")

    people = m.get("object_events", [])
    if people:
        out += ["## People and objects", ""]
        for o in people:
            gfx = o.get("graphics_id", "").replace("OBJ_EVENT_GFX_", "").replace("_", " ").title()
            who = nice(str(o.get("local_id") or ""), "LOCALID_") or gfx
            line = f"### {who} ({gfx}) at {o['x']},{o['y']}"
            out.append(line)
            meta = []
            if o.get("trainer_type") not in (None, "TRAINER_TYPE_NONE", "0"):
                meta.append(f"trainer, sight {o.get('trainer_sight_or_berry_tree_id')}")
            if o.get("flag") not in (None, "0"):
                meta.append(f"hidden by `{o['flag']}`")
            if meta:
                out.append("*" + "; ".join(meta) + "*")
            script = o.get("script", "0x0")
            if script in ("0x0", "0", ""):
                out += ["(no talk script; used in cutscenes)", ""]
                continue
            out.append(f"Script `{script}`")
            out.append("")
            for kind, val in walk(script, scripts, set()):
                if kind == "text" and val in texts:
                    out += [fmt_text(texts[val]), ""]
                elif kind == "trainer" and val in trainers:
                    t = trainers[val]
                    team = ", ".join(f"{s} Lv{l}" for s, l, _ in t["mons"])
                    out += [f"**Battle:** {t['cls']} {t['name']} ({'double' if t['double'] else 'single'}): {team}", ""]
    signs = [b for b in m.get("bg_events", []) if b.get("type") == "sign"]
    if signs:
        out += ["## Signs", ""]
        for b in signs:
            ts = [texts[v] for k, v in walk(b["script"], scripts, set()) if k == "text" and v in texts]
            out += [f"- at {b['x']},{b['y']}: " + (" / ".join(t.replace(chr(10), ' ') for t in ts) or f"`{b['script']}`")]
        out.append("")
    if m["id"] in wild:
        e, rates = wild[m["id"]]
        out += ["## Wild Pokemon", ""]
        for field, title in (("land_mons", "Grass"), ("water_mons", "Surfing"), ("fishing_mons", "Fishing"), ("rock_smash_mons", "Rock Smash")):
            if field in e:
                tot = {}
                for mon, rate in zip(e[field]["mons"], rates.get(field, [])):
                    sp = mon["species"].replace("SPECIES_", "").title()
                    lv = f"{mon['min_level']}-{mon['max_level']}" if mon["min_level"] != mon["max_level"] else str(mon["min_level"])
                    key = sp
                    tot.setdefault(key, [0, set()])
                    tot[key][0] += rate
                    tot[key][1].add(lv)
                out.append(f"**{title}:** " + ", ".join(f"{k} {v[0]}% (Lv {'/'.join(sorted(v[1]))})" for k, v in sorted(tot.items(), key=lambda x: -x[1][0])))
        out.append("")
    exits = [w["dest_map"] for w in m.get("warp_events", [])] + [c["map"] for c in m.get("connections") or []]
    if exits:
        out += ["## Exits", "", ", ".join(sorted({nice(x, 'MAP_') for x in exits})), ""]
    return "\n".join(out)


def scene_doc(path, texts, trainers):
    name = os.path.basename(path)[:-4]
    head = [l[2:].strip() for l in open(path) if l.startswith("@ Research Red:")][:1]
    out = [f"# Scene: {name}", "", head[0] if head else "", ""]
    for line in open(path):
        s = line.strip()
        m = re.match(r"^([A-Za-z0-9_]+)::\s*$", s)
        if m and "_Text_" not in m.group(1) and "Movement" not in m.group(1):
            out += [f"### {m.group(1)}", ""]
            continue
        m = re.match(r"^(msgbox|message)\s+([A-Za-z0-9_]+)", s)
        if m and m.group(2) in texts:
            out += [fmt_text(texts[m.group(2)]), ""]
            continue
        m = re.match(r"^(trainerbattle_\w+|givemon|additem|setfollowernpc|rr_ivy_\w+|warp|setvar VAR_0x40FE|dynmultichoice)\b(.*)", s)
        if m:
            extra = m.group(2).split(",")[0].strip()
            if m.group(1).startswith("trainerbattle"):
                ids = re.findall(r"TRAINER_\w+", s)
                extra = " + ".join(f"{trainers[i]['cls']} {trainers[i]['name']} ({', '.join(x[0] for x in trainers[i]['mons'])})" for i in ids if i in trainers)
            out += [f"*[{m.group(1)} {extra}]*", ""]
    return "\n".join(out)


STORY_ORDER = ["rr_breakfast", "rr_grass_hook", "rr_lab", "rr_daisy", "rr_route1", "rr_route22", "rr_lunch", "rr_forest", "rr_pewter", "rr_brock_watch", "rr_ivy"]


def full_script(texts, trainers):
    """One document with every Research Red line, in story order."""
    def clean(t):
        return t.replace("\\n", " ").replace("\\l", " ").replace("\\p", "\n\n")
    out = ["# Research Red: the full script", "",
           "Every line of Research Red's own dialogue in story order, for reading straight through.",
           "Generated by `tools/research_red/export_story.py`; edit the game files (or send notes), not this.", ""]
    src = open(os.path.join(REPO, "src/rr_intro_call.c"), encoding="utf-8").read()
    out += ["## 00 · The intro call", ""]
    out += [clean(m).strip() + "\n" for m in re.findall(r'STEP_LINE, 0, \(const u8\[\]\)_\("(.*?)"\)\}', src)]
    for i, name in enumerate(STORY_ORDER, 1):
        path = os.path.join(REPO, "data/scripts/research_red", name + ".inc")
        if os.path.exists(path):
            doc = scene_doc(path, texts, trainers).split("\n", 1)[1]
            out += [f"## {i:02d} · {name}", doc, ""]
    slides = os.path.join(REPO, "src/data/research_red/trainer_slides.h")
    out += ["## Battle lines (mid-battle trainer slides)", "",
            "Elm and Jenny have one version per starter; their lines are the same in each, so they're listed once.", ""]
    families, cur = {}, None
    for line in open(slides, encoding="utf-8"):
        m = re.match(r"\s*\[(TRAINER_(?!SLIDE_)\w+)\] =", line)
        if m:
            cur = re.sub(r"_(BULBASAUR|CHARMANDER|SQUIRTLE|CHIKORITA|CYNDAQUIL|TOTODILE)$", "", m.group(1))
            families.setdefault(cur, [])
        m = re.search(r"\[TRAINER_SLIDE_(\w+)\] = COMPOUND_STRING\(\"(.*?)\{PAUSE_UNTIL_PRESS\}\"\)", line)
        if m and cur:
            entry = f"- **{m.group(1).replace('RR_', '').replace('SWITCHIN', 'Pokémon out').replace('_', ' ').lower()}**: " + clean(m.group(2)).replace("\n\n", " / ").replace("{B_PLAYER_NAME}", "{PLAYER}")
            if entry not in families[cur]:
                families[cur].append(entry)
    for fam, entries in families.items():
        title = fam.replace("TRAINER_RR_", "").replace("TRAINER_", "").replace("_R22", " (Route 22)").replace("_PEWTER", " (Pewter)")
        out += [f"### {title.replace('_', ' ').title().replace('(Route 22)', '(Route 22)')}"] + entries + [""]
    out += ["", "## Pokédex banter", "", "(Source: `data/research_red/dex_banter.txt`.)", ""]
    for line in open(os.path.join(REPO, "data/research_red/dex_banter.txt"), encoding="utf-8"):
        if line.startswith("#"):
            continue
        out.append(("### " + line.strip()[1:-1]) if line.startswith("[") else line.rstrip())
    return "\n".join(out) + "\n"


def main():
    texts, scripts, trainers = load_texts(), load_scripts(), load_trainers()
    TEXTS.update(texts)
    wild = wild_tables()
    if os.path.isdir(OUT):
        shutil.rmtree(OUT)
    os.makedirs(os.path.join(OUT, "scenes"))
    index = ["# Research Red story data", "",
             "Generated by `tools/research_red/export_story.py`. Edit a copy and send it back;",
             "these files are rebuilt from the game data.", "", "## Scenes", ""]
    for path in sorted(glob.glob(os.path.join(REPO, "data/scripts/research_red/rr_*.inc"))):
        name = os.path.basename(path)[:-4]
        open(os.path.join(OUT, "scenes", name + ".md"), "w").write(scene_doc(path, texts, trainers))
        index.append(f"- [{name}](scenes/{name}.md)")
    index += ["", "## Locations", ""]
    all_maps = sorted(os.listdir(os.path.join(REPO, "data/maps")))
    for area, prefix in AREAS:
        maps = [d for d in all_maps if (d == prefix or d.startswith(prefix + "_")) and os.path.exists(os.path.join(REPO, "data/maps", d, "map.json"))]
        if prefix == "Route2":
            maps = [d for d in maps if not d.startswith("Route22")]
        os.makedirs(os.path.join(OUT, area.replace(" ", "")), exist_ok=True)
        index.append(f"### {area}")
        for d in maps:
            rel = f"{area.replace(' ', '')}/{d}.md"
            open(os.path.join(OUT, rel), "w").write(map_doc(d, texts, scripts, trainers, wild))
            index.append(f"- [{nice(d, '')}]({rel})")
        index.append("")
    rr = [t for k, t in trainers.items() if k.startswith("TRAINER_RR_")]
    tdoc = ["# Research Red trainers", ""]
    for t in rr:
        tdoc.append(f"## {t['cls']} {t['name']} (`{t['id']}`, {'double' if t['double'] else 'single'})")
        tdoc += [f"- {s} Lv{l}" + (f": {', '.join(mv)}" if mv else "") for s, l, mv in t["mons"]] + [""]
    open(os.path.join(OUT, "trainers.md"), "w").write("\n".join(tdoc))
    index += ["## Trainers", "", "- [Research Red trainers](trainers.md)", ""]
    open(os.path.join(OUT, "FULL_SCRIPT.md"), "w", encoding="utf-8").write(full_script(texts, trainers))
    index.insert(5, "**Read it all at once: [FULL_SCRIPT.md](FULL_SCRIPT.md)**\n")
    open(os.path.join(OUT, "README.md"), "w").write("\n".join(index))
    if "--sync" in sys.argv and os.path.isdir(os.path.dirname(SYNC)):
        if os.path.isdir(SYNC):
            shutil.rmtree(SYNC)
        shutil.copytree(OUT, SYNC)
    print("wrote", OUT)


if __name__ == "__main__":
    main()
