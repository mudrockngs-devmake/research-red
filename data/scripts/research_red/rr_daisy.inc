@ Research Red: Daisy's house: maps, Muffin, the bet (PalletTown_RivalsHouse)
@ Moved out of the FireRed map files; the map keeps only its map_script hooks.

@ ===================================================================
@ Research Red: Daisy's house scene (maps, Muffin, Kukui and Burnett, the bet)
@ Elm and Jenny share FLAG_HIDE_TOWN_MAP, so they disappear for good once the
@ Town Map is handed over.
@ ===================================================================
RR_Daisy_EventScript_Scene::
	lockall
	rr_ivy_to_scene LOCALID_RIVALS_HOUSE_IVY
	applymovement LOCALID_PLAYER, RR_Daisy_Movement_Up2
	applymovement LOCALID_RIVALS_HOUSE_IVY, RR_Daisy_Movement_Up2
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_ElmOnChair, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Elm_Help, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_MuffinSits, MSGBOX_DEFAULT
	applymovement LOCALID_DAISY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Daisy_Muffin, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Elm_PersianEyes, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Jenny_ComeDown, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DAISY, Common_Movement_FacePlayer
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_P_Maps, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Daisy_TwoMaps, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Elm_Strategy, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Jenny_Toothbrush, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_DAISY, Common_Movement_WalkInPlaceFasterRight
	waitmovement 0
	removeobject LOCALID_TOWN_MAP
	delay 15
	applymovement LOCALID_DAISY, Common_Movement_FacePlayer
	waitmovement 0
	additem ITEM_TOWN_MAP, 1
	setvar VAR_MAP_SCENE_PALLET_TOWN_RIVALS_HOUSE, 2
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_OBTAIN_KEY_ITEM
	message RR_Daisy_Text_ReceivedMap
	waitmessage
	waitfanfare
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Daisy_TwoMoreProfessors, MSGBOX_DEFAULT
	msgbox RR_Daisy_Text_Ivy_KukuiBurnett, MSGBOX_DEFAULT
	msgbox RR_Daisy_Text_Ivy_Vulpix, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_P_MidVulpix, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Ivy_Professional, MSGBOX_DEFAULT
	closemessage
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_MuffinHops, MSGBOX_DEFAULT
	closemessage
	@ Muffin hops toward Elm's chair, Elm leaps onto the table, Muffin takes the chair
	applymovement LOCALID_RIVALS_HOUSE_MUFFIN, RR_Daisy_Movement_JumpRight
	waitmovement LOCALID_RIVALS_HOUSE_MUFFIN
	applymovement LOCALID_RIVALS_HOUSE_ELM, RR_Daisy_Movement_JumpLeft
	waitmovement LOCALID_RIVALS_HOUSE_ELM
	applymovement LOCALID_RIVALS_HOUSE_MUFFIN, RR_Daisy_Movement_JumpUp
	waitmovement LOCALID_RIVALS_HOUSE_MUFFIN
	playse SE_PIN
	applymovement LOCALID_RIVALS_HOUSE_ELM, Common_Movement_ExclamationMark
	waitmovement 0
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Elm_Choo, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Daisy_Rattata, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Elm_Fluffy, MSGBOX_DEFAULT
	msgbox RR_Daisy_Text_Elm_Terms, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Jenny_SmallStakes, MSGBOX_DEFAULT
	msgbox RR_Daisy_Text_Ivy_Dishes, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_P_Mimey, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Ivy_MimeysDishes, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Elm_Deal, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	playfanfare MUS_LEVEL_UP
	message RR_Daisy_Text_NumbersRegistered
	waitmessage
	waitfanfare
	msgbox RR_Daisy_Text_BetIsOn, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Daisy_Excited, MSGBOX_DEFAULT
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Elm_SoExcited, MSGBOX_DEFAULT
	msgbox RR_Daisy_Text_Elm_SeeYou, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_RIVALS_HOUSE_ELM, RR_Daisy_Movement_ElmExit
	waitmovement 0
	removeobject LOCALID_RIVALS_HOUSE_ELM
	playse SE_EXIT
	applymovement LOCALID_RIVALS_HOUSE_JENNY, RR_Daisy_Movement_JennyExit
	waitmovement 0
	removeobject LOCALID_RIVALS_HOUSE_JENNY
	playse SE_EXIT
	rr_ivy_follow LOCALID_RIVALS_HOUSE_IVY
	setvar VAR_RR_ROUTE1_CATCHES, 0
	setvar VAR_0x40FE, 4
	releaseall
	end

RR_Daisy_Movement_Up2:
	walk_up
	walk_up
	step_end

RR_Daisy_Movement_JumpRight:
	jump_right
	step_end

RR_Daisy_Movement_JumpLeft:
	jump_left
	step_end

RR_Daisy_Movement_JumpUp:
	jump_up
	step_end

@ Elm hops down from the table (7,5) and both leave over the doormat (4,8)
RR_Daisy_Movement_ElmExit:
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

@ Jenny holds the chair from its right side (9,5)
RR_Daisy_Movement_JennyExit:
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_down
	step_end

RR_Daisy_Text_ElmOnChair::
    .string "Elm is standing on a chair.\n"
    .string "Jenny is holding it steady.$"

RR_Daisy_Text_Elm_Help::
    .string "Elm: {PLAYER}! Help!\n"
    .string "It's looking at me!$"

RR_Daisy_Text_MuffinSits::
    .string "A small purple Pokémon sits\n"
    .string "calmly on the rug.$"

