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
    virtual ~Level1Enemy() = default;

    void setPaused(bool value);

signals:
    void playerKilled();
    void reduceScore(int amount);

public slots:
    void updateAI();

private:
    Player*              player;
    QGraphicsPixmapItem* playerSprite;

    bool isChasing;
    bool paused;
    int  shootCooldownMs;

    // Directional sprites matching the sprite sheet:
    // DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT
    QPixmap imgIdle;
    QPixmap imgDown;
    QPixmap imgDownLeft;
    QPixmap imgLeft;
    QPixmap imgUpLeft;
    QPixmap imgUp;
    QPixmap imgUpRight;
    QPixmap imgRight;
    QPixmap imgDownRight;
    QPixmap imgAttack;       // used when firing projectile
    QPixmap imgProjectile;   // golden orb / sun projectile

    QSoundEffect* hitSound;
    QSoundEffect* fireballHitSound;  // played when projectile hits player

    void loadAssets();
    void shootHomingProjectile();
};

#endif // LEVEL1ENEMY_H
