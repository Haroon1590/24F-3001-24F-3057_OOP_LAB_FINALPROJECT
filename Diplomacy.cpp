#include "Stronghold.h"
Diplomacy::Diplomacy() : relation(0) {
    strncpy_s(status, "Peace", MAX_NAME - 1);
    status[MAX_NAME - 1] = 0;
}
void Diplomacy::declareWar() {
    strncpy_s(status, "War", MAX_NAME - 1);
    status[MAX_NAME - 1] = 0;
    relation -= 50;
    if (relation < -100)
        relation = -100;
}
void Diplomacy::formAlliance() {
    strncpy_s(status, "Alliance", MAX_NAME - 1);
    status[MAX_NAME - 1] = 0;
    relation += 50;
    if (relation > 100)
        relation = 100;
}
void Diplomacy::makePeace() {
    strncpy_s(status, "Peace", MAX_NAME - 1);
    status[MAX_NAME - 1] = 0;
    relation += 20;
    if (relation > 100)
        relation = 100;
}
void Diplomacy::update(Army& army, Leader& leader) {
    if (strcmp(status, "War") == 0)
        army.battle(army.getStrength() / 2);
    if (strcmp(status, "Alliance") == 0)
        leader.boostLegitimacy(5);
}
void Diplomacy::print() const {
    cout << "\n=== Diplomacy ===\nStatus: " << status << " Relation: " << relation << endl;
}
const char* Diplomacy::getStatus() const {
    return status;
}
int Diplomacy::getRelation() const {
    return relation;
}
