#include "level2enemy.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <cmath>

/* ================= CONSTRUCTOR ================= */

Level2Enemy::Level2Enemy(Player* target, QGraphicsPixmapItem* pSprite)
    : Enemy(100, 10, 1.5) {

    //================ DEFAULT VALUES ================//

    paused = false;
    player = target;
    playerSprite = pSprite;
    isChasing = false;
    strikeCooldownMs = 0;
    hasFirstScreechPlayed = false;

    //================ LOAD ASSETS =================//

    loadAssets();
    setPixmap(imgIdle);
    setScale(0.1);

    //================ SOUND EFFECT =================//

    screechSound = new QSoundEffect(this);
    screechSound->setSource(QUrl("qrc:/new/prefix1/sounds/ghost sound.wav"));
    screechSound->setVolume(1.0);

    //================ TIMER =================//

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level2Enemy::updateAI);
    timer->start(33);
}

/* ================= PAUSE ================= */

void Level2Enemy::setPaused(bool value) {
    paused = value;
}

/* ================= LOAD ASSETS ================= */

void Level2Enemy::loadAssets() {
    imgIdle.load(":/new/prefix1/images/ghost idle-.png");
    imgForward.load(":/new/prefix1/images/ghost front-.png");
    imgBack.load(":/new/prefix1/images/ghost back-.png");
    imgLeft.load(":/new/prefix1/images/ghost left-.png");
    imgRight.load(":/new/prefix1/images/ghost right-.png");
}

/* ================= UPDATE AI ================= */

void Level2Enemy::updateAI() {

    //================ CHECKS =================//

    if (player == nullptr) {
        return;
    }

    if (paused) {
        return;
    }

    //================ DISTANCE =================//

    double differenceX = player->getX() - this->x();
    double differenceY = player->getY() - this->y();
    double distanceToPlayer = std::sqrt((differenceX * differenceX) + (differenceY * differenceY));

    if (distanceToPlayer < 700) {
        isChasing = true;
    }

    //================ CHASING LOGIC =================//

    if (isChasing == true) {

        //================ COOLDOWN =================//

        if (strikeCooldownMs > 0) {
            strikeCooldownMs = strikeCooldownMs - 33;
        }

        //================ AUDIO CUE =================//

        if (distanceToPlayer < 150) {
            if (hasFirstScreechPlayed == false) {
                screechSound->play();
                hasFirstScreechPlayed = true;
            }
        }

        //================ MOVEMENT =================//

        if (distanceToPlayer > speed) {
            double moveX = (differenceX / distanceToPlayer) * speed;
            double moveY = (differenceY / distanceToPlayer) * speed;

            setPos(x() + moveX, y() + moveY);
        }

        //================ CHANGE SPRITE =================//

        if (std::abs(differenceX) > std::abs(differenceY)) {
            if (differenceX > 0) {
                setPixmap(imgRight);
                setScale(0.1);
            } else {
                setPixmap(imgLeft);
                setScale(0.1);
            }
        } else {
            if (differenceY > 0) {
                setPixmap(imgForward);
                setScale(0.15);
            } else {
                setPixmap(imgBack);
                setScale(0.1);
            }
        }

        //================ STRIKE MECHANIC =================//


        if (distanceToPlayer <= 70) {
            if (strikeCooldownMs <= 0) {
                strikeCooldownMs = 2000;


                emit reduceSpeed();
                emit ghostScreech();


            }
        }
    }
}