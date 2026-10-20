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
	.string "Oak: {PLAYER}! I forgot one thing\n"
	.string "from the grant: the Ranch Pager!\p"
	.string "Page my Pokémon from the ranch\n"
	.string "whenever you need to Fly, Flash,\l"
	.string "Teleport or Dig.\p"
	.string "And no need to teach anyone Cut\n"
	.string "or Surf: my Pokémon come to help\l"
	.string "when nobody on your team knows it!$"

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
	.string "It's Prof. Oak's Pikachu.\n"
	.string "It's on call for Flash.$"

RR_RanchPager_Text_OakFarfetchd:
	.string "Farfetch'd: Kwaa!\p"
	.string "It's Prof. Oak's Farfetch'd.\n"
	.string "It's on call for Cut.$"

RR_RanchPager_Text_OakAbra:
	.string "Abra: …Abra.\p"
	.string "It's Prof. Oak's Abra.\n"
	.string "It's on call for Teleport.$"

RR_RanchPager_Text_OakMachop:
	.string "Machop: Macho!\p"
	.string "It's Prof. Oak's Machop.\n"
	.string "It's on call for Rock Smash.$"

RR_RanchPager_Text_OakTauros:
	.string "Tauros: Mooo!\p"
	.string "It's Prof. Oak's Tauros.\n"
	.string "It's on call for Strength.$"

RR_RanchPager_Text_OakPidgeot:
	.string "Pidgeot: Pijooo!\p"
	.string "It's Prof. Oak's Pidgeot.\n"
	.string "It's on call for Fly.$"

RR_RanchPager_Text_OakLapras:
	.string "Lapras: Laaa…\p"
	.string "It's Prof. Oak's Lapras.\n"
	.string "It's on call for Surf.$"

RR_RanchPager_Text_OakGyarados:
	.string "Gyarados: Gyaaa!\p"
	.string "It's Prof. Oak's Gyarados.\n"
	.string "It's on call for Waterfall.$"
