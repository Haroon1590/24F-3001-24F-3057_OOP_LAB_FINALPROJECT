#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

// Constants
const int MAX_NAME = 64;
const int MAX_SOCIAL_CLASSES = 4;
const int MAX_EVENTS = 100;
const int MAX_EVENT_POOL = 1000;
const int MAX_TRADE_OFFERS = 10;
const int MAX_LOGS = 1000;
const int MAX_HISTORY = 100;
const int MAX_EVENT_DESC = 128;
// Game balance constants
const double REVOLT_HAPPINESS_THRESHOLD = 0.3;
const double WARNING_HAPPINESS_THRESHOLD = 0.5;
const double BASE_WEALTH_RATIO_PENALTY = 0.3;
const double HIGH_WEALTH_RATIO_BONUS = 1.5;
const double TAX_HAPPINESS_MULTIPLIER = 0.1;
const double TAX_LOYALTY_MULTIPLIER = 0.05;
const int BASE_WEALTH = 1000;
const int LOW_WEALTH_THRESHOLD = 500;
const int HIGH_WEALTH_THRESHOLD = 5000;
const int MIN_WEALTH_REVOLT = 300;
const int MIN_TAX_RATE = 0;
const int MAX_TAX_RATE = 50;
const double WEALTH_TAX_MULTIPLIER = 1.5;

// Forward declarations (only declare once)
class Society;
class Population;
class Army;
class Economy;
class Bank;
class Leader;
class EventHistory;
class TradeSystem;
class Diplomacy;
class AIKingdom;
class AnalyticsLogger;
class FileManager;
class StorageInt;
class SocialClass;
class Peasant;
class Noble;
class Merchant;
class Clergy;
class EventsManager;
class History;
// Basic structures
enum class EventType {
    ECONOMIC, SOCIAL, MILITARY, NATURAL, POLITICAL
};

struct Demographics {
    int working_age;
    int children;
    int elderly;
    double education_level;
    
    Demographics() : working_age(0), children(0), elderly(0), education_level(0.0) {}
};

struct Culture {
    double artistic_development;
    double technological_progress;
    
    Culture() : artistic_development(0.0), technological_progress(0.0) {}
};


class RevoltData {
    int strength;
    int organization;
    char demands[MAX_NAME] = {};
    int demandCount;
    double success_chance;
public:
    RevoltData() : strength(0), organization(0), demandCount(0), success_chance(0.0) {
        std::memset(demands, 0, sizeof(demands));
    }
};

struct ResourceData {
    int food_stores;
    int housing;
    int military_equipment;
    int food;
    int wood;
    int stone;
    int iron;
    int storage;
    
    ResourceData() : food_stores(0), housing(0), military_equipment(0),
                    food(0), wood(0), stone(0), iron(0), storage(100) {}
};

struct Event {
    char description[MAX_NAME];
    int severity;
    bool trigger;
    EventType type;
    
    Event() : severity(0), trigger(false), type(EventType::SOCIAL) {
        description[0] = '\0';
    }
};

struct Relationship {
    int targetClassId;
    int opinion;
    int tension;
    
    Relationship() : targetClassId(-1), opinion(50), tension(0) {}
    Relationship(int id, int op) : targetClassId(id), opinion(op), tension(0) {}
};



class History {
public:
    int population_history[MAX_HISTORY];
    int wealth_history[MAX_HISTORY];
    char significant_events[MAX_HISTORY][MAX_NAME];
    int historyCount;

    History() : historyCount(0) {
        std::memset(population_history, 0, sizeof(population_history));
        std::memset(wealth_history, 0, sizeof(wealth_history));
        for (int i = 0; i < MAX_HISTORY; ++i)
            significant_events[i][0] = '\0';
    }
};

// Population class definition
class Population {
private:
    int total, sick, revolting;
    double growthRate, mortality;
    int housing;
    bool famine, plague;
public:
    Population(int t);
    void update(int food);
    void print() const;
    int getTotal() const;
    void grow(int n);
    void shrink(int n);
    void buildHousing(int n);
    void setFamine(bool b);
    void setPlague(bool b);
    int getHousing() const;
    int getSick() const;
    int getRevolting() const;
    void cureDisease(int n);
    void suppressRevolt(int n);
    void resetRevolting();
};

