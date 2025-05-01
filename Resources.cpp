#include "Stronghold.h"

Resource::Resource() : food(100), wood(100), stone(50), iron(20) {
}

Resource::Resource(int food, int wood, int stone, int iron)
    : food(food), wood(wood), stone(stone), iron(iron) {
}

int Resource::getFood() const {
    return food;
}

int Resource::getWood() const {
    return wood;
}

int Resource::getStone() const {
    return stone;
}

int Resource::getIron() const {
    return iron;
}

void Resource::setFood(int food) {
    this->food = food;
}

void Resource::setWood(int wood) {
    this->wood = wood;
}

void Resource::setStone(int stone) {
    this->stone = stone;
}

void Resource::setIron(int iron) {
    this->iron = iron;
}

void Resource::addFood(int amount) {
    food += amount;
}

void Resource::addWood(int amount) {
    wood += amount;
}

void Resource::addStone(int amount) {
    stone += amount;
}

void Resource::addIron(int amount) {
    iron += amount;
}

bool Resource::consumeFood(int amount) {
    if (food >= amount) {
        food -= amount;
        return true;
    }
    return false;
}

bool Resource::consumeWood(int amount) {
    if (wood >= amount) {
        wood -= amount;
        return true;
    }
    return false;
}

bool Resource::consumeStone(int amount) {
    if (stone >= amount) {
        stone -= amount;
        return true;
    }
    return false;
}

bool Resource::consumeIron(int amount) {
    if (iron >= amount) {
        iron -= amount;
        return true;
    }
    return false;
}

void Resource::displayResources() const {
    std::cout << "\n===== RESOURCES =====\n";
    std::cout << "Food: " << food << std::endl;
    std::cout << "Wood: " << wood << std::endl;
    std::cout << "Stone: " << stone << std::endl;
    std::cout << "Iron: " << iron << std::endl;
    std::cout << "=====================\n";
}