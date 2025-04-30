#include "Stronghold.h"
#include <iostream>

Bank::Bank(int t) : treasury(t), loans(0), fraud(0), reputation(100) {
    if (t < 0) treasury = 0;
}

void Bank::giveLoan(int amt, Economy& eco) {
    if (amt > 0 && amt <= treasury) {
        loans += amt;
        treasury -= amt;
        eco.borrow(amt);
        reputation -= 2;
        if (reputation < 0) reputation = 0;
    }
}

void Bank::audit() {
    if (fraud > 0) {
        std::cout << "Fraud detected: " << fraud << std::endl;
        fraud = 0;
        reputation -= 10;
        if (reputation < 0) reputation = 0;
    }
    else {
        reputation += 5;
        if (reputation > 100) reputation = 100;
    }
}

void Bank::scam(int amt) {
    if (amt > 0) {
        fraud += amt;
        reputation -= amt / 10;
        if (reputation < 0) reputation = 0;
    }
}

void Bank::print() const {
    std::cout << "\n=== Bank ===\n"
              << "Treasury: " << treasury 
              << " | Loans: " << loans 
              << " | Fraud: " << fraud 
              << " | Reputation: " << reputation 
              << std::endl;
}

int Bank::getTreasury() const {
    return treasury;
}

int Bank::getFraud() const {
    return fraud;
}

int Bank::getReputation() const {
    return reputation;
}

void Bank::adjustReputation(int n) {
    reputation += n;
    if (reputation > 100) reputation = 100;
    if (reputation < 0) reputation = 0;
}

void Bank::receiveRepayment(int amt) {
    if (amt > 0) {
        treasury += amt;
        loans -= amt;
        if (loans < 0) loans = 0;
    }
}

void Bank::deposit(int amt) {
    if (amt > 0) {
        treasury += amt;
    }
}

void Bank::withdraw(int amt, Economy& eco) {
    if (amt > 0) {
        if (amt > treasury) {
            amt = treasury;
        }
        treasury -= amt;
        eco.addGold(amt);
    }
}
