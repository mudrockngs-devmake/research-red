// Research Red: custom native script commands.
#include "global.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "follower_npc.h"
#include "load_save.h"
#include "pokemon.h"
#include "battle.h"
#include "constants/battle.h"
#include "constants/maps.h"
#include "constants/region_map_sections.h"
#include "constants/species.h"
#include "script.h"
#include "constants/event_objects.h"

// rr_ivy_to_scene localId: Ivy stops following and becomes a cutscene object.
// If the follower is visible, the scene object takes her tile and facing so
// the handoff is seamless; VAR_RESULT says whether that happened.
void RR_ScriptIvyFollowerToObject(struct ScriptContext *ctx)
{
    u32 localId = ScriptReadByte(ctx);
    struct ObjectEvent *follower;
    s16 x, y;
    enum Direction facing;
    u32 objId;

    gSpecialVar_Result = FALSE;
    if (!PlayerHasFollowerNPC())
        return;

    follower = &gObjectEvents[GetFollowerNPCObjectId()];
    if (follower->invisible)
    {
        DestroyFollowerNPC();
        return;
    }

    x = follower->currentCoords.x;
    y = follower->currentCoords.y;
    facing = follower->facingDirection;
    DestroyFollowerNPC();

    objId = GetObjectEventIdByLocalId(localId);
    if (objId >= OBJECT_EVENTS_COUNT)
        objId = TrySpawnObjectEvent(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
    if (objId >= OBJECT_EVENTS_COUNT)
        return;

    MoveObjectEventToMapCoords(&gObjectEvents[objId], x, y);
    ObjectEventTurn(&gObjectEvents[objId], facing);
    gSpecialVar_Result = TRUE;
}

// Elm and Jenny fight Oak while the player watches (lab scene, if the player
// lost to them). Their two Pokemon (species in VAR_0x8004 / VAR_0x8005) briefly
// replace the player's party; created directly so they are not added to the
// player's Pokedex. RR_ScriptReturnPlayerTeam puts the player's party back.
void RR_ScriptBorrowRivalTeam(struct ScriptContext *ctx)
{
    SavePlayerParty();
    ZeroPlayerPartyMons();
    CreateRandomMon(&gPlayerParty[0], VarGet(VAR_0x8004), 6);
    CreateRandomMon(&gPlayerParty[1], VarGet(VAR_0x8005), 6);
    CalculatePlayerPartyCount();
}

void RR_ScriptReturnPlayerTeam(struct ScriptContext *ctx)
{
    LoadPlayerParty();
    CalculatePlayerPartyCount();
}

// Route 1 catch reactions: remember the first four catches made on Route 1
// while the group scene is pending (VAR_0x40FE == 4). Called when a wild
// battle ends (battle_setup.c, CB2_EndWildBattle).
void RR_OnWildBattleEnd(void)
{
    u32 count;

    if (gBattleOutcome != B_OUTCOME_CAUGHT || VarGet(VAR_0x40FE) != 4)
        return;
    if (gSaveBlock1Ptr->location.mapGroup != MAP_GROUP(MAP_ROUTE1)
     || gSaveBlock1Ptr->location.mapNum != MAP_NUM(MAP_ROUTE1))
        return;

    count = VarGet(VAR_RR_ROUTE1_CATCHES);
    if (count < 4)
        VarSet(VAR_RR_ROUTE1_CATCH_0 + count, gBattleResults.caughtMonSpecies);
    VarSet(VAR_RR_ROUTE1_CATCHES, count + 1);
}

// Reaction for Route 1 catch number VAR_0x8004: VAR_0x8005 = species,
// VAR_RESULT = which reaction (matches the case list in rr_route1.inc).
enum
{
    RR_REACT_DEFAULT, RR_REACT_SPEAROW, RR_REACT_RATTATA, RR_REACT_MANKEY, RR_REACT_NIDORAN_M,
    RR_REACT_PIDGEY, RR_REACT_NIDORAN_F, RR_REACT_ODDISH, RR_REACT_SENTRET, RR_REACT_DUPLICATE,
    RR_REACT_MEOWTH, RR_REACT_EEVEE, RR_REACT_HOPPIP, RR_REACT_PIKACHU, RR_REACT_SANDSHREW,
    RR_REACT_DIGLETT, RR_REACT_HOOTHOOT, RR_REACT_GASTLY, RR_REACT_MURKROW,
};

void RR_ScriptRoute1Reaction(struct ScriptContext *ctx)
{
    u32 index = VarGet(VAR_0x8004);
    u16 species = VarGet(VAR_RR_ROUTE1_CATCH_0 + index);
    u32 i;

    gSpecialVar_0x8005 = species;
    for (i = 0; i < index; i++)
    {
        if (VarGet(VAR_RR_ROUTE1_CATCH_0 + i) == species)
        {
            gSpecialVar_Result = RR_REACT_DUPLICATE;
            return;
        }
    }
    switch (species)
    {
    case SPECIES_SPEAROW:    gSpecialVar_Result = RR_REACT_SPEAROW;    break;
    case SPECIES_RATTATA:    gSpecialVar_Result = RR_REACT_RATTATA;    break;
    case SPECIES_MANKEY:     gSpecialVar_Result = RR_REACT_MANKEY;     break;
    case SPECIES_NIDORAN_M:  gSpecialVar_Result = RR_REACT_NIDORAN_M;  break;
    case SPECIES_PIDGEY:     gSpecialVar_Result = RR_REACT_PIDGEY;     break;
    case SPECIES_NIDORAN_F:  gSpecialVar_Result = RR_REACT_NIDORAN_F;  break;
    case SPECIES_ODDISH:     gSpecialVar_Result = RR_REACT_ODDISH;     break;
    case SPECIES_SENTRET:    gSpecialVar_Result = RR_REACT_SENTRET;    break;
    case SPECIES_MEOWTH:     gSpecialVar_Result = RR_REACT_MEOWTH;     break;
    case SPECIES_EEVEE:      gSpecialVar_Result = RR_REACT_EEVEE;      break;
    case SPECIES_HOPPIP:     gSpecialVar_Result = RR_REACT_HOPPIP;     break;
    case SPECIES_PIKACHU:    gSpecialVar_Result = RR_REACT_PIKACHU;    break;
    case SPECIES_SANDSHREW:  gSpecialVar_Result = RR_REACT_SANDSHREW;  break;
    case SPECIES_DIGLETT:    gSpecialVar_Result = RR_REACT_DIGLETT;    break;
    case SPECIES_HOOTHOOT:   gSpecialVar_Result = RR_REACT_HOOTHOOT;   break;
    case SPECIES_GASTLY:     gSpecialVar_Result = RR_REACT_GASTLY;     break;
    case SPECIES_MURKROW:    gSpecialVar_Result = RR_REACT_MURKROW;    break;
    default:                 gSpecialVar_Result = RR_REACT_DEFAULT;    break;
    }
}

// Ivy's follower chat: which area's lines to use (VAR_RESULT, matches the
// case list in rr_ivy.inc). Indoor maps share their town's map section.
enum
{
    RR_IVY_AREA_ANYWHERE, RR_IVY_AREA_PALLET, RR_IVY_AREA_ROUTE1, RR_IVY_AREA_VIRIDIAN, RR_IVY_AREA_ROUTE22, RR_IVY_AREA_ROUTE2, RR_IVY_AREA_FOREST, RR_IVY_AREA_PEWTER,
};

void RR_ScriptIvyTalkArea(struct ScriptContext *ctx)
{
    switch (gMapHeader.regionMapSectionId)
    {
    case MAPSEC_PALLET_TOWN:
        gSpecialVar_Result = RR_IVY_AREA_PALLET;
        break;
    case MAPSEC_ROUTE_1:
        gSpecialVar_Result = RR_IVY_AREA_ROUTE1;
        break;
    case MAPSEC_VIRIDIAN_CITY:
        gSpecialVar_Result = RR_IVY_AREA_VIRIDIAN;
        break;
    case MAPSEC_ROUTE_22:
        gSpecialVar_Result = RR_IVY_AREA_ROUTE22;
        break;
    case MAPSEC_ROUTE_2:
    case MAPSEC_DIGLETTS_CAVE:
        gSpecialVar_Result = RR_IVY_AREA_ROUTE2;
        break;
    case MAPSEC_VIRIDIAN_FOREST:
        gSpecialVar_Result = RR_IVY_AREA_FOREST;
        break;
    case MAPSEC_PEWTER_CITY:
        gSpecialVar_Result = RR_IVY_AREA_PEWTER;
        break;
    default:
        gSpecialVar_Result = RR_IVY_AREA_ANYWHERE;
        break;
    }
}

// Debug skips: raise every party Pokemon below VAR_0x8004's level to it, so
// skipping ahead gives a team that fits the story point.
void RR_ScriptDebugSetPartyLevel(struct ScriptContext *ctx)
{
    u32 i, level = gSpecialVar_0x8004;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct Pokemon *mon = &gPlayerParty[i];
        u32 species = GetMonData(mon, MON_DATA_SPECIES);
        if (species == SPECIES_NONE || GetMonData(mon, MON_DATA_LEVEL) >= level)
            continue;
        SetMonData(mon, MON_DATA_EXP, &gExperienceTables[gSpeciesInfo[species].growthRate][level]);
        CalculateMonStats(mon);
    }
}
