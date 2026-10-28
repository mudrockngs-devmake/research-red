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
import re
import os
import struct
import sys
from collections import deque

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
LAYOUTS = {l["id"]: l for l in json.load(open(os.path.join(REPO, "data/layouts/layouts.json")))["layouts"] if "id" in l}
_by_const = None
_cache = {}
# nav.py turns this on: scene copies of the cast and people with a lasting
# hide flag don't block (usually hidden; nav's runtime checks catch anyone in
# the way; other FLAG_TEMP objects such as Cut trees do block), and a warp only counts where the tile really is a door, stairs or an
# exit mat entered in its arrow's direction (from the tilesets' behaviours)
SMART = False
_attrs = {}
_behaviors = None


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
         "objs": {(o["x"], o["y"]) for o in d.get("object_events", [])
                  if not (SMART and is_scene_copy(o))} | set(MOVED_PEOPLE.get(const, []) if SMART else []),
         "tilesets": (lay["primary_tileset"], lay["secondary_tileset"]),
         "warps": {(w["x"], w["y"]): (w["dest_map"], int(w["dest_warp_id"])) for w in d.get("warp_events", [])},
         "conns": d.get("connections") or []}
    _cache[const] = m
    return m


# People a map script moves somewhere their map data doesn't say (SMART only).
MOVED_PEOPLE = {
    # FireRed's sign lady stands at the Route 1 entrance (12, 2) once the
    # player has walked past the sign, until her Start-menu lesson (walk by on
    # x = 13 to get it)
    "MAP_PALLET_TOWN": [(12, 2)],
}

SCENE_CAST = ("OBJ_EVENT_GFX_RR_ELM", "OBJ_EVENT_GFX_RR_JENNY", "OBJ_EVENT_GFX_RR_IVY", "OBJ_EVENT_GFX_RR_OFFICER_JENNY")


def is_scene_copy(o):
    """A cast member placed on a map for a scene (hidden by a flag the rest of
    the time), or anyone with a lasting hide flag: walk through them."""
    flag = o.get("flag", "0")
    if flag in ("0", ""):
        return False
    return o["graphics_id"] in SCENE_CAST or not flag.startswith("FLAG_TEMP")


def behaviors():
    """MB_* name -> number (include/constants/metatile_behaviors.h, in order)."""
    global _behaviors
    if _behaviors is None:
        text = open(os.path.join(REPO, "include/constants/metatile_behaviors.h")).read()
        names = re.findall(r"^\s+(MB_\w+),", text.split("enum", 1)[1], re.M)
        _behaviors = {n: i for i, n in enumerate(names)}
    return _behaviors


def tileset_attrs(tileset):
    """gTileset_X -> its metatile attribute words (u32 each)."""
    if tileset not in _attrs:
        headers = open(os.path.join(REPO, "src/data/tilesets/headers.h")).read()
        block = headers.split(f"const struct Tileset {tileset} =", 1)[1].split("};", 1)[0]
        sym = re.search(r"\.metatileAttributes = (\w+)", block)[1]
        files = open(os.path.join(REPO, "src/data/tilesets/metatiles.h")).read()
        path = re.search(rf"{sym}\[\] = INCBIN_U32\(\"([^\"]+)\"\)", files)[1]
        data = open(os.path.join(REPO, path), "rb").read()
        _attrs[tileset] = struct.unpack(f"<{len(data) // 4}I", data)
    return _attrs[tileset]


def attribute(m, x, y):
    """The metatile attribute word at (x, y) (behaviour in bits 0-8, encounter type 24-26)."""
    v = struct.unpack_from("<H", m["blocks"], 2 * (y * m["w"] + x))[0] & 0x3FF
    primary, secondary = m["tilesets"]
    table, i = (tileset_attrs(primary), v) if v < 640 else (tileset_attrs(secondary), v - 640)
    return table[i] if i < len(table) else 0


ARROWS = {"MB_SOUTH_ARROW_WARP": "DOWN", "MB_NORTH_ARROW_WARP": "UP",
          "MB_EAST_ARROW_WARP": "RIGHT", "MB_WEST_ARROW_WARP": "LEFT"}
DOORS = ("MB_WARP_DOOR", "MB_REGULAR_WARP", "MB_CAVE_DOOR", "MB_LADDER", "MB_FALL_WARP",
         "MB_UP_RIGHT_STAIR_WARP", "MB_UP_LEFT_STAIR_WARP", "MB_DOWN_RIGHT_STAIR_WARP", "MB_DOWN_LEFT_STAIR_WARP",
         "MB_UP_ESCALATOR", "MB_DOWN_ESCALATOR")


def warp_works(m, x, y, direction):
    """Does stepping onto (x, y) going `direction` take this warp?"""
    b = attribute(m, x, y) & 0x1FF
    mb = behaviors()
    if any(b == mb[n] for n in DOORS):
        return True
    return any(b == mb[n] and direction == d for n, d in ARROWS.items())


def is_grass(m, x, y):
    """A tile with land encounters (tall grass and the like)."""
    return (attribute(m, x, y) >> 24) & 7 == 1


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
            if (nx, ny) in m["warps"] and (not SMART or warp_works(m, nx, ny, name)):
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
