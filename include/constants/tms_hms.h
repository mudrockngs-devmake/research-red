#ifndef GUARD_CONSTANTS_TMS_HMS_H
#define GUARD_CONSTANTS_TMS_HMS_H

#define FOREACH_TM(F) \
    F(FOCUS_PUNCH) \
    F(DRAGON_CLAW) \
    F(WATER_PULSE) \
    F(CALM_MIND) \
    F(ROAR) \
    F(TOXIC) \
    F(HAIL) \
    F(BULK_UP) \
    F(BULLET_SEED) \
    F(HIDDEN_POWER) \
    F(SUNNY_DAY) \
    F(TAUNT) \
    F(ICE_BEAM) \
    F(BLIZZARD) \
    F(HYPER_BEAM) \
    F(LIGHT_SCREEN) \
    F(PROTECT) \
    F(RAIN_DANCE) \
    F(GIGA_DRAIN) \
    F(SAFEGUARD) \
    F(FRUSTRATION) \
    F(SOLAR_BEAM) \
    F(IRON_TAIL) \
    F(THUNDERBOLT) \
    F(THUNDER) \
    F(EARTHQUAKE) \
    F(RETURN) \
    F(DIG) \
    F(PSYCHIC) \
    F(SHADOW_BALL) \
    F(BRICK_BREAK) \
    F(DOUBLE_TEAM) \
    F(REFLECT) \
    F(SHOCK_WAVE) \
    F(FLAMETHROWER) \
    F(SLUDGE_BOMB) \
    F(SANDSTORM) \
    F(FIRE_BLAST) \
    F(ROCK_TOMB) \
    F(AERIAL_ACE) \
    F(TORMENT) \
    F(FACADE) \
    F(SECRET_POWER) \
    F(REST) \
    F(ATTRACT) \
    F(THIEF) \
    F(STEEL_WING) \
    F(SKILL_SWAP) \
    F(SNATCH) \
    F(OVERHEAT) \
    /* Research Red (#90): TM51-65 were FireRed's move tutors (#57); the */ \
    /* relearner's Tutor Moves teach those for free, so these slots hold */ \
    /* moves you couldn't get otherwise */ \
    F(LOW_KICK) \
    F(KNOCK_OFF) \
    F(FOUL_PLAY) \
    F(NASTY_PLOT) \
    F(AGILITY) \
    F(ROOST) \
    F(HEAT_WAVE) \
    F(MUD_SHOT) \
    F(DRILL_RUN) \
    F(POWER_GEM) \
    F(SMART_STRIKE) \
    F(IRON_DEFENSE) \
    F(BODY_PRESS) \
    F(CHARGE_BEAM) \
    F(GRASS_KNOT) \
    /* Research Red (#57): TM66-100, modern moves */ \
    F(HYPER_VOICE) \
    F(WILL_O_WISP) \
    F(FIRE_FANG) /* Research Red (#90): was Fire Punch (a tutor move) */ \
    F(SCALD) \
    F(LIQUIDATION) \
    F(ENERGY_BALL) \
    F(SEED_BOMB) \
    F(VOLT_SWITCH) \
    F(WILD_CHARGE) \
    F(THUNDER_FANG) /* Research Red (#90): was Thunder Punch */ \
    F(ICE_FANG) /* Research Red (#90): was Ice Punch */ \
    F(POUNCE) /* Research Red (#90): was Icy Wind */ \
    F(DRAIN_PUNCH) \
    F(AURA_SPHERE) \
    F(POISON_JAB) \
    F(EARTH_POWER) \
    F(BULLDOZE) \
    F(AIR_SLASH) \
    F(PSYSHOCK) \
    F(ZEN_HEADBUTT) \
    F(TRICK_ROOM) \
    F(U_TURN) \
    F(X_SCISSOR) \
    F(BUG_BUZZ) \
    F(STONE_EDGE) \
    F(STEALTH_ROCK) \
    F(SHADOW_CLAW) \
    F(HEX) \
    F(DRAGON_PULSE) \
    F(DARK_PULSE) \
    F(CRUNCH) \
    F(FLASH_CANNON) \
    F(IRON_HEAD) \
    F(DAZZLING_GLEAM) \
    F(PLAY_ROUGH)

#define FOREACH_HM(F) \
    F(CUT) \
    F(FLY) \
    F(SURF) \
    F(STRENGTH) \
    F(FLASH) \
    F(ROCK_SMASH) \
    F(WATERFALL) \
    F(DIVE)

#define FOREACH_TMHM(F) \
    FOREACH_TM(F) \
    FOREACH_HM(F)

#endif
