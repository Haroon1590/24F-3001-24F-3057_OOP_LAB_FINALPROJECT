#include "Stronghold.h"
EventNode::EventNode(const Event& e, int t) : event(e), turn(t), next(0) {}

EventHistory::EventHistory() : head(0), size(0) {}
EventHistory::~EventHistory() {
    EventNode* current = head;
    while (current) {
        EventNode* next = current->next;
        delete current;
        current = next;
    }
}
void EventHistory::addEvent(const Event& e, int turn) {
    EventNode* node = new EventNode(e, turn);
    node->next = head;
    head = node;
    size++;
}
void EventHistory::print() const {
    cout << "\n=== Event History ===\n";
    EventNode* current = head;
    while (current) {
        cout << "Turn " << current->turn << ": " << current->event.description << " (Severity: " << current->event.severity << ")\n";
        current = current->next;
    }
}
void EventHistory::save(ofstream& out) const {
    out << size << "\n";
    EventNode* current = head;
    while (current) {
        out << current->turn << " " << current->event.description << " " << current->event.severity << "\n";
        current = current->next;
    }
}
void EventHistory::load(ifstream& in) {
    int count;
    in >> count;
    for (int i = 0; i < count; ++i) {
        int turn, severity;
        char desc[MAX_EVENT_DESC];
        in >> turn;
        in.ignore();
        in.getline(desc, MAX_EVENT_DESC, ' ');
        in >> severity;
        Event e;
        strncpy_s(e.description, desc, MAX_EVENT_DESC - 1); e.description[MAX_NAME - 1] = 0;
        e.severity = severity;
        addEvent(e, turn);
    }
}
