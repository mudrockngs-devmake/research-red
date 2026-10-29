#!/usr/bin/env python3
"""Research Red: the full playthrough (#18), power-on to everything in Pewter.

No debug skips: the Mareep splash, the title screen, Slowking's notices and the
intro call, then every story scene in order, walking between them by map data
(nav.py), the optional Oak + Daisy fight with a wild check after it (#106),
Viridian Forest, training, the Gym, then the Pewter rematch and Brock watch.
Still to add: Brock's rematch tiers, Delia, Elm and Jenny's rebattles (#127).

How it plays:
  - The AI plays your side against trainers (test mode's flag, no Lv 30 team:
    your real team, with a careful trainer's AI settings poked in at the start
    of each battle). Wild battles: the bot runs (if it can't, it fights with
    the first move that has PP, and sends the next healthy Pokémon when one
    faints). Battle animations off and the Set style, to save time. It heals
    at the Pokémon Centers in Viridian and Pewter.
  - Before the Forest: Quick Balls from Lab Supplies and one training round
    on Route 2 (catching up to six). Before the Gym: Rare Candies to the cap
    (Lv 14).
  - Every story question gets A (Yes). Your starter is the first one offered.
    A new move with four known: Yes, forget the first one.
  - Each leg starts from the state the leg before saved; the states are kept
    (test-runs/playthrough/<run>/states/*.ss: open them in mGBA to jump in).
  - Every scene check runs too (dialogue, leftovers, brightness, battles).

Output: PLAYTHROUGH.md (a row per leg: story step, where, team levels, money,
battles) - the real EXP curve (#110) - plus REPORT.md and contact sheets.

  python3 tools/research_red/test/playthrough.py            # the whole thing
  python3 tools/research_red/test/playthrough.py --singles  # trainers as singles (Research Rules)
  python3 tools/research_red/test/playthrough.py --list
  python3 tools/research_red/test/playthrough.py --from p07_route22 RUN_DIR
        (start at a leg, using the states of an earlier run in RUN_DIR)
"""
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import nav  # noqa: E402
import smoke  # noqa: E402
import tester as T  # noqa: E402
from tester import check, expect, play_until, scene_end, step  # noqa: E402

LEGS = {}


def leg(name, desc):
    def wrap(f):
        LEGS[name] = (f, desc)
        return f
    return wrap


def G():
    return T.G


def here(fallback):
    """Where the previous leg really ended (read from the game), else fallback."""
    return T.LAST_POSITION or fallback


def demo_maps():
    """Every map in the demo area, so a leg's end position can be named."""
    import route_plan
    route_plan.map_folder("MAP_PALLET_TOWN")
    areas = ("MAP_PALLET_TOWN", "MAP_ROUTE1", "MAP_VIRIDIAN", "MAP_ROUTE22", "MAP_ROUTE2", "MAP_PEWTER",
             "MAP_DIGLETTS_CAVE", "MAP_ROUTE3", "MAP_ROUTE21")
    return sorted(m for m in route_plan._by_const if m.startswith(areas))


# --- set-up on every leg --------------------------------------------------------
def careful_ai():
    """At the start of every battle (60 frames in, once the AI settings are
    made), give the player's battlers a careful trainer's AI flags (balance.py)."""
    g = G()
    ptr = g.sym("gAiThinkingStruct")
    off = g.const("offsetof(struct AiThinkingStruct, aiFlags)")
    flags = g.const("AI_FLAG_BASIC_TRAINER")
    lines = []
    for battler in (0, 2):
        at = off + battler * 8
        lines.append(f"trigger *{ptr:#x}+{at:#x}:4 {flags & 0xFFFFFFFF:#x} 60 {g.in_battle()}")
    return lines


# Research Rules "trainers": doubles (the default) or singles (--singles). Both
# runs go before every build (owner 2026-10-29): every trainer on the way must
# be a double in one and a single in the other (rr_checks battles_judge).
TRAINERS = "doubles"


