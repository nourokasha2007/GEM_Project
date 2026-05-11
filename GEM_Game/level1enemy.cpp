
#include "level1enemy.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <cmath>
#include <QElapsedTimer>

Level1Enemy::Level1Enemy(Player* target, QGraphicsPixmapItem* pSprite)
    : Enemy(100, 10, 1.2) {

    // Initialize basic enemy states
    paused = false;
    player = target;
    playerSprite = pSprite;
    isChasing = false;
    shootCooldownMs = 9000;

    // Load images and set the starting size
    loadAssets();
    setPixmap(imgIdle);
    setScale(0.35);

    // Set up the fireball sound effect
    fireballHitSound = new QSoundEffect(this);
    fireballHitSound->setSource(QUrl("qrc:/new/prefix1/sounds/fireball boom.wav"));
    fireballHitSound->setVolume(1.0);

    // Create a timer to run the AI logic roughly 30 times a second
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level1Enemy::updateAI);
    timer->start(33);
}

void Level1Enemy::setPaused(bool value) {
    paused = value;
}

void Level1Enemy::loadAssets() {
    // Load all the different direction sprites
    imgIdle.load(":/new/prefix1/images/spirit front.png");
    imgForward.load(":/new/prefix1/images/spirit front.png");
    imgBack.load(":/new/prefix1/images/spirit back.png");
    imgLeft.load(":/new/prefix1/images/spirit left.png");
    imgRight.load(":/new/prefix1/images/spirit right.png");
    imgProjectile.load(":/new/prefix1/images/fireball.png");
}

void Level1Enemy::updateAI() {
    // Stop moving if the player is dead or the game is paused
    if (player == nullptr) {
        return;
    }

    if (paused) {
        return;
    }

    // Calculate distance between enemy and player
    double differenceX = player->getX() - this->x();
    double differenceY = player->getY() - this->y();
    double distanceToPlayer = std::sqrt((differenceX * differenceX) + (differenceY * differenceY));

    // Start chasing if the player gets close enough
    if (distanceToPlayer < 700) {
        isChasing = true;
    }

    if (isChasing == true) {
        // Handle shooting cooldown
        shootCooldownMs = shootCooldownMs - 33;

        if (shootCooldownMs <= 0) {
            shootCooldownMs = 6000;
            shootHomingProjectile();
        }

        // Move towards the player if not already touching them
        if (distanceToPlayer > speed) {
            double moveX = (differenceX / distanceToPlayer) * speed;
            double moveY = (differenceY / distanceToPlayer) * speed;

            setPos(x() + moveX, y() + moveY);
        }

        // Change the sprite based on which direction the enemy is moving the most
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
    // Prevent shooting if the game is paused
    if (paused) {
        return;
    }

    QGraphicsScene* currentScene = scene();

    if (currentScene == nullptr) {
        return;
    }

    // Create the fireball and add it to the scene
    QGraphicsPixmapItem* projectile = currentScene->addPixmap(imgProjectile);
    projectile->setScale(0.15);
    projectile->setPos(this->x(), this->y());
    projectile->setZValue(999);

    // Create a timer to control the fireball's movement
    QTimer* homingTimer = new QTimer();
    homingTimer->setParent(this);

    // Track how long the fireball has been alive
    int lifetimeMs = 9000;
    QElapsedTimer lifetime;
    lifetime.start();

    connect(homingTimer, &QTimer::timeout, this, [=]() mutable {
        if (paused) {
            return;
        }

        // Destroy the fireball if the player dies
        if (player == nullptr) {
            homingTimer->stop();
            currentScene->removeItem(projectile);
            delete projectile;
            homingTimer->deleteLater();
            return;
        }

        // Destroy the fireball if its lifetime runs out
        if (lifetime.elapsed() >= lifetimeMs) {
            homingTimer->stop();
            currentScene->removeItem(projectile);
            delete projectile;
            homingTimer->deleteLater();
            return;
        }

        // Calculate distance from the fireball to the player
        double playerX = player->getX();
        double playerY = player->getY();
        double projectileX = projectile->x();
        double projectileY = projectile->y();

        double distanceX = playerX - projectileX;
        double distanceY = playerY - projectileY;
        double distanceToTarget = std::sqrt((distanceX * distanceX) + (distanceY * distanceY));

        // Damage the player and destroy the fireball if it hits
        if (distanceToTarget < 10) {
            fireballHitSound->play();
            player->deductScore(10);
            player->registerFireballHit();

            homingTimer->stop();
            currentScene->removeItem(projectile);
            delete projectile;
            homingTimer->deleteLater();

            if (player->isDead()) {
                emit playerKilled();
            }

            return;
        }

        // Prevent division by zero error
        if (distanceToTarget < 0.001) {
            distanceToTarget = 0.001;
        }

        // Move the fireball slightly towards the player
        double normalX = distanceX / distanceToTarget;
        double normalY = distanceY / distanceToTarget;

        projectile->setPos(projectileX + (normalX * 4), projectileY + (normalY * 4));
    });

    homingTimer->start(50);
}
