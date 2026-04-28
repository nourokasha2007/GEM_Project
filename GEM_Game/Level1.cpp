#include "Level1.h"
#include <QBrush>
#include <QPen>
Level1::Level1()
{
    // optional: you can initialize level data here
}

void Level1::loadScene(QGraphicsScene *scene)
{
    background = scene->addPixmap(QPixmap(":/new/prefix1/images/level1 background.png"));

    QGraphicsRectItem* statue1 =
        scene->addRect(
            946, 670,
            100, 100,
            QPen(Qt::red),
            QBrush(Qt::red)
            );

    obstacles.push_back(statue1);

    QGraphicsRectItem* statue2 =
        scene->addRect(
            944, 339,
            100, 100,
            QPen(Qt::red),
            QBrush(Qt::red)
            );

    obstacles.push_back(statue2);


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
}
    /*
    QGraphicsPixmapItem* artifact1 =
        scene->addPixmap(QPixmap(":/assets/artifact.png"));

    artifact1->setPos(200,600);

    artifacts.push_back(artifact1);


    QGraphicsPixmapItem* artifact2 =
        scene->addPixmap(QPixmap(":/assets/artifact.png"));

    artifact2->setPos(700,450);

    artifacts.push_back(artifact2);
*/
