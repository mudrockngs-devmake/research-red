#!/usr/bin/env python3
"""Research Red Art Kit: battle back pictures (the view from behind) from the
front-facing reference art. DRAFTS: meant to be finished by hand.

The reference art only shows the front, so the back is worked out:
  1. the head and upper body are cut from the front picture (FireRed back
     pictures show a trainer from about the waist up, close to the camera);
  2. from behind, the head is all hair: the face (skin inside the head) is
     painted over with the hair colour;
  3. front details (buttons, ties, faces) are flattened away by the
     pixel-art shrinker, and the figure is mirrored (left and right swap
     when you turn round);
  4. it's framed like FireRed's back pictures: big, standing on the bottom edge.

  python3 tools/research_red/art/back_maker.py ivy player_m player_f     # cast names
  python3 tools/research_red/art/back_maker.py --all

Writes graphics/research_red/cast/<name>/back.png (64x64, 16 colours). A
hand-drawn graphics/research_red/cast_handmade/<name>/back.png always wins;
a back-view drawing in the reference folder (<number>_<name>_back.png) is used
instead of guessing when there is one. Preview:
E:\\Workspace\\romhacks\\research-red\\art\\toolkit\\battle\\backs.png
"""
import glob
import os
import shutil
import sys

from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.dirname(HERE))
import artlib  # noqa: E402
import build_cast_art as cast  # noqa: E402
import pixelize  # noqa: E402

UPPER = 0.46       # how much of the figure (from the top of the head) a back picture shows
HEAD_SCALE = 1.25  # back pictures are close to the camera: the head reads bigger


def ref_for(name, back=False):
    for ref, n in cast.CAST.items():
        if n == name:
            p = os.path.join(cast.REF_DIR, ref + ("_back" if back else "") + ".png")
            if os.path.exists(p):
                return p
    return None


def to_image(rows, w, h):
    im = Image.new("RGBA", (w, h))
    im.putdata([p for r in rows for p in r])
    return im


def dist2(a, b):
    return sum((a[i] - b[i]) ** 2 for i in range(3))


def is_skin(c):
    """Warm, fairly light colours with some colour in them: skin of any tone."""
    import colorsys
    r, g, b = (v / 255 for v in c)
    hh, ss, vv = colorsys.rgb_to_hsv(r, g, b)
    return (hh < 0.12 or hh > 0.97) and 0.12 < ss < 0.75 and vv > 0.30 and r > b + 0.08


def paint_face_over(im, head_box):
    """Skin and face details inside the head become hair: the back of a head is hair."""
    x0, y0, x1, y1 = head_box
    px = im.load()
    w, h = im.size
    # hair: the commonest colour in the top of the head (a collar or a raised
    # hand can reach the lower part) that isn't skin, an outline or see-through
    counts = {}
    for y in range(max(0, y0), min(h, y0 + (y1 - y0) * 2 // 5)):
        for x in range(max(0, x0), min(w, x1)):
            p = px[x, y]
            if p[3] >= 128 and not is_skin(p[:3]) and sum(p[:3]) > 150:
                c = tuple(v // 12 * 12 for v in p[:3])
                counts[c] = counts.get(c, 0) + 1
    hair = max(counts, key=counts.get) if counts else (90, 60, 40)
    # the face: skin, plus the dark bits (eyes, brows, glasses) and the white
    # eye highlights between skin pixels; the box is widened a little because
    # big hair moves the measured head off centre
    pad = (x1 - x0) // 5
    for y in range(max(0, y0), min(h, y1 + (y1 - y0) // 4)):
        row = [px[x, y] for x in range(w)]
        skin_x = [x for x in range(max(0, x0 - pad), min(w, x1 + pad)) if row[x][3] >= 128 and is_skin(row[x][:3])]
        if not skin_x:
            continue
        lo, hi = min(skin_x), max(skin_x)
        for x in range(lo, hi + 1):
            p = row[x]
            if p[3] < 128:
                continue
            if is_skin(p[:3]) or sum(p[:3]) < 240 or sum(p[:3]) > 660:
                k = sum(p[:3]) / 600
                shade = 0.8 + 0.25 * min(1.0, k)
                px[x, y] = tuple(max(0, min(255, int(c * shade))) for c in hair) + (255,)
    return im


def make_back(name):
    hand = os.path.join(cast.HANDMADE_DIR, name, "back.png")
    out = os.path.join(cast.OUT_DIR, name, "back.png")
    if os.path.exists(hand):
        shutil.copyfile(hand, out)
        return artlib.load_grid(out), "hand-made"
    drawn = ref_for(name, back=True)
    path = drawn or ref_for(name)
    if not path:
        return None, "no ref art"
    w, h, rows = cast.load_ref(path)
    x0, y0, x1, y1, head_x, head_top = cast.figure_box(w, h, rows)
    if name in cast.HEAD_OVERRIDES:
        head_x, head_top = cast.HEAD_OVERRIDES[name]
    fh = y1 - y0
    im = to_image(rows, w, h)
    hw, hh = int(fh * cast.HEAD_FRAC_W), int(fh * cast.HEAD_FRAC_H)
    head_box = (head_x - hw // 2, head_top, head_x + hw // 2, head_top + hh)
    if not drawn:
        im = paint_face_over(im, head_box)
    im = pixelize.fr_proportions(im, head_box, head_scale=HEAD_SCALE, widen=1.1)
    # the upper part only, a square-ish bust around the head, then turned round
    im = im.crop((0, 0, im.width, int(im.height * UPPER)))
    im = im.crop(im.getbbox())
    bust_w = int(im.height * 1.0)
    # where the head ended up: the widest run of opaque pixels in the top rows
    top = [x for y in range(min(im.height, im.height // 5)) for x in range(im.width) if im.getpixel((x, y))[3] >= 128]
    cx = sum(top) // len(top) if top else im.width // 2
    left = max(0, min(im.width - bust_w, cx - bust_w // 2)) if im.width > bust_w else 0
    im = im.crop((left, 0, min(im.width, left + bust_w), im.height))
    if not drawn:
        im = im.transpose(Image.FLIP_LEFT_RIGHT)
    k = min(62 / im.width, 64 / im.height)
    ow, oh = max(4, int(im.width * k)), max(4, int(im.height * k))
    g = pixelize.pixelize(im, ow, oh, colours=20, keep_dark=0.55)
    canvas = artlib.blank(64, 64)
    artlib.paste(canvas, g, (64 - ow) // 2, 64 - oh)
    os.makedirs(os.path.dirname(out), exist_ok=True)
    artlib.save_indexed(canvas, out)
    return canvas, "from a back drawing" if drawn else "draft from the front art"


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0
    names = sorted(set(cast.CAST.values())) if argv == ["--all"] else argv
    items = []
    for n in names:
        g, how = make_back(n)
        print("  %-14s %s" % (n, how))
        if g:
            front = os.path.join(cast.OUT_DIR, n, "battle_fr.png")
            if os.path.exists(front):
                items.append((n + " front", artlib.load_grid(front)))
            items.append((n + " back (%s)" % how.split()[0], g))
    out = os.path.join(artlib.out_dir("battle"), "backs.png")
    artlib.label_sheet(items, cols=6, scale=2).save(out)
    print("preview ->", out)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
