#!/usr/bin/env python3
"""Research Red Art Kit: the overworld sprite maker.

Every character is a "recipe" in graphics/research_red/overworld/recipes.toml:
start from an existing body (FireRed, Emerald or ours), swap colours, borrow a
head, draw a few pixels, then let walkgen.py make the walking (and running)
frames by copying the motion of similar sprites. One command turns all
recipes into game-ready sheets, previews, and the code that adds them.

  python3 tools/research_red/art/ow_maker.py build [name ...]   # sheets + previews
  python3 tools/research_red/art/ow_maker.py install            # also write the game code
  python3 tools/research_red/art/ow_maker.py check              # are the outputs current? (rr_check)
  python3 tools/research_red/art/ow_maker.py colours BASE       # a base's colours, numbered, with where they're used
  python3 tools/research_red/art/ow_maker.py ref 08_elm         # the main colours of a ref art picture
  python3 tools/research_red/art/ow_maker.py templates BASE     # which walking sheets move most like BASE
  python3 tools/research_red/art/ow_maker.py grid NAME [0,1,2]   # frames as letter grids, to plan patches

Recipe keys (all optional except base):
  base       = "scientist"        body to start from (see artlib.sheet_path for names)
  head_from  = "em:steven"        take the head (rows 0..head_rows-1) from another sheet
  head_rows  = 23                 people stand in rows 11-30: hair ~11-16, face ~16-22,
                                  body ~23-27, legs ~28-30
  recolour   = [ { from = ["#hex", ...], to = "#hex", contrast = 1.0, rows = [0, 12], anchor = "#hex" }, ... ]
               a group of shades (e.g. the hair's light/mid/dark) -> one new colour,
               light and dark steps kept; rows (optional) = only rows a..b-1
               (for a colour the base uses on both hair and trousers);
               anchor (optional) = the shade that lands exactly on `to` (pick the
               one most of the area uses; otherwise the middle shade does);
               dirs (optional) = only frames facing these ways, e.g. ["up"]
               (long hair covers the back in the up frames);
               box (optional) = [x0, y0, x1, y1]: only inside that rectangle
               (x1, y1 not included), e.g. the hair hanging beside the body
  swap       = { "#old" = "#new" }  exact single-colour swaps (after recolour, whole sprite)
  patches    = [ { dir = "down", x = 4, y = 0, rows = ["..ab..", ...], colours = { a = "#hex" } } ]
               draw pixels on a standing frame (dir: down, up, left); "." keeps,
               "_" makes see-through. Walk frames are made after patches, so the
               walk picks them up; with walk = "keep" a patch is copied onto that
               direction's walk frames too (good for hair and faces).
  grow       = [ { colours = ["#hex", ...], outline = "#hex", fill = "#hex", rows = [11, 25], dirs = [...] } ]
               make a part bigger by one pixel all round (hair volume): see-through
               pixels touching those colours become the outline, the old outline
               becomes fill
  parts      = [ { part = "open_coat_scientist", colours = { S = "#hex", ... } } ]
               reusable pixel patches from graphics/research_red/overworld/parts.toml
               (an open lab coat, ...); colours fill the part's letters
  walk       = "make" | "keep"      make new walk frames, or keep the base's own
  templates  = ["auto"]           walking sheets to copy motion from ("auto" = the
                                  3 closest in body shape; or names)
  run        = false               also make running frames (player sheets: 20 frames)
  gfx        = "RR_ELM"           new OBJ_EVENT_GFX_RR_ELM constant ...
  replaces   = "BROCK"            ... or take over an existing OBJ_EVENT_GFX_BROCK
  palette    = "elm"              palette name; recipes with the same name share one
  fit_palette = "path.pal"        force every colour onto an existing palette (players)
  output     = "path.png"         where the sheet goes (default graphics/research_red/overworld/<name>.png)
  ref        = "08_elm"           the ref art picture to show beside it in overview.png
                                  (default: the ref file whose name ends with the recipe name)
  notes      = "..."              for people reading the recipe
  hand       = "bases/hand_elm.png"  a hand-edited sheet (aseprite.py import): used exactly
                                  as drawn; everything above is then only the history

Outputs: the sheet PNG + palette (.pal) in graphics/research_red/overworld/,
previews (walk GIF, frames, before/after) in E:\\...\\art\\toolkit\\overworld\\.
"""
import os
import re
import sys
import tomllib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import artlib  # noqa: E402
import walkgen  # noqa: E402

