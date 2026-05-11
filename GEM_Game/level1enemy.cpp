#include "level1enemy.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <cmath>
#include <QElapsedTimer>

Level1Enemy::Level1Enemy(Player* target, QGraphicsPixmapItem* pSprite)
    : Enemy(100, 10, 1.2) {

    player = target;
    playerSprite = pSprite;
    isChasing = false;
    shootCooldownMs = 9000;

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

void Level1Enemy::loadAssets() {
    imgIdle.load(":/new/prefix1/images/spirit front.png");
    imgForward.load(":/new/prefix1/images/spirit front.png");
    imgBack.load(":/new/prefix1/images/spirit back.png");
    imgLeft.load(":/new/prefix1/images/spirit left.png");
    imgRight.load(":/new/prefix1/images/spirit right.png");
    imgProjectile.load(":/new/prefix1/images/fireball.png");
}

void Level1Enemy::updateAI() {
    if (player == nullptr) {
        return;
    }

    double differenceX = player->getX() - this->x();
    double differenceY = player->getY() - this->y();
    double distanceToPlayer = std::sqrt((differenceX * differenceX) + (differenceY * differenceY));

    if (distanceToPlayer < 700) {
        isChasing = true;
    }

    if (isChasing == true) {
        shootCooldownMs = shootCooldownMs - 33;

        if (shootCooldownMs <= 0) {
            shootCooldownMs = 6000;
            shootHomingProjectile();
        }

        if (distanceToPlayer > speed) {
            double moveX = (differenceX / distanceToPlayer) * speed;
            double moveY = (differenceY / distanceToPlayer) * speed;

            setPos(x() + moveX, y() + moveY);
        }

        if (std::abs(differenceX) > std::abs(differenceY)) {
            if (differenceX > 0) {
                setPixmap(imgRight);
            } else {
                setPixmap(imgLeft);
            }
        } else {
            if (differenceY > 0) {
                setPixmap(imgForward);
            } else {
                setPixmap(imgBack);
            }
        }
    }
}

void Level1Enemy::shootHomingProjectile() {
    QGraphicsScene* currentScene = scene();

    if (currentScene == nullptr) {
        return;
    }

    QGraphicsPixmapItem* projectile = currentScene->addPixmap(imgProjectile);
    projectile->setScale(0.15);
    projectile->setPos(this->x(), this->y());
    projectile->setZValue(999);

    QTimer* homingTimer = new QTimer();
    homingTimer->setParent(this);

    int lifetimeMs = 9000;
    QElapsedTimer lifetime;
    lifetime.start();

    connect(homingTimer, &QTimer::timeout, this, [=]() mutable {
        if (player == nullptr) {
            homingTimer->stop();
            currentScene->removeItem(projectile);
            delete projectile;
            homingTimer->deleteLater();
            return;
        }

        if (lifetime.elapsed() >= lifetimeMs) {
            homingTimer->stop();
            currentScene->removeItem(projectile);
            delete projectile;
            homingTimer->deleteLater();
            return;
        }

        double playerX = player->getX();
        double playerY = player->getY();
        double projectileX = projectile->x();
        double projectileY = projectile->y();

        double distanceX = playerX - projectileX;
        double distanceY = playerY - projectileY;
        double distanceToTarget = std::sqrt((distanceX * distanceX) + (distanceY * distanceY));

        if (distanceToTarget < 10) {
            fireballHitSound->play();
            player->deductScore(10);

            homingTimer->stop();
            currentScene->removeItem(projectile);
            delete projectile;
            homingTimer->deleteLater();
            return;
        }

        if (distanceToTarget < 0.001) {
            distanceToTarget = 0.001;
        }

        double normalX = distanceX / distanceToTarget;
        double normalY = distanceY / distanceToTarget;

        projectile->setPos(projectileX + (normalX * 3.5), projectileY + (normalY * 3.5));
    });

    homingTimer->start(50);
}