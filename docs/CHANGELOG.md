# Changelog

Test builds are v0.0.x. Each entry lists what changed for players and testers, and ends with a **Save:** line (see SAVES.md).

## v0.0.19
The big one after the v0.0.18 playtest (issues #76-#100).
- New: a title screen of our own (#12): notebook paper, the Research Red logo and Slowpoke in a lab coat (worn low so its face and ear show, a sleeve tucked around the front). At power-on a little Mareep walks in, sparks, and "A fan project by MudrockNGS" fades in (A skips). On a new game, a Slowking presents the fan-project notice, a short spoiler-free welcome to the story, and the demo notice before the intro call.
- New: watch Elm and Jenny take on Brock (#69). After the Pewter rematch Elm asks if you want to come; Yes takes you straight into the Gym, No leaves it for when you walk in. Three rounds, you and Ivy commentating.
- New: Viridian Forest: you and Ivy talk after each Bug Catcher, and a Samurai waits in the clearing (#9).
- New: ambient Pokémon (#64): Pallet's Dodrio, Oak's Diglett, a Chansey beside Nurse Joy in Viridian and Pewter, and the Forest's guardian Pidgeot in a standoff with a Fearow. Talk to them: a cry, a line, and they're marked as seen in the Pokédex.
- New: Pokédex banter is a little cutscene over the new entry after a catch (#5); press SELECT on an entry to replay it (#4). Ivy and you slide in, the speaker hops, the Pokémon cries and hops, its number is stamped on like an ink stamp, a pencil scribble per line, a ! ? or ♪ bubble on some lines, and Elm or Jenny call in for their family's lines. A for the next line, B skips. Portraits by default; Debug > Scripts > 13 switches to full-body pictures (here and in battle) to compare.
- New: in battle, your and Ivy's lines show your portraits (#95). Your side of the battle shows you and Ivy together (#7); the Trainer Card uses your own picture (#24).
- New: your own sprite on the bike, surfing and using field moves (#80, #28); Ivy steps aside while you ride. The female player has her own walking sprite (#93).
- Changed: scenes re-staged (#83-#85): everyone sits at breakfast and at Daisy's tea, Jenny walks beside Elm everywhere, Route 1 is reworked (#85, #88), a picnic blanket for the Viridian lunch (#8). Jenny's Growlithe, Cruller, joins the cast.
- Changed: the lab after the Elm and Jenny battle (#86), win or lose: they challenge Oak's two leftover starters (Oak heals them first) and win; Elm pushes his luck, and Oak's Lv 50 Pidgeot and Tauros flatten them (watching is optional). Then you and Ivy may battle the starters too (optional, healed first). Oak keeps them, and the lab's Slowpoke naps on the empty starter table from then on (the lab's Pokémon and Daisy's Muffin notice).
- Changed: proper case for all of FireRed's text (#11): "Pokémon", "Save", "Pallet Town" instead of ALL CAPS. Battle text boxes in the notebook colours (#92).
- Changed: the Pokédex lists Kanto's 151 in order, then Johto (#77); the Area page matches the time of day (#20). The Field Guide is back in the Start menu, below the Pokédex (#91).
- Changed: Oak's research grant is reusable things only: every TM and HM and the key items (#96). Supplies come from **Lab Supplies**, a free shop on every PC (Start > PC too): every Poké Ball except the Master Ball, medicine and status heals, Repels, X items, held battle items and berries, Rare Candy (it stops at the level cap), PP Up/Max and vitamins (#98-#100). Pockets are FireRed's sizes again. The lab aide can move a Pokémon into a different Poké Ball (#97).
- Changed: the move relearner (#89): you stay in the list after teaching a move, the TM list shows TM numbers, and the menu isn't red any more. 19 TMs that repeated a free tutor move now teach new moves (#90): Low Kick, Knock Off, Foul Play, Nasty Plot, Agility, Roost, Heat Wave, Mud Shot, Drill Run, Power Gem, Smart Strike, Iron Defense, Body Press, Charge Beam, Grass Knot (TM51-65), Fire Fang, Thunder Fang, Ice Fang and Pounce. Mega Punch, Body Slam, Swords Dance and the rest are still in LEARN MOVES > Tutor Moves.
- Changed: Emerald-style map name pop-ups; item descriptions only the first time you find an item.
- Changed: dialogue polish: Oak's Pokémon, his Ranch Pager tutorial, the Rules call, Delia's Rest offer, Oak's rematch haiku.
- Fixed: portraits vanishing on a message's last page (#76); the type display (#78); the Start menu PC fading to black (#79); the intro call keeping the name after you hang up (#81); SELECT opening the battle debug menu (#82); Sumac is now Sequoia (#94); starters get sensible moves (#87).
- Test builds: START skips the new-game call (as Maple). Behind the scenes: unused link, e-Reader and Mystery Gift save data freed (about 3.3 KB of save room for later), new smoke tests (nickname, Lab Supplies, the lab's optional parts, the banter cutscene in both looks).
- Not yet: landscape art behind battles (#74) waits for the background art.
- **Save:** start a new game (the save layout changed: item pockets, the first-time item flags and the freed link data).

## v0.0.18
- New: battle backgrounds that match where you are (#74), in cozier colours: warmer, sunnier grass on routes; a shaded forest in Viridian Forest (every battle there); a sandy platform under a soft sky in towns and on paths; a warm wooden floor in Oak's Lab and homes; Pewter stone for Gym leaders; a richer sea and richer cave earth. Same art, new colours only, so nothing changes size.
- Fixed (test builds): with the debug test mode on (Debug > Scripts > 9), a wild battle froze ("TRAINER None sent out..."). Test mode now only plays trainer battles for you; wild battles are yours.
- Testing tools: a route planner across maps and doors, and the test runner can set game flags (no wild encounters while it walks somewhere). New automatic test: a battle in Viridian Forest.
- **Save:** old saves work.

## v0.0.17
- New: the Field notebook look (#73). Menus, the dialogue box, the Start menu and Options are cream paper with brown ink and tan frames; the battle text box is leather brown with brass trim; the FIGHT/BAG menu and the move list are cream with brown frames; the HP boxes have warm brown borders. Layouts and the HP bar colours are unchanged.
- New: picking up an item shows its picture and a short description.
- Changed: battles are a little snappier (shorter pauses between battle messages), and in wild battles B jumps the cursor straight to RUN.
- Already in from earlier builds, for testers who missed them: move info in the FIGHT menu (power, accuracy, category), the ability pop-up, R to throw your last-used ball, critical captures, fast HP and EXP bars.
- **Save:** old saves work.

## v0.0.16
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
- Changed: no trading needed to evolve (#65). Kadabra, Machoke, Graveler and Haunter are ready to evolve from level 37 (no Linking Cord). Pokémon that evolve by trading while holding an item (Onix and Scyther with a Metal Coat, Slowpoke and Poliwhirl with a King's Rock, Seadra with a Dragon Scale, Porygon with an Up-Grade...) evolve when you use that item on them from the Bag, like a stone. The Pokédex's EVO page says so. Those items (King's Rock, Metal Coat, Dragon Scale, Up-Grade and the like) stay in your Bag afterwards, so one is enough; evolution stones are still used up.
- Ready for later generations: the same rules apply to every trade evolution (Dusknoir, Gurdurr's family, Clamperl, and so on). Karrablast and Shelmet evolve from level 37 with the other one in your party.
- Changed: relearn moves (party menu > LEARN MOVES): every level-up move of a Pokémon's earlier forms is now available, whatever its level (a Wartortle can relearn Squirtle's later moves). Its own moves still wait for their level. Each move is listed once (#51).
- New: 100 TMs (#57). TM51-TM65 are FireRed's old move tutors (Mega Punch, Body Slam, Counter, Soft-Boiled, Substitute...), TM66-TM100 are newer moves (Scald, Volt Switch, U-turn, Drain Punch, Stealth Rock, Dazzling Gleam, Play Rough...). Oak's research grant gives all of them; an older save that already got the grant receives TM51-TM100 once, with a note from Oak. The TM Case shows three-digit numbers, and every new TM has its own description.
- New: the Ranch Pager (#29), a key item from Oak's research grant. Use it from the Bag (or register it and press SELECT) to call one of Oak's Pokémon: Pidgeot to Fly, Pikachu for Flash, Abra to Teleport, Diglett to Dig. No flyer needed in your team. Cut, Surf, Strength, Rock Smash and Waterfall still work by pressing A on the tree, water or rock; if nobody on your team knows the move, Oak's Farfetch'd, Lapras, Tauros, Machop or Gyarados steps in. Oak explains it in the lab; older saves that already got the grant receive the Pager once. Oak's Pokémon are out and about: Pikachu, Farfetch'd, Abra and Machop in the lab (after the lab scene), Tauros and Pidgeot in the lab yard, Lapras and Gyarados on the Pallet shore. Talk to them.
- New: Rest (#56). Nurse Joy and Delia offer Heal, Rest or No thanks. Rest heals your team too and moves the clock forward: during the day to 8 PM (night), at night to 7 AM the next morning. Night now always starts at 8 PM and lasts until 5 AM, whatever the season (it used to shift with the seasons).
- Night water: surfing and fishing find different Pokémon at night (#56). Pallet's sea glows with Chinchou and Staryu; Wooper takes over the Route 22 and Viridian ponds, with a rare Slowpoke in Viridian.
- New: changing a Research Rule plays a short video call (#45): close the Rules (Options > Research Rules) after changing something and you and Ivy call Elm and Jenny, who note it for the bet. No change, no call. The same lines for every rule for now. Behind it, the intro's video-call screen can now host other calls (#13), ready for badge calls and the like.
- New: Oak + Daisy rematch (#53). After the Route 22 6v6, Daisy waits by Oak in his lab; talk to either for a double battle, as often as you like. Oak brings the two starters left over in the lab and his Slowpoke; Daisy brings Muffin (a Rattata forever, with an Eviolite), Clefairy and Vulpix. Their levels follow your badges (just under your level cap), they evolve at set badges, and after the Elite Four there's a stronger post-game tier (Lv 75). No prize money, 1.5x EXP. Oak, Daisy and Delia use their own battle art.
- New: battle Delia (#70). After the Route 22 6v6, Delia asks if you want a battle before Heal / Rest (No goes to the usual menu). A double on her own: Mimey leads, with Oddish (Gloom, then Bellossom), Grimer (Muk later), Miltank, a shiny Butterfree and Munchlax (a Snorlax after the Elite Four: the grocery bill). Same badge tiers, no money, 1.5x EXP.
- New: Brock rematches (#52). After his badge, talk to Brock in the Gym and pick a challenge, up to your badge count ("1 badge: Lv. 21" and so on, "Champion: Lv. 75" after the League). His Gym team grows: Sandslash at 2 badges, Graveler at 3, Golem, Kabutops, Omastar, Rhydon and Steelix at 4. Prize money as usual, 1.5x EXP, no whiteout. The other Gym leaders follow when their Gyms are reworked.
- Post-game rematches bring regional forms (owner's rule): after the Elite Four, Mimey and Muffin stay home while exchange-program visitors fight in their place (a Galarian Mr. Rime, an Alolan Raticate), with Alolan Ninetales, Alolan Muk, Hisuian Typhlosion, Alolan Golem, Alolan Sandslash and Rhyperior.
- Fixed: the Start menu clock box no longer shows garbled text (#27). It now reads "Day 14:05" or "Night 20:00" (the season line is gone; seasons don't change anything in this game).
- New: a lab aide in Oak's Lab (#72) explains every research tool on request: Ranch Pager, PC anywhere, DexNav, Field Guide, Research Rules, evolving, TMs, resting and rematches.
- Test builds: the debug menu stays on (Start > Debug > Scripts). Skip 10 (after Brock) now also makes Pallet and Viridian Fly targets.
- Still to come (moved to v0.0.17): the cozier interface colours (pick from the mock-ups), the title screen, Brock watch scene, proper case for FireRed's own text, and the art items.
- **Save:** old saves work. Saves that already got the research grant receive TM51-TM100 and the Ranch Pager once, with a note from Oak.

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
