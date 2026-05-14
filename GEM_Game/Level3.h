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
#include <functional>

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

    // Set this before entering Level 3 so "Play Again" works
    void setRestartCallback(std::function<void()> cb)
    {
        restartCallback = cb;
    }

private:

    QGraphicsPixmapItem* treasure;

    bool treasureOpened;

    bool passwordSolved;

    bool popupShown;

    QPixmap closedTreasure;

    QPixmap openTreasure;

    QRect treasureRect;

    std::function<void()> restartCallback;

    void showPasswordPopup();

    void showTreasurePopup();

    void showVictoryPopup();
    void showFinalMenu();
};

#endif
