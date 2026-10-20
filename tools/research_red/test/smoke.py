#!/usr/bin/env python3
"""Research Red: automated smoke tests.

Plays the ROM headless through each story scene using the debug skips, and
checks that every scene really finishes and leaves the game where it should.
This file only describes Research Red's scenes; the reusable engine (running,
stuck detection, memory checks, report) is tester.py. See docs/TESTING.md.

Usage:
  python3 tools/research_red/test/smoke.py              # all scenarios
  python3 tools/research_red/test/smoke.py lunch banter # just these
  python3 tools/research_red/test/smoke.py --list

Output: <workspace>/test-runs/<date>_<version>_<commit>/
  REPORT.md            pass / warn / fail per scenario, where each one ended
  <scenario>.png       contact sheet
  <scenario>/          every screenshot
Run it after building, before handing a ROM to the owner.
"""
import datetime
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))
sys.path.insert(0, HERE)
import tester as T  # noqa: E402
from tester import advance, check, expect, play_until, scene_end, start_menu_item, step  # noqa: E402

TOOLS = os.environ.get("RR_TOOLS", os.path.expanduser("~/romhacks/_tools"))
WORKSPACE = "/mnt/e/Workspace/romhacks/research-red"
STORY = "VAR_0x40FE"  # story progress, see CLAUDE.md

# Debug menu (Start > DEBUG > Scripts): see data/scripts/debug.inc
SCRIPT_LAB, SCRIPT_AFTER_LAB, SCRIPT_ROUTE1, SCRIPT_ROUTE22, SCRIPT_TEAM, \
    SCRIPT_LUNCH, SCRIPT_BANTER, SCRIPT_ROUTE1_ELM, SCRIPT_TEST_MODE, SCRIPT_AFTER_BROCK = range(1, 11)


def debug_script(n):
    """Start > DEBUG > Scripts > script n."""
    lines = ["press START", "wait 20", "press A", "wait 20"]
    lines += ["press DOWN", "wait 6"] * 5          # Scripts is the 6th entry
    lines += ["press A", "wait 20"]
    lines += ["press DOWN", "wait 6"] * (n - 1)
    lines += ["press A", "wait 240"]
    return lines


# --- scenarios ------------------------------------------------------------
# Each returns script lines. "@bedroom" is the state saved at the end of the
# intro (a new game standing in the bedroom); the others start from it.
def sc_intro():
    lines = ["wait 240", "press START", "wait 200", "press START", "wait 150"]
    lines += play_until("bedroom", f"{T.G.on_map('MAP_PALLET_TOWN_PLAYERS_HOUSE_2F')} && {T.G.field_free()}",
                        12000, every=270)
    lines += ["wait 200", "mash B 120", "wait 60", "shot s999"]
    lines += expect("MAP_PALLET_TOWN_PLAYERS_HOUSE_2F", story=0)
    return lines + ["save @bedroom"]


def sc_breakfast():
    return (["load @bedroom"] + step("RIGHT", 4) + step("UP", 5) + ["hold LEFT 20", "wait 60", "shot s000"]
            + play_until("breakfast", scene_end(1), 6000)
            + expect("MAP_PALLET_TOWN_PLAYERS_HOUSE_1F", story=1))


def sc_lab():
    return (["load @bedroom"] + debug_script(SCRIPT_TEST_MODE) + ["mash B 40"] + debug_script(SCRIPT_LAB)
            + ["shot s000"] + step("UP") + play_until("lab", scene_end(3), 60000)
            + expect(story=3, party=1))


def sc_daisy():
    # Debug 2 leaves us below Daisy's door (story 3): step in, the scene plays
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + ["shot s000"] + step("UP")
            + play_until("daisy", scene_end(4), 12000, key="B", every=30)
            + expect("MAP_PALLET_TOWN_RIVALS_HOUSE", story=4)
            # then talk to Daisy at (5,4) from (4,4): her own lines, with portrait
            + step("UP", 2) + ["press RIGHT 2", "wait 10", "press A", "wait 40", "shot t001",
                               "press A", "wait 60", "shot t002", "mash B 120", "shot t003"])


def sc_route1():
    return (["load @bedroom"] + debug_script(SCRIPT_ROUTE1_ELM) + ["shot s000"] + step("DOWN")
            + play_until("route1", scene_end(5), 6000, key="B", every=24)
            + advance(20, key="B", every=24, prefix="t")   # the Mewtwo fly-by
            + expect("MAP_ROUTE1", story=5))


