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
    : Enemy(120, 10, 1.0)
{
    paused       = false;
    player       = target;
    playerSprite = pSprite;
    isChasing    = false;
    shootCooldownMs = 10000;

    loadAssets();
    setPixmap(imgIdle);
    setScale(0.8);
    setZValue(900);

    hitSound = new QSoundEffect(this);
    hitSound->setSource(QUrl("qrc:/new/prefix1/sounds/fireball boom.wav"));
    hitSound->setVolume(1.0);

    fireballHitSound = new QSoundEffect(this);
    fireballHitSound->setSource(QUrl("qrc:/new/prefix1/sounds/fireball boom.wav"));
    fireballHitSound->setVolume(1.0);

    QTimer* aiTimer = new QTimer(this);
    connect(aiTimer, &QTimer::timeout, this, &Level1Enemy::updateAI);
    aiTimer->start(33);
}

void Level1Enemy::setPaused(bool value)
{
    paused = value;
}

/* ------------------------------------------------------------------ */
void Level1Enemy::loadAssets()
{
    imgIdle     .load(":/new/prefix1/images/spirit front.png");
    imgDown     .load(":/new/prefix1/images/spirit front.png");
    imgDownLeft .load(":/new/prefix1/images/spirit front.png");
    imgLeft     .load(":/new/prefix1/images/spirit left.png");
    imgUpLeft   .load(":/new/prefix1/images/spirit back.png");
    imgUp       .load(":/new/prefix1/images/spirit back.png");
    imgUpRight  .load(":/new/prefix1/images/spirit back.png");
    imgRight    .load(":/new/prefix1/images/spirit right.png");
    imgDownRight.load(":/new/prefix1/images/spirit front.png");
    imgAttack   .load(":/new/prefix1/images/spirit front.png");
    imgProjectile.load(":/new/prefix1/images/fireball.png");

    if (imgDown     .isNull()) imgDown      = imgIdle;
    if (imgDownLeft .isNull()) imgDownLeft  = imgIdle;
    if (imgLeft     .isNull()) imgLeft      = imgIdle;
    if (imgUpLeft   .isNull()) imgUpLeft    = imgIdle;
    if (imgUp       .isNull()) imgUp        = imgIdle;
    if (imgUpRight  .isNull()) imgUpRight   = imgIdle;
    if (imgRight    .isNull()) imgRight     = imgIdle;
    if (imgDownRight.isNull()) imgDownRight = imgIdle;
    if (imgAttack   .isNull()) imgAttack    = imgIdle;

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
    if (paused)  return;

    double differenceX     = player->getX() - this->x();
    double differenceY     = player->getY() - this->y();
    double distanceToPlayer = std::sqrt(differenceX * differenceX + differenceY * differenceY);

    if (distanceToPlayer < 700)
        isChasing = true;

    if (!isChasing) return;

    // --- Shoot cooldown ---
    shootCooldownMs -= 33;
    if (shootCooldownMs <= 0)
    {
        shootCooldownMs = 10000;
        setPixmap(imgAttack);
        shootHomingProjectile();
    }

    // --- Chase movement ---
    if (distanceToPlayer > speed)
    {
        double moveX = (differenceX / distanceToPlayer) * speed;
        double moveY = (differenceY / distanceToPlayer) * speed;
        setPos(x() + moveX, y() + moveY);
    }

    // --- 8-directional sprite selection ---
    double angle = std::atan2(differenceY, differenceX) * 180.0 / M_PI;

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
    if (paused) return;

    QGraphicsScene* scn = scene();
    if (!scn) return;

    QGraphicsPixmapItem* proj = scn->addPixmap(imgProjectile);
    proj->setScale(0.15);
    proj->setPos(this->x(), this->y());
    proj->setZValue(999);

    QTimer* homingTimer = new QTimer(this);
    QElapsedTimer lifetime;
    lifetime.start();
    const int lifetimeMs = 6000;

    connect(homingTimer, &QTimer::timeout, this, [=]() mutable {

        if (paused) return;

        if (!player || lifetime.elapsed() >= lifetimeMs)
        {
            homingTimer->stop();
            if (scn) scn->removeItem(proj);
            delete proj;
            homingTimer->deleteLater();
            return;
        }

        double playerX     = player->getX();
        double playerY     = player->getY();
        double projectileX = proj->x();
        double projectileY = proj->y();
        double distanceX   = playerX - projectileX;
        double distanceY   = playerY - projectileY;
        double distToTarget = std::sqrt(distanceX * distanceX + distanceY * distanceY);

        if (distToTarget < 12)
        {
            fireballHitSound->play();
            player->registerFireballHit();

            homingTimer->stop();
            scn->removeItem(proj);
            delete proj;
            homingTimer->deleteLater();

            if (player->isDead())
                emit playerKilled();

            return;
        }

        if (distToTarget < 0.001) distToTarget = 0.001;
        proj->setPos(projectileX + (distanceX / distToTarget) * 4,
                     projectileY + (distanceY / distToTarget) * 4);
    });

    homingTimer->start(33);
}
