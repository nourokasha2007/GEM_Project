#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Enemy : public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    Enemy(int hp, int dmg, double spd);
    virtual ~Enemy() = default;

    void takeDamage(int amount);
    int getDamage() const;

protected:
    int health;
    int damage;
    double speed;
};

#endif // ENEMY_H
