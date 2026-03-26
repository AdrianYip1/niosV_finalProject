#include "battleLoop.h"
#include "../entities/pokemonObject.h"
#include "../bag.h"
#include "../../../hardware/audio.h"
#include "../../se/recover_audio.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void battleClearMessages(BattleState *state) {
    if (state == NULL) return;
    state->messageCount = 0;
    state->messageReadIndex = 0;
    for (int i = 0; i < BATTLE_MSG_MAX; i++) state->messages[i][0] = '\0';
}

static void battlePushMessage(BattleState *state, const char *msg) {
    if (state == NULL || msg == NULL) return;
    if (state->messageCount < 0) state->messageCount = 0;
    if (state->messageCount >= BATTLE_MSG_MAX) return;
    snprintf(state->messages[state->messageCount], sizeof(state->messages[state->messageCount]), "%s", msg);
    state->messageCount++;
}

static void battlePushUsedMessage(BattleState *state, const pokemonInBattle *attacker, const AttackData *move, bool opposing, const pokemonInBattle *target) {
    const char *attackerName = (attacker != NULL && attacker->id.data != NULL && attacker->id.data->name != NULL)
                                   ? attacker->id.data->name
                                   : "???";
    const char *moveName = (move != NULL && move->name != NULL) ? move->name : "???";
    char buf[96];

    const float eff = (move != NULL) ? getTypeEffectiveness(move->type, target->type1, target->type2) : 1.0f;
    const bool hasEffectivenessMsg = (eff == 0.0f) || (eff < 0.99f) || (eff > 1.01f);
    char effectivenessBuf[96];
    if (hasEffectivenessMsg) {
        if (eff == 0.0f) {
            snprintf(effectivenessBuf, sizeof(effectivenessBuf), "It had no effect!");
        } else if (eff < 1.0f) {
            snprintf(effectivenessBuf, sizeof(effectivenessBuf), "It's not very effective...");
        } else {
            snprintf(effectivenessBuf, sizeof(effectivenessBuf), "It's super effective!");
        }
    }

    if (opposing) {
        snprintf(buf, sizeof(buf), "Opposing %s used %s!", attackerName, moveName);
    }
    else {
        snprintf(buf, sizeof(buf), "%s used %s!", attackerName, moveName);
    }

    battlePushMessage(state, buf);
    if (hasEffectivenessMsg) {
        battlePushMessage(state, effectivenessBuf);
    }
}

