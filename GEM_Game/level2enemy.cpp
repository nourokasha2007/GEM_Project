#include "level2enemy.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <cmath>

Level2Enemy::Level2Enemy(Player* target, QGraphicsPixmapItem* pSprite)
    : Enemy(100, 10, 1.5) {

    paused = false;
    player = target;
    playerSprite = pSprite;
    isChasing = false;
    strikeCooldownMs = 0;
    strikeCount = 0;
    hasFirstScreechPlayed = false;

    loadAssets();
    setPixmap(imgIdle);
    setScale(0.05);

    screechSound = new QSoundEffect(this);
    screechSound->setSource(QUrl("qrc:/new/prefix1/sounds/ghost sound.wav"));
    screechSound->setVolume(1.0);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level2Enemy::updateAI);
    timer->start(33);
}

void Level2Enemy::setPaused(bool value) {
    paused = value;
}

void Level2Enemy::loadAssets() {
    imgIdle.load(":/new/prefix1/images/ghost idle-.png");
    imgForward.load(":/new/prefix1/images/ghost front-.png");
    imgBack.load(":/new/prefix1/images/ghost back-.png");
    imgLeft.load(":/new/prefix1/images/ghost left-.png");
    imgRight.load(":/new/prefix1/images/ghost right-.png");
}

void Level2Enemy::updateAI() {
    if (player == nullptr) {
        return;
    }

    if (paused) {
        return;
    }

    double differenceX = player->getX() - this->x();
    double differenceY = player->getY() - this->y();
    double distanceToPlayer = std::sqrt((differenceX * differenceX) + (differenceY * differenceY));

    if (distanceToPlayer < 700) {
        isChasing = true;
    }

    if (isChasing == true) {
        if (strikeCooldownMs > 0) {
            strikeCooldownMs = strikeCooldownMs - 33;
        }

        if (distanceToPlayer < 150) {
            if (hasFirstScreechPlayed == false) {
                screechSound->play();
                hasFirstScreechPlayed = true;
            }
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

        if (distanceToPlayer <= 30) {
            if (strikeCooldownMs <= 0) {
                strikeCooldownMs = 2000;
                strikeCount = strikeCount + 1;

                emit reduceSpeed();

                if (strikeCount >= 3) {
                    screechSound->play();
                    emit ghostScreech();
                    paused = true;
                }
            }
        }
    }
}