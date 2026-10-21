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
from tester import advance, check, expect, play_until, ready, scene_end, start_menu_item, step  # noqa: E402

TOOLS = os.environ.get("RR_TOOLS", os.path.expanduser("~/romhacks/_tools"))
WORKSPACE = "/mnt/e/Workspace/romhacks/research-red"
STORY = "VAR_0x40FE"  # story progress, see CLAUDE.md

# Debug menu (Start > DEBUG > Scripts): see data/scripts/debug.inc
SCRIPT_LAB, SCRIPT_AFTER_LAB, SCRIPT_ROUTE1, SCRIPT_ROUTE22, SCRIPT_TEAM, \
    SCRIPT_LUNCH, SCRIPT_BANTER, SCRIPT_ROUTE1_ELM, SCRIPT_TEST_MODE, SCRIPT_AFTER_BROCK, SCRIPT_CATCH, SCRIPT_BIKE = range(1, 13)


def debug_script(n):
    """Start > DEBUG > Scripts > script n."""
    lines = ["press START", "wait 20", "press A", "wait 20"]
    lines += ["press DOWN", "wait 6"] * 5          # Scripts is the 6th entry
    lines += ["press A", "wait 20"]
    lines += ["press DOWN", "wait 6"] * (n - 1)
    lines += ["press A", "wait 240"]
    return lines


def after_brock_test_mode():
    """Debug 10 (after Brock), THEN test mode: debug 10 hands out a fresh
    Lv 12 team, so test mode has to come second to raise it to Lv 30
    (the other way round the AI fights at Lv 12 and can lose)."""
    return debug_script(SCRIPT_AFTER_BROCK) + debug_script(SCRIPT_TEST_MODE) + ["mash B 40"]


# --- scenarios ------------------------------------------------------------
# Each returns script lines. "@bedroom" is the state saved at the end of the
# intro (a new game standing in the bedroom); the others start from it.
def walk_via(label, start, *points, max_frames=600):
    """Walk in straight lines through each point, tapping the direction until
    the player is really there: a wandering NPC in the way only costs time,
    instead of throwing every later step off."""
    out, (x, y) = [], start
    for i, (nx, ny) in enumerate(points):
        key = "LEFT" if nx < x else "RIGHT" if nx > x else "UP" if ny < y else "DOWN"
        # the position changes when a step starts: let it finish
        out += [f"until {label}{i} {max_frames} {key} {T.G.player_at(nx, ny)}", "wait 20"]
        x, y = nx, ny
    return out


def on_title():
    """Our title screen (#12) is up: its task is the first one."""
    return f"{T.G.sym('gTasks'):#x}:4 == {T.G.sym('Task_Title') | 1:#x}"


def boot_to_title():
    """From power-on: START skips the Mareep splash and FireRed's opening
    until the title screen is up (#12)."""
    return [f"until title 4000 START {on_title()}", "wait 60"]


def sc_title():
    # #12: the Mareep splash (no buttons: it walks in, sparks, the credit
    # fades in), the title screen, then New Game: Slowking's two notices,
    # then the intro call
    cb2 = T.G.sym("gMain") + T.G.const("offsetof(struct Main, callback2)")
    in_call = f"{cb2:#x}:4 == {T.G.sym('CB2_Call') | 1:#x}"
    return (["wait 150", "shot t001", "wait 90", "shot t002", "wait 90", "shot t003", "wait 70", "shot t004",
             "wait 50", "shot t005", "wait 60", "shot t005b"]
            + boot_to_title() + ["shot t006", "wait 40", "shot t007", "press START", "wait 150", "shot t008",
                                 "press A", "wait 120", "shot t009", "press A", "wait 40", "shot t010", "press A"]
            + [f"until call 600 - {in_call}", "wait 60", "shot t011"])


def sc_intro():
    lines = boot_to_title() + ["press START", "wait 150"]
    lines += play_until("bedroom", f"{T.G.on_map('MAP_PALLET_TOWN_PLAYERS_HOUSE_2F')} && {T.G.field_free()}",
                        12000, every=270)
    lines += ["wait 200", "mash B 120", "wait 60", "shot s999"]
    lines += expect("MAP_PALLET_TOWN_PLAYERS_HOUSE_2F", story=0)
    return lines + ["save @bedroom"]


def sc_intro_skip():
    # Test builds: START during the new-game call skips it as Maple (male)
    bedroom = f"{T.G.on_map('MAP_PALLET_TOWN_PLAYERS_HOUSE_2F')} && {T.G.field_free()}"
    cb2 = T.G.sym("gMain") + T.G.const("offsetof(struct Main, callback2)")
    in_call = f"{cb2:#x}:4 == {T.G.sym('CB2_Call') | 1:#x}"
    return (boot_to_title() + ["press START", f"until call 3000 START {in_call}",
             "wait 200", "shot k001", "press START", "wait 30", "shot k002"]
            + play_until("bedroom", bedroom, 2500, key="B", every=120)
            + check("male", f"{T.G.sym('gSaveBlock2Ptr'):#x}:4 != 0"))


def sc_breakfast():
    return (["load @bedroom"] + step("RIGHT", 4) + step("UP", 5) + ["hold LEFT 20", "wait 60", "shot s000"]
            + play_until("breakfast", scene_end(1), 6000)
            + expect("MAP_PALLET_TOWN_PLAYERS_HOUSE_1F", story=1))


def sc_portraits():
    # #76: the portrait stays while a finished line waits for a button (it
    # used to vanish on every last page). Breakfast: the first six lines all
    # have a speaker; let each finish printing, check, then press A.
    portrait_up = f"{T.G.sym('sPortraitIdPlus1'):#x}:1 != 0"
    box_open = f"{T.G.sym('sBoxOpen'):#x}:1 != 0"
    lines = ["load @bedroom"] + step("RIGHT", 4) + step("UP", 5) + ["hold LEFT 20"]
    lines += [f"until first_line 1200 - {box_open}"]
    for i in range(1, 7):
        lines += ["wait 200", f"shot p{i:02d}", f"check portrait_{i} {portrait_up}", "press A", "wait 10"]
    return lines + play_until("breakfast", scene_end(1), 6000)

def sc_starter_menu():
    # #76: the starter list sits on the left with its picture box; the
    # speaker's portrait steps aside while you choose. Stops on the menu.
    menu_open = f"{T.G.sym('sDynamicMenuEventId'):#x}:1 == 2"   # DYN_MULTICHOICE_CB_RR_STARTER
    portrait_up = f"{T.G.sym('sPortraitIdPlus1'):#x}:1 != 0"
    return (["load @bedroom"] + debug_script(SCRIPT_LAB) + step("UP")
            + [f"until starter_menu 40000 A {menu_open}", "wait 40", "shot m001",
               f"check no_portrait {portrait_up.replace('!=', '==')}", "press DOWN", "wait 30", "shot m002"])


