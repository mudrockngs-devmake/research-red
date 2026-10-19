# Research Red — Demo Checklist (through Brock)

Tick items as they ship. Each milestone ends with an owner playtest.

## v0.1 — Pallet Town complete
- [x] National Dex unlocked from the start (v0.0.9)
- [x] Grant QoL: reusable TMs, Running Shoes from the start, all key items, Master Balls (placeholder dialogue) (v0.0.10)
- [x] Six-starter picker (v0.0.10; Johto row stacked under the Kanto table): scrollable menu with pictures; Johto balls on a duplicate table under the Kanto one. Chain: Ivy = counter from your region, Elm = counter from the other region, Jenny = counter to Elm from his region; Oak gets the 2 leftovers
- [x] Battle sprites (64x64) scaled from the full-body ref art: Elm, casual Jenny in use; Ivy, player M/F saved in `graphics/research_red/trainer_*.png` (portraits stay for dialogue/calls). Hand cleanup welcome (v0.0.10)
- [x] Temporary overworld sprites for Ivy (Cooltrainer F) and Jenny (Sabrina) via `OBJ_EVENT_GFX_RR_*` aliases (v0.0.10). Player stays Red/Leaf until custom sprites: a player swap needs every sheet (walk/run/bike/surf/fish/item)
- [x] Cast art set from all 32 refs: battle 64x64, portrait 64x64, call 32x32 in `graphics/research_red/cast/` (Ivy swimsuit and Jenny police as alternates; Jenny casual is default)
- [ ] Custom overworld sprites (player M/F, Ivy, Elm, Jenny casual) hand-pixeled from the ref art
- [x] Fix lab battle freeze (no tutorial battle mode; player has 2 Pokemon) (v0.0.8)
- [x] Lab 2v2: player + Ivy vs Elm + Jenny with their Johto picks from the starter chain (v0.0.8; owner chose: player commands both, Ivy's mon stays in the party, so it is a normal two-trainer double)
- [x] Elm and Jenny trainer entries in `trainers.party`; Elm's battle pic from `graphics/research_red/portrait_elm.png` (no Gary) (v0.0.8; Jenny's pic from her portrait too)
- [x] Optional Oak double battle (v0.0.10; yes/no prompt, Lv 6) (his 2 remaining Kanto starters); loss branch: haiku "A student falls down... / Old man forgot to hold back... / The grant still goes through." then Elm offers his own funding
- [x] Full research grant: larger Bag pockets; every Poke Ball x100; all TMs and HMs; held and battle items; medicine (v0.0.10; Items pocket 42 -> 60, PC item storage 30 -> 10)
- [x] HMs usable as battle moves and forgettable; field moves work with no HM or badge (v0.0.10)
- [ ] Cell Phone key item: registered numbers (Elm, Jenny, Oak, Delia); calling Elm opens the Research Rules menu (see v0.2)
- [x] Ivy as NPC follower from the end of breakfast (v0.0.11; cutscenes hand off with `rr_ivy_to_scene` / `rr_ivy_follow`, data/scripts/rr_ivy.inc) (`include/config/follower_npc.h`); remove duplicate scene Ivys (including the lab)
- [x] Route 1 first walk: Elm and Jenny wait at the north end (v0.0.12); pink streak as a Mew blur sprite still to do (camera bug fixed v0.0.10) (Mewtwo blur fly-by v0.0.13)
- [x] Route 1 catch reactions: Elm/Jenny comment per species, duplicate fallback, rising jealousy; Elm runs to Route 22 (v0.0.12; reactions play when you reach them, for your first four catches)
- [ ] Mimey uses the Mr. Mime overworld sprite (verify), Johto balls behind the Kanto table (verify)

## v0.2 — Viridian, Route 22, the picnic
- [x] Route 22: Elm + Jenny 6v6 double; trainer lines every two faints and before their last pair (v0.0.13; lines in src/data/research_red/trainer_slides.h)
- [x] Old man stays asleep until the Route 22 battle (already scripted, verify) (v0.0.13: now wakes after the Viridian lunch)
- [ ] Viridian Gym roof hole as map art (Porymap) to match the "hole from the inside" text
- [x] Picnic lunch outside the Pokemon Center (dialogue in the animatic `lunch()`) (v0.0.13, plus Officer Jenny cousin gag)
- [x] Research Rules menu: level caps on/off, hard/soft cap, difficulty (Relaxed/Standard/Challenge), battle style (Shift/Set); saved; changeable by calling Elm (v0.0.13: level caps, cap style, trainer/wild doubles, battle style; Start > Rules. Difficulty still to do)
- [x] Level cap system tied to badges (v0.0.13, src/caps.c)
- [x] Encounter tables for Route 1 and Route 22 (below) (Route 1 done v0.0.12; FRLG slot rates are fixed, so: Pidgey 20, Rattata 20, Spearow 11, Mankey 10, Nidoran M 10, Oddish 10, Nidoran F 8, Bellsprout 5, Meowth 5, Eevee 1) (v0.0.13: data/research_red/encounters/, day and night)

## v0.3 — Route 2 and Viridian Forest
- [ ] Route 2 NPC, Abra encounter, trade-house Mimey gag text
- [x] Viridian Forest: Bug Catchers Rick, Doug, Anthony, Charlie, Sammy as double battles (teams in animatic `forest()`), each with one short you-and-Ivy exchange afterward (v0.0.13 teams and opening lines; you-and-Ivy exchanges still to do)
- [x] Encounter tables for Route 2 and Viridian Forest (below) (v0.0.13)
- [x] Field Guide: per-area encounter info screen (start with the Pokedex area view plus an Ivy note; custom UI later) (v0.0.13: Start > Guide)

