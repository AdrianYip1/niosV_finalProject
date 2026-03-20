#ifndef BATTLE_LOOP_H
#define BATTLE_LOOP_H

#include "../entities/pokemonObject.h"
#include "../entities/learnset.h"
#include "party.h"

typedef enum {
    BATTLE_WILD,
    BATTLE_TRAINER
} BattleType;

typedef enum {
    BATTLE_RESULT_ONGOING = 0,
    BATTLE_RESULT_PLAYER_WIN,
    BATTLE_RESULT_PLAYER_LOSE,
    BATTLE_RESULT_FLED,
    BATTLE_RESULT_CAUGHT
} BattleResult;

typedef enum {
    ACTION_ATTACK = 0,
    ACTION_SWITCH,
    ACTION_ITEM,
    ACTION_RUN
} BattleAction;

typedef struct {
    Party *playerParty;
    Party *enemyParty;
    BattleType type;
    BattleResult result;
    int fleeAttempts;
} BattleState;

BattleResult runBattle(Party *playerParty, Party *enemyParty, BattleType type);

#endif /* BATTLE_LOOP_H */