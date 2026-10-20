@ Research Red: lunch in Viridian City (animatic 07)
@ After the Route 22 battle (story progress 6), Elm and Jenny sit to the right
@ of the Pokemon Center (29-30, 26); talking to either starts it (#43).
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

RR_Lunch_EventScript_Start::
	lockall
	faceplayer
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
	@ --- the picnic ---
	fadescreen FADE_TO_BLACK
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
	fadescreen FADE_FROM_BLACK
	@ The old man has finished his nap: the road north is open
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 2
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
    .string "Elm: There you are! Lunch!\n"
    .string "Jenny made enough for a Snorlax.$"

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
