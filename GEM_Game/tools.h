#ifndef TOOL_H
#define TOOL_H

#include "Item.h"

class Tool : public Item {
private:
    int uses;

public:
    Tool();
    Tool(string n, bool c, int u);

    void use();
    int getUses();
};

#endif
