#include "Stronghold.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

// Forward declarations
void printStats(Society& soc, Population& pop, Army& army, Leader& leader, Economy& eco, 
               Bank& bank, Resources& res, TradeSystem& trade, Diplomacy& diplo, 
               AIKingdom& ai, AnalyticsLogger& logger, EventHistory& history);
void showMenu();
void displayKingdomStatus(const Society& soc, const Population& pop, const Army& army, 
                         const Economy& eco, const Resources& res, const Leader& leader);
void handleTaxCollection(Society& soc, Economy& eco);
void handleResourceDistribution(Society& society);
void handleClassActions(Society& society, Economy& economy);
void handleSocialMobility(Society& society);
void handleCulturalDevelopment(Society& society, Economy& economy);

// ---------- Helper: Main Menu ----------
void printStats(Society& soc, Population& pop, Army& army, Leader& leader, Economy& eco, Bank& bank, Resources& res, TradeSystem& trade, Diplomacy& diplo, AIKingdom& ai, AnalyticsLogger& logger, EventHistory& history) {
    soc.print();
    pop.print();
    army.print();
    leader.print();
    eco.print();
    bank.print();
    res.print();
    trade.print();
    diplo.print();
    ai.print();
    logger.printReport();
    history.print();
    
    // Print death rates for each social class
    cout << "\n=== Death Rates ===\n";
    for (int i = 0; i < soc.getClassCount(); ++i) {
        SocialClass* sc = soc.getClassByIndex(i);
        cout << sc->getName() << " Death Rate: " << sc->getDeathRate() << "%\n";
    }
}

void showMenu() {
    cout << "\n--- STRONGHOLD ---\n"
        << "1. Collect Taxes\n"
        << "2. Recruit Soldiers\n"
        << "3. Train Army\n"
        << "4. Feed Army\n"
        << "5. Gather Resources\n"
        << "6. Build Structures\n"
        << "7. Expand Storage\n"
        << "8. Handle Event\n"
        << "9. Show Status\n"
        << "10. Save Game\n"
        << "11. Load Game\n"
        << "12. Next Turn\n"
        << "13. Trade Resources\n"
        << "14. Manage Diplomacy\n"
        << "15. Recruit Archers\n"
        << "16. Recruit Cavalry\n"
        << "0. Exit\n"
        << "Choose: ";
}

void displayKingdomStatus(const Society& soc, const Population& pop, const Army& army, 
                         const Economy& eco, const Resources& res, const Leader& leader) {
    cout << "\n=== Kingdom Status ===" << endl;
    cout << "Treasury: " << eco.getGold() << endl;
    cout << "\nSocial Classes:" << endl;
    
    for (int i = 0; i < soc.getClassCount(); i++) {
        SocialClass* sc = soc.getClassByIndex(i);
        if (sc) {
            cout << "\n" << sc->getName() << ":" << endl;
            cout << "Population: " << sc->getPopulation() << endl;
            cout << "Demographics:" << endl;
            cout << "  Working Age: " << sc->getDemographics().working_age << endl;
            cout << "  Children: " << sc->getDemographics().children << endl;
            cout << "  Elderly: " << sc->getDemographics().elderly << endl;
            cout << "  Education Level: " << fixed << setprecision(1) 
                 << sc->getDemographics().education_level * 100 << "%" << endl;
            
            cout << "Wealth: " << sc->getWealth() << endl;
            cout << "Happiness: " << fixed << setprecision(1) 
                 << sc->getHappiness() * 100 << "%" << endl;
            cout << "Loyalty: " << fixed << setprecision(1) 
                 << sc->getLoyalty() * 100 << "%" << endl;
            
            // Resource status
            cout << "Resources:" << endl;
            cout << "  Food Stores: " << sc->getResources().food_stores << endl;
            cout << "  Housing: " << sc->getResources().housing << endl;
            cout << "  Military Equipment: " << sc->getResources().military_equipment << endl;
            cout << "  Raw Resources - Food: " << sc->getResources().food 
                 << ", Wood: " << sc->getResources().wood 
                 << ", Stone: " << sc->getResources().stone 
                 << ", Iron: " << sc->getResources().iron << endl;
            
            // Cultural development
            cout << "Cultural Development:" << endl;
            cout << "  Artistic: " << fixed << setprecision(1) 
                 << sc->getCulture().artistic_development * 100 << "%" << endl;
            cout << "  Technological: " << fixed << setprecision(1) 
                 << sc->getCulture().technological_progress * 100 << "%" << endl;
            
            // Relationships
            cout << "Relationships:" << endl;
            for (int j = 0; j < soc.getClassCount(); j++) {
                SocialClass* other = soc.getClassByIndex(j);
                if (other && other != sc) {
                    cout << "  With " << other->getName() << ": " 
                         << fixed << setprecision(1)
                         << sc->getRelationshipStatus(other->getName()) * 100 << "%" << endl;
                }
            }
            
            // Revolt status
            if (sc->isRevolting()) {
                cout << "STATUS: REVOLTING!" << endl;
                cout << "Revolt Demands:" << endl;
                for (const auto& demand : sc->getRevoltInfo().demands) {
                    cout << "  - " << demand << endl;
                }
            }
        }
    }
}

