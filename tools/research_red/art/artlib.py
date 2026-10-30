"""Research Red Art Kit: shared helpers for the art tools.

Everything the GBA wants from a picture lives here, so each tool stays short:
  - indexed images: up to 16 colours, colour 0 = transparent
  - overworld sheets: 16x32 frames side by side, in FireRed's frame order
  - previews: pictures blown up with a checkerboard, contact sheets, GIFs
  - colour maths: recolouring a group of shades ("a ramp") to a new colour
    while keeping its light and dark steps

Needs Pillow (Ubuntu package python3-pil). No other modules.
"""
import colorsys
import os

from PIL import Image, ImageDraw

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
# Windows-side output folder for previews (E:\Workspace\romhacks\research-red\art\toolkit);
# RR_TOOLKIT_OUT overrides it (the GitHub Action writes its previews elsewhere)
TOOLKIT_OUT = os.environ.get("RR_TOOLKIT_OUT", "/mnt/e/Workspace/romhacks/research-red/art/toolkit")
PEOPLE = os.path.join(REPO, "graphics/object_events/pics/people")
# pret's pokeemerald, sparse copy (people + trainer pics), kept outside the repo
EMERALD_REF = os.path.expanduser("~/romhacks/_tools/pokeemerald-ref")
# bases borrowed from outside the repo are copied here so the build never needs EMERALD_REF
OW_DIR = os.path.join(REPO, "graphics/research_red/overworld")
REF_ART = "/mnt/e/Workspace/romhacks/research-red/art/ref/ResearchRed_ref_art"
# Which Art Kit pieces are in the game: graphics/research_red/art_kit.toml is the
# asset tracker (every piece, its stage) and the gate (only "approved" pieces go
# in; tools/research_red/art/art_gate.py apply). Off = drafts only.
ART_KIT_SETTINGS = os.path.join(REPO, "graphics/research_red/art_kit.toml")
DRAFTS_DIR = os.path.join(REPO, "graphics/research_red/drafts")
# what the game had before any Art Kit piece went in (put back when a piece is taken out)
PLACEHOLDER_DIR = os.path.join(REPO, "graphics/research_red/placeholder")
STAGES = ["placeholder", "draft", "hand-finished", "approved"]


def load_pieces():
    """{piece id: {what, where, game_now, stage, slice, notes}} from art_kit.toml."""
    import tomllib
    with open(ART_KIT_SETTINGS, "rb") as f:
        return tomllib.load(f).get("pieces", {})


def approved(piece):
    """True when the owner has let this piece into the game."""
    return load_pieces().get(piece, {}).get("stage") == "approved"


def in_game(piece):
    """True when the game uses this piece: approved, or switched on for testing
    (testing = true; not an approval, art_gate.py test ID off puts the original back)."""
    v = load_pieces().get(piece, {})
    return v.get("stage") == "approved" or v.get("testing") is True

# FireRed overworld frame order (16x32 people). Right-facing frames are the
# left ones mirrored by the game, so they are never drawn.
FRAME_NAMES = ["down", "up", "left",
               "down_walk1", "down_walk2", "up_walk1", "up_walk2", "left_walk1", "left_walk2"]
STAND_OF = {3: 0, 4: 0, 5: 1, 6: 1, 7: 2, 8: 2}  # walk frame -> its standing frame

TRANSPARENT = None  # a pixel value meaning "see-through" in our pixel grids


def out_dir(*parts):
    """A folder under the Windows toolkit folder (made if missing)."""
    path = os.path.join(TOOLKIT_OUT, *parts)
    os.makedirs(path, exist_ok=True)
    return path


# ---------------------------------------------------------------- pixel grids
# A "grid" is a list of rows, each a list of (r, g, b) tuples or None.

