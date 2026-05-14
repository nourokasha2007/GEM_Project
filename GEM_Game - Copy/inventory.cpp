#include "inventory.h"

#include <iostream>

using namespace std;

/* ================= CONSTRUCTOR ================= */

Inventory::Inventory()
{

}

/* ================= DESTRUCTOR ================= */

Inventory::~Inventory()
{
    clear();
}

/* ================= ADD ITEM ================= */

void Inventory::addItem(
    Item* item
    )
{
    if(item == nullptr)
        return;

    item->collect();

    items.push_back(item);
}

/* ================= REMOVE ITEM ================= */

void Inventory::removeItem(
    QString name
    )
{
    for(size_t i = 0; i < items.size(); i++)
    {
        QString itemName =
            QString::fromStdString(
                items[i]->getName()
                );

        if(itemName == name)
        {
            items.erase(
                items.begin() + i
                );

            return;
        }
    }
}

/* ================= HAS ITEM ================= */

bool Inventory::hasItem(
    QString name
    ) const
{
    for(size_t i = 0; i < items.size(); i++)
    {
        QString itemName =
            QString::fromStdString(
                items[i]->getName()
                );

        if(itemName == name)
        {
            return true;
        }
    }

    return false;
}

/* ================= COUNT ITEM ================= */

int Inventory::countItem(
    QString name
    ) const
{
    int count = 0;

    for(size_t i = 0; i < items.size(); i++)
    {
        QString itemName =
            QString::fromStdString(
                items[i]->getName()
                );

        if(itemName == name)
        {
            count++;
        }
    }

    return count;
}

/* ================= CLEAR ================= */

void Inventory::clear()
{
    items.clear();
}

/* ================= GET ITEMS ================= */

std::vector<Item*> Inventory::getItems() const
{
    return items;
}
