#include "Level1.h"
#include <QBrush>
#include <QPen>
Level1::Level1()
{
    // optional: you can initialize level data here
}

    void Level1::loadScene(QGraphicsScene *scene)
    {
        QPixmap bg(":/new/prefix1/images/level1 (1).png");

        background = scene->addPixmap(bg);

        scene->setSceneRect(bg.rect());
    }
/*

    QGraphicsRectItem* leftWall =
        scene->addRect(
            829,891,
            30,50,
            QPen(Qt::red),
            QBrush(Qt::red)
            );

    obstacles.push_back(leftWall);

    QGraphicsRectItem* rightWall =
        scene->addRect(
            1055,891,
            30,50,
            QPen(Qt::red),
            QBrush(Qt::red)
            );

    obstacles.push_back(rightWall);
    QGraphicsRectItem* coffin3 =
        scene->addRect(
            986,711,
            100,160,
            QPen(Qt::red),
            QBrush(Qt::red)
            );

    obstacles.push_back(coffin3);
}

*/
