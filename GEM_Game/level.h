#ifndef LEVEL_H
#define LEVEL_H
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <vector>

class Level
{
protected:
    QGraphicsPixmapItem* background;

public:

    //================ OBJECTS ================//

    std::vector<QGraphicsRectItem*> obstacles;

    std::vector<QGraphicsPixmapItem*> artifacts;

    //================ CORE =================//

    virtual void loadScene(QGraphicsScene* scene) = 0;

    virtual ~Level() = default;

    //================ ARTIFACTS =================//

    QGraphicsItem* checkArtifactCollision(
        QGraphicsPixmapItem* player
        );

    void removeArtifact(
        QGraphicsItem* artifact,
        QGraphicsScene* scene
        );

    int getArtifactCount() const;

    //================ HELPERS =================//

    void clearLevel(QGraphicsScene* scene);
};

#endif
