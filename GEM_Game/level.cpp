#include "level.h"

#include <QColor>

/* ================= CONSTRUCTOR ================= */

Level::Level()
{
    background = nullptr;
}

/* ================= DESTRUCTOR ================= */

Level::~Level()
{

}

/* ================= GET COLLISION MASK ================= */

QImage Level::getCollisionMask() const
{
    return collisionMask;
}

/* ================= WALKABLE ================= */

bool Level::isWalkable(
    QPointF newPos,
    QRectF playerRect,
    QRectF sceneRect
    )
{
    QPointF scenePos(
        newPos.x() + playerRect.width() / 2,
        newPos.y() + playerRect.height() - 5
        );

    int maskX =
        (scenePos.x() / sceneRect.width())
        * collisionMask.width();

    int maskY =
        (scenePos.y() / sceneRect.height())
        * collisionMask.height();

    //================ SAFE BOUNDS =================//

    maskX = qBound(
        0,
        maskX,
        collisionMask.width() - 1
        );

    maskY = qBound(
        0,
        maskY,
        collisionMask.height() - 1
        );

    QColor color =
        collisionMask.pixelColor(
            maskX,
            maskY
            );
    qDebug() << color;
    //================ BLACK = BLOCKED =================//

    return !(
        color.red()   < 20 &&
        color.green() < 20 &&
        color.blue()  < 20
        );
}

/* ================= CHECK ARTIFACT COLLISION ================= */

QGraphicsItem* Level::checkArtifactCollision(
    QGraphicsPixmapItem* player
    )
{
    for(QGraphicsPixmapItem* artifact : artifacts)
    {
        if(
            artifact &&
            player->collidesWithItem(artifact)
            )
        {
            return artifact;
        }
    }

    return nullptr;
}

/* ================= REMOVE ARTIFACT ================= */

void Level::removeArtifact(
    QGraphicsItem* artifact,
    QGraphicsScene* scene
    )
{
    artifacts.removeOne(
        static_cast<QGraphicsPixmapItem*>(artifact)
        );

    scene->removeItem(artifact);

    delete artifact;
}

/* ================= GET ARTIFACT COUNT ================= */

int Level::getArtifactCount() const
{
    return artifacts.size();
}
