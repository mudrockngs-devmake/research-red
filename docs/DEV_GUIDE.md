# Research Red: Developer Guide

How the project is laid out and how work moves through it.

## Where Research Red lives

| What | Where |
|---|---|
| Story scenes (one file each) | `data/scripts/research_red/rr_*.inc` (included at the end of `data/event_scripts.s`) |
| Map hooks | Each FireRed map keeps only its `map_script` / trigger lines pointing at the `RR_*` scene label |
| Ivy follower | `data/scripts/research_red/rr_ivy.inc`; macros in `asm/macros/event.inc` (Research Red section); C in `src/research_red.c` |
| Dialogue portraits | `src/rr_portrait.c`: a line starting "<Speaker>:" shows that portrait and a name plate above the box for the rest of the scene. Speakers, colors and outfits: `graphics/research_red/portraits.txt` (run `build_portraits.py`). Pokemon portraits are cut from their front sprites |
| Custom C | `src/research_red.c` (new natives); small tagged edits elsewhere (`grep -rn "Research Red" src include`) |
| Trainers | `src/data/trainers.party` (Research Red block at the end); IDs in `include/constants/opponents.h` |
| Flags / vars | `include/constants/flags.h`, `vars.h` (`FLAG_RR_*`, `VAR_RR_*`), registered in `docs/FLAGS_VARS.md` |
| Config switches | `include/config/*.h`, every change tagged `// Research Red` |
| Cast art | `graphics/research_red/cast/<name>/{battle,portrait,call}.png`; hand-made overrides in `cast_handmade/` |
| Video calls | `src/rr_intro_call.c` (Discord-style; version stamp at the top of `sCallScript`). FireRed's Oak speech in `src/oak_speech.c` is bypassed. Later calls (#13): write the steps, add an entry to `sCalls` and a number to `include/constants/rr_calls.h`; a script starts one with `fadescreen FADE_TO_BLACK`, `setvar VAR_0x8004, RR_CALL_X`, `callnative RR_ScriptStartCall`, `waitstate` (example: `RR_EventScript_ResearchRules`) |
| Debug skips | `data/scripts/debug.inc` (Start > Debug > Scripts, named 1-9; table in [PLAYTESTING.md](PLAYTESTING.md)) |
| Save version | `include/rr_save_version.h`, `src/rr_save_version.c` ([SAVES.md](SAVES.md)) |
| Tests | `tools/research_red/test/` ([TESTING.md](TESTING.md)) |
| Wild Pokemon | `data/research_red/encounters/<area>.txt` -> `tools/research_red/build_encounters.py`; day/night tables (`OW_TIME_OF_DAY_ENCOUNTERS`) |
| Field Guide / Research Rules | `src/rr_field_guide.c` (Pokédex > START > FIELD GUIDE; the menu entry is in `src/pokedex_plus_hgss.c`, its words in `graphics/pokedex/hgss/tileset_menu_list.png`), `src/rr_rules.c` (Options > Research Rules, from Daisy's house on; the row is in `src/option_menu.c`) |
| Pokedex banter | `data/research_red/dex_banter.txt` -> `tools/research_red/build_dex_banter.py`; `src/rr_dex_banter.c` (queued on new species, played back on the map) |
| Title screen, splash, notices (#12) | `src/rr_title.c` (FireRed's `CB2_InitTitleScreen` hands over to it); picture from `tools/research_red/build_title.py` -> `graphics/research_red/title/`. The version shown comes from the stamp in `src/rr_intro_call.c` |
| TMs | List: `include/constants/tms_hms.h` (TM51-100 are ours, #57/#90). Descriptions: `data/research_red/tm_descriptions.txt` -> `tools/research_red/build_tm_descriptions.py`. Moves the relearner teaches as tutor moves (`src/data/tutor_moves.h`) should not also be TMs |
| Mid-battle lines | `src/data/research_red/trainer_slides.h` (adds slide types RR_TWO_DOWN / RR_FOUR_DOWN) |
| Tools | `tools/research_red/` |

## Conventions
- Labels: `RR_<Scene>_EventScript_*`, `RR_<Scene>_Text_*`, `RR_<Scene>_Movement_*`.
- Text: proper case, max 34 characters per line, story lines use `NPC_TEXT_COLOR_NEUTRAL`.
- Staging cutscenes (`asm/macros/event.inc`, Research Red section): `rr_move_pair leader, follower, movement` (the follower trails one step behind, e.g. Jenny behind Elm, #83; wait on the follower), `rr_walk_to id, x, y[, yFirst, delay]` (walk someone to a tile from wherever they are, e.g. Ivy after `rr_ivy_to_scene`). Check the staging with the smoke tests' screenshots.
- Any edit to a FireRed/expansion file gets a `Research Red:` comment so it can be found with grep and carried through upstream merges.

## FireRed content we bypass
Scenes Research Red replaces (Oak's parcel errand, the tutorial rival battle, Oak stopping you at the grass, the early Route 22 rival) are **left in place, not deleted**. Later parts of FireRed still reference those scripts, maps and flags, and deleting them would break the rest of the game for no gain (unreached script costs no RAM and very little ROM). When a later milestone replaces a whole area, its old scripts are removed then.

## Memory budget
- ROM: 32 MB hard limit (the GBA address space). About 76% used.
- EWRAM (main RAM): 256 KB, about 96.5% used. This is the tight one; new systems should allocate their buffers only while they are open (`Alloc`/`Free`) instead of adding permanent `EWRAM_DATA`.
- Save: SaveBlock1 has ~84 bytes free; SaveBlock3 has its padding (`unused[]`), shrink it when adding fields.
These limits are the hardware's and are kept so the patch works on every emulator.

## Workflow
1. `make firered -j6`
2. `python3 tools/research_red/rr_check.py` (text length, caps, registry, stale generated files)
3. `python3 tools/research_red/test/smoke.py` and look at the contact sheets ([TESTING.md](TESTING.md))
4. Commit, then `tools/research_red/rr_install_rom.sh` (installs `ResearchRed_local.gba`, keeps a versioned copy in `builds/`)
5. Push; for a test build also tag it (`git tag -a v0.0.x`, `git push origin v0.0.x`) so the Action publishes a release ([RELEASING.md](RELEASING.md))
6. After the owner's playtest: issues for what they found, `tools/research_red/rr_archive_test.sh <name>`, fill in its `NOTES.md` (include the cost from `session-cost`) ([PLAYTESTING.md](PLAYTESTING.md))
7. Update [STATUS.md](STATUS.md)

**Check before every commit:** `.githooks/pre-commit` runs `rr_check.py` (about 2 seconds) and stops a commit that would fail it. Turn it on once per clone: `git config core.hooksPath .githooks`. On GitHub, the smoke tests run after every push (docs/TESTING.md).

## Generated files
Edit the source, run the tool, commit both. `rr_check.py` fails if a generated file is stale.
- `graphics/research_red/portraits.txt` -> `src/data/research_red/portraits.h` (`build_portraits.py`)
- `data/research_red/encounters/*.txt` -> `src/data/wild_encounters.json` (`build_encounters.py`)
- `data/research_red/dex_banter.txt` -> `src/data/research_red/dex_banter.h` (`build_dex_banter.py`)
- `data/research_red/tm_descriptions.txt` -> the TM51-TM100 descriptions in `src/data/items.h` (`build_tm_descriptions.py`; rerun after changing the TM list in `include/constants/tms_hms.h`, #90)
- trainers, encounters, maps -> `docs/player/*.md`, `docs/BALANCE.md` (`export_player_docs.py`)
- scenes and text -> `docs/story/` incl. `FULL_SCRIPT.md` (`export_story.py --sync`; not checked by rr_check, rerun when text changes)
- `graphics/research_red/overworld/recipes.toml` -> overworld sheets (story cast, Delia, gym leaders, Elite Four, both players) and the marked code blocks in the object event files (`tools/research_red/art/ow_maker.py install`; [ART_TOOLS.md](ART_TOOLS.md))
- intro call graphics (`build_call_ui.py`)

## Other tools
- `tools/research_red/build_cast_art.py`: reference art to battle/portrait/call art and a contact sheet
- `tools/research_red/render_map.py MAP_X|LAYOUT_X out.png [--grid] [--objects]`: a map to an image (tile coordinates, people marked); `--metatiles gTileset_X out.png`: every tile of a tileset with its number, to pick tiles for map edits (e.g. the chairs in Daisy's house)
- `tools/research_red/export_story.py`: story data export (`docs/story/`)
- `tools/research_red/art/`: the Art Kit (sprite library, overworld recipes, battle pictures, map pieces with `tile_kit.py`, new buildings with `building.py`), see [ART_TOOLS.md](ART_TOOLS.md)

Installed on the dev machine (outside the repo; nothing in the build needs them yet):
- Pillow (`python3-pil`) and ImageMagick (`convert`), from Ubuntu's packages: image editing for sprite and portrait work.
- Porytiles 2.0.0 in `~/romhacks/_tools/porytiles/`: whole tilesets from drawn PNG layers ([ART_TOOLS.md](ART_TOOLS.md#whole-new-tilesets)).
- pret pokeemerald, sparse copy in `~/romhacks/_tools/pokeemerald-ref/`: Emerald people and trainer pictures for the sprite library (bases used in recipes are copied into the repo).
- Windows, `E:\Workspace\_tools\`: Porymap 6.3.1 (map editor) and LibreSprite 1.3 (pixel editor).
