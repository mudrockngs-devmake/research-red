#!/usr/bin/env python3
"""Research Red Art Kit: map tiles (furniture, building fronts, decorations).

Draw a piece as an ordinary PNG (any colours, see-through background, width
and height in steps of 16 pixels) and this adds it to a tileset as new map
blocks ("metatiles"), ready to paint in Porymap or place with a script.

It only ever ADDS: new tiles go on the end of the tileset, new colours go into
palette slots nothing uses yet, and new blocks get the next free numbers.
Nothing that already exists moves, so no existing map can break.

  python3 tools/research_red/art/tile_kit.py info gTileset_PalletTown
      How full a tileset is (tiles, blocks, palette slots) and a numbered
      picture of its blocks with what each one does (behaviour, layer).
  python3 tools/research_red/art/tile_kit.py add gTileset_GenericBuilding1 table.png
          --floor 0x201 [--top table_top.png] [--behavior MB_COUNTER]
          [--label Table] [--dry-run]
      Add a piece. table.png is the part drawn UNDER the player; --top is an
      optional same-size picture drawn OVER the player (a roof edge, the top
      of a tree, a lamp: things you walk behind). --floor: the block whose
      ground shows through the see-through parts (a number from `info`);
      needed whenever the piece isn't a full square of pixels, because the
      GBA shows a garbage tile under see-through ground.
      --behavior: what the blocks do (MB_NORMAL, MB_COUNTER, MB_BOOKSHELF,
      MB_PC, MB_TELEVISION, ... see `behaviors`); whether you can walk on it
      is set per map square in Porymap (collision), not here. --label adds
      METATILE_<Tileset>_<Label>_<n> names to include/constants/metatile_labels.h
      for scripts (setmetatile). --dry-run: report only, change nothing.
  python3 tools/research_red/art/tile_kit.py place LAYOUT_X|MAP_X X Y FIRST_ID W H [--collision 1]
      Stamp W x H blocks starting at block number FIRST_ID (as printed by
      add) into a map at square X, Y. Collision 1 = blocked, 0 = walkable.
  python3 tools/research_red/art/tile_kit.py behaviors [WORD]
      List the behaviour names (optionally only those containing WORD).

Previews go to E:\\Workspace\\romhacks\\research-red\\art\\toolkit\\tiles\\.
For a whole NEW tileset (a greenhouse, a fossil park), use Porytiles, which
builds a tileset from layer PNGs: see docs/ART_TOOLS.md.
"""
import argparse
import json
import os
import re
import struct
import sys

from PIL import Image, ImageDraw

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import artlib  # noqa: E402
import render_map  # noqa: E402

REPO = artlib.REPO
NUM_PRIMARY_TILES, NUM_TILES_TOTAL = 640, 1024
NUM_PRIMARY_METATILES, NUM_METATILES_TOTAL = 640, 1024
NUM_PRIMARY_PALS, NUM_PALS_TOTAL = 7, 13
LAYERS = {"normal": 0, "covered": 1, "split": 2}
LAYER_NAMES = {0: "N", 1: "C", 2: "S"}


# ---------------------------------------------------------------- reading the project

def behaviors():
    """Behaviour name -> number, from include/constants/metatile_behaviors.h."""
    text = open(os.path.join(REPO, "include/constants/metatile_behaviors.h")).read()
    body = text[text.index("{") + 1:text.index("}")]
    out, n = {}, 0
    for line in body.split("\n"):
        m = re.match(r"\s*(MB_\w+)\s*(?:=\s*(\w+))?\s*,", line)
        if m:
            if m.group(2):
                n = int(m.group(2), 0)
            out[m.group(1)] = n
            n += 1
    return out


def is_secondary(symbol):
    headers = open(os.path.join(REPO, "src/data/tilesets/headers.h")).read()
    body = headers.split("const struct Tileset %s =" % symbol)[1].split("};")[0]
    return "isSecondary = TRUE" in body


def partner_primary(secondary):
    """A primary tileset that maps using this secondary are drawn with."""
    for lay in json.load(open(os.path.join(REPO, "data/layouts/layouts.json")))["layouts"]:
        if lay.get("secondary_tileset") == secondary:
            return lay["primary_tileset"]
    return "gTileset_Building"


def read_pal(path):
    if not os.path.exists(path):
        return [(0, 0, 0)] * 16
    lines = open(path).read().split("\n")[3:19]
    cols = [tuple(int(v) for v in ln.split()) for ln in lines if ln.strip()]
    return cols + [(0, 0, 0)] * (16 - len(cols))


