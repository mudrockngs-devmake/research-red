#!/usr/bin/env python3
"""Research Red: walk anywhere in a test, by map data (#18).

go(G, start, goal_map, goal_xy) plans the shortest walk with route_plan.py
(walkable tiles, map connections, doors) and turns it into runner lines that
check every leg really happened: each straight stretch taps the direction
until the player stands on its last tile, and each door waits until the new
map is loaded and the player can walk. A person in the way only costs time
(wanderers move on);
something that never lets you through FAILs at that leg (see its timeout_
screenshot), instead of throwing every later step off.

  start, goal  ("MAP_PALLET_TOWN", x, y); goal_xy None = the first tile reached
  on goal_map. Positions are map tiles as Porymap shows them.

Limits are route_plan's: no ledges, no surfing; objects block where they
stand at the start of the map's data.
"""
import os
import struct
import sys
from collections import deque

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import route_plan  # noqa: E402

route_plan.SMART = True

_count = [0]


def metatile(const, x, y):
    m = route_plan.load(const)
    return struct.unpack_from("<H", m["blocks"], 2 * (y * m["w"] + x))[0] & 0x3FF


def is_grass(node):
    m, x, y = node
    return route_plan.is_grass(route_plan.load(m), x, y)


def path(start, goal_map, goal_xy=None, goal=None, avoid=()):
    """[(direction, (map, x, y), through_door)] one entry per step, or None.
    goal: a test on (map, x, y) instead of goal_map/goal_xy.
    avoid: (map, x, y) tiles someone stands on right now (a scene's cast)."""
    prev = {start: None}
    q = deque([start])
    end = None
    while q:
        cur = q.popleft()
        if (goal(cur) if goal else cur[0] == goal_map and (goal_xy is None or cur[1:] == tuple(goal_xy))):
            end = cur
            break
        for name, nxt, warp in route_plan.neighbours(*cur):
            if nxt not in prev and nxt not in avoid:
                prev[nxt] = (cur, name, warp)
                q.append(nxt)
    if end is None:
        return None
    steps = []
    while prev[end]:
        cur, name, warp = prev[end]
        steps.append((name, end, warp))
        end = cur
    return steps[::-1]


def go(G, start, goal_map, goal_xy=None, max_frames=20000, label="go", goal=None, avoid=()):
    """Runner lines walking from start to the goal (raises if there's no way).
    max_frames per stretch counts every frame, so it leaves room for a wild
    battle on the way (the playthrough plays those out); a stretch that's
    fine finishes as soon as the player gets there."""
    steps = path(tuple(start), goal_map, goal_xy, goal, avoid)
    if steps is None:
        raise ValueError(f"no walking route from {start} to {goal_map} {goal_xy or ''}")
    lines = []
    i = 0
    while i < len(steps):
        d, (m, x, y), door = steps[i]
        # extend the stretch while the direction holds, on the same map, no door
        j = i
        while (j + 1 < len(steps) and steps[j + 1][0] == d and not steps[j][2]
               and not steps[j + 1][2] and steps[j + 1][1][0] == m):
            j += 1
        d, (m, x, y), door = steps[j]
        _count[0] += 1
        tag = f"{label}{_count[0]}"
        if door:
            lines += [f"until {tag} {max_frames} {d} {G.on_map(m)}", "wait 8",
                      f"until {tag}r {max_frames} - {G.field_free()}"]
        else:
            lines += [f"until {tag} {max_frames} {d} {G.on_map(m)} && {G.player_at(x, y)}", "wait 20"]
        i = j + 1
    return lines


def where(G):
    """Runner lines that print the player's map and tile (PEEK map/x/y)."""
    base = G.sym("gObjectEvents") + G.const("offsetof(struct ObjectEvent, currentCoords)")
    return [f"peek where_map {G.map_addr()}", f"peek where_x {base:#x}:2", f"peek where_y {base + 2:#x}:2"]


def grass_route(start, grass_map):
    """Walk from start to the nearest tall grass on grass_map with a grass
    neighbour to pace to: (lines-free path end, the two tiles)."""
    def ok(node):
        m, x, y = node
        if m != grass_map or not is_grass(node):
            return False
        return any(route_plan.walkable(route_plan.load(m), x + dx, y) and is_grass((m, x + dx, y)) for dx in (1, -1))
    steps = path(tuple(start), None, goal=ok)
    if steps is None:
        raise ValueError(f"no grass reachable on {grass_map} from {start}")
    m, x, y = steps[-1][1] if steps else start
    nx = x + 1 if route_plan.walkable(route_plan.load(m), x + 1, y) and is_grass((m, x + 1, y)) else x - 1
    return (m, x, y), (m, nx, y)


def wild_probe(G, start, grass_map, max_paces=150, guarded=False):
    """Walk to grass on grass_map and pace until a wild battle starts; record
    what appeared (PEEK wild_species_1 / _3, wild_trainer_flags). rr_checks'
    wild judge then checks it's a wild Pokemon from that map's table (#106).
    guarded: a guard plays battles out as they come (the playthrough): pace
    a while and let the judge read the first battle on that map from the
    battle record instead."""
    a, b = grass_route(start, grass_map)
    lines = ["mark wild_probe"] + go(G, start, None, goal=lambda n: n == a, label="wildgo")
    d_out = "RIGHT" if b[1] > a[1] else "LEFT"
    d_back = "LEFT" if d_out == "RIGHT" else "RIGHT"
    pace = []
    for _ in range(max_paces):
        pace += [f"hold {d_out} 16", "wait 4", f"hold {d_back} 16", "wait 4"]
    if guarded:
        return lines + pace[:4 * 60] + [
            f"# wild-map {grass_map}", f"# wild-map-id {G.map_value(grass_map)}",
            f"# wild-allowed {' '.join(str(G.const(c)) for c in wild_species(grass_map))}",
            f"# wild-trainer-bit {G.const('BATTLE_TYPE_TRAINER')}"]
    mons = G.sym("gBattleMons")
    size = G.const("sizeof(struct BattlePokemon)")
    sp = G.const("offsetof(struct BattlePokemon, species)")
    lines += pace + [f"until wild_battle 600 - {G.in_battle()}"]
    lines += ["wait 300", "shot wild_probe",
              f"peek wild_species_1 {mons + size + sp:#x}:2",
              f"peek wild_species_3 {mons + 3 * size + sp:#x}:2",
              f"peek wild_trainer_flags {G.sym('gBattleTypeFlags'):#x}:4",
              f"# wild-map {grass_map}",
              f"# wild-allowed {' '.join(str(G.const(c)) for c in wild_species(grass_map))}",
              f"# wild-trainer-bit {G.const('BATTLE_TYPE_TRAINER')}"]
    return lines


def wild_species(map_const):
    """Every species (SPECIES_*) in the map's encounter file, day and night."""
    import re
    folder = os.path.join(route_plan.REPO, "data", "research_red", "encounters")
    sys.path.insert(0, os.path.dirname(HERE))
    import build_encounters as be
    known = set(re.findall(r"\b(SPECIES_\w+)\s*=", open(be.SPECIES_H).read()))
    for f in sorted(os.listdir(folder)):
        if f.endswith(".txt") and f != "README.md":
            area = be.parse(os.path.join(folder, f), known)
            if area["map"] == map_const:
                return sorted({mon[0] for sec in area["sections"].values() for mon in sec["mons"]})
    raise ValueError(f"no encounter file for {map_const}")