RECIPES = os.path.join(artlib.OW_DIR, "recipes.toml")
PARTS = os.path.join(artlib.OW_DIR, "parts.toml")
# the walking templates "auto" picked, saved so every machine builds the same
# sprites (delete a line, or run "build --retemplate", to pick again)
LOCK = os.path.join(artlib.OW_DIR, "templates.lock")
PREVIEW = "overworld"
DIRS = {"down": 0, "up": 1, "left": 2}
WALKS_OF = {0: (3, 4), 1: (5, 6), 2: (7, 8)}
PAL_TAG_BASE = 0x1180  # free range above the Poke Ball tags (0x1150-0x116A)


def load_lock():
    lock = {}
    if os.path.exists(LOCK):
        for line in open(LOCK):
            if "=" in line and not line.startswith("#"):
                k, v = line.split("=", 1)
                lock[k.strip()] = [t.strip() for t in v.split(",") if t.strip()]
    return lock


def save_lock(lock):
    with open(LOCK, "w") as f:
        f.write("# Walking templates picked by ow_maker.py for recipes with templates = [\"auto\"].\n"
                "# Delete a line (or run ow_maker.py build --retemplate) to pick again.\n")
        for k in sorted(lock):
            f.write("%s = %s\n" % (k, ", ".join(lock[k])))


LOCKED = load_lock()
RETEMPLATE = False


def load_recipes():
    with open(RECIPES, "rb") as f:
        data = tomllib.load(f)
    return {k: v for k, v in data.items() if isinstance(v, dict)}


# ---------------------------------------------------------------- building one sheet

def apply_patch(frame, patch):
    legend = {k: artlib.parse_hex(v) for k, v in patch.get("colours", {}).items()}
    for dy, line in enumerate(patch["rows"]):
        for dx, ch in enumerate(line):
            x, y = patch.get("x", 0) + dx, patch.get("y", 0) + dy
            if ch == "." or not (0 <= y < len(frame) and 0 <= x < len(frame[0])):
                continue
            frame[y][x] = None if ch == "_" else legend[ch]


def load_parts():
    if not os.path.exists(PARTS):
        return {}
    with open(PARTS, "rb") as f:
        return tomllib.load(f)


def grow(frame, g):
    """One pixel more of a part (hair) all round, inside the given rows."""
    cols = {artlib.parse_hex(c) for c in g["colours"]}
    outline, fill = artlib.parse_hex(g["outline"]), artlib.parse_hex(g.get("fill", g["colours"][0]))
    y0, y1 = g.get("rows", [0, 32])
    h, w = len(frame), len(frame[0])
    new = []
    for y in range(y0, min(y1, h)):
        for x in range(w):
            if frame[y][x] is None and any(0 <= x + dx < w and 0 <= y + dy < h and frame[y + dy][x + dx] in cols
                                           for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1))):
                new.append((x, y))
    old_edge = set()
    for x, y in new:
        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            if 0 <= x + dx < w and 0 <= y + dy < h and frame[y + dy][x + dx] == outline:
                old_edge.add((x + dx, y + dy))
    for x, y in new:
        frame[y][x] = outline
    for x, y in old_edge:
        if (x, y) not in new:
            frame[y][x] = fill