def every_leg(run=True):
    """Lines at the start of each leg (after its load). run: run from wild
    battles (False on training legs: fight them)."""
    g = G()
    # battles on the way play out; any script that stops the walk (a trainer
    # spotting you, the sign lady's lesson) gets A through its text
    talking = f"{g.overworld()} && {g.sym('sGlobalScriptContextStatus'):#x}:1 != {g.const('script_shutdown')}"
    # a level-up move with four known: Yes, forget one; A on the summary
    # screen picks the first move (sMonSummaryScreen is set only while it's open)
    summary = f"{g.sym('sMonSummaryScreen'):#x}:4 != 0"
    # the lab's watched battle switches "the AI plays your side" off again: on every leg
    # a lost battle: "<you> scurried back home..." (a task on a black screen)
    tasks, tsize = g.sym("gTasks"), g.const("sizeof(struct Task)")
    whiteout = " || ".join(f"{tasks + i * tsize:#x}:4 == {g.sym('Task_RushInjuredPokemonToCenter') | 1:#x}"
                           for i in range(16))
    # (singles: cleared on every leg, since the Rules at Daisy's set it)
    rules = (T.clear_flag("FLAG_RR_FORCE_DOUBLES") if TRAINERS == "singles" else []) + [f"# trainers {TRAINERS}"]
    return (rules + T.set_flag("FLAG_RR_AI_VS_AI") + careful_ai() + party_menu_guards()
            + ([] if run else blocked_move_guards() + catch_guards()) + wild_action(run) + move_menu_guards()
            + [f"guard A {whiteout}", f"guard A {g.in_battle()} || {talking} || {summary}",
                            smoke.rr_checks.TEST_MODE_MARK])


def wild_action(run):
    """Wild battles, the action menu (0 Fight 1 Bag / 2 Pokémon 3 Run; the
    cursor stays where the last battle left it):
      run: Run, like a player heading for the next town (a lone starter
           pressing A through every one wears down and blacks out); A again
           after "Can't escape!"
      else (training): back to Fight, then A picks the first move with PP."""
    g = G()
    funcs, cursor = g.sym("gBattlerControllerFuncs"), g.sym("gActionSelectionCursor")
    # four controllers have a static HandleInputChooseAction: the player's is
    # the one in battle_controller_player.c, next to HandleInputChooseMove
    near = g.sym("HandleInputChooseMove")
    action = min(g.all_symbols["HandleInputChooseAction"], key=lambda a: abs(a - near))
    menu = (f"{funcs:#x}:4 == {action | 1:#x} && "
            f"{g.sym('gBattleTypeFlags'):#x}:4 !& {g.const('BATTLE_TYPE_TRAINER')}")
    moves = ((0, "DOWN"), (1, "DOWN"), (2, "RIGHT"), (3, "A")) if run else ((1, "LEFT"), (2, "UP"), (3, "LEFT"))
    return [f"guard {key} {menu} && {cursor:#x}:1 == {at}" for at, key in moves]


def blocked_move_guards():
    """Training legs: when the lead's move is Disabled or Encored, A on it
    only says it can't be used, again and again: run from that wild battle
    instead (the action menu's cursor to Run, 3). Must come before
    catch_guards and wild_action."""
    g = G()
    funcs, cursor = g.sym("gBattlerControllerFuncs"), g.sym("gActionSelectionCursor")
    near = g.sym("HandleInputChooseMove")
    action = min(g.all_symbols["HandleInputChooseAction"], key=lambda a: abs(a - near))
    vol = g.sym("gBattleMons") + g.const("offsetof(struct BattlePokemon, volatiles)")
    blocked = [f"{vol + at:#x}:1 & {mask:#x}" for member in ("disabledMove", "encoredMove")
               for at, mask in g.bitfield_bytes("Volatiles", member).items()]
    menu = (f"{funcs:#x}:4 == {action | 1:#x} && "
            f"{g.sym('gBattleTypeFlags'):#x}:4 !& {g.const('BATTLE_TYPE_TRAINER')}")
    # no brackets in the runner's conditions (&& binds tighter): one && group per blocked byte
    return [f"guard {key} " + " || ".join(f"{menu} && {cursor:#x}:1 == {at} && {b}" for b in blocked)
            for at, key in ((0, "DOWN"), (1, "DOWN"), (2, "RIGHT"), (3, "A"))]


NO_ATTACK = ("SPECIES_ABRA", "SPECIES_METAPOD", "SPECIES_KAKUNA", "SPECIES_MAGIKARP")   # not worth catching


