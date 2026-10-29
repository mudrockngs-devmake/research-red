// Research Red: scripted fights you watch (#105, owner 2026-10-29).
//
// Elm and Jenny against Oak in the lab: a real battle on the battle screen
// that plays out the same way every time, with lines between turns. It is a
// watched fight (AI vs AI, FLAG_RR_AI_VS_AI) with a few rules on top:
//   - Oak's Pokemon stand on the near side with no trainer picture (Oak just
//     calls them out); Elm and Jenny are the trainer on the far side
//     (TRAINER_RR_ELM_JENNY_WATCH, their pair picture).
//   - Every turn each Pokemon uses the kind of move the fight's turn table
//     gives it, against the Pokemon facing it. Nothing misses, nothing lands
//     a critical hit, nobody switches (FireRed's Poke Dude battles work the
//     same way).
//   - Lines play before the first turn and after the turns the line table
//     names: Elm and Jenny's slide in with their picture, the others are
//     plain messages (Ivy's show her portrait).
//   - No "You are challenged" and no prize money.
//
// From a script (rr_lab.inc):
//   call RR_Lab_EventScript_SetupPair   @ Elm and Jenny's picks in VAR_0x8004/5
//   setvar VAR_0x8006, RR_FIGHT_LAB_STARTERS
//   callnative RR_ScriptStartScriptedFight
//   trainerbattle_no_intro TRAINER_RR_ELM_JENNY_WATCH, <Elm and Jenny's losing line>
//   callnative RR_ScriptEndScriptedFight
#include "global.h"
#include "battle.h"
#include "battle_ai_main.h"
#include "battle_controllers.h"
#include "battle_scripts.h"
#include "battle_util.h"
#include "event_data.h"
#include "load_save.h"
#include "pokemon.h"
#include "rr_scripted_fight.h"
#include "constants/rr_fights.h"
#include "script.h"
#include "constants/battle.h"
#include "constants/opponents.h"
#include "constants/species.h"

// What a Pokemon does on a turn
enum
{
    RR_HIT,    // its first plain hit (Tackle, Scratch: weak on purpose, so the fight stays safe)
    RR_BEST,   // its strongest damaging move against the Pokemon facing it (same type and type matchups count)
    RR_STATUS, // its first move that does no damage (Growl, Leer, Tail Whip...)
};

// One turn: what each battler does. The near side is Oak's (battlers 0 and
// 2), the far side Elm and Jenny's (1 and 3). The last turn repeats.
struct RRFightTurn
{
    u8 move[MAX_BATTLERS_COUNT]; // RR_HIT / RR_BEST / RR_STATUS, by battler
};

struct RRFightLine
{
    u8 afterTurn;   // 0 = before the first turn
    bool8 slide;    // Elm and Jenny slide in for it
    const u8 *text;
};

struct RRScriptedFight
{
    const u8 *intro;
    u8 oakLevel, elmJennyLevel, oakIvs, elmJennyIvs;
    u16 oakSpecies[2]; // SPECIES_NONE: Oak's two leftover lab starters
    const struct RRFightTurn *turns;
    u8 turnCount;
    const struct RRFightLine *lines;
    u8 lineCount;
};

// Fight 1: Elm and Jenny (Lv 6) beat Oak's leftover starters (Lv 5). Oak's
// side only uses plain hits (no Growl: it made the fight drag), so Elm and
// Jenny always win, in about four turns.
static const struct RRFightTurn sTurns_LabStarters[] =
{
    {{ [0] = RR_HIT, [2] = RR_HIT, [1] = RR_BEST, [3] = RR_BEST }},
};

static const struct RRFightLine sLines_LabStarters[] =
{
    { 0, TRUE,  COMPOUND_STRING("Elm: Team Science, go! Jenny, the protocol!\pJenny: Step one: don't fall over, dear.{PAUSE_UNTIL_PRESS}") },
    { 1, FALSE, COMPOUND_STRING("Oak: Ho ho! Good form! But can you keep it up?{PAUSE_UNTIL_PRESS}") },
    { 2, TRUE,  COMPOUND_STRING("Elm: Jenny! We're winning! Against Oak!\pJenny: Don't jinx it, dear.{PAUSE_UNTIL_PRESS}") },
    { 3, FALSE, COMPOUND_STRING("Ivy: Oak looks happier losing than most people look winning.{PAUSE_UNTIL_PRESS}") },
};

// Fight 2, the gag: Elm and Jenny (Lv 7) against Oak's ranch Pidgeot and
// Tauros (Lv 50). Oak tries to go easy for one turn.
static const struct RRFightTurn sTurns_LabRanch[] =
{
    {{ [0] = RR_STATUS, [2] = RR_STATUS, [1] = RR_BEST, [3] = RR_BEST }},
    {{ [0] = RR_BEST,   [2] = RR_BEST,   [1] = RR_BEST, [3] = RR_BEST }},
};

