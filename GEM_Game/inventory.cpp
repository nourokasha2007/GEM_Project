#include "inventory.h"
#include <iostream>
using namespace std;

void Inventory::addItem(Item* item) {
    items.push_back(item);
}

void Inventory::showItems() {
    for (size_t i = 0; i < items.size(); i++) {
        cout << items[i]->getName() << endl;
    }
}

int Inventory::countItem(string name) {
    int count = 0;

    for (size_t i = 0; i < items.size(); i++) {
        if (items[i]->getName() == name) {
            count++;
        }
    }

    return count;
}
