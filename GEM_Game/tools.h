#ifndef TOOL_H
#define TOOL_H
#include <string>
#include "item.h"
using namespace std;

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
