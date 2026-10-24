"""Research Red Art Kit: make walking (and running) frames from standing ones.

How it works ("motion transfer"), in plain words:
  1. Pick a template: an existing sheet that already walks and whose standing
     body has a similar shape (or name one yourself).
  2. For every pixel of the template's walking frame, find where that pixel
     came from in the template's standing frame (it usually moved a pixel or
     two: a leg forward, an arm back). That gives a little "move map".
  3. Apply the same move map to OUR standing frame. Our colours, their motion.
People stand in rows 11-30 of their 32-pixel frame (hair ~11-16, face ~16-22,
body ~23-27, legs ~28-30). In FireRed's walks the whole figure bobs down a
pixel and the legs and arms swing; the move map picks all of that up.

Used by ow_maker.py; can also be run to check itself:
  python3 tools/research_red/art/walkgen.py selftest
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import artlib  # noqa: E402

HEAD_ROWS = 4       # rows 0-3 are always empty: never touched (hats may reach row 9)
RIGID_TO = 21       # rows above this (the head) move as one piece: only the bob
REACH = 2           # how far (pixels) a body part may move between frames

# run frames in the player's 20-frame sheet: 9-11 south (pose, step, step),
# 12-14 north, 15-17 west (not the walk order!), 18-19 spare
RUN_OFFSET = 9


def _patch_cost(stand, walk, sx, sy, wx, wy):
    """How badly the 3x3 area around stand(sx,sy) matches walk(wx,wy)."""
    h, w = len(stand), len(stand[0])
    cost = 0
    for oy in (-1, 0, 1):
        for ox in (-1, 0, 1):
            a = stand[sy + oy][sx + ox] if 0 <= sy + oy < h and 0 <= sx + ox < w else None
            b = walk[wy + oy][wx + ox] if 0 <= wy + oy < h and 0 <= wx + ox < w else None
            if a != b:
                cost += 3 if (ox == 0 and oy == 0) else 1
    return cost


def move_map(t_stand, t_walk):
    """For each pixel of the template walk frame: (dx, dy) to the pixel of the
    template stand frame it came from, or 'gone' / 'new' / None (keep)."""
    h, w = len(t_stand), len(t_stand[0])
    out = [[None] * w for _ in range(h)]
    for y in range(HEAD_ROWS, h):
        for x in range(w):
            if t_walk[y][x] == t_stand[y][x] and _patch_cost(t_stand, t_walk, x, y, x, y) == 0:
                continue  # unchanged pixel: keep ours
            if t_walk[y][x] is None:
                out[y][x] = "gone"
                continue
            best = None
            for dy in range(-REACH, REACH + 1):
                for dx in range(-REACH, REACH + 1):
                    sx, sy = x + dx, y + dy
                    if not (0 <= sx < w and 0 <= sy < h) or t_stand[sy][sx] != t_walk[y][x]:
                        continue
                    c = _patch_cost(t_stand, t_walk, sx, sy, x, y) * 4 + abs(dx) + abs(dy)
                    if best is None or c < best[0]:
                        best = (c, dx, dy)
            out[y][x] = (best[1], best[2]) if best else "new"
    return out


def colour_link(t_stand, o_stand):
    """Template colour -> our colour found at the same spots (for 'new' pixels)."""
    votes = {}
    for y in range(len(t_stand)):
        for x in range(len(t_stand[0])):
            a, b = t_stand[y][x], o_stand[y][x]
            if a is not None and b is not None:
                votes.setdefault(a, {}).setdefault(b, 0)
                votes[a][b] += 1
    return {a: max(v, key=v.get) for a, v in votes.items()}


def _near_own(o_stand, x, y, want, link_back):
    """Our colour for a pixel the template drew fresh: look around (x, y) in our
    standing frame for a colour that plays the same part as the template's."""
    h, w = len(o_stand), len(o_stand[0])
    best = None
    for r in (1, 2, 3):
        for dy in range(-r, r + 1):
            for dx in range(-r, r + 1):
                xx, yy = x + dx, y + dy
                if 0 <= xx < w and 0 <= yy < h and o_stand[yy][xx] is not None:
                    c = o_stand[yy][xx]
                    score = (0 if link_back.get(c) == want else 1, abs(dx) + abs(dy))
                    if best is None or score < best[0]:
                        best = (score, c)
        if best and best[0][0] == 0:
            break
    return best[1] if best else None


def head_bob(t_stand, t_walk):
    """How many pixels the template's head moved down in this frame (-1..2)."""
    best = None
    for dy in (0, 1, -1, 2):
        same = 0
        for y in range(HEAD_ROWS, RIGID_TO):
            if 0 <= y - dy < 32:
                same += sum(1 for x in range(16) if t_walk[y][x] is not None and t_walk[y][x] == t_stand[y - dy][x])
        if best is None or same > best[0]:
            best = (same, dy)
    return best[1]


