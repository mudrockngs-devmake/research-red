# Art Kit: tools for sprites, battle pictures and map tiles

Research Red's own art tools live in `tools/research_red/art/`. They need
only Python 3 and Pillow (Ubuntu: `sudo apt install python3-pil`). Previews
(GIFs, contact sheets) go to `E:\Workspace\romhacks\research-red\art\toolkit\`
on the dev machine; set `RR_TOOLKIT_OUT` to send them somewhere else.

| I want to... | Tool | Section |
|---|---|---|
| see what sprites already exist | `library.py` | [Sprite library](#sprite-library) |
| make or change a character's overworld sprite | `ow_maker.py` + `graphics/research_red/overworld/recipes.toml` | [Overworld sprites](#overworld-sprites) |
| give a standing-only sprite walking frames | `walkgen.py` (used by `ow_maker.py`) | [How walking frames are made](#how-walking-frames-are-made) |
| make battle pictures from reference art | `tools/research_red/build_cast_art.py` + `pixelize.py` | [Battle pictures](#battle-pictures) |
| draft a back view (battle back picture) | `back_maker.py`, `tools/research_red/build_back_pics.py` | [Back pictures](#back-pictures) |
| hand-finish anything in Aseprite | `aseprite.py` | [Hand-finishing in Aseprite](#hand-finishing-in-aseprite) |
| show pictures live, stage a scene by dragging | `tools/research_red/viewer/viewer.py` | [Live viewer and scene board](#live-viewer-and-scene-board) |
| check / fix / animate a battle picture | `battle_kit.py` | [Battle pictures](#battle-pictures) |
| add furniture or a building front to a tileset | `tile_kit.py` | [Map pieces](#map-pieces) |
| add a building you can walk into | `building.py` | [Buildings and doors](#buildings-and-doors) |
| build a whole new tileset | Porytiles | [Whole new tilesets](#whole-new-tilesets) |
| paint maps by hand | Porymap | [Programs](#programs) |

## Is the art in the game? (no, for now)

**Not yet (owner, 2026-10-24):** the Art Kit's sprites and pictures stay out
of every game version until the art generation and edits are better. The
switch is `graphics/research_red/art_kit.toml` (`in_game = false`). While it's
off:

- `ow_maker.py build` and `install` both make sheets and previews only: every
  sheet (the players' too) lands in `graphics/research_red/overworld/`, and
  the game code isn't touched.
- `build_back_pics.py` writes to `graphics/research_red/drafts/`.
- The game uses v0.0.19's pictures: FireRed's overworld people (the story cast
  on stand-ins: Elm = Scientist, Ivy = Cooltrainer, Jenny = Hex Maniac, Mom
  for Delia), FireRed's gym leader and Elite Four battle pictures, the
  players' `battle.png`, the old back pics and `player.pal`.

### Putting the art in the game
Only with the owner's OK, and ideally a piece at a time:

1. Set `in_game = true` in `art_kit.toml`.
2. `python3 tools/research_red/art/ow_maker.py install`: overworld sprites
   into the game code; `player_warm.pal` is copied over `player.pal`.
3. `python3 tools/research_red/build_back_pics.py`: the player + Ivy back pics.
4. Battle pictures are wired by hand in `src/data/graphics/trainers.h`: the
   full hook-up is in commit `6bf4af269` (`git diff v0.0.19 6bf4af269 --
   src/data/graphics/trainers.h`); point each picture at
   `graphics/research_red/cast/<name>/battle_fr`.
5. Build, `rr_check.py`, smoke tests, then look at it in the game.

## Sprite library

    python3 tools/research_red/art/library.py

Picture sheets of every overworld person (split into "walks" and "stands
still only"), objects, trainer pictures (fronts and backs) and every Pokémon
(front frames, back, overworld), plus `catalogue.md`: which `OBJ_EVENT_GFX_`
name uses which picture and how many frames it really has.

With a sparse copy of pret's pokeemerald in `~/romhacks/_tools/pokeemerald-ref`
(people, trainer front and back pictures), `library.py emerald` adds Emerald's
people as well (about 125 more bodies to build from):

    git clone --depth 1 --filter=blob:none --sparse https://github.com/pret/pokeemerald.git ~/romhacks/_tools/pokeemerald-ref
    cd ~/romhacks/_tools/pokeemerald-ref
    git sparse-checkout set graphics/object_events/pics/people graphics/trainers/front_pics graphics/trainers/back_pics graphics/object_events/palettes

## Overworld sprites

Every custom overworld sprite is a **recipe** in
`graphics/research_red/overworld/recipes.toml`. A recipe starts from an
existing body and changes it:

```toml
[elm]
notes = "Professor Elm: brown hair, glasses, lab coat, teal shirt, olive trousers."
base = "scientist"                 # FireRed's people/scientist.png
gfx = "RR_ELM"                     # a new OBJ_EVENT_GFX_RR_ELM
recolour = [
  { from = ["#393918", "#7b7341", "#cdac62"], to = "#9b6a3c", rows = [0, 23] },  # hair -> brown
  { from = ["#4a4a5a"], to = "#7f8f4f", rows = [26, 32] },                     # trousers -> olive
]
```

- **base**: a FireRed sheet by name (`scientist`), an Emerald one (`em:mom`,
  `em:frontier_brains/lucy`; copied into `graphics/research_red/overworld/bases/`
  on first use), one of ours (`rr:elm`), or any PNG path.
- **recolour**: a group of shades becomes one new colour, keeping the light
  and dark steps. `rows` limits it to part of the body (people stand in rows
  11-30 of their 32-pixel frame: hair about 11-16, face 16-22, body 23-27,
  legs 28-30); `dirs = ["up"]` limits it to the frames facing one way.
- **anchor**: which shade of a group lands exactly on the new colour (use the
  one most of the area is drawn in; otherwise a mostly-dark area can go black).
- **box** / **dirs** on a recolour: only inside a rectangle / only the frames
  facing one way (Jenny's hair hanging beside her body, Sabrina's hair down her back).
- **swap**: exact one-colour swaps. **head_from**: borrow another sheet's head
  (Jessie wears the Beauty's hair on a Rocket body).
- **patches**: draw pixels on a standing frame (Officer Jenny's cap, Delia's
  apron, Agatha's cane, Mrs. Hale's sun hat). On kept walk frames a patch on
  the head follows the head's bob, so hats stay on.
- **parts**: reusable patches from `graphics/research_red/overworld/parts.toml`
  (the open lab coat for the Scientist body: Elm, the male player, Prof. Hale);
  the recipe fills in the colours.
- **grow**: one pixel more of something all round (Ivy's bigger hair).
- **hand**: a hand-finished sheet (`aseprite.py import`): used exactly as drawn.
- **walk**: `keep` the base's walking frames, or `make` new ones (automatic
  when the base only stands). **templates**: which sheets to copy the motion
  from (`auto` picks the three closest body shapes; the picks are saved in
  `templates.lock` so every machine builds the same sprite).
- **run**: also make running frames (the player's 20-frame sheets).
- **replaces**: take over an existing graphic (`MOM`, `BROCK`) instead of a
  new `gfx`. **palette**: recipes with the same name share one palette.
  **fit_palette**: force the colours onto an existing palette (the players use
  `player.pal`, shared with the bike, surf and fishing sprites).

Commands:

    python3 tools/research_red/art/ow_maker.py colours scientist   # the base's colours, where each is used
    python3 tools/research_red/art/ow_maker.py ref 08_elm          # main colours of a reference picture
    python3 tools/research_red/art/ow_maker.py templates brock     # walking sheets that move most like it
    python3 tools/research_red/art/ow_maker.py grid elm 0,1,2      # frames as letter grids, to plan patches
    python3 tools/research_red/art/ow_maker.py build elm           # sheet + previews only
    python3 tools/research_red/art/ow_maker.py install             # all recipes, and the game code (art in the game only)
    make firered -j6

With the art in the game (see [above](#is-the-art-in-the-game-no-for-now)),
`install` writes the sheets (PNG + `.pal`) and regenerates marked blocks in
the engine files (`include/constants/event_objects.h`,
`src/data/object_events/*.h`, `src/event_object_movement.c`); edit the
recipes, never those blocks. `rr_check.py` fails when a sheet is older than
its recipe. Previews: `overworld/<name>_walk.gif`, `_run.gif`, `_frames.png`
(before and after) and `overview.png` (every recipe beside its reference art).

Map scripts keep using the names: `OBJ_EVENT_GFX_RR_ELM`, `_RR_IVY`,
`_RR_JENNY`, `_RR_OFFICER_JENNY`; replaced ones keep theirs (once installed,
`OBJ_EVENT_GFX_MOM` is Delia and `OBJ_EVENT_GFX_BROCK` walks).

Hand-made art always wins: draw over a generated sheet in LibreSprite or
Aseprite (16x32 frames, 16 colours, colour 0 = background), save it as a new
base PNG and point the recipe's `base` at it with `walk = "keep"`.

### How walking frames are made

FireRed's people have 9 frames: down, up, left standing, then two steps for
each direction (right is the left frames mirrored). Gym leaders, the Elite
Four and Mom only have the 3 standing frames. `walkgen.py` makes the steps by
**motion transfer**:

1. Pick templates: walking sheets whose standing bodies have a similar shape.
2. For each pixel of a template's step frame, find where it came from in the
   template's standing frame (a leg moved a pixel forward, the figure bobbed
   down one).
3. Do the same moves on our standing frame. Three templates vote pixel by
   pixel, which cleans up stray pixels; the head moves as one piece so hair
   never breaks up.

`python3 tools/research_red/art/walkgen.py selftest` rebuilds the walks of
sheets that already walk and lays them beside the real ones
(`checks/walkgen_selftest.png`). Results are drafts: good at 1x, worth a look
in LibreSprite for big hair or capes. Large movements (a whole body turning,
as in a Poké Ball throw) don't transfer well.

## Battle pictures

`tools/research_red/build_cast_art.py` turns each full-body reference picture
into `graphics/research_red/cast/<name>/`: **`battle_fr.png`** (the one meant
for the game once the art goes in: full body in FireRed's proportions, a bigger head and sturdier limbs),
`battle.png` (plain full body), `battle_frlg.png` (cropped at the thighs),
`portrait.png`, `call.png`. Files in `cast_handmade/<name>/` replace the
generated ones.

`battle_fr.png` is made by `pixelize.py`, a shrinker for turning big art into
pixel art that doesn't look shrunk: it flattens the colours first, shrinks by
vote (each pixel takes its area's commonest colour, no blur; line art wins
where there's enough of it), removes stray pixels, fills one-pixel holes (legs
stay connected), thickens one-pixel limbs and draws a dark outline. Faces are
1-2 pixels at this size: they're where a hand pass helps most.

`battle_kit.py`:

    python3 tools/research_red/art/battle_kit.py check FILE.png [--kind trainer|mon|back|trainer_back|icon|portrait|call]
    python3 tools/research_red/art/battle_kit.py fix IN.png OUT.png --kind trainer   # any picture -> valid 16-colour GBA picture
    python3 tools/research_red/art/battle_kit.py shiny graphics/pokemon/<name> [--hue 0.5] [--only "#hex,#hex"]
    python3 tools/research_red/art/battle_kit.py anim FRONT.png      # second front frame (a squash) + GIF
    python3 tools/research_red/art/battle_kit.py backpic POSE.png OUT.png   # 5-frame trainer back pic (throw) draft
    python3 tools/research_red/art/battle_kit.py gif SHEET.png --frame 64x64

`backpic` follows how FireRed's Red leans in each frame of the throw; a real
throwing arm still has to be drawn by hand (`--mode transfer` tries the
pixel motion copy, which breaks up on such a big movement).

## Back pictures

The reference art shows the front only. `back_maker.py` drafts the view from
behind: the head and upper body, the face painted over with hair (the back of
a head is hair), mirrored, framed like FireRed's back pictures:

    python3 tools/research_red/art/back_maker.py ivy player_m player_f    # or --all
    python3 tools/research_red/build_back_pics.py                         # the player + Ivy back pic (drafts/ while the art is out)

They're drafts (front clothing details still show): finish them in Aseprite
(`aseprite.py open back:ivy`). A drawing of the back in the reference folder
(`<number>_<name>_back.png`) is used instead of guessing.
`build_back_pics.py` puts the player and Ivy together from their back views,
with the throw frames leaning the way FireRed's Red does.

## Hand-finishing in Aseprite

    python3 tools/research_red/art/aseprite.py open ow:elm          # overworld sheet, 16x32 grid
    python3 tools/research_red/art/aseprite.py open battle:jenny    # battle picture, 64x64 grid
    python3 tools/research_red/art/aseprite.py open back:ivy
    python3 tools/research_red/art/aseprite.py import ow:elm        # back into the game

`open` copies the picture to `E:\Workspace\romhacks\research-red\art\edit\`
and opens it in Aseprite with the grid on. Save (PNG, or .aseprite with
layers). `import` checks it (size, 16 colours, colour 0 = see-through), stores
it where hand-made art wins (overworld: `bases/hand_<name>.png` + `hand = ...`
in the recipe; battle/back/portrait: `cast_handmade/<name>/`) and rebuilds.

## Live viewer and scene board

    python3 tools/research_red/viewer/viewer.py open                 # the page, in the Windows browser
    python3 tools/research_red/viewer/viewer.py show FILE.png --title "Which hair?"
    python3 tools/research_red/viewer/viewer.py board MAP_PALLET_TOWN --title "Where does Ivy stand?"
    python3 tools/research_red/viewer/viewer.py answers
    python3 tools/research_red/test/smoke.py --watch lab             # tests stream into it

A small local page (only this computer can reach it): new pictures from the
art tools and the newest test run stream in (pause, click to pin); `show`
puts a picture in a "Claude asks" panel with a reply box; `board` shows a map
with its people: drag them, click to turn, add someone from the list, then
**Send to Claude** (the positions land in `viewer\answers\`).

## Map pieces

`tile_kit.py` adds a drawn piece (table, plant, shelf, building front) to a
tileset as new map blocks. It only adds: new tiles go at the end, new colours
into unused palette slots, new blocks get the next free numbers, so existing
maps never change.

    python3 tools/research_red/art/tile_kit.py info gTileset_GenericBuilding1
    python3 tools/research_red/art/tile_kit.py add gTileset_GenericBuilding1 plant.png --top plant_top.png --floor 0x001 --label RR_PottedPlant
    python3 tools/research_red/art/tile_kit.py place LAYOUT_PALLET_TOWN_PLAYERS_HOUSE_1F 3 7 0x298 1 2

- Draw at 16-pixel steps. `plant.png` is what's drawn under the player;
  `--top` (same size) is drawn over the player (leaves, a roof edge, a lamp:
  things you walk behind).
- `--floor`: the block whose ground shows through see-through pixels (FireRed
  shows a garbage tile under see-through ground). `info` numbers the blocks.
- `--behavior`: what the blocks do (`MB_COUNTER`, `MB_BOOKSHELF`, `MB_PC`...;
  `tile_kit.py behaviors`). Walking on or not is set per square in the map
  (collision), e.g. `place ... --collision 1`.
- `--label` adds `METATILE_<Tileset>_<Label>_<n>` names to
  `include/constants/metatile_labels.h` for scripts (`setmetatile`).
- Budget: a secondary tileset holds 384 tiles and 384 blocks, palettes 7-12.

## Buildings and doors

    python3 tools/research_red/art/building.py doors MAP_PALLET_TOWN       # door squares, used or free
    python3 tools/research_red/art/building.py new PalletTown_RR_Greenhouse \
            --copy MAP_PALLET_TOWN_RIVALS_HOUSE --outside MAP_PALLET_TOWN --door 12,14
    python3 tools/research_red/art/building.py check MAP_PALLET_TOWN_RR_GREENHOUSE

`new` copies the template's floor plan (`data/layouts/<Name>/`) and settings
(`data/maps/<Name>/`, no people), adds the map to the template's map group and
`data/event_scripts.s`, and links the exit mat and the outside door both ways.
The outside square must be a door: paint one in Porymap, or add a building
front with `tile_kit.py` first. Then `make` and walk in; repaint the inside
in Porymap.

## Whole new tilesets

For a place with its own look (a greenhouse, a fossil park) use
[Porytiles](https://github.com/grunt-lucas/porytiles), installed in
`~/romhacks/_tools/porytiles/`. It builds a tileset from layer PNGs (bottom,
middle, top; 128 pixels wide) and handles palettes and duplicate tiles:

    ~/romhacks/_tools/porytiles/porytiles create-tileset --secondary gTileset_RR_Greenhouse
    # draw the layer PNGs it sets up, then:
    ~/romhacks/_tools/porytiles/porytiles compile-tileset gTileset_RR_Greenhouse

User guide: https://grunt-lucas.github.io/porytiles-user-docs. Porytiles
recompiles a whole tileset, so for small additions to an existing one prefer
`tile_kit.py`, which never moves what's there.

## Programs

| Program | What for | Where |
|---|---|---|
| [Porymap](https://github.com/huderlem/porymap) 6.3 | The standard map editor: paint blocks, collision, place people, warps, signs | Windows: `E:\Workspace\_tools\Porymap\porymap.exe`, open the project folder through `\\wsl.localhost\<Ubuntu>\home\<you>\...\research-red` |
| [Aseprite](https://www.aseprite.org) 1.3 | The pixel editor most Pokémon spriters use; `aseprite.py` opens and imports for you | Windows (Steam): `E:\SteamLibrary\steamapps\common\Aseprite\` |
| [LibreSprite](https://libresprite.github.io) 1.3 (free) | A free alternative | Windows: `E:\Workspace\_tools\LibreSprite\libresprite.exe` |
| Porytiles 2.0 | Whole tilesets from layer PNGs | Ubuntu: `~/romhacks/_tools/porytiles/` |
| gbagfx | PNG to GBA formats (the build runs it) | `tools/gbagfx/` |

Porymap edits the real project files: save, then build, check (`rr_check.py`)
and commit as usual.

## What other projects use (checked 2026-10)

- **Porymap** and **Porytiles**: the community standards for maps and tilesets (above).
- **decomp-editor** (River707, github.com/River707/decomp-editor): an overworld
  editor that adds sprite sheets and palettes for you; an early release.
  `ow_maker.py install` does the same job from recipes.
- **All NPCs walk** (Bivurnum, github.com/Bivurnum/pokeemerald, branch
  `all-npcs-walk`): hand-made walking frames for Emerald's standing NPCs. Our
  gym leaders get generated ones instead; theirs are worth a look for Emerald
  characters we borrow.
- **Aseprite** scripts and **LibreSprite**: the usual pixel editors; save
  16-colour indexed PNGs with the background as colour 0.

## Automatic checks

- `rr_check.py` runs `ow_maker.py --check` (sheets current with their recipes,
  and the 16x32 build rule for `graphics/research_red/overworld/` present).
- The GitHub Action "Art Kit check" (`.github/workflows/art-kit.yml`) runs on
  every push that touches art: recipe check, every cast picture through
  `battle_kit.py check`, then the previews, kept as an `art-kit-previews` zip
  on the run's page for 30 days.
- Overworld sprites still need a look in the game: the smoke tests' contact
  sheets (`docs/TESTING.md`) show them in their scenes.
