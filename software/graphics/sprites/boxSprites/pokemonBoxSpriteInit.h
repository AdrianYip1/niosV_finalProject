#pragma once

#include <stdbool.h>

#include "../staticSprite.h"

// Initializes a 68x56 "party box" sprite (used in battle party UI).
// Returns false if pokemon_id is unknown (sprite is set to empty).
bool initPokemonBoxSprite(StaticSprite* out, int pokemon_id, int x, int y);

// Updates an existing sprite's pixels/size for a new pokemon id while preserving x/y.
// Returns false if pokemon_id is unknown (sprite is set to empty).
bool setPokemonBoxSpriteId(StaticSprite* sprite, int pokemon_id);

