# Roadmap: every open idea, easiest and safest first

Every open issue, sorted by how much work it is and how likely it is to break
something. Made 2026-10-19 from the owner's playtest notes and ideas list.
The issues hold the details; this page is the order. Update it when issues are
added or closed.

**Risk** means: could it crash the game, break old saves, or break other scenes?
- low: text, map tiles, a script line, a setting
- medium: changes a scene's flow, a menu, or shared code (followers, portraits)
- high: new game mechanics touching battles, evolution, items or the save file

## 0. Fix first (before anything new)
| # | What | Work | Risk |
|---|---|---|---|
| [#19](https://github.com/mudrockngs-devmake/research-red/issues/19) | Pokédex crash (blue screen) going from the Area page to the next | unknown until reproduced | it *is* a crash |
| [#41](https://github.com/mudrockngs-devmake/research-red/issues/41) | Level caps: Totodile didn't evolve on the hard cap after Brock | small to check | medium (caps affect every battle) |
| [#2](https://github.com/mudrockngs-devmake/research-red/issues/2) | Things still untested since v0.0.13 | playtest | none |

## 1. Quick and safe (text, settings, small script or tile fixes)
| # | What | Risk |
|---|---|---|
| [#26](https://github.com/mudrockngs-devmake/research-red/issues/26) | New game defaults: fast text, L/R buttons | low |
| [#47](https://github.com/mudrockngs-devmake/research-red/issues/47) | Lab: proper dialogue after losing to Elm and Jenny | low |
| [#48](https://github.com/mudrockngs-devmake/research-red/issues/48) | Elm's pastry nicknames for his starters | low |
| [#31](https://github.com/mudrockngs-devmake/research-red/issues/31) | Daisy's normal lines with her portrait | low |
| [#38](https://github.com/mudrockngs-devmake/research-red/issues/38) | Elm and Jenny leave Daisy's house through the door | low |
| [#39](https://github.com/mudrockngs-devmake/research-red/issues/39) | Muffin actually on the chair | low |
| [#22](https://github.com/mudrockngs-devmake/research-red/issues/22) | Portrait and name tag sit lower (no gap when bouncing) | low |
| [#23](https://github.com/mudrockngs-devmake/research-red/issues/23) | Oak's Lab table pieces join up | low |
| [#14](https://github.com/mudrockngs-devmake/research-red/issues/14) | Route 22 odd tiles | low |
| [#61](https://github.com/mudrockngs-devmake/research-red/issues/61) | Lab Slowpoke wanders too fast | low |
| [#35](https://github.com/mudrockngs-devmake/research-red/issues/35) | Demo blocker at Pewter's exit | low |
| [#36](https://github.com/mudrockngs-devmake/research-red/issues/36) | Demo blocker: Gerald at Pallet's shore | low |
| [#54](https://github.com/mudrockngs-devmake/research-red/issues/54) | Rule: heal after every battle | low (new rule flag) |

## 2. Moderate (scene rework, menus, art)
| # | What | Risk |
|---|---|---|
| [#30](https://github.com/mudrockngs-devmake/research-red/issues/30) | Route 1 Mewtwo: flies right to upper left; Elm and Jenny wait by the grass | low-medium |
| [#43](https://github.com/mudrockngs-devmake/research-red/issues/43) | Viridian lunch starts by talking to Elm and Jenny right of the Pokémon Center | medium (do with #44) |
| [#44](https://github.com/mudrockngs-devmake/research-red/issues/44) | Research Rules talk moves to Daisy's house | medium (story order; old saves past Daisy need care, docs/SAVES.md) |
| [#46](https://github.com/mudrockngs-devmake/research-red/issues/46) | Lab: watch or skip Elm and Jenny vs Oak | medium |
| [#21](https://github.com/mudrockngs-devmake/research-red/issues/21) | Portraits hide while a choice menu is open | medium (portrait code is shared) |
| [#3](https://github.com/mudrockngs-devmake/research-red/issues/3) | Hide portraits during camera pans (decided) | medium |
| [#25](https://github.com/mudrockngs-devmake/research-red/issues/25) | Opponent types shown for the whole battle (decided; new work) | medium |
| [#42](https://github.com/mudrockngs-devmake/research-red/issues/42) | Field Guide cleanup: icons, caught color, day and night | low-medium |
| [#20](https://github.com/mudrockngs-devmake/research-red/issues/20) | Pokédex tab glitches (with #19) | medium |
| [#27](https://github.com/mudrockngs-devmake/research-red/issues/27) | Start menu clock glitch | medium |
| [#34](https://github.com/mudrockngs-devmake/research-red/issues/34) | PC from the Start menu (grant-to-PC part is tier 3) | medium |
| [#37](https://github.com/mudrockngs-devmake/research-red/issues/37) | Ivy hops ledges with the player | medium (follower code runs everywhere) |
| [#40](https://github.com/mudrockngs-devmake/research-red/issues/40) | Jenny walking frames; cousin Jenny in police uniform | low (art) |
| [#28](https://github.com/mudrockngs-devmake/research-red/issues/28) | Surf sprite: the player instead of Red | low (art) |
| [#24](https://github.com/mudrockngs-devmake/research-red/issues/24) | Trainer Card picture | low (art) |
| [#32](https://github.com/mudrockngs-devmake/research-red/issues/32) | Portraits for Pokémon characters (Mimey, Slowpoke, Jigglypuff...) | low |
| [#8](https://github.com/mudrockngs-devmake/research-red/issues/8) | Picnic blanket for the lunch (with #43) | low (art) |
| [#12](https://github.com/mudrockngs-devmake/research-red/issues/12) | Title screen and boot disclaimer | low-medium |
| [#59](https://github.com/mudrockngs-devmake/research-red/issues/59) | Johto Pokémon in the encounter tables, rebalanced | low code risk, lots of data work |

## 3. Bigger systems (new mechanics; build one at a time, test well)
| # | What | Risk |
|---|---|---|
| [#13](https://github.com/mudrockngs-devmake/research-red/issues/13) | Reusable video-call commands (needed by #45) | medium |
| [#45](https://github.com/mudrockngs-devmake/research-red/issues/45) | Rule changes play as a call with Elm and Jenny | medium |
| [#51](https://github.com/mudrockngs-devmake/research-red/issues/51) | Move relearner in the party menu, egg moves | medium (the expansion has the screen) |
| [#57](https://github.com/mudrockngs-devmake/research-red/issues/57) | More TMs; move tutors as TMs | medium (TM list, balance) |
| [#29](https://github.com/mudrockngs-devmake/research-red/issues/29) | Oak's Pokémon do field moves; one "pager" item incl. Fly | medium-high (field moves, new item) |
| [#56](https://github.com/mudrockngs-devmake/research-red/issues/56) | Time of day: clearer night, rest at Pokémon Centers | medium-high (the clock) |
| [#5](https://github.com/mudrockngs-devmake/research-red/issues/5) | Dex banter as a cutscene over the entry, before nicknaming | medium-high (catch flow) |
| [#4](https://github.com/mudrockngs-devmake/research-red/issues/4) | Replay banter from the Pokédex | medium |
| [#6](https://github.com/mudrockngs-devmake/research-red/issues/6) | Difficulty rule (Relaxed / Standard / Challenge) | medium-high (every trainer) |
| [#7](https://github.com/mudrockngs-devmake/research-red/issues/7) | Battle intros and back sprites: player and Ivy | medium (lots of art) |
| [#50](https://github.com/mudrockngs-devmake/research-red/issues/50) | Evolve on demand from the party menu | high (evolution, every Pokémon, saves) |
| [#58](https://github.com/mudrockngs-devmake/research-red/issues/58) | Held items not used up / restock shop | high (battle engine) |
| [#52](https://github.com/mudrockngs-devmake/research-red/issues/52) | Gym leader rematches at chosen difficulty | high (many teams, caps, money) |
| [#53](https://github.com/mudrockngs-devmake/research-red/issues/53) | Oak rematches, team grows per badge | medium-high (after #52, same system) |
| [#55](https://github.com/mudrockngs-devmake/research-red/issues/55) | Wild doubles: choose which to catch | high (battle engine) |

## 4. Later milestones and big content
| # | What |
|---|---|
| [#9](https://github.com/mudrockngs-devmake/research-red/issues/9) | Viridian Forest exchanges after each Bug Catcher (v0.3) |
| [#10](https://github.com/mudrockngs-devmake/research-red/issues/10) | Pewter finale: Elm and Jenny's double (v1.0) |
| [#49](https://github.com/mudrockngs-devmake/research-red/issues/49) | Museum grass with rare fossils + museum scenes |
| [#15](https://github.com/mudrockngs-devmake/research-red/issues/15) | Portraits toward all 151 + regional forms |
| [#11](https://github.com/mudrockngs-devmake/research-red/issues/11) | Proper case and more natural text everywhere |
| [#33](https://github.com/mudrockngs-devmake/research-red/issues/33) | Animated battle sprites (gen 5 style) |
| [#60](https://github.com/mudrockngs-devmake/research-red/issues/60) | Postgame Wild Area |
| [#1](https://github.com/mudrockngs-devmake/research-red/issues/1) | Post-Elite Four payoff: Mega Evolution and regional forms |

## Waiting on the owner (decisions)
- [#29](https://github.com/mudrockngs-devmake/research-red/issues/29) The pager item's name (Oak's field-move Pokémon decided 2026-10-19, see the issue)
- Decided 2026-10-19: #1 (Mega + regional forms are the post-Elite Four payoff; no Z-Moves/Dynamax/Tera this game; see STORY_BIBLE.md), #3 (hide portraits during pans), #25 (types shown all battle)

## Tooling (no game changes)
[#16](https://github.com/mudrockngs-devmake/research-red/issues/16) phone / hardware test,
[#17](https://github.com/mudrockngs-devmake/research-red/issues/17) smaller patch,
[#18](https://github.com/mudrockngs-devmake/research-red/issues/18) tester "later" ideas (a Pokédex-tabs scenario would help #19).

## Keep as is (the owner likes these)
- Type-effectiveness circles on moves.
- Brock's teams; the Caterpie twins "Cater" and "Pee"; normal trainers keep their original teams (gym leaders and key characters 6v6); the wild-doubles rule.

## Ground rule for new systems
Document every new system (how it works, which files, how to turn it off) and
test it with the smoke tests, so nothing breaks and each one could be reused in
other ROM hacks later.

## Suggested next playtest (v0.0.15)
Everything in tiers 0 and 1, plus the scene fixes that polish what's already
playable: #30, #43 + #44 (+ #8), #21, #42, #34, #20. That's mostly low risk,
fixes the crash, and keeps the big systems (tier 3) for later builds, one at a
time.
