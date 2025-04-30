//#include "Stronghold.h"
//#include<string>
//#include <iostream>
//using namespace std;
//
//SocialClass::SocialClass(const char* n, int pop): population(pop), happiness(0.5), loyalty(0.5), wealth(10), revolting(false) {
//    strncpy_s(name, n, MAX_NAME - 1);
//    name[MAX_NAME - 1] = 0;
//}
//void SocialClass::print() const {
//    cout << name << " | Pop: " << population << " | Happy: " << happiness<< " | Loyal: " << loyalty << " | Wealth: " << wealth;
//    if (revolting) 
//        cout << " | REVOLTING!";
//    cout << endl;
//}
//const char* SocialClass::getName() const {
//    return name;
//}
//int SocialClass::getPopulation() const {
//    return population;
//}
//void SocialClass::changePopulation(int delta) {
//    population += delta;
//    if (population < 0) population = 0; 
//}
//void SocialClass::changeHappiness(double delta) { 
//    happiness += delta;
//    if (happiness > 1) happiness = 1;
//    if (happiness < 0) happiness = 0;
//}
//void SocialClass::changeLoyalty(double delta) {
//    loyalty += delta;
//    if (loyalty > 1) loyalty = 1; 
//    if (loyalty < 0) loyalty = 0;
//}
//bool SocialClass::isRevolting() const { 
//    return revolting;
//}
//void SocialClass::checkRevolt() { 
//    revolting = (happiness < 0.2 && loyalty < 0.3);
//}
//int SocialClass::getWealth() const {
//    return wealth;
//}
//void SocialClass::changeWealth(int delta) { 
//    wealth += delta; 
//    if (wealth < 0) wealth = 0;
//}
//double SocialClass::getHappiness() const {
//    return happiness;
//}
//
//Peasant::Peasant(int pop) : SocialClass("Peasant", pop) {}
//void Peasant::update() {
//    if (happiness > 0.5 && wealth > 5)
//        changePopulation(int(population * 0.05));
//    else if (happiness < 0.3 || wealth < 3)
//        changePopulation(-int(population * 0.03));
//    if (happiness < 0.3)
//        changeLoyalty(-0.05);
//    checkRevolt();
//}
//void Peasant::interact(SocialClass& other) {
//    if (strcmp(other.getName(), "Noble") == 0) { 
//        changeHappiness(-0.02); changeLoyalty(-0.01);
//    }
//    else if (strcmp(other.getName(), "Merchant") == 0) {
//        changeHappiness(0.01);
//    }
//}
//
//Noble::Noble(int pop) : SocialClass("Noble", pop) {}
//void Noble::update() {
//    if (happiness > 0.8 && wealth > 30) 
//        changePopulation(1);
//    else if (happiness < 0.3) 
//        changePopulation(-1);
//    checkRevolt();
//}
//void Noble::interact(SocialClass& other) {
//    if (strcmp(other.getName(), "Peasant") == 0) { changeHappiness(0.01); }
//    else if (strcmp(other.getName(), "Merchant") == 0) { changeHappiness(0.02); }
//}
//
//Merchant::Merchant(int pop) : SocialClass("Merchant", pop) {}
//void Merchant::update() {
//    if (happiness > 0.4 && wealth > 10) changePopulation(int(population * 0.03));
//    else if (happiness < 0.2 || wealth < 5) changePopulation(-int(population * 0.02));
//    checkRevolt();
//}
//void Merchant::interact(SocialClass& other) {
//    if (strcmp(other.getName(), "Peasant") == 0) { changeHappiness(0.01); }
//    else if (strcmp(other.getName(), "Noble") == 0) { changeHappiness(0.03); }
//}
//
//Clergy::Clergy(int pop) : SocialClass("Clergy", pop) {}
//void Clergy::update() {
//    if (happiness > 0.5) 
//        changePopulation(1);
//    else if (happiness < 0.3) 
//        changePopulation(-1);
//    checkRevolt();
//}
//void Clergy::interact(SocialClass& other) {
//    if (strcmp(other.getName(), "Peasant") == 0 || strcmp(other.getName(), "Noble") == 0) { 
//        other.changeHappiness(0.01);
//    }
//}
//
//Society::Society() : classCount(0) {
//    for (int i = 0; i < MAX_SOCIAL_CLASSES; ++i) classes[i] = 0;
//}
//Society::~Society() {
//    for (int i = 0; i < classCount; ++i) {
//        delete classes[i];
//    }
//}
//void Society::addClass(SocialClass* c) {
//    if (classCount < MAX_SOCIAL_CLASSES)
//        classes[classCount++] = c;
//}
//void Society::update() {
//    interactAll();
//    for (int i = 0; i < classCount; ++i)
//        classes[i]->update();
//}
//void Society::print() const {
//    cout << "\n=== Social Structure ===\n";
//    for (int i = 0; i < classCount; ++i) 
//        classes[i]->print();
//}
//int Society::totalPopulation() const {
//    int total = 0;
//    for (int i = 0; i < classCount; ++i)
//        total += classes[i]->getPopulation();
//    return total;
//}
//SocialClass* Society::getClassByName(const char* name_) {
//    for (int i = 0; i < classCount; ++i)
//        if (strcmp(classes[i]->getName(), name_) == 0)
//            return classes[i];
//    return 0;
//}
//bool Society::unrest() const {
//    for (int i = 0; i < classCount; ++i) 
//        if (classes[i]->isRevolting())
//            return true;
//    return false;
//}
//int Society::countRevolters() const {
//    int n = 0;
//    for (int i = 0; i < classCount; ++i)
//        if (classes[i]->isRevolting())
//            n += classes[i]->getPopulation();
//    return n;
//}
//void Society::interactAll() {
//    for (int i = 0; i < classCount; ++i)
//        for (int j = i + 1; j < classCount; ++j) {
//            classes[i]->interact(*classes[j]); classes[j]->interact(*classes[i]);
//        }
//}
//
// 
//Population::Population(int t) : total(t), sick(0), revolting(0), growthRate(0.05), mortality(0.02), housing(t), famine(false), plague(false) {}
//void Population::update(int food) {
//    if (!famine && !plague && housing >= total) 
//        total += int(total * growthRate);
//    int dead = int(total * mortality);
//    total -= dead;
//    if (famine) { 
//        total -= total / 20; 
//        revolting += total / 50;
//    }
//    if (plague) {
//        sick += total / 10; total -= sick / 5;
//    }
//    if (total > housing) 
//        revolting += (total - housing) / 10;
//    if (total < 0) 
//        total = 0;
//    if (revolting > total)
//        revolting = total;
//}
//void Population::print() const {
//    cout << "\n=== Population ===\nTotal: " << total << " Sick: " << sick << " Revolting: " << revolting << " Housing: " << housing << endl;
//    if (famine) 
//        cout << "!!! FAMINE !!!\n";
//    if (plague) 
//        cout << "!!! PLAGUE !!!\n";
//}
//int Population::getTotal() const {
//    return total;
//}
//void Population::grow(int n) {
//    total += n;
//}
//void Population::shrink(int n) { 
//    total -= n; 
//    if (total < 0)
//        total = 0;
//}
//void Population::buildHousing(int n) { 
//    housing += n;
//}
//void Population::setFamine(bool b) {
//    famine = b;
//}
//void Population::setPlague(bool b) {
//    plague = b;
//}
//int Population::getHousing() const {
//    return housing;
//}
//int Population::getSick() const {
//    return sick;
//}
//int Population::getRevolting() const {
//    return revolting;
//}
//void Population::cureDisease(int n) { 
//    sick -= n;
//    if (sick < 0)
//        sick = 0;
//}
//void Population::suppressRevolt(int n) {
//    revolting -= n; 
//    if (revolting < 0)
//        revolting = 0; 
//}
//
//Army::Army(int s) : soldiers(s), morale(50), food(100), corruption(0), training(1), archers(0), cavalry(0) {}
//void Army::feed(int amt) {
//    food += amt; 
//}
//void Army::train(int n) {
//    soldiers += n;
//    morale += 2; 
//    training++; 
//}
//void Army::update() {
//    if (food < soldiers / 2)
//        morale -= 5;
//    if (morale < 0) morale = 0;
//}
//void Army::print() const {
//    cout << "\n=== Army ===\nSoldiers: " << soldiers << " Archers: " << archers << " Cavalry: " << cavalry
//        << " Morale: " << morale << " Food: " << food << " Corruption: " << corruption << " Training: " << training <<endl;
//}
//int Army::getStrength() const {
//    return (soldiers + archers * 2 + cavalry * 3) * morale * training / 60; 
//}
//int Army::getSoldiers() const {
//    return soldiers;
//}
//int Army::getMorale() const { 
//    return morale;
//}
//int Army::getCorruption() const { 
//    return corruption;
//}
//void Army::pay(int gold) {
//    morale += gold / 10;
//    if (morale > 100) morale = 100;
//    corruption -= gold / 100;
//    if (corruption < 0) corruption = 0;
//}
//void Army::battle(int enemyStrength) {
//    int myStrength = getStrength();
//    cout << "\nBATTLE! Your strength: " << myStrength << " Enemy: " << enemyStrength << endl;
//    if (myStrength > enemyStrength) {
//        cout << "VICTORY!\n";
//        morale += 10;
//        sufferCasualties(enemyStrength / 10);
//    }
//    else {
//        cout << "DEFEAT!\n";
//        morale -= 20;
//        sufferCasualties(soldiers / 3);
//    }
//    if (morale < 0)
//        morale = 0;
//    if (morale > 100) 
//        morale = 100;
//}
//void Army::sufferCasualties(int n) {
//    soldiers -= n / 2;
//    archers -= n / 4;
//    cavalry -= n / 4;
//    if (soldiers < 0)
//        soldiers = 0;
//    if (archers < 0)
//        archers = 0;
//    if (cavalry < 0)
//        cavalry = 0;
//}
//void Army::conscript(int n) { 
//    soldiers += n;
//}
//void Army::recruitArchers(int n) { 
//    archers += n;
//}
//void Army::recruitCavalry(int n) { 
//    cavalry += n;
//}
//int Army::getArchers() const { 
//    return archers;
//}
//int Army::getCavalry() const { 
//    return cavalry;
//}
//
//Leader::Leader(const char* n, const char* t) : legitimacy(100), popularity(50), reignTurns(0) {
//    strncpy_s(name, n, MAX_NAME - 1); name[MAX_NAME - 1] = 0;
//    strncpy_s(type, t, MAX_NAME - 1); type[MAX_NAME - 1] = 0;
//}
//void Leader::update(int gold, int morale, int unrest, int pop) {
//    if (gold > 1000)
//        popularity += 2;
//    if (morale < 30)
//        legitimacy -= 2;
//    if (unrest > pop / 10) 
//        popularity -= 5;
//    reignTurns++;
//    if (popularity < 0) 
//        popularity = 0;
//    if (legitimacy < 0)
//        legitimacy = 0;
//}
//void Leader::coup() {
//    legitimacy -= 30;
//    popularity -= 20; 
//    strncpy_s(type, "Usurper", MAX_NAME - 1);
//    type[MAX_NAME - 1] = 0; }
//void Leader::election() {
//    legitimacy = 100; 
//    popularity += 20;
//    strncpy_s(type, "Elected", MAX_NAME - 1);
//    type[MAX_NAME - 1] = 0;
//}
//void Leader::print() const {
//    cout << "\n=== Leadership ===\n" << name << " (" << type << ") | Legitimacy: " << legitimacy << " | Popularity: " << popularity << " | Reign: " << reignTurns << " turns\n";
//}
//const char* Leader::getName() const {
//    return name;
//}
//int Leader::getLegitimacy() const { 
//    return legitimacy;
//}
//int Leader::getPopularity() const {
//    return popularity;
//}
//const char* Leader::getType() const {
//    return type; 
//}
//int Leader::getReignTurns() const { 
//    return reignTurns;
//}
//void Leader::boostLegitimacy(int n) { 
//    legitimacy += n;
//    if (legitimacy > 100) legitimacy = 100;
//}
//void Leader::changePopularity(int n) {
//    popularity += n;
//    if (popularity > 100) popularity = 100; 
//    if (popularity < 0) popularity = 0;
//}
//
//Economy::Economy(int g) : gold(g), taxRate(10), inflation(0), debt(0) {}
//void Economy::tax(int pop) {
//    gold += (taxRate * pop) / 10;
//    cout << "Tax collected: " << taxRate << endl;
//}
//void Economy::spend(int amt) {
//    gold -= amt;
//    if (gold < 0) gold = 0; 
//}
//void Economy::war() {
//    inflation += 5;
//}
//void Economy::peace() {
//    if (inflation > 0) 
//        inflation -= 2;
//}
//void Economy::print() const { 
//    cout << "\n=== Economy ===\nGold: " << gold << " Tax: " << taxRate << "% Inflation: " << inflation << " Debt: " << debt << endl;
//}
//int Economy::getGold() const {
//    return gold; }
//void Economy::setTax(int r) { taxRate = r;
//}
//int Economy::getTaxRate() const { 
//    return taxRate; 
//}
//int Economy::getInflation() const { 
//    return inflation;
//}
//int Economy::getDebt() const {
//    return debt;
//}
//void Economy::borrow(int amt) { 
//    debt += amt;
//    gold += amt;
//}
//void Economy::repay(int amt) {
//    if (amt > gold)
//        amt = gold; 
//    if (amt > debt)
//        amt = debt;
//    gold -= amt; 
//    debt -= amt;
//}
//void Economy::addGold(int g) {
//    gold += g;
//}
//
//Bank::Bank(int t) : treasury(t), loans(0), fraud(0), reputation(100) {}
//void Bank::giveLoan(int amt, Economy& eco) { 
//    loans += amt; 
//    treasury -= amt;
//    eco.borrow(amt);
//    reputation -= 2;
//}
//void Bank::audit() {
//    if (fraud > 0) {
//        cout << "Fraud detected: " << fraud << endl; 
//        fraud = 0; reputation -= 10; 
//    } 
//    else { 
//        reputation += 5;
//        if (reputation > 100) 
//            reputation = 100;
//    }
//}
//void Bank::scam(int amt) { 
//    fraud += amt; 
//    reputation -= amt / 10;
//}
//void Bank::print() const {
//    cout << "\n=== Bank ===\nTreasury: " << treasury << " Loans: " << loans << " Fraud: " << fraud << " Reputation: " << reputation << endl;
//}
//int Bank::getTreasury() const {
//    return treasury;
//}
//int Bank::getFraud() const { 
//    return fraud;
//}
//int Bank::getReputation() const {
//    return reputation;
//}
//void Bank::adjustReputation(int n) {
//    reputation += n;
//    if (reputation > 100) reputation = 100;
//    if (reputation < 0) reputation = 0;
//}
//void Bank::receiveRepayment(int amt) {
//    treasury += amt;
//    loans -= amt; 
//    if (loans < 0) loans = 0;
//}
//void Bank::deposit(int amt) { 
//    treasury += amt;
//}
//void Bank::withdraw(int amt, Economy& eco) {
//    if (amt > treasury) amt = treasury;
//    treasury -= amt; eco.addGold(amt);
//}
//
//Resources::Resources(int f, int w, int s, int i) : food(f), wood(w), stone(s), iron(i),
//farmProd(10), woodProd(5), stoneProd(3), ironProd(2), foodStore(500), woodStore(300), stoneStore(200), ironStore(100) {}
//void Resources::gather(const char* type, int amt) {
//    if (strcmp(type, "food") == 0) {
//        food += amt;
//        if (food > foodStore)
//            food = foodStore; 
//    }
//    else if (strcmp(type, "wood") == 0) { 
//        wood += amt;
//        if (wood > woodStore) 
//            wood = woodStore;
//    }
//    else if (strcmp(type, "stone") == 0) {
//        stone += amt;
//        if (stone > stoneStore)
//            stone = stoneStore;
//    }
//    else if (strcmp(type, "iron") == 0) { 
//        iron += amt;
//        if (iron > ironStore) iron = ironStore;
//    }
//    else
//        cout << "Unknown resource.\n";
//}
//void Resources::consume(const char* type, int amt) {
//    if (strcmp(type, "food") == 0) { 
//        food -= amt;
//        if (food < 0) food = 0;
//    }
//    else if (strcmp(type, "wood") == 0) {
//        wood -= amt;
//        if (wood < 0)
//            wood = 0; 
//    }
//    else if (strcmp(type, "stone") == 0) {
//        stone -= amt;
//        if (stone < 0) 
//            stone = 0; 
//    }
//    else if (strcmp(type, "iron") == 0) {
//        iron -= amt;
//        if (iron < 0)
//            iron = 0; 
//    }
//    else
//        cout << "Unknown resource.\n";
//}
//int Resources::getFood() const {
//    return food;
//}
//int Resources::getWood() const {
//    return wood;
//}
//int Resources::getStone() const {
//    return stone;
//}
//int Resources::getIron() const {
//    return iron;
//}
//void Resources::print() const {
//    cout << "\n=== Resources ===\nFood: " << food << "/" << foodStore << " Wood: " << wood << "/" << woodStore
//        << " Stone: " << stone << "/" << stoneStore << " Iron: " << iron << "/" << ironStore << endl;
//}
//void Resources::build(const char* type, int amt) {
//    if (strcmp(type, "farm") == 0) {
//        if (wood >= 10 && stone >= 5) { 
//            farmProd += amt;
//            wood -= 10; 
//            stone -= 5;
//            cout << "Built farm\n"; } else cout << "Need more wood/stone!\n"; 
//    }
//    else if (strcmp(type, "lumber") == 0) { 
//        if (wood >= 5 && stone >= 2) {
//            woodProd += amt; wood -= 5;
//            stone -= 2;
//            cout << "Built lumber mill\n";
//        }
//        else 
//            cout << "Need more wood/stone!\n";
//    }
//    else if (strcmp(type, "mine") == 0) {
//        if (wood >= 5 && stone >= 5) {
//            ironProd += amt;
//            wood -= 5;
//            stone -= 5; 
//            cout << "Built mine\n";
//        } 
//        else
//            cout << "Need more wood/stone!\n"; 
//    }
//    else 
//        cout << "Unknown build!\n";
//}
//void Resources::update(int pop) {
//    food += farmProd - pop / 10;
//    if (food > foodStore) 
//        food = foodStore; 
//    if (food < 0) 
//        food = 0;
//    wood += woodProd;
//    if (wood > woodStore)
//        wood = woodStore;
//    stone += stoneProd;
//    if (stone > stoneStore) 
//        stone = stoneStore;
//    iron += ironProd;
//    if (iron > ironStore)
//        iron = ironStore;
//}
//void Resources::expandStorage(const char* type, int amt) {
//    if (strcmp(type, "food") == 0)
//        foodStore += amt;
//    else if (strcmp(type, "wood") == 0) 
//        woodStore += amt;
//    else if (strcmp(type, "stone") == 0)
//        stoneStore += amt;
//    else if (strcmp(type, "iron") == 0)
//        ironStore += amt;
//    else
//        cout << "Unknown storage type.\n";
//}
//void Resources::trade(const char* resource, int amount, Economy& eco) {
//    if (strcmp(resource, "food") == 0 && food >= amount) {
//        food -= amount;
//        eco.addGold(amount * 2); 
//    }
//    else if (strcmp(resource, "wood") == 0 && wood >= amount) { 
//        wood -= amount;
//        eco.addGold(amount * 1);
//    }
//    else if (strcmp(resource, "stone") == 0 && stone >= amount) {
//        stone -= amount;
//        eco.addGold(amount * 3);
//    }
//    else if (strcmp(resource, "iron") == 0 && iron >= amount) {
//        iron -= amount;
//        eco.addGold(amount * 5);
//    }
//    else 
//        cout << "Insufficient resources for trade!\n";
//}
//
//void Event::trigger(Society& soc, Population& pop, Army& army, Economy& eco, Resources& res, Leader& leader) const {
//    cout << "\nEVENT: " << desc << " (Severity: " << severity << ")\n";
//    if (strcmp(desc, "Famine") == 0) {
//        res.consume("food", 50 * severity);
//        pop.shrink(10 * severity);
//    }
//    else if (strcmp(desc, "Bandit Attack") == 0) { 
//        eco.spend(30 * severity);
//        leader.changePopularity(-5 * severity);
//    }
//    else if (strcmp(desc, "Plague") == 0) {
//        pop.shrink(15 * severity);
//        pop.setPlague(true); 
//    }
//    else if (strcmp(desc, "Noble Revolt") == 0) {
//        leader.coup();
//        SocialClass* noble = soc.getClassByName("Noble"); 
//        if (noble) noble->changeLoyalty(-0.2 * severity);
//    }
//    else if (strcmp(desc, "Bountiful Harvest") == 0) { 
//        res.gather("food", 100 * severity);
//        leader.boostLegitimacy(5 * severity);
//    }
//    else if (strcmp(desc, "Foreign Invasion") == 0) {
//        army.battle(army.getStrength() * severity / 2);
//    }
//}
//
//EventManager::EventManager() : eventCount(0), poolCount(6) {
//    strncpy_s(pool[0].desc, "Famine", MAX_EVENT_DESC - 1); 
//    pool[0].desc[MAX_EVENT_DESC - 1] = 0;
//    pool[0].severity = 2;
//    strncpy_s(pool[1].desc, "Bandit Attack", MAX_EVENT_DESC - 1);
//    pool[1].desc[MAX_EVENT_DESC - 1] = 0;
//    pool[1].severity = 1;
//    strncpy_s(pool[2].desc, "Plague", MAX_EVENT_DESC - 1); 
//    pool[2].desc[MAX_EVENT_DESC - 1] = 0; 
//    pool[2].severity = 2;
//    strncpy_s(pool[3].desc, "Noble Revolt", MAX_EVENT_DESC - 1);
//    pool[3].desc[MAX_EVENT_DESC - 1] = 0;
//    pool[3].severity = 2;
//    strncpy_s(pool[4].desc, "Bountiful Harvest", MAX_EVENT_DESC - 1);
//    pool[4].desc[MAX_EVENT_DESC - 1] = 0; 
//    pool[4].severity = 1;
//    strncpy_s(pool[5].desc, "Foreign Invasion", MAX_EVENT_DESC - 1);
//    pool[5].desc[MAX_EVENT_DESC - 1] = 0;
//    pool[5].severity = 3;
//}
//void EventManager::addEvent(const char* d, int s) {
//    if (eventCount >= MAX_EVENTS)
//        return;
//    strncpy_s(events[eventCount].desc, d, MAX_EVENT_DESC - 1);
//    events[eventCount].desc[MAX_EVENT_DESC - 1] = 0;
//    events[eventCount].severity = s;
//    eventCount++;
//}
//void EventManager::triggerAll(Society& soc, Population& pop, Army& army, Economy& eco, Resources& res, Leader& l) {
//    for (int i = 0; i < eventCount; ++i) events[i].trigger(soc, pop, army, eco, res, l);
//    eventCount = 0;
//}
//void EventManager::triggerRandom(Society& soc, Population& pop, Army& army, Economy& eco, Resources& res, Leader& l) {
//    int idx = rand() % poolCount;
//    pool[idx].trigger(soc, pop, army, eco, res, l);
//}
//const Event* EventManager::getPool(int& outCount) const {
//    outCount = poolCount;
//    return pool;
//}
//
//
//EventNode::EventNode(const Event& e, int t) : event(e), turn(t), next(0) {}
//
//EventHistory::EventHistory() : head(0), size(0) {}
//EventHistory::~EventHistory() {
//    EventNode* current = head;
//    while (current) {
//        EventNode* next = current->next;
//        delete current;
//        current = next;
//    }
//}
//void EventHistory::addEvent(const Event& e, int turn) {
//    EventNode* node = new EventNode(e, turn);
//    node->next = head;
//    head = node;
//    size++;
//}
//void EventHistory::print() const {
//    cout << "\n=== Event History ===\n";
//    EventNode* current = head;
//    while (current) {
//        cout << "Turn " << current->turn << ": " << current->event.desc << " (Severity: " << current->event.severity << ")\n";
//        current = current->next;
//    }
//}
//void EventHistory::save(ofstream& out) const {
//    out << size << "\n";
//    EventNode* current = head;
//    while (current) {
//        out << current->turn << " " << current->event.desc << " " << current->event.severity << "\n";
//        current = current->next;
//    }
//}
//void EventHistory::load(ifstream& in) {
//    int count;
//    in >> count;
//    for (int i = 0; i < count; ++i) {
//        int turn, severity;
//        char desc[MAX_EVENT_DESC];
//        in >> turn;
//        in.ignore();
//        in.getline(desc, MAX_EVENT_DESC, ' ');
//        in >> severity;
//        Event e;
//        strncpy_s(e.desc, desc, MAX_EVENT_DESC - 1); e.desc[MAX_EVENT_DESC - 1] = 0;
//        e.severity = severity;
//        addEvent(e, turn);
//    }
//}
//
//TradeSystem::TradeSystem() : offerCount(4) {
//    strncpy_s(tradeOffers[0].resource, "food", MAX_NAME - 1);
//    tradeOffers[0].resource[MAX_NAME - 1] = 0;
//    tradeOffers[0].price = 2;
//    strncpy_s(tradeOffers[1].resource, "wood", MAX_NAME - 1);
//    tradeOffers[1].resource[MAX_NAME - 1] = 0;
//    tradeOffers[1].price = 1;
//    strncpy_s(tradeOffers[2].resource, "stone", MAX_NAME - 1);
//    tradeOffers[2].resource[MAX_NAME - 1] = 0;
//    tradeOffers[2].price = 3;
//    strncpy_s(tradeOffers[3].resource, "iron", MAX_NAME - 1);
//    tradeOffers[3].resource[MAX_NAME - 1] = 0; 
//    tradeOffers[3].price = 5;
//}
//void TradeSystem::updateOffers() {
//    for (int i = 0; i < offerCount; ++i) {
//        int change = rand() % 3 - 1;
//        tradeOffers[i].price += change;
//        if (tradeOffers[i].price < 1) 
//            tradeOffers[i].price = 1;
//    }
//}
//void TradeSystem::trade(const char* resource, int amount, Resources& res, Economy& eco) {
//    int found = -1;
//    for (int i = 0; i < offerCount; ++i) {
//        if (strcmp(tradeOffers[i].resource, resource) == 0) 
//            found = i;
//    }
//    if (found == -1) { 
//        cout << "Invalid trade resource!\n";
//        return; }
//    res.trade(resource, amount, eco);
//    cout << "Traded " << amount << " " << resource << " for " << amount * tradeOffers[found].price << " gold\n";
//}
//void TradeSystem::print() const {
//    cout << "\n=== Trade Offers ===\n";
//    for (int i = 0; i < offerCount; ++i) {
//        cout << tradeOffers[i].resource << ": " << tradeOffers[i].price << " gold/unit\n";
//    }
//}
//
//Diplomacy::Diplomacy() : relation(0) { 
//    strncpy_s(status, "Peace", MAX_NAME - 1);
//    status[MAX_NAME - 1] = 0;
//}
//void Diplomacy::declareWar() { 
//    strncpy_s(status, "War", MAX_NAME - 1); 
//    status[MAX_NAME - 1] = 0;
//    relation -= 50;
//    if (relation < -100)
//        relation = -100;
//}
//void Diplomacy::formAlliance() {
//    strncpy_s(status, "Alliance", MAX_NAME - 1);
//    status[MAX_NAME - 1] = 0;
//    relation += 50;
//    if (relation > 100) 
//        relation = 100;
//}
//void Diplomacy::makePeace() { 
//    strncpy_s(status, "Peace", MAX_NAME - 1); 
//    status[MAX_NAME - 1] = 0; 
//    relation += 20;
//    if (relation > 100)
//        relation = 100;
//}
//void Diplomacy::update(Army& army, Leader& leader) {
//    if (strcmp(status, "War") == 0)
//        army.battle(army.getStrength() / 2);
//    if (strcmp(status, "Alliance") == 0)
//        leader.boostLegitimacy(5);
//}
//void Diplomacy::print() const {
//    cout << "\n=== Diplomacy ===\nStatus: " << status << " Relation: " << relation << endl;
//}
//const char* Diplomacy::getStatus() const {
//    return status;
//}
//int Diplomacy::getRelation() const { 
//    return relation;
//}
//
//
//AIKingdom::AIKingdom() : eco(300), army(15), res(150, 80, 40, 15), leader("AI King", "King"), taxRate(10) {}
//void AIKingdom::makeDecisions(Society& playerSoc, Population& playerPop, Army& playerArmy, Economy& playerEco, Resources& playerRes) {
//    if (eco.getGold() < 200) 
//        taxRate += 2;
//    else if (eco.getGold() > 1000)
//        taxRate -= 2;
//    eco.setTax(taxRate);
//    eco.tax(100);
//    if (army.getStrength() < playerArmy.getStrength()) army.conscript(5);
//    res.gather("food", 20);
//    cout << "\nAI Kingdom acts: Tax rate set to " << taxRate << "%, recruited soldiers, gathered food.\n";
//}
//void AIKingdom::print() const {
//    cout << "\n=== AI Kingdom ===\n";
//    eco.print(); army.print(); res.print(); leader.print();
//}
//AnalyticsLogger::AnalyticsLogger() : logCount(0) {}
//void AnalyticsLogger::log(int turn, int pop, int gold, int soldiers, int food) {
//    if (logCount < MAX_LOGS) {
//        logs[logCount].turn = turn;
//        logs[logCount].population = pop;
//        logs[logCount].gold = gold;
//        logs[logCount].soldiers = soldiers;
//        logs[logCount].food = food;
//        logCount++;
//    }
//}
//void AnalyticsLogger::save(const char* filename) const {
//    ofstream out(filename);
//    if (!out) { 
//        cout << "Cannot open score file.\n";
//        return;
//    }
//    for (int i = 0; i < logCount; ++i) {
//        out << "Turn " << logs[i].turn << ": Pop=" << logs[i].population << ", Gold=" << logs[i].gold
//            << ", Soldiers=" << logs[i].soldiers << ", Food=" << logs[i].food << "\n";
//    }
//    out.close();
//}
//void AnalyticsLogger::printReport() const {
//    cout << "\n=== Analytics Report ===\n";
//    for (int i = 0; i < logCount; ++i) {
//        cout << "Turn " << logs[i].turn << ": Population=" << logs[i].population << ", Gold=" << logs[i].gold
//            << ", Soldiers=" << logs[i].soldiers << ", Food=" << logs[i].food << "\n";
//    }
//}
//void FileManager::saveGame(const char* fn, Economy& eco, Society& soc, Army& a, Resources& r, Leader& leader, EventHistory& history) {
//    std::ofstream out(fn);
//    if (!out) { 
//        cout << "Cannot open save file.\n"; 
//        return;
//    }
//    out << eco.getGold() << " " << soc.totalPopulation() << " " << a.getSoldiers() << " " << a.getArchers() << " "
//        << a.getCavalry() << " " << r.getFood() << " " << r.getWood() << " " << r.getStone() << " " << r.getIron() << " "
//        << leader.getName() << " " << leader.getType() << " " << leader.getLegitimacy() << " " << leader.getPopularity() << "\n";
//    history.save(out);
//    out.close();
//}
//void FileManager::loadGame(const char* fn, Economy& eco, Society& soc, Army& a, Resources& r, Leader& leader, EventHistory& history) {
//    std::ifstream in(fn);
//    if (!in) { cout << "Cannot open load file.\n";
//    return; 
//    }
//    int gold, pop, soldiers, archers, cavalry, food, wood, stone, iron, leg, popu;
//    char lname[MAX_NAME], ltype[MAX_NAME];
//    in >> gold >> pop >> soldiers >> archers >> cavalry >> food >> wood >> stone >> iron >> lname >> ltype >> leg >> popu;
//    eco = Economy(gold);
//    a = Army(soldiers);
//    a.recruitArchers(archers);
//    a.recruitCavalry(cavalry);
//    r = Resources(food, wood, stone, iron);
//    leader = Leader(lname, ltype);
//    history.load(in);
//    in.close();
//}
//
//
//StorageInt::StorageInt() : count(0) {}
//void StorageInt::add(int item) {
//    if (count < MAX_SOCIAL_CLASSES)
//        items[count++] = item;
//    else 
//        cout << "Storage full!\n";
//}
//int& StorageInt::get(int idx) {
//    if (idx < 0 || idx >= count) {
//        cout << "Invalid index!\n"; 
//        static int dummy = -1;
//        return dummy; 
//    } 
//    return items[idx];
//}
//int StorageInt::size() const { 
//    return count;
//}