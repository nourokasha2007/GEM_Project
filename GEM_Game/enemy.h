#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include "character.h"

class Enemy : public QObject, public QGraphicsPixmapItem, public Character {
    Q_OBJECT
public:
    Enemy(int hp, int dmg, double spd);
    virtual ~Enemy() = default;

    // Standard enemy functions
    void takeDamage(int amount);
    int getDamage() const { return damage; }

protected:
    int health;
    int damage;
    double speed;
};

#endif // ENEMY_H