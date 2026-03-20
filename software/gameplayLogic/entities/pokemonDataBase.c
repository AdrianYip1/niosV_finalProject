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