#!/usr/bin/env python3
"""Research Red Art Kit: battle pictures (trainers and Pokemon).

  python3 tools/research_red/art/battle_kit.py check FILE.png [--kind K]
      Is it a valid GBA picture? Size, at most 16 colours, colour 0 = the
      see-through background. Kinds: trainer (64x64), mon (64x64 or 64x128:
      two frames), back (64x64), trainer_back (64 x 64*frames), icon (32x64),
      portrait (64x64), call (32x32). Without --kind it guesses from the size.
  python3 tools/research_red/art/battle_kit.py fix IN.png OUT.png --kind K
      Make any picture (full colour, any size) into a valid one: shrinks it
      with hard pixels if needed, stands it on the bottom edge, merges colours
      down to 15, puts the background at colour 0.
  python3 tools/research_red/art/battle_kit.py shiny DIR [--hue 0.5] [--only "#hex,..."]
      A shiny palette for a Pokemon folder (graphics/pokemon/<name>): turns the
      colours round the colour wheel (or only the listed ones) and writes
      shiny.pal next to normal.pal, plus a normal-vs-shiny preview.
  python3 tools/research_red/art/battle_kit.py anim FRONT.png [--squash 2]
      A second front frame for a new Pokemon (the engine shows frame 2 when it
      enters): the body squashed down a little, as many Gen 3 sprites do.
      Writes FRONT as a 64x128 two-frame sheet and an animated GIF preview.
  python3 tools/research_red/art/battle_kit.py backpic POSE.png OUT.png [--template em:red]
      A trainer back sprite: FireRed plays 5 frames when the trainer throws a
      Poke Ball. Makes frames 2-5 by following how FireRed's Red ("em:red",
      default) moves in each frame: POSE leans and slides the same way
      (--mode slide, default). --mode transfer copies the motion pixel by
      pixel like the walk maker; it breaks up here because the whole body
      turns, so only try it for small moves. Template names: em:<file> from
      Emerald's back_pics folder (red, leaf, brendan, may, wally, steven).
      A proper throwing arm still needs drawing by hand.
  python3 tools/research_red/art/battle_kit.py gif FILE.png [--frame 64x64] [--ms 200]
      Any sheet of frames as an animated GIF (previews folder).

Previews go to E:\\Workspace\\romhacks\\research-red\\art\\toolkit\\battle\\.
"""
import argparse
import os
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import artlib  # noqa: E402
import walkgen  # noqa: E402

KINDS = {  # kind: (width, height or list of allowed heights)
    "trainer": (64, [64]),
    "mon": (64, [64, 128]),
    "back": (64, [64]),
    "trainer_back": (64, None),     # any multiple of 64
    "icon": (32, [64]),
    "portrait": (64, [64]),
    "call": (32, [32]),
}


def guess_kind(w, h):
    if (w, h) == (32, 64):
        return "icon"
    if (w, h) == (32, 32):
        return "call"
    if w == 64 and h == 128:
        return "mon"
    if w == 64 and h > 128 and h % 64 == 0:
        return "trainer_back"
    return "trainer"


def check(path, kind=None, quiet=False):
    """List of problems (empty = fine)."""
    im = Image.open(path)
    w, h = im.size
    kind = kind or guess_kind(w, h)
    kw, kh = KINDS[kind]
    problems = []
    if w != kw or (kh and h not in kh) or (kh is None and h % 64):
        problems.append("size %dx%d; a %s picture is %dx%s" % (w, h, kind, kw, "/".join(map(str, kh)) if kh else "64*frames"))
    if im.mode != "P":
        problems.append("not indexed (mode %s): save as an indexed / 16-colour PNG" % im.mode)
    else:
        used = sorted(set(im.getdata()))
        if used and used[-1] > 15:
            problems.append("uses palette entries above 15 (%d colours)" % len(used))
        # bodies often touch the bottom edge, so only the top corners must be background
        if {im.getpixel((0, 0)), im.getpixel((w - 1, 0))} != {0}:
            problems.append("the top corners are not colour 0, so the background may not be see-through")
    if not quiet:
        print("%s: %s (%s)" % (path, "OK" if not problems else "; ".join(problems), kind))
    return problems


