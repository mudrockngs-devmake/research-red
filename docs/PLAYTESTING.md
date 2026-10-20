# Playtesting

How a test round works, from a new build to an archived round.

## The round, step by step
1. **Build and check** (Claude): `make firered -j6`, `python3 tools/research_red/rr_check.py`, then the smoke tests (`python3 tools/research_red/test/smoke.py`, see [TESTING.md](TESTING.md)). Only a build that passes goes to the owner.
2. **Install** (Claude): `tools/research_red/rr_install_rom.sh` puts it at `E:\Workspace\romhacks\research-red\ResearchRed_local.gba` (a numbered copy stays in `builds/`).
3. **Hand-off** (Claude): what changed, what to test, and the save note ("new game needed" or "old saves fine"), from [CHANGELOG.md](CHANGELOG.md).
4. **Play** (owner): open `ResearchRed_local.gba` in mGBA. Press F12 for screenshots; they land in `_emu\screenshots`. Save states are fine.
5. **Report** (owner): tell Claude in chat, or use the issue form (below). Attach screenshots or a save state for anything weird.
6. **File and archive** (Claude):
   - each problem becomes a GitHub issue labeled `playtest` (plus `bug`, `story`, `art`...) on a milestone
   - `tools/research_red/rr_archive_test.sh <short-name>` moves everything out of the five `_emu` folders into `test-archive/<date>_<version>_<name>/`
   - fill in that round's `NOTES.md`: what was tested, what was found (with issue numbers), follow-ups, and the session cost (`session-cost --all-days` lists every session since the last round)
7. **Update [STATUS.md](STATUS.md)** (Claude).

## Every-round checklist (5 minutes)
Things that have broken before or would be embarrassing. Do these every round, on top of the new stuff:
- [ ] New game: the intro call plays to the end (Slowpoke stays), you wake up in the bedroom
- [ ] Your player sprite walks, runs, bikes and surfs correctly (male and, now and then, female)
- [ ] Breakfast: portraits and name plates switch with each speaker; Ivy follows you after
- [ ] A trainer battle is a double; the battle ends and the scene continues
- [ ] Start menu: Guide opens and closes; Save works and Continue loads it
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
| 6 | Lunch | Route 22 done, in Viridian two steps below Elm and Jenny (right of the Pokémon Center): talk to start |
| 7 | Dex banter | Queues the banter for Tentacool, Slowpoke, Quilava |
| 8 | Route 1 Elm | Daisy done, one step from Elm and Jenny's Route 1 scene |
| 9 | Test mode | On/off: the AI fights your battles; team to Lv 30 |
| 10 | After Brock | Badge 1, story past the lunch, in Pewter below the Pokémon Center door (Elm and Jenny's rematch inside; the demo blocker at the east exit) |

Skips 4 and 6 give a Lv 12 team.