def sc_grass():
    # The grass scene outside the house (story 1 -> 2): Elm and Jenny at the
    # grass, Oak, then everyone walks to the lab; Jenny trails Elm (#83).
    # Breakfast leaves you in your chair (5,5) (#84): down to (5,8), left onto
    # the exit mat (4,8), down off it
    return (sc_breakfast() + step("DOWN", 3) + step("LEFT") + step("DOWN")
            + play_until("grass", scene_end(2), 20000, key="B", every=40)
            + expect("MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", story=2))


def sc_lab():
    return (["load @bedroom"] + debug_script(SCRIPT_TEST_MODE) + ["mash B 40"] + debug_script(SCRIPT_LAB)
            + ["shot s000"] + step("UP") + play_until("lab", scene_end(3), 60000)
            + expect(story=3, party=1))


def sc_lab_loss():
    # #86: lose the lab battle (both your Pokémon at 1 HP, memory poke once you
    # have them): Oak steps in with Pidgeot and Tauros against Elm and Jenny
    # (watch: Yes), the battle plays itself, Oak wins, the scene goes on.
    party = T.G.sym("gPlayerParty")
    hp = T.G.const("offsetof(struct Pokemon, hp)")
    size = T.G.const("sizeof(struct Pokemon)")
    low_hp = [f"poke {party + i * size + hp:#x}:2 1" for i in range(2)]
    return (["load @bedroom"] + debug_script(SCRIPT_LAB) + step("UP")
            + play_until("two_mons", T.G.party_count(2), 40000, key="A", every=200)
            + low_hp
            + play_until("lab_lost", scene_end(3), 60000, key="A", every=120)
            + expect(story=3, party=2))


def sc_daisy():
    # Debug 2 leaves us below Daisy's door (story 3): step in, the scene plays
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + ["shot s000"] + step("UP")
            + play_until("daisy", scene_end(4), 12000, key="B", every=30)
            + expect("MAP_PALLET_TOWN_RIVALS_HOUSE", story=4)
            # then talk to Daisy at the head of the table (6,3) from (5,3): you
            # end the scene in your chair (5,5) (#84); her own lines, with portrait
            + step("UP", 2) + ["press RIGHT 2", "wait 10", "press A", "wait 40", "shot t001",
                               "press A", "wait 60", "shot t002", "mash B 120", "shot t003"])


def sc_route1():
    # Debug 8 leaves us at (19,6): walk up into the trigger row (y 4), as from the grass
    return (["load @bedroom"] + debug_script(SCRIPT_ROUTE1_ELM) + ["shot s000"] + step("UP", 2)
            + play_until("route1", scene_end(5), 6000, key="B", every=24)
            + advance(20, key="B", every=24, prefix="t")   # the Mewtwo fly-by
            + expect("MAP_ROUTE1", story=5))


def sc_route22():
    return (["load @bedroom"] + debug_script(SCRIPT_ROUTE22) + debug_script(SCRIPT_TEST_MODE) + ["mash B 40", "shot s000"]
            + step("LEFT") + play_until("route22", scene_end(6), 60000)   # a 6v6 double fought by the AI: long
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


def sc_dex_replay():
    # #4: SELECT on Bulbasaur's entry replays its banter; then the entry is back
    lines = ["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + start_menu_item(1)
    lines += ["wait 90", "press A", "wait 120", "shot r00", "press SELECT", "wait 40", "shot r01"]
    for i in range(2, 6):
        lines += ["press A", "wait 40", f"shot r{i:02d}"]
    return lines + play_until("dex_closed", T.G.field_free(), 2000, key="B", every=30)

def sc_pokedex():
    # Start menu after the lab: DEBUG, POKEDEX, GUIDE. Open Bulbasaur and flip through
    # every page both ways (the Evo page used to crash, #19), then an unseen entry.
    pages = []
    for i, key in enumerate(["A"] + ["RIGHT"] * 7 + ["LEFT"] * 7 + ["B", "DOWN", "A", "RIGHT", "B", "B"], 1):
        pages += [f"press {key}", "wait 120", f"shot p{i:03d}"]
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + start_menu_item(1) + ["wait 60", "shot p000"]
            + pages + expect(story=3))


def mark_caught(national_num):
    """Test setup: mark a species seen and caught in the Pokédex (memory poke)."""
    lines = []
    for field in ("dexSeen", "dexCaught"):
        off = T.G.const(f"offsetof(struct SaveBlock1, {field})") + (national_num - 1) // 8
        lines.append(f"poke {T.G._sb1(off, 1)} |{1 << ((national_num - 1) % 8)}")
    return lines


def sc_dex_numbers():
    # #77: the Kanto list is the 151 in order, then Johto with national
    # numbers (Magikarp 129, Cyndaquil 155, Totodile 158). Shots: the list's
    # end (d02) and Totodile's entry (d03, "No158").
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB)
            + mark_caught(129) + mark_caught(155) + mark_caught(158)
            + start_menu_item(1) + ["wait 90", "shot d01", "hold DOWN 1100", "wait 30", "shot d02",
                                    "press A", "wait 120", "shot d03", "press B", "wait 60"]
            + play_until("dex_closed", T.G.field_free(), 1500, key="B", every=30))


def sc_dex_area():
    # #20: Magikarp's Area page by day and evening shows Pallet's sea (the
    # daytime table the game borrows), not "AREA UNKNOWN"; at night it isn't there.
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + mark_caught(129)
            + start_menu_item(1) + ["wait 90", "hold DOWN 900", "wait 30", "press A", "wait 120",
                                    "press RIGHT", "wait 150", "shot a01", "press DOWN", "wait 60", "shot a02",
                                    "press DOWN", "wait 60", "shot a03"]
            + play_until("dex_closed", T.G.field_free(), 2000, key="B", every=30))


def sc_catch():
    # #5: after a catch, the banter plays over the new Pokédex entry (Ivy and
    # the player facing each other), then the nickname question; nothing is
    # left to play on the map afterwards. Debug 11: Poké Balls, wild Pidgey;
    # R throws the last used ball (a Poké Ball): throw until it's caught.
    throws = []
    for i in range(5):      # throw, then B through the messages (and, once caught, the banter)
        throws += ["press R", "wait 30"] + sum([["press B", "wait 70", f"shot c{i}{j:02d}"] for j in range(10)], [])
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + debug_script(SCRIPT_CATCH)
            + play_until("battle", T.G.in_battle(), 2000, key="B", every=60)
            + ["wait 300", "shot c001"] + throws
            + play_until("back_on_map", T.G.field_free(), 3000, key="B", every=60)
            + ["wait 120"] + check("no_map_banter", T.G.field_free()) + expect(party=2))

