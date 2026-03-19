#include "pokemonObject.h"

typedef struct{
    int monster_ID;

    int frontFrame_ID;
    int backFrame_ID;

    int* baseStats[5]; //{hp, attack, sp attack, def, sp def, speed} array from a monster template
} pokemonID;

typedef struct{
    pokemonID id;
    int level;
    int exp;
    bool alive;

    int* scaledStatsWithLevel[5]; //{hp, attack, sp attack, def, sp def, speed} array from a monster template

    bool inBattle;
    char* attacks[3]; // 4 attacks per pokemon
} pokemonInBattle;

void scaleStatsWithLevel(pokemonInBattle pokemon) {
    for (int i = 0; i < 5; i++) {
        pokemon.scaledStatsWithLevel[i] = (((int)pokemon.id.baseStats[i] * 2) / 100 * pokemon.level);
    }
}

void gainExp(pokemonInBattle pokemon, pokemonInBattle defeatedPokemon) {
    int expGained = experienceGained(pokemon.level, defeatedPokemon.level);

    if (expGained == 0) return;

    while (experienceGained > 0) {
        pokemon.exp += expGained;
        if (pokemon.exp > expRequiredAtLevel(pokemon.level)) {
            levelUp(pokemon);
        }
    }
}

void dealDamage(pokemonInBattle attacker, pokemonInBattle target, int baseDamage, int damageType) { //0 is physical, 1 is special, 2 is status
    int modifierID = (damageType == 1) ? 1 : 2; //physical uses attack, special uses sp attack
    int dealtDamage = ((baseDamage * attacker.level + 10) / 250) * (int)attacker.scaledStatsWithLevel[modifierID];

    takeDamage(target, dealDamage, damageType);

    return;
}

void takeDamage(pokemonInBattle target, int incomingDamage, int damageType) {
    srand(time(NULL));

    double min = 0.85;
    double max = 1.0;
    double random_num;

    // Generate a random double between 0.0 and 1.0 using the standard method
    double scale = (double)rand() / (double)RAND_MAX;

    random_num = min + scale * (max - min);

    //calculate damage with basestats
    int modifierID = (damageType == 1) ? 3 : 4; // physicaluses def, "Special" uses spdef
    int actualDamage = (incomingDamage / (int)target.scaledStatsWithLevel[modifierID]) * random_num; //damage calculations

    if (actualDamage > target.scaledStatsWithLevel[0]) {
        target.alive = false; //kill pokemon 
    }
    else {
        target.scaledStatsWithLevel[0] -= actualDamage;
    }

    return;
}

void levelUp(pokemonInBattle pokemon) {
    int currentExp = pokemon.exp;
    int neededExp = expRequiredAtLevel(pokemon.level);

    currentExp -= neededExp;
    pokemon.level++;
}

int expRequiredAtLevel(int level) {
    return pow((0.8 * level), 3);
}

int experienceGained(int levelSelf, int levelOpponent) {
    return ((BASE_EXP_GAINED * levelOpponent) / 7) * EXP_MULTIPLIER;
}

bool isAlive(pokemonInBattle pokemon) {
    return pokemon.alive;
}

int getHp(pokemonInBattle pokemon) {
    return pokemon.id.baseStats[0];
}
int getAttack(pokemonInBattle pokemon){
    return pokemon.id.baseStats[1];
}

int getSpAttack(pokemonInBattle pokemon){
    return pokemon.id.baseStats[2];
}

int getDef(pokemonInBattle pokemon) {
    return pokemon.id.baseStats[3];
}

int getSpDef(pokemonInBattle pokemon) {
    return pokemon.id.baseStats[4];
}

int getSpd(pokemonInBattle pokemon) {
    return pokemon.id.baseStats[5];
}