## v0.4 -> v1.0 — Pewter and Brock
- [ ] Pewter NPCs: Gym Guide, Museum curator (forms talk with Ivy), Jigglypuff fan, Mart clerk
- [ ] Pewter Gym: Camper Liam (Geodude, Sandshrew, Diglett); Brock double (teams and mid-battle lines done v0.0.13: Brock has six); stew, puns, Boulder Badge, TM39
- [ ] Post-Brock Elm + Jenny 6v6 double outside the Gym (animatic `finale()`) (teams and lines ready v0.0.13; scene to build)
- [ ] Demo end card; first badge group call (if the call UI is ready)

## Encounter tables (grass %)
| Area | Table |
| --- | --- |
| Route 1 | Pidgey 20, Rattata 20, Spearow 12, Mankey 10, Nidoran F 8, Nidoran M 8, Oddish 8, Bellsprout 6, Meowth 6, Eevee 2 |
| Route 22 | Rattata 15, Spearow 15, Mankey 15, Nidoran F 12, Nidoran M 12, Ekans 10, Sandshrew 10, Doduo 8, Ponyta 3 |
| Route 2 | Pidgey 15, Rattata 15, Caterpie 12, Weedle 12, Venonat 10, Paras 8, Nidoran F 8, Nidoran M 8, Abra 6, Jigglypuff 6 |
| Viridian Forest | Caterpie 18, Weedle 18, Paras 10, Metapod 8, Kakuna 8, Venonat 8, Oddish 8, Bellsprout 8, Pikachu 6, Pidgey 4, Scyther 2, Pinsir 2 |

## v0.0.12 round (owner playtest of v0.0.11)
- [x] Fix "Rival Elm and Officer Elm" (rival name was applied to the partner)
- [x] Oak's table line (Kanto on top, Johto below)
- [x] Battle sprites back to plain shrink, with eye detail kept
- [x] Temporary player overworld: recolored Red (red hair, white coat) and Leaf (red hair, coat, jeans)
- [x] Muffin (Rattata) in Daisy's house; Daisy faces the door for the scene
- [x] Slowpoke wandering in Oak's lab
- [x] Forced double battles for trainers (`FLAG_RR_FORCE_DOUBLES`, on); wild doubles flag (off)
- [x] Cleanup: scenes moved to data/scripts/research_red/, rr_check.py, docs/DEV_GUIDE.md
- [x] Story data export (docs/story/, synced to E:)
- [x] Lab: losing to Elm and Jenny means they fight Oak while you watch; new decline/loss dialogue (placeholder)

## Cross-cutting (any milestone, owner decides when)
- [ ] Visual overhaul research: community battle UI branches (healthboxes, backgrounds, stat displays) in the style of Radical Red / HGSS hacks; credit authors
- [ ] Battle UI configs: show types, move effectiveness, physical/special icons, fast HP/EXP bars, trainer slide messages (check expansion configs first)
- [x] Portraits above the text box for key characters (Oak, Elm, Ivy, Jenny, Delia, Daisy, player M/F) from `graphics/research_red/cast/` (v0.0.12; automatic from the "Name:" prefix; Ivy swimsuit / Jenny police via flags)
- [x] Video call UI: call frame, signal bars, pop-in call circles; reusable script commands (start call, add/drop caller, signal strength) (v0.0.13 intro; reusable script commands still to do)
- [ ] Intro player pictures from `intro_player_m/f.png` (resize/pad to the size the intro expects)
- [ ] Title screen (Research Red logo; Slowpoke art from owner or in-game sprite) and boot credits/disclaimer screen
- [x] Test archive: `rr_install_rom.sh` (versioned builds) and `rr_archive_test.sh` (per-round folders with NOTES.md)
- [x] Testing toolkit: scripted scene playthroughs with screenshots and crash checks, checkpoint save states (v0.0.14: tools/research_red/test/, docs/TESTING.md)
- [ ] Testing extras: mGBA Lua debug overlay for the owner (map, X/Y, story vars), bug-report hotkey to `_emu/bug-reports/`
- [ ] Proper-case sweep across all game text
- [x] `docs/FLAGS_VARS.md` registry and `CREDITS.md` (CREDITS has a Permission column since v0.0.14)
- [ ] Release checklist: debug OFF, version stamp, patch only (never a ROM)

## Gameplay upgrades (owner request, schedule as bundles)
- [ ] Expand TMs beyond the vanilla 50 (expansion supports up to 100 + HMs) with modern moves
- [x] Area variety: more species per area (all gens where it fits the story) (v0.0.13 for Pallet to Viridian Forest)
- [x] Field Guide card per area (animatic) (v0.0.13: Start > Guide)

## After the demo (design first in chat)
- Badge video calls after every Gym; Elm/Jenny battles in each city
- Endgame forms arc: Mega Evolution (incl. Kalos Megas for Kanto starters), Dynamax/Gigantamax, regional and Hisuian starter forms, custom item area
- Mew (Bill arc), Mewtwo, Prof. Mudrock postgame

## Requested v0.0.13 follow-ups (now GitHub issues #1-#16; the issues are the source of truth)
- [ ] Re-reading a Pokédex entry replays its banter (the after-catch banter is done)
- [ ] Pokédex banter shown over the dex page itself instead of back on the map
- [ ] Pokémon dialogue portraits for more of the 151 + regional forms (add a line to graphics/research_red/portraits.txt)
- [ ] Battle intros/back sprites showing the player and Ivy
- [ ] Mega, Z-Moves, Dynamax, Tera: decide when the story unlocks them
