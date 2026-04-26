#ifndef LEVEL_H
#define LEVEL_H

class Level
{
public:
    Level() = default;
    virtual void setup() = 0;
    virtual void update() = 0;

    virtual ~Level() = default;
};

#endif
