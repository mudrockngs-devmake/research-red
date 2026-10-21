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
#include "item.h"
#include "bike.h"
#include "script_movement.h"
#include "fieldmap.h"
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
// (or in Pallet's sea) while the group scene is pending (VAR_0x40FE == 4). Called when a wild
// battle ends (battle_setup.c, CB2_EndWildBattle).
void RR_OnWildBattleEnd(void)
{
    u32 count;

    if (gBattleOutcome != B_OUTCOME_CAUGHT || VarGet(VAR_0x40FE) != 4)
        return;
    // #85: Route 1, and Pallet's sea (surfing and fishing before the walk)
    if (!(gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_ROUTE1) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_ROUTE1))
     && !(gSaveBlock1Ptr->location.mapGroup == MAP_GROUP(MAP_PALLET_TOWN) && gSaveBlock1Ptr->location.mapNum == MAP_NUM(MAP_PALLET_TOWN)))
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
    RR_REACT_TENTACOOL, RR_REACT_MAGIKARP, RR_REACT_HORSEA, RR_REACT_CHINCHOU, RR_REACT_QWILFISH,
    RR_REACT_KRABBY, RR_REACT_STARYU, RR_REACT_SHELLDER, RR_REACT_REMORAID, RR_REACT_DRATINI,
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
    case SPECIES_TENTACOOL:  gSpecialVar_Result = RR_REACT_TENTACOOL;  break;
    case SPECIES_MAGIKARP:   gSpecialVar_Result = RR_REACT_MAGIKARP;   break;
    case SPECIES_HORSEA:     gSpecialVar_Result = RR_REACT_HORSEA;     break;
    case SPECIES_CHINCHOU:   gSpecialVar_Result = RR_REACT_CHINCHOU;   break;
    case SPECIES_QWILFISH:   gSpecialVar_Result = RR_REACT_QWILFISH;   break;
    case SPECIES_KRABBY:     gSpecialVar_Result = RR_REACT_KRABBY;     break;
    case SPECIES_STARYU:     gSpecialVar_Result = RR_REACT_STARYU;     break;
    case SPECIES_SHELLDER:   gSpecialVar_Result = RR_REACT_SHELLDER;   break;
    case SPECIES_REMORAID:   gSpecialVar_Result = RR_REACT_REMORAID;   break;
    case SPECIES_DRATINI:    gSpecialVar_Result = RR_REACT_DRATINI;    break;
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

// ---------------------------------------------------------------------------
// rr_move_pair (#83): the leader walks a movement script, the follower trails
// one step behind (Jenny behind Elm, the way Ivy follows the player).
// ---------------------------------------------------------------------------
#define RR_TRAIL_MAX 64
static EWRAM_DATA u8 sRRTrailMovement[RR_TRAIL_MAX] = {0};

// Walk actions come in groups of four (down, up, left, right)
static const u8 sRRWalkBases[] =
{
    MOVEMENT_ACTION_WALK_SLOW_DOWN,
    MOVEMENT_ACTION_WALK_NORMAL_DOWN,
    MOVEMENT_ACTION_WALK_FAST_DOWN,
    MOVEMENT_ACTION_WALK_FASTER_DOWN,
};

// The walk group of an action (or -1), and its direction (0 down 1 up 2 left 3 right)
static s32 RR_WalkBase(u8 action, u32 *dir)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sRRWalkBases); i++)
    {
        if (action >= sRRWalkBases[i] && action < sRRWalkBases[i] + 4)
        {
            *dir = action - sRRWalkBases[i];
            return sRRWalkBases[i];
        }
    }
    return -1;
}

static bool32 RR_IsDelay(u8 action)
{
    return action >= MOVEMENT_ACTION_DELAY_1 && action <= MOVEMENT_ACTION_DELAY_16;
}

