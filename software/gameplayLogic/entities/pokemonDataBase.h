#pragma once
#include <stdbool.h>
#include <stddef.h>

#define NUM_TYPES 18

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

// pokemon
extern const PokemonData CHARMANDER;
extern const PokemonData CHARMELEON;
extern const PokemonData CHARIZARD;

// type chart — defined once in pokemonObject.c
extern const float typeChart[NUM_TYPES][NUM_TYPES];