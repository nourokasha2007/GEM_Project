#include "level1enemy.h"
#include <QTimer>
#include <cmath>

Level1Enemy::Level1Enemy(QGraphicsPixmapItem* targetSprite)
    : Enemy(100, 10, 1.2) // 100 HP, 10 Dmg, 1.2 Speed
{
    playerSprite = targetSprite;
    isChasing = true;

    loadAssets();
    setPixmap(imgIdle);

    // AI Loop: Runs every 33ms (~30 FPS)
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level1Enemy::updateAI);
    timer->start(33);
}

void Level1Enemy::loadAssets() {
    // Notice the (1) added to match your new file names
    imgIdle.load(":/new/prefix1/images/lvl1 mummy idle(1).png");
    imgForward.load(":/new/prefix1/images/lvl1 mummy walking forward(1).png");
    imgBack.load(":/new/prefix1/images/lvl1 mummy walking back(1).png");
    imgLeft.load(":/new/prefix1/images/lvl1mummy walking left(1).png");
    imgRight.load(":/new/prefix1/images/lvl1 mummy walking right(1).png");

    // // Projectile image
    // imgProjectile.load(":/new/prefix1/images/fireball.png");
}

void Level1Enemy::updateAI()
{
    if (!playerSprite || !isChasing) return;

    // Get current positions
    double ex = this->x();
    double ey = this->y();
    double px = playerSprite->x();
    double py = playerSprite->y();

    // Calculate distance and direction
    double dx = px - ex;
    double dy = py - ey;
    double dist = std::sqrt(dx * dx + dy * dy);

    if (dist < 5.0) return; // Close enough, stop jittering

    // Normalize and move
    double nx = dx / dist;
    double ny = dy / dist;

    setPos(ex + (nx * speed), ey + (ny * speed));

    // Update facing sprite based on dominant movement direction
    if (std::abs(dx) > std::abs(dy)) {
        if (dx > 0 && !imgRight.isNull()) setPixmap(imgRight);
        else if (dx < 0 && !imgLeft.isNull()) setPixmap(imgLeft);
    } else {
        if (dy > 0 && !imgForward.isNull()) setPixmap(imgForward);
        else if (dy < 0 && !imgBack.isNull()) setPixmap(imgBack);
    }
}