void RR_ScriptMovePair(struct ScriptContext *ctx)
{
    u16 leaderId = VarGet(ScriptReadHalfword(ctx));
    u16 followerId = VarGet(ScriptReadHalfword(ctx));
    const u8 *movement = (const u8 *)ScriptReadWord(ctx);
    u8 mapNum = gSaveBlock1Ptr->location.mapNum, mapGroup = gSaveBlock1Ptr->location.mapGroup;
    struct ObjectEvent *leader = &gObjectEvents[GetObjectEventIdByLocalId(leaderId)];
    struct ObjectEvent *follower = &gObjectEvents[GetObjectEventIdByLocalId(followerId)];
    s32 dx = leader->currentCoords.x - follower->currentCoords.x;
    s32 dy = leader->currentCoords.y - follower->currentCoords.y;
    u32 n = 0, i, dir, walks = 0, walksSeen = 0;
    s32 firstBase = MOVEMENT_ACTION_WALK_NORMAL_DOWN;

    for (i = 0; movement[i] != MOVEMENT_ACTION_STEP_END; i++)
    {
        s32 base = RR_WalkBase(movement[i], &dir);
        if (base >= 0)
        {
            if (walks == 0)
                firstBase = base;
            walks++;
        }
    }
    // Step onto the leader's tile first, if the follower starts next to it
    if (walks != 0 && (dx * dx + dy * dy) == 1)
    {
        u32 toLeader = dy > 0 ? 0 : dy < 0 ? 1 : dx < 0 ? 2 : 3;
        sRRTrailMovement[n++] = firstBase + toLeader;
        walks--; // the follower ends one step short of the leader
    }
    for (i = 0; movement[i] != MOVEMENT_ACTION_STEP_END && n < RR_TRAIL_MAX - 1; i++)
    {
        if (RR_WalkBase(movement[i], &dir) >= 0)
        {
            if (walksSeen++ < walks)
                sRRTrailMovement[n++] = movement[i];
        }
        else if (RR_IsDelay(movement[i]))
        {
            sRRTrailMovement[n++] = movement[i];
        }
    }
    sRRTrailMovement[n] = MOVEMENT_ACTION_STEP_END;

    leader->directionOverwrite = DIR_NONE;
    follower->directionOverwrite = DIR_NONE;
    ScriptMovement_StartObjectMovementScript(leaderId, mapNum, mapGroup, movement);
    ScriptMovement_StartObjectMovementScript(followerId, mapNum, mapGroup, sRRTrailMovement);
}

// rr_walk_to: walk an object to (x, y) along an L-shaped path, after a delay
static EWRAM_DATA u8 sRRWalkToMovement[4][RR_TRAIL_MAX] = {0};
static EWRAM_DATA u8 sRRWalkToNext = 0;

static u32 RR_AddSteps(u8 *buf, u32 n, s32 delta, u8 negAction, u8 posAction)
{
    while (delta != 0 && n < RR_TRAIL_MAX - 1)
    {
        buf[n++] = delta < 0 ? negAction : posAction;
        delta += delta < 0 ? 1 : -1;
    }
    return n;
}

void RR_ScriptWalkTo(struct ScriptContext *ctx)
{
    u16 localId = VarGet(ScriptReadHalfword(ctx));
    s32 x = VarGet(ScriptReadHalfword(ctx));
    s32 y = VarGet(ScriptReadHalfword(ctx));
    bool32 yFirst = ScriptReadByte(ctx);
    u32 delay = ScriptReadByte(ctx);
    struct ObjectEvent *obj = &gObjectEvents[GetObjectEventIdByLocalId(localId)];
    // several walkers can be on the way at once: each gets its own buffer
    u8 *buf = sRRWalkToMovement[sRRWalkToNext++ % ARRAY_COUNT(sRRWalkToMovement)];
    s32 dx = x - (obj->currentCoords.x - MAP_OFFSET);
    s32 dy = y - (obj->currentCoords.y - MAP_OFFSET);
    u32 n = 0;

    for (; delay >= 16 && n < 8; delay -= 16)
        buf[n++] = MOVEMENT_ACTION_DELAY_16;
    for (; delay >= 1 && n < 12; delay--)
        buf[n++] = MOVEMENT_ACTION_DELAY_1;
    if (yFirst)
        n = RR_AddSteps(buf, n, dy, MOVEMENT_ACTION_WALK_NORMAL_UP, MOVEMENT_ACTION_WALK_NORMAL_DOWN);
    n = RR_AddSteps(buf, n, dx, MOVEMENT_ACTION_WALK_NORMAL_LEFT, MOVEMENT_ACTION_WALK_NORMAL_RIGHT);
    if (!yFirst)
        n = RR_AddSteps(buf, n, dy, MOVEMENT_ACTION_WALK_NORMAL_UP, MOVEMENT_ACTION_WALK_NORMAL_DOWN);
    buf[n] = MOVEMENT_ACTION_STEP_END;
    obj->directionOverwrite = DIR_NONE;
    ScriptMovement_StartObjectMovementScript(localId, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup, buf);
}

// Debug Script 12 (bike test): get on the Bicycle, as using it from the Bag does
void RR_ScriptDebugMountBike(struct ScriptContext *ctx)
{
    GetOnOffBike(PLAYER_AVATAR_STATE_MACH_BIKE);
    FollowerNPC_HandleBike();
}