void handleTaxCollection(Society& soc, Economy& eco) {
    cout << "\nCurrent tax rate: " << eco.getTaxRate() * 100 << "%" << endl;
    
    int totalTaxes = 0;
    for (int i = 0; i < soc.getClassCount(); i++) {
        SocialClass* sc = soc.getClassByIndex(i);
        if (sc) {
            cout << "\nCollecting taxes from " << sc->getName() << ":" << endl;
            cout << "Current wealth: " << sc->getWealth() << endl;
            
            int taxes = sc->payTaxes(eco.getTaxRate());
            totalTaxes += taxes;
            
            cout << "Taxes paid: " << taxes << endl;
            cout << "Remaining wealth: " << sc->getWealth() << endl;
            cout << "Happiness: " << fixed << setprecision(1) 
                 << sc->getHappiness() * 100 << "%" << endl;
            cout << "Loyalty: " << fixed << setprecision(1) 
                 << sc->getLoyalty() * 100 << "%" << endl;
            
            if (sc->isRevolting()) {
                cout << "WARNING: " << sc->getName() << " has started revolting!" << endl;
            }
        }
    }
    
    eco.addGold(totalTaxes);
    cout << "\nTotal taxes collected: " << totalTaxes << endl;
    cout << "New treasury balance: " << eco.getGold() << endl;
}

void handleResourceDistribution(Society& society) {
    cout << "\n=== Resource Distribution ===" << endl;
    for (int i = 0; i < society.getClassCount(); i++) {
        SocialClass* sc = society.getClassByIndex(i);
        if (sc) {
            cout << "\nDistributing resources to " << sc->getName() << ":" << endl;
            cout << "Before distribution:" << endl;
            cout << "Food Stores: " << sc->getResources().food_stores << endl;
            cout << "Housing: " << sc->getResources().housing << endl;
            
            sc->distributeResources();
            
            cout << "After distribution:" << endl;
            cout << "Food Stores: " << sc->getResources().food_stores << endl;
            cout << "Housing: " << sc->getResources().housing << endl;
            cout << "Happiness: " << fixed << setprecision(1) 
                 << sc->getHappiness() * 100 << "%" << endl;
        }
    }
}

