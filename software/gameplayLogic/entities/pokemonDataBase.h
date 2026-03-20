#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef enum {
    TYPE_NORMAL = 0,
    TYPE_FIRE,
    TYPE_WATER,
    TYPE_GRASS,
    TYPE_ELECTRIC,
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
    TYPE_NONE   // for pokemon with 1 type
} PokemonType;


typedef enum {
    ATTACK_PHYSICAL = 0,
    ATTACK_SPECIAL  = 1,
    ATTACK_STATUS   = 2
} AttackCategory;

typedef struct {
    int            id;
    const char    *name;
    int            power;
    int            accuracy;
    int            maxPP;
    AttackCategory category;
    PokemonType    type;
} AttackData;

typedef enum {
    EVO_NONE  = 0,
    EVO_LEVEL,  
    EVO_ITEM,      
} EvoTrigger;

typedef struct PokemonData PokemonData;

typedef struct {
    EvoTrigger        trigger;
    int               levelThreshold;  
    const PokemonData *into;         
} EvolutionData;

struct PokemonData {
    int               id;
    const char       *name;
    int               baseHp;
    int               baseAttack;
    int               baseSpAttack;
    int               baseDefense;
    int               baseSpDefense;
    int               baseSpeed;
    PokemonType       type1;
    PokemonType       type2;
    const AttackData *moves[4];
    EvolutionData     evolution;
};

static inline const PokemonData *checkEvolution(const PokemonData *species, int level) {
    if (species->evolution.trigger == EVO_LEVEL &&
        level >= species->evolution.levelThreshold) {
        return species->evolution.into;
    }
    return NULL;
}


// ATTACK DATABASE

//normal type
static const AttackData TACKLE = {
    .id = 1, 
    .name = "Tackle",
    .power = 40, 
    .accuracy = 100, 
    .maxPP = 35,
    .category = ATTACK_PHYSICAL, 
    .type = TYPE_NORMAL
};
static const AttackData SCRATCH = {
    .id = 2, 
    .name = "Scratch",
    .power = 40, 
    .accuracy = 100, 
    .maxPP = 35,
    .category = ATTACK_PHYSICAL, 
    .type = TYPE_NORMAL
};
static const AttackData GROWL = {
    .id = 3, 
    .name = "Growl",
    .power = 0, .accuracy = 100, 
    .maxPP = 40,
    .category = ATTACK_STATUS, 
    .type = TYPE_NORMAL
};
static const AttackData SLASH = {
    .id = 4, 
    .name = "Slash",
    .power = 70, 
    .accuracy = 100, 
    .maxPP = 20,
    .category = ATTACK_PHYSICAL, 
    .type = TYPE_NORMAL
};
static const AttackData SCARY_FACE = {
    .id = 5, 
    .name = "Scary Face",
    .power = 0, .accuracy = 100, 
    .maxPP = 10,
    .category = ATTACK_STATUS, 
    .type = TYPE_NORMAL
};

// fire type
static const AttackData EMBER = {
    .id = 20, 
    .name = "Ember",
    .power = 40, 
    .accuracy = 100, 
    .maxPP = 25,
    .category = ATTACK_SPECIAL, 
    .type = TYPE_FIRE
};
static const AttackData FIRE_FANG = {
    .id = 21, 
    .name = "Fire Fang",
    .power = 65, 
    .accuracy = 95, 
    .maxPP = 15,
    .category = ATTACK_PHYSICAL, 
    .type = TYPE_FIRE
};
static const AttackData FLAMETHROWER = {
    .id = 22, 
    .name = "Flamethrower",
    .power = 90, 
    .accuracy = 100, 
    .maxPP = 15,
    .category = ATTACK_SPECIAL, 
    .type = TYPE_FIRE
};
static const AttackData FIRE_SPIN = {
    .id = 23, 
    .name = "Fire Spin",
    .power = 35, 
    .accuracy = 85, 
    .maxPP = 15,
    .category = ATTACK_SPECIAL, 
    .type = TYPE_FIRE
};
static const AttackData INFERNO = {
    .id = 24, 
    .name = "Inferno",
    .power = 100, 
    .accuracy = 50, 
    .maxPP = 5,
    .category = ATTACK_SPECIAL, 
    .type = TYPE_FIRE
};
static const AttackData HEAT_WAVE = {
    .id = 25, 
    .name = "Heat Wave",
    .power = 95, 
    .accuracy = 90, 
    .maxPP = 10,
    .category = ATTACK_SPECIAL, 
    .type = TYPE_FIRE
};

//dragon type
static const AttackData DRAGON_RAGE = {
    .id = 40, 
    .name = "Dragon Rage",
    .power = 40, 
    .accuracy = 100, 
    .maxPP = 10,
    .category = ATTACK_SPECIAL, 
    .type = TYPE_DRAGON
};
static const AttackData DRAGON_CLAW = {
    .id = 41, 
    .name = "Dragon Claw",
    .power = 80, 
    .accuracy = 100, 
    .maxPP = 15,
    .category = ATTACK_PHYSICAL, 
    .type = TYPE_DRAGON
};

//flying type
static const AttackData WING_ATTACK = {
    .id = 50, 
    .name = "Wing Attack",
    .power = 60, 
    .accuracy = 100, 
    .maxPP = 35,
    .category = ATTACK_PHYSICAL, 
    .type = TYPE_FLYING
};
static const AttackData AIR_SLASH = {
    .id = 51, 
    .name = "Air Slash",
    .power = 75, 
    .accuracy = 95, 
    .maxPP = 15,
    .category = ATTACK_SPECIAL, 
    .type = TYPE_FLYING
};

static const PokemonData CHARMANDER;
static const PokemonData CHARMELEON;
static const PokemonData CHARIZARD;

// pokemon database

static const PokemonData CHARMANDER = {
    .id           = 1,
    .name         = "Charmander",
    .baseHp       = 39,
    .baseAttack   = 52,
    .baseSpAttack = 43,
    .baseDefense  = 43,
    .baseSpDefense= 50,
    .baseSpeed    = 65,
    .type1        = TYPE_FIRE,
    .type2        = TYPE_NONE,
    .moves        = { &SCRATCH, &GROWL, &EMBER, &TACKLE },
    .evolution    = { .trigger = EVO_LEVEL, .levelThreshold = 16, .into = &CHARMELEON } //evoles at elvel 16
};

static const PokemonData CHARMELEON = {
    .id           = 2,
    .name         = "Charmeleon",
    .baseHp       = 58,
    .baseAttack   = 64,
    .baseSpAttack = 58,
    .baseDefense  = 58,
    .baseSpDefense= 65,
    .baseSpeed    = 80,
    .type1        = TYPE_FIRE,
    .type2        = TYPE_NONE,
    .moves        = { &SCRATCH, &EMBER, &SCARY_FACE, &FIRE_FANG },
    .evolution    = { .trigger = EVO_LEVEL, .levelThreshold = 36, .into = &CHARIZARD } //evolves at level 36
};

static const PokemonData CHARIZARD = {
    .id           = 3,
    .name         = "Charizard",
    .baseHp       = 78,
    .baseAttack   = 84,
    .baseSpAttack = 109,
    .baseDefense  = 78,
    .baseSpDefense= 85,
    .baseSpeed    = 100,
    .type1        = TYPE_FIRE,
    .type2        = TYPE_FLYING,
    .moves        = { &FLAMETHROWER, &AIR_SLASH, &DRAGON_CLAW, &HEAT_WAVE },
    .evolution    = { .trigger = EVO_NONE, .levelThreshold = 0, .into = NULL }
};