@ Research Red: Breakfast at Delia's (PalletTown_PlayersHouse_1F)
@ Moved out of the FireRed map files; the map keeps only its map_script hooks.

@ ---------------------------------------------------------------
@ Research Red: breakfast cutscene (VAR_0x40FE 0 -> 1)
@ ---------------------------------------------------------------
PalletTown_PlayersHouse_1F_EventScript_Breakfast::
	lockall
	@ #84: breakfast at the table. Ivy is already in her chair (5,4); you sit
	@ below her (5,5), Mom across (8,4). Facing each other = up/down, Mom = right.
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_PlayerToTable
	waitmovement 0
	applymovement LOCALID_MOM, PalletTown_PlayersHouse_1F_Movement_FaceLeft
	waitmovement 0
	delay 15
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_GoodMorning, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceUp
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_FaceDown
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_Ferry, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_Gerald, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_Respect, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_ThrewUp, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_Sacrifice, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_BraveSailor, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_Pancakes, MSGBOX_DEFAULT
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_Dangerous, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceUp
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_Authorities, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceRight
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_FaceRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_OakCalled, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_FaceDown
	waitmovement 0
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_CertainSomeone, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_LoudPassion, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_SixAM, MSGBOX_DEFAULT
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_Competition, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_PeakBeforeLunch, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_EatUp, MSGBOX_DEFAULT
	closemessage
	@ Mimey sweeps past
	applymovement LOCALID_PALLET_HOUSE_MIMEY, PalletTown_PlayersHouse_1F_Movement_MimeySweep
	delay 60
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceDown
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_FaceDown
	waitmovement LOCALID_PALLET_HOUSE_MIMEY
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_PayingRent, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_Family, MSGBOX_DEFAULT
	closemessage
	@ Ivy elbows you
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_IvyElbow
	waitmovement 0
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_BeNice, MSGBOX_DEFAULT
	closemessage
	delay 30
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_Supervised, MSGBOX_DEFAULT
	closemessage
	@ Mimey waves off the credit with a little bow
	applymovement LOCALID_PALLET_HOUSE_MIMEY, PalletTown_PlayersHouse_1F_Movement_MimeyBow
	waitmovement 0
	textcolor NPC_TEXT_COLOR_MON
	msgbox PalletTown_PlayersHouse_1F_Text_Mimey_MimeMime, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceUp
	applymovement LOCALID_PALLET_HOUSE_IVY, PalletTown_PlayersHouse_1F_Movement_FaceDown
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_WritingThisDown, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_Ivy_PublishLater, MSGBOX_DEFAULT
	closemessage
	applymovement OBJ_EVENT_ID_PLAYER, PalletTown_PlayersHouse_1F_Movement_FaceRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_P_Thanks, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PalletTown_PlayersHouse_1F_Text_Delia_DoorOpen, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_MOM, PalletTown_PlayersHouse_1F_Movement_FaceLeft
	waitmovement 0
	@ From here on Ivy follows the player (data/scripts/rr_ivy.inc)
	rr_ivy_follow LOCALID_PALLET_HOUSE_IVY
	setvar VAR_0x40FE, 1
	releaseall
	end

@ ---------------------------------------------------------------
@ Movements
@ ---------------------------------------------------------------
@ From the stairs (10,2), round the table to the lower left chair (5,5)
PalletTown_PlayersHouse_1F_Movement_PlayerToTable:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	face_right
	step_end

PalletTown_PlayersHouse_1F_Movement_FaceUp:
	face_up
	step_end

PalletTown_PlayersHouse_1F_Movement_FaceDown:
	face_down
	step_end

PalletTown_PlayersHouse_1F_Movement_FaceLeft:
	face_left
	step_end

PalletTown_PlayersHouse_1F_Movement_FaceRight:
	face_right
	step_end

PalletTown_PlayersHouse_1F_Movement_MimeySweep:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	face_left
	step_end

PalletTown_PlayersHouse_1F_Movement_IvyElbow:
	face_down
	walk_in_place_fast_down
	step_end

PalletTown_PlayersHouse_1F_Movement_MimeyBow:
	face_down
	delay_16
	face_left
	step_end

PalletTown_PlayersHouse_1F_Movement_IvyExit:
	walk_down
	walk_left
	walk_down
	step_end

@ ---------------------------------------------------------------
@ Research Red: breakfast cutscene
@ ---------------------------------------------------------------
PalletTown_PlayersHouse_1F_Text_Delia_GoodMorning::
    .string "Delia: Good morning! You two got\n"
    .string "in so late last night.$"

PalletTown_PlayersHouse_1F_Text_P_Ferry::
    .string "{PLAYER}: The ferry from Cinnabar\n"
    .string "was “delayed.”$"

