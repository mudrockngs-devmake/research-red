// Research Red: custom native script commands.
#include "global.h"
#include "rr_scripted_fight.h"
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
#include "string_util.h"
#include "pokeball.h"
#include "party_menu.h"
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

// Elm and Jenny fight Oak while the player watches (lab scene). Their two
// Pokemon (species in VAR_0x8004 / VAR_0x8005, level in VAR_0x8006) briefly
// replace the player's party; created directly so they are not added to the
// player's Pokedex. RR_ScriptReturnPlayerTeam puts the player's party back.
void RR_ScriptBorrowRivalTeam(struct ScriptContext *ctx)
{
    u32 level = VarGet(VAR_0x8006);

    SavePlayerParty();
    ZeroPlayerPartyMons();
    CreateRandomMon(&gPlayerParty[0], VarGet(VAR_0x8004), level);
    CreateRandomMon(&gPlayerParty[1], VarGet(VAR_0x8005), level);
    CalculatePlayerPartyCount();
}

void RR_ScriptReturnPlayerTeam(struct ScriptContext *ctx)
{
    LoadPlayerParty();
    CalculatePlayerPartyCount();
}

// Debug > Scripts > 1 (#153): back to before the lab with no Pokemon, so a
// second try doesn't keep the first try's team
void RR_ScriptEmptyParty(struct ScriptContext *ctx)
{
    ZeroPlayerPartyMons();
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

// Debug 11 (catch test): the smoke tests may poke a species into VAR_TEMP_F
// to meet that one instead of the Pidgey (e.g. one with only the default
// banter lines, #5)
void RR_ScriptDebugWildSpecies(struct ScriptContext *ctx)
{
    u32 species = VarGet(VAR_TEMP_F);

    if (species != SPECIES_NONE && species < NUM_SPECIES)
        CreateRandomMon(&gEnemyParty[0], species, 3);
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
    if (RR_IsScriptedFight()) // #105: Elm and Jenny on your side
        return TRAINER_PIC_RR_ELM_JENNY;
    return sRRWatchPic;
}

// ---------------------------------------------------------------------------
// Lab Supplies: a free shop on every PC once you have the research grant
// (FireRed's Poké Mart screen, every price 0; src/shop.c RR_CreateLabSuppliesMenu,
// data/scripts/pc.inc). Every Poké Ball except the Master Ball (a later
// reward), medicine and status heals, Repels, X items, held battle items,
// Rare Candy (it stops at the level cap), PP Up/Max and vitamins.
// ---------------------------------------------------------------------------
const u16 gRRLabSupplies[] =
{
    // Poké Balls
    ITEM_POKE_BALL, ITEM_GREAT_BALL, ITEM_ULTRA_BALL, ITEM_PREMIER_BALL, ITEM_HEAL_BALL,
    ITEM_NET_BALL, ITEM_NEST_BALL, ITEM_DIVE_BALL, ITEM_DUSK_BALL, ITEM_TIMER_BALL,
    ITEM_QUICK_BALL, ITEM_REPEAT_BALL, ITEM_LUXURY_BALL, ITEM_FAST_BALL, ITEM_LEVEL_BALL,
    ITEM_LURE_BALL, ITEM_HEAVY_BALL, ITEM_LOVE_BALL, ITEM_FRIEND_BALL, ITEM_MOON_BALL,
    ITEM_SPORT_BALL, ITEM_DREAM_BALL, ITEM_BEAST_BALL,
    // medicine
    ITEM_POTION, ITEM_SUPER_POTION, ITEM_HYPER_POTION, ITEM_MAX_POTION, ITEM_FULL_RESTORE,
    ITEM_REVIVE, ITEM_MAX_REVIVE, ITEM_SACRED_ASH, ITEM_FRESH_WATER, ITEM_SODA_POP,
    ITEM_LEMONADE, ITEM_MOOMOO_MILK, ITEM_ETHER, ITEM_MAX_ETHER, ITEM_ELIXIR, ITEM_MAX_ELIXIR,
    // status heals
    ITEM_FULL_HEAL, ITEM_ANTIDOTE, ITEM_PARALYZE_HEAL, ITEM_AWAKENING, ITEM_BURN_HEAL, ITEM_ICE_HEAL,
    // Repels
    ITEM_REPEL, ITEM_SUPER_REPEL, ITEM_MAX_REPEL, ITEM_ESCAPE_ROPE,
    // X items
    ITEM_X_ATTACK, ITEM_X_DEFENSE, ITEM_X_SP_ATK, ITEM_X_SP_DEF, ITEM_X_SPEED, ITEM_X_ACCURACY,
    ITEM_DIRE_HIT, ITEM_GUARD_SPEC,
    // growing up
    ITEM_RARE_CANDY, ITEM_PP_UP, ITEM_PP_MAX,
    ITEM_HP_UP, ITEM_PROTEIN, ITEM_IRON, ITEM_CALCIUM, ITEM_ZINC, ITEM_CARBOS,
    // held battle items
    ITEM_LEFTOVERS, ITEM_LIFE_ORB, ITEM_CHOICE_BAND, ITEM_CHOICE_SPECS, ITEM_CHOICE_SCARF,
    ITEM_FOCUS_SASH, ITEM_ASSAULT_VEST, ITEM_ROCKY_HELMET, ITEM_EVIOLITE, ITEM_EXPERT_BELT,
    ITEM_LIGHT_CLAY, ITEM_WEAKNESS_POLICY, ITEM_AIR_BALLOON, ITEM_HEAVY_DUTY_BOOTS,
    ITEM_AMULET_COIN, ITEM_LUCKY_EGG, ITEM_SOOTHE_BELL, ITEM_QUICK_CLAW, ITEM_SCOPE_LENS,
    ITEM_KINGS_ROCK, ITEM_BLACK_SLUDGE, ITEM_FLAME_ORB, ITEM_TOXIC_ORB, ITEM_SHELL_BELL,
    ITEM_WIDE_LENS, ITEM_MUSCLE_BAND, ITEM_WISE_GLASSES, ITEM_FOCUS_BAND, ITEM_RED_CARD,
    ITEM_EJECT_BUTTON,
    // held berries
    ITEM_ORAN_BERRY, ITEM_SITRUS_BERRY, ITEM_LUM_BERRY, ITEM_CHESTO_BERRY,
    ITEM_NONE
};

// Lab aide (#97): put party Pokémon VAR_0x8005 into ball VAR_RESULT of the
// aide's list (same order as RR_Lab_EventScript_AideBall). STR_VAR_1 = its
// name, STR_VAR_2 = the ball's.
static const u16 sRRAideBalls[] =
{
    ITEM_POKE_BALL,
    ITEM_GREAT_BALL,
    ITEM_ULTRA_BALL,
    ITEM_PREMIER_BALL,
    ITEM_HEAL_BALL,
    ITEM_NET_BALL,
    ITEM_NEST_BALL,
    ITEM_DIVE_BALL,
    ITEM_DUSK_BALL,
    ITEM_TIMER_BALL,
    ITEM_QUICK_BALL,
    ITEM_REPEAT_BALL,
    ITEM_LUXURY_BALL,
    ITEM_MASTER_BALL,
    ITEM_FAST_BALL,
    ITEM_LEVEL_BALL,
    ITEM_LURE_BALL,
    ITEM_HEAVY_BALL,
    ITEM_LOVE_BALL,
    ITEM_FRIEND_BALL,
    ITEM_MOON_BALL,
    ITEM_SPORT_BALL,
    ITEM_DREAM_BALL,
    ITEM_BEAST_BALL,
};

void RR_ScriptSetMonBall(struct ScriptContext *ctx)
{
    struct Pokemon *mon = &gPlayerParty[gSpecialVar_0x8005 % PARTY_SIZE];
    u16 item = sRRAideBalls[gSpecialVar_Result % ARRAY_COUNT(sRRAideBalls)];
    u32 ball = ItemIdToBallId(item);

    SetMonData(mon, MON_DATA_POKEBALL, &ball);
    GetMonNickname(mon, gStringVar1);
    CopyItemName(item, gStringVar2);
}

// ---------------------------------------------------------------------------
// Watching Elm and Jenny take on Brock (#69): the four Pokémon on the field in
// round VAR_0x8004 (1-3), as overworld sprites through VAR_OBJ_GFX_ID_0-3
// (Brock's left, right; Elm's, Jenny's). Elm's and Jenny's starters follow the
// lab pick; the rest is their Pewter team.
// ---------------------------------------------------------------------------
static const u16 sRRStarters[6] = {SPECIES_BULBASAUR, SPECIES_CHARMANDER, SPECIES_SQUIRTLE, SPECIES_CHIKORITA, SPECIES_CYNDAQUIL, SPECIES_TOTODILE};

static u16 RR_RivalStarterSpecies(bool32 forElm)
{
    u32 pick = VarGet(VAR_RR_STARTER) % 6;
    u32 region = pick / 3, type = pick % 3;
    return sRRStarters[(1 - region) * 3 + (type + (forElm ? 1 : 2)) % 3];
}

void RR_ScriptBrockWatchRound(struct ScriptContext *ctx)
{
    static const u16 sBrock[3][2] = {
        {SPECIES_GEODUDE, SPECIES_SANDSHREW}, {SPECIES_KABUTO, SPECIES_OMANYTE}, {SPECIES_RHYHORN, SPECIES_ONIX},
    };
    u32 round = (gSpecialVar_0x8004 - 1) % 3;
    u16 elm, jenny;

    switch (round)
    {
    case 0:  elm = RR_RivalStarterSpecies(TRUE); jenny = RR_RivalStarterSpecies(FALSE); break;
    case 1:  elm = SPECIES_PIDGEOTTO; jenny = SPECIES_NIDORINA; break;   // Pudding, Macaron
    default: elm = SPECIES_SANDSHREW; jenny = SPECIES_GROWLITHE; break;  // Churro, Cruller
    }
    VarSet(VAR_OBJ_GFX_ID_0, OBJ_EVENT_MON + sBrock[round][0]);
    VarSet(VAR_OBJ_GFX_ID_1, OBJ_EVENT_MON + sBrock[round][1]);
    VarSet(VAR_OBJ_GFX_ID_2, OBJ_EVENT_MON + elm);
    VarSet(VAR_OBJ_GFX_ID_3, OBJ_EVENT_MON + jenny);
    StringCopy(gStringVar1, GetSpeciesName(elm));
    StringCopy(gStringVar2, GetSpeciesName(jenny));
}
