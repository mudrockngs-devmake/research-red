# Playthrough tester

Plays a GBA ROM with no window, following scripted button presses, and checks
that each scene finishes, leaves the game where it should, and never freezes.
How to use it for Research Red: [docs/TESTING.md](../../../docs/TESTING.md).

## Two halves
| Reusable (any pokefirered / pokeemerald-style decomp) | Research Red only |
|---|---|
| `rr_run.c`: the headless emulator runner (C, links libmgba) | `smoke.py`: our scenes, the debug-menu skips, story variable `VAR_0x40FE`, where reports go |
| `setup_mgba.sh`: downloads mGBA + CMake and builds `rr_run` | |
| `tester.py`: building blocks, runs scenes in parallel, judges results, writes the report | |
| `game_memory.py`: names (flags, vars, maps, team size) -> memory addresses | |
| `contact_sheet.py` (+ `../rrpng.py`): screenshot grids | |

## Reusing it for another hack
1. Copy the five reusable files above (and `rrpng.py`) into the new project, keeping
   `rrpng.py` one folder above the others.
2. Run `setup_mgba.sh` once (skip it if `~/romhacks/_tools/rr_run` already exists:
   one runner serves every project).
3. Write a small scenes file like `smoke.py`: scenario functions that return
   script lines, then `tester.main(tester.Suite(...), sys.argv[1:])`. Set
   `progress_var` to the hack's story variable (or leave it out), `first` to the
   scenario that saves the starting state, and `game_define="EMERALD"` for an
   Emerald-based hack.
4. If the hack's code differs from the decomp, check that `game_memory.py`'s
   `field_free()` symbols still exist (`sGlobalScriptContextStatus`,
   `sLockFieldControls`, `sMessageBoxType`, `CB2_Overworld`).

## Building blocks (tester.py)
- `step(dir, tiles)`, `advance(cycles, key, every)`, `start_menu_item(downs)`
- `play_until(label, cond, max_frames, key)`: press a key until `cond` holds; FAIL at the limit
- `not_black(shot)`: FAIL if that screenshot is all black (judged from the picture)
- `ready(label)`: press nothing until the player can walk again (after a door, warp or fade); use it instead of a fixed `wait` after map changes
- `scene_end(step)`: story variable == step and the player can walk
- `expect(map_name=, story=, party=)`, `check(label, cond)`
- conditions from `tester.G` (a `GameMemory`): `var_is`, `flag_set`, `flag_clear`,
  `on_map`, `party_count`, `party_nickname_is`, `field_free`, `textbox_closed`, `in_battle`

Raw runner commands are listed at the top of `rr_run.c`.
