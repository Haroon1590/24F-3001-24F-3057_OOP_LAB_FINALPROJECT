#include "Stronghold.h"

Kingdom::Kingdom() : name("DefaultKingdom"), turnNumber(1), mapX(0), mapY(0) {
}

Kingdom::Kingdom(const std::string& name) : name(name), turnNumber(1), mapX(0), mapY(0) {
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

    leadership.makeDecision(economy, military);
    gatherResources();
    population.updatePopulation(resources, economy.getTaxRate());
    if (population.isRevolting()) {
        population.handleRevolt();
    }
    economy.collectTaxes(population);
    economy.payMilitary(military);
    if (economy.hasSufficientFunds(military.getTotalForces() * 3)) {
        population.updateHappinessFromAction("military_training", 2);
    }
    else {
        population.updateHappinessFromAction("military_training", -3);
    }
    banking.calculateInterest();
    military.updateMorale(economy, population);
    leadership.updatePopularity(economy, population);
    leadership.handleCoup(population, military);
    economy.adjustInflation(population);
    eventHandler.generateRandomEvent(*this);
    turnNumber++;
    std::cout << "\nTurn " << (turnNumber - 1) << " completed.\n";
}

void Kingdom::gatherResources() {
    int foodGathered = population.getPeasants() / 2;
    int woodGathered = population.getPeasants() / 5;
    int stoneGathered = population.getPeasants() / 10;
    int ironGathered = population.getPeasants() / 20;

    resources.addFood(foodGathered);
    resources.addWood(woodGathered);
    resources.addStone(stoneGathered);
    resources.addIron(ironGathered);

    std::cout << "Resources gathered: " << foodGathered << " food, "
        << woodGathered << " wood, " << stoneGathered << " stone, "
        << ironGathered << " iron." << std::endl;

    int foodConsumed = population.getTotalPopulation() / 2;
    if (resources.consumeFood(foodConsumed)) {
        std::cout << "Population consumed " << foodConsumed << " food." << std::endl;
        population.updateHappinessFromAction("food_distribution", 2);
    }
    else {
        std::cout << "WARNING: Not enough food to feed population!" << std::endl;
        population.updateHappinessFromAction("food_distribution", -5);
    }
}

void Kingdom::displayKingdomStatus() const {
    std::cout << "\n==========================================================\n";
    std::cout << "                 " << name << " - Turn " << turnNumber << "\n";
    std::cout << "                 Position: (" << mapX << ", " << mapY << ")\n";
    std::cout << "==========================================================\n";

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

    file << name << std::endl;
    file << turnNumber << std::endl;
    file << mapX << " " << mapY << std::endl;
    file << resources.getFood() << " " << resources.getWood() << " " << resources.getStone() << " " << resources.getIron() << std::endl;
    file << population.getPeasants() << " " << population.getMerchants() << " " << population.getNobles() << " " << population.getHappiness() << std::endl;
    file << economy.getTreasury() << " " << economy.getTaxRate() << " " << economy.getInflationRate() << std::endl;
    file << military.getInfantry() << " " << military.getArchers() << " " << military.getCavalry() << " " << military.getMorale() << std::endl;
    file << leadership.getName() << std::endl;
    file << leadership.getDiplomacy() << " " << leadership.getMilitarySkill() << " " << leadership.getEconomySkill() << " " << leadership.getPopularity() << std::endl;
    file << banking.getLoanAmount() << " " << banking.getInterestRate() << " " << banking.getCorruptionLevel() << std::endl;

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

    std::getline(file, name);
    file >> turnNumber >> mapX >> mapY;
    int food, wood, stone, iron;
    file >> food >> wood >> stone >> iron;
    resources = Resource(food, wood, stone, iron);
    int peasants, merchants, nobles, happiness;
    file >> peasants >> merchants >> nobles >> happiness;
    population = Population(peasants, merchants, nobles);
    population.setHappiness(happiness);
    int treasury, taxRate, inflationRate;
    file >> treasury >> taxRate >> inflationRate;
    economy = Economy(treasury, taxRate);
    economy.setInflationRate(inflationRate);
    int infantry, archers, cavalry, morale;
    file >> infantry >> archers >> cavalry >> morale;
    military = Military(infantry, archers, cavalry);
    military.setMorale(morale);
    std::string leaderName;
    file.ignore();
    std::getline(file, leaderName);
    int diplomacy, militarySkill, economySkill, popularity;
    file >> diplomacy >> militarySkill >> economySkill >> popularity;
    leadership = Leadership(leaderName, diplomacy, militarySkill, economySkill);
    leadership.setPopularity(popularity);
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

    int populationScore = population.getTotalPopulation() * 2;
    int resourceScore = (resources.getFood() + resources.getWood() + resources.getStone() + resources.getIron()) / 10;
    int economyScore = economy.getTreasury() / 10;
    int militaryScore = military.calculateMilitaryStrength() * 5;
    int happinessScore = population.getHappiness() * 10;
    int loanPenalty = banking.getLoanAmount() / 10;
    int totalScore = populationScore + resourceScore + economyScore + militaryScore + happinessScore - loanPenalty;

    char timeStr[26];
    time_t now = time(0);
    ctime_s(timeStr, sizeof(timeStr), &now);

    file << timeStr;
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