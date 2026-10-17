Route1_MapScripts::
	.byte 0

Route1_EventScript_MartClerk::
	lock
	faceplayer
	goto_if_set FLAG_GOT_POTION_ON_ROUTE_1, Route1_EventScript_AlreadyGotPotion
	msgbox Route1_Text_WorkAtPokeMartTakeSample
	textcolor NPC_TEXT_COLOR_NEUTRAL
	checkitemspace ITEM_POTION
	goto_if_eq VAR_RESULT, FALSE, EventScript_BagIsFull
	bufferitemname STR_VAR_2, ITEM_POTION
	playfanfare MUS_LEVEL_UP
	message Text_ObtainedTheX
	waitmessage
	waitfanfare
	additem ITEM_POTION
	msgbox Route1_Text_PutPotionAway
	call EventScript_RestorePrevTextColor
	setflag FLAG_GOT_POTION_ON_ROUTE_1
	release
	end

Route1_EventScript_AlreadyGotPotion::
	msgbox Route1_Text_ComeSeeUsIfYouNeedPokeBalls
	release
	end

Route1_EventScript_Boy::
	msgbox Route1_Text_CanJumpFromLedges, MSGBOX_NPC
	end

Route1_EventScript_RouteSign::
	msgbox Route1_Text_RouteSign, MSGBOX_SIGN
	end

@ ===================================================================
@ Research Red: first walk up Route 1 (VAR_0x40FE == 4, after Daisy's house)
@ Ivy, Elm and Jenny fall in behind the player; something pink streaks
@ across the sky; Elm and Jenny dash ahead. Sets VAR_0x40FE to 5.
@ The objects use FLAG_HIDE_OAK_IN_PALLET_TOWN (already set), so they only
@ appear when this script adds them.
@ ===================================================================
RR_Route1_EventScript_Walk::
	lockall
	getplayerxy VAR_0x8004, VAR_0x8005
	copyvar VAR_0x8006, VAR_0x8005
	addvar VAR_0x8006, 1
	setobjectxyperm LOCALID_ROUTE1_IVY, VAR_0x8004, VAR_0x8006
	addvar VAR_0x8006, 1
	setobjectxyperm LOCALID_ROUTE1_ELM, VAR_0x8004, VAR_0x8006
	addvar VAR_0x8006, 1
	setobjectxyperm LOCALID_ROUTE1_JENNY, VAR_0x8004, VAR_0x8006
	addobject LOCALID_ROUTE1_IVY
	addobject LOCALID_ROUTE1_ELM
	addobject LOCALID_ROUTE1_JENNY
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_WeMeetAgain, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_RightHere, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Humming, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Ivy_OffKey, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_P_OwnKey, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Ivy_Married, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_Banter, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_Singing, MSGBOX_DEFAULT
	closemessage
	@ --- something pink streaks across the sky ---
	playse SE_PIN
	applymovement LOCALID_ROUTE1_ELM, Common_Movement_ExclamationMark
	waitmovement 0
	special SpawnCameraObject
	applymovement LOCALID_CAMERA, RR_Route1_Movement_CameraUp
	waitmovement 0
	delay 40
	applymovement LOCALID_CAMERA, RR_Route1_Movement_CameraDown
	waitmovement 0
	special RemoveCameraObject
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_SawIt, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_P_Believe, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_Believe, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_Tail, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_P_Seventy, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Ivy_Sixty, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Elm_Race, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Jenny_Protocol, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_ROUTE1_ELM
	removeobject LOCALID_ROUTE1_JENNY
	fadescreen FADE_FROM_BLACK
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_DashedAhead, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_Ivy_Quiet, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Route1_Text_P_Wonderful, MSGBOX_DEFAULT
	closemessage
	removeobject LOCALID_ROUTE1_IVY
	setvar VAR_0x40FE, 5
	releaseall
	end

RR_Route1_Movement_CameraUp:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

RR_Route1_Movement_CameraDown:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

RR_Route1_Text_Elm_WeMeetAgain::
    .string "Elm: Route One! We meet again!\n"
    .string "This time I brought a partner!$"

RR_Route1_Text_Jenny_RightHere::
    .string "Jenny: And a wife.\n"
    .string "I'm right here, dear.$"

RR_Route1_Text_Humming::
    .string "Elm starts humming a traveling\n"
    .string "song. Ivy joins in.$"

RR_Route1_Text_Ivy_OffKey::
    .string "Ivy: You're off-key, sweetie.$"

RR_Route1_Text_P_OwnKey::
    .string "{PLAYER}: I'm in a key of my own.\n"
    .string "It's very exclusive.$"

RR_Route1_Text_Ivy_Married::
    .string "Ivy: Ha! That's why I married you.$"

RR_Route1_Text_Elm_Banter::
    .string "Elm: Jenny, why don't we banter\n"
    .string "like that?$"

RR_Route1_Text_Jenny_Singing::
    .string "Jenny: We do. You just don't hear\n"
    .string "me over the singing.$"

RR_Route1_Text_Elm_SawIt::
    .string "Elm: Did you see that?!\n"
    .string "Something pink! Flying!\p"
    .string "It looked right at me!$"

RR_Route1_Text_P_Believe::
    .string "{PLAYER}: I believe you, Elm.$"

RR_Route1_Text_Jenny_Believe::
    .string "Jenny: I believe you saw\n"
    .string "something, dear.$"

RR_Route1_Text_Elm_Tail::
    .string "Elm: It had a tail!$"

RR_Route1_Text_P_Seventy::
    .string "{PLAYER}: (I believe him about\n"
    .string "seventy percent.)$"

RR_Route1_Text_Ivy_Sixty::
    .string "Ivy: (Sixty. But I love him\n"
    .string "for it.)$"

RR_Route1_Text_Elm_Race::
    .string "Elm: It flew toward Viridian!\n"
    .string "Jenny! After it!$"

RR_Route1_Text_Jenny_Protocol::
    .string "Jenny: Dear, by protocol we stay\n"
    .string "together. Dear? …Dear!$"

RR_Route1_Text_DashedAhead::
    .string "Elm and Jenny dashed ahead\n"
    .string "toward Viridian City.$"

RR_Route1_Text_Ivy_Quiet::
    .string "Ivy: And they're off. Lunch is\n"
    .string "going to be very quiet, sweetie.$"

RR_Route1_Text_P_Wonderful::
    .string "{PLAYER}: Lunch is going to be\n"
    .string "wonderful.$"
