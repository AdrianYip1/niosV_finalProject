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
static const AttackData TACKLE = {
    .id = 1,
    .name = "Tackle",
    .power = 40,
    .accuracy = 100,
    .maxPP = 35,
    .category = ATTACK_PHYSICAL
};

// Placeholder Pokemon entry with one valid move.
static const PokemonData CHARIZARD = {
    .id = 1,
    .name = "Charizard",
    .baseHp = 78,
    .baseAttack = 84,
    .baseSpAttack = 109,
    .baseDefense = 78,
    .baseSpDefense = 85,
    .baseSpeed = 100,
    .moves = {
        &TACKLE,
        NULL,
        NULL,
        NULL
    }
};
