#include "Item.h"

class Artifact : public Item {
public:
    
    Artifact() : Item() {}

    Artifact(string n, bool c): Item(n,c, "Artifact") {}
};

