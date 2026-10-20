#ifndef GUARD_RR_RULES_H
#define GUARD_RR_RULES_H

// Research Red: Elm's Research Rules menu (src/rr_rules.c)
struct ScriptContext;
void RR_ScriptResearchRules(struct ScriptContext *ctx);
bool32 RR_CanOpenRulesFromOptions(void);  // Options opened from the Start menu, after Daisy's house
void RR_FieldCB_OpenResearchRules(void);  // back on the map: show the Rules window

#endif // GUARD_RR_RULES_H
