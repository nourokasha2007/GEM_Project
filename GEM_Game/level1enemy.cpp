#include "level1enemy.h"
#include "player.h"
#include <QTimer>
#include <cmath>

Level1Enemy::Level1Enemy(Player* target)
    : Enemy(100, 10, 1.2), player(target) { // 100 HP, 1.2 Speed

    isChasing = false;
    loadAssets();

    setPixmap(imgIdle); // Default look
    setScale(3.0);      // Boss size

    // AI Loop: Runs every 33ms (~30 FPS)
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level1Enemy::updateAI);
    timer->start(33);
}

void Level1Enemy::loadAssets() {
    // Using your new "lvl1 mummy direction" naming

    // Note: Ensure the ":/" paths match your actual .qrc setup
    imgIdle.load(":/new/prefix1/images/lvl1 mummy idle.png");
    imgForward.load(":/new/prefix1/images/lvl1 mummy walking forward.png");
    imgBack.load(":/new/prefix1/images/lvl1 mummy walking back.png");
    imgLeft.load(":/new/prefix1/images/lvl1mummy walking left.png");
    imgRight.load(":/new/prefix1/images/lvl1 mummy walking right.png");
}

void Level1Enemy::updateAI() {
    if (!player) return;

    // 1. Check distance for aggro
    double dx = player->getX() - this->x();
    double dy = player->getY() - this->y();
    double distance = std::sqrt(dx*dx + dy*dy);

    // 2. Start chase if player is within 400 pixels
    if (distance < 400) isChasing = true;

    if (isChasing) {
        // 3. Movement and Sprite Swapping
        if (std::abs(dx) > std::abs(dy)) {
            // Primarily Horizontal Movement
            if (dx > 0) {
                setPos(x() + speed, y());
                setPixmap(imgRight);
            } else {
                setPos(x() - speed, y());
                setPixmap(imgLeft);
            }
        } else {
            // Primarily Vertical Movement
            if (dy > 0) {
                setPos(x(), y() + speed);
                setPixmap(imgForward);
            } else {
                setPos(x(), y() - speed);
                setPixmap(imgBack);
            }
        }
    }
}