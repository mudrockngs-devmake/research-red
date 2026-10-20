// Research Red: rematches that grow with your badges (#53, later #70, #52).
//
// A rematch trainer has a placeholder team in src/data/trainers.party (held
// items, nicknames, IVs, AI). When the battle starts, RR_AdjustRematchParty
// sets each Pokemon's species, level and moves from the current tier:
//   tier 0-8  the number of badges; levels sit at that badge count's level cap
//             (src/caps.c: 14, 21, 24, 29, 43, 43, 47, 50, 63)
//   tier 9    after the Elite Four (no cap then): a clear step up, Lv 75
// The team's aces are at the tier level, the others one below (two post-game).
// Rematches give no money and 1.5x EXP (a place to train instead of grinding).
//
// Oak + Daisy (#53): a double after the Route 22 6v6, in Oak's Lab. Oak brings
// the two starters left over in the lab (they depend on your pick) and the
// lab's Slowpoke; Daisy brings Muffin (a Rattata forever, with an Eviolite and
// better moves instead), Clefairy and Vulpix.
//
// Delia (#70): a double on her own at home, from the Route 22 6v6 on. Mimey
// leads (the ace); Oddish, Grimer, Miltank, a shiny Butterfree and Munchlax
// (the grocery bill), who only becomes a Snorlax after the Elite Four.
#include "global.h"
#include "event_data.h"
#include "move.h"
#include "pokemon.h"
#include "rr_rematch.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/species.h"

#define RR_POSTGAME_TIER 9
#define RR_NUM_TIERS     10

static const u8 sTierLevels[RR_NUM_TIERS] = {14, 21, 24, 29, 43, 43, 47, 50, 63, 75};

// 0-8: badges; 9: after the Elite Four
u32 RR_GetRematchTier(void)
{
    u32 i, badges = 0;

    if (FlagGet(FLAG_SYS_GAME_CLEAR))
        return RR_POSTGAME_TIER;
    for (i = FLAG_BADGE01_GET; i <= FLAG_BADGE08_GET; i++)
    {
        if (FlagGet(i))
            badges++;
    }
    return badges;
}

bool32 RR_IsRematchTrainer(u16 trainerId)
{
    return trainerId == TRAINER_RR_OAK_REMATCH || trainerId == TRAINER_RR_DAISY_REMATCH
        || trainerId == TRAINER_RR_DELIA_REMATCH;
}

// A starter line by tier: basic, middle stage from 1 badge, final from 4
struct StarterLine { u16 stage[3]; };
static const struct StarterLine sStarters[] =
{
    {{SPECIES_BULBASAUR, SPECIES_IVYSAUR, SPECIES_VENUSAUR}},
    {{SPECIES_CHARMANDER, SPECIES_CHARMELEON, SPECIES_CHARIZARD}},
    {{SPECIES_SQUIRTLE, SPECIES_WARTORTLE, SPECIES_BLASTOISE}},
    {{SPECIES_CHIKORITA, SPECIES_BAYLEEF, SPECIES_MEGANIUM}},
    {{SPECIES_CYNDAQUIL, SPECIES_QUILAVA, SPECIES_TYPHLOSION}},
    {{SPECIES_TOTODILE, SPECIES_CROCONAW, SPECIES_FERALIGATR}},
};
enum { BULBASAUR, CHARMANDER, SQUIRTLE, CHIKORITA, CYNDAQUIL, TOTODILE };

// Oak's two leftover starters for each pick (VAR_RR_STARTER, the lab's chain:
// data/scripts/research_red/rr_lab.inc): Kanto, then Johto
static const u8 sOakLeftovers[6][2] =
{
    [BULBASAUR]  = {SQUIRTLE, CHIKORITA},
    [CHARMANDER] = {BULBASAUR, CYNDAQUIL},
    [SQUIRTLE]   = {CHARMANDER, TOTODILE},
    [CHIKORITA]  = {BULBASAUR, TOTODILE},
    [CYNDAQUIL]  = {CHARMANDER, CHIKORITA},
    [TOTODILE]   = {SQUIRTLE, CYNDAQUIL},
};