def catch_guards():
    """Training legs: with fewer than six Pokémon and Quick Balls in the Bag
    (from Lab Supplies), throw one at every wild Pokémon, like a player
    filling the team before the Gym: the action menu's cursor to Bag (1),
    then in the Bag the Poké Balls pocket, its first slot, A (and A on Use).
    "Give it a nickname?": B (No). Must come before wild_action's guards."""
    g = G()
    funcs, cursor = g.sym("gBattlerControllerFuncs"), g.sym("gActionSelectionCursor")
    near = g.sym("HandleInputChooseMove")
    action = min(g.all_symbols["HandleInputChooseAction"], key=lambda a: abs(a - near))
    wants = (f"{g.party_addr()} < 6 && {g.bag_slot_addr('pokeBalls', 0)} == {g.const('ITEM_QUICK_BALL')}")
    # not ones with no attacking move at these levels: a lone Teleport Abra lost
    # the singles playthrough's Brock fight (2026-10-29)
    foe = g.sym("gBattleMons") + g.const("sizeof(struct BattlePokemon)") + g.const("offsetof(struct BattlePokemon, species)")
    for sp in NO_ATTACK:
        wants += f" && {foe:#x}:2 != {g.const(sp)}"
    menu = (f"{funcs:#x}:4 == {action | 1:#x} && {wants} && "
            f"{g.sym('gBattleTypeFlags'):#x}:4 !& {g.const('BATTLE_TYPE_TRAINER')}")
    cb2 = g.sym("gMain") + g.const("main_callback2")
    bag = f"{cb2:#x}:4 == {g.sym('CB2_BagMenuRun') | 1:#x}"
    pos = g.sym("gBagPosition")
    pocket = f"{pos + g.const('offsetof(struct BagPosition, pocket)'):#x}:2"
    balls = g.const("POCKET_POKE_BALLS")
    row = f"{pos + g.const('offsetof(struct BagPosition, cursorPosition)') + 2 * balls:#x}:2"
    nick = f"*{g.sym('gBattlescriptCurrInstr'):#x}:1 == {g.const('B_SCR_OP_TRYGIVECAUGHTMONNICK')}"
    return ([f"guard B {g.in_battle()} && {nick}"]
            + [f"guard {key} {menu} && {cursor:#x}:1 == {at}" for at, key in ((0, "RIGHT"), (2, "UP"), (3, "UP"))]
            + [f"guard A {menu} && {cursor:#x}:1 == 1",
               f"guard RIGHT {bag} && {pocket} != {balls}",
               f"guard UP {bag} && {pocket} == {balls} && {row} != 0",
               f"guard A {bag}"])


def supplies_row(item):
    """An item's row in the Lab Supplies list (gRRLabSupplies, src/research_red.c)."""
    src = open(os.path.join(smoke.REPO, "src/research_red.c")).read()
    block = src.split("const u16 gRRLabSupplies[] =", 1)[1].split("};", 1)[0]
    return re.findall(r"\bITEM_\w+", re.sub(r"//[^\n]*", "", block)).index(item)


def lab_supplies(start_tile, center, item, tens, label, pocket, run=True):
    """The research grant's free shop on the Center's PC (Lab Supplies, the
    third PC entry): `tens` x 10 of `item` (RIGHT adds 10 to the amount).
    The guards would press A in these menus, so they're off for the visit and
    come back after. The item must land in the first slot of its Bag pocket
    (an empty pocket: the later guards pick slot 0). Ends at the PC, (11, 2)."""
    g = G()
    lines = nav.go(g, start_tile, center, (11, 2)) + ["guard clear", "trigger clear",
             "press UP 4", "wait 10", "press A 4", "wait 80", "shot pc_on",
             "press A 4", "wait 60", "shot pc_menu",
             "press DOWN 4", "wait 10", "press DOWN 4", "wait 10", "press A 4", "wait 60",
             "press A 4", "wait 120", "shot supplies", "press A 4", "wait 60"]  # the shop's greeting
    lines += ["press DOWN 4", "wait 24"] * supplies_row(item)
    lines += [f"shot {label}", "press A 4", "wait 120"] + ["press RIGHT 4", "wait 16"] * tens + ["shot how_many",
              "press A 4", "wait 90", "shot ok", "press A 4", "wait 90", "press A 4", "wait 60",
              "shot bought", "press B 4", "wait 90", "press B 4", "wait 60"]
    # B out of the list and the PC menu before the guards come back (they'd A into a PC)
    lines += play_until("pc_off", g.field_free(), 1200, key="B", every=40) + every_leg(run)
    return lines + check(label, f"{g.bag_slot_addr(pocket, 0)} == {g.const(item)}")


CANDY_LEVEL = 14   # the level cap with no badges (docs/DESIGN.md section 1)


