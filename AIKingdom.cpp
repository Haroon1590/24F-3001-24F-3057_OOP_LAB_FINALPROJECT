#include "Stronghold.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

AIKingdom::AIKingdom() : eco(1000), army(50), res(500, 300, 200, 100), leader("AI King", "AI"), taxRate(15) {}

void AIKingdom::makeDecisions(Society& playerSoc, Population& playerPop, Army& playerArmy, Economy& playerEco, Resources& playerRes) {
    // Update AI's own state
    eco.tax(playerSoc);
    army.update();
    res.update(playerPop.getTotal());
    leader.update(eco.getGold(), army.getMorale(), playerSoc.countRevolters(), playerPop.getTotal());
    
    // Make strategic decisions
    if (playerSoc.unrest()) {
        // If player has unrest, AI might take advantage
        if (rand() % 100 < 30) {
            army.battle(playerArmy.getStrength());
        }
    }
    
    // Adjust tax rate based on player's strength
    if (playerArmy.getStrength() > army.getStrength()) {
        taxRate = 20; // Higher taxes to build up military
    } else {
        taxRate = 15; // Normal taxes
    }
    
    // Manage resources
    if (res.getFood() < 200) {
        res.gather("food", 100);
    }
    if (res.getWood() < 100) {
        res.gather("wood", 50);
    }
    if (res.getStone() < 50) {
        res.gather("stone", 25);
    }
    if (res.getIron() < 25) {
        res.gather("iron", 10);
    }
    
    // Train army if possible
    if (eco.getGold() > 100) {
        army.train(10);
        eco.spend(100);
    }
}

void AIKingdom::print() const {
    std::cout << "AI Kingdom | Gold: " << eco.getGold() << " | Army: " << army.getStrength() << " | Tax Rate: " << taxRate << "%" << std::endl;
} 