static u16 StarterForTier(u32 line, u32 tier)
{
    return sStarters[line].stage[tier >= 4 ? 2 : tier >= 1 ? 1 : 0];
}

// The name follows the species, except for nicknamed Pokemon (Muffin)
static void SetMonSpeciesAndLevel(struct Pokemon *mon, u16 species, u32 level, bool32 keepNickname)
{
    u32 exp = gExperienceTables[gSpeciesInfo[species].growthRate][level];

    SetMonData(mon, MON_DATA_SPECIES, &species);
    if (!keepNickname)
        SetMonData(mon, MON_DATA_NICKNAME, GetSpeciesName(species));
    SetMonData(mon, MON_DATA_EXP, &exp);
    SetMonData(mon, MON_DATA_LEVEL, &level);
    CalculateMonStats(mon);
    GiveMonInitialMoveset(mon);
}

static void SetMove(struct Pokemon *mon, u32 slot, u16 move)
{
    u8 pp = GetMovePP(move);
    SetMonData(mon, MON_DATA_MOVE1 + slot, &move);
    SetMonData(mon, MON_DATA_PP1 + slot, &pp);
}

// Called after a trainer's team is created (src/battle_main.c)
void RR_AdjustRematchParty(struct Pokemon *party, u16 trainerId, u32 count)
{
    u32 tier = RR_GetRematchTier();
    u32 level = sTierLevels[tier];
    u32 below = level - (tier == RR_POSTGAME_TIER ? 2 : 1);
    u32 pick = VarGet(VAR_RR_STARTER) % 6;

    if (count < 3)
        return;
    if (trainerId == TRAINER_RR_OAK_REMATCH)
    {
        // the leftover starters are the aces; Slowpoke turns up as a
        // Galarian Slowking from 5 badges, and nobody can work out how
        SetMonSpeciesAndLevel(&party[0], StarterForTier(sOakLeftovers[pick][0], tier), level, FALSE);
        SetMonSpeciesAndLevel(&party[1], StarterForTier(sOakLeftovers[pick][1], tier), level, FALSE);
        SetMonSpeciesAndLevel(&party[2], tier >= 5 ? SPECIES_SLOWKING_GALAR : SPECIES_SLOWPOKE, below, FALSE);
    }
    else if (trainerId == TRAINER_RR_DAISY_REMATCH)
    {
        // Muffin never evolves: better moves instead
        SetMonSpeciesAndLevel(&party[0], SPECIES_RATTATA, below, TRUE);
        if (tier >= 1)
            SetMove(&party[0], 0, MOVE_SUPER_FANG);
        if (tier >= 3)
            SetMove(&party[0], 1, MOVE_SUCKER_PUNCH);
        if (tier >= 4)
            SetMove(&party[0], 2, MOVE_CRUNCH);
        SetMonSpeciesAndLevel(&party[1], tier >= 3 ? SPECIES_CLEFABLE : SPECIES_CLEFAIRY, below, FALSE);
        SetMonSpeciesAndLevel(&party[2], tier >= 4 ? SPECIES_NINETALES : SPECIES_VULPIX, below, FALSE);
    }
    else if (trainerId == TRAINER_RR_DELIA_REMATCH && count >= 6)
    {
        SetMonSpeciesAndLevel(&party[0], SPECIES_MR_MIME, level, TRUE); // Mimey
        SetMonSpeciesAndLevel(&party[1], tier >= 4 ? SPECIES_BELLOSSOM : tier >= 1 ? SPECIES_GLOOM : SPECIES_ODDISH, below, FALSE);
        SetMonSpeciesAndLevel(&party[2], tier >= 4 ? SPECIES_MUK : SPECIES_GRIMER, below, FALSE);
        SetMonSpeciesAndLevel(&party[3], SPECIES_MILTANK, below, FALSE);
        SetMonSpeciesAndLevel(&party[4], SPECIES_BUTTERFREE, below, FALSE);
        SetMonSpeciesAndLevel(&party[5], tier == RR_POSTGAME_TIER ? SPECIES_SNORLAX : SPECIES_MUNCHLAX, below, FALSE);
    }
}
