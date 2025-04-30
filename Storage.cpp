#include "Stronghold.h"
StorageInt::StorageInt() : count(0) {}
void StorageInt::add(int item) {
    if (count < MAX_SOCIAL_CLASSES)
        items[count++] = item;
    else cout << "Storage full!\n";
}
int& StorageInt::get(int idx) {
    if (idx < 0 || idx >= count) {
        cout << "Invalid index!\n"; static int dummy = -1;
        return dummy;
    } return items[idx];
}
int StorageInt::size() const {
    return count;
}