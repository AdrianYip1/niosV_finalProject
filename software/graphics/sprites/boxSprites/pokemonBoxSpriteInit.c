#include "pokemonBoxSpriteInit.h"

#include "../pokemon/pokemonSpriteInit.h"
#include "charizardBoxSprite.h"
#include "charmanderBoxSprite.h"
#include "charmeleonBoxSprite.h"

static void setEmpty(StaticSprite* out) {
    if (!out) return;
    out->pixels = 0;
    out->width = 0;
    out->height = 0;
}

bool initPokemonBoxSprite(StaticSprite* out, int pokemon_id, int x, int y) {
    if (!out) return false;
    out->x = x;
    out->y = y;
    return setPokemonBoxSpriteId(out, pokemon_id);
}

bool setPokemonBoxSpriteId(StaticSprite* sprite, int pokemon_id) {
    if (!sprite) return false;

    switch (pokemon_id) {
        case POKEMON_ID_CHARMANDER:
            sprite->pixels = charmanderBox;
            sprite->width = CHARMANDER_BOX_WIDTH;
            sprite->height = CHARMANDER_BOX_HEIGHT;
            return true;
        case POKEMON_ID_CHARMELEON:
            sprite->pixels = charmeleonBox;
            sprite->width = CHARMELEON_BOX_WIDTH;
            sprite->height = CHARMELEON_BOX_HEIGHT;
            return true;
        case POKEMON_ID_CHARIZARD:
            sprite->pixels = charizardBox;
            sprite->width = CHARIZARD_BOX_WIDTH;
            sprite->height = CHARIZARD_BOX_HEIGHT;
            return true;
        default:
            setEmpty(sprite);
            return false;
    }
}

