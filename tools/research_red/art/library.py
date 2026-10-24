#!/usr/bin/env python3
"""Research Red Art Kit: the sprite library.

Picture sheets of every overworld person, trainer picture and Pokemon in the
game, so you can browse what exists before making something new (a base to
recolour, a head to borrow, a walk cycle to copy). Also writes a catalogue
that says which OBJ_EVENT_GFX_ name uses which picture and how many frames it
has (3 = stands still, 9+ = walks).

  python3 tools/research_red/art/library.py            # everything
  python3 tools/research_red/art/library.py people     # just one part
  parts: people, emerald, objects, trainers, pokemon, catalogue
("emerald" needs pret's pokeemerald people sheets in ~/romhacks/_tools/pokeemerald-ref,
see docs/ART_TOOLS.md; recipes can use them as bases with the "em:" prefix.)

Output: E:\\Workspace\\romhacks\\research-red\\art\\toolkit\\library\\
"""
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import artlib  # noqa: E402

LIB = artlib.out_dir("library") if __name__ == "__main__" else os.path.join(artlib.TOOLKIT_OUT, "library")
G = os.path.join(artlib.REPO, "graphics")


def people():
    """Every 16x32 person: down / up / left, plus how many frames the sheet has."""
    walkers, standers = [], []
    for path in sorted(glob.glob(os.path.join(artlib.PEOPLE, "*.png"))):
        name = os.path.basename(path)[:-4]
        grid = artlib.load_grid(path)
        if len(grid) != 32:
            continue
        frames = artlib.sheet_frames(grid)
        row = artlib.join_frames(frames[:3])
        (walkers if len(frames) >= 9 else standers).append(("%s %d" % (name, len(frames)), row))
    artlib.label_sheet(walkers, cols=8, scale=2).save(os.path.join(LIB, "people_walking.png"))
    artlib.label_sheet(standers, cols=8, scale=2).save(os.path.join(LIB, "people_standing_only.png"))
    print("people: %d walk, %d stand only" % (len(walkers), len(standers)))


def emerald():
    """Emerald's people (reference copy outside the repo): more bases to build from."""
    root = os.path.join(artlib.EMERALD_REF, "graphics/object_events/pics/people")
    if not os.path.isdir(root):
        print("emerald: no reference copy at", root)
        return
    walkers, standers = [], []
    for path in sorted(glob.glob(os.path.join(root, "**/*.png"), recursive=True)):
        grid = artlib.load_grid(path)
        if len(grid) != 32 or len(grid[0]) % 16:
            continue
        name = os.path.relpath(path, root)[:-4]
        frames = artlib.sheet_frames(grid)
        (walkers if len(frames) >= 9 else standers).append(("em:%s %d" % (name, len(frames)), artlib.join_frames(frames[:3])))
    artlib.label_sheet(walkers + standers, cols=8, scale=2).save(os.path.join(LIB, "people_emerald.png"))
    print("emerald: %d walk, %d stand only" % (len(walkers), len(standers)))


def objects():
    """Everything in the overworld that isn't a 16x32 person (items, rocks, boats, Pokemon)."""
    items = []
    for sub in ("misc", "pokemon_old", "berry_trees"):
        for path in sorted(glob.glob(os.path.join(G, "object_events/pics", sub, "*.png"))):
            grid = artlib.load_grid(path)
            h = len(grid)
            items.append((os.path.basename(path)[:-4], artlib.crop(grid, 0, 0, min(len(grid[0]), h if h >= 16 else 16), h)))
    for path in sorted(glob.glob(os.path.join(artlib.PEOPLE, "*.png"))):
        grid = artlib.load_grid(path)
        if len(grid) != 32:
            items.append((os.path.basename(path)[:-4], artlib.crop(grid, 0, 0, min(len(grid[0]), len(grid)), len(grid))))
    artlib.label_sheet(items, cols=10, scale=2).save(os.path.join(LIB, "objects.png"))
    print("objects: %d" % len(items))


