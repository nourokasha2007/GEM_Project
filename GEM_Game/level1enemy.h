#ifndef LEVEL1ENEMY_H
#define LEVEL1ENEMY_H

#include "enemy.h"
#include <QPixmap>

class Player;

class Level1Enemy : public Enemy {
    Q_OBJECT
public:
    Level1Enemy(Player* target, QGraphicsPixmapItem* playerSprite);

signals:
    void reduceTime(int amount);

public slots:
    void updateAI();

private:
    Player* player;
    QGraphicsPixmapItem* playerSprite;
    bool isChasing;

    QPixmap imgIdle;
    QPixmap imgForward;
    QPixmap imgBack;
    QPixmap imgLeft;
    QPixmap imgRight;
    QPixmap imgProjectile;

    int shootCooldownMs;

    void loadAssets();
    void shootHomingProjectile();
};

#endif