def load_grid(path):
    """Any PNG -> grid. Palette index 0 (indexed PNGs) or alpha < 128 = None."""
    im = Image.open(path)
    if im.mode == "P":
        pal = im.getpalette()
        px = im.load()
        return [[None if px[x, y] == 0 else tuple(pal[px[x, y] * 3:px[x, y] * 3 + 3])
                 for x in range(im.width)] for y in range(im.height)]
    im = im.convert("RGBA")
    px = im.load()
    return [[None if px[x, y][3] < 128 else px[x, y][:3] for x in range(im.width)] for y in range(im.height)]


def load_grid_from_image(im):
    """A PIL image (any mode) -> grid, alpha < 128 = None."""
    im = im.convert("RGBA")
    px = im.load()
    return [[None if px[x, y][3] < 128 else px[x, y][:3] for x in range(im.width)] for y in range(im.height)]


def blank(w, h):
    return [[None] * w for _ in range(h)]


def crop(grid, x, y, w, h):
    return [row[x:x + w] for row in grid[y:y + h]]


def paste(dst, src, x, y, keep_transparent=True):
    """Copy src onto dst at x, y. Transparent src pixels leave dst alone."""
    for yy, row in enumerate(src):
        for xx, p in enumerate(row):
            if 0 <= y + yy < len(dst) and 0 <= x + xx < len(dst[0]):
                if p is not None or not keep_transparent:
                    dst[y + yy][x + xx] = p


def mirror(grid):
    return [list(reversed(r)) for r in grid]


def colours(grid):
    """The distinct opaque colours, most used first."""
    count = {}
    for row in grid:
        for p in row:
            if p is not None:
                count[p] = count.get(p, 0) + 1
    return sorted(count, key=lambda c: -count[c])


