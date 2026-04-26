#ifndef LEVEL1_H
#define LEVEL1_H

#include "Level.h"

class Level1 : public Level
{
public:
    Level1();

    void setup() override;
    void update() override;

    ~Level1() override = default;
};

#endif