def transfer(t_stand, t_walk, o_stand):
    """Our walking frame from our standing frame and the template's pair.
    The head (rows above RIGID_TO) is our own head moved by the template's bob,
    so hair shapes never break up; the body and legs follow the move map."""
    bob = head_bob(t_stand, t_walk)
    mm = move_map(t_stand, t_walk)
    link = colour_link(t_stand, o_stand)
    link_back = {}
    for a, b in link.items():
        link_back.setdefault(b, a)
    h, w = len(o_stand), len(o_stand[0])
    out = [row[:] for row in o_stand]
    for y in range(HEAD_ROWS, RIGID_TO + bob):
        out[y] = o_stand[y - bob][:] if 0 <= y - bob < h else [None] * w
    for y in range(RIGID_TO + bob, h):
        for x in range(w):
            m = mm[y][x]
            if m is None:
                continue
            if m == "gone":
                out[y][x] = None
            elif m == "new":
                out[y][x] = _near_own(o_stand, x, y, t_walk[y][x], link_back)
            else:
                dx, dy = m
                src = o_stand[y + dy][x + dx]
                # the template moved a body pixel here; if ours has nothing
                # there, use a nearby colour of ours that plays the same part
                out[y][x] = src if src is not None else _near_own(o_stand, x, y, t_walk[y][x], link_back)
    return out


def shape_score(a_frames, b_frames):
    """How alike two sets of standing frames are below the head (0..1)."""
    same = total = 0
    for fa, fb in zip(a_frames, b_frames):
        for y in range(HEAD_ROWS, 32):
            for x in range(16):
                pa, pb = fa[y][x] is not None, fb[y][x] is not None
                if pa or pb:
                    total += 1
                    same += pa and pb
    return same / max(1, total)


def template_candidates():
    """Every walking 16x32 sheet we can learn from: (name, frames)."""
    import glob
    out = []
    roots = [(artlib.PEOPLE, "")]
    em = os.path.join(artlib.EMERALD_REF, "graphics/object_events/pics/people")
    if os.path.isdir(em):
        roots.append((em, "em:"))
    for root, prefix in roots:
        for path in sorted(glob.glob(os.path.join(root, "**/*.png"), recursive=True)):
            name = prefix + os.path.relpath(path, root)[:-4]
            if any(k in name for k in ("bike", "surf", "fish", "water", "vs_seeker", "underwater", "field_move", "decorat", "acro", "mach")):
                continue
            g = artlib.load_grid(path)
            if len(g) != 32 or len(g[0]) < 144:
                continue
            out.append((name, artlib.sheet_frames(g)))
    return out


def best_templates(stand_frames, n=5, exclude=()):
    scored = []
    for name, frames in template_candidates():
        if name in exclude:
            continue
        scored.append((shape_score(stand_frames, frames[:3]), name, frames))
    scored.sort(key=lambda t: -t[0])
    return scored[:n]


def vote(candidates):
    """Pixel-by-pixel majority of several made frames (first one wins ties)."""
    h, w = len(candidates[0]), len(candidates[0][0])
    out = [[None] * w for _ in range(h)]
    for y in range(h):
        for x in range(w):
            count = {}
            for c in candidates:
                count[c[y][x]] = count.get(c[y][x], 0) + 1
            top = max(count.values())
            out[y][x] = next(c[y][x] for c in candidates if count[c[y][x]] == top)
    return out


def make_walk(stand_frames, templates):
    """9 frames: our 3 standing frames + 6 walking frames moved like the
    template(s). templates: one template's frames, or a list of several
    (they vote pixel by pixel, which cleans up odd pixels)."""
    if templates and isinstance(templates[0][0][0], (tuple, type(None))):
        templates = [templates]
    frames = [f for f in stand_frames[:3]]
    for wi in range(3, 9):
        s = artlib.STAND_OF[wi]
        frames.append(vote([transfer(t[s], t[wi], stand_frames[s]) for t in templates]))
    return frames


def make_run(walk9, run_template18):
    """Running frames 9-17 (player sheets) from our standing frames and a
    template whose frames 9-17 are runs (FireRed's red_normal): for each run
    frame, the template's standing -> running move map is put on ours."""
    run = []
    for j in range(9):
        d = j // 3  # 0 south, 1 north, 2 west
        run.append(transfer(run_template18[d], run_template18[RUN_OFFSET + j], walk9[d]))
    return run


def selftest():
    """Rebuild the walk frames of sheets that already walk, from their standing
    frames and the best OTHER template, and lay real vs made side by side."""
    tests = ["scientist", "lass", "youngster", "gentleman", "hex_maniac", "giovanni", "prof_oak", "lorelei"]
    items = []
    for name in tests:
        real = artlib.sheet_frames(artlib.people_sheet(name))
        best = best_templates(real[:3], n=3, exclude=(name, "em:" + name))
        made = make_walk(real[:3], [b[2] for b in best])
        items.append(("%s real" % name, artlib.join_frames(real[:9])))
        items.append(("made: " + ", ".join(b[1] for b in best), artlib.join_frames(made)))
    out = os.path.join(artlib.out_dir("checks"), "walkgen_selftest.png")
    artlib.label_sheet(items, cols=2, scale=3).save(out)
    print("selftest ->", out)


if __name__ == "__main__":
    if sys.argv[1:] == ["selftest"]:
        selftest()
    else:
        print(__doc__)
