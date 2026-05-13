#include "level1enemy.h"
#include "player.h"

#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include <QPainter>
#include <cmath>

/* ================================================================
   Level1Enemy — Golden Hawk Spirit Guardian
   ================================================================ */

Level1Enemy::Level1Enemy(Player* target, QGraphicsPixmapItem* pSprite)
    : Enemy(120, 10, 1.0)   // reduced speed: 1.6 → 1.0 so player can outrun it
{
    player       = target;
    playerSprite = pSprite;
    isChasing    = false;
    shootCooldownMs = 10000;   // fires every 10 seconds (was 6) — gives player breathing room

    loadAssets();
    setPixmap(imgIdle);
    setScale(3.0);
    setZValue(900);

    // Hit sound — reuse fireball boom or swap for your own sfx
    hitSound = new QSoundEffect(this);
    hitSound->setSource(QUrl("qrc:/new/prefix1/sounds/fireball boom.wav"));
    hitSound->setVolume(1.0);

    QTimer* aiTimer = new QTimer(this);
    connect(aiTimer, &QTimer::timeout, this, &Level1Enemy::updateAI);
    aiTimer->start(33);   // ~30 fps
}

/* ------------------------------------------------------------------ */
void Level1Enemy::loadAssets()
{
    // ---------------------------------------------------------------
    // Replace these paths with your actual hawk-spirit sprite images
    // once you add them to your .qrc file, e.g.:
    //   :/new/prefix1/images/hawk_idle.png
    //   :/new/prefix1/images/hawk_down.png  ... etc.
    //
    // The sprite sheet you provided has 8 directional sprites:
    //   DOWN, DOWN_LEFT, LEFT, UP_LEFT, UP, UP_RIGHT, RIGHT, DOWN_RIGHT
    // plus IDLE, ATTACK (used when firing the sun orb).
    //
    // Until you add the real art, the paths below are placeholders
    // that match the naming convention — just add the files to your
    // .qrc and the code will pick them up automatically.
    // ---------------------------------------------------------------

    imgIdle     .load(":/new/prefix1/images/hawk_idle.png");
    imgDown     .load(":/new/prefix1/images/hawk_down.png");
    imgDownLeft .load(":/new/prefix1/images/hawk_downleft.png");
    imgLeft     .load(":/new/prefix1/images/hawk_left.png");
    imgUpLeft   .load(":/new/prefix1/images/hawk_upleft.png");
    imgUp       .load(":/new/prefix1/images/hawk_up.png");
    imgUpRight  .load(":/new/prefix1/images/hawk_upright.png");
    imgRight    .load(":/new/prefix1/images/hawk_right.png");
    imgDownRight.load(":/new/prefix1/images/hawk_downright.png");
    imgAttack   .load(":/new/prefix1/images/hawk_attack.png");
    imgProjectile.load(":/new/prefix1/images/hawk_orb.png");

    // --- Fallback: if any sprite failed to load, reuse idle ---
    // (so the game still runs even with missing art)
    if (imgDown     .isNull()) imgDown      = imgIdle;
    if (imgDownLeft .isNull()) imgDownLeft  = imgIdle;
    if (imgLeft     .isNull()) imgLeft      = imgIdle;
    if (imgUpLeft   .isNull()) imgUpLeft    = imgIdle;
    if (imgUp       .isNull()) imgUp        = imgIdle;
    if (imgUpRight  .isNull()) imgUpRight   = imgIdle;
    if (imgRight    .isNull()) imgRight     = imgIdle;
    if (imgDownRight.isNull()) imgDownRight = imgIdle;
    if (imgAttack   .isNull()) imgAttack    = imgIdle;

    // Projectile fallback: draw a small yellow circle via a plain pixmap
    if (imgProjectile.isNull())
    {
        QPixmap orb(24, 24);
        orb.fill(Qt::transparent);
        QPainter p(&orb);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(QColor(255, 200, 50, 220));
        p.setPen(QPen(QColor(255, 240, 100), 2));
        p.drawEllipse(2, 2, 20, 20);
        imgProjectile = orb;
    }
}

