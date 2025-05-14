#include "Stronghold.h"
#include <iostream>
#include <vector>

using namespace std;

void displayMenu() {
    cout << "\n================================ STRONGHOLD : KINGDOM MANAGEMENT ==============================\n";
    cout << "1. Display Kingdom Status\n";
    cout << "2. Simulate Next Turn\n";
    cout << "3. Resource Management\n";
    cout << "4. Population Management\n";
    cout << "5. Economy Management\n";
    cout << "6. Military Management\n";
    cout << "7. Leadership Management\n";
    cout << "8. Banking Options\n";
    cout << "9. Save/Load Game\n";
    cout << "10. Send Message\n";
    cout << "11. View Messages\n";
    cout << "12. Form Alliance\n";
    cout << "13. Break Alliance\n";
    cout << "14. Offer Trade\n";
    cout << "15. Accept Trade\n";
    cout << "16. Smuggle Resources\n";
    cout << "17. Declare War\n";
    cout << "18. Resolve Battle\n";
    cout << "19. Betray Ally\n";
    cout << "20. Move Kingdom on Map\n";
    cout << "21. Display Map\n";
    cout << "22. Exit Game\n";
    cout << "==============================================================================================\n";
    cout << "Enter your choice: ";
}

int main() {
    srand(time(0));
    int numPlayers;
    cout << "Enter the number of players (max 5): ";
    cin >> numPlayers;
    if (numPlayers < 1 || numPlayers > 5) {
        cout << "Invalid number of players. Setting to 2.\n";
        numPlayers = 2;
    }
    cin.ignore();

    vector<Kingdom> kingdoms;
    for (int i = 0; i < numPlayers; i++) {
        string name;
        cout << "Enter name for Player " << (i + 1) << "'s kingdom: ";
        getline(cin, name);
        kingdoms.push_back(Kingdom(name));
    }

    Communication* comm = new Communication();
    Alliance* alliances = new Alliance();
    Market* market = new Market();
    Conflict* conflicts = new Conflict();
    Map* map = new Map(10, 10);

    // Place kingdoms on map
    for (int i = 0; i < numPlayers; i++) {
        map->placeKingdom(kingdoms[i], i * 2, i * 2);
    }

    int currentPlayer = 0;
    bool exitGame = false;

    cout << "Welcome to Stronghold Multiplayer Game!\n";

    while (!exitGame) {
        cout << "\n=== Turn for " << kingdoms[currentPlayer].getName() << " ===\n";
        displayMenu();
        int choice;
        cin >> choice;
        cin.ignore();

        try {
            if (choice == 1) {
                kingdoms[currentPlayer].displayKingdomStatus();
            }
            else if (choice == 2) {
                kingdoms[currentPlayer].simulateTurn();
            }
            else if (choice == 3) {
                int action;
                cout << "1. Gather Resources\n2. Consume Food\nEnter choice: ";
                cin >> action;
                if (action == 1) kingdoms[currentPlayer].gatherResources();
                else if (action == 2) {
                    int amount;
                    cout << "Enter amount to consume: ";
                    cin >> amount;
                    if (!kingdoms[currentPlayer].getResources().consumeFood(amount)) {
                        cout << "Not enough food!\n";
                    }
                }
            }
            else if (choice == 4) {
                int action;
                cout << "1. Update Population\n2. Adjust Happiness\nEnter choice: ";
                cin >> action;
                if (action == 1) {
                    int taxRate;
                    cout << "Enter tax rate (0-100): ";
                    cin >> taxRate;
                    kingdoms[currentPlayer].getPopulation().updatePopulation(kingdoms[currentPlayer].getResources(), taxRate);
                }
                else if (action == 2) {
                    int amount;
                    cout << "Enter happiness adjustment: ";
                    cin >> amount;
                    if (amount > 0) kingdoms[currentPlayer].getPopulation().increaseHappiness(amount);
                    else kingdoms[currentPlayer].getPopulation().decreaseHappiness(-amount);
                }
            }
            else if (choice == 5) {
                int action;
                cout << "1. Collect Taxes\n2. Pay Military\nEnter choice: ";
                cin >> action;
                if (action == 1) kingdoms[currentPlayer].getEconomy().collectTaxes(kingdoms[currentPlayer].getPopulation());
                else if (action == 2) kingdoms[currentPlayer].getEconomy().payMilitary(kingdoms[currentPlayer].getMilitary());
            }
            else if (choice == 6) {
                int action;
                cout << "1. Recruit Soldiers\n2. Train Troops\nEnter choice: ";
                cin >> action;
                if (action == 1) {
                    int count, type;
                    cout << "Enter number of soldiers: ";
                    cin >> count;
                    cout << "Enter type (0: Infantry, 1: Archers, 2: Cavalry): ";
                    cin >> type;
                    kingdoms[currentPlayer].getMilitary().recruitSoldiers(kingdoms[currentPlayer].getPopulation(), kingdoms[currentPlayer].getResources(), count, type);
                }
                else if (action == 2) kingdoms[currentPlayer].getMilitary().trainTroops(kingdoms[currentPlayer].getResources());
            }
            else if (choice == 7) {
                int action;
                cout << "1. Make Decision\n2. Update Popularity\nEnter choice: ";
                cin >> action;
                if (action == 1) kingdoms[currentPlayer].getLeadership().makeDecision(kingdoms[currentPlayer].getEconomy(), kingdoms[currentPlayer].getMilitary());
                else if (action == 2) kingdoms[currentPlayer].getLeadership().updatePopularity(kingdoms[currentPlayer].getEconomy(), kingdoms[currentPlayer].getPopulation());
            }
            else if (choice == 8) {
                int action;
                cout << "1. Take Loan\n2. Repay Loan\nEnter choice: ";
                cin >> action;
                if (action == 1) {
                    int amount;
                    cout << "Enter loan amount: ";
                    cin >> amount;
                    kingdoms[currentPlayer].getBanking().takeLoan(kingdoms[currentPlayer].getEconomy(), amount);
                }
                else if (action == 2) {
                    int amount;
                    cout << "Enter repayment amount: ";
                    cin >> amount;
                    kingdoms[currentPlayer].getBanking().repayLoan(kingdoms[currentPlayer].getEconomy(), amount);
                }
            }
            else if (choice == 9) {
                int action;
                cout << "1. Save Game\n2. Load Game\nEnter choice: ";
                cin >> action;
                string filename;
                cout << "Enter filename prefix: ";
                cin >> filename;
                if (action == 1) {
                    kingdoms[currentPlayer].saveGame(filename + "_" + kingdoms[currentPlayer].getName() + ".txt");
                    comm->saveChatLog(filename + "_chat.txt");
                    alliances->saveTreatyLog(filename + "_treaties.txt");
                    market->saveMarketLog(filename + "_market.txt");
                    conflicts->saveConflictLog(filename + "_conflicts.txt");
                    cout << "Game saved.\n";
                }
                else if (action == 2) {
                    kingdoms[currentPlayer].loadGame(filename + "_" + kingdoms[currentPlayer].getName() + ".txt");
                    map->placeKingdom(kingdoms[currentPlayer], kingdoms[currentPlayer].getMapX(), kingdoms[currentPlayer].getMapY());
                    cout << "Game loaded.\n";
                }
            }
            else if (choice == 10) {
                int receiver;
                string message;
                cout << "Select receiver (0-" << (numPlayers - 1) << "): ";
                cin >> receiver;
                cin.ignore();
                if (receiver < 0 || receiver >= numPlayers) throw GameException("Invalid kingdom index.");
                cout << "Enter message: ";
                getline(cin, message);
                comm->sendMessage(kingdoms[currentPlayer], kingdoms[receiver], message);
            }
            else if (choice == 11) {
                comm->displayMessages(kingdoms[currentPlayer]);
            }
            else if (choice == 12) {
                int k2;
                string treatyName;
                cout << "Select kingdom to ally with (0-" << (numPlayers - 1) << "): ";
                cin >> k2;
                cin.ignore();
                if (k2 < 0 || k2 >= numPlayers) throw GameException("Invalid kingdom index.");
                cout << "Enter treaty name: ";
                getline(cin, treatyName);
                alliances->formAlliance(kingdoms[currentPlayer], kingdoms[k2], treatyName);
            }
            else if (choice == 13) {
                int k2;
                cout << "Select kingdom to break alliance with (0-" << (numPlayers - 1) << "): ";
                cin >> k2;
                if (k2 < 0 || k2 >= numPlayers) throw GameException("Invalid kingdom index.");
                alliances->breakAlliance(kingdoms[currentPlayer], kingdoms[k2]);
            }
            else if (choice == 14) {
                string itemType;
                int quantity, goldCost;
                cout << "Enter item type (food, wood, stone, iron): ";
                cin >> itemType;
                cout << "Enter quantity: ";
                cin >> quantity;
                cout << "Enter gold cost: ";
                cin >> goldCost;
                TradeItem<int> item(itemType, quantity, goldCost);
                market->offerTrade(kingdoms[currentPlayer], item);
            }
            else if (choice == 15) {
                int seller;
                int offerIndex;
                market->displayMarket();
                cout << "Select seller (0-" << (numPlayers - 1) << "): ";
                cin >> seller;
                cout << "Select offer number: ";
                cin >> offerIndex;
                if (seller < 0 || seller >= numPlayers || offerIndex < 1) throw GameException("Invalid input.");
                market->acceptTrade(kingdoms[currentPlayer], kingdoms[seller], offerIndex - 1);
            }
            else if (choice == 16) {
                int receiver;
                string itemType;
                int quantity;
                cout << "Select receiver (0-" << (numPlayers - 1) << "): ";
                cin >> receiver;
                cout << "Enter item type (food, wood, stone, iron): ";
                cin >> itemType;
                cout << "Enter quantity: ";
                cin >> quantity;
                if (receiver < 0 || receiver >= numPlayers) throw GameException("Invalid kingdom index.");
                TradeItem<int> item(itemType, quantity, 0);
                market->smuggle(kingdoms[currentPlayer], kingdoms[receiver], item);
            }
            else if (choice == 17) {
                int defender;
                cout << "Select defender (0-" << (numPlayers - 1) << "): ";
                cin >> defender;
                if (defender < 0 || defender >= numPlayers) throw GameException("Invalid kingdom index.");
                if (alliances->areAllied(kingdoms[currentPlayer], kingdoms[defender])) {
                    cout << "Warning: Declaring war on an ally will break the alliance and incur penalties.\n";
                    alliances->breakAlliance(kingdoms[currentPlayer], kingdoms[defender]);
                    kingdoms[currentPlayer].getPopulation().decreaseHappiness(10);
                }
                conflicts->declareWar(kingdoms[currentPlayer], kingdoms[defender]);
            }
            else if (choice == 18) {
                int defender;
                cout << "Select defender (0-" << (numPlayers - 1) << "): ";
                cin >> defender;
                if (defender < 0 || defender >= numPlayers) throw GameException("Invalid kingdom index.");
                conflicts->resolveBattle(kingdoms[currentPlayer], kingdoms[defender]);
            }
            else if (choice == 19) {
                int betrayed;
                cout << "Select kingdom to betray (0-" << (numPlayers - 1) << "): ";
                cin >> betrayed;
                if (betrayed < 0 || betrayed >= numPlayers) throw GameException("Invalid kingdom index.");
                if (!alliances->areAllied(kingdoms[currentPlayer], kingdoms[betrayed])) {
                    throw GameException("Cannot betray a non-ally.");
                }
                alliances->breakAlliance(kingdoms[currentPlayer], kingdoms[betrayed]);
                conflicts->betrayAlly(kingdoms[currentPlayer], kingdoms[betrayed]);
            }
            else if (choice == 20) {
                int x, y;
                cout << "Enter new X coordinate: ";
                cin >> x;
                cout << "Enter new Y coordinate: ";
                cin >> y;
                map->moveKingdom(kingdoms[currentPlayer], x, y);
            }
            else if (choice == 21) {
                map->displayMap();
            }
            else if (choice == 22) {
                cout << "Are you sure you want to exit? (1 for Yes, 0 for No): ";
                cin >> choice;
                if (choice == 1) {
                    exitGame = true;
                    cout << "Thank you for playing Stronghold!\n";
                }
            }
            else {
                throw GameException("Invalid menu choice.");
            }
        }
        catch (const GameException& e) {
            cout << "Error: " << e.getMessage() << endl;
        }

        if (!exitGame) {
            currentPlayer = (currentPlayer + 1) % numPlayers;
            cout << "\nPress Enter to continue to next player's turn...";
            cin.get();
        }
    }

    // Cleanup
    delete comm;
    delete alliances;
    delete market;
    delete conflicts;
    delete map;

    return 0;
}