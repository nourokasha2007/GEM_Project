#include "Level2.h"
<<<<<<< HEAD

=======
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>
<<<<<<< HEAD
#include <QFont>
#include <QGraphicsTextItem>

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

/* ------------------------------------------------------------------ */
void Level2::loadScene(QGraphicsScene* scene)
{
    /* ---- Background: dark Egyptian tomb ---- */
    QPixmap bg(":/new/prefix1/images/level2_tomb.png");
    background = scene->addPixmap(bg);
    scene->setSceneRect(bg.rect());

    /* ---- Hieroglyph rocks — spread far across the room floor ---- */
    double s = 0.20;

    addRock(scene, ":/new/prefix1/images/rock_M.png",   220, 450, s);
    addRock(scene, ":/new/prefix1/images/rock_A-3.png", 1080, 580, s);
    addRock(scene, ":/new/prefix1/images/rock_N.png",   650, 230, s);

    /* ---- Exit door ---- */
    // Drawn as a golden rectangle on the right wall.
    // Replace with a proper sprite if you have one.
    doorItem = scene->addPixmap(QPixmap());   // invisible placeholder
    doorItem->setPos(1250, 350);

    // Door outline — visible even in the dark so the player has
    // a vague sense of "there is a door somewhere".
    QGraphicsRectItem* doorFrame = scene->addRect(
        1250, 330, 80, 160,
        QPen(QColor(100, 80, 30, 120), 3),
        QBrush(QColor(60, 40, 10, 80))
        );
    doorFrame->setZValue(10);

    // Golden light — hidden until all rocks are collected
    doorLight = scene->addRect(
        1240, 320, 100, 180,
        QPen(Qt::NoPen),
        QBrush(QColor(255, 215, 0, 0))   // alpha = 0 → invisible
        );
    doorLight->setZValue(9);

    /* ---- Player ambient glow ---- */
    // Large bright circle so the player can clearly see themselves
    // and a meaningful area around them. Radius 220 px.
    playerGlow = scene->addEllipse(
        -220, -220, 440, 440,
        QPen(Qt::NoPen),
        QBrush(QColor(255, 230, 150, 120))   // brighter: alpha 120 (was 55)
        );
    playerGlow->setZValue(802);   // above overlay AND wraith glow

    /* ---- Dark overlay ---- */
    // Reduced to 150 opacity (was 210) so player can see the room
    // silhouette while it still feels dark and atmospheric.
    overlay = scene->addRect(
        scene->sceneRect(),
        QPen(Qt::NoPen),
        QBrush(QColor(0, 0, 0, 150))   // lighter dark: was 210
        );
    overlay->setZValue(800);
}

/* ------------------------------------------------------------------ */
void Level2::addRock(QGraphicsScene* scene,
                     const QString& imagePath,
                     int x, int y, double scale)
{
    QGraphicsPixmapItem* item = scene->addPixmap(QPixmap(imagePath));
    item->setPos(x, y);
    item->setScale(scale);
    item->setZValue(900);        // above overlay (800) AND player glow (802) so rocks are always visible
    item->setData(0, "rock");   // type tag used by game.collectArtifact()
    artifacts.push_back(item);
}

/* ------------------------------------------------------------------ */
void Level2::updatePlayerGlow(QGraphicsPixmapItem* playerSprite)
{
    if (!playerGlow || !playerSprite) return;

    QPixmap px  = playerSprite->pixmap();
    double  sc  = playerSprite->scale();
    double  cx  = playerSprite->x() + (px.width()  * sc) / 2.0;
    double  cy  = playerSprite->y() + (px.height() * sc) / 2.0;
    playerGlow->setPos(cx, cy);
}

/* ------------------------------------------------------------------ */
void Level2::illuminateDoor()
{
    if (!doorLight) return;

    // Animate the alpha from 0 → 200 in steps using a QTimer
    // (we can't use QPropertyAnimation on a plain QGraphicsRectItem,
    //  so we step it manually).
    QColor c = doorLight->brush().color();
    c.setAlpha(200);
    doorLight->setBrush(QBrush(c));

    // Also widen the glow rectangle slightly for drama
    doorLight->setRect(1220, 300, 140, 220);
}

/* ------------------------------------------------------------------ */
QPointF Level2::doorPosition() const
{
    // Centre of the exit door rectangle
    return QPointF(1290, 410);
}
=======

/* ================= CONSTRUCTOR ================= */

Level2::Level2()
{
    overlay = nullptr;

    playerGlow = nullptr;

 //================ LOAD COLLISION MASK =================//

    collisionMask.load( ":/new/prefix1/images/Level2 - BW.png" );
}

/* ================= LOAD LEVEL ================= */

void Level2::loadScene(
    QGraphicsScene* scene
    )
{
    //================ BACKGROUND =================//

    QPixmap bg(":/new/prefix1/images/Level2.jpeg");

    background =
        scene->addPixmap(bg);

    scene->setSceneRect(
        bg.rect()
        );

    //================ ROCKS =================//

    addRock(
        scene,
        ":/new/prefix1/images/rock_M.png",
        220,
        450,
        0.18
        );

    addRock(
        scene,
        ":/new/prefix1/images/rock_A-3.png",
        850,
        600,
        0.18
        );

    addRock(
        scene,
        ":/new/prefix1/images/rock_N.png",
        300,
        700,
        0.18
        );

    //================ PLAYER GLOW =================//

    playerGlow =
        scene->addEllipse(
            -220,
            -220,
            440,
            440,
            QPen(Qt::NoPen),
            QBrush(
                QColor(
                    255,
                    220,
                    120,
                    120
                    )
                )
            );

    playerGlow->setZValue(800);

    //================ DARK OVERLAY =================//

    overlay =
        scene->addRect(
            scene->sceneRect(),
            QPen(Qt::NoPen),
            QBrush(
                QColor(
                    0,
                    0,
                    0,
                    210
                    )
                )
            );

    overlay->setZValue(799);
}

/* ================= ADD ROCK ================= */

void Level2::addRock(
    QGraphicsScene* scene,
    const QString& imagePath,
    int x,
    int y,
    double scale
    )
{
    QGraphicsPixmapItem* rock =
        scene->addPixmap(
            QPixmap(imagePath)
            );

    rock->setPos(x, y);

    rock->setScale(scale);

    rock->setZValue(200);

    //================ TYPE =================//

    rock->setData(0, "rock");

    artifacts.push_back(rock);
}

/* ================= UPDATE PLAYER LIGHT ================= */

void Level2::updatePlayerGlow(
    QGraphicsPixmapItem* playerSprite
    )
{
    if(!playerGlow || !playerSprite)
    {
        return;
    }

    QPixmap px =
        playerSprite->pixmap();

    double scale =
        playerSprite->scale();

    double cx =
        playerSprite->x()
        + (px.width() * scale) / 2.0;

    double cy =
        playerSprite->y()
        + (px.height() * scale) / 2.0;

    playerGlow->setPos(cx, cy);
}
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870