PalletTown_PlayersHouse_1F_Text_Ivy_Gerald::
    .string "Ivy: Gerald sat on the anchor\n"
    .string "line. For two hours.$"

PalletTown_PlayersHouse_1F_Text_P_Respect::
    .string "{PLAYER}: Honestly? Respect.\n"
    .string "I couldn't look away from him.$"

PalletTown_PlayersHouse_1F_Text_Ivy_ThrewUp::
    .string "Ivy: You watched him so long you\n"
    .string "got seasick, sweetie. Twice.$"

PalletTown_PlayersHouse_1F_Text_Ivy_BraveSailor::
    .string "Ivy: Mm-hm. Glad you're feeling\n"
    .string "better, my brave sailor.$"

PalletTown_PlayersHouse_1F_Text_P_Sacrifice::
    .string "{PLAYER}: Once. The second time\n"
    .string "was a sacrifice to the sea.$"

PalletTown_PlayersHouse_1F_Text_Delia_Pancakes::
    .string "Delia: Well, I made pancakes.\n"
    .string "Sit, sit!$"

PalletTown_PlayersHouse_1F_Text_Ivy_Dangerous::
    .string "Ivy: I've already had three.\n"
    .string "They're dangerous, sweetie.$"

PalletTown_PlayersHouse_1F_Text_P_Authorities::
    .string "{PLAYER}: Three? I'll notify the\n"
    .string "authorities.$"

PalletTown_PlayersHouse_1F_Text_Delia_OakCalled::
    .string "Delia: Prof. Oak called earlier.\n"
    .string "He's so excited you're here.\p"
    .string "He says his two best students\n"
    .string "are finally back in Pallet.$"

PalletTown_PlayersHouse_1F_Text_Ivy_CertainSomeone::
    .string "Ivy: And one of them is staying\n"
    .string "next door. Loudly?$"

PalletTown_PlayersHouse_1F_Text_P_LoudPassion::
    .string "{PLAYER}: I've always respected\n"
    .string "Elm's passion.\p"
    .string "A very loud passion, nonetheless.$"

PalletTown_PlayersHouse_1F_Text_Delia_SixAM::
    .string "Delia: Oh, I heard him at 6 AM.\p"
    .string "Something about “bragging rights.”$"

PalletTown_PlayersHouse_1F_Text_Ivy_Competition::
    .string "Ivy: He's already started the\n"
    .string "competition, hasn't he?$"

PalletTown_PlayersHouse_1F_Text_P_PeakBeforeLunch::
    .string "{PLAYER}: He'll peak before lunch.\n"
    .string "Let him have his morning.$"

PalletTown_PlayersHouse_1F_Text_Delia_EatUp::
    .string "Delia: Eat up first! Nobody wins\n"
    .string "on an empty stomach.\p"
    .string "Ash used to forget breakfast\n"
    .string "every time. Every. Time.$"

PalletTown_PlayersHouse_1F_Text_P_PayingRent::
    .string "{PLAYER}: …Is he paying rent?$"

PalletTown_PlayersHouse_1F_Text_Delia_Family::
    .string "Delia: Mimey does the dishes,\n"
    .string "so he's family!$"

PalletTown_PlayersHouse_1F_Text_Ivy_BeNice::
    .string "Ivy: Be nice. He made the\n"
    .string "pancakes.$"

PalletTown_PlayersHouse_1F_Text_Delia_Supervised::
    .string "Delia: …I supervised.$"

PalletTown_PlayersHouse_1F_Text_Mimey_MimeMime::
    .string "Mimey: Mime. Mime mime mime!\n"
    .string "(It did not need supervising.)$"

PalletTown_PlayersHouse_1F_Text_P_WritingThisDown::
    .string "{PLAYER}: (Field notes: Mimey.\n"
    .string "Pancakes. Dangerous. See Ivy.)$"

PalletTown_PlayersHouse_1F_Text_Ivy_PublishLater::
    .string "Ivy: Eat first, sweetie.\n"
    .string "Publish later.\p"
    .string "We'll head to the lab after this.\n"
    .string "Oak's waiting.$"

PalletTown_PlayersHouse_1F_Text_P_Thanks::
    .string "{PLAYER}: Thanks for having us,\n"
    .string "Delia. Truly.$"

PalletTown_PlayersHouse_1F_Text_Delia_DoorOpen::
    .string "Delia: Come back anytime you need\n"
    .string "rest. My door's always open.$"

@ ---------------------------------------------------------------
@ Talk-to texts
@ ---------------------------------------------------------------
PalletTown_PlayersHouse_1F_Text_Delia_AfterBreakfast::
    .string "Delia: Prof. Oak's waiting at the\n"
    .string "lab. Good luck, you two!$"

PalletTown_PlayersHouse_1F_Text_Mimey_Talk::
    .string "Mimey: Mime! Mime mime!$"
