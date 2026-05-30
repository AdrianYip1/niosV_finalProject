#include "pokemonDataBase.h"
#include "../../graphics/sprites/pokemon/pokemonSpriteInit.h"

/*
 All game data (type chart, moves, Pokemon stats) is stored in static
 compile-time tables
 Intentional becasue the DE1-SoC runs without an OS or memory manager,
 so all data must be baked into the binary at a fixed address.
 */

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

const AttackData TACKLE = {1, "Tackle", "A physical attack in which the user charges.", 40, 100, 35, ATTACK_PHYSICAL, TYPE_NORMAL};
const AttackData SCRATCH = {2, "Scratch", "Sharp claws rake the target.", 40, 100, 35, ATTACK_PHYSICAL, TYPE_NORMAL};
const AttackData GROWL = {3, "Growl", "The user growls to lower the foe's power.", 0, 100, 40, ATTACK_STATUS, TYPE_NORMAL};
const AttackData SLASH = {4, "Slash", "Slashes with claws. High critical-hit ratio.", 70, 100, 20, ATTACK_PHYSICAL, TYPE_NORMAL};
const AttackData SCARY_FACE = {5, "Scary Face", "Frightens the target to harshly lower Speed.", 0, 100, 10, ATTACK_STATUS, TYPE_NORMAL};
const AttackData SMOKESCREEN = {6, "Smokescreen", "Lowers the foe's accuracy with smoke.", 0, 100, 20, ATTACK_STATUS, TYPE_NORMAL};
const AttackData EMBER = {20, "Ember", "A weak fire attack that may burn the foe.", 40, 100, 25, ATTACK_SPECIAL, TYPE_FIRE};
const AttackData FIRE_FANG = {21, "Fire Fang", "Bites with flame. May burn or flinch.", 65,  95, 15, ATTACK_PHYSICAL, TYPE_FIRE};
const AttackData FLAMETHROWER ={22, "Flamethrower", "Scorches the target with intense flames.", 90, 100, 15, ATTACK_SPECIAL, TYPE_FIRE};
const AttackData FIRE_SPIN = {23, "Fire Spin", "Traps the target in a vortex of fire.", 35, 85, 15, ATTACK_SPECIAL, TYPE_FIRE};
const AttackData INFERNO = {24, "Inferno", "Engulfs the target in a fiery blast.", 100, 50,  5, ATTACK_SPECIAL, TYPE_FIRE};
const AttackData HEAT_WAVE = {25, "Heat Wave", "Exhales hot breath to attack everything nearby.", 95, 90, 10, ATTACK_SPECIAL, TYPE_FIRE};
const AttackData DRAGON_RAGE = {40, "Dragon Rage", "A burst of dragon energy to strike the foe.", 40, 100, 10, ATTACK_SPECIAL, TYPE_DRAGON};
const AttackData DRAGON_CLAW = {41, "Dragon Claw", "Slashes the foe with sharp dragon claws.", 80, 100, 15, ATTACK_PHYSICAL, TYPE_DRAGON};
const AttackData WING_ATTACK = {50, "Wing Attack", "Strikes the target with wings.", 60, 100, 35, ATTACK_PHYSICAL, TYPE_FLYING};
const AttackData AIR_SLASH = {51, "Air Slash", "Attacks with a blade of air. May flinch.", 75, 95, 15, ATTACK_SPECIAL,  TYPE_FLYING};
const AttackData TWISTER = {52, "Twister", "Whips up a tornado to strike the foe.", 40, 100, 20, ATTACK_SPECIAL,  TYPE_DRAGON};
const AttackData ANCIENT_POWER = {53, "Ancient Power", "Attacks with ancient power. May raise stats.", 60, 100, 5, ATTACK_SPECIAL,  TYPE_ROCK};
const AttackData DRAGON_DANCE = {54, "Dragon Dance", "A mystic dance that boosts Attack and Speed.", 0, 100, 20, ATTACK_STATUS,  TYPE_DRAGON};
const AttackData CRUNCH = {55, "Crunch", "Crunches with sharp fangs. May lower Defense.", 80, 100, 15, ATTACK_PHYSICAL,  TYPE_DARK};
const AttackData FLY = {56, "Fly", "Soars up, then strikes on the next turn.", 70, 95, 15, ATTACK_PHYSICAL,  TYPE_FLYING};
const AttackData REST = {57, "Rest", "The user sleeps to restore HP and heal status.", 0, 100, 5, ATTACK_STATUS,  TYPE_PSYCHIC};
const AttackData EXTREME_SPEED = {58, "Extreme Speed", "A powerful priority attack that strikes first.", 90, 100, 5, ATTACK_PHYSICAL,  TYPE_NORMAL};
const AttackData OUTRAGE = {59, "Outrage", "Ramps up, then attacks wildly for a while.", 120, 90, 10, ATTACK_PHYSICAL,  TYPE_DRAGON};
const AttackData HYPERBEAM = {60, "Hyper Beam", "A powerful beam. The user must recharge.", 150, 90, 5, ATTACK_SPECIAL,  TYPE_NORMAL};
const AttackData METAL_CLAW = {61, "Metal Claw", "Rakes the foe with metal claws. May raise Attack.", 50, 95, 35, ATTACK_PHYSICAL, TYPE_STEEL};
const AttackData BONE_RUSH = {62, "Bone Rush", "Strikes the foe with a bone club.", 55, 90, 10, ATTACK_PHYSICAL, TYPE_GROUND};
const AttackData EARTHQUAKE = {63, "Earthquake", "A powerful quake that hits everything nearby.", 100, 100, 10, ATTACK_PHYSICAL, TYPE_GROUND};
const AttackData WATER_GUN = {64, "Water Gun", "Shoots water to strike the target.", 40, 100, 25, ATTACK_SPECIAL, TYPE_WATER};
const AttackData SURF = {65, "Surf", "Creates a huge wave to crash down on the foe.", 90, 100, 15, ATTACK_SPECIAL, TYPE_WATER};
const AttackData HYDRO_PUMP = {66, "Hydro Pump", "Blasts water at high power, but may miss.", 110, 80, 5, ATTACK_SPECIAL, TYPE_WATER};
const AttackData ICE_BEAM = {67, "Ice Beam", "Fires an icy beam that may freeze the target.", 90, 100, 10, ATTACK_SPECIAL, TYPE_ICE};
const AttackData AURA_SPHERE = {68, "Aura Sphere", "Fires a blast that never misses.", 80, 100, 20, ATTACK_SPECIAL, TYPE_FIGHTING};
const AttackData CLOSE_COMBAT = {69, "Close Combat", "A full-power attack that lowers defenses.", 120, 100, 5, ATTACK_PHYSICAL, TYPE_FIGHTING};
const AttackData SLUDGE_BOMB = {70, "Sludge Bomb", "Hurls sludge at the foe. May poison.", 90, 100, 10, ATTACK_SPECIAL, TYPE_POISON};
const AttackData GIGA_DRAIN = {71, "Giga Drain", "Drains the target's HP to heal the user.", 75, 100, 10, ATTACK_SPECIAL, TYPE_GRASS};
const AttackData SHADOW_BALL = {72, "Shadow Ball", "Hurls a shadowy blob. May lower Sp. Def.", 80, 100, 15, ATTACK_SPECIAL, TYPE_GHOST};
const AttackData DAZZLING_GLEAM = {73, "Dazzling Gleam", "Damages the foe by emitting a dazzling flash.", 80, 100, 10, ATTACK_SPECIAL, TYPE_FAIRY};
const AttackData DARK_PULSE = {74, "Dark Pulse", "Attacks with a dark aura. May make the foe flinch.", 80, 100, 15, ATTACK_SPECIAL, TYPE_DARK};
const AttackData POISON_JAB = {75, "Poison Jab", "Stabs the target with a poisoned barb. May poison.", 80, 100, 20, ATTACK_PHYSICAL, TYPE_POISON};
const AttackData LEAF_STORM = {76, "Leaf Storm", "Whips up a storm of leaves to strike the foe.", 130, 90, 5, ATTACK_SPECIAL, TYPE_GRASS};
const AttackData AQUA_TAIL = {77, "Aqua Tail", "Attacks by swinging a tail like a wave.", 90, 90, 10, ATTACK_PHYSICAL, TYPE_WATER};
const AttackData SUCKER_PUNCH = {78, "Sucker Punch", "Strikes first if the foe is preparing an attack.", 70, 100, 5, ATTACK_PHYSICAL, TYPE_DARK};

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
    .id = POKEMON_ID_RAYQUAZA, .name = "Rayquaza",
    .baseHp = 105, .baseAttack = 150, .baseSpAttack = 150,
    .baseDefense = 90, .baseSpDefense = 90, .baseSpeed = 95,
    .type1 = TYPE_DRAGON, .type2 = TYPE_FLYING,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData GIBLE = {
    .id = POKEMON_ID_GIBLE, .name = "Gible",
    .baseHp = 58, .baseAttack = 70, .baseSpAttack = 40,
    .baseDefense = 45, .baseSpDefense = 45, .baseSpeed = 42,
    .type1 = TYPE_DRAGON, .type2 = TYPE_GROUND,
    .evolution = {EVO_LEVEL, 24, &GABITE}
};

