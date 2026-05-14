#ifndef LEVEL_H
#define LEVEL_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QVector>
#include <QPointF>

class Level
{
protected:

    //================ BACKGROUND ================//

    QGraphicsPixmapItem* background;

    //================ COLLISION MASK ================//

    QImage collisionMask;

    //================ ARTIFACTS ================//

    QVector<QGraphicsPixmapItem*> artifacts;

public:

    Level();

    virtual ~Level();

    //================ LOAD SCENE ================//

    virtual void loadScene(
        QGraphicsScene* scene
        ) = 0;

    //================ GET COLLISION MASK ================//

    QImage getCollisionMask() const;

    //================ WALKABLE =================//

    bool isWalkable(
        QPointF newPos,
        QRectF playerRect,
        QRectF sceneRect
        );

    //================ ARTIFACT COLLISION =================//

    QGraphicsItem* checkArtifactCollision(
        QGraphicsPixmapItem* player
        );

    //================ REMOVE ARTIFACT =================//

    void removeArtifact(
        QGraphicsItem* artifact,
        QGraphicsScene* scene
        );

    //================ ARTIFACT COUNT =================//

    int getArtifactCount() const;
};

#endif // LEVEL_H
