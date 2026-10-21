#!/usr/bin/env python3
"""Research Red: draw a map (or one tileset's metatiles) as a PNG, from the
decomp's own data, for planning scenes and map edits.

  python3 tools/research_red/render_map.py MAP_PALLET_TOWN_RIVALS_HOUSE out.png [--grid] [--objects]
  python3 tools/research_red/render_map.py LAYOUT_PALLET_TOWN_RIVALS_HOUSE out.png [--grid]
  python3 tools/research_red/render_map.py --metatiles gTileset_GenericBuilding2 out.png

--grid writes each tile's x,y; --objects marks the map's people (local id
initials). --metatiles draws every metatile of a tileset with its number (hex),
to find a tile to place (e.g. a chair facing up).
FireRed layout: 640 primary tiles / metatiles, 7 primary palettes; metatiles are
8 tile entries (bottom layer 4, top layer 4).
"""
import glob
import json
import os
import re
import struct
import sys

from PIL import Image, ImageDraw  # python3-pil (see docs/DEV_GUIDE.md)

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
NUM_PRIMARY_TILES = 640
NUM_PRIMARY_METATILES = 640
NUM_PRIMARY_PALS = 7


def tileset_dir(symbol):
    """gTileset_GenericBuilding2 -> data/tilesets/secondary/generic_building_2"""
    headers = open(os.path.join(REPO, "src/data/tilesets/headers.h")).read()
    m = re.search(r"const struct Tileset %s =\s*\{(.*?)\};" % re.escape(symbol), headers, re.S)
    metatiles_sym = re.search(r"\.metatiles = (\w+)", m.group(1)).group(1)
    metatiles = open(os.path.join(REPO, "src/data/tilesets/metatiles.h")).read()
    path = re.search(r"%s\[\] = INCBIN_U16\(\"(data/tilesets/[^\"]+)/metatiles" % re.escape(metatiles_sym), metatiles).group(1)
    return os.path.join(REPO, path)


def load_tileset(symbol):
    d = tileset_dir(symbol)
    tiles = Image.open(os.path.join(d, "tiles.png"))
    pals = []
    for i in range(16):
        p = os.path.join(d, "palettes", f"{i:02d}.pal")
        cols = []
        if os.path.exists(p):
            lines = open(p).read().split("\n")[3:19]
            cols = [tuple(int(v) for v in ln.split()) for ln in lines if ln.strip()]
        pals.append(cols + [(0, 0, 0)] * (16 - len(cols)))
    metatiles = open(os.path.join(d, "metatiles.bin"), "rb").read()
    return tiles, pals, metatiles


