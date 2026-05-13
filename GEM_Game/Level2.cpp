#include "Level2.h"
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>

/* ================================================================
   Level2 — The Dark Hieroglyph Room
   ================================================================ */

Level2::Level2()
    : overlay(nullptr)
    , playerGlow(nullptr)
    , doorItem(nullptr)
    , doorLight(nullptr)
{
}

/* ================================================================
   LOAD SCENE
   ================================================================ */

void Level2::loadScene(QGraphicsScene* scene)
{
    //================ BACKGROUND =================//

    QPixmap bg(":/new/prefix1/images/Level2.jpeg");
    background = scene->addPixmap(bg);
    scene->setSceneRect(bg.rect());

    //================ ROCKS =================//
    // Z = 900 — above overlay (795) and glow (800) — always visible

    addRock(scene, ":/new/prefix1/images/rock_M.png",   220,  450, 0.18);
    addRock(scene, ":/new/prefix1/images/rock_A-3.png", 1000, 580, 0.18);
    addRock(scene, ":/new/prefix1/images/rock_N.png",   650,  250, 0.18);

    //================ EXIT DOOR =================//
    // Invisible placeholder — replaced by doorLight on completion

    doorItem = scene->addPixmap(QPixmap());
    doorItem->setPos(1250, 350);
    doorItem->setZValue(10);

    // Door frame — faint golden outline visible in the dark
    QGraphicsRectItem* doorFrame = scene->addRect(
        1250, 330, 80, 160,
        QPen(QColor(100, 80, 30, 120), 3),
        QBrush(QColor(60, 40, 10, 80))
        );
    doorFrame->setZValue(10);

    // Golden glow — alpha=0 (invisible) until illuminateDoor() is called
    doorLight = scene->addRect(
        1240, 320, 100, 180,
        QPen(Qt::NoPen),
        QBrush(QColor(255, 215, 0, 0))
        );
    doorLight->setZValue(9);

    //================ PLAYER GLOW =================//
    // Z = 802 — above overlay (795), visible to player

    playerGlow = scene->addEllipse(
        -220, -220, 440, 440,
        QPen(Qt::NoPen),
        QBrush(QColor(255, 220, 120, 120))
        );
    playerGlow->setZValue(802);

    //================ DARK OVERLAY =================//
    // Z = 795 — BELOW glow (802) and rocks (900) so both show through

    overlay = scene->addRect(
        scene->sceneRect(),
        QPen(Qt::NoPen),
        QBrush(QColor(0, 0, 0, 180))
        );
    overlay->setZValue(795);
}

/* ================================================================
   ADD ROCK
   ================================================================ */

void Level2::addRock(QGraphicsScene* scene,
                     const QString&  imagePath,
                     int x, int y, double scale)
{
    QGraphicsPixmapItem* item = scene->addPixmap(QPixmap(imagePath));
    item->setPos(x, y);
    item->setScale(scale);
    item->setZValue(900);
    item->setData(0, "rock");
    artifacts.push_back(item);
}

/* ================================================================
   UPDATE PLAYER GLOW
   ================================================================ */

void Level2::updatePlayerGlow(QGraphicsPixmapItem* playerSprite)
{
    if (!playerGlow || !playerSprite) return;

    QPixmap px = playerSprite->pixmap();
    double  sc = playerSprite->scale();
    double  cx = playerSprite->x() + (px.width()  * sc) / 2.0;
    double  cy = playerSprite->y() + (px.height() * sc) / 2.0;
    playerGlow->setPos(cx, cy);
}

/* ================================================================
   ILLUMINATE DOOR — called when all rocks collected
   ================================================================ */

void Level2::illuminateDoor()
{
    if (!doorLight) return;

    QColor c = doorLight->brush().color();
    c.setAlpha(200);
    doorLight->setBrush(QBrush(c));
    doorLight->setRect(1220, 300, 140, 220);
}

/* ================================================================
   DOOR POSITION — used by GameWindow for collision detection
   ================================================================ */

QPointF Level2::doorPosition() const
{
    return QPointF(1290, 410);
}
