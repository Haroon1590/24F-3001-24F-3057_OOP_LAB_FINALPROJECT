#include "Stronghold.h"

// Initialize static members
int Economy::lastTaxCollectionTurn = 0;
int Economy::currentTurn = 0;

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

bool Economy::collectTaxes(Population& population) {
    currentTurn++;

    // Can only collect taxes every 4 turns
    if (currentTurn - lastTaxCollectionTurn < 4) {
        std::cout << "Cannot collect taxes yet. Must wait " 
                  << (4 - (currentTurn - lastTaxCollectionTurn)) 
                  << " more turns." << std::endl;
        return false;
    }

    // Calculate base taxes from different social classes with more balanced rates
    double peasantRate = 1.0 + (taxRate / 200.0);  // 1.0 to 1.25 multiplier
    double merchantRate = 1.0 + (taxRate / 150.0);  // 1.0 to 1.33 multiplier
    double nobleRate = 1.0 + (taxRate / 100.0);    // 1.0 to 1.5 multiplier

    int peasantTaxes = static_cast<int>(population.getPeasants() * 2.0 * peasantRate);
    int merchantTaxes = static_cast<int>(population.getMerchants() * 8.0 * merchantRate);
    int nobleTaxes = static_cast<int>(population.getNobles() * 20.0 * nobleRate);

    // Apply inflation adjustment with diminishing returns
    double inflationMultiplier = 1.0 + (inflationRate / 150.0);  // More gradual effect
    int totalTaxes = static_cast<int>((peasantTaxes + merchantTaxes + nobleTaxes) * inflationMultiplier);

    // Add to treasury
    treasury += totalTaxes;

    // Update last tax collection turn
    lastTaxCollectionTurn = currentTurn;

    // Calculate happiness impact based on multiple factors
    int baseTaxImpact = -(taxRate / 6);  // Less severe base impact
    int inflationImpact = -(inflationRate / 3);
    int wealthImpact = (treasury > 5000) ? 1 : -1;  // Wealthy treasury slightly offsets unhappiness

    int happinessImpact = baseTaxImpact + inflationImpact + wealthImpact;
    
    // Cap the maximum negative impact
    if (happinessImpact < -15) happinessImpact = -15;
    
    population.updateHappinessFromAction("tax_collection", happinessImpact);

    // Detailed tax collection report
    std::cout << "\n=== Tax Collection Report ===\n";
    std::cout << "Taxes collected from:\n";
    std::cout << "  - Peasants: " << peasantTaxes << " gold (Rate: " << (peasantRate * 100 - 100) << "% extra)\n";
    std::cout << "  - Merchants: " << merchantTaxes << " gold (Rate: " << (merchantRate * 100 - 100) << "% extra)\n";
    std::cout << "  - Nobles: " << nobleTaxes << " gold (Rate: " << (nobleRate * 100 - 100) << "% extra)\n";
    std::cout << "Inflation adjustment: " << ((inflationMultiplier - 1.0) * 100) << "%\n";
    std::cout << "Total collected: " << totalTaxes << " gold\n";
    std::cout << "Happiness impact: " << happinessImpact << " points\n";
    std::cout << "=========================\n";
    
    return true;
}

void Economy::payMilitary(const Military& military) {
    // More realistic military costs with inflation adjustment
    int infantryCost = static_cast<int>(military.getInfantry() * 2.5 * (1.0 + inflationRate / 100.0));
    int archerCost = static_cast<int>(military.getArchers() * 4.0 * (1.0 + inflationRate / 100.0));
    int cavalryCost = static_cast<int>(military.getCavalry() * 7.5 * (1.0 + inflationRate / 100.0));

    int totalMilitaryCost = infantryCost + archerCost + cavalryCost;

    if (hasSufficientFunds(totalMilitaryCost)) {
        spendTreasury(totalMilitaryCost);
        std::cout << "Paid " << totalMilitaryCost << " gold to military forces." << std::endl;
        std::cout << "Breakdown: " << infantryCost << " for infantry, "
                  << archerCost << " for archers, "
                  << cavalryCost << " for cavalry." << std::endl;
    }
    else {
        // Partial payment if insufficient funds
        int partialPayment = treasury;
        spendTreasury(partialPayment);
        std::cout << "WARNING: Insufficient funds to pay military! Only paid " 
                  << partialPayment << " gold out of " << totalMilitaryCost << " required." << std::endl;
    }
}

