# Artist Guide (Research Red)

Thanks for helping! There are two ways to contribute art. Either is fine, per character.

## Option 1: redesign the reference art (easiest)
- One full-body drawing per character/outfit: **1024x1536 PNG, transparent background**, character standing, whole body in frame.
- Save it with the existing file name (e.g. `08_elm.png`) in `E:\Workspace\romhacks\research-red\art\ref\ResearchRed_ref_art\`, or send it to the owner.
- The battle sprite, dialogue portrait and video-call icon are then generated automatically (`tools/research_red/build_cast_art.py`).
- New character or outfit: new numbered file; it gets added to the `CAST` list in that script.

## Option 2: hand-pixel the final sprites (best quality)
All game art: **indexed PNG, max 16 colors, color #0 = the transparent background** (use pure green `#00FF00`).

| Asset | Size | Notes |
|---|---|---|
| Battle sprite | 64x64 | Full body facing slightly left, feet near the bottom edge (like FireRed trainer art) |
| Dialogue portrait | 64x64 | Head and chest, cropped at the bottom |
| Video-call icon | 32x32 | Face close-up |
| Overworld sprite | 16x32 per frame | Copy the frame order of an existing sheet in `graphics/object_events/pics/people/` (down, up, left x3 walk frames each). Right-facing is the mirrored left |

Put finished files in `graphics/research_red/cast_handmade/<name>/battle.png` (or `portrait.png`, `call.png`).
They always win over the generated versions. Overworld sheets go to the owner and are hooked up in code.

Character names: see `CAST` in `tools/research_red/build_cast_art.py` (e.g. `elm`, `ivy`, `ivy_swimsuit`, `jenny`, `jenny_police`).

Tools: LibreSprite (free) or Aseprite. Keep your full-color originals; export the 16-color version.

## Sending art
Shared folder synced to `E:\Workspace\romhacks\research-red\art\incoming\`, or a GitHub pull request to mudrockngs-devmake/research-red.

## Credit
Every contribution gets a line in `CREDITS.md` with your name or handle.

## Pokémon portraits

Pokémon that talk in dialogue (Mimey, Muffin, Slowpoke so far) get a portrait cut from their front sprite with a white outline. To replace one, put a 64x64, 16-color PNG at `graphics/research_red/pokemon_handmade/<species>.png` (for example `mr_mime.png`) and run `python3 tools/research_red/build_portraits.py`. To make a new character or Pokémon talk, add a line to `graphics/research_red/portraits.txt`.
