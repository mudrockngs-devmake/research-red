# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).

_Last updated: 2026-10-20 evening (test tools #18, playtest form in the report)_

## Current build
- **v0.0.18** (2026-10-20): v0.0.17 + battle backgrounds per place in cozier colours (#74, `tools/research_red/battle_bg_theme.py`), the test-mode wild battle freeze fix. Installed as `ResearchRed_local.gba`, tagged, pre-release on GitHub. Smoke tests 35/35 (`test-runs/2026-10-20_0952_v0.0.18_*`). **Save: old saves work.**
- **v0.0.17** (2026-10-20, pre-release): the Field notebook look (#73) + item descriptions on pickup, snappier battle text, B to RUN.
- **v0.0.16** (2026-10-20, pre-release on GitHub): the big systems build, see [CHANGELOG.md](CHANGELOG.md).
- **Reports for the testers:** `E:\Workspace\romhacks\research-red\whats-new\v0.0.16\report.html` (33 items, everything new since v0.0.15) and `...\whats-new\v0.0.17\report.html` (the new look). Rebuild with `python3 tools/research_red/rr_whats_new.py <version>`.
- Interface colours: `tools/research_red/ui_theme.py notebook` is applied in the source (`--restore` returns to FireRed's). Mock-ups: `E:\Workspace\romhacks\research-red\mockups\`.
- Last playtest archived: `test-archive/2026-10-20_v0.0.18_playtest/` (owner, 9 bug cards -> issues #76-#95 + comments; NOTES.md has the session costs).

## Waiting on the owner
- [ ] Rework the placeholder lines added in v0.0.16 (Ranch Pager tutorial, Oak's Pokémon, Rest, the Rules call, the rematches, the lab aide).
- [ ] Optional: phone emulator / real hardware (#16) (v0.0.18 was played on Android RetroArch).
- [ ] Workshop the report's playtest form when wanted.

## Next up: v0.0.19
- Owner wants all remaining demo items in v0.0.19, portraits (#76) first, using the upgraded tester. New issues from the playtest: #76-#95.
- Plan: [ROADMAP.md](ROADMAP.md) "Next build: v0.0.19" (optional new background art #74 round 2; title screen mock-ups #12, rematch Megas and other Gym leaders #52, Brock watch scene #69, banter cutscene #5/#4, ambient Pokémon #64, Forest exchanges #9, Pokédex Area tab #20, proper case #11, Cruller, art items).
- Test tools (#18): done: route planner, memory pokes, `T.ready()` (wait-until-ready), a fake clock in rr_run (runs repeatable at any hour), the engine's own tests (`make check`: 4584 pass, 1 known upstream failure, docs/TESTING.md). Left: the AI Explosion test timeout, the issue's "later" ideas. Then a smaller patch (#17).

## v0.0.19 run (in progress, started 2026-10-20 evening)
Owner approved everything (ROADMAP.md "Owner answers"). Work through this list, commit after each, keep this updated. Stop before install/tag/release.
- [x] #76 portraits stay on the last page; starter list on the left
- [x] #78 type display back to the engine's
- [x] #79 Start menu PC
- [x] #81 intro call name
- [x] #82 Move Info black
- [x] #94 Sumac -> Sequoia
- [x] #87 starter moves
- [x] #77 dex numbering
- [x] #20 Area tab (demo areas)
- [x] #5 / #4 banter over the dex page, SELECT replay
- [x] #91 Field Guide in the Start menu
- [ ] #83 Jenny follows Elm
- [ ] #84 Daisy tea scene seated; breakfast seated; other positioning passes
- [ ] #85 Route 1 rework; #88 Elm's dialogue
- [ ] #86 Oak steps in after a lab loss
- [ ] Cruller (Jenny's Growlithe)
- [ ] #92 battle boxes in notebook colours; #11 proper case
- [ ] #93 female overworld; #80 bike; #28 surf/Pager sprites
- [ ] #7 battle back sprites + Elm/Jenny pictures; #95 portraits in battle dialogue; #24 Trainer Card
- [ ] #40 Jenny sprites; #32 portraits; #8 picnic blanket
- [ ] #74 landscape battle backgrounds
- [ ] #9 Forest exchanges + Samurai; #69 watch Brock
- [ ] #64 ambient Pokémon (demo areas)
- [ ] #89 move relearner; #90 TM swaps
- [ ] v0.0.16 placeholder lines polished
- [ ] #12 title screen, Mareep splash, Slowking disclaimer
- [ ] Emerald map pop-ups, first-time item descriptions (RR_SAVE_VERSION bump)
- [ ] Final: full smoke run, rr_check, CHANGELOG, what's-new report, CREDITS list of art used; NO install/tag

## Half-finished / open threads
- Nothing half-done: everything is committed and pushed.
- Rematch rules (owner, 2026-10-20) are on #52: build on each character's existing team, separate tiers for 7 and 8 badges, a post-game tier with regional forms and exchange-program visitors (Mimey, Muffin stay home), Megas in the post-game tier (to add).
- Still needs the owner's eyes: #20 (Pokédex tab glitches), #14 (Route 22 tiles), #37 (Ivy and ledges), #41 (Totodile evolution under the cap).
- The League gate block (Route 22) has no automatic test; check by hand (#2).
- 13 rows of `docs/reference/kanto_anime.csv` are marked "memory" (to verify).

## How to resume in a new session
1. Open Ubuntu (or double-click `E:\Workspace\Claude Hub.bat`): the Claude Hub menu appears. Work on a project > research-red > Start a fresh conversation (normal; Continue only to resume the same task). (By hand: `cd ~/romhacks/research-red && claude --continue`.)
2. Claude reads `CLAUDE.md` (rules) and this file (state), then checks `git log -5` and open issues (`gh issue list`).
3. If the test runner is missing (`~/romhacks/_tools/rr_run`), run `tools/research_red/test/setup_mgba.sh`.
