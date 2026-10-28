// Research Red (#112): the whole type chart against the official one (Gen 6+),
// so nothing we change can quietly break super / not very / no effect.
// One row per attacking type; the columns are the defending types in this order:
//   Normal Fighting Flying Poison Ground Rock Bug Ghost Steel
//   Fire Water Grass Electric Psychic Ice Dragon Dark Fairy
// '.' normal, '+' super effective (x2), '-' not very effective (x0.5), '0' no effect.
#include "global.h"
#include "test/battle.h"

static const enum Type sRRTypes[] =
{
    TYPE_NORMAL, TYPE_FIGHTING, TYPE_FLYING, TYPE_POISON, TYPE_GROUND, TYPE_ROCK, TYPE_BUG, TYPE_GHOST, TYPE_STEEL,
    TYPE_FIRE, TYPE_WATER, TYPE_GRASS, TYPE_ELECTRIC, TYPE_PSYCHIC, TYPE_ICE, TYPE_DRAGON, TYPE_DARK, TYPE_FAIRY,
};

static const char *const sRROfficialChart[] =
{
    ".....-.0-.........", // Normal
    "+.--.+-0+....-+.+-", // Fighting
    ".+...-+.-..+-.....", // Flying
    "...---.-0..+.....+", // Poison
    "..0+.+-.++.-+.....", // Ground
    ".-+.-.+.-+....+...", // Rock
    ".---...---.+.+..+-", // Bug
    "0......+.....+..-.", // Ghost
    ".....+..---.-.+..+", // Steel
    ".....-+.+--+..+-..", // Fire
    "....++...+--...-..", // Water
    "..--++-.--+-...-..", // Grass
    "..+.0.....+--..-..", // Electric
    ".+.+....-....-..0.", // Psychic
    "..+.+...---+..-+..", // Ice
    "........-......+.0", // Dragon
    ".-.....+.....+..--", // Dark
    ".+.-....--.....++.", // Fairy
};

TEST("Research Red: every type matchup matches the official chart (#112)")
{
    u32 atk = 0, def;

    for (u32 i = 0; i < ARRAY_COUNT(sRRTypes); i++)
    {
        PARAMETRIZE_LABEL("%S attacking", gTypesInfo[sRRTypes[i]].name) { atk = i; }
    }
    for (def = 0; def < ARRAY_COUNT(sRRTypes); def++)
    {
        uq4_12_t want;
        switch (sRROfficialChart[atk][def])
        {
        case '+': want = UQ_4_12(2.0); break;
        case '-': want = UQ_4_12(0.5); break;
        case '0': want = UQ_4_12(0.0); break;
        default:  want = UQ_4_12(1.0); break;
        }
        EXPECT_EQ(GetTypeModifier(sRRTypes[atk], sRRTypes[def]), want);
    }
}
