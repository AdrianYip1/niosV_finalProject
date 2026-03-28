#include "pokemonObject.h"
#include "learnSet.h"
#include <stdio.h>

#ifndef POKEMON_DEBUG_STDIO
#define POKEMON_DEBUG_STDIO 0
#endif

#if POKEMON_DEBUG_STDIO
#define DBG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DBG_PRINTF(...) do { } while (0)
#endif

static int clampStage(int stage) {
    if (stage > 6) return 6;
    if (stage < -6) return -6;
    return stage;
}

static float stageMultiplierStat(int stage) {
    stage = clampStage(stage);
    if (stage >= 0) return (2.0f + (float)stage) / 2.0f;
    return 2.0f / (2.0f - (float)stage);
}

static float stageMultiplierAccEva(int stage) {
    stage = clampStage(stage);
    if (stage >= 0) return (3.0f + (float)stage) / 3.0f;
    return 3.0f / (3.0f - (float)stage);
}

static int applyStageToStat(int baseValue, int stage) {
    int v = (int)((float)baseValue * stageMultiplierStat(stage) + 0.5f);
    if (v < 1) v = 1;
    return v;
}

void resetStatStages(pokemonInBattle *pokemon) {
    if (pokemon == NULL) return;
    pokemon->statStageAttack = 0;
    pokemon->statStageDefense = 0;
    pokemon->statStageSpAttack = 0;
    pokemon->statStageSpDefense = 0;
    pokemon->statStageSpeed = 0;
    pokemon->statStageAccuracy = 0;
    pokemon->statStageEvasion = 0;
}


//pokedex stuff to determine caught/seen -> grayscale the caughtIcon for seen but not caught
void initPokemonInBattle(pokemonInBattle *pokemon, const PokemonData *template, int level) {
    pokemon->id.data = template;
    pokemon->id.frontFrame_ID = template->id;
    pokemon->id.backFrame_ID = template->id;
    pokemon->level = level;
    pokemon->exp = 0;
    pokemon->alive = true;
    pokemon->caughtIn = POKEBALL_NONE;
    pokemon->inBattle = false;
    pokemon->status = STATUS_NONE;
    pokemon->sleepTurnsRemaining = 0;
    pokemon->type1 = template->type1;
    pokemon->type2 = template->type2;
    resetStatStages(pokemon);
    initMovesFromLearnset(pokemon);
    scaleStatsWithLevel(pokemon);
    pokemon->maxHp = pokemon->scaledStatsWithLevel[0];

    for (int i = 0; i < 4; i++) pokemon->pendingLearnMoves[i] = NULL;
    pokemon->pendingLearnMoveCount = 0;
}

void scaleStatsWithLevel(pokemonInBattle *pokemon) {
    const PokemonData *d = pokemon->id.data;
    int base[6] = {d->baseHp, d->baseAttack, d->baseSpAttack, d->baseDefense, d->baseSpDefense, d->baseSpeed};
    // HP: floor((2*base*level)/100) + level + 10
    // Other stats: floor((2*base*level)/100) + 5
    pokemon->scaledStatsWithLevel[0] = ((2 * base[0] * pokemon->level) / 100) + pokemon->level + 10;
    for (int i = 1; i < 6; i++) {
        pokemon->scaledStatsWithLevel[i] = ((2 * base[i] * pokemon->level) / 100) + 5;
    }
}

//add exp share later maybe
void gainExp(pokemonInBattle *pokemon, pokemonInBattle *defeatedPokemon) {
    int expGained = experienceGained(pokemon->level, defeatedPokemon->level);
    if (expGained == 0) return;
    pokemon->exp += expGained;
    while (pokemon->level < MAX_POKEMON_LEVEL && pokemon->exp >= expRequiredAtLevel(pokemon->level)) {
        levelUp(pokemon);
    }
    if (pokemon->level >= MAX_POKEMON_LEVEL) {
        pokemon->level = MAX_POKEMON_LEVEL;
        pokemon->exp = 0;
    }
}

