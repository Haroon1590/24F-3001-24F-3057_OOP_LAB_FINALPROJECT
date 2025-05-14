#include "Stronghold.h"

Map::Map(int w, int h) : width(w), height(h), positionCount(0), capacity(10) {
    positions = new Position * [capacity];
    for (int i = 0; i < capacity; i++) positions[i] = nullptr;
}

Map::~Map() {
    for (int i = 0; i < positionCount; i++) delete positions[i];
    delete[] positions;
}

void Map::placeKingdom(const Kingdom& kingdom, int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) throw GameException("Invalid map coordinates.");
    for (int i = 0; i < positionCount; i++) {
        if (positions[i] && positions[i]->x == x && positions[i]->y == y) {
            throw GameException("Position already occupied.");
        }
    }
    if (positionCount >= capacity) {
        int newCapacity = capacity * 2;
        Position** newPositions = new Position * [newCapacity];
        for (int i = 0; i < newCapacity; i++) newPositions[i] = nullptr;
        for (int i = 0; i < positionCount; i++) newPositions[i] = positions[i];
        delete[] positions;
        positions = newPositions;
        capacity = newCapacity;
    }
    positions[positionCount++] = new Position(kingdom.getName(), x, y);
    const_cast<Kingdom&>(kingdom).setMapPosition(x, y);
    std::cout << kingdom.getName() << " placed at (" << x << ", " << y << ").\n";
}

void Map::moveKingdom(Kingdom& kingdom, int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) throw GameException("Invalid map coordinates.");
    for (int i = 0; i < positionCount; i++) {
        if (positions[i] && positions[i]->x == x && positions[i]->y == y) {
            throw GameException("Position already occupied.");
        }
        if (positions[i] && positions[i]->kingdomName == kingdom.getName()) {
            positions[i]->x = x;
            positions[i]->y = y;
            kingdom.setMapPosition(x, y);
            std::cout << kingdom.getName() << " moved to (" << x << ", " << y << ").\n";
            return;
        }
    }
    throw GameException("Kingdom not found on map.");
}

void Map::displayMap() const {
    std::cout << "\n=== Game Map (" << width << "x" << height << ") ===\n";
    char** grid = new char* [height];
    for (int i = 0; i < height; i++) {
        grid[i] = new char[width];
        for (int j = 0; j < width; j++) grid[i][j] = '.';
    }
    for (int i = 0; i < positionCount; i++) {
        if (positions[i]) {
            grid[positions[i]->y][positions[i]->x] = positions[i]->kingdomName[0];
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }
    for (int i = 0; i < height; i++) delete[] grid[i];
    delete[] grid;
    std::cout << "====================\n";
}

int Map::getMapX(const Kingdom& kingdom) const {
    for (int i = 0; i < positionCount; i++) {
        if (positions[i] && positions[i]->kingdomName == kingdom.getName()) {
            return positions[i]->x;
        }
    }
    return -1;
}

int Map::getMapY(const Kingdom& kingdom) const {
    for (int i = 0; i < positionCount; i++) {
        if (positions[i] && positions[i]->kingdomName == kingdom.getName()) {
            return positions[i]->y;
        }
    }
    return -1;
}