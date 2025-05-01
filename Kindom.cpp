#include "Stronghold.h"

Kingdom::Kingdom() : name("DefaultKingdom"), turnNumber(1) {
}

Kingdom::Kingdom(const std::string& name) : name(name), turnNumber(1) {
}

std::string Kingdom::getName() const {
    return name;
}

Resource& Kingdom::getResources() {
    return resources;
}

Population& Kingdom::getPopulation() {
    return population;
}

Economy& Kingdom::getEconomy() {
    return economy;
}

Military& Kingdom::getMilitary() {
    return military;
}

Leadership& Kingdom::getLeadership() {
    return leadership;
}

Banking& Kingdom::getBanking() {
    return banking;
}

void Kingdom::setName(const std::string& name) {
    this->name = name;
}

void Kingdom::simulateTurn() {
    std::cout << "\n==========================================================\n";
    std::cout << "                    TURN " << turnNumber << ": " << name << "\n";
    std::cout << "==========================================================\n";

    // 1. Leadership makes decisions
    leadership.makeDecision(economy, military);

    // 2. Gather resources
    gatherResources();

    // 3. Update population
    population.updatePopulation(resources, economy.getTaxRate());

    // 4. Check for population revolt
    if (population.isRevolting()) {
        population.handleRevolt();
    }

    // 5. Collect taxes (happiness effects handled in Economy class)
    economy.collectTaxes(population);

    // 6. Pay military
    economy.payMilitary(military);
    // Military payment affects happiness
    if (economy.hasSufficientFunds(military.getTotalForces() * 3)) {
        population.updateHappinessFromAction("military_training", 2);
    }
    else {
        population.updateHappinessFromAction("military_training", -3);
    }

    // 7. Calculate loan interest
    banking.calculateInterest();

    // 8. Update military morale
    military.updateMorale(economy, population);

    // 9. Update leader popularity
    leadership.updatePopularity(economy, population);

    // 10. Check for coup
    leadership.handleCoup(population, military);

    // 11. Adjust inflation (happiness effects handled in Economy class)
    economy.adjustInflation(population);

    // 12. Generate random event
    eventHandler.generateRandomEvent(*this);

    // Increment turn number
    turnNumber++;

    std::cout << "\nTurn " << (turnNumber - 1) << " completed.\n";
}

void Kingdom::gatherResources() {
    // Basic resource gathering tied to population sizes
    int foodGathered = population.getPeasants() / 2;
    int woodGathered = population.getPeasants() / 5;
    int stoneGathered = population.getPeasants() / 10;
    int ironGathered = population.getPeasants() / 20;

    // Add gathered resources
    resources.addFood(foodGathered);
    resources.addWood(woodGathered);
    resources.addStone(stoneGathered);
    resources.addIron(ironGathered);

    std::cout << "Resources gathered: " << foodGathered << " food, "
        << woodGathered << " wood, " << stoneGathered << " stone, "
        << ironGathered << " iron." << std::endl;

    // Consume food based on population
    int foodConsumed = population.getTotalPopulation() / 2;
    if (resources.consumeFood(foodConsumed)) {
        std::cout << "Population consumed " << foodConsumed << " food." << std::endl;
        // Food consumption affects happiness
        population.updateHappinessFromAction("food_distribution", 2);
    }
    else {
        std::cout << "WARNING: Not enough food to feed population!" << std::endl;
        // Food shortage affects happiness negatively
        population.updateHappinessFromAction("food_distribution", -5);
    }
}

void Kingdom::displayKingdomStatus() const {
    std::cout << "\n==========================================================\n";
    std::cout << "                 " << name << " - Turn " << turnNumber << "\n";
    std::cout << "==========================================================\n";

    // Display all subsystems
    resources.displayResources();
    population.displayPopulation();
    economy.displayEconomy();
    military.displayMilitary();
    leadership.displayLeadership();
    banking.displayBanking();
}