def sc_bike():
    # #80: on the bike the player keeps their own sprite and Ivy steps out of
    # view (still keeping up); getting off (debug 12 again), she's back behind you.
    # #28 too: no Red sprite in any state. Debug 2 leaves us at (15,8) in Pallet.
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + step("LEFT") + ["wait 30", "shot b001"]
            + debug_script(SCRIPT_BIKE) + ["wait 30", "shot b002"]
            + ["hold DOWN 24", "wait 10", "hold LEFT 40", "wait 20", "shot b003", "hold UP 24", "wait 20", "shot b004"]
            + debug_script(SCRIPT_BIKE) + ["wait 30", "shot b005"] + step("RIGHT", 2) + ["wait 20", "shot b006"]
            + check("back_on_foot", T.G.field_free()))

def sc_pewter():
    # After Brock: Elm and Jenny's rematch outside the Pokemon Center (#10).
    # Debug 10 leaves us below the door at (17,26); Elm stands at (16,27) with
    # Jenny behind him (16,28) (#83).
    return (["load @bedroom"] + after_brock_test_mode() + ["shot s000"]
            + step("LEFT") + ["wait 20", "press DOWN 6", "wait 20", "shot s001", "press A", "wait 30"]
            + play_until("pewter", scene_end(8), 45000)  # the autopilot battle can run long
            # A says Yes to "Want to come watch?": the Brock scene plays in the Gym (#69)
            + play_until("watched", T.G.var_is("VAR_RR_BROCK_WATCH", 2) + " && " + T.G.field_free(), 20000, key="A", every=60)
            + expect("MAP_PEWTER_CITY_GYM", story=8, party=1))


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
            + step("DOWN", 2) + step("RIGHT", 6) + step("UP", 3) + ready("cave") + ["shot s003"]
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
    # DEBUG, POKEDEX, GUIDE, POKEMON, PC: open it, enter Pokemon storage, back out.
    # #79: the PC's menus show on the map (it used to fade to black first):
    # p001 must show "Which PC should be accessed?" over the town
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + start_menu_item(4)
            + ["wait 60", "shot p001"] + T.not_black("p001") + ["press A", "wait 60", "mash A 60", "wait 90", "shot p002",
               "press A", "wait 90", "shot p003"]
            + play_until("closed", T.G.field_free(), 1500, key="B", every=30)
            + check("still_in_pallet", T.G.on_map("MAP_PALLET_TOWN")))


def sc_pc_items():
    # #96: the research grant's medicine and held items wait in the PC's item
    # storage (62 kinds: slot 61 filled). Start > PC > your PC > Withdraw Item.
    slot = T.G.const("offsetof(struct SaveBlock1, pcItems[61].itemId)")
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB)
            + check("pc_slot_61", f"{T.G._sb1(slot, 2)} != 0")
            + start_menu_item(4) + ["wait 60", "press DOWN", "wait 10", "press A", "wait 90", "shot i001",
                                    "press A", "wait 60", "press A", "wait 60", "shot i002",     # Item Storage
                                    "press A", "wait 120", "shot i003", "hold DOWN 300", "wait 20", "shot i004"]  # Withdraw Item
            + play_until("pc_closed", T.G.field_free(), 3000, key="B", every=40))


def sc_trainer_card():
    # #24: Start > (your name): the Trainer Card shows your picture, not Red's
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + start_menu_item(6)
            + ["wait 150", "shot c001", "press A", "wait 120", "shot c002"]
            + play_until("card_closed", T.G.field_free(), 2000, key="B", every=40))


def sc_tms():
    # #57: the grant fills the TM Case: 8 HMs + TM01-TM100 = all 108 slots,
    # so the last slot holds something. Then look: Start > BAG (after the
    # lab: DEBUG, POKEDEX, GUIDE, POKEMON, PC, BAG), Key Items, the TM Case (9th
    # key item, below the Ranch Pager, just above Cancel), OPEN, then one screenshot per new TM
    # (TM51-TM100: name fits the row, description fits the box) and the HMs.
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB)
            + check("tm_case_full", T.G.bag_slot_filled("TMsHMs", 107))
            + start_menu_item(5) + ["wait 90", "shot t001", "press RIGHT", "wait 30"]
            + ["press DOWN", "wait 8"] * 7 + ["shot t002", "press A", "wait 20", "shot t003", "press A", "wait 90",
                                              "shot t004"]
            + ["press DOWN", "wait 6"] * 49
            + sum([["press DOWN", "wait 10", f"shot t{n:03d}"] for n in range(51, 101)], [])
            # the longest name, selected: "No099 Dazzling Gleam is selected."
            + ["press UP", "wait 10", "press A", "wait 30", "shot t199", "press B", "wait 30"]
            + ["hold DOWN 300", "wait 30", "shot t200"]
            + play_until("closed", T.G.field_free(), 2000, key="B", every=30))


def open_ranch_pager(choice, prefix):
    """After the lab (debug 2): Start > BAG > Key Items > Ranch Pager (7th from the top,
    below the Berry Pouch) > USE, then pick menu entry `choice` (0 Fly,
    1 Flash, 2 Teleport, 3 Dig)."""
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB)
            + start_menu_item(5) + ["wait 90", "press RIGHT", "wait 30"] + ["press DOWN", "wait 8"] * 6
            + [f"shot {prefix}001", "press A", "wait 20", "press A", "wait 150", f"shot {prefix}002"]
            + ["press DOWN", "wait 8"] * choice + [f"shot {prefix}003", "press A", "wait 60"])


def sc_pager_fly():
    # #29: Ranch Pager > Fly opens the Fly map without a flyer in the team;
    # A on Pallet Town: Oak's Pidgeot flies us there
    return (open_ranch_pager(0, "f") + ["wait 120", "shot f004", "press A", "wait 30", "shot f005", "press A"]
            + play_until("landed", T.G.field_free(), 3000, key="B", every=20)
            + expect("MAP_PALLET_TOWN", story=3))


