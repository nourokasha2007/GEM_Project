#ifndef LEVEL1ENEMY_H
#define LEVEL1ENEMY_H

#include "enemy.h"
#include <QPixmap>
#include <QSoundEffect>
#include <QUrl>
#include <QVector>

class QTimer;
class Player;

class Level1Enemy : public Enemy {
    Q_OBJECT
public:
    Level1Enemy(Player* target, QGraphicsPixmapItem* playerSprite);
    void setPaused(bool value);

    /** Stop homing timers before scene->clear(); avoids fireball lambdas touching freed items. */
    void discardActiveHomingProjectiles();

signals:
     void playerKilled();
    void reduceScore(int amount);
    /** Emitted when a homing fireball hits the player (for HUD lives, etc.). */
    void fireballHitRegistered();

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
    QSoundEffect* fireballHitSound;
    bool paused;
    int shootCooldownMs;
    void loadAssets();
    void shootHomingProjectile();

    struct HomingEntry {
        QTimer* timer = nullptr;
        QGraphicsPixmapItem* projectile = nullptr;
    };

    QVector<HomingEntry> homingEntries;

    void removeHomingEntry(QTimer* timer, QGraphicsPixmapItem* projectile);

    QTimer* aiTimer;
};

#endif // LEVEL1ENEMY_H
