#include "Stronghold.h"
#include <iostream>
#include <cstring>
#include <cmath>
#include <algorithm>

struct RevoltData {
    int strength;
    int organization;
    char demands[3][MAX_NAME];  // Maximum 3 demands
    int demandCount;
    double success_chance;
    RevoltData() : strength(0), organization(0), demandCount(0), success_chance(0.0) {}
};

struct History {
    int population_history[MAX_HISTORY];
    int wealth_history[MAX_HISTORY];
    char significant_events[MAX_HISTORY][MAX_NAME];
    int historyCount;
    History() : historyCount(0) {}
};

SocialClass::SocialClass(const char* n, int pop, Population* popMgr) : 
    population(pop), 
    happiness(1.0), 
    loyalty(1.0), 
    wealth(BASE_WEALTH), 
    revolting(false), 
    deathRate(0),
    popManager(popMgr),
    relationshipCount(0) {
    strncpy_s(name, MAX_NAME, n, MAX_NAME - 1);
    name[MAX_NAME - 1] = '\0';
    
    // Initialize demographics
    demographics.working_age = static_cast<int>(pop * 0.6);
    demographics.children = static_cast<int>(pop * 0.3);
    demographics.elderly = pop - demographics.working_age - demographics.children;
}

void SocialClass::print() const {
    std::cout << name << ": Population=" << population 
         << ", Happiness=" << (happiness * 100) << "%"
         << ", Loyalty=" << (loyalty * 100) << "%"
         << ", Wealth=" << wealth
         << ", Death Rate=" << deathRate << "%";
    if (revolting) std::cout << " (REVOLTING)";
    std::cout << std::endl;
}

const char* SocialClass::getName() const {
    return name;
}

int SocialClass::getPopulation() const {
    return population;
}

void SocialClass::changePopulation(int delta) {
    population = std::max(0, population + delta);
}

void SocialClass::changeHappiness(double delta) {
    happiness = std::max(0.0, std::min(1.0, happiness + delta));
    checkRevolt();  // Check for revolt after happiness change
}

void SocialClass::changeLoyalty(double delta) {
    loyalty = std::max(0.0, std::min(1.0, loyalty + delta));
    checkRevolt();  // Check for revolt after loyalty change
}

bool SocialClass::isRevolting() const {
    return revolting;
}

void SocialClass::checkRevolt() {
    // Check for revolt conditions based on happiness or loyalty reaching 0%
    if (happiness <= 0.0 || loyalty <= 0.0) {
        if (!revolting) {
            revolting = true;
            if (popManager != nullptr) {
                popManager->suppressRevolt(-getPopulation());
            }
            std::cout << name << " has started revolting due to " 
                 << (happiness <= 0.0 ? "zero happiness!" : "zero loyalty!") << std::endl;
        }
    } else if (revolting && happiness > 0.0 && loyalty > 0.0) {
        // Only recover from revolt if both happiness and loyalty are above 0
        revolting = false;
        if (popManager != nullptr) {
            popManager->suppressRevolt(getPopulation());
        }
        std::cout << name << " has stopped revolting as conditions have improved." << std::endl;
    }

    // Ensure happiness and loyalty stay within bounds
    happiness = std::max(0.0, std::min(1.0, happiness));
    loyalty = std::max(0.0, std::min(1.0, loyalty));
}

void SocialClass::update() {
    // Update happiness and loyalty based on wealth changes
    double wealthRatio = static_cast<double>(wealth) / BASE_WEALTH;
    
    if (wealthRatio < 0.3) {
        changeHappiness(-0.2);
        changeLoyalty(-0.2);
    } else if (wealthRatio > 1.5) {
        changeHappiness(0.1);
        changeLoyalty(0.1);
    }

    // Update death rate based on happiness
    updateDeathRate();
    
    // Check for revolt conditions
    checkRevolt();
}

int SocialClass::getWealth() const {
    return wealth;
}

void SocialClass::changeWealth(int delta) {
    wealth = std::max(0, wealth + delta);
}

double SocialClass::getHappiness() const {
    return happiness;
}

double SocialClass::getLoyalty() const {
    return loyalty;
}

