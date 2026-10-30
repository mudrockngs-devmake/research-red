# Playtesting

How a test round works, from a new build to an archived round.

## Two kinds of round (owner 2026-10-29)
- **Spot check** (most builds, 10-20 minutes): each change has a debug jump (Start > Debug, right before that scene, with a fitting team and story state) and the what's-new checklist lists those jumps. Things to only look at (sizes, positions) come as pictures in the viewer. No replay from the start.
- **Full playthrough** (about every three builds, or after a big story-flow change): a new game to the end of the demo, covering everything since the last full run. Next: after v0.0.23.
- Every build, either way: the bot's full playthrough (`playthrough.py`), the settings matrix and all smoke scenes pass before the owner gets it. The hand-off says which kind of round it is.

## The round, step by step
1. **Build and check** (Claude): `make firered -j6`, `python3 tools/research_red/rr_check.py`, then the smoke tests (`python3 tools/research_red/test/smoke.py`, see [TESTING.md](TESTING.md)). Only a build that passes goes to the owner.
2. **Install** (Claude): `tools/research_red/rr_install_rom.sh` puts it at `E:\Workspace\romhacks\research-red\ResearchRed_local.gba` (a numbered copy stays in `builds/`).
3. **Hand-off** (Claude): what changed, what to test, and the save note ("new game needed" or "old saves fine"), from [CHANGELOG.md](CHANGELOG.md).
4. **Play** (owner): open `ResearchRed_local.gba` in mGBA. Press F12 for screenshots; they land in `_emu\screenshots`. Save states are fine. Notes go in the what's-new report (checklist + "Your playtest" at the end); "Save my notes" downloads a text file: put it in `_emu\notes`.
5. **Report** (owner): tell Claude in chat, or use the issue form (below). Attach screenshots or a save state for anything weird.
6. **File and archive** (Claude):
   - each problem becomes a GitHub issue labeled `playtest` (plus `bug`, `story`, `art`...) on a milestone
   - `tools/research_red/rr_archive_test.sh <short-name>` moves everything out of the six `_emu` folders (incl. `notes`) into `test-archive/<date>_<version>_<name>/`
   - fill in that round's `NOTES.md`: what was tested, what was found (with issue numbers), follow-ups, and the session cost (`session-cost --all-days` lists every session since the last round)
7. **Update [STATUS.md](STATUS.md)** (Claude).

## Full-run checklist (5 minutes)
Things that have broken before or would be embarrassing. Do these on every full playthrough, on top of the new stuff:
- [ ] New game: the intro call plays to the end (Slowpoke stays), you wake up in the bedroom
- [ ] Your player sprite walks, runs, bikes and surfs correctly (male and, now and then, female)
- [ ] Breakfast: portraits and name plates switch with each speaker; Ivy follows you after
- [ ] A trainer battle is a double; the battle ends and the scene continues
- [ ] Start menu: the clock box reads "Day 14:05" / "Night 20:00" cleanly; Save works and Continue loads it
- [ ] Pokédex > START > FIELD GUIDE opens and closes
- [ ] A wild catch of something new: the Pokédex banter plays back on the map
- [ ] Nothing flashes the wrong colors when the speaker changes

## Reporting a problem
The most useful report has: **build** (the version in the intro call, e.g. v0.0.14), **where**, **what happened vs what you expected**, and **a save state from just before it**. A screenshot of an error screen (blue screen with a file name and line) tells us exactly where the code broke.

You can report in chat, or file it directly: GitHub, Issues tab, New issue, "Bug report". The form asks for exactly those things.

## Where things are
| What | Where |
|---|---|
| ROM to play | `E:\Workspace\romhacks\research-red\ResearchRed_local.gba` |
| Every build | `E:\Workspace\romhacks\research-red\builds\` |
| Screenshots, saves, states | `E:\Workspace\romhacks\_emu\` (emptied into the archive after each round) |
| Past rounds | `E:\Workspace\romhacks\research-red\test-archive\` (`INDEX.md` lists them) |
| Automated test runs | `E:\Workspace\romhacks\research-red\test-runs\` (contact sheets per scene) |
| Player guides | `E:\Workspace\romhacks\research-red\player-docs\` |
| Story for reading/editing | `E:\Workspace\romhacks\research-red\story\` |

## Debug skips (test builds only)
Start > DEBUG > Scripts:

| # | Name | What it does |
|---|---|---|
| 1 | Lab scene | Grass scene done, standing at the lab's trigger |
| 2 | After lab | Lab done, outside in Pallet, Ivy following |
| 3 | Route 1 | Daisy done, at Pallet's north exit |
| 4 | Route 22 | Route 1 done, just east of Elm and Jenny's trigger |
| 5 | Test team | Adds Spearow, Rattata, Mankey, Nidoran♂ |
| 6 | Lunch | Route 22 done, in Viridian below Elm and Jenny (right of the Pokémon Center): the lunch starts on arrival |
| 7 | Dex banter | Queues the banter for Tentacool, Slowpoke, Quilava |
| 8 | Route 1 Elm | Daisy done, one step from Elm and Jenny's Route 1 scene |
| 9 | Test mode | On/off: the AI fights your battles; team to Lv 30 |
| 10 | After Brock | Badge 1, story past the lunch, in Pewter below the Pokémon Center door (Elm and Jenny's rematch just outside; the demo blocker at the east exit). Pallet, Viridian and Pewter are Fly targets |
| 11 | Catch test | 10 Poké Balls and a wild Pidgey |
| 12 | Bike test | A Bicycle, and straight onto it |
| 13 | Banter look | Switches the banter between portraits and full-body pictures |
| 14 | Your battle pic | Switches your battle picture between side by side and turned to each other |
| 15 | Pewter rebattle | Like 10, with Elm and Jenny's Pewter rematch done: leave Pewter and come back, they wait by the Center (#143) |
| 16 | Watch Brock | Elm and Jenny fight Brock on the battle screen, from the Gym door (#142) |
| 17 | Gym first visit | At the Gym door, Brock not beaten yet, Lv 14 team: the crowd, his fight, the Alolan Geodude (#142) |

Skips 4 and 6 give a Lv 12 team.
