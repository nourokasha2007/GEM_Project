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
#include "game.h"
#include "level1enemy.h"

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

    void pauseGame();

    void restartGame();

    void exitGame();

    void updateGame();

    void showBriefingPopup(const QString &playerName);

private:

    //================ GAME =================//
    void showFireballGameOver();
    Game game;

    Level* currentLevel;
    QString playerName;
    QTimer* timer;
    int seconds;

    //================ STACK =================//

    QStackedWidget* stack;

    QWidget* startScreen;

    QWidget* gameScreen;

    QWidget* gameOverScreen;

    QWidget* hieroglyphScreen;

    QWidget* winScreen;

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

    //================ HUD =================//

    QLabel* clockLabel;

    QLabel* scoreLabel;

    QLabel* statusLabel;

    void saveGame();

    void loadGame();

    QLabel* bgOverlay;

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

    //================ GRAPHICS =================//

    QGraphicsScene* scene;

    QGraphicsView* view;
    Level1Enemy* mummy;
    QGraphicsPixmapItem* playerSprite;

    //================ DIRECTIONAL SPRITES =================//

    QPixmap spriteFront;   // player front.png  — down arrow
    QPixmap spriteBack;    // player back.png   — up arrow
    QPixmap spriteLeft;    // player left.png   — left arrow
    QPixmap spriteRight;   // player right.png  — right arrow

    //================ SOUND =================//

    QSoundEffect* startMusic;

    //================ HELPERS =================//

    void setupStartScreen();

    void setupGameScreen();

    void setupGameOverScreen();

    void setupHUD(QVBoxLayout* mainLayout);

    void setupInventoryUI(QHBoxLayout* gameLayout);

    void setupButtons(QVBoxLayout* mainLayout);

    void updateHUD();

    void updateInventoryUI();

    void movePlayer(int dx, int dy, const QPixmap& sprite);
    void checkArtifactCollisions();
    void showHieroglyphScreen();
    void setupLevel2HUD();

    void updateLevel2HUD();
    void setupHieroglyphScreen();

};

#endif
