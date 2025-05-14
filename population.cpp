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

    // Base growth rates (per 100 people)
    const double BASE_PEASANT_GROWTH = 0.02;  // 2%
    const double BASE_MERCHANT_GROWTH = 0.015; // 1.5%
    const double BASE_NOBLE_GROWTH = 0.01;    // 1%

    // Modifiers based on conditions
    double foodModifier = (foodPerPerson >= 2.0) ? 1.2 : 
                         (foodPerPerson >= 1.5) ? 1.1 : 
                         (foodPerPerson >= 1.0) ? 1.0 : 0.8;

    double happinessModifier = (happiness >= 80) ? 1.2 :
                              (happiness >= 60) ? 1.1 :
                              (happiness >= 40) ? 1.0 :
                              (happiness >= 20) ? 0.9 : 0.8;

    double taxModifier = (taxRate <= 10) ? 1.1 :
                        (taxRate <= 20) ? 1.0 :
                        (taxRate <= 30) ? 0.9 : 0.8;

    // Calculate growth/decline for each class
    if (foodPerPerson >= 1.0 && happiness > 30) {
        // Population growth with modifiers
        int peasantGrowth = static_cast<int>(peasants * BASE_PEASANT_GROWTH * foodModifier * happinessModifier * taxModifier);
        int merchantGrowth = static_cast<int>(merchants * BASE_MERCHANT_GROWTH * foodModifier * happinessModifier * taxModifier);
        int nobleGrowth = static_cast<int>(nobles * BASE_NOBLE_GROWTH * foodModifier * happinessModifier);

        peasants += peasantGrowth;
        merchants += merchantGrowth;
        nobles += nobleGrowth;

        cout << "\n=== Population Growth Report ===\n";
        cout << "Growth factors:\n";
        cout << "- Food availability: " << (foodModifier * 100 - 100) << "% bonus\n";
        cout << "- Happiness: " << (happinessModifier * 100 - 100) << "% bonus\n";
        cout << "- Tax rate: " << (taxModifier * 100 - 100) << "% bonus\n";
        cout << "Population changes:\n";
        cout << "- Peasants: +" << peasantGrowth << "\n";
        cout << "- Merchants: +" << merchantGrowth << "\n";
        cout << "- Nobles: +" << nobleGrowth << "\n";

        // Social mobility: Small chance for peasants to become merchants and merchants to become nobles
        if (happiness >= 60 && foodPerPerson >= 1.5) {
            int peasantPromotion = peasants / 100;  // 1% chance
            int merchantPromotion = merchants / 200; // 0.5% chance
            
            peasants -= peasantPromotion;
            merchants += peasantPromotion;
            merchants -= merchantPromotion;
            nobles += merchantPromotion;

            cout << "Social mobility occurred!\n";
            cout << "- " << peasantPromotion << " peasants became merchants\n";
            cout << "- " << merchantPromotion << " merchants became nobles\n";
        }

        cout << "============================\n";
    }
    else {
        // Population decline
        double declineRate = (foodPerPerson < 1.0) ? 0.05 : 0.03;
        if (happiness < 20) declineRate += 0.02;

        int peasantDecline = static_cast<int>(peasants * declineRate);
        int merchantDecline = static_cast<int>(merchants * declineRate);
        int nobleDecline = static_cast<int>(nobles * declineRate);

        peasants -= peasantDecline;
        merchants -= merchantDecline;
        nobles -= nobleDecline;

        cout << "\n=== Population Decline Report ===\n";
        cout << "Decline factors:\n";
        cout << "- Food shortage: " << (foodPerPerson < 1.0 ? "Yes" : "No") << "\n";
        cout << "- Low happiness: " << (happiness < 20 ? "Yes" : "No") << "\n";
        cout << "Population losses:\n";
        cout << "- Peasants: -" << peasantDecline << "\n";
        cout << "- Merchants: -" << merchantDecline << "\n";
        cout << "- Nobles: -" << nobleDecline << "\n";
        cout << "============================\n";
    }

    // Ensure no negative population
    if (peasants < 0) peasants = 0;
    if (merchants < 0) merchants = 0;
    if (nobles < 0) nobles = 0;

    // Update happiness based on population changes
    if (getTotalPopulation() > totalPop) {
        increaseHappiness(2);
    } else if (getTotalPopulation() < totalPop) {
        decreaseHappiness(3);
    }
}

