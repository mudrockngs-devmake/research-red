#!/usr/bin/env python3
"""Research Red: plan a walking route across maps for the smoke tests (#18).

Searches the map data (data/maps/*/map.json + layouts) for the shortest walk:
walkable tiles, map edges joined by connections, and doors/warps. Prints the
steps as smoke-test lines, e.g. step("UP", 5) + step("LEFT", 3).

  python3 tools/research_red/test/route_plan.py MAP_PEWTER_CITY 17 26 MAP_VIRIDIAN_FOREST [x y]

Without x y, the route ends at the first tile reached on the target map.
Limits: no ledges (jumping), no surfing, objects count as walls where they
stand at the start (people who walk around can still get in the way).
"""
import json
import os
import struct
import sys
from collections import deque

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
LAYOUTS = {l["id"]: l for l in json.load(open(os.path.join(REPO, "data/layouts/layouts.json")))["layouts"] if "id" in l}
_by_const = None
_cache = {}


def map_folder(const):
    global _by_const
    if _by_const is None:
        _by_const = {}
        for d in os.listdir(os.path.join(REPO, "data/maps")):
            p = os.path.join(REPO, "data/maps", d, "map.json")
            if os.path.exists(p):
                _by_const[json.load(open(p))["id"]] = d
    return _by_const[const]


def load(const):
    if const in _cache:
        return _cache[const]
    d = json.load(open(os.path.join(REPO, "data/maps", map_folder(const), "map.json")))
    lay = LAYOUTS[d["layout"]]
    blocks = open(os.path.join(REPO, lay["blockdata_filepath"]), "rb").read()
    m = {"w": lay["width"], "h": lay["height"], "blocks": blocks, "json": d,
         "objs": {(o["x"], o["y"]) for o in d.get("object_events", [])},
         "warps": {(w["x"], w["y"]): (w["dest_map"], int(w["dest_warp_id"])) for w in d.get("warp_events", [])},
         "conns": d.get("connections") or []}
    _cache[const] = m
    return m


def walkable(m, x, y):
    if not (0 <= x < m["w"] and 0 <= y < m["h"]):
        return False
    v = struct.unpack_from("<H", m["blocks"], 2 * (y * m["w"] + x))[0]
    return not ((v >> 10) & 3) and (x, y) not in m["objs"] and (v >> 12) != 1


def neighbours(const, x, y):
    m = load(const)
    for dx, dy, name in ((1, 0, "RIGHT"), (-1, 0, "LEFT"), (0, 1, "DOWN"), (0, -1, "UP")):
        nx, ny = x + dx, y + dy
        if 0 <= nx < m["w"] and 0 <= ny < m["h"]:
            if (nx, ny) in m["warps"]:
                dest, wid = m["warps"][(nx, ny)]
                if dest.startswith("MAP_") and dest != "MAP_DYNAMIC":
                    w = load(dest)["json"]["warp_events"][wid]
                    yield name, (dest, w["x"], w["y"]), True
                continue
            if walkable(m, nx, ny):
                yield name, (const, nx, ny), False
            continue
        # off the edge: follow a connection
        for c in m["conns"]:
            d, off, other = c["direction"], c["offset"], c["map"]
            om = load(other)
            if d == "up" and ny < 0:
                tx, ty = nx - off, om["h"] - 1
            elif d == "down" and ny >= m["h"]:
                tx, ty = nx - off, 0
            elif d == "left" and nx < 0:
                tx, ty = om["w"] - 1, ny - off
            elif d == "right" and nx >= m["w"]:
                tx, ty = 0, ny - off
            else:
                continue
            if walkable(om, tx, ty):
                yield name, (other, tx, ty), False


def plan(start, goal_map, goal_xy=None):
    prev = {start: None}
    q = deque([start])
    end = None
    while q:
        cur = q.popleft()
        if cur[0] == goal_map and (goal_xy is None or cur[1:] == goal_xy):
            end = cur
            break
        for name, nxt, warp in neighbours(*cur):
            if nxt not in prev:
                prev[nxt] = (cur, name + ("*" if warp else ""))
                q.append(nxt)
    if end is None:
        return None
    steps = []
    while prev[end]:
        end, name = prev[end]
        steps.append(name)
    steps.reverse()
    # "*" marks a step through a door/warp: the screen fades, so the test waits after it
    out = []
    for s in steps:
        warp = s.endswith("*")
        s = s.rstrip("*")
        if out and out[-1][0] == s and not out[-1][2]:
            out[-1][1] += 1
            out[-1][2] = warp
        else:
            out.append([s, 1, warp])
    return out


def main():
    a = sys.argv[1:]
    if len(a) < 4:
        print(__doc__)
        sys.exit(1)
    goal_xy = (int(a[4]), int(a[5])) if len(a) >= 6 else None
    route = plan((a[0], int(a[1]), int(a[2])), a[3], goal_xy)
    if route is None:
        print("no route found")
        sys.exit(1)
    parts = []
    for d, n, warp in route:
        parts.append(f'step("{d}", {n})' if n > 1 else f'step("{d}")')
        if warp:
            parts.append('["wait 90"]')
    print(" + ".join(parts))


if __name__ == "__main__":
    main()
