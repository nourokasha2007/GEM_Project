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
    std::vector<QGraphicsRectItem*> obstacles;
    std::vector<QGraphicsPixmapItem*> artifacts;
    virtual void loadScene(QGraphicsScene* scene) = 0;
    virtual ~Level() = default;
};

#endif
