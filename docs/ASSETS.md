# Assets: formats, sizes, locations

Owner-made art lives in `graphics/research_red/`. Large reference art (full-body images) stays OUTSIDE the repo at `E:\Workspace\romhacks\research-red\art\`.

| Asset | GBA format | Repo location | Status |
| --- | --- | --- | --- |
| Dialogue portraits (8) | 64x64 PNG, indexed, max 16 colors, index 0 = transparent | `graphics/research_red/portrait_*.png` | Ready |
| Call circles (4) | 32x32, same rules | `graphics/research_red/callcircle_*.png` | Ready |
| Call frame | 240x160 | `graphics/research_red/callframe.png` | Ready |
| Signal bars | 16x32 (4 frames of 16x8) | `graphics/research_red/signalbars.png` | Ready |
| Intro player pics M/F | Must match what `oak_speech.c` loads (check existing red/leaf pics) | `graphics/oak_speech/...` | Needs resize/pad from 64x64 |
| Trainer battle pics (Elm, Jenny, Ivy) | 64x64 front pic, 16 colors | `graphics/trainers/front_pics/` | Placeholder: portraits |
| Overworld sprites (Elm, Ivy, casual Jenny, professors) | Copy the frame layout of an existing NPC sheet in `graphics/object_events/pics/people/` (16x32 frames), 16 colors | same folder + register in object event tables | Needed |
| Title screen / logo | Per title screen code | `graphics/title_screen/` | Needed |

## Converting art to GBA format
- Research Red tools (no extra installs): `tools/research_red/ref_to_trainer_pic.py` (full-body ref art -> 64x64 16-color trainer pic), `png8_to_png4.py` (8-bit indexed -> 4-bit), `render_map.py` (map layout -> PNG).
- Claude Code can convert any PNG with Python + Pillow inside WSL: resize with nearest-neighbor, reduce to <= 16 colors, put the transparent color at palette index 0, save as indexed PNG.
- For hand editing: LibreSprite (free) or Aseprite. Always keep the original full-color file in `art\` and export the GBA version.
- Rule: never draw over another artist's sprite without permission; credit every community sprite in `CREDITS.md`.
