#include "Stronghold.h"

Economy::Economy() : treasury(1000), taxRate(10), inflationRate(2) {
}

Economy::Economy(int treasury, int taxRate)
    : treasury(treasury), taxRate(taxRate), inflationRate(2) {
}

int Economy::getTreasury() const {
    return treasury;
}

int Economy::getTaxRate() const {
    return taxRate;
}

int Economy::getInflationRate() const {
    return inflationRate;
}

void Economy::setTreasury(int treasury) {
    this->treasury = treasury;
}

void Economy::setTaxRate(int taxRate) {
    this->taxRate = taxRate;
    if (this->taxRate < 0) this->taxRate = 0;
    if (this->taxRate > 50) this->taxRate = 50;
}

void Economy::setInflationRate(int rate) {
    this->inflationRate = rate;
    if (this->inflationRate < 0) this->inflationRate = 0;
    if (this->inflationRate > 30) this->inflationRate = 30;
}

void Economy::collectTaxes(const Population& population) {
    int peasantTax = population.getPeasants() * 1 * taxRate / 100;
    int merchantTax = population.getMerchants() * 5 * taxRate / 100;
    int nobleTax = population.getNobles() * 20 * taxRate / 100;

    int totalTaxes = peasantTax + merchantTax + nobleTax;
    addToTreasury(totalTaxes);

    std::cout << "Collected " << totalTaxes << " gold in taxes." << std::endl;
}

void Economy::payMilitary(const Military& military) {
    int infantryCost = military.getInfantry() * 2;
    int archerCost = military.getArchers() * 3;
    int cavalryCost = military.getCavalry() * 5;

    int totalMilitaryCost = infantryCost + archerCost + cavalryCost;

    if (hasSufficientFunds(totalMilitaryCost)) {
        spendTreasury(totalMilitaryCost);
        std::cout << "Paid " << totalMilitaryCost << " gold to military forces." << std::endl;
    }
    else {
        std::cout << "WARNING: Insufficient funds to pay military! Only paid " << treasury << " gold." << std::endl;
        setTreasury(0);
    }
}

void Economy::adjustInflation() {
    // Simple inflation model based on treasury size and tax rate
    if (treasury > 5000 && taxRate < 15) {
        inflationRate += 1;
    }
    else if (treasury < 1000 || taxRate > 30) {
        inflationRate -= 1;
    }

    // Keep inflation in reasonable bounds
    if (inflationRate < 0) inflationRate = 0;
    if (inflationRate > 30) inflationRate = 30;
}

void Economy::spendTreasury(int amount) {
    if (amount <= treasury) {
        treasury -= amount;
    }
    else {
        treasury = 0;
    }
}

void Economy::addToTreasury(int amount) {
    treasury += amount;
}

bool Economy::hasSufficientFunds(int amount) const {
    return treasury >= amount;
}

void Economy::displayEconomy() const {
    std::cout << "\n===== ECONOMY =====\n";
    std::cout << "Treasury: " << treasury << " gold" << std::endl;
    std::cout << "Tax Rate: " << taxRate << "%" << std::endl;
    std::cout << "Inflation Rate: " << inflationRate << "%" << std::endl;
    std::cout << "==================\n";
}