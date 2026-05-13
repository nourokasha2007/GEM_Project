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
<<<<<<< Updated upstream
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include "game.h"
<<<<<<< HEAD
=======
#include <QList>
#include "game.h"
#include "level1enemy.h"
>>>>>>> Stashed changes
#include "Level2.h"
=======
#include "level1enemy.h"
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    void showLevel2BriefingPopup();
    explicit GameWindow(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void startGame();
<<<<<<< Updated upstream
    void startLevel2();
    void showHieroglyphScreen();
    void pauseGame();
    void showPauseMenu();          // proper pause overlay
=======
    void pauseGame();
>>>>>>> Stashed changes
    void restartGame();
    void exitGame();
    void updateGame();
    void showBriefingPopup(const QString &playerName);
<<<<<<< Updated upstream
    void showLevel2BriefingPopup();
    void tickTitleAnimation();     // drives animated start screen particles
=======
    void tickTitleAnimation();   // drives floating star particles
>>>>>>> Stashed changes

private:

    //================ GAME =================//
<<<<<<< HEAD
<<<<<<< Updated upstream
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
=======
    void showFireballGameOver();
    void showHieroglyphScreen();
    Game game;
    Level* currentLevel;
    QString playerName;
    QTimer* timer;
=======
    void showFireballGameOver();
    Game game;

    Level* currentLevel;
    QString playerName;
    QTimer* timer;
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870
    int seconds;
>>>>>>> Stashed changes

    //================ STACK =================//
    QStackedWidget* stack;
    QWidget* startScreen;
    QWidget* gameScreen;
<<<<<<< Updated upstream
    QWidget* pauseScreen;          // proper pause overlay
    QWidget* gameOverScreen;
<<<<<<< HEAD
    QWidget* winScreen;
=======
    QWidget* gameOverScreen;
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
    //================ MINI-MAP (Level 2) =================//
    QWidget* miniMap;              // small overlay showing room + rock dots
    QLabel*  miniMapRock1;
    QLabel*  miniMapRock2;
    QLabel*  miniMapRock3;
    QLabel*  miniMapPlayer;
=======
    int coinCount   = 2;
    int scrollCount = 2;
    int maskCount   = 2;
    int amuletCount = 2;
    int timerCount  = 2;
>>>>>>> Stashed changes
=======

    QWidget* hieroglyphScreen;

    QWidget* winScreen;
    QWidget* victoryScreen;

    QLabel *coinText;
    QLabel *scrollText;
    QLabel *maskText;
    QLabel *amuletText;
    QLabel *timerText;

    int coinCount    = 2;
    int scrollCount  = 2;
    int maskCount    = 2;
    int amuletCount  = 2;
    int timerCount   = 2;
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870

    //================ HUD =================//
    QLabel* clockLabel;
    QLabel* scoreLabel;

    QLabel* statusLabel;
<<<<<<< HEAD
<<<<<<< Updated upstream
    QLabel* bgOverlay;
=======
=======
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870

    void saveGame();

    void loadGame();

    QLabel*    bgOverlay;
>>>>>>> Stashed changes
    QLineEdit* guestNameEdit;
    QWidget* level2HUD;

    QLabel* level2ScoreLabel;

    QLabel* level2TimerLabel;

    QLabel* dangerLabel;

    QLabel* rock1Slot;
    QLabel* rock2Slot;
    QLabel* rock3Slot;

    int rocksCollected = 0;

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

<<<<<<< Updated upstream
    //================ SOUNDS =================//
=======
    //================ GRAPHICS =================//
<<<<<<< HEAD
    QGraphicsScene*      scene;
    QGraphicsView*       view;
    Level1Enemy*         mummy;
=======

    QGraphicsScene* scene;
    QGraphicsView* view;
    Level1Enemy* mummy;
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870
    QGraphicsPixmapItem* playerSprite;
    QImage               collisionMask;

    //================ DIRECTIONAL SPRITES =================//
<<<<<<< HEAD
    QPixmap spriteFront;
    QPixmap spriteBack;
    QPixmap spriteLeft;
    QPixmap spriteRight;
=======

    QPixmap spriteFront;   // player front.png  — down arrow
    QPixmap spriteBack;    // player back.png   — up arrow
    QPixmap spriteLeft;    // player left.png   — left arrow
    QPixmap spriteRight;   // player right.png  — right arrow
      QImage collisionMask;
    //================ SOUND =================//
>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870

    //================ SOUND =================//
>>>>>>> Stashed changes
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

    //================ STAR PARTICLES (start screen) =================//
    QTimer*       animTimer;        // 80 ms tick for particle animation
    QList<QLabel*> particles;       // the floating ✦ star labels
    int            animFrame;       // counter used for sine-fade

    //================ HELPERS =================//
    void setupStartScreen();
    void setupGameScreen();
<<<<<<< Updated upstream
    void setupPauseScreen();       // proper pause overlay
    void setupGameOverScreen();
    void setupWinScreen();
    void setupHieroglyphScreen();
    void setupLevel2HUD();
    void setupMiniMap();
    void updateLevel2HUD();
    void updateMiniMap();
    void updateTorchFlicker();
=======
    void setupGameOverScreen();
    void setupHieroglyphScreen();
>>>>>>> Stashed changes
    void setupHUD(QVBoxLayout* mainLayout);
    void setupInventoryUI(QHBoxLayout* gameLayout);
    void setupButtons(QVBoxLayout* mainLayout);
    void updateHUD();
    void updateInventoryUI();
<<<<<<< HEAD
<<<<<<< Updated upstream
    void updateHealthDisplay();
    void takeDamage(int amount);   // deduct health, flash, check death
    void movePlayer(int dx, int dy);
    bool isWalkable(QPointF newPos);
    void checkArtifactCollisions();
    void checkDoorCollision();
    void saveGame();
    void loadGame();
=======

    void movePlayer(int dx, int dy, const QPixmap& sprite);
    bool isWalkable(QPointF newPos);
    void checkArtifactCollisions();
>>>>>>> Stashed changes
=======

    void movePlayer(int dx, int dy, const QPixmap& sprite);
    void checkArtifactCollisions();
    void showHieroglyphScreen();
    void setupLevel2HUD();

    void updateLevel2HUD();
    void setupHieroglyphScreen();
    void setupVictoryScreen();
    void showTreasureRecoveredPopup();
    void showVictoryScreen();

>>>>>>> 2d648edcaf07e7c600827024964acbff54c7d870
};

#endif // GAMEWINDOW_H
