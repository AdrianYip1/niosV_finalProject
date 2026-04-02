#include "battleLoop.h"
#include "../entities/pokemonObject.h"
#include "../bag.h"
#include "../../../hardware/audio.h"
#include "../../se/recover_audio.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


//TODO if move misses, have ban indication, also PP should still go down
//also fullrestore should not restore pp

static void battleClearMessages(BattleState *state) {
    if (state == NULL) return;
    state->messageCount = 0;
    state->messageReadIndex = 0;
    for (int i = 0; i < BATTLE_MSG_MAX; i++) {
        state->messages[i][0] = '\0';
        state->messageFlags[i] = 0;
        state->hitEffects[i] = (signed char)BATTLE_HIT_EFFECT_NORMAL;
        state->hpAfterPlayer[i] = -1;
        state->hpAfterEnemy[i] = -1;
        state->displayPlayerIndex[i] = -1;
        state->displayEnemyIndex[i] = -1;
    }
}

static void battlePushMessage(BattleState *state, const char *msg) {
    if (state == NULL || msg == NULL) return;
    if (state->messageCount < 0) state->messageCount = 0;
    if (state->messageCount >= BATTLE_MSG_MAX) return;
    const int idx = state->messageCount;
    snprintf(state->messages[idx], sizeof(state->messages[idx]), "%s", msg);
    state->messageFlags[idx] = 0;
    state->hitEffects[idx] = (signed char)BATTLE_HIT_EFFECT_NORMAL;
    state->hpAfterPlayer[idx] = -1;
    state->hpAfterEnemy[idx] = -1;
    state->displayPlayerIndex[idx] = (state->playerParty != NULL) ? (signed char)state->playerParty->activeIndex : (signed char)-1;
    state->displayEnemyIndex[idx] = (state->enemyParty != NULL) ? (signed char)state->enemyParty->activeIndex : (signed char)-1;
    state->messageCount++;
}

static void battlePushUsedMessage(BattleState *state, const pokemonInBattle *attacker, const AttackData *move, bool opposing, const pokemonInBattle *target) {
    (void)target;
    const char *attackerName = (attacker != NULL && attacker->id.data != NULL && attacker->id.data->name != NULL)
                                   ? attacker->id.data->name
                                   : "???";
    const char *moveName = (move != NULL && move->name != NULL) ? move->name : "???";
    char buf[96];

    if (opposing) {
        snprintf(buf, sizeof(buf), "Opposing %s used %s!", attackerName, moveName);
    }
    else {
        snprintf(buf, sizeof(buf), "%s used %s!", attackerName, moveName);
    }

    battlePushMessage(state, buf);
}

static void battlePushEffectivenessMessage(BattleState *state, float eff) {
    if (state == NULL) return;
    const bool hasEffectivenessMsg = (eff == 0.0f) || (eff < 0.99f) || (eff > 1.01f);
    if (!hasEffectivenessMsg) return;

    if (eff == 0.0f) {
        battlePushMessage(state, "It had no effect!");
    } else if (eff < 1.0f) {
        battlePushMessage(state, "It's not very effective...");
    } else {
        battlePushMessage(state, "It's super effective!");
    }
}

static void battlePushMissMessage(BattleState *state) {
    if (state == NULL) return;
    battlePushMessage(state, "But it missed!");
}

static void battlePushFaintedMessage(BattleState *state, const pokemonInBattle *pokemon, bool opposing);

static void battleFormatPokemonName(char *out, size_t outSize, const pokemonInBattle *pokemon, bool opposing) {
    const char *name = (pokemon != NULL && pokemon->id.data != NULL && pokemon->id.data->name != NULL)
                           ? pokemon->id.data->name
                           : "???";
    if (opposing) snprintf(out, outSize, "Opposing %s", name);
    else snprintf(out, outSize, "%s", name);
}

static void battlePushFailedMessage(BattleState *state) {
    if (state == NULL) return;
    battlePushMessage(state, "But it failed!");
}

static bool pokemonHasType(const pokemonInBattle *pokemon, PokemonType type) {
    if (pokemon == NULL) return false;
    return (pokemon->type1 == type) || (pokemon->type2 == type);
}