def candy_guards(slot, target):
    """Rare Candies on party slot `slot` until it reaches `target`, through the
    menus a player uses: Start > Bag > Items pocket, its first slot (the
    candies) > Use > that Pokémon. A through level-ups, new moves (Yes, the
    summary screen's first move goes) and evolutions; B out once it's there."""
    g = G()
    party, size = g.sym("gPlayerParty"), g.const("sizeof(struct Pokemon)")
    lvl = f"{party + slot * size + g.const('offsetof(struct Pokemon, level)'):#x}:1"
    need = f"{lvl} != 0 && {lvl} < {target}"
    cb2 = g.sym("gMain") + g.const("main_callback2")
    # the Start menu is open: field controls locked by it, no script, its input handler set
    start_menu = (f"{g.overworld()} && {g.sym('sLockFieldControls'):#x}:1 != 0 && "
                  f"{g.sym('sGlobalScriptContextStatus'):#x}:1 == {g.const('script_shutdown')} && "
                  f"{g.sym('gMenuCallback'):#x}:4 == {g.sym('HandleStartMenuInput') | 1:#x}")
    menu_pos, actions = g.sym("sStartMenuCursorPos"), g.sym("sCurrentStartMenuActions")
    # MENU_ACTION_* is an enum inside src/start_menu.c (no header): count it from there
    enum = open(os.path.join(smoke.REPO, "src/start_menu.c")).read().split("MENU_ACTION_", 1)[1]
    bag_action = re.findall(r"MENU_ACTION_\w+|^\w+", "MENU_ACTION_" + enum.split("}", 1)[0]).index("MENU_ACTION_BAG")
    not_bag = " || ".join(f"{start_menu} && {need} && {menu_pos:#x}:1 == {k} && "
                          f"{actions + k:#x}:1 != {bag_action}" for k in range(9))
    bag = f"{cb2:#x}:4 == {g.sym('CB2_BagMenuRun') | 1:#x}"
    pos = g.sym("gBagPosition")
    pocket = f"{pos + g.const('offsetof(struct BagPosition, pocket)'):#x}:2"
    items = g.const("POCKET_ITEMS")
    row = f"{pos + g.const('offsetof(struct BagPosition, cursorPosition)') + 2 * items:#x}:2"
    party_menu = f"{cb2:#x}:4 == {g.sym('CB2_UpdatePartyMenu') | 1:#x}"
    pslot = f"{g.sym('gPartyMenu') + g.const('offsetof(struct PartyMenu, slotId)'):#x}:1"
    evolving = f"{cb2:#x}:4 == {g.sym('CB2_EvolutionSceneUpdate') | 1:#x}"
    summary = f"{g.sym('sMonSummaryScreen'):#x}:4 != 0"
    # FireRed's item-use scene ("Rare Candy was used on ...", the level-up) waits for A
    item_scene = f"{cb2:#x}:4 == {g.sym('CB2_PSA') | 1:#x}"
    return [f"guard A {evolving} || {summary} || {item_scene}",
            f"guard DOWN {party_menu} && {need} && {pslot} != {slot}",
            f"guard A {party_menu} && {need}",
            f"guard B {party_menu}",
            f"guard LEFT {bag} && {need} && {pocket} != {items}",
            f"guard UP {bag} && {need} && {row} != 0",
            f"guard A {bag} && {need}",
            f"guard B {bag}",
            f"guard DOWN {not_bag}",
            f"guard A {start_menu} && {need}",
            f"guard B {start_menu}",
            f"guard START {g.field_free()} && {need}",
            # level-up and move texts on the map after the Bag closes
            f"guard A {g.overworld()} && {g.sym('sGlobalScriptContextStatus'):#x}:1 != {g.const('script_shutdown')}"]


def candies(slot, target, frames=12000):
    g = G()
    party, size = g.sym("gPlayerParty"), g.const("sizeof(struct Pokemon)")
    lvl = f"{party + slot * size + g.const('offsetof(struct Pokemon, level)'):#x}:1"
    return (["guard clear"] + candy_guards(slot, target)
            + [f"until candy{slot} {frames} - {lvl} >= {target} && {g.field_free()} || {lvl} == 0 && {g.field_free()}",
               "guard clear"])


def move_menu_guards():
    """Wild battles: A picks the move under the cursor. If it has no PP left
    ("There's no PP left for this move!" and the menu again), step the cursor
    round the 2x2 move grid (0 1 / 2 3) to the next move first."""
    g = G()
    funcs = g.sym("gBattlerControllerFuncs")
    choose = g.sym("HandleInputChooseMove") | 1
    cursor = g.sym("gMoveSelectionCursor")
    mons, size = g.sym("gBattleMons"), g.const("sizeof(struct BattlePokemon)")
    pp = g.const("offsetof(struct BattlePokemon, pp)")
    lines = []
    for slot, key in ((0, "RIGHT"), (1, "DOWN"), (3, "LEFT"), (2, "UP")):
        conds = [f"{funcs + 4 * b:#x}:4 == {choose:#x} && {cursor + b:#x}:1 == {slot} && "
                 f"{mons + b * size + pp + slot:#x}:1 == 0" for b in (0, 2)]
        lines.append(f"guard {key} {' || '.join(conds)}")
    return lines


