# Roadmap: every open idea, easiest and safest first

Every open issue, sorted by how much work it is and how likely it is to break
something. Made 2026-10-19 from the owner's playtest notes and ideas list;
v0.0.15 (2026-10-19) shipped 24 of them (closed on GitHub, see CHANGELOG.md).
The issues hold the details; this page is the order. Update it when issues are
added or closed.

**Risk** means: could it crash the game, break old saves, or break other scenes?
- low: text, map tiles, a script line, a setting
- medium: changes a scene's flow, a menu, or shared code (followers, portraits)
- high: new game mechanics touching battles, evolution, items or the save file

## 0. Fix first (before anything new)
| # | What | Work | Risk |
|---|---|---|---|
| [#41](https://github.com/mudrockngs-devmake/research-red/issues/41) | Level caps: Totodile didn't evolve on the hard cap after Brock | small to check | medium (caps affect every battle) |
| [#2](https://github.com/mudrockngs-devmake/research-red/issues/2) | Things still untested since v0.0.13 | playtest | none |

## 1. Quick and safe (text, settings, small script or tile fixes)
| # | What | Risk |
|---|---|---|
| [#14](https://github.com/mudrockngs-devmake/research-red/issues/14) | Route 22 odd tiles | low |

## 2. Moderate (scene rework, menus, art)
| # | What | Risk |
|---|---|---|
| [#25](https://github.com/mudrockngs-devmake/research-red/issues/25) | Opponent types shown for the whole battle (decided; new work) | medium |
| [#20](https://github.com/mudrockngs-devmake/research-red/issues/20) | Pokédex tab glitches (with #19) | medium |
| [#27](https://github.com/mudrockngs-devmake/research-red/issues/27) | Start menu clock glitch | medium |
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
| [#49](https://github.com/mudrockngs-devmake/research-red/issues/49) | Museum grass with rare fossils + museum scenes |
| [#15](https://github.com/mudrockngs-devmake/research-red/issues/15) | Portraits toward all 151 + regional forms |
| [#11](https://github.com/mudrockngs-devmake/research-red/issues/11) | Proper case and more natural text everywhere |
| [#33](https://github.com/mudrockngs-devmake/research-red/issues/33) | Animated battle sprites (gen 5 style) |
| [#60](https://github.com/mudrockngs-devmake/research-red/issues/60) | Postgame Wild Area |
| [#1](https://github.com/mudrockngs-devmake/research-red/issues/1) | Post-Elite Four payoff: Mega Evolution and regional forms |

## Waiting on the owner (decisions)
- [#29](https://github.com/mudrockngs-devmake/research-red/issues/29) The pager item's final name ("Oak's Pager" for now; Claude suggests "Ranch Pager")
- Decided 2026-10-19: #6 closed (the rules already cover difficulty), #7 (our cast art in battle for now), #35 (Pewter's usual blocker), #53 (Oak + Daisy double, Muffin, EXP not money), #56 (heal or rest at Joy and Delia), #58 (held items never break); #1 (Mega + regional forms are the post-Elite Four payoff; no Z-Moves/Dynamax/Tera this game; see STORY_BIBLE.md), #3 (hide portraits during pans), #25 (types shown all battle)

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

## Build v0.0.16 (planned with the owner 2026-10-19/20; built 2026-10-20)
The owner skips the v0.0.15 playtest and tests v0.0.16 long-term (and gives it to the v0.0.13 testers).
Every decision is on its issue; the character sheets are in `docs/characters/`, the anime and lore reference in `docs/reference/`.

**Already in master (since v0.0.15):** Pewter rematch outside the Center (#10); level caps on/off, counting badges, Rare Candy cap, catch-up EXP, Rules shows the cap (#41); Johto Pokémon and the evolution-level rule in every demo table, one Fishing Rod (#59); TV homage; Diglett's Cave and the League gate closed; Oak/Ivy line updates; reference database and character sheets.

**Step 1: dialogue pass, scene by scene** (Claude redrafts in the sheets' voices, the owner reviews in chat): every demo scene, plus `docs/characters/REWORK.md` (Elm knows his Jenny, the dinner-at-Delia's bet, the joint paper with Oak as PI, the Viridian City sing-along with the owner's phrases, Elm and Jenny saw your Brock win), tutorial banter for new features (Ranch Pager, PC, DexNav, Options > Research Rules), Brock watch scene (`docs/scenes/pewter_brock_watch.md`).

**Step 2: systems and content** (test each one):
- Menus: Start menu cleanup (Rules into Options #45, Guide into the Pokédex), DexNav, rename + restore names (#71).
- Battles: types all battle (#25), held items restored after battle incl. Berries (#58), choose which wild to catch in doubles (#55: not supported by the engine; moved to later).
- Evolution and moves: evolve on demand (#50), trade evolutions at 37, no Linking Cord (#65), pre-evolution moves relearnable at any level (#51), 100 TMs all from the grant (#57).
- Ranch Pager (#29), Rest for day/night + clock fix (#56, #27), rule-change calls (#13, #45).
- Rematches: Oak + Daisy (#53), Delia (#70), Brock tiers (#52); watch Elm and Jenny vs Brock (#69; Elm and Jenny inside the Gym entrance during your run; Brock's nine siblings: 5 in the Gym, 4 in town, generic kid sprites); dex banter cutscene and replay with field notes (#5, #4).
- World: ambient Pokémon pilot (#64), Forest exchanges + the Samurai (#9), Pokédex Area tab (#20), proper case (#11), Cruller (Jenny's Growlithe) in the Daisy scene and on Jenny's team, features list + a lab aide who explains them (#72).
- Art: Jenny sprites (#40), surf sprite (#28), Trainer Card (#24), portraits (#32), picnic blanket (#8), battle sprites (#7), title screen + Slowking disclaimer and demo notice + a Mareep "A fan project by MudrockNGS" boot splash (#12, mock-ups first), cozier interface colours (#73, mock-ups first), battle backgrounds per location (#74). If the build drags, the art items move to v0.0.17.

**Step 3:** full test run, the what's-new report with screenshots of every change (#75, first version: start to demo end), release v0.0.16, CHANGELOG with a clear **Save:** line.

**Shipped in v0.0.16 (2026-10-20):** all of Step 1; from Step 2 the menus, battles, evolution and moves, 100 TMs (#57), Ranch Pager with Oak's Pokémon on the maps (#29), Rest and the clock fix (#56, #27, incl. night water tables), reusable video calls and the Rules call (#13, #45), rematches for Oak + Daisy, Delia and Brock with a post-game tier (#53, #70, #52), features list and the lab aide (#72). The rest moved to v0.0.17 (below) so the owner and testers can play the new systems.

**Shipped in v0.0.17 (2026-10-20):** the Field notebook look (#73: menus, dialogue, battle text box, FIGHT menu, HP boxes; `tools/research_red/ui_theme.py notebook`), item descriptions on pickup, snappier battle text, B to RUN in wild battles.

**Shipped in v0.0.18 (2026-10-20):** battle backgrounds per place in cozier colours (#74, `tools/research_red/battle_bg_theme.py`; Viridian Forest uses the long-grass background), the test-mode wild battle freeze fix, the route planner and memory pokes for tests (#18, part).

## v0.0.19 (moved from v0.0.16-18 on 2026-10-20; built 2026-10-21, see CHANGELOG.md and STATUS.md)
**Owner decision (2026-10-20, after the v0.0.18 playtest): one big v0.0.19** with everything below, in this order:

**Owner answers (2026-10-20, all suggestions approved):** Claude drafts all new lines (owner reviews in v0.0.20); Oak steps in with a real self-playing battle (Pidgeot, Tauros), Oak always wins; Elm and Jenny narrowly beat Brock in the watch scene; Claude polishes the v0.0.16 placeholder lines; starter moves Lv 5: Bulbasaur Tackle/Growl/Vine Whip, Charmander Scratch/Growl/Ember, Squirtle Tackle/Tail Whip/Water Gun, Chikorita Tackle/Growl/Razor Leaf, Cyndaquil Tackle/Leer/Ember, Totodile Scratch/Leer/Water Gun; Claude picks the TM swaps (listed in the report); Kanto dex = 151 + Johto Pokémon met in Kanto after #151, National = official numbers; banter over the dex page after a new entry, SELECT on an entry replays it; Field Guide below Pokédex in the Start menu; art from official games and free community art, credited (list what was used); bike: player's own sprite, Ivy waits while riding; female overworld: Flannery if no scientist sprite; title screen + Mareep splash + Slowking disclaimer built in (owner reviews in v0.0.20); Emerald map pop-ups and first-time item descriptions ON; old saves don't need to work (new game); ambient Pokémon in the demo areas; post-game Mega rematches held. Also: better positioning everywhere (breakfast: everyone seated eating). Tool upgrades approved as needed. Stop before install/tag/release.
1. Quick fixes: portraits stay on the last page (#76), type display back to the engine's (#78), Start menu PC (#79), intro call name (#81), Move Info black (#82), Sumac -> Sequoia (#94), starter movesets (#87).
2. Pokédex: national numbers (#77), Area tab (#20), banter over the dex page + replay (#5, #4), Field Guide in the Start menu (#91).
3. Scenes: Jenny follows Elm (#83) first, then the Daisy tea scene (#84), Route 1 rework (#85), Elm's dialogue (#88), Oak steps in after a lab loss (#86), Cruller.
4. Look: battle boxes in notebook colours (#92), proper case (#11), female overworld (#93), bike (#80), Pager/surf sprites (#28), battle back sprites (#7), portraits in battle dialogue (#95), Trainer Card (#24), Jenny (#40), portraits (#32), picnic blanket (#8).
5. Bigger extras: landscape battle backgrounds (#74), Forest exchanges (#9), watch the Gym battles (#69), rematches (#52), ambient Pokémon (#64), move relearner (#89), TMs vs tutors (#90).

Older notes for this build:
- Battle backgrounds, round 2 (#74, optional): new art (not just colours) for places that deserve it, e.g. the lab, the sea, Brock's Gym; mock-ups first, within the palette and tile limits.
- **Waiting on the owner:** title screen + boot splash mock-ups (#12).
- Rematches: post-game Mega Evolutions, the other Gym leaders as their Gyms are reworked (#52); watch Elm and Jenny vs Brock (#69); dex banter cutscene and replay with field notes (#5, #4).
- World: ambient Pokémon pilot (#64), Forest exchanges + the Samurai (#9), Pokédex Area tab (#20), proper-case sweep (#11, also "PKMN TRAINER", "YES/NO", FireRed's map names), Cruller (Jenny's Growlithe) in the Daisy scene and on Jenny's team.
- Art: Jenny sprites (#40), surf sprite (#28), Trainer Card (#24), portraits (#32), picnic blanket (#8), battle sprites (#7), the Ranch Pager's own icon (Poké Radar's for now). The notebook colours for the map name pop-ups and other screens that keep their own palettes (Bag, party, summary, Pokédex), if wanted.
- Rework the placeholder lines added in v0.0.16 (Ranch Pager tutorial, Oak's Pokémon, Rest, the Rules call, rematches, the lab aide).
- Engine extras to consider (built in, off for now): Emerald-style map pop-ups (`OW_MAP_POPUP_EMERALD`); item descriptions only the first time (needs a save-layout change, so at a save-version bump); follower Pokémon (clashes with Ivy following, an experiment at most). Type-tinted battle text boxes would be new work.

**After v0.0.16:** Claude's test-tool upgrades (#18: memory-poke warps and no-encounter mode, a route planner from map data, the engine's battle tests, wait-until-ready), smaller patch (#17).
