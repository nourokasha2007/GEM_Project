#ifndef LEVEL1_H
#define LEVEL1_H
#include "level.h"

class Level1 : public Level
{
public:
    Level1();

    void loadScene(QGraphicsScene* scene) override;

private:

    //================ HELPERS ================//

    void addArtifact(
        QGraphicsScene* scene,
        QString imagePath,
        QString type,
        int x,
        int y,
        double scale
        );

};

#endif
