#include "pokemonDataBase.h"

const float typeChart[NUM_TYPES][NUM_TYPES] = {
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.0, 1.0, 1.0, 0.5, 1.0, 1.0}, // normal
    {1.0, 0.5, 0.5, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 0.5, 1.0, 2.0, 1.0, 1.0}, // fire
    {1.0, 2.0, 0.5, 1.0, 0.5, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0}, // water
    {1.0, 1.0, 2.0, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0, 2.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0}, // electric
    {1.0, 0.5, 2.0, 1.0, 0.5, 1.0, 1.0, 0.5, 2.0, 0.5, 1.0, 0.5, 2.0, 1.0, 0.5, 1.0, 0.5, 1.0, 1.0}, // grass
    {1.0, 0.5, 0.5, 1.0, 2.0, 0.5, 1.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0}, // ice
    {2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 0.5, 0.5, 0.5, 2.0, 0.0, 1.0, 2.0, 2.0, 0.5, 1.0}, // fighting
    {1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 0.5, 0.5, 1.0, 1.0, 0.0, 2.0, 1.0}, // poison
    {1.0, 2.0, 1.0, 2.0, 0.5, 1.0, 1.0, 2.0, 1.0, 0.0, 1.0, 0.5, 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0}, // ground
    {1.0, 1.0, 1.0, 0.5, 2.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0}, // flying
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 0.0, 0.5, 1.0, 1.0}, // psychic
    {1.0, 0.5, 1.0, 1.0, 2.0, 1.0, 0.5, 0.5, 1.0, 0.5, 2.0, 1.0, 1.0, 0.5, 1.0, 2.0, 0.5, 0.5, 1.0}, // bug
    {1.0, 2.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 0.5, 2.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0}, // rock
    {0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0, 1.0}, // ghost
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 0.0, 1.0}, // dragon
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 2.0, 2.0, 1.0, 0.5, 1.0, 0.5, 1.0, 0.5, 1.0}, // dark
    {1.0, 0.5, 0.5, 0.5, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 0.5, 2.0, 1.0}, // steel
    {1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 0.5, 0.5, 1.0}, // fairy
    {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, // none
};

