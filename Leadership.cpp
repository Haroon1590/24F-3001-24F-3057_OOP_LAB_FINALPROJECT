#include "Stronghold.h"

Leadership::Leadership() : name(""), diplomacy(0), militarySkill(0), economySkill(0), popularity(0) {}

Leadership::Leadership(const string& name, int diplomacy, int military, int economy)
    : name(name), diplomacy(diplomacy), militarySkill(military), economySkill(economy), popularity(50) {}

string Leadership::getName() const {
    return name;
}

int Leadership::getDiplomacy() const {
    return diplomacy;
}

int Leadership::getMilitarySkill() const {
    return militarySkill;
}

int Leadership::getEconomySkill() const {
    return economySkill;
}

int Leadership::getPopularity() const {
    return popularity;
}

void Leadership::setName(const string& name) {
    this->name = name;
}

void Leadership::setDiplomacy(int diplomacy) {
    this->diplomacy = diplomacy;
}

void Leadership::setMilitarySkill(int military) {
    this->militarySkill = military;
}

void Leadership::setEconomySkill(int economy) {
    this->economySkill = economy;
}

void Leadership::setPopularity(int popularity) {
    this->popularity = popularity;
}

void Leadership::makeDecision(Economy& economy, Military& military) {
    // Placeholder: Implement decision-making logic
    cout << name << " makes a decision affecting economy and military.\n";
}

void Leadership::handleCoup(const Population& population, const Military& military) {
    // Placeholder: Implement coup logic
    cout << "Handling coup attempt...\n";
}

void Leadership::updatePopularity(const Economy& economy, const Population& population) {
    // Placeholder: Update popularity based on economy and population
    popularity = std::max(0, std::min(100, popularity + economy.getTreasury() / 100 - population.getHappiness() / 10));
}

void Leadership::displayLeadership() const {
    cout << "Leader: " << name << "\n";
    cout << "Diplomacy: " << diplomacy << "\n";
    cout << "Military Skill: " << militarySkill << "\n";
    cout << "Economy Skill: " << economySkill << "\n";
    cout << "Popularity: " << popularity << "\n";
}