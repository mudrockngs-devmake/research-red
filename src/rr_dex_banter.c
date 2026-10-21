// Research Red: Pokédex banter.
//
// After a catch, the lines play over the new Pokédex entry page, with Ivy's
// and the player's portraits facing each other (pokedex_plus_hgss.c, #5);
// SELECT on an entry replays them (#4). A new species from evolving (no entry
// page) is queued instead, and once you're back on the map with control,
// RR_EventScript_DexBanter plays the lines, one message per line so the
// portrait follows the speaker. For each species:
//   1. the species' own lines (or a random [DEFAULT] pair)
//   2. [ELM FAMILY] / [JENNY FAMILY] lines if it shares a family with Elm's or
//      Jenny's lab starter
// Lines: data/research_red/dex_banter.txt (tools/research_red/build_dex_banter.py).
#include "global.h"
#include "event_data.h"
#include "pokemon.h"
#include "random.h"
#include "rr_dex_banter.h"
#include "script.h"
#include "string_util.h"
#include "event_scripts.h"
#include "constants/species.h"

struct RRDexBanter
{
    u16 species;
    const u8 *const *lines;
};

#include "data/research_red/dex_banter.h"

#define QUEUE_SIZE 4

enum
{
    STAGE_SPECIES,
    STAGE_ELM,
    STAGE_JENNY,
    STAGE_DONE,
};

// Lab starters (VAR_RR_STARTER order) and their families
static const u16 sStarterFamilies[6][3] =
{
    {SPECIES_BULBASAUR, SPECIES_IVYSAUR, SPECIES_VENUSAUR},
    {SPECIES_CHARMANDER, SPECIES_CHARMELEON, SPECIES_CHARIZARD},
    {SPECIES_SQUIRTLE, SPECIES_WARTORTLE, SPECIES_BLASTOISE},
    {SPECIES_CHIKORITA, SPECIES_BAYLEEF, SPECIES_MEGANIUM},
    {SPECIES_CYNDAQUIL, SPECIES_QUILAVA, SPECIES_TYPHLOSION},
    {SPECIES_TOTODILE, SPECIES_CROCONAW, SPECIES_FERALIGATR},
};

static EWRAM_DATA u16 sQueue[QUEUE_SIZE] = {0};
static EWRAM_DATA u8 sQueueCount = 0;
static EWRAM_DATA u8 sStage = 0;
static EWRAM_DATA u8 sLine = 0;
static EWRAM_DATA u8 sDefaultPair = 0;
static EWRAM_DATA u16 sCurSpecies = 0;   // the species whose lines are playing

void RR_DexBanterQueue(u16 species)
{
    u32 i;

    for (i = 0; i < sQueueCount; i++)
    {
        if (sQueue[i] == species)
            return;
    }
    if (sQueueCount < QUEUE_SIZE)
        sQueue[sQueueCount++] = species;
}

// Called from ProcessPlayerFieldInput: start the banter once the player has control
bool32 RR_TryStartDexBanter(void)
{
    // Ivy travels with you from the lab on (story progress 3+)
    if (sQueueCount == 0 || VarGet(VAR_0x40FE) < 3)
        return FALSE;
    RR_DexBanterBegin(sQueue[0]);
    ScriptContext_SetupScript(RR_EventScript_DexBanter);
    return TRUE;
}

// Ivy is with you (story progress 3+), so there's someone to banter with
bool32 RR_DexBanterAvailable(void)
{
    return VarGet(VAR_0x40FE) >= 3;
}

void RR_DexBanterBegin(u16 species)
{
    sCurSpecies = species;
    sStage = STAGE_SPECIES;
    sLine = 0;
    sDefaultPair = Random() % (ARRAY_COUNT(sDexBanter_Default) / 2);
}

// Drop a species from the map queue (its lines already played over the dex page)
void RR_DexBanterForget(u16 species)
{
    u32 i, j;

    for (i = 0; i < sQueueCount; i++)
    {
        if (sQueue[i] != species)
            continue;
        for (j = i + 1; j < sQueueCount; j++)
            sQueue[j - 1] = sQueue[j];
        sQueueCount--;
        return;
    }
}

static const u8 *const *GetSpeciesLines(u16 species)
{
    u32 i;
    for (i = 0; i < ARRAY_COUNT(sDexBanterTable); i++)
    {
        if (sDexBanterTable[i].species == species)
            return sDexBanterTable[i].lines;
    }
    return NULL;
}

// Elm's (forElm) or Jenny's lab starter, from the starter chain
static u16 GetRivalStarter(bool32 forElm)
{
    u32 pick = VarGet(VAR_RR_STARTER) % 6;
    u32 region = pick / 3, type = pick % 3;
    return sStarterFamilies[(1 - region) * 3 + (type + (forElm ? 1 : 2)) % 3][0];
}

static bool32 InStarterFamily(u16 species, u16 starter)
{
    u32 i, j;
    for (i = 0; i < ARRAY_COUNT(sStarterFamilies); i++)
    {
        if (sStarterFamilies[i][0] != starter)
            continue;
        for (j = 0; j < 3; j++)
        {
            if (sStarterFamilies[i][j] == species)
                return TRUE;
        }
    }
    return FALSE;
}

// The next line for sCurSpecies (placeholders not yet expanded: {PLAYER},
// STR_VAR_2 = the rival's starter, STR_VAR_3 = the species), or NULL when done
const u8 *RR_DexBanterNext(void)
{
    u16 species = sCurSpecies;
    const u8 *line = NULL;
    const u8 *const *lines;

    StringCopy(gStringVar3, GetSpeciesName(species));
    while (line == NULL && sStage != STAGE_DONE)
    {
        switch (sStage)
        {
        case STAGE_SPECIES:
            lines = GetSpeciesLines(species);
            if (lines != NULL)
                line = lines[sLine];
            else if (sLine < 2)
                line = sDexBanter_Default[sDefaultPair * 2 + sLine];
            break;
        case STAGE_ELM:
        case STAGE_JENNY:
        {
            u16 starter = GetRivalStarter(sStage == STAGE_ELM);
            if (VarGet(VAR_0x40FE) >= 3 && InStarterFamily(species, starter))
            {
                StringCopy(gStringVar2, GetSpeciesName(starter));
                line = (sStage == STAGE_ELM ? sDexBanter_ElmFamily : sDexBanter_JennyFamily)[sLine];
            }
            break;
        }
        }
        if (line == NULL)
        {
            sStage++;
            sLine = 0;
        }
    }

    if (line != NULL)
        sLine++;
    return line;
}

// callnative: puts the next line in gStringVar1 (VAR_RESULT TRUE), or finishes
// this species (VAR_RESULT FALSE)
void RR_ScriptDexBanterNextLine(struct ScriptContext *ctx)
{
    const u8 *line = RR_DexBanterNext();

    if (line != NULL)
    {
        StringCopy(gStringVar1, line);
        gSpecialVar_Result = TRUE;
        return;
    }
    // This species is done: drop it from the queue
    RR_DexBanterForget(sCurSpecies);
    gSpecialVar_Result = FALSE;
}

// Debug Script 7: queue the banter for the species in VAR_0x8004
void RR_ScriptDebugQueueDexBanter(struct ScriptContext *ctx)
{
    RR_DexBanterQueue(gSpecialVar_0x8004);
}