void handleClassActions(Society& society, Economy& economy) {
    cout << "\n=== Class Actions ===" << endl;
    
    // Peasant farming
    Peasant* peasant = dynamic_cast<Peasant*>(society.getClassByIndex(0));
    if (peasant) {
        cout << "\nPeasants are farming..." << endl;
        ResourceData& res = economy.getResources();
        peasant->farm(res);
        cout << "New food stores: " << res.food << endl;
        cout << "Peasant wealth: " << peasant->getWealth() << endl;
    }
    
    // Noble rent collection
    Noble* noble = dynamic_cast<Noble*>(society.getClassByIndex(1));
    if (noble) {
        cout << "\nNobles are collecting rent..." << endl;
        Peasant* peasants[MAX_SOCIAL_CLASSES] = {nullptr};
        int peasantCount = 0;
        
        // Find all peasants
        for (int i = 0; i < society.getClassCount(); ++i) {
            if (Peasant* p = dynamic_cast<Peasant*>(society.getClassByIndex(i))) {
                peasants[peasantCount++] = p;
            }
        }
        
        noble->collectRent(peasants, peasantCount);
        cout << "Noble wealth: " << noble->getWealth() << endl;
    }
    
    // Merchant trading
    Merchant* merchant = dynamic_cast<Merchant*>(society.getClassByIndex(2));
    if (merchant) {
        cout << "\nMerchants are trading..." << endl;
        ResourceData& res = economy.getResources();
        merchant->trade(res);
        cout << "Merchant wealth: " << merchant->getWealth() << endl;
    }
    
    // Clergy preaching
    Clergy* clergy = dynamic_cast<Clergy*>(society.getClassByIndex(3));
    if (clergy) {
        cout << "\nClergy is preaching..." << endl;
        clergy->preach(society);
        cout << "Updated relationships:" << endl;
        for (int i = 0; i < society.getClassCount(); i++) {
            SocialClass* other = society.getClassByIndex(i);
            if (other != clergy) {
                cout << other->getName() << "'s loyalty: " << fixed << setprecision(1) 
                     << other->getLoyalty() * 100 << "%" << endl;
            }
        }
    }
}

void handleSocialMobility(Society& society) {
    cout << "\n=== Social Mobility ===" << endl;
    society.handleSocialMobility();
    cout << "Social mobility changes have been processed." << endl;
}

void handleCulturalDevelopment(Society& society, Economy& economy) {
    cout << "\n=== Cultural Development ===" << endl;
    int investment = economy.getGold() / 10;  // 10% of treasury
    
    for (int i = 0; i < society.getClassCount(); i++) {
        SocialClass* sc = society.getClassByIndex(i);
        if (sc) {
            cout << "\nInvesting in " << sc->getName() << "'s cultural development..." << endl;
            sc->developCulture(investment);
            cout << "New artistic development: " << fixed << setprecision(1) 
                 << sc->getCulture().artistic_development * 100 << "%" << endl;
            cout << "New technological progress: " << fixed << setprecision(1) 
                 << sc->getCulture().technological_progress * 100 << "%" << endl;
        }
    }
    
    economy.addGold(-investment * society.getClassCount());
}