def build_frames(name, r):
    """The finished frames for one recipe (list of 16x32 grids)."""
    if r.get("hand"):
        # hand-finished in a pixel editor: the drawing is the sprite
        return artlib.sheet_frames(artlib.load_grid(os.path.join(artlib.OW_DIR, r["hand"])))
    frames = artlib.sheet_frames(artlib.people_sheet(r["base"]))
    frames = [[row[:] for row in f] for f in frames]
    if len(frames) < 3:
        raise SystemExit("%s: base %s has fewer than 3 frames" % (name, r["base"]))
    if r.get("head_from"):
        hr = r.get("head_rows", 23)
        head = artlib.sheet_frames(artlib.people_sheet(r["head_from"]))
        for i, f in enumerate(frames[:9]):
            src = head[i] if i < len(head) else head[artlib.STAND_OF.get(i, i)]
            for y in range(hr):
                f[y] = src[y][:]
    # colours: ramps first (each limited to its rows), then exact swaps
    orig = [[row[:] for row in f] for f in frames]
    for rc in r.get("recolour", []):
        cmap = artlib.recolour_ramp([artlib.parse_hex(c) for c in rc["from"]], artlib.parse_hex(rc["to"]),
                                    rc.get("contrast", 1.0), artlib.parse_hex(rc["anchor"]) if rc.get("anchor") else None)
        y0, y1 = rc.get("rows", [0, 32])
        bx0, by0, bx1, by1 = rc.get("box", [0, 0, 16, 32])
        y0, y1 = max(y0, by0), min(y1, by1)
        want = {DIRS[d] for d in rc.get("dirs", DIRS)}
        for i, (f, o) in enumerate(zip(frames, orig)):
            if i < 9 and artlib.STAND_OF.get(i, i) not in want:
                continue
            for y in range(y0, min(y1, len(f))):
                f[y] = [cmap[o[y][x]] if (o[y][x] in cmap and bx0 <= x < bx1) else f[y][x] for x in range(len(f[y]))]
    cmap = {artlib.parse_hex(a): artlib.parse_hex(b) for a, b in r.get("swap", {}).items()}
    frames = [artlib.apply_map(f, cmap) for f in frames]
    walk = r.get("walk", "make" if len(frames) < 9 else "keep")
    for g in r.get("grow", []):
        for d in g.get("dirs", DIRS):
            grow(frames[DIRS[d]], g)
            if walk == "keep" and len(frames) >= 9:
                for wi in WALKS_OF[DIRS[d]]:
                    grow(frames[wi], g)
    patches = list(r.get("patches", []))
    parts = load_parts()
    for use in r.get("parts", []):
        for p in parts[use["part"]]["patches"]:
            patches.append(dict(p, colours=dict(p.get("colours", {}), **use.get("colours", {}))))
    for p in patches:
        d = DIRS[p["dir"]]
        apply_patch(frames[d], p)
        if walk == "keep" and len(frames) >= 9:
            for wi in WALKS_OF[d]:
                # the base's own walk frames bob the head down; a patch on the
                # head (a cap) has to bob with it or it slides off
                bob = walkgen.head_bob(frames[d], frames[wi]) if p.get("y", 0) < walkgen.RIGID_TO else 0
                apply_patch(frames[wi], dict(p, y=p.get("y", 0) + bob))
    if walk == "make" or len(frames) < 9:
        names = r.get("templates", ["auto"])
        if names == ["auto"] and name in LOCKED and not RETEMPLATE:
            names = LOCKED[name]
        if names == ["auto"]:
            picks = walkgen.best_templates(frames[:3], n=3, exclude=(r["base"],))
            templates = [p[2] for p in picks]
            r["_templates"] = LOCKED[name] = [p[1] for p in picks]
            for t in r["_templates"]:
                artlib.sheet_path(t)  # copies an Emerald template into the repo
        else:
            templates = [artlib.sheet_frames(artlib.people_sheet(t)) for t in names]
            r["_templates"] = names
        frames = walkgen.make_walk(frames[:3], templates)
    else:
        frames = frames[:9]
    if r.get("run"):
        # FireRed keeps Red's runs in red_surf_run frames 3-11 (south, north, west)
        red = artlib.sheet_frames(artlib.people_sheet("red_normal"))[:9] + artlib.sheet_frames(artlib.people_sheet("red_surf_run"))[3:12]
        frames = frames + walkgen.make_run(frames, red) + [frames[0], frames[0]]
    return frames


def fit_to_palette(frames, pal_path):
    pal = read_jasc(pal_path)[1:]
    out = []
    for f in frames:
        out.append([[None if p is None else min(pal, key=lambda c: artlib.dist2(c, p)) for p in row] for row in f])
    return out, pal


def read_jasc(path):
    lines = open(path).read().split()
    n = int(lines[2])
    vals = list(map(int, lines[3:3 + n * 3]))
    return [tuple(vals[i:i + 3]) for i in range(0, n * 3, 3)]


def output_png(name, r):
    return os.path.join(artlib.REPO, r["output"]) if r.get("output") else os.path.join(artlib.OW_DIR, name + ".png")


