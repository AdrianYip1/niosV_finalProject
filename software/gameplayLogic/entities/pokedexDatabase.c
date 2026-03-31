#include "pokedexDatabase.h"

static const PokedexEntry POKEDEX_DB[] = {
    {
        .pokemon_id = 1,
        .category = "Lizard Pokemon",
        .height_cm = 60,
        .width_kg = 9,
        .entry = "It has a preference for hot things.\nWhen it rains, steam is said to spout\nfrom the tip of its tail.",
    },
    {
        .pokemon_id = 2,
        .category = "Flame Pokemon",
        .height_cm = 110,
        .width_kg = 19,
        .entry = "It lashes about with its tail to knock\ndown its foes.\nThe flame burns hotter as it fights.",
    },
    {
        .pokemon_id = 3,
        .category = "Flame Pokemon",
        .height_cm = 170,
        .width_kg = 91,
        .entry = "It spits fire that is hot enough to melt\nboulders.\nIt may cause forest fires by blowing flames.",
    },
    {
        .pokemon_id = 4,
        .category = "Sky High Pokemon",
        .height_cm = 700,
        .width_kg = 207,
        .entry = "It lives in the ozone layer far above.\nIt flies without ever landing and is said\nto calm storms with its presence.",
    },
    {
        .pokemon_id = 5,
        .category = "Mach Pokemon",
        .height_cm = 190,
        .width_kg = 95,
        .entry = "When it folds up its body and extends its\nwings, it looks like a jet plane.\nIt can fly at supersonic speed.",
    },
    {
        .pokemon_id = 6,
        .category = "Aura Pokemon",
        .height_cm = 120,
        .width_kg = 54,
        .entry = "It can sense the aura of all things.\nIt understands human speech and can read\nthe thoughts of others.",
    },
    {
        .pokemon_id = 7,
        .category = "Tender Pokemon",
        .height_cm = 620,
        .width_kg = 162,
        .entry = "It is said to live at the bottom of large\nlakes.\nIts beauty has inspired many artists.",
    },
    {
        .pokemon_id = 8,
        .category = "Bouquet Pokemon",
        .height_cm = 90,
        .width_kg = 15,
        .entry = "Its sweet aroma attracts prey.\nWith the toxins in its right hand, it can\nwilt even the strongest foes.",
    },
    {
        .pokemon_id = 9,
        .category = "Forbidden Pokemon",
        .height_cm = 100,
        .width_kg = 108,
        .entry = "A bound spirit of many souls.\nIt is said to appear in cracks in odd\nkeystones and whisper from the shadows.",
    },
    {
        .pokemon_id = 10,
        .category = "Jubilee Pokemon",
        .height_cm = 150,
        .width_kg = 38,
        .entry = "It will never appear where there is strife.\nIts sightings have become an emblem of\nhappiness and peace.",
    },
};

const PokedexEntry *getPokedexEntry(int pokemon_id) {
    for (unsigned int i = 0; i < (sizeof(POKEDEX_DB) / sizeof(POKEDEX_DB[0])); i++) {
        if (POKEDEX_DB[i].pokemon_id == pokemon_id) return &POKEDEX_DB[i];
    }
    return 0;
}
