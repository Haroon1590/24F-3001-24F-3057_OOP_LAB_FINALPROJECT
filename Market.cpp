#include "Stronghold.h"

Market::Market() : offerCount(0), capacity(10) {
    offers = new TradeOffer * [capacity];
    for (int i = 0; i < capacity; i++) offers[i] = nullptr;
}

Market::~Market() {
    for (int i = 0; i < offerCount; i++) delete offers[i];
    delete[] offers;
}

void Market::offerTrade(const Kingdom& seller, const TradeItem<int>& item) {
    if (item.getQuantity() <= 0 || item.getGoldCost() < 0) throw GameException("Invalid trade offer parameters.");
    if (offerCount >= capacity) {
        int newCapacity = capacity * 2;
        TradeOffer** newOffers = new TradeOffer * [newCapacity];
        for (int i = 0; i < newCapacity; i++) newOffers[i] = nullptr;
        for (int i = 0; i < offerCount; i++) newOffers[i] = offers[i];
        delete[] offers;
        offers = newOffers;
        capacity = newCapacity;
    }
    offers[offerCount++] = new TradeOffer(seller.getName(), item);
    std::cout << seller.getName() << " offered " << item.getQuantity() << " " << item.getItemType() <<
        " for " << item.getGoldCost() << " gold.\n";
}

void Market::acceptTrade(Kingdom& buyer, Kingdom& seller, int offerIndex) {
    if (offerIndex < 0 || offerIndex >= offerCount || !offers[offerIndex] || offers[offerIndex]->seller != seller.getName()) {
        throw GameException("Invalid trade offer index or seller.");
    }
    TradeItem<int> item = offers[offerIndex]->item;
    if (!buyer.getEconomy().hasSufficientFunds(item.getGoldCost())) {
        throw GameException("Buyer has insufficient funds.");
    }
    bool hasResources = false;
    if (item.getItemType() == "food" && seller.getResources().getFood() >= item.getQuantity()) {
        seller.getResources().consumeFood(item.getQuantity());
        buyer.getResources().addFood(item.getQuantity());
        hasResources = true;
    }
    else if (item.getItemType() == "wood" && seller.getResources().getWood() >= item.getQuantity()) {
        seller.getResources().consumeWood(item.getQuantity());
        buyer.getResources().addWood(item.getQuantity());
        hasResources = true;
    }
    else if (item.getItemType() == "stone" && seller.getResources().getStone() >= item.getQuantity()) {
        seller.getResources().consumeStone(item.getQuantity());
        buyer.getResources().addStone(item.getQuantity());
        hasResources = true;
    }
    else if (item.getItemType() == "iron" && seller.getResources().getIron() >= item.getQuantity()) {
        seller.getResources().consumeIron(item.getQuantity());
        buyer.getResources().addIron(item.getQuantity());
        hasResources = true;
    }
    if (!hasResources) throw GameException("Seller lacks sufficient resources.");
    buyer.getEconomy().spendTreasury(item.getGoldCost());
    seller.getEconomy().addToTreasury(item.getGoldCost());
    delete offers[offerIndex];
    offers[offerIndex] = offers[offerCount - 1];
    offers[offerCount - 1] = nullptr;
    offerCount--;
    std::cout << buyer.getName() << " accepted trade from " << seller.getName() << ": " <<
        item.getQuantity() << " " << item.getItemType() << " for " << item.getGoldCost() << " gold.\n";
}

void Market::smuggle(Kingdom& smuggler, Kingdom& receiver, const TradeItem<int>& item) {
    if (item.getQuantity() <= 0) throw GameException("Invalid smuggling quantity.");
    bool hasResources = false;
    if (item.getItemType() == "food" && smuggler.getResources().getFood() >= item.getQuantity()) {
        smuggler.getResources().consumeFood(item.getQuantity());
        receiver.getResources().addFood(item.getQuantity());
        hasResources = true;
    }
    else if (item.getItemType() == "wood" && smuggler.getResources().getWood() >= item.getQuantity()) {
        smuggler.getResources().consumeWood(item.getQuantity());
        receiver.getResources().addWood(item.getQuantity());
        hasResources = true;
    }
    else if (item.getItemType() == "stone" && smuggler.getResources().getStone() >= item.getQuantity()) {
        smuggler.getResources().consumeStone(item.getQuantity());
        receiver.getResources().addStone(item.getQuantity());
        hasResources = true;
    }
    else if (item.getItemType() == "iron" && smuggler.getResources().getIron() >= item.getQuantity()) {
        smuggler.getResources().consumeIron(item.getQuantity());
        receiver.getResources().addIron(item.getQuantity());
        hasResources = true;
    }
    if (!hasResources) throw GameException("Smuggler lacks sufficient resources.");
    int discoveryChance = smuggler.getBanking().getCorruptionLevel() * 2;
    if (rand() % 100 < discoveryChance) {
        smuggler.getPopulation().decreaseHappiness(10);
        smuggler.getBanking().setCorruptionLevel(smuggler.getBanking().getCorruptionLevel() + 5);
        std::cout << "Smuggling attempt by " << smuggler.getName() << " was discovered! Happiness decreased, corruption increased.\n";
    }
    else {
        std::cout << smuggler.getName() << " successfully smuggled " << item.getQuantity() << " " <<
            item.getItemType() << " to " << receiver.getName() << ".\n";
    }
}

void Market::displayMarket() const {
    std::cout << "\n=== Market Offers ===\n";
    for (int i = 0; i < offerCount; i++) {
        if (offers[i]) {
            std::cout << (i + 1) << ". " << offers[i]->seller << " offers " <<
                offers[i]->item.getQuantity() << " " << offers[i]->item.getItemType() <<
                " for " << offers[i]->item.getGoldCost() << " gold\n";
        }
    }
    if (offerCount == 0) std::cout << "No trade offers available.\n";
}

void Market::saveMarketLog(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) throw GameException("Could not open market log file.");
    for (int i = 0; i < offerCount; i++) {
        if (offers[i]) {
            file << "Offer by " << offers[i]->seller << ": " << offers[i]->item.getQuantity() << " " <<
                offers[i]->item.getItemType() << " for " << offers[i]->item.getGoldCost() << " gold\n";
        }
    }
    file.close();
}