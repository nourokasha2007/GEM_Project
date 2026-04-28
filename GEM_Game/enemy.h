/*
#ifndef ENEMY_H
#define ENEMY_H

#include "character.h"

// allows enemy to know player exists
class Player;

class Enemy : public Character {
protected:
    int damageValue;

public:
    // Constructor and Virtual Destructor
    Enemy(int startX, int startY, int startSpeed, int startDamage);
    virtual ~Enemy();

    // Getters and Setters (Const correct!)
    int getDamage() const;
    void setDamage(int newDamage);

    // Collision Logic
    bool checkCollision(const Player& targetPlayer) const;

    virtual void triggerGimmick() = 0;
};

#endif // ENEMY_H
*/