void levelUp(pokemonInBattle *pokemon) {
    if (pokemon == NULL) return;
    if (pokemon->level >= MAX_POKEMON_LEVEL) {
        pokemon->level = MAX_POKEMON_LEVEL;
        pokemon->exp = 0;
        return;
    }
    const int prevMaxHp = pokemon->maxHp;
    const int prevCurHp = pokemon->scaledStatsWithLevel[0];

    pokemon->exp -= expRequiredAtLevel(pokemon->level);
    pokemon->level++;
    scaleStatsWithLevel(pokemon);


    pokemon->maxHp = pokemon->scaledStatsWithLevel[0];
    const int deltaHp = pokemon->maxHp - prevMaxHp;
    int newCurHp = prevCurHp + ((deltaHp > 0) ? deltaHp : 0);
    if (newCurHp > pokemon->maxHp) newCurHp = pokemon->maxHp;
    if (newCurHp < 1 && pokemon->alive) newCurHp = 1;
    pokemon->scaledStatsWithLevel[0] = newCurHp;
    DBG_PRINTF("%s grew to level %d!\n", pokemon->id.data->name, pokemon->level);
    checkLevelUpMoves(pokemon, onLearnMove);
    const PokemonData *next = checkEvolution(pokemon->id.data, pokemon->level);
    if (next != NULL) {
        DBG_PRINTF("%s is evolving into %s!\n", pokemon->id.data->name, next->name);
        pokemon->id.data = next;
        pokemon->id.frontFrame_ID = next->id;
        pokemon->id.backFrame_ID = next->id;
        pokemon->type1 = next->type1;
        pokemon->type2 = next->type2;
        // Evolution changes base stats; preserve current HP by applying the new max-HP delta.
        const int evoPrevMaxHp = pokemon->maxHp;
        const int evoPrevCurHp = pokemon->scaledStatsWithLevel[0];
        scaleStatsWithLevel(pokemon);
        pokemon->maxHp = pokemon->scaledStatsWithLevel[0];
        const int evoDeltaHp = pokemon->maxHp - evoPrevMaxHp;
        int evoCurHp = evoPrevCurHp + ((evoDeltaHp > 0) ? evoDeltaHp : 0);
        if (evoCurHp > pokemon->maxHp) evoCurHp = pokemon->maxHp;
        if (evoCurHp < 1 && pokemon->alive) evoCurHp = 1;
        pokemon->scaledStatsWithLevel[0] = evoCurHp;
        checkLevelUpMoves(pokemon, onLearnMove);
        DBG_PRINTF("%s evolved!\n", next->name);
    }
}

int expRequiredAtLevel(int level) {
    if (level < 1) level = 1;
    int req = (level * level) * 3;
    if (req < 50) req = 50;
    return req;
}

int experienceGained(int levelSelf, int levelOpponent) {
    (void)levelSelf;
    if (levelOpponent < 1) levelOpponent = 1;
    return (BASE_EXP_GAINED * levelOpponent) * EXP_MULTIPLIER;
}

float getTypeEffectiveness(PokemonType attackType, PokemonType defendType1, PokemonType defendType2) {
    float effectiveness = typeChart[attackType][defendType1];
    if (defendType2 != defendType1) {
        effectiveness *= typeChart[attackType][defendType2];
    }
    return effectiveness;
}

void dealDamage(pokemonInBattle *attacker, pokemonInBattle *target, int baseDamage, int damageType, PokemonType moveType) {
    if (attacker == NULL || target == NULL) return;
    if (baseDamage <= 0) return;

    const int atkBase = (damageType == 0) ? attacker->scaledStatsWithLevel[1] : attacker->scaledStatsWithLevel[2];
    const int defBase = (damageType == 0) ? target->scaledStatsWithLevel[3] : target->scaledStatsWithLevel[4];
    const int atkStage = (damageType == 0) ? attacker->statStageAttack : attacker->statStageSpAttack;
    const int defStage = (damageType == 0) ? target->statStageDefense : target->statStageSpDefense;
    const int atkStat = applyStageToStat(atkBase, atkStage);
    const int defStat = applyStageToStat(defBase, defStage);
    const int def = (defStat > 0) ? defStat : 1;

    // Pokémon-style base damage:
    // base = floor(floor(floor((2*L/5+2) * P * A / D) / 50) + 2)
    const int level = (attacker->level > 0) ? attacker->level : 1;
    const int levelFactor = ((2 * level) / 5) + 2;
    int base = (levelFactor * baseDamage * atkStat) / def;
    base = (base / 50) + 2;

    const float eff = getTypeEffectiveness(moveType, target->type1, target->type2);
    if (eff <= 0.0f) {
        return; // no effect
    }

    const bool stab = (moveType != TYPE_NONE) && ((attacker->type1 == moveType) || (attacker->type2 == moveType)); //does 1.5x more damage
    const float stabMult = stab ? 1.5f : 1.0f;
    const float randMult = (float)(85 + (rand() % 16)) / 100.0f; // 0.85..1.00
    const float burnMult = (damageType == ATTACK_PHYSICAL && attacker->status == STATUS_BURN) ? 0.5f : 1.0f;

    float modifier = stabMult * eff * randMult * burnMult;
    int dmg = (int)((float)base * modifier);
    if (dmg < 1) dmg = 1;

    takeDamage(target, dmg, damageType);
}

