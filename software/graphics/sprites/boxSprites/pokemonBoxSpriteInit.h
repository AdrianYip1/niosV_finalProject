#pragma once

#include <stdbool.h>

#include "../staticSprite.h"


bool initPokemonBoxSprite(StaticSprite* out, int pokemon_id, int x, int y);

bool setPokemonBoxSpriteId(StaticSprite* sprite, int pokemon_id);

