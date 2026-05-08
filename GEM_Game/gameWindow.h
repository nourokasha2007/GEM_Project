#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include <QPixmap>
#include <QRect>

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
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    // Navigation and Logic slots
    void startGame();
    void pauseGame();
    void restartGame();
    void exitGame();
    void updateGame(); // Triggered by the QTimer
    void showBriefingPopup(const QString &playerName);

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

    QLabel *coinIcon;
    QLabel *scrollIcon;
    QLabel *maskIcon;
    QLabel *amuletIcon;
    QLabel *timerIcon;
    QLabel *noseIcon;

    QLabel *coinCounter;
    QLabel *scrollCounter;
    QLabel *maskCounter;
    QLabel *amuletCounter;
    QLabel *timerCounter;
    QLabel *noseCounter;

    int coinCount=3;
    int scrollCount = 3;
    int maskCount = 3;
    int amuletCount = 3;
    int timerCount = 3;

    //-----------------------------------
    // HUD & Feedback Components
    //-----------------------------------
    // These are updated dynamically in the .cpp
    QLabel *clockLabel;
    QLabel *scoreLabel;
    QLabel *statusLabel;        // Displays "You Win", "You Lose", etc.
    QLabel *bgOverlay;          // Start screen dark overlay (resizes with window)
    QLineEdit *guestNameEdit;   // Guest name input on start screen

    //-----------------------------------
    // Graphics System
    //-----------------------------------
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsPixmapItem *playerSprite;
    QImage collisionMask;
    // Helper function to setup the UI layouts
    void setupScreens();



};

#endif // GAMEWINDOW_H
