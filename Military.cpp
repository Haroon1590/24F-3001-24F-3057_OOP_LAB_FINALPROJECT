#include "Stronghold.h"

Military::Military() : infantry(20), archers(10), cavalry(5), morale(70) {
}

Military::Military(int infantry, int archers, int cavalry)
    : infantry(infantry), archers(archers), cavalry(cavalry), morale(70) {
}

int Military::getInfantry() const {
    return infantry;
}

int Military::getArchers() const {
    return archers;
}

int Military::getCavalry() const {
    return cavalry;
}

int Military::getTotalForces() const {
    return infantry + archers + cavalry;
}

int Military::getMorale() const {
    return morale;
}

void Military::setInfantry(int infantry) {
    this->infantry = infantry;
    if (this->infantry < 0) this->infantry = 0;
}

void Military::setArchers(int archers) {
    this->archers = archers;
    if (this->archers < 0) this->archers = 0;
}

void Military::setCavalry(int cavalry) {
    this->cavalry = cavalry;
    if (this->cavalry < 0) this->cavalry = 0;
}

void Military::setMorale(int morale) {
    this->morale = morale;
    if (this->morale > 100) this->morale = 100;
    if (this->morale < 0) this->morale = 0;
}

bool Military::recruitSoldiers(Population& population, Resource& resources, int count, int type) {
    // Check if enough peasants are available
    if (population.getPeasants() < count) {
        std::cout << "Not enough peasants to recruit " << count << " soldiers." << std::endl;
        return false;
    }

    // Check resource requirements
    bool hasFood = resources.getFood() >= count * 2;
    bool hasWeaponMaterial = false;

    switch (type) {
    case 1: // Infantry
        hasWeaponMaterial = resources.getIron() >= count * 1 && resources.getWood() >= count * 1;
        break;
    case 2: // Archers
        hasWeaponMaterial = resources.getWood() >= count * 2;
        break;
    case 3: // Cavalry
        hasWeaponMaterial = resources.getIron() >= count * 2 && resources.getWood() >= count * 1;
        break;
    default:
        std::cout << "Invalid soldier type." << std::endl;
        return false;
    }

    if (!hasFood || !hasWeaponMaterial) {
        std::cout << "Not enough resources to recruit " << count << " soldiers." << std::endl;
        return false;
    }

    // Recruit soldiers
    switch (type) {
    case 1: // Infantry
        infantry += count;
        resources.consumeFood(count * 2);
        resources.consumeIron(count * 1);
        resources.consumeWood(count * 1);
        break;
    case 2: // Archers
        archers += count;
        resources.consumeFood(count * 2);
        resources.consumeWood(count * 2);
        break;
    case 3: // Cavalry
        cavalry += count;
        resources.consumeFood(count * 2);
        resources.consumeIron(count * 2);
        resources.consumeWood(count * 1);
        break;
    }

    // Remove peasants that became soldiers
    population.setPeasants(population.getPeasants() - count);

    std::cout << "Successfully recruited " << count << " ";
    switch (type) {
    case 1: std::cout << "infantry"; break;
    case 2: std::cout << "archers"; break;
    case 3: std::cout << "cavalry"; break;
    }
    std::cout << " soldiers." << std::endl;

    return true;
}

void Military::trainTroops(Resource& resources) {
    int totalForces = getTotalForces();
    if (totalForces == 0) {
        std::cout << "No troops to train." << std::endl;
        return;
    }

    // Resource requirements for training
    int foodNeeded = totalForces * 1;

    if (resources.getFood() >= foodNeeded) {
        resources.consumeFood(foodNeeded);
        morale += 5;
        if (morale > 100) morale = 100;

        std::cout << "Troops trained successfully. Morale increased to " << morale << "." << std::endl;
    }
    else {
        std::cout << "Not enough food to train troops." << std::endl;
    }
}

void Military::updateMorale(const Economy& economy, const Population& population) {
    // Morale factors:
    // 1. Population happiness
    // 2. Whether military is paid
    // 3. Military size relative to population

    int previousMorale = morale;

    // Population happiness influences military morale
    if (population.getHappiness() > 70) {
        morale += 2;
    }
    else if (population.getHappiness() < 30) {
        morale -= 3;
    }

    // If treasury is low, morale suffers (assuming military might not get paid)
    if (economy.getTreasury() < getTotalForces() * 3) {
        morale -= 5;
    }

    // Keep morale in bounds
    if (morale > 100) morale = 100;
    if (morale < 0) morale = 0;

    if (morale != previousMorale) {
        std::cout << "Military morale " << (morale > previousMorale ? "increased" : "decreased")
            << " to " << morale << "." << std::endl;
    }
}

int Military::calculateMilitaryStrength() const {
    // Calculate overall military strength based on units and morale
    int infantryStrength = infantry * 1;
    int archerStrength = archers * 2;
    int cavalryStrength = cavalry * 3;

    int baseStrength = infantryStrength + archerStrength + cavalryStrength;

    // Morale modifier (ranges from 0.5 to 1.5)
    double moraleModifier = 0.5 + (static_cast<double>(morale) / 100.0);

    return static_cast<int>(baseStrength * moraleModifier);
}

void Military::displayMilitary() const {
    std::cout << "\n===== MILITARY =====\n";
    std::cout << "Infantry: " << infantry << std::endl;
    std::cout << "Archers: " << archers << std::endl;
    std::cout << "Cavalry: " << cavalry << std::endl;
    std::cout << "Total Forces: " << getTotalForces() << std::endl;
    std::cout << "Military Strength: " << calculateMilitaryStrength() << std::endl;
    std::cout << "Morale: " << morale << "/100" << std::endl;
    std::cout << "===================\n";
}