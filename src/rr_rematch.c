// Research Red: rematches that grow with your badges (#53, later #70, #52).
//
// A rematch trainer has a placeholder team in src/data/trainers.party (held
// items, nicknames, IVs, AI). When the battle starts, RR_AdjustRematchParty
// sets each Pokemon's species, level and moves from the current tier:
//   tier 0-8  the number of badges; levels sit at that badge count's level cap
//             (src/caps.c: 14, 21, 24, 29, 43, 43, 47, 50, 63)
//   tier 9    after the Elite Four (no cap then): a clear step up, Lv 75
// The team's aces are at the tier level, the others one below (two post-game).
// Owner's levels, 2026-10-30, before the Elite Four: Oak + Daisy's whole team
// sits halfway between the last cap and this one (sOakLevels: a place to train),
// Delia's whole team at the cap. Elm + Jenny's Pewter fight (trainers.party) is
// the cap just passed plus one: Lv 15 after Brock.
// Rematches give 1.5x EXP (a place to train instead of grinding); only Gym
// leaders pay prize money. After the Elite Four the teams bring regional forms,
// and the family Pokemon (Mimey, Muffin) stay home while an exchange-program
// visitor fights in their place (owner, 2026-10-20).
//
// Gym leaders (#52, Brock so far): talk to him after his badge and pick a tier
// up to your badge count (RR_ScriptSetChosenTier); his Gym team grows from there.
//
// Oak + Daisy (#53): after the Route 22 6v6, in Oak's Lab. One trainer with
// one team (#107): Oak's and Daisy's Pokemon take turns in the party (0, 2, 4
// Oak's; 1, 3, 5 Daisy's), so in doubles they lead together. Oak brings the
// two starters left over in the lab (they depend on your pick) and Nidoran M,
// a Nidoking later (his first Pokemon in the anime; the lab's Slowpoke never
// battles, #121, owner 2026-10-28); Daisy brings Muffin (a Rattata forever, with an Eviolite and
// better moves instead), Clefairy and Vulpix.
//
// Delia (#70): on her own at home, from the Route 22 6v6 on. Mimey
// leads (the ace); Oddish, Grimer, Miltank, a shiny Butterfree and Munchlax
// (the grocery bill), who only becomes a Snorlax after the Elite Four.
#include "global.h"
#include "battle.h"
#include "event_data.h"
#include "move.h"
#include "pokemon.h"
#include "rr_rematch.h"
#include "script.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/species.h"

#define RR_POSTGAME_TIER 9
#define RR_NUM_TIERS     10

static const u8 sTierLevels[RR_NUM_TIERS] = {14, 21, 24, 29, 43, 43, 47, 50, 63, 75};
static const u8 sOakLevels[RR_NUM_TIERS] = {10, 17, 22, 26, 36, 43, 45, 48, 56, 75};

// Tier picked from a Gym leader's menu, plus 1 (0: none, use your badges)
static EWRAM_DATA u8 sChosenTierPlusOne = 0;

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
    return trainerId == TRAINER_RR_OAK_REMATCH
        || trainerId == TRAINER_RR_DELIA_REMATCH || trainerId == TRAINER_RR_BROCK_REMATCH;
}

// Family rematches pay nothing (Oak gave the grant); Gym leaders do
bool32 RR_RematchGivesNoMoney(u16 trainerId)
{
    return RR_IsRematchTrainer(trainerId) && trainerId != TRAINER_RR_BROCK_REMATCH;
}

// VAR_RESULT = the highest tier you can pick (0-8 badges, 9 after the League)
void RR_ScriptGetRematchTier(struct ScriptContext *ctx)
{
    gSpecialVar_Result = RR_GetRematchTier();
}

