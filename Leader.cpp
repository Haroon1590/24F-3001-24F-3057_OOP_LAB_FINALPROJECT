#include "Stronghold.h"
Leader::Leader(const char* n, const char* t) : legitimacy(100), popularity(50), reignTurns(0) {
    strncpy_s(name, n, MAX_NAME - 1); name[MAX_NAME - 1] = 0;
    strncpy_s(type, t, MAX_NAME - 1); type[MAX_NAME - 1] = 0;
}
void Leader::update(int gold, int morale, int unrest, int pop) {
    if (gold > 1000)
        popularity += 2;
    if (morale < 30)
        legitimacy -= 2;
    if (unrest > pop / 10)
        popularity -= 5;
    reignTurns++;
    if (popularity < 0)
        popularity = 0;
    if (legitimacy < 0)
        legitimacy = 0;
}
void Leader::coup() {
    legitimacy -= 30;
    popularity -= 20;
    strncpy_s(type, "Usurper", MAX_NAME - 1);
    type[MAX_NAME - 1] = 0;
}
void Leader::election() {
    legitimacy = 100;
    popularity += 20;
    strncpy_s(type, "Elected", MAX_NAME - 1);
    type[MAX_NAME - 1] = 0;
}
void Leader::print() const {
    cout << "\n=== Leadership ===\n" << name << " (" << type << ") | Legitimacy: " << legitimacy << " | Popularity: " << popularity << " | Reign: " << reignTurns << " turns\n";
}
const char* Leader::getName() const {
    return name;
}
int Leader::getLegitimacy() const {
    return legitimacy;
}
int Leader::getPopularity() const {
    return popularity;
}
const char* Leader::getType() const {
    return type;
}
int Leader::getReignTurns() const {
    return reignTurns;
}
void Leader::boostLegitimacy(int n) {
    legitimacy += n;
    if (legitimacy > 100) legitimacy = 100;
}
void Leader::changePopularity(int n) {
    popularity += n;
    if (popularity > 100) popularity = 100;
    if (popularity < 0) popularity = 0;
}
