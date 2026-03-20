#include "pokemonObject.h"
#include "learnset.h"
#include <stdio.h>

void initPokemon(PokemonData* pokemon, int id, const char* name,
                 int baseHp, int baseAttack, int baseSpAttack,
                 int baseDefense, int baseSpDefense, int baseSpeed,
                 PokemonType type1, PokemonType type2) {

    pokemon->id = id;
    pokemon->name = name;
    pokemon->baseHp = baseHp;
    pokemon->baseAttack = baseAttack;
    pokemon->baseSpAttack = baseSpAttack;
    pokemon->baseDefense = baseDefense;
    pokemon->baseSpDefense = baseSpDefense;
    pokemon->baseSpeed = baseSpeed;
    pokemon->type1 = type1;
    pokemon->type2 = type2;

}

void initPokemonInBattle(pokemonInBattle* pokemon, const PokemonData* template, int level) {
    pokemon->id.data = template;
    pokemon->id.frontFrame_ID = template->id;
    pokemon->id.backFrame_ID = template->id;

    pokemon->level = level;
    pokemon->exp = 0;
    pokemon->alive = true;
    pokemon->inBattle = false;

    pokemon->status = STATUS_NONE;
    pokemon->sleepTurnsRemaining = 0;

    pokemon->type1 = template->type1;
    pokemon->type2 = template->type2;

    initMovesFromLearnset(pokemon);

    scaleStatsWithLevel(pokemon);
    pokemon->maxHp = pokemon->scaledStatsWithLevel[0];
}

void scaleStatsWithLevel(pokemonInBattle* pokemon) {
    const PokemonData* d = pokemon->id.data;
    int baseStats[6] = {d->baseHp, d->baseAttack, d->baseSpAttack, d->baseDefense, d->baseSpDefense, d->baseSpeed};
    for (int i = 0; i < 6; i++) {
        pokemon->scaledStatsWithLevel[i] = (((int)baseStats[i] * 2) / 100 * pokemon->level);
    }
}

void gainExp(pokemonInBattle* pokemon, pokemonInBattle* defeatedPokemon) {
    int expGained = experienceGained(pokemon->level, defeatedPokemon->level);
    if (expGained == 0) return;
    pokemon->exp += expGained;
    while (pokemon->exp >= expRequiredAtLevel(pokemon->level)) {
        levelUp(pokemon);
    }
}

void levelUp(pokemonInBattle* pokemon) {
    pokemon->exp -= expRequiredAtLevel(pokemon->level);
    pokemon->level++;
    scaleStatsWithLevel(pokemon);
    pokemon->maxHp = pokemon->scaledStatsWithLevel[0];

    printf("%s grew to level %d!\n", pokemon->id.data->name, pokemon->level);

    checkLevelUpMoves(pokemon, onLearnMove);

    // check for evolution
    const PokemonData* next = checkEvolution(pokemon->id.data, pokemon->level);
    if (next != NULL) {
        printf("\n%s is evolving into %s!\n", pokemon->id.data->name, next->name);

        pokemon->id.data          = next;
        pokemon->id.frontFrame_ID = next->id;
        pokemon->id.backFrame_ID  = next->id;
        pokemon->type1 = next->type1;
        pokemon->type2 = next->type2;

        scaleStatsWithLevel(pokemon);
        pokemon->maxHp = pokemon->scaledStatsWithLevel[0];

        checkLevelUpMoves(pokemon, onLearnMove);

        printf("%s evolved!\n\n", next->name);
    }
}

int expRequiredAtLevel(int level) {
    return pow((0.8 * level), 3);
}

int experienceGained(int levelSelf, int levelOpponent) {
    return ((BASE_EXP_GAINED * levelOpponent) / 7) * EXP_MULTIPLIER;
}

float getTypeEffectiveness(PokemonType attackType, PokemonType defendType1, PokemonType defendType2) {
    float effectiveness = typeChart[attackType][defendType1];
    if (defendType2 != defendType1) {
        effectiveness *= typeChart[attackType][defendType2];
    }
    return effectiveness;
}

