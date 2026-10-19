# Automated smoke tests

A headless copy of mGBA plays the game from scripted button presses, takes screenshots, and checks that nothing crashed. It runs on every build before the owner gets it.

## Run them
```
python3 tools/research_red/test/smoke.py            # all scenarios (about 3 minutes)
python3 tools/research_red/test/smoke.py lunch guide # just these
python3 tools/research_red/test/smoke.py --list      # what's covered
```
Results go to `E:\Workspace\romhacks\research-red\test-runs\<date>_<version>_<commit>\`:
- `REPORT.md`: PASS / WARN / FAIL per scenario
- `<scenario>.png`: a contact sheet (a grid of up to 30 screenshots) to skim the scene in seconds
- `<scenario>\`: every screenshot

**PASS means it didn't crash or end on a black screen.** It doesn't mean the scene is right: open the contact sheets. The first run of these tests found a real bug (a portrait flashing in the previous speaker's colors) that way.

## What's covered
| Scenario | How it gets there |
|---|---|
| intro | New game, the whole call, ends in the bedroom (saved as the starting point for the rest) |
| breakfast | Walks downstairs |
| lab | Debug 9 (test mode) + debug 1, then the lab scene and battles |
| route1 | Debug 8, the Route 1 scene and the Mewtwo fly-by |
| route22 | Debug 4 + test mode, the 6v6 double and its banter |
| lunch | Debug 6, the lunch, Officer Jenny, Research Rules |
| banter | Debug 2 + 7, the Pokédex banter preview |
| guide | Debug 3, walks onto Route 1, Start > Guide |
| rules | The lunch, then Start > Rules |

Battles use **test mode** (debug script 9): the AI plays your side, so nobody has to choose moves.

## First-time setup (once per machine)
```
tools/research_red/test/setup_mgba.sh
```
Downloads CMake and mGBA 0.10.5 into `~/romhacks/_tools/` (outside the repo) and builds `rr_run`. Takes a few minutes. Run it again if `~/romhacks/_tools` is ever deleted.

## Adding a scenario
Scenarios are short functions in `smoke.py` that return script lines. Building blocks:
- `debug_script(n)`: Start > DEBUG > Scripts > n
- `step("UP", 3)`: walk 3 tiles
- `advance(40, key="B")`: press B through text 40 times, screenshot each time
- raw commands for `rr_run`: `wait N`, `press KEYS`, `hold KEYS N`, `mash KEYS N`, `shot NAME`, `save PATH` / `load PATH` (see the top of `rr_run.c`)

Use B to advance text when the player might be facing something (A talks to it). Name screenshots so they sort in order (`s001`, `s002`...).

## Files
| File | What |
|---|---|
| `tools/research_red/test/rr_run.c` | The runner (C, links libmgba) |
| `tools/research_red/test/setup_mgba.sh` | Builds the runner |
| `tools/research_red/test/smoke.py` | Scenarios, checks, report |
| `tools/research_red/test/contact_sheet.py` | Reads screenshots, makes the grids |
