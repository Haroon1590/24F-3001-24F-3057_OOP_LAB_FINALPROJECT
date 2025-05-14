#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <sstream>
using namespace std;
// Custom exception class for game errors
class GameException {
public:
    GameException(const std::string& msg) : message(msg) {}
    std::string getMessage() const { return message; }
private:
    std::string message;
};

// Forward declarations
class Resource;
class Population;
class Economy;
class Military;
class Leadership;
class Banking;
class EventHandler;
class Kingdom;
class Communication;
class Alliance;
class Market;
class Conflict;
class Map;

// Resource class
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

// Population class
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
    void updateHappinessFromAction(const std::string& action, int impact);
    bool isRevolting() const;
    void handleRevolt();

    void displayPopulation() const;

private:
    int peasants;
    int merchants;
    int nobles;
    int happiness;
};

// Economy class
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

    bool collectTaxes(Population& population);
    void payMilitary(const Military& military);
    void adjustInflation(Population& population);
    void spendTreasury(int amount);
    void addToTreasury(int amount);
    bool hasSufficientFunds(int amount) const;

    void displayEconomy() const;

private:
    int treasury;
    int taxRate;
    int inflationRate;
    static int lastTaxCollectionTurn;
    static int currentTurn;
};

// Military class
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
    int morale;
};

// Leadership class
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
    int diplomacy;
    int militarySkill;
    int economySkill;
    int popularity;
};

// Banking class
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
    int interestRate;
    int corruptionLevel;
};

// EventHandler class
class EventHandler {
public:
    EventHandler();

    void generateRandomEvent(Kingdom& kingdom);
    void triggerFamine(Kingdom& kingdom);
    void triggerDisease(Kingdom& kingdom);
    void triggerWeatherEvent(Kingdom& kingdom);
    void triggerEconomicEvent(Kingdom& kingdom);

private:
    int eventProbability;
};

// Template class for trade items
template <typename T>
class TradeItem {
public:
    TradeItem(const std::string& type, T quantity, int goldCost)
        : itemType(type), quantity(quantity), goldCost(goldCost) {}
    std::string getItemType() const { return itemType; }
    T getQuantity() const { return quantity; }
    int getGoldCost() const { return goldCost; }
private:
    std::string itemType;
    T quantity;
    int goldCost;
};

// Communication class for messaging
class Communication {
public:
    Communication();
    ~Communication();
    void sendMessage(const Kingdom& sender, const Kingdom& receiver, const std::string& message);
    void displayMessages(const Kingdom& kingdom) const;
    void saveChatLog(const std::string& filename) const;

private:
    struct Message {
        std::string sender;
        std::string receiver;
        std::string content;
        time_t timestamp;
        Message(const std::string& s, const std::string& r, const std::string& c)
            : sender(s), receiver(r), content(c), timestamp(time(0)) {}
    };
    Message** messages;
    int messageCount;
    int capacity;
};

// Alliance class for treaties
class Alliance {
public:
    Alliance();
    ~Alliance();
    void formAlliance(const Kingdom& k1, const Kingdom& k2, const std::string& treatyName);
    void breakAlliance(const Kingdom& k1, const Kingdom& k2);
    bool areAllied(const Kingdom& k1, const Kingdom& k2) const;
    void saveTreatyLog(const std::string& filename) const;

private:
    struct Treaty {
        std::string kingdom1;
        std::string kingdom2;
        std::string treatyName;
        time_t timestamp;
        Treaty(const std::string& k1, const std::string& k2, const std::string& tn)
            : kingdom1(k1), kingdom2(k2), treatyName(tn), timestamp(time(0)) {}
    };
    Treaty** treaties;
    int treatyCount;
    int capacity;
};

// Market class for trade and smuggling
class Market {
public:
    Market();
    ~Market();
    void offerTrade(const Kingdom& seller, const TradeItem<int>& item);
    void acceptTrade(Kingdom& buyer, Kingdom& seller, int offerIndex);
    void smuggle(Kingdom& smuggler, Kingdom& receiver, const TradeItem<int>& item);
    void displayMarket() const;
    void saveMarketLog(const std::string& filename) const;

private:
    struct TradeOffer {
        std::string seller;
        TradeItem<int> item;
        TradeOffer(const std::string& s, const TradeItem<int>& i)
            : seller(s), item(i) {}
    };
    TradeOffer** offers;
    int offerCount;
    int capacity;
};

// Conflict class for wars and betrayals
class Conflict {
public:
    Conflict();
    ~Conflict();
    void declareWar(const Kingdom& attacker, const Kingdom& defender);
    void resolveBattle(Kingdom& attacker, Kingdom& defender);
    void betrayAlly(Kingdom& betrayer, Kingdom& betrayed);
    void saveConflictLog(const std::string& filename) const;

private:
    struct War {
        std::string attacker;
        std::string defender;
        time_t timestamp;
        War(const std::string& a, const std::string& d)
            : attacker(a), defender(d), timestamp(time(0)) {}
    };
    War** wars;
    int warCount;
    int capacity;
};

// Map class for positioning
class Map {
public:
    Map(int width, int height);
    ~Map();
    void placeKingdom(const Kingdom& kingdom, int x, int y);
    void moveKingdom(Kingdom& kingdom, int x, int y);
    void displayMap() const;
    int getMapX(const Kingdom& kingdom) const;
    int getMapY(const Kingdom& kingdom) const;

private:
    struct Position {
        std::string kingdomName;
        int x, y;
        Position(const std::string& name, int x, int y)
            : kingdomName(name), x(x), y(y) {}
    };
    Position** positions;
    int positionCount;
    int capacity;
    int width;
    int height;
};

// Kingdom class
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
    int getMapX() const { return mapX; }
    int getMapY() const { return mapY; }

    void setName(const std::string& name);
    void setMapPosition(int x, int y) { mapX = x; mapY = y; }

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
    int mapX, mapY;
};