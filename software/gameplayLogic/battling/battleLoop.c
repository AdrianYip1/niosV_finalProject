#include "battleLoop.h"
#include <stdio.h>
#include <stdlib.h>

#include "battleLoop.h"


static BattleAction promptPlayerAction(BattleState *state) {
    // ask player: attack / switch / item / run

    return ACTION_ATTACK;
}

static int promptMoveChoice(pokemonInBattle *pokemon) {
    // show pokemon's current moves + PP
    // return 0-3
    return 0;
}

static int promptSwitchChoice(Party *party) {
    // show party list, return slot index player wants to switch to
    return 0;
}



static int aiChooseMove(pokemonInBattle *pokemon) {
    // pick random move
    return 0;
}

static void aiChooseAction(BattleState *state) {
    // if active pokemon fainted, switch to first alive slot
    // otherwise attack
}


static void resolveAttack(pokemonInBattle *attacker, pokemonInBattle *target, int moveIndex) {
    // call useAttack
    // check if target fainted
}

static void resolveSwitch(Party *party, int slot) {
    // call switchPokemon
}

static void resolveFlee(BattleState *state) {
    // only valid in wild battles
    // call attemptFlee
}

static void resolvePlayerTurn(BattleState *state, BattleAction action, int param) {
    // resolveAttack / resolveSwitch / item / resolveFlee
}

static void resolveEnemyTurn(BattleState *state) {
    // aiChooseAction then route to resolveAttack / resolveSwitch
}

static void resolveTurn(BattleState *state, BattleAction playerAction, int playerParam) {
    // determine speed order
    // if both attacking: faster goes first, check for faint before second acts
    // if player switching: switch happens before enemy attacks
    // tick status effects end of turn
}


static void handleFaint(BattleState *state, Party *party, bool isPlayer) {
    // if isPlayer: prompt switch choice
    // if enemy: ai picks first alive slot
    // if no alive pokemon remain: set result to win/lose
}

static void checkBattleOver(BattleState *state) {
    // if !hasAlivePokemon(playerParty) -> PLAYER_LOSE
    // if !hasAlivePokemon(enemyParty)  -> PLAYER_WIN
}

BattleResult runBattle(Party *playerParty, Party *enemyParty, BattleType type) {
    BattleState state = {
        .playerParty  = playerParty,
        .enemyParty   = enemyParty,
        .type         = type,
        .result       = BATTLE_RESULT_ONGOING,
        .fleeAttempts = 0
    };

    while (state.result == BATTLE_RESULT_ONGOING) {
        // 1. print battle status
        // 2. get player action
        // 3. resolve turn
        // 4. check battle over
    }

    return state.result;
}