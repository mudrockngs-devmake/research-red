# Automated smoke tests

A headless copy of mGBA plays the game from scripted button presses, takes screenshots, and checks the game's memory to see that each scene really finished. It runs on every build before the owner gets it.

## Run them
```
python3 tools/research_red/test/smoke.py            # all scenarios (about 5 minutes)
python3 tools/research_red/test/smoke.py lunch guide # just these
python3 tools/research_red/test/smoke.py --list      # what's covered
RR_SHOW_PEEKS=1 python3 tools/research_red/test/smoke.py ivy   # also print every memory peek (debugging)
```
Results go to `E:\Workspace\romhacks\research-red\test-runs\<date>_<version>_<commit>\`:
- `REPORT.md`: PASS / WARN / FAIL per scenario, with where each one ended (story step, map, team size)
- `<scenario>.png`: a contact sheet (a grid of up to 30 screenshots) to skim the scene in seconds
- `<scenario>\`: every screenshot, plus `timeout_*.png` / `stuck_*.png` when something went wrong

What the results mean:
- **FAIL**: the blue error screen, a scene that never reached its end within its time limit (`timeout_<scene>.png` shows where it was), or a fact check that came out wrong (e.g. the lab ended with no Pokémon).
- **WARN**: the screen froze for 20 seconds while buttons were being pressed (`stuck_<line>.png`; the note names the last screenshot before the freeze), or the scenario ended on a black screen.
- **PASS**: every scene reached its end and the facts check out. It still doesn't mean the scene *looks* right: open the contact sheets. The first run of these tests found a real bug (a portrait flashing in the previous speaker's colors) that way.

## How the checks work
- **Wait-until**: a scene presses A (or B) until the game says it's over: story step set and the player free to walk (on the map, no script running, no text box). Longer dialogue just takes longer; a scene that never ends fails at its time limit instead of quietly passing.
- **Memory checks**: after a scene, `expect(map, story, team)` reads the game's memory and compares.
- **Stuck detector**: the runner fingerprints every frame. If the picture doesn't change for 1200 frames (20 seconds) while buttons are pressed, it reports "stuck". Standing still on an indoor map is normal, so that case is ignored.
- `game_memory.py` turns names into addresses: RAM symbols come from `pokefirered.elf`, and struct offsets and constants (`FLAG_*`, `MAP_*`, `VAR_*`) come from compiling a tiny C file against the game's headers, so they stay right when the code changes. Nothing to update by hand.

## What's covered
| Scenario | How it gets there |
|---|---|
| intro | New game, the whole call, ends in the bedroom (saved as the starting point for the rest) |
| breakfast | Walks downstairs |
| lab | Debug 9 (test mode) + debug 1, then the lab scene and battles |
| daisy | Debug 2, steps into Daisy's house, the whole scene |
| route1 | Debug 8, the Route 1 scene and the Mewtwo fly-by |
| route22 | Debug 4 + test mode, the 6v6 double and its banter |
| lunch | Debug 6, talks to Elm by the Pokémon Center: the lunch and Officer Jenny |
| banter | Debug 2 + 7, the Pokédex banter preview |
| pokedex | Debug 2, Start > Pokédex, every page of Bulbasaur both ways (the Evo page crashed before v0.0.15) |
| pewter | Test mode + debug 10, into the Pewter Pokémon Center: Elm and Jenny's rematch (story 8) |
| demo_end | Debug 10 (after Brock), walks to the east exit: the Gym guide turns you back (demo) |
| gerald | Debug 2, surfs south from Pallet: Gerald blocks the sea route (demo) |
| pc | Debug 2, Start > PC, into Pokémon storage and back out |
| guide | Debug 3, walks onto Route 1, Pokédex > START > FIELD GUIDE |
| rules | The lunch, then Options > Research Rules: change a rule, close, the call with Elm and Jenny plays (#45); reopen and close unchanged: no call |
| dexnav | Route 22: the 6v6, Jenny gives the DexNav, then Start > DexNav there |
| ivy | Talk to Ivy in Pallet and on Route 1 (area lines) |
| rename | Summary > A: rename to one letter, then to an empty name (the species name comes back) |
| wild | Pace in Route 1 grass until a wild battle, look at the opponent's type icon, run away |
| evolve | Test mode (Lv 30): Squirtle shows EVOLVE in the party menu; evolves into Wartortle; LEARN MOVES > LEVEL MOVES lists Squirtle's moves at any level |
| pager_fly | Bag > Ranch Pager > Fly: the Fly map opens with no flyer in the team; Oak's Pidgeot flies to Pallet Town |
| pager_other | Ranch Pager > Flash outside a cave (not needed), then Teleport: Yes, Oak's Abra takes you back |
| pager_select | Register the Ranch Pager in the Bag; SELECT on the map opens its menu, B closes it |
| pager_dig | After Brock, inside Diglett's Cave: Ranch Pager > Dig > Yes, Oak's Diglett digs you out to Route 2 |
| lab_aide | After the lab: walk into the lab, the aide's topic menu, the Ranch Pager explanation, close |
| forest | After Brock: walk (no wild encounters on the way) into Viridian Forest, wander until a battle: the forest background |
| oak_mons | Oak's Pager Pokémon: talk to Tauros in the lab yard, pass Lapras and Gyarados on the shore, Pikachu, Farfetch'd, Abra and Machop in the lab |
| rest | At home: Delia > Rest till night (clock 20:00, Start menu "Night 20:00"), then Rest till morning (7:00, "Day 7:00") |
| rest_center | Pewter Pokémon Center: Nurse Joy > Rest till night (clock 20:00) |
| brock_rematch | After Brock: into the Gym (the Camper first), Brock offers a rematch: tier menu (1 badge: Lv. 21), his team at Lv 20-21 |
| delia_battle | After Brock: Fly to Pallet, home, Delia asks for a battle (Yes): Mimey and Gloom lead (1 badge, Lv 21) |
| oak_rematch | After Brock: Ranch Pager > Fly to Pallet, into the lab, Oak + Daisy double (1 badge: middle-stage starters, Lv 21) |
| tms | After the lab: the TM Case holds all 108 TMs and HMs (memory check); one screenshot per TM51-TM100 (names fit their rows, descriptions their box) and the longest name selected |

Battles use **test mode** (debug script 9): the AI plays your side, so nobody has to choose moves.

## First-time setup (once per machine)
```
tools/research_red/test/setup_mgba.sh
```
Downloads CMake and mGBA 0.10.5 into `~/romhacks/_tools/` (outside the repo) and builds `rr_run`. Takes a few minutes. Run it again if `~/romhacks/_tools` is ever deleted.

## Adding a scenario
Paths: `python3 tools/research_red/test/route_plan.py MAP_FROM x y MAP_TO [x y]` prints the steps (doors included). To walk without wild battles, wrap the walk in `T.set_flag("FLAG_RR_NO_ENCOUNTERS")` ... `T.clear_flag(...)` (the runner's `poke` command).

Scenarios are short functions in `smoke.py` that return script lines. Building blocks (from `tester.py`):
- `play_until(label, scene_end(step), max_frames, key="A")`: press a key until the scene sets story progress to `step` and hands control back; screenshots every 60 frames
- `expect(map_name="MAP_ROUTE1", story=5, party=1)`: check where the scene left us
- `check(label, G.flag_set("FLAG_X"))`: any other fact. `G` (`game_memory.py`) has `var_is`, `flag_set`, `flag_clear`, `on_map`, `party_count`, `field_free`, `textbox_closed`, `in_battle`; join conditions with `" && "`
- `debug_script(n)`: Start > DEBUG > Scripts > n
- `step("UP", 3)`: walk 3 tiles
- `advance(40, key="B")`: press B through text 40 times, screenshot each time
- raw commands for `rr_run`: `wait N`, `press KEYS`, `hold KEYS N`, `mash KEYS N`, `shot NAME`, `save PATH` / `load PATH`, `check`, `until`, `peek`, `stuck`, `idle`, `autoshot` (see the top of `rr_run.c`)

Use B to advance text when the player might be facing something (A talks to it). Mashing A also answers YES to "Give a nickname?" and types a name of A's ("Aaaaaaaaaa" in the lab screenshots): that's the test, not a bug.

After changing `rr_run.c`, rebuild it with `setup_mgba.sh` (it skips the downloads and only recompiles). Name screenshots so they sort in order (`s001`, `s002`...).

## Files
| File | What |
|---|---|
| `tools/research_red/test/rr_run.c` | The runner (C, links libmgba) |
| `tools/research_red/test/setup_mgba.sh` | Builds the runner |
| `tools/research_red/test/smoke.py` | Research Red's scenarios (the only file specific to this hack) |
| `tools/research_red/test/tester.py` | The reusable engine: building blocks, running, checks, report |
| `tools/research_red/test/game_memory.py` | Names (story step, flags, maps, team size) -> memory addresses |
| `tools/research_red/test/README.md` | How the pieces fit, and how to reuse the tester for another hack |
| `tools/research_red/test/contact_sheet.py` | Reads screenshots, makes the grids |