def party_menu_guards():
    """Wild battles are yours to play (the AI only plays your side against
    trainers; A = the first move). When your Pokémon faints the party menu
    opens: go DOWN past fainted ones, then A (and A on Shift)."""
    g = G()
    cb2 = g.sym("gMain") + g.const("main_callback2")
    menu = f"{cb2:#x}:4 == {g.sym('CB2_UpdatePartyMenu') | 1:#x}"
    slot = f"{g.sym('gPartyMenu') + g.const('offsetof(struct PartyMenu, slotId)'):#x}:1"
    party, size = g.sym("gPlayerParty"), g.const("sizeof(struct Pokemon)")
    hp = g.const("offsetof(struct Pokemon, hp)")
    down = [f"{menu} && {slot} == 0"]
    down += [f"{menu} && {slot} == {k} && {party + k * size + hp:#x}:2 == 0" for k in range(1, 6)]
    return [f"guard DOWN {' || '.join(down)}", f"guard A {menu}"]


def leg_end(name):
    """Record the state for PLAYTHROUGH.md and save it for the next leg."""
    g = G()
    party, size = g.sym("gPlayerParty"), g.const("sizeof(struct Pokemon)")
    lvl = g.const("offsetof(struct Pokemon, level)")
    money = g.const("offsetof(struct SaveBlock1, money)")
    lines = ["guard clear", "trigger clear", "wait 30", f"shot zz_{name}",
             f"peek pt_count {g.party_addr()}",
             f"peek pt_money *{g.sym('gSaveBlock1Ptr'):#x}+{money:#x}:4"]
    lines += [f"peek pt_lv{i} {party + i * size + lvl:#x}:1" for i in range(6)]
    return lines + [f"save @{name}"]


def start(prev, run=True):
    return [f"load @{prev}"] + every_leg(run)


def setup_flags():
    """Once, after the intro: the AI plays your side; animations off, Set style."""
    return T.set_flag("FLAG_RR_AI_VS_AI") + smoke.options_bits(2 | 4) + [smoke.rr_checks.TEST_MODE_MARK]


# --- the legs -------------------------------------------------------------------
@leg("p01_boot", "Power-on: Mareep splash, title, Slowking's notices, the intro call, the bedroom")
def p01():
    g = G()
    lines = smoke.sc_title()                       # splash and title (no skipping), notices, into the call
    lines += play_until("bedroom", f"{g.on_map('MAP_PALLET_TOWN_PLAYERS_HOUSE_2F')} && {g.field_free()}",
                        12000, every=270)
    lines += ["wait 200", "mash B 120", "wait 60"] + expect("MAP_PALLET_TOWN_PLAYERS_HOUSE_2F", story=0)
    return lines + setup_flags() + leg_end("p01_boot")


@leg("p02_breakfast", "Downstairs: breakfast")
def p02():
    return (start("p01_boot") + step("RIGHT", 4) + step("UP", 5) + ["hold LEFT 20", "wait 60"]
            + play_until("breakfast", scene_end(1), 6000) + expect("MAP_PALLET_TOWN_PLAYERS_HOUSE_1F", story=1)
            + leg_end("p02_breakfast"))


@leg("p03_grass", "Outside: Elm and Jenny at the grass, Oak, the walk to the lab")
def p03():
    return (start("p02_breakfast") + step("DOWN", 3) + step("LEFT") + step("DOWN")
            + play_until("grass", scene_end(2), 20000, key="B", every=40)
            + expect("MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", story=2) + leg_end("p03_grass"))


@leg("p04_lab", "The lab: the starters, you and Ivy vs Elm and Jenny, the Oak rounds (Yes to all)")
def p04():
    return (start("p03_grass") + step("UP") + play_until("lab", scene_end(3), 60000)
            + expect(story=3) + smoke.table_nap() + leg_end("p04_lab"))


