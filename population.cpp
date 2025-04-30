#include "Stronghold.h"
Population::Population(int t) : total(t), sick(0), revolting(0), growthRate(0.05), mortality(0.02), housing(t), famine(false), plague(false) {}
void Population::update(int food) {
    if (!famine && !plague && housing >= total)
        total += int(total * growthRate);
    int dead = int(total * mortality);
    total -= dead;
    if (famine) {
        total -= total / 20;
        revolting += total / 50;
    }
    if (plague) {
        sick += total / 10; total -= sick / 5;
    }
    if (total > housing)
        revolting += (total - housing) / 10;
    if (total < 0)
        total = 0;
    if (revolting > total)
        revolting = total;
}
void Population::print() const {
    cout << "\n=== Population ===\nTotal: " << total << " Sick: " << sick << " Revolting: " << revolting << " Housing: " << housing << endl;
    if (famine)
        cout << "!!! FAMINE !!!\n";
    if (plague)
        cout << "!!! PLAGUE !!!\n";
}
int Population::getTotal() const {
    return total;
}
void Population::grow(int n) {
    total += n;
}
void Population::shrink(int n) {
    total -= n;
    if (total < 0)
        total = 0;
}
void Population::buildHousing(int n) {
    housing += n;
}
void Population::setFamine(bool b) {
    famine = b;
}
void Population::setPlague(bool b) {
    plague = b;
}
int Population::getHousing() const {
    return housing;
}
int Population::getSick() const {
    return sick;
}
int Population::getRevolting() const {
    return revolting;
}
void Population::cureDisease(int n) {
    sick -= n;
    if (sick < 0)
        sick = 0;
}
void Population::suppressRevolt(int n) {
    revolting -= n;
    if (revolting < 0)
        revolting = 0;
}
void Population::resetRevolting() {
    revolting = 0;
}