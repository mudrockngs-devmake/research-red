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
	cleartrainerflag TRAINER_RR_DAISY_REMATCH
	trainerbattle_two_trainers TRAINER_RR_OAK_REMATCH, RR_Rematch_Text_OakDefeat, TRAINER_RR_DAISY_REMATCH, RR_Rematch_Text_DaisyDefeat
	clearflag FLAG_RR_NO_WHITEOUT
	cleartrainerflag TRAINER_RR_OAK_REMATCH
	cleartrainerflag TRAINER_RR_DAISY_REMATCH
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
	.string "Oak: Ho ho! Splendid work!$"

RR_Rematch_Text_DaisyDefeat:
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
