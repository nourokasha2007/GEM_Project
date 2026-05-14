#ifndef LEVEL2ENEMY_H
#define LEVEL2ENEMY_H

#include "enemy.h"
#include <QPixmap>
#include <QSoundEffect>
#include <QUrl>

class Player;

class Level2Enemy : public Enemy {
    Q_OBJECT
public:
    Level2Enemy(Player* target, QGraphicsPixmapItem* playerSprite);
    void setPaused(bool value);

    int getStrikeCount() const;

signals:
    void ghostScreech();
    void reduceSpeed();

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
    QSoundEffect* screechSound;
    bool paused;
    int strikeCooldownMs;
    int strikeCount;
    bool hasFirstScreechPlayed;
    void loadAssets();
};

#endif
