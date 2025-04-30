#include "Stronghold.h"
TradeSystem::TradeSystem() : offerCount(4) {
    strncpy_s(tradeOffers[0].resource, "food", MAX_NAME - 1);
    tradeOffers[0].resource[MAX_NAME - 1] = 0;
    tradeOffers[0].price = 2;
    strncpy_s(tradeOffers[1].resource, "wood", MAX_NAME - 1);
    tradeOffers[1].resource[MAX_NAME - 1] = 0;
    tradeOffers[1].price = 1;
    strncpy_s(tradeOffers[2].resource, "stone", MAX_NAME - 1);
    tradeOffers[2].resource[MAX_NAME - 1] = 0;
    tradeOffers[2].price = 3;
    strncpy_s(tradeOffers[3].resource, "iron", MAX_NAME - 1);
    tradeOffers[3].resource[MAX_NAME - 1] = 0;
    tradeOffers[3].price = 5;
}
void TradeSystem::updateOffers() {
    for (int i = 0; i < offerCount; ++i) {
        int change = rand() % 3 - 1;
        tradeOffers[i].price += change;
        if (tradeOffers[i].price < 1)
            tradeOffers[i].price = 1;
    }
}
void TradeSystem::trade(const char* resource, int amount, Resources& res, Economy& eco) {
    int found = -1;
    for (int i = 0; i < offerCount; ++i) {
        if (strcmp(tradeOffers[i].resource, resource) == 0)
            found = i;
    }
    if (found == -1) {
        cout << "Invalid trade resource!\n";
        return;
    }
    res.trade(resource, amount, eco);
    cout << "Traded " << amount << " " << resource << " for " << amount * tradeOffers[found].price << " gold\n";
}
void TradeSystem::print() const {
    cout << "\n=== Trade Offers ===\n";
    for (int i = 0; i < offerCount; ++i) {
        cout << tradeOffers[i].resource << ": " << tradeOffers[i].price << " gold/unit\n";
    }
}