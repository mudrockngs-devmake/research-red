@ ===================================================================
@ Research Red: Ivy, the player's wife, follows the player from the end
@ of breakfast on. Macros: rr_ivy_to_scene / rr_ivy_follow /
@ rr_ivy_create_follower (asm/macros/event.inc).
@ Talking to her: a line for the area you're in (or a general one).
@ ===================================================================
RR_EventScript_IvyTalk::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	callnative RR_ScriptIvyTalkArea
	switch VAR_RESULT
	case 0, RR_EventScript_IvyTalk_Anywhere
	case 1, RR_EventScript_IvyTalk_Pallet
	case 2, RR_EventScript_IvyTalk_Route1
	case 3, RR_EventScript_IvyTalk_Viridian
	case 4, RR_EventScript_IvyTalk_Route22
	case 5, RR_EventScript_IvyTalk_Route2
	case 6, RR_EventScript_IvyTalk_Forest
	case 7, RR_EventScript_IvyTalk_Pewter
	goto RR_EventScript_IvyTalk_Anywhere

RR_EventScript_IvyTalk_Anywhere::
	random 3
	switch VAR_RESULT
	case 1, RR_EventScript_IvyTalk_Anywhere_1
	case 2, RR_EventScript_IvyTalk_Anywhere_2
	msgbox RR_Text_IvyTalk_Anywhere_0, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Anywhere_1::
	msgbox RR_Text_IvyTalk_Anywhere_1, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Anywhere_2::
	msgbox RR_Text_IvyTalk_Anywhere_2, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Pallet::
	random 3
	switch VAR_RESULT
	case 1, RR_EventScript_IvyTalk_Pallet_1
	case 2, RR_EventScript_IvyTalk_Pallet_2
	msgbox RR_Text_IvyTalk_Pallet_0, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Pallet_1::
	msgbox RR_Text_IvyTalk_Pallet_1, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Pallet_2::
	msgbox RR_Text_IvyTalk_Pallet_2, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Route1::
	random 3
	switch VAR_RESULT
	case 1, RR_EventScript_IvyTalk_Route1_1
	case 2, RR_EventScript_IvyTalk_Route1_2
	msgbox RR_Text_IvyTalk_Route1_0, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Route1_1::
	msgbox RR_Text_IvyTalk_Route1_1, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Route1_2::
	msgbox RR_Text_IvyTalk_Route1_2, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Viridian::
	random 3
	switch VAR_RESULT
	case 1, RR_EventScript_IvyTalk_Viridian_1
	case 2, RR_EventScript_IvyTalk_Viridian_2
	msgbox RR_Text_IvyTalk_Viridian_0, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Viridian_1::
	msgbox RR_Text_IvyTalk_Viridian_1, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Viridian_2::
	msgbox RR_Text_IvyTalk_Viridian_2, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Route22::
	random 3
	switch VAR_RESULT
	case 1, RR_EventScript_IvyTalk_Route22_1
	case 2, RR_EventScript_IvyTalk_Route22_2
	msgbox RR_Text_IvyTalk_Route22_0, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Route22_1::
	msgbox RR_Text_IvyTalk_Route22_1, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Route22_2::
	msgbox RR_Text_IvyTalk_Route22_2, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Route2::
	random 3
	switch VAR_RESULT
	case 1, RR_EventScript_IvyTalk_Route2_1
	case 2, RR_EventScript_IvyTalk_Route2_2
	msgbox RR_Text_IvyTalk_Route2_0, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Route2_1::
	msgbox RR_Text_IvyTalk_Route2_1, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Route2_2::
	msgbox RR_Text_IvyTalk_Route2_2, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Forest::
	random 3
	switch VAR_RESULT
	case 1, RR_EventScript_IvyTalk_Forest_1
	case 2, RR_EventScript_IvyTalk_Forest_2
	msgbox RR_Text_IvyTalk_Forest_0, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Forest_1::
	msgbox RR_Text_IvyTalk_Forest_1, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Forest_2::
	msgbox RR_Text_IvyTalk_Forest_2, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Pewter::
	random 3
	switch VAR_RESULT
	case 1, RR_EventScript_IvyTalk_Pewter_1
	case 2, RR_EventScript_IvyTalk_Pewter_2
	msgbox RR_Text_IvyTalk_Pewter_0, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Pewter_1::
	msgbox RR_Text_IvyTalk_Pewter_1, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk_Pewter_2::
	msgbox RR_Text_IvyTalk_Pewter_2, MSGBOX_DEFAULT
	release
	end

