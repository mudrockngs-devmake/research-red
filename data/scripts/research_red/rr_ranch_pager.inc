@ Research Red (#29): the Ranch Pager. Bag > Key Items > Ranch Pager (or
@ registered to SELECT) calls one of Oak's Pokémon for a field move. Cut,
@ Surf, Strength and the rest work by pressing A on the tree/water/boulder
@ (Oak's Pokémon step in when nobody in the team knows the move).
@ C side: src/rr_ranch_pager.c. Oak's Pokémon: defaultSpecies in src/field_move.c.
RR_EventScript_RanchPager::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	message RR_RanchPager_Text_WhoToCall
	waitmessage
	dynmultichoice 19, 0, FALSE, 5, 0, DYN_MULTICHOICE_CB_NONE, RR_RanchPager_Text_Fly, RR_RanchPager_Text_Flash, RR_RanchPager_Text_Teleport, RR_RanchPager_Text_Dig, RR_RanchPager_Text_Cancel
	closemessage
	switch VAR_RESULT
	case 0, RR_RanchPager_EventScript_Fly
	case 1, RR_RanchPager_EventScript_Flash
	case 2, RR_RanchPager_EventScript_Teleport
	case 3, RR_RanchPager_EventScript_Dig
	releaseall
	end

RR_RanchPager_EventScript_Fly::
	setvar VAR_0x8004, FIELD_MOVE_FLY
	callnative RR_ScriptRanchPagerCheck
	goto_if_eq VAR_RESULT, FALSE, RR_RanchPager_EventScript_CantUse
	goto RR_RanchPager_EventScript_Go

RR_RanchPager_EventScript_Flash::
	setvar VAR_0x8004, FIELD_MOVE_FLASH
	callnative RR_ScriptRanchPagerCheck
	goto_if_eq VAR_RESULT, FALSE, RR_RanchPager_EventScript_NotDark
	goto RR_RanchPager_EventScript_Go

RR_RanchPager_EventScript_Teleport::
	setvar VAR_0x8004, FIELD_MOVE_TELEPORT
	callnative RR_ScriptRanchPagerCheck
	goto_if_eq VAR_RESULT, FALSE, RR_RanchPager_EventScript_CantUse
	msgbox RR_RanchPager_Text_TeleportTo, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_RanchPager_EventScript_Release
	goto RR_RanchPager_EventScript_Go

RR_RanchPager_EventScript_Dig::
	setvar VAR_0x8004, FIELD_MOVE_DIG
	callnative RR_ScriptRanchPagerCheck
	goto_if_eq VAR_RESULT, FALSE, RR_RanchPager_EventScript_NoDig
	msgbox RR_RanchPager_Text_DigTo, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_RanchPager_EventScript_Release
	goto RR_RanchPager_EventScript_Go

@ The screen fades, then the game returns to the map (or opens the Fly map)
@ and Oak's Pokémon does the move, as if chosen from the party menu
RR_RanchPager_EventScript_Go::
	closemessage
	fadescreen FADE_TO_BLACK
	callnative RR_ScriptRanchPagerGo
	releaseall
	end

RR_RanchPager_EventScript_CantUse::
	msgbox RR_RanchPager_Text_CantUse, MSGBOX_DEFAULT
	goto RR_RanchPager_EventScript_Release

RR_RanchPager_EventScript_NotDark::
	msgbox RR_RanchPager_Text_NotDark, MSGBOX_DEFAULT
	goto RR_RanchPager_EventScript_Release

RR_RanchPager_EventScript_NoDig::
	msgbox RR_RanchPager_Text_NoDig, MSGBOX_DEFAULT
	goto RR_RanchPager_EventScript_Release

RR_RanchPager_EventScript_Release::
	closemessage
	releaseall
	end

@ An older save that got the research grant before the Pager existed gets
@ it once (src/rr_save_version.c)
RR_EventScript_TopUpRanchPager::
	lockall
	additem ITEM_RANCH_PAGER
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_RanchPager_Text_TopUp, MSGBOX_DEFAULT
	releaseall
	end

RR_RanchPager_Text_WhoToCall:
	.string "Which of Prof. Oak's Pokémon\n"
	.string "should you call?$"

RR_RanchPager_Text_Fly:
	.string "Fly$"

RR_RanchPager_Text_Flash:
	.string "Flash$"

RR_RanchPager_Text_Teleport:
	.string "Teleport$"

RR_RanchPager_Text_Dig:
	.string "Dig$"

RR_RanchPager_Text_Cancel:
	.string "Cancel$"

RR_RanchPager_Text_CantUse:
	.string "Oak's {STR_VAR_1} can't do that\n"
	.string "here.$"

RR_RanchPager_Text_NotDark:
	.string "It isn't dark here. Oak's\n"
	.string "{STR_VAR_1} can stay home.$"

RR_RanchPager_Text_NoDig:
	.string "There's no cave to dig out of\n"
	.string "here.$"

RR_RanchPager_Text_TeleportTo:
	.string "Oak's {STR_VAR_1} can take you\n"
	.string "back to {STR_VAR_2}. Go?$"

RR_RanchPager_Text_DigTo:
	.string "Oak's {STR_VAR_1} can dig you out\n"
	.string "to {STR_VAR_2}. Go?$"

RR_RanchPager_Text_TopUp:
	.string "Oak: {PLAYER}! I nearly forgot\n"
	.string "the best part of the grant!\p"
	.string "The Ranch Pager! Page my Pokémon\n"
	.string "from the ranch to Fly, Flash,\l"
	.string "Teleport or Dig.\p"
	.string "And when nobody on your team\n"
	.string "knows Cut or Surf, my Pokémon\l"
	.string "come running to help.\p"
	.string "As they say, a Pidgeot in the\n"
	.string "hand is worth two in the bush!$"

@ Oak's Pokemon in the lab, the lab yard and on the Pallet shore (#29).
@ The lab ones are hidden (FLAG_TEMP_3) until the lab scene is over.
RR_RanchPager_EventScript_HideLabPokemon::
	setflag FLAG_TEMP_3
	return

RR_RanchPager_EventScript_OakPikachu::
	lock
	faceplayer
	playmoncry SPECIES_PIKACHU, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_RanchPager_Text_OakPikachu, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_RanchPager_EventScript_OakFarfetchd::
	lock
	faceplayer
	playmoncry SPECIES_FARFETCHD, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_RanchPager_Text_OakFarfetchd, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_RanchPager_EventScript_OakAbra::
	lock
	faceplayer
	playmoncry SPECIES_ABRA, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_RanchPager_Text_OakAbra, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_RanchPager_EventScript_OakMachop::
	lock
	faceplayer
	playmoncry SPECIES_MACHOP, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_RanchPager_Text_OakMachop, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_RanchPager_EventScript_OakTauros::
	lock
	faceplayer
	playmoncry SPECIES_TAUROS, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_RanchPager_Text_OakTauros, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_RanchPager_EventScript_OakPidgeot::
	lock
	faceplayer
	playmoncry SPECIES_PIDGEOT, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_RanchPager_Text_OakPidgeot, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_RanchPager_EventScript_OakLapras::
	lock
	faceplayer
	playmoncry SPECIES_LAPRAS, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_RanchPager_Text_OakLapras, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_RanchPager_EventScript_OakGyarados::
	lock
	faceplayer
	playmoncry SPECIES_GYARADOS, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_RanchPager_Text_OakGyarados, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_RanchPager_Text_OakPikachu:
	.string "Pikachu: Pika pika!\p"
	.string "Prof. Oak's Pikachu. It lights up\n"
	.string "the dark for Flash. And the room.$"

RR_RanchPager_Text_OakFarfetchd:
	.string "Farfetch'd: Kwaa!\p"
	.string "Prof. Oak's Farfetch'd. Its leek\n"
	.string "is sharp enough to Cut trees.$"

RR_RanchPager_Text_OakAbra:
	.string "Abra: …Abra.\p"
	.string "Prof. Oak's Abra. It's asleep. It\n"
	.string "Teleports in its sleep, too.$"

RR_RanchPager_Text_OakMachop:
	.string "Machop: Macho!\p"
	.string "Prof. Oak's Machop. It practices\n"
	.string "Rock Smash on the lab's walnuts.$"

RR_RanchPager_Text_OakTauros:
	.string "Tauros: Mooo!\p"
	.string "Prof. Oak's Tauros. Strength? It\n"
	.string "has plenty, and it knows it.$"

RR_RanchPager_Text_OakPidgeot:
	.string "Pidgeot: Pijooo!\p"
	.string "Prof. Oak's Pidgeot. It knows the\n"
	.string "way to every town. Fly anytime!$"

RR_RanchPager_Text_OakLapras:
	.string "Lapras: Laaa…\p"
	.string "Prof. Oak's Lapras. It hums while\n"
	.string "it Surfs. Rather beautifully.$"

RR_RanchPager_Text_OakGyarados:
	.string "Gyarados: Gyaaa!\p"
	.string "Prof. Oak's Gyarados. It climbs\n"
	.string "Waterfalls. It was a Magikarp!$"
