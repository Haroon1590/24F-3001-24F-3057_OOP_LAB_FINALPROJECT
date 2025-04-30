#include "Stronghold.h"
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

// Remove all constant definitions as they are now in GameConstants.h

Economy::Economy(int g) : gold(g), taxRate(10), inflation(0), debt(0) {
    if (g < 0) gold = 0;
    
    // Initialize resources with starting values
    resources.food_stores = 100;
    resources.housing = 50;
    resources.military_equipment = 25;
    resources.food = 100;
    resources.wood = 50;
    resources.stone = 25;
    resources.iron = 10;
    resources.storage = 1000;
}

void Economy::tax(Society& soc) {
    int totalTaxes = 0;
    for (int i = 0; i < soc.getClassCount(); ++i) {
        SocialClass* sc = soc.getClassByIndex(i);
        if (sc != nullptr) {
            totalTaxes += sc->payTaxes(taxRate);
        }
    }
    addGold(totalTaxes);
}

void Economy::spend(int amt) {
    if (amt <= gold) {
        gold -= amt;
    } else {
        int deficit = amt - gold;
        gold = 0;
        borrow(deficit);
    }
}

void Economy::war() {
    inflation += 5;
    debt += 100;
}

void Economy::peace() {
    inflation = max(0, inflation - 2);
    debt = max(0, debt - 50);
}

void Economy::print() const {
    cout << "Economy: " << gold << " gold, Tax Rate: " << taxRate 
         << "%, Inflation: " << inflation << "%, Debt: " << debt << endl;
    
    cout << "Resources - Food: " << resources.food 
         << ", Wood: " << resources.wood 
         << ", Stone: " << resources.stone 
         << ", Iron: " << resources.iron << endl;
}

int Economy::getGold() const {
    return gold;
}

void Economy::setTax(int r) {
    taxRate = max(MIN_TAX_RATE, min(MAX_TAX_RATE, r));
}

int Economy::getTaxRate() const {
    return taxRate;
}

int Economy::getInflation() const {
    return inflation;
}

int Economy::getDebt() const {
    return debt;
}

void Economy::borrow(int amt) {
    debt += amt;
    inflation += (amt / 100);
}

void Economy::repay(int amt) {
    if (amt <= gold) {
        gold -= amt;
        debt = max(0, debt - amt);
    }
}

void Economy::addGold(int g) {
    gold += g;
}

void Economy::setInflation(int i) {
    inflation = max(0, i);
}

void Economy::setDebt(int d) {
    debt = max(0, d);
}

double Economy::calculateWealthRatio(SocialClass* sc, double totalWealth) const {
    if (totalWealth == 0) return 0.0;
    return sc->getWealth() / totalWealth;
}

int Economy::calculateDynamicTaxRate(double wealthRatio) const {
    double baseRate = taxRate;
    double wealthFactor = wealthRatio * WEALTH_TAX_MULTIPLIER;
    double dynamicRate = baseRate * (1.0 + wealthFactor);
    return static_cast<int>(std::max(static_cast<double>(MIN_TAX_RATE), 
                                   std::min(static_cast<double>(MAX_TAX_RATE), dynamicRate)));
}
ResourceData& Economy::getResources() {
    return resources;
}