void dealDamage(pokemonInBattle* attacker, pokemonInBattle* target, int baseDamage, int damageType, PokemonType moveType) {
    int modifierID = (damageType == 0) ? 1 : 2;
    int dealtDamage = ((baseDamage * attacker->level + 10) / 250) * attacker->scaledStatsWithLevel[modifierID];
    float effectiveness = getTypeEffectiveness(moveType, target->type1, target->type2);
    dealtDamage = (int)(dealtDamage * effectiveness);
    takeDamage(target, dealtDamage, damageType);
}

void takeDamage(pokemonInBattle* target, int incomingDamage, int damageType) {
    double min = 0.85;
    double max = 1.0;
    double scale = (double)rand() / (double)RAND_MAX;
    double random_num = min + scale * (max - min);
    int modifierID = (damageType == 0) ? 3 : 4;
    int actualDamage = (int)((double)incomingDamage / target->scaledStatsWithLevel[modifierID] * random_num);
    if (actualDamage > target->scaledStatsWithLevel[0]) {
        target->alive = false;
        target->scaledStatsWithLevel[0] = 0;
    } else {
        target->scaledStatsWithLevel[0] -= actualDamage;
    }
}

bool useAttack(pokemonInBattle* attacker, pokemonInBattle* target, int attackIndex) {
    if (attackIndex < 0 || attackIndex > 3) return false;
    if (attacker->attacks[attackIndex] == NULL) return false;

    const AttackData* move = attacker->attacks[attackIndex];
    if (attacker->currentPP[attackIndex] <= 0) return false;

    int accuracyRoll = rand() % 100;
    if (accuracyRoll >= move->accuracy) return false;

    attacker->currentPP[attackIndex]--;
    if (move->category != ATTACK_STATUS) {
        dealDamage(attacker, target, move->power, move->category, move->type);
    }
    return true;
}

int determineturnOrder(pokemonInBattle* pokemon1, pokemonInBattle* pokemon2) {
    int spd1 = pokemon1->scaledStatsWithLevel[5];
    int spd2 = pokemon2->scaledStatsWithLevel[5];
    if (spd1 > spd2) return 1;
    if (spd2 > spd1) return 2;
    return (rand() % 2 == 0) ? 1 : 2;
}

bool attemptFlee(pokemonInBattle* fleeing, pokemonInBattle* opponent) {
    int fleeingSpd = fleeing->scaledStatsWithLevel[5];
    int opponentSpd = opponent->scaledStatsWithLevel[5];
    if (fleeingSpd >= opponentSpd) return true;
    int fleeChance = (fleeingSpd * 128 / opponentSpd) + 30;
    return (rand() % 256) < fleeChance;
}

void applyStatusEffect(pokemonInBattle* pokemon, StatusCondition status) {
    if (pokemon->status != STATUS_NONE) return;
    pokemon->status = status;
    if (status == STATUS_SLEEP) {
        pokemon->sleepTurnsRemaining = (rand() % 4) + 1;
    }
}

void tickStatusEffect(pokemonInBattle* pokemon) {
    switch (pokemon->status) {
        case STATUS_BURN:
            takeDamage(pokemon, pokemon->maxHp / 8, ATTACK_STATUS);
            break;
        case STATUS_POISON:
            takeDamage(pokemon, pokemon->maxHp / 16, ATTACK_STATUS);
            break;
        case STATUS_SLEEP:
            if (pokemon->sleepTurnsRemaining > 0) {
                pokemon->sleepTurnsRemaining--;
            }
            if (pokemon->sleepTurnsRemaining == 0) {
                pokemon->status = STATUS_NONE;
            }
            break;
        case STATUS_FREEZE:
            if (rand() % 100 < 20) {
                pokemon->status = STATUS_NONE;
            }
            break;
        default:
            break;
    }
}

