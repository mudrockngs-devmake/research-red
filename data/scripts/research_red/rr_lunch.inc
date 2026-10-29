@ Research Red: lunch in Viridian City (animatic 07)
@ Straight after the Route 22 battle (story progress 6, #148): the Route 22
@ scene warps you below Elm, who sits right of the Pokemon Center (29-30, 26),
@ and Viridian's ON_FRAME starts the lunch (RR_Lunch_EventScript_Arrive).
@ Talking to Elm or Jenny still starts it too (#43).
@ Jenny runs into her cousin, the Viridian Officer Jenny, then a picnic.
@ (Elm's Research Rules moved to Daisy's house, #44.) Afterwards the old man
@ finishes his nap and the road north (Route 2, Viridian Forest) is open.
@ Ivy stays a follower for the whole scene.

@ Viridian ON_TRANSITION: Elm and Jenny wait by the Pokemon Center while the
@ lunch is pending; the cousin only appears during the scene
RR_Lunch_OnTransition::
	call_if_ne VAR_0x40FE, 6, RR_Lunch_EventScript_HideElmJenny
	setflag FLAG_TEMP_15
	return

RR_Lunch_EventScript_HideElmJenny::
	setflag FLAG_TEMP_13
	setflag FLAG_TEMP_14
	return

@ Arriving in Viridian with the lunch pending (from Route 22, or by Fly or a
@ debug skip): walk up below Elm and start (the walk goes down first, then
@ across, so it never crosses Elm and Jenny's spots)
RR_Lunch_EventScript_Arrive::
	lockall
	rr_walk_to LOCALID_PLAYER, 29, 27, TRUE
	waitmovement LOCALID_PLAYER
	applymovement LOCALID_PLAYER, Common_Movement_FaceUp
	applymovement LOCALID_VIRIDIAN_ELM, Common_Movement_FaceDown
	waitmovement 0
	goto RR_Lunch_EventScript_Main

RR_Lunch_EventScript_Start::
	lockall
	faceplayer
RR_Lunch_EventScript_Main::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Lunch_Text_Elm_ThereYouAre, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Jenny_Packed, MSGBOX_DEFAULT
	closemessage
	@ Officer Jenny walks up from the south
	addobject LOCALID_VIRIDIAN_OFFICER_JENNY
	applymovement LOCALID_VIRIDIAN_OFFICER_JENNY, RR_Lunch_Movement_OfficerArrives
	waitmovement 0
	applymovement LOCALID_VIRIDIAN_JENNY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_VIRIDIAN_JENNY, Common_Movement_ExclamationMark
	applymovement LOCALID_VIRIDIAN_OFFICER_JENNY, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox RR_Lunch_Text_OfficerJenny_Cousin, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Jenny_Cousin, MSGBOX_DEFAULT
	@ Elm always knows his Jenny (docs/characters/ELM.md)
	msgbox RR_Lunch_Text_Elm_PleasedToMeet, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Ivy_HowDidYouKnow, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Elm_Obviously, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_OfficerJenny_Family, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_P_HowMany, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Jenny_Reunion, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_OfficerJenny_Patrol, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Jenny_Protocol, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_OfficerJenny_Bye, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_VIRIDIAN_OFFICER_JENNY, RR_Lunch_Movement_OfficerLeaves
	waitmovement 0
	removeobject LOCALID_VIRIDIAN_OFFICER_JENNY
	msgbox RR_Lunch_Text_Elm_SeventeenCousins, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Jenny_Eighteen, MSGBOX_DEFAULT
	closemessage
	@ --- the picnic (#8): a blanket on the grass right of the Pokémon
	@ Center; Elm and Jenny on its left, you on its right ---
	fadescreen FADE_TO_BLACK
	setmetatile 30, 25, METATILE_RR_PicnicBlanket_TopLeft, TRUE
	setmetatile 31, 25, METATILE_RR_PicnicBlanket_TopRight, TRUE
	setmetatile 30, 26, METATILE_RR_PicnicBlanket_BottomLeft, TRUE
	setmetatile 31, 26, METATILE_RR_PicnicBlanket_BottomRight, TRUE
	special DrawWholeMapView
	applymovement LOCALID_VIRIDIAN_ELM, RR_Lunch_Movement_FaceRight
	applymovement LOCALID_VIRIDIAN_JENNY, RR_Lunch_Movement_FaceRight
	rr_walk_to LOCALID_PLAYER, 32, 26, FALSE, 0
	waitmovement LOCALID_PLAYER
	applymovement LOCALID_PLAYER, RR_Lunch_Movement_FaceLeft
	waitmovement 0
	msgbox RR_Lunch_Text_Picnic, MSGBOX_DEFAULT
	closemessage
	fadescreen FADE_FROM_BLACK
	msgbox RR_Lunch_Text_Jenny_Labeled, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Elm_Dessert, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Ivy_ForYourself, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Elm_InSpirit, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_P_Apology, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Ivy_Krabby, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Jenny_Report, MSGBOX_DEFAULT
	closemessage
	@ Elm and Jenny head north first (a fade: they'd walk through you otherwise)
	fadescreen FADE_TO_BLACK
	removeobject LOCALID_VIRIDIAN_ELM
	removeobject LOCALID_VIRIDIAN_JENNY
	@ the blanket is packed away
	setmetatile 30, 25, METATILE_General_RR_GrassEdge, FALSE
	setmetatile 31, 25, METATILE_General_RR_PathEdge, FALSE
	setmetatile 30, 26, METATILE_General_RR_GrassEdge, FALSE
	setmetatile 31, 26, METATILE_General_RR_PathEdge, FALSE
	@ The old man has finished his nap: the road north is open. He gets up
	@ now, not on the next visit (he lay across the road until you left, #148)
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 2
	removeobject LOCALID_TUTORIAL_MAN
	setvar VAR_OBJ_GFX_ID_0, OBJ_EVENT_GFX_OLD_MAN_1
	setobjectxyperm LOCALID_TUTORIAL_MAN, 21, 6
	setobjectmovementtype LOCALID_TUTORIAL_MAN, MOVEMENT_TYPE_LOOK_AROUND
	addobject LOCALID_TUTORIAL_MAN
	special DrawWholeMapView
	fadescreen FADE_FROM_BLACK
	setvar VAR_0x40FE, 7
	msgbox RR_Lunch_Text_OldManAwake, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Ivy_Forest, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_P_Patience, MSGBOX_DEFAULT
	releaseall
	end

@ The player stands at x 19; Elm stops beside them, Jenny just behind
@ The cousin comes in from the east along the row below the picnic (37,27)
@ -> (32,27): never through the tiles you can talk to Elm and Jenny from
RR_Lunch_Movement_OfficerArrives::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	face_left
	step_end

RR_Lunch_Movement_FaceRight:
	face_right
	step_end

RR_Lunch_Movement_FaceLeft:
	face_left
	step_end

RR_Lunch_Movement_OfficerLeaves::
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

@ ===================================================================
@ Research Red: lunch text (placeholder, from the animatic)
@ ===================================================================
RR_Lunch_Text_Elm_ThereYouAre::
    .string "Elm: Lunch! Lunch! Jenny made\n"
    .string "enough for a Snorlax.$"

RR_Lunch_Text_Jenny_Packed::
    .string "Jenny: I packed for six.\n"
    .string "Elm counts as three.$"

RR_Lunch_Text_OfficerJenny_Cousin::
    .string "Officer Jenny: Cousin?!$"

RR_Lunch_Text_Jenny_Cousin::
    .string "Jenny: Cousin!$"

RR_Lunch_Text_Elm_PleasedToMeet::
    .string "Elm: Hello! Pleased to meet you,\n"
    .string "Officer.$"

RR_Lunch_Text_Ivy_HowDidYouKnow::
    .string "Ivy: Elm, how did you know which\n"
    .string "one was which?$"

RR_Lunch_Text_Elm_Obviously::
    .string "Elm: That's my Jenny. Obviously.$"

RR_Lunch_Text_OfficerJenny_Family::
    .string "Officer Jenny: So you're Elm! The\n"
    .string "family says you always know.$"

RR_Lunch_Text_P_HowMany::
    .string "{PLAYER}: How many Jennys\n"
    .string "are there, exactly?$"

RR_Lunch_Text_Jenny_Reunion::
    .string "Jenny: The reunion needs a\n"
    .string "stadium. Name tags are mandatory.$"

RR_Lunch_Text_OfficerJenny_Patrol::
    .string "Officer Jenny: I'm on patrol. Some\n"
    .string "Pokémon thief keeps sneaking up\l"
    .string "on Pokémon Centers.$"

RR_Lunch_Text_Jenny_Protocol::
    .string "Jenny: By protocol, I'll keep my\n"
    .string "eyes open. Off duty. Mostly.$"

RR_Lunch_Text_OfficerJenny_Bye::
    .string "Officer Jenny: Say hi to Aunt\n"
    .string "Jenny! And Aunt Jenny!$"

RR_Lunch_Text_Elm_SeventeenCousins::
    .string "Elm: I've met seventeen cousins.\n"
    .string "I can tell three of them apart.$"

RR_Lunch_Text_Jenny_Eighteen::
    .string "Jenny: That was eighteen, dear.$"

RR_Lunch_Text_Picnic::
    .string "A picnic blanket outside the\n"
    .string "Pokémon Center.\p"
    .string "Jenny packed lunch.\n"
    .string "Of course she did.$"

RR_Lunch_Text_Jenny_Labeled::
    .string "Jenny: Sandwiches are labeled.\n"
    .string "Crusts left, crustless right.$"

RR_Lunch_Text_Elm_Dessert::
    .string "Elm: I was in charge of dessert.\n"
    .string "Then I ate dessert. On the walk\l"
    .string "here.$"

RR_Lunch_Text_Ivy_ForYourself::
    .string "Ivy: So you bought dessert for\n"
    .string "yourself.$"

RR_Lunch_Text_Elm_InSpirit::
    .string "Elm: I bought it in spirit.$"

RR_Lunch_Text_P_Apology::
    .string "{PLAYER}: In spirit, I accept your\n"
    .string "apology.$"

RR_Lunch_Text_Ivy_Krabby::
    .string "Ivy: Remember the Orange Islands\n"
    .string "picnic? The Krabby incident?$"

RR_Lunch_Text_Jenny_Report::
    .string "Jenny: I still have the report.\n"
    .string "Four pages. Two are diagrams.$"

RR_Lunch_Text_OldManAwake::
    .string "Up north, the old man has finally\n"
    .string "finished his nap. The road to\l"
    .string "Viridian Forest is open.$"

RR_Lunch_Text_Ivy_Forest::
    .string "Ivy: Viridian Forest, sweetie.\n"
    .string "Bring bug spray. And patience.$"

RR_Lunch_Text_P_Patience::
    .string "{PLAYER}: I am the patience.$"