void Population::increaseHappiness(int amount) {
    happiness += amount;
    if (happiness > 100) happiness = 100;
}

void Population::decreaseHappiness(int amount) {
    happiness -= amount;
    if (happiness < 0) happiness = 0;
}

// New method to handle happiness changes from kingdom actions
void Population::updateHappinessFromAction(const string& action, int impact) {
    int previousHappiness = happiness;
    
    // Base happiness change
    happiness += impact;
    
    // Additional effects based on population composition
    double peasantRatio = static_cast<double>(peasants) / getTotalPopulation();
    double merchantRatio = static_cast<double>(merchants) / getTotalPopulation();
    double nobleRatio = static_cast<double>(nobles) / getTotalPopulation();

    if (action == "tax_collection") {
        // Different classes react differently to taxes
        happiness -= static_cast<int>(impact * (peasantRatio * 1.2));  // Peasants are most affected
        happiness -= static_cast<int>(impact * (merchantRatio * 0.8)); // Merchants less affected
        happiness -= static_cast<int>(impact * (nobleRatio * 0.5));   // Nobles least affected
    }
    else if (action == "food_distribution") {
        // Poor benefit more from food distribution
        happiness += static_cast<int>(impact * (peasantRatio * 1.5));
        happiness += static_cast<int>(impact * merchantRatio);
        happiness += static_cast<int>(impact * (nobleRatio * 0.5));
    }
    else if (action == "military_recruitment") {
        // Nobles prefer strong military
        happiness += static_cast<int>(impact * (nobleRatio * 1.5));
        happiness += static_cast<int>(impact * merchantRatio);
        happiness += static_cast<int>(impact * (peasantRatio * 0.7));
    }
    else if (action == "construction") {
        // Merchants benefit most from construction
        happiness += static_cast<int>(impact * (merchantRatio * 1.3));
        happiness += static_cast<int>(impact * peasantRatio);
        happiness += static_cast<int>(impact * nobleRatio);
    }

    // Keep happiness in bounds
    if (happiness > 100) happiness = 100;
    if (happiness < 0) happiness = 0;

    // Report significant changes
    if (happiness != previousHappiness) {
        cout << "\n=== Happiness Update ===\n";
        cout << "Action: " << action << "\n";
        cout << "Base impact: " << impact << "\n";
        cout << "Class-specific reactions:\n";
        cout << "- Peasants (" << (peasantRatio * 100) << "% of population)\n";
        cout << "- Merchants (" << (merchantRatio * 100) << "% of population)\n";
        cout << "- Nobles (" << (nobleRatio * 100) << "% of population)\n";
        cout << "Total happiness change: " << (happiness - previousHappiness) << "\n";
        cout << "New happiness level: " << happiness << "/100\n";
        cout << "=====================\n";
    }
}

bool Population::isRevolting() const {
    return happiness < 20;
}

void Population::handleRevolt() {
    cout << "\n*** REVOLT IN THE KINGDOM! ***\n";
    cout << "The population is unhappy and revolting!\n";

    // Consequences of revolt
    peasants -= static_cast<int>(peasants * 0.1);
    merchants -= static_cast<int>(merchants * 0.15);
    nobles -= static_cast<int>(nobles * 0.05);

    // Reset happiness to slightly better than revolt level
    happiness = 25;

    // Display revolt consequences
    cout << "Population losses: " << static_cast<int>(peasants * 0.1) << " peasants, "
              << static_cast<int>(merchants * 0.15) << " merchants, "
              << static_cast<int>(nobles * 0.05) << " nobles." << endl;
}

void Population::displayPopulation() const {
    cout << "\n===== POPULATION =====\n";
    cout << "Peasants: " << peasants << endl;
    cout << "Merchants: " << merchants << endl;
    cout << "Nobles: " << nobles << endl;
    cout << "Total Population: " << getTotalPopulation() << endl;
    cout << "Happiness Level: " << happiness << "/100" << endl;

    // Display happiness status
    cout << "Happiness Status: ";
    if (happiness > 80) cout << "Very Happy";
    else if (happiness > 60) cout << "Content";
    else if (happiness > 40) cout << "Neutral";
    else if (happiness > 20) cout << "Unhappy";
    else cout << "On the Verge of Revolt!";
    cout << endl;

    cout << "=====================\n";
}