def sc_pager_other():
    # #29: Flash outside a cave says it isn't needed; Teleport asks, then
    # Oak's Abra takes us back to the last healing spot
    return (open_ranch_pager(1, "l") + ["wait 60", "shot l004"]
            + play_until("flash_closed", T.G.field_free(), 1500, key="B", every=30)
            + ["wait 30", "press START", "wait 20", "press B", "wait 20"]
            + start_menu_item(0) + ["wait 90", "shot t001", "press A", "wait 20", "press A", "wait 150", "shot t002"]
            + ["press DOWN", "wait 8"] * 2 + ["shot t003", "press A", "wait 90", "shot t004", "press A", "wait 60", "shot t005"]
            + play_until("teleported", T.G.field_free(), 3000, key="B", every=20))


def sc_pager_select():
    # #29: register the Ranch Pager (Bag > Key Items > Ranch Pager > REGISTER),
    # close the Bag, then SELECT opens the Pager's menu on the map; B cancels
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB)
            + start_menu_item(5) + ["wait 90", "press RIGHT", "wait 30"] + ["press DOWN", "wait 8"] * 6
            + ["press A", "wait 20", "press DOWN", "wait 10", "shot r001", "press A", "wait 30", "shot r002"]
            + play_until("bag_closed", T.G.field_free(), 1500, key="B", every=30)
            + ["wait 30", "press SELECT", "wait 150", "shot r003", "press B", "wait 60", "shot r004"]
            + play_until("pager_closed", T.G.field_free(), 1500, key="B", every=30))


def sc_pager_dig():
    # #29: Ranch Pager > Dig inside Diglett's Cave (the entrance, as in
    # "digletts"): Yes, Oak's Diglett digs us out to Route 2
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_BROCK)
            + step("DOWN", 9) + step("RIGHT", 4) + step("DOWN", 5) + step("DOWN", 12) + step("RIGHT", 2)
            + ["press DOWN 6", "wait 20", "press A", "wait 60"]
            + play_until("cut", T.G.field_free(), 1500, key="A", every=30)
            + step("DOWN", 2) + step("RIGHT", 6) + step("UP", 3) + ready("cave")
            + check("in_entrance", T.G.on_map("MAP_DIGLETTS_CAVE_NORTH_ENTRANCE"))
            # after Brock the Start menu has DEXNAV too: BAG is the 7th entry
            + start_menu_item(6) + ["wait 90", "press RIGHT", "wait 30"] + ["press DOWN", "wait 8"] * 6
            + ["shot d001", "press A", "wait 20", "press A", "wait 150"] + ["press DOWN", "wait 8"] * 3
            + ["shot d002", "press A", "wait 90", "shot d003", "press A", "wait 60", "shot d004"]
            + play_until("dug_out", T.G.field_free(), 3000, key="B", every=20)
            + expect("MAP_ROUTE2"))


def sc_rest():
    # #56: Delia offers Heal / Rest. Rest moves the clock to 8 PM (night),
    # then again to 7 AM (morning). The Start menu clock says "Night 20:00",
    # then "Day 7:00" (#27: no more garbled clock box). Debug 2 leaves us at
    # (15,8); home is the door at (6,7); Delia stands at (8,4).
    hour = f"{T.G.sym('gLocalTime') + 2:#x}:1"
    def rest(prefix):
        # A: Delia asks and the menu appears with her question; DOWN to Rest
        return (["press A", "wait 150", f"shot {prefix}1",
                 "press DOWN", "wait 10", f"shot {prefix}2", "press A", "wait 60"]
                + play_until(f"rested_{prefix}", T.G.field_free(), 3000, key="A", every=60))
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + step("LEFT", 9) + step("UP") + ready("home")
            + step("UP", 5) + step("RIGHT", 4) + ["press DOWN 6", "wait 20", "shot r000"]
            + rest("a") + check("night_20h", f"{hour} == 20")
            + ["press START", "wait 40", "shot r001", "press B", "wait 30"]
            + rest("b") + check("morning_7h", f"{hour} == 7")
            + ["press START", "wait 40", "shot r002", "press B", "wait 30"]
            + check("closed", T.G.field_free()))


def sc_rest_center():
    # #56: Nurse Joy in Pewter's Pokemon Center offers Heal / Rest / No thanks.
    # Debug 10 leaves us below the door at (17,26); the counter is straight up.
    hour = f"{T.G.sym('gLocalTime') + 2:#x}:1"
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_BROCK) + step("UP") + ready("center") + ["shot c000"]
            + step("UP", 4) + ["press UP 6", "wait 20", "press A", "wait 200", "press A", "wait 120", "shot c001",
                                "press DOWN", "wait 10", "shot c002", "press A", "wait 60"]
            + play_until("rested", T.G.field_free(), 4000, key="A", every=60)
            + check("night_20h", f"{hour} == 20") + ["shot c003"])


def fly_to_pallet_after_brock():
    """Debug 10 (after Brock, test mode), then Ranch Pager > Fly to Pallet.
    Lands at (6,8), in front of home."""
    return (["load @bedroom"] + after_brock_test_mode()
            + ["press START", "wait 20", "press B", "wait 20"] + start_menu_item(6)
            + ["wait 90", "press RIGHT", "wait 30"] + ["press DOWN", "wait 8"] * 6
            + ["press A", "wait 20", "press A", "wait 150", "press A", "wait 150"]
            + ["press DOWN", "wait 30"] * 7 + ["wait 30", "press A", "wait 90"]
            + play_until("landed", T.G.field_free(), 3000, key="START", every=400)
            + check("in_pallet", T.G.on_map("MAP_PALLET_TOWN")))


def sc_delia_battle():
    # #70: after Brock, home: Delia asks for a battle first (Yes), a double
    # with Mimey leading (1 badge: Lv 21 Mimey, Gloom)
    return (fly_to_pallet_after_brock() + step("UP") + ready("home") + step("UP", 5) + step("RIGHT", 4)
            + ["press DOWN 6", "wait 20", "press A", "wait 90", "shot b001", "press A", "wait 60", "shot b002"]
            + play_until("battle", T.G.in_battle(), 1500, key="A", every=40)
            + ["wait 300", "shot b003"]
            + play_until("battle_over", T.G.field_free(), 40000, key="A", every=120)
            + expect("MAP_PALLET_TOWN_PLAYERS_HOUSE_1F", story=7))


