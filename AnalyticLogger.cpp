#include "Stronghold.h"
AnalyticsLogger::AnalyticsLogger() : logCount(0) {}
void AnalyticsLogger::log(int turn, int pop, int gold, int soldiers, int food) {
    if (logCount < MAX_LOGS) {
        logs[logCount].turn = turn;
        logs[logCount].population = pop;
        logs[logCount].gold = gold;
        logs[logCount].soldiers = soldiers;
        logs[logCount].food = food;
        logCount++;
    }
}
void AnalyticsLogger::save(const char* filename) const {
    ofstream out(filename);
    if (!out) {
        cout << "Cannot open score file.\n";
        return;
    }
    for (int i = 0; i < logCount; ++i) {
        out << "Turn " << logs[i].turn << ": Pop=" << logs[i].population << ", Gold=" << logs[i].gold
            << ", Soldiers=" << logs[i].soldiers << ", Food=" << logs[i].food << "\n";
    }
    out.close();
}
void AnalyticsLogger::printReport() const {
    cout << "\n=== Analytics Report ===\n";
    for (int i = 0; i < logCount; ++i) {
        cout << "Turn " << logs[i].turn << ": Population=" << logs[i].population << ", Gold=" << logs[i].gold
            << ", Soldiers=" << logs[i].soldiers << ", Food=" << logs[i].food << "\n";
    }
}
