#include "bag.h"
#include "itemDatabase.h"
#include <string.h>

void bagInit(Bag *bag) {
    if (bag == NULL) return;
    memset(bag->counts, 0, sizeof(bag->counts));
    bag->lastUsedItem = ITEM_NONE;
}

int bagCount(const Bag *bag, ItemId item) {
    if (bag == NULL) return 0;
    if (item <= ITEM_NONE || item >= ITEM_ID_COUNT) return 0;
    const int c = bag->counts[item];
    return (c < 0) ? 0 : c;
}

bool bagAdd(Bag *bag, ItemId item, int amount) {
    if (bag == NULL) return false;
    if (item <= ITEM_NONE || item >= ITEM_ID_COUNT) return false;
    if (amount <= 0) return false;
    long next = (long)bag->counts[item] + (long)amount;
    if (next > 999) next = 999;
    bag->counts[item] = (int)next;
    return true;
}

bool bagRemove(Bag *bag, ItemId item, int amount) {
    if (bag == NULL) return false;
    if (item <= ITEM_NONE || item >= ITEM_ID_COUNT) return false;
    if (amount <= 0) return false;
    if (bagCount(bag, item) < amount) return false;
    bag->counts[item] -= amount;
    if (bag->counts[item] < 0) bag->counts[item] = 0;
    bag->lastUsedItem = item;
    return true;
}

const char *itemName(ItemId item) {
    return getItemData(item)->name;
}

int bagHpItemCount(void) { return 7; }
int bagBallItemCount(void) { return 5; }

ItemId bagHpItemAt(int index) {
    static const ItemId items[] = { ITEM_POTION, ITEM_SUPER_POTION, ITEM_HYPER_POTION, ITEM_FULL_RESTORE, ITEM_REVIVE, ITEM_MAX_REVIVE, ITEM_SHINY_STONE };
    if (index < 0 || index >= (int)(sizeof(items) / sizeof(items[0]))) return ITEM_NONE;
    return items[index];
}

ItemId bagBallItemAt(int index) {

    static const ItemId items[] = { ITEM_POKEBALL, ITEM_GREAT_BALL, ITEM_ULTRA_BALL, ITEM_PREMIER_BALL, ITEM_MASTER_BALL };
    if (index < 0 || index >= (int)(sizeof(items) / sizeof(items[0]))) return ITEM_NONE;
    return items[index];
}

static ItemId visibleAt(const Bag *bag, ItemId (*at)(int), int (*countFn)(void), int visibleIndex) {
    if (bag == NULL) return ITEM_NONE;
    if (visibleIndex < 0) return ITEM_NONE;
    const int n = countFn();
    int seen = 0;
    for (int i = 0; i < n; i++) {
        const ItemId item = at(i);
        if (item != ITEM_NONE && bagCount(bag, item) > 0) {
            if (seen == visibleIndex) return item;
            seen++;
        }
    }
    return ITEM_NONE;
}

static int visibleCount(const Bag *bag, ItemId (*at)(int), int (*countFn)(void)) {
    if (bag == NULL) return 0;
    const int n = countFn();
    int c = 0;
    for (int i = 0; i < n; i++) {
        const ItemId item = at(i);
        if (item != ITEM_NONE && bagCount(bag, item) > 0) c++;
    }
    return c;
}

int bagHpVisibleCount(const Bag *bag) { return visibleCount(bag, bagHpItemAt, bagHpItemCount); }
int bagBallVisibleCount(const Bag *bag) { return visibleCount(bag, bagBallItemAt, bagBallItemCount); }
ItemId bagHpVisibleAt(const Bag *bag, int visibleIndex) { return visibleAt(bag, bagHpItemAt, bagHpItemCount, visibleIndex); }
ItemId bagBallVisibleAt(const Bag *bag, int visibleIndex) { return visibleAt(bag, bagBallItemAt, bagBallItemCount, visibleIndex); }
