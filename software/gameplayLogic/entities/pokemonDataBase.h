#pragma once

#include <stdbool.h>
#include <stddef.h>

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
    const AttackData *moves[4];
} PokemonData;

// Placeholder move for early battle-system
static const AttackData ATTACK_PLACEHOLDER_TACKLE = {
    .id = 1,
    .name = "Tackle",
    .power = 40,
    .accuracy = 100,
    .maxPP = 35,
    .category = ATTACK_PHYSICAL
};

// Placeholder Pokemon entry with one valid move.
static const PokemonData POKEMON_PLACEHOLDER_CHARMANDER = {
    .id = 4,
    .name = "Charmander",
    .baseHp = 39,
    .baseAttack = 52,
    .baseSpAttack = 60,
    .baseDefense = 43,
    .baseSpDefense = 50,
    .baseSpeed = 65,
    .moves = {
        &ATTACK_PLACEHOLDER_TACKLE,
        NULL,
        NULL,
        NULL
    }
};
