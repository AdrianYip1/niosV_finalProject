#pragma once
#include <stdbool.h>
#include <stddef.h>

#define NUM_TYPES 19

typedef enum {
    TYPE_NORMAL = 0,
    TYPE_FIRE,
    TYPE_WATER,
    TYPE_ELECTRIC,
    TYPE_GRASS,
    TYPE_ICE,
    TYPE_FIGHTING,
    TYPE_POISON,
    TYPE_GROUND,
    TYPE_FLYING,
    TYPE_PSYCHIC,
    TYPE_BUG,
    TYPE_ROCK,
    TYPE_GHOST,
    TYPE_DRAGON,
    TYPE_DARK,
    TYPE_STEEL,
    TYPE_FAIRY,
    TYPE_NONE
} PokemonType;

typedef enum {
    ATTACK_PHYSICAL = 0,
    ATTACK_SPECIAL = 1,
    ATTACK_STATUS = 2
} AttackCategory;

typedef struct {
    int id;
    const char *name;
    int power;
    int accuracy;
    int maxPP;
    AttackCategory category;
    PokemonType type;
} AttackData;

typedef enum {
    EVO_NONE  = 0,
    EVO_LEVEL,
    EVO_ITEM,
} EvoTrigger;

typedef struct PokemonData PokemonData;

typedef struct {
    EvoTrigger trigger;
    int levelThreshold;
    const PokemonData *into;
} EvolutionData;

struct PokemonData {
    int id;
    const char *name;
    int baseHp;
    int baseAttack;
    int baseSpAttack;
    int baseDefense;
    int baseSpDefense;
    int baseSpeed;
    PokemonType type1;
    PokemonType type2;
    EvolutionData evolution;
};

static inline const PokemonData *checkEvolution(const PokemonData *species, int level) {
    if (species->evolution.trigger == EVO_LEVEL &&
        level >= species->evolution.levelThreshold) {
        return species->evolution.into;
    }
    return NULL;
}

// attacks
extern const AttackData TACKLE;
extern const AttackData SCRATCH;
extern const AttackData GROWL;
extern const AttackData SLASH;
extern const AttackData SCARY_FACE;
extern const AttackData SMOKESCREEN;
extern const AttackData EMBER;
extern const AttackData FIRE_FANG;
extern const AttackData FLAMETHROWER;
extern const AttackData FIRE_SPIN;
extern const AttackData INFERNO;
extern const AttackData HEAT_WAVE;
extern const AttackData DRAGON_RAGE;
extern const AttackData DRAGON_CLAW;
extern const AttackData WING_ATTACK;
extern const AttackData AIR_SLASH;

extern const AttackData TWISTER;
extern const AttackData ANCIENT_POWER;
extern const AttackData DRAGON_DANCE;
extern const AttackData CRUNCH;
extern const AttackData FLY;
extern const AttackData REST;
extern const AttackData EXTREME_SPEED;
extern const AttackData OUTRAGE;
extern const AttackData HYPERBEAM;
extern const AttackData METAL_CLAW;
extern const AttackData BONE_RUSH;
extern const AttackData EARTHQUAKE;
extern const AttackData WATER_GUN;
extern const AttackData SURF;
extern const AttackData HYDRO_PUMP;
extern const AttackData ICE_BEAM;
extern const AttackData AURA_SPHERE;
extern const AttackData CLOSE_COMBAT;
extern const AttackData SLUDGE_BOMB;
extern const AttackData GIGA_DRAIN;
extern const AttackData SHADOW_BALL;
extern const AttackData DAZZLING_GLEAM;
extern const AttackData DARK_PULSE;
extern const AttackData POISON_JAB;
extern const AttackData LEAF_STORM;
extern const AttackData AQUA_TAIL;
extern const AttackData SUCKER_PUNCH;


// pokemon
extern const PokemonData CHARMANDER;
extern const PokemonData CHARMELEON;
extern const PokemonData CHARIZARD;
extern const PokemonData RAYQUAZA;
extern const PokemonData GIBLE;
extern const PokemonData GABITE;
extern const PokemonData GARCHOMP;
extern const PokemonData RIOLU;
extern const PokemonData LUCARIO;
extern const PokemonData FEEBAS;
extern const PokemonData MILOTIC;
extern const PokemonData BUDEW;
extern const PokemonData ROSERADE;
extern const PokemonData SPIRITOMB;
extern const PokemonData TOGEPI;
extern const PokemonData TOGETIC;
extern const PokemonData TOGEKISS;

// type chart
extern const float typeChart[NUM_TYPES][NUM_TYPES];
