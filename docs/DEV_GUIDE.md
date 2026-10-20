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
| Intro call | `src/rr_intro_call.c` (Discord-style; version stamp at the top of `sCallScript`). FireRed's Oak speech in `src/oak_speech.c` is bypassed |
| Debug skips | `data/scripts/debug.inc` (Start > Debug > Scripts, named 1-9; table in [PLAYTESTING.md](PLAYTESTING.md)) |
| Save version | `include/rr_save_version.h`, `src/rr_save_version.c` ([SAVES.md](SAVES.md)) |
| Tests | `tools/research_red/test/` ([TESTING.md](TESTING.md)) |
| Wild Pokemon | `data/research_red/encounters/<area>.txt` -> `tools/research_red/build_encounters.py`; day/night tables (`OW_TIME_OF_DAY_ENCOUNTERS`) |
| Field Guide / Research Rules | `src/rr_field_guide.c` (Pokédex > START > FIELD GUIDE; the menu entry is in `src/pokedex_plus_hgss.c`, its words in `graphics/pokedex/hgss/tileset_menu_list.png`), `src/rr_rules.c` (Options > Research Rules, from Daisy's house on; the row is in `src/option_menu.c`) |
| Pokedex banter | `data/research_red/dex_banter.txt` -> `tools/research_red/build_dex_banter.py`; `src/rr_dex_banter.c` (queued on new species, played back on the map) |
| Mid-battle lines | `src/data/research_red/trainer_slides.h` (adds slide types RR_TWO_DOWN / RR_FOUR_DOWN) |
| Tools | `tools/research_red/` |

## Conventions
- Labels: `RR_<Scene>_EventScript_*`, `RR_<Scene>_Text_*`, `RR_<Scene>_Movement_*`.
- Text: proper case, max 34 characters per line, story lines use `NPC_TEXT_COLOR_NEUTRAL`.
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

## Generated files
Edit the source, run the tool, commit both. `rr_check.py` fails if a generated file is stale.
- `graphics/research_red/portraits.txt` -> `src/data/research_red/portraits.h` (`build_portraits.py`)
- `data/research_red/encounters/*.txt` -> `src/data/wild_encounters.json` (`build_encounters.py`)
- `data/research_red/dex_banter.txt` -> `src/data/research_red/dex_banter.h` (`build_dex_banter.py`)
- trainers, encounters, maps -> `docs/player/*.md`, `docs/BALANCE.md` (`export_player_docs.py`)
- scenes and text -> `docs/story/` incl. `FULL_SCRIPT.md` (`export_story.py --sync`; not checked by rr_check, rerun when text changes)
- intro call graphics (`build_call_ui.py`), player walking sheets (`build_player_ow.py`)

## Other tools
- `tools/research_red/build_cast_art.py`: reference art to battle/portrait/call art and a contact sheet
- `tools/research_red/render_map.py LAYOUT_ID out.png`: map layout to an image
- `tools/research_red/export_story.py`: story data export (`docs/story/`)

Installed on the dev machine (outside the repo; nothing in the build needs them yet):
- Pillow (`python3-pil`) and ImageMagick (`convert`), from Ubuntu's packages: image editing for sprite and portrait work.
- Porytiles 2.0.0 in `~/romhacks/_tools/porytiles/`: drawn PNG layers to map tiles and metatiles (new buildings, the picnic blanket).
- Porymap (optional, Windows): visual map editor for the owner.