static bool statusCanApplyToTarget(StatusCondition status, const pokemonInBattle *target) {
    if (target == NULL) return false;
    if (status == STATUS_POISON && (pokemonHasType(target, TYPE_POISON) || pokemonHasType(target, TYPE_STEEL))) return false;
    if (status == STATUS_BURN && pokemonHasType(target, TYPE_FIRE)) return false;
    if (status == STATUS_FREEZE && pokemonHasType(target, TYPE_ICE)) return false;
    return true;
}

static void battlePushStatusInflictedMessage(BattleState *state, const pokemonInBattle *target, bool targetOpposing, StatusCondition status) {
    if (state == NULL) return;
    char name[64];
    battleFormatPokemonName(name, sizeof(name), target, targetOpposing);
    char buf[96];

    switch (status) {
        case STATUS_BURN: snprintf(buf, sizeof(buf), "%s was burned!", name); break;
        case STATUS_POISON: snprintf(buf, sizeof(buf), "%s was poisoned!", name); break;
        case STATUS_PARALYSIS: snprintf(buf, sizeof(buf), "%s was paralyzed!", name); break;
        case STATUS_SLEEP: snprintf(buf, sizeof(buf), "%s fell asleep!", name); break;
        case STATUS_FREEZE: snprintf(buf, sizeof(buf), "%s was frozen solid!", name); break;
        default: return;
    }
    battlePushMessage(state, buf);
}

static bool battleTryInflictStatus(BattleState *state, pokemonInBattle *target, bool targetOpposing, StatusCondition status) {
    if (state == NULL || target == NULL) return false;
    if (!target->alive) return false;
    if (target->status != STATUS_NONE) return false;
    if (!statusCanApplyToTarget(status, target)) return false;

    applyStatusEffect(target, status);
    if (target->status == status) {
        battlePushStatusInflictedMessage(state, target, targetOpposing, status);
        return true;
    }
    return false;
}

static int clampStageLocal(int stage) {
    if (stage > 6) return 6;
    if (stage < -6) return -6;
    return stage;
}

static BattleHitEffect battleApplyStageDelta(BattleState *state, pokemonInBattle *pokemon, bool opposing, const char *statName, int *stageField, int delta) {
    if (state == NULL || pokemon == NULL || statName == NULL || stageField == NULL) return BATTLE_HIT_EFFECT_NORMAL;
    char name[64];
    battleFormatPokemonName(name, sizeof(name), pokemon, opposing);

    const int before = *stageField;
    const int after = clampStageLocal(before + delta);
    *stageField = after;

    if (after == before) { //if maxed out a certain stat
        char buf[96];
        snprintf(buf, sizeof(buf), "%s's %s won't go any %s!", name, statName, (delta > 0) ? "higher" : "lower");
        battlePushMessage(state, buf);
        return BATTLE_HIT_EFFECT_NORMAL;
    }

    const int actualDelta = after - before;
    const int absDelta = (actualDelta < 0) ? -actualDelta : actualDelta;
    const bool up = (actualDelta > 0);
    const char *verb = up ? "rose" : "fell";
    if (absDelta >= 2) verb = up ? "sharply rose" : "harshly fell";

    char buf[96];
    snprintf(buf, sizeof(buf), "%s's %s %s!", name, statName, verb);
    battlePushMessage(state, buf);
    return up ? BATTLE_HIT_EFFECT_STATS_UP : BATTLE_HIT_EFFECT_STATS_DOWN;
}

static void battlePushCannotActMessage(BattleState *state, const pokemonInBattle *pokemon, bool opposing, StatusCondition reason) {
    if (state == NULL || pokemon == NULL) return;
    char name[64];
    battleFormatPokemonName(name, sizeof(name), pokemon, opposing);
    char buf[96];

    switch (reason) {
        case STATUS_SLEEP: snprintf(buf, sizeof(buf), "%s is fast asleep!", name); break;
        case STATUS_FREEZE: snprintf(buf, sizeof(buf), "%s is frozen solid!", name); break;
        case STATUS_PARALYSIS: snprintf(buf, sizeof(buf), "%s is paralyzed! It can't move!", name); break;
        default: return;
    }
    battlePushMessage(state, buf);
}

