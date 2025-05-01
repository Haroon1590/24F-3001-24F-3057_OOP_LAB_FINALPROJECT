#include "Stronghold.h"

Population::Population() : peasants(100), merchants(20), nobles(5), happiness(50) {
}

Population::Population(int peasants, int merchants, int nobles)
    : peasants(peasants), merchants(merchants), nobles(nobles), happiness(50) {
}

int Population::getPeasants() const {
    return peasants;
}

int Population::getMerchants() const {
    return merchants;
}

int Population::getNobles() const {
    return nobles;
}

int Population::getTotalPopulation() const {
    return peasants + merchants + nobles;
}

int Population::getHappiness() const {
    return happiness;
}

void Population::setPeasants(int peasants) {
    this->peasants = peasants;
}

void Population::setMerchants(int merchants) {
    this->merchants = merchants;
}

void Population::setNobles(int nobles) {
    this->nobles = nobles;
}

void Population::setHappiness(int happiness) {
    this->happiness = happiness;
    if (this->happiness > 100) this->happiness = 100;
    if (this->happiness < 0) this->happiness = 0;
}

void Population::updatePopulation(const Resource& resources, int taxRate) {
    // Calculate food availability per person
    int totalPop = getTotalPopulation();
    if (totalPop == 0) return;

    double foodPerPerson = static_cast<double>(resources.getFood()) / totalPop;

    // Population growth or decline based on food and happiness
    if (foodPerPerson >= 2.0 && happiness > 50) {
        // Population growth
        peasants += peasants * 0.05;
        merchants += merchants * 0.03;
        nobles += nobles * 0.01;
    }
    else if (foodPerPerson < 1.0 || happiness < 30) {
        // Population decline
        peasants -= peasants * 0.08;
        merchants -= merchants * 0.05;
        nobles -= nobles * 0.02;
    }

    // Tax rate affects happiness
    if (taxRate > 20) {
        decreaseHappiness((taxRate - 20) / 5);
    }
    else {
        increaseHappiness(2);
    }

    // Ensure no negative population
    if (peasants < 0) peasants = 0;
    if (merchants < 0) merchants = 0;
    if (nobles < 0) nobles = 0;
}

void Population::increaseHappiness(int amount) {
    happiness += amount;
    if (happiness > 100) happiness = 100;
}

void Population::decreaseHappiness(int amount) {
    happiness -= amount;
    if (happiness < 0) happiness = 0;
}

bool Population::isRevolting() const {
    return happiness < 20;
}

void Population::handleRevolt() {
    std::cout << "\n*** REVOLT IN THE KINGDOM! ***\n";
    std::cout << "The population is unhappy and revolting!\n";

    // Consequences of revolt
    peasants -= peasants * 0.1;
    merchants -= merchants * 0.15;

    // Reset happiness to slightly better than revolt level
    happiness = 25;
}

void Population::displayPopulation() const {
    std::cout << "\n===== POPULATION =====\n";
    std::cout << "Peasants: " << peasants << std::endl;
    std::cout << "Merchants: " << merchants << std::endl;
    std::cout << "Nobles: " << nobles << std::endl;
    std::cout << "Total Population: " << getTotalPopulation() << std::endl;
    std::cout << "Happiness Level: " << happiness << "/100" << std::endl;

    std::cout << "Happiness Status: ";
    if (happiness > 80) std::cout << "Very Happy";
    else if (happiness > 60) std::cout << "Content";
    else if (happiness > 40) std::cout << "Neutral";
    else if (happiness > 20) std::cout << "Unhappy";
    else std::cout << "On the Verge of Revolt!";
    std::cout << std::endl;

    std::cout << "=====================\n";
}