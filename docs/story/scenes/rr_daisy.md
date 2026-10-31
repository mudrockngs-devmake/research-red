# Scene: rr_daisy

Research Red: Daisy's house: maps, Muffin, the bet (PalletTown_RivalsHouse)

### RR_Daisy_EventScript_Scene

*[rr_ivy_to_scene LOCALID_RIVALS_HOUSE_IVY]*

> Elm is standing on a chair.
> Jenny is holding it steady.

> Elm: {PLAYER}! Help!
> It's looking at me!

> A small purple Pokémon sits
> calmly on the rug.

> Daisy: It's a Rattata, Elm.
> Her name is Muffin.

> Elm: Muffin has the eyes
> of a Persian.

> Jenny: Muffin has the eyes of a
> Rattata, dear. Come down.

> Cruller, Jenny's Growlithe, sits
> by her chair. Perfectly still.

> Ivy: Cruller's so well behaved.
> Better than some professors.

> Jenny: Police training. I wanted
> to give him a proper police name.

> Elm: Cruller IS a proper police
> name! I gave a whole lecture!

> Jenny: His lecture had citations.
> I lost.

> {PLAYER}: Daisy? Your grandpa says
> you have Town Maps for us.

> Daisy: I had four. Gary took one.
> Ash borrowed another.
>
> I'm still waiting.
> So: one per couple.

> Elm: One per couple?!
> But strategy! Divide and conquer!

> Jenny: We share a toothbrush cup,
> dear. We can share a map.

*[additem ITEM_TOWN_MAP]*

> {PLAYER} received the Town Map!

> Daisy: Grandpa said two more
> professors were supposed to come.
>
> A couple from Alola?

> Ivy: Kukui and Burnett.
> They couldn't get away.
>
> Something about starting
> a league.

> Ivy: Burnett once showed me a
> Vulpix from Alola's mountains.
>
> White. Cold to the touch.
> Same species, different home.

> {PLAYER}: That's when she married
> me, by the way. Mid-Vulpix story.

> Ivy: I married you after the
> story, sweetie.
>
> I'm a professional.

> Muffin hops onto the chair.
> Elm hops onto the table.

> Elm: Ah… ah… choo!
> Allergies. Daisy's Persian.

> Daisy: Muffin. Rattata.

> Elm: …A very fluffy Rattata.

> Elm: Official terms! First team
> to win all eight Gym badges
> takes the bet!

> Jenny: Badges? Small stakes, dear.
> What does the loser do?

> Ivy: Losers cook a big dinner at
> Delia's. Dessert included.

> {PLAYER}: Mimey won't like anyone
> else in his kitchen.

> Ivy: Then Mimey supervises. And
> Delia gets the night off.

> Elm: Deal! Not that I'll lose.
> But I'd make a great tart.
>
> We swap numbers, and
> it's official!

> Elm's and Jenny's numbers were
> saved!

> The bet is on: first team to
> eight Gym badges.

> Daisy: I'll keep score.
> Somebody neutral should.

> Daisy: Grandpa is going to pretend
> he's not excited about this.

> Elm: He is so excited about this.

> Daisy: Muffin. Here, sweetheart.

> Elm climbs down onto his chair.
> Calmly. Mostly.

> Elm: So. {PLAYER}. Before the Gyms,
> I've been thinking about rules.

> {PLAYER}: You've been thinking?
> Should we sit down?

> Elm: We are sitting down.

> Elm: Real research needs controls!
> Level caps before each Gym,
> so nobody overtrains.

> Jenny: He means so he doesn't
> overtrain. He has a problem.

> Elm: I have a passion.

> Elm: Want to change them later?
> Options, “Research Rules.”
>
> Or call me! Day or night!

> Jenny: Please not night.

> Elm: …Hey. I'm really glad you two
> came. Johto was quiet without you.

> {PLAYER}: Elm. You called me
> every week.

> Elm: And it was quiet between
> the calls!

> {PLAYER}: …I'm glad we came too.

> Ivy: Group hug, or are we too
> professional?

> Jenny: In my professional opinion:
> group hug.

> Elm: See you on Route 1!
> Last one to Viridian buys lunch!

*[rr_ivy_follow LOCALID_RIVALS_HOUSE_IVY]*

*[setvar VAR_0x40FE ]*

### RR_Daisy_EventScript_NotYet

*[rr_ivy_to_scene LOCALID_PALLET_IVY @ she steps back too]*

> Someone's shouting from Daisy's
> window.

> Elm: Hey! Where are you going?!
> Daisy's house is the other way!

> Ivy: It matters to Elm, sweetie.
> Let's be good sports.

*[rr_ivy_follow LOCALID_PALLET_IVY]*

### RR_Daisy_EventScript_SetupScene

### RR_Daisy_EventScript_Talk

> Daisy: Muffin has claimed the
> chair. Elm will have to stand.
>
> Grandpa's proud of you two, you
> know. He just won't say it.

### RR_Daisy_EventScript_Muffin

> Muffin: Rat! Tat tat!
>
> She looks extremely pleased with
> herself.

### RR_Daisy_EventScript_Cruller

> Cruller: Woof! (He sits. Very
> properly.)