def sc_brock_rematch():
    # #52: after Brock (1 badge, test mode), back into the Gym: Brock offers a
    # rematch with a tier menu (1 badge: Lv. 21, Cancel); his Gym team, stronger.
    # Paths from a route search over the map data.
    return (["load @bedroom"] + after_brock_test_mode()
            + step("RIGHT", 5) + step("UP", 14) + step("LEFT", 14) + step("DOWN", 5) + step("RIGHT", 7) + step("UP")
            + ready("gym") + ["shot g001"] + check("in_gym", T.G.on_map("MAP_PEWTER_CITY_GYM"))
            # the Camper spots us on the way up: beat him first (autopilot)
            + step("UP", 8) + play_until("camper", T.G.in_battle(), 1500, key="A", every=40)
            + play_until("camper_over", T.G.field_free(), 30000, key="A", every=120)
            + step("UP", 8) + ["press UP 6", "wait 20", "shot g002", "press A", "wait 90", "shot g003",
                               "press A", "wait 90", "press A", "wait 120", "shot g004", "press A", "wait 60", "shot g005"]
            + play_until("battle", T.G.in_battle(), 1500, key="A", every=40)
            + ["wait 300", "shot g006"]
            + play_until("rematch_over", T.G.field_free(), 40000, key="A", every=120)
            + expect("MAP_PEWTER_CITY_GYM", story=7))


def sc_oak_rematch():
    # #53: after Brock (story 7, 1 badge, test mode), Ranch Pager > Fly to
    # Pallet, into the lab, talk to Oak: the Oak + Daisy double (tier 1:
    # middle-stage starters at Lv 21). Daisy stands by Oak.
    fly = (["press START", "wait 20"] + ["press B", "wait 20"] + start_menu_item(6)
           + ["wait 90", "press RIGHT", "wait 30"] + ["press DOWN", "wait 8"] * 6
           + ["press A", "wait 20", "press A", "wait 150", "press A", "wait 150", "shot k001"]
           + sum([["press DOWN", "wait 30", f"shot k1{i:02d}"] for i in range(7)], [])
           + ["wait 30", "press A", "wait 90", "shot k002"]   # Pallet Town
           + play_until("landed", T.G.field_free(), 3000, key="START", every=400)
           + check("in_pallet", T.G.on_map("MAP_PALLET_TOWN")))
    # from the landing spot (6,8): around to the lab door (16,13), up to Oak (6,4)
    walk = (["shot k003"] + step("DOWN") + step("RIGHT", 6) + step("DOWN", 5) + step("RIGHT", 4) + step("UP")
            + ready("lab") + ["shot k004"] + check("in_lab", T.G.on_map("MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB"))
            + step("UP", 8) + ["press UP 6", "wait 20", "shot k005"])
    battle = (["press A", "wait 60", "shot k006"]
              + play_until("battle", T.G.in_battle(), 1500, key="A", every=40)
              + ["wait 300", "shot k007"]
              + play_until("rematch_over", T.G.field_free(), 40000, key="A", every=120)
              + expect("MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", story=7))
    return (["load @bedroom"] + after_brock_test_mode()
            + fly + walk + battle)


def sc_lab_aide():
    # #72: the lab aide explains the research tools. Debug 2 leaves us at
    # (15,8); route-searched path to the lab door, then to the aide at (3,11).
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + step("LEFT", 3) + step("DOWN", 6) + step("RIGHT", 4)
            + step("UP") + ready("lab") + check("in_lab", T.G.on_map("MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB"))
            + step("UP") + step("LEFT", 2) + ["press LEFT 6", "wait 20", "press A", "wait 90", "shot l001", "press A", "wait 90",
                                               "press A", "wait 90", "shot l002",   # the topic menu
                                               "press A", "wait 90", "shot l003"]    # Ranch Pager
            + advance(4, key="A", every=90, prefix="l1")
            + ["shot l004", "press B", "wait 60", "shot l005"]
            + play_until("aide_done", T.G.field_free(), 2000, key="B", every=60)
            # #97: Change a Ball (10th topic): your first Pokémon into a Great Ball (2nd)
            + ["press LEFT 6", "wait 20", "press A", "wait 120", "press A", "wait 90", "press A", "wait 150", "shot l199"]  # greeting: 2 pages
            + ["press DOWN", "wait 10"] * 9 + ["shot l200", "press A", "wait 90",   # "Which Pokémon?"
                                              "press A", "wait 120", "shot l201",     # the party screen
                                              "press A", "wait 150", "shot l202",     # first Pokémon -> the ball list
                                              "press DOWN", "wait 10", "press A", "wait 90", "shot l203",   # Great Ball: "All done!"
                                              "press A", "wait 150", "shot l204"]
            # back in the aide's menu: "That's all" is the 11th topic (B doesn't close it)
            + ["press DOWN", "wait 10"] * 10 + ["press A", "wait 60"]
            + play_until("aide_done_2", T.G.field_free(), 3000, key="B", every=60))


def sc_forest():
    # #74: any battle in Viridian Forest uses the forest background. After
    # Brock (test mode), walk south into the forest (route from
    # tools/research_red/test/route_plan.py), wander until a battle starts.
    wander = []
    for _ in range(40):
        wander += ["hold DOWN 16", "wait 4", "hold UP 16", "wait 4", "hold RIGHT 16", "wait 4", "hold LEFT 16", "wait 4"]
    return (["load @bedroom"] + after_brock_test_mode()
            + T.set_flag("FLAG_RR_NO_ENCOUNTERS")   # no wild battles on the way (Route 2 grass)
            + step("RIGHT", 2) + step("DOWN", 5) + step("RIGHT") + step("DOWN", 11) + step("LEFT", 2) + step("DOWN", 11)
            + ready("gatehouse") + step("DOWN", 12) + ready("gate_exit") + step("DOWN", 2) + ready("forest") + ["shot v001"]
            + check("in_forest", T.G.on_map("MAP_VIRIDIAN_FOREST"))
            + step("DOWN", 6) + T.clear_flag("FLAG_RR_NO_ENCOUNTERS") + wander + check("in_battle", T.G.in_battle())
            + ["wait 240", "shot v002", "wait 120", "shot v003"]
            + play_until("battle_over", T.G.field_free(), 30000, key="A", every=120))


def sc_forest_charlie():
    # #9: walking to Charlie, Bug Catcher Sammy spots you first (test mode
    # wins): then you and Ivy talk about forest Pikachu. Path: route_plan.py.
    return (["load @bedroom"] + after_brock_test_mode() + T.set_flag("FLAG_RR_NO_ENCOUNTERS")
            + step("RIGHT", 2) + step("DOWN", 5) + step("RIGHT") + step("DOWN", 11) + step("LEFT", 2) + step("DOWN", 11)
            + ready("gatehouse") + step("DOWN", 12) + ready("gate_exit") + step("DOWN", 2) + ready("forest")
            + step("DOWN", 3) + step("RIGHT") + step("DOWN", 10) + step("RIGHT") + step("DOWN", 4) + step("RIGHT", 8)
            + step("UP", 17) + step("RIGHT") + step("UP", 4)
            + ["press UP 6", "wait 20", "shot f001", "press A", "wait 60"]
            + play_until("battle", T.G.in_battle(), 1500, key="A", every=40)
            + play_until("battle_over", f"{T.G.overworld()}", 30000, key="A", every=120)
            + sum([["press B", "wait 70", f"shot f1{i:02d}"] for i in range(8)], [])   # B: A would talk to people
            + play_until("talk_done", T.G.field_free(), 3000, key="B", every=60)
            + T.clear_flag("FLAG_RR_NO_ENCOUNTERS"))


