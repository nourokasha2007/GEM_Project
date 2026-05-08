#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"

#include <vector>

#include <QString>

class Inventory
{
private:

    //================ ITEMS =================//

    std::vector<Item*> items;

public:

    //================ CORE =================//

    Inventory();

    ~Inventory();

    //================ ITEMS =================//

    void addItem(
        Item* item
        );

    void removeItem(
        QString name
        );

    bool hasItem(
        QString name
        ) const;

    int countItem(
        QString name
        ) const;

    void clear();

    //================ GETTERS =================//

    std::vector<Item*> getItems() const;
};

#endif
