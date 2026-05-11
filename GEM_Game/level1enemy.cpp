#include "level1enemy.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <cmath>
#include <QElapsedTimer>


Level1Enemy::Level1Enemy(Player* target, QGraphicsPixmapItem* pSprite)
    : Enemy(100, 10, 1.2) {
    paused = false;
    player = target;
    playerSprite = pSprite;
    isChasing = false;
    // Fireballs spawn every 8 seconds while chasing
    shootCooldownMs = 8000;

    loadAssets();
    setPixmap(imgIdle);
    setScale(0.35);

    fireballHitSound = new QSoundEffect(this);
    fireballHitSound->setSource(QUrl("qrc:/new/prefix1/sounds/fireball boom.wav"));
    fireballHitSound->setVolume(1.0);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level1Enemy::updateAI);
    timer->start(33);
}
void Level1Enemy::setPaused(bool value)
{
    paused = value;
}

void Level1Enemy::loadAssets() {
    imgIdle.load(":/new/prefix1/images/spirit front.png");
    imgForward.load(":/new/prefix1/images/spirit front.png");
    imgBack.load(":/new/prefix1/images/spirit back.png");
    imgLeft.load(":/new/prefix1/images/spirit left.png");
    imgRight.load(":/new/prefix1/images/spirit right.png");
    imgProjectile.load(":/new/prefix1/images/fireball.png");
}

void Level1Enemy::updateAI() {
    if (!player) return;
    if(paused)
    {
        return;
    }
    double dx = player->getX() - this->x();
    double dy = player->getY() - this->y();
    double distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 700) {
        isChasing = true;
    }

    if (isChasing) {
        shootCooldownMs -= 33;
        if (shootCooldownMs <= 0) {
            shootCooldownMs = 6000;
            shootHomingProjectile();
        }

        if (distance > speed) {
            double moveX = (dx / distance) * speed;
            double moveY = (dy / distance) * speed;

            setPos(x() + moveX, y() + moveY);
        }

        if (std::abs(dx) > std::abs(dy)) {
            if (dx > 0) {
                setPixmap(imgRight);
            } else {
                setPixmap(imgLeft);
            }
        } else {
            if (dy > 0) {
                setPixmap(imgForward);
            } else {
                setPixmap(imgBack);
            }
        }
    }
}
void Level1Enemy::shootHomingProjectile() {
    if(paused)
    {
        return;
    }
    QGraphicsScene* scn = scene();
    if (!scn) return;

    QGraphicsPixmapItem* proj = scn->addPixmap(imgProjectile);
    proj->setScale(0.2);
    proj->setPos(this->x(), this->y());
    proj->setZValue(999);

    QTimer* homingTimer = new QTimer();
    homingTimer->setParent(this);

    // Despawn after 5 seconds
    const int lifetimeMs = 9000;
    QElapsedTimer lifetime;
    lifetime.start();

   connect(homingTimer, &QTimer::timeout, this, [=]() mutable {

    if(paused)
    {
        return;
    }

    if (!player) {
            homingTimer->stop();
            scn->removeItem(proj);
            delete proj;
            homingTimer->deleteLater();
            return;
        }

        if (lifetime.elapsed() >= lifetimeMs) {
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

        if (dist < 10) {
            fireballHitSound->play();
            player->deductScore(10);
            homingTimer->stop();
            scn->removeItem(proj);
            delete proj;
            homingTimer->deleteLater();
            return;
        }

        if (dist < 0.001) dist = 0.001;

        double nx = vx / dist;
        double ny = vy / dist;

        proj->setPos(bx + nx * 4.0, by + ny * 4.0);
    });

    homingTimer->start(50);
}
