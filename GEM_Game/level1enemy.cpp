#include "level1enemy.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <cmath>

Level1Enemy::Level1Enemy(Player* target, QGraphicsPixmapItem* pSprite)
    : Enemy(100, 10, 1.2) {

    player = target;
    playerSprite = pSprite;
    isChasing = false;
    shootCooldownMs = 3000;

    loadAssets();
    setPixmap(imgIdle);
    setScale(4.0);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level1Enemy::updateAI);
    timer->start(33);
}

void Level1Enemy::loadAssets() {
    imgIdle.load("E:/University Stuff/CS 2 Course/CS 2 PROJECT/GEM_Project/GEM_Game/images/lvl1 mummy idle(1).png");
    imgForward.load("E:/University Stuff/CS 2 Course/CS 2 PROJECT/GEM_Project/GEM_Game/images/lvl1 mummy walking forward(1).png");
    imgBack.load("E:/University Stuff/CS 2 Course/CS 2 PROJECT/GEM_Project/GEM_Game/images/lvl1 mummy walking back(1).png");
    imgLeft.load("E:/University Stuff/CS 2 Course/CS 2 PROJECT/GEM_Project/GEM_Game/images/lvl1mummy walking left(1).png");
    imgRight.load("E:/University Stuff/CS 2 Course/CS 2 PROJECT/GEM_Project/GEM_Game/images/lvl1 mummy walking right(1).png");
    imgProjectile.load("E:/University Stuff/CS 2 Course/CS 2 PROJECT/GEM_Project/GEM_Game/images/fireball.png");
}

void Level1Enemy::updateAI() {
    if (!player) return;

    double dx = player->getX() - this->x();
    double dy = player->getY() - this->y();
    double distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 700) {
        isChasing = true;
    }

    if (isChasing) {
        shootCooldownMs -= 33;
        if (shootCooldownMs <= 0) {
            shootCooldownMs = 7000;
            shootHomingProjectile();
        }

        if (std::abs(dx) > std::abs(dy)) {
            if (dx > 0) {
                setPos(x() + speed, y());
                setPixmap(imgRight);
            } else {
                setPos(x() - speed, y());
                setPixmap(imgLeft);
            }
        } else {
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

void Level1Enemy::shootHomingProjectile() {
    QGraphicsScene* scn = scene();
    if (!scn) return;

    QGraphicsPixmapItem* proj = scn->addPixmap(imgProjectile);
    proj->setScale(0.2);
    proj->setPos(this->x(), this->y());
    proj->setZValue(999);

    QTimer* homingTimer = new QTimer();
    homingTimer->setParent(this);

    connect(homingTimer, &QTimer::timeout, this, [=]() mutable {
        if (!player) {
            homingTimer->stop();
            scn->removeItem(proj);
            delete proj;
            homingTimer->deleteLater();
            return;
        }

        double px = player->getX();
        double py = player->getY();
        double bx = proj->x();
        double by = proj->y();

        double vx = px - bx;
        double vy = py - by;
        double dist = std::sqrt(vx * vx + vy * vy);

        if (dist < 20) {
            emit reduceTime(15);
            homingTimer->stop();
            scn->removeItem(proj);
            delete proj;
            homingTimer->deleteLater();
            return;
        }

        if (dist < 0.001) dist = 0.001;

        double nx = vx / dist;
        double ny = vy / dist;

        proj->setPos(bx + nx * 3.0, by + ny * 3.0);
    });

    homingTimer->start(50);
}