void takeDamage(pokemonInBattle *target, int incomingDamage, int damageType) {
    (void)damageType;
    if (target == NULL) return;
    if (incomingDamage <= 0) return;

    const int actualDamage = incomingDamage;
    if (actualDamage >= target->scaledStatsWithLevel[0]) {
        target->alive = false;
        target->scaledStatsWithLevel[0] = 0;
    } else {
        target->scaledStatsWithLevel[0] -= actualDamage;
    }
}

bool useAttack(pokemonInBattle *attacker, pokemonInBattle *target, int attackIndex) {
    if (attackIndex < 0 || attackIndex > 3) return false;
    if (attacker->attacks[attackIndex] == NULL) return false;
    const AttackData *move = attacker->attacks[attackIndex];
    if (attacker->currentPP[attackIndex] <= 0) return false;
    // PP is consumed even if the move misses.
    attacker->currentPP[attackIndex]--;

    // Accuracy check respects accuracy/evasion stat stages.
    float finalAcc = (float)move->accuracy;
    if (target != NULL) {
        const float accMult = stageMultiplierAccEva(attacker->statStageAccuracy);
        const float evaMult = stageMultiplierAccEva(target->statStageEvasion);
        if (evaMult > 0.0f) finalAcc = finalAcc * (accMult / evaMult);
    }
    if (finalAcc < 1.0f) finalAcc = 1.0f;
    if (finalAcc > 100.0f) finalAcc = 100.0f;
    if ((rand() % 100) >= (int)(finalAcc + 0.5f)) return false;
    if (move->category != ATTACK_STATUS) {
        // Fixed-damage move: Dragon Rage (40 damage, still respects immunity).
        if (move->id == 40) {
            const float eff = getTypeEffectiveness(move->type, target->type1, target->type2);
            if (eff > 0.0f) takeDamage(target, 40, move->category);
        } else {
            dealDamage(attacker, target, move->power, move->category, move->type);
        }
    }
    return true;
}

int determineTurnOrder(pokemonInBattle *pokemon1, pokemonInBattle *pokemon2) {
    int spd1 = applyStageToStat(pokemon1->scaledStatsWithLevel[5], pokemon1->statStageSpeed);
    int spd2 = applyStageToStat(pokemon2->scaledStatsWithLevel[5], pokemon2->statStageSpeed);
    if (spd1 > spd2) return 1;
    if (spd2 > spd1) return 2;
    return 1;
}

bool attemptFlee(pokemonInBattle *fleeing, pokemonInBattle *opponent) {
    int fleeingSpd = fleeing->scaledStatsWithLevel[5];
    int opponentSpd = opponent->scaledStatsWithLevel[5];
    if (fleeingSpd >= opponentSpd) return true;
    int fleeChance = (fleeingSpd * 128 / opponentSpd) + 30;
    return (rand() % 256) < fleeChance;
}

void applyStatusEffect(pokemonInBattle *pokemon, StatusCondition status) {
    if (pokemon->status != STATUS_NONE) return;
    pokemon->status = status;
    if (status == STATUS_SLEEP) {
        pokemon->sleepTurnsRemaining = (rand() % 4) + 1;
    }
}

void tickStatusEffect(pokemonInBattle *pokemon) {
    switch (pokemon->status) {
        case STATUS_BURN:
            takeDamage(pokemon, pokemon->maxHp / 8, ATTACK_STATUS);
            break;
        case STATUS_POISON:
            takeDamage(pokemon, pokemon->maxHp / 16, ATTACK_STATUS);
            break;
        case STATUS_SLEEP:
            if (pokemon->sleepTurnsRemaining > 0) pokemon->sleepTurnsRemaining--;
            if (pokemon->sleepTurnsRemaining == 0) pokemon->status = STATUS_NONE;
            break;
        case STATUS_FREEZE:
            if (rand() % 100 < 20) pokemon->status = STATUS_NONE;
            break;
        default:
            break;
    }
}

bool canAct(pokemonInBattle *pokemon) {
    return canActThisTurn(pokemon, NULL);
}

bool canActThisTurn(pokemonInBattle *pokemon, StatusCondition *blockedBy) {
    if (blockedBy != NULL) *blockedBy = STATUS_NONE;
    if (pokemon == NULL) return false;
    if (!pokemon->alive) return false;

    if (pokemon->status == STATUS_SLEEP) {
        if (blockedBy != NULL) *blockedBy = STATUS_SLEEP;
        return false;
    }
    if (pokemon->status == STATUS_FREEZE) {
        if (blockedBy != NULL) *blockedBy = STATUS_FREEZE;
        return false;
    }
    if (pokemon->status == STATUS_PARALYSIS && rand() % 100 < 25) {
        if (blockedBy != NULL) *blockedBy = STATUS_PARALYSIS;
        return false;
    }
    return true;
}

