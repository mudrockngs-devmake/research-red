// Research Red: the Ranch Pager (#29).
//
// A key item that calls one of Oak's Pokemon for a field move, so the team
// never needs a flyer. Its menu (data/scripts/research_red/rr_ranch_pager.inc)
// offers the field moves that aren't used by pressing A on something:
// Fly, Flash, Teleport and Dig. Cut, Surf, Strength and the rest already call
// Oak's Pokemon when nobody in the team knows the move (OW_FIELD_MOVES_WITHOUT_HMS).
//
// Oak's Pokemon for each move are the defaultSpecies in src/field_move.c.
// The Pager reuses the party menu's field-move code: it runs the move's
// setUpFunc, then returns to the map (or opens the Fly map) the same way the
// party menu does. The engine spots that the Pager is in use through
// RR_FieldMoveMonArg(), which shows Oak's Pokemon instead of a party member.
#include "global.h"
#include "event_data.h"
#include "field_move.h"
#include "main.h"
#include "overworld.h"
#include "party_menu.h"
#include "region_map.h"
#include "rr_ranch_pager.h"
#include "script.h"
#include "string_util.h"
#include "constants/field_effects.h"

static EWRAM_DATA u16 sPagerSpecies = SPECIES_NONE;

// Argument for the field-move animation: Oak's Pokemon while the Pager is in
// use (used up by this call), otherwise the party member the menu picked.
u32 RR_FieldMoveMonArg(void)
{
    if (sPagerSpecies != SPECIES_NONE)
    {
        u32 arg = sPagerSpecies | SHOW_MON_NOT_IN_PARTY;
        sPagerSpecies = SPECIES_NONE;
        return arg;
    }
    return GetCursorSelectionMonId();
}

// Leaving the Fly map without choosing a town: back to the map, not the party menu
bool32 RR_RanchPagerCancelFly(void)
{
    if (sPagerSpecies == SPECIES_NONE)
        return FALSE;
    sPagerSpecies = SPECIES_NONE;
    SetMainCallback2(CB2_ReturnToField);
    return TRUE;
}

static void BufferMapName(const struct WarpData *warp)
{
    const struct MapHeader *mapHeader = Overworld_GetMapHeaderByGroupAndId(warp->mapGroup, warp->mapNum);
    GetMapNameGeneric(gStringVar2, mapHeader->regionMapSectionId);
}

// VAR_0x8004 = FIELD_MOVE_*. VAR_RESULT = TRUE if it can be used here.
// STR_VAR_1 = Oak's Pokemon's name, STR_VAR_2 = where Teleport/Dig lead.
void RR_ScriptRanchPagerCheck(struct ScriptContext *ctx)
{
    enum FieldMove fieldMove = gSpecialVar_0x8004;

    StringCopy(gStringVar1, GetSpeciesName(FieldMove_GetDefaultSpecies(fieldMove)));
    if (fieldMove == FIELD_MOVE_TELEPORT)
        BufferMapName(&gSaveBlock1Ptr->lastHealLocation);
    else if (fieldMove == FIELD_MOVE_DIG)
        BufferMapName(&gSaveBlock1Ptr->escapeWarp);

    gSpecialVar_Result = gFieldMovesInfo[fieldMove].setUpFunc() == TRUE;
    // only checking: Go sets the callbacks up again
    gFieldCallback2 = NULL;
    gPostMenuFieldCallback = NULL;
}

// VAR_0x8004 = FIELD_MOVE_*, already checked. Call after fadescreen FADE_TO_BLACK.
void RR_ScriptRanchPagerGo(struct ScriptContext *ctx)
{
    enum FieldMove fieldMove = gSpecialVar_0x8004;

    if (gFieldMovesInfo[fieldMove].setUpFunc() != TRUE)
        return;
    sPagerSpecies = FieldMove_GetDefaultSpecies(fieldMove);
    CleanupOverworldWindowsAndTilemaps();
    if (fieldMove == FIELD_MOVE_FLY)
        SetMainCallback2(CB2_OpenFlyMap);
    else
        SetMainCallback2(CB2_ReturnToField);
}