bool Kingdom::saveGame(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file for saving." << std::endl;
        return false;
    }

    // Save kingdom basic info
    file << name << std::endl;
    file << turnNumber << std::endl;

    // Save resources
    file << resources.getFood() << " "
        << resources.getWood() << " "
        << resources.getStone() << " "
        << resources.getIron() << std::endl;

    // Save population
    file << population.getPeasants() << " "
        << population.getMerchants() << " "
        << population.getNobles() << " "
        << population.getHappiness() << std::endl;

    // Save economy
    file << economy.getTreasury() << " "
        << economy.getTaxRate() << " "
        << economy.getInflationRate() << std::endl;

    // Save military
    file << military.getInfantry() << " "
        << military.getArchers() << " "
        << military.getCavalry() << " "
        << military.getMorale() << std::endl;

    // Save leadership
    file << leadership.getName() << std::endl;
    file << leadership.getDiplomacy() << " "
        << leadership.getMilitarySkill() << " "
        << leadership.getEconomySkill() << " "
        << leadership.getPopularity() << std::endl;

    // Save banking
    file << banking.getLoanAmount() << " "
        << banking.getInterestRate() << " "
        << banking.getCorruptionLevel() << std::endl;

    file.close();
    std::cout << "Game saved successfully to " << filename << std::endl;
    return true;
}

bool Kingdom::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file for loading." << std::endl;
        return false;
    }

    // Load kingdom basic info
    std::getline(file, name);
    file >> turnNumber;
    file.ignore(); // Ignore newline

    // Load resources
    int food, wood, stone, iron;
    file >> food >> wood >> stone >> iron;
    resources = Resource(food, wood, stone, iron);

    // Load population
    int peasants, merchants, nobles, happiness;
    file >> peasants >> merchants >> nobles >> happiness;
    population = Population(peasants, merchants, nobles);
    population.setHappiness(happiness);

    // Load economy
    int treasury, taxRate, inflationRate;
    file >> treasury >> taxRate >> inflationRate;
    economy = Economy(treasury, taxRate);
    economy.setInflationRate(inflationRate);

    // Load military
    int infantry, archers, cavalry, morale;
    file >> infantry >> archers >> cavalry >> morale;
    military = Military(infantry, archers, cavalry);
    military.setMorale(morale);

    // Load leadership
    std::string leaderName;
    file.ignore(); // Ignore newline
    std::getline(file, leaderName);
    int diplomacy, militarySkill, economySkill, popularity;
    file >> diplomacy >> militarySkill >> economySkill >> popularity;
    leadership = Leadership(leaderName, diplomacy, militarySkill, economySkill);
    leadership.setPopularity(popularity);

    // Load banking
    int loanAmount, interestRate, corruptionLevel;
    file >> loanAmount >> interestRate >> corruptionLevel;
    banking = Banking(loanAmount, interestRate);
    banking.setCorruptionLevel(corruptionLevel);

    file.close();
    std::cout << "Game loaded successfully from " << filename << std::endl;
    return true;
}

void Kingdom::logScore(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file for logging score." << std::endl;
        return;
    }

    // Calculate score based on various metrics
    int populationScore = population.getTotalPopulation() * 2;
    int resourceScore = (resources.getFood() + resources.getWood() + resources.getStone() + resources.getIron()) / 10;
    int economyScore = economy.getTreasury() / 10;
    int militaryScore = military.calculateMilitaryStrength() * 5;
    int happinessScore = population.getHappiness() * 10;

    // Deduct for loans
    int loanPenalty = banking.getLoanAmount() / 10;

    int totalScore = populationScore + resourceScore + economyScore + militaryScore + happinessScore - loanPenalty;

#include <ctime>
    // ...
    time_t now = time(0);
    char dt[26]; // ctime_s needs a buffer of at least 26 characters
    ctime_s(dt, sizeof(dt), &now);
    // Now use dt as your date/time string

    file << dt;
    file << "Kingdom: " << name << std::endl;
    file << "Turns Survived: " << turnNumber << std::endl;
    file << "Population: " << population.getTotalPopulation() << std::endl;
    file << "Treasury: " << economy.getTreasury() << std::endl;
    file << "Military Strength: " << military.calculateMilitaryStrength() << std::endl;
    file << "Resources: " << resources.getFood() << " food, "
        << resources.getWood() << " wood, "
        << resources.getStone() << " stone, "
        << resources.getIron() << " iron" << std::endl;
    file << "Happiness: " << population.getHappiness() << std::endl;
    file << "Loans: " << banking.getLoanAmount() << std::endl;
    file << "TOTAL SCORE: " << totalScore << std::endl;
    file << "=======================================" << std::endl;

    file.close();
    std::cout << "Score logged to " << filename << std::endl;
}