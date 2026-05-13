#include "Level2enemy.h"
#include "player.h"

#include <QGraphicsScene>
#include <QTimer>
#include <QRandomGenerator>
#include <cmath>

/* ================================================================
   Level2Enemy — chases the player directly in the dark room
   ================================================================ */

Level2Enemy::Level2Enemy(Player* target, QGraphicsPixmapItem* pSprite)
    : Enemy(100, 15, 2.0)
{
    player       = target;
    playerSprite = pSprite;

    wanderChangeMs = 0;
    wanderCooldown = 0;

    // Glow ellipse — red/purple eerie effect around the wraith
    glow = new QGraphicsEllipseItem(-140, -140, 280, 280);
    glow->setBrush(QBrush(QColor(180, 60, 255, 50)));
    glow->setPen(Qt::NoPen);
    glow->setZValue(800);

    loadAssets();
    setPixmap(imgIdle);
    setScale(0.9);
    setZValue(860);

    pickNewTarget();

    QTimer* aiTimer = new QTimer(this);
    connect(aiTimer, &QTimer::timeout, this, &Level2Enemy::updateAI);
    aiTimer->start(33);
}

/* ------------------------------------------------------------------ */
void Level2Enemy::loadAssets()
{
    // Use spirit sprites which are already in the qrc
    imgIdle .load(":/new/prefix1/images/spirit front.png");
    imgRight.load(":/new/prefix1/images/spirit right.png");
    imgLeft .load(":/new/prefix1/images/spirit left.png");
    imgDown .load(":/new/prefix1/images/spirit front.png");
    imgUp   .load(":/new/prefix1/images/spirit back.png");

    // Fallback if any are null
    if (imgRight.isNull()) imgRight = imgIdle;
    if (imgLeft .isNull()) imgLeft  = imgIdle;
    if (imgDown .isNull()) imgDown  = imgIdle;
    if (imgUp   .isNull()) imgUp    = imgIdle;
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
    QPixmap px = pixmap();
    double cx = x() + (px.width()  * scale()) / 2.0;
    double cy = y() + (px.height() * scale()) / 2.0;
    glow->setPos(cx, cy);
}

/* ------------------------------------------------------------------ */
void Level2Enemy::checkPlayerCollision()
{
    if (!player) return;

    double px   = player->getX();
    double py   = player->getY();
    double ex   = x() + (pixmap().width()  * scale()) / 2.0;
    double ey   = y() + (pixmap().height() * scale()) / 2.0;
    double dist = std::sqrt((px - ex)*(px - ex) + (py - ey)*(py - ey));

    if (dist < 55)
    {
        emit wraithHitPlayer();
        // Teleport wraith away
        setPos(120 + QRandomGenerator::global()->bounded(1100),
               120 + QRandomGenerator::global()->bounded(640));
    }
}

/* ------------------------------------------------------------------ */
void Level2Enemy::updateAI()
{
    if (!player) return;

    double dx   = player->getX() - x();
    double dy   = player->getY() - y();
    double dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 5.0)
    {
        double nx = dx / dist;
        double ny = dy / dist;
        setPos(x() + nx * speed, y() + ny * speed);

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
