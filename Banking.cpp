#include "Stronghold.h"

Banking::Banking() : loanAmount(0), interestRate(10), corruptionLevel(5) {
}

Banking::Banking(int loanAmount, int interestRate)
    : loanAmount(loanAmount), interestRate(interestRate), corruptionLevel(5) {
}

int Banking::getLoanAmount() const {
    return loanAmount;
}

int Banking::getInterestRate() const {
    return interestRate;
}

int Banking::getCorruptionLevel() const {
    return corruptionLevel;
}

void Banking::setLoanAmount(int amount) {
    this->loanAmount = amount;
    if (this->loanAmount < 0) this->loanAmount = 0;
}

void Banking::setInterestRate(int rate) {
    this->interestRate = rate;
    if (this->interestRate < 5) this->interestRate = 5;
    if (this->interestRate > 30) this->interestRate = 30;
}

void Banking::setCorruptionLevel(int level) {
    this->corruptionLevel = level;
    if (this->corruptionLevel < 0) this->corruptionLevel = 0;
    if (this->corruptionLevel > 100) this->corruptionLevel = 100;
}

void Banking::takeLoan(Economy& economy, int amount) {
    if (amount <= 0) {
        cout << "Loan amount must be positive." << endl;
        return;
    }

    // Maximum loan limited by current treasury
    int maxLoan = economy.getTreasury() * 2;
    if (amount > maxLoan) {
        cout << "Maximum loan available is " << maxLoan << " gold." << endl;
        amount = maxLoan;
    }

    // Add the loan to the existing loan amount
    loanAmount += amount;

    // Adjust interest rate based on total loan amount
    if (loanAmount > 2000) {
        interestRate += 2;
        if (interestRate > 30) interestRate = 30;
    }

    // Add the loan to treasury
    economy.addToTreasury(amount);

    cout << "Took a loan of " << amount << " gold at " << interestRate << "% interest rate." << endl;
}

void Banking::repayLoan(Economy& economy, int amount) {
    if (loanAmount == 0) {
        cout << "No loans to repay." << endl;
        return;
    }

    if (amount > loanAmount) {
        amount = loanAmount;
    }

    if (!economy.hasSufficientFunds(amount)) {
        cout << "Insufficient funds to repay loan." << endl;
        return;
    }

    // Deduct payment from treasury
    economy.spendTreasury(amount);

    // Reduce loan amount
    loanAmount -= amount;

    // Adjust interest rate if loan significantly reduced
    if (loanAmount < 1000 && interestRate > 10) {
        interestRate -= 2;
        if (interestRate < 10) interestRate = 10;
    }

    cout << "Repaid " << amount << " gold of loan. Remaining loan: " << loanAmount << " gold." << endl;
}

void Banking::calculateInterest() {
    if (loanAmount > 0) {
        int interestAmount = (loanAmount * interestRate) / 100;
        loanAmount += interestAmount;

        cout << "Interest of " << interestAmount << " gold added to loan. New loan amount: " << loanAmount << " gold." << endl;
    }
}

void Banking::conductAudit(Economy& economy) {
    cout << "\nConducting financial audit..." << endl;

    // Chance to discover corruption based on corruption level
    int discoveryChance = corruptionLevel * 2;
    int roll = rand() % 100;

    if (roll < discoveryChance) {
        // Corruption discovered
        int stolenAmount = (economy.getTreasury() * corruptionLevel) / 200; // Up to half the corruption percentage

        cout << "Corruption discovered! " << stolenAmount << " gold has been embezzled." << endl;

        // Recover some of the stolen funds
        int recoveredAmount = stolenAmount / 2;
        economy.addToTreasury(recoveredAmount);

        cout << "Recovered " << recoveredAmount << " gold from corrupt officials." << endl;

        // Reduce corruption level as corrupt officials have been caught
        corruptionLevel -= rand() % 5 + 1;
        if (corruptionLevel < 0) corruptionLevel = 0;
    }
    else {
        // No corruption discovered (even if there is some)
        cout << "Audit completed. No significant issues found." << endl;

        // Small chance corruption increases if not detected
        if (roll > 80) {
            corruptionLevel += rand() % 3;
            if (corruptionLevel > 100) corruptionLevel = 100;
        }
    }
}

void Banking::displayBanking() const {
    cout << "\n===== BANKING =====\n";
    cout << "Current Loans: " << loanAmount << " gold" << endl;
    cout << "Interest Rate: " << interestRate << "%" << endl;
    cout << "Corruption Level: " << corruptionLevel << "/100" << endl;
    cout << "=================\n";
}