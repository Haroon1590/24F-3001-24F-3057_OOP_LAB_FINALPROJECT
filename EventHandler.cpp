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
    std::cout << "\n*** FAMINE EVENT ***\n";
    std::cout << "A famine has struck the kingdom!" << std::endl;

    // Reduce food by 30-50%
    int foodReduction = 30 + rand() % 21;
    int currentFood = kingdom.getResources().getFood();
    int reducedAmount = (currentFood * foodReduction) / 100;

    if (reducedAmount > 0) {
        kingdom.getResources().setFood(currentFood - reducedAmount);
        std::cout << "Food stocks reduced by " << reducedAmount << " units." << std::endl;
    }

    // Population effects
    kingdom.getPopulation().decreaseHappiness(15);

    // Offer player choice: Distribute reserves or not
    std::cout << "\nHow will you respond?\n";
    std::cout << "1. Distribute emergency food reserves (costs gold)\n";
    std::cout << "2. Let the people fend for themselves\n";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        int cost = kingdom.getPopulation().getTotalPopulation() * 2;
        if (kingdom.getEconomy().hasSufficientFunds(cost)) {
            kingdom.getEconomy().spendTreasury(cost);
            kingdom.getPopulation().increaseHappiness(10);
            std::cout << "You spent " << cost << " gold on emergency food. Population happiness improved." << std::endl;
        }
        else {
            std::cout << "Not enough gold to distribute emergency food!" << std::endl;
        }
    }
    else {
        kingdom.getPopulation().decreaseHappiness(10);
        std::cout << "The people struggle through the famine without aid. Happiness decreases further." << std::endl;
    }
}

void EventHandler::triggerDisease(Kingdom& kingdom) {
    std::cout << "\n*** DISEASE OUTBREAK ***\n";
    std::cout << "A disease is spreading through the kingdom!" << std::endl;

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

    std::cout << "The disease claimed " << populationLoss << " lives." << std::endl;

    // Offer player choice: Medical response
    std::cout << "\nHow will you respond?\n";
    std::cout << "1. Fund medical treatment (costs gold)\n";
    std::cout << "2. Quarantine affected areas (costs less gold but higher population loss)\n";
    std::cout << "3. Take no action\n";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        int cost = totalPop;
        if (kingdom.getEconomy().hasSufficientFunds(cost)) {
            kingdom.getEconomy().spendTreasury(cost);
            population.increaseHappiness(8);
            std::cout << "You spent " << cost << " gold on medical treatment. The disease is contained." << std::endl;
        }
        else {
            std::cout << "Not enough gold for medical treatment!" << std::endl;
        }
    }
    else if (choice == 2) {
        int cost = totalPop / 3;
        if (kingdom.getEconomy().hasSufficientFunds(cost)) {
            kingdom.getEconomy().spendTreasury(cost);

            // Additional population loss from quarantine
            int additionalLoss = peasantLoss / 2;
            population.setPeasants(population.getPeasants() - additionalLoss);

            std::cout << "You spent " << cost << " gold on quarantine measures. ";
            std::cout << "The disease is contained but " << additionalLoss << " additional peasants died." << std::endl;
        }
        else {
            std::cout << "Not enough gold for quarantine measures!" << std::endl;
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

        std::cout << "The disease continues to spread, claiming " << additionalLoss << " additional lives." << std::endl;
    }
}

void EventHandler::triggerWeatherEvent(Kingdom& kingdom) {
    std::cout << "\n*** SEVERE WEATHER EVENT ***\n";

    // Determine weather type
    int weatherType = rand() % 3;
    Resource& resources = kingdom.getResources();

    switch (weatherType) {
    case 0: {
        std::cout << "Heavy storms have damaged buildings and crops!" << std::endl;

        // Reduce food and wood
        int foodLoss = (resources.getFood() * (10 + rand() % 21)) / 100;  // 10-30%
        int woodLoss = (resources.getWood() * (5 + rand() % 16)) / 100;   // 5-20%

        resources.setFood(resources.getFood() - foodLoss);
        resources.setWood(resources.getWood() - woodLoss);

        std::cout << "Lost " << foodLoss << " food and " << woodLoss << " wood." << std::endl;

        // Repair options
        std::cout << "\nWill you allocate resources for repairs?\n";
        std::cout << "1. Yes (costs wood and gold)\n";
        std::cout << "2. No\n";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            int woodNeeded = woodLoss / 2;
            int goldNeeded = kingdom.getPopulation().getTotalPopulation() / 2;

            if (resources.getWood() >= woodNeeded &&
                kingdom.getEconomy().hasSufficientFunds(goldNeeded)) {

                resources.consumeWood(woodNeeded);
                kingdom.getEconomy().spendTreasury(goldNeeded);
                kingdom.getPopulation().increaseHappiness(5);

                std::cout << "Repairs completed using " << woodNeeded << " wood and "
                    << goldNeeded << " gold. Population is grateful." << std::endl;
            }
            else {
                std::cout << "Not enough resources for repairs!" << std::endl;
                kingdom.getPopulation().decreaseHappiness(5);
            }
        }
        else {
            kingdom.getPopulation().decreaseHappiness(10);
            std::cout << "The people must fend for themselves. Happiness decreases." << std::endl;
        }
        break;
    }
    case 1: {
        std::cout << "Drought has affected the kingdom's food production!" << std::endl;

        // Severe food reduction
        int foodLoss = (resources.getFood() * (30 + rand() % 21)) / 100;  // 30-50%
        resources.setFood(resources.getFood() - foodLoss);

        std::cout << "Food production decreased by " << foodLoss << " units." << std::endl;
        kingdom.getPopulation().decreaseHappiness(8);

        // Option to import food
        std::cout << "\nWill you import food from neighboring regions?\n";
        std::cout << "1. Yes (costs gold)\n";
        std::cout << "2. No\n";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            int goldNeeded = foodLoss * 2;
            if (kingdom.getEconomy().hasSufficientFunds(goldNeeded)) {
                kingdom.getEconomy().spendTreasury(goldNeeded);
                resources.addFood(foodLoss);
                kingdom.getPopulation().increaseHappiness(3);

                std::cout << "Imported " << foodLoss << " food for " << goldNeeded << " gold." << std::endl;
            }
            else {
                std::cout << "Not enough gold to import food!" << std::endl;
            }
        }
        else {
            kingdom.getPopulation().decreaseHappiness(7);
            std::cout << "The people will have to get by with less food. Happiness decreases." << std::endl;
        }
        break;
    }
    case 2: {
        std::cout << "Floods have damaged the kingdom's infrastructure!" << std::endl;

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

        std::cout << "Lost " << stoneLoss << " stone, " << woodLoss << " wood, and up to "
            << goldLoss << " gold in economic damage." << std::endl;

        kingdom.getPopulation().decreaseHappiness(12);
        break;
    }
    }
}

