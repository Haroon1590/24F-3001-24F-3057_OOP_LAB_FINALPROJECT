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
    // Resource requirements for different unit types
    int foodNeeded = count * 2;
    int woodNeeded = count * 1;
    int ironNeeded = count * 1;

    // Additional requirements based on unit type
    switch (type) {
    case 1: // Infantry
        ironNeeded += count * 1;
        break;
    case 2: // Archers
        woodNeeded += count * 2;
        break;
    case 3: // Cavalry
        foodNeeded += count * 3;
        ironNeeded += count * 2;
        break;
    }

    // Check if we have enough resources
    if (resources.getFood() < foodNeeded ||
        resources.getWood() < woodNeeded ||
        resources.getIron() < ironNeeded) {
        cout << "Not enough resources to recruit soldiers." << endl;
        return false;
    }

    // Check if we have enough population to recruit from
    int availablePopulation = population.getPeasants() / 2; // Only half of peasants can be recruited
    if (count > availablePopulation) {
        cout << "Not enough available population to recruit " << count << " soldiers." << endl;
        return false;
    }

    // Consume resources
    resources.consumeFood(foodNeeded);
    resources.consumeWood(woodNeeded);
    resources.consumeIron(ironNeeded);

    // Update population
    population.setPeasants(population.getPeasants() - count);

    // Add soldiers based on type
    switch (type) {
    case 1:
        infantry += count;
        break;
    case 2:
        archers += count;
        break;
    case 3:
        cavalry += count;
        break;
    }

    // Update morale based on recruitment
    morale += count / 5; // Small morale boost for new recruits
    if (morale > 100) morale = 100;

    cout << "Successfully recruited " << count << " ";
    switch (type) {
    case 1: cout << "infantry"; break;
    case 2: cout << "archers"; break;
    case 3: cout << "cavalry"; break;
    }
    cout << " soldiers." << endl;

    return true;
}

void Military::trainTroops(Resource& resources) {
    int totalForces = getTotalForces();
    if (totalForces == 0) {
        cout << "No troops to train." << endl;
        return;
    }

    // Resource requirements for training
    int foodNeeded = totalForces * 2;
    int woodNeeded = totalForces / 2;
    int ironNeeded = totalForces / 2;

    if (resources.getFood() >= foodNeeded &&
        resources.getWood() >= woodNeeded &&
        resources.getIron() >= ironNeeded) {
        resources.consumeFood(foodNeeded);
        resources.consumeWood(woodNeeded);
        resources.consumeIron(ironNeeded);
        
        // Training effectiveness based on current morale
        int moraleBonus = morale / 20; // 0-5 bonus based on morale
        morale += 5 + moraleBonus;
        if (morale > 100) morale = 100;

        cout << "Troops trained successfully. Morale increased to " << morale << "." << endl;
        cout << "Resources consumed: " << foodNeeded << " food, "
                  << woodNeeded << " wood, " << ironNeeded << " iron." << endl;
    }
    else {
        cout << "Not enough resources to train troops." << endl;
    }
}

void Military::updateMorale(const Economy& economy, const Population& population) {
    int previousMorale = morale;

    // Base morale factors
    int happinessFactor = (population.getHappiness() - 50) / 10; // -5 to +5
    int treasuryFactor = (economy.getTreasury() > 2000) ? 2 : -2;
    int populationFactor = (getTotalForces() < population.getTotalPopulation() / 10) ? 1 : -1;

    // Calculate new morale
    morale += happinessFactor + treasuryFactor + populationFactor;

    // Keep morale in bounds
    if (morale > 100) morale = 100;
    if (morale < 0) morale = 0;

    if (morale != previousMorale) {
        cout << "Military morale " << (morale > previousMorale ? "increased" : "decreased")
            << " to " << morale << "." << endl;
        if (happinessFactor != 0) {
            cout << "Population happiness " << (happinessFactor > 0 ? "boosted" : "reduced")
                << " morale by " << abs(happinessFactor) << "." << endl;
        }
        if (treasuryFactor != 0) {
            cout << "Treasury status " << (treasuryFactor > 0 ? "boosted" : "reduced")
                << " morale by " << abs(treasuryFactor) << "." << endl;
        }
        if (populationFactor != 0) {
            cout << "Military size relative to population " << (populationFactor > 0 ? "boosted" : "reduced")
                << " morale by " << abs(populationFactor) << "." << endl;
        }
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
    cout << "\n===== MILITARY =====\n";
    cout << "Infantry: " << infantry << endl;
    cout << "Archers: " << archers << endl;
    cout << "Cavalry: " << cavalry << endl;
    cout << "Total Forces: " << getTotalForces() << endl;
    cout << "Military Strength: " << calculateMilitaryStrength() << endl;
    cout << "Morale: " << morale << "/100" << endl;
    cout << "===================\n";
}