@leg("p05_daisy", "To Daisy's house: tea and the bet")
def p05():
    return (start("p04_lab") + nav.go(G(), here(("MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", 6, 4)), "MAP_PALLET_TOWN", (15, 8))
            + step("UP") + play_until("daisy", scene_end(4), 12000, key="B", every=30)
            + expect("MAP_PALLET_TOWN_RIVALS_HOUSE", story=4) + leg_end("p05_daisy"))


@leg("p06_route1", "Route 1: Elm and Jenny, the walk, the Mewtwo fly-by")
def p06():
    return (start("p05_daisy") + nav.go(G(), here(("MAP_PALLET_TOWN_RIVALS_HOUSE", 5, 3)), "MAP_ROUTE1", (19, 6))
            + step("UP", 2) + play_until("route1", scene_end(5), 6000, key="B", every=24)
            + smoke.advance(20, key="B", every=24, prefix="t") + expect("MAP_ROUTE1", story=5)
            + leg_end("p06_route1"))


@leg("p07_route22", "Viridian, then Route 22: the 6v6 double with Elm and Jenny, straight into the lunch (#148)")
def p07():
    return (start("p06_route1") + nav.go(G(), here(("MAP_ROUTE1", 12, 2)), "MAP_ROUTE22", (34, 5))
            + step("LEFT") + play_until("route22", scene_end(7), 70000, key="B", every=50)
            + expect("MAP_VIRIDIAN_CITY", story=7) + leg_end("p07_route22"))


@leg("p08_lunch", "After the lunch: the road north is open (the old man's spot), then back to the Pokémon Center")
def p08():
    g = G()
    return (start("p07_route22") + nav.go(g, here(("MAP_VIRIDIAN_CITY", 32, 26)), "MAP_ROUTE2", (7, 76))
            + expect("MAP_ROUTE2") + nav.go(g, ("MAP_ROUTE2", 7, 76), "MAP_VIRIDIAN_CITY", (32, 26))
            + expect("MAP_VIRIDIAN_CITY", story=7) + leg_end("p08_lunch"))


@leg("p09_oak_daisy", "Optional: back to the lab for Oak and Daisy (their first fight), then a wild battle on Route 1 (#106)")
def p09():
    g = G()
    return (start("p08_lunch") + nav.go(g, here(("MAP_VIRIDIAN_CITY", 32, 26)), "MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", (6, 4))
            + ["press UP 6", "wait 20", "press A", "wait 60"]
            + play_until("oak_battle", g.in_battle(), 1500, key="A", every=40)
            + play_until("oak_over", g.field_free(), 40000, key="A", every=120)
            + one_wild_battle()
            + nav.wild_probe(g, ("MAP_PALLET_TOWN_PROFESSOR_OAKS_LAB", 6, 4), "MAP_ROUTE1", guarded=True)
            + [f"until probe_over 20000 A {g.field_free()}"] + T.clear_flag("FLAG_RR_NO_ENCOUNTERS")
            + leg_end("p09_oak_daisy"))


def one_wild_battle():
    """The wild probe needs one battle: once one starts, switch encounters off
    (a worn-out team pacing the grass would meet more and black out)."""
    addr, bit = G().flag_addr("FLAG_RR_NO_ENCOUNTERS")
    return [f"trigger {addr} |{bit} 0 {G().in_battle()}"]


def heal(start_tile, center):
    """Walk into a Pokémon Center, talk to the nurse across the counter
    (stand at (7, 4)), Yes to healing, A through her lines. Ends at (7, 4)."""
    g = G()
    return (nav.go(g, start_tile, center, (7, 4)) + ["press UP 2", "wait 10", "press A", "wait 30"]
            + play_until(f"healed_{center[4:].lower()}", g.field_free(), 3000, key="A", every=60)
            + check("team_healed", f"{G().sym('gPlayerParty') + G().const('offsetof(struct Pokemon, hp)'):#x}:2 > 0"))


TRAINING_LEVEL = 10        # the two leads' level before Viridian Forest (its Bug Catchers: Lv 9-11)
TRAINING_FRAMES = 300000   # at most (about 80 minutes of play)


