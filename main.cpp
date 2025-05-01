#include "Stronghold.h"
// Function prototypes for menu options
void displayMainMenu();
void handleResourceManagement(Kingdom& kingdom);
void handlePopulationManagement(Kingdom& kingdom);
void handleEconomyManagement(Kingdom& kingdom);
void handleMilitaryManagement(Kingdom& kingdom);
void handleLeadershipManagement(Kingdom& kingdom);
void handleBankingManagement(Kingdom& kingdom);
void handleSaveLoad(Kingdom& kingdom);

int main() {
    // Seed random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Initialize kingdom with default values
    Kingdom kingdom("Player's Kingdom");
    std::cout << "Welcome to Stronghold Game - Module 1: Core Kingdom Engine\n";
    std::cout << "Your kingdom \"" << kingdom.getName() << "\" has been established!\n\n";

    int choice = 0;
    bool exitGame = false;

    // Main game loop
    while (!exitGame) {
        displayMainMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        // Clear any bad input
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(100, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        std::cout << "\n";

        switch (choice) {
        case 1: // Display kingdom status
            kingdom.displayKingdomStatus();
            break;
        case 2: // Simulate a turn
            kingdom.simulateTurn();
            std::cout << "Turn simulated!\n";
            break;
        case 3: // Resource management
            handleResourceManagement(kingdom);
            break;
        case 4: // Population management
            handlePopulationManagement(kingdom);
            break;
        case 5: // Economy management
            handleEconomyManagement(kingdom);
            break;
        case 6: // Military management
            handleMilitaryManagement(kingdom);
            break;
        case 7: // Leadership management
            handleLeadershipManagement(kingdom);
            break;
        case 8: // Banking management
            handleBankingManagement(kingdom);
            break;
        case 9: // Save/Load game
            handleSaveLoad(kingdom);
            break;
        case 0: // Exit
            std::cout << "Are you sure you want to exit? (1 for Yes, 0 for No): ";
            std::cin >> choice;
            if (choice == 1) {
                exitGame = true;
                std::cout << "Thank you for playing Stronghold!\n";
            }
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
        std::cout << "\n";
    }

    // Log final score before exiting
    kingdom.logScore("score.txt");

    return 0;
}

void displayMainMenu() {
    std::cout << "======= STRONGHOLD: KINGDOM MANAGEMENT =======\n";
    std::cout << "1. Display Kingdom Status\n";
    std::cout << "2. Simulate Next Turn\n";
    std::cout << "3. Resource Management\n";
    std::cout << "4. Population Management\n";
    std::cout << "5. Economy Management\n";
    std::cout << "6. Military Management\n";
    std::cout << "7. Leadership Management\n";
    std::cout << "8. Banking Options\n";
    std::cout << "9. Save/Load Game\n";
    std::cout << "0. Exit Game\n";
    std::cout << "===========================================\n";
}

void handleResourceManagement(Kingdom& kingdom) {
    int choice = 0;
    int amount = 0;
    bool back = false;

    while (!back) {
        std::cout << "======= RESOURCE MANAGEMENT =======\n";
        kingdom.getResources().displayResources();
        std::cout << "1. Gather Resources\n";
        std::cout << "2. Allocate Food to Population\n";
        std::cout << "3. Allocate Wood for Construction\n";
        std::cout << "4. Allocate Stone for Buildings\n";
        std::cout << "5. Allocate Iron for Weapons\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << "=================================\n";

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            kingdom.gatherResources();
            std::cout << "Resources gathered!\n";
            break;
        case 2:
            std::cout << "Enter amount of food to allocate: ";
            std::cin >> amount;
            if (kingdom.getResources().consumeFood(amount)) {
                std::cout << amount << " food allocated. Population happiness increased.\n";
                kingdom.getPopulation().increaseHappiness(amount / 10);
            }
            else {
                std::cout << "Not enough food available.\n";
            }
            break;
        case 3:
            std::cout << "Enter amount of wood to allocate: ";
            std::cin >> amount;
            if (kingdom.getResources().consumeWood(amount)) {
                std::cout << amount << " wood allocated for construction.\n";
            }
            else {
                std::cout << "Not enough wood available.\n";
            }
            break;
        case 4:
            std::cout << "Enter amount of stone to allocate: ";
            std::cin >> amount;
            if (kingdom.getResources().consumeStone(amount)) {
                std::cout << amount << " stone allocated for buildings.\n";
            }
            else {
                std::cout << "Not enough stone available.\n";
            }
            break;
        case 5:
            std::cout << "Enter amount of iron to allocate: ";
            std::cin >> amount;
            if (kingdom.getResources().consumeIron(amount)) {
                std::cout << amount << " iron allocated for weapons.\n";
            }
            else {
                std::cout << "Not enough iron available.\n";
            }
            break;
        case 0:
            back = true;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

void handlePopulationManagement(Kingdom& kingdom) {
    int choice = 0;
    int amount = 0;
    bool back = false;

    while (!back) {
        std::cout << "======= POPULATION MANAGEMENT =======\n";
        kingdom.getPopulation().displayPopulation();
        std::cout << "1. Increase Happiness (Requires Resources)\n";
        std::cout << "2. Check for Revolts\n";
        std::cout << "3. Update Population (Based on Resources)\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << "===================================\n";

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter food amount to distribute (increases happiness): ";
            std::cin >> amount;
            if (kingdom.getResources().consumeFood(amount)) {
                kingdom.getPopulation().increaseHappiness(amount / 20);
                std::cout << "Distributed food to population. Happiness increased.\n";
            }
            else {
                std::cout << "Not enough food available.\n";
            }
            break;
        case 2:
            if (kingdom.getPopulation().isRevolting()) {
                std::cout << "WARNING: Population is on the verge of revolt!\n";
                std::cout << "Do you want to address their concerns? (1 for Yes, 0 for No): ";
                std::cin >> choice;
                if (choice == 1) {
                    // Lower taxes temporarily
                    int currentTax = kingdom.getEconomy().getTaxRate();
                    kingdom.getEconomy().setTaxRate(currentTax - 5);
                    kingdom.getPopulation().increaseHappiness(10);
                    std::cout << "You've lowered taxes and addressed concerns. Happiness improved.\n";
                }
                else {
                    kingdom.getPopulation().handleRevolt();
                }
            }
            else {
                std::cout << "Population is not revolting at the moment.\n";
            }
            break;
        case 3:
            kingdom.getPopulation().updatePopulation(kingdom.getResources(), kingdom.getEconomy().getTaxRate());
            std::cout << "Population updated based on current resources and conditions.\n";
            break;
        case 0:
            back = true;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

void handleEconomyManagement(Kingdom& kingdom) {
    int choice = 0;
    int amount = 0;
    bool back = false;

    while (!back) {
        std::cout << "======= ECONOMY MANAGEMENT =======\n";
        kingdom.getEconomy().displayEconomy();
        std::cout << "1. Collect Taxes\n";
        std::cout << "2. Adjust Tax Rate\n";
        std::cout << "3. Pay Military\n";
        std::cout << "4. Adjust Inflation\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << "================================\n";

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            kingdom.getEconomy().collectTaxes(kingdom.getPopulation());
            std::cout << "Taxes collected from the population.\n";
            break;
        case 2:
            std::cout << "Enter new tax rate (0-100): ";
            std::cin >> amount;
            kingdom.getEconomy().setTaxRate(amount);
            std::cout << "Tax rate adjusted to " << kingdom.getEconomy().getTaxRate() << "%.\n";
            break;
        case 3:
            kingdom.getEconomy().payMilitary(kingdom.getMilitary());
            std::cout << "Military has been paid.\n";
            break;
        case 4:
            kingdom.getEconomy().adjustInflation();
            std::cout << "Inflation adjusted to " << kingdom.getEconomy().getInflationRate() << "%.\n";
            break;
        case 0:
            back = true;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

void handleMilitaryManagement(Kingdom& kingdom) {
    int choice = 0;
    int amount = 0;
    int type = 0;
    bool back = false;

    while (!back) {
        std::cout << "======= MILITARY MANAGEMENT =======\n";
        kingdom.getMilitary().displayMilitary();
        std::cout << "1. Recruit Infantry\n";
        std::cout << "2. Recruit Archers\n";
        std::cout << "3. Recruit Cavalry\n";
        std::cout << "4. Train Troops (Improves Morale)\n";
        std::cout << "5. Update Military Morale\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << "=================================\n";

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter number of infantry to recruit: ";
            std::cin >> amount;
            kingdom.getMilitary().recruitSoldiers(kingdom.getPopulation(), kingdom.getResources(), amount, 1);
            break;
        case 2:
            std::cout << "Enter number of archers to recruit: ";
            std::cin >> amount;
            kingdom.getMilitary().recruitSoldiers(kingdom.getPopulation(), kingdom.getResources(), amount, 2);
            break;
        case 3:
            std::cout << "Enter number of cavalry to recruit: ";
            std::cin >> amount;
            kingdom.getMilitary().recruitSoldiers(kingdom.getPopulation(), kingdom.getResources(), amount, 3);
            break;
        case 4:
            kingdom.getMilitary().trainTroops(kingdom.getResources());
            break;
        case 5:
            kingdom.getMilitary().updateMorale(kingdom.getEconomy(), kingdom.getPopulation());
            std::cout << "Military morale updated.\n";
            break;
        case 0:
            back = true;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

void handleLeadershipManagement(Kingdom& kingdom) {
    int choice = 0;
    int value = 0;
    std::string name;
    bool back = false;

    while (!back) {
        std::cout << "======= LEADERSHIP MANAGEMENT =======\n";
        kingdom.getLeadership().displayLeadership();
        std::cout << "1. Change Leader's Name\n";
        std::cout << "2. Make Strategic Decision\n";
        std::cout << "3. Check for Potential Coup\n";
        std::cout << "4. Update Leader Popularity\n";
        std::cout << "5. Adjust Leader Skills\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << "===================================\n";

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter new leader name: ";
            std::cin.ignore();
            std::getline(std::cin, name);
            kingdom.getLeadership().setName(name);
            std::cout << "Leader name changed to " << name << ".\n";
            break;
        case 2:
            kingdom.getLeadership().makeDecision(kingdom.getEconomy(), kingdom.getMilitary());
            break;
        case 3:
            kingdom.getLeadership().handleCoup(kingdom.getPopulation(), kingdom.getMilitary());
            break;
        case 4:
            kingdom.getLeadership().updatePopularity(kingdom.getEconomy(), kingdom.getPopulation());
            break;
        case 5:
            std::cout << "1. Adjust Diplomacy\n";
            std::cout << "2. Adjust Military Skill\n";
            std::cout << "3. Adjust Economy Skill\n";
            std::cout << "Select skill to adjust: ";
            std::cin >> choice;

            std::cout << "Enter new value (0-100): ";
            std::cin >> value;

            switch (choice) {
            case 1:
                kingdom.getLeadership().setDiplomacy(value);
                std::cout << "Diplomacy skill set to " << kingdom.getLeadership().getDiplomacy() << ".\n";
                break;
            case 2:
                kingdom.getLeadership().setMilitarySkill(value);
                std::cout << "Military skill set to " << kingdom.getLeadership().getMilitarySkill() << ".\n";
                break;
            case 3:
                kingdom.getLeadership().setEconomySkill(value);
                std::cout << "Economy skill set to " << kingdom.getLeadership().getEconomySkill() << ".\n";
                break;
            default:
                std::cout << "Invalid skill choice.\n";
            }
            break;
        case 0:
            back = true;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

void handleBankingManagement(Kingdom& kingdom) {
    int choice = 0;
    int amount = 0;
    bool back = false;

    while (!back) {
        std::cout << "======= BANKING OPTIONS =======\n";
        kingdom.getBanking().displayBanking();
        std::cout << "1. Take a Loan\n";
        std::cout << "2. Repay Loan\n";
        std::cout << "3. Calculate Interest\n";
        std::cout << "4. Conduct Financial Audit\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << "=============================\n";

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter loan amount: ";
            std::cin >> amount;
            kingdom.getBanking().takeLoan(kingdom.getEconomy(), amount);
            break;
        case 2:
            std::cout << "Enter repayment amount: ";
            std::cin >> amount;
            kingdom.getBanking().repayLoan(kingdom.getEconomy(), amount);
            break;
        case 3:
            kingdom.getBanking().calculateInterest();
            break;
        case 4:
            kingdom.getBanking().conductAudit(kingdom.getEconomy());
            break;
        case 0:
            back = true;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}

void handleSaveLoad(Kingdom& kingdom) {
    int choice = 0;
    std::string filename;
    bool back = false;

    while (!back) {
        std::cout << "======= SAVE/LOAD GAME =======\n";
        std::cout << "1. Save Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Log Current Score\n";
        std::cout << "0. Back to Main Menu\n";
        std::cout << "============================\n";

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            std::cout << "Enter filename to save (e.g., save.txt): ";
            std::cin >> filename;
            if (kingdom.saveGame(filename)) {
                std::cout << "Game saved successfully to " << filename << ".\n";
            }
            else {
                std::cout << "Failed to save game.\n";
            }
            break;
        case 2:
            std::cout << "Enter filename to load (e.g., save.txt): ";
            std::cin >> filename;
            if (kingdom.loadGame(filename)) {
                std::cout << "Game loaded successfully from " << filename << ".\n";
            }
            else {
                std::cout << "Failed to load game.\n";
            }
            break;
        case 3:
            kingdom.logScore("score.txt");
            std::cout << "Score logged to score.txt.\n";
            break;
        case 0:
            back = true;
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    }
}