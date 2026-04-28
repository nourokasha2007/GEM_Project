#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include "game.h"
#include "level.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);

protected:
    // Input Handling
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    // Navigation and Logic slots
    void startGame();
    void pauseGame();
    void restartGame();
    void exitGame();
    void updateGame(); // Triggered by the QTimer

private:
    //-----------------------------------
    // Core Game Logic
    //-----------------------------------
    Game game;
    Level* currentLevel;
    QTimer *timer;
    int seconds;

    //-----------------------------------
    // UI Structure (The Stack)
    //-----------------------------------
    QStackedWidget *stack;      // The main container

    QWidget *startScreen;       // Page 0
    QWidget *gameScreen;        // Page 1
    QWidget *gameOverScreen;    // Page 2 (Used for Win, Lose, and Game Over)

    //-----------------------------------
    // HUD & Feedback Components
    //-----------------------------------
    // These are updated dynamically in the .cpp
    QLabel *clockLabel;
    QLabel *scoreLabel;
    QLabel *statusLabel;        // Displays "You Win", "You Lose", etc.

    //-----------------------------------
    // Graphics System
    //-----------------------------------
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPixmapItem *playerSprite;

    // Helper function to setup the UI layouts
    void setupScreens();
};

#endif // GAMEWINDOW_H
