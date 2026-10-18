@ Research Red: lunch in Viridian City (animatic 07)
@ After the Route 22 battle (story progress 6), walking back into Viridian
@ from Route 22 (triggers at x 19, y 16-19) starts it: Elm and Jenny catch up,
@ Jenny runs into her cousin, the Viridian Officer Jenny, then a picnic outside
@ the Pokemon Center and Elm's Research Rules. Afterwards the old man finishes
@ his nap and the road north (Route 2, Viridian Forest) is open.
@ Ivy stays a follower for the whole scene.

@ Viridian ON_TRANSITION: the scene's people only appear during the scene
RR_Lunch_OnTransition::
	setflag FLAG_TEMP_13
	setflag FLAG_TEMP_14
	setflag FLAG_TEMP_15
	return

RR_Lunch_EventScript_Start::
	lockall
	textcolor NPC_TEXT_COLOR_NEUTRAL
	@ Elm and Jenny jog over from the Pokemon Center
	addobject LOCALID_VIRIDIAN_ELM
	addobject LOCALID_VIRIDIAN_JENNY
	applymovement LOCALID_VIRIDIAN_ELM, RR_Lunch_Movement_ElmArrives
	applymovement LOCALID_VIRIDIAN_JENNY, RR_Lunch_Movement_JennyArrives
	waitmovement LOCALID_VIRIDIAN_ELM
	waitmovement LOCALID_VIRIDIAN_JENNY
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	msgbox RR_Lunch_Text_Elm_ThereYouAre, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Jenny_Packed, MSGBOX_DEFAULT
	closemessage
	@ Officer Jenny walks up from the south
	addobject LOCALID_VIRIDIAN_OFFICER_JENNY
	applymovement LOCALID_VIRIDIAN_OFFICER_JENNY, RR_Lunch_Movement_OfficerArrives
	waitmovement 0
	applymovement LOCALID_VIRIDIAN_JENNY, Common_Movement_WalkInPlaceFasterDown
	waitmovement 0
	playse SE_PIN
	applymovement LOCALID_VIRIDIAN_JENNY, Common_Movement_ExclamationMark
	applymovement LOCALID_VIRIDIAN_OFFICER_JENNY, Common_Movement_ExclamationMark
	waitmovement 0
	msgbox RR_Lunch_Text_OfficerJenny_Cousin, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Jenny_Cousin, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Elm_WhichOne, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Ivy_Jeans, MSGBOX_DEFAULT
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
	msgbox RR_Lunch_Text_Elm_Rules, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_P_SitDown, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Elm_Sitting, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Elm_Controls, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Jenny_Problem, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Elm_Passion, MSGBOX_DEFAULT
	closemessage
	callnative RR_ScriptResearchRules
	waitstate
	msgbox RR_Lunch_Text_Elm_CallMe, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Jenny_NotNight, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Elm_Glad, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_P_EveryWeek, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Elm_Quiet, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_P_GladToo, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Ivy_GroupHug, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Jenny_GroupHug, MSGBOX_DEFAULT
	closemessage
	@ Elm and Jenny head north first
	applymovement LOCALID_VIRIDIAN_ELM, RR_Lunch_Movement_Leave
	applymovement LOCALID_VIRIDIAN_JENNY, RR_Lunch_Movement_Leave
	waitmovement LOCALID_VIRIDIAN_ELM
	waitmovement LOCALID_VIRIDIAN_JENNY
	removeobject LOCALID_VIRIDIAN_ELM
	removeobject LOCALID_VIRIDIAN_JENNY
	@ The old man has finished his nap: the road north is open
	setvar VAR_MAP_SCENE_VIRIDIAN_CITY_OLD_MAN, 2
	setvar VAR_0x40FE, 7
	msgbox RR_Lunch_Text_OldManAwake, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_Ivy_Forest, MSGBOX_DEFAULT
	msgbox RR_Lunch_Text_P_Patience, MSGBOX_DEFAULT
	releaseall
	end

@ The player stands at x 19; Elm stops beside them, Jenny just behind
RR_Lunch_Movement_ElmArrives::
	walk_left
	walk_left
	walk_left
	walk_left
	face_left
	step_end

RR_Lunch_Movement_JennyArrives::
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	face_left
	step_end

RR_Lunch_Movement_OfficerArrives::
	walk_up
	walk_up
	walk_up
	face_up
	step_end

RR_Lunch_Movement_OfficerLeaves::
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

RR_Lunch_Movement_Leave::
	walk_right
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

RR_Lunch_Text_Elm_WhichOne::
    .string "Elm: …Ivy. Which one is my wife?$"

RR_Lunch_Text_Ivy_Jeans::
    .string "Ivy: The one in jeans, sweetie.$"

RR_Lunch_Text_OfficerJenny_Family::
    .string "Officer Jenny: So this is the\n"
    .string "famous Elm. The family talks.$"

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
    .string "Elm: I lost the bet, so I bought\n"
    .string "dessert. Then I ate dessert.\l"
    .string "On the walk here.$"

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

RR_Lunch_Text_Elm_Rules::
    .string "Elm: So. {PLAYER}. Before the Gyms,\n"
    .string "I've been thinking about rules.$"

RR_Lunch_Text_P_SitDown::
    .string "{PLAYER}: You've been thinking?\n"
    .string "Should we sit down?$"

RR_Lunch_Text_Elm_Sitting::
    .string "Elm: We are sitting down.$"

RR_Lunch_Text_Elm_Controls::
    .string "Elm: Real research needs controls!\n"
    .string "Level caps before each Gym,\l"
    .string "so nobody overtrains.$"

RR_Lunch_Text_Jenny_Problem::
    .string "Jenny: He means so he doesn't\n"
    .string "overtrain. He has a problem.$"

RR_Lunch_Text_Elm_Passion::
    .string "Elm: I have a passion.$"

RR_Lunch_Text_Elm_CallMe::
    .string "Elm: Want to change them later?\n"
    .string "Start menu, “Rules.” Day or night.$"

RR_Lunch_Text_Jenny_NotNight::
    .string "Jenny: Please not night.$"

RR_Lunch_Text_Elm_Glad::
    .string "Elm: …Hey. I'm really glad you two\n"
    .string "came. Johto was quiet without you.$"

RR_Lunch_Text_P_EveryWeek::
    .string "{PLAYER}: Elm. You called me\n"
    .string "every week.$"

RR_Lunch_Text_Elm_Quiet::
    .string "Elm: And it was quiet between\n"
    .string "the calls!$"

RR_Lunch_Text_P_GladToo::
    .string "{PLAYER}: …I'm glad we came too.$"

RR_Lunch_Text_Ivy_GroupHug::
    .string "Ivy: Group hug, or are we too\n"
    .string "professional?$"

RR_Lunch_Text_Jenny_GroupHug::
    .string "Jenny: In my professional opinion:\n"
    .string "group hug.$"

RR_Lunch_Text_OldManAwake::
    .string "Up north, the old man has finally\n"
    .string "finished his nap. The road to\l"
    .string "Viridian Forest is open.$"

RR_Lunch_Text_Ivy_Forest::
    .string "Ivy: Viridian Forest, sweetie.\n"
    .string "Bring bug spray. And patience.$"

RR_Lunch_Text_P_Patience::
    .string "{PLAYER}: I am the patience.$"
