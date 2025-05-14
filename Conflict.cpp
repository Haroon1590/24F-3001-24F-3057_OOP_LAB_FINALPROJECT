#include "Stronghold.h"

Conflict::Conflict() : warCount(0), capacity(10) {
    wars = new War * [capacity];
    for (int i = 0; i < capacity; i++) wars[i] = nullptr;
}

Conflict::~Conflict() {
    for (int i = 0; i < warCount; i++) delete wars[i];
    delete[] wars;
}

void Conflict::declareWar(const Kingdom& attacker, const Kingdom& defender) {
    if (attacker.getName() == defender.getName()) throw GameException("Cannot declare war on self.");
    if (warCount >= capacity) {
        int newCapacity = capacity * 2;
        War** newWars = new War * [newCapacity];
        for (int i = 0; i < newCapacity; i++) newWars[i] = nullptr;
        for (int i = 0; i < warCount; i++) newWars[i] = wars[i];
        delete[] wars;
        wars = newWars;
        capacity = newCapacity;
    }
    wars[warCount++] = new War(attacker.getName(), defender.getName());
    std::cout << attacker.getName() << " has declared war on " << defender.getName() << "!\n";
}

void Conflict::resolveBattle(Kingdom& attacker, Kingdom& defender) {
    int attackerStrength = attacker.getMilitary().calculateMilitaryStrength();
    int defenderStrength = defender.getMilitary().calculateMilitaryStrength();
    int attackerRoll = rand() % 100 + attackerStrength;
    int defenderRoll = rand() % 100 + defenderStrength;

    if (attackerRoll > defenderRoll) {
        int losses = defender.getMilitary().getTotalForces() / 4;
        defender.getMilitary().setInfantry(defender.getMilitary().getInfantry() - losses / 3);
        defender.getMilitary().setArchers(defender.getMilitary().getArchers() - losses / 3);
        defender.getMilitary().setCavalry(defender.getMilitary().getCavalry() - losses / 3);
        defender.getPopulation().decreaseHappiness(15);
        attacker.getPopulation().increaseHappiness(10);
        std::cout << attacker.getName() << " won the battle against " << defender.getName() << "!\n";
    }
    else {
        int losses = attacker.getMilitary().getTotalForces() / 4;
        attacker.getMilitary().setInfantry(attacker.getMilitary().getInfantry() - losses / 3);
        attacker.getMilitary().setArchers(attacker.getMilitary().getArchers() - losses / 3);
        attacker.getMilitary().setCavalry(attacker.getMilitary().getCavalry() - losses / 3);
        attacker.getPopulation().decreaseHappiness(15);
        defender.getPopulation().increaseHappiness(10);
        std::cout << defender.getName() << " repelled " << attacker.getName() << "'s attack!\n";
    }
}

void Conflict::betrayAlly(Kingdom& betrayer, Kingdom& betrayed) {
    betrayer.getPopulation().decreaseHappiness(20);
    betrayer.getMilitary().setMorale(betrayer.getMilitary().getMorale() - 20);
    betrayed.getPopulation().decreaseHappiness(10);
    std::cout << betrayer.getName() << " betrayed " << betrayed.getName() << "! Significant penalties applied.\n";
}

void Conflict::saveConflictLog(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) throw GameException("Could not open conflict log file.");
    for (int i = 0; i < warCount; i++) {
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &wars[i]->timestamp);
        file << "[" << timeStr << "] War declared: " << wars[i]->attacker << " vs " << wars[i]->defender << std::endl;
    }
    file.close();
}