#include "Stronghold.h"

EventHandler::EventHandler() : eventProbability(15) {
    // Initialize random seed
    srand(time(0));
}

void EventHandler::generateRandomEvent(Kingdom& kingdom) {
    // Roll to determine if an event occurs
    int roll = rand() % 100;

    if (roll < eventProbability) {
        // An event occurs! Determine which type
        int eventType = rand() % 4;

        switch (eventType) {
        case 0:
            triggerFamine(kingdom);
            break;
        case 1:
            triggerDisease(kingdom);
            break;
        case 2:
            triggerWeatherEvent(kingdom);
            break;
        case 3:
            triggerEconomicEvent(kingdom);
            break;
        }
    }
}

void EventHandler::triggerFamine(Kingdom& kingdom) {
    cout << "\n*** FAMINE EVENT ***\n";
    cout << "A famine has struck the kingdom!" << endl;

    // Reduce food by 30-50%
    int foodReduction = 30 + rand() % 21;
    int currentFood = kingdom.getResources().getFood();
    int reducedAmount = (currentFood * foodReduction) / 100;

    if (reducedAmount > 0) {
        kingdom.getResources().setFood(currentFood - reducedAmount);
        cout << "Food stocks reduced by " << reducedAmount << " units." << endl;
    }

    // Population effects
    kingdom.getPopulation().decreaseHappiness(15);

    // Offer player choice: Distribute reserves or not
    cout << "\nHow will you respond?\n";
    cout << "1. Distribute emergency food reserves (costs gold)\n";
    cout << "2. Let the people fend for themselves\n";

    int choice;
    cin >> choice;

    if (choice == 1) {
        int cost = kingdom.getPopulation().getTotalPopulation() * 2;
        if (kingdom.getEconomy().hasSufficientFunds(cost)) {
            kingdom.getEconomy().spendTreasury(cost);
            kingdom.getPopulation().increaseHappiness(10);
            cout << "You spent " << cost << " gold on emergency food. Population happiness improved." << endl;
        }
        else {
            cout << "Not enough gold to distribute emergency food!" << endl;
        }
    }
    else {
        kingdom.getPopulation().decreaseHappiness(10);
        cout << "The people struggle through the famine without aid. Happiness decreases further." << endl;
    }
}

void EventHandler::triggerDisease(Kingdom& kingdom) {
    cout << "\n*** DISEASE OUTBREAK ***\n";
    cout << "A disease is spreading through the kingdom!" << endl;

    // Calculate population loss (3-8% of total)
    Population& population = kingdom.getPopulation();
    int totalPop = population.getTotalPopulation();
    int diseaseImpact = 3 + rand() % 6;
    int populationLoss = (totalPop * diseaseImpact) / 100;

    // Apply population losses proportionally
    int peasantLoss = (population.getPeasants() * diseaseImpact) / 100;
    int merchantLoss = (population.getMerchants() * diseaseImpact) / 100;
    int nobleLoss = (population.getNobles() * diseaseImpact) / 100;

    population.setPeasants(population.getPeasants() - peasantLoss);
    population.setMerchants(population.getMerchants() - merchantLoss);
    population.setNobles(population.getNobles() - nobleLoss);

    // Happiness decrease
    population.decreaseHappiness(12);

    cout << "The disease claimed " << populationLoss << " lives." << endl;

    // Offer player choice: Medical response
    cout << "\nHow will you respond?\n";
    cout << "1. Fund medical treatment (costs gold)\n";
    cout << "2. Quarantine affected areas (costs less gold but higher population loss)\n";
    cout << "3. Take no action\n";

    int choice;
    cin >> choice;

    if (choice == 1) {
        int cost = totalPop;
        if (kingdom.getEconomy().hasSufficientFunds(cost)) {
            kingdom.getEconomy().spendTreasury(cost);
            population.increaseHappiness(8);
            cout << "You spent " << cost << " gold on medical treatment. The disease is contained." << endl;
        }
        else {
            cout << "Not enough gold for medical treatment!" << endl;
        }
    }
    else if (choice == 2) {
        int cost = totalPop / 3;
        if (kingdom.getEconomy().hasSufficientFunds(cost)) {
            kingdom.getEconomy().spendTreasury(cost);

            // Additional population loss from quarantine
            int additionalLoss = peasantLoss / 2;
            population.setPeasants(population.getPeasants() - additionalLoss);

            cout << "You spent " << cost << " gold on quarantine measures. ";
            cout << "The disease is contained but " << additionalLoss << " additional peasants died." << endl;
        }
        else {
            cout << "Not enough gold for quarantine measures!" << endl;
        }
    }
    else {
        // Additional population loss from inaction
        int additionalLoss = populationLoss;
        peasantLoss = (population.getPeasants() * diseaseImpact) / 100;
        merchantLoss = (population.getMerchants() * diseaseImpact) / 100;
        nobleLoss = (population.getNobles() * diseaseImpact) / 100;

        population.setPeasants(population.getPeasants() - peasantLoss);
        population.setMerchants(population.getMerchants() - merchantLoss);
        population.setNobles(population.getNobles() - nobleLoss);

        population.decreaseHappiness(15);

        cout << "The disease continues to spread, claiming " << additionalLoss << " additional lives." << endl;
    }
}

