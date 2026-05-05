
#include "item.h"
#include <string>

using namespace std;
Item::Item()
{
    name = "";
    type = "";
    collected = false;
}
Item::Item(string n, bool c, string t)
{
    name = n;
    type = t;
    collected = c;
}
void Item::collect()
{
    collected = true;
}
string Item::getName()
{
    return name;
}
bool Item::isCollected()
{
    return collected;
}
string Item::getType()
{
    return type;
}
void Item::clear()
{
    name = "";
    type = "";
    collected = false;
}