static void battleTickStatusWithMessages(BattleState *state, pokemonInBattle *pokemon, bool opposing) {
    if (state == NULL || pokemon == NULL) return;
    if (!pokemon->alive) return;

    const StatusCondition beforeStatus = pokemon->status;
    const int beforeHp = pokemon->scaledStatsWithLevel[0];

    tickStatusEffect(pokemon);

    const int afterHp = pokemon->scaledStatsWithLevel[0];
    const StatusCondition afterStatus = pokemon->status;

    char name[64];
    battleFormatPokemonName(name, sizeof(name), pokemon, opposing);

    if (beforeStatus == STATUS_BURN && afterHp < beforeHp) {
        char buf[96];
        snprintf(buf, sizeof(buf), "%s is hurt by its burn!", name);
        battlePushMessage(state, buf);
        if (state->messageCount > 0) {
            const int idx = state->messageCount - 1;
            if (idx >= 0 && idx < BATTLE_MSG_MAX) {
                if (opposing) state->hpAfterEnemy[idx] = afterHp;
                else state->hpAfterPlayer[idx] = afterHp;
            }
        }
    } else if (beforeStatus == STATUS_POISON && afterHp < beforeHp) {
        char buf[96];
        snprintf(buf, sizeof(buf), "%s is hurt by poison!", name);
        battlePushMessage(state, buf);
        if (state->messageCount > 0) {
            const int idx = state->messageCount - 1;
            if (idx >= 0 && idx < BATTLE_MSG_MAX) {
                if (opposing) state->hpAfterEnemy[idx] = afterHp;
                else state->hpAfterPlayer[idx] = afterHp;
            }
        }
    }

    if (beforeStatus == STATUS_SLEEP && afterStatus == STATUS_NONE) {
        char buf[96];
        snprintf(buf, sizeof(buf), "%s woke up!", name);
        battlePushMessage(state, buf);
    } else if (beforeStatus == STATUS_FREEZE && afterStatus == STATUS_NONE) {
        char buf[96];
        snprintf(buf, sizeof(buf), "%s thawed out!", name);
        battlePushMessage(state, buf);
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

static void battlePushFaintedMessage(BattleState *state, const pokemonInBattle *pokemon, bool opposing) {
    const char *name = (pokemon != NULL && pokemon->id.data != NULL && pokemon->id.data->name != NULL)
                           ? pokemon->id.data->name
                           : "???";
    char buf[96];
    if (opposing) {
        snprintf(buf, sizeof(buf), "Opposing %s fainted!", name);
    } else {
        snprintf(buf, sizeof(buf), "%s fainted!", name);
    }
    battlePushMessage(state, buf);
}

static void battlePushExpMessage(BattleState *state, int expGained) {
    if (state == NULL) return;
    if (expGained <= 0) return;
    char buf[96];
    snprintf(buf, sizeof(buf), "Gained %d EXP!", expGained);
    battlePushMessage(state, buf);
}

static void awardExpForDefeat(BattleState *state, pokemonInBattle *player, pokemonInBattle *defeated, bool defeatedWasOpposing) {
    if (state == NULL || player == NULL || defeated == NULL) return;
    if (defeated->alive) return;

    battlePushFaintedMessage(state, defeated, defeatedWasOpposing);
    const int expGained = experienceGained(player->level, defeated->level);
    const int prevLevel = player->level;
    gainExp(player, defeated);
    battlePushExpMessage(state, expGained);
    if (player->level > prevLevel) {
        const char *name = (player->id.data != NULL && player->id.data->name != NULL) ? player->id.data->name : "???";
        char buf[96];
        snprintf(buf, sizeof(buf), "%s grew to level %d!", name, player->level);
        battlePushMessage(state, buf);
    }
}

static bool resolveAttack(BattleState *state, pokemonInBattle *attacker, pokemonInBattle *target, int moveIndex, bool opposing) {
    if (attacker == NULL || target == NULL) return false;
    StatusCondition blockedBy = STATUS_NONE;
    if (!canActThisTurn(attacker, &blockedBy)) { //like with status effects
        battlePushCannotActMessage(state, attacker, opposing, blockedBy);
        return false;
    }

    const AttackData *move = (moveIndex >= 0 && moveIndex < 4) ? attacker->attacks[moveIndex] : NULL;
    const int usedMsgIndex = (state != NULL) ? state->messageCount : 0;

    if (move != NULL) battlePushUsedMessage(state, attacker, move, opposing, target);

    const bool aliveBefore = target->alive;
    const int hpBefore = target->scaledStatsWithLevel[0];
    const int ppBefore = (moveIndex >= 0 && moveIndex < 4) ? attacker->currentPP[moveIndex] : 0;
    const bool hit = useAttack(attacker, target, moveIndex);
    const int ppAfter = (moveIndex >= 0 && moveIndex < 4) ? attacker->currentPP[moveIndex] : 0;
    const int hpAfter = target->scaledStatsWithLevel[0];
    const int tookDamage = (hpBefore > hpAfter) ? 1 : 0;
    const bool faintedNow = aliveBefore && (!target->alive);
    BattleHitEffect usedMsgEffect = BATTLE_HIT_EFFECT_NORMAL;

    // Miss message (PP is still consumed).
    if (!hit && move != NULL && ppBefore > 0 && ppAfter == ppBefore - 1) {
        battlePushMissMessage(state);
    }

    if (hit && move != NULL) {
        const bool targetOpposing = !opposing;
        const int damageDealt = (hpBefore > hpAfter) ? (hpBefore - hpAfter) : 0;

        if (move->category == ATTACK_STATUS) {
            switch (move->id) {
                case 3: // Growl
                    usedMsgEffect = battleApplyStageDelta(state, target, targetOpposing, "Attack", &target->statStageAttack, -1);
                    break;
                case 5: // Scary Face
                    usedMsgEffect = battleApplyStageDelta(state, target, targetOpposing, "Speed", &target->statStageSpeed, -2);
                    break;
                case 6: // Smokescreen
                    usedMsgEffect = battleApplyStageDelta(state, target, targetOpposing, "accuracy", &target->statStageAccuracy, -1);
                    break;
                case 54: // Dragon Dance
                    usedMsgEffect = battleApplyStageDelta(state, attacker, opposing, "Attack", &attacker->statStageAttack, +1);
                    {
                        const BattleHitEffect speedEffect =
                            battleApplyStageDelta(state, attacker, opposing, "Speed", &attacker->statStageSpeed, +1);
                        if (usedMsgEffect == BATTLE_HIT_EFFECT_NORMAL) usedMsgEffect = speedEffect;
                    }
                    break;
                case 57: // Rest
                    {
                        if (attacker->scaledStatsWithLevel[0] >= attacker->maxHp) {
                            battlePushFailedMessage(state);
                            break;
                        }
                        // Rest: fully heal, cure status, then sleep for 2 turns.
                        attacker->status = STATUS_NONE;
                        attacker->sleepTurnsRemaining = 0;
                        healPokemon(attacker, attacker->maxHp);
                        attacker->status = STATUS_SLEEP;
                        attacker->sleepTurnsRemaining = 2;
                        battlePushStatusInflictedMessage(state, attacker, opposing, STATUS_SLEEP);
                        battlePushMessage(state, "Recovered HP!");
                        if (state->messageCount > 0) {
                            const int idx = state->messageCount - 1;
                            if (idx >= 0 && idx < BATTLE_MSG_MAX) {
                                if (opposing) state->hpAfterEnemy[idx] = attacker->scaledStatsWithLevel[0];
                                else state->hpAfterPlayer[idx] = attacker->scaledStatsWithLevel[0];
                            }
                        }
                    }
                    break;
                default:
                    battlePushFailedMessage(state);
                    break;
            }
        } else {
            // Draining moves.
            if (move->id == 71 && damageDealt > 0) { // Giga Drain
                const int beforeHealHp = attacker->scaledStatsWithLevel[0];
                int heal = damageDealt / 2;
                if (heal < 1) heal = 1;
                healPokemon(attacker, heal);
                const int gained = attacker->scaledStatsWithLevel[0] - beforeHealHp;
                if (gained > 0) {
                    char attackerName[64];
                    battleFormatPokemonName(attackerName, sizeof(attackerName), attacker, opposing);
                    char buf[96];
                    snprintf(buf, sizeof(buf), "%s absorbed %d HP!", attackerName, gained);
                    battlePushMessage(state, buf);
                    if (state->messageCount > 0) {
                        const int idx = state->messageCount - 1;
                        if (idx >= 0 && idx < BATTLE_MSG_MAX) {
                            if (opposing) state->hpAfterEnemy[idx] = attacker->scaledStatsWithLevel[0];
                            else state->hpAfterPlayer[idx] = attacker->scaledStatsWithLevel[0];
                        }
                    }
                }
            }

            // Secondary status effects 
            if (damageDealt > 0 && target->alive) {
                StatusCondition status = STATUS_NONE;
                int chance = 0;
                switch (move->id) {
                    case 20: status = STATUS_BURN; chance = 10; break; // Ember
                    case 21: status = STATUS_BURN; chance = 10; break; // Fire Fang
                    case 22: status = STATUS_BURN; chance = 10; break; // Flamethrower
                    case 24: status = STATUS_BURN; chance = 100; break; // Inferno
                    case 67: status = STATUS_FREEZE; chance = 10; break; // Ice Beam
                    case 70: status = STATUS_POISON; chance = 30; break; // Sludge Bomb
                    case 75: status = STATUS_POISON; chance = 30; break; // Poison Jab
                    default: break;
                }
                if (status != STATUS_NONE && chance > 0) {
                    if ((rand() % 100) < chance) {
                        (void)battleTryInflictStatus(state, target, targetOpposing, status);
                    }
                }
            }
        }
    }

    // Effectiveness message only when the move actually hits and is damaging.
    if (hit && move != NULL && move->category != ATTACK_STATUS) {
        const float eff = getTypeEffectiveness(move->type, target->type1, target->type2);
        battlePushEffectivenessMessage(state, eff);
    }

    // check flag for damage in message 
    if (state != NULL && usedMsgIndex >= 0 && usedMsgIndex < BATTLE_MSG_MAX) {
        state->messageFlags[usedMsgIndex] = (unsigned char)tookDamage;
        state->hitEffects[usedMsgIndex] = (signed char)usedMsgEffect;
        if (tookDamage && hit && move != NULL && move->category != ATTACK_STATUS) {
            const float eff = getTypeEffectiveness(move->type, target->type1, target->type2);
            if (eff > 1.01f) state->hitEffects[usedMsgIndex] = (signed char)BATTLE_HIT_EFFECT_SUPER_EFFECTIVE;
            else if (eff < 0.99f) state->hitEffects[usedMsgIndex] = (signed char)BATTLE_HIT_EFFECT_NOT_EFFECTIVE;
            else state->hitEffects[usedMsgIndex] = (signed char)BATTLE_HIT_EFFECT_NORMAL;
        }
        // HP should change during the "used" message for better timing in the UI.
        if (tookDamage) {
            if (opposing) state->hpAfterPlayer[usedMsgIndex] = hpAfter;
            else state->hpAfterEnemy[usedMsgIndex] = hpAfter;
        }
    }

    return faintedNow;
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

                const int packed = param;
                const ItemId item = (ItemId)(packed & 0xFF);
                const int targetSlotPlus1 = (packed >> 8) & 0xFF; // 0=active, else 1..6 => slots 0..5

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

                pokemonInBattle *target = player;
                if (targetSlotPlus1 > 0) {
                    const int idx = targetSlotPlus1 - 1;
                    if (state->playerParty == NULL || idx < 0 || idx >= state->playerParty->count) {
                        battlePushMessage(state, "No target!");
                        break;
                    }
                    target = state->playerParty->slots[idx];
                }
                if (target == NULL) {
                    battlePushMessage(state, "No target!");
                    break;
                }

                if (itemIsHealing(item)) {
                    // Revives require a fainted target
                    if (itemIsRevive(item)) {
                        if (target->alive) {
                            battlePushMessage(state, "It won't have any effect!");
                            break;
                        }
                    } else {
                        if (!target->alive) {
                            battlePushMessage(state, "It won't have any effect!");
                            break;
                        }
                    }

                    (void)bagRemove(state->playerBag, item, 1);
                    snprintf(buf, sizeof(buf), "Used %s!", name);
                    battlePushMessage(state, buf);

                    play_sfx(recover_audio, recover_audio_len);

                    if (itemIsRevive(item)) {
                        target->alive = true;
                        target->status = STATUS_NONE;
                        target->sleepTurnsRemaining = 0;
                        const int maxHp = (target->maxHp > 0) ? target->maxHp : 1;
                        int hp = (item == ITEM_MAX_REVIVE) ? maxHp : (maxHp / 2);
                        if (hp < 1) hp = 1;
                        if (hp > maxHp) hp = maxHp;
                        target->scaledStatsWithLevel[0] = hp;
                        battlePushMessage(state, "Revived!");
                    } else if (item == ITEM_FULL_RESTORE) {
                        fullRestore(target);
                        battlePushMessage(state, "Restored health!");
                    } else {
                        int heal = 0;
                        if (item == ITEM_POTION) heal = 20;
                        else if (item == ITEM_SUPER_POTION) heal = 60;
                        else if (item == ITEM_HYPER_POTION) heal = 120;
                        healPokemon(target, heal);
                        battlePushMessage(state, "Recovered HP!");
                    }

                    const bool targetIsActive = (state->playerParty != NULL) &&
                                                (target == getActivePokemon(state->playerParty));
                    if (targetIsActive && state->messageCount > 0) {
                        const int msgIdx = state->messageCount - 1;
                        if (msgIdx >= 0 && msgIdx < BATTLE_MSG_MAX) {
                            state->hpAfterPlayer[msgIdx] = target->scaledStatsWithLevel[0];
                        }
                    }
                    break;
                }

                if (itemIsBall(item)) {
                    (void)bagRemove(state->playerBag, item, 1);
                    snprintf(buf, sizeof(buf), "Used %s!", name);
                    battlePushMessage(state, buf);

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

                if (item == ITEM_SHINY_STONE) {
                    if (!target->alive) {
                        battlePushMessage(state, "It won't have any effect!");
                        break;
                    }
                    if (target->id.data == &TOGETIC) {
                        (void)bagRemove(state->playerBag, item, 1);
                        snprintf(buf, sizeof(buf), "Used %s!", name);
                        battlePushMessage(state, buf);

                        target->pendingEvolutionInto = &TOGEKISS;
                        (void)applyPendingEvolution(target);
                        battlePushMessage(state, "Evolved!");
                    } else {
                        battlePushMessage(state, "It won't have any effect!");
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
    if (state->playerMustSwitch) return;
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
    (void)resolveAttack(state, enemy, player, moveIndex, true);
}

static void resolveTurn(BattleState *state, BattleAction playerAction, int playerParam) {
    if (state == NULL) return;
    if (state->result != BATTLE_RESULT_ONGOING) return;
    if (state->playerMustSwitch) return;
    if (state->playerParty == NULL || state->enemyParty == NULL) return;

    pokemonInBattle *player = getActivePokemon(state->playerParty);
    pokemonInBattle *enemy = getActivePokemon(state->enemyParty);
    if (player == NULL || enemy == NULL) return;

    // If player's active is fainted, require a manual switch (no auto-switch).
    if (!player->alive) {
        state->playerMustSwitch = true;
        checkBattleOver(state);
        return;
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
            const bool enemyFaintedNow = resolveAttack(state, player, enemy, playerParam, false);
            if (enemyFaintedNow) {
                awardExpForDefeat(state, player, enemy, true);
                handleFaint(state, state->enemyParty, false);
            }
            if (state->result == BATTLE_RESULT_ONGOING && enemy->alive) {
                (void)resolveAttack(state, enemy, player, enemyMove, true);
                if (!player->alive) {
                    battlePushFaintedMessage(state, player, false);
                    state->playerMustSwitch = true;
                    checkBattleOver(state);
                    return;
                }
            }
        } else {
            (void)resolveAttack(state, enemy, player, enemyMove, true);
            if (!player->alive) {
                battlePushFaintedMessage(state, player, false);
                state->playerMustSwitch = true;
                checkBattleOver(state);
                return;
            }
            if (state->result == BATTLE_RESULT_ONGOING && player->alive) {
                const bool enemyFaintedNow = resolveAttack(state, player, enemy, playerParam, false);
                if (enemyFaintedNow) {
                    awardExpForDefeat(state, player, enemy, true);
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
            battlePushFaintedMessage(state, player, false);
            state->playerMustSwitch = true;
            checkBattleOver(state);
            return;
        }
    }

    if (state->result != BATTLE_RESULT_ONGOING) return;
    player = getActivePokemon(state->playerParty);
    enemy = getActivePokemon(state->enemyParty);
    const bool playerAliveBeforeTick = (player != NULL) ? player->alive : false;
    const bool enemyAliveBeforeTick = (enemy != NULL) ? enemy->alive : false;
    if (player != NULL) battleTickStatusWithMessages(state, player, false);
    if (enemy != NULL) battleTickStatusWithMessages(state, enemy, true);

    // Award EXP immediately if the opposing Pokemon faints due to status at end of turn.
    if (state->result != BATTLE_RESULT_ONGOING) return;
    if (enemy != NULL && enemyAliveBeforeTick && !enemy->alive) {
        awardExpForDefeat(state, player, enemy, true);
        handleFaint(state, state->enemyParty, false);
    }

    if (state->result != BATTLE_RESULT_ONGOING) return;
    if (player != NULL && playerAliveBeforeTick && !player->alive) {
        battlePushFaintedMessage(state, player, false);
        state->playerMustSwitch = true;
        checkBattleOver(state);
        return;
    }
}

void initBattleState(BattleState *state, Party *playerParty, Party *enemyParty, Bag *playerBag, BattleType type) {
    if (state == NULL) return;
    state->playerParty = playerParty;
    state->enemyParty = enemyParty;
    state->playerBag = playerBag;
    state->type = type;
    state->result = BATTLE_RESULT_ONGOING;
    state->fleeAttempts = 0;
    state->playerMustSwitch = false;
    battleClearMessages(state);

    // clear stat stages at start of battle.
    if (state->playerParty != NULL) {
        for (int i = 0; i < state->playerParty->count; i++) {
            if (state->playerParty->slots[i] != NULL) resetStatStages(state->playerParty->slots[i]);
        }
    }
    if (state->enemyParty != NULL) {
        for (int i = 0; i < state->enemyParty->count; i++) {
            if (state->enemyParty->slots[i] != NULL) resetStatStages(state->enemyParty->slots[i]);
        }
    }
}

void battleApplyPlayerAction(BattleState *state, BattleAction action, int param) {
    if (state == NULL) return;
    if (state->result != BATTLE_RESULT_ONGOING) return;
    battleClearMessages(state);

    if (state->playerMustSwitch) {
        if (action != ACTION_SWITCH) {
            battlePushMessage(state, "Choose a Pokemon!");
            return;
        }
        if (state->playerParty == NULL) {
            battlePushMessage(state, "No party!");
            return;
        }

        const int before = state->playerParty->activeIndex;
        (void)switchPokemon(state->playerParty, param);
        const int after = state->playerParty->activeIndex;
        if (after == before) {
            battlePushMessage(state, "Can't switch to that Pokemon!");
            return;
        }

        state->playerMustSwitch = false;
        return;
    }

    resolveTurn(state, action, param);
    checkBattleOver(state);

    //clear stat stages when battle ends (win/lose/flee/caught).
    if (state->result != BATTLE_RESULT_ONGOING) {
        if (state->playerParty != NULL) {
            for (int i = 0; i < state->playerParty->count; i++) {
                if (state->playerParty->slots[i] != NULL) resetStatStages(state->playerParty->slots[i]);
            }
        }
        if (state->enemyParty != NULL) {
            for (int i = 0; i < state->enemyParty->count; i++) {
                if (state->enemyParty->slots[i] != NULL) resetStatStages(state->enemyParty->slots[i]);
            }
        }
    }
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