def trainers():
    """Battle pictures: trainer fronts, the backs (first frame), and the cast's generated ones."""
    fronts = [(os.path.basename(p)[:-len("_front_pic.png")], artlib.load_grid(p))
              for p in sorted(glob.glob(os.path.join(G, "trainers/front_pics/*.png")))]
    for page in range(0, len(fronts), 96):
        artlib.label_sheet(fronts[page:page + 96], cols=12, scale=1).save(
            os.path.join(LIB, "trainer_fronts_%d.png" % (page // 96 + 1)))
    backs = []
    for p in sorted(glob.glob(os.path.join(G, "trainers/back_pics/*.png"))):
        grid = artlib.load_grid(p)
        w = len(grid[0])
        backs.append(("%s x%d" % (os.path.basename(p)[:-len("_back_pic.png")], len(grid) // w), artlib.crop(grid, 0, 0, w, w)))
    artlib.label_sheet(backs, cols=8, scale=2).save(os.path.join(LIB, "trainer_backs.png"))
    print("trainers: %d fronts, %d backs" % (len(fronts), len(backs)))


def pokemon():
    """Every Pokemon: front frame 1, front frame 2, back, overworld (first frame).
    Split into pages of 60, in folder (name) order."""
    entries = []
    for d in sorted(glob.glob(os.path.join(G, "pokemon/*/"))):
        name = os.path.basename(d.rstrip("/"))
        front = os.path.join(d, "anim_front.png")
        if not os.path.exists(front):
            continue
        f = artlib.load_grid(front)
        parts = [artlib.crop(f, 0, 0, 64, 64)]
        parts.append(artlib.crop(f, 0, 64, 64, 64) if len(f) >= 128 else artlib.blank(64, 64))
        back = os.path.join(d, "back.png")
        parts.append(artlib.load_grid(back) if os.path.exists(back) else artlib.blank(64, 64))
        ow = os.path.join(d, "overworld.png")
        if os.path.exists(ow):
            o = artlib.load_grid(ow)
            s = len(o)
            o = artlib.crop(o, 0, 0, s, s)
            box = artlib.blank(64, 64)
            artlib.paste(box, o, (64 - s) // 2, (64 - s) // 2)
            parts.append(box)
        else:
            parts.append(artlib.blank(64, 64))
        row = artlib.blank(256, 64)
        for i, p in enumerate(parts):
            artlib.paste(row, p, i * 64, 0)
        entries.append((name, row))
    pages = 0
    for page in range(0, len(entries), 60):
        artlib.label_sheet(entries[page:page + 60], cols=6, scale=1).save(
            os.path.join(LIB, "pokemon_%02d.png" % (page // 60 + 1)))
        pages += 1
    print("pokemon: %d species folders, %d pages" % (len(entries), pages))


def gfx_table():
    """OBJ_EVENT_GFX_ name -> (png path, frame width, frame height, frames used,
    frames drawn, palette tag), read from the object event source files."""
    src = os.path.join(artlib.REPO, "src/data/object_events")
    pics = {}  # gObjectEventPic_X -> png path
    for m in re.finditer(r'(gObjectEventPic_\w+)\[\]\s*=\s*INCBIN_\w+\("([^"]+)\.4bpp',
                         open(os.path.join(src, "object_event_graphics.h")).read()):
        pics[m.group(1)] = m.group(2) + ".png"
    tables = {}  # sPicTable_X -> (pic, frames)
    text = open(os.path.join(src, "object_event_pic_tables.h")).read()
    for m in re.finditer(r"(sPicTable_\w+)\[\]\s*=\s*\{(.*?)\};", text, re.S):
        used = re.findall(r"(gObjectEventPic_\w+)", m.group(2))
        tables[m.group(1)] = (used[0] if used else "", len(re.findall(r"frame\(", m.group(2))))
    infos = {}  # gObjectEventGraphicsInfo_X -> (table, palette tag, w, h)
    text = open(os.path.join(src, "object_event_graphics_info.h")).read()
    for m in re.finditer(r"(gObjectEventGraphicsInfo_\w+)\s*=\s*\{(.*?)\};", text, re.S):
        body = m.group(2)

        def field(f):
            fm = re.search(r"\.%s\s*=\s*([^,]+)," % f, body)
            return fm.group(1).strip() if fm else ""
        infos[m.group(1)] = (field("images"), field("paletteTag"), field("width"), field("height"))
    out = {}
    text = open(os.path.join(src, "object_event_graphics_info_pointers.h")).read()
    for m in re.finditer(r"\[(OBJ_EVENT_GFX_\w+)\]\s*=\s*&(gObjectEventGraphicsInfo_\w+)", text):
        table, pal, w, h = infos.get(m.group(2), ("", "", "", ""))
        pic, nframes = tables.get(table, ("", 0))
        png = pics.get(pic, "")
        real = 0
        if png and os.path.exists(os.path.join(artlib.REPO, png)) and w.isdigit():
            real = len(artlib.load_grid(os.path.join(artlib.REPO, png))[0]) // int(w)
        out[m.group(1)] = (png, int(w) if w.isdigit() else 16, int(h) if h.isdigit() else 32, nframes, real,
                           pal.replace("OBJ_EVENT_PAL_TAG_", ""))
    # aliases like "#define OBJ_EVENT_GFX_TWIN OBJ_EVENT_GFX_LITTLE_GIRL"
    for m in re.finditer(r"#define (OBJ_EVENT_GFX_\w+)\s+(OBJ_EVENT_GFX_\w+)",
                         open(os.path.join(artlib.REPO, "include/constants/event_objects.h")).read()):
        if m.group(2) in out and m.group(1) not in out:
            out[m.group(1)] = out[m.group(2)]
    return out


def catalogue():
    """catalogue.md: OBJ_EVENT_GFX_ name -> picture file, frame count, palette."""
    rows = [(k,) + (v[0], "%dx%d" % (v[1], v[2]), v[3], v[4], v[5]) for k, v in gfx_table().items()]
    out = os.path.join(LIB, "catalogue.md")
    with open(out, "w") as f:
        f.write("# Overworld graphics catalogue\n\nMade by `tools/research_red/art/library.py`.\n"
                "Frames drawn: 3 = stands still only, 9 = walks, 18+ = walks and runs.\n\n"
                "| Constant | Picture | Size | Frames used | Frames drawn | Palette |\n|---|---|---|---|---|---|\n")
        for r in rows:
            f.write("| %s | `%s` | %s | %d | %d | %s |\n" % r)
    print("catalogue: %d graphics -> %s" % (len(rows), out))


PARTS = {"people": people, "emerald": emerald, "objects": objects, "trainers": trainers, "pokemon": pokemon, "catalogue": catalogue}

if __name__ == "__main__":
    for part in (sys.argv[1:] or PARTS):
        PARTS[part]()
    print("library ->", LIB)
