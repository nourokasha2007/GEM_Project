#ifndef LEVEL1ENEMY_H
#define LEVEL1ENEMY_H

#include "enemy.h"
#include <QPixmap>

class Player; // Forward declaration

class Level1Enemy : public Enemy {
    Q_OBJECT
public:
    Level1Enemy(Player* target);

public slots:
    void updateAI(); // Decisions for movement and sprites

private:
    Player* player;
    bool isChasing;

    // Pixmaps for your renamed files
    QPixmap imgIdle;
    QPixmap imgForward;
    QPixmap imgBack;
    QPixmap imgLeft;
    QPixmap imgRight;

    void loadAssets();
};

#endif // LEVEL1ENEMY_H