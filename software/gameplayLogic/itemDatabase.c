#include "itemDatabase.h"
#include <stddef.h>

static const ItemData ITEM_DB[ITEM_ID_COUNT] = {
    [ITEM_NONE] = {ITEM_NONE, "Item", ""},

    // Healing
    [ITEM_POTION] = {ITEM_POTION, "Potion", "Restores 20 HP."},
    [ITEM_SUPER_POTION] = {ITEM_SUPER_POTION, "Super Potion", "Restores 60 HP."},
    [ITEM_HYPER_POTION] = {ITEM_HYPER_POTION, "Hyper Potion", "Restores 120 HP."},
    [ITEM_FULL_RESTORE] = {ITEM_FULL_RESTORE, "Full Restore", "Fully restores HP + cures status conditions."},
    [ITEM_REVIVE] = {ITEM_REVIVE, "Revive", "Revive a fainted Pokemon with half their HP"},
    [ITEM_MAX_REVIVE] = {ITEM_MAX_REVIVE, "Max Revive", "Revive a fainted Pokemon with full HP"},

    // Balls
    [ITEM_POKEBALL] = {ITEM_POKEBALL, "Poke Ball", "Catches wild Pokemon."},
    [ITEM_GREAT_BALL] = {ITEM_GREAT_BALL, "Great Ball", "Catches wild Pokemon with 1.5x the rate of a Poke Ball."},
    [ITEM_ULTRA_BALL] = {ITEM_ULTRA_BALL, "Ultra Ball", "Catches wild Pokemon with 2x the rate of a Poke Ball"},
    [ITEM_PREMIER_BALL] = {ITEM_PREMIER_BALL, "Premier Ball", "Catches wild Pokemon; received after buying 10 Poke Balls."},
    [ITEM_MASTER_BALL] = {ITEM_MASTER_BALL, "Master Ball", "Catches any wild Pokemon without fail."},
};

const ItemData *getItemData(ItemId id) {
    if (id < 0 || id >= ITEM_ID_COUNT) return &ITEM_DB[ITEM_NONE];
    const ItemData *d = &ITEM_DB[id];
    if (d->name == NULL) return &ITEM_DB[ITEM_NONE];
    return d;
}

const char *getItemDescription(ItemId id) {
    const ItemData *d = getItemData(id);
    return (d->description != NULL) ? d->description : "";
}