void Economy::adjustInflation(Population& population) {
    int previousInflation = inflationRate;
    
    // More sophisticated inflation calculation
    double inflationChange = 0;
    
    // Treasury size effect (with diminishing returns)
    if (treasury > 20000) inflationChange += 2;
    else if (treasury > 10000) inflationChange += 1;
    else if (treasury < 1000) inflationChange -= 1;
    
    // Tax rate effects on inflation (more nuanced)
    if (taxRate > 40) {
        // Very high taxes: Initial deflationary pressure due to reduced spending
        inflationChange -= 1;
        // But if treasury is high, government spending could cause inflation
        if (treasury > 5000) inflationChange += 2;
    }
    else if (taxRate > 30) {
        // High taxes: Moderate deflationary pressure
        inflationChange -= 0.5;
        // But still some government spending effect
        if (treasury > 7500) inflationChange += 1;
    }
    else if (taxRate < 10) {
        // Very low taxes: Could lead to inflation due to increased spending power
        inflationChange += 1;
        // Unless treasury is very low (government austerity)
        if (treasury < 1000) inflationChange -= 1;
    }
    
    // Population size effect
    int totalPop = population.getTotalPopulation();
    if (totalPop > 1000) inflationChange += 1;
    else if (totalPop < 200) inflationChange -= 1;
    
    // Economic activity modifier based on merchant population
    double merchantRatio = static_cast<double>(population.getMerchants()) / totalPop;
    if (merchantRatio > 0.2) inflationChange += 1; // High merchant activity can drive inflation
    
    // Tax collection frequency effect
    if (currentTurn - lastTaxCollectionTurn <= 1) {
        // Frequent tax collection can affect market stability
        inflationChange += 0.5;
    }
    
    // Random market fluctuation (-1 to +1)
    inflationChange += (rand() % 3 - 1);
    
    // Apply change with bounds checking
    inflationRate += static_cast<int>(inflationChange);
    if (inflationRate > 30) inflationRate = 30;
    if (inflationRate < 0) inflationRate = 0;
    
    // Calculate happiness impact
    if (inflationRate != previousInflation) {
        int happinessImpact = (previousInflation - inflationRate) / 2;  // Positive when inflation decreases
        
        std::cout << "\n=== Economic Report ===\n";
        std::cout << "Inflation " << (inflationRate > previousInflation ? "increased" : "decreased");
        std::cout << " from " << previousInflation << "% to " << inflationRate << "%\n";
        
        std::cout << "Economic Factors:\n";
        if (treasury > 10000) std::cout << "- High treasury reserves (Inflationary)\n";
        if (treasury < 1000) std::cout << "- Low treasury reserves (Deflationary)\n";
        if (taxRate > 30) std::cout << "- High tax rate (Initially Deflationary)\n";
        if (taxRate < 10) std::cout << "- Low tax rate (Potentially Inflationary)\n";
        if (totalPop > 1000) std::cout << "- Large population (Inflationary)\n";
        if (merchantRatio > 0.2) std::cout << "- High merchant activity (Inflationary)\n";
        if (currentTurn - lastTaxCollectionTurn <= 1) std::cout << "- Frequent tax collection (Market Instability)\n";
        
        population.updateHappinessFromAction("inflation", happinessImpact);
        std::cout << "Population happiness impact: " << happinessImpact << " points\n";
        std::cout << "=====================\n";
    }
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