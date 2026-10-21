@ ===================================================================
@ Research Red: demo boundaries (#35, #36, #62)
@ The demo ends at Pewter. Two friendly stops keep the world from getting
@ too open while the rest is written:
@  - Pewter's east exit: the Gym guide (the boy who walks you to Brock)
@    stays after Brock, gives a developer note and walks you back.
@    The Running Shoes aide never appears (you can run from the start).
@  - Pallet's shore: Gerald the Tentacool blocks the sea route to Cinnabar.
@  - Diglett's Cave (Route 2): a Dugtrio sits on the ladder with four
@    Diglett around it (the anime's "Dig Those Diglett!" crew).
@  - Route 22's League gate: the officer turns everyone back, badge or not
@    (Route 23 has level 30+ Pokemon).
@ Remove these when the story continues past Pewter / to Cinnabar.
@ ===================================================================

@ PewterCity OnTransition, after Brock: keep the guide, drop the aide
RR_Pewter_EventScript_DemoSetup::
	clearflag FLAG_HIDE_PEWTER_CITY_GYM_GUIDE
	setflag FLAG_HIDE_PEWTER_CITY_RUNNING_SHOES_GUY
	return

@ Talking to the guide after Brock
RR_Pewter_EventScript_DemoGuideTalk::
	textcolor NPC_TEXT_COLOR_MALE
	msgbox RR_Pewter_Text_DemoEnd, MSGBOX_DEFAULT
	release
	end

@ Stepping onto the east exit tiles after Brock
RR_Pewter_EventScript_DemoBlock::
	lockall
	playse SE_PIN
	applymovement LOCALID_PEWTER_GYM_GUIDE, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_PEWTER_GYM_GUIDE, Common_Movement_FacePlayer
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MALE
	msgbox RR_Pewter_Text_DemoEnd, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, RR_Demo_Movement_WalkBackWest
	waitmovement 0
	releaseall
	end

RR_Demo_Movement_WalkBackWest:
	walk_left
	walk_left
	step_end

RR_Pewter_Text_DemoEnd::
    .string "Hey! Sorry, the road past Pewter\n"
    .string "isn't built yet.\p"
    .string "This is a demo of Research Red.\n"
    .string "Thanks so much for playing!\p"
    .string "We'll open the way in a future\n"
    .string "update. Please understand!$"

@ Pallet's shore: surfing onto the row in front of Gerald
RR_Pallet_EventScript_GeraldBlock::
	lockall
	playmoncry SPECIES_TENTACOOL, CRY_MODE_NORMAL
	applymovement LOCALID_PALLET_GERALD, Common_Movement_ExclamationMark
	waitmovement 0
	waitmoncry
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Pallet_Text_GeraldBlocks, MSGBOX_DEFAULT
	msgbox RR_Pallet_Text_Ivy_Gerald, MSGBOX_DEFAULT
	msgbox RR_Pallet_Text_GeraldSign, MSGBOX_DEFAULT
	msgbox RR_Pallet_Text_P_Respect, MSGBOX_DEFAULT
	msgbox RR_Pallet_Text_Ivy_ComeBack, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, RR_Demo_Movement_BackNorth
	waitmovement 0
	releaseall
	end

@ Talking to Gerald from the water
RR_Pallet_EventScript_Gerald::
	lock
	playmoncry SPECIES_TENTACOOL, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Pallet_Text_GeraldBlocks, MSGBOX_DEFAULT
	msgbox RR_Pallet_Text_GeraldSign, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_Demo_Movement_BackNorth:
	walk_up
	step_end

RR_Pallet_Text_GeraldBlocks::
    .string "Gerald: Tenta! Cool!\p"
    .string "Gerald the Tentacool is floating\n"
    .string "right in the way.$"

RR_Pallet_Text_Ivy_Gerald::
    .string "Ivy: Gerald! You followed us\n"
    .string "all the way from the ferry?$"

RR_Pallet_Text_GeraldSign::
    .string "Gerald holds up a tiny sign:\p"
    .string "“Sea route under construction.\n"
    .string "This is a demo of Research Red.\p"
    .string "Cinnabar and beyond come later.\n"
    .string "Thanks for playing!”$"

RR_Pallet_Text_P_Respect::
    .string "{PLAYER}: Still blocking boats.\n"
    .string "Honestly? Respect.$"

RR_Pallet_Text_Ivy_ComeBack::
    .string "Ivy: Don't worry, Gerald.\n"
    .string "We'll come back for you.$"

@ Diglett's Cave north entrance (Route 2): the digging crew blocks the ladder
RR_DiglettsCave_EventScript_Diglett::
	lock
	playmoncry SPECIES_DIGLETT, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_DiglettsCave_Text_Diglett, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_DiglettsCave_EventScript_Dugtrio::
	lock
	playmoncry SPECIES_DUGTRIO, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_DiglettsCave_Text_Dugtrio, MSGBOX_DEFAULT
	waitmoncry
	release
	end

@ Placeholder lines: the owner will write the final ones
RR_DiglettsCave_Text_Diglett::
    .string "Diglett: Diglett dig, Diglett dig…$"

RR_DiglettsCave_Text_Dugtrio::
    .string "Dugtrio: Trio, trio, trio!\p"
    .string "Dugtrio is sitting right on the\n"
    .string "ladder, holding up a tiny sign:\p"
    .string "“Tunnel closed for digging.\n"
    .string "Thank you for your patience.\p"
    .string "This is a demo of Research Red.\n"
    .string "Thanks so much for playing!”$"

@ Route 22's League gate: closed in the demo, even with the Boulder Badge
RR_Route22Gate_EventScript_DemoGuardTalk::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_MALE
	msgbox RR_Route22Gate_Text_Demo, MSGBOX_DEFAULT
	release
	end

RR_Route22Gate_EventScript_DemoBlock::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	msgbox RR_Route22Gate_Text_Demo, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PLAYER, RR_Demo_Movement_WalkBackDown
	waitmovement 0
	releaseall
	end

RR_Demo_Movement_WalkBackDown:
	walk_down
	step_end

RR_Route22Gate_Text_Demo::
    .string "Officer: The road to the Pokémon\n"
    .string "League is closed for now.\p"
    .string "This is a demo of Research Red.\n"
    .string "Thanks so much for playing!$"