const PokemonData GABITE = {
    .id = POKEMON_ID_GABITE, .name = "Gabite",
    .baseHp = 68, .baseAttack = 90, .baseSpAttack = 50,
    .baseDefense = 65, .baseSpDefense = 55, .baseSpeed = 82,
    .type1 = TYPE_DRAGON, .type2 = TYPE_GROUND,
    .evolution = {EVO_LEVEL, 48, &GARCHOMP}
};

const PokemonData GARCHOMP = {
    .id = POKEMON_ID_GARCHOMP, .name = "Garchomp",
    .baseHp = 108, .baseAttack = 130, .baseSpAttack = 80,
    .baseDefense = 95, .baseSpDefense = 85, .baseSpeed = 102,
    .type1 = TYPE_DRAGON, .type2 = TYPE_GROUND,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData RIOLU = {
    .id = POKEMON_ID_RIOLU, .name = "Riolu",
    .baseHp = 40, .baseAttack = 70, .baseSpAttack = 35,
    .baseDefense = 40, .baseSpDefense = 40, .baseSpeed = 60,
    .type1 = TYPE_FIGHTING, .type2 = TYPE_NONE,
    .evolution = {EVO_LEVEL, 20, &LUCARIO}
};

const PokemonData LUCARIO = {
    .id = POKEMON_ID_LUCARIO, .name = "Lucario",
    .baseHp = 70, .baseAttack = 110, .baseSpAttack = 115,
    .baseDefense = 70, .baseSpDefense = 70, .baseSpeed = 90,
    .type1 = TYPE_FIGHTING, .type2 = TYPE_STEEL,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData FEEBAS = {
    .id = POKEMON_ID_FEEBAS, .name = "Feebas",
    .baseHp = 20, .baseAttack = 15, .baseSpAttack = 10,
    .baseDefense = 20, .baseSpDefense = 55, .baseSpeed = 80,
    .type1 = TYPE_WATER, .type2 = TYPE_NONE,
    .evolution = {EVO_LEVEL, 20, &MILOTIC}
};

const PokemonData MILOTIC = {
    .id = POKEMON_ID_MILOTIC, .name = "Milotic",
    .baseHp = 95, .baseAttack = 60, .baseSpAttack = 100,
    .baseDefense = 79, .baseSpDefense = 125, .baseSpeed = 81,
    .type1 = TYPE_WATER, .type2 = TYPE_NONE,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData BUDEW = {
    .id = POKEMON_ID_BUDEW, .name = "Budew",
    .baseHp = 40, .baseAttack = 30, .baseSpAttack = 40,
    .baseDefense = 35, .baseSpDefense = 55, .baseSpeed = 55,
    .type1 = TYPE_GRASS, .type2 = TYPE_POISON,
    .evolution = {EVO_LEVEL, 20, &ROSERADE}
};

const PokemonData ROSERADE = {
    .id = POKEMON_ID_ROSERADE, .name = "Roserade",
    .baseHp = 60, .baseAttack = 70, .baseSpAttack = 125,
    .baseDefense = 65, .baseSpDefense = 105, .baseSpeed = 90,
    .type1 = TYPE_GRASS, .type2 = TYPE_POISON,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData SPIRITOMB = {
    .id = POKEMON_ID_SPIRITOMB, .name = "Spiritomb",
    .baseHp = 50, .baseAttack = 92, .baseSpAttack = 92,
    .baseDefense = 108, .baseSpDefense = 108, .baseSpeed = 35,
    .type1 = TYPE_GHOST, .type2 = TYPE_DARK,
    .evolution = {EVO_NONE, 0, NULL}
};

const PokemonData TOGEPI = {
    .id = POKEMON_ID_TOGEPI, .name = "Togepi",
    .baseHp = 35, .baseAttack = 20, .baseSpAttack = 40,
    .baseDefense = 65, .baseSpDefense = 65, .baseSpeed = 20,
    .type1 = TYPE_FAIRY, .type2 = TYPE_NONE,
    .evolution = {EVO_LEVEL, 20, &TOGETIC}
};

const PokemonData TOGETIC = {
    .id = POKEMON_ID_TOGETIC, .name = "Togetic",
    .baseHp = 55, .baseAttack = 40, .baseSpAttack = 80,
    .baseDefense = 85, .baseSpDefense = 105, .baseSpeed = 40,
    .type1 = TYPE_FAIRY, .type2 = TYPE_FLYING,
    .evolution = {EVO_ITEM, 0, &TOGEKISS}
};

const PokemonData TOGEKISS = {
    .id = POKEMON_ID_TOGEKISS, .name = "Togekiss",
    .baseHp = 85, .baseAttack = 50, .baseSpAttack = 120,
    .baseDefense = 95, .baseSpDefense = 115, .baseSpeed = 80,
    .type1 = TYPE_FAIRY, .type2 = TYPE_FLYING,
    .evolution = {EVO_NONE, 0, NULL}
};