// Base class for social classes
class SocialClass {
protected:
    char name[MAX_NAME];
    int population;
    double happiness;
    double loyalty;
    int wealth;
    int initialWealth;
    bool revolting;
    int deathRate;
    Population* popManager;
    Relationship relationships[MAX_SOCIAL_CLASSES];
    int relationshipCount;
    ResourceData resources;
    Demographics demographics;
    RevoltData revoltInfo;
    Culture culture;
    History* history;

public:
    SocialClass(const char* n, int pop, Population* popMgr = nullptr);
    virtual ~SocialClass() {}
    
    // Core functionality
    virtual void update();
    virtual void interact(SocialClass& other) = 0;
    virtual void print() const;

    // Getters
    const char* getName() const;
    int getPopulation() const;
    double getHappiness() const;
    double getLoyalty() const;
    bool isRevolting() const;
    int getWealth() const;
    int getInitialWealth() const { return initialWealth; }
    int getDeathRate() const;
    const Demographics& getDemographics() const;
    const RevoltData& getRevoltInfo() const;
    const ResourceData& getResources() const;
    const Culture& getCulture() const { return culture; }

    // Setters and modifiers
    void changePopulation(int delta);
    void changeHappiness(double delta);
    void changeLoyalty(double delta);
    void changeWealth(int delta);
    void updateDeathRate();
    void checkRevolt();
    int payTaxes(int taxRate);
    void updateDemographics();
    void educatePopulation(double cost);
    bool transferWealth(SocialClass& other, int amount);
    void organizeRevolt();
    bool negotiateTerms();
    void allocateResources();
    void distributeResources();
    void developCulture(double investment);
    void spreadInfluence();
    void recordHistory(History& history);
    void analyzeHistoricalTrends(History history);
    void updateRelationship(const char* otherClass, double opinionDelta);
    double getRelationshipStatus(const char* otherClass) const;
    void handleEvent(const Event& event);
};

// Society class definition
class Society {
private:
    SocialClass* classes[MAX_SOCIAL_CLASSES];
    int classCount;
    Population* population;

public:
    Society(Population* pop);
    ~Society();
    void addClass(SocialClass* c);
    void update();
    void print() const;
    int totalPopulation() const;
    SocialClass* getClassByName(const char* name);
    bool unrest() const;
    int countRevolters() const;
    void interactAll();
    int getClassCount() const { return classCount; }
    SocialClass* getClassByIndex(int index) const;
    void updateRevoltingPopulation();
    void handleSocialMobility();
};

// Derived social classes
class Peasant : public SocialClass {
public:
    Peasant(int pop, Population* popMgr);
    void update() override;
    void interact(SocialClass& other) override;
    void farm(ResourceData& resources);
};

class Noble : public SocialClass {
public:
    Noble(int pop, Population* popMgr);
    void update() override;
    void interact(SocialClass& other) override;
    void collectRent(Peasant* peasants[], int peasantCount);
};

class Merchant : public SocialClass {
public:
    Merchant(int pop, Population* popMgr);
    void update() override;
    void interact(SocialClass& other) override;
    void trade(ResourceData& resources);
};

class Clergy : public SocialClass {
public:
    Clergy(int pop, Population* popMgr);
    void update() override;
    void interact(SocialClass& other) override;
    void preach(Society& society);
};
class Army {
private:
    int soldiers;
    int morale;
    int food;
    int corruption;
    int training;
    int archers;
    int cavalry;

public:
    Army(int s = 0);

    void feed(int amt);
    void train(int n);
    void update();
    void print() const;

    int getStrength() const;
    int getSoldiers() const;
    int getMorale() const;
    int getCorruption() const;

    void pay(int gold);
    void battle(int enemyStrength);
    void sufferCasualties(int n);

    void conscript(int n);
    void recruitArchers(int n);
    void recruitCavalry(int n);

    int getArchers() const;
    int getCavalry() const;
};
class Leader {
private:
    char name[MAX_NAME];
    char type[MAX_NAME];
    int legitimacy;
    int popularity;
    int reignTurns;

public:
    Leader(const char* n, const char* t);

    void update(int gold, int morale, int unrest, int pop);
    void coup();
    void election();
    void print() const;

    const char* getName() const;
    int getLegitimacy() const;
    int getPopularity() const;
    const char* getType() const;
    int getReignTurns() const;

    void boostLegitimacy(int n);
    void changePopularity(int n);
};
class Economy {
private:
    int gold;
    int taxRate;
    int inflation;
    int debt;
    ResourceData resources;

public:
    Economy(int g = 0);

