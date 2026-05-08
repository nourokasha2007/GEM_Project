#ifndef ARTIFACT_H
#define ARTIFACT_H

#include "item.h"

#include <string>

class Artifact : public Item
{
public:

    //================ CONSTRUCTORS =================//

    Artifact();

    Artifact(
        std::string name,
        bool collected
        );
};

#endif