int main() {
    srand((unsigned int)time(0));
    Population pop(400);
    Society soc(&pop);  // Pass population pointer to Society
    soc.addClass(new Peasant(250, &pop));    // Most numerous
    soc.addClass(new Noble(30, &pop));       // Smallest group
    soc.addClass(new Merchant(70, &pop));    // Medium sized group
    soc.addClass(new Clergy(50, &pop));      // Smaller group
    Army army(20);
    Leader leader("Arthur", "King");
    Economy eco(500);
    Bank bank(250);
    Resources res(200, 100, 50, 20);

    TradeSystem trade;
    Diplomacy diplo;
    AIKingdom ai;
    AnalyticsLogger logger;
    EventHistory history;
    int turn = 1;
    bool running = true;
    
    while (running) {
        showMenu();
        int c;
        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (c) {
        case 1: {
            // Collect taxes from all social classes
            handleTaxCollection(soc, eco);
            break;
        }
        case 2: {
            cout << "How many? ";
            int n;
            std::cin >> n;
            if (n > pop.getTotal()) { cout << "Not enough population.\n"; break; }
            army.conscript(n);
            pop.shrink(n);
            cout << "Recruited " << n << " soldiers.\n";
            break;
        }
        case 3: {
            cout << "Train army. How much gold? ";
            int g;
            std::cin >> g;
            if (g > eco.getGold()) { cout << "Not enough gold.\n"; break; }
            eco.spend(g);
            army.pay(g);
            cout << "Army trained with " << g << " gold.\n";
            break;
        }
        case 4: {
            cout << "Feed army. How much food? ";
            int a;
            std::cin >> a;
            if (a > res.getFood()) { cout << "Not enough food.\n"; break; }
            res.consume("food", a);
            army.feed(a);
            cout << "Army fed.\n";
            break;
        }
        case 5: {
            cout << "Which (food/wood/stone/iron)? ";
            char t[MAX_NAME];
            std::cin >> t;
            cout << "Amount? ";
            int a;
            std::cin >> a;
            res.gather(t, a);
            break;
        }
        case 6: {
            cout << "Build (farm/lumber/mine)? ";
            char t[MAX_NAME];
            std::cin >> t;
            cout << "Amount? ";
            int a;
            std::cin >> a;
            res.build(t, a);
            break;
        }
        case 7: {
            cout << "Expand which storage (food/wood/stone/iron)? ";
            char t[MAX_NAME];
            std::cin >> t;
            cout << "By how much? ";
            int a;
            std::cin >> a;
            res.expandStorage(t, a);
            break;
        }
        case 8: {
            EventManager events;
            events.triggerRandom(soc, pop, army, eco, res, leader);
            int poolCount;
            const Event* pool = events.getPool(poolCount);
            int idx = rand() % poolCount;
            history.addEvent(pool[idx], turn);
            break;
        }
        case 9:
            displayKingdomStatus(soc, pop, army, eco, res, leader);
            break;
        case 10:
            FileManager fileManager;
            fileManager.saveGame("save.txt", eco, soc, army, res, leader, history);
            cout << "Game saved.\n";
            break;
        case 11:
            fileManager.loadGame("save.txt", eco, soc, army, res, leader, history);
            cout << "Game loaded.\n";
            break;
        case 12: {
            cout << "\n--- NEXT TURN (" << ++turn << ") ---\n";
            
            // Update social classes and handle deaths
            for (int i = 0; i < soc.getClassCount(); ++i) {
                SocialClass* sc = soc.getClassByIndex(i);
                if (sc != nullptr) {  // Add null check
                    sc->updateDeathRate();
                    int deaths = (sc->getPopulation() * sc->getDeathRate()) / 100;
                    if (deaths > 0) {
                        cout << sc->getName() << " suffered " << deaths << " deaths this turn.\n";
                        sc->changePopulation(-deaths);
                    }
                }
            }
            
            soc.update();
            pop.update(res.getFood());
            army.update();
            leader.update(eco.getGold(), army.getMorale(), soc.countRevolters(), pop.getTotal());
            eco.peace();
            res.update(pop.getTotal());
            trade.updateOffers();
            diplo.update(army, leader);
            ai.makeDecisions(soc, pop, army, eco, res);
            logger.log(turn, pop.getTotal(), eco.getGold(), army.getSoldiers(), res.getFood());
            logger.save("score.txt");
            
            // Update demographics and record history at the end of each turn
            for (int i = 0; i < soc.getClassCount(); i++) {
                SocialClass* sc = soc.getClassByIndex(i);
                if (sc) {
                    sc->updateDemographics();
                    sc->recordHistory();
                    sc->analyzeHistoricalTrends();
                }
            }
            break;
        }
        case 13: {
            cout << "Trade which (food/wood/stone/iron)? ";
            char r[MAX_NAME];
            std::cin >> r;
            cout << "Amount? ";
            int a;
            std::cin >> a;
            trade.trade(r, a, res, eco);
            break;
        }
        case 14: {
            cout << "1. Declare War\n2. Form Alliance\n3. Make Peace\nChoose: ";
            int d;
            std::cin >> d;
            if (d == 1) diplo.declareWar();
            else if (d == 2) diplo.formAlliance();
            else if (d == 3) diplo.makePeace();
            else cout << "Invalid diplomacy option.\n";
            break;
        }
        case 15: {
            cout << "How many archers? ";
            int n;
            std::cin >> n;
            if (n > pop.getTotal()) { cout << "Not enough population.\n"; break; }
            army.recruitArchers(n);
            pop.shrink(n);
            cout << "Recruited " << n << " archers.\n";
            break;
        }
        case 16: {
            cout << "How many cavalry? ";
            int n;
            std::cin >> n;
            if (n > pop.getTotal()) { cout << "Not enough population.\n"; break; }
            army.recruitCavalry(n);
            pop.shrink(n);
            cout << "Recruited " << n << " cavalry.\n";
            break;
        }
        case 0:
            running = false;
            break;
        default:
            cout << "Invalid.\n";
        }
    }
    cout << "Thanks for playing Stronghold!\n";
    return 0;
}