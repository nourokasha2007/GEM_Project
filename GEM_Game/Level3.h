// Level3.h

#ifndef LEVEL3_H
#define LEVEL3_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QPixmap>
#include <QImage>
#include <QPoint>

#include "TreasurePopup.h"

class Level3 : public QWidget
{
    Q_OBJECT

public:
    explicit Level3(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updateGame();

private:

    // PLAYER
    int playerX;
    int playerY;

    int playerSpeed;

    bool moveUp;
    bool moveDown;
    bool moveLeft;
    bool moveRight;

    // IMAGES
    QPixmap background;
    QPixmap playerImage;

    // COLLISION MASK
    QImage collisionMask;

    // TIMER
    QTimer *gameTimer;

    // TREASURE
    bool treasureOpened;

    QPoint treasurePosition;

    // FUNCTIONS
    void movePlayer();

    bool isBlocked(int x, int y);

    void checkTreasureCollision();
};

#endif // LEVEL3_H
