#include "Level2.h"
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPixmap>

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
        0.09
        );

    addRock(
        scene,
        ":/new/prefix1/images/rock_A-3.png",
        850,
        600,
        0.09
        );

    addRock(
        scene,
        ":/new/prefix1/images/rock_N.png",
        300,
        700,
        0.09
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
    QPixmap px(imagePath);

    QGraphicsPixmapItem* rock =
        scene->addPixmap(px);

    rock->setShapeMode(
        QGraphicsPixmapItem::MaskShape
        );

    rock->setPos(x, y);

    rock->setScale(scale);

    rock->setTransformationMode(Qt::SmoothTransformation);

    rock->setPixmap(rock->pixmap().copy());

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