@leg("p10_train", f"Heal in Viridian, Quick Balls from Lab Supplies, Route 2 grass (catching up to six) until the leads reach Lv {TRAINING_LEVEL}")
def p10_train():
    """A training round before Viridian Forest, as a player does it: heal at
    the Viridian Pokémon Center, fill the Bag with Quick Balls from Lab
    Supplies, then walk back and forth on two tall-grass tiles of Route 2,
    fighting wild Pokémon (and catching them while the team has fewer than
    six) until the two lead Pokémon reach TRAINING_LEVEL. A team that lost
    the opening fights is still at Lv 7 here and blacks out in the Forest
    without it."""
    g = G()
    center = "MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F"
    a, b = nav.grass_route((center, 11, 2), "MAP_ROUTE2")
    d_out = "RIGHT" if b[1] > a[1] else "LEFT"
    d_back = "LEFT" if d_out == "RIGHT" else "RIGHT"
    party, size = g.sym("gPlayerParty"), g.const("sizeof(struct Pokemon)")
    lvl = g.const("offsetof(struct Pokemon, level)")
    trained = " && ".join(f"{party + k * size + lvl:#x}:1 >= {TRAINING_LEVEL}" for k in (0, 1))
    lines = (start("p09_oak_daisy", run=False) + heal(here(None), center)
             + lab_supplies((center, 7, 4), center, "ITEM_QUICK_BALL", 5, "quick_balls", "pokeBalls", run=False)
             + nav.go(g, (center, 11, 2), None, goal=lambda n: n == a, label="train"))
    # pacing: a tap on one tile turns (or steps) toward the other
    lines += [f"guard {d_out} {g.field_free()} && {g.player_at(a[1], a[2])}",
              f"guard {d_back} {g.field_free()} && {g.player_at(b[1], b[2])}",
              f"until trained {TRAINING_FRAMES} A {trained} && {g.field_free()}"]
    return lines + leg_end("p10_train")


@leg("p10_forest", "Heal in Viridian, then Route 2 and Viridian Forest (Bug Catchers, the Samurai) to Pewter City")
def p10():
    center = "MAP_VIRIDIAN_CITY_POKEMON_CENTER_1F"
    return (start("p10_train") + heal(here(None), center)
            + nav.go(G(), (center, 7, 4), "MAP_PEWTER_CITY", (17, 26))
            + expect("MAP_PEWTER_CITY") + leg_end("p10_forest"))


@leg("p11_candies", f"Heal in Pewter, Rare Candies from Lab Supplies: the whole team to the cap (Lv {CANDY_LEVEL})")
def p11_candies():
    """The rest of the way to Brock the quick way a player can take: Rare
    Candies from Lab Supplies on each Pokémon up to the cap, then one more on
    the lead, which must do nothing (the cap stops candies, docs/DESIGN.md)."""
    g = G()
    center = "MAP_PEWTER_CITY_POKEMON_CENTER_1F"
    party, size = g.sym("gPlayerParty"), g.const("sizeof(struct Pokemon)")
    lvl = g.const("offsetof(struct Pokemon, level)")
    lines = (start("p10_forest") + heal(here(("MAP_PEWTER_CITY", 17, 26)), center)
             + lab_supplies((center, 7, 4), center, "ITEM_RARE_CANDY", 6, "rare_candies", "items"))
    for k in range(6):
        lines += candies(k, CANDY_LEVEL)
    # at the cap: one more candy on the lead, by hand (the candy guards would
    # loop Bag -> Pokémon -> Bag); it must say it won't have any effect.
    # The Start menu's cursor is still on Bag from the candies.
    lines += (["guard clear", "press START", "wait 30", "shot cap_start", "press A", "wait 90", "press A", "wait 30", "press A", "wait 90", "shot cap_choose",
                 "press A", "wait 60", "shot cap_no_effect"]
              + play_until("menus_closed", g.field_free(), 1200, key="B", every=30))
    lines += check("cap_holds", f"{party + lvl:#x}:1 == {CANDY_LEVEL}")
    lines += [ln for k in range(6) for ln in check(f"lv_{k}", f"{party + k * size + lvl:#x}:1 == {CANDY_LEVEL} || "
                                                   f"{party + k * size + lvl:#x}:1 == 0")]
    return lines + every_leg() + leg_end("p11_candies")


@leg("p11_brock", "Heal in Pewter, then the Gym: the Camper, then Brock")
def p11():
    g = G()
    center = "MAP_PEWTER_CITY_POKEMON_CENTER_1F"
    return (start("p11_candies") + heal(here(("MAP_PEWTER_CITY", 17, 26)), center)
            + nav.go(g, (center, 7, 4), "MAP_PEWTER_CITY_GYM", (6, 6))
            + ["press UP 6", "wait 20", "press A", "wait 60"]
            + play_until("brock_battle", g.in_battle(), 3000, key="A", every=40)
            + play_until("brock_over", g.field_free(), 60000, key="A", every=120)
            + check("badge", g.flag_set("FLAG_BADGE01_GET")) + leg_end("p11_brock"))


