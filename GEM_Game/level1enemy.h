#ifndef LEVEL1ENEMY_H
#define LEVEL1ENEMY_H

#include "enemy.h"
#include <QPixmap>
#include <QSoundEffect>
#include <QUrl>

class Player;

/* ================================================================
   Level1Enemy — The Golden Hawk Spirit Guardian
   A glowing hawk-headed ancient deity that chases the night guard
   and fires homing golden projectiles when close enough.
   ================================================================ */
class Level1Enemy : public Enemy
{
    Q_OBJECT

public:
    Level1Enemy(Player* target, QGraphicsPixmapItem* playerSprite);
<<<<<<< HEAD
    virtual ~Level1Enemy() = default;
=======
     void setPaused(bool value);

signals:
     void playerKilled();
    void reduceScore(int amount);
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870

public slots:
    void updateAI();

private:
    Player*              player;
    QGraphicsPixmapItem* playerSprite;

    bool isChasing;
<<<<<<< HEAD
    int  shootCooldownMs;

    // Directional sprites matching the sprite sheet:
    // DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT
=======
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870
    QPixmap imgIdle;
    QPixmap imgDown;
    QPixmap imgDownLeft;
    QPixmap imgLeft;
    QPixmap imgUpLeft;
    QPixmap imgUp;
    QPixmap imgUpRight;
    QPixmap imgRight;
<<<<<<< HEAD
    QPixmap imgDownRight;
    QPixmap imgAttack;       // used when firing projectile

    QPixmap imgProjectile;   // golden orb / sun projectile

    QSoundEffect* hitSound;

=======
    QPixmap imgProjectile;
    QSoundEffect* fireballHitSound;
    bool paused;
    int shootCooldownMs;
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870
    void loadAssets();
    void shootHomingProjectile();
};

<<<<<<< HEAD
#endif // LEVEL1ENEMY_H
=======
#endif
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870
