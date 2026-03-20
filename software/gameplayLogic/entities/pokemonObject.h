#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define BASE_EXP_GAINED 50
#define EXP_MULTIPLIER 1
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
    TYPE_FAIRY
} PokemonType;

typedef enum {
    ATTACK_PHYSICAL = 0,
    ATTACK_SPECIAL = 1,
    ATTACK_STATUS = 2
} AttackCategory;

typedef enum {
    STATUS_NONE = 0,
    STATUS_BURN,
    STATUS_POISON,
    STATUS_PARALYSIS,
    STATUS_SLEEP,
    STATUS_FREEZE
} StatusCondition;

typedef struct {
    int id;
    const char *name;
    int power;
    int accuracy;
    int maxPP;
    AttackCategory category;
    PokemonType type;
} AttackData;

typedef struct {
    int id;
    const char *name;
    int baseHp;
    int baseAttack;
    int baseSpAttack;
    int baseDefense;
    int baseSpDefense;
    int baseSpeed;
    PokemonType type1;
    PokemonType type2; // same as type1 if single type
    const AttackData *moves[4];
} PokemonData;

typedef struct {
    const PokemonData* data; // points back to the species template, no copying needed
    int frontFrame_ID;
    int backFrame_ID;
} pokemonID;

typedef struct {
    pokemonID id;
    int level;
    int exp;
    bool alive;
    int maxHp;
    int scaledStatsWithLevel[6]; //{hp, attack, sp attack, def, sp def, speed}
    int currentPP[4]; // current pp for each move
    bool inBattle;
    StatusCondition status;
    int sleepTurnsRemaining;
    PokemonType type1;
    PokemonType type2; // same as type1 if single type
    const AttackData* attacks[4]; // 4 attacks per pokemon
} pokemonInBattle;

// type effectiveness table [attacking type][defending type]
static const float typeChart[NUM_TYPES][NUM_TYPES] = {
//      NOR  FIR  WAT  ELE  GRA  ICE  FIG  POI  GRO  FLY  PSY  BUG  ROC  GHO  DRA  DAR  STE  FAI
       {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.0, 1.0, 1.0, 0.5, 1.0}, // normal
       {1.0, 0.5, 0.5, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 0.5, 1.0, 2.0, 1.0}, // fire
       {1.0, 2.0, 0.5, 1.0, 0.5, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0, 1.0}, // water
       {1.0, 1.0, 2.0, 0.5, 0.5, 1.0, 1.0, 1.0, 0.0, 2.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0}, // electric
       {1.0, 0.5, 2.0, 1.0, 0.5, 1.0, 1.0, 0.5, 2.0, 0.5, 1.0, 0.5, 2.0, 1.0, 0.5, 1.0, 0.5, 1.0}, // grass
       {1.0, 0.5, 0.5, 1.0, 2.0, 0.5, 1.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0}, // ice
       {2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 0.5, 0.5, 0.5, 2.0, 0.0, 1.0, 2.0, 2.0, 0.5}, // fighting
       {1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 0.5, 0.5, 1.0, 1.0, 1.0, 0.5, 0.5, 1.0, 1.0, 0.0, 2.0}, // poison
       {1.0, 2.0, 1.0, 2.0, 0.5, 1.0, 1.0, 2.0, 1.0, 0.0, 1.0, 0.5, 2.0, 1.0, 1.0, 1.0, 2.0, 1.0}, // ground
       {1.0, 1.0, 1.0, 0.5, 2.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 1.0, 1.0, 0.5, 1.0}, // flying
       {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 0.0, 0.5, 1.0}, // psychic
       {1.0, 0.5, 1.0, 1.0, 2.0, 1.0, 0.5, 0.5, 1.0, 0.5, 2.0, 1.0, 1.0, 0.5, 1.0, 2.0, 0.5, 0.5}, // bug
       {1.0, 2.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 0.5, 2.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0}, // rock
       {0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 0.5, 1.0, 1.0}, // ghost
       {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 0.5, 0.0}, // dragon
       {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 2.0, 2.0, 1.0, 0.5, 1.0, 0.5, 1.0, 0.5}, // dark
       {1.0, 0.5, 0.5, 0.5, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 0.5, 2.0}, // steel
       {1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 0.5, 0.5}, // fairy
};

static const AttackData TACKLE = {
    .id = 1,
    .name = "Tackle",
    .power = 40,
    .accuracy = 100,
    .maxPP = 35,
    .category = ATTACK_PHYSICAL,
    .type = TYPE_NORMAL
};

static const PokemonData CHARIZARD = {
    .id = 1,
    .name = "Charizard",
    .baseHp = 78,
    .baseAttack = 84,
    .baseSpAttack = 109,
    .baseDefense = 78,
    .baseSpDefense = 85,
    .baseSpeed = 100,
    .type1 = TYPE_FIRE,
    .type2 = TYPE_FLYING,
    .moves = {
        &TACKLE,
        NULL,
        NULL,
        NULL
    }
};

// init
void initPokemon(PokemonData* pokemon, int id, const char* name,
                 int baseHp, int baseAttack, int baseSpAttack,
                 int baseDefense, int baseSpDefense, int baseSpeed,
                 PokemonType type1, PokemonType type2,
                 const AttackData* move1, const AttackData* move2,
                 const AttackData* move3, const AttackData* move4);
void initPokemonInBattle(pokemonInBattle* pokemon, const PokemonData* template, int level);

// stats
void scaleStatsWithLevel(pokemonInBattle* pokemon);
void levelUp(pokemonInBattle* pokemon);
void gainExp(pokemonInBattle* pokemon, pokemonInBattle* defeatedPokemon);
int expRequiredAtLevel(int level);
int experienceGained(int levelSelf, int levelOpponent);

// type
float getTypeEffectiveness(PokemonType attackType, PokemonType defendType1, PokemonType defendType2);

// battle actions
void dealDamage(pokemonInBattle* attacker, pokemonInBattle* target, int baseDamage, int damageType, PokemonType moveType);
void takeDamage(pokemonInBattle* target, int incomingDamage, int damageType);
bool useAttack(pokemonInBattle* attacker, pokemonInBattle* target, int attackIndex);
int determineturnOrder(pokemonInBattle* pokemon1, pokemonInBattle* pokemon2);
bool attemptFlee(pokemonInBattle* fleeing, pokemonInBattle* opponent);

// status
void applyStatusEffect(pokemonInBattle* pokemon, StatusCondition status);
void tickStatusEffect(pokemonInBattle* pokemon);
bool canAct(pokemonInBattle* pokemon);

// healing
void healPokemon(pokemonInBattle* pokemon, int amount);
void restorePP(pokemonInBattle* pokemon, int attackIndex);
void restoreAllPP(pokemonInBattle* pokemon);
void fullHeal(pokemonInBattle* pokemon);

// catch
bool attemptCatch(pokemonInBattle* wildPokemon, float ballModifier);

// getters
bool isAlive(pokemonInBattle* pokemon);
int getHp(pokemonInBattle* pokemon);
int getAttack(pokemonInBattle* pokemon);
int getSpAttack(pokemonInBattle* pokemon);
int getDef(pokemonInBattle* pokemon);
int getSpDef(pokemonInBattle* pokemon);
int getSpd(pokemonInBattle* pokemon);