const AttackData TACKLE = {1, "Tackle", 40, 100, 35, ATTACK_PHYSICAL, TYPE_NORMAL};
const AttackData SCRATCH = {2, "Scratch", 40, 100, 35, ATTACK_PHYSICAL, TYPE_NORMAL};
const AttackData GROWL = {3, "Growl", 0, 100, 40, ATTACK_STATUS, TYPE_NORMAL};
const AttackData SLASH = {4, "Slash", 70, 100, 20, ATTACK_PHYSICAL, TYPE_NORMAL};
const AttackData SCARY_FACE = {5, "Scary Face", 0, 100, 10, ATTACK_STATUS, TYPE_NORMAL};
const AttackData SMOKESCREEN = {6, "Smokescreen", 0, 100, 20, ATTACK_STATUS, TYPE_NORMAL};
const AttackData EMBER = {20, "Ember", 40, 100, 25, ATTACK_SPECIAL, TYPE_FIRE};
const AttackData FIRE_FANG = {21, "Fire Fang", 65,  95, 15, ATTACK_PHYSICAL, TYPE_FIRE};
const AttackData FLAMETHROWER ={22, "Flamethrower", 90, 100, 15, ATTACK_SPECIAL, TYPE_FIRE};
const AttackData FIRE_SPIN = {23, "Fire Spin", 35, 85, 15, ATTACK_SPECIAL, TYPE_FIRE};
const AttackData INFERNO = {24, "Inferno", 100, 50,  5, ATTACK_SPECIAL, TYPE_FIRE};
const AttackData HEAT_WAVE = {25, "Heat Wave", 95, 90, 10, ATTACK_SPECIAL, TYPE_FIRE};
const AttackData DRAGON_RAGE = {40, "Dragon Rage", 40, 100, 10, ATTACK_SPECIAL, TYPE_DRAGON};
const AttackData DRAGON_CLAW = {41, "Dragon Claw", 80, 100, 15, ATTACK_PHYSICAL, TYPE_DRAGON};
const AttackData WING_ATTACK = {50, "Wing Attack", 60, 100, 35, ATTACK_PHYSICAL, TYPE_FLYING};
const AttackData AIR_SLASH = {51, "Air Slash", 75, 95, 15, ATTACK_SPECIAL,  TYPE_FLYING};
const AttackData TWISTER = {52, "Twister", 40, 100, 20, ATTACK_SPECIAL,  TYPE_DRAGON};
const AttackData ANCIENT_POWER = {53, "Ancient Power", 60, 100, 5, ATTACK_SPECIAL,  TYPE_ROCK};
const AttackData DRAGON_DANCE = {54, "Dragon Dance", 0, 100, 20, ATTACK_STATUS,  TYPE_DRAGON};
const AttackData CRUNCH = {55, "Crunch", 80, 100, 15, ATTACK_PHYSICAL,  TYPE_DARK};
const AttackData FLY = {56, "Fly", 70, 95, 15, ATTACK_PHYSICAL,  TYPE_FLYING};
const AttackData REST = {57, "Rest", 0, 100, 5, ATTACK_STATUS,  TYPE_PSYCHIC};
const AttackData EXTREME_SPEED = {58, "Extreme Speed", 90, 100, 5, ATTACK_PHYSICAL,  TYPE_NORMAL};
const AttackData OUTRAGE = {59, "Outrage", 120, 90, 10, ATTACK_PHYSICAL,  TYPE_DRAGON};
const AttackData HYPERBEAM = {60, "Hyper Beam", 150, 90, 5, ATTACK_SPECIAL,  TYPE_NORMAL};
const AttackData METAL_CLAW = {61, "Metal Claw", 50, 95, 35, ATTACK_PHYSICAL, TYPE_STEEL};
const AttackData BONE_RUSH = {62, "Bone Rush", 55, 90, 10, ATTACK_PHYSICAL, TYPE_GROUND};
const AttackData EARTHQUAKE = {63, "Earthquake", 100, 100, 10, ATTACK_PHYSICAL, TYPE_GROUND};
const AttackData WATER_GUN = {64, "Water Gun", 40, 100, 25, ATTACK_SPECIAL, TYPE_WATER};
const AttackData SURF = {65, "Surf", 90, 100, 15, ATTACK_SPECIAL, TYPE_WATER};
const AttackData HYDRO_PUMP = {66, "Hydro Pump", 110, 80, 5, ATTACK_SPECIAL, TYPE_WATER};
const AttackData ICE_BEAM = {67, "Ice Beam", 90, 100, 10, ATTACK_SPECIAL, TYPE_ICE};
const AttackData AURA_SPHERE = {68, "Aura Sphere", 80, 100, 20, ATTACK_SPECIAL, TYPE_FIGHTING};
const AttackData CLOSE_COMBAT = {69, "Close Combat", 120, 100, 5, ATTACK_PHYSICAL, TYPE_FIGHTING};
const AttackData SLUDGE_BOMB = {70, "Sludge Bomb", 90, 100, 10, ATTACK_SPECIAL, TYPE_POISON};
const AttackData GIGA_DRAIN = {71, "Giga Drain", 75, 100, 10, ATTACK_SPECIAL, TYPE_GRASS};
const AttackData SHADOW_BALL = {72, "Shadow Ball", 80, 100, 15, ATTACK_SPECIAL, TYPE_GHOST};
const AttackData DAZZLING_GLEAM = {73, "Dazzling Gleam", 80, 100, 10, ATTACK_SPECIAL, TYPE_FAIRY};
const AttackData DARK_PULSE = {74, "Dark Pulse", 80, 100, 15, ATTACK_SPECIAL, TYPE_DARK};
const AttackData POISON_JAB = {75, "Poison Jab", 80, 100, 20, ATTACK_PHYSICAL, TYPE_POISON};
const AttackData LEAF_STORM = {76, "Leaf Storm", 130, 90, 5, ATTACK_SPECIAL, TYPE_GRASS};
const AttackData AQUA_TAIL = {77, "Aqua Tail", 90, 90, 10, ATTACK_PHYSICAL, TYPE_WATER};
const AttackData SUCKER_PUNCH = {78, "Sucker Punch", 70, 100, 5, ATTACK_PHYSICAL, TYPE_DARK};

