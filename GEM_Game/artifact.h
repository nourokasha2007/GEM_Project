#ifndef ARTIFACT_H
#define ARTIFACT_H
#include <string>
#include "item.h"
using namespace std;

class Artifact : public Item {
public:
    Artifact();
    Artifact(string n, bool c);
};

#endif