def tile_pixels(tiles_img, index):
    """8x8 colour indices of tile `index` from an indexed tiles.png"""
    per_row = tiles_img.width // 8
    x, y = (index % per_row) * 8, (index // per_row) * 8
    if y >= tiles_img.height:
        return [[0] * 8 for _ in range(8)]
    px = tiles_img.load()
    return [[px[x + i, y + j] % 16 for i in range(8)] for j in range(8)]


class Renderer:
    def __init__(self, primary, secondary):
        self.p = load_tileset(primary)
        self.s = load_tileset(secondary) if secondary else None

    def metatile(self, mid):
        img = Image.new("RGBA", (16, 16), (0, 0, 0, 0))
        src = self.p if mid < NUM_PRIMARY_METATILES or not self.s else self.s
        local = mid if src is self.p else mid - NUM_PRIMARY_METATILES
        data = src[2][local * 16:(local + 1) * 16]
        if len(data) < 16:
            return img
        px = img.load()
        for layer in range(2):
            for k in range(4):
                e = struct.unpack_from("<H", data, (layer * 4 + k) * 2)[0]
                tid, hf, vf, pal = e & 0x3FF, (e >> 10) & 1, (e >> 11) & 1, e >> 12
                tset = self.p if tid < NUM_PRIMARY_TILES or not self.s else self.s
                t = tile_pixels(tset[0], tid if tset is self.p else tid - NUM_PRIMARY_TILES)
                pal_src = self.p if pal < NUM_PRIMARY_PALS or not self.s else self.s
                colors = pal_src[1][pal]
                ox, oy = (k % 2) * 8, (k // 2) * 8
                for j in range(8):
                    for i in range(8):
                        c = t[7 - j if vf else j][7 - i if hf else i]
                        if c == 0 and (layer == 1 or True):
                            if layer == 0:
                                px[ox + i, oy + j] = colors[0] + (255,)
                            continue
                        px[ox + i, oy + j] = colors[c] + (255,)
        return img


def find_map(map_const):
    for p in glob.glob(os.path.join(REPO, "data/maps/*/map.json")):
        m = json.load(open(p))
        if m.get("id") == map_const:
            return m
    sys.exit(f"{map_const} not found")


def render_map(map_const, out, grid=False, objects=False):
    """map_const: a map (MAP_X, people can be marked) or a layout (LAYOUT_X)."""
    layouts = json.load(open(os.path.join(REPO, "data/layouts/layouts.json")))["layouts"]
    if map_const.startswith("LAYOUT_"):
        m = {"layout": map_const, "object_events": []}
    else:
        m = find_map(map_const)
    lay = next(l for l in layouts if l.get("id") == m["layout"])
    w, h = lay["width"], lay["height"]
    blocks = open(os.path.join(REPO, lay["blockdata_filepath"]), "rb").read()
    r = Renderer(lay["primary_tileset"], lay["secondary_tileset"])
    img = Image.new("RGBA", (w * 16, h * 16))
    cache = {}
    for y in range(h):
        for x in range(w):
            mid = struct.unpack_from("<H", blocks, (y * w + x) * 2)[0] & 0x3FF
            if mid not in cache:
                cache[mid] = r.metatile(mid)
            img.paste(cache[mid], (x * 16, y * 16))
    img = img.resize((w * 32, h * 32), Image.NEAREST)
    d = ImageDraw.Draw(img)
    if grid:
        for y in range(h):
            for x in range(w):
                d.text((x * 32 + 2, y * 32 + 1), f"{x},{y}", fill=(255, 255, 255, 255))
    if objects:
        for o in m.get("object_events", []):
            x, y = o["x"] * 32, o["y"] * 32
            d.rectangle((x + 2, y + 2, x + 29, y + 29), outline=(255, 0, 0, 255), width=2)
            label = o.get("local_id", "?").replace("LOCALID_", "")[-6:]
            d.text((x + 3, y + 18), label, fill=(255, 255, 0, 255))
    img.save(out)
    print(f"{out}: {w}x{h} tiles")


def render_metatiles(tileset, out):
    secondary = None
    r = Renderer(tileset, None)
    primary = re.search(r"isSecondary = TRUE", open(os.path.join(REPO, "src/data/tilesets/headers.h")).read()
                        .split(f"const struct Tileset {tileset} =")[1].split("};")[0])
    if primary:
        # draw a secondary tileset over the building primary set (any primary works for its own tiles)
        r = Renderer("gTileset_Building", tileset)
        base = NUM_PRIMARY_METATILES
        count = len(r.s[2]) // 16
    else:
        base, count = 0, len(r.p[2]) // 16
    cols = 16
    rows = (count + cols - 1) // cols
    img = Image.new("RGBA", (cols * 48, rows * 48), (40, 40, 40, 255))
    d = ImageDraw.Draw(img)
    for i in range(count):
        t = r.metatile(base + i).resize((32, 32), Image.NEAREST)
        x, y = (i % cols) * 48, (i // cols) * 48
        img.paste(t, (x + 8, y + 2), t)
        d.text((x + 8, y + 35), f"{base + i:03x}", fill=(255, 255, 255, 255))
    img.save(out)
    print(f"{out}: {count} metatiles from {base:#x}")


if __name__ == "__main__":
    a = sys.argv[1:]
    if not a or a[0] in ("-h", "--help"):
        print(__doc__)
    elif a[0] == "--metatiles":
        render_metatiles(a[1], a[2])
    else:
        render_map(a[0], a[1], grid="--grid" in a, objects="--objects" in a)