def sc_route22():
    return (["load @bedroom"] + debug_script(SCRIPT_ROUTE22) + debug_script(SCRIPT_TEST_MODE) + ["mash B 40", "shot s000"]
            + step("LEFT") + play_until("route22", scene_end(6), 30000)
            + expect("MAP_ROUTE22", story=6, party=1))


def sc_lunch():
    # Debug 6 leaves us two steps below Elm (29,26): one step up, talk to him
    return (["load @bedroom"] + debug_script(SCRIPT_LUNCH) + ["shot s000"] + step("UP", 1)
            + ["press UP 2", "wait 10", "press A", "wait 30"]
            + play_until("lunch", scene_end(7), 10000, key="B", every=50)
            + expect("MAP_VIRIDIAN_CITY", story=7))


def sc_banter():
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + debug_script(SCRIPT_BANTER) + ["wait 30"]
            + advance(26, every=50) + expect(story=3, party=1))


def sc_pokedex():
    # Start menu after the lab: DEBUG, POKEDEX. Open Bulbasaur and flip through
    # every page both ways (the Evo page used to crash, #19), then an unseen entry.
    pages = []
    for i, key in enumerate(["A"] + ["RIGHT"] * 7 + ["LEFT"] * 7 + ["B", "DOWN", "A", "RIGHT", "B", "B"], 1):
        pages += [f"press {key}", "wait 120", f"shot p{i:03d}"]
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + start_menu_item(1) + ["wait 60", "shot p000"]
            + pages + expect(story=3))


def sc_pewter():
    # After Brock: Elm and Jenny's rematch outside the Pokemon Center (#10).
    # Debug 10 leaves us below the door at (17,26); they stand at (15-16, 27).
    return (["load @bedroom"] + debug_script(SCRIPT_TEST_MODE) + ["mash B 40"]
            + debug_script(SCRIPT_AFTER_BROCK) + ["shot s000"]
            + step("LEFT") + ["wait 20", "press DOWN 6", "wait 20", "shot s001", "press A", "wait 30"]
            + play_until("pewter", scene_end(8), 30000)
            + expect("MAP_PEWTER_CITY", story=8, party=1))


def sc_demo_end():
    # After Brock, walking east out of Pewter: the guide turns us back (#35).
    # Debug 10 leaves us at (17,26); the exit tiles are at x 42.
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_BROCK) + ["shot s000"]
            + step("RIGHT", 5) + step("UP", 4) + step("RIGHT", 20)
            + play_until("turned_back", T.G.field_free(), 3000, key="B", every=20)
            + check("still_in_pewter", T.G.on_map("MAP_PEWTER_CITY"))
            + step("RIGHT", 4) + ["wait 30", "shot t001"]
            + check("still_in_pewter_again", T.G.on_map("MAP_PEWTER_CITY")))


def sc_digletts():
    # After Brock, south into Route 2, Cut the tree, into Diglett's Cave:
    # Dugtrio sits on the ladder (demo block). Debug 10 leaves us at Pewter (17,26).
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_BROCK) + ["shot s000"]
            + step("DOWN", 9) + step("RIGHT", 4) + step("DOWN", 5)      # Route 2 (9,0)
            + step("DOWN", 12) + step("RIGHT", 2) + ["shot s001"]        # (11,12), tree below
            + ["press DOWN 6", "wait 20", "press A", "wait 60", "shot s002"]
            + play_until("cut", T.G.field_free(), 1500, key="A", every=30)
            + step("DOWN", 2) + step("RIGHT", 6) + step("UP", 3) + ["wait 420", "shot s003"]  # past the area preview
            + check("in_entrance", T.G.on_map("MAP_DIGLETTS_CAVE_NORTH_ENTRANCE"))
            + step("RIGHT", 2)  # from the doormat (4,5) to (6,5), below Dugtrio
            + ["press UP 6", "wait 20", "press A", "wait 60", "shot s004"]
            + play_until("dugtrio", T.G.field_free(), 1500, key="A", every=30)
            + step("UP", 2) + ["wait 30", "shot s005"]
            + check("still_in_entrance", T.G.on_map("MAP_DIGLETTS_CAVE_NORTH_ENTRANCE")))


