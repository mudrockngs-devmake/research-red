#ifndef GUARD_CONSTANTS_RR_CALLS_H
#define GUARD_CONSTANTS_RR_CALLS_H

// Research Red (#13): video calls, for setvar VAR_0x8004 before
// callnative RR_ScriptStartCall. The calls themselves are sCalls in
// src/rr_intro_call.c.
#define RR_CALL_INTRO 0 // the new-game intro (started by the game, not a script)
#define RR_CALL_RULES 1 // a Research Rule changed (#45)
#define RR_CALL_COUNT 2

#endif // GUARD_CONSTANTS_RR_CALLS_H
