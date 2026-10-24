#!/usr/bin/env python3
"""Research Red Art Kit: new buildings you can walk into.

One command makes a new inside map, copied from an existing one you like (a
house, a lab, a shop), and links its door both ways with the outside map:

  python3 tools/research_red/art/building.py new PalletTown_RR_Greenhouse \\
          --copy MAP_PALLET_TOWN_RIVALS_HOUSE --outside MAP_PALLET_TOWN --door 12,14

  - data/layouts/<Name>/  a copy of the template's floor plan (repaint it in Porymap)
  - data/maps/<Name>/     map.json (the template's settings, no people) + scripts.inc
  - the map is added to the template's map group and to data/event_scripts.s
  - the inside exit mat warps out to the door; a new warp on the door square
    of the outside map leads in. The outside square must look like a door
    (door behaviour); the tool says so if it doesn't: paint one there in
    Porymap, or add your own building front with tile_kit.py first.

  python3 tools/research_red/art/building.py check MAP_X
      List a map's warps and whether each one leads somewhere that leads back.
  python3 tools/research_red/art/building.py doors MAP_X
      The door squares of an outside map (where a warp could go).

After `new`: build (make), then walk in. The map's id is MAP_<NAME_IN_CAPITALS>.
"""
import argparse
import json
import os
import re
import shutil
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import artlib  # noqa: E402
import tile_kit  # noqa: E402

REPO = artlib.REPO
DOOR_WORDS = ("DOOR", "WARP", "STAIRS", "LADDER", "ESCALATOR")


def map_path(map_id):
    for d in os.listdir(os.path.join(REPO, "data/maps")):
        p = os.path.join(REPO, "data/maps", d, "map.json")
        if os.path.exists(p) and json.load(open(p)).get("id") == map_id:
            return p
    raise SystemExit("no map %s" % map_id)


def const_name(name):
    """PalletTown_RR_Greenhouse -> PALLET_TOWN_RR_GREENHOUSE (the decomp's style)."""
    s = re.sub(r"(?<=[a-z0-9])(?=[A-Z])", "_", name)
    return re.sub(r"_+", "_", s).upper()


def layouts_file():
    return os.path.join(REPO, "data/layouts/layouts.json")


def layout_of(map_json):
    lays = json.load(open(layouts_file()))["layouts"]
    return next(l for l in lays if l.get("id") == map_json["layout"])


def square_behavior(map_json, x, y):
    """Behaviour name of the block at x, y of a map."""
    lay = layout_of(map_json)
    data = open(os.path.join(REPO, lay["blockdata_filepath"]), "rb").read()
    mid = struct.unpack_from("<H", data, (y * lay["width"] + x) * 2)[0] & 0x3FF
    sym = lay["primary_tileset"] if mid < tile_kit.NUM_PRIMARY_METATILES else lay["secondary_tileset"]
    ts = tile_kit.Tileset(sym)
    local = mid - ts.meta_base
    beh = struct.unpack_from("<I", ts.attrs, local * 4)[0] & 0x1FF
    names = {v: k for k, v in tile_kit.behaviors().items()}
    return mid, names.get(beh, str(beh))


def save_json(path, data):
    with open(path, "w") as f:
        json.dump(data, f, indent=2, ensure_ascii=False)
        f.write("\n")


