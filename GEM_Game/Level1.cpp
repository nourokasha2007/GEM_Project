#include "Level1.h"
#include <QBrush>
#include <QPixmap>
#include <QPen>

/* ================= CONSTRUCTOR ================= */

Level1::Level1()
{
    //================ LOAD COLLISION MASK ================//

    collisionMask.load(
        ":/new/prefix1/images/level1 BW.png"
        );
}

/* ================= LOAD SCENE ================= */

void Level1::loadScene(
    QGraphicsScene *scene
    )
{
    //================ BACKGROUND ================//

    QPixmap bg(
        ":/new/prefix1/images/level1 (1).png"
        );

    background =
        scene->addPixmap(bg);

    scene->setSceneRect(
        bg.rect()
        );

    double s = 0.04;

    //================ COINS ================//

    addArtifact(
        scene,
        ":/new/prefix1/images/coint.png",
        "coin",
        310,
        500,
        s
        );

    addArtifact(
        scene,
        ":/new/prefix1/images/coint.png",
        "coin",
        520,
        620,
        s
        );

    //================ SCROLLS ================//

    addArtifact(
        scene,
        ":/new/prefix1/images/scrollt.png",
        "scroll",
        420,
        800,
        s
        );

    addArtifact(
        scene,
        ":/new/prefix1/images/scrollt.png",
        "scroll",
        720,
        760,
        s
        );

    //================ MASKS ================//

    addArtifact(
        scene,
        ":/new/prefix1/images/maskt.png",
        "mask",
        960,
        400,
        s
        );

    addArtifact(
        scene,
        ":/new/prefix1/images/maskt.png",
        "mask",
        900,
        850,
        s
        );

    //================ AMULETS ================//

    addArtifact(
        scene,
        ":/new/prefix1/images/amulett.png",
        "amulet",
        470,
        430,
        s
        );

    addArtifact(
        scene,
        ":/new/prefix1/images/amulett.png",
        "amulet",
        870,
        700,
        s
        );

    //================ TIMERS ================//

    addArtifact(
        scene,
        ":/new/prefix1/images/timert.png",
        "timer",
        610,
        410,
        s
        );

    addArtifact(
        scene,
        ":/new/prefix1/images/timert.png",
        "timer",
        850,
        540,
        s
        );
}

/* ================= ADD ARTIFACT ================= */

void Level1::addArtifact(
    QGraphicsScene* scene,
    QString imagePath,
    QString type,
    int x,
    int y,
    double scale
    )
{
    QGraphicsPixmapItem* item =
        scene->addPixmap(
            QPixmap(imagePath)
            );

    item->setPos(x, y);

    item->setScale(scale);

    item->setZValue(999);

    item->setData(0, type);

    artifacts.push_back(item);
}
