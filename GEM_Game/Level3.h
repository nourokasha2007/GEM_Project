#ifndef LEVEL3_H
#define LEVEL3_H

#include "level.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class Level3 : public Level
{
public:

    Level3();

    void loadScene(
        QGraphicsScene* scene
        ) override;

    void checkTreasureCollision(
        QGraphicsPixmapItem* playerSprite
        );

private:

    QGraphicsPixmapItem* treasure;

    bool treasureOpened;

    bool passwordSolved;

    QPixmap closedTreasure;

    QPixmap openTreasure;

    QRect treasureRect;

    void showPasswordPopup();

    void showTreasurePopup();

    void showVictoryPopup();
};

#endif