def sc_samurai():
    # #9: the Samurai in the forest clearing (30,23) spots you; test mode wins;
    # then you and Ivy on being "from Pallet". The Bug Catchers on the way are
    # marked beaten (memory poke) so only he battles. Path: route_plan.py.
    beaten = sum([T.set_flag(f"TRAINER_FLAGS_START + TRAINER_BUG_CATCHER_{n}") for n in ("SAMMY", "CHARLIE", "ANTHONY", "DOUG", "RICK")], [])
    return (["load @bedroom"] + after_brock_test_mode() + T.set_flag("FLAG_RR_NO_ENCOUNTERS") + beaten
            + step("RIGHT", 2) + step("DOWN", 5) + step("RIGHT") + step("DOWN", 11) + step("LEFT", 2) + step("DOWN", 11)
            + ready("gatehouse") + step("DOWN", 12) + ready("gate_exit") + step("DOWN", 2) + ready("forest")
            + step("DOWN", 3) + step("RIGHT") + step("DOWN", 10) + step("RIGHT") + step("DOWN", 4) + step("RIGHT", 8)
            + step("UP", 17) + step("RIGHT", 8) + step("DOWN", 13) + step("RIGHT", 4)
            + play_until("battle", T.G.in_battle(), 1500, key="B", every=40)
            + play_until("battle_over", T.G.overworld(), 30000, key="A", every=120)
            + sum([["press B", "wait 70", f"shot m1{i:02d}"] for i in range(6)], [])
            + play_until("talk_done", T.G.field_free(), 3000, key="B", every=60)
            + T.clear_flag("FLAG_RR_NO_ENCOUNTERS"))


def sc_route2_wild():
    # Diagnosis: a wild battle on Route 2 after Brock WITHOUT test mode
    wander = []
    for _ in range(40):
        wander += ["hold DOWN 16", "wait 4", "hold UP 16", "wait 4"]
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_BROCK)
            + step("RIGHT", 2) + step("DOWN", 5) + step("RIGHT") + step("DOWN", 11) + step("LEFT", 2) + step("DOWN", 3)
            + wander + check("in_battle", T.G.in_battle())
            + ["wait 240", "shot q001", "press A", "wait 60", "shot q002", "press A", "wait 60", "shot q003"]
            + ["press B", "wait 20", "press A", "wait 60", "shot q004"]
            + play_until("ran", T.G.field_free(), 6000, key="A", every=40))


def sc_oak_mons():
    # #29: Oak's Ranch Pager Pokemon. Debug 2 leaves us at (15,8) in Pallet:
    # east to the lab yard (Tauros 21,10 and Pidgeot 21,12), talk to Tauros,
    # west along the shore (Lapras 7,17, Gyarados 10,17), into the lab
    # (Pikachu, Farfetch'd, Abra, Machop; hidden until the lab scene is over).
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + step("RIGHT", 5) + step("DOWN", 2)
            + ["wait 30", "shot o001", "press RIGHT", "wait 10", "press A", "wait 60", "shot o002", "press A", "wait 60",
               "shot o003", "mash B 120", "wait 30"]
            + step("DOWN", 5) + step("LEFT", 11) + ["wait 30", "shot o004"]
            + step("RIGHT", 7) + step("UP", 2) + ready("lab") + ["shot o005"] + step("UP", 4) + ["wait 30", "shot o006"]
            + expect("MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", story=3))


def sc_ambient():
    # #64: ambient Pokémon. Pallet's Dodrio (3,14): talk from below, its cry,
    # a line, and Dodrio is marked seen in the Pokédex. Then after Brock, the
    # Chansey beside Nurse Joy in Pewter's Pokémon Center (talk across the counter).
    dodrio = T.G.const("NATIONAL_DEX_DODRIO")
    seen_off = T.G.const("offsetof(struct SaveBlock1, dexSeen)") + (dodrio - 1) // 8
    seen = f"{T.G._sb1(seen_off, 1)} & {1 << ((dodrio - 1) % 8)}"
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB)
            # the Pallet woman wanders across this path (x 2-4, y 6-14): walk by waypoints
            + walk_via("to_dodrio", (15, 8), (11, 8), (11, 9), (4, 9), (4, 15), (3, 15))
            + ["press UP 6", "wait 20", "press A", "wait 90", "shot a001", "press A", "wait 60", "shot a002"]
            + play_until("dodrio_done", T.G.field_free(), 2000, key="B", every=40)
            + check("dodrio_seen", seen)
            + ["load @bedroom"] + debug_script(SCRIPT_AFTER_BROCK)
            + step("UP") + ready("center") + step("UP") + step("RIGHT") + step("UP", 3)
            + ["press UP 6", "wait 20", "press A", "wait 90", "shot a003"]
            + play_until("chansey_done", T.G.field_free(), 2000, key="B", every=40))


def sc_extras():
    # Emerald extras (v0.0.19): the first Potion you pick up shows its
    # description (and marks the Potion as seen in the save), and walking from
    # Pewter onto Route 2 shows the Emerald-style map name pop-up.
    potion = T.G.const("ITEM_POTION")
    flag_off = T.G.const("offsetof(struct SaveBlock3, itemFlags)") + potion // 8
    seen = f"*{T.G.sym('gSaveBlock3Ptr'):#x}+{flag_off:#x}:1 & {1 << (potion % 8)}"
    return (["load @bedroom"] + debug_script(SCRIPT_LUNCH)
            + step("LEFT", 12) + step("UP", 3) + step("LEFT", 3) + step("UP", 2) + step("LEFT", 3) + step("UP")
            + step("LEFT", 3) + step("UP", 17) + step("RIGHT", 8)
            + ["press RIGHT 6", "wait 20", "press A", "wait 120", "shot x001", "press A", "wait 90", "shot x002"]
            + play_until("potion", T.G.field_free(), 1500, key="B", every=40)
            + check("potion_seen", seen)
            + ["load @bedroom"] + debug_script(SCRIPT_AFTER_BROCK)
            + step("DOWN", 9) + step("RIGHT", 4) + step("DOWN", 5) + step("DOWN", 3)
            + ["wait 10", "shot x003", "wait 40", "shot x004"]
            + check("on_route2", T.G.on_map("MAP_ROUTE2")))