@leg("p12_pewter", "Outside the Pokémon Center: Elm and Jenny's rematch, then watching them take on Brock")
def p12():
    g = G()
    return (start("p11_brock") + nav.go(g, here(("MAP_PEWTER_CITY_GYM", 6, 6)), "MAP_PEWTER_CITY", (12, 24))
            + ["press DOWN 6", "wait 20", "press A", "wait 30"]   # Jenny (12,25), from above (the way from the Gym)
            + play_until("pewter", scene_end(8), 60000)
            + play_until("watched", g.var_is("VAR_RR_BROCK_WATCH", 2) + " && " + g.field_free(), 20000, key="A", every=60)
            + expect(story=8) + leg_end("p12_pewter"))


def main():
    global TRAINERS
    argv = sys.argv[1:]
    if "--singles" in argv:
        argv.remove("--singles")
        TRAINERS = "singles"
    run_from = None
    if "--from" in argv and len(argv) > argv.index("--from") + 2:
        run_from = argv.pop(argv.index("--from") + 2)    # the earlier run's folder (its states/)
    scenes = {name: (f, desc) for name, (f, desc) in LEGS.items()}
    version, commit = smoke.version_and_commit()
    out = os.path.join(smoke.WORKSPACE if os.path.isdir(smoke.WORKSPACE) else os.path.join(smoke.REPO, "build"),
                       "test-runs", "playthrough")
    import datetime
    run_dir = os.path.join(out, f"{datetime.datetime.now():%Y-%m-%d_%H%M}_{version}_{commit}"
                           + ("_singles" if TRAINERS == "singles" else ""))
    states = os.path.join(run_dir, "states")
    if run_from:
        import shutil
        os.makedirs(states, exist_ok=True)
        for f in os.listdir(os.path.join(run_from, "states")):
            shutil.copy(os.path.join(run_from, "states", f), states)
    T.main(T.Suite(
        title=f"Playthrough: {version} ({commit}), trainers as {TRAINERS}", repo=smoke.REPO,
        rom=os.path.join(smoke.REPO, "pokefirered.gba"), scenarios=scenes, out_dir=run_dir,
        runner=os.path.join(smoke.TOOLS, "rr_run"), progress_var=smoke.STORY, map_names=demo_maps(),
        setup_hint="run tools/research_red/test/setup_mgba.sh first", after_run=summary,
        always_start=smoke.rr_checks.ALWAYS_START, always_end=smoke.rr_checks.ALWAYS_END,
        judges=smoke.rr_checks.JUDGES, sequential=True, keep_states=states,
        timeout=1800), argv)


def summary(run_dir):
    """PLAYTHROUGH.md: a row per leg, from each leg's runner.log and battles.json."""
    rows = ["| Leg | Result | Story | Team levels | Money | Battles (curated) | Trainer singles / doubles |",
            "|---|---|---|---|---|---|---|"]
    report = open(os.path.join(run_dir, "REPORT.md")).read()
    for name, (_, desc) in LEGS.items():
        log = os.path.join(run_dir, name, "runner.log")
        if not os.path.exists(log):
            continue
        peeks = dict(re.findall(r"^PEEK (\w+) (\d+)$", open(log).read(), re.M))
        count = int(peeks.get("pt_count", 0))
        levels = ", ".join(peeks.get(f"pt_lv{i}", "?") for i in range(min(count, 6)))
        money = int(peeks.get("pt_money", 0))
        bpath = os.path.join(run_dir, name, "battles.json")
        battles = json.load(open(bpath)) if os.path.exists(bpath) else []
        cur = sum(1 for b in battles if b["curated"])
        tr = [b for b in battles if b["trainer"]]
        fmt = f"{sum(1 for b in tr if not b['double'])} / {sum(1 for b in tr if b['double'])}"
        m = re.search(rf"^\| {name}: .*? \| (\w+) \|", report, re.M)
        rows.append(f"| {name}: {desc} | {m[1] if m else '?'} | {peeks.get('story', '?')} | {levels} | "
                    f"{money} | {len(battles)} ({cur}) | {fmt} |")
    open(os.path.join(run_dir, "PLAYTHROUGH.md"), "w").write(
        f"# Playthrough (trainers as {TRAINERS})\n\nPower-on to Pewter with no debug skips; the AI plays your side. "
        "Team levels after each leg are the real EXP curve (#110).\n\n" + "\n".join(rows) + "\n")


if __name__ == "__main__":
    main()