def cmd_new(name, copy, outside, door, group=None, dry=False):
    if not re.match(r"^[A-Za-z0-9_]+$", name):
        raise SystemExit("name: letters, digits and _ only (e.g. PalletTown_RR_Greenhouse)")
    map_id, layout_id = "MAP_" + const_name(name), "LAYOUT_" + const_name(name)
    map_dir = os.path.join(REPO, "data/maps", name)
    if os.path.exists(map_dir):
        raise SystemExit("%s already exists" % map_dir)
    tpath = map_path(copy)
    tmap = json.load(open(tpath))
    tlay = layout_of(tmap)
    opath = map_path(outside)
    omap = json.load(open(opath))
    dx, dy = door
    mid, beh = square_behavior(omap, dx, dy)
    door_ok = any(w in beh for w in DOOR_WORDS)
    print("%s: new inside map, floor plan copied from %s (%dx%d, %s + %s)" % (
        map_id, copy, tlay["width"], tlay["height"], tlay["primary_tileset"], tlay["secondary_tileset"]))
    print("  door: %s square %d,%d is block 0x%03X (%s)%s" % (
        outside, dx, dy, mid, beh, "" if door_ok else "  <- not a door yet: paint a door here in Porymap"))
    if not tmap.get("warp_events"):
        raise SystemExit("%s has no warps to copy the exit from" % copy)
    if dry:
        print("  dry run: nothing changed")
        return
    # 1. layout: copy the floor plan files, add the layout entry
    lay_dir = os.path.join(REPO, "data/layouts", name)
    os.makedirs(lay_dir)
    shutil.copy(os.path.join(REPO, tlay["blockdata_filepath"]), os.path.join(lay_dir, "map.bin"))
    shutil.copy(os.path.join(REPO, tlay["border_filepath"]), os.path.join(lay_dir, "border.bin"))
    lays = json.load(open(layouts_file()))
    new_lay = dict(tlay, id=layout_id, name=name + "_Layout",
                   blockdata_filepath="data/layouts/%s/map.bin" % name,
                   border_filepath="data/layouts/%s/border.bin" % name)
    lays["layouts"].append(new_lay)
    save_json(layouts_file(), lays)
    # 2. the outside warp goes in first so the inside knows its number
    o_index = len(omap.get("warp_events") or [])
    elev = next((w["elevation"] for w in omap.get("warp_events") or []), 0)
    omap.setdefault("warp_events", []).append({"x": dx, "y": dy, "elevation": elev, "dest_map": map_id, "dest_warp_id": "0"})
    save_json(opath, omap)
    # 3. the map itself: the template's settings, its exit warps, no people or signs
    new = {k: tmap[k] for k in tmap}
    new.update(id=map_id, name=name, layout=layout_id, object_events=[], coord_events=[], bg_events=[],
               connections=None)
    exits = [w for w in tmap["warp_events"] if w["dest_map"] == tmap["warp_events"][0]["dest_map"]]
    new["warp_events"] = [dict(w, dest_map=outside, dest_warp_id=str(o_index)) for w in exits]
    os.makedirs(map_dir)
    save_json(os.path.join(map_dir, "map.json"), new)
    with open(os.path.join(map_dir, "scripts.inc"), "w") as f:
        f.write("@ Research Red: %s (made by tools/research_red/art/building.py)\n%s_MapScripts::\n\t.byte 0\n" % (name, name))
    # 4. map group (the template's) and the script include
    gpath = os.path.join(REPO, "data/maps/map_groups.json")
    groups = json.load(open(gpath))
    tname = tmap["name"]
    group = group or next(g for g in groups["group_order"] if tname in groups[g])
    groups[group].append(name)
    save_json(gpath, groups)
    epath = os.path.join(REPO, "data/event_scripts.s")
    text = open(epath).read()
    anchor = '\t.include "data/maps/%s/scripts.inc"\n' % tname
    text = text.replace(anchor, anchor + '\t.include "data/maps/%s/scripts.inc" @ Research Red (building.py)\n' % name, 1)
    open(epath, "w").write(text)
    print("  made data/maps/%s, data/layouts/%s; group %s; %d exit warp(s) -> %s warp %d" % (
        name, name, group, len(exits), outside, o_index))
    print("  next: make; then walk in through %d,%d. Repaint the inside in Porymap." % (dx, dy))


def cmd_check(map_id):
    m = json.load(open(map_path(map_id)))
    for i, w in enumerate(m.get("warp_events") or []):
        dest = w["dest_map"]
        note = ""
        try:
            d = json.load(open(map_path(dest)))
            dw = (d.get("warp_events") or [])
            k = int(w["dest_warp_id"]) if str(w["dest_warp_id"]).isdigit() else -1
            if not (0 <= k < len(dw)):
                note = "  <- %s has no warp %s" % (dest, w["dest_warp_id"])
            elif dw[k]["dest_map"] != map_id:
                note = "  (that warp leads to %s, not back here)" % dw[k]["dest_map"]
        except SystemExit:
            note = "  (special destination)"
        print("  warp %d at %d,%d -> %s warp %s%s" % (i, w["x"], w["y"], dest, w["dest_warp_id"], note))


def cmd_doors(map_id):
    m = json.load(open(map_path(map_id)))
    lay = layout_of(m)
    warps = {(w["x"], w["y"]) for w in m.get("warp_events") or []}
    for y in range(lay["height"]):
        for x in range(lay["width"]):
            mid, beh = square_behavior(m, x, y)
            if any(w in beh for w in DOOR_WORDS):
                print("  %2d,%2d  block 0x%03X  %s%s" % (x, y, mid, beh, "  (has a warp)" if (x, y) in warps else "  (free)"))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cmd", choices=["new", "check", "doors"])
    ap.add_argument("name")
    ap.add_argument("--copy", help="template map (MAP_X) whose floor plan and settings to copy")
    ap.add_argument("--outside", help="outside map (MAP_X) the door is on")
    ap.add_argument("--door", help="X,Y of the door square on the outside map")
    ap.add_argument("--group", help="map group (default: the template's)")
    ap.add_argument("--dry-run", action="store_true")
    a = ap.parse_args()
    if a.cmd == "new":
        if not (a.copy and a.outside and a.door):
            ap.error("new needs --copy, --outside and --door")
        cmd_new(a.name, a.copy, a.outside, tuple(int(v) for v in a.door.split(",")), a.group, a.dry_run)
    elif a.cmd == "check":
        cmd_check(a.name)
    else:
        cmd_doors(a.name)
    return 0


if __name__ == "__main__":
    sys.exit(main())