int SocialClass::payTaxes(int taxRate) {
    // Store wealth before taxes for percentage calculation
    int initialWealth = wealth;
    
    // Calculate taxes
    double taxAmount = (wealth * taxRate) / 100.0;
    int taxes = static_cast<int>(std::ceil(taxAmount));
    taxes = std::min(taxes, wealth);  // Don't take more than available
    
    // Apply tax deduction
    changeWealth(-taxes);
    
    // Standard 10% decrease in happiness and loyalty for tax collection
    changeHappiness(-0.10);  // 10% decrease in happiness
    changeLoyalty(-0.10);    // 10% decrease in loyalty
    
    // Calculate remaining wealth percentage
    double remainingWealthPercent = (static_cast<double>(wealth) / initialWealth) * 100;
    
    // Additional effects based on remaining wealth
    if (remainingWealthPercent <= 30) {
        changeHappiness(-0.1);  // Additional penalty for very low wealth
        changeLoyalty(-0.1);
    } else if (remainingWealthPercent <= 50) {
        changeHappiness(-0.05);  // Smaller additional penalty
        changeLoyalty(-0.05);
    }
    
    checkRevolt();
    return taxes;
}

void SocialClass::updateDeathRate() {
    deathRate = static_cast<int>((1.0 - happiness) * 10 + (wealth < LOW_WEALTH_THRESHOLD ? 5 : 0));
}

int SocialClass::getDeathRate() const {
    return deathRate;
}

Peasant::Peasant(int pop, Population* popMgr) : SocialClass("Peasant", pop, popMgr) {
    happiness = 1.0;   // 100%
    loyalty = 1.0;     // 100%
    wealth = 1000;     // Initial wealth for peasants
}
void Peasant::update() {
    updateDeathRate();
    int deaths = (population * deathRate) / 100;
    changePopulation(-deaths);
    
    if (happiness > 0.5 && wealth > LOW_WEALTH_THRESHOLD) {
        changePopulation(int(population * 0.05));
    }
    else if (happiness < 0.3 || wealth < MIN_WEALTH_REVOLT) {
        changePopulation(-int(population * 0.03));
    }
    
    checkRevolt();
}
void Peasant::interact(SocialClass& other) {
    if (strcmp(other.getName(), "Noble") == 0) {
        changeHappiness(-0.02);
        changeLoyalty(-0.01);
        if (other.getWealth() > wealth * 5) {
            changeHappiness(-0.03);
        }
    }
    else if (strcmp(other.getName(), "Merchant") == 0) {
        if (wealth > LOW_WEALTH_THRESHOLD) {
            changeHappiness(0.01);
        }
        else {
            changeHappiness(-0.01);
        }
    }
    else if (strcmp(other.getName(), "Clergy") == 0) {
        changeHappiness(0.02);
        changeLoyalty(0.01);
    }
}

Noble::Noble(int pop, Population* popMgr) : SocialClass("Noble", pop, popMgr) {
    happiness = 1.0;   // 100%
    loyalty = 1.0;     // 100%
    wealth = 10000;    // Initial wealth for nobles
}
void Noble::update() {
    updateDeathRate();
    int deaths = (population * deathRate) / 100;
    changePopulation(-deaths);
    
    if (happiness > 0.8 && wealth > HIGH_WEALTH_THRESHOLD) {
        changePopulation(1);
    }
    else if (happiness < 0.3) {
        changePopulation(-1);
    }
    
    checkRevolt();
}
void Noble::interact(SocialClass& other) {
    if (strcmp(other.getName(), "Peasant") == 0) {
        if (other.getWealth() > LOW_WEALTH_THRESHOLD) {
            changeHappiness(0.01);
        }
    }
    else if (strcmp(other.getName(), "Merchant") == 0) {
        if (wealth > other.getWealth()) {
            changeHappiness(0.02);
        }
        else {
            changeHappiness(-0.01);
        }
    }
    else if (strcmp(other.getName(), "Clergy") == 0) {
        changeHappiness(0.02);
        changeLoyalty(0.01);
    }
}

