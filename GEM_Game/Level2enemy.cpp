#include "level2enemy.h"
#include "player.h"

#include <QGraphicsScene>
#include <QTimer>
#include <QRandomGenerator>
#include <cmath>

/* ================================================================
   Level2Enemy — chases the player directly in the dark room
   ================================================================ */

Level2Enemy::Level2Enemy(Player* target, QGraphicsPixmapItem* pSprite)
    : Enemy(100, 15, 2.8)   // speed 2.8 — fast enough to be threatening
{
    player       = target;
    playerSprite = pSprite;

    wanderChangeMs = 0;
    wanderCooldown = 0;

    // Glow ellipse — warm amber torch effect around the enemy
    glow = new QGraphicsEllipseItem(-160, -160, 320, 320);
    glow->setBrush(QBrush(QColor(255, 180, 60, 55)));
    glow->setPen(Qt::NoPen);
    glow->setZValue(800);

    loadAssets();
    setPixmap(imgIdle);
    setScale(3.5);
    setZValue(860);

    pickNewTarget();

    QTimer* aiTimer = new QTimer(this);
    connect(aiTimer, &QTimer::timeout, this, &Level2Enemy::updateAI);
    aiTimer->start(33);
}

/* ------------------------------------------------------------------ */
void Level2Enemy::loadAssets()
{
    // Reuse the mummy sprites as a fallback — swap these paths once
    // you add proper wraith/creature sprites to your .qrc.
    //
    //   ":/new/prefix1/images/wraith_idle.png"   ← add your own art here
    //
    // Until then we fall back to the existing mummy sheets so the
    // game compiles and runs immediately.
    imgIdle .load(":/new/prefix1/images/lvl1 mummy idle(1).png");
    imgRight.load(":/new/prefix1/images/lvl1 mummy walking right(1).png");
    imgLeft .load(":/new/prefix1/images/lvl1mummy walking left(1).png");
    imgDown .load(":/new/prefix1/images/lvl1 mummy walking forward(1).png");
    imgUp   .load(":/new/prefix1/images/lvl1 mummy walking back(1).png");
}

/* ------------------------------------------------------------------ */
void Level2Enemy::pickNewTarget()
{
    targetX = 120 + QRandomGenerator::global()->bounded(1180);
    targetY = 120 + QRandomGenerator::global()->bounded(660);
    wanderChangeMs = wanderCooldown;
}

/* ------------------------------------------------------------------ */
void Level2Enemy::updateGlowPosition()
{
    if (!glow) return;
    // Centre the glow on the creature's centre pixel
    QPixmap px = pixmap();
    double cx = x() + (px.width()  * scale()) / 2.0;
    double cy = y() + (px.height() * scale()) / 2.0;
    glow->setPos(cx, cy);
}

/* ------------------------------------------------------------------ */
void Level2Enemy::checkPlayerCollision()
{
    if (!player) return;

    double px = player->getX();
    double py = player->getY();
    double ex = x() + (pixmap().width()  * scale()) / 2.0;
    double ey = y() + (pixmap().height() * scale()) / 2.0;

    double dist = std::sqrt((px - ex)*(px - ex) + (py - ey)*(py - ey));

    if (dist < 55)
    {
        player->deductScore(10);
        targetX = 120 + QRandomGenerator::global()->bounded(1100);
        targetY = 120 + QRandomGenerator::global()->bounded(640);
        setPos(targetX, targetY);
    }
}

/* ------------------------------------------------------------------ */
void Level2Enemy::updateAI()
{
    if (!player) return;

    // --- Always chase the player directly ---
    double dx   = player->getX() - x();
    double dy   = player->getY() - y();
    double dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 5.0)
    {
        double nx = dx / dist;
        double ny = dy / dist;
        setPos(x() + nx * speed, y() + ny * speed);

        // Directional sprite
        if (std::abs(dx) > std::abs(dy))
            setPixmap(dx > 0 ? imgRight : imgLeft);
        else
            setPixmap(dy > 0 ? imgDown : imgUp);
    }
    else
    {
        setPixmap(imgIdle);
    }

    updateGlowPosition();
    checkPlayerCollision();
}
