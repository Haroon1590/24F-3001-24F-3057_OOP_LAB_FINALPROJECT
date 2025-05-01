#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>

// Forward declarations
class Resource;
class Population;
class Economy;
class Military;
class Leadership;
class Banking;
class EventHandler;
class Kingdom;

// Resource class to handle different types of resources
class Resource {
public:
    Resource();
    Resource(int food, int wood, int stone, int iron);

    int getFood() const;
    int getWood() const;
    int getStone() const;
    int getIron() const;

    void setFood(int food);
    void setWood(int wood);
    void setStone(int stone);
    void setIron(int iron);

    void addFood(int amount);
    void addWood(int amount);
    void addStone(int amount);
    void addIron(int amount);

    bool consumeFood(int amount);
    bool consumeWood(int amount);
    bool consumeStone(int amount);
    bool consumeIron(int amount);

    void displayResources() const;

private:
    int food;
    int wood;
    int stone;
    int iron;
};

// Population class to handle different social classes and their dynamics
class Population {
public:
    Population();
    Population(int peasants, int merchants, int nobles);

    int getPeasants() const;
    int getMerchants() const;
    int getNobles() const;
    int getTotalPopulation() const;
    int getHappiness() const;

    void setPeasants(int peasants);
    void setMerchants(int merchants);
    void setNobles(int nobles);
    void setHappiness(int happiness);

    void updatePopulation(const Resource& resources, int taxRate);
    void increaseHappiness(int amount);
    void decreaseHappiness(int amount);
    bool isRevolting() const;
    void handleRevolt();

    void displayPopulation() const;

private:
    int peasants;
    int merchants;
    int nobles;
    int happiness; // 0-100 scale
};

// Economy class to handle taxes, inflation, and economic management
class Economy {
public:
    Economy();
    Economy(int treasury, int taxRate);

    int getTreasury() const;
    int getTaxRate() const;
    int getInflationRate() const;

    void setTreasury(int treasury);
    void setTaxRate(int taxRate);
    void setInflationRate(int rate);

    void collectTaxes(const Population& population);
    void payMilitary(const Military& military);
    void adjustInflation();
    void spendTreasury(int amount);
    void addToTreasury(int amount);
    bool hasSufficientFunds(int amount) const;

    void displayEconomy() const;

private:
    int treasury;
    int taxRate; // Percentage (0-100)
    int inflationRate; // Percentage (0-100)
};

// Military class to handle army, training, and warfare
class Military {
public:
    Military();
    Military(int infantry, int archers, int cavalry);

    int getInfantry() const;
    int getArchers() const;
    int getCavalry() const;
    int getTotalForces() const;
    int getMorale() const;

    void setInfantry(int infantry);
    void setArchers(int archers);
    void setCavalry(int cavalry);
    void setMorale(int morale);

    bool recruitSoldiers(Population& population, Resource& resources, int count, int type);
    void trainTroops(Resource& resources);
    void updateMorale(const Economy& economy, const Population& population);
    int calculateMilitaryStrength() const;

    void displayMilitary() const;

private:
    int infantry;
    int archers;
    int cavalry;
    int morale; // 0-100 scale
};

// Leadership class to handle rulers and their behaviors
class Leadership {
public:
    Leadership();
    Leadership(const std::string& name, int diplomacy, int military, int economy);

    std::string getName() const;
    int getDiplomacy() const;
    int getMilitarySkill() const;
    int getEconomySkill() const;
    int getPopularity() const;

    void setName(const std::string& name);
    void setDiplomacy(int diplomacy);
    void setMilitarySkill(int military);
    void setEconomySkill(int economy);
    void setPopularity(int popularity);

    void makeDecision(Economy& economy, Military& military);
    void handleCoup(const Population& population, const Military& military);
    void updatePopularity(const Economy& economy, const Population& population);

    void displayLeadership() const;

private:
    std::string name;
    int diplomacy; // 0-100 scale
    int militarySkill; // 0-100 scale
    int economySkill; // 0-100 scale
    int popularity; // 0-100 scale
};

// Banking class to handle loans, interest, and financial operations
class Banking {
public:
    Banking();
    Banking(int loanAmount, int interestRate);

    int getLoanAmount() const;
    int getInterestRate() const;
    int getCorruptionLevel() const;

    void setLoanAmount(int amount);
    void setInterestRate(int rate);
    void setCorruptionLevel(int level);

    void takeLoan(Economy& economy, int amount);
    void repayLoan(Economy& economy, int amount);
    void calculateInterest();
    void conductAudit(Economy& economy);

    void displayBanking() const;

private:
    int loanAmount;
    int interestRate; // Percentage (0-100)
    int corruptionLevel; // 0-100 scale
};

// EventHandler class to manage random events
class EventHandler {
public:
    EventHandler();

    void generateRandomEvent(Kingdom& kingdom);
    void triggerFamine(Kingdom& kingdom);
    void triggerDisease(Kingdom& kingdom);
    void triggerWeatherEvent(Kingdom& kingdom);
    void triggerEconomicEvent(Kingdom& kingdom);

private:
    int eventProbability; // Chance of event occurring (0-100)
};

// Kingdom class to manage all systems together
class Kingdom {
public:
    Kingdom();
    Kingdom(const std::string& name);

    std::string getName() const;
    Resource& getResources();
    Population& getPopulation();
    Economy& getEconomy();
    Military& getMilitary();
    Leadership& getLeadership();
    Banking& getBanking();

    void setName(const std::string& name);

    void simulateTurn();
    void gatherResources();
    void displayKingdomStatus() const;

    bool saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename);
    void logScore(const std::string& filename) const;

private:
    std::string name;
    Resource resources;
    Population population;
    Economy economy;
    Military military;
    Leadership leadership;
    Banking banking;
    EventHandler eventHandler;
    int turnNumber;
};