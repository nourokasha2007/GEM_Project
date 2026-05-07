#include "level1enemy.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsPixmapItem>
#include <cmath>
#include <algorithm>

Level1Enemy::Level1Enemy(Player* target, QGraphicsPixmapItem* playerSprite)
    : Enemy(100, 10, 1.2), player(target), playerSprite(playerSprite) { // 100 HP, 1.2 Speed

    isChasing = false;
    shootCooldownMs = SHOT_COOLDOWN_MS; // shoot soon after aggro
    loadAssets();

    setPixmap(imgIdle); // Default look
    setScale(3.0);      // Boss size

    // AI Loop: Runs every 33ms (~30 FPS)
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Level1Enemy::updateAI);
    timer->start(33);
}

void Level1Enemy::loadAssets() {
    imgIdle.load(":/new/prefix1/images/lvl1 mummy idle.png");
    imgForward.load(":/new/prefix1/images/lvl1 mummy walking forward.png");
    imgBack.load(":/new/prefix1/images/lvl1 mummy walking back.png");
    imgLeft.load(":/new/prefix1/images/lvl1mummy walking left.png");
    imgRight.load(":/new/prefix1/images/lvl1 mummy walking right.png");

    // Projectile image (homing bullet)
    imgProjectile.load(":/new/prefix1/images/fireball.png");
}

void Level1Enemy::updateAI() {
    if (!player) return;

    // 1. Check distance for aggro
    double dx = player->getX() - this->x();
    double dy = player->getY() - this->y();
    double distance = std::sqrt(dx * dx + dy * dy);

    // 2. Start chase if player is within 400 pixels
    if (distance < 400)
        isChasing = true;

    // Shoot periodically when chasing
    if (isChasing) {
        shootCooldownMs -= 33;
        if (shootCooldownMs <= 0) {
            shootCooldownMs = SHOT_COOLDOWN_MS;
            shootHomingProjectile();
        }
    }

    if (isChasing) {
        // 3. Movement and Sprite Swapping (simple)
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
    if (!scn)
        return;

    QGraphicsPixmapItem* proj = scn->addPixmap(imgProjectile);
    proj->setScale(0.15);
    proj->setPos(this->x(), this->y());

    // Homing tick + lifetime
    const int tickMs = 50;   // move every 50ms
    const int lifetimeMs = 5000; // disappear after 5 seconds

    QTimer* homingTimer = new QTimer();
    homingTimer->setParent(this);
    homingTimer->setInterval(tickMs);

    int elapsed = 0;

    connect(homingTimer, &QTimer::timeout, this, [=]() mutable {

        elapsed += tickMs;

        if (!player || elapsed >= lifetimeMs) {
            homingTimer->stop();
            scn->removeItem(proj);
            delete proj;
            homingTimer->deleteLater();
            return;
        }

        // Collision with player sprite.
        // The default collision uses QGraphicsItem::boundingRect(), which for pixmaps can be larger than the visible sprite.
        // Fix: use a manual circle-vs-circle hit test using a tighter radius.
        if (playerSprite) {
            // playerSprite->pixmap().size() is in source pixels; scale() is already applied to the item.
            // Use a conservative fraction so the hitbox matches the visible character better.
            const QSizeF pSize = playerSprite->pixmap().isNull() ? QSizeF(64, 64) : QSizeF(playerSprite->pixmap().size());
            const double playerRadius = 0.5 * std::min(pSize.width(), pSize.height()) * 0.08; // tuned for your player scale

            // Projectiles are small; approximate them as circles too.
            const QSizeF projSize = proj->pixmap().isNull() ? QSizeF(16, 16) : QSizeF(proj->pixmap().size());
            const double projRadius = 0.5 * std::min(projSize.width(), projSize.height()) * 0.15 * 0.15;

            const double dxHit = proj->x() - playerSprite->x();
            const double dyHit = proj->y() - playerSprite->y();
            const double rSum = playerRadius + projRadius;

            if (dxHit * dxHit + dyHit * dyHit <= rSum * rSum) {
                // Apply damage
                player->takeDamage(10);

                // Remove projectile
                homingTimer->stop();
                scn->removeItem(proj);
                delete proj;
                homingTimer->deleteLater();
                return;
            } // <-- FIXED: Added missing closing brace for the 'if' collision check here
        }

        // Vector from bullet to player
        double px = player->getX();
        double py = player->getY();
        double bx = proj->x();
        double by = proj->y();

        double vx = px - bx;
        double vy = py - by;

        double dist = std::sqrt(vx * vx + vy * vy);
        if (dist < 0.001)
            dist = 0.001;

        double nx = vx / dist;
        double ny = vy / dist;

        proj->setPos(bx + nx * PROJECTILE_SPEED, by + ny * PROJECTILE_SPEED);
    });

    homingTimer->start();
} // <-- FIXED: Removed the garbage characters (;};)}) at the end of the file