static const struct RRFightLine sLines_LabRanch[] =
{
    { 0, TRUE,  COMPOUND_STRING("Elm: Level 50 is just a number, Jenny!\pJenny: It's a very large number, dear.{PAUSE_UNTIL_PRESS}") },
    { 1, FALSE, COMPOUND_STRING("Oak: Pidgeot! Tauros! Gently now, they're colleagues!{PAUSE_UNTIL_PRESS}") },
};

static const struct RRScriptedFight sFights[RR_FIGHT_COUNT] =
{
    [RR_FIGHT_LAB_STARTERS] =
    {
        .intro = COMPOUND_STRING("Elm and Jenny challenge Prof. Oak!\p"),
        .oakLevel = 5, .elmJennyLevel = 6, .oakIvs = 0, .elmJennyIvs = 31,
        .oakSpecies = { SPECIES_NONE, SPECIES_NONE },
        .turns = sTurns_LabStarters, .turnCount = ARRAY_COUNT(sTurns_LabStarters),
        .lines = sLines_LabStarters, .lineCount = ARRAY_COUNT(sLines_LabStarters),
    },
    [RR_FIGHT_LAB_RANCH] =
    {
        .intro = COMPOUND_STRING("Elm and Jenny challenge Prof. Oak!\nAgain!\p"),
        .oakLevel = 50, .elmJennyLevel = 7, .oakIvs = 20, .elmJennyIvs = 31,
        .oakSpecies = { SPECIES_PIDGEOT, SPECIES_TAUROS },
        .turns = sTurns_LabRanch, .turnCount = ARRAY_COUNT(sTurns_LabRanch),
        .lines = sLines_LabRanch, .lineCount = ARRAY_COUNT(sLines_LabRanch),
    },
};

// The two starters Oak has left, by VAR_RR_STARTER (the same as
// RR_Lab_EventScript_SetupPair and the TRAINER_RR_OAK_LAB_* teams)
static const u16 sOakLeftovers[6][2] =
{
    { SPECIES_SQUIRTLE,   SPECIES_CHIKORITA },  // you picked Bulbasaur
    { SPECIES_BULBASAUR,  SPECIES_CYNDAQUIL },  // Charmander
    { SPECIES_CHARMANDER, SPECIES_TOTODILE },   // Squirtle
    { SPECIES_TOTODILE,   SPECIES_BULBASAUR },  // Chikorita
    { SPECIES_CHIKORITA,  SPECIES_CHARMANDER }, // Cyndaquil
    { SPECIES_CYNDAQUIL,  SPECIES_SQUIRTLE },   // Totodile
};

static EWRAM_DATA u8 sFightPlus1 = 0;          // the fight running + 1 (0 = none)
static EWRAM_DATA u16 sElmJennySpecies[2] = {0};
static EWRAM_DATA u8 sLinesPlayed = 0;         // one bit per line

static const struct RRScriptedFight *GetFight(void)
{
    return &sFights[sFightPlus1 - 1];
}

bool32 RR_IsScriptedFight(void)
{
    return sFightPlus1 != 0 && IsAiVsAiBattle();
}

// VAR_0x8006 = RR_FIGHT_*, VAR_0x8004/5 = Elm and Jenny's species. Oak's
// Pokemon stand in for your party until RR_ScriptEndScriptedFight.
void RR_ScriptStartScriptedFight(struct ScriptContext *ctx)
{
    const struct RRScriptedFight *fight;
    u32 i, starter = VarGet(VAR_RR_STARTER);
    u16 species;

    if (VarGet(VAR_0x8006) >= RR_FIGHT_COUNT)
        return;
    sFightPlus1 = VarGet(VAR_0x8006) + 1;
    sLinesPlayed = 0;
    sElmJennySpecies[0] = VarGet(VAR_0x8004);
    sElmJennySpecies[1] = VarGet(VAR_0x8005);
    fight = GetFight();

    SavePlayerParty();
    ZeroPlayerPartyMons();
    for (i = 0; i < 2; i++)
    {
        species = fight->oakSpecies[i];
        if (species == SPECIES_NONE)
            species = sOakLeftovers[starter < 6 ? starter : 0][i];
        CreateRandomMonWithIVs(&gPlayerParty[i], species, fight->oakLevel, fight->oakIvs);
    }
    CalculatePlayerPartyCount();
    FlagSet(FLAG_RR_AI_VS_AI);
    FlagSet(FLAG_RR_NO_WHITEOUT);
}

void RR_ScriptEndScriptedFight(struct ScriptContext *ctx)
{
    sFightPlus1 = 0;
    FlagClear(FLAG_RR_AI_VS_AI);
    FlagClear(FLAG_RR_NO_WHITEOUT);
    LoadPlayerParty();
    CalculatePlayerPartyCount();
}