def sc_gerald():
    # Surf south from Pallet's shore: Gerald blocks the sea route (#36).
    # Debug 2 leaves us at (15,8); round the lab to the shore at (8,16),
    # face the water (8,17), surf, then one step south onto the trigger row.
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + step("LEFT", 3) + step("DOWN", 8)
            + step("LEFT", 4) + ["press DOWN 2", "wait 10", "shot s000", "press A", "wait 30",
                                 "mash A 200", "wait 60", "shot s001"]
            + step("DOWN", 1)
            + play_until("gerald_done", T.G.field_free(), 3000, key="B", every=20)
            + check("still_in_pallet", T.G.on_map("MAP_PALLET_TOWN")) + ["shot t001"])


def sc_pc():
    # Start > PC from anywhere (#34). After the lab (story 3) the menu is
    # DEBUG, POKEDEX, POKEMON, PC: open it, enter Pokemon storage, back out.
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + start_menu_item(3)
            + ["wait 60", "shot p001", "press A", "wait 60", "mash A 60", "wait 90", "shot p002",
               "press A", "wait 90", "shot p003"]
            + play_until("closed", T.G.field_free(), 1500, key="B", every=30)
            + check("still_in_pallet", T.G.on_map("MAP_PALLET_TOWN")))


def sc_guide():
    return (["load @bedroom"] + debug_script(SCRIPT_ROUTE1) + step("UP", 4) + ["wait 60", "shot s000"]
            # Start > POKEDEX, then the Pokédex's START menu: BACK TO LIST, LIST TOP,
            # FIELD GUIDE, CLOSE. It closes the Pokédex and opens the Guide on the map.
            + start_menu_item(1) + ["wait 150", "shot s001", "press START", "wait 40",
                                    "press DOWN", "wait 10", "shot s001b", "press DOWN", "wait 10", "shot s002",
                                    "press A", "wait 180", "shot s003", "press RIGHT", "wait 20", "shot s004",
                                    "press B", "wait 40", "shot s005"]
            + check("guide_closed", T.G.field_free())
            + expect("MAP_ROUTE1"))


def sc_rules():
    # after the lunch: Start menu DEBUG, POKEDEX, POKEMON, PC, BAG, PLAYER,
    # SAVE, OPTION. Options row 3 is Research Rules: A leaves Options and
    # opens Elm's Rules window on the map (#45).
    return (sc_lunch() + ["mash B 200", "wait 60"] + start_menu_item(7)
            + ["wait 60", "shot t001", "press DOWN", "wait 10", "press DOWN", "wait 10", "shot t002",
               "press A", "wait 120", "shot t003", "press DOWN", "wait 10", "press RIGHT", "wait 10", "shot t004",
               "press B", "wait 30", "shot t005"]
            + check("rules_closed", T.G.field_free()))


def sc_ivy():
    # Talk to Ivy (the follower, right behind you) in Pallet and on Route 1:
    # each area has its own lines (rr_ivy.inc). Tap toward her once to face
    # her, then talk three times (a random line each time).
    def talk(toward, tag):
        lines = [f"press {toward} 2", "wait 10"]
        for i in range(1, 4):
            lines += ["press A", "wait 50", f"shot {tag}{i}", "mash B 60", "wait 20"]
        return lines
    # Pallet: debug 2 leaves us below Daisy's door; step left, Ivy is to the right
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + step("LEFT") + ["wait 30"] + talk("RIGHT", "p")
            + check("still_in_pallet", T.G.on_map("MAP_PALLET_TOWN"))
            + ["load @bedroom"] + debug_script(SCRIPT_ROUTE1) + step("UP", 4) + ["wait 60"] + talk("DOWN", "r")
            + expect("MAP_ROUTE1"))

def sc_dexnav():
    # Route 22: after the 6v6 Jenny gives you the DexNav (win or lose). Then
    # Start (DEBUG, POKEDEX, DEXNAV, ...) > DEXNAV on Route 22 itself.
    return (sc_route22() + ["mash B 200", "wait 60"] + check("dexnav_flag", T.G.flag_set("FLAG_RR_DEXNAV_GET"))
            + start_menu_item(2) + ["wait 120", "shot d001", "press RIGHT", "wait 20", "shot d002",
                                    "press DOWN", "wait 20", "shot d003", "press B", "wait 120", "shot d004"]
            + play_until("dexnav_closed", T.G.field_free(), 1500, key="B", every=30)
            + expect("MAP_ROUTE22"))