Merchant::Merchant(int pop, Population* popMgr) : SocialClass("Merchant", pop, popMgr) {
    happiness = 1.0;   // 100%
    loyalty = 1.0;     // 100%
    wealth = 5000;     // Initial wealth for merchants
}
void Merchant::update() {
    updateDeathRate();
    int deaths = (population * deathRate) / 100;
    changePopulation(-deaths);
    
    if (happiness > 0.4 && wealth > LOW_WEALTH_THRESHOLD) {
        changePopulation(int(population * 0.03));
    }
    else if (happiness < 0.2 || wealth < LOW_WEALTH_THRESHOLD) {
        changePopulation(-int(population * 0.02));
    }
    
    checkRevolt();
}
void Merchant::interact(SocialClass& other) {
    if (strcmp(other.getName(), "Peasant") == 0) {
        // Merchants need peasant customers
        if (other.getWealth() > 3) {
            changeHappiness(0.01);
        }
    }
    else if (strcmp(other.getName(), "Noble") == 0) {
        // Merchants benefit from noble patronage
        if (wealth > 15) {
            changeHappiness(0.03);
        }
    }
    else if (strcmp(other.getName(), "Clergy") == 0) {
        // Merchants and clergy have mixed relations
        changeHappiness(0.01);
    }
}

Clergy::Clergy(int pop, Population* popMgr) : SocialClass("Clergy", pop, popMgr) {
    happiness = 1.0;   // 100%
    loyalty = 1.0;     // 100%
    wealth = 3000;     // Initial wealth for clergy
}
void Clergy::update() {
    // Update death rate
    updateDeathRate();
    
    // Apply deaths
    int deaths = (population * deathRate) / 100;
    changePopulation(-deaths);
    
    // Population changes based on conditions
    if (happiness > 0.5) {
        changePopulation(1);
    }
    else if (happiness < 0.3) {
        changePopulation(-1);
    }
    
    checkRevolt();
}
void Clergy::interact(SocialClass& other) {
    if (strcmp(other.getName(), "Peasant") == 0 || strcmp(other.getName(), "Noble") == 0) {
        // Clergy provides spiritual guidance to all
        other.changeHappiness(0.01);
        if (other.getWealth() < 5) {
            other.changeHappiness(0.02); // Extra comfort for the poor
        }
    }
    else if (strcmp(other.getName(), "Merchant") == 0) {
        // Clergy may be wary of excessive wealth
        if (other.getWealth() > 20) {
            changeHappiness(-0.01);
        }
    }
}

Society::Society(Population* pop) : classCount(0), population(pop) {
    for (int i = 0; i < MAX_SOCIAL_CLASSES; ++i) {
        classes[i] = nullptr;
    }
}

Society::~Society() {
    for (int i = 0; i < classCount; ++i) {
        delete classes[i];
        classes[i] = nullptr;
    }
    classCount = 0;
}

void Society::addClass(SocialClass* c) {
    if (c && classCount < MAX_SOCIAL_CLASSES) {
        classes[classCount++] = c;
    }
}

void Society::update() {
    for (int i = 0; i < classCount; ++i) {
        if (classes[i]) {
            classes[i]->update();
            classes[i]->checkRevolt();
        }
    }
    updateRevoltingPopulation();
}

void Society::print() const {
    std::cout << "\nSocial Structure:" << std::endl;
    for (int i = 0; i < classCount; ++i) {
        if (classes[i]) {
            classes[i]->print();
        }
    }
}

int Society::totalPopulation() const {
    int total = 0;
    for (int i = 0; i < classCount; ++i) {
        if (classes[i]) {
            total += classes[i]->getPopulation();
        }
    }
    return total;
}

SocialClass* Society::getClassByName(const char* name) {
    for (int i = 0; i < classCount; ++i) {
        if (classes[i] && strcmp(classes[i]->getName(), name) == 0) {
            return classes[i];
        }
    }
    return nullptr;
}

bool Society::unrest() const {
    for (int i = 0; i < classCount; ++i) {
        if (classes[i] && classes[i]->isRevolting()) {
            return true;
        }
    }
    return false;
}

int Society::countRevolters() const {
    int count = 0;
    for (int i = 0; i < classCount; ++i) {
        if (classes[i] && classes[i]->isRevolting()) {
            count += classes[i]->getPopulation();
        }
    }
    return count;
}

void Society::interactAll() {
    for (int i = 0; i < classCount; ++i) {
        for (int j = i + 1; j < classCount; ++j) {
            if (classes[i] && classes[j]) {
                classes[i]->interact(*classes[j]);
                classes[j]->interact(*classes[i]);
            }
        }
    }
}

SocialClass* Society::getClassByIndex(int index) const {
    if (index >= 0 && index < classCount) {
        return classes[index];
    }
    return nullptr;
}

void Society::updateRevoltingPopulation() {
    if (!population) return;
    
    population->resetRevolting();
    
    for (int i = 0; i < classCount; ++i) {
        if (classes[i] && classes[i]->isRevolting()) {
            population->suppressRevolt(-classes[i]->getPopulation());
        }
    }
}

