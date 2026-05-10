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
#include <QImage>
#include <QSoundEffect>
#include <QUrl>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include "game.h"
#include "Level2.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:

    void startGame();
    void startLevel2();
    void showHieroglyphScreen();
    void pauseGame();
    void showPauseMenu();          // proper pause overlay
    void restartGame();
    void exitGame();
    void updateGame();
    void showBriefingPopup(const QString &playerName);
    void showLevel2BriefingPopup();
    void tickTitleAnimation();     // drives animated start screen particles

private:

    //================ GAME =================//
    Game    game;
    Level*  currentLevel;
    Level2* level2Ptr;
    QTimer* timer;
    QTimer* animTimer;             // drives start screen animation
    int     seconds;
    bool    inLevel2;
    bool    level2DoorUnlocked;
    int     level1Collected;

    //================ HEALTH =================//
    int     playerHealth;          // 3 lives (ankh icons)
    int     maxHealth;
    QLabel* healthLabel;           // ankh row in HUD
    bool    isInvincible;          // brief grace period after hit
    int     invincibleFrames;

    //================ STACK =================//
    QStackedWidget* stack;
    QWidget* startScreen;
    QWidget* gameScreen;
    QWidget* pauseScreen;          // proper pause overlay
    QWidget* gameOverScreen;
    QWidget* winScreen;
    QWidget* hieroglyphScreen;

    //================ LEVEL 2 HUD =================//
    QWidget* level2HUD;
    QLabel*  rock1Slot;
    QLabel*  rock2Slot;
    QLabel*  rock3Slot;
    QLabel*  level2TimerLabel;
    QLabel*  dangerLabel;
    QLabel*  dangerBar;
    QLabel*  multiplierLabel;
    QLabel*  level2ScoreLabel;
    int      rocksCollected;
    int      torchFlicker;
    int      scoreMultiplier;
    int      rockCollectTime;
    bool     wraithWarningShown;

    //================ MINI-MAP (Level 2) =================//
    QWidget* miniMap;              // small overlay showing room + rock dots
    QLabel*  miniMapRock1;
    QLabel*  miniMapRock2;
    QLabel*  miniMapRock3;
    QLabel*  miniMapPlayer;

    //================ HUD =================//
    QLabel* clockLabel;
    QLabel* scoreLabel;
    QLabel* statusLabel;
    QLabel* bgOverlay;
    QLineEdit* guestNameEdit;

    //================ INVENTORY =================//
    QLabel* coinIcon;
    QLabel* scrollIcon;
    QLabel* maskIcon;
    QLabel* amuletIcon;
    QLabel* timerIcon;
    QLabel* coinCounter;
    QLabel* scrollCounter;
    QLabel* maskCounter;
    QLabel* amuletCounter;
    QLabel* timerCounter;

    //================ SOUNDS =================//
    QSoundEffect* startMusic;
    QSoundEffect* collectSound;    // artifact/rock pickup
    QSoundEffect* hitSound;        // player takes damage
    QSoundEffect* levelWinSound;   // level complete fanfare

    //================ GRAPHICS =================//
    QGraphicsScene*      scene;
    QGraphicsView*       view;
    QGraphicsPixmapItem* playerSprite;
    QImage               collisionMask;

    //================ ANIMATION (title screen) =================//
    QList<QLabel*> particles;      // floating gold particle labels
    int            animFrame;

    //================ HELPERS =================//
    void setupStartScreen();
    void setupGameScreen();
    void setupPauseScreen();       // proper pause overlay
    void setupGameOverScreen();
    void setupWinScreen();
    void setupHieroglyphScreen();
    void setupLevel2HUD();
    void setupMiniMap();
    void updateLevel2HUD();
    void updateMiniMap();
    void updateTorchFlicker();
    void setupHUD(QVBoxLayout* mainLayout);
    void setupInventoryUI(QHBoxLayout* gameLayout);
    void setupButtons(QVBoxLayout* mainLayout);
    void updateHUD();
    void updateInventoryUI();
    void updateHealthDisplay();
    void takeDamage(int amount);   // deduct health, flash, check death
    void movePlayer(int dx, int dy);
    bool isWalkable(QPointF newPos);
    void checkArtifactCollisions();
    void checkDoorCollision();
    void saveGame();
    void loadGame();
};

#endif // GAMEWINDOW_H
