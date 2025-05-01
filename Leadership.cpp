#include "Stronghold.h"

Leadership::Leadership()
    : name("Default Ruler"), diplomacy(50), militarySkill(50), economySkill(50), popularity(50) {
}

Leadership::Leadership(const std::string& name, int diplomacy, int military, int economy)
    : name(name), diplomacy(diplomacy), militarySkill(military), economySkill(economy), popularity(50) {
}

std::string Leadership::getName() const {
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

void Leadership::setName(const std::string& name) {
    this->name = name;
}

void Leadership::setDiplomacy(int diplomacy) {
    this->diplomacy = diplomacy;
    if (this->diplomacy > 100) this->diplomacy = 100;
    if (this->diplomacy < 0) this->diplomacy = 0;
}

void Leadership::setMilitarySkill(int military) {
    this->militarySkill = military;
    if (this->militarySkill > 100) this->militarySkill = 100;
    if (this->militarySkill < 0) this->militarySkill = 0;
}

void Leadership::setEconomySkill(int economy) {
    this->economySkill = economy;
    if (this->economySkill > 100) this->economySkill = 100;
    if (this->economySkill < 0) this->economySkill = 0;
}

void Leadership::setPopularity(int popularity) {
    this->popularity = popularity;
    if (this->popularity > 100) this->popularity = 100;
    if (this->popularity < 0) this->popularity = 0;
}

void Leadership::makeDecision(Economy& economy, Military& military) {
    std::cout << "\n" << name << " is making decisions for the kingdom...\n";

    // Economic decisions based on economy skill
    if (economySkill > 70) {
        // Skilled economic leader makes better decisions
        if (economy.getTreasury() < 500) {
            int taxIncrease = 2 + (economy.getTaxRate() < 20 ? 1 : 0);
            economy.setTaxRate(economy.getTaxRate() + taxIncrease);
            std::cout << "Tax rate increased by " << taxIncrease << "% to " 
                      << economy.getTaxRate() << "% to address low treasury." << std::endl;
        }
        else if (economy.getTaxRate() > 15 && economy.getTreasury() > 2000) {
            int taxDecrease = 2 + (economy.getTaxRate() > 25 ? 1 : 0);
            economy.setTaxRate(economy.getTaxRate() - taxDecrease);
            std::cout << "Tax rate decreased by " << taxDecrease << "% to " 
                      << economy.getTaxRate() << "% to improve happiness." << std::endl;
        }
    }
    else if (economySkill < 30) {
        // Poor economic leader makes worse decisions
        if (economy.getTreasury() < 1000) {
            int taxIncrease = 5 + (economy.getTaxRate() < 15 ? 3 : 0);
            economy.setTaxRate(economy.getTaxRate() + taxIncrease);
            std::cout << "Tax rate increased by " << taxIncrease << "% to " 
                      << economy.getTaxRate() << "% due to treasury concerns." << std::endl;
        }
    }

    // Military decisions based on military skill
    if (militarySkill > 70) {
        // Skilled military leader focuses on training and morale
        if (military.getMorale() < 60) {
            int moraleBoost = 5 + (military.getMorale() < 40 ? 5 : 0);
            military.setMorale(military.getMorale() + moraleBoost);
            std::cout << "Leader focuses on improving military morale by " 
                      << moraleBoost << " points." << std::endl;
        }
    }
    else if (militarySkill < 30) {
        // Poor military leader may make suboptimal decisions
        if (military.getTotalForces() < 30) {
            std::cout << "Leader is concerned about low military forces but lacks the skill to address it effectively." << std::endl;
        }
    }

    // Diplomatic decisions based on diplomacy skill
    if (diplomacy > 70) {
        // Skilled diplomat can improve relations and reduce tensions
        if (popularity < 40) {
            int popularityBoost = 3 + (popularity < 20 ? 2 : 0);
            setPopularity(popularity + popularityBoost);
            std::cout << "Leader's diplomatic efforts increased popularity by " 
                      << popularityBoost << " points." << std::endl;
        }
    }
}

void Leadership::handleCoup(const Population& population, const Military& military) {
    // More sophisticated coup calculation
    bool coupPossible = popularity < 30 && population.getHappiness() < 40;
    int coupChance = 0;

    if (coupPossible) {
        // Calculate coup chance based on multiple factors
        coupChance += (30 - popularity) / 2;  // Lower popularity = higher chance
        coupChance += (40 - population.getHappiness()) / 4;  // Lower happiness = higher chance
        coupChance += military.getMorale() < 40 ? 20 : 0;  // Low morale = higher chance
        coupChance += military.getTotalForces() > population.getTotalPopulation() / 5 ? 15 : 0;  // Large military = higher chance

        // Random element
        coupChance += rand() % 20;

        if (coupChance > 50) {
            std::cout << "\n*** COUP D'ÉTAT ***\n";
            std::cout << name << " has been overthrown by military forces!" << std::endl;

            // Generate a new leader with attributes influenced by the coup
            name = "New Ruler";
            diplomacy = 30 + rand() % 50;  // 30-80
            militarySkill = 50 + rand() % 40;  // 50-90 (Military coup leaders tend to have good military skills)
            economySkill = 30 + rand() % 50;  // 30-80
            popularity = 50 + rand() % 20;    // 50-70 (Initial honeymoon period)

            std::cout << name << " has assumed leadership of the kingdom." << std::endl;
            std::cout << "New leader's skills: Diplomacy=" << diplomacy 
                      << ", Military=" << militarySkill 
                      << ", Economy=" << economySkill << std::endl;
        }
    }
}

void Leadership::updatePopularity(const Economy& economy, const Population& population) {
    int previousPopularity = popularity;

    // Economic factors influence popularity
    if (economy.getTaxRate() > 25) {
        popularity -= (economy.getTaxRate() - 25) / 5;
    }
    else if (economy.getTaxRate() < 15) {
        popularity += 1;
    }

    // Population happiness influences popularity
    if (population.getHappiness() > 70) {
        popularity += 2;
    }
    else if (population.getHappiness() < 30) {
        popularity -= 3;
    }

    // Treasury state influences popularity
    if (economy.getTreasury() > 3000) {
        popularity += 1;
    }
    else if (economy.getTreasury() < 500) {
        popularity -= 2;
    }

    // Leader's skills influence popularity
    popularity += (diplomacy - 50) / 20;  // Diplomatic skill bonus
    popularity += (economySkill - 50) / 25;  // Economic skill bonus

    // Keep popularity in bounds
    if (popularity > 100) popularity = 100;
    if (popularity < 0) popularity = 0;

    if (popularity != previousPopularity) {
        std::cout << "Leader's popularity " << (popularity > previousPopularity ? "increased" : "decreased")
            << " to " << popularity << "." << std::endl;
    }
}

void Leadership::displayLeadership() const {
    std::cout << "\n===== LEADERSHIP =====\n";
    std::cout << "Ruler: " << name << std::endl;
    std::cout << "Diplomacy Skill: " << diplomacy << "/100" << std::endl;
    std::cout << "Military Skill: " << militarySkill << "/100" << std::endl;
    std::cout << "Economy Skill: " << economySkill << "/100" << std::endl;
    std::cout << "Popularity: " << popularity << "/100" << std::endl;
    std::cout << "=====================\n";
}