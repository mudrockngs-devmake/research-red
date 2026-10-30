@ Research Red (#69, #142): watching Elm and Jenny take on Brock, a scripted
@ fight on the battle screen. Spec: docs/scenes/pewter_brock_watch.md.
@ After the Pewter rematch Elm asks if you want to watch (rr_pewter.inc):
@ Yes warps you into the Gym, No leaves it waiting; either way it plays when
@ you enter the Gym with VAR_RR_BROCK_WATCH at 1. Brock sends two at a time
@ in three rounds; the lines are in RR_FIGHT_BROCK_WATCH (src/rr_scripted_fight.c).

@ The scene's people and Pokémon stay hidden unless it is playing
RR_BrockWatch_OnTransition::
	setflag FLAG_TEMP_11
	setflag FLAG_TEMP_12
	setflag FLAG_TEMP_13
	goto_if_set FLAG_DEFEATED_BROCK, RR_BrockWatch_OnTransition_End
	@ before you've beaten Brock, Elm and Jenny watch from the side, by the door
	clearflag FLAG_TEMP_11
	clearflag FLAG_TEMP_12
	setobjectxyperm LOCALID_PEWTER_GYM_ELM, 2, 13
	setobjectxyperm LOCALID_PEWTER_GYM_JENNY, 3, 13
RR_BrockWatch_OnTransition_End:
	return

RR_BrockWatch_OnFrame::
	map_script_2 VAR_RR_BROCK_WATCH, 1, RR_BrockWatch_EventScript_Scene
	.2byte 0

RR_BrockWatch_EventScript_Scene::
	lockall
	@ you and Ivy take the side of the field, Elm and Jenny walk in
	rr_ivy_to_scene LOCALID_PEWTER_GYM_IVY
	rr_walk_to LOCALID_PLAYER, 2, 9, FALSE, 0
	rr_walk_to LOCALID_PEWTER_GYM_IVY, 2, 10, FALSE, 16
	waitmovement LOCALID_PEWTER_GYM_IVY
	waitmovement LOCALID_PLAYER
	applymovement LOCALID_PLAYER, RR_BrockWatch_Movement_FaceRight
	applymovement LOCALID_PEWTER_GYM_IVY, RR_BrockWatch_Movement_FaceRight
	waitmovement 0
	setobjectxy LOCALID_PEWTER_GYM_ELM, 5, 13
	setobjectxy LOCALID_PEWTER_GYM_JENNY, 7, 13
	addobject LOCALID_PEWTER_GYM_ELM
	addobject LOCALID_PEWTER_GYM_JENNY
	applymovement LOCALID_PEWTER_GYM_ELM, RR_BrockWatch_Movement_Up3
	applymovement LOCALID_PEWTER_GYM_JENNY, RR_BrockWatch_Movement_Up3
	waitmovement LOCALID_PEWTER_GYM_JENNY
	textcolor NPC_TEXT_COLOR_NEUTRAL
	closemessage
	@ Elm and Jenny's first picks; the rest of the fight is in rr_scripted_fight.c
	callnative RR_ScriptBrockWatchPair
	setvar VAR_0x8006, RR_FIGHT_BROCK_WATCH
	callnative RR_ScriptStartScriptedFight
	trainerbattle_no_intro TRAINER_RR_BROCK_WATCH, RR_BrockWatch_Text_Brock_Defeat
	callnative RR_ScriptEndScriptedFight
	@ --- the badge ---
	playfanfare MUS_OBTAIN_BADGE
	msgbox RR_BrockWatch_Text_Brock_Badge, MSGBOX_DEFAULT
	waitfanfare
	msgbox RR_BrockWatch_Text_Elm_Allergies, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Brock_Ivy, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Ivy_Thanks, MSGBOX_DEFAULT
	closemessage
	@ --- the four of you by the Gym door ---
	fadescreen FADE_TO_BLACK
	setobjectxy LOCALID_PEWTER_GYM_ELM, 5, 12
	setobjectxy LOCALID_PEWTER_GYM_JENNY, 7, 12
	turnobject LOCALID_PEWTER_GYM_ELM, DIR_SOUTH
	turnobject LOCALID_PEWTER_GYM_JENNY, DIR_SOUTH
	setobjectxy LOCALID_PLAYER, 5, 13
	setobjectxy LOCALID_PEWTER_GYM_IVY, 7, 13
	turnobject LOCALID_PLAYER, DIR_NORTH
	turnobject LOCALID_PEWTER_GYM_IVY, DIR_NORTH
	special DrawWholeMapView
	fadescreen FADE_FROM_BLACK
	msgbox RR_BrockWatch_Text_Elm_RaceOn, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Jenny_Dinner, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_P_Menu, MSGBOX_DEFAULT
	msgbox RR_BrockWatch_Text_Ivy_Sweetie, MSGBOX_DEFAULT
	closemessage
	@ Elm and Jenny head out first; Ivy follows you again
	applymovement LOCALID_PLAYER, RR_BrockWatch_Movement_StepAsideLeft
	applymovement LOCALID_PEWTER_GYM_IVY, RR_BrockWatch_Movement_StepAsideRight
	waitmovement 0
	rr_move_pair LOCALID_PEWTER_GYM_ELM, LOCALID_PEWTER_GYM_JENNY, RR_BrockWatch_Movement_ElmOut
	waitmovement LOCALID_PEWTER_GYM_JENNY
	removeobject LOCALID_PEWTER_GYM_ELM
	removeobject LOCALID_PEWTER_GYM_JENNY
	playse SE_EXIT
	rr_ivy_follow LOCALID_PEWTER_GYM_IVY
	setvar VAR_RR_BROCK_WATCH, 2
	releaseall
	end

RR_BrockWatch_Movement_FaceRight:
	face_right
	step_end

RR_BrockWatch_Movement_Up3:
	walk_up
	walk_up
	walk_up
	step_end

RR_BrockWatch_Movement_StepAsideLeft:
	walk_left
	face_right
	step_end

RR_BrockWatch_Movement_StepAsideRight:
	walk_right
	face_left
	step_end

RR_BrockWatch_Movement_ElmOut:
	walk_down
	walk_down
	set_invisible
	step_end

RR_BrockWatch_Text_Brock_Defeat:
	.string "Brock: I have never lost to a\n"
	.string "lecture before.$"

RR_BrockWatch_Text_Brock_Badge:
	.string "Brock: Fair and square. The\n"
	.string "Boulder Badge is yours.$"

RR_BrockWatch_Text_Elm_Allergies:
	.string "Elm: I'm not crying! It's\n"
	.string "allergies! This Gym is very…\l"
	.string "rocky!$"

RR_BrockWatch_Text_Brock_Ivy:
	.string "Brock: Professor Ivy, your notes\n"
	.string "on regional forms helped my\l"
	.string "breeding more than I can say.$"

RR_BrockWatch_Text_Ivy_Thanks:
	.string "Ivy: Then they did their job.\n"
	.string "Thank you, Brock.$"

RR_BrockWatch_Text_Elm_RaceOn:
	.string "Elm: One badge each! The race\n"
	.string "is ON!$"

RR_BrockWatch_Text_Jenny_Dinner:
	.string "Jenny: And dinner at Delia's is\n"
	.string "still on the line, dear.$"

RR_BrockWatch_Text_P_Menu:
	.string "{PLAYER}: I've already planned\n"
	.string "the menu. For you to cook.$"

RR_BrockWatch_Text_Ivy_Sweetie:
	.string "Ivy: Sweetie.$"

@ --- Your first Brock battle: spoken lines, so everyone's portrait shows ---
RR_Gym_EventScript_BrockIntro::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Gym_Text_Brock_Welcome, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_P_SoundsLikeHim, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Brock_Team, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_P_Shenanigans, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Brock_Respect, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Ivy_Audience, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Brock_Nine, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Ivy_Nicest, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Elm_Cheer, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Jenny_Cheer, MSGBOX_DEFAULT
	playbgm MUS_ENCOUNTER_GYM_LEADER, FALSE
	msgbox RR_Gym_Text_Brock_Begin, MSGBOX_DEFAULT
	closemessage
	trainerbattle_no_intro TRAINER_LEADER_BROCK, PewterCity_Gym_Text_BrockDefeat
	goto PewterCity_Gym_EventScript_DefeatedBrock

@ Brock's gift: one of the Alolan Geodude he bred from Ivy's notes
RR_Gym_EventScript_BrockGeodude::
	msgbox RR_Gym_Text_Brock_Geodude, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Ivy_Hair, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_P_Hair, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Ivy_IronSand, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_P_Attractive, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Ivy_TakeIt, MSGBOX_DEFAULT
	givemon SPECIES_GEODUDE_ALOLA, 12
	copyvar VAR_0x8000, VAR_RESULT
	setflag FLAG_GOT_TM39_FROM_BROCK
	playfanfare MUS_LEVEL_UP
	message RR_Gym_Text_ReceivedGeodude
	waitmessage
	waitfanfare
	waitbuttonpress
	goto_if_ne VAR_0x8000, MON_GIVEN_TO_PC, RR_Gym_EventScript_BrockStew
	msgbox RR_Gym_Text_GeodudeToPC, MSGBOX_DEFAULT
RR_Gym_EventScript_BrockStew::
	msgbox RR_Gym_Text_Brock_Stew, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Ivy_Lucky, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Brock_Relaxing, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_P_Mimey, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, SPECIES_GEODUDE_ALOLA
	callnative RR_ScriptDebugQueueDexBanter @ its own lines, back on the map
	release
	end

RR_Gym_EventScript_LiamBeaten::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox PewterCity_Gym_Text_LiamPostBattle, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_Ivy_Lunch, MSGBOX_DEFAULT
	msgbox RR_Gym_Text_P_LikeHim, MSGBOX_DEFAULT
	release
	end

RR_Gym_EventScript_ElmCheer::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_set FLAG_DEFEATED_BROCK, RR_Gym_EventScript_ElmAfter
	msgbox RR_Gym_Text_Elm_Talk, MSGBOX_DEFAULT
	release
	end

RR_Gym_EventScript_ElmAfter:
	msgbox RR_Gym_Text_Elm_After, MSGBOX_DEFAULT
	release
	end

RR_Gym_EventScript_JennyCheer::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_set FLAG_DEFEATED_BROCK, RR_Gym_EventScript_JennyAfter
	msgbox RR_Gym_Text_Jenny_Talk, MSGBOX_DEFAULT
	release
	end

RR_Gym_EventScript_JennyAfter:
	msgbox RR_Gym_Text_Jenny_After, MSGBOX_DEFAULT
	release
	end

@ Brock's nine brothers and sisters, behind him
RR_Gym_EventScript_Kid0::
	lock
	faceplayer
	msgbox RR_Gym_Text_Kid0, MSGBOX_DEFAULT
	release
	end

RR_Gym_EventScript_Kid1::
	lock
	faceplayer
	msgbox RR_Gym_Text_Kid1, MSGBOX_DEFAULT
	release
	end

RR_Gym_EventScript_Kid2::
	lock
	faceplayer
	msgbox RR_Gym_Text_Kid2, MSGBOX_DEFAULT
	release
	end

RR_Gym_Text_Brock_Welcome:
	.string "Brock: Welcome to Pewter Gym!\n"
	.string "I am Brock.\p"
	.string "Professor Ivy. I've read all your\n"
	.string "papers. It's an honor.\p"
	.string "You too, Professor {PLAYER}.\p"
	.string "Professor Oak called to explain\n"
	.string "what you two are up to.\p"
	.string "He said not to go easy on you.$"

RR_Gym_Text_P_SoundsLikeHim:
	.string "{PLAYER}: That sounds like him.$"

RR_Gym_Text_Brock_Team:
	.string "Brock: He did say the League is\n"
	.string "interested in what you're working\l"
	.string "on.\p"
	.string "So he told every Gym Leader to\n"
	.string "keep things reasonably fair.\p"
	.string "Two of you, one of me. Reasonably\n"
	.string "fair, apparently.$"

RR_Gym_Text_P_Shenanigans:
	.string "{PLAYER}: Oh, so that's why we can\n"
	.string "do shenanigans like this.$"

RR_Gym_Text_Brock_Respect:
	.string "Brock: I respect it. My Pokémon\n"
	.string "and I are a team too.$"

RR_Gym_Text_Ivy_Audience:
	.string "Ivy: And your audience?$"

RR_Gym_Text_Brock_Nine:
	.string "Brock: My brothers and sisters.\n"
	.string "All nine of them.\p"
	.string "They're here for the battle, and\n"
	.string "for lunch. I made stew.\p"
	.string "After the battles. Win or lose.$"

RR_Gym_Text_Ivy_Nicest:
	.string "Ivy: This is the nicest Gym in\n"
	.string "Kanto.$"

RR_Gym_Text_Elm_Cheer:
	.string "Elm: Go on, {PLAYER}! You've got\n"
	.string "this! Just not by too much!$"

RR_Gym_Text_Jenny_Cheer:
	.string "Jenny: We're cheering for you,\n"
	.string "dear. Both of us. Aren't we, dear?$"

RR_Gym_Text_Brock_Begin:
	.string "Brock: My rock-hard willpower is\n"
	.string "evident even in my Pokémon.\p"
	.string "My Pokémon are all rock hard, and\n"
	.string "have true-grit determination.\p"
	.string "That's right! My Pokémon are all\n"
	.string "the Rock type! Fuhaha!\p"
	.string "You'd challenge me knowing that\n"
	.string "you'll lose? Two professors?\p"
	.string "That's a Trainer's honor. Or poor\n"
	.string "peer review.\p"
	.string "Fine, then! Show me your best!$"

RR_Gym_Text_P_Pun:
	.string "{PLAYER}: …Did you plan that pun?$"

RR_Gym_Text_Brock_Puns:
	.string "Brock: I plan all my puns.\n"
	.string "They keep me grounded.$"

RR_Gym_Text_Brock_Stew:
	.string "Brock: The Boulder Badge. A\n"
	.string "Geodude. And soon, a bowl of stew.\p"
	.string "Mostly the stew.\p"
	.string "It needs a while yet. Lunch is\n"
	.string "after my next challenge.$"

RR_Gym_Text_Brock_Geodude:
	.string "Brock: Professor Ivy, your notes\n"
	.string "on regional forms got me breeding\l"
	.string "these.\p"
	.string "An Alolan Geodude. Hatched right\n"
	.string "here in Pewter. It's yours.$"

RR_Gym_Text_Ivy_Hair:
	.string "Ivy: Sweetie, look at its hair!$"

RR_Gym_Text_P_Hair:
	.string "{PLAYER}: It has better hair than\n"
	.string "me.$"

RR_Gym_Text_Ivy_IronSand:
	.string "Ivy: That's iron sand. It sticks\n"
	.string "because the Geodude is magnetic.$"

RR_Gym_Text_P_Attractive:
	.string "{PLAYER}: So it's scientifically\n"
	.string "attractive.$"

RR_Gym_Text_Ivy_TakeIt:
	.string "Ivy: Take the Geodude, sweetie.$"

RR_Gym_Text_ReceivedGeodude:
	.string "{PLAYER} received an Alolan Geodude\n"
	.string "from Brock!$"

RR_Gym_Text_GeodudeToPC:
	.string "Your team is full, so it went to\n"
	.string "the PC.$"

RR_Gym_Text_Ivy_Lucky:
	.string "Ivy: Your siblings are lucky to\n"
	.string "have a big brother like you.$"

RR_Gym_Text_Brock_Relaxing:
	.string "Brock: Nine siblings. I cook a\n"
	.string "lot. Battling is the relaxing\l"
	.string "part.$"

RR_Gym_Text_P_Mimey:
	.string "{PLAYER}: We know someone like\n"
	.string "that. His name is Mimey.$"

RR_Gym_Text_Ivy_Lunch:
	.string "Ivy: Brock makes lunch for\n"
	.string "challengers?$"

RR_Gym_Text_P_LikeHim:
	.string "{PLAYER}: I like him already.$"

RR_Gym_Text_Elm_Talk:
	.string "Elm: We came to watch! And to\n"
	.string "take notes on his weak points!$"

RR_Gym_Text_Elm_After:
	.string "Elm: One badge doesn't win a\n"
	.string "race! See you outside!$"

RR_Gym_Text_Jenny_Talk:
	.string "Jenny: Go on, dear. We'll cheer\n"
	.string "from here. Good seats, too.$"

RR_Gym_Text_Jenny_After:
	.string "Jenny: Well fought, dear.\n"
	.string "Save us some stew.$"

RR_Gym_Text_Kid0:
	.string "Big brother's stew is the best!\n"
	.string "He says you can have some too!$"

RR_Gym_Text_Kid1:
	.string "Shh! We're watching Brock work!\n"
	.string "And waiting for lunch!$"

RR_Gym_Text_Kid2:
	.string "There's nine of us. I'm his\n"
	.string "favorite. Don't tell the others!$"
