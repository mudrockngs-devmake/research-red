@ Research Red (#72): a lab aide in Oak's Lab explains the research tools.
@ Talk to the aide by the bookshelves (3,11); pick a topic, as many as you
@ like. Placeholder lines for the owner to rework.
RR_Lab_EventScript_FeatureAide::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_LabAide_Text_Hello, MSGBOX_DEFAULT
RR_Lab_EventScript_FeatureAideMenu::
	message RR_LabAide_Text_Which
	waitmessage
	dynmultichoice 16, 0, FALSE, 6, 0, DYN_MULTICHOICE_CB_NONE, RR_LabAide_Text_Pager, RR_LabAide_Text_PC, RR_LabAide_Text_DexNav, RR_LabAide_Text_Guide, RR_LabAide_Text_Rules, RR_LabAide_Text_Evolve, RR_LabAide_Text_TMs, RR_LabAide_Text_Rest, RR_LabAide_Text_Rematch, RR_LabAide_Text_Ball, RR_LabAide_Text_Done
	closemessage
	switch VAR_RESULT
	case 0, RR_Lab_EventScript_AidePager
	case 1, RR_Lab_EventScript_AidePC
	case 2, RR_Lab_EventScript_AideDexNav
	case 3, RR_Lab_EventScript_AideGuide
	case 4, RR_Lab_EventScript_AideRules
	case 5, RR_Lab_EventScript_AideEvolve
	case 6, RR_Lab_EventScript_AideTMs
	case 7, RR_Lab_EventScript_AideRest
	case 8, RR_Lab_EventScript_AideRematch
	case 9, RR_Lab_EventScript_AideBall
	msgbox RR_LabAide_Text_Bye, MSGBOX_DEFAULT
	release
	end

RR_Lab_EventScript_AidePager::
	msgbox RR_LabAide_Text_AboutPager, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_FeatureAideMenu

RR_Lab_EventScript_AidePC::
	msgbox RR_LabAide_Text_AboutPC, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_FeatureAideMenu

RR_Lab_EventScript_AideDexNav::
	msgbox RR_LabAide_Text_AboutDexNav, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_FeatureAideMenu

RR_Lab_EventScript_AideGuide::
	msgbox RR_LabAide_Text_AboutGuide, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_FeatureAideMenu

RR_Lab_EventScript_AideRules::
	msgbox RR_LabAide_Text_AboutRules, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_FeatureAideMenu

RR_Lab_EventScript_AideEvolve::
	msgbox RR_LabAide_Text_AboutEvolve, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_FeatureAideMenu

RR_Lab_EventScript_AideTMs::
	msgbox RR_LabAide_Text_AboutTMs, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_FeatureAideMenu

RR_Lab_EventScript_AideRest::
	msgbox RR_LabAide_Text_AboutRest, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_FeatureAideMenu

RR_Lab_EventScript_AideRematch::
	msgbox RR_LabAide_Text_AboutRematch, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_FeatureAideMenu

@ #97: move any party Pokémon into the Poké Ball type you like (free)
RR_Lab_EventScript_AideBall::
	msgbox RR_LabAide_Text_BallWhich, MSGBOX_DEFAULT
	special ChoosePartyMon
	waitstate
	goto_if_ge VAR_0x8004, PARTY_SIZE, RR_Lab_EventScript_FeatureAideMenu
	copyvar VAR_0x8005, VAR_0x8004
	message RR_LabAide_Text_BallPick
	waitmessage
	dynmultichoice 16, 0, TRUE, 6, 0, DYN_MULTICHOICE_CB_NONE, RR_LabAide_Text_BallPoke, RR_LabAide_Text_BallGreat, RR_LabAide_Text_BallUltra, RR_LabAide_Text_BallPremier, RR_LabAide_Text_BallHeal, RR_LabAide_Text_BallNet, RR_LabAide_Text_BallNest, RR_LabAide_Text_BallDive, RR_LabAide_Text_BallDusk, RR_LabAide_Text_BallTimer, RR_LabAide_Text_BallQuick, RR_LabAide_Text_BallRepeat, RR_LabAide_Text_BallLuxury, RR_LabAide_Text_BallMaster, RR_LabAide_Text_BallFast, RR_LabAide_Text_BallLevel, RR_LabAide_Text_BallLure, RR_LabAide_Text_BallHeavy, RR_LabAide_Text_BallLove, RR_LabAide_Text_BallFriend, RR_LabAide_Text_BallMoon, RR_LabAide_Text_BallSport, RR_LabAide_Text_BallDream, RR_LabAide_Text_BallBeast
	closemessage
	goto_if_ge VAR_RESULT, 24, RR_Lab_EventScript_FeatureAideMenu
	callnative RR_ScriptSetMonBall
	playse SE_BALL
	msgbox RR_LabAide_Text_BallDone, MSGBOX_DEFAULT
	goto RR_Lab_EventScript_FeatureAideMenu

RR_LabAide_Text_Hello:
	.string "Aide: Hello, Professor! I keep the\n"
	.string "lab's research tools in order.\p"
	.string "Ask me about any of them, as often\n"
	.string "as you like.$"

RR_LabAide_Text_Which:
	.string "Aide: Which one?$"

RR_LabAide_Text_Pager:
	.string "Ranch Pager$"

RR_LabAide_Text_PC:
	.string "PC anywhere$"

RR_LabAide_Text_DexNav:
	.string "DexNav$"

RR_LabAide_Text_Guide:
	.string "Field Guide$"

RR_LabAide_Text_Rules:
	.string "Research Rules$"

RR_LabAide_Text_Evolve:
	.string "Evolving$"

RR_LabAide_Text_TMs:
	.string "TMs$"

RR_LabAide_Text_Rest:
	.string "Resting$"

RR_LabAide_Text_Rematch:
	.string "Rematches$"

RR_LabAide_Text_Done:
	.string "That's all$"

RR_LabAide_Text_AboutPager:
	.string "Aide: The Ranch Pager calls the\n"
	.string "Professor's ranch Pokémon.\p"
	.string "Use it from the Bag, or register\n"
	.string "it for SELECT. Pidgeot Flies,\l"
	.string "Pikachu Flashes, Abra Teleports,\l"
	.string "and Diglett Digs.\p"
	.string "Trees, water and boulders: just\n"
	.string "press A. If nobody on your team\l"
	.string "knows the move, his Pokémon help.$"

RR_LabAide_Text_AboutPC:
	.string "Aide: The PC is on the Start menu\n"
	.string "wherever you go: Pokémon storage\l"
	.string "and your item box.$"

RR_LabAide_Text_AboutDexNav:
	.string "Aide: Officer Jenny gives you the\n"
	.string "DexNav after Route 22.\p"
	.string "Start > DexNav shows what lives\n"
	.string "nearby. Pick one you've seen and\l"
	.string "sneak up on it, slowly!$"

RR_LabAide_Text_AboutGuide:
	.string "Aide: The Field Guide is on the\n"
	.string "Start menu, under the Pokédex.\p"
	.string "It lists what lives in the area,\n"
	.string "by day and by night.$"

RR_LabAide_Text_AboutRules:
	.string "Aide: Elm's Research Rules are in\n"
	.string "Options: level caps, doubles or\l"
	.string "singles, healing after battles.\p"
	.string "Change one, and Elm and Jenny\n"
	.string "will want to hear about it!$"

RR_LabAide_Text_AboutEvolve:
	.string "Aide: Pokémon evolve when you say\n"
	.string "so: look for EVOLVE in the party\l"
	.string "menu once they're ready.\p"
	.string "Trade evolutions need no trade:\n"
	.string "Lv. 37, or the right item from the\l"
	.string "Bag. LEARN MOVES also has every\l"
	.string "move of their earlier forms.$"

RR_LabAide_Text_AboutTMs:
	.string "Aide: The grant came with all 100\n"
	.string "TMs, and they never run out.\p"
	.string "Moves LEARN MOVES teaches for free\n"
	.string "aren't TMs, so the TM slots go\l"
	.string "to moves you can't get elsewhere.$"

RR_LabAide_Text_AboutRest:
	.string "Aide: Nurse Joy and Delia let you\n"
	.string "rest: until night if it's day,\l"
	.string "until morning if it's night.\p"
	.string "Night starts at 8 PM. Some Pokémon\n"
	.string "only come out then!$"

RR_LabAide_Text_AboutRematch:
	.string "Aide: After Route 22, Prof. Oak\n"
	.string "and Daisy battle you here, and\l"
	.string "Delia at home.\p"
	.string "Brock rematches you in his Gym at\n"
	.string "the level you choose. Their teams\l"
	.string "grow with your badges!$"

RR_LabAide_Text_Bye:
	.string "Aide: Good luck with the research,\n"
	.string "Professor!$"

RR_LabAide_Text_Ball:
	.string "Change a Ball$"

RR_LabAide_Text_BallWhich:
	.string "Aide: A new Poké Ball? Sure! It's\n"
	.string "quick and painless. Which Pokémon?$"

RR_LabAide_Text_BallPick:
	.string "Aide: And which Poké Ball?$"

RR_LabAide_Text_BallDone:
	.string "Aide: All done! {STR_VAR_1} is in\n"
	.string "its new {STR_VAR_2}.$"

RR_LabAide_Text_BallPoke:
	.string "Poké Ball$"

RR_LabAide_Text_BallGreat:
	.string "Great Ball$"

RR_LabAide_Text_BallUltra:
	.string "Ultra Ball$"

RR_LabAide_Text_BallPremier:
	.string "Premier Ball$"

RR_LabAide_Text_BallHeal:
	.string "Heal Ball$"

RR_LabAide_Text_BallNet:
	.string "Net Ball$"

RR_LabAide_Text_BallNest:
	.string "Nest Ball$"

RR_LabAide_Text_BallDive:
	.string "Dive Ball$"

RR_LabAide_Text_BallDusk:
	.string "Dusk Ball$"

RR_LabAide_Text_BallTimer:
	.string "Timer Ball$"

RR_LabAide_Text_BallQuick:
	.string "Quick Ball$"

RR_LabAide_Text_BallRepeat:
	.string "Repeat Ball$"

RR_LabAide_Text_BallLuxury:
	.string "Luxury Ball$"

RR_LabAide_Text_BallMaster:
	.string "Master Ball$"

RR_LabAide_Text_BallFast:
	.string "Fast Ball$"

RR_LabAide_Text_BallLevel:
	.string "Level Ball$"

RR_LabAide_Text_BallLure:
	.string "Lure Ball$"

RR_LabAide_Text_BallHeavy:
	.string "Heavy Ball$"

RR_LabAide_Text_BallLove:
	.string "Love Ball$"

RR_LabAide_Text_BallFriend:
	.string "Friend Ball$"

RR_LabAide_Text_BallMoon:
	.string "Moon Ball$"

RR_LabAide_Text_BallSport:
	.string "Sport Ball$"

RR_LabAide_Text_BallDream:
	.string "Dream Ball$"

RR_LabAide_Text_BallBeast:
	.string "Beast Ball$"
