#ifndef BATTLE_LOOP_H
#define BATTLE_LOOP_H

#include "../entities/pokemonObject.h"
#include "../entities/learnSet.h"
#include "../bag.h"
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

typedef enum {
    BATTLE_HIT_EFFECT_NORMAL = 0,
    BATTLE_HIT_EFFECT_NOT_EFFECTIVE,
    BATTLE_HIT_EFFECT_SUPER_EFFECTIVE,
    BATTLE_HIT_EFFECT_STATS_UP,
    BATTLE_HIT_EFFECT_STATS_DOWN
} BattleHitEffect;

typedef struct {
    Party *playerParty;
    Party *enemyParty;
    Bag *playerBag;
    BattleType type;
    BattleResult result;
    int fleeAttempts;
    bool playerMustSwitch;

    // Level-up stats screen data (consumed by UI).
    bool levelUpStatsPending;
    signed char levelUpPokemonIndex; // index in playerParty->slots
    int levelUpOldStats[6]; // {HPmax, Atk, Def, SpAtk, SpDef, Spd}
    int levelUpNewStats[6]; // {HPmax, Atk, Def, SpAtk, SpDef, Spd}

#ifndef BATTLE_MSG_MAX
#define BATTLE_MSG_MAX 16
#endif
    char messages[BATTLE_MSG_MAX][96];
    unsigned char messageFlags[BATTLE_MSG_MAX];  // 1 if this message displays a damaging hit (exclude no effect)
    signed char hitEffects[BATTLE_MSG_MAX];

    //timing damage with shake animations
    int hpAfterPlayer[BATTLE_MSG_MAX];
    int hpAfterEnemy[BATTLE_MSG_MAX];
    // which pokemon should be displayed for this message index
    signed char displayPlayerIndex[BATTLE_MSG_MAX];
    signed char displayEnemyIndex[BATTLE_MSG_MAX];
    int messageCount;
    int messageReadIndex;
} BattleState;

BattleResult runBattle(Party *playerParty, Party *enemyParty, BattleType type);


void initBattleState(BattleState *state, Party *playerParty, Party *enemyParty, Bag *playerBag, BattleType type);
void battleApplyPlayerAction(BattleState *state, BattleAction action, int param);

static inline void battleStubApplyPlayerAction(BattleState *state, BattleAction action, int param) {
    battleApplyPlayerAction(state, action, param);
}

#endif /* BATTLE_LOOP_H */
