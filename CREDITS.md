# Credits

- **Engine:** pokefirered-expansion (the rh-hideout expansion family), built on pret's pokefirered disassembly.
- **Research Red:** MudrockNGS (story, character designs, animatic), mudrockngs-devmake.
- **Pokémon** and everything from the original games: © Nintendo, Creatures, Game Freak. Research Red is a free, unofficial fan project, not affiliated with or endorsed by them, and not for sale.

## Tools
- Automated test runs use mGBA (endrift, MPL 2.0) as a library. It isn't part of the patch.

## Quoted material
| What | Where | Source / author | Permission |
|---|---|---|---|
| A few short phrases sung by the cast | Route 1 sing-along on the way to Viridian City (v0.0.16) | "Viridian City", written by Neil Jason and John Loeffler, performed by Jason Paige; *Pokémon 2.B.A. Master* (Koch Records, 1999) | needed (a few words, credited; confirm or trim before a public release) |

## Assets
One row per asset that isn't the game's own art or the owner's own work, plus the owner's art for the record.
**Permission** says whether the author has OK'd its use in a public release:
`own` (the owner's work), `game` (Game Freak art from the original games, used like every fan hack), `granted` (written OK, link it), `asked`, `needed` (must be confirmed or replaced before a public release).

| Asset | Path | Source / author | Permission |
|---|---|---|---|
| Cast art (battle, portrait, call icon) for 32 characters | `graphics/research_red/cast/` | Converted from full-body reference art of the owner's character designs | own |
| Intro video call frame and avatars | `graphics/research_red/intro_call/` | Drawn by `tools/research_red/build_call_ui.py` from the owner's animatic layout; avatars from the cast art | own |
| Pokémon dialogue portraits | `graphics/research_red/pokemon/` | Cut from the games' front sprites | game |
| Player walking sprite (male) | `graphics/research_red/player_ow/src/prof_birch.png` | Professor Birch overworld sprite (Pokémon Emerald, via pret/pokeemerald), recolored | game |
| Player walking sprite (female) | `graphics/research_red/player_ow/` | FireRed's Reporter overworld sprite, recolored | game |
| Pokédex START menu words, "FIELD GUIDE" added | `graphics/pokedex/hgss/tileset_menu_list.png` | The engine's HGSS-style Pokédex art (pokefirered-expansion), with FIELD GUIDE drawn in its font | granted (ships with the engine) |
| Route 1 fly-by streak | `src/rr_flyby.c` | Mewtwo's front sprite, blurred in game | game |
| Animatic placeholder art: Slowpoke, Mimey portrait | `docs/animatic/` only (not in the game) | Fan sprites from the Pokémon Snakewood wiki and FusionDex | needed if they ever go in the game |

Before a public release, every row must be `own`, `game` or `granted` ([docs/RELEASING.md](docs/RELEASING.md)).