static int aiChooseMove(pokemonInBattle *pokemon) {
    if (pokemon == NULL) return 0;

    // Prefer attacking moves so the enemy actually deals damage.
    int candidates[4];
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (pokemon->attacks[i] != NULL && pokemon->currentPP[i] > 0) {
            const AttackData *m = pokemon->attacks[i];
            if (m->category != ATTACK_STATUS && m->power > 0) {
                candidates[count++] = i;
            }
        }
    }
    if (count > 0) return candidates[rand() % count];

    // Fallback: any move with PP.
    count = 0;
    for (int i = 0; i < 4; i++) {
        if (pokemon->attacks[i] != NULL && pokemon->currentPP[i] > 0) candidates[count++] = i;
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

static void resolveAttack(BattleState *state, pokemonInBattle *attacker, pokemonInBattle *target, int moveIndex, bool opposing) {
    if (attacker == NULL || target == NULL) return;
    if (!canAct(attacker)) return;

    const AttackData *move = (moveIndex >= 0 && moveIndex < 4) ? attacker->attacks[moveIndex] : NULL;
    if (move != NULL) battlePushUsedMessage(state, attacker, move, opposing, target);
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
            resolveAttack(state, player, enemy, param, false);
            break;
        case ACTION_SWITCH:
            {
                const int before = state->playerParty->activeIndex;
                resolveSwitch(state->playerParty, param);
                const int after = state->playerParty->activeIndex;
                if (after != before) {
                    pokemonInBattle *nowActive = getActivePokemon(state->playerParty);
                    const char *name = (nowActive != NULL && nowActive->id.data != NULL && nowActive->id.data->name != NULL)
                                           ? nowActive->id.data->name
                                           : "???";
                    char buf[96];
                    snprintf(buf, sizeof(buf), "Go! %s!", name);
                    battlePushMessage(state, buf);
                }
            }
            break;
        case ACTION_ITEM:
            {
                if (state->playerBag == NULL) {
                    battlePushMessage(state, "No bag!");
                    break;
                }

                const ItemId item = (ItemId)param;
                const char *name = itemName(item);
                char buf[96];

                if (item == ITEM_NONE) {
                    battlePushMessage(state, "No item selected!");
                    break;
                }

                if (bagCount(state->playerBag, item) <= 0) {
                    snprintf(buf, sizeof(buf), "You're out of %s!", name);
                    battlePushMessage(state, buf);
                    break;
                }

                // Balls only work in wild battles.
                if (itemIsBall(item) && state->type != BATTLE_WILD) {
                    battlePushMessage(state, "You can't catch that Pokemon!");
                    break;
                }

                (void)bagRemove(state->playerBag, item, 1);

                snprintf(buf, sizeof(buf), "Used %s!", name);
                battlePushMessage(state, buf);

                if (itemIsHealing(item)) {
                    play_sfx(recover_audio, recover_audio_len);
                    int heal = 0;
                    if (item == ITEM_POTION) heal = 20;
                    else if (item == ITEM_SUPER_POTION) heal = 50;
                    else if (item == ITEM_HYPER_POTION) heal = 200;

                    if (item == ITEM_FULL_RESTORE) {
                        fullHeal(player);
                        battlePushMessage(state, "Restored health!");
                    } else {
                        healPokemon(player, heal);
                        battlePushMessage(state, "Recovered HP!");
                    }
                    break;
                }

                if (itemIsBall(item)) {
                    PokeballType ball = POKEBALL_POKE;
                    if (item == ITEM_POKEBALL) ball = POKEBALL_POKE;
                    else if (item == ITEM_GREAT_BALL) ball = POKEBALL_GREAT;
                    else if (item == ITEM_ULTRA_BALL) ball = POKEBALL_ULTRA;
                    else if (item == ITEM_PREMIER_BALL) ball = POKEBALL_PREMIER;
                    else if (item == ITEM_MASTER_BALL) ball = POKEBALL_MASTER;

                    const bool caught = attemptCatchWithBall(enemy, ball);
                    if (caught) {
                        battlePushMessage(state, "Gotcha!");
                        state->result = BATTLE_RESULT_CAUGHT;
                    } else {
                        battlePushMessage(state, "Oh no! It broke free!");
                    }
                    break;
                }
            }
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
    resolveAttack(state, enemy, player, moveIndex, true);
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
    } else if (playerAction == ACTION_ATTACK) { //need to check pp conditions, print supereffective/not effective
        const int enemyMove = aiChooseMove(enemy);
        const int order = determineTurnOrder(player, enemy); // 1=player first, 2=enemy first
        if (order == 1) {
            resolveAttack(state, player, enemy, playerParam, false);
            if (!enemy->alive) {
                handleFaint(state, state->enemyParty, false);
            }
            if (state->result == BATTLE_RESULT_ONGOING && enemy->alive) {
                resolveAttack(state, enemy, player, enemyMove, true);
                if (!player->alive) {
                    handleFaint(state, state->playerParty, true);
                }
            }
        } else {
            resolveAttack(state, enemy, player, enemyMove, true);
            if (!player->alive) {
                handleFaint(state, state->playerParty, true);
            }
            if (state->result == BATTLE_RESULT_ONGOING && player->alive) {
                resolveAttack(state, player, enemy, playerParam, false);
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

void initBattleState(BattleState *state, Party *playerParty, Party *enemyParty, Bag *playerBag, BattleType type) {
    if (state == NULL) return;
    state->playerParty = playerParty;
    state->enemyParty = enemyParty;
    state->playerBag = playerBag;
    state->type = type;
    state->result = BATTLE_RESULT_ONGOING;
    state->fleeAttempts = 0;
    battleClearMessages(state);
}

void battleApplyPlayerAction(BattleState *state, BattleAction action, int param) {
    if (state == NULL) return;
    if (state->result != BATTLE_RESULT_ONGOING) return;
    battleClearMessages(state);
    resolveTurn(state, action, param);
    checkBattleOver(state);
}

BattleResult runBattle(Party *playerParty, Party *enemyParty, BattleType type) {
    BattleState state;
    initBattleState(&state, playerParty, enemyParty, NULL, type);


    while (state.result == BATTLE_RESULT_ONGOING) {
        pokemonInBattle *player = getActivePokemon(state.playerParty);
        const int moveIndex = aiChooseMove(player);
        battleApplyPlayerAction(&state, ACTION_ATTACK, moveIndex);
    }

    return state.result;
}

