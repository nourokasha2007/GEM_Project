#ifndef INVENTORY_H
#define INVENTORY_H
#include <string>
#include <vector>
#include "item.h"
using namespace std;

class Inventory {
private:
    vector<Item*> items;

public:
    void addItem(Item* item);
    void showItems();
    int countItem(string name);
    bool hasItem(string name);
};

#endif