@ Ivy's follower lines, by area (src/research_red.c RR_ScriptIvyTalkArea)
RR_Text_IvyTalk_Anywhere_0:
	.string "Ivy: Field notes are up to date,\n"
	.string "sweetie. Lead the way.$"

RR_Text_IvyTalk_Anywhere_1:
	.string "Ivy: Regional forms can start\n"
	.string "with one stubborn ancestor.\p"
	.string "Science is so romantic.$"

RR_Text_IvyTalk_Anywhere_2:
	.string "Ivy: If Elm wins the bet, we'll\n"
	.string "send him malasadas from Alola.\p"
	.string "A whole box. He'll have earned it.$"

RR_Text_IvyTalk_Pallet_0:
	.string "Ivy: Pallet smells like sea salt\n"
	.string "and pancakes. I approve.$"

RR_Text_IvyTalk_Pallet_1:
	.string "Ivy: Oak's Slowpoke still hasn't\n"
	.string "hung up the call, you know.$"

RR_Text_IvyTalk_Pallet_2:
	.string "Ivy: Ash grew up here. You can\n"
	.string "tell. Even the fences look brave.$"

RR_Text_IvyTalk_Route1_0:
	.string "Ivy: Elm got lost on this road.\n"
	.string "It's one road, sweetie.$"

RR_Text_IvyTalk_Route1_1:
	.string "Ivy: Kanto Pidgey are bigger than\n"
	.string "Valencia's. All that sea wind.$"

RR_Text_IvyTalk_Route1_2:
	.string "Ivy: Are you still humming that\n"
	.string "song? …Me too.$"

RR_Text_IvyTalk_Viridian_0:
	.string "Ivy: Jenny's cousin waved at me.\n"
	.string "Or Jenny did. I'll ask Elm.$"

RR_Text_IvyTalk_Viridian_1:
	.string "Ivy: The Gym's locked. Somebody\n"
	.string "here is very busy.$"

RR_Text_IvyTalk_Viridian_2:
	.string "Ivy: That old man and his nap.\n"
	.string "I respect a man with priorities.$"

RR_Text_IvyTalk_Route22_0:
	.string "Ivy: The League is up that way.\n"
	.string "Not for us. Not yet.$"

RR_Text_IvyTalk_Route22_1:
	.string "Ivy: The Spearow here have\n"
	.string "attitude. Regional temperament?$"

RR_Text_IvyTalk_Route22_2:
	.string "Ivy: Elm still says that was a\n"
	.string "warm-up. Let him have it.$"

RR_Text_IvyTalk_Route2_0:
	.string "Ivy: Diglett's Cave is closed. I\n"
	.string "think they're having a meeting.$"

RR_Text_IvyTalk_Route2_1:
	.string "Ivy: Caterpie everywhere. The\n"
	.string "forest is spilling over.$"

RR_Text_IvyTalk_Route2_2:
	.string "Ivy: I love bug season. Everything\n"
	.string "here is about to evolve.$"

RR_Text_IvyTalk_Forest_0:
	.string "Ivy: Pikachu in a forest. Ash's\n"
	.string "was from somewhere like this.$"

RR_Text_IvyTalk_Forest_1:
	.string "Ivy: Every Bug Catcher wants a\n"
	.string "battle. It's a rite of passage.$"

RR_Text_IvyTalk_Forest_2:
	.string "Ivy: If you meet a Samurai in\n"
	.string "here, sweetie, just say yes.$"

RR_Text_IvyTalk_Pewter_0:
	.string "Ivy: Pewter is all rock and grit.\n"
	.string "Even the Pidgey look sturdier.$"

RR_Text_IvyTalk_Pewter_1:
	.string "Ivy: One badge. I'm already\n"
	.string "bragging to Jenny.$"

RR_Text_IvyTalk_Pewter_2:
	.string "Ivy: The Museum has a moon rock.\n"
	.string "Elm is going to cry.$"
