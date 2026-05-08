#ifndef LEVEL1ENEMY_H
#define LEVEL1ENEMY_H

#include "enemy.h"
#include <QPixmap>
#include <QGraphicsScene>

class Level1Enemy : public Enemy {
    Q_OBJECT
public:
    Level1Enemy(QGraphicsPixmapItem* targetSprite);

public slots:
    void updateAI();

private:
    QGraphicsPixmapItem* playerSprite;
    bool isChasing;

    QPixmap imgIdle;
    QPixmap imgForward;
    QPixmap imgBack;
    QPixmap imgLeft;
    QPixmap imgRight;

    void loadAssets();
};

#endif // LEVEL1ENEMY_H