def grid_to_image(grid, palette=None):
    """Grid -> indexed PIL image, index 0 transparent (shown as pure green
    #00FF00, the project's key colour). palette: list of (r,g,b) to use for
    indices 1.., else built from the grid. More than 15 colours is an error."""
    if palette is None:
        palette = colours(grid)
    palette = list(palette)
    if len(palette) > 15:
        raise ValueError("%d colours; the GBA allows 15 plus transparent" % len(palette))
    index = {c: i + 1 for i, c in enumerate(palette)}
    h, w = len(grid), len(grid[0])
    im = Image.new("P", (w, h), 0)
    flat = [0, 255, 0]
    for c in palette:
        flat += list(c)
    flat += [0, 0, 0] * (256 - len(flat) // 3)
    im.putpalette(flat)
    px = im.load()
    for y in range(h):
        for x in range(w):
            p = grid[y][x]
            px[x, y] = 0 if p is None else index[p]
    return im


def save_indexed(grid, path, palette=None):
    os.makedirs(os.path.dirname(path) or ".", exist_ok=True)
    grid_to_image(grid, palette).save(path)


def write_jasc_pal(palette, path):
    """A JASC .pal (16 colours, index 0 = the transparent green), which the
    build turns into a .gbapal."""
    cols = [(0, 255, 0)] + list(palette)
    cols += [(0, 0, 0)] * (16 - len(cols))
    with open(path, "w", newline="\r\n") as f:
        f.write("JASC-PAL\n0100\n16\n")
        for r, g, b in cols:
            f.write("%d %d %d\n" % (r, g, b))


def gba_colour(c):
    """Round a colour to what the GBA can show (5 bits per channel)."""
    return tuple((v >> 3) * 255 // 31 for v in c)


# ---------------------------------------------------------------- overworld sheets

def sheet_frames(grid, fw=16, fh=32):
    n = len(grid[0]) // fw
    return [crop(grid, i * fw, 0, fw, fh) for i in range(n)]


def join_frames(frames):
    fh, fw = len(frames[0]), len(frames[0][0])
    out = blank(fw * len(frames), fh)
    for i, f in enumerate(frames):
        paste(out, f, i * fw, 0, keep_transparent=False)
    return out


def sheet_path(name):
    """Where a base sheet lives. Names:
      scientist        FireRed's graphics/object_events/pics/people/scientist.png
      em:mom           Emerald's people/mom.png (copied into the repo on first use)
      em:gym_leaders/flannery  Emerald subfolders work too
      rr:elm           one of ours, graphics/research_red/overworld/elm.png
      some/file.png    any PNG path (relative to the repo)"""
    if name.startswith("em:"):
        rel = name[3:]
        local = os.path.join(OW_DIR, "bases", "em_" + rel.replace("/", "_") + ".png")
        if not os.path.exists(local):
            src = os.path.join(EMERALD_REF, "graphics/object_events/pics/people", rel + ".png")
            if not os.path.exists(src):
                raise SystemExit("no Emerald sheet %s (is %s there?)" % (rel, EMERALD_REF))
            os.makedirs(os.path.dirname(local), exist_ok=True)
            import shutil
            shutil.copy(src, local)
        return local
    if name.startswith("rr:"):
        return os.path.join(OW_DIR, name[3:] + ".png")
    if name.endswith(".png"):
        return os.path.join(REPO, name)
    return os.path.join(PEOPLE, name + ".png")


def people_sheet(name):
    """A people sheet by name (see sheet_path)."""
    return load_grid(sheet_path(name))


# ---------------------------------------------------------------- previews

def to_rgba(grid, scale=1, bg=None):
    """Grid -> RGBA image, scaled with hard pixels. bg: 'checker', a colour, or None."""
    h, w = len(grid), len(grid[0])
    im = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    px = im.load()
    for y in range(h):
        for x in range(w):
            p = grid[y][x]
            if p is not None:
                px[x, y] = p + (255,)
    im = im.resize((w * scale, h * scale), Image.NEAREST)
    if bg is None:
        return im
    base = Image.new("RGBA", im.size, (0, 0, 0, 255))
    if bg == "checker":
        d = ImageDraw.Draw(base)
        s = max(4, scale * 4)
        for yy in range(0, im.height, s):
            for xx in range(0, im.width, s):
                shade = (205, 205, 205) if (xx // s + yy // s) % 2 else (235, 235, 235)
                d.rectangle([xx, yy, xx + s - 1, yy + s - 1], fill=shade + (255,))
    else:
        base.paste(tuple(bg) + (255,), [0, 0, im.width, im.height])
    base.alpha_composite(im)
    return base


def label_sheet(items, cols=8, scale=3, pad=6, label_h=12, bg=(60, 60, 70)):
    """Contact sheet. items: list of (label, grid or RGBA image)."""
    tiles = [(lab, g if isinstance(g, Image.Image) else to_rgba(g, scale, "checker")) for lab, g in items]
    if not tiles:
        return Image.new("RGBA", (10, 10))
    cw = max(t.width for _, t in tiles) + pad
    ch = max(t.height for _, t in tiles) + pad + label_h
    rows = (len(tiles) + cols - 1) // cols
    sheet = Image.new("RGBA", (cw * min(cols, len(tiles)) + pad, ch * rows + pad), bg + (255,))
    d = ImageDraw.Draw(sheet)
    for i, (lab, t) in enumerate(tiles):
        x, y = pad + (i % cols) * cw, pad + (i // cols) * ch
        sheet.alpha_composite(t, (x, y + label_h))
        d.text((x, y), lab[:cw // 6], fill=(255, 255, 255, 255))
    return sheet


def save_gif(frames, path, durations, scale=4, bg=(120, 180, 120)):
    """Animated GIF from grids (each frame drawn on a flat background)."""
    ims = [to_rgba(f, scale, bg).convert("RGB") for f in frames]
    os.makedirs(os.path.dirname(path), exist_ok=True)
    ims[0].save(path, save_all=True, append_images=ims[1:], duration=durations, loop=0, disposal=2)


def walk_strip(frames):
    """The frames the game plays for walking down, up, left, right (right = left
    mirrored): 4 loops of stand, step 1, stand, step 2. Returns (grids, ms)."""
    seq = []
    for stand, a, b, flip in ((0, 3, 4, False), (1, 5, 6, False), (2, 7, 8, False), (2, 7, 8, True)):
        for _ in range(2):
            for i in (a, stand, b, stand):
                f = frames[i]
                seq.append(mirror(f) if flip else f)
    return seq, [130] * len(seq)


def run_strip(run9):
    """Like walk_strip for the player's 9 running frames (south, north, west
    in groups of pose, step, step)."""
    seq = []
    for d, flip in ((0, False), (1, False), (2, False), (2, True)):
        pose, a, b = run9[d * 3], run9[d * 3 + 1], run9[d * 3 + 2]
        for _ in range(2):
            for f in (pose, a, pose, b):
                seq.append(mirror(f) if flip else f)
    return seq, [85] * len(seq)


# ---------------------------------------------------------------- colour maths

def hls(c):
    return colorsys.rgb_to_hls(*(v / 255 for v in c))


def from_hls(h, l, s):
    return tuple(max(0, min(255, round(v * 255))) for v in colorsys.hls_to_rgb(h % 1.0, max(0, min(1, l)), max(0, min(1, s))))


def parse_hex(s):
    s = s.strip().lstrip("#")
    return tuple(int(s[i:i + 2], 16) for i in (0, 2, 4))


def hexc(c):
    return "#%02x%02x%02x" % c


def recolour_ramp(base_cols, target, contrast=1.0, anchor=None):
    """Map a group of shades to a new colour, keeping the steps between them.
    The middle shade of the group (or `anchor`, one of base_cols: use the
    shade most of the area is drawn in) lands on the target; lighter and
    darker shades keep their distance (times contrast). Returns {old: new}."""
    th, tl, ts = hls(target)
    ls = sorted(hls(c)[1] for c in base_cols)
    mid = hls(anchor)[1] if anchor else ls[len(ls) // 2]
    out = {}
    for c in base_cols:
        _, l, s = hls(c)
        # keep a little of each shade's own saturation so dark outlines stay dark
        out[c] = gba_colour(from_hls(th, tl + (l - mid) * contrast, ts * (0.6 + 0.4 * min(1, s / max(ts, 0.01)))))
    return out


def apply_map(grid, cmap):
    return [[cmap.get(p, p) if p is not None else None for p in row] for row in grid]


def dist2(a, b):
    # weighted RGB distance (cheap and good enough for picking near colours)
    dr, dg, db = a[0] - b[0], a[1] - b[1], a[2] - b[2]
    return 2 * dr * dr + 4 * dg * dg + 3 * db * db


def reduce_colours(grid, n=15, keep=()):
    """Merge the closest colours until at most n remain (keep = never merged away)."""
    cols = colours(grid)
    count = {}
    for row in grid:
        for p in row:
            if p is not None:
                count[p] = count.get(p, 0) + 1
    cmap = {c: c for c in cols}
    live = list(cols)
    while len(live) > n:
        best = None
        for i in range(len(live)):
            for j in range(i + 1, len(live)):
                d = dist2(live[i], live[j])
                if best is None or d < best[0]:
                    best = (d, i, j)
        _, i, j = best
        a, b = live[i], live[j]
        # the rarer colour (and never a kept one) goes
        if b in keep or (a not in keep and count.get(a, 0) < count.get(b, 0)):
            a, b = b, a
        live.remove(b)
        count[a] = count.get(a, 0) + count.get(b, 0)
        for k, v in cmap.items():
            if v == b:
                cmap[k] = a
    return apply_map(grid, cmap)


def dominant_colours(grid, n=12):
    """A small palette that sums up a big picture (for reading colours off ref art)."""
    pts = [p for row in grid for p in row if p is not None]
    im = Image.new("RGB", (len(pts), 1))
    im.putdata(pts)
    q = im.quantize(colors=n, method=Image.Quantize.MEDIANCUT)
    pal = q.getpalette()[:n * 3]
    counts = sorted(q.getcolors(), reverse=True)
    return [(tuple(pal[i * 3:i * 3 + 3]), c) for c, i in counts]