def build_all(only=None):
    recipes = load_recipes()
    built = {}
    for name, r in recipes.items():
        if only and name not in only:
            continue
        frames = build_frames(name, r)
        built[name] = frames
    # palettes: recipes sharing a palette name share one set of colours
    groups = {}
    for name, frames in built.items():
        groups.setdefault(recipes[name].get("palette", name), []).append(name)
    results = {}
    for pal_name, members in groups.items():
        r0 = recipes[members[0]]
        if r0.get("fit_palette"):
            for m in members:
                built[m], pal = fit_to_palette(built[m], os.path.join(artlib.REPO, r0["fit_palette"]))
                results[m] = (built[m], pal, None)
            continue
        # snap to GBA colours, then merge if the group has too many
        for m in members:
            built[m] = [artlib.apply_map(f, {c: artlib.gba_colour(c) for c in artlib.colours(f)}) for f in built[m]]
        union = artlib.join_frames([f for m in members for f in built[m]])
        cols = artlib.colours(union)
        if len(cols) > 15:
            print("  %s: %d colours, merging the closest down to 15" % (pal_name, len(cols)))
            merged = artlib.reduce_colours(union, 15)
            cmap = {}
            for y, row in enumerate(union):
                for x, p in enumerate(row):
                    if p is not None:
                        cmap[p] = merged[y][x]
            for m in members:
                built[m] = [artlib.apply_map(f, cmap) for f in built[m]]
            cols = artlib.colours(artlib.join_frames([f for m in members for f in built[m]]))
        pal = sorted(cols, key=lambda c: artlib.hls(c)[1])  # dark to light, easier to read
        for m in members:
            results[m] = (built[m], pal, pal_name if not r0.get("fit_palette") else None)
    return recipes, results


def write_outputs(recipes, results):
    for name, (frames, pal, pal_name) in results.items():
        r = recipes[name]
        png = output_png(name, r)
        artlib.save_indexed(artlib.join_frames(frames), png, pal)
        if pal_name:
            artlib.write_jasc_pal(pal, os.path.join(artlib.OW_DIR, pal_name + ".pal"))
        previews(name, r, frames)
        print("  %-14s %2d frames, %2d colours%s -> %s" % (
            name, len(frames), len(pal), "  walk like " + ", ".join(r["_templates"]) if r.get("_templates") else "",
            os.path.relpath(png, artlib.REPO)))
    overview(recipes, results)


def ref_thumb(name, r, height=96):
    """The ref art picture for a recipe, shrunk (None if there is none)."""
    import glob
    from PIL import Image
    want = r.get("ref")
    paths = [os.path.join(artlib.REF_ART, want + ".png")] if want else \
        [p for p in glob.glob(os.path.join(artlib.REF_ART, "*.png")) if os.path.basename(p)[3:-4] in (name, name.replace("lt_", ""))]
    if not paths or not os.path.exists(paths[0]):
        return None
    im = Image.open(paths[0]).convert("RGBA")
    im = im.crop(im.getbbox())
    im.thumbnail((height, height))
    return im


