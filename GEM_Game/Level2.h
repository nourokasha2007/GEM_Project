#ifndef LEVEL2_H
#define LEVEL2_H
#include "level.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>

class Level2 : public Level
{
public:

    Level2();

    void loadScene(
        QGraphicsScene* scene
        ) override;

    //================ DARKNESS =================//

    void updatePlayerGlow(
        QGraphicsPixmapItem* playerSprite
        );

private:

    //================ LIGHTING =================//

    QGraphicsRectItem* overlay;

    QGraphicsEllipseItem* playerGlow;

    //================ HELPERS =================//

    void addRock(
        QGraphicsScene* scene,
        const QString& imagePath,
        int x,
        int y,
        double scale
        );
};

#endif
