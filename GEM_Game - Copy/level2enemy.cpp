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
    strikeCount = 0;
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

int Level2Enemy::getStrikeCount() const
{
    return strikeCount;
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
    if(!scene())
    {
        return;
    }

    //================ 1. ALWAYS TICK DOWN COOLDOWN ================//

    if (strikeCooldownMs > 0) {
        strikeCooldownMs = strikeCooldownMs - 33;
    }

    //================ 2. CHECKS ================//

    if (player == nullptr) {
        return;
    }

    if (paused) {
        return;
    }

    //================ 3. DISTANCE ================//

    double differenceX = player->getX() - this->x();
    double differenceY = player->getY() - this->y();
    double distanceToPlayer = std::sqrt((differenceX * differenceX) + (differenceY * differenceY));

    if (distanceToPlayer < 1000) {
        isChasing = true;
    }

    //================ 4. CHASING LOGIC ================//

    if (isChasing == true) {

        //================ AUDIO CUE ================//

        if (distanceToPlayer < 70) {
            if (hasFirstScreechPlayed == false) {
                screechSound->play();
                hasFirstScreechPlayed = true;
            }
        }

        //================ MOVEMENT ================//

        if (distanceToPlayer > speed) {
            double moveX = (differenceX / distanceToPlayer) * speed;
            double moveY = (differenceY / distanceToPlayer) * speed;

            setPos(x() + moveX, y() + moveY);
        }

        //================ CHANGE SPRITE ================//

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

        //================ 5. STRIKE MECHANIC ================//
        if (distanceToPlayer <= 30) {
            if (strikeCooldownMs <= 0) {

                strikeCooldownMs = 4000;
                strikeCount = strikeCount + 1;


                if (screechSound->isPlaying()) {
                    screechSound->stop();
                }
                screechSound->play();

                emit reduceSpeed();


                double pushX = (differenceX / distanceToPlayer) * -100;
                double pushY = (differenceY / distanceToPlayer) * -100;
                setPos(x() + pushX, y() + pushY);


                if (strikeCount >= 3) {
                    emit ghostScreech();
                    paused = true;
                }
            }
        }

        }
    }
