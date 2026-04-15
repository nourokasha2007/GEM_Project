#ifndef ITEM_H
#define ITEM_H

#include <string>
using namespace std;

class Item {
protected:
    string name;
    bool collected;
    string type;

public:
    Item();
    Item(string n, bool c, string t);

    void collect();
    string getName();
    bool isCollected();
    string getType();
};

#endif
