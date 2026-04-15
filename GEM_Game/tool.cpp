#include "Tool.h"
#include <iostream>
using namespace std;

Tool::Tool() : Item() {
    uses = 0;
}

Tool::Tool(string n, bool c, int u)
    : Item(n, c, "Tool") {
    uses = u;
}

void Tool::use() {
    if (uses > 0) {
        uses--;
        cout << name << " used. Remaining uses: " << uses << endl;
    } else {
        cout << name << " has no uses left." << endl;
    }
}

int Tool::getUses() {
    return uses;
}