def overview(recipes, results):
    """One sheet: each recipe's ref art beside its new frames (for reviewing)."""
    from PIL import Image
    items = []
    for name, (frames, _, _) in results.items():
        strip = artlib.to_rgba(artlib.join_frames(frames[:9]), 3, "checker")
        ref = ref_thumb(name, recipes[name])
        row = Image.new("RGBA", (strip.width + 104, 96), (60, 60, 70, 255))
        if ref:
            row.alpha_composite(ref, ((96 - ref.width) // 2, 0))
        row.alpha_composite(strip, (104, 0))
        items.append((name, row))
    out = os.path.join(artlib.out_dir(PREVIEW), "overview.png")
    artlib.label_sheet(items, cols=2).save(out)
    print("overview ->", out)


def previews(name, r, frames):
    d = artlib.out_dir(PREVIEW)
    seq, ms = artlib.walk_strip(frames[:9])
    artlib.save_gif(seq, os.path.join(d, name + "_walk.gif"), ms)
    if len(frames) >= 18:
        seq, ms = artlib.run_strip(frames[9:18])
        artlib.save_gif(seq, os.path.join(d, name + "_run.gif"), ms)
    before = artlib.sheet_frames(artlib.people_sheet(r["base"]))
    items = [("before: " + r["base"], artlib.join_frames(before[:9] if len(before) >= 9 else before[:3])),
             ("after: " + name, artlib.join_frames(frames[:9]))]
    if len(frames) >= 18:
        items.append(("running", artlib.join_frames(frames[9:18])))
    artlib.label_sheet(items, cols=1, scale=4).save(os.path.join(d, name + "_frames.png"))


# ---------------------------------------------------------------- game code

MARK_BEGIN = "// Research Red: Art Kit overworld sprites (%s) BEGIN: generated by tools/research_red/art/ow_maker.py install; edit recipes.toml, not this block"
MARK_END = "// Research Red: Art Kit overworld sprites (%s) END"


def code_names(name, r):
    camel = "RR_" + "".join(p.capitalize() for p in name.split("_"))
    return {
        "gfx": "OBJ_EVENT_GFX_" + (r["replaces"] if r.get("replaces") else r.get("gfx", "RR_" + name.upper())),
        "pic": "gObjectEventPic_" + camel,
        "table": "sPicTable_" + camel,
        "info": "gObjectEventGraphicsInfo_" + camel,
    }


def pal_names(pal_name):
    camel = "RR_" + "".join(p.capitalize() for p in pal_name.split("_"))
    return "gObjectEventPal_" + camel, "OBJ_EVENT_PAL_TAG_" + "RR_" + pal_name.upper()


def replace_block(path, part, body, anchor_before=None, anchor_after=None, indent=""):
    """Write body between the Art Kit markers for one part (a file can have
    several parts) in an engine file; the first time, put the markers just
    before anchor_before (or after anchor_after)."""
    text = open(path).read()
    begin, end = MARK_BEGIN % part, MARK_END % part
    block = "%s%s\n%s%s%s\n" % (indent, begin, body, indent, end)
    if begin in text:
        text = re.sub(r"[ \t]*" + re.escape(begin) + r".*?" + re.escape(end) + r"\n", lambda m: block, text, flags=re.S)
    elif anchor_before:
        i = text.index(anchor_before)
        i = text.rfind("\n", 0, i) + 1
        text = text[:i] + block + text[i:]
    else:
        i = text.index(anchor_after) + len(anchor_after)
        i = text.index("\n", i) + 1
        text = text[:i] + block + text[i:]
    open(path, "w").write(text)


def install(recipes, results):
    src = os.path.join(artlib.REPO, "src/data/object_events")
    pals = []  # (pal_name, path) in recipe order, no repeats
    for name in recipes:
        if name in results and results[name][2] and results[name][2] not in [p for p, _ in pals]:
            pals.append((results[name][2], os.path.relpath(os.path.join(artlib.OW_DIR, results[name][2] + ".gbapal"), artlib.REPO)))
    entries = [(n, recipes[n], code_names(n, recipes[n])) for n in recipes if n in results and not recipes[n].get("output")]

    # constants: new gfx ids (inside the enum) and palette tags
    body = "".join("    %s,\n" % c["gfx"] for n, r, c in entries if not r.get("replaces"))
    replace_block(os.path.join(artlib.REPO, "include/constants/event_objects.h"), "ids", body, anchor_before="    NUM_OBJ_EVENT_GFX,", indent="    ")
    path = os.path.join(artlib.REPO, "include/constants/event_objects.h")
    text = open(path).read()
    tags = "".join("#define %-40s 0x%04X\n" % (pal_names(p)[1], PAL_TAG_BASE + i) for i, (p, _) in enumerate(pals))
    tb, te = "// Research Red: Art Kit palette tags BEGIN (generated)\n", "// Research Red: Art Kit palette tags END\n"
    if tb in text:
        text = re.sub(re.escape(tb) + ".*?" + re.escape(te), lambda m: tb + tags + te, text, flags=re.S)
    else:
        i = text.index("#define OBJ_EVENT_PAL_TAG_DYNAMIC")
        i = text.index("\n", i) + 1
        text = text[:i] + "\n" + tb + tags + te + text[i:]
    open(path, "w").write(text)

    # pictures and palettes
    body = "".join('const u16 %s[] = INCBIN_U16("%s");\n' % (c["pic"], os.path.relpath(output_png(n, r), artlib.REPO)[:-4] + ".4bpp")
                   for n, r, c in entries)
    body += "".join('const u16 %s[] = INCBIN_U16("%s");\n' % (pal_names(p)[0], path) for p, path in pals)
    replace_block(os.path.join(src, "object_event_graphics.h"), "pictures", body, anchor_before="const u16 gObjectEventPic_RedNormal[]")

    # frame tables
    body = ""
    for n, r, c in entries:
        nf = len(results[n][0])
        body += "static const struct SpriteFrameImage %s[] = {\n" % c["table"]
        body += "".join("    overworld_frame(%s, 2, 4, %d),\n" % (c["pic"], i) for i in range(nf))
        body += "};\n\n"
    replace_block(os.path.join(src, "object_event_pic_tables.h"), "frames", body, anchor_before="static const struct SpriteFrameImage sPicTable_RedNormal[]")

    # graphics info (copies FireRed's standard 16x32 person)
    body = ""
    for n, r, c in entries:
        body += """const struct ObjectEventGraphicsInfo %s = {
    .tileTag = TAG_NONE,
    .paletteTag = %s,
    .reflectionPaletteTag = OBJ_EVENT_PAL_TAG_NONE,
    .size = 256,
    .width = 16,
    .height = 32,
    .paletteSlot = PALSLOT_NPC_1,
    .shadowSize = SHADOW_SIZE_M,
    .inanimate = FALSE,
    .compressed = FALSE,
    .tracks = TRACKS_FOOT,
    .oam = &gObjectEventBaseOam_16x32,
    .subspriteTables = sOamTables_16x32,
    .anims = sAnimTable_Standard,
    .images = %s,
    .affineAnims = gDummySpriteAffineAnimTable,
};

""" % (c["info"], pal_names(results[n][2])[1], c["table"])
    replace_block(os.path.join(src, "object_event_graphics_info.h"), "info", body, anchor_before="const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_RedNormal")

    # pointers: externs + table entries for new ids; replaced ids are re-pointed
    path = os.path.join(src, "object_event_graphics_info_pointers.h")
    body = "".join("extern const struct ObjectEventGraphicsInfo %s;\n" % c["info"] for n, r, c in entries)
    replace_block(path, "externs", body, anchor_before="extern const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_RedNormal;")
    body = "".join("    [%s] = &%s,\n" % (c["gfx"], c["info"]) for n, r, c in entries if not r.get("replaces"))
    text = open(path).read()
    anchor = re.search(r"\n[ \t]*\[OBJ_EVENT_GFX_RED_NORMAL\]", text).group(0).strip()
    replace_block(path, "table", body, anchor_before=anchor, indent="    ")
    text = open(path).read()
    for n, r, c in entries:
        if r.get("replaces"):
            pat = re.compile(r"(\[%s\]\s*=\s*&)gObjectEventGraphicsInfo_\w+,[^\n]*" % re.escape(c["gfx"]))
            if not pat.search(text):
                raise SystemExit("%s: no pointer line for %s" % (n, c["gfx"]))
            text = pat.sub(lambda m: m.group(1) + c["info"] + ", // Research Red: Art Kit (%s)" % n, text)
    open(path, "w").write(text)

    # palette table
    body = "".join("    {%s, %s},\n" % pal_names(p) for p, _ in pals)
    replace_block(os.path.join(artlib.REPO, "src/event_object_movement.c"), "palettes", body, anchor_before="    {gObjectEventPal_Substitute,", indent="    ")
    print("installed %d sprites, %d palettes into the game code" % (len(entries), len(pals)))


# ---------------------------------------------------------------- helper commands

def cmd_colours(base):
    frames = artlib.sheet_frames(artlib.people_sheet(base))
    sheet = artlib.join_frames(frames[:3])
    cols = artlib.colours(sheet)
    items = [("all", sheet)]
    for c in cols:
        mask = [[(c if p == c else ((40, 40, 40) if p is not None else None)) for p in row] for row in sheet]
        items.append((artlib.hexc(c), mask))
    out = os.path.join(artlib.out_dir(PREVIEW, "colours"), base.replace(":", "_").replace("/", "_") + ".png")
    artlib.label_sheet(items, cols=7, scale=2).save(out)
    print("  colour   hair(-16) face(16-22) body(23-27) legs(28-)")
    for c in cols:
        print("  %s  %5d %5d %5d %5d" % (artlib.hexc(c), *(sum(row.count(c) for row in sheet[a:b]) for a, b in ((0, 16), (16, 23), (23, 28), (28, 32)))))
    print("->", out)


def cmd_ref(ref):
    path = os.path.join(artlib.REF_ART, ref + ".png")
    from PIL import Image
    im = Image.open(path).convert("RGBA")
    im.thumbnail((256, 384))
    g = [[None if a < 128 else (r, gg, b) for (r, gg, b, a) in [im.getpixel((x, y)) for x in range(im.width)]] for y in range(im.height)]
    dom = artlib.dominant_colours(g, 16)
    print("\n".join("  %s  %5d" % (artlib.hexc(c), n) for c, n in dom))
    items = [(ref, artlib.to_rgba(g, 1, "checker"))] + [(artlib.hexc(c), [[c] * 24 for _ in range(24)]) for c, _ in dom]
    out = os.path.join(artlib.out_dir(PREVIEW, "colours"), "ref_" + ref + ".png")
    artlib.label_sheet(items, cols=6, scale=2).save(out)
    print("->", out)


def cmd_grid(name, frames="0,1,2"):
    """Print frames as letter grids (one letter per colour) to plan patches.
    name: a recipe (its built sheet) or any base name."""
    recipes = load_recipes()
    if name in recipes:
        g = artlib.load_grid(output_png(name, recipes[name]))
    else:
        g = artlib.people_sheet(name)
    fr = artlib.sheet_frames(g)
    idx = [int(i) for i in frames.split(",")]
    cols = artlib.colours(artlib.join_frames([fr[i] for i in idx]))
    key = {c: "abcdefghijklmnopqrstuvwxyz"[i] for i, c in enumerate(cols)}
    print("  " + "  ".join("%s=%s" % (key[c], artlib.hexc(c)) for c in cols))
    print("     " + "   ".join(("frame %d" % i).ljust(16) for i in idx))
    print("     " + "   ".join("0123456789abcdef" for _ in idx))
    for y in range(8, 32):
        print("  %2d " % y + "   ".join("".join("." if p is None else key[p] for p in fr[i][y]) for i in idx))


def cmd_templates(base):
    frames = artlib.sheet_frames(artlib.people_sheet(base))
    for score, name, _ in walkgen.best_templates(frames[:3], n=8, exclude=(base,)):
        print("  %.2f  %s" % (score, name))


def cmd_check():
    """Rebuild in memory and compare with the files (stale outputs = fail)."""
    rules = open(os.path.join(artlib.REPO, "spritesheet_rules.mk")).read()
    if "graphics/research_red/overworld/%.4bpp" not in rules:
        # without it the build cuts the sheets into tiles in the wrong order
        print("spritesheet_rules.mk lost the 16x32 rule for graphics/research_red/overworld/")
        return 1
    recipes, results = build_all()
    stale = []
    for name, (frames, pal, _) in results.items():
        png = output_png(name, recipes[name])
        if not os.path.exists(png) or artlib.load_grid(png) != artlib.join_frames(frames):
            stale.append(name)
    if stale:
        print("overworld sprites out of date (run ow_maker.py install):", ", ".join(stale))
        return 1
    print("overworld sprites: %d recipes, all current" % len(results))
    return 0


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0
    cmd, rest = argv[0], argv[1:]
    global RETEMPLATE
    if "--retemplate" in rest:
        RETEMPLATE = True
        rest = [a for a in rest if a != "--retemplate"]
    if cmd == "build":
        write_outputs(*build_all(set(rest) or None))
        save_lock(LOCKED)
    elif cmd == "install":
        recipes, results = build_all()
        write_outputs(recipes, results)
        save_lock(LOCKED)
        install(recipes, results)
    elif cmd in ("check", "--check"):
        return cmd_check()
    elif cmd == "colours":
        cmd_colours(rest[0])
    elif cmd == "ref":
        cmd_ref(rest[0])
    elif cmd == "templates":
        cmd_templates(rest[0])
    elif cmd == "grid":
        cmd_grid(rest[0], rest[1] if len(rest) > 1 else "0,1,2")
    else:
        print(__doc__)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