// VAR_RESULT = the tier picked from the menu, for the next rematch battle
void RR_ScriptSetChosenTier(struct ScriptContext *ctx)
{
    sChosenTierPlusOne = gSpecialVar_Result < RR_NUM_TIERS ? gSpecialVar_Result + 1 : 0;
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
    u32 tier = (sChosenTierPlusOne != 0 && trainerId == TRAINER_RR_BROCK_REMATCH) ? sChosenTierPlusOne - 1 : RR_GetRematchTier();
    bool32 postgame;
    u32 level = sTierLevels[tier];
    u32 below = level - (tier == RR_POSTGAME_TIER ? 2 : 1);
    u32 pick = VarGet(VAR_RR_STARTER) % 6;

    // Only in trainer battles (#106): the game builds the enemy party through
    // here for wild battles too, with the last trainer's id still set and the
    // count of that trainer's team, so a wild battle after the Oak + Daisy
    // rematch got Oak's starters in place of the wild Pokemon.
    if (!(gBattleTypeFlags & BATTLE_TYPE_TRAINER) || !RR_IsRematchTrainer(trainerId) || count < 6)
        return;
    postgame = tier == RR_POSTGAME_TIER;
    if (trainerId == TRAINER_RR_OAK_REMATCH && !postgame)
        level = below = sOakLevels[tier];
    else if (trainerId == TRAINER_RR_DELIA_REMATCH && !postgame)
        below = level;
    if (trainerId == TRAINER_RR_OAK_REMATCH)
    {
        // Oak (0, 2, 4): the leftover starters are the aces; Nidoran M
        // grows like they do (Nidorino from 1 badge, Nidoking from 4)
        // post-game: a Cyndaquil-line starter comes back as a Hisuian Typhlosion
        u16 johto = StarterForTier(sOakLeftovers[pick][1], tier);
        if (postgame && johto == SPECIES_TYPHLOSION)
            johto = SPECIES_TYPHLOSION_HISUI;
        SetMonSpeciesAndLevel(&party[0], StarterForTier(sOakLeftovers[pick][0], tier), level, FALSE);
        SetMonSpeciesAndLevel(&party[2], johto, level, FALSE);
        SetMonSpeciesAndLevel(&party[4], tier >= 4 ? SPECIES_NIDOKING : tier >= 1 ? SPECIES_NIDORINO : SPECIES_NIDORAN_M, below, FALSE);

        // Daisy (1, 3, 5): Muffin never evolves: better moves instead.
        // Post-game she stays home and an Alolan Raticate on the exchange
        // program fights instead.
        if (postgame)
            SetMonSpeciesAndLevel(&party[1], SPECIES_RATICATE_ALOLA, below, FALSE);
        else
            SetMonSpeciesAndLevel(&party[1], SPECIES_RATTATA, below, TRUE);
        if (tier >= 1 && !postgame)
            SetMove(&party[1], 0, MOVE_SUPER_FANG);
        if (tier >= 3 && !postgame)
            SetMove(&party[1], 1, MOVE_SUCKER_PUNCH);
        if (tier >= 4 && !postgame)
            SetMove(&party[1], 2, MOVE_CRUNCH);
        SetMonSpeciesAndLevel(&party[3], tier >= 3 ? SPECIES_CLEFABLE : SPECIES_CLEFAIRY, below, FALSE);
        SetMonSpeciesAndLevel(&party[5], postgame ? SPECIES_NINETALES_ALOLA : tier >= 4 ? SPECIES_NINETALES : SPECIES_VULPIX, below, FALSE);
    }
    else if (trainerId == TRAINER_RR_DELIA_REMATCH)
    {
        // post-game: Mimey stays home; a Galarian Mr. Rime visits on an exchange
        if (postgame)
            SetMonSpeciesAndLevel(&party[0], SPECIES_MR_RIME, level, FALSE);
        else
            SetMonSpeciesAndLevel(&party[0], SPECIES_MR_MIME, level, TRUE); // Mimey
        SetMonSpeciesAndLevel(&party[1], tier >= 4 ? SPECIES_BELLOSSOM : tier >= 1 ? SPECIES_GLOOM : SPECIES_ODDISH, below, FALSE);
        SetMonSpeciesAndLevel(&party[2], postgame ? SPECIES_MUK_ALOLA : tier >= 4 ? SPECIES_MUK : SPECIES_GRIMER, below, FALSE);
        SetMonSpeciesAndLevel(&party[3], SPECIES_MILTANK, below, FALSE);
        SetMonSpeciesAndLevel(&party[4], SPECIES_BUTTERFREE, below, FALSE);
        SetMonSpeciesAndLevel(&party[5], postgame ? SPECIES_SNORLAX : SPECIES_MUNCHLAX, below, FALSE);
    }
    else if (trainerId == TRAINER_RR_BROCK_REMATCH)
    {
        // his Gym team, growing; Onix (then Steelix) is the ace
        SetMonSpeciesAndLevel(&party[0], postgame ? SPECIES_GOLEM_ALOLA : tier >= 4 ? SPECIES_GOLEM : tier >= 3 ? SPECIES_GRAVELER : SPECIES_GEODUDE, below, FALSE);
        SetMonSpeciesAndLevel(&party[1], postgame ? SPECIES_SANDSLASH_ALOLA : tier >= 2 ? SPECIES_SANDSLASH : SPECIES_SANDSHREW, below, FALSE);
        SetMonSpeciesAndLevel(&party[2], tier >= 4 ? SPECIES_KABUTOPS : SPECIES_KABUTO, below, FALSE);
        SetMonSpeciesAndLevel(&party[3], tier >= 4 ? SPECIES_OMASTAR : SPECIES_OMANYTE, below, FALSE);
        SetMonSpeciesAndLevel(&party[4], postgame ? SPECIES_RHYPERIOR : tier >= 4 ? SPECIES_RHYDON : SPECIES_RHYHORN, below, FALSE);
        SetMonSpeciesAndLevel(&party[5], tier >= 4 ? SPECIES_STEELIX : SPECIES_ONIX, level, FALSE);
    }
}