void EventHandler::triggerWeatherEvent(Kingdom& kingdom) {
    cout << "\n*** SEVERE WEATHER EVENT ***\n";

    // Determine weather type
    int weatherType = rand() % 3;
    Resource& resources = kingdom.getResources();

    switch (weatherType) {
    case 0: {
        cout << "Heavy storms have damaged buildings and crops!" << endl;

        // Reduce food and wood
        int foodLoss = (resources.getFood() * (10 + rand() % 21)) / 100;  // 10-30%
        int woodLoss = (resources.getWood() * (5 + rand() % 16)) / 100;   // 5-20%

        resources.setFood(resources.getFood() - foodLoss);
        resources.setWood(resources.getWood() - woodLoss);

        cout << "Lost " << foodLoss << " food and " << woodLoss << " wood." << endl;

        // Repair options
        cout << "\nWill you allocate resources for repairs?\n";
        cout << "1. Yes (costs wood and gold)\n";
        cout << "2. No\n";

        int choice;
        cin >> choice;

        if (choice == 1) {
            int woodNeeded = woodLoss / 2;
            int goldNeeded = kingdom.getPopulation().getTotalPopulation() / 2;

            if (resources.getWood() >= woodNeeded &&
                kingdom.getEconomy().hasSufficientFunds(goldNeeded)) {

                resources.consumeWood(woodNeeded);
                kingdom.getEconomy().spendTreasury(goldNeeded);
                kingdom.getPopulation().increaseHappiness(5);

                cout << "Repairs completed using " << woodNeeded << " wood and "
                    << goldNeeded << " gold. Population is grateful." << endl;
            }
            else {
                cout << "Not enough resources for repairs!" << endl;
                kingdom.getPopulation().decreaseHappiness(5);
            }
        }
        else {
            kingdom.getPopulation().decreaseHappiness(10);
            cout << "The people must fend for themselves. Happiness decreases." << endl;
        }
        break;
    }
    case 1: {
        cout << "Drought has affected the kingdom's food production!" << endl;

        // Severe food reduction
        int foodLoss = (resources.getFood() * (30 + rand() % 21)) / 100;  // 30-50%
        resources.setFood(resources.getFood() - foodLoss);

        cout << "Food production decreased by " << foodLoss << " units." << endl;
        kingdom.getPopulation().decreaseHappiness(8);

        // Option to import food
        cout << "\nWill you import food from neighboring regions?\n";
        cout << "1. Yes (costs gold)\n";
        cout << "2. No\n";

        int choice;
        cin >> choice;

        if (choice == 1) {
            int goldNeeded = foodLoss * 2;
            if (kingdom.getEconomy().hasSufficientFunds(goldNeeded)) {
                kingdom.getEconomy().spendTreasury(goldNeeded);
                resources.addFood(foodLoss);
                kingdom.getPopulation().increaseHappiness(3);

                cout << "Imported " << foodLoss << " food for " << goldNeeded << " gold." << endl;
            }
            else {
                cout << "Not enough gold to import food!" << endl;
            }
        }
        else {
            kingdom.getPopulation().decreaseHappiness(7);
            cout << "The people will have to get by with less food. Happiness decreases." << endl;
        }
        break;
    }
    case 2: {
        cout << "Floods have damaged the kingdom's infrastructure!" << endl;

        // Damage to buildings (represented by stone and wood loss)
        int stoneLoss = (resources.getStone() * (10 + rand() % 11)) / 100;  // 10-20%
        int woodLoss = (resources.getWood() * (15 + rand() % 16)) / 100;    // 15-30%

        resources.setStone(resources.getStone() - stoneLoss);
        resources.setWood(resources.getWood() - woodLoss);

        // Also causes economic damage
        int goldLoss = kingdom.getPopulation().getTotalPopulation() * (rand() % 2 + 1);
        if (kingdom.getEconomy().getTreasury() > goldLoss) {
            kingdom.getEconomy().spendTreasury(goldLoss);
        }
        else {
            kingdom.getEconomy().setTreasury(0);
        }

        cout << "Lost " << stoneLoss << " stone, " << woodLoss << " wood, and up to "
            << goldLoss << " gold in economic damage." << endl;

        kingdom.getPopulation().decreaseHappiness(12);
        break;
    }
    }
}

