#ifndef ARTIFACT_H
#define ARTIFACT_H

#include "Item.h"

class Artifact : public Item {
public:
    Artifact();
    Artifact(string n, bool c);
};

#endif
