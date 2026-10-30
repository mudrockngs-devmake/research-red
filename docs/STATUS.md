# Status

The one-page "where are we right now". **Read it at the start of every session;
update it at the end of every session** (and whenever a build goes out).
Keep it short (about 80 lines): when a run is finished, move its checklist to
[history/](history/) (one file per month) and leave a one-line result here.

_Last updated: 2026-10-30 (v0.0.22 still waiting on the owner's spot check; #108 fixed in master, not in a build yet)_

## Now
- **v0.0.21 spot check done and archived** (`test-archive/2026-10-29_v0.0.21_spotcheck/`): new issues #149-#156; #116, #139, #140, #148 closed; follow-ups on #105 (Elm and Jenny's perspective, instant gag, auto-advance), #110, #142, #143.
- Two lanes: game-lane sessions use the art already in the game; Art Kit sessions improve the tools and drafts
  (#104: Pallet Town first). No Art Kit piece is approved yet (`art_gate.py status`). Exception (owner 2026-10-28,
  #125): v0.0.20 tries the cast's overworld drafts as they are, for testing only (not approved).
- **#108 day and night fixed in master, not in a build yet** (2026-10-30): two light levels, day fully bright, night option B (owner's pick). Smoke 70/70; only the daynight scene looks different from the last full run. Next: the v0.0.23 build (test-build skill), after the owner's OK.
- Since v0.0.19 (in master, not in a build yet): tester upgrades (#18, docs/TESTING.md); debug 10's team is Lv 14.
- New test tools (2026-10-28, docs/TESTING.md): checks on every scene (dialogue/portraits and boxes closing early,
  leftovers, battles #128/#107, brightness, wild, script coverage), settings matrix (`matrix.py`), type chart engine
  test (#112), sprite tours (#125), day/night (#108, fails until fixed), wild probes (#106, fail until fixed), and the
  **full playthrough** (`playthrough.py`, power-on to the Pewter rematch, no debug skips): Quick Balls from Lab
  Supplies, one training round with catching, Rare Candies to Lv 14, beats Brock (2026-10-28). The full run from
  power-on can still black out on Route 2 to #106's leftover Squirtle (wild Pokémon reuse Oak's team) when the team
  is weak; after #106 is fixed it should go through. Not yet in it: Brock rematch tiers, Delia, Elm/Jenny rebattles
  (#127, open question on EXP/money).

## Working setup run (2026-10-28, items 1-6 done; owner answers in ROADMAP.md "Working setup")
- [x] 5 short STATUS, history in docs/history/
- [x] 1 glossary docs/GLOSSARY.md
- [x] 2 game design document docs/DESIGN.md
- [x] 3 notes intake skill (.claude/skills/owner-notes)
- [x] 4 project skills: test build, playtest intake, new scene
- [x] 6 rr_check before every commit (git hook)
- [ ] later, in a claude-setup session: 7 session-start hook, 8 project template (glossary, DESIGN, short STATUS +
      history, .claude/skills, .githooks + core.hooksPath, GitHub checks, issue forms), 9 handbook, 10 global owner-notes skill

## Current build
- **v0.0.22 The lab and its scripted fights** (2026-10-29, spot-check round): installed as `ResearchRed_local.gba`,
  tagged, pre-release on GitHub, and in the handheld folder `F:\ROM-DATABASE\ROMs\gba\RomhackDev` with
  `what to check.txt` and `Claude's answers.txt` (owner 2026-10-29: every build from now on). #105 fights seen from
  Elm & Jenny's side, instant gag, text plays itself, watch offered after a win, quiet heals; #150 #151 #152 #153 #154;
  #143 spot (left of the Center, facing down); debug 15 fixed (played the first rematch since v0.0.21).
  Smoke 69/70 (daynight until #108), settings matrix 21/21, playthrough doubles 14/14, singles 12/14 (bot loses to
  Brock, #156). What's-new report: `E:\Workspace\romhacks\research-red\whats-new\v0.0.22\report.html`.
  **Save: old saves work.** Issues #150-#154 stay open until the owner's spot check.
- Last playtest archived: `test-archive/2026-10-29_v0.0.21_spotcheck/`.

## Waiting on the owner
- [ ] Balance: the Pewter rematch with Elm and Jenny wins 6% for a
      team at the cap (Lv 14), Brock 94% (docs/BALANCE.md "Simulated fights"). Tune it, or keep it a hard optional fight?
- [ ] Try the viewer (F5: Approve / Needs work, Mark a spot, Animation, In game, History, themes) and the hub's look,
      Today screen, Settings and desktop shortcut; say what feels off.
- [ ] Hub "I'm done for today" (backups were 5 days old on 2026-10-24).
- [ ] The 3 questions at the end of docs/ART_BIBLE.md (outline colour, battle picture proportions, signature colours).
- [ ] Rework draft lines: Oak's grant / Lab Supplies, the lab's Oak rounds (#86, now #105), the aide's PC topic,
      Slowking's pages; and the v0.0.16 placeholders (Ranch Pager tutorial, Oak's Pokémon, Rest, the Rules call, rematches, the aide).
- [ ] Optional: phone emulator / real hardware (#16); workshop the report's playtest form.

## Next up
- **Small themed builds** (owner 2026-10-28): 3-5 related issues per test build, risky systems on their own, each fix
  with a test that fails before and passes after. **Spot-check rounds** for most builds (a debug jump per change,
  the owner plays only those); a **full playthrough** about every third build (owner 2026-10-29, docs/PLAYTESTING.md).
  Milestones on GitHub (plan agreed 2026-10-29):
  - v0.0.21 Elm & Jenny on the road: built and spot-checked 2026-10-29.
  - **v0.0.22 the lab and scripted fights: built 2026-10-29, waiting for the owner's spot check.**
  - Next: v0.0.23 day and night (#108), plus the battle pictures (#133) and the overworld sprite placeholders for testing (#125; needs a "testing" route in the art gate, drafts stay unapproved); v0.0.24 Pallet and Daisy's house (full playthrough);
    the lunch rework (#155, new travelling talk: draft lines for the owner first) gets its own build.
  - Later milestones (shifted one on 2026-10-29): v0.0.25 text and menus, v0.0.26 levels and balance, v0.0.27 moves
    and evolution, v0.0.28 battle pictures, v0.0.29 the lab's look (GitHub milestones).
  - Then the features: scripted battle scenes (#142 vs Brock, on #105's system), #121's video
    call, #113, #114 and the rest of ROADMAP.md.
- Tasks for the owner: #129 credits/permissions list, #130 Oak's aides list.
- Tooling: a smaller patch (#17).

## Watch closely in the v0.0.19 playtest (new or reworked code, most likely to break)
- Lab Supplies: buying, a full Items pocket ("no more room"), leaving back to the PC menu, from Start > PC and a real PC.
- The lab scene after the Elm and Jenny battle, win and loss, Yes and No to both questions; Slowpoke on the table later; Muffin when Daisy visits.
- The dex banter after a catch and with SELECT: B to skip, mashing A or B, Elm/Jenny family species, switching the look (debug 13).
- Battles where you or Ivy speak (Route 22): pictures appearing and leaving.
- The move relearner, the PC item box and the Berry Pouch (pocket sizes changed), saving and loading on a new game.

## Asked for, not done yet
- [x] Viewer (owner 2026-10-30): zoom in/out below 100%, fit to any window, pictures shrink dynamically, usual viewer features.
- [x] Viewer (owner 2026-10-30): every picture Claude takes, tests or generates shows up in the live stream, efficiently.
- [x] (in master, goes into v0.0.23; drafts stay unapproved) v0.0.23 (owner 2026-10-30): use the updated overworld sprite placeholders for the player, Ivy, Jenny, Elm and Delia (#125), without a big break from the roadmap.
  - Swap-back (owner 2026-10-30), done and tested both ways: `art_gate.py test ow:elm off`, then rebuild (`docs/ART_TOOLS.md`).
  - These sprites are FireRed/Emerald sprites recoloured by recipe (`recipes.toml`), with colours picked from the large art; they aren't shrunk from it, so the large-art-first rule for combined battle pictures doesn't change them.
- [x] (in master, owner to check in the viewer) Battle pictures (owner 2026-10-30, #133): player and Ivy closer together, like Elm and Jenny; in the front-view lab cutscenes Elm and Jenny's picture faces the same way as the player and Ivy's (not mirrored). Male player a little bigger.
- [x] (in master, owner to check in the viewer) Oak's pager Pokémon (owner 2026-10-30): spread them out on the Pallet map and in the lab so they aren't lined up vertically, until a ranch tile area exists.
- [x] (in master) Owner's reply 2026-10-30: female player a tiny bit smaller; lab Farfetch'd (11,6) and Machop (9,11) clear of the bookcases. Sharper faces tried and dropped: the stronger settings came out muddier and lost Ivy's hair colour (owner: not if splotchy). Jenny stays diagonal from Elm in Pallet: Elm steps onto her column in the grass scene, then she steps in behind him.
- [ ] Gerald (#113, owner 2026-10-30): between the two fence posts, more Tentacool blocking the gap, text adjusted. Set to milestone v0.0.24 (Pallet and Daisy's house).
- [ ] Owner's v0.0.22 spot-check notes (2026-10-30), read back before filing: `_emu/notes/v0.0.22_spotcheck_notes.txt` (12 items: swipe-in bug, first Oak fight half as long, Brock fight in battle, Oak and Ivy lines, skip-all safety, Tracy replaces the systems aide, start menu rework if it fits v0.0.23).
- [x] (in master, tested) Pewter Gym tidy-up for v0.0.23 (owner 2026-10-30): clean up Camper Liam's dialogue (keep the light-years joke; explain why Brock is making lunch); Elm and Jenny stand off to the side when you first enter, watching and cheering; Brock's nine brothers and sisters behind him, with dialogue about them and the stew so it all makes sense; Brock's portrait over the text box before the battle
- [ ] Owner 2026-10-30, v0.0.23 extras (approved to roll all in, test, and show before the install decision): banter for the regional forms not yet mentioned (Kanto: Articuno, Zapdos, Moltres, Growlithe, Arcanine, Voltorb, Electrode; Johto: Slowking, Corsola; owner 2026-10-30: never mention Mega Evolution or Hisuian forms in any banter (Mega is learned about later; Hisuian would spoil Typhlosion), so Growlithe, Arcanine, Voltorb, Electrode, Qwilfish, Sneasel get none); try the police sprite for Officer Jenny in the picnic scene; Alolan Geodude goes to party, or box if full, without tripping flags or the Pokedex; find out why the bot lost to Brock; anything else that helps this update; verify with proper playthroughs.
- [x] (in master, being tested) Brock gives an Alolan Geodude instead of TM39 Rock Tomb (owner 2026-10-30), with Ivy and player dialogue and a Dex banter entry; check whether regional forms for Kanto/Johto have banter yet
- [ ] Daisy's house: fix Ivy's position so she's sitting in the chair (owner 2026-10-30)
- [ ] (coded, tested 2026-10-30, waiting for the owner's OK to install and tag) v0.0.23 is one big build, just this once (owner 2026-10-30; later builds go back to small): #108, #133, #125 (six sprites only: players, Ivy, Elm, Jenny, Delia, Oak; done), the 12 spot-check notes (#105, #142 Brock in battle, Tracy, start menu #124, swipe-in bug), Elm's lines towards evolution. Owner may want to rework Oak's new line.

## Half-finished / open threads
- Tool bug: `compare.py`'s automatic "vs v0.0.x" uses much older runs as the "before" for many scenes (26 of 34 flagged on 2026-10-30 were from v0.0.19), so it over-reports. Until fixed, use `compare.py RUN --against LAST_FULL_RUN`.
- Nothing half-done. All Art Kit art is drafts (tracker: docs/ART_TRACKER.md).
- In AI-vs-AI test battles the player's side plays almost at random (no partner = no AI settings); harmless so far.
  The lab's watched fights are now scripted (#105); #142 (Elm & Jenny vs Brock) should reuse that system.
- Still needs the owner's eyes: #20 (Pokédex tab glitches), #14 (Route 22 tiles), #37 (Ivy and ledges), #41 (Totodile under the cap; Cyndaquil evolving at 14 is expected).
- The League gate block (Route 22) has no automatic test; check by hand (#2).
- 13 rows of `docs/reference/kanto_anime.csv` are marked "memory" (to verify).

## Session costs not yet logged (log them in the next playtest round's NOTES.md)
- Logged up to 2026-10-27 in the v0.0.19 playtest NOTES.md. The session started 2026-10-27 21:29 ran three days
  (v0.0.20 intake, v0.0.21 and v0.0.22 builds, v0.0.21 intake): **$113.81** at the 2026-10-30 wrap-up (3h41m API).
  It's in the v0.0.21 NOTES.md; one long session shows as one line in `session-cost`, not one per day (that's why
  the daily log looked empty). Starting a new conversation each day gives one line per day.

## Capacity (measured 2026-10-21)
| Space | Used | Notes |
| --- | --- | --- |
| Cartridge (ROM) | ~24.6 of 32 MB | ~7 MB free |
| Save: SaveBlock1 | 13732 of 15872 bytes | 2140 free |
| Save: SaveBlock2 | 2732 of 3968 | 1236 free |
| Save: SaveBlock3 | 1536 of 1624 | Research Red's own room: 674 bytes of padding + 88 |
| Save: PC boxes | 34144 of 35712 | 1.5 KB spare |
| Flags / vars | 2304 / 256 | ~570 flags and ~90 vars unused; keep FLAGS_VARS.md current |
| RAM (EWRAM, permanent) | 250156 of 262144 bytes | ~11.7 KB free; new screens should allocate while open |

## History
Finished runs, older builds and their checklists: [history/2026-10.md](history/2026-10.md).

## How to resume in a new session
1. Open Ubuntu (or double-click `E:\Workspace\Claude Hub.bat`): Work on a project > research-red > Start a fresh conversation.
2. Claude reads `CLAUDE.md` (rules) and this file (state), then `git log -5` and `gh issue list`.
3. If the test runner is missing (`~/romhacks/_tools/rr_run`), run `tools/research_red/test/setup_mgba.sh`.