void EventHandler::triggerEconomicEvent(Kingdom& kingdom) {
    std::cout << "\n*** ECONOMIC EVENT ***\n";

    int eventType = rand() % 3;
    Economy& economy = kingdom.getEconomy();

    switch (eventType) {
    case 0: {
        std::cout << "Market boom! Merchants are thriving." << std::endl;

        // Economic benefits
        int goldGain = kingdom.getPopulation().getMerchants() * (5 + rand() % 6);  // 5-10 gold per merchant
        economy.addToTreasury(goldGain);

        // Slight inflation increase
        economy.setInflationRate(economy.getInflationRate() + 2);

        std::cout << "Kingdom gained " << goldGain << " gold from increased trade." << std::endl;
        std::cout << "Inflation rate increased to " << economy.getInflationRate() << "%." << std::endl;

        kingdom.getPopulation().increaseHappiness(5);
        break;
    }
    case 1: {
        std::cout << "Economic recession! Trade has slowed down." << std::endl;

        // Treasury reduction
        int treasuryReduction = (economy.getTreasury() * (10 + rand() % 11)) / 100;  // 10-20%
        economy.spendTreasury(treasuryReduction);

        // Inflation decrease
        economy.setInflationRate(economy.getInflationRate() - 3);

        std::cout << "Kingdom lost " << treasuryReduction << " gold due to economic slowdown." << std::endl;
        std::cout << "Inflation rate decreased to " << economy.getInflationRate() << "%." << std::endl;

        kingdom.getPopulation().decreaseHappiness(10);

        // Response options
        std::cout << "\nHow will you respond to the recession?\n";
        std::cout << "1. Lower taxes to stimulate growth\n";
        std::cout << "2. Maintain current taxes\n";
        std::cout << "3. Raise taxes to cover losses\n";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            int oldTaxRate = economy.getTaxRate();
            economy.setTaxRate(oldTaxRate - 5);
            kingdom.getPopulation().increaseHappiness(8);
            std::cout << "Lowered tax rate to " << economy.getTaxRate() << "%. People are pleased." << std::endl;
        }
        else if (choice == 3) {
            int oldTaxRate = economy.getTaxRate();
            economy.setTaxRate(oldTaxRate + 5);
            kingdom.getPopulation().decreaseHappiness(8);
            std::cout << "Raised tax rate to " << economy.getTaxRate() << "%. People are displeased." << std::endl;
        }
        else {
            std::cout << "Maintaining current tax rate." << std::endl;
        }
        break;
    }
    case 2: {
        std::cout << "Foreign traders have arrived with exotic goods!" << std::endl;

        // Trade opportunity
        std::cout << "\nDo you wish to trade with them?\n";
        std::cout << "1. Yes (spend gold for resources)\n";
        std::cout << "2. No\n";

        int choice;
        std::cin >> choice;

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

                std::cout << "Spent " << tradeCost << " gold and received:" << std::endl;
                std::cout << "- " << foodGain << " food" << std::endl;
                std::cout << "- " << woodGain << " wood" << std::endl;
                std::cout << "- " << stoneGain << " stone" << std::endl;
                std::cout << "- " << ironGain << " iron" << std::endl;

                kingdom.getPopulation().increaseHappiness(3);
            }
            else {
                std::cout << "Not enough gold to trade!" << std::endl;
            }
        }
        else {
            std::cout << "You decline the trade offer. The foreign traders move on." << std::endl;
        }
        break;
    }
    }
}