void EventHandler::triggerEconomicEvent(Kingdom& kingdom) {
    cout << "\n*** ECONOMIC EVENT ***\n";

    int eventType = rand() % 3;
    Economy& economy = kingdom.getEconomy();

    switch (eventType) {
    case 0: {
        cout << "Market boom! Merchants are thriving." << endl;

        // Economic benefits
        int goldGain = kingdom.getPopulation().getMerchants() * (5 + rand() % 6);  // 5-10 gold per merchant
        economy.addToTreasury(goldGain);

        // Slight inflation increase
        economy.setInflationRate(economy.getInflationRate() + 2);

        cout << "Kingdom gained " << goldGain << " gold from increased trade." << endl;
        cout << "Inflation rate increased to " << economy.getInflationRate() << "%." << endl;

        kingdom.getPopulation().increaseHappiness(5);
        break;
    }
    case 1: {
        cout << "Economic recession! Trade has slowed down." << endl;

        // Treasury reduction
        int treasuryReduction = (economy.getTreasury() * (10 + rand() % 11)) / 100;  // 10-20%
        economy.spendTreasury(treasuryReduction);

        // Inflation decrease
        economy.setInflationRate(economy.getInflationRate() - 3);

        cout << "Kingdom lost " << treasuryReduction << " gold due to economic slowdown." << endl;
        cout << "Inflation rate decreased to " << economy.getInflationRate() << "%." << endl;

        kingdom.getPopulation().decreaseHappiness(10);

        // Response options
        cout << "\nHow will you respond to the recession?\n";
        cout << "1. Lower taxes to stimulate growth\n";
        cout << "2. Maintain current taxes\n";
        cout << "3. Raise taxes to cover losses\n";

        int choice;
        cin >> choice;

        if (choice == 1) {
            int oldTaxRate = economy.getTaxRate();
            economy.setTaxRate(oldTaxRate - 5);
            kingdom.getPopulation().increaseHappiness(8);
            cout << "Lowered tax rate to " << economy.getTaxRate() << "%. People are pleased." << endl;
        }
        else if (choice == 3) {
            int oldTaxRate = economy.getTaxRate();
            economy.setTaxRate(oldTaxRate + 5);
            kingdom.getPopulation().decreaseHappiness(8);
            cout << "Raised tax rate to " << economy.getTaxRate() << "%. People are displeased." << endl;
        }
        else {
            cout << "Maintaining current tax rate." << endl;
        }
        break;
    }
    case 2: {
        cout << "Foreign traders have arrived with exotic goods!" << endl;

        // Trade opportunity
        cout << "\nDo you wish to trade with them?\n";
        cout << "1. Yes (spend gold for resources)\n";
        cout << "2. No\n";

        int choice;
        cin >> choice;

        if (choice == 1) {
            int tradeCost = 200 + rand() % 301;  // 200-500 gold

            if (economy.hasSufficientFunds(tradeCost)) {
                economy.spendTreasury(tradeCost);

                // Gain random resources
                int foodGain = 50 + rand() % 151;  // 50-200 food
                int woodGain = 30 + rand() % 71;   // 30-100 wood
                int stoneGain = 20 + rand() % 41;  // 20-60 stone
                int ironGain = 10 + rand() % 21;   // 10-30 iron

                kingdom.getResources().addFood(foodGain);
                kingdom.getResources().addWood(woodGain);
                kingdom.getResources().addStone(stoneGain);
                kingdom.getResources().addIron(ironGain);

                cout << "Spent " << tradeCost << " gold and received:" << endl;
                cout << "- " << foodGain << " food" << endl;
                cout << "- " << woodGain << " wood" << endl;
                cout << "- " << stoneGain << " stone" << endl;
                cout << "- " << ironGain << " iron" << endl;

                kingdom.getPopulation().increaseHappiness(3);
            }
            else {
                cout << "Not enough gold to trade!" << endl;
            }
        }
        else {
            cout << "You decline the trade offer. The foreign traders move on." << endl;
        }
        break;
    }
    }
}