bool canAct(pokemonInBattle* pokemon) {
    if (!pokemon->alive) return false;
    if (pokemon->status == STATUS_SLEEP) return false;
    if (pokemon->status == STATUS_FREEZE) return false;
    if (pokemon->status == STATUS_PARALYSIS) {
        if (rand() % 100 < 25) return false;
    }
    return true;
}

void healPokemon(pokemonInBattle* pokemon, int amount) {
    pokemon->scaledStatsWithLevel[0] += amount;
    if (pokemon->scaledStatsWithLevel[0] > pokemon->maxHp) {
        pokemon->scaledStatsWithLevel[0] = pokemon->maxHp;
    }
}

void restorePP(pokemonInBattle* pokemon, int attackIndex) {
    if (attackIndex < 0 || attackIndex > 3) return;
    if (pokemon->attacks[attackIndex] == NULL) return;
    pokemon->currentPP[attackIndex] = pokemon->attacks[attackIndex]->maxPP;
}

void restoreAllPP(pokemonInBattle* pokemon) {
    for (int i = 0; i < 4; i++) {
        restorePP(pokemon, i);
    }
}

void fullHeal(pokemonInBattle* pokemon) {
    healPokemon(pokemon, pokemon->maxHp);
    restoreAllPP(pokemon);
    pokemon->status = STATUS_NONE;
    pokemon->sleepTurnsRemaining = 0;
}

bool attemptCatch(pokemonInBattle* wildPokemon, float ballModifier) {
    float hpFactor = 1.0f + 2.0f * (1.0f - (float)wildPokemon->scaledStatsWithLevel[0] / wildPokemon->maxHp);
    float statusBonus = 1.0f;
    if (wildPokemon->status == STATUS_SLEEP || wildPokemon->status == STATUS_FREEZE) statusBonus = 2.0f;
    if (wildPokemon->status == STATUS_PARALYSIS || wildPokemon->status == STATUS_BURN || wildPokemon->status == STATUS_POISON) statusBonus = 1.5f;
    float catchChance = hpFactor * statusBonus * ballModifier;
    int catchRoll = rand() % 256;
    return catchRoll < (int)(catchChance * 50);
}

//placeholder for move learning
void onLearnMove(pokemonInBattle* pokemon, const AttackData* move) {
    printf("\n%s wants to learn %s!\n", pokemon->id.data->name, move->name);

    for (int i = 0; i < 4; i++) {
        if (pokemon->attacks[i] == NULL) {
            learnMove(pokemon, move, -1);
            printf("%s learned %s!\n", pokemon->id.data->name, move->name);
            return;
        }
    }

    // all 4 slots full, replace one if wanted
    printf("%s already knows 4 moves. Which move should be forgotten?\n",
           pokemon->id.data->name);
    for (int i = 0; i < 4; i++) {
        printf("  %d) %s\n", i + 1, pokemon->attacks[i]->name);
    }
    printf("  5) Don't learn %s\n", move->name);
    printf("> ");

    int choice = 0;
    scanf("%d", &choice);
    if (choice >= 1 && choice <= 4) {
        printf("%s forgot %s and learned %s!\n",
               pokemon->id.data->name,
               pokemon->attacks[choice - 1]->name,
               move->name);
        learnMove(pokemon, move, choice - 1);
    } else {
        printf("%s did not learn %s.\n", pokemon->id.data->name, move->name);
    }
}

bool isAlive(pokemonInBattle* pokemon) {
    return pokemon->alive;
}
int getHp(pokemonInBattle* pokemon) {
    return pokemon->id.data->baseHp;
}
int getAttack(pokemonInBattle* pokemon) {
    return pokemon->id.data->baseAttack;
}
int getSpAttack(pokemonInBattle* pokemon) {
    return pokemon->id.data->baseSpAttack;
}
int getDef(pokemonInBattle* pokemon) {
    return pokemon->id.data->baseDefense;
}
int getSpDef(pokemonInBattle* pokemon) {
    return pokemon->id.data->baseSpDefense;
}
int getSpd(pokemonInBattle* pokemon) {
    return pokemon->id.data->baseSpeed;
}