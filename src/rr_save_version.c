// Research Red: save layout version check.
//
// New games store RR_SAVE_VERSION (include/rr_save_version.h). When a save is
// continued and its version differs, the player gets one message back on the
// map suggesting a new game, and the save is marked current so it isn't shown
// again. Saves from before v0.0.14 have no version (0) and get the message.
#include "global.h"
#include "event_data.h"
#include "event_scripts.h"
#include "item.h"
#include "constants/items.h"
#include "rr_save_version.h"
#include "script.h"

static EWRAM_DATA bool8 sWarningPending = FALSE;

void RR_CheckSaveVersion(void)
{
    if (gSaveBlock3Ptr->rrSaveVersion != RR_SAVE_VERSION)
    {
        sWarningPending = TRUE;
        gSaveBlock3Ptr->rrSaveVersion = RR_SAVE_VERSION;
    }
}

bool32 RR_TryStartSaveVersionWarning(void)
{
    if (sWarningPending)
    {
        sWarningPending = FALSE;
        ScriptContext_SetupScript(RR_EventScript_OldSaveWarning);
        return TRUE;
    }
    // #57: a save that got the research grant before TM51-100 existed gets
    // them once (the grant also set FLAG_RR_EXP_SHARE)
    if (FlagGet(FLAG_RR_EXP_SHARE) && !CheckBagHasItem(ITEM_TM51, 1) && !CheckPCHasItem(ITEM_TM51, 1))
    {
        ScriptContext_SetupScript(RR_EventScript_TopUpTMs);
        return TRUE;
    }
    // #29: the same for the Ranch Pager
    if (FlagGet(FLAG_RR_EXP_SHARE) && !CheckBagHasItem(ITEM_RANCH_PAGER, 1))
    {
        ScriptContext_SetupScript(RR_EventScript_TopUpRanchPager);
        return TRUE;
    }
    return FALSE;
}