/* ------------------------------------------------------------------ */
void Level1Enemy::updateAI()
{
    if (!player) return;

    double dx       = player->getX() - this->x();
    double dy       = player->getY() - this->y();
    double distance = std::sqrt(dx * dx + dy * dy);

    // Start chasing once within 650 px
    if (distance < 650)
        isChasing = true;

    if (!isChasing) return;

    // --- Shoot cooldown ---
    shootCooldownMs -= 33;
    if (shootCooldownMs <= 0)
    {
        shootCooldownMs = 10000;   // 10 s between shots
        setPixmap(imgAttack);
        shootHomingProjectile();
    }

<<<<<<< Updated upstream
    // --- Chase movement ---
    if (distance > speed)
    {
        double nx = dx / distance;
        double ny = dy / distance;
        setPos(x() + nx * speed, y() + ny * speed);
=======
    if (isChasing == true) {
        // Handle shooting cooldown
        shootCooldownMs = shootCooldownMs - 33;

        if (shootCooldownMs <= 0) {
            shootCooldownMs = 12000;
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
>>>>>>> Stashed changes
    }

    // --- 8-directional sprite selection ---
    // Use atan2 to pick the closest of 8 directions
    double angle = std::atan2(dy, dx) * 180.0 / M_PI;
    // angle: right=0, down=90, left=±180, up=-90

    if      (angle >= -22.5  && angle <  22.5)  setPixmap(imgRight);
    else if (angle >=  22.5  && angle <  67.5)  setPixmap(imgDownRight);
    else if (angle >=  67.5  && angle < 112.5)  setPixmap(imgDown);
    else if (angle >= 112.5  && angle < 157.5)  setPixmap(imgDownLeft);
    else if (angle >= 157.5  || angle < -157.5) setPixmap(imgLeft);
    else if (angle >= -157.5 && angle < -112.5) setPixmap(imgUpLeft);
    else if (angle >= -112.5 && angle < -67.5)  setPixmap(imgUp);
    else if (angle >= -67.5  && angle < -22.5)  setPixmap(imgUpRight);
}

/* ------------------------------------------------------------------ */
void Level1Enemy::shootHomingProjectile()
{
    QGraphicsScene* scn = scene();
    if (!scn) return;

    QGraphicsPixmapItem* proj = scn->addPixmap(imgProjectile);
    proj->setScale(0.8);
    proj->setPos(this->x(), this->y());
    proj->setZValue(999);

    QTimer*      homingTimer = new QTimer(this);
    QElapsedTimer lifetime;
    lifetime.start();
    const int lifetimeMs = 6000;   // despawns after 6 s (was 8)

    connect(homingTimer, &QTimer::timeout, this, [=]() mutable {

        // Despawn if owner destroyed or lifetime expired
        if (!player || lifetime.elapsed() >= lifetimeMs)
        {
            homingTimer->stop();
            if (scn) scn->removeItem(proj);
            delete proj;
            homingTimer->deleteLater();
            return;
        }

        double px   = player->getX();
        double py   = player->getY();
        double bx   = proj->x();
        double by   = proj->y();
        double vx   = px - bx;
        double vy   = py - by;
        double dist = std::sqrt(vx * vx + vy * vy);

        // Hit detection — within 12 px
        if (dist < 12)
        {
            hitSound->play();
            player->deductScore(10);
            homingTimer->stop();
            scn->removeItem(proj);
            delete proj;
            homingTimer->deleteLater();
            return;
        }

        if (dist < 0.001) dist = 0.001;
        proj->setPos(bx + (vx / dist) * 2.5,   // slow orb: was 5.0
                     by + (vy / dist) * 2.5);
    });

    homingTimer->start(33);   // projectile ticks at ~30 fps
}