// Relationship Management
void SocialClass::updateRelationship(const char* otherClass, double opinionDelta) {
    // Find existing relationship or create new one
    int index = -1;
    for (int i = 0; i < relationshipCount; ++i) {
        if (relationships[i].targetClassId >= 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1 && relationshipCount < MAX_SOCIAL_CLASSES) {
        index = relationshipCount++;
    }
    
    if (index != -1) {
        int newOpinion = relationships[index].opinion;
        newOpinion += static_cast<int>(opinionDelta * 100);
        newOpinion = std::max(0, std::min(100, newOpinion));
        relationships[index].opinion = newOpinion;
        
        if (opinionDelta < 0) {
            relationships[index].tension += static_cast<int>(std::abs(opinionDelta * 50));
            relationships[index].tension = std::min(100, relationships[index].tension);
        }
    }
}

double SocialClass::getRelationshipStatus(const char* otherClass) const {
    for (int i = 0; i < relationshipCount; ++i) {
        if (strcmp(otherClass, name) == 0) {
            return relationships[i].opinion;
        }
    }
    return 0.5; // Default neutral relationship
}

// Demographics Management
void SocialClass::updateDemographics() {
    // Age progression
    int new_adults = static_cast<int>(demographics.children * 0.1);  // 10% of children become adults
    int new_elderly = static_cast<int>(demographics.working_age * 0.05);  // 5% of adults become elderly
    int deaths = static_cast<int>(demographics.elderly * 0.1);  // 10% of elderly die

    demographics.children = std::max(0, demographics.children - new_adults);
    demographics.working_age = demographics.working_age + new_adults - new_elderly;
    demographics.elderly = demographics.elderly + new_elderly - deaths;

    // Natural population growth if conditions are good
    if (happiness > 0.7 && wealth > LOW_WEALTH_THRESHOLD) {
        int births = static_cast<int>(demographics.working_age * 0.1);  // 10% birth rate
        demographics.children += births;
    }
}

void SocialClass::educatePopulation(double cost) {
    if (wealth >= cost) {
        changeWealth(-static_cast<int>(cost));
        demographics.education_level = std::min(1.0, demographics.education_level + 0.1);
        changeHappiness(0.05);
    }
}

// Economic Interactions
bool SocialClass::transferWealth(SocialClass& other, int amount) {
    if (wealth >= amount) {
        changeWealth(-amount);
        other.changeWealth(amount);
        return true;
    }
    return false;
}

// Revolt System
void SocialClass::organizeRevolt() {
    if (happiness < 0.3 || loyalty < 0.3) {
        revoltInfo.strength = static_cast<int>(population * (1.0 - loyalty) * 0.5);
        revoltInfo.organization = static_cast<int>(demographics.education_level * 100);
        revoltInfo.success_chance = (revoltInfo.strength * revoltInfo.organization) / 10000.0;
        
        revoltInfo.demandCount = 0;
        if (wealth < LOW_WEALTH_THRESHOLD && revoltInfo.demandCount < 3) {
            strncpy_s(revoltInfo.demands[revoltInfo.demandCount++], MAX_NAME, "Wealth redistribution", MAX_NAME - 1);
        }
        if (happiness < 0.3 && revoltInfo.demandCount < 3) {
            strncpy_s(revoltInfo.demands[revoltInfo.demandCount++], MAX_NAME, "Better living conditions", MAX_NAME - 1);
        }
        if (loyalty < 0.3 && revoltInfo.demandCount < 3) {
            strncpy_s(revoltInfo.demands[revoltInfo.demandCount++], MAX_NAME, "Political reforms", MAX_NAME - 1);
        }
    }
}

bool SocialClass::negotiateTerms() {
    if (revoltInfo.demandCount > 0) {
        double random = static_cast<double>(rand()) / RAND_MAX;
        return random < revoltInfo.success_chance;
    }
    return false;
}

// Resource Management
void SocialClass::allocateResources() {
    if (wealth > 0) {
        int allocation = wealth / 3;
        resources.food_stores += allocation;
        resources.housing += allocation;
        resources.military_equipment += allocation;
        changeWealth(-allocation * 3);
    }
}

void SocialClass::distributeResources() {
    if (resources.food_stores > population) {
        changeHappiness(0.1);
        resources.food_stores -= population;
    } else {
        changeHappiness(-0.1);
    }
}

// Culture System
void SocialClass::developCulture(double investment) {
    if (wealth >= investment) {
        changeWealth(-static_cast<int>(investment));
        culture.artistic_development = std::min(1.0, culture.artistic_development + 0.1);
        culture.technological_progress = std::min(1.0, culture.technological_progress + 0.05);
        changeHappiness(0.05);
    }
}

void SocialClass::spreadInfluence() {
    // Influence spreads based on cultural development
    double influence = (culture.artistic_development + culture.technological_progress) / 2.0;
    changeHappiness(influence * 0.1);
    changeLoyalty(influence * 0.05);
}

// History System
void SocialClass::recordHistory(History& history) {
    if (history.historyCount < MAX_HISTORY) {
        history.population_history[history.historyCount] = population;
        history.wealth_history[history.historyCount] = wealth;
        if (revolting) {
            char event[MAX_NAME];
            snprintf(event, MAX_NAME, "Revolt started in %s", name);
            strncpy_s(history.significant_events[history.historyCount], MAX_NAME, event, MAX_NAME - 1);
        }
        history.historyCount++;
    }
}

void SocialClass::analyzeHistoricalTrends(History history) {
    if (history.historyCount > 10) {
        int pop_change = history.population_history[history.historyCount - 1] - 
                        history.population_history[history.historyCount - 10];
        if (pop_change < 0) {
            std::cout << name << " population is declining: " << pop_change << " over last 10 turns\n";
        }
    }
}

// Event System
void SocialClass::handleEvent(const Event& event) {
    // Simple event handling
    switch (event.type) {
        case EventType::ECONOMIC:
            changeWealth(static_cast<int>(event.severity * 100));
            break;
        case EventType::SOCIAL:
            changeHappiness(event.severity * 0.1);
            break;
        case EventType::MILITARY:
            changeLoyalty(event.severity * 0.1);
            break;
        case EventType::NATURAL:
            changePopulation(-static_cast<int>(population * event.severity * 0.1));
            break;
        case EventType::POLITICAL:
            changeLoyalty(event.severity * 0.2);
            break;
    }
}

// Unique class implementations
void Peasant::farm(ResourceData& res) {
    int production = static_cast<int>(demographics.working_age * (1.0 + demographics.education_level));
    res.food += production;
    if (res.food > res.storage) {
        res.food = res.storage;
    }
    changeWealth(production / 2);
    changeHappiness(0.05);
}

void Noble::collectRent(Peasant* peasants[], int peasantCount) {
    for (int i = 0; i < peasantCount; ++i) {
        if (peasants[i]) {
            int rent = static_cast<int>(peasants[i]->getWealth() * 0.1);
            if (peasants[i]->transferWealth(*this, rent)) {
                updateRelationship(peasants[i]->getName(), -0.05);
            }
        }
    }
}

void Merchant::trade(ResourceData& res) {
    int profit = static_cast<int>((res.food + res.iron) * 0.2);
    changeWealth(profit);
    res.food = static_cast<int>(res.food * 0.8);
    changeHappiness(0.1);
}

void Clergy::preach(Society& society) {
    for (int i = 0; i < society.getClassCount(); ++i) {
        SocialClass* other = society.getClassByIndex(i);
        if (other && strcmp(other->getName(), getName()) != 0) {
            other->changeLoyalty(0.05);
            updateRelationship(other->getName(), 0.1);
        }
    }
}

// Society implementation
void Society::handleSocialMobility() {
    for (int i = 0; i < classCount; ++i) {
        SocialClass* current = classes[i];
        if (current && dynamic_cast<Peasant*>(current) != nullptr) {
            if (current->getWealth() > HIGH_WEALTH_THRESHOLD) {
                int mobile_pop = static_cast<int>(current->getPopulation() * 0.1);
                if (mobile_pop > 0 && classCount < MAX_SOCIAL_CLASSES) {
                    classes[classCount++] = new Merchant(mobile_pop, population);
                    current->changePopulation(-mobile_pop);
                }
            }
        }
    }
}

// Add these getter implementations
const Demographics& SocialClass::getDemographics() const {
    return demographics;
}

const RevoltData& SocialClass::getRevoltInfo() const {
    return revoltInfo;
}

const ResourceData& SocialClass::getResources() const {
    return resources;
}

const Culture& SocialClass::getCulture() const {
    return culture;
}