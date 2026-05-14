#ifndef LEVEL1_H
#define LEVEL1_H

#include "Level.h"

class Level1 : public Level
{
public:

    Level1();

    //================ LOAD LEVEL =================//

    void loadScene(
        QGraphicsScene* scene
        ) override;

    //================ ADD ARTIFACT =================//

    void addArtifact(
        QGraphicsScene* scene,
        QString imagePath,
        QString type,
        int x,
        int y,
        double scale
        );
};

#endif // LEVEL1_H