def sc_rename():
    # #71: Start > POKEMON > first Pokémon > SUMMARY, A on the info page renames.
    # Name it one letter, then rename again to an empty name: the species name
    # comes back (so it takes the new species name when it evolves).
    def rename(keys):
        return ["press A", "wait 120", "shot {}a"] + ["press B", "wait 8"] * 12 + keys + [
                "wait 20", "shot {}c", "press START", "wait 40", "press A", "wait 150", "shot {}b"]
    one = [l.format("n1") if "{}" in l else l for l in rename(["press A", "wait 10"])]
    empty = [l.format("n2") if "{}" in l else l for l in rename([])]
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + start_menu_item(2)
            + ["wait 90", "press A", "wait 30", "shot s001", "press A", "wait 120", "shot s002"]
            + one + empty + ["mash B 120", "wait 30", "shot s003"]
            + play_until("rename_closed", T.G.field_free(), 2000, key="B", every=30))

# Maps the report can name (anything else shows as a number)
MAP_NAMES = ["MAP_PALLET_TOWN", "MAP_PALLET_TOWN_PLAYERS_HOUSE_1F", "MAP_PALLET_TOWN_PLAYERS_HOUSE_2F",
             "MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", "MAP_PALLET_TOWN_RIVALS_HOUSE", "MAP_ROUTE1",
             "MAP_VIRIDIAN_CITY", "MAP_ROUTE22"]

SCENARIOS = {
    "intro": (sc_intro, "New game: the video call, ending in the bedroom"),
    "breakfast": (sc_breakfast, "Downstairs: breakfast with Delia, Ivy and Mimey"),
    "lab": (sc_lab, "Lab scene (test mode: the AI fights)"),
    "daisy": (sc_daisy, "Daisy's house: Muffin, the Town Map, the bet"),
    "route1": (sc_route1, "Route 1: Elm and Jenny, the Mewtwo fly-by"),
    "route22": (sc_route22, "Route 22: the 6v6 double (test mode)"),
    "lunch": (sc_lunch, "Viridian lunch by the Pokémon Center, Officer Jenny"),
    "banter": (sc_banter, "Pokédex banter preview (debug 7)"),
    "pokedex": (sc_pokedex, "Pokédex: every page of an entry, both ways"),
    "pewter": (sc_pewter, "After Brock: Elm and Jenny's rematch outside the Pokémon Center (test mode)"),
    "digletts": (sc_digletts, "After Brock: Diglett's Cave is closed by the Diglett crew (demo)"),
    "demo_end": (sc_demo_end, "After Brock: the way east out of Pewter is closed (demo)"),
    "gerald": (sc_gerald, "Gerald blocks surfing from Pallet to Cinnabar (demo)"),
    "pc": (sc_pc, "Start > PC: Pokémon storage from anywhere"),
    "dexnav": (sc_dexnav, "Route 22: Jenny gives the DexNav, then Start > DEXNAV there"),
    "rename": (sc_rename, "Rename from the summary; an empty name restores the species name (#71)"),
    "ivy": (sc_ivy, "Talking to Ivy: her lines change with the area (Pallet, Route 1)"),
    "guide": (sc_guide, "Pokédex > START > FIELD GUIDE on Route 1"),
    "rules": (sc_rules, "Options > Research Rules after the lunch (opens Elm's Rules on the map)"),
}


def version_and_commit():
    src = open(os.path.join(REPO, "src/rr_intro_call.c"), encoding="utf-8").read()
    m = re.search(r"v\d+\.\d+\.\d+", src)
    commit = subprocess.run(["git", "-C", REPO, "rev-parse", "--short=9", "HEAD"], capture_output=True, text=True).stdout.strip()
    dirty = subprocess.run(["git", "-C", REPO, "status", "--porcelain"], capture_output=True, text=True).stdout.strip()
    return (m.group(0) if m else "v?"), commit + ("-dirty" if dirty else "")


def main():
    version, commit = version_and_commit()
    base = os.path.join(WORKSPACE, "test-runs") if os.path.isdir(WORKSPACE) else os.path.join(REPO, "build/test-runs")
    T.main(T.Suite(
        title=f"Smoke test: {version} ({commit})",
        repo=REPO,
        rom=os.path.join(REPO, "pokefirered.gba"),
        scenarios=SCENARIOS,
        out_dir=os.path.join(base, f"{datetime.datetime.now():%Y-%m-%d_%H%M}_{version}_{commit}"),
        runner=os.path.join(TOOLS, "rr_run"),
        first="intro",
        progress_var=STORY,
        map_names=MAP_NAMES,
        setup_hint="run tools/research_red/test/setup_mgba.sh first",
    ), sys.argv[1:])


if __name__ == "__main__":
    main()
