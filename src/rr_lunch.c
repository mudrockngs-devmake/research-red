// Research Red: the Pewter Gym lunch, the Pokémon's turn (docs/scenes/pewter_lunch.md).
// Your team, Elm and Jenny's and Brock's eat together, drawn with the game's small menu icons: they take
// little sprite memory and share a few colour sets, so all eighteen fit with the right colours.
// If a colour set ever ran out, that Pokémon stays
// out of the picture rather than showing wrong colours.
#include "global.h"
#include "data.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_camera.h"
#include "field_effect.h"
#include "field_player_avatar.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "script.h"
#include "sprite.h"
#include "constants/event_objects.h"
#include "constants/opponents.h"
#include "constants/pokemon_icon.h"
#include "constants/vars.h"

#define RR_LUNCH_MAX_MONS 18
#define RR_LUNCH_PER_ROW  6
#define RR_LUNCH_GYM_PEOPLE 15
#define RR_LUNCH_TABLE_X    135

// Same pairing as the Pewter rematch (rr_pewter.inc), by your lab pick
static const u16 sRR_LunchRivalTeams[6] =
{
    TRAINER_RR_ELM_PEWTER_CYNDAQUIL,
    TRAINER_RR_ELM_PEWTER_TOTODILE,
    TRAINER_RR_ELM_PEWTER_CHIKORITA,
    TRAINER_RR_ELM_PEWTER_CHARMANDER,
    TRAINER_RR_ELM_PEWTER_SQUIRTLE,
    TRAINER_RR_ELM_PEWTER_BULBASAUR,
};

static EWRAM_DATA u8 sRR_LunchIcons[RR_LUNCH_MAX_MONS] = {0};
static EWRAM_DATA u8 sRR_LunchCount = 0;

// Two matching rows, one above the table and one below, filled from the middle out so a small
// team still looks even. Elm and Jenny's team on top, yours below.
static const u8 sRR_LunchSeatX[RR_LUNCH_PER_ROW] = {121, 149, 93, 177, 65, 205};
static const u8 sRR_LunchSeatY[2] = {18, 102};
// Brock's team down the two sides, between the ends of those rows
static const u8 sRR_LunchSideX[RR_LUNCH_PER_ROW] = {65, 205, 65, 205, 65, 205};
static const u8 sRR_LunchSideY[RR_LUNCH_PER_ROW] = {40, 40, 60, 60, 80, 80};

static void RR_LunchAddMon(u32 species, u32 personality, u32 x, u32 y)
{
    u32 spriteId;

    if (sRR_LunchCount >= RR_LUNCH_MAX_MONS || species == SPECIES_NONE || species == SPECIES_EGG)
        return;
    if (IndexOfSpritePaletteTag(TAG_NONE) == 0xFF
     && IndexOfSpritePaletteTag(POKE_ICON_BASE_PAL_TAG + gSpeciesInfo[species].iconPalIndex) == 0xFF)
        return;
    LoadMonIconPalette(species);
    spriteId = CreateMonIcon(species, SpriteCB_MonIcon, x, y, 2, personality);
    if (spriteId != MAX_SPRITES)
    {
        // Icons all look left: mirror the right-hand ones so everyone faces the table
        if (x > RR_LUNCH_TABLE_X)
            gSprites[spriteId].hFlip = TRUE;
        sRR_LunchIcons[sRR_LunchCount++] = spriteId;
    }
}

// With the screen black. The people stay at the table; the Pokémon sit above and below them.
void RR_ScriptLunchPokemonShow(struct ScriptContext *ctx)
{
    const struct TrainerMon *rivals = GetTrainerPartyFromId(sRR_LunchRivalTeams[VarGet(VAR_RR_STARTER) % 6]);
    u32 rivalCount = GetTrainerPartySizeFromId(sRR_LunchRivalTeams[VarGet(VAR_RR_STARTER) % 6]);
    const struct TrainerMon *brock = GetTrainerPartyFromId(TRAINER_RR_BROCK_WATCH);
    u32 brockCount = GetTrainerPartySizeFromId(TRAINER_RR_BROCK_WATCH);
    u32 i;

    sRR_LunchCount = 0;
    for (i = 0; i < rivalCount && i < RR_LUNCH_PER_ROW; i++)
        RR_LunchAddMon(rivals[i].species, 0, sRR_LunchSeatX[i], sRR_LunchSeatY[0]);
    for (i = 0; i < PARTY_SIZE; i++)
        RR_LunchAddMon(GetMonData(&gPlayerParty[i], MON_DATA_SPECIES_OR_EGG),
                       GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY), sRR_LunchSeatX[i], sRR_LunchSeatY[1]);
    for (i = 0; i < brockCount && i < RR_LUNCH_PER_ROW; i++)
        RR_LunchAddMon(brock[i].species, 0, sRR_LunchSideX[i], sRR_LunchSideY[i]);
}

// With the screen black again: the Pokémon go
void RR_ScriptLunchPokemonHide(struct ScriptContext *ctx)
{
    u32 i;

    for (i = 0; i < sRR_LunchCount; i++)
        FreeAndDestroyMonIconSprite(&gSprites[sRR_LunchIcons[i]]);
    FreeMonIconPalettes();
    // every person on the map, also any the camera lost track of: the next shot moves them
    for (i = 1; i <= RR_LUNCH_GYM_PEOPLE; i++)
        TrySpawnObjectEvent(i, gSaveBlock1Ptr->location.mapNum, gSaveBlock1Ptr->location.mapGroup);
}

// setobjectxy on the player leaves the camera where it was: bring it along (screen black)
void RR_ScriptCameraOnPlayer(struct ScriptContext *ctx)
{
    u32 i;

    RR_MoveCameraToPlayer();
    // everyone's sprite is placed relative to the camera: put them back on their tiles
    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        if (gObjectEvents[i].active)
            MoveObjectEventToMapCoords(&gObjectEvents[i], gObjectEvents[i].currentCoords.x, gObjectEvents[i].currentCoords.y);
    }
}
