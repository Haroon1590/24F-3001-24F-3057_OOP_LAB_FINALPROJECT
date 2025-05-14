#include "Stronghold.h"

Alliance::Alliance() : treatyCount(0), capacity(10) {
    treaties = new Treaty * [capacity];
    for (int i = 0; i < capacity; i++) treaties[i] = nullptr;
}

Alliance::~Alliance() {
    for (int i = 0; i < treatyCount; i++) delete treaties[i];
    delete[] treaties;
}

void Alliance::formAlliance(const Kingdom& k1, const Kingdom& k2, const std::string& treatyName) {
    if (k1.getName() == k2.getName()) throw GameException("Cannot form alliance with self.");
    if (treatyName.empty()) throw GameException("Treaty name cannot be empty.");
    if (areAllied(k1, k2)) throw GameException("Kingdoms are already allied.");
    if (treatyCount >= capacity) {
        int newCapacity = capacity * 2;
        Treaty** newTreaties = new Treaty * [newCapacity];
        for (int i = 0; i < newCapacity; i++) newTreaties[i] = nullptr;
        for (int i = 0; i < treatyCount; i++) newTreaties[i] = treaties[i];
        delete[] treaties;
        treaties = newTreaties;
        capacity = newCapacity;
    }
    treaties[treatyCount++] = new Treaty(k1.getName(), k2.getName(), treatyName);
    std::cout << "Alliance formed between " << k1.getName() << " and " << k2.getName() << " under treaty '" << treatyName << "'.\n";
}

void Alliance::breakAlliance(const Kingdom& k1, const Kingdom& k2) {
    for (int i = 0; i < treatyCount; i++) {
        if (treaties[i] &&
            ((treaties[i]->kingdom1 == k1.getName() && treaties[i]->kingdom2 == k2.getName()) ||
                (treaties[i]->kingdom1 == k2.getName() && treaties[i]->kingdom2 == k1.getName()))) {
            delete treaties[i];
            treaties[i] = treaties[treatyCount - 1];
            treaties[treatyCount - 1] = nullptr;
            treatyCount--;
            std::cout << "Alliance between " << k1.getName() << " and " << k2.getName() << " has been broken.\n";
            return;
        }
    }
    throw GameException("No alliance exists between these kingdoms.");
}

bool Alliance::areAllied(const Kingdom& k1, const Kingdom& k2) const {
    for (int i = 0; i < treatyCount; i++) {
        if (treaties[i] &&
            ((treaties[i]->kingdom1 == k1.getName() && treaties[i]->kingdom2 == k2.getName()) ||
                (treaties[i]->kingdom1 == k2.getName() && treaties[i]->kingdom2 == k1.getName()))) {
            return true;
        }
    }
    return false;
}

void Alliance::saveTreatyLog(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) throw GameException("Could not open treaty log file.");
    for (int i = 0; i < treatyCount; i++) {
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &treaties[i]->timestamp);
        file << "[" << timeStr << "] Alliance: " << treaties[i]->kingdom1 << " & " <<
            treaties[i]->kingdom2 << " - Treaty: " << treaties[i]->treatyName << std::endl;
    }
    file.close();
}