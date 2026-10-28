# Automated smoke tests

A headless copy of mGBA plays the game from scripted button presses, takes screenshots, and checks the game's memory to see that each scene really finished. It runs on every build before the owner gets it.

Live view: add `--watch` (`python3 tools/research_red/test/smoke.py --watch lab`) and the screenshots stream into the viewer page in the Windows browser as they're taken (tools/research_red/viewer, docs/ART_TOOLS.md).

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

## Checks on every scene (`rr_checks.py`)
Every smoke scene, matrix run and playthrough leg gets these, without the scene asking for them:
- **Dialogue** (`dialogue.py`, #120 #76): the runner records the message text, whose portrait is up, and the text box's state on every frame. FAIL if a "Name:" line shows someone else's portrait, if narration still shows a cast portrait, or if **a text box closes while its text is still printing** (a box that vanishes early). Signs you walk into and away from are fine (FireRed's walk-away feature). WARN if a speaker's portrait was skipped. This reads memory, so a screenshot taken a moment late can't fool it.
- **Leftovers**: when the scene hands control back, nothing a battle switches on for a while is still on ("no whiteout", the AI playing your side outside test mode).
- **Battles** (#128, #107): every battle's start goes into `battles.json` (single/double, trainer(s), team size). A curated fight (Research Red's own trainers, the Gym leaders) that starts as a single FAILs; two separate teams instead of one team of 6 WARNs.
- **Brightness**: the mean brightness of every screenshot (`brightness.json`). Scenes can demand a range or "one light level" (`rr_checks.bright`, `same_bright`; the `daynight` scene, #108).
- **Wild Pokémon**: a `nav.wild_probe` walks into grass, waits for a wild battle and checks it's a wild Pokémon from that map's table (#106).
- **Script coverage**: which event scripts ran (`coverage.json`). `coverage.py RUN_FOLDER [...]` adds runs up into `COVERAGE.md`: every label in `data/scripts/research_red/` that no test reaches. It's a guide, not an exact count: the script engine is checked once a frame (and its call stack, for text shown through FireRed's shared message scripts), so a label that runs and moves on within one frame can be missed.
- **Where it ended**: the player's map and tile, in the runner log (the playthrough starts each leg from there).

## The full playthrough (`playthrough.py`, #18)
Power-on to "everything done in Pewter", **no debug skips**: the Mareep splash, the title, Slowking's notices, the intro call, every story scene in order, walking between them by the map data (`nav.py`), wild Pokémon and trainers on the way, the Gym, the Pewter rematch and Brock watch, and the optional fights with a wild check after each. How it plays:
- **Trainer battles**: the AI plays your side with a careful trainer's settings (your real team, no test-mode Lv 30s).
- **Wild battles on the way**: it runs, like a player heading for the next town. **Training legs** before the Gym (heal at the Pewter Center, fight wild Pokémon on Route 2, three rounds) fight them instead: the first move with PP left, the next healthy Pokémon when one faints.
- It heals at the Viridian and Pewter Pokémon Centers; story questions get Yes; "forget a move?" gets Yes (the first move goes); a blackout gets pressed through. Battle animations are off and the battle style is Set.
- It doesn't catch Pokémon (yet), so it reaches Brock with two. First run (2026-10-28): Lv 10 and 12 after training, and Brock's six (Lv 12-14) won. That's a balance data point for #110 as much as a bot limit.
```
python3 tools/research_red/test/playthrough.py                      # the whole thing
python3 tools/research_red/test/playthrough.py --list
python3 tools/research_red/test/playthrough.py --from p08_lunch RUN_FOLDER   # restart at a leg
```
Results: `test-runs\playthrough\<date>_<version>_<commit>\`: `PLAYTHROUGH.md` (a row per leg: story step, team levels, money, battles: the real EXP curve, #110), `REPORT.md`, contact sheets, and `states\<leg>.ss`: **save states you can open in mGBA** to jump to any point of the playthrough. A FAIL stops the run (the next leg needs its state); fix it and restart there with `--from`. Save states only work with the build that made them: after a game change, play from the start (`--from` refuses otherwise).

What the bot does that a person would too: opens its way through the sign lady's Start-menu lesson, waits for wanderers to move, answers "forget a move?" with Yes (the first move goes). Walking plans come from the map data; people who stand somewhere their map data doesn't say are listed in `route_plan.MOVED_PEOPLE` (or a leg's `avoid=`).

## Settings matrix (`matrix.py`, #128)
Every curated fight (Route 22, Pewter, Oak + Daisy, Delia, Brock) under each Research Rules setting (default, trainers as singles, wild doubles). `MATRIX.md`: how each fight started. A curated fight must be a double under every setting.

## Asking the owner mid-run (viewer)
`viewer.py show SHOT.png --title "Does this look right?" --choices "Yes,No" --wait 10` pops the question up in the Windows browser and waits up to 10 minutes for the reply (printed; exit 2 if none came). `viewer.py answer STAMP` fetches a reply later. The live stream already shows every new screenshot of the newest run.

## How the checks work
- **Wait-until**: a scene presses A (or B) until the game says it's over: story step set and the player free to walk (on the map, no script running, no text box). Longer dialogue just takes longer; a scene that never ends fails at its time limit instead of quietly passing.
- **Memory checks**: after a scene, `expect(map, story, team)` reads the game's memory and compares.
- **Stuck detector**: the runner fingerprints every frame. If the picture doesn't change for 1200 frames (20 seconds) while buttons are pressed, it reports "stuck". Standing still on an indoor map is normal, so that case is ignored.
- **Fake clock**: the runner never reads the computer's clock. Every run starts at 2026-10-20 10:00 (morning) and time moves with the game, so a run plays out the same at 9 AM or 9 PM: day/night, the Rest menu and anything random come out identical each time. (Before this, the Rest tests failed in the evening and battles were won or lost by luck.)
- `game_memory.py` turns names into addresses: RAM symbols come from `pokefirered.elf`, and struct offsets and constants (`FLAG_*`, `MAP_*`, `VAR_*`) come from compiling a tiny C file against the game's headers, so they stay right when the code changes. Nothing to update by hand.

## Before/after pictures (`compare.py`)
After every smoke run, each screenshot is compared with the same screenshot from the last tagged build (`git describe`, e.g. v0.0.19): the newest clean run (no `-dirty`) of a commit at or before that tag that played the scenario. The runner and its clock are repeatable, so an unchanged scene gives the very same pictures; anything else is a real change. Tiny differences (under 0.2% of the screen) are ignored.
- `REPORT.md` gets a "Pictures compared" list at the end; `COMPARE.md` has a line per scenario (same / changed: how many pictures and the first one).
- `compare.html` (open it in the browser): before | after | what changed (changed pixels in pink), up to 6 per scene.
- A scene that now takes longer shifts all its later pictures, so "changed: 140 of 142" can mean one longer line of text. Look at the first changed picture.
```
python3 tools/research_red/test/compare.py RUN_FOLDER                  # again, e.g. after a new tag
python3 tools/research_red/test/compare.py RUN_FOLDER --against OLD_RUN # against one run
python3 tools/research_red/test/compare.py RUN_FOLDER --show lab        # before/after slider in the viewer
python3 tools/research_red/test/smoke.py --no-compare                   # skip it
```

## Random-button stress test (`monkey.py`)
From 6 starting points (bedroom, Pallet after the lab, Route 22, lunch, Pewter after Brock, a wild battle) it presses random buttons for a while: walking, A/B, the Start menu, the bag, the Pokédex, battles, saving, renaming. Same checks as the smoke tests (error screen, frozen screen, black end), same report. On demand, not on every build. Cheap: 10 game minutes from every start take about half a minute.
- **Repeatable**: the presses come from a seed (printed in the report and the folder name). The same seed on the same build presses the very same buttons, so a crash it finds can be replayed: `monkey.py --seed 1234 pewter`.
- **No debug menu**: if random presses open it, the runner backs out with B right away (`guard` in rr_run), so no debug warps or gifts.
```
python3 tools/research_red/test/monkey.py                          # every start, 10 game minutes, a new seed
python3 tools/research_red/test/monkey.py --seeds 5 --minutes 30   # a bigger hunt (about 6 minutes)
python3 tools/research_red/test/monkey.py --seed 1234 pewter       # replay one
```
Results: `E:\Workspace\romhacks\research-red\test-runs\monkey\<date>_<version>_<commit>_seed<N>\` (a screenshot every 10 seconds).
First hunt (2026-10-27, v0.0.19 build): 30 runs x 30 game minutes, nothing broke.

## Battle simulations (`balance.py`)
Plays key story fights 50 times each in the real game with the AI on both sides, a different random seed each time, and counts wins, turns and fainted Pokémon. Results: `data/research_red/battle_sims.json`, shown in docs/BALANCE.md ("Simulated fights", made by `export_player_docs.py`).
- Fights: the lab double, Route 22's 6v6 double, Elm and Jenny's Pewter rematch, Brock. Add one in `FIGHTS` (a setup up to just before the fight, what starts it).
- **Your team is a guess**: the team the debug skip hands out at that point (`data/scripts/debug.inc`). Change that to change the guess.
- **Your side's AI**: in an AI-vs-AI battle the game gives your side the AI settings of a battle partner, and with none it plays almost at random (test mode never noticed: its Lv 30 team wins anyway). The simulator sets your side to a basic trainer's settings right after each battle starts (no useless moves, goes for KOs), like Brock's. It still uses the odd Growl.
- About 8 minutes for all four at 50 runs (low priority). `balance.py brock --runs 20` for one.

## On GitHub (`.github/workflows/smoke-tests.yml`)
Every push to master runs a quick set of 10 scenes (breakfast, grass, lab, route1, route22, lunch, pewter, catch, pokedex, pc); a version tag (`v*`) runs all of them. A tick or a cross shows next to the commit on GitHub (and in the Actions tab). The report and screenshots are attached to the run ("Artifacts", kept 14 days). The built test runner is cached, so only the first run builds mGBA. Free for a public repository. To run everything by hand: Actions > Smoke tests > Run workflow > tick "Run every scene". No picture comparison there (the earlier runs live on this PC).

## What's covered
| Scenario | How it gets there |
|---|---|
| intro | New game, the whole call, ends in the bedroom (saved as the starting point for the rest) |
| title | The Mareep splash (walks in, credit fades in), the title screen, then New Game: Slowking's fan-project and demo notices, then the intro call (#12). New-game tests reach the title with `boot_to_title()` |
| intro_skip | Test builds: START during the new-game call skips it as Maple, straight to the bedroom |
| breakfast | Walks downstairs |
| grass | After breakfast, out of the door: the grass scene to the lab door; Jenny diagonally behind Elm, trailing him to the lab (#83) |
| portraits | Breakfast, slowly: each finished line still has its portrait (memory check, #76) |
| starter_menu | Lab: stops on the starter list (left, picture box, no portrait over it, #76) |
| lab | Debug 9 (test mode) + debug 1, then the lab scene and battles; Yes to watching Oak's Lv 50 ranch Pokémon and to battling Oak; the table is empty and Slowpoke naps on it (memory) (#86) |
| lab_loss | Lab scene, both your Pokémon set to 1 HP (memory poke): you lose, Elm and Jenny gloat, then the same Oak rounds; table and Slowpoke checked (#86) |
| lab_skip | Lab scene answering No to watching round two and to battling Oak (never forced); table and Slowpoke checked (#86) |
| daisy | Debug 2, steps into Daisy's house, the whole scene |
| route1 | Debug 8, the Route 1 scene and the Mewtwo fly-by |
| route22 | Debug 4 + test mode, the 6v6 double and its banter |
| lunch | Debug 6, talks to Elm by the Pokémon Center: the lunch and Officer Jenny |
| banter | Debug 2 + 7, the Pokédex banter preview |
| catch | Debug 11 (Poké Balls, wild Pidgey): throw until caught; the banter plays over the new entry, then the nickname; nothing replays on the map (#5) |
| pokedex | Debug 2, Start > Pokédex, every page of Bulbasaur both ways (the Evo page crashed before v0.0.15) |
| dex_replay | SELECT on Bulbasaur's entry replays the banter with both portraits; the entry comes back (#4) |
| banter_looks | The banter cutscene (#5/#4): slide in, the ink stamp, hops, B skips, slide out; portraits, then full-body (debug 13) |
| banter_female | The same with the female player, both looks |
| banter_family | Elm, then Jenny, call in for their family's lines (VAR_RR_STARTER poked) |
| banter_long | Rattata's long line fits the box; Krabby's question pops a ? bubble (memory) |
| banter_default | Catch a Treecko (debug 11 + VAR_TEMP_F poke): the default lines play after the catch |
| battle_lines, battle_lines_full | Route 22: when you or Ivy speak in battle, your picture stands over the message box; portraits, then full-body (#95) |
| dex_numbers | Marks Magikarp, Cyndaquil, Totodile caught (memory), scrolls the Kanto list to its end: 155 Cyndaquil, 158 Totodile (#77) |
| dex_area | Magikarp caught (memory): its Area page by day, evening, night (#20) |
| pewter | Test mode + debug 10, into the Pewter Pokémon Center: Elm and Jenny's rematch (story 8); then Yes to "Want to come watch?": the scripted Brock scene in the Gym, three rounds, ends with VAR_RR_BROCK_WATCH 2 (#69) |
| demo_end | Debug 10 (after Brock), walks to the east exit: the Gym guide turns you back (demo) |
| bike | Debug 12: on the Bicycle with your own sprite, Ivy out of view; off again, she is back behind you (#80) |
| gerald | Debug 2, surfs south from Pallet: Gerald blocks the sea route (demo) |
| pc | Debug 2, Start > PC, into Pokémon storage and back out |
| lab_supplies | The grant gives no balls and nothing goes to the PC (memory); Start > PC > Lab Supplies: take a free Poké Ball, it's in the Bag (memory), back out (#96, #98-#100) |
| ambient | Ambient Pokémon: talk to Pallet's Dodrio (marked seen in the Pokédex, memory check), then Pewter's Chansey across the counter (#64) |
| extras | Emerald extras: the first Potion picked up in Viridian shows its description (memory check: the Potion's seen flag), and walking from Pewter onto Route 2 shows the map name pop-up |
| guide | Route 1: Pokédex > START > FIELD GUIDE, then Start > GUIDE (#91), both over the map |
| rules | The lunch, then Options > Research Rules: change a rule, close, the call with Elm and Jenny plays (#45); reopen and close unchanged: no call |
| dexnav | Route 22: the 6v6, Jenny gives the DexNav, then Start > DexNav there |
| ivy | Talk to Ivy in Pallet and on Route 1 (area lines) |
| rename | Summary > A: rename to one letter, then to an empty name (the species name comes back) |
| wild | Pace in Route 1 grass until a wild battle, look at the opponent's type icon, run away |
| relearn | Move relearner: Learn Moves > TM Moves shows TM numbers; teach one, the list stays open on the same Pokémon without it; normal-ink sub-menu (#89) |
| evolve | Test mode (Lv 30): Squirtle shows EVOLVE in the party menu; evolves into Wartortle; LEARN MOVES > LEVEL MOVES lists Squirtle's moves at any level |
| pager_fly | Bag > Ranch Pager > Fly: the Fly map opens with no flyer in the team; Oak's Pidgeot flies to Pallet Town |
| pager_other | Ranch Pager > Flash outside a cave (not needed), then Teleport: Yes, Oak's Abra takes you back |
| pager_select | Register the Ranch Pager in the Bag; SELECT on the map opens its menu, B closes it |
| pager_dig | After Brock, inside Diglett's Cave: Ranch Pager > Dig > Yes, Oak's Diglett digs you out to Route 2 |
| lab_aide | After the lab: walk into the lab, the aide's topic menu, the Ranch Pager explanation, close; then Change a Ball: your first Pokémon into a Great Ball (#97) |
| forest | After Brock: walk (no wild encounters on the way) into Viridian Forest, wander until a battle: the forest background |
| oak_mons | Oak's Pager Pokémon: talk to Tauros in the lab yard, pass Lapras and Gyarados on the shore, Pikachu, Farfetch'd, Abra and Machop in the lab |
| rest | At home: Delia > Rest till night (clock 20:00, Start menu "Night 20:00"), then Rest till morning (7:00, "Day 7:00") |
| rest_center | Pewter Pokémon Center: Nurse Joy > Rest till night (clock 20:00) |
| brock_rematch | After Brock: into the Gym (the Camper first), Brock offers a rematch: tier menu (1 badge: Lv. 21), his team at Lv 20-21 |
| delia_battle | After Brock: Fly to Pallet, home, Delia asks for a battle (Yes): Mimey and Gloom lead (1 badge, Lv 21) |
| oak_rematch | After Brock: Ranch Pager > Fly to Pallet, into the lab, Oak + Daisy double (1 badge: middle-stage starters, Lv 21) |
| trainer_card | Start > your name: the Trainer Card shows your own picture (#24) |
| daynight | Outside at 10:00 (day), then Rest at home to night, morning, night, morning; same spot each time. Days must match days, nights nights (#108). **Fails until #108 is fixed** |
| oak_rematch_wild | Oak + Daisy's rematch, then a wild battle on Route 1: must be a Route 1 wild Pokémon (#106). **Fails until #106 is fixed** |
| delia_wild | Delia's battle, then a wild battle (#106). **Fails until #106 is fixed** |
| sprites_m / sprites_f | Sprite tour for each player: in and out of a door, surf (with reflection), walk four ways, run, bike (#125: the before/after check for new overworld sprites) |
| tms | After the lab: the TM Case holds all 108 TMs and HMs (memory check); one screenshot per TM51-TM100 (names fit their rows, descriptions their box) and the longest name selected |

Battles use **test mode** (debug script 9): the AI plays your side, so nobody has to choose moves.

## The engine's own tests (`make check`)
pokefirered-expansion ships ~5,200 small battle tests ("Burn deals 1/16 damage", "Mind Blown is blocked by Damp"...). `make check -j6` builds a separate test copy of the game (in `build/firered-test/`, the normal ROM isn't touched) and runs them all: about 30 minutes the first time, faster after. One test, or a group by name: `make check TESTS="Sunlight"`.

Run it after changing battle code, move/ability/item data or the save layout. Expected (2026-10-27): **everything passes** (see the end of the run for the count).

Research Red's adjustments to the engine tests (each marked `Research Red`):
- `test/save.c`: our SaveBlock1 size (15864). If it fails, the save layout moved: bump `RR_SAVE_VERSION` (docs/SAVES.md), then the number.
- `test/battle/exp.c`: two experience tests switch our level caps off (`FLAG_RR_LEVEL_CAPS_OFF`), otherwise the cap gives 0 exp.
- `test/battle/ai/ai.c`, *AI will consider using Explosion...*: caps off too. Our catch-up EXP (`B_LEVEL_CAP_EXP_UP`) levelled the Lv 5 Zigzagoon up after the KO, which the test doesn't expect, so it timed out (found 2026-10-27, #18). Other upstream tests with a Lv 5 winner may need the same line.

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
- `check(label, G.flag_set("FLAG_X"))`: any other fact. `G` (`game_memory.py`) has `var_is`, `flag_set`, `flag_clear`, `on_map`, `party_count`, `field_free`, `textbox_closed`, `in_battle`; join conditions with `" && "`, and groups of those with `" || "` (any group)
- `debug_script(n)`: Start > DEBUG > Scripts > n
- `step("UP", 3)`: walk 3 tiles
- `not_black("p001")`: FAIL if that screenshot came out all black (a menu that should show over the map)
- `ready("lab")`: after a door or warp, wait (pressing nothing) until the player can walk again; FAILs with `timeout_lab.png` if that never happens. Use it instead of a fixed `wait` after map changes
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
| `tools/research_red/test/balance.py` | Battle simulations for balance (results in `data/research_red/battle_sims.json` -> docs/BALANCE.md) |
| `tools/research_red/test/monkey.py` | The random-button stress test (uses smoke.py's intro and debug skips) |
| `tools/research_red/test/compare.py` | Before/after pictures against the last tagged build's runs (needs Pillow) |
| `tools/research_red/test/rr_checks.py` | The checks on every scene (dialogue, leftovers, battles, brightness, wild, coverage) |
| `tools/research_red/test/dialogue.py` | The dialogue check (portraits, boxes closing early) |
| `tools/research_red/test/nav.py` | Walk anywhere by map data; the wild probe |
| `tools/research_red/test/playthrough.py` | The full playthrough, power-on to Pewter |
| `tools/research_red/test/matrix.py` | Curated fights x Research Rules settings |
| `tools/research_red/test/coverage.py` | Script coverage report from runs |
| `test/battle/rr_type_chart.c` | Engine test: every type matchup against the official chart (#112) |
