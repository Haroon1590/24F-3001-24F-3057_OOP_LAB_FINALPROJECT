#include "Stronghold.h"
#include <cstring>
EventManager::EventManager() : eventCount(0), poolCount(0) {
    // Initialize with some default events
    addEvent("Good harvest", 1);
    addEvent("Poor harvest", -1);
    addEvent("Trade boom", 2);
    addEvent("Economic crisis", -2);
    addEvent("Population growth", 1);
    addEvent("Disease outbreak", -2);
}

void EventManager::addEvent(const char* desc, int sev) {
    if (eventCount < MAX_EVENTS) {
        strncpy_s(events[eventCount].description, MAX_NAME, desc, MAX_NAME - 1);
        events[eventCount].description[MAX_NAME - 1] = '\0';
        events[eventCount].severity = sev;
        events[eventCount].trigger = false;
        eventCount++;
    }
}

void EventManager::triggerAll(Society& soc, Population& pop, Army& army, 
                            Economy& eco, Resources& res, Leader& l) {
    for (int i = 0; i < eventCount; i++) {
        if (!events[i].trigger) {
            // Apply event effects based on severity
            if (events[i].severity > 0) {
                eco.addGold(events[i].severity * 100);
                pop.grow(events[i].severity * 10);
            } else {
                eco.spend(-events[i].severity * 100);
                pop.shrink(-events[i].severity * 10);
            }
            events[i].trigger = true;
        }
    }
}

void EventManager::triggerRandom(Society& soc, Population& pop, Army& army,
                               Economy& eco, Resources& res, Leader& l) {
    if (eventCount > 0) {
        int idx = rand() % eventCount;
        if (!events[idx].trigger) {
            // Apply random event effects
            if (events[idx].severity > 0) {
                eco.addGold(events[idx].severity * 100);
                pop.grow(events[idx].severity * 10);
            } else {
                eco.spend(-events[idx].severity * 100);
                pop.shrink(-events[idx].severity * 10);
            }
            events[idx].trigger = true;
        }
    }
}

const Event* EventManager::getPool(int& outCount) const {
    outCount = eventCount;
    return events;
}
