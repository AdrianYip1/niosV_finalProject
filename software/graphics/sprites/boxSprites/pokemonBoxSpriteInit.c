#include "pokemonBoxSpriteInit.h"

#include "../pokemon/pokemonSpriteInit.h"
#include "charizardBoxSprite.h"
#include "charmanderBoxSprite.h"
#include "charmeleonBoxSprite.h"
#include "garchompBoxSprite.h"
#include "lucarioBoxSprite.h"
#include "miloticBoxSprite.h"
#include "rayquazaBoxSprite.h"
#include "roseradeBoxSprite.h"
#include "spiritombBoxSprite.h"
#include "togekissBoxSprite.h"

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
        case POKEMON_ID_RAYQUAZA:
            sprite->pixels = rayquazaBox;
            sprite->width = RAYQUAZA_BOX_WIDTH;
            sprite->height = RAYQUAZA_BOX_HEIGHT;
            return true;
        case POKEMON_ID_GARCHOMP:
            sprite->pixels = garchompBox;
            sprite->width = GARCHOMP_BOX_WIDTH;
            sprite->height = GARCHOMP_BOX_HEIGHT;
            return true;
        case POKEMON_ID_LUCARIO:
            sprite->pixels = lucarioBox;
            sprite->width = LUCARIO_BOX_WIDTH;
            sprite->height = LUCARIO_BOX_HEIGHT;
            return true;
        case POKEMON_ID_MILOTIC:
            sprite->pixels = miloticBox;
            sprite->width = MILOTIC_BOX_WIDTH;
            sprite->height = MILOTIC_BOX_HEIGHT;
            return true;
        case POKEMON_ID_ROSERADE:
            sprite->pixels = roseradeBox;
            sprite->width = ROSERADE_BOX_WIDTH;
            sprite->height = ROSERADE_BOX_HEIGHT;
            return true;
        case POKEMON_ID_SPIRITOMB:
            sprite->pixels = spiritombBox;
            sprite->width = SPIRITOMB_BOX_WIDTH;
            sprite->height = SPIRITOMB_BOX_HEIGHT;
            return true;
        case POKEMON_ID_TOGEKISS:
            sprite->pixels = togekissBox;
            sprite->width = TOGEKISS_BOX_WIDTH;
            sprite->height = TOGEKISS_BOX_HEIGHT;
            return true;
        default:
            setEmpty(sprite);
            return false;
    }
}