const PokemonData CHARMANDER = {
    .id = 1, .name = "Charmander",
    .baseHp = 39, .baseAttack = 52, .baseSpAttack = 43,
    .baseDefense = 43, .baseSpDefense = 50, .baseSpeed = 65,
    .type1 = TYPE_FIRE, .type2 = TYPE_NONE,
    .evolution = {EVO_LEVEL, 16, &CHARMELEON}
};
const PokemonData CHARMELEON = {
    .id = 2, .name = "Charmeleon",
    .baseHp = 58, .baseAttack = 64, .baseSpAttack = 58,
    .baseDefense = 58, .baseSpDefense = 65, .baseSpeed = 80,
    .type1 = TYPE_FIRE, .type2 = TYPE_NONE,
    .evolution = {EVO_LEVEL, 36, &CHARIZARD}
};
const PokemonData CHARIZARD = {
    .id = 3, .name = "Charizard",
    .baseHp = 78, .baseAttack = 84, .baseSpAttack = 109,
    .baseDefense = 78, .baseSpDefense = 85, .baseSpeed = 100,
    .type1 = TYPE_FIRE, .type2 = TYPE_FLYING,
    .evolution = {EVO_NONE, 0, NULL}
};
const PokemonData RAYQUAZA = {
    .id = 4, .name = "Rayquaza",
    .baseHp = 105, .baseAttack = 150, .baseSpAttack = 150,
    .baseDefense = 90, .baseSpDefense = 90, .baseSpeed = 95,
    .type1 = TYPE_DRAGON, .type2 = TYPE_FLYING,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData GARCHOMP = {
    .id = 5, .name = "Garchomp",
    .baseHp = 108, .baseAttack = 130, .baseSpAttack = 80,
    .baseDefense = 95, .baseSpDefense = 85, .baseSpeed = 102,
    .type1 = TYPE_DRAGON, .type2 = TYPE_GROUND,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData LUCARIO = {
    .id = 6, .name = "Lucario",
    .baseHp = 70, .baseAttack = 110, .baseSpAttack = 115,
    .baseDefense = 70, .baseSpDefense = 70, .baseSpeed = 90,
    .type1 = TYPE_FIGHTING, .type2 = TYPE_STEEL,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData MILOTIC = {
    .id = 7, .name = "Milotic",
    .baseHp = 95, .baseAttack = 60, .baseSpAttack = 100,
    .baseDefense = 79, .baseSpDefense = 125, .baseSpeed = 81,
    .type1 = TYPE_WATER, .type2 = TYPE_NONE,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData ROSERADE = {
    .id = 8, .name = "Roserade",
    .baseHp = 60, .baseAttack = 70, .baseSpAttack = 125,
    .baseDefense = 65, .baseSpDefense = 105, .baseSpeed = 90,
    .type1 = TYPE_GRASS, .type2 = TYPE_POISON,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData SPIRITOMB = {
    .id = 9, .name = "Spiritomb",
    .baseHp = 50, .baseAttack = 92, .baseSpAttack = 92,
    .baseDefense = 108, .baseSpDefense = 108, .baseSpeed = 35,
    .type1 = TYPE_GHOST, .type2 = TYPE_DARK,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData TOGEKISS = {
    .id = 10, .name = "Togekiss",
    .baseHp = 85, .baseAttack = 50, .baseSpAttack = 120,
    .baseDefense = 95, .baseSpDefense = 115, .baseSpeed = 80,
    .type1 = TYPE_FAIRY, .type2 = TYPE_FLYING,
    .evolution = {EVO_NONE, 0, NULL}
};
