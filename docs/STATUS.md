# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).

_Last updated: 2026-10-21 (v0.0.19 part 1 done and tested; part 2 planned below, start a new chat for it)_

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

## NEXT SESSION START HERE: v0.0.19 part 2 (owner-approved plan, 2026-10-21)
Part 1 (everything ticked in the list below) is done, committed and tested: smoke 53/53 after the ambient
fix, `make check` only the known Explosion timeout (the 13 AI "failures" were test wording after #11; fixed).
Stamp is already v0.0.19 and the CHANGELOG v0.0.19 section exists (extend it). The owner will start a fresh
save. Same rules as part 1: commit after each item, one wait, **stop before install/tag/release**.

1. **Slowpoke in the lab coat** (title, `tools/research_red/build_title.py`): wrap it better, show more of the face
   and ear (coat lower and further back, a sleeve tucked around the front). New mock-ups to `art/generated/title/`.
2. **Slowking's welcome page** (`src/rr_title.c`), a middle page between the two notices, cozy and spoiler-free:
   "You and Ivy, two Pokémon professors, set out across Kanto with Elm and Jenny. There's one big research paper
   to write, and a friendly race to eight badges. Take your time: catch what you love, take notes, and enjoy the
   road." (wrap to the window; owner liked the wording).
3. **Nickname smoke test** (#71 feature exists: Summary > info page > A): rename, check the name in memory,
   erase it all + OK, check it's the species name again.
4. **Grant rework** (#96 #98 #99 #100; keep #97 Change a Ball as is):
   - Grant gives only reusable things: all TMs/HMs and the key items (Bicycle, Ranch Pager, Exp. Share, rods...).
   - **Lab Supplies**: a free shop in the **PC menu** (FireRed's Poké Mart screen, every price 0, no money taken):
     every Poké Ball **except the Master Ball**, all medicine and status heals, Repels, X items, held battle items,
     Rare Candy, PP Up/Max and vitamins (Rare Candy stops at the cap; Master Ball held back for a later reward).
   - Undo part 1's version cleanly: no grant items in the PC, no 150 balls, pocket sizes back to FireRed's,
     RR_SAVE_VERSION 4, SAVES.md, tests (pc_items etc.) and docs.
   - Rewrite Oak's grant lines and the lab aide's explanations in their voices (drafts; owner reviews).
5. **Oak lab rework** (replaces #86's "Oak steps in with Pidgeot/Tauros Lv18 and always wins"). On **both** paths:
   - Win or lose against Elm & Jenny, they ask Oak for a match with the **two leftover starters**. Oak **heals
     them first**. Watched (AI vs AI); **Elm & Jenny win**.
   - Elm's over-the-top victory (style guide voice) invigorates Oak; Elm foolishly asks for another round.
   - **Optional** "watch?" (Yes/No): Oak **heals them again**, then his own Tauros and Pidgeot at **Lv 50** (his
     ranch/ride Pokémon, never fought again) flatten them, slapstick. Funny banter, Oak calms down, a funny moment.
   - **Optional for the player**: you and Ivy may battle Oak's two leftover starters too; Oak **heals your team
     first**. Never forced (no back-to-back battle without healing this early).
   - The starter table is then **empty**; Oak keeps the two starters (they're already in his rematch team).
     From then on **Slowpoke naps on the table** like a cozy bed; Oak doesn't mind; the Pager Pokémon (and Muffin
     when Daisy is there) react when talked to. Leads into the later Oak + Daisy rematches.
   - Update smoke tests (lab, lab_loss) and add one for the optional watch.
6. **Banter cutscenes + full-body art** (#5 #4 #95):
   - Build **both** looks behind a switch: portraits (current) and the full-body `cast/*/battle.png` (64x64) standing
     behind the dialogue, in the dex banter (after a catch, SELECT replay) and when you/Ivy speak in battle; both
     genders. Ship with Claude's recommendation; put portrait-vs-full-body screenshots of each place in the
     what's-new report so the owner picks during the playtest.
   - Cutscene feel: slide in at the start/out at the end; the speaker hops when their line starts (other dimmed);
     the Pokémon cries and hops at the start; a soft pencil-scribble sound per line; emote bubbles (! ? ♪) on some
     lines; Elm or Jenny appear for their family lines; the dex number stamped on like an ink stamp.
   - Safety: no free colour/sprite slot (especially in battle) -> skip that picture; clean up on end and on B.
   - Smoke tests with screenshots: after a catch, SELECT replay, battle dialogue, both genders, a long line, a
     default-lines species, an Elm/Jenny family species.
7. **GitHub front page** (README): what the game is, the spoiler-free story setup, what makes it different
   (Ranch Pager, banter, Research Rules, evolve when you want, Lab Supplies, rematches...), screenshots. Public:
   game only, nothing personal (see the private memories). **Show the owner the draft before pushing it.**
8. **Save-space groundwork** (see "Capacity" below): set the unused `FREE_*` options in `include/config/save.h`
   (Mystery Event, Mystery Gift, record-mixing hall records, union room chat, link battle records, Enigma Berry,
   Battle Tower e-Reader, Pokémon Jump; keep Trainer Tower and Match Call for now), measure, record in SAVES.md;
   move `sRRTMNames` (1.4 KB of permanent RAM) to the relearner's own allocation.
9. **Final**: full smoke run, rr_check, `make check`, extend CHANGELOG (Save: new game), what's-new report v0.0.19
   (json in `tools/research_red/whats_new/`, include the #90 TM list and the portrait/full-body comparison),
   CREDITS, close finished issues with a note, STATUS. NO install/tag.

Session cost (v0.0.19 part 1, the long session that ended 2026-10-21): last status-line reading $110.10 (`session-cost`; the reading may lag behind the session's end).

### Capacity (measured 2026-10-21)
| Space | Used | Notes |
| --- | --- | --- |
| Cartridge (ROM) | 24.6 of 32 MB | ~7 MB free; maps, scripts, text and our art are small. Biggest users are the expansion's sprites for every species |
| Save: SaveBlock1 | 15864 of 15872 bytes | **Full.** The `FREE_*` options free ~3-4 KB of link/e-Reader/Mystery Gift data we never use |
| Save: SaveBlock2 | 3968 of 3968 | full, engine data |
| Save: SaveBlock3 | 950 used + 674 spare | Research Red's own save room: put new saved data here |
| Save: PC boxes | 34144 of 35712 | 1.5 KB spare |
| Flags / vars | 2304 / 256 | ~570 flags and ~90 vars still unused (FireRed/Emerald leftovers); keep FLAGS_VARS.md current |
| RAM (EWRAM, permanent) | 249 of 256 KB | ~7 KB free; new screens should allocate while open, not keep permanent buffers |

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
