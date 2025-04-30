#include "Stronghold.h"
Resources::Resources(int f, int w, int s, int i) : food(f), wood(w), stone(s), iron(i),
farmProd(10), woodProd(5), stoneProd(3), ironProd(2), foodStore(500), woodStore(300), stoneStore(200), ironStore(100) {}
void Resources::gather(const char* type, int amt) {
    if (strcmp(type, "food") == 0) {
        food += amt;
        if (food > foodStore)
            food = foodStore;
    }
    else if (strcmp(type, "wood") == 0) {
        wood += amt;
        if (wood > woodStore)
            wood = woodStore;
    }
    else if (strcmp(type, "stone") == 0) {
        stone += amt;
        if (stone > stoneStore)
            stone = stoneStore;
    }
    else if (strcmp(type, "iron") == 0) {
        iron += amt;
        if (iron > ironStore) iron = ironStore;
    }
    else
        cout << "Unknown resource.\n";
}
void Resources::consume(const char* type, int amt) {
    if (strcmp(type, "food") == 0) {
        food -= amt;
        if (food < 0) food = 0;
    }
    else if (strcmp(type, "wood") == 0) {
        wood -= amt;
        if (wood < 0)
            wood = 0;
    }
    else if (strcmp(type, "stone") == 0) {
        stone -= amt;
        if (stone < 0)
            stone = 0;
    }
    else if (strcmp(type, "iron") == 0) {
        iron -= amt;
        if (iron < 0)
            iron = 0;
    }
    else
        cout << "Unknown resource.\n";
}
int Resources::getFood() const {
    return food;
}
int Resources::getWood() const {
    return wood;
}
int Resources::getStone() const {
    return stone;
}
int Resources::getIron() const {
    return iron;
}
void Resources::print() const {
    cout << "\n=== Resources ===\nFood: " << food << "/" << foodStore << " Wood: " << wood << "/" << woodStore
        << " Stone: " << stone << "/" << stoneStore << " Iron: " << iron << "/" << ironStore << endl;
}
void Resources::build(const char* type, int amt) {
    if (strcmp(type, "farm") == 0) {
        if (wood >= 10 && stone >= 5) {
            farmProd += amt;
            wood -= 10;
            stone -= 5;
            cout << "Built farm\n";
        }
        else cout << "Need more wood/stone!\n";
    }
    else if (strcmp(type, "lumber") == 0) {
        if (wood >= 5 && stone >= 2) {
            woodProd += amt; wood -= 5;
            stone -= 2;
            cout << "Built lumber mill\n";
        }
        else
            cout << "Need more wood/stone!\n";
    }
    else if (strcmp(type, "mine") == 0) {
        if (wood >= 5 && stone >= 5) {
            ironProd += amt;
            wood -= 5;
            stone -= 5;
            cout << "Built mine\n";
        }
        else
            cout << "Need more wood/stone!\n";
    }
    else
        cout << "Unknown build!\n";
}
void Resources::update(int pop) {
    food += farmProd - pop / 10;
    if (food > foodStore)
        food = foodStore;
    if (food < 0)
        food = 0;
    wood += woodProd;
    if (wood > woodStore)
        wood = woodStore;
    stone += stoneProd;
    if (stone > stoneStore)
        stone = stoneStore;
    iron += ironProd;
    if (iron > ironStore)
        iron = ironStore;
}
void Resources::expandStorage(const char* type, int amt) {
    if (strcmp(type, "food") == 0)
        foodStore += amt;
    else if (strcmp(type, "wood") == 0)
        woodStore += amt;
    else if (strcmp(type, "stone") == 0)
        stoneStore += amt;
    else if (strcmp(type, "iron") == 0)
        ironStore += amt;
    else
        cout << "Unknown storage type.\n";
}
void Resources::trade(const char* resource, int amount, Economy& eco) {
    if (strcmp(resource, "food") == 0 && food >= amount) {
        food -= amount;
        eco.addGold(amount * 2);
    }
    else if (strcmp(resource, "wood") == 0 && wood >= amount) {
        wood -= amount;
        eco.addGold(amount * 1);
    }
    else if (strcmp(resource, "stone") == 0 && stone >= amount) {
        stone -= amount;
        eco.addGold(amount * 3);
    }
    else if (strcmp(resource, "iron") == 0 && iron >= amount) {
        iron -= amount;
        eco.addGold(amount * 5);
    }
    else
        cout << "Insufficient resources for trade!\n";
}
