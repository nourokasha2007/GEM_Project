#include "Level1.h"
#include <QBrush>
#include <QPen>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDebug>
Level1::Level1()
{
    // optional: you can initialize level data here
}

void Level1::loadScene(QGraphicsScene *scene)
{
    background = scene->addPixmap(QPixmap(":/new/prefix1/images/level1.png"));
    QPixmap coinPixmap(":/new/prefix1/coin.png");

    qDebug() << "coin null?" << QPixmap(":/new/prefix1/images/coin.png").isNull();

   /* QGraphicsPixmapItem* coinItem = scene->addPixmap(coinPixmap);
    coinItem->setPos(400, 400);
    coinItem->setScale(0.4);
    coinItem->setZValue(999);
    artifacts.push_back(coinItem);

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
    QGraphicsRectItem* coffin1 =
        scene->addRect(
            986,711,
            100,160,
            QPen(Qt::red),
            QBrush(Qt::red)
            );

    obstacles.push_back(coffin1);
*/

    double s = 0.025;

    // ===== COINS (3) =====
    QGraphicsPixmapItem* coin1 = scene->addPixmap(QPixmap(":/new/prefix1/images/coint.png"));
    coin1->setPos(310, 500);
    coin1->setScale(s);
    coin1->setOpacity(0.9);
    coin1->setZValue(999);
    artifacts.push_back(coin1);

    QGraphicsPixmapItem* coin2 = scene->addPixmap(QPixmap(":/new/prefix1/images/coint.png"));
    coin2->setPos(520, 620);
    coin2->setScale(s);
    coin2->setZValue(999);
    artifacts.push_back(coin2);




    // ===== SCROLLS (3) =====
    QGraphicsPixmapItem* scroll1 = scene->addPixmap(QPixmap(":/new/prefix1/images/scrollt.png"));
    scroll1->setPos(420, 800);
    scroll1->setScale(s);
    scroll1->setZValue(999);
    artifacts.push_back(scroll1);



    QGraphicsPixmapItem* scroll2 = scene->addPixmap(QPixmap(":/new/prefix1/images/scrollt.png"));
    scroll2->setPos(720, 760);
    scroll2->setScale(s);
    scroll2->setZValue(999);
    artifacts.push_back(scroll2);


    // ===== MASKS (3) =====


    QGraphicsPixmapItem* mask1 = scene->addPixmap(QPixmap(":/new/prefix1/images/maskt.png"));
    mask1->setPos(960, 400);
    mask1->setScale(s);
    mask1->setZValue(999);
    artifacts.push_back(mask1);

    QGraphicsPixmapItem* mask2 = scene->addPixmap(QPixmap(":/new/prefix1/images/maskt.png"));
    mask2->setPos(900,850);
    mask2->setScale(s);
    mask2->setZValue(999);
    artifacts.push_back(mask2);


    // ===== AMULETS (3) =====
    QGraphicsPixmapItem* amulet1 = scene->addPixmap(QPixmap(":/new/prefix1/images/amulett.png"));
    amulet1->setPos(470, 430);
    amulet1->setScale(s);
    amulet1->setZValue(999);
    artifacts.push_back(amulet1);

    QGraphicsPixmapItem* amulet2 = scene->addPixmap(QPixmap(":/new/prefix1/images/amulett.png"));
    amulet2->setPos(870, 700);
    amulet2->setScale(s);
    amulet2->setZValue(999);
    artifacts.push_back(amulet2);




    // ===== TIMER TOOLS (3) =====
    QGraphicsPixmapItem* timer1 = scene->addPixmap(QPixmap(":/new/prefix1/images/timert.png"));
    timer1->setPos(610, 410);
    timer1->setScale(s);
    timer1->setZValue(999);
    artifacts.push_back(timer1);

    QGraphicsPixmapItem* timer2 = scene->addPixmap(QPixmap(":/new/prefix1/images/timert.png"));
    timer2->setPos(850, 540);
    timer2->setScale(s);
    timer2->setZValue(999);
    artifacts.push_back(timer2);


}
