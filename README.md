# Pokémon Research Red

A cozy, slapstick FireRed hack about grown-ups who never stopped loving Pokémon.

You play a professor visiting Pallet Town with your wife Ivy for a research stay:
pancakes with Delia and Mimey, a friendly rivalry with Professor Elm (and his wife,
Officer Jenny), regional forms, and a road trip from Pallet to Pewter.
Inspired by the early anime: warm, silly, and kid-friendly.

> **Status: early test builds.** Pallet Town to Pewter City is playable, up to and
> including Brock. Saves may not carry over between test builds before v0.1.

## How to play
1. Get the **Pokémon FireRed (USA) v1.1 (Rev 1)** ROM from your own cartridge.
   SHA1 `dd5945db9b930750cb39d00c84da8571feebf417`.
2. Download the `.bps` patch from [Releases](../../releases).
3. Apply it with [Rom Patcher JS](https://www.marcrobledo.com/RomPatcher.js/) or Floating IPS.
4. Play in [mGBA](https://mgba.io/) (recommended).

This repository only ever contains patches and source code: **no ROMs**, and please
don't ask for or share them in Issues.

## Features (so far)
**Story and cast**
- Story scenes with character portraits, a video-call intro and a following partner
  who chats about wherever you are
- Comments from the cast on every Gen 1 and Gen 2 Pokémon you register
- Rival trainers with mid-battle banter, and a friendly bet that runs all game

**Research tools**
- **Research Rules** (Options): level caps on/off, double or single battles,
  battle style, heal after battles. Changing one calls Elm and Jenny.
- **Ranch Pager**: call Prof. Oak's Pokémon to Fly, Flash, Teleport or Dig. No
  HM slaves: Oak's Pokémon also Cut, Surf and push boulders when nobody on your
  team knows the move.
- **PC from anywhere** (Start menu), **DexNav** (find and sneak up on Pokémon),
  **Field Guide** in the Pokédex (what lives here, by day and night)
- **100 TMs**, all from the research grant; TM51-65 are FireRed's old move tutors

**Pokémon**
- Evolve when you want (an EVOLVE option in the party menu); trade evolutions
  without trading; relearn earlier forms' moves at any level; rename anytime
- Anime-inspired wild Pokémon with Johto neighbours, different by day and night
  (surfing and fishing too)
- Held items and Berries come back after every battle

**World**
- Rest at Pokémon Centers or at home to skip to night or morning
- Rematches that grow with your badges: Prof. Oak and Daisy, Delia, and Brock at a
  difficulty you choose, with a stronger post-game tier

Full history: [docs/CHANGELOG.md](docs/CHANGELOG.md). Player guides (wild Pokémon,
trainers, items): [docs/player/](docs/player/).

## Feedback
Bug reports and ideas are welcome on the [Issues](../../issues) tab (there are forms
for each). This is a personal hobby project, so replies may be slow.

## Credits
- Built on [pokefirered-expansion](https://github.com/rh-hideout/pokefirered-expansion)
  (rh-hideout) and [pret's pokefirered](https://github.com/pret/pokefirered) disassembly.
  Their original README: [docs/ENGINE_README.md](docs/ENGINE_README.md).
- Story and character designs: MudrockNGS.
- Everything else, asset by asset: [CREDITS.md](CREDITS.md).

## Disclaimer
Research Red is a free, non-commercial fan project. It is not affiliated with, endorsed
by, or sponsored by Nintendo, Creatures Inc., Game Freak or The Pokémon Company.
Pokémon and all related names are trademarks of their respective owners. No money is
made from it, and there are no donations, paid builds or paid perks.
