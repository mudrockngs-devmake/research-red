# Changelog

Test builds are v0.0.x. Each entry lists what changed for players/testers.

## v0.0.13
- New intro: a Discord-style video call like the animatic (live dot, signal bars, people joining as round avatars, gold ring on whoever is talking; Slowpoke never hangs up). The controls tutorial is gone.
- Dialogue portraits stay up for the whole scene, bob gently, and have a name plate. Pokémon can talk too: Mimey, Muffin and the lab Slowpoke have portraits from their sprites.
- The player walks as Professor Birch (male) or the Reporter (female), with red hair; biking, surfing and fishing use FireRed's own sprites again. This fixes the glitchy player sprite.
- Route 1: something purple streaks across the sky (a blurred Mewtwo).
- Wild Pokémon: new day and night tables for Pallet Town's water, Route 1, Viridian City, Route 22, Route 2 and Viridian Forest. Route 1 has every non-legendary from the anime's first episode.
- Start > Guide: which wild Pokémon live where you are, how often they appear, and which you've caught.
- Route 22: Elm and Jenny's 6v6 double (their lab starters plus dessert-named catches), with mid-battle banter every two faints and on their last Pokémon. Win or lose, the story goes on.
- Lunch in Viridian City on the way back from Route 22: Jenny runs into her cousin Officer Jenny, a picnic, and Elm's Research Rules (level caps, soft/hard, doubles, wild doubles, battle style; change later in Start > Rules). The old man wakes up and the road north opens.
- Viridian Forest Bug Catchers, Camper Liam and Brock (now six Pokémon) have full teams and opening lines.
- Pokédex banter: register a new species (catch or evolution) and you and Ivy trade a joke or fact about it, with regional-form notes and lines for Elm's and Jenny's starter families.
- Level caps (on by default, soft): Brock's cap is Lv 14.
- Debug: Script 6 skips to the Viridian lunch, Script 7 previews the Pokédex banter; skips give a Lv 12 team.

## v0.0.12
- Fixed: the battle intro called Jenny "Officer Elm".
- Fixed: Oak describes the starters as Kanto on top, Johto below.
- Battle sprites are plain-shrunk again (no big heads), with eyes kept visible.
- Temporary player sprites: red hair and a white lab coat (male), red hair, coat and jeans (female).
- Muffin the Rattata lives in Daisy's house; Daisy faces the door when you arrive for the scene.
- A Slowpoke wanders Oak's lab.
- Every trainer battle is now a double battle (two against one if they only have one Pokémon).
- Route 1 has the new Kanto-only encounter table (Pidgey, Rattata, Spearow, Mankey, both Nidoran, Oddish, Bellsprout, Meowth, rare Eevee).
- Character portraits appear above the text box whenever Elm, Ivy, Jenny, Oak, Delia, Daisy or you speak.
- Lab: if Elm and Jenny beat you, they take on Prof. Oak while you watch. Declining Oak or losing to him now has its own dialogue.
- Route 1: Elm and Jenny wait at the north end instead of popping in, and react to the first four Pokémon you caught on the way (with Elm getting more jealous each time).
- Behind the scenes: scenes moved into their own files; story data export for writing.

## v0.0.11
- Ivy now follows you from the end of breakfast. Talk to her for a few placeholder lines.
- In cutscenes she steps in from where she is and falls back in behind you afterward (lab, Daisy's house, Route 1).
- Debug skips 2-4 start with Ivy following.
- Battle sprites redrawn with slightly larger heads and preserved dark details so eyes are visible.

## v0.0.10
- Research grant now includes Master Balls, every Poké Ball type x100, all TMs and HMs, held items, battle items, more medicine, berries, and key items (Bicycle, rods, Itemfinder, VS Seeker, Exp. Share, Berry Pouch).
- TMs are reusable. HMs can be forgotten. Field moves need no badge.
- Running Shoes from the start. Exp. Share is a key item that turns on for the whole party.
- Battles show types and move effectiveness at all times.
- Move relearner from the party menu and summary screen (includes pre-evolution and TM moves).
- Evolution items like Razor Claw can be used from the Bag.
- HGSS-style Pokédex.
- Map name popups no longer repeat inside the same area.
- Choose from all six starters (Kanto and Johto) in a list that shows each Pokémon's picture. The Johto starters sit on a second table row under the Kanto ones.
- The starter chain works for every pick: Ivy counters you from your region, Elm counters you from the other region, Jenny counters Elm.
- Optional double battle against Prof. Oak and the two leftover starters. Losing plays his haiku and Elm offers his own funding.
- Fixed: the Route 1 camera no longer stays offset after the "something pink" moment.
- Elm and Jenny's battle pictures are now full-body sprites made from the reference art (Jenny in casual clothes).
- Ivy and Jenny use closer stand-in overworld sprites (purple-haired Cooltrainer for Ivy, long-haired Sabrina for Jenny).
- Save layout changed (bigger Items pocket): start a new game.

## v0.0.9
- The National Pokedex is unlocked from the start of a new game.

## v0.0.8
- Lab battle is now a double battle: you and Ivy vs Elm and Jenny with their Johto starters. Fixes the lab freeze.
- Elm and Jenny have their own battle pictures (from their portraits) and Jenny is an Officer.
- Losing the lab battle continues the story instead of whiting out.

## v0.0.7
- Route 22 gray text; Route 1 scene moved near Viridian; Ivy joins the Daisy scene.
