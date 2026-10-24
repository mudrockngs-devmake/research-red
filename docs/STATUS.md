# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).

_Last updated: 2026-10-21 (v0.0.19 released for the playtest)_

## Current build
- **v0.0.19** (2026-10-21): parts 1 and 2 (see CHANGELOG.md). Installed as `ResearchRed_local.gba`, tagged `v0.0.19`, pre-release on GitHub. Smoke 61/61 (`test-runs/2026-10-21_0234_v0.0.19_c883da619`). What's-new report: `E:\Workspace\romhacks\research-red\whats-new\v0.0.19\report.html`. **Save: start a new game.**
- **v0.0.18** (2026-10-20): v0.0.17 + battle backgrounds per place in cozier colours (#74, `tools/research_red/battle_bg_theme.py`), the test-mode wild battle freeze fix. Installed as `ResearchRed_local.gba`, tagged, pre-release on GitHub. Smoke tests 35/35 (`test-runs/2026-10-20_0952_v0.0.18_*`). **Save: old saves work.**
- **v0.0.17** (2026-10-20, pre-release): the Field notebook look (#73) + item descriptions on pickup, snappier battle text, B to RUN.
- **v0.0.16** (2026-10-20, pre-release on GitHub): the big systems build, see [CHANGELOG.md](CHANGELOG.md).
- **Reports for the testers** (each folder also has `report.pdf` for phones): `E:\Workspace\romhacks\research-red\whats-new\v0.0.16\report.html` (33 items, everything new since v0.0.15) and `...\whats-new\v0.0.17\report.html` (the new look). Rebuild with `python3 tools/research_red/rr_whats_new.py <version>`.
- Interface colours: `tools/research_red/ui_theme.py notebook` is applied in the source (`--restore` returns to FireRed's). Mock-ups: `E:\Workspace\romhacks\research-red\mockups\`.
- Last playtest archived: `test-archive/2026-10-20_v0.0.18_playtest/` (owner, 9 bug cards -> issues #76-#95 + comments; NOTES.md has the session costs).

## Waiting on the owner
- [ ] v0.0.19 playtest with the what's-new report (pick portraits or full-body there; Debug > Scripts > 13 switches).
- [ ] Rework the draft lines from part 2: Oak's grant / Lab Supplies lines, the lab's Oak rounds (#86), the aide's PC topic, Slowking's pages.
- [ ] Rework the placeholder lines added in v0.0.16 (Ranch Pager tutorial, Oak's Pokémon, Rest, the Rules call, the rematches, the lab aide).
- [ ] Optional: phone emulator / real hardware (#16) (v0.0.18 was played on Android RetroArch).
- [ ] Workshop the report's playtest form when wanted.

## Next up: v0.0.19
- Owner wants all remaining demo items in v0.0.19, portraits (#76) first, using the upgraded tester. New issues from the playtest: #76-#95.
- Plan: [ROADMAP.md](ROADMAP.md) "Next build: v0.0.19" (optional new background art #74 round 2; title screen mock-ups #12, rematch Megas and other Gym leaders #52, Brock watch scene #69, banter cutscene #5/#4, ambient Pokémon #64, Forest exchanges #9, Pokédex Area tab #20, proper case #11, Cruller, art items).
- Test tools (#18): done: route planner, memory pokes, `T.ready()` (wait-until-ready), a fake clock in rr_run (runs repeatable at any hour), the engine's own tests (`make check`: 4584 pass, 1 known upstream failure, docs/TESTING.md). Left: the AI Explosion test timeout, the issue's "later" ideas. Then a smaller patch (#17).

## NEXT SESSION START HERE: v0.0.19 is out for the owner's playtest
Parts 1 and 2 are done, committed and tested (smoke 61/61, `test-runs/2026-10-21_0234_v0.0.19_c883da619`; `make check`: only the known Explosion timeout;
rr_check clean). Installed and tagged 2026-10-21 with the owner's OK. After the playtest:
`tools/research_red/rr_archive_test.sh <test-name>`, then turn the notes into issues.

Part 2, all done (2026-10-21):
1. Title: Slowpoke's coat lower and further back, a sleeve around the front (mock-up `art/generated/title/2b_title_screen_coat_v2.png`).
2. Slowking's welcome page between the two notices (the owner's wording).
3. Nickname smoke test with memory checks (`rename`).
4. Grant rework (#96, #98-#100): the grant is TMs/HMs and key items only; **Lab Supplies** is a free shop on every PC (no Master Ball); pockets back to FireRed's; RR_SAVE_VERSION 4. Oak's/Ivy's/Jenny's/aide lines are drafts for the owner.
5. Oak lab rework (#86): Elm and Jenny beat Oak's leftover starters (watched), Oak's Lv 50 Pidgeot and Tauros flatten them (optional), optional player battle, Slowpoke naps on the empty table; Muffin added by Daisy. Tests lab, lab_loss, lab_skip.
6. Banter cutscene (#5 #4 #95): slide in/out, hops, cry, ink stamp, pencil sound, ! ? ♪ bubbles, Elm/Jenny call-ins, B skips; portraits by default (they stand on the text box, owner's request), full-body behind Debug > Scripts > 13. Tests banter_*, battle_lines(_full).
7. README: the GitHub front page, reworked with the owner and published (2026-10-21), with screenshots in `docs/images/readme/`, a first-project note, thanks, and an AI-assistance line. Repo description updated.
8. Save room: unused link / e-Reader / Mystery Gift data freed (see Capacity); relearner TM names no longer permanent RAM.
9. Final: CHANGELOG, what's-new report `E:\Workspace\romhacks\research-red\whats-new\v0.0.19\report.html` (with the portrait-vs-full-body comparison for the owner to pick), CREDITS, issues closed.

Watch closely in the playtest (new or reworked code, most likely to break):
- Lab Supplies: buying, a full Items pocket ("no more room"), leaving back to the PC menu, from Start > PC and from a real PC.
- The lab scene after the Elm and Jenny battle, both win and loss, Yes and No to both questions; Slowpoke on the table on later visits; Muffin when Daisy visits.
- The dex banter after a catch and with SELECT: B to skip, quickly mashing A or B, Elm/Jenny family species, switching the look (debug 13); the type icons coming back after.
- Battles where you or Ivy speak (Route 22): pictures appearing and leaving.
- The move relearner (its memory moved), the PC item box and the Berry Pouch (pocket sizes changed), saving and loading on a new game.

Session costs (`session-cost`, readings may lag): v0.0.19 part 1 $110.87; part 2 (2026-10-21 from 01:41) $28.04; total $138.91. Log them in the round's NOTES.md when archiving the playtest.

Test tooling added: `runner.log` per scene, `||` in conditions, `GameMemory.party_nickname_is` / `all_symbols`, debug 11 meets the species poked into VAR_TEMP_F.

### Capacity (measured 2026-10-21, after part 2)
| Space | Used | Notes |
| --- | --- | --- |
| Cartridge (ROM) | ~24.6 of 32 MB | ~7 MB free |
| Save: SaveBlock1 | 13732 of 15872 bytes | 2140 free (link / Mystery Gift data freed) |
| Save: SaveBlock2 | 2732 of 3968 | 1236 free |
| Save: SaveBlock3 | 1536 of 1624 | Research Red's own room: 674 bytes of padding + 88 |
| Save: PC boxes | 34144 of 35712 | 1.5 KB spare |
| Flags / vars | 2304 / 256 | ~570 flags and ~90 vars still unused; keep FLAGS_VARS.md current |
| RAM (EWRAM, permanent) | 250156 of 262144 bytes | ~11.7 KB free; new screens should allocate while open |

## Art Kit run (started 2026-10-24; owner answers in ROADMAP.md "Art Kit run")
Work through this list, commit after each, keep this updated. Nothing gets installed or tagged.
- [x] Windows tools: Porymap + LibreSprite in `E:\Workspace\_tools\`; survey of other romhack projects' art tools
- [x] Sprite library: catalogue + contact sheets of every overworld / trainer / Pokemon picture; walk-motion study
- [x] Overworld maker: recipes (base sheet + recolour + pixel patches), walk/run frame generator, GIF previews, add-to-game command
- [x] Overworld sprites: Elm, Jenny, Officer Jenny, Ivy, players M/F, Delia; gym leaders + Elite Four
- [x] Battle pictures: gym leaders + Elite Four from the ref art; battle sprite checker/fixer, shiny palettes, back-pic throw frames, GIFs
- [x] Map art: tileset inspector, furniture/building importer (Porytiles or our own), building + door wizard (tested in a scratch copy)
- [ ] Cloud: GitHub Action for art checks + preview gallery
- [ ] Guide: docs/ART_TOOLS.md, ARTIST_GUIDE/ASSETS/DEV_GUIDE, rr_check art rule, CREDITS; private sprite-editing notes on E:
- [ ] Final: build, rr_check, smoke tests; gallery for the owner

## v0.0.19 run (done, started 2026-10-20 evening)
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
- [x] #83 Jenny follows Elm
- [x] #84 Daisy tea scene seated; breakfast seated; other positioning passes
- [x] #85 Route 1 rework; #88 Elm's dialogue
- [x] #86 Oak steps in after a lab loss
- [x] Cruller (Jenny's Growlithe)
- [x] #92 battle boxes in notebook colours; #11 proper case
- [x] #93 female overworld; #80 bike; #28 surf/Pager sprites
- [x] #7 battle back sprites + Elm in watched battles; #95 portraits in battle dialogue
- [x] #24 Trainer Card
- [x] #32 Pokémon portraits from game sprites
- [x] #40 Jenny sprites (Hex Maniac stand-in), Officer Jenny in uniform; #8 picnic blanket
- [~] #74 landscape battle backgrounds: moved past v0.0.19. No GBA game has scenery backgrounds; the DS rips are blocked for downloads, so the owner drops the HGSS sheet in `art/incoming/` (see the issue)
- [x] #9 Forest exchanges + Samurai
- [x] #69 watch Brock (scripted scene, Yes now or later)
- [x] #64 ambient Pokémon: Pallet Dodrio and Oak's Diglett, Chansey in the Viridian and Pewter Centers, Forest Pidgeot vs Fearow
- [x] #89 move relearner: normal ink, stays in the list after teaching, TM numbers
- [x] #90 TM swaps: 19 TMs that repeated a free tutor move replaced. Out: Mega Punch, Mega Kick, Swords Dance, Body Slam, Double-Edge, Counter, Seismic Toss, Mimic, Metronome, Soft-Boiled, Dream Eater, Thunder Wave, Explosion, Rock Slide, Substitute, Fire/Thunder/Ice Punch, Icy Wind (all still in the relearner's Tutor Moves). In: TM51-65 Low Kick, Knock Off, Foul Play, Nasty Plot, Agility, Roost, Heat Wave, Mud Shot, Drill Run, Power Gem, Smart Strike, Iron Defense, Body Press, Charge Beam, Grass Knot; TM68 Fire Fang, TM75 Thunder Fang, TM76 Ice Fang, TM77 Pounce (for the what's-new report)
- [x] v0.0.16 placeholder lines polished: Oak's Pokémon, Oak's Pager tutorial (with a proverb), the Rules call (Jenny's professional opinion), Delia's Rest offer, Oak's rematch haiku; the aide's Field Guide and TM facts corrected
- [x] #12 title screen (notebook paper, logo, Slowpoke in a lab coat), Mareep splash, Slowking notices; mock-ups in `E:\Workspace\romhacks\research-red\art\generated\title\` for the owner to review
- [x] Emerald map pop-ups, first-time item descriptions (RR_SAVE_VERSION 3)
- [x] Owner's 2026-10-21 ideas: grant to PC, 150 balls, more consumables, 6 battle items (#96, #98-100), Change a Ball (#97), START skips the intro call (test builds), banter forms fallback
- [x] Final: full smoke run, rr_check, CHANGELOG, what's-new report, CREDITS list of art used; NO install/tag

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
