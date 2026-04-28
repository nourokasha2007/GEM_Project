#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "Game.h"
#include "level.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void startGame();
    void pauseGame();
    void restartGame();
    void exitGame();
    void updateGame();

private:
    //-----------------------------------
    // Core game logic
    //-----------------------------------
    Game game;
    Level* currentLevel;

    //-----------------------------------
    // Stacked screens
    //-----------------------------------
    QStackedWidget *stack;
    QWidget *startScreen;
    QWidget *gameScreen;
    QWidget *gameOverScreen;

    //-----------------------------------
    // HUD labels
    //-----------------------------------
    QLabel *clockLabel;
    QLabel *scoreLabel;
    QLabel *statusLabel;

    //-----------------------------------
    // Timer
    //-----------------------------------
    QTimer *timer;
    int seconds;

    //-----------------------------------
    // Graphics system
    //-----------------------------------
    QGraphicsScene *scene;
    QGraphicsView *view;
    //-----------------------------------
    // Visual player sprite
    //-----------------------------------
    QGraphicsPixmapItem *playerSprite;
    void mousePressEvent(QMouseEvent *event);
};

#endif
