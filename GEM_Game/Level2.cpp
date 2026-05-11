#include "Level2.h"
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>

Level2::Level2()
{
    overlay = nullptr;

    playerGlow = nullptr;
}

/* ================================================================
   LOAD LEVEL 2
   ================================================================ */

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
        1000,
        580,
        0.18
        );

    addRock(
        scene,
        ":/new/prefix1/images/rock_N.png",
        650,
        250,
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

/* ================================================================
   ADD ROCK
   ================================================================ */

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

    rock->setZValue(900);

    // IMPORTANT
    rock->setData(0, "rock");

    artifacts.push_back(rock);
}

/* ================================================================
   UPDATE PLAYER LIGHT
   ================================================================ */

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
