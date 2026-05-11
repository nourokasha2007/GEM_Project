// Level3.cpp

#include "Level3.h"

#include <cmath>

Level3::Level3(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(1600, 900);

    setFocusPolicy(Qt::StrongFocus);

    // PLAYER START POSITION
    playerX = 200;
    playerY = 700;

    playerSpeed = 6;

    // MOVEMENT
    moveUp = false;
    moveDown = false;
    moveLeft = false;
    moveRight = false;

    // LOAD MAP
    background.load(":/new/prefix1/images/level3.png");

    // LOAD PLAYER
    playerImage.load(":/new/prefix1/images/ChatGPT Image Apr 28, 2026, 05_48_57 PM.png");

    // LOAD COLLISION MASK
    collisionMask.load(":/new/prefix1/images/level3 BW.png");

    // TREASURE POSITION
    treasureOpened = false;

    treasurePosition = QPoint(1350, 180);

    // TIMER
    gameTimer = new QTimer(this);

    connect(gameTimer,
            &QTimer::timeout,
            this,
            &Level3::updateGame);

    gameTimer->start(16);
}

void Level3::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // DRAW BACKGROUND
    painter.drawPixmap(rect(), background);

    // DRAW PLAYER
    painter.drawPixmap(playerX,
                       playerY,
                       80,
                       80,
                       playerImage);
}

void Level3::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
        moveUp = true;


    if(event->key() == Qt::Key_S)
        moveDown = true;

    if(event->key() == Qt::Key_A)
        moveLeft = true;

    if(event->key() == Qt::Key_D)
        moveRight = true;
}

void Level3::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
        moveUp = false;

    if(event->key() == Qt::Key_S)
        moveDown = false;

    if(event->key() == Qt::Key_A)
        moveLeft = false;

    if(event->key() == Qt::Key_D)
        moveRight = false;
}

bool Level3::isBlocked(int x, int y)
{
    if(x < 0 || y < 0 ||
        x >= collisionMask.width() ||
        y >= collisionMask.height())
    {
        return true;
    }

    QColor color = collisionMask.pixelColor(x, y);

    // BLACK = BLOCKED
    return (color.red() == 0 &&
            color.green() == 0 &&
            color.blue() == 0);
}

void Level3::movePlayer()
{
    int newX = playerX;
    int newY = playerY;

    if(moveUp)
        newY -= playerSpeed;

    if(moveDown)
        newY += playerSpeed;

    if(moveLeft)
        newX -= playerSpeed;

    if(moveRight)
        newX += playerSpeed;

    // PLAYER COLLISION POINTS
    int left = newX;
    int right = newX + 79;

    int top = newY;
    int bottom = newY + 79;

    bool blocked =
        isBlocked(left, top) ||
        isBlocked(right, top) ||
        isBlocked(left, bottom) ||
        isBlocked(right, bottom);

    if(!blocked)
    {
        playerX = newX;
        playerY = newY;
    }
}

void Level3::checkTreasureCollision()
{
    if(treasureOpened)
        return;

    int playerCenterX = playerX + 40;
    int playerCenterY = playerY + 40;

    int distanceX =
        abs(playerCenterX - treasurePosition.x());

    int distanceY =
        abs(playerCenterY - treasurePosition.y());

    if(distanceX < 100 &&
        distanceY < 100)
    {
        treasureOpened = true;

        gameTimer->stop();

        TreasurePopup popup(this);

        popup.exec();
    }
}

void Level3::updateGame()
{
    movePlayer();

    checkTreasureCollision();

    update();
}
