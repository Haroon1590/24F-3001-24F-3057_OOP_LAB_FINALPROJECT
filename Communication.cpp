#include "Stronghold.h"

Communication::Communication() : messageCount(0), capacity(10) {
    messages = new Message * [capacity];
    for (int i = 0; i < capacity; i++) messages[i] = nullptr;
}

Communication::~Communication() {
    for (int i = 0; i < messageCount; i++) delete messages[i];
    delete[] messages;
}

void Communication::sendMessage(const Kingdom& sender, const Kingdom& receiver, const std::string& message) {
    if (message.empty()) throw GameException("Message cannot be empty.");
    if (messageCount >= capacity) {
        int newCapacity = capacity * 2;
        Message** newMessages = new Message * [newCapacity];
        for (int i = 0; i < newCapacity; i++) newMessages[i] = nullptr;
        for (int i = 0; i < messageCount; i++) newMessages[i] = messages[i];
        delete[] messages;
        messages = newMessages;
        capacity = newCapacity;
    }
    messages[messageCount++] = new Message(sender.getName(), receiver.getName(), message);
    std::cout << "Message sent from " << sender.getName() << " to " << receiver.getName() << std::endl;
}

void Communication::displayMessages(const Kingdom& kingdom) const {
    std::cout << "\nMessages for " << kingdom.getName() << ":\n";
    bool hasMessages = false;
    for (int i = 0; i < messageCount; i++) {
        if (messages[i] && messages[i]->receiver == kingdom.getName()) {
            char timeStr[26];
            ctime_s(timeStr, sizeof(timeStr), &messages[i]->timestamp);
            std::cout << "[" << timeStr << "] From " << messages[i]->sender << ": " << messages[i]->content << std::endl;
            hasMessages = true;
        }
    }
    if (!hasMessages) std::cout << "No messages received.\n";
}

void Communication::saveChatLog(const std::string& filename) const {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) throw GameException("Could not open chat log file.");
    for (int i = 0; i < messageCount; i++) {
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &messages[i]->timestamp);
        file << "[" << timeStr << "] " << messages[i]->sender << " -> " << messages[i]->receiver << ": " << messages[i]->content << std::endl;
    }
    file.close();
}