def sc_guide():
    return (["load @bedroom"] + debug_script(SCRIPT_ROUTE1) + step("UP", 4) + ["wait 60", "shot s000"]
            # Start > POKEDEX, then the Pokédex's START menu: BACK TO LIST, LIST TOP,
            # FIELD GUIDE, CLOSE. It closes the Pokédex and opens the Guide on the map.
            + start_menu_item(1) + ["wait 150", "shot s001", "press START", "wait 40",
                                    "press DOWN", "wait 10", "shot s001b", "press DOWN", "wait 10", "shot s002",
                                    "press A", "wait 180", "shot s003", "press RIGHT", "wait 20", "shot s004",
                                    "press B", "wait 40", "shot s005"]
            + check("guide_closed", T.G.field_free())
            # #91: Start > GUIDE opens it straight from the Start menu, over the map
            # (the menu remembers its cursor: still on POKEDEX, GUIDE is one down)
            + start_menu_item(1) + ["wait 120", "shot s006"] + T.not_black("s006")
            + check("guide_open", f"{T.G.sym('sGlobalScriptContextStatus'):#x}:1 != 2")
            + play_until("guide_closed_2", T.G.field_free(), 1500, key="B", every=30)
            + expect("MAP_ROUTE1"))


def sc_rules():
    # after the lunch: Start menu DEBUG, POKEDEX, GUIDE, DEXNAV, POKEMON, PC, BAG,
    # PLAYER, SAVE, OPTION. Options row 3 is Research Rules: A leaves Options
    # and opens Elm's Rules window on the map (#45).
    return (sc_lunch() + ["mash B 200", "wait 60"] + start_menu_item(9)
            + ["wait 60", "shot t001", "press DOWN", "wait 10", "press DOWN", "wait 10", "shot t002",
               "press A", "wait 120", "shot t003", "press DOWN", "wait 10", "press RIGHT", "wait 10", "shot t004",
               "press B", "wait 30", "shot t005"]
            # a rule changed: the call with Elm and Jenny plays, then the map (#45, #13)
            + ["wait 90", "shot t006"]
            + play_until("call_over", T.G.field_free(), 6000, key="A", every=40)
            + expect(story=7)
            # open the Rules again and close them unchanged: no call
            + start_menu_item(0) + ["wait 60", "press DOWN", "wait 10", "press DOWN", "wait 10", "press A", "wait 120",
                                    "shot t007", "press B", "wait 40", "shot t008"]
            + check("no_call", T.G.field_free()))


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
    # Start (DEBUG, POKEDEX, GUIDE, DEXNAV, ...) > DEXNAV on Route 22 itself.
    return (sc_route22() + ["mash B 200", "wait 60"] + check("dexnav_flag", T.G.flag_set("FLAG_RR_DEXNAV_GET"))
            + start_menu_item(3) + ["wait 120", "shot d001", "press RIGHT", "wait 20", "shot d002",
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
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + start_menu_item(3)
            + ["wait 90", "press A", "wait 30", "shot s001", "press A", "wait 120", "shot s002"]
            + one + empty + ["mash B 120", "wait 30", "shot s003"]
            + play_until("rename_closed", T.G.field_free(), 2000, key="B", every=30))

def sc_wild():
    # A wild single battle on Route 1 (#25: the opponent's types stay on screen
    # all battle): pace in the grass until something appears, look, run away.
    pace = []
    for _ in range(60):
        pace += ["hold LEFT 16", "wait 4", "hold RIGHT 16", "wait 4"]
    return (["load @bedroom"] + debug_script(SCRIPT_ROUTE1) + step("UP", 6) + pace
            + check("in_wild_battle", T.G.in_battle())
            + ["wait 200", "shot w001", "mash B 120", "wait 30", "shot w002", "press A", "wait 60", "shot w003",
               "press B", "wait 30", "press RIGHT", "wait 10", "press DOWN", "wait 10", "shot w004", "press A"]
            # keep choosing RUN (the cursor stays on it) until the escape works
            + play_until("ran_away", T.G.field_free(), 6000, key="A", every=40))

def sc_relearn():
    # #89: Start > POKEMON > first Pokémon > Learn Moves > TM Moves: the list
    # shows TM numbers; teach the first one (it has a free move slot), and
    # the list stays open for the next; then back out.
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + start_menu_item(3)
            + ["wait 90", "press A", "wait 30", "shot r001", "press DOWN", "wait 10", "press A", "wait 30", "shot r002",
               "press DOWN", "wait 10", "press A", "wait 150", "shot r003",   # TM Moves
               "press A", "wait 60", "shot r004", "press A", "wait 90", "shot r005",                  # teach? Yes
               "wait 120", "press A", "wait 60", "shot r006", "wait 60", "shot r007"]                 # learned: still in the list
            + ["press B", "wait 60", "press A", "wait 90"]                                              # Cancel, "give up?" Yes
            + play_until("closed", T.G.field_free(), 4000, key="B", every=60))


def sc_evolve():
    # #50: no automatic evolution; EVOLVE in the party menu when ready. Test
    # mode raises the team to Lv 30 (no evolving), so Squirtle can become
    # Wartortle: Start > POKEMON > Squirtle > EVOLVE (second entry).
    return (["load @bedroom"] + debug_script(SCRIPT_AFTER_LAB) + debug_script(SCRIPT_TEST_MODE) + ["mash B 60"]
            + start_menu_item(3) + ["wait 90", "press A", "wait 30", "shot e001", "press DOWN", "wait 10", "shot e002",
                                    "press A", "wait 120", "shot e003"]
            + advance(12, key="A", every=90, prefix="v")
            + play_until("evolved", T.G.field_free(), 4000, key="B", every=60)
            # #51: Wartortle's LEARN MOVES > LEVEL MOVES lists Squirtle's moves at any level
            # (first let the new species' Pokédex banter play out on the map)
            + ["wait 120", "mash B 400", "wait 60"] + play_until("banter_done", T.G.field_free(), 2000, key="B", every=30)
            # the Start menu remembers its cursor: still on POKEMON
            + start_menu_item(0) + ["wait 90", "shot e004", "press A", "wait 30", "shot e005",
                                    "press DOWN", "wait 10", "press A", "wait 30", "shot e006", "press A", "wait 120", "shot e007"]
            + sum([["press DOWN", "wait 8"] * 4 + [f"shot e1{i:02d}"] for i in range(6)], [])
            + ["press A", "wait 60", "mash A 60", "wait 90"]  # CANCEL, "Give up?" Yes
            + play_until("closed", T.G.field_free(), 6000, key="B", every=40))

