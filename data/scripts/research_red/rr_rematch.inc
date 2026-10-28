@ Research Red (#53): the Oak + Daisy rematch in Oak's Lab, from the Route 22
@ 6v6 on (story 6+). A double; the teams follow your badges (src/rr_rematch.c).
@ No money, 1.5x EXP, repeat as often as you like. A loss doesn't white out.
@ Placeholder lines for the owner to rework.
RR_Lab_EventScript_OakRematch::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Rematch_Text_OakAsk, MSGBOX_DEFAULT
	msgbox RR_Rematch_Text_OakShallWe, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_Lab_EventScript_OakRematchDeclined
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Rematch_Text_DaisyMuffin, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_RR_NO_WHITEOUT
	cleartrainerflag TRAINER_RR_OAK_REMATCH
	trainerbattle_no_intro TRAINER_RR_OAK_REMATCH, RR_Rematch_Text_OakDefeat
	clearflag FLAG_RR_NO_WHITEOUT
	cleartrainerflag TRAINER_RR_OAK_REMATCH
	special HealPlayerParty
	specialvar VAR_RESULT, GetBattleOutcome
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RR_Lab_EventScript_OakRematchWon
	msgbox RR_Rematch_Text_DaisyWeWon, MSGBOX_DEFAULT
	msgbox RR_Rematch_Text_OakAnytime, MSGBOX_DEFAULT
	release
	end

RR_Lab_EventScript_OakRematchWon::
	msgbox RR_Rematch_Text_OakGrown, MSGBOX_DEFAULT
	release
	end

RR_Lab_EventScript_OakRematchDeclined::
	msgbox RR_Rematch_Text_OakDeclined, MSGBOX_DEFAULT
	release
	end

RR_Lab_EventScript_DaisyRematch::
	lock
	faceplayer
	goto RR_Lab_EventScript_OakRematch

@ Daisy waits by Oak only once the rematch is open (FLAG_TEMP_4 hides her)
RR_Lab_EventScript_HideDaisy::
	setflag FLAG_TEMP_4
	return

RR_Rematch_Text_OakAsk:
	.string "Oak: {PLAYER}! Daisy dropped by\n"
	.string "with Muffin. Fancy a battle?\p"
	.string "Oak: No prize money, mind. You've\n"
	.string "had the grant! But plenty of EXP.$"

RR_Rematch_Text_OakShallWe:
	.string "Oak: Shall we?$"

RR_Rematch_Text_DaisyMuffin:
	.string "Daisy: Muffin's been training.\n"
	.string "She's not evolving. She's perfect.$"

RR_Rematch_Text_OakDefeat:
	.string "Oak: Ho ho! Splendid! A haiku:\p"
	.string "“Bright young researcher,\n"
	.string "my Slowpoke still hasn't noticed\l"
	.string "the battle is done.”\p"
	.string "Daisy: Oh, Muffin. You were still\n"
	.string "the cutest one out there.$"


RR_Rematch_Text_OakGrown:
	.string "Oak: You've grown, {PLAYER}. My\n"
	.string "team will have to grow too!$"

RR_Rematch_Text_DaisyWeWon:
	.string "Daisy: We won! Muffin, did you\n"
	.string "hear that? We won!$"

RR_Rematch_Text_OakAnytime:
	.string "Oak: Ho ho! Come back anytime.$"

RR_Rematch_Text_OakDeclined:
	.string "Oak: Anytime! The lab door is\n"
	.string "always open.$"

@ Research Red (#70): Delia offers a battle before Heal / Rest (rr_rest.inc).
@ A double on her own; the team follows your badges (src/rr_rematch.c).
RR_EventScript_DeliaBattleAsk::
	textcolor NPC_TEXT_COLOR_NEUTRAL
	msgbox RR_Rematch_Text_DeliaAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RR_EventScript_DeliaHealOrRestMenu
	msgbox RR_Rematch_Text_DeliaApronOff, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_RR_NO_WHITEOUT
	cleartrainerflag TRAINER_RR_DELIA_REMATCH
	trainerbattle_no_intro TRAINER_RR_DELIA_REMATCH, RR_Rematch_Text_DeliaDefeat
	clearflag FLAG_RR_NO_WHITEOUT
	cleartrainerflag TRAINER_RR_DELIA_REMATCH
	special HealPlayerParty
	specialvar VAR_RESULT, GetBattleOutcome
	textcolor NPC_TEXT_COLOR_NEUTRAL
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RR_EventScript_DeliaBattleWon
	msgbox RR_Rematch_Text_DeliaMomWins, MSGBOX_DEFAULT
	release
	end

RR_EventScript_DeliaBattleWon::
	msgbox RR_Rematch_Text_DeliaProud, MSGBOX_DEFAULT
	release
	end

RR_Rematch_Text_DeliaAsk:
	.string "Delia: {PLAYER}! Mimey's been\n"
	.string "itching for a battle. Up for one?$"

RR_Rematch_Text_DeliaApronOff:
	.string "Delia: Mimey, apron off!\n"
	.string "Everyone, out to play!$"

RR_Rematch_Text_DeliaDefeat:
	.string "Delia: Oh, well done, sweetie!$"

RR_Rematch_Text_DeliaProud:
	.string "Delia: You've gotten so strong.\n"
	.string "I'm so proud of you.$"

RR_Rematch_Text_DeliaMomWins:
	.string "Delia: Mom still has it! Now,\n"
	.string "who wants pancakes?$"

@ Research Red (#52): Gym leader rematches, Brock so far. After his badge,
@ talking to him offers a rematch at any tier up to your badges (and the
@ post-game tier after the League); his Gym team grows (src/rr_rematch.c).
@ Prize money as usual, 1.5x EXP, no whiteout. Placeholder lines.
RR_Gym_EventScript_BrockRematch::
	msgbox RR_Rematch_Text_BrockAsk, MSGBOX_DEFAULT
	callnative RR_ScriptGetRematchTier
	copyvar VAR_0x8005, VAR_RESULT
	message RR_Rematch_Text_BrockWhich
	waitmessage
	call_if_ge VAR_0x8005, 1, RR_Gym_EventScript_PushTier1
	call_if_ge VAR_0x8005, 2, RR_Gym_EventScript_PushTier2
	call_if_ge VAR_0x8005, 3, RR_Gym_EventScript_PushTier3
	call_if_ge VAR_0x8005, 4, RR_Gym_EventScript_PushTier4
	call_if_ge VAR_0x8005, 5, RR_Gym_EventScript_PushTier5
	call_if_ge VAR_0x8005, 6, RR_Gym_EventScript_PushTier6
	call_if_ge VAR_0x8005, 7, RR_Gym_EventScript_PushTier7
	call_if_ge VAR_0x8005, 8, RR_Gym_EventScript_PushTier8
	call_if_ge VAR_0x8005, 9, RR_Gym_EventScript_PushTier9
	dynmultipush RR_Rematch_Text_TierCancel, 127
	dynmultistack 16, 0, FALSE, 6, FALSE, 0, DYN_MULTICHOICE_CB_NONE
	closemessage
	goto_if_ge VAR_RESULT, 10, RR_Gym_EventScript_BrockRematchDeclined
	callnative RR_ScriptSetChosenTier
	msgbox RR_Rematch_Text_BrockGo, MSGBOX_DEFAULT
	closemessage
	setflag FLAG_RR_NO_WHITEOUT
	cleartrainerflag TRAINER_RR_BROCK_REMATCH
	trainerbattle_no_intro TRAINER_RR_BROCK_REMATCH, RR_Rematch_Text_BrockDefeat
	clearflag FLAG_RR_NO_WHITEOUT
	cleartrainerflag TRAINER_RR_BROCK_REMATCH
	special HealPlayerParty
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, RR_Gym_EventScript_BrockRematchWon
	msgbox RR_Rematch_Text_BrockWon, MSGBOX_DEFAULT
	release
	end

RR_Gym_EventScript_BrockRematchWon::
	msgbox RR_Rematch_Text_BrockLost, MSGBOX_DEFAULT
	release
	end

RR_Gym_EventScript_BrockRematchDeclined::
	msgbox RR_Rematch_Text_BrockAnytime, MSGBOX_DEFAULT
	release
	end

RR_Gym_EventScript_PushTier1::
	dynmultipush RR_Rematch_Text_Tier1, 1
	return

RR_Gym_EventScript_PushTier2::
	dynmultipush RR_Rematch_Text_Tier2, 2
	return

RR_Gym_EventScript_PushTier3::
	dynmultipush RR_Rematch_Text_Tier3, 3
	return

RR_Gym_EventScript_PushTier4::
	dynmultipush RR_Rematch_Text_Tier4, 4
	return

RR_Gym_EventScript_PushTier5::
	dynmultipush RR_Rematch_Text_Tier5, 5
	return

RR_Gym_EventScript_PushTier6::
	dynmultipush RR_Rematch_Text_Tier6, 6
	return

RR_Gym_EventScript_PushTier7::
	dynmultipush RR_Rematch_Text_Tier7, 7
	return

RR_Gym_EventScript_PushTier8::
	dynmultipush RR_Rematch_Text_Tier8, 8
	return

RR_Gym_EventScript_PushTier9::
	dynmultipush RR_Rematch_Text_Tier9, 9
	return

RR_Rematch_Text_Tier1:
	.string "1 badge: Lv. 21$"

RR_Rematch_Text_Tier2:
	.string "2 badges: Lv. 24$"

RR_Rematch_Text_Tier3:
	.string "3 badges: Lv. 29$"

RR_Rematch_Text_Tier4:
	.string "4 badges: Lv. 43$"

RR_Rematch_Text_Tier5:
	.string "5 badges: Lv. 43$"

RR_Rematch_Text_Tier6:
	.string "6 badges: Lv. 47$"

RR_Rematch_Text_Tier7:
	.string "7 badges: Lv. 50$"

RR_Rematch_Text_Tier8:
	.string "8 badges: Lv. 63$"

RR_Rematch_Text_Tier9:
	.string "Champion: Lv. 75$"

RR_Rematch_Text_TierCancel:
	.string "Cancel$"

RR_Rematch_Text_BrockAsk:
	.string "Brock: You already have my Boulder\n"
	.string "Badge. Here for a rematch?\p"
	.string "Brock: My Pokémon have been\n"
	.string "training. Pick how hard I go.$"

RR_Rematch_Text_BrockWhich:
	.string "Brock: Which challenge?$"

RR_Rematch_Text_BrockGo:
	.string "Brock: Rock-hard defense.\n"
	.string "Let's see you crack it!$"

RR_Rematch_Text_BrockDefeat:
	.string "Brock: Cracked again!\n"
	.string "You really have grown.$"

RR_Rematch_Text_BrockLost:
	.string "Brock: Come back anytime. My\n"
	.string "team keeps training too.$"

RR_Rematch_Text_BrockWon:
	.string "Brock: Solid as a rock! Train up\n"
	.string "and try again.$"

RR_Rematch_Text_BrockAnytime:
	.string "Brock: Anytime you're ready.$"
