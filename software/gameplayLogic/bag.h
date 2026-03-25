#pragma once
#include <stdbool.h>

typedef enum {
    ITEM_NONE = 0,

    // Healing
    ITEM_POTION,
    ITEM_SUPER_POTION,
    ITEM_HYPER_POTION,
    ITEM_FULL_RESTORE,

    // Pokeballs
    ITEM_POKEBALL,
    ITEM_GREAT_BALL,
    ITEM_ULTRA_BALL,
    ITEM_PREMIER_BALL,
    ITEM_MASTER_BALL,

    ITEM_ID_COUNT
} ItemId;

typedef struct {
    int counts[ITEM_ID_COUNT];
    ItemId lastUsedItem;
} Bag;

void bagInit(Bag *bag);
int bagCount(const Bag *bag, ItemId item);
bool bagAdd(Bag *bag, ItemId item, int amount);
bool bagRemove(Bag *bag, ItemId item, int amount);

static inline bool itemIsHealing(ItemId item) {
    return item == ITEM_POTION || item == ITEM_SUPER_POTION || item == ITEM_HYPER_POTION || item == ITEM_FULL_RESTORE;
}

static inline bool itemIsBall(ItemId item) {
    return item == ITEM_POKEBALL || item == ITEM_GREAT_BALL || item == ITEM_ULTRA_BALL || item == ITEM_PREMIER_BALL || item == ITEM_MASTER_BALL;
}

const char *itemName(ItemId item);


int bagHpItemCount(void);
int bagBallItemCount(void);
ItemId bagHpItemAt(int index);
ItemId bagBallItemAt(int index);

// Visible items: filters out items with 0 count.
int bagHpVisibleCount(const Bag *bag);
int bagBallVisibleCount(const Bag *bag);
ItemId bagHpVisibleAt(const Bag *bag, int visibleIndex);
ItemId bagBallVisibleAt(const Bag *bag, int visibleIndex);
