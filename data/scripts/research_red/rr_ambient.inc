@ Research Red (#64): ambient Pokémon that make the world feel alive. They
@ don't battle; talking to one plays its cry, a short line, and marks it as
@ seen in the Pokédex (research!). Pilot set approved by the owner (2026-10-20).

@ VAR_0x8004 = species: cry, and seen in the Pokédex
RR_Ambient_EventScript_Seen::
	playmoncry VAR_0x8004, CRY_MODE_NORMAL
	special SetSeenMon
	waitmoncry
	textcolor NPC_TEXT_COLOR_MON
	return

RR_Ambient_EventScript_Dodrio::
	lock
	faceplayer
	setvar VAR_0x8004, SPECIES_DODRIO
	call RR_Ambient_EventScript_Seen
	msgbox RR_Ambient_Text_Dodrio, MSGBOX_DEFAULT
	release
	end

RR_Ambient_EventScript_Diglett::
	lock
	setvar VAR_0x8004, SPECIES_DIGLETT
	call RR_Ambient_EventScript_Seen
	msgbox RR_Ambient_Text_Diglett, MSGBOX_DEFAULT
	release
	end

RR_Ambient_EventScript_Chansey::
	lock
	faceplayer
	setvar VAR_0x8004, SPECIES_CHANSEY
	call RR_Ambient_EventScript_Seen
	msgbox RR_Ambient_Text_Chansey, MSGBOX_DEFAULT
	release
	end

RR_Ambient_EventScript_ForestPidgeot::
	lock
	setvar VAR_0x8004, SPECIES_PIDGEOT
	call RR_Ambient_EventScript_Seen
	msgbox RR_Ambient_Text_ForestPidgeot, MSGBOX_DEFAULT
	release
	end

RR_Ambient_EventScript_Fearow::
	lock
	setvar VAR_0x8004, SPECIES_FEAROW
	call RR_Ambient_EventScript_Seen
	msgbox RR_Ambient_Text_Fearow, MSGBOX_DEFAULT
	release
	end

RR_Ambient_Text_Dodrio:
	.string "Dodrio: Do-dodo-DRIO!\p"
	.string "Three heads, three opinions. It\n"
	.string "crows at the morning anyway.$"

RR_Ambient_Text_Diglett:
	.string "Diglett: Dig dig!\p"
	.string "It's Prof. Oak's Diglett. It's on\n"
	.string "call for Dig.$"

RR_Ambient_Text_Chansey:
	.string "Chansey: Chaaan-sey!\p"
	.string "Nurse Joy's helper. It's checking\n"
	.string "your Poké Balls are polished.$"

RR_Ambient_Text_ForestPidgeot:
	.string "Pidgeot: Pijooo!\p"
	.string "The forest's guardian. It's\n"
	.string "keeping an eye on that Fearow.$"

RR_Ambient_Text_Fearow:
	.string "Fearow: Fearrr!\p"
	.string "It stares at Pidgeot. Neither of\n"
	.string "them blinks.$"
