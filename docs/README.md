# Research Red documentation

Start here. Everything about how the project works, in one place.

## Where we are
- **[STATUS.md](STATUS.md)**: the current build, what's waiting on whom, what's next. Read this first in every session; update it at the end of every session.
- [ROADMAP.md](ROADMAP.md): every open issue, easiest and safest first, and what goes in the next playtest.
- [CHECKLIST.md](CHECKLIST.md): the long plan, milestone by milestone.
- [CHANGELOG.md](CHANGELOG.md): what changed in each test build (becomes the release notes).
- Bugs and ideas: [GitHub Issues](https://github.com/mudrockngs-devmake/research-red/issues), grouped by milestone.

## Playing and testing
- [PLAYTESTING.md](PLAYTESTING.md): how a playtest round works, the checklist to run every round, how to report problems.
- [TESTING.md](TESTING.md): the automated smoke tests (run on every build before it reaches the owner).
- [COMPATIBILITY.md](COMPATIBILITY.md): which emulators and devices have been tested.
- [SAVES.md](SAVES.md): when old saves keep working, and the save version.
- [player/](player/README.md): player guides generated from the game data (wild Pokémon, trainers, items).
- [BALANCE.md](BALANCE.md): levels per area and level caps, generated.

## Making the game
- [DEV_GUIDE.md](DEV_GUIDE.md): where everything lives, conventions, generated files, memory budget.
- [STYLE_GUIDE.md](STYLE_GUIDE.md): how the characters talk, text rules.
- [addons/](addons/): Research Red features written up so other hacks can reuse them (evolve on demand).
- [STORY_BIBLE.md](STORY_BIBLE.md): the story. [animatic/](animatic/): the scene spec. `scenes/`: scene specs that override it.
- [reference/ANIME_AND_GEN2.md](reference/ANIME_AND_GEN2.md): what each area had in the anime and in Gold/Silver/Crystal, for wild tables and homages (copy on E: in `reference/`).
- [reference/KANTO_ANIME.md](reference/KANTO_ANIME.md): Kanto in the anime, place by place (characters, set pieces, ideas); source sheet `kanto_anime.csv` opens in Excel (`tools/research_red/build_reference.py`).
- [reference/VOICES_EARLY_ANIME.md](reference/VOICES_EARLY_ANIME.md): how each cast member talked in the early anime, and our sitcom take, with sample lines.
- [story/](story/README.md): every scene, map and line exported for reading (FULL_SCRIPT.md reads it all in order).
- [FLAGS_VARS.md](FLAGS_VARS.md): every flag, var and trainer Research Red uses.
- [ARTIST_GUIDE.md](ARTIST_GUIDE.md): how to make or replace art. [ASSETS.md](ASSETS.md): the asset list.
- [ART_TOOLS.md](ART_TOOLS.md): the Art Kit: sprite library, overworld sprite recipes and walking frames, battle pictures, map pieces, new buildings, Porymap/Porytiles.
- [ART_BIBLE.md](ART_BIBLE.md): how Research Red looks (the rules every sprite, picture and tile follows). [ART_TRACKER.md](ART_TRACKER.md): every piece that will get new art and its stage; only approved pieces are in the game (generated).
- [RESOURCES.md](RESOURCES.md): sources and credit rules. [../CREDITS.md](../CREDITS.md): who made what, and permissions.
- [CHAT_TO_CODE.md](CHAT_TO_CODE.md): how ideas from chat become changes.

## Shipping
- [RELEASING.md](RELEASING.md): versions, tags, GitHub Releases, the release checklist.
- [UPSTREAM.md](UPSTREAM.md): which engine version we're on and how to update it.
- [ENGINE_README.md](ENGINE_README.md): the engine's original README (pokefirered-expansion).
