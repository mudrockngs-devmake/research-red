@ Research Red: Start menu > Guide (src/rr_field_guide.c)
RR_EventScript_FieldGuide::
	lockall
	callnative RR_ScriptFieldGuide
	waitstate
	releaseall
	end

@ Research Red: Start menu > Rules (src/rr_rules.c)
@ Start > PC (#34): the usual PC menu (Pokemon storage, your item storage,
@ Prof. Oak's PC) from anywhere. Turning it off only animates a real PC.
RR_EventScript_StartMenuPC::
	lockall
	playse SE_PC_ON
	goto EventScript_PCMainMenu
	end

RR_EventScript_ResearchRules::
	lockall
	callnative RR_ScriptResearchRules
	waitstate
	releaseall
	end

@ Research Red: shown once when continuing a save from an older layout
@ (src/rr_save_version.c, docs/SAVES.md)
RR_EventScript_OldSaveWarning::
	lockall
	msgbox RR_Text_OldSaveWarning, MSGBOX_DEFAULT
	releaseall
	end

RR_Text_OldSaveWarning::
	.string "This save is from an older test\n"
	.string "build of Research Red.\p"
	.string "It may still work, but if anything\n"
	.string "looks wrong, start a new game.$"
