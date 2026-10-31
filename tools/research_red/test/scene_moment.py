#!/usr/bin/env python3
"""Research Red: where everyone really stands at one moment of a scene.

  python3 tools/research_red/test/scene_moment.py grass_oak       # play to it, make its board data
  python3 tools/research_red/viewer/viewer.py board MAP_PALLET_TOWN --moment grass_oak --title "..."

Plays the real game up to the moment the script loads a chosen line of dialogue
(everyone has finished walking by then), reads every object on the map, and saves
that (plus a screenshot) for the viewer's scene board, so the owner can drag people
to their final spots (docs/DEV_GUIDE.md). Add a moment to MOMENTS to get a new board.
"""
import glob
import json
import os
import re
import shutil
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(HERE, "..", "viewer"))
import smoke  # noqa: E402
import tester as T  # noqa: E402
import viewer  # noqa: E402

FACING = {1: "down", 2: "up", 3: "left", 4: "right"}  # DIR_SOUTH, NORTH, WEST, EAST


def to_grass():
    return smoke.sc_breakfast() + smoke.step("DOWN", 3) + smoke.step("LEFT") + smoke.step("DOWN")


# name: (how to get near it, the line it stops on, map, what the owner is looking at)
MOMENTS = {
    "grass_oak": (to_grass, "PalletTown_Text_RR_Oak_WildMons", "MAP_PALLET_TOWN",
                  "Grass hook: Oak has hurried over and says wild Pokémon live in tall grass"),
}


def moment_lines(name):
    start, text, _map, _about = MOMENTS[name]
    g = T.G
    data0 = g.sym("sGlobalScriptContext") + g.const("offsetof(struct ScriptContext, data)")
    lines = start() + [f"until moment 30000 B {data0:#x}:4 == {g.sym(text):#x}", "wait 1", "shot moment"]
    size, base = g.const("sizeof(struct ObjectEvent)"), g.sym("gObjectEvents")
    off = {f: g.const(f"offsetof(struct ObjectEvent, {f})") for f in ("localId", "currentCoords", "previousCoords")}
    for i in range(16):
        b = base + i * size
        lines += [f"peek o{i}_active {b:#x}:1", f"peek o{i}_id {b + off['localId']:#x}:1",
                  f"peek o{i}_x {b + off['currentCoords']:#x}:2", f"peek o{i}_y {b + off['currentCoords'] + 2:#x}:2",
                  f"peek o{i}_facing {b + off['previousCoords'] + 4:#x}:1"]
    return lines


def people_from(log, map_id):
    """The objects the game reported, named from the map file."""
    peeks = dict(re.findall(r"PEEK (o\d+_\w+) (-?\w+)", log))
    val = lambda k: int(peeks[k], 0)
    m = __import__("render_map").find_map(map_id)
    consts = open(os.path.join(smoke.REPO, "include/constants/map_event_ids.h")).read()
    by_id = {}
    for o in m["object_events"]:
        lid = o.get("local_id")
        hit = re.search(rf"#define {lid} (\d+)", consts) if lid else None
        if hit:
            by_id[int(hit.group(1))] = o
    people = []
    for i in range(16):
        if not val(f"o{i}_active") & 1:
            continue
        lid = val(f"o{i}_id")
        o = by_id.get(lid)
        if lid == 255:
            who, gfx = "PLAYER", viewer.PLAYER_GFX
        elif lid == 254:
            who, gfx = "IVY (following)", "OBJ_EVENT_GFX_RR_IVY"
        elif o:
            who, gfx = o["local_id"], o["graphics_id"]
        else:
            continue  # the camera or a nameless map object far from the scene
        people.append({"id": who, "gfx": gfx, "x": val(f"o{i}_x") - 7, "y": val(f"o{i}_y") - 7,
                       "facing": FACING.get(val(f"o{i}_facing") & 15, "down")})
    return people


def main():
    name = sys.argv[1]
    if name not in MOMENTS:
        sys.exit("moments: " + ", ".join(MOMENTS))
    scenarios = {"intro": smoke.SCENARIOS["intro"], name: (lambda: moment_lines(name), MOMENTS[name][3])}
    try:
        smoke.main(scenarios=scenarios, kind="moments", argv=[name, "--no-compare"], title="Scene moment", after_run=lambda d: None)
    except SystemExit:
        pass
    run = max(glob.glob(os.path.join(smoke.WORKSPACE, "test-runs", "moments", "*")), key=os.path.getmtime)
    log = open(os.path.join(run, name, "runner.log")).read()
    people = people_from(log, MOMENTS[name][2])
    os.makedirs(os.path.join(viewer.BOARDS, "moments"), exist_ok=True)
    shot = os.path.join(viewer.BOARDS, "moments", name + ".png")
    shutil.copy(os.path.join(run, name, "moment.png"), shot)
    json.dump({"about": MOMENTS[name][3], "shot": shot, "people": people},
              open(os.path.join(viewer.BOARDS, "moments", name + ".json"), "w"), indent=2)
    for p in people:
        print(f"{p['id']:32} {p['x']:3},{p['y']:<3} {p['facing']}")


if __name__ == "__main__":
    main()