RR_Daisy_Text_Daisy_Muffin::
    .string "Daisy: It's a Rattata, Elm.\n"
    .string "Her name is Muffin.$"

RR_Daisy_Text_Elm_PersianEyes::
    .string "Elm: Muffin has the eyes\n"
    .string "of a Persian.$"

RR_Daisy_Text_Jenny_ComeDown::
    .string "Jenny: Muffin has the eyes of a\n"
    .string "Rattata, dear. Come down.$"

RR_Daisy_Text_P_Maps::
    .string "{PLAYER}: Daisy? Your grandpa says\n"
    .string "you have Town Maps for us.$"

RR_Daisy_Text_Daisy_TwoMaps::
    .string "Daisy: I have two. Gary took one.\n"
    .string "Ash borrowed the other one.\p"
    .string "I'm still waiting.\n"
    .string "So: one per couple.$"

RR_Daisy_Text_Elm_Strategy::
    .string "Elm: One per couple?!\n"
    .string "But strategy! Divide and conquer!$"

RR_Daisy_Text_Jenny_Toothbrush::
    .string "Jenny: We share a toothbrush cup,\n"
    .string "dear. We can share a map.$"

RR_Daisy_Text_ReceivedMap::
    .string "{PLAYER} received the Town Map!$"

RR_Daisy_Text_Daisy_TwoMoreProfessors::
    .string "Daisy: Grandpa said two more\n"
    .string "professors were supposed to come.\p"
    .string "A couple from Alola?$"

RR_Daisy_Text_Ivy_KukuiBurnett::
    .string "Ivy: Kukui and Burnett.\n"
    .string "They couldn't get away.\p"
    .string "Something about starting\n"
    .string "a league.$"

RR_Daisy_Text_Ivy_Vulpix::
    .string "Ivy: Burnett once showed me a\n"
    .string "Vulpix from Alola's mountains.\p"
    .string "White. Cold to the touch.\n"
    .string "Same species, different home.$"

RR_Daisy_Text_P_MidVulpix::
    .string "{PLAYER}: That's when she married\n"
    .string "me, by the way. Mid-Vulpix story.$"

RR_Daisy_Text_Ivy_Professional::
    .string "Ivy: I married you after the\n"
    .string "story, sweetie.\p"
    .string "I'm a professional.$"

RR_Daisy_Text_MuffinHops::
    .string "Muffin hops onto the chair.\n"
    .string "Elm hops onto the table.$"

RR_Daisy_Text_Elm_Choo::
    .string "Elm: Ah… ah… choo!\n"
    .string "Allergies. Daisy's Persian.$"

RR_Daisy_Text_Daisy_Rattata::
    .string "Daisy: Muffin. Rattata.$"

RR_Daisy_Text_Elm_Fluffy::
    .string "Elm: …A very fluffy Rattata.$"

RR_Daisy_Text_Elm_Terms::
    .string "Elm: Official terms! First team\n"
    .string "to win all eight Gym badges\l"
    .string "takes the bet!$"

RR_Daisy_Text_Jenny_SmallStakes::
    .string "Jenny: Small stakes.\n"
    .string "In my professional opinion.$"

RR_Daisy_Text_Ivy_Dishes::
    .string "Ivy: Losers do the dishes\n"
    .string "for a month.$"

RR_Daisy_Text_P_Mimey::
    .string "{PLAYER}: Mimey already does\n"
    .string "the dishes.$"

RR_Daisy_Text_Ivy_MimeysDishes::
    .string "Ivy: Then the losers do\n"
    .string "Mimey's dishes.$"

RR_Daisy_Text_Elm_Deal::
    .string "Elm: Deal! We swap numbers,\n"
    .string "and it's official!$"

RR_Daisy_Text_NumbersRegistered::
    .string "Elm's and Jenny's numbers were\n"
    .string "saved!$"

RR_Daisy_Text_BetIsOn::
    .string "The bet is on: first team to\n"
    .string "eight Gym badges.$"

RR_Daisy_Text_Daisy_Excited::
    .string "Daisy: Grandpa is going to pretend\n"
    .string "he's not excited about this.$"

RR_Daisy_Text_Elm_SoExcited::
    .string "Elm: He is so excited about this.$"

RR_Daisy_Text_Elm_SeeYou::
    .string "Elm: See you on Route 1!\n"
    .string "Last one to Viridian buys lunch!$"

@ Daisy waits at the table facing the door when you arrive for the scene
RR_Daisy_EventScript_SetupScene::
	setobjectmovementtype LOCALID_DAISY, MOVEMENT_TYPE_FACE_DOWN
	return

@ Daisy after her scene (#31): her own lines, with her portrait
RR_Daisy_EventScript_Talk::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Talk, MSGBOX_DEFAULT
	release
	end

RR_Daisy_Text_Talk::
    .string "Daisy: Muffin has claimed the\n"
    .string "chair. Elm will have to stand.\p"
    .string "Grandpa's proud of you two, you\n"
    .string "know. He just won't say it.$"

@ Muffin, Daisy's Rattata (Elm insists she is a Persian)
RR_Daisy_EventScript_Muffin::
	lock
	faceplayer
	playmoncry SPECIES_RATTATA, CRY_MODE_NORMAL
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Daisy_Text_Muffin, MSGBOX_DEFAULT
	waitmoncry
	release
	end

RR_Daisy_Text_Muffin::
    .string "Muffin: Rat! Tat tat!\p"
    .string "She looks extremely pleased with\n"
    .string "herself.$"
