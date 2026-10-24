"""Research Red Art Kit: shrink big art into GBA-sized pixel art that doesn't
look shrunk.

A plain resize averages colours: edges go blurry, thin legs break into
jagged single pixels, and the result looks like a photo made small. This
does what a pixel artist does by hand, step by step:

  1. flatten the big picture into a few colour areas first (shading becomes
     flat bands, like pixel-art shading);
  2. shrink by vote: each small pixel takes the colour most of its area has
     (no blending), but dark line art wins where there's enough of it, so
     eyes and outlines survive;
  3. clean up: stray single pixels take their neighbours' colour, one-pixel
     holes inside the figure are filled (legs stay connected);
  4. thicken: limbs only one pixel wide get a second pixel;
  5. outline: a dark line around the outside, like the games' sprites;
  6. 15 colours (the GBA's limit) at the end.

  from pixelize import pixelize
  grid = pixelize(pil_rgba_image, out_w, out_h)

FireRed proportions (a bigger head, sturdier limbs) are made before
shrinking by fr_proportions().
"""
import os
import sys

from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import artlib  # noqa: E402

N4 = ((1, 0), (-1, 0), (0, 1), (0, -1))


def _is_dark(c):
    return c[0] * 3 + c[1] * 6 + c[2] < 900


def flatten(im, colours=28):
    """The big picture with its opaque colours reduced to a few flat areas."""
    alpha = im.getchannel("A")
    rgb = im.convert("RGB").quantize(colors=colours, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE).convert("RGB")
    out = rgb.convert("RGBA")
    out.putalpha(alpha)
    return out


def vote_shrink(im, ow, oh, keep_dark=0.30, coverage=0.45):
    """Each output pixel: the most common colour of its block (transparent if
    the block is mostly empty); dark line art wins when it covers keep_dark."""
    w, h = im.size
    px = im.load()
    sx, sy = w / ow, h / oh
    out = [[None] * ow for _ in range(oh)]
    for oy in range(oh):
        y0, y1 = int(oy * sy), max(int(oy * sy) + 1, int((oy + 1) * sy))
        for ox in range(ow):
            x0, x1 = int(ox * sx), max(int(ox * sx) + 1, int((ox + 1) * sx))
            counts, dark, total, opaque = {}, {}, 0, 0
            for y in range(y0, min(y1, h)):
                for x in range(x0, min(x1, w)):
                    p = px[x, y]
                    total += 1
                    if p[3] >= 128:
                        opaque += 1
                        c = p[:3]
                        counts[c] = counts.get(c, 0) + 1
                        if _is_dark(c):
                            dark[c] = dark.get(c, 0) + 1
            if not total or opaque < total * coverage:
                continue
            if dark and sum(dark.values()) >= opaque * keep_dark:
                out[oy][ox] = max(dark, key=dark.get)
            else:
                out[oy][ox] = max(counts, key=counts.get)
    return out


def cleanup(g):
    """Stray single pixels join their neighbours; one-pixel holes get filled."""
    h, w = len(g), len(g[0])
    out = [row[:] for row in g]
    for y in range(h):
        for x in range(w):
            nb = [g[y + dy][x + dx] for dx, dy in N4 if 0 <= x + dx < w and 0 <= y + dy < h]
            c = g[y][x]
            if c is None:
                filled = [n for n in nb if n is not None]
                if len(filled) >= 3:  # a hole inside the figure
                    out[y][x] = max(set(filled), key=filled.count)
            elif c not in nb:
                others = [n for n in nb if n is not None]
                if len(others) >= 3:  # a lone pixel: take the commonest neighbour
                    out[y][x] = max(set(others), key=others.count)
    return out


def thicken(g, from_row=0):
    """A part only one pixel wide (a leg, an arm) gets a second pixel next to it."""
    h, w = len(g), len(g[0])
    out = [row[:] for row in g]
    for y in range(from_row, h):
        for x in range(1, w - 1):
            c = g[y][x]
            if c is not None and g[y][x - 1] is None and g[y][x + 1] is None:
                # part of something running up and down (a limb), not a lone dot
                if (y > 0 and g[y - 1][x] is not None) or (y < h - 1 and g[y + 1][x] is not None):
                    side = x + 1 if (x + 2 >= w or g[y][x + 2] is None) else x - 1
                    out[y][side] = c
    return out


def outline_outside(g):
    """A dark outline around the figure, drawn in the empty pixels just outside
    it (each outline pixel a darker version of the colour it borders)."""
    h, w = len(g), len(g[0])
    out = [row[:] for row in g]
    for y in range(h):
        for x in range(w):
            if g[y][x] is not None:
                continue
            nb = [g[y + dy][x + dx] for dx, dy in N4 if 0 <= x + dx < w and 0 <= y + dy < h and g[y + dy][x + dx] is not None]
            if nb:
                c = min(nb, key=lambda c: sum(c))
                out[y][x] = (c[0] // 4, c[1] // 4, c[2] // 3)
    return out


def pixelize(im, ow, oh, colours=28, keep_dark=0.40, thick_from=0, outline=True):
    """Big RGBA picture -> ow x oh pixel grid (colours as (r,g,b), None = see-through).
    If outline is True the figure is shrunk to leave a pixel for it on each side."""
    im = flatten(im.convert("RGBA"), colours)
    inner_w, inner_h = (ow - 2, oh - 1) if outline else (ow, oh)
    g = vote_shrink(im, inner_w, inner_h, keep_dark)
    g = cleanup(g)
    g = thicken(g, thick_from)
    if outline:
        framed = artlib.blank(ow, oh)
        artlib.paste(framed, g, 1, 0)
        g = outline_outside(framed)
    g = [[None if p is None else artlib.gba_colour(p) for p in row] for row in g]
    if len(artlib.colours(g)) > 15:
        g = artlib.reduce_colours(g, 15)
    return g


def fr_proportions(im, head_box, head_scale=1.3, widen=1.15):
    """FireRed trainer proportions from a full-body picture: the head drawn
    bigger (standing on the same chin line) and the whole figure a little
    wider, which also makes thin limbs sturdier. head_box = (x0, y0, x1, y1)
    of the head in the picture. Returns a new RGBA picture (cropped)."""
    im = im.convert("RGBA")
    hx0, hy0, hx1, hy1 = head_box
    head = im.crop(head_box)
    big = head.resize((int(head.width * head_scale), int(head.height * head_scale)), Image.LANCZOS)
    extra_top = big.height - head.height
    canvas = Image.new("RGBA", (im.width + big.width, im.height + extra_top), (0, 0, 0, 0))
    ox = big.width // 2
    canvas.alpha_composite(im, (ox, extra_top))
    # the big head keeps its chin where the old chin was, centred on the old head
    cx = ox + (hx0 + hx1) // 2
    canvas.alpha_composite(big, (cx - big.width // 2, extra_top + hy1 - big.height))
    canvas = canvas.crop(canvas.getbbox())
    if widen != 1.0:
        canvas = canvas.resize((int(canvas.width * widen), canvas.height), Image.LANCZOS)
    return canvas