    void tax(Society& soc);
    void spend(int amt);
    void war();
    void peace();
    void print() const;

    int getGold() const;
    void setTax(int r);
    int getTaxRate() const;
    int getInflation() const;
    int getDebt() const;
    void borrow(int amt);
    void repay(int amt);
    void addGold(int g);
    void setInflation(int i);
    void setDebt(int d);

    double calculateWealthRatio(SocialClass* sc, double totalWealth) const;
    int calculateDynamicTaxRate(double wealthRatio) const;
    ResourceData& getResources(); 
};
class Resources {
private:
    int food, wood, stone, iron;
    int farmProd, woodProd, stoneProd, ironProd;
    int foodStore, woodStore, stoneStore, ironStore;

public:
    Resources(int f = 0, int w = 0, int s = 0, int i = 0);

    void gather(const char* type, int amt);
    void consume(const char* type, int amt);
    int getFood() const;
    int getWood() const;
    int getStone() const;
    int getIron() const;
    void print() const;
    void build(const char* type, int amt);
    void update(int pop);
    void expandStorage(const char* type, int amt);
    void trade(const char* resource, int amount, Economy& eco);
};
class Bank {
private:
    int treasury;
    int loans;
    int fraud;
    int reputation;

public:
    Bank(int t = 0);

    void giveLoan(int amt, Economy& eco);
    void audit();
    void scam(int amt);
    void print() const;

    int getTreasury() const;
    int getFraud() const;
    int getReputation() const;

    void adjustReputation(int n);
    void receiveRepayment(int amt);
    void deposit(int amt);
    void withdraw(int amt, Economy& eco);
};
struct TradeOffer {
    char resource[MAX_NAME];
    int price;
};

class TradeSystem {
private:
    static const int MAX_OFFERS = 4;
    TradeOffer tradeOffers[MAX_OFFERS];
    int offerCount;

public:
    TradeSystem();

    void updateOffers();
    void trade(const char* resource, int amount, Resources& res, Economy& eco);
    void print() const;
};
class Diplomacy {
private:
    char status[MAX_NAME];
    int relation;

public:
    Diplomacy();

    void declareWar();
    void formAlliance();
    void makePeace();
    void update(Army& army, Leader& leader);
    void print() const;
    const char* getStatus() const;
    int getRelation() const;
};
class AIKingdom {
private:
    Economy eco;
    Army army;
    Resources res;
    Leader leader;
    int taxRate;

public:
    AIKingdom();

    void makeDecisions(Society& playerSoc, Population& playerPop, Army& playerArmy, Economy& playerEco, Resources& playerRes);
    void print() const;
};
struct AnalyticsLogEntry {
    int turn;
    int population;
    int gold;
    int soldiers;
    int food;
};

class AnalyticsLogger {
private:
    AnalyticsLogEntry logs[MAX_LOGS];
    int logCount;

public:
    AnalyticsLogger();

    void log(int turn, int pop, int gold, int soldiers, int food);
    void save(const char* filename) const;
    void printReport() const;
};
class EventNode {
public:
    Event event;
    int turn;
    EventNode* next;

    EventNode(const Event& e, int t);
};

class EventHistory {
private:
    EventNode* head;
    int size;

public:
    EventHistory();
    ~EventHistory();

    void addEvent(const Event& e, int turn);
    void print() const;
    void save(std::ofstream& out) const;
    void load(std::ifstream& in);
};

class EventManager {
private:
    Event events[MAX_EVENTS];
    int eventCount;
    int poolCount; // (not strictly needed per your code, but matches your variable list)

public:
    EventManager();

    void addEvent(const char* desc, int sev);

    void triggerAll(Society& soc, Population& pop, Army& army,
        Economy& eco, Resources& res, Leader& l);

    void triggerRandom(Society& soc, Population& pop, Army& army,
        Economy& eco, Resources& res, Leader& l);

    const Event* getPool(int& outCount) const;
};
class FileManager {
public:
    void saveGame(const char* fn, Economy& eco, Society& soc, Army& a, Resources& r, Leader& leader, EventHistory& history);
    void loadGame(const char* fn, Economy& eco, Society& soc, Army& a, Resources& r, Leader& leader, EventHistory& history);
};
class StorageInt {
private:
    int items[MAX_SOCIAL_CLASSES];
    int count;
public:
    StorageInt();
    void add(int item);
    int& get(int idx);
    int size() const;
};