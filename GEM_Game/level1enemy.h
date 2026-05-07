#ifndef LEVEL1ENEMY_H
#define LEVEL1ENEMY_H

#include "enemy.h"
#include <QPixmap>

class Player; // Forward declaration

class Level1Enemy : public Enemy {
    Q_OBJECT
public:
    Level1Enemy(Player* target, QGraphicsPixmapItem* playerSprite);


public slots:
    void updateAI(); // Decisions for movement and sprites

private:
    Player* player;
    QGraphicsPixmapItem* playerSprite;
    bool isChasing;


    // Pixmaps for your renamed files
    QPixmap imgIdle;
    QPixmap imgForward;
    QPixmap imgBack;
    QPixmap imgLeft;
    QPixmap imgRight;

    // Homing projectile
    QPixmap imgProjectile;

    // Simple tuning values (beginner-friendly)
    static constexpr int SHOT_COOLDOWN_MS = 3000;      // shoot every 3s
    static constexpr double PROJECTILE_SPEED = 10.0;   // pixels per tick
    int shootCooldownMs = 0;

    void loadAssets();
    void shootHomingProjectile();
};




#endif // LEVEL1ENEMY_H