# Maps the report can name (anything else shows as a number)
MAP_NAMES = ["MAP_PALLET_TOWN", "MAP_PALLET_TOWN_PLAYERS_HOUSE_1F", "MAP_PALLET_TOWN_PLAYERS_HOUSE_2F",
             "MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", "MAP_PALLET_TOWN_RIVALS_HOUSE", "MAP_ROUTE1",
             "MAP_VIRIDIAN_CITY", "MAP_ROUTE22", "MAP_ROUTE2", "MAP_VIRIDIAN_FOREST"]

SCENARIOS = {
    "intro": (sc_intro, "New game: the video call, ending in the bedroom"),
    "title": (sc_title, "Mareep splash, title screen, Slowking's notices on New Game, then the call (#12)"),
    "intro_skip": (sc_intro_skip, "Test builds: START skips the new-game call (as Maple)"),
    "breakfast": (sc_breakfast, "Downstairs: breakfast with Delia, Ivy and Mimey"),
    "grass": (sc_grass, "Outside after breakfast: Elm and Jenny at the grass, Oak, the walk to the lab (#83)"),
    "portraits": (sc_portraits, "Portraits stay up while a finished line waits for a button (#76)"),
    "starter_menu": (sc_starter_menu, "Lab: the starter list on the left, no portrait over it (#76)"),
    "lab": (sc_lab, "Lab scene (test mode: the AI fights)"),
    "lab_loss": (sc_lab_loss, "Lab scene, battle lost on purpose: Oak steps in with Pidgeot and Tauros and wins (#86)"),
    "daisy": (sc_daisy, "Daisy's house: Muffin, the Town Map, the bet"),
    "route1": (sc_route1, "Route 1: Elm and Jenny, the Mewtwo fly-by"),
    "route22": (sc_route22, "Route 22: the 6v6 double (test mode)"),
    "lunch": (sc_lunch, "Viridian lunch by the Pokémon Center, Officer Jenny"),
    "banter": (sc_banter, "Pokédex banter preview (debug 7)"),
    "catch": (sc_catch, "A catch: the banter over the new Pokédex entry, then the nickname (#5)"),
    "dex_replay": (sc_dex_replay, "Pokédex: SELECT on an entry replays its banter (#4)"),
    "pokedex": (sc_pokedex, "Pokédex: every page of an entry, both ways"),
    "dex_area": (sc_dex_area, "Pokédex Area page: Magikarp in Pallet's sea by day and evening (#20)"),
    "dex_numbers": (sc_dex_numbers, "Pokédex list: the 151 in order, then Johto by national number (#77)"),
    "pewter": (sc_pewter, "After Brock: Elm and Jenny's rematch outside the Pokémon Center (test mode), then watching them take on Brock (#69)"),
    "digletts": (sc_digletts, "After Brock: Diglett's Cave is closed by the Diglett crew (demo)"),
    "demo_end": (sc_demo_end, "After Brock: the way east out of Pewter is closed (demo)"),
    "bike": (sc_bike, "On the bike: your own sprite, Ivy out of view; off again, she's back (#80, #28)"),
    "gerald": (sc_gerald, "Gerald blocks surfing from Pallet to Cinnabar (demo)"),
    "pc": (sc_pc, "Start > PC: Pokémon storage from anywhere"),
    "dexnav": (sc_dexnav, "Route 22: Jenny gives the DexNav, then Start > DEXNAV there"),
    "rename": (sc_rename, "Rename from the summary; an empty name restores the species name (#71)"),
    "wild": (sc_wild, "A wild single battle on Route 1: types while choosing a move (engine default, #78), then run away"),
    "relearn": (sc_relearn, "Move relearner: TM list with numbers, teach one, the list stays open (#89)"),
    "evolve": (sc_evolve, "Evolve on demand: EVOLVE in the party menu (#50); pre-evolution moves to relearn (#51)"),
    "pc_items": (sc_pc_items, "The research grant's supplies in the PC's item storage (#96, #99, #100)"),
    "trainer_card": (sc_trainer_card, "The Trainer Card shows your own picture (#24)"),
    "tms": (sc_tms, "The TM Case after the lab: TM01-TM100 and the HMs (#57)"),
    "pager_fly": (sc_pager_fly, "Ranch Pager > Fly: the Fly map, Oak's Pidgeot flies to Pallet (#29)"),
    "pager_other": (sc_pager_other, "Ranch Pager > Flash (not dark), then Teleport with Oak's Abra (#29)"),
    "pager_select": (sc_pager_select, "Register the Ranch Pager, then SELECT opens it on the map (#29)"),
    "pager_dig": (sc_pager_dig, "Ranch Pager > Dig in Diglett's Cave: Oak's Diglett digs us out (#29)"),
    "rest": (sc_rest, "Delia: Rest till night, then till morning; the Start menu clock (#56, #27)"),
    "rest_center": (sc_rest_center, "Nurse Joy in Pewter: Rest till night (#56)"),
    "oak_rematch": (sc_oak_rematch, "Oak + Daisy rematch in the lab after Brock (#53)"),
    "delia_battle": (sc_delia_battle, "Delia's double battle at home after Brock (#70)"),
    "brock_rematch": (sc_brock_rematch, "Brock's rematch in the Gym, tier menu, his team at 1 badge (#52)"),
    "lab_aide": (sc_lab_aide, "The lab aide explains the research tools (#72)"),
    "forest": (sc_forest, "Viridian Forest: a battle there uses the forest background (#74)"),
    "forest_charlie": (sc_forest_charlie, "Viridian Forest: beat a Bug Catcher (Sammy), then you and Ivy talk (#9)"),
    "samurai": (sc_samurai, "Viridian Forest: the Samurai's battle, then you and Ivy (#9)"),
    "route2_wild": (sc_route2_wild, "Diagnosis: wild battle on Route 2 after Brock, no test mode"),
    "oak_mons": (sc_oak_mons, "Oak's Pager Pokemon: lab yard, shore, lab (#29)"),
    "ivy": (sc_ivy, "Talking to Ivy: her lines change with the area (Pallet, Route 1)"),
    "ambient": (sc_ambient, "Ambient Pokémon: Pallet's Dodrio (seen in the Pokédex), Pewter's Chansey (#64)"),
    "extras": (sc_extras, "Emerald extras: first-time item description (Viridian Potion, memory check), Route 2 map pop-up"),
    "guide": (sc_guide, "Pokédex > START > FIELD GUIDE on Route 1, then Start > GUIDE (#91)"),
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
