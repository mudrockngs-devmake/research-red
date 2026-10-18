@ ===================================================================
@ Research Red: Ivy, the player's wife, follows the player from the end
@ of breakfast on. Macros: rr_ivy_to_scene / rr_ivy_follow /
@ rr_ivy_create_follower (asm/macros/event.inc).
@ Placeholder lines until the owner writes her follower dialogue.
@ ===================================================================
RR_EventScript_IvyTalk::
	lock
	faceplayer
	textcolor NPC_TEXT_COLOR_NEUTRAL
	random 3
	switch VAR_RESULT
	case 0, RR_EventScript_IvyTalk0
	case 1, RR_EventScript_IvyTalk1
	msgbox RR_Text_IvyTalk2, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk0::
	msgbox RR_Text_IvyTalk0, MSGBOX_DEFAULT
	release
	end

RR_EventScript_IvyTalk1::
	msgbox RR_Text_IvyTalk1, MSGBOX_DEFAULT
	release
	end

RR_Text_IvyTalk0:
	.string "Ivy: Field notes are up to date,\n"
	.string "sweetie. Lead the way.$"

RR_Text_IvyTalk1:
	.string "Ivy: Regional forms can start\n"
	.string "with one stubborn ancestor.\p"
	.string "Science is so romantic.$"

RR_Text_IvyTalk2:
	.string "Ivy: If Elm wins this bet, we're\n"
	.string "moving to Alola. No forwarding\l"
	.string "address.$"
