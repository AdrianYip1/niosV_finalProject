#include "battleLoop.h"
#include <stdbool.h>
#include <stdlib.h>

static int aiChooseMove(pokemonInBattle *pokemon) {
    if (pokemon == NULL) return 0;

    int candidates[4];
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (pokemon->attacks[i] != NULL && pokemon->currentPP[i] > 0) {
            candidates[count++] = i;
        }
    }
    if (count == 0) {
        for (int i = 0; i < 4; i++) {
            if (pokemon->attacks[i] != NULL) candidates[count++] = i;
        }
    }
    if (count == 0) return 0;
    return candidates[rand() % count];
}

static void checkBattleOver(BattleState *state) {
    if (state == NULL) return;
    if (state->playerParty != NULL && !hasAlivePokemon(state->playerParty)) {
        state->result = BATTLE_RESULT_PLAYER_LOSE;
    }
    if (state->enemyParty != NULL && !hasAlivePokemon(state->enemyParty)) {
        state->result = BATTLE_RESULT_PLAYER_WIN;
    }
}

static void handleFaint(BattleState *state, Party *party, bool isPlayer) {
    if (state == NULL || party == NULL) return;

    if (!hasAlivePokemon(party)) {
        state->result = isPlayer ? BATTLE_RESULT_PLAYER_LOSE : BATTLE_RESULT_PLAYER_WIN;
        return;
    }

    const int next = getFirstAlivePokemon(party);
    if (next >= 0) party->activeIndex = next;
}

static void resolveAttack(pokemonInBattle *attacker, pokemonInBattle *target, int moveIndex) {
    if (attacker == NULL || target == NULL) return;
    if (!canAct(attacker)) return;
    (void)useAttack(attacker, target, moveIndex);
}

static void resolveSwitch(Party *party, int slot) {
    if (party == NULL) return;
    (void)switchPokemon(party, slot);
}

static void resolveFlee(BattleState *state) {
    if (state == NULL) return;
    if (state->type != BATTLE_WILD) return;
    if (state->playerParty == NULL || state->enemyParty == NULL) return;

    pokemonInBattle *player = getActivePokemon(state->playerParty);
    pokemonInBattle *enemy = getActivePokemon(state->enemyParty);
    if (player == NULL || enemy == NULL) return;

    state->fleeAttempts++;
    if (attemptFlee(player, enemy)) {
        state->result = BATTLE_RESULT_FLED;
    }
}

static void resolvePlayerTurn(BattleState *state, BattleAction action, int param) {
    if (state == NULL) return;
    if (state->playerParty == NULL || state->enemyParty == NULL) return;

    pokemonInBattle *player = getActivePokemon(state->playerParty);
    pokemonInBattle *enemy = getActivePokemon(state->enemyParty);
    if (player == NULL || enemy == NULL) return;

    switch (action) {
        case ACTION_ATTACK:
            resolveAttack(player, enemy, param);
            break;
        case ACTION_SWITCH:
            resolveSwitch(state->playerParty, param);
            break;
        case ACTION_ITEM:
            // todo: implement bag/items; for now it consumes the player's decision.
            break;
        case ACTION_RUN:
            resolveFlee(state);
            break;
        default:
            break;
    }
}

static void resolveEnemyTurn(BattleState *state) {
    if (state == NULL) return;
    if (state->result != BATTLE_RESULT_ONGOING) return;
    if (state->playerParty == NULL || state->enemyParty == NULL) return;

    pokemonInBattle *enemy = getActivePokemon(state->enemyParty);
    pokemonInBattle *player = getActivePokemon(state->playerParty);
    if (enemy == NULL || player == NULL) return;

    if (!enemy->alive) {
        handleFaint(state, state->enemyParty, false);
        if (state->result != BATTLE_RESULT_ONGOING) return;
        enemy = getActivePokemon(state->enemyParty);
        if (enemy == NULL) return;
    }

    const int moveIndex = aiChooseMove(enemy);
    resolveAttack(enemy, player, moveIndex);
}

