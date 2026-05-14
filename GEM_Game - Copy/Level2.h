#ifndef LEVEL2_H
#define LEVEL2_H
#include "Level.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>

class Level2 : public Level
{
private:

    //================ DARKNESS =================//

    QGraphicsRectItem* overlay;

    //================ PLAYER LIGHT =================//

    QGraphicsEllipseItem* playerGlow;

public:

    Level2();

    //================ LOAD LEVEL =================//

    void loadScene(
        QGraphicsScene* scene
        ) override;

    //================ ADD ROCK =================//

    void addRock(
        QGraphicsScene* scene,
        const QString& imagePath,
        int x,
        int y,
        double scale
        );

    //================ UPDATE LIGHT =================//

    void updatePlayerGlow(
        QGraphicsPixmapItem* playerSprite
        );
};

#endif // LEVEL2_H
