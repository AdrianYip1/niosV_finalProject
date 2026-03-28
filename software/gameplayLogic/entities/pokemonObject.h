#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "pokemonDataBase.h"

#define BASE_EXP_GAINED 50
#define EXP_MULTIPLIER 1
#define MAX_POKEMON_LEVEL 100
#ifndef NUM_TYPES
#define NUM_TYPES 19
#endif

typedef enum {
    STATUS_NONE = 0,
    STATUS_BURN,
    STATUS_POISON,
    STATUS_PARALYSIS,
    STATUS_SLEEP,
    STATUS_FREEZE
} StatusCondition;

typedef enum {
    POKEBALL_NONE = 0,
    POKEBALL_POKE,
    POKEBALL_GREAT,
    POKEBALL_ULTRA,
    POKEBALL_PREMIER,
    POKEBALL_MASTER
} PokeballType;

typedef struct {
    const PokemonData *data;
    int frontFrame_ID;
    int backFrame_ID;
} pokemonID;

typedef struct {
    pokemonID id;
    int level;
    int exp;
    bool alive;
    PokeballType caughtIn;
    int maxHp;
    int scaledStatsWithLevel[6]; // {hp, attack, sp attack, def, sp def, speed}

    // Battle stat stages (-6..+6)
    int statStageAttack;
    int statStageDefense;
    int statStageSpAttack;
    int statStageSpDefense;
    int statStageSpeed;
    int statStageAccuracy;
    int statStageEvasion;

    int currentPP[4];
    bool inBattle;
    StatusCondition status;
    int sleepTurnsRemaining;
    PokemonType type1;
    PokemonType type2;
    const AttackData *attacks[4];

    // Pending level-up learn-move prompts (has 4 moves already).
    const AttackData *pendingLearnMoves[4];
    int pendingLearnMoveCount;
} pokemonInBattle;

// init
void initPokemonInBattle(pokemonInBattle *pokemon, const PokemonData *template, int level);

// stats
void scaleStatsWithLevel(pokemonInBattle *pokemon);
void levelUp(pokemonInBattle *pokemon);
void gainExp(pokemonInBattle *pokemon, pokemonInBattle *defeatedPokemon);
int expRequiredAtLevel(int level);
int experienceGained(int levelSelf, int levelOpponent);

// type
float getTypeEffectiveness(PokemonType attackType, PokemonType defendType1, PokemonType defendType2);

// battle actions
void dealDamage(pokemonInBattle *attacker, pokemonInBattle *target, int baseDamage, int damageType, PokemonType moveType);
void takeDamage(pokemonInBattle *target, int incomingDamage, int damageType);
bool useAttack(pokemonInBattle *attacker, pokemonInBattle *target, int attackIndex);
int determineTurnOrder(pokemonInBattle *pokemon1, pokemonInBattle *pokemon2);
bool attemptFlee(pokemonInBattle *fleeing, pokemonInBattle *opponent);

// status
void applyStatusEffect(pokemonInBattle *pokemon, StatusCondition status);
void tickStatusEffect(pokemonInBattle *pokemon);
bool canAct(pokemonInBattle *pokemon);
bool canActThisTurn(pokemonInBattle *pokemon, StatusCondition *blockedBy);
void resetStatStages(pokemonInBattle *pokemon);

// healing
void healPokemon(pokemonInBattle *pokemon, int amount);
void restorePP(pokemonInBattle *pokemon, int attackIndex);
void restoreAllPP(pokemonInBattle *pokemon);
void fullRestore(pokemonInBattle *pokemon);
void fullHeal(pokemonInBattle *pokemon);

// catch
bool attemptCatch(pokemonInBattle *wildPokemon, float ballModifier);
bool attemptCatchWithBall(pokemonInBattle *wildPokemon, PokeballType ball);

// getters
bool isAlive(pokemonInBattle *pokemon);
int getHp(pokemonInBattle *pokemon);
int getAttack(pokemonInBattle *pokemon);
int getSpAttack(pokemonInBattle *pokemon);
int getDef(pokemonInBattle *pokemon);
int getSpDef(pokemonInBattle *pokemon);
int getSpd(pokemonInBattle *pokemon);


void onLearnMove(pokemonInBattle *pokemon, const AttackData *move);
