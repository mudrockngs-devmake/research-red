#include "global.h"
#include "rr_ranch_pager.h" // Research Red (#29)
#include "field_effect.h"
#include "field_player_avatar.h"
#include "fldeff.h"
#include "item_use.h"
#include "overworld.h"
#include "party_menu.h"

static void FieldCallback_Dig(void);
static void StartDigFieldEffect(void);

bool32 FieldMove_SetUpDig(void)
{
    if (CanUseEscapeRopeOnCurrMap() == TRUE)
    {
        gFieldCallback2 = FieldCallback_PrepareFadeInFromMenu;
        gPostMenuFieldCallback = FieldCallback_Dig;
        return TRUE;
    }
    return FALSE;
}

static void FieldCallback_Dig(void)
{
    Overworld_ResetStateAfterDigEscRope();
    FieldEffectStart(FLDEFF_USE_DIG);
    gFieldEffectArguments[0] = RR_FieldMoveMonArg(); // Research Red (#29): Oak's Pokemon with the Ranch Pager
}

u32 FldEff_UseDig(void)
{
    u8 taskId = CreateFieldEffectShowMon();

    FLDEFF_SET_FUNC_TO_DATA(StartDigFieldEffect);
    SetPlayerAvatarTransitionState(PLAYER_AVATAR_STATE_NORMAL);
    return FALSE;
}

static void StartDigFieldEffect(void)
{
    u8 taskId;

    FieldEffectActiveListRemove(FLDEFF_USE_DIG);
    taskId = CreateTask(Task_UseDigEscapeRopeOnField, 8);
    gTasks[taskId].data[0] = 0;
}
