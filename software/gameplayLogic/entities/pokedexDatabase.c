#include "pokedexDatabase.h"
#include "../../graphics/sprites/pokemon/pokemonSpriteInit.h"

static const PokedexEntry POKEDEX_DB[] = {
    {
        .pokemon_id = POKEMON_ID_CHARMANDER,
        .category = "Lizard Pokemon",
        .height_cm = 60,
        .width_kg = 9,
        .entry = "It has a preference for hot things.\nWhen it rains, steam is said to spout\nfrom the tip of its tail.",
    },
    {
        .pokemon_id = POKEMON_ID_CHARMELEON,
        .category = "Flame Pokemon",
        .height_cm = 110,
        .width_kg = 19,
        .entry = "It lashes about with its tail to knock\ndown its foes.\nThe flame burns hotter as it fights.",
    },
    {
        .pokemon_id = POKEMON_ID_CHARIZARD,
        .category = "Flame Pokemon",
        .height_cm = 170,
        .width_kg = 91,
        .entry = "It spits fire that is hot enough to melt\nboulders.\nIt may cause forest fires by blowing flames.",
    },
    {
        .pokemon_id = POKEMON_ID_RAYQUAZA,
        .category = "Sky High Pokemon",
        .height_cm = 700,
        .width_kg = 207,
        .entry = "It lives in the ozone layer far above.\nIt flies without ever landing and is said\nto calm storms with its presence.",
    },
    {
        .pokemon_id = POKEMON_ID_GARCHOMP,
        .category = "Mach Pokemon",
        .height_cm = 190,
        .width_kg = 95,
        .entry = "When it folds up its body and extends its\nwings, it looks like a jet plane.\nIt can fly at supersonic speed.",
    },
    {
        .pokemon_id = POKEMON_ID_LUCARIO,
        .category = "Aura Pokemon",
        .height_cm = 120,
        .width_kg = 54,
        .entry = "It can sense the aura of all things.\nIt understands human speech and can read\nthe thoughts of others.",
    },
    {
        .pokemon_id = POKEMON_ID_MILOTIC,
        .category = "Tender Pokemon",
        .height_cm = 620,
        .width_kg = 162,
        .entry = "It is said to live at the bottom of large\nlakes.\nIts beauty has inspired many artists.",
    },
    {
        .pokemon_id = POKEMON_ID_ROSERADE,
        .category = "Bouquet Pokemon",
        .height_cm = 90,
        .width_kg = 15,
        .entry = "Its sweet aroma attracts prey.\nWith the toxins in its right hand, it can\nwilt even the strongest foes.",
    },
    {
        .pokemon_id = POKEMON_ID_SPIRITOMB,
        .category = "Forbidden Pokemon",
        .height_cm = 100,
        .width_kg = 108,
        .entry = "A bound spirit of many souls.\nIt is said to appear in cracks in odd\nkeystones and whisper from the shadows.",
    },
    {
        .pokemon_id = POKEMON_ID_TOGEKISS,
        .category = "Jubilee Pokemon",
        .height_cm = 150,
        .width_kg = 38,
        .entry = "It will never appear where there is strife.\nIts sightings have become an emblem of\nhappiness and peace.",
    },
    {
        .pokemon_id = POKEMON_ID_BUDEW,
        .category = "Bud Pokemon",
        .height_cm = 20,
        .width_kg = 1,
        .entry = "Its sensitive bud is said to contain\na healing aroma.\nIt often hides among flowers.",
    },
    {
        .pokemon_id = POKEMON_ID_FEEBAS,
        .category = "Fish Pokemon",
        .height_cm = 60,
        .width_kg = 7,
        .entry = "It is famous for its shabby appearance.\nIt swims by weakly flopping its fins.",
    },
    {
        .pokemon_id = POKEMON_ID_GIBLE,
        .category = "Land Shark Pokemon",
        .height_cm = 70,
        .width_kg = 20,
        .entry = "It nests in small caves.\nWhen it senses prey, it lunges with\na sudden burst of speed.",
    },
    {
        .pokemon_id = POKEMON_ID_GABITE,
        .category = "Cave Pokemon",
        .height_cm = 140,
        .width_kg = 56,
        .entry = "It loves shiny objects.\nIt can dig at high speed to chase prey\nthrough narrow tunnels.",
    },
    {
        .pokemon_id = POKEMON_ID_RIOLU,
        .category = "Emanation Pokemon",
        .height_cm = 70,
        .width_kg = 20,
        .entry = "It uses its aura to sense danger.\nWhen it concentrates, it can unleash\npowerful strikes.",
    },
    {
        .pokemon_id = POKEMON_ID_TOGEPI,
        .category = "Spike Ball Pokemon",
        .height_cm = 30,
        .width_kg = 2,
        .entry = "It is said to bring good fortune.\nIt shares happiness with those who treat\nit kindly.",
    },
    {
        .pokemon_id = POKEMON_ID_TOGETIC,
        .category = "Happiness Pokemon",
        .height_cm = 60,
        .width_kg = 3,
        .entry = "It is a symbol of good luck.\nIt is said to appear only before\nkindhearted people.",
    },
};

const PokedexEntry *getPokedexEntry(int pokemon_id) {
    for (unsigned int i = 0; i < (sizeof(POKEDEX_DB) / sizeof(POKEDEX_DB[0])); i++) {
        if (POKEDEX_DB[i].pokemon_id == pokemon_id) return &POKEDEX_DB[i];
    }
    return 0;
}
