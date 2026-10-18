@ Research Red: Start menu > Guide (src/rr_field_guide.c)
RR_EventScript_FieldGuide::
	lockall
	callnative RR_ScriptFieldGuide
	waitstate
	releaseall
	end

@ Research Red: Start menu > Rules (src/rr_rules.c)
RR_EventScript_ResearchRules::
	lockall
	callnative RR_ScriptResearchRules
	waitstate
	releaseall
	end
