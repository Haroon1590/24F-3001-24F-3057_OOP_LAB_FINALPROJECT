#include "Stronghold.h"
#include <cstring>

struct HistoryEntry {
    Event event;
    int turn;
    HistoryEntry* next;
    
    HistoryEntry(const Event& e, int t) : event(e), turn(t), next(nullptr) {}
};

EventHistory::EventHistory() : head(nullptr), size(0) {}

EventHistory::~EventHistory() {
    while (head) {
        HistoryEntry* temp = head;
        head = head->next;
        delete temp;
    }
}

void EventHistory::addEvent(const Event& e, int turn) {
    HistoryEntry* newEntry = new HistoryEntry(e, turn);
    newEntry->next = head;
    head = newEntry;
    size++;
}

void EventHistory::print() const {
    cout << "\nEvent History:" << endl;
    HistoryEntry* current = head;
    while (current) {
        cout << "Turn " << current->turn << ": " 
             << current->event.description 
             << " (Severity: " << current->event.severity << ")" << endl;
        current = current->next;
    }
}

void EventHistory::save(std::ofstream& out) const {
    out << size << endl;
    HistoryEntry* current = head;
    while (current) {
        out << current->turn << " "
            << current->event.description << " "
            << current->event.severity << endl;
        current = current->next;
    }
}

void EventHistory::load(std::ifstream& in) {
    // Clear existing history
    while (head) {
        HistoryEntry* temp = head;
        head = head->next;
        delete temp;
    }
    size = 0;
    
    int count;
    in >> count;
    in.ignore(); // Skip newline
    
    for (int i = 0; i < count; i++) {
        Event e;
        int turn;
        in >> turn;
        in.ignore(); // Skip space
        in.getline(e.description, MAX_NAME);
        in >> e.severity;
        in.ignore(); // Skip newline
        
        addEvent(e, turn);
    }
} 