// Battle start (CreateNPCTrainerParty): Elm and Jenny's two Pokemon
void RR_ScriptedFightAdjustParty(struct Pokemon *party, u16 trainerId)
{
    u32 i;

    if (sFightPlus1 == 0 || trainerId != TRAINER_RR_ELM_JENNY_WATCH)
        return;
    for (i = 0; i < 2; i++)
        CreateRandomMonWithIVs(&party[i], sElmJennySpecies[i], GetFight()->elmJennyLevel, GetFight()->elmJennyIvs);
}

static bool32 IsSameType(enum BattlerId battler, enum Move move)
{
    enum Type type = GetMoveType(move);
    return gBattleMons[battler].types[0] == type || gBattleMons[battler].types[1] == type;
}

// How hard a move hits the target: power, x1.5 for the same type, x the type matchup
static u32 MoveScore(enum BattlerId battler, enum Move move, enum BattlerId target)
{
    u32 score = GetMovePower(move) * (IsSameType(battler, move) ? 3 : 2);
    enum Type type = GetMoveType(move);

    score = score * GetTypeModifier(type, gBattleMons[target].types[0]) / UQ_4_12(1.0);
    if (gBattleMons[target].types[1] != gBattleMons[target].types[0])
        score = score * GetTypeModifier(type, gBattleMons[target].types[1]) / UQ_4_12(1.0);
    return score;
}

// The move slot for RR_HIT / RR_BEST / RR_STATUS against the target (slot 0 if it has none)
static u32 PickMoveSlot(enum BattlerId battler, u32 kind, enum BattlerId target)
{
    u32 i, slot = 0, best = 0, score;
    enum Move move;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        move = gBattleMons[battler].moves[i];
        if (move == MOVE_NONE || gBattleMons[battler].pp[i] == 0)
            continue;
        switch (kind)
        {
        case RR_HIT:
            if (GetMovePower(move) != 0 && !IsSameType(battler, move))
                return i;
            break;
        case RR_STATUS:
            if (GetMovePower(move) == 0)
                return i;
            break;
        case RR_BEST:
            score = MoveScore(battler, move, target);
            if (score > best)
            {
                best = score;
                slot = i;
            }
            break;
        }
    }
    if (kind == RR_HIT) // no plain hit: the strongest move it has
        return PickMoveSlot(battler, RR_BEST, target);
    return slot;
}

// After the AI has thought (ComputeBattlerDecisions): the turn table decides
void RR_ScriptedFightChooseMove(enum BattlerId battler)
{
    const struct RRScriptedFight *fight;
    enum BattlerId target;
    u32 turn;

    if (!RR_IsScriptedFight() || battler >= MAX_BATTLERS_COUNT)
        return;
    fight = GetFight();
    turn = gBattleTurnCounter < fight->turnCount ? gBattleTurnCounter : fight->turnCount - 1;

    // the Pokemon facing it, or its partner once that one has fainted
    target = BATTLE_OPPOSITE(battler);
    if (!IsBattlerAlive(target))
        target = BATTLE_PARTNER(target);
    gAiBattleData->chosenTarget[battler] = target;
    gAiBattleData->chosenMoveIndex[battler] = PickMoveSlot(battler, fight->turns[turn].move[battler], target);
    gAiLogicData->shouldSwitch &= ~(1u << battler);
}

static bool32 IsSideDown(enum BattleSide side)
{
    enum BattlerId battler;

    for (battler = 0; battler < gBattlersCount; battler++)
    {
        if (GetBattlerSide(battler) == side && IsBattlerAlive(battler))
            return FALSE;
    }
    return TRUE;
}

// Before the first turn and at the end of each turn: the battle script for
// the next line due (NULL if none). gBattleTurnCounter is the number of
// turns done.
const u8 *RR_ScriptedFightLineScript(void)
{
    const struct RRScriptedFight *fight;
    u32 i;

    if (!RR_IsScriptedFight() || IsSideDown(B_SIDE_PLAYER) || IsSideDown(B_SIDE_OPPONENT))
        return NULL;
    fight = GetFight();
    for (i = 0; i < fight->lineCount; i++)
    {
        if (fight->lines[i].afterTurn != gBattleTurnCounter || (sLinesPlayed & (1u << i)))
            continue;
        sLinesPlayed |= 1u << i;
        gBattleStruct->trainerSlideMsg = fight->lines[i].text;
        return fight->lines[i].slide ? BattleScript_TrainerASlideMsgEnd2 : BattleScript_RRScriptedLineEnd2;
    }
    return NULL;
}

const u8 *RR_ScriptedFightIntroText(void)
{
    return GetFight()->intro;
}

// Oak's side has no trainer picture: it's drawn (the intro needs it) but hidden
void RR_ScriptedFightHideTrainerPic(enum BattlerId battler)
{
    if (RR_IsScriptedFight() && GetBattlerSide(battler) == B_SIDE_PLAYER)
        gSprites[gBattleStruct->trainerSlideSpriteIds[battler]].invisible = TRUE;
}