void healPokemon(pokemonInBattle *pokemon, int amount) {
    pokemon->scaledStatsWithLevel[0] += amount;
    if (pokemon->scaledStatsWithLevel[0] > pokemon->maxHp) {
        pokemon->scaledStatsWithLevel[0] = pokemon->maxHp;
    }
}

void restorePP(pokemonInBattle *pokemon, int attackIndex) {
    if (attackIndex < 0 || attackIndex > 3) return;
    if (pokemon->attacks[attackIndex] == NULL) return;
    pokemon->currentPP[attackIndex] = pokemon->attacks[attackIndex]->maxPP;
}

void restoreAllPP(pokemonInBattle *pokemon) {
    for (int i = 0; i < 4; i++) restorePP(pokemon, i);
}

void fullRestore(pokemonInBattle *pokemon) {
    healPokemon(pokemon, pokemon->maxHp);
    //restoreAllPP(pokemon);
    pokemon->status = STATUS_NONE;
    pokemon->sleepTurnsRemaining = 0;
}

void fullHeal(pokemonInBattle *pokemon) {
    //healPokemon(pokemon, pokemon->maxHp);
    //restoreAllPP(pokemon);
    pokemon->status = STATUS_NONE;
    pokemon->sleepTurnsRemaining = 0;
}

bool attemptCatch(pokemonInBattle *wildPokemon, float ballModifier) {
    float hpFactor = 1.0f + 2.0f * (1.0f - (float)wildPokemon->scaledStatsWithLevel[0] / wildPokemon->maxHp);
    float statusBonus = 1.0f;
    if (wildPokemon->status == STATUS_SLEEP || wildPokemon->status == STATUS_FREEZE) statusBonus = 2.0f;
    if (wildPokemon->status == STATUS_PARALYSIS || wildPokemon->status == STATUS_BURN || wildPokemon->status == STATUS_POISON) statusBonus = 1.5f;
    float catchChance = hpFactor * statusBonus * ballModifier;
    return (rand() % 256) < (int)(catchChance * 50);
}

bool attemptCatchWithBall(pokemonInBattle *wildPokemon, PokeballType ball) {
    if (wildPokemon == NULL) return false;

    float modifier = 1.0f;
    switch (ball) {
        case POKEBALL_POKE: modifier = 1.0f; break;
        case POKEBALL_GREAT: modifier = 1.5f; break;
        case POKEBALL_ULTRA: modifier = 2.0f; break;
        case POKEBALL_PREMIER: modifier = 1.0f; break;
        case POKEBALL_MASTER: modifier = 255.0f; break; // guaranteed 
        default: modifier = 1.0f; break;
    }

    const bool caught = attemptCatch(wildPokemon, modifier);
    if (caught) wildPokemon->caughtIn = ball;
    return caught;
}

void onLearnMove(pokemonInBattle *pokemon, const AttackData *move) {
    if (pokemon == NULL || pokemon->id.data == NULL || move == NULL) return;

    // Avoid scanf on the board.
    for (int i = 0; i < 4; i++) {
        if (pokemon->attacks[i] == move) return; // already knows
    }
    for (int i = 0; i < pokemon->pendingLearnMoveCount; i++) {
        if (pokemon->pendingLearnMoves[i] == move) return; // already pending
    }

    DBG_PRINTF("%s wants to learn %s!\n", pokemon->id.data->name, move->name);
    for (int i = 0; i < 4; i++) {
        if (pokemon->attacks[i] == NULL) {
            learnMove(pokemon, move, -1);
            DBG_PRINTF("%s learned %s!\n", pokemon->id.data->name, move->name);
            return;
        }
    }

    // Already has 4 moves: queue for UI-driven learn/forget flow in main.
    if (pokemon->pendingLearnMoveCount < 4) {
        pokemon->pendingLearnMoves[pokemon->pendingLearnMoveCount++] = move;
    } else {
        // If somehow multiple moves are learned at once, drop extras rather than blocking/crashing.
        DBG_PRINTF("Learn-move queue full; skipping %s.\n", move->name);
    }
}

bool isAlive(pokemonInBattle *pokemon) { return pokemon->alive; }
int getHp(pokemonInBattle *pokemon) { return pokemon->id.data->baseHp; }
int getAttack(pokemonInBattle *pokemon) { return pokemon->id.data->baseAttack; }
int getSpAttack(pokemonInBattle *pokemon) { return pokemon->id.data->baseSpAttack; }
int getDef(pokemonInBattle *pokemon) { return pokemon->id.data->baseDefense; }
int getSpDef(pokemonInBattle *pokemon) { return pokemon->id.data->baseSpDefense; }
int getSpd(pokemonInBattle *pokemon) { return pokemon->id.data->baseSpeed; }