// ---------------------------------------------------------------------------
// The picture on your side of a battle that plays itself (#7): who you're
// watching, e.g. Elm while he and Jenny face Oak. Set before the battle with
// callnative RR_ScriptSetWatchPic (VAR_0x8004 = TRAINER_PIC_*, 0 = nobody:
// your own picture); the scene sets it back to 0 afterwards.
// ---------------------------------------------------------------------------
static EWRAM_DATA u16 sRRWatchPic = 0;

void RR_ScriptSetWatchPic(struct ScriptContext *ctx)
{
    sRRWatchPic = gSpecialVar_0x8004;
}

// The front pic of whoever is being watched, or TRAINER_PIC_NONE for your own
u16 RR_GetWatchTrainerPic(void)
{
    return sRRWatchPic;
}

// ---------------------------------------------------------------------------
// The research grant's supplies go to the PC's item storage (#96): withdraw
// what you want. Medicine and status items (#99), X items, and 6 of every
// held battle item (#100). The Poké Balls (#98) and key items go to the Bag.
// ---------------------------------------------------------------------------
static const u16 sRRGrantToPC[][2] =
{
    {ITEM_POTION, 30}, {ITEM_SUPER_POTION, 30}, {ITEM_HYPER_POTION, 20}, {ITEM_MAX_POTION, 10},
    {ITEM_FULL_RESTORE, 10}, {ITEM_REVIVE, 15}, {ITEM_MAX_REVIVE, 5}, {ITEM_SACRED_ASH, 2},
    {ITEM_FULL_HEAL, 20}, {ITEM_ANTIDOTE, 15}, {ITEM_PARALYZE_HEAL, 15}, {ITEM_AWAKENING, 15},
    {ITEM_BURN_HEAL, 15}, {ITEM_ICE_HEAL, 15}, {ITEM_FRESH_WATER, 20}, {ITEM_LEMONADE, 10},
    {ITEM_MOOMOO_MILK, 10}, {ITEM_ETHER, 10}, {ITEM_MAX_ETHER, 5}, {ITEM_ELIXIR, 10},
    {ITEM_MAX_ELIXIR, 5}, {ITEM_PP_UP, 10}, {ITEM_MAX_REPEL, 30}, {ITEM_ESCAPE_ROPE, 10},
    {ITEM_X_ATTACK, 10}, {ITEM_X_DEFENSE, 10}, {ITEM_X_SP_ATK, 10}, {ITEM_X_SP_DEF, 10},
    {ITEM_X_SPEED, 10}, {ITEM_X_ACCURACY, 10}, {ITEM_DIRE_HIT, 10}, {ITEM_GUARD_SPEC, 10},
    {ITEM_LEFTOVERS, 6}, {ITEM_LIFE_ORB, 6}, {ITEM_CHOICE_BAND, 6}, {ITEM_CHOICE_SPECS, 6},
    {ITEM_CHOICE_SCARF, 6}, {ITEM_FOCUS_SASH, 6}, {ITEM_ASSAULT_VEST, 6}, {ITEM_ROCKY_HELMET, 6},
    {ITEM_EVIOLITE, 6}, {ITEM_EXPERT_BELT, 6}, {ITEM_LIGHT_CLAY, 6}, {ITEM_WEAKNESS_POLICY, 6},
    {ITEM_AIR_BALLOON, 6}, {ITEM_HEAVY_DUTY_BOOTS, 6}, {ITEM_AMULET_COIN, 6}, {ITEM_LUCKY_EGG, 6},
    {ITEM_SOOTHE_BELL, 6}, {ITEM_QUICK_CLAW, 6}, {ITEM_SCOPE_LENS, 6}, {ITEM_KINGS_ROCK, 6},
    {ITEM_BLACK_SLUDGE, 6}, {ITEM_FLAME_ORB, 6}, {ITEM_TOXIC_ORB, 6}, {ITEM_SHELL_BELL, 6},
    {ITEM_WIDE_LENS, 6}, {ITEM_MUSCLE_BAND, 6}, {ITEM_WISE_GLASSES, 6}, {ITEM_FOCUS_BAND, 6},
    {ITEM_RED_CARD, 6}, {ITEM_EJECT_BUTTON, 6},
};

void RR_ScriptGrantToPC(struct ScriptContext *ctx)
{
    u32 i;

    for (i = 0; i < ARRAY_COUNT(sRRGrantToPC); i++)
        AddPCItem(sRRGrantToPC[i][0], sRRGrantToPC[i][1]);
}