def fix(src, dst, kind):
    kw, kh = KINDS[kind]
    target_h = kh[0] if kh else 64
    grid = artlib.load_grid(src)
    im = artlib.to_rgba(grid)
    box = im.getbbox()
    if box:
        im = im.crop(box)
    if im.width > kw or im.height > target_h:
        k = min(kw / im.width, target_h / im.height)
        im = im.resize((max(1, int(im.width * k)), max(1, int(im.height * k))), Image.NEAREST)
    canvas = Image.new("RGBA", (kw, target_h), (0, 0, 0, 0))
    canvas.alpha_composite(im, ((kw - im.width) // 2, target_h - im.height))
    g = [[None if canvas.getpixel((x, y))[3] < 128 else canvas.getpixel((x, y))[:3] for x in range(kw)] for y in range(target_h)]
    g = [[None if p is None else artlib.gba_colour(p) for p in row] for row in g]
    if len(artlib.colours(g)) > 15:
        g = artlib.reduce_colours(g, 15)
    artlib.save_indexed(g, dst)
    check(dst, kind)


def read_pal(path):
    lines = open(path).read().split()
    n = int(lines[2])
    v = list(map(int, lines[3:3 + n * 3]))
    return [tuple(v[i:i + 3]) for i in range(0, n * 3, 3)]


def shiny(folder, hue=0.5, only=None):
    pal = read_pal(os.path.join(folder, "normal.pal"))
    only = {artlib.parse_hex(c) for c in only.split(",")} if only else None
    new = [pal[0]]
    for c in pal[1:]:
        if only is not None and c not in only:
            new.append(c)
            continue
        h, l, s = artlib.hls(c)
        new.append(artlib.gba_colour(artlib.from_hls(h + hue, l, s)))
    with open(os.path.join(folder, "shiny.pal"), "w", newline="\r\n") as f:
        f.write("JASC-PAL\n0100\n%d\n" % len(new))
        for r, g, b in new:
            f.write("%d %d %d\n" % (r, g, b))
    front = Image.open(os.path.join(folder, "anim_front.png"))
    a = front.crop((0, 0, 64, 64)).convert("RGBA")
    b = front.crop((0, 0, 64, 64))
    flat = []
    for c in new:
        flat += list(c)
    b.putpalette(flat + [0] * (768 - len(flat)))
    b = b.convert("RGBA")
    sheet = artlib.label_sheet([("normal", artlib.load_grid_from_image(a)), ("shiny", artlib.load_grid_from_image(b))], cols=2, scale=3)
    out = os.path.join(artlib.out_dir("battle"), os.path.basename(folder.rstrip("/")) + "_shiny.png")
    sheet.save(out)
    print("shiny.pal written; preview ->", out)


def anim(path, squash=2):
    grid = artlib.load_grid(path)
    f1 = artlib.crop(grid, 0, 0, 64, 64)
    im = artlib.to_rgba(f1)
    box = im.getbbox()
    f2 = artlib.blank(64, 64)
    if box:
        body = im.crop(box)
        squashed = body.resize((body.width + squash, max(1, body.height - squash)), Image.NEAREST)
        canvas = Image.new("RGBA", (64, 64), (0, 0, 0, 0))
        canvas.alpha_composite(squashed, (max(0, box[0] - squash // 2), box[3] - squashed.height))
        f2 = artlib.load_grid_from_image(canvas)
    sheet = f1 + f2
    pal = artlib.colours(sheet)
    artlib.save_indexed(sheet, path, pal[:15])
    out = os.path.join(artlib.out_dir("battle"), os.path.basename(os.path.dirname(os.path.abspath(path))) + "_anim.gif")
    artlib.save_gif([f1, f2, f1, f2, f1], out, [300, 120, 120, 120, 600], scale=3)
    print("two-frame sheet written to", path, "; preview ->", out)


def _head(f):
    """Centre x and top y of the head (the top 14 rows of the figure): the
    arm swings wide in a throw, the head shows how the body really moves."""
    ys = [y for y, row in enumerate(f) if any(p is not None for p in row)]
    if not ys:
        return 0, 0
    top = ys[0]
    xs = [x for y in range(top, min(top + 14, len(f))) for x, p in enumerate(f[y]) if p is not None]
    return sum(xs) // len(xs), top


def backpic(pose_path, out_path, template="em:red", mode="slide"):
    pose = artlib.crop(artlib.load_grid(pose_path), 0, 0, 64, 64)
    # "em:red" = FireRed's real throwing Red from the Emerald reference copy
    # (ours are the still portrait pair); a plain name = our back_pics folder
    if template.startswith("em:"):
        tpath = os.path.join(artlib.EMERALD_REF, "graphics/trainers/back_pics/%s.png" % template[3:])
    else:
        tpath = os.path.join(artlib.REPO, "graphics/trainers/back_pics/%s_back_pic.png" % template)
    t = artlib.load_grid(tpath)
    tf = [artlib.crop(t, 0, i * 64, 64, 64) for i in range(len(t) // 64)]
    if mode == "transfer":
        # experimental: per-pixel motion copy; fine for small moves, breaks up
        # when the whole body turns (as in FireRed's throw)
        frames = [pose] + [walkgen.transfer(tf[0], tf[i], pose, start=0, rigid_to=0, reach=6) for i in range(1, len(tf))]
    else:
        # slide: follow how far the template's head moves each frame, so the
        # throw reads as a lean forward and back
        h0 = _head(tf[0])
        frames = [pose]
        for f in tf[1:]:
            h = _head(f)
            dx, dy = h[0] - h0[0], h[1] - h0[1]
            moved = artlib.blank(64, 64)
            artlib.paste(moved, pose, dx, dy)
            frames.append(moved)
    sheet = [row for f in frames for row in f]
    if len(artlib.colours(sheet)) > 15:
        sheet = artlib.reduce_colours(sheet, 15)
    artlib.save_indexed(sheet, out_path)
    frames = [artlib.crop(sheet, 0, i * 64, 64, 64) for i in range(len(frames))]
    out = os.path.join(artlib.out_dir("battle"), os.path.basename(out_path)[:-4] + "_throw.gif")
    artlib.save_gif(frames, out, [400] + [110] * (len(frames) - 1), scale=3)
    items = [("template %d" % i, f) for i, f in enumerate(tf)] + [("made %d" % i, f) for i, f in enumerate(frames)]
    artlib.label_sheet(items, cols=len(tf), scale=2).save(out[:-4] + ".png")
    print("%d-frame back pic -> %s; preview -> %s" % (len(frames), out_path, out))


def gif(path, frame="64x64", ms=200):
    fw, fh = map(int, frame.split("x"))
    grid = artlib.load_grid(path)
    frames = [artlib.crop(grid, x, y, fw, fh) for y in range(0, len(grid), fh) for x in range(0, len(grid[0]), fw)]
    out = os.path.join(artlib.out_dir("battle"), os.path.basename(path)[:-4] + ".gif")
    artlib.save_gif(frames, out, [ms] * len(frames), scale=3)
    print("%d frames -> %s" % (len(frames), out))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cmd", choices=["check", "fix", "shiny", "anim", "backpic", "gif"])
    ap.add_argument("paths", nargs="+")
    ap.add_argument("--kind", choices=sorted(KINDS))
    ap.add_argument("--hue", type=float, default=0.5)
    ap.add_argument("--only")
    ap.add_argument("--squash", type=int, default=2)
    ap.add_argument("--template", default="em:red")
    ap.add_argument("--mode", choices=["slide", "transfer"], default="slide")
    ap.add_argument("--frame", default="64x64")
    ap.add_argument("--ms", type=int, default=200)
    a = ap.parse_args()
    if a.cmd == "check":
        bad = sum(1 for p in a.paths if check(p, a.kind))
        return 1 if bad else 0
    if a.cmd == "fix":
        fix(a.paths[0], a.paths[1], a.kind or "trainer")
    elif a.cmd == "shiny":
        shiny(a.paths[0], a.hue, a.only)
    elif a.cmd == "anim":
        anim(a.paths[0], a.squash)
    elif a.cmd == "backpic":
        backpic(a.paths[0], a.paths[1], a.template, a.mode)
    elif a.cmd == "gif":
        gif(a.paths[0], a.frame, a.ms)
    return 0


if __name__ == "__main__":
    sys.exit(main())