def write_pal(path, cols):
    with open(path, "w", newline="\r\n") as f:
        f.write("JASC-PAL\n0100\n16\n")
        for r, g, b in cols:
            f.write("%d %d %d\n" % (r, g, b))


class Tileset:
    """One tileset's files, loaded for reading and appending."""

    def __init__(self, symbol):
        self.symbol = symbol
        self.dir = render_map.tileset_dir(symbol)
        self.secondary = is_secondary(symbol)
        self.tiles = Image.open(os.path.join(self.dir, "tiles.png"))
        self.pals = [read_pal(os.path.join(self.dir, "palettes", "%02d.pal" % i)) for i in range(16)]
        self.metatiles = bytearray(open(os.path.join(self.dir, "metatiles.bin"), "rb").read())
        self.attrs = bytearray(open(os.path.join(self.dir, "metatile_attributes.bin"), "rb").read())
        self.tile_base = NUM_PRIMARY_TILES if self.secondary else 0
        self.meta_base = NUM_PRIMARY_METATILES if self.secondary else 0
        self.tile_max = (NUM_TILES_TOTAL - NUM_PRIMARY_TILES) if self.secondary else NUM_PRIMARY_TILES
        self.meta_max = (NUM_METATILES_TOTAL - NUM_PRIMARY_METATILES) if self.secondary else NUM_PRIMARY_METATILES
        self.own_pals = range(NUM_PRIMARY_PALS, NUM_PALS_TOTAL) if self.secondary else range(0, NUM_PRIMARY_PALS)

    def tile_count(self):
        """Tiles in tiles.png up to the last one that isn't empty."""
        n = (self.tiles.width // 8) * (self.tiles.height // 8)
        while n > 0 and not any(any(r) for r in render_map.tile_pixels(self.tiles, n - 1)):
            n -= 1
        return n

    def meta_count(self):
        return len(self.metatiles) // 16

    def used_slots(self):
        """For each palette: the colour slots some block of THIS tileset uses."""
        used = {p: set() for p in range(16)}
        for m in range(self.meta_count()):
            for k in range(8):
                e = struct.unpack_from("<H", self.metatiles, (m * 8 + k) * 2)[0]
                tid, pal = e & 0x3FF, e >> 12
                local = tid - self.tile_base
                if 0 <= local < (self.tiles.width // 8) * (self.tiles.height // 8):
                    for row in render_map.tile_pixels(self.tiles, local):
                        used[pal].update(row)
        return used

    def save(self):
        self.tiles.save(os.path.join(self.dir, "tiles.png"))
        open(os.path.join(self.dir, "metatiles.bin"), "wb").write(self.metatiles)
        open(os.path.join(self.dir, "metatile_attributes.bin"), "wb").write(self.attrs)
        for p in self.own_pals:
            write_pal(os.path.join(self.dir, "palettes", "%02d.pal" % p), self.pals[p])


# ---------------------------------------------------------------- info

def cmd_info(symbol):
    ts = Tileset(symbol)
    used = ts.used_slots()
    print("%s (%s tileset, %s)" % (symbol, "secondary" if ts.secondary else "primary", os.path.relpath(ts.dir, REPO)))
    print("  tiles:  %d of %d used" % (ts.tile_count(), ts.tile_max))
    print("  blocks: %d of %d used (numbers 0x%03X-0x%03X)" % (ts.meta_count(), ts.meta_max, ts.meta_base, ts.meta_base + ts.meta_count() - 1))
    for p in ts.own_pals:
        free = 15 - len(used[p] - {0})
        print("  palette %2d: %2d colour slots free" % (p, free))
    names = {v: k for k, v in behaviors().items()}
    primary = partner_primary(symbol) if ts.secondary else symbol
    r = render_map.Renderer(primary, symbol if ts.secondary else None)
    count = ts.meta_count()
    cols = 8
    img = Image.new("RGBA", (cols * 120, ((count + cols - 1) // cols) * 60), (40, 40, 40, 255))
    d = ImageDraw.Draw(img)
    for i in range(count):
        a = struct.unpack_from("<I", ts.attrs, i * 4)[0]
        beh, layer = a & 0x1FF, (a >> 29) & 3
        t = r.metatile(ts.meta_base + i).resize((32, 32), Image.NEAREST)
        x, y = (i % cols) * 120, (i // cols) * 60
        img.paste(t, (x + 4, y + 4), t)
        d.text((x + 40, y + 4), "%03X %s" % (ts.meta_base + i, LAYER_NAMES.get(layer, "?")), fill=(255, 255, 255, 255))
        if beh:
            d.text((x + 40, y + 18), names.get(beh, str(beh)).replace("MB_", "")[:12].lower(), fill=(255, 220, 120, 255))
    out = os.path.join(artlib.out_dir("tiles"), symbol.replace("gTileset_", "") + "_blocks.png")
    img.save(out)
    print("  picture ->", out, "(N/C/S = normal/covered/split layer; yellow = behaviour)")


# ---------------------------------------------------------------- add

def split_tiles(grid):
    """Grid (multiple of 16) -> {(bx, by): [4 tiles of 8x8 grids]} per 16x16 block."""
    h, w = len(grid), len(grid[0])
    blocks = {}
    for by in range(h // 16):
        for bx in range(w // 16):
            blocks[(bx, by)] = [artlib.crop(grid, bx * 16 + (k % 2) * 8, by * 16 + (k // 2) * 8, 8, 8) for k in range(4)]
    return blocks


def fit_palette(ts, colours_needed, used, forced=None):
    """Pick one of the tileset's own palettes for the piece and say which slot
    each colour goes to. Reuses near-identical colours already in the palette,
    puts the rest into free slots. Returns (palette, {colour: slot}, new_pal) or
    (None, reason, None)."""
    best = None
    for p in ([forced] if forced is not None else ts.own_pals):
        pal = list(ts.pals[p])
        mapping, free = {}, [s for s in range(1, 16) if s not in used[p]]
        cost = 0
        for c in colours_needed:
            slot = next((s for s in range(1, 16) if s in used[p] and artlib.dist2(pal[s], c) <= 3 * 64), None)
            if slot is None:
                if not free:
                    break
                slot = free.pop(0)
                pal[slot] = c
                used_now = True  # noqa: F841 (slot taken for this piece)
                cost += 1
            mapping[c] = slot
        else:
            # prefer the palette that needs the fewest new colours
            if best is None or cost < best[0]:
                best = (cost, p, mapping, pal)
    if best is None:
        return None, "no palette of %s has room for %d colours" % (ts.symbol, len(colours_needed)), None
    return best[1], best[2], best[3]


def find_tile(ts, tile_idx, extra):
    """Index of an identical tile (also mirrored) already in the tileset or
    added in this run; returns (index, hflip, vflip) or None."""
    variants = [(tile_idx, 0, 0), ([r[::-1] for r in tile_idx], 1, 0),
                (tile_idx[::-1], 0, 1), ([r[::-1] for r in tile_idx[::-1]], 1, 1)]
    for i, t in enumerate(extra):
        for v, hf, vf in variants:
            if t == v:
                return i, hf, vf
    return None


def floor_grid(ts, floor_id, w, h):
    """The floor block's look (its bottom layer) repeated to w x h pixels."""
    primary = partner_primary(ts.symbol) if ts.secondary else ts.symbol
    r = render_map.Renderer(primary, ts.symbol if ts.secondary else None)
    one = artlib.load_grid_from_image(r.metatile(floor_id))
    return [[one[y % 16][x % 16] for x in range(w)] for y in range(h)]


def cmd_add(symbol, bottom_path, top_path=None, layer="normal", behavior="MB_NORMAL", label=None, dry=False, floor=None):
    ts = Tileset(symbol)
    bottom = artlib.load_grid(bottom_path)
    top = artlib.load_grid(top_path) if top_path else artlib.blank(len(bottom[0]), len(bottom))
    h, w = len(bottom), len(bottom[0])
    if w % 16 or h % 16:
        raise SystemExit("%s is %dx%d; width and height must be steps of 16 pixels" % (bottom_path, w, h))
    floor_only = set()  # blocks whose ground has nothing drawn on it: reuse the floor block's tiles
    if any(p is None for row in bottom for p in row):
        if floor is None:
            raise SystemExit("the piece has see-through pixels: give --floor BLOCK (the ground under it; numbers in `info`)")
        for by in range(h // 16):
            for bx in range(w // 16):
                if all(bottom[y][x] is None for y in range(by * 16, by * 16 + 16) for x in range(bx * 16, bx * 16 + 16)):
                    floor_only.add((bx, by))
        ground = floor_grid(ts, floor, w, h)
        bottom = [[bottom[y][x] if bottom[y][x] is not None else ground[y][x] for x in range(w)] for y in range(h)]
    if len(top) != h or len(top[0]) != w:
        raise SystemExit("--top must be the same size as the piece")
    beh = behaviors().get(behavior)
    if beh is None:
        raise SystemExit("unknown behaviour %s (see: tile_kit.py behaviors)" % behavior)
    # GBA colours, at most 15 for the whole piece (plain-floor squares don't count)
    for (bx, by) in floor_only:
        for y in range(by * 16, by * 16 + 16):
            for x in range(bx * 16, bx * 16 + 16):
                bottom[y][x] = None
    both = bottom + top
    both = [[None if p is None else artlib.gba_colour(p) for p in row] for row in both]
    if len(artlib.colours(both)) > 15:
        print("  the piece has %d colours; merging the closest down to 15" % len(artlib.colours(both)))
        both = artlib.reduce_colours(both, 15)
    bottom, top = both[:h], both[h:]
    used = ts.used_slots()
    pal, mapping, new_pal = fit_palette(ts, artlib.colours(both), used)
    if pal is None:
        raise SystemExit(mapping)
    added_cols = sum(1 for c, s in mapping.items() if s not in used[pal])
    # tiles: colour indices; reuse identical tiles (this tileset's own, mirrored too)
    n_tiles = ts.tile_count()
    existing = [render_map.tile_pixels(ts.tiles, i) for i in range(n_tiles)]
    blank_tile = [[0] * 8 for _ in range(8)]
    new_tiles = []
    meta_entries = []
    bb, tb = split_tiles(bottom), split_tiles(top)
    order = sorted(bb, key=lambda k: (k[1], k[0]))
    floor_entries = None
    if floor_only:
        src = render_map.load_tileset(partner_primary(symbol) if ts.secondary and floor < NUM_PRIMARY_METATILES else symbol)[2]
        local = floor if floor < NUM_PRIMARY_METATILES else floor - NUM_PRIMARY_METATILES
        floor_entries = list(struct.unpack_from("<4H", src, local * 16))
    for key in order:
        entries = []
        for li, layer_tiles in enumerate((bb[key], tb[key])):
            if li == 0 and key in floor_only:
                entries += floor_entries
                continue
            for t in layer_tiles:
                idx = [[0 if p is None else mapping[p] for p in row] for row in t]
                if idx == blank_tile:
                    entries.append(0)  # tile 0 = empty, palette 0
                    continue
                hit = find_tile(ts, idx, existing + new_tiles)
                if hit is None:
                    new_tiles.append(idx)
                    hit = (len(existing) + len(new_tiles) - 1, 0, 0)
                tid = ts.tile_base + hit[0]
                entries.append(tid | (hit[1] << 10) | (hit[2] << 11) | (pal << 12))
        meta_entries.append(entries)
    first = ts.meta_base + ts.meta_count()
    print("%s + %s: %d blocks (%dx%d), %d new tiles (%d reused), palette %d (+%d colours)" % (
        symbol, os.path.basename(bottom_path), len(order), w // 16, h // 16, len(new_tiles),
        sum(1 for e in meta_entries for x in e if x) - len(new_tiles), pal, added_cols))
    if n_tiles + len(new_tiles) > ts.tile_max or ts.meta_count() + len(order) > ts.meta_max:
        raise SystemExit("  not enough room: tiles %d+%d of %d, blocks %d+%d of %d" % (
            n_tiles, len(new_tiles), ts.tile_max, ts.meta_count(), len(order), ts.meta_max))
    print("  new block numbers: 0x%03X-0x%03X (row by row, left to right)" % (first, first + len(order) - 1))
    if dry:
        print("  dry run: nothing changed")
        return first, len(order)
    # write: tiles.png grows by whole rows of 16 tiles
    per_row = ts.tiles.width // 8
    total = n_tiles + len(new_tiles)
    rows = (total + per_row - 1) // per_row
    img = Image.new("P", (ts.tiles.width, max(ts.tiles.height, rows * 8)), 0)
    img.putpalette(ts.tiles.getpalette())
    img.paste(ts.tiles, (0, 0))
    px = img.load()
    for i, t in enumerate(new_tiles):
        n = n_tiles + i
        x0, y0 = (n % per_row) * 8, (n // per_row) * 8
        for y in range(8):
            for x in range(8):
                px[x0 + x, y0 + y] = t[y][x]
    ts.tiles = img
    ts.pals[pal] = new_pal
    for entries in meta_entries:
        ts.metatiles += struct.pack("<8H", *entries)
        ts.attrs += struct.pack("<I", beh | (LAYERS[layer] << 29))
    ts.save()
    if label:
        add_labels(symbol, label, first, w // 16, h // 16)
    # preview: the piece rebuilt from the new blocks, beside the drawing
    primary = partner_primary(symbol) if ts.secondary else symbol
    r = render_map.Renderer(primary, symbol if ts.secondary else None)
    built = Image.new("RGBA", (w, h))
    for i, key in enumerate(order):
        built.paste(r.metatile(first + i), (key[0] * 16, key[1] * 16))
    sheet = artlib.label_sheet([("drawn", artlib.to_rgba(bottom, 1)), ("in the game's tiles", built.convert("RGBA"))], cols=2, scale=1)
    out = os.path.join(artlib.out_dir("tiles"), "%s_%s.png" % (symbol.replace("gTileset_", ""), label or os.path.basename(bottom_path)[:-4]))
    sheet.resize((sheet.width * 3, sheet.height * 3), Image.NEAREST).save(out)
    print("  preview ->", out)
    return first, len(order)


def add_labels(symbol, label, first, bw, bh):
    path = os.path.join(REPO, "include/constants/metatile_labels.h")
    text = open(path).read()
    name = symbol.replace("gTileset_", "")
    lines = ["// Research Red: %s (Art Kit tile_kit.py)" % label]
    for i in range(bw * bh):
        lines.append("#define METATILE_%s_%s_%d 0x%03X" % (name, label, i, first + i))
    marker = "// %s\n" % symbol
    block = "\n".join(lines) + "\n"
    if marker in text:
        i = text.index(marker) + len(marker)
        # after the existing labels of this tileset
        j = text.find("\n\n", i)
        j = len(text) if j < 0 else j + 1
        text = text[:j] + block + text[j:]
    else:
        end = text.rindex("#endif")
        text = text[:end] + marker + block + "\n" + text[end:]
    open(path, "w").write(text)
    print("  labels METATILE_%s_%s_0..%d -> include/constants/metatile_labels.h" % (name, label, bw * bh - 1))


# ---------------------------------------------------------------- place

def cmd_place(target, x, y, first, w, h, collision=1):
    layouts = json.load(open(os.path.join(REPO, "data/layouts/layouts.json")))["layouts"]
    lay_id = target if target.startswith("LAYOUT_") else render_map.find_map(target)["layout"]
    lay = next(l for l in layouts if l.get("id") == lay_id)
    path = os.path.join(REPO, lay["blockdata_filepath"])
    data = bytearray(open(path, "rb").read())
    for dy in range(h):
        for dx in range(w):
            xx, yy = x + dx, y + dy
            if not (0 <= xx < lay["width"] and 0 <= yy < lay["height"]):
                raise SystemExit("square %d,%d is outside the map (%dx%d)" % (xx, yy, lay["width"], lay["height"]))
            off = (yy * lay["width"] + xx) * 2
            old = struct.unpack_from("<H", data, off)[0]
            elevation = old & 0xF000
            struct.pack_into("<H", data, off, (first + dy * w + dx) | (collision << 10) | elevation)
    open(path, "wb").write(data)
    print("placed %dx%d blocks from 0x%03X at %d,%d in %s" % (w, h, first, x, y, lay_id))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cmd", choices=["info", "add", "place", "behaviors"])
    ap.add_argument("args", nargs="*")
    ap.add_argument("--top")
    ap.add_argument("--layer", choices=sorted(LAYERS), default="normal")
    ap.add_argument("--behavior", default="MB_NORMAL")
    ap.add_argument("--label")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--collision", type=int, default=1)
    ap.add_argument("--floor", type=lambda v: int(v, 0))
    a = ap.parse_args()
    if a.cmd == "info":
        cmd_info(a.args[0])
    elif a.cmd == "add":
        cmd_add(a.args[0], a.args[1], a.top, a.layer, a.behavior, a.label, a.dry_run, a.floor)
    elif a.cmd == "place":
        t, x, y, first, w, h = a.args
        cmd_place(t, int(x), int(y), int(first, 0), int(w), int(h), a.collision)
    elif a.cmd == "behaviors":
        for name, n in behaviors().items():
            if not a.args or a.args[0].upper() in name:
                print("  %3d  %s" % (n, name))
    return 0


if __name__ == "__main__":
    sys.exit(main())
