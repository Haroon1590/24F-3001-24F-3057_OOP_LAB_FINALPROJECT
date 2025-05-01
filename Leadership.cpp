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

    // Economic decision based on economy skill
    if (economySkill > 70) {
        // A skilled economic leader may adjust tax rates intelligently
        if (economy.getTreasury() < 500) {
            economy.setTaxRate(economy.getTaxRate() + 3);
            std::cout << "Tax rate increased to " << economy.getTaxRate() << "% to address low treasury." << std::endl;
        }
        else if (economy.getTaxRate() > 15 && economy.getTreasury() > 2000) {
            economy.setTaxRate(economy.getTaxRate() - 2);
            std::cout << "Tax rate decreased to " << economy.getTaxRate() << "% to improve happiness." << std::endl;
        }
    }
    else {
        // Less skilled economic leaders make less optimal decisions
        if (economy.getTreasury() < 1000) {
            economy.setTaxRate(economy.getTaxRate() + 5);
            std::cout << "Tax rate increased to " << economy.getTaxRate() << "% due to treasury concerns." << std::endl;
        }
    }

    // Military decision based on military skill
    if (militarySkill > 70) {
        // Skilled military leaders focus on training
        if (military.getMorale() < 60) {
            std::cout << "Leader focuses on improving military morale." << std::endl;
            military.setMorale(military.getMorale() + 10);
        }
    }
    else {
        // Poor military leaders may make suboptimal decisions
        if (military.getTotalForces() < 30) {
            std::cout << "Leader is concerned about low military forces." << std::endl;
        }
    }
}

void Leadership::handleCoup(const Population& population, const Military& military) {
    // Calculate coup success chance based on popularity, military morale and strength
    bool coupPossible = popularity < 30 && population.getHappiness() < 40;

    if (coupPossible) {
        // Determine if military supports the coup based on morale
        bool militarySupport = military.getMorale() < 40;

        if (militarySupport) {
            std::cout << "\n*** COUP D'ÉTAT ***\n";
            std::cout << name << " has been overthrown by military forces!" << std::endl;

            // Generate a new leader with random attributes
            name = "New Ruler";
            diplomacy = 30 + rand() % 50;  // 30-80
            militarySkill = 50 + rand() % 40;  // 50-90 (Military coup leaders tend to have good military skills)
            economySkill = 30 + rand() % 50;  // 30-80
            popularity = 50 + rand() % 20;    // 50-70 (Initial honeymoon period)

            std::cout << name << " has assumed leadership of the kingdom." << std::endl;
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