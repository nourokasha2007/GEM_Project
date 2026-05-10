#ifndef LEVEL2ENEMY_H
#define LEVEL2ENEMY_H

#include "enemy.h"
#include <QPixmap>
#include <QTimer>
#include <QGraphicsEllipseItem>

class Player;

// -------------------------------------------------------
//  Level2Enemy — a wandering wraith / creature
//  It roams the dark room randomly. It carries its own
//  "torch glow" ellipse so the player can see it moving.
//  If the player walks into the glow radius the score is
//  docked and the creature resets to a new random target.
// -------------------------------------------------------
class Level2Enemy : public Enemy
{
    Q_OBJECT

public:
    Level2Enemy(Player* target, QGraphicsPixmapItem* playerSprite);
    virtual ~Level2Enemy() = default;

    // The glow ellipse must be added to the scene separately
    // by whoever constructs this enemy (see Level2.cpp).
    QGraphicsEllipseItem* glowItem() const { return glow; }

public slots:
    void updateAI();

private:
    Player*                 player;
    QGraphicsPixmapItem*    playerSprite;

    // Wandering state
    double  targetX;
    double  targetY;
    int     wanderChangeMs;   // countdown until new random target
    int     wanderCooldown;

    // Directional sprites
    QPixmap imgIdle;
    QPixmap imgLeft;
    QPixmap imgRight;
    QPixmap imgUp;
    QPixmap imgDown;

    // The glow "flashlight" circle that travels with this enemy
    QGraphicsEllipseItem* glow;

    void loadAssets();
    void pickNewTarget();
    void updateGlowPosition();
    void checkPlayerCollision();
};

#endif // LEVEL2ENEMY_H
