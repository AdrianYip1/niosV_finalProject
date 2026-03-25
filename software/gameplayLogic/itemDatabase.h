#pragma once

#include "bag.h"

typedef struct {
    ItemId id;
    const char *name;
    const char *description;
} ItemData;

const ItemData *getItemData(ItemId id);
const char *getItemDescription(ItemId id);