static void resolveTurn(BattleState *state, BattleAction playerAction, int playerParam) {
    if (state == NULL) return;
    if (state->result != BATTLE_RESULT_ONGOING) return;
    if (state->playerParty == NULL || state->enemyParty == NULL) return;

    pokemonInBattle *player = getActivePokemon(state->playerParty);
    pokemonInBattle *enemy = getActivePokemon(state->enemyParty);
    if (player == NULL || enemy == NULL) return;

    // Ensure both sides have a live active before the turn.
    if (!player->alive) {
        handleFaint(state, state->playerParty, true);
        if (state->result != BATTLE_RESULT_ONGOING) return;
        player = getActivePokemon(state->playerParty);
        if (player == NULL) return;
    }
    if (!enemy->alive) {
        handleFaint(state, state->enemyParty, false);
        if (state->result != BATTLE_RESULT_ONGOING) return;
        enemy = getActivePokemon(state->enemyParty);
        if (enemy == NULL) return;
    }

    if (playerAction == ACTION_RUN) {
        resolvePlayerTurn(state, playerAction, playerParam);
        if (state->result == BATTLE_RESULT_FLED) return;
        resolveEnemyTurn(state);
    } else if (playerAction == ACTION_ATTACK) {
        const int enemyMove = aiChooseMove(enemy);
        const int order = determineTurnOrder(player, enemy); // 1=player first, 2=enemy first
        if (order == 1) {
            resolveAttack(player, enemy, playerParam);
            if (!enemy->alive) {
                handleFaint(state, state->enemyParty, false);
            }
            if (state->result == BATTLE_RESULT_ONGOING && enemy->alive) {
                resolveAttack(enemy, player, enemyMove);
                if (!player->alive) {
                    handleFaint(state, state->playerParty, true);
                }
            }
        } else {
            resolveAttack(enemy, player, enemyMove);
            if (!player->alive) {
                handleFaint(state, state->playerParty, true);
            }
            if (state->result == BATTLE_RESULT_ONGOING && player->alive) {
                resolveAttack(player, enemy, playerParam);
                if (!enemy->alive) {
                    handleFaint(state, state->enemyParty, false);
                }
            }
        }
    } else {
        // Switch / Item first, then enemy attacks.
        resolvePlayerTurn(state, playerAction, playerParam);
        if (state->result != BATTLE_RESULT_ONGOING) return;
        resolveEnemyTurn(state);
        if (state->result != BATTLE_RESULT_ONGOING) return;
        player = getActivePokemon(state->playerParty);
        if (player != NULL && !player->alive) {
            handleFaint(state, state->playerParty, true);
        }
    }

    if (state->result != BATTLE_RESULT_ONGOING) return;
    player = getActivePokemon(state->playerParty);
    enemy = getActivePokemon(state->enemyParty);
    if (player != NULL) tickStatusEffect(player);
    if (enemy != NULL) tickStatusEffect(enemy);
}

void initBattleState(BattleState *state, Party *playerParty, Party *enemyParty, BattleType type) {
    if (state == NULL) return;
    state->playerParty = playerParty;
    state->enemyParty = enemyParty;
    state->type = type;
    state->result = BATTLE_RESULT_ONGOING;
    state->fleeAttempts = 0;
}

void battleApplyPlayerAction(BattleState *state, BattleAction action, int param) {
    if (state == NULL) return;
    if (state->result != BATTLE_RESULT_ONGOING) return;
    resolveTurn(state, action, param);
    checkBattleOver(state);
}

BattleResult runBattle(Party *playerParty, Party *enemyParty, BattleType type) {
    BattleState state;
    initBattleState(&state, playerParty, enemyParty, type);


    while (state.result == BATTLE_RESULT_ONGOING) {
        pokemonInBattle *player = getActivePokemon(state.playerParty);
        const int moveIndex = aiChooseMove(player);
        battleApplyPlayerAction(&state, ACTION_ATTACK, moveIndex);
    }

    return state.result;
}

