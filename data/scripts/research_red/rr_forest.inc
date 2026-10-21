@ Research Red (#9): Viridian Forest. After you beat each of the five Bug
@ Catchers, a short exchange between you and Ivy (animatic 09, forest()).
@ The trainers' own scripts are in data/maps/ViridianForest/scripts.inc.

RR_Forest_EventScript_AfterRick::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Forest_Text_Ivy_Snacks, MSGBOX_DEFAULT
	msgbox RR_Forest_Text_P_Snacks, MSGBOX_DEFAULT
	release
	end

RR_Forest_EventScript_AfterDoug::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Forest_Text_Ivy_Patience, MSGBOX_DEFAULT
	msgbox RR_Forest_Text_P_Patient, MSGBOX_DEFAULT
	msgbox RR_Forest_Text_Ivy_KeepWaiting, MSGBOX_DEFAULT
	release
	end

RR_Forest_EventScript_AfterAnthony::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Forest_Text_Ivy_NameOurs, MSGBOX_DEFAULT
	msgbox RR_Forest_Text_P_SquirtleLe, MSGBOX_DEFAULT
	msgbox RR_Forest_Text_Ivy_Regret, MSGBOX_DEFAULT
	release
	end

RR_Forest_EventScript_AfterCharlie::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Forest_Text_P_Relaxing, MSGBOX_DEFAULT
	msgbox RR_Forest_Text_Ivy_Rocks, MSGBOX_DEFAULT
	release
	end

RR_Forest_EventScript_AfterSammy::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Forest_Text_Ivy_PowerPlant, MSGBOX_DEFAULT
	msgbox RR_Forest_Text_P_TellMeMore, MSGBOX_DEFAULT
	release
	end

RR_Forest_Text_Ivy_Snacks:
	.string "Ivy: Bug Catchers always have the\n"
	.string "best snacks. Learned that in the\l"
	.string "field.$"

RR_Forest_Text_P_Snacks:
	.string "{PLAYER}: I learned it in the field\n"
	.string "too. From a Bug Catcher.\l"
	.string "Who wouldn't share.$"

RR_Forest_Text_Ivy_Patience:
	.string "Ivy: Patience is a research\n"
	.string "skill, sweetie.$"

RR_Forest_Text_P_Patient:
	.string "{PLAYER}: I'm very patient. I've\n"
	.string "waited years for you to admit\l"
	.string "I'm funny.$"

RR_Forest_Text_Ivy_KeepWaiting:
	.string "Ivy: Keep waiting.$"

RR_Forest_Text_Ivy_NameOurs:
	.string "Ivy: We should name ours like\n"
	.string "that. Cater and Pie.$"

RR_Forest_Text_P_SquirtleLe:
	.string "{PLAYER}: Squirtle and… Le.$"

RR_Forest_Text_Ivy_Regret:
	.string "Ivy: I regret everything.$"

RR_Forest_Text_P_Relaxing:
	.string "{PLAYER}: Two Metapod using Harden.\n"
	.string "Most relaxing battle of my life.$"

RR_Forest_Text_Ivy_Rocks:
	.string "Ivy: Like watching very\n"
	.string "determined rocks.$"

RR_Forest_Text_Ivy_PowerPlant:
	.string "Ivy: A forest Pikachu. The ones\n"
	.string "by the Power Plant are bigger,\l"
	.string "did you know?$"

RR_Forest_Text_P_TellMeMore:
	.string "{PLAYER}: I didn't. Tell me more\n"
	.string "on the way.$"

@ #9: the Samurai (anime EP004 homage), in the clearing in the middle of the
@ forest (30,23). Pinsir and a Metapod that only knows Harden.
RR_Forest_EventScript_Samurai::
	trainerbattle_single TRAINER_RR_SAMURAI, RR_Forest_Text_SamuraiIntro, RR_Forest_Text_SamuraiDefeat, RR_Forest_EventScript_AfterSamurai
	msgbox RR_Forest_Text_SamuraiAfter, MSGBOX_AUTOCLOSE
	end

RR_Forest_EventScript_AfterSamurai::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Forest_Text_Ivy_Pallet, MSGBOX_DEFAULT
	msgbox RR_Forest_Text_P_Pallet, MSGBOX_DEFAULT
	release
	end

RR_Forest_Text_SamuraiIntro:
	.string "Halt! Are you from Pallet Town?\p"
	.string "…You are?! Then draw your\n"
	.string "Poké Balls!$"

RR_Forest_Text_SamuraiDefeat:
	.string "My Metapod hardened its heart.\n"
	.string "It was not enough.$"

RR_Forest_Text_SamuraiAfter:
	.string "I ask everyone who passes if\n"
	.string "they're from Pallet Town.\p"
	.string "Some day, a trainer from Pallet\n"
	.string "will give me a real duel!$"

RR_Forest_Text_Ivy_Pallet:
	.string "Ivy: Technically we're from\n"
	.string "Pallet this week, sweetie.$"

RR_Forest_Text_P_Pallet:
	.string "{PLAYER}: Technically we're from a\n"
	.string "lab. Spiritually, a picnic.$"
