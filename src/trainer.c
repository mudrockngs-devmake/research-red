#include "global.h"
#include "constants/trainers.h"
#include "event_data.h" // Research Red: FlagGet (FLAG_RR_BACK_PIC_FACING)

static enum TrainerPicID GetHoennTrainerPic(enum Gender gender)
{
    return gender == MALE ? TRAINER_PIC_RS_BRENDAN : TRAINER_PIC_RS_MAY;
}

static enum TrainerPicID GetKantoTrainerPic(enum Gender gender)
{
    // Research Red: you and Ivy side by side (like Elm and Jenny, #107), or
    // turned to each other (#7) with FLAG_RR_BACK_PIC_FACING (Debug > Scripts > 14)
    if (!FlagGet(FLAG_RR_BACK_PIC_FACING))
        return gender == MALE ? TRAINER_PIC_RR_PLAYER_M_IVY : TRAINER_PIC_RR_PLAYER_F_IVY;
    return gender == MALE ? TRAINER_PIC_RED : TRAINER_PIC_LEAF;
}

enum TrainerPicID GetPlayerTrainerPic(enum Gender gender, enum GameVersion version)
{
    switch (version)
    {
        case VERSION_SAPPHIRE:
        case VERSION_RUBY:
        case VERSION_EMERALD:
            return GetHoennTrainerPic(gender);
        case VERSION_LEAF_GREEN:
        case VERSION_FIRE_RED:
        default:
            return GetKantoTrainerPic(gender);
    }
}

STATIC_ASSERT(TRAINERS_COUNT <= MAX_TRAINERS_COUNT, tooManyTrainers);
