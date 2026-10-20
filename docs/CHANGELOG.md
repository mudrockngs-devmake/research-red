# Changelog

Test builds are v0.0.x. Each entry lists what changed for players and testers, and ends with a **Save:** line (see SAVES.md).

## v0.0.16 (in progress)
- Changed: Elm and Jenny's Pewter rematch happens just outside the Pokémon Center (Nurse Joy doesn't allow battles indoors).
- Fixed: level caps follow how many badges you have, so gyms beaten out of order (Surge, Erika, Koga, Sabrina) don't leave the cap too low.
- Wild Pokémon: more Johto neighbors in every demo area (Sentret, Hoppip, Ledyba, Pineco, Phanpy, Marill, Chinchou and more), Kanto still the majority. Evolved Pokémon only appear at or near the level they evolve (Butterfree and Beedrill at 10, Metapod and Kakuna at 7; no more low-level Dodrio, Gengar, Noctowl and so on). The Pallet sea gets Horsea, Shellder and Krabby (#59).
- One Fishing Rod instead of three (the Field Guide shows "Fishing"). Surfing finds the everyday water Pokémon, fishing the rarer ones, at levels that match the area. Pallet's sea: Tentacool, Magikarp, Horsea, Chinchou, Qwilfish (surfing); Krabby, Staryu, Shellder, Remoraid, Dratini (fishing).
- Demo boundaries: Diglett's Cave (Route 2) is closed by a Dugtrio on the ladder and four Diglett (placeholder lines), and Route 22's League gate turns you back even with the Boulder Badge.
- Level caps simplified to what most romhacks do: on or off (no more soft/hard). At the cap your team gets no EXP; Rare Candies stop at the cap too. Pokémon below the cap get bonus EXP to catch up. Start > Rules shows the current cap ("Lv. 21").
- Dialogue pass: the intro video call is rewritten in the new character voices (Oak starts his famous welcome speech out of habit, he greets Jenny as Elm's Jenny, and the plan is spelled out: one joint paper, Oak leading, all four of you as authors) and breakfast at Delia's (Mimey won't share the credit for the pancakes, a field note, a thank-you to Delia) and the tall-grass moment (Oak meets Jenny in person, Jenny starts rating your jokes) and Oak's lab (Elm's starters came by PC transfer with a card, now on Oak's fridge; everything goes in the joint paper; Oak got carried away at the Poké Mart; Elm is a good sport after losing to Oak) and Daisy's house (new bet stakes: the losers cook a big dinner at Delia's, dessert included, and Delia gets the night off; Daisy keeps score) and Route 1 (a sing-along on the road to Viridian City: Elm and you start, Ivy and Jenny share a look, then join in) and its catch reactions (one for every Pokémon in the new Route 1 tables, day and night; Ivy adds regional-form facts) and Route 22 (Elm says a sincere "good game" after losing; Jenny's mid-battle line is about dinner now) and the Viridian lunch (Elm always knows his Jenny, even next to her cousin) and Pewter (Elm and Jenny saw your whole Brock battle; the old "Mimey's dishes" bet is gone everywhere). Talking to Ivy while she follows you: her lines now depend on where you are (Pallet, Route 1, Viridian, Route 22, Route 2, Viridian Forest, Pewter, plus a few for anywhere else). Pokédex banter reviewed: kinder teasing, more field notes and real science, no spoilers for the starters' other forms, and new entries for the 14 Johto Pokémon in the demo's wild tables. A comedy pass on top: science puns, cute couple moments, a few anime and other-media nods, and a Seel joke for the grown-ups. Every Gen 1 and Gen 2 Pokémon now has its own banter (251).
- Changed: Research Rules moved from the Start menu into Options (#45). The Options row "Research Rules" (where Battle style was; Battle style is in the Rules) opens Elm's Rules on the map, now titled as the bet's rules. It works from Daisy's house on, and not from the title screen's Options.
- Changed: the Field Guide moved into the Pokédex: press START in the Pokédex, then FIELD GUIDE (it replaces LIST BOTTOM; holding Down still reaches the end of the list). The Start menu is shorter: no more Guide or Rules.
- New: the DexNav. After the Route 22 battle (win or lose), Jenny gives you her old Pokémon-spotting logbook, turned into an app. Start > DexNav shows the area's Pokémon; pick one you've seen and sneak up on it (search levels, hidden abilities, egg moves, and hidden Pokémon in the grass).
- New: rename your Pokémon anytime (#71): in its summary, press A on the info page. The name box starts with the current name; OK keeps it, and erasing it all and pressing OK gives back the species name, so it takes its new species name when it evolves.
- New: the opponent's types stay next to its health bar for the whole battle (#25), not only while you pick a move. They step aside for a moment during attack animations. Your own side still shows its types while you choose a move.
- New: in the move list, move names are coloured by how well they work on the foe: green for super effective, grey for not very effective, red-brown for no effect (the same answer as the circle; status moves and normal hits stay dark).
- Changed: held items come back after every battle, Berries included (#58). They still work normally during the battle (a Sitrus Berry is eaten once), then the Pokémon holds it again afterwards. Stolen or swapped items follow the usual rules.
- New: evolve when you want (#50). Pokémon no longer evolve on their own after a battle or a Rare Candy. When one is ready, its party menu shows EVOLVE (B still cancels). Evolution stones work as before.

## v0.0.15
- Fixed: the Pokédex crashed (blue error screen) when moving from the Stats page to the Evo page.
- New: after Brock, Elm and Jenny wait in the Pewter Pokémon Center for a 6v6 rematch, then head off to challenge Brock themselves.
- New: the demo ends at Pewter. After Brock, the boy at the east exit turns you back with a note, and Gerald the Tentacool blocks the sea route south of Pallet. The Running Shoes delivery in Pewter is gone (you can run from the start).
- Daisy's house: Muffin really hops onto the chair and Elm onto the table. Elm and Jenny leave through the door, and Daisy has her own lines afterwards.
- Elm's Research Rules talk now happens at Daisy's house, after things calm down. Start > Rules is there from then on.
- Viridian lunch: Elm and Jenny sit right of the Pokémon Center; talk to them to start it.
- Route 1: Elm and Jenny wait where the path meets the top of the route, you line up in front of them, and Mewtwo streaks from the right to the upper left.
- Lab: losing to Elm and Jenny has its own lines, and you can choose whether to watch their battle with Oak. The Johto starters' table is one table now, and the Slowpoke takes it easy.
- Elm and Jenny's starters get pastry nicknames (from Route 22 on).
- Portraits sit lower, on the text box, and step aside when nobody's talking (people walking off, camera pans). Oak's starter list no longer hides behind his portrait.
- Start > PC: the PC from anywhere once you have a Pokémon.
- Start > Guide: separate day and night pages, and species you've caught are shown in green.
- New Research Rule: heal after battles (off by default).
- New games start with fast text, and L/R work as buttons instead of opening Help.
- Blacking out at home: Delia has her own lines.
- Debug skip 10: after Brock, outside the Pewter Pokémon Center.
- **Save:** old saves work. If your save is between Daisy's house and the Viridian lunch, you'll miss Elm's rules talk, but Start > Rules works.

## v0.0.14
- Fixed: a portrait could flash for a moment in the previous speaker's colors when the speaker changed (most visible in Oak's lab), and could show up in the colors of a nearby person.
- Portraits now leave the screen as soon as a battle starts.
- Continuing a save from an older test build shows a one-time note suggesting a new game if anything looks wrong.
- Debug skips have names, plus two new ones: 8 (Route 1 scene with Elm and Jenny) and 9 (test mode: the AI fights your battles, team to Lv 30).
- Behind the scenes: automated playthrough tests, player guides (wild Pokémon, trainers, items), a balance overview, GitHub issues and releases, a writing style guide and a full readable script.
- **Save:** old saves work (they show the one-time note).

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
- **Save:** old saves work.

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
