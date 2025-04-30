#include "Stronghold.h"
#include <iostream>

Army::Army(int s) : soldiers(s), morale(50), food(100), corruption(0), training(1), archers(0), cavalry(0) {
    if (s < 0) soldiers = 0;
}

void Army::feed(int amt) {
    if (amt > 0) {
        food += amt;
    }
}

void Army::train(int n) {
    if (n > 0) {
        soldiers += n;
        morale += 2;
        training++;
    }
}

void Army::update() {
    if (food < soldiers / 2) {
        morale -= 5;
    }
    if (morale < 0) morale = 0;
    if (morale > 100) morale = 100;
    
    // Natural decay of training
    if (training > 1) {
        training--;
    }
}

void Army::print() const {
    std::cout << "\n=== Army ===\n"
              << "Soldiers: " << soldiers 
              << " | Archers: " << archers 
              << " | Cavalry: " << cavalry
              << " | Morale: " << morale 
              << " | Food: " << food 
              << " | Corruption: " << corruption 
              << " | Training: " << training 
              << std::endl;
}

int Army::getStrength() const {
    return (soldiers + archers * 2 + cavalry * 3) * morale * training / 60;
}

int Army::getSoldiers() const {
    return soldiers;
}

int Army::getMorale() const {
    return morale;
}

int Army::getCorruption() const {
    return corruption;
}

void Army::pay(int gold) {
    if (gold > 0) {
        morale += gold / 10;
        if (morale > 100) morale = 100;
        corruption -= gold / 100;
        if (corruption < 0) corruption = 0;
    }
}

void Army::battle(int enemyStrength) {
    if (enemyStrength < 0) {
        enemyStrength = 0;
    }
    
    int myStrength = getStrength();
    std::cout << "\nBATTLE! Your strength: " << myStrength << " Enemy: " << enemyStrength << std::endl;
    
    if (myStrength > enemyStrength) {
        std::cout << "VICTORY!\n";
        morale += 10;
        sufferCasualties(enemyStrength / 10);
    }
    else {
        std::cout << "DEFEAT!\n";
        morale -= 20;
        sufferCasualties(soldiers / 3);
    }
    
    if (morale < 0) morale = 0;
    if (morale > 100) morale = 100;
}

void Army::sufferCasualties(int n) {
    if (n > 0) {
        soldiers -= n / 2;
        archers -= n / 4;
        cavalry -= n / 4;
        
        if (soldiers < 0) soldiers = 0;
        if (archers < 0) archers = 0;
        if (cavalry < 0) cavalry = 0;
    }
}

void Army::conscript(int n) {
    if (n > 0) {
        soldiers += n;
    }
}

void Army::recruitArchers(int n) {
    if (n > 0) {
        archers += n;
    }
}

void Army::recruitCavalry(int n) {
    if (n > 0) {
        cavalry += n;
    }
}

int Army::getArchers() const {
    return archers;
}

int Army::getCavalry() const {
    return cavalry;
}
