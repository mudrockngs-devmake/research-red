# Credits

- **Engine:** pokefirered-expansion (the rh-hideout expansion family), built on pret's pokefirered disassembly.
- **Research Red:** MudrockNGS (story, character designs, animatic), mudrockngs-devmake.
- **Pokémon** and everything from the original games: © Nintendo, Creatures, Game Freak. Research Red is a free, unofficial fan project, not affiliated with or endorsed by them, and not for sale.

## Tools
- Automated test runs use mGBA (endrift, MPL 2.0) as a library. It isn't part of the patch.
- Map and art tools (not part of the patch): Porymap (huderlem and contributors), Porytiles (grunt-lucas), LibreSprite; pret's pokeemerald for reference sprites.

## Quoted material
| What | Where | Source / author | Permission |
|---|---|---|---|
| A few short phrases sung by the cast | Route 1 sing-along on the way to Viridian City (v0.0.16) | "Viridian City", written by Neil Jason and John Loeffler, performed by Jason Paige; *Pokémon 2.B.A. Master* (Koch Records, 1999) | needed (a few words, credited; confirm or trim before a public release) |

## Assets
One row per asset that isn't the game's own art or the owner's own work, plus the owner's art for the record. Art Kit drafts are listed too; which of them are actually in the game is in [docs/ART_TRACKER.md](docs/ART_TRACKER.md) (only approved pieces).
**Permission** says whether the author has OK'd its use in a public release:
`own` (the owner's work), `game` (Game Freak art from the original games, used like every fan hack), `granted` (written OK, link it), `asked`, `needed` (must be confirmed or replaced before a public release).

| Asset | Path | Source / author | Permission |
|---|---|---|---|
| Cast art (battle, portrait, call icon) for 32 characters | `graphics/research_red/cast/` | Converted from full-body reference art of the owner's character designs | own |
| Intro video call frame and avatars | `graphics/research_red/intro_call/` | Drawn by `tools/research_red/build_call_ui.py` from the owner's animatic layout; avatars from the cast art | own |
| Pokémon dialogue portraits | `graphics/research_red/pokemon/` | Cut from the games' front sprites | game |
| Player walking and running sprites (male) | `graphics/research_red/player_ow/` (in the game); Art Kit draft with running frames in `graphics/research_red/overworld/player_m.png` | Professor Birch's overworld sprite (Pokémon Emerald, via pret/pokeemerald), recolored to the character design; the draft's running frames made with the Art Kit from Red's | game |
| Player walking and running sprites (female) | `graphics/research_red/player_ow/` (in the game); Art Kit draft in `graphics/research_red/overworld/player_f.png` | Lorelei's overworld sprite (Pokémon FireRed), recolored to the character design (lab coat, jeans) | game |
| Overworld sprites: Elm, Ivy, Jenny, Officer Jenny, Delia | `graphics/research_red/overworld/` | Recolors of the games' overworld sprites (FireRed's Scientist; Emerald's Anabel, Lucy and Mom, via pret/pokeemerald), Officer Jenny's cap drawn on (Art Kit recipes) | game |
| Overworld sprites: the gym leaders and Elite Four, with walking frames | `graphics/research_red/overworld/` | The games' own sprites, recolored to the character designs; walking frames generated from other game sprites' walks (Art Kit) | game |
| Battle pictures: the cast, gym leaders and Elite Four | `graphics/research_red/cast/*/battle_fr.png` | Converted from the character designs' reference art in FireRed's proportions (Art Kit `pixelize.py`) | own |
| Battle pictures of pairs: Elm and Jenny, Oak and Daisy (#107); you and Ivy side by side | `graphics/research_red/cast/pairs/*/battle.png`, `pairs/player_*_ivy/back.png` | Converted from the character designs' reference art, two figures in one picture (`tools/research_red/build_pair_art.py`) | own |
| Battle back pictures (drafts): the player, Ivy and others | `graphics/research_red/cast/*/back.png`, `graphics/trainers/back_pics/{red,leaf}_back_pic.png` | Back views worked out from the character designs' reference art (Art Kit `back_maker.py`); throw lean measured from FireRed's Red | own |
| Overworld sprites: Oak, Bill, DJ Mary, Prof. Westwood, Melody, Lisa, Mrs. Hale, Prof. Hale, Jessie, James, Prof. Mudrock | `graphics/research_red/overworld/` | Recolours and combinations of the games' overworld sprites (FireRed's Oak, Bill, Lorelei, Blaine, Lass, woman, Scientist, Rocket grunts, Beauty, Cooltrainer; Emerald's May via pret/pokeemerald), hats, beards and coats drawn on (Art Kit recipes) | game |
| Title screen picture (logo, Slowpoke in a lab coat, notebook paper) | `graphics/research_red/title/` | Drawn by `tools/research_red/build_title.py`: Slowpoke's front sprite from the games with a lab coat drawn over it; logo lettering in DejaVu Serif Bold (Bitstream Vera / DejaVu font licence, free to use and modify) | game + own |
| Creator splash and new-game notices | `src/rr_title.c` | Mareep's and Slowking's front sprites from the games | game |
| Banter emote bubbles (! ? note) | `graphics/research_red/banter/emotes.png` | Drawn by `tools/research_red/build_banter_fx.py` | own |
| Banter ink stamp, call-in faces, full-body look | `src/rr_portrait.c` | Stamp drawn in code (the game's font); call faces and full-body pictures from the cast art above | own |
| Pokédex START menu words, "FIELD GUIDE" added | `graphics/pokedex/hgss/tileset_menu_list.png` | The engine's HGSS-style Pokédex art (pokefirered-expansion), with FIELD GUIDE drawn in its font | granted (ships with the engine) |
| Route 1 fly-by streak | `src/rr_flyby.c` | Mewtwo's front sprite, blurred in game | game |
| Animatic placeholder art: Slowpoke, Mimey portrait | `docs/animatic/` only (not in the game) | Fan sprites from the Pokémon Snakewood wiki and FusionDex | needed if they ever go in the game |

Before a public release, every row must be `own`, `game` or `granted` ([docs/RELEASING.md](docs/RELEASING.md)).
