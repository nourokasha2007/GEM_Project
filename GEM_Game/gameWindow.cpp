#include "gameWindow.h"
#include "level1enemy.h"
#include "Level2enemy.h"
#include "Level2.h"
#include "Level3.h"

#include <QDebug>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QDir>
#include <cmath>
#include <cstdlib>

/* ================= CONSTRUCTOR ================= */

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    seconds            = 300;
    currentLevel       = nullptr;
    level2Ptr          = nullptr;
    inLevel2           = false;
    level2DoorUnlocked = false;
    level1Collected    = 0;
    rocksCollected     = 0;
    torchFlicker       = 0;
    scoreMultiplier    = 1;
    rockCollectTime    = 0;
    wraithWarningShown = false;
    level2HUD          = nullptr;
    miniMap            = nullptr;
    animFrame          = 0;
    playerHealth       = 3;
    maxHealth          = 3;
    isInvincible       = false;
    invincibleFrames   = 0;
    mummy              = nullptr;
    view               = nullptr;
    playerSprite       = nullptr;

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    setupStartScreen();
    setupGameScreen();
    setupLevel2HUD();
    setupPauseScreen();
    setupGameOverScreen();
    setupWinScreen();
    setupHieroglyphScreen();
    setupVictoryScreen();

    setFocusPolicy(Qt::StrongFocus);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::updateGame);

    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &GameWindow::tickTitleAnimation);
    animTimer->start(80);

    stack->setCurrentWidget(startScreen);
    showMaximized();

    startMusic = new QSoundEffect(this);
    startMusic->setSource(QUrl("qrc:/new/prefix1/sounds/desert sounds.wav"));
    startMusic->setVolume(0.5);
    startMusic->setLoopCount(QSoundEffect::Infinite);
    startMusic->play();

    collectSound = new QSoundEffect(this);
    collectSound->setSource(QUrl("qrc:/new/prefix1/sounds/collect.wav"));
    collectSound->setVolume(0.8);

    hitSound = new QSoundEffect(this);
    hitSound->setSource(QUrl("qrc:/new/prefix1/sounds/fireball boom.wav"));
    hitSound->setVolume(0.7);

    levelWinSound = new QSoundEffect(this);
    levelWinSound->setSource(QUrl("qrc:/new/prefix1/sounds/desert sounds.wav"));
    levelWinSound->setVolume(0.6);
}

/* ================================================================
   SETUP GAME SCREEN
   ================================================================ */

void GameWindow::setupGameScreen()
{
    gameScreen = new QWidget();
    gameScreen->setStyleSheet("background-color: black;");

    QVBoxLayout* mainLayout = new QVBoxLayout(gameScreen);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    setupHUD(mainLayout);

    QHBoxLayout* gameLayout = new QHBoxLayout();
    setupInventoryUI(gameLayout);

    scene = new QGraphicsScene(this);
    view  = new QGraphicsView(scene);
    view->setFocusPolicy(Qt::NoFocus);
    view->setFixedSize(1200, 650);

    gameLayout->addWidget(view);
    mainLayout->addLayout(gameLayout);

    setupButtons(mainLayout);

    stack->addWidget(gameScreen);
}

/* ================================================================
   LEVEL 2 HUD
   ================================================================ */

void GameWindow::setupLevel2HUD()
{
    level2HUD = new QWidget(gameScreen);
    level2HUD->setFixedHeight(80);
    level2HUD->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:0,y2:1,"
        " stop:0 rgba(20,10,2,245), stop:1 rgba(8,4,0,255));"
        "border-bottom: 2px solid #8a6020;"
        );
    level2HUD->hide();

    QHBoxLayout* lay = new QHBoxLayout(level2HUD);
    lay->setContentsMargins(16, 6, 16, 6);
    lay->setSpacing(12);

    QVBoxLayout* leftCol = new QVBoxLayout();
    leftCol->setSpacing(3);

    QLabel* lvlTag = new QLabel("WING  II");
    lvlTag->setStyleSheet(
        "color: #c8a84b; font-size: 11px; font-weight: bold;"
        "letter-spacing: 5px; background: transparent;"
        );

    dangerLabel = new QLabel("● WRAITH NEARBY");
    dangerLabel->setStyleSheet(
        "color: #e03030; font-size: 11px; font-weight: bold;"
        "background: transparent; letter-spacing:2px;"
        );
    dangerLabel->hide();

    QWidget* barBg = new QWidget();
    barBg->setFixedSize(130, 8);
    barBg->setStyleSheet(
        "background: rgba(60,10,10,180);"
        "border: 1px solid rgba(200,50,50,120);"
        "border-radius: 4px;"
        );
    dangerBar = new QLabel(barBg);
    dangerBar->setGeometry(0, 0, 0, 8);
    dangerBar->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        " stop:0 #8B0000, stop:1 #FF2020);"
        "border-radius: 4px;"
        );

    leftCol->addWidget(lvlTag);
    leftCol->addWidget(dangerLabel);
    leftCol->addWidget(barBg);

    QVBoxLayout* scoreCol = new QVBoxLayout();
    scoreCol->setSpacing(2);
    scoreCol->setAlignment(Qt::AlignCenter);

    level2ScoreLabel = new QLabel("SCORE  0");
    level2ScoreLabel->setStyleSheet(
        "color: #f5d060; font-size: 18px; font-weight: bold;"
        "background: transparent; letter-spacing:2px;"
        );
    level2ScoreLabel->setAlignment(Qt::AlignCenter);

    multiplierLabel = new QLabel("1×");
    multiplierLabel->setStyleSheet(
        "color: #7a5820; font-size: 12px; font-weight: bold;"
        "background: transparent; letter-spacing:2px;"
        );
    multiplierLabel->setAlignment(Qt::AlignCenter);

    scoreCol->addWidget(level2ScoreLabel);
    scoreCol->addWidget(multiplierLabel);

    QHBoxLayout* rockRow = new QHBoxLayout();
    rockRow->setSpacing(8);
    rockRow->setAlignment(Qt::AlignCenter);

    QString emptySlot =
        "border: 2px solid rgba(200,160,60,90);"
        "border-radius: 8px;"
        "background: rgba(30,18,4,200);"
        "font-size: 20px; color: rgba(200,160,60,60);";

    rock1Slot = new QLabel("?");
    rock1Slot->setFixedSize(58, 58);
    rock1Slot->setAlignment(Qt::AlignCenter);
    rock1Slot->setStyleSheet(emptySlot);

    rock2Slot = new QLabel("?");
    rock2Slot->setFixedSize(58, 58);
    rock2Slot->setAlignment(Qt::AlignCenter);
    rock2Slot->setStyleSheet(emptySlot);

    rock3Slot = new QLabel("?");
    rock3Slot->setFixedSize(58, 58);
    rock3Slot->setAlignment(Qt::AlignCenter);
    rock3Slot->setStyleSheet(emptySlot);

    QLabel* rockTitle = new QLabel("SACRED\nSTONES");
    rockTitle->setStyleSheet(
        "color: #7a5820; font-size: 9px; font-weight: bold;"
        "letter-spacing: 2px; background: transparent;"
        );
    rockTitle->setAlignment(Qt::AlignCenter);

    rockRow->addWidget(rockTitle);
    rockRow->addWidget(rock1Slot);
    rockRow->addWidget(rock2Slot);
    rockRow->addWidget(rock3Slot);

    QVBoxLayout* rightCol = new QVBoxLayout();
    rightCol->setSpacing(2);
    rightCol->setAlignment(Qt::AlignCenter);

    QLabel* timerTitle = new QLabel("TIME");
    timerTitle->setStyleSheet(
        "color: #7a5820; font-size: 10px; font-weight: bold;"
        "letter-spacing: 4px; background: transparent;"
        );
    timerTitle->setAlignment(Qt::AlignCenter);

    level2TimerLabel = new QLabel("03:00");
    level2TimerLabel->setStyleSheet(
        "color: #f5d060; font-size: 30px; font-weight: bold;"
        "background: transparent; font-family: monospace; letter-spacing:3px;"
        );
    level2TimerLabel->setAlignment(Qt::AlignCenter);

    rightCol->addWidget(timerTitle);
    rightCol->addWidget(level2TimerLabel);

    lay->addLayout(leftCol);
    lay->addStretch(1);
    lay->addLayout(scoreCol);
    lay->addStretch(1);
    lay->addLayout(rockRow);
    lay->addStretch(1);
    lay->addLayout(rightCol);
}

/* ================================================================
   LEVEL 2 HUD UPDATE
   ================================================================ */

void GameWindow::updateLevel2HUD()
{
    if (!level2HUD) return;

    int m = seconds / 60;
    int s = seconds % 60;
    level2TimerLabel->setText(
        QString("%1:%2")
            .arg(m, 2, 10, QChar('0'))
            .arg(s, 2, 10, QChar('0'))
        );

    if (seconds <= 30)
        level2TimerLabel->setStyleSheet(
            "color: #ff2020; font-size: 30px; font-weight: bold;"
            "background: transparent; font-family: monospace;"
            );
    else if (seconds <= 60)
        level2TimerLabel->setStyleSheet(
            "color: #ff8c00; font-size: 30px; font-weight: bold;"
            "background: transparent; font-family: monospace;"
            );

    level2ScoreLabel->setText("SCORE  " + QString::number(game.getPlayer().getScore()));

    QString multColor = scoreMultiplier >= 3 ? "#ff9500" :
                            scoreMultiplier == 2  ? "#c8a84b" : "#7a5820";
    multiplierLabel->setText(QString::number(scoreMultiplier) + "×  MULTIPLIER");
    multiplierLabel->setStyleSheet(
        "color: " + multColor + "; font-size: 12px; font-weight: bold;"
                                "background: transparent; letter-spacing:2px;"
        );

    int dangerPct = qBound(0, (180 - seconds) * 2 / 3, 100);
    if (seconds < 90) dangerPct = qBound(0, dangerPct + 20, 100);

    int barW = (dangerBar->parentWidget()->width() * dangerPct) / 100;
    dangerBar->setGeometry(0, 0, barW, 8);

    if (dangerPct > 50)
    {
        dangerLabel->show();
        dangerLabel->setStyleSheet(
            "color: #ff2020; font-size: 11px; font-weight: bold;"
            "background: transparent; letter-spacing:2px;"
            );
    }
    else if (dangerPct > 25)
    {
        dangerLabel->show();
        dangerLabel->setStyleSheet(
            "color: #ff8c00; font-size: 11px; font-weight: bold;"
            "background: transparent; letter-spacing:2px;"
            );
        dangerLabel->setText("● WRAITH NEARBY");
    }
    else
    {
        dangerLabel->hide();
    }

    QString emptyStyle =
        "border: 2px solid rgba(200,160,60,90);"
        "border-radius: 8px;"
        "background: rgba(30,18,4,200);"
        "font-size: 20px; color: rgba(200,160,60,60);";

    QString filledStyle =
        "border: 2px solid #c8a84b;"
        "border-radius: 8px;"
        "background: rgba(80,52,8,220);"
        "outline: 0px solid #ffdd88;";

    if (rocksCollected >= 1)
    {
        QPixmap px1(":/new/prefix1/images/rock_M.png");
        rock1Slot->setText("");
        rock1Slot->setPixmap(px1.scaled(52, 52, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        rock1Slot->setStyleSheet(filledStyle);
    }
    else
    {
        rock1Slot->setPixmap(QPixmap());
        rock1Slot->setText("?");
        rock1Slot->setStyleSheet(emptyStyle);
    }

    if (rocksCollected >= 2)
    {
        QPixmap px2(":/new/prefix1/images/rock_A-3.png");
        rock2Slot->setText("");
        rock2Slot->setPixmap(px2.scaled(52, 52, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        rock2Slot->setStyleSheet(filledStyle);
    }
    else
    {
        rock2Slot->setPixmap(QPixmap());
        rock2Slot->setText("?");
        rock2Slot->setStyleSheet(emptyStyle);
    }

    if (rocksCollected >= 3)
    {
        QPixmap px3(":/new/prefix1/images/rock_N.png");
        rock3Slot->setText("");
        rock3Slot->setPixmap(px3.scaled(52, 52, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        rock3Slot->setStyleSheet(filledStyle);
    }
    else
    {
        rock3Slot->setPixmap(QPixmap());
        rock3Slot->setText("?");
        rock3Slot->setStyleSheet(emptyStyle);
    }
}

/* ================================================================
   TORCH FLICKER
   ================================================================ */

void GameWindow::updateTorchFlicker()
{
    if (!level2Ptr || !playerSprite) return;
    torchFlicker = (torchFlicker + 1) % 20;
    if (seconds < 60 && (torchFlicker % 10 == 0))
    {
        statusLabel->setStyleSheet(
            "color: #ff2020; font-size: 14px; font-weight: bold;"
            "background: rgba(0,0,0,120); padding: 4px 10px; border-radius: 4px;"
            );
        statusLabel->setText("⚠  WRAITH CLOSING IN");
    }
    else if (torchFlicker % 10 == 5)
    {
        statusLabel->setStyleSheet(
            "color: #f5d060; font-size: 14px; font-weight: bold;"
            "background: rgba(0,0,0,120); padding: 4px 10px; border-radius: 4px;"
            );
        int remaining = 3 - rocksCollected;
        statusLabel->setText(
            remaining > 0
                ? QString("FIND %1 MORE STONE%2").arg(remaining).arg(remaining > 1 ? "S" : "")
                : "DECODE THE STONES"
            );
    }
}

/* ================================================================
   ANIMATED TITLE SCREEN
   ================================================================ */

void GameWindow::tickTitleAnimation()
{
    animFrame++;
    int screenHeight = startScreen->height();

    for (QLabel* p : particles)
    {
        // 1. Move
        int newY = p->y() - 1;
        if (newY < -20) newY = screenHeight + 10;
        p->move(p->x(), newY);

        // 2. Calculate Alpha
        double alphaVal = 0.3 + 0.55 * std::abs(std::sin(animFrame * 0.04 + p->x() * 0.01));

        // 3. Update Color without re-parsing CSS
        QPalette pal = p->palette();
        QColor color(200, 160, 60, static_cast<int>(alphaVal * 255));
        pal.setColor(QPalette::WindowText, color);
        p->setPalette(pal);
    }
}

/* ================================================================
   HEALTH SYSTEM
   ================================================================ */

void GameWindow::updateHealthDisplay()
{
    QString display;
    for (int i = 0; i < maxHealth; i++)
    {
        if (i < playerHealth)
            display += "<span style='color:#c8a84b; font-size:20px;'>☥ </span>";
        else
            display += "<span style='color:rgba(100,60,10,120); font-size:20px;'>☥ </span>";
    }
    healthLabel->setText(display);
    healthLabel->setTextFormat(Qt::RichText);
}

void GameWindow::takeDamage(int amount)
{
    if (isInvincible) return;

    playerHealth -= amount;
    if (hitSound) hitSound->play();
    updateHealthDisplay();

    if (playerHealth <= 0)
    {
        timer->stop();
        QMessageBox* msg = new QMessageBox(this);
        msg->setWindowTitle("YOU FELL");
        msg->setText(
            "<span style='font-size:16px; color:#c8a84b;'>"
            "<b>The Wraith claimed you...</b><br><br>"
            "The sacred stones remain lost in the dark."
            "</span>"
            );
        msg->setStyleSheet(
            "QMessageBox { background-color: #0a0500; }"
            "QLabel { color: #f5e6c8; }"
            "QPushButton { background: rgba(180,130,40,200); color:#fff8e7;"
            " border:2px solid #c8a84b; border-radius:4px; padding:6px 20px; }"
            );
        QPushButton* rb = msg->addButton("TRY AGAIN", QMessageBox::AcceptRole);
        msg->addButton("EXIT", QMessageBox::RejectRole);
        msg->exec();
        if (msg->clickedButton() == rb) restartGame();
        else exitGame();
        return;
    }

    isInvincible     = true;
    invincibleFrames = 60;

    statusLabel->setText("⚠  HIT!");
    statusLabel->setStyleSheet(
        "color:#ff2020; font-size:14px; font-weight:bold;"
        "background:rgba(0,0,0,120); padding:4px 10px; border-radius:4px;"
        );
}

/* ================================================================
   PAUSE SCREEN
   ================================================================ */

void GameWindow::setupPauseScreen()
{
    pauseScreen = new QWidget();
    pauseScreen->setStyleSheet("background-color: rgba(0,0,0,0);");

    QVBoxLayout* lay = new QVBoxLayout(pauseScreen);
    lay->setAlignment(Qt::AlignCenter);
    lay->setSpacing(0);

    QWidget* card = new QWidget();
    card->setFixedSize(400, 340);
    card->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:0,y2:1,"
        " stop:0 rgba(14,8,2,240), stop:1 rgba(6,3,0,255));"
        "border: 2px solid #8a6020;"
        "border-radius: 16px;"
        );

    QVBoxLayout* cl = new QVBoxLayout(card);
    cl->setSpacing(18);
    cl->setContentsMargins(40, 36, 40, 36);

    QLabel* title = new QLabel("⏸  PAUSED");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "color:#f5d060; font-size:28px; font-weight:bold;"
        "letter-spacing:6px; background:transparent;"
        );

    QLabel* sub = new QLabel("The museum waits for you.");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color:#8a6020; font-size:12px; background:transparent;");

    QString btnStyle =
        "QPushButton { background:rgba(180,130,40,200); color:#fff8e7;"
        " font-size:15px; font-weight:bold; letter-spacing:3px;"
        " border:2px solid #c8a84b; border-radius:8px; padding:12px 0; }"
        "QPushButton:hover { background:rgba(220,170,60,240); border-color:#fff; }";

    QPushButton* resumeBtn  = new QPushButton("▶   RESUME");
    QPushButton* restartBtn = new QPushButton("↺   RESTART");
    QPushButton* exitBtn    = new QPushButton("✕   EXIT");

    resumeBtn ->setStyleSheet(btnStyle);
    restartBtn->setStyleSheet(btnStyle);
    exitBtn->setStyleSheet(
        "QPushButton { background:rgba(100,30,20,200); color:#fff8e7;"
        " font-size:15px; font-weight:bold; letter-spacing:3px;"
        " border:2px solid #8a2020; border-radius:8px; padding:12px 0; }"
        "QPushButton:hover { background:rgba(160,40,30,240); }"
        );

    connect(resumeBtn, &QPushButton::clicked, this, [=]() {
        stack->setCurrentWidget(gameScreen);
        game.resumeGame();
        timer->start(1000);
    });
    connect(restartBtn, &QPushButton::clicked, this, &GameWindow::restartGame);
    connect(exitBtn,    &QPushButton::clicked, this, &GameWindow::exitGame);

    cl->addWidget(title);
    cl->addWidget(sub);
    cl->addSpacing(8);
    cl->addWidget(resumeBtn);
    cl->addWidget(restartBtn);
    cl->addWidget(exitBtn);

    lay->addWidget(card, 0, Qt::AlignCenter);
    stack->addWidget(pauseScreen);
}

void GameWindow::showPauseMenu()
{
    game.pauseGame();
    timer->stop();
    stack->setCurrentWidget(pauseScreen);
}

/* ================================================================
   MINI-MAP
   ================================================================ */

void GameWindow::setupMiniMap()
{
    miniMap = new QWidget(gameScreen);
    miniMap->setFixedSize(120, 85);
    miniMap->setStyleSheet(
        "background: rgba(8,4,0,200);"
        "border: 1px solid rgba(200,160,60,120);"
        "border-radius: 6px;"
        );
    miniMap->hide();

    QLabel* roomOutline = new QLabel(miniMap);
    roomOutline->setGeometry(8, 8, 104, 69);
    roomOutline->setStyleSheet(
        "border: 1px solid rgba(200,160,60,60);"
        "border-radius: 3px;"
        "background: rgba(40,24,8,100);"
        );

    auto makeDot = [=](QLabel*& dot, int sceneX, int sceneY, const QString& color) {
        dot = new QLabel(miniMap);
        int mx = 8 + int(sceneX * 104.0 / 1400.0);
        int my = 8 + int(sceneY *  69.0 /  900.0);
        dot->setGeometry(mx-4, my-4, 8, 8);
        dot->setStyleSheet("background:" + color + "; border-radius: 4px;");
    };

    makeDot(miniMapRock1,  220, 450, "#f5d060");
    makeDot(miniMapRock2, 1080, 580, "#f5d060");
    makeDot(miniMapRock3,  650, 230, "#f5d060");

    miniMapPlayer = new QLabel(miniMap);
    miniMapPlayer->setFixedSize(6, 6);
    miniMapPlayer->setStyleSheet("background: #00ff88; border-radius: 3px;");
}

void GameWindow::updateMiniMap()
{
    if (!miniMap || !miniMap->isVisible() || !playerSprite) return;

    double px = playerSprite->x();
    double py = playerSprite->y();
    int mx = 8 + int(px * 104.0 / 1400.0);
    int my = 8 + int(py *  69.0 /  900.0);
    miniMapPlayer->move(mx-3, my-3);

    if (rocksCollected >= 1) miniMapRock1->setStyleSheet("background:rgba(200,160,60,40);border-radius:4px;");
    if (rocksCollected >= 2) miniMapRock2->setStyleSheet("background:rgba(200,160,60,40);border-radius:4px;");
    if (rocksCollected >= 3) miniMapRock3->setStyleSheet("background:rgba(200,160,60,40);border-radius:4px;");
}

/* ================================================================
   START SCREEN
   ================================================================ */

void GameWindow::setupStartScreen()
{
    startScreen = new QWidget();
    startScreen->setObjectName("startScreen");
    startScreen->setStyleSheet(
        "QWidget#startScreen {"
        "  background-image: url(:/new/prefix1/images/PHOTO-2026-04-29-21-07-06.jpg);"
        "  background-position: center;"
        "  background-repeat: no-repeat;"
        "  background-color: #000000;"
        "}"
        );

    bgOverlay = new QLabel(startScreen);
    bgOverlay->setStyleSheet("background-color: rgba(0,0,0,110);");
    bgOverlay->setGeometry(0, 0, 9999, 9999);

    QVBoxLayout* layout = new QVBoxLayout(startScreen);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(12);

    QWidget* card = new QWidget();
    card->setFixedSize(380, 220);
    card->setStyleSheet(
        "background-color: rgba(10, 5, 0, 185);"
        "border: 1px solid rgba(200, 160, 60, 160);"
        "border-radius: 10px;"
        );

    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(12);
    cardLayout->setContentsMargins(30, 24, 30, 24);

    QLabel* cardTitle = new QLabel("ENTER THE MUSEUM");
    cardTitle->setAlignment(Qt::AlignCenter);
    cardTitle->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #c8a84b;"
        "letter-spacing: 4px; background: transparent; border: none;"
        );

    guestNameEdit = new QLineEdit();
    guestNameEdit->setPlaceholderText("Your Name");
    guestNameEdit->setFixedHeight(44);
    guestNameEdit->setStyleSheet(
        "QLineEdit { background-color: rgba(255,255,255,15);"
        "  border: 1px solid rgba(200,160,60,120); border-radius: 4px;"
        "  color: #f5e6c8; font-size: 15px; padding-left: 12px; }"
        "QLineEdit:focus { border: 1px solid #c8a84b;"
        "  background-color: rgba(255,255,255,25); }"
        );

    QPushButton* enterBtn = new QPushButton("▶   PROTECT THE HERITAGE");
    enterBtn->setFixedHeight(48);
    enterBtn->setCursor(Qt::PointingHandCursor);
    enterBtn->setStyleSheet(
        "QPushButton { background-color: rgba(180,130,40,220); color: #fff8e7;"
        "  font-size: 15px; font-weight: bold; letter-spacing: 3px;"
        "  border: 2px solid #c8a84b; border-radius: 4px; }"
        "QPushButton:hover { background-color: rgba(220,170,60,240);"
        "  border: 2px solid #ffffff; color: #ffffff; }"
        "QPushButton:pressed { background-color: rgba(130,90,20,255); }"
        );

    cardLayout->addWidget(cardTitle);
    cardLayout->addWidget(guestNameEdit);
    cardLayout->addSpacing(4);
    cardLayout->addWidget(enterBtn);

    connect(enterBtn, &QPushButton::clicked, this, [=]() {
        QString name = guestNameEdit->text().trimmed();
        if (name.isEmpty()) name = "Night Guard";
        showBriefingPopup(name);
    });

    layout->addStretch(3);
    layout->addWidget(card, 0, Qt::AlignCenter);
    layout->addStretch(1);

    for (int i = 0; i < 18; i++)
    {
        QLabel* p = new QLabel("✦", startScreen);
        p->setStyleSheet("color: rgba(200,160,60,120); font-size: 14px; background: transparent;");
        p->resize(20, 20);
        p->move(50 + QRandomGenerator::global()->bounded(1200),
                100 + QRandomGenerator::global()->bounded(700));
        p->show();
        particles.append(p);
    }

    startScreen->setMinimumSize(1000, 700);
    stack->addWidget(startScreen);
}

/* ================================================================
   GAME OVER SCREEN
   ================================================================ */

void GameWindow::setupGameOverScreen()
{
    gameOverScreen = new QWidget();
    gameOverScreen->setStyleSheet("background-color: #0a0500;");

    QVBoxLayout* layout = new QVBoxLayout(gameOverScreen);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    QLabel* over = new QLabel("GAME OVER");
    over->setAlignment(Qt::AlignCenter);
    over->setStyleSheet("font-size: 48px; font-weight: bold; color: #c8a84b; letter-spacing: 8px;");

    QPushButton* restartBtn = new QPushButton("RESTART");
    QPushButton* exitBtn    = new QPushButton("EXIT");

    QString btnStyle =
        "QPushButton { background-color: rgba(180,130,40,200); color: #fff8e7;"
        "  font-size: 16px; font-weight: bold; border: 2px solid #c8a84b;"
        "  border-radius: 6px; padding: 10px 40px; }"
        "QPushButton:hover { background-color: rgba(220,170,60,240); }";

    restartBtn->setStyleSheet(btnStyle);
    exitBtn->setStyleSheet(btnStyle);

    connect(restartBtn, &QPushButton::clicked, this, &GameWindow::restartGame);
    connect(exitBtn,    &QPushButton::clicked, this, &GameWindow::exitGame);

    layout->addWidget(over);
    layout->addWidget(restartBtn, 0, Qt::AlignCenter);
    layout->addWidget(exitBtn,    0, Qt::AlignCenter);

    stack->addWidget(gameOverScreen);
}

/* ================================================================
   WIN SCREEN
   ================================================================ */

void GameWindow::setupWinScreen()
{
    winScreen = new QWidget();
    winScreen->setStyleSheet("background-color: #050300;");

    QVBoxLayout* layout = new QVBoxLayout(winScreen);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    QLabel* winLabel = new QLabel("THE MUSEUM IS SAFE!");
    winLabel->setAlignment(Qt::AlignCenter);
    winLabel->setStyleSheet("font-size: 42px; font-weight: bold; color: #f5d060; letter-spacing: 6px;");

    QLabel* sub = new QLabel(
        "You recovered the Sacred Stones and escaped the Hieroglyph Vault.\n"
        "History lives on because of you, Night Guard."
        );
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("font-size: 16px; color: #e8d5a8;");

    QPushButton* exitBtn = new QPushButton("EXIT");
    exitBtn->setStyleSheet(
        "QPushButton { background-color: rgba(180,130,40,200); color: #fff8e7;"
        "  font-size: 16px; font-weight: bold; border: 2px solid #c8a84b;"
        "  border-radius: 6px; padding: 10px 40px; }"
        "QPushButton:hover { background-color: rgba(220,170,60,240); }"
        );

    connect(exitBtn, &QPushButton::clicked, this, &GameWindow::exitGame);

    layout->addWidget(winLabel);
    layout->addWidget(sub);
    layout->addWidget(exitBtn, 0, Qt::AlignCenter);

    stack->addWidget(winScreen);
}

/* ================================================================
   HIEROGLYPH SCREEN
   ================================================================ */

void GameWindow::setupHieroglyphScreen()
{
    hieroglyphScreen = new QWidget();
    hieroglyphScreen->setStyleSheet("background-color: #080400;");

    QVBoxLayout* root = new QVBoxLayout(hieroglyphScreen);
    root->setContentsMargins(10, 8, 10, 8);
    root->setSpacing(6);

    QLabel* title = new QLabel("THE STONES HAVE SPOKEN");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: #f5d060;"
        "letter-spacing: 5px; background: transparent;"
        );

    QLabel* instruction = new QLabel(
        "Study the hieroglyph alphabet below carefully.\n"
        "Each stone carries one symbol — identify what each one means.\n"
        "Memorize the symbols and their English letters. You will need this in Level 3."
        );
    instruction->setAlignment(Qt::AlignCenter);
    instruction->setWordWrap(true);
    instruction->setStyleSheet(
        "font-size: 13px; color: #c8a84b; font-style: italic;"
        "background: transparent; line-height: 1.6;"
        );

    QLabel* chart = new QLabel();
    chart->setPixmap(
        QPixmap(":/new/prefix1/images/hieroglyph_chart-2.png")
            .scaled(780, 420, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    chart->setAlignment(Qt::AlignCenter);

    QLabel* reminder = new QLabel("⚠   You will be asked to use this knowledge in Level 3. Do not forget.");
    reminder->setAlignment(Qt::AlignCenter);
    reminder->setStyleSheet(
        "font-size: 12px; color: rgba(200,160,60,160);"
        "background: transparent; letter-spacing: 1px;"
        );

    QPushButton* proceedBtn = new QPushButton("✓   I HAVE MEMORIZED THE SYMBOLS");
    proceedBtn->setFixedHeight(52);
    proceedBtn->setFixedWidth(420);
    proceedBtn->setCursor(Qt::PointingHandCursor);
    proceedBtn->setStyleSheet(
        "QPushButton { background-color: rgba(180,130,40,220); color: #fff8e7;"
        "  font-size: 14px; font-weight: bold; letter-spacing: 3px;"
        "  border: 2px solid #c8a84b; border-radius: 8px; }"
        "QPushButton:hover { background-color: rgba(220,170,60,240); border-color: #fff8e7; }"
        "QPushButton:pressed { background-color: rgba(130,90,20,255); }"
        );

    connect(proceedBtn, &QPushButton::clicked, this, [=]() {
        stack->setCurrentWidget(winScreen);
    });

    root->addWidget(title);
    root->addWidget(instruction);
    root->addWidget(chart);
    root->addWidget(reminder);
    root->addWidget(proceedBtn, 0, Qt::AlignCenter);

    stack->addWidget(hieroglyphScreen);
}

/* ================================================================
   HUD SETUP
   ================================================================ */

void GameWindow::setupHUD(QVBoxLayout* mainLayout)
{
    QHBoxLayout* hud = new QHBoxLayout();

    clockLabel  = new QLabel("00:00:00");
    scoreLabel  = new QLabel("Score: 0");
    statusLabel = new QLabel("ALIVE");
    healthLabel = new QLabel("☥ ☥ ☥");
    healthLabel->setTextFormat(Qt::RichText);

    QString labelStyle =
        "color: #f5d060; font-size: 14px; font-weight: bold;"
        "background: rgba(0,0,0,120); padding: 4px 10px; border-radius: 4px;";

    clockLabel ->setStyleSheet(labelStyle);
    scoreLabel ->setStyleSheet(labelStyle);
    statusLabel->setStyleSheet(labelStyle);
    healthLabel->setStyleSheet(labelStyle);

    hud->addWidget(clockLabel);
    hud->addWidget(scoreLabel);
    hud->addWidget(healthLabel);
    hud->addStretch(1);
    hud->addWidget(statusLabel);

    mainLayout->addLayout(hud);
}

/* ================================================================
   INVENTORY UI SETUP
   ================================================================ */

void GameWindow::setupInventoryUI(QHBoxLayout* gameLayout)
{
    QVBoxLayout* inventoryLayout = new QVBoxLayout();
    inventoryLayout->setSpacing(20);
    inventoryLayout->setAlignment(Qt::AlignTop);

    coinIcon    = new QLabel();
    scrollIcon  = new QLabel();
    maskIcon    = new QLabel();
    amuletIcon  = new QLabel();
    timerIcon   = new QLabel();
    coinCounter   = new QLabel();
    scrollCounter = new QLabel();
    maskCounter   = new QLabel();
    amuletCounter = new QLabel();
    timerCounter  = new QLabel();

    coinIcon->setPixmap(QPixmap(":/new/prefix1/images/coint.png").scaled(40, 40));
    scrollIcon->setPixmap(QPixmap(":/new/prefix1/images/scrollt.png").scaled(40, 40));
    maskIcon->setPixmap(QPixmap(":/new/prefix1/images/maskt.png").scaled(40, 40));
    amuletIcon->setPixmap(QPixmap(":/new/prefix1/images/amulett.png").scaled(40, 40));
    timerIcon->setPixmap(QPixmap(":/new/prefix1/images/timert.png").scaled(40, 40));

    updateInventoryUI();

    QHBoxLayout* coinRow   = new QHBoxLayout();
    QHBoxLayout* scrollRow = new QHBoxLayout();
    QHBoxLayout* maskRow   = new QHBoxLayout();
    QHBoxLayout* amuletRow = new QHBoxLayout();
    QHBoxLayout* timerRow  = new QHBoxLayout();

    coinRow->addWidget(coinIcon);     coinRow->addWidget(coinCounter);
    scrollRow->addWidget(scrollIcon); scrollRow->addWidget(scrollCounter);
    maskRow->addWidget(maskIcon);     maskRow->addWidget(maskCounter);
    amuletRow->addWidget(amuletIcon); amuletRow->addWidget(amuletCounter);
    timerRow->addWidget(timerIcon);   timerRow->addWidget(timerCounter);

    inventoryLayout->addLayout(coinRow);
    inventoryLayout->addLayout(scrollRow);
    inventoryLayout->addLayout(maskRow);
    inventoryLayout->addLayout(amuletRow);
    inventoryLayout->addLayout(timerRow);

    gameLayout->addLayout(inventoryLayout);
}

/* ================================================================
   BUTTONS SETUP
   ================================================================ */

void GameWindow::setupButtons(QVBoxLayout* mainLayout)
{
    QHBoxLayout* buttons = new QHBoxLayout();

    QPushButton* pauseBtn   = new QPushButton("PAUSE");
    QPushButton* restartBtn = new QPushButton("RESTART");
    QPushButton* exitBtn    = new QPushButton("EXIT");
    QPushButton* saveBtn    = new QPushButton("SAVE");

    connect(pauseBtn,   &QPushButton::clicked, this, &GameWindow::pauseGame);
    connect(restartBtn, &QPushButton::clicked, this, &GameWindow::restartGame);
    connect(exitBtn,    &QPushButton::clicked, this, &GameWindow::exitGame);
    connect(saveBtn,    &QPushButton::clicked, this, &GameWindow::saveGame);

    buttons->addWidget(pauseBtn);
    buttons->addWidget(restartBtn);
    buttons->addWidget(exitBtn);
    buttons->addWidget(saveBtn);

    mainLayout->addLayout(buttons);
}

/* ================================================================
   START GAME
   ================================================================ */

void GameWindow::startGame()
{
    game.startGame();
    seconds = 300;
    timer->start(1000);
    scene->clear();

    spriteFront = QPixmap(":/new/prefix1/images/player front.png");
    spriteBack  = QPixmap(":/new/prefix1/images/player back.png");
    spriteLeft  = QPixmap(":/new/prefix1/images/player left.png");
    spriteRight = QPixmap(":/new/prefix1/images/player right.png");

    currentLevel = game.getCurrentLevel();
    currentLevel->loadScene(scene);

    playerSprite = scene->addPixmap(spriteFront);
    playerSprite->setScale(0.12);
    playerSprite->setPos(game.getPlayer().getX(), game.getPlayer().getY());

    if (mummy)
    {
        scene->removeItem(mummy);
        delete mummy;
        mummy = nullptr;
    }

    mummy = new Level1Enemy(&game.getPlayer(), playerSprite);
    mummy->setPos(800, 500);
    mummy->setZValue(999);
    scene->addItem(mummy);

    connect(mummy, &Level1Enemy::reduceScore, this, [=]() {
        game.getPlayer().deductScore(10);
        updateHUD();
    });
    connect(mummy, &Level1Enemy::playerKilled, this, &GameWindow::showFireballGameOver);
    connect(mummy, &Level1Enemy::reduceScore, this, [=]() { takeDamage(1); updateHUD(); });

    updateInventoryUI();
    updateHUD();

    stack->setCurrentWidget(gameScreen);
    this->setFocus();
}

/* ================================================================
   START LEVEL 2
   ================================================================ */

void GameWindow::startLevel2()
{
    inLevel2           = true;
    level2DoorUnlocked = false;
    rocksCollected     = 0;
    torchFlicker       = 0;
    scoreMultiplier    = 1;
    rockCollectTime    = 180;
    wraithWarningShown = false;
    playerHealth       = 3;
    isInvincible       = false;
    invincibleFrames   = 0;

    if (startMusic && startMusic->isPlaying())
        startMusic->stop();

    scene->clear();

    game.loadLevel(2);
    currentLevel = game.getCurrentLevel();
    level2Ptr    = dynamic_cast<Level2*>(currentLevel);
    currentLevel->loadScene(scene);

    playerSprite = scene->addPixmap(
        QPixmap(":/new/prefix1/images/ChatGPT Image Apr 28, 2026, 05_48_57 PM.png")
        );
    playerSprite->setScale(0.08);
    playerSprite->setZValue(900);
    playerSprite->setPos(200, 450);
    game.getPlayer().moveTo(200, 450);

    collisionMask = QImage();

    Level2Enemy* wraith = new Level2Enemy(&game.getPlayer(), playerSprite);
    wraith->setPos(900, 250);
    wraith->setZValue(860);

    if (wraith->glowItem())
    {
        wraith->glowItem()->setZValue(801);
        scene->addItem(wraith->glowItem());
    }
    scene->addItem(wraith);

    connect(wraith, &Level2Enemy::wraithHitPlayer, this, [=]() {
        takeDamage(1);
        updateHealthDisplay();
        statusLabel->setText("⚠  THE WRAITH TOUCHED YOU!");
        statusLabel->setStyleSheet(
            "color:#ff2020; font-size:14px; font-weight:bold;"
            "background:rgba(0,0,0,120); padding:4px 10px; border-radius:4px;"
            );
    });

    if (level2Ptr)
        level2Ptr->updatePlayerGlow(playerSprite);

    seconds = 180;
    timer->start(1000);

    coinIcon->hide();   scrollIcon->hide();
    maskIcon->hide();   amuletIcon->hide();
    timerIcon->hide();
    coinCounter->hide();  scrollCounter->hide();
    maskCounter->hide();  amuletCounter->hide();
    timerCounter->hide();

    if (level2HUD)
    {
        level2HUD->setParent(gameScreen);
        level2HUD->setGeometry(0, 0, gameScreen->width(), 90);
        level2HUD->raise();
        level2HUD->show();
    }

    statusLabel->setText("WING II — FIND THE STONES");
    updateLevel2HUD();
    updateHUD();
    updateHealthDisplay();

    setupMiniMap();
    if (miniMap)
    {
        miniMap->setParent(gameScreen);
        miniMap->move(gameScreen->width() - 135, 90);
        miniMap->raise();
        miniMap->show();
    }

    stack->setCurrentWidget(gameScreen);
    setFocus();
}

/* ================================================================
   MOVEMENT
   ================================================================ */

void GameWindow::movePlayer(int dx, int dy, const QPixmap& sprite)
{
    if (playerSprite)
        playerSprite->setPixmap(sprite);

    int speed = inLevel2 ? 5 : 6;

    QPointF newPos = playerSprite->pos();
    newPos.setX(newPos.x() + dx * speed / 3);
    newPos.setY(newPos.y() + dy * speed / 3);

    QRectF sr   = scene->sceneRect();
    QRectF pr   = playerSprite->boundingRect();
    double pW   = pr.width()  * playerSprite->scale();
    double pH   = pr.height() * playerSprite->scale();
    double minX = sr.left()   + 110;
    double maxX = sr.right()  - pW - 110;
    double minY = sr.top()    + 110;
    double maxY = sr.bottom() - pH - 110;

    newPos.setX(qBound(minX, newPos.x(), maxX));
    newPos.setY(qBound(minY, newPos.y(), maxY));

    if (isWalkable(newPos))
    {
        playerSprite->setPos(newPos);
        game.getPlayer().moveTo(newPos.x(), newPos.y());
        checkArtifactCollisions();

        if (inLevel2)
        {
            if (level2Ptr)
                level2Ptr->updatePlayerGlow(playerSprite);

            if (level2DoorUnlocked)
                checkDoorCollision();
        }
    }
}

/* ================================================================
   WALKABLE CHECK
   ================================================================ */

bool GameWindow::isWalkable(QPointF newPos)
{
    if (collisionMask.isNull()) return true;

    QRectF rect = playerSprite->sceneBoundingRect();

    QPointF scenePos(
        newPos.x() + rect.width()  / 2,
        newPos.y() + rect.height() - 5
        );

    QRectF sceneRect = scene->sceneRect();

    int maskX = (scenePos.x() / sceneRect.width())  * collisionMask.width();
    int maskY = (scenePos.y() / sceneRect.height()) * collisionMask.height();

    QColor color = collisionMask.pixelColor(maskX, maskY);

    return !(color.red() < 20 && color.green() < 20 && color.blue() < 20);
}

/* ================================================================
   ARTIFACT COLLISIONS
   ================================================================ */

void GameWindow::checkArtifactCollisions()
{
    QGraphicsItem* artifact = currentLevel->checkArtifactCollision(playerSprite);
    if (!artifact) return;

    QString type = artifact->data(0).toString();
    currentLevel->removeArtifact(artifact, scene);
    game.collectArtifact(type);

    if (type == "timer")
        seconds += 30;

    if (collectSound) collectSound->play();

    if (!inLevel2)
        level1Collected++;

    if (inLevel2 && type == "rock")
    {
        rocksCollected++;

        int timeSinceLast = rockCollectTime - seconds;
        if (timeSinceLast < 20)      scoreMultiplier = 3;
        else if (timeSinceLast < 40) scoreMultiplier = 2;
        else                         scoreMultiplier = 1;
        rockCollectTime = seconds;

        int bonus = 20 * scoreMultiplier;
        game.getPlayer().addScore(bonus);

        statusLabel->setText(
            QString("STONE %1 SECURED!  +%2").arg(rocksCollected).arg(bonus)
            );
        statusLabel->setStyleSheet(
            "color: #ffdd44; font-size: 14px; font-weight: bold;"
            "background: rgba(0,0,0,120); padding: 4px 10px; border-radius: 4px;"
            );

        updateLevel2HUD();

        if (currentLevel->getArtifactCount() == 0 && !level2DoorUnlocked)
        {
            level2DoorUnlocked = true;
            showHieroglyphScreen();
            seconds += 30;
        }

        updateInventoryUI();
        updateHUD();
        return;
    }

    if (!inLevel2 && level1Collected >= 5)
    {
        timer->stop();

        if (startMusic && startMusic->isPlaying())
            startMusic->stop();

        QMessageBox* msg = new QMessageBox(this);
        msg->setWindowTitle("WING I SECURED!");
        msg->setText(
            "<span style='font-size:16px; color:#f5d060;'>"
            "<b>Well done, Night Guard!</b><br><br>"
            "You recovered <b>5 artifacts</b> from the first hall.<br>"
            "The east wing awaits... if you dare."
            "</span>"
            );
        msg->setStyleSheet(
            "QMessageBox { background-color: #0a0500; }"
            "QLabel { color: #f5e6c8; font-size: 13px; }"
            "QPushButton { background-color: rgba(180,130,40,200); color: #fff8e7;"
            "  font-size: 13px; font-weight: bold; border: 2px solid #c8a84b;"
            "  border-radius: 4px; padding: 6px 20px; }"
            );
        msg->addButton("ENTER THE VAULT", QMessageBox::AcceptRole);
        msg->exec();

        showLevel2BriefingPopup();
        return;
    }

    updateInventoryUI();
    updateHUD();
}

/* ================================================================
   DOOR COLLISION
   ================================================================ */

void GameWindow::checkDoorCollision()
{
    if (!level2Ptr) return;

    QPointF door   = level2Ptr->doorPosition();
    QPointF player = playerSprite->pos();

    double dx   = door.x() - player.x();
    double dy   = door.y() - player.y();
    double dist = std::sqrt(dx*dx + dy*dy);

    if (dist < 80)
    {
        timer->stop();

        QMessageBox* msg = new QMessageBox(this);
        msg->setWindowTitle("THE VAULT IS SECURE!");
        msg->setText(
            "<span style='font-size:16px; color:#f5d060;'>"
            "<b>You escaped the Hieroglyph Vault!</b><br><br>"
            "The three Sacred Stones are recovered.<br>"
            "History lives on because of you, Night Guard."
            "</span>"
            );
        msg->setStyleSheet(
            "QMessageBox { background-color: #0a0500; }"
            "QLabel { color: #f5e6c8; font-size: 13px; }"
            "QPushButton { background-color: rgba(180,130,40,200); color: #fff8e7;"
            "  font-size: 13px; font-weight: bold; border: 2px solid #c8a84b;"
            "  border-radius: 4px; padding: 6px 20px; }"
            );
        msg->addButton("FINISH", QMessageBox::AcceptRole);
        msg->exec();

        stack->setCurrentWidget(winScreen);
    }
}

/* ================================================================
   HUD UPDATE
   ================================================================ */

void GameWindow::updateHUD()
{
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;

    clockLabel->setText(
        QString("%1:%2:%3")
            .arg(h, 2, 10, QChar('0'))
            .arg(m, 2, 10, QChar('0'))
            .arg(s, 2, 10, QChar('0'))
        );

    scoreLabel->setText("Score: " + QString::number(game.getPlayer().getScore()));
}

/* ================================================================
   INVENTORY UPDATE
   ================================================================ */

void GameWindow::updateInventoryUI()
{
    if (game.getLevelIndex() == 2)
    {
        coinIcon->setPixmap(QPixmap(":/new/prefix1/images/rock_M.png").scaled(40, 40));
        scrollIcon->clear();
        maskIcon->clear();
        amuletIcon->clear();
        timerIcon->clear();
        coinCounter->setText("x" + QString::number(rocksCollected));
        scrollCounter->setText("");
        maskCounter->setText("");
        amuletCounter->setText("");
        timerCounter->setText("");
        return;
    }

    coinIcon->setPixmap(QPixmap(":/new/prefix1/images/coint.png").scaled(40, 40));
    scrollIcon->setPixmap(QPixmap(":/new/prefix1/images/scrollt.png").scaled(40, 40));
    maskIcon->setPixmap(QPixmap(":/new/prefix1/images/maskt.png").scaled(40, 40));
    amuletIcon->setPixmap(QPixmap(":/new/prefix1/images/amulett.png").scaled(40, 40));
    timerIcon->setPixmap(QPixmap(":/new/prefix1/images/timert.png").scaled(40, 40));

    coinCounter->setText("x" + QString::number(game.getArtifactCount("coin")));
    scrollCounter->setText("x" + QString::number(game.getArtifactCount("scroll")));
    maskCounter->setText("x" + QString::number(game.getArtifactCount("mask")));
    amuletCounter->setText("x" + QString::number(game.getArtifactCount("amulet")));
    timerCounter->setText("x" + QString::number(game.getArtifactCount("timer")));
}

/* ================================================================
   UPDATE GAME (timer tick)
   ================================================================ */

void GameWindow::updateGame()
{
    seconds--;
    updateHUD();

    if (inLevel2 && level2Ptr && playerSprite)
    {
        level2Ptr->updatePlayerGlow(playerSprite);
        updateLevel2HUD();
        updateTorchFlicker();
        updateMiniMap();

        if (isInvincible)
        {
            invincibleFrames--;
            if (invincibleFrames <= 0)
                isInvincible = false;
        }
    }

    game.update(1.0f);

    if (seconds <= 0)
    {
        timer->stop();

        QMessageBox* msg = new QMessageBox(this);
        msg->setWindowTitle("TIME'S UP!");
        msg->setText(
            "<span style='font-size:16px; color:#c8a84b;'>"
            "<b>The museum is lost...</b><br><br>"
            "You ran out of time, Night Guard.<br>"
            "The artifacts remain in the shadows."
            "</span>"
            );
        msg->setStyleSheet(
            "QMessageBox { background-color: #0a0500; }"
            "QLabel { color: #f5e6c8; font-size: 13px; }"
            "QPushButton { background-color: rgba(180,130,40,200); color: #fff8e7;"
            "  font-size: 13px; font-weight: bold; border: 2px solid #c8a84b;"
            "  border-radius: 4px; padding: 6px 20px; }"
            );
        QPushButton* restartBtn = msg->addButton("TRY AGAIN", QMessageBox::AcceptRole);
        msg->addButton("EXIT", QMessageBox::RejectRole);
        msg->exec();

        if (msg->clickedButton() == restartBtn)
            restartGame();
        else
            exitGame();

        return;
    }

    if (game.getstate() == Gamestate::gameOver)
    {
        timer->stop();
        stack->setCurrentWidget(gameOverScreen);
    }
}

/* ================================================================
   KEY PRESS
   ================================================================ */

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if (stack->currentWidget() != gameScreen) return;

    int step = 3;

    if      (event->key() == Qt::Key_Up)    movePlayer(0,     -step, spriteBack);
    else if (event->key() == Qt::Key_Down)  movePlayer(0,      step, spriteFront);
    else if (event->key() == Qt::Key_Left)  movePlayer(-step,  0,    spriteLeft);
    else if (event->key() == Qt::Key_Right) movePlayer( step,  0,    spriteRight);
    else if (event->key() == Qt::Key_Escape) pauseGame();
}

/* ================================================================
   PAUSE GAME
   ================================================================ */

void GameWindow::pauseGame()
{
    game.pauseGame();
    if (mummy) mummy->setPaused(true);
    timer->stop();

    QWidget* dimmer = new QWidget(this);
    dimmer->setGeometry(0, 0, width(), height());
    dimmer->setStyleSheet("background-color: rgba(0,0,0,170);");
    dimmer->show();
    dimmer->raise();

    QWidget* popup = new QWidget(dimmer);
    popup->setFixedSize(520, 360);
    popup->move((width() - 520) / 2, (height() - 360) / 2);
    popup->setStyleSheet(
        "background-color: rgba(8,4,0,235);"
        "border: 2px solid rgba(200,160,60,200);"
        "border-radius: 14px;"
        );

    QVBoxLayout* layout = new QVBoxLayout(popup);
    layout->setContentsMargins(40, 35, 40, 35);
    layout->setSpacing(20);

    QLabel* title = new QLabel("GAME PAUSED");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 28px; font-weight: bold; letter-spacing: 6px;"
        "color: #f5d060; background: transparent; border: none;"
        );

    QLabel* message = new QLabel("The museum remains in silence...\nPress continue when you're ready.");
    message->setAlignment(Qt::AlignCenter);
    message->setWordWrap(true);
    message->setStyleSheet("font-size: 15px; color: #e8d5a8; background: transparent; border: none;");

    QString buttonStyle =
        "QPushButton { background-color: rgba(180,130,40,220); color: #fff8e7;"
        " font-size: 15px; font-weight: bold; letter-spacing: 3px;"
        " border: 2px solid #c8a84b; border-radius: 8px; padding: 14px; }"
        "QPushButton:hover { background-color: rgba(220,170,60,240); border: 2px solid #ffffff; }";

    QPushButton* continueBtn = new QPushButton("▶   CONTINUE");
    continueBtn->setCursor(Qt::PointingHandCursor);
    continueBtn->setFixedHeight(55);
    continueBtn->setStyleSheet(buttonStyle);

    QPushButton* exitBtn = new QPushButton("✕   EXIT GAME");
    exitBtn->setCursor(Qt::PointingHandCursor);
    exitBtn->setFixedHeight(55);
    exitBtn->setStyleSheet(
        "QPushButton { background-color: rgba(100,30,20,220); color: #fff8e7;"
        " font-size: 15px; font-weight: bold; letter-spacing: 3px;"
        " border: 2px solid #8a2020; border-radius: 8px; padding: 14px; }"
        "QPushButton:hover { background-color: rgba(160,40,30,240); }"
        );

    layout->addWidget(title);
    layout->addWidget(message);
    layout->addStretch();
    layout->addWidget(continueBtn);
    layout->addWidget(exitBtn);

    popup->show();

    connect(continueBtn, &QPushButton::clicked, this, [=]() {
        dimmer->deleteLater();
        game.resumeGame();
        if (mummy) mummy->setPaused(false);
        timer->start(1000);
        stack->setCurrentWidget(gameScreen);
        this->setFocus();
    });

    connect(exitBtn, &QPushButton::clicked, this, [=]() {
        dimmer->deleteLater();
        exitGame();
    });
}

/* ================================================================
   BRIEFING POPUP (before Level 1)
   ================================================================ */

void GameWindow::showBriefingPopup(const QString &playerName)
{
    this->playerName = playerName;

    QWidget* dimmer = new QWidget(this);
    dimmer->setGeometry(0, 0, width(), height());
    dimmer->setStyleSheet("background-color:rgba(0,0,0,200);");
    dimmer->show();
    dimmer->raise();

    QWidget* popup = new QWidget(dimmer);
    popup->setFixedSize(760, 620);
    popup->move((dimmer->width() - 760) / 2, (dimmer->height() - 620) / 2);
    popup->setStyleSheet(
        "background-color:rgba(5,3,0,240);"
        "border:2px solid rgba(200,160,60,220);"
        "border-radius:14px;"
        );

    QVBoxLayout* pl = new QVBoxLayout(popup);
    pl->setContentsMargins(44, 36, 44, 32);
    pl->setSpacing(18);

    QLabel* title = new QLabel("NIGHT SHIFT INITIATED!");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size:24px; font-weight:bold; color:#f5d060;"
        "letter-spacing:4px; background:transparent; border:none;"
        );

    QString storyText = QString(
                            "Welcome, <b>%1</b>.<br><br>"
                            "You are the night guard of the Grand Egyptian Museum — the largest archaeological museum in the world.<br><br>"
                            "Priceless artifacts have gone missing from the halls. "
                            "Something ancient has awakened... and it hunts."
                            ).arg(playerName);

    QLabel* story = new QLabel(storyText);
    story->setWordWrap(true);
    story->setAlignment(Qt::AlignCenter);
    story->setStyleSheet(
        "font-size:15px; color:#e8d5a8; background:transparent; border:none;"
        );

    QLabel* objective = new QLabel(
        "<span style='color:#c8a84b; font-weight:bold; font-size:15px;'>YOUR MISSION BRIEFING</span><br><br>"
        "<span style='color:#f5d060;'>&#9656;</span> "
        "<b>RECOVER THE RELICS</b> — find every missing artifact before dawn.<br>"
        "<span style='color:#f5d060;'>&#9656;</span> "
        "<b>FACE THE UNKNOWN</b> — watch out for enemies lurking in the dark!<br>"
        "<span style='color:#f5d060;'>&#9656;</span> "
        "<b>LEVEL UP</b> — progress through increasingly challenging wings.<br>"
        "<span style='color:#f5d060;'>&#9656;</span> "
        "<b>FOLLOW THE TRAIL</b> — use hidden hints to track what was stolen."
        );
    objective->setWordWrap(true);
    objective->setStyleSheet(
        "font-size:13px; color:#d4c090; background:transparent; border:none;"
        );

    QLabel* warning = new QLabel("The hieroglyphs whisper in the dark...");
    warning->setAlignment(Qt::AlignCenter);
    warning->setStyleSheet(
        "font-size:13px; color:rgba(200,160,60,180); font-style:italic;"
        "background:transparent; border:none;"
        );

    QPushButton* enterBtn = new QPushButton("▶   ENTER THE MUSEUM");
    enterBtn->setFixedHeight(58);
    enterBtn->setCursor(Qt::PointingHandCursor);
    enterBtn->setStyleSheet(
        "QPushButton { background-color:rgba(180,130,40,220); color:#fff8e7;"
        " font-size:16px; font-weight:bold; letter-spacing:3px;"
        " border:2px solid #c8a84b; border-radius:8px; }"
        "QPushButton:hover { background-color:rgba(220,170,60,240); border:2px solid #fff; }"
        "QPushButton:pressed { background-color:rgba(130,90,20,255); }"
        );

    pl->addWidget(title);
    pl->addWidget(story);
    pl->addWidget(objective);
    pl->addWidget(warning);
    pl->addStretch();
    pl->addWidget(enterBtn, 0, Qt::AlignCenter);

    popup->show();

    connect(enterBtn, &QPushButton::clicked, this, [=]() {
        dimmer->deleteLater();
        startGame();
    });
}

/* ================================================================
   LEVEL 2 BRIEFING POPUP
   ================================================================ */

void GameWindow::showLevel2BriefingPopup()
{
    timer->stop();
    if (mummy) mummy->setPaused(true);

    QWidget* dimmer = new QWidget(this);
    dimmer->setGeometry(0, 0, width(), height());
    dimmer->setStyleSheet("background-color:rgba(0,0,0,200);");
    dimmer->show();
    dimmer->raise();

    QWidget* popup = new QWidget(dimmer);
    popup->setFixedSize(760, 620);
    popup->move((dimmer->width() - 760) / 2, (dimmer->height() - 620) / 2);
    popup->setStyleSheet(
        "background-color:rgba(5,3,0,240);"
        "border:2px solid rgba(200,160,60,220);"
        "border-radius:14px;"
        );

    QVBoxLayout* pl = new QVBoxLayout(popup);
    pl->setContentsMargins(44, 36, 44, 32);
    pl->setSpacing(18);

    QLabel* title = new QLabel("WING II — THE HIEROGLYPH VAULT");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size:24px; font-weight:bold; color:#f5d060;"
        "letter-spacing:4px; background:transparent; border:none;"
        );

    QString storyText = QString(
                            "You secured the first hall — well done, <b>%1</b>.<br><br>"
                            "You now enter the sacred Room of Tutankhamun.<br><br>"
                            "But the east wing has gone dark. "
                            "Something ancient has awakened in the deepest vault "
                            "of the museum... and it is not alone."
                            ).arg(playerName);

    QLabel* story = new QLabel(storyText);
    story->setWordWrap(true);
    story->setAlignment(Qt::AlignCenter);
    story->setStyleSheet(
        "font-size:15px; color:#e8d5a8; background:transparent; border:none;"
        );

    QLabel* objective = new QLabel(
        "<span style='color:#c8a84b; font-weight:bold; font-size:15px;'>NEW OBJECTIVE</span><br><br>"
        "The vault contains <b>3 Sacred Stones</b>, each carved "
        "with an ancient hieroglyph.<br><br>"
        "The stones have been scattered across the chamber. "
        "<b>Collect all three</b> to reveal their secret.<br><br>"
        "<span style='color:#f5d060;'>&#9656;</span> "
        "The room is <b>pitch black</b> — trust your torch.<br><br>"
        "<span style='color:#f5d060;'>&#9656;</span> "
        "A dark spirit roams the chamber. Avoid it or lose score.<br><br>"
        "<span style='color:#f5d060;'>&#9656;</span> "
        "Collect all three stones to unlock the ancient secret."
        );
    objective->setWordWrap(true);
    objective->setStyleSheet(
        "font-size:13px; color:#d4c090; background:transparent; border:none;"
        );

    QLabel* warning = new QLabel("The hieroglyphs whisper in the dark...");
    warning->setAlignment(Qt::AlignCenter);
    warning->setStyleSheet(
        "font-size:13px; color:rgba(200,160,60,180); font-style:italic;"
        "background:transparent; border:none;"
        );

    QPushButton* enterBtn = new QPushButton("▶   ENTER THE ROOM OF TUTANKHAMUN");
    enterBtn->setFixedHeight(58);
    enterBtn->setCursor(Qt::PointingHandCursor);
    enterBtn->setStyleSheet(
        "QPushButton { background-color:rgba(180,130,40,220); color:#fff8e7;"
        " font-size:16px; font-weight:bold; letter-spacing:3px;"
        " border:2px solid #c8a84b; border-radius:8px; }"
        "QPushButton:hover { background-color:rgba(220,170,60,240); border:2px solid #fff; }"
        "QPushButton:pressed { background-color:rgba(130,90,20,255); }"
        );

    pl->addWidget(title);
    pl->addWidget(story);
    pl->addWidget(objective);
    pl->addWidget(warning);
    pl->addStretch();
    pl->addWidget(enterBtn, 0, Qt::AlignCenter);

    popup->show();

    connect(enterBtn, &QPushButton::clicked, this, [=]() {
        dimmer->deleteLater();
        startLevel2();
    });
}

/* ================================================================
   HIEROGLYPH SCREEN (shown after all 3 rocks collected)
   ================================================================ */

void GameWindow::showHieroglyphScreen()
{
    timer->stop();

    QWidget* dimmer = new QWidget(this);
    dimmer->setGeometry(0, 0, width(), height());
    dimmer->setStyleSheet("background-color:rgba(0,0,0,220);");
    dimmer->show();
    dimmer->raise();

    QWidget* popup = new QWidget(dimmer);
    popup->setFixedSize(860, 640);
    popup->move((width() - 900) / 2, (height() - 650) / 2);
    popup->setStyleSheet(
        "background-color:rgba(8,4,0,240);"
        "border:2px solid rgba(200,160,60,220);"
        "border-radius:14px;"
        );

    QVBoxLayout* layout = new QVBoxLayout(popup);
    layout->setContentsMargins(30, 20, 30, 20);
    layout->setSpacing(16);

    QLabel* title = new QLabel("THE SACRED STONES REVEAL A WORD");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size:22px; font-weight:bold; color:#f5d060; letter-spacing:4px;"
        );

    QLabel* image = new QLabel();
    image->setPixmap(
        QPixmap(":/new/prefix1/images/hieroglyph_chart-2.png")
            .scaled(700, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    image->setAlignment(Qt::AlignCenter);

    QLabel* password = new QLabel("The symbols translate to:\n\nM   •   A   •   N\n\nMemorize the ancient password.");
    password->setAlignment(Qt::AlignCenter);
    password->setStyleSheet("font-size:18px; font-weight:bold; color:#f5d060; line-height:1.8;");

    QPushButton* continueBtn = new QPushButton("▶ CONTINUE");
    continueBtn->setFixedHeight(50);
    continueBtn->setStyleSheet(
        "QPushButton { background-color:rgba(180,130,40,220); color:white;"
        " font-size:15px; font-weight:bold; border-radius:8px;"
        " border:2px solid #c8a84b; padding:10px; }"
        "QPushButton:hover { background-color:rgba(220,170,60,240); }"
        );

    layout->addWidget(title);
    layout->addWidget(image);
    layout->addStretch();
    layout->addWidget(continueBtn, 0, Qt::AlignCenter);

    popup->show();

    connect(continueBtn, &QPushButton::clicked, this, [=]() {
        dimmer->deleteLater();

        game.loadLevel(3);
        currentLevel = game.getCurrentLevel();
        scene->clear();
        currentLevel->loadScene(scene);

        collisionMask = QImage(":/new/prefix1/images/level3 BW.png");

        playerSprite = scene->addPixmap(spriteFront);
        playerSprite->setScale(0.12);
        playerSprite->setPos(150, 650);
        game.getPlayer().moveTo(150, 650);

        seconds = 300;
        timer->start(1000);

        updateHUD();
        updateInventoryUI();
        stack->setCurrentWidget(gameScreen);
        this->setFocus();
    });
}

/* ================================================================
   FIREBALL GAME OVER
   ================================================================ */

void GameWindow::showFireballGameOver()
{
    timer->stop();
    game.pauseGame();
    if (mummy) mummy->setPaused(true);

    QWidget* dimmer = new QWidget(this);
    dimmer->setGeometry(0, 0, width(), height());
    dimmer->setStyleSheet("background-color: rgba(0,0,0,210);");
    dimmer->show();
    dimmer->raise();

    QWidget* popup = new QWidget(dimmer);
    popup->setFixedSize(600, 420);
    popup->move((dimmer->width() - 600) / 2, (dimmer->height() - 420) / 2);
    popup->setStyleSheet(
        "background-color: rgba(60,5,0,240);"
        "border: 3px solid rgba(220,60,20,200);"
        "border-radius: 16px;"
        );

    QVBoxLayout* layout = new QVBoxLayout(popup);
    layout->setContentsMargins(50, 40, 50, 40);
    layout->setSpacing(18);

    QLabel* flames = new QLabel("🔥  🔥  🔥");
    flames->setAlignment(Qt::AlignCenter);
    flames->setStyleSheet("font-size: 36px; background: transparent; border: none;");

    QLabel* title = new QLabel("GAME OVER");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 52px; font-weight: bold; letter-spacing: 10px;"
        "color: #ff3300; background: transparent; border: none;"
        );

    QLabel* subtitle = new QLabel("You were consumed by RA's flames!");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setWordWrap(true);
    subtitle->setStyleSheet("font-size: 15px; color: #f5c89a; background: transparent; border: none;");

    QString btnStyle =
        "QPushButton { background-color: rgba(180,130,40,220); color: #fff8e7;"
        "  font-size: 15px; font-weight: bold; letter-spacing: 3px;"
        "  border: 2px solid #c8a84b; border-radius: 8px; padding: 14px; }"
        "QPushButton:hover { background-color: rgba(220,170,60,240); border: 2px solid #ffffff; }";

    QPushButton* restartBtn = new QPushButton("↺   TRY AGAIN");
    restartBtn->setFixedHeight(52);
    restartBtn->setCursor(Qt::PointingHandCursor);
    restartBtn->setStyleSheet(btnStyle);

    QPushButton* exitBtn = new QPushButton("✕   EXIT GAME");
    exitBtn->setFixedHeight(52);
    exitBtn->setCursor(Qt::PointingHandCursor);
    exitBtn->setStyleSheet(
        "QPushButton { background-color: rgba(100,30,20,220); color: #fff8e7;"
        "  font-size: 15px; font-weight: bold; letter-spacing: 3px;"
        "  border: 2px solid #8a2020; border-radius: 8px; padding: 14px; }"
        "QPushButton:hover { background-color: rgba(160,40,30,240); }"
        );

    layout->addWidget(flames);
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addStretch();
    layout->addWidget(restartBtn);
    layout->addWidget(exitBtn);

    popup->show();

    connect(restartBtn, &QPushButton::clicked, this, [=]() { dimmer->deleteLater(); restartGame(); });
    connect(exitBtn,    &QPushButton::clicked, this, [=]() { dimmer->deleteLater(); exitGame(); });
}

/* ================================================================
   RESTART
   ================================================================ */

void GameWindow::restartGame()
{
    timer->stop();

    if (mummy)
    {
        mummy->setPaused(true);
        mummy = nullptr;
    }

    scene->clear();
    inLevel2           = false;
    level2DoorUnlocked = false;
    level2Ptr          = nullptr;
    level1Collected    = 0;
    rocksCollected     = 0;
    torchFlicker       = 0;
    scoreMultiplier    = 1;
    wraithWarningShown = false;
    if (level2HUD) level2HUD->hide();
    if (miniMap)   miniMap->hide();

    playerHealth     = 3;
    isInvincible     = false;
    invincibleFrames = 0;

    coinIcon->show();   scrollIcon->show();
    maskIcon->show();   amuletIcon->show();
    timerIcon->show();
    coinCounter->show();  scrollCounter->show();
    maskCounter->show();  amuletCounter->show();
    timerCounter->show();

    game.restartGame();
    startGame();
}

/* ================================================================
   EXIT
   ================================================================ */

void GameWindow::exitGame()
{
    game.exitGame();
    close();
}

/* ================================================================
   RESIZE
   ================================================================ */

void GameWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
}

/* ================================================================
   SAVE / LOAD
   ================================================================ */

void GameWindow::saveGame()
{
    QFile file("savegame.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << QDir::currentPath();
        QTextStream out(&file);
        out << "PlayerX: " << game.getPlayer().getX() << "\n";
        out << "PlayerY: " << game.getPlayer().getY() << "\n";
        out << "Score: "   << game.getPlayer().getScore() << "\n";
        out << "Seconds: " << seconds << "\n";
        out << "Coins: "   << coinCount << "\n";
        out << "Scrolls: " << scrollCount << "\n";
        out << "Masks: "   << maskCount << "\n";
        out << "Amulets: " << amuletCount << "\n";
        out << "Timers: "  << timerCount << "\n";
        file.close();
        QMessageBox::information(this, "Saved", "Game saved successfully!");
    }
}

void GameWindow::loadGame()
{
    QFile file("savegame.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        int x, y, score, secs, lvl;
        in >> x >> y >> score >> secs >> lvl;
        file.close();

        game.getPlayer().moveTo(x, y);
        seconds = secs;

        if (playerSprite)
            playerSprite->setPos(x, y);

        updateHUD();
    }
}

/* ================================================================
   VICTORY SCREEN
   ================================================================ */

void GameWindow::setupVictoryScreen()
{
    victoryScreen = new QWidget();
    victoryScreen->setStyleSheet(
        "background-image:url(:/new/prefix1/images/victory_background.png);"
        "background-position:center; background-repeat:no-repeat; background-color:black;"
        );

    QVBoxLayout* layout = new QVBoxLayout(victoryScreen);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(18);

    QLabel* title = new QLabel("HERITAGE PROTECTED");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size:38px; font-weight:bold; letter-spacing:6px;"
        "color:#f5d060; background:transparent;"
        );

    QLabel* subtitle = new QLabel("The museum is safe once more.");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("font-size:16px; color:#f5d8a0; background:transparent;");

    QLabel* score = new QLabel("Final Score: " + QString::number(game.getPlayer().getScore()));
    score->setAlignment(Qt::AlignCenter);
    score->setStyleSheet("font-size:20px; font-weight:bold; color:white; background:transparent;");

    QString buttonStyle =
        "QPushButton { background-color:rgba(180,130,40,220); color:#fff8e7;"
        " font-size:16px; font-weight:bold; letter-spacing:3px;"
        " border:2px solid #c8a84b; border-radius:8px; padding:14px 30px; }"
        "QPushButton:hover { background-color:rgba(220,170,60,240); border:2px solid white; }";

    QPushButton* replayBtn = new QPushButton("▶   PLAY AGAIN");
    QPushButton* exitBtn   = new QPushButton("✕   EXIT");

    replayBtn->setStyleSheet(buttonStyle);
    exitBtn->setStyleSheet(buttonStyle);
    replayBtn->setFixedSize(260, 58);
    exitBtn->setFixedSize(260, 58);

    connect(replayBtn, &QPushButton::clicked, this, &GameWindow::restartGame);
    connect(exitBtn,   &QPushButton::clicked, this, &GameWindow::exitGame);

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addSpacing(14);
    layout->addWidget(score);
    layout->addSpacing(30);
    layout->addWidget(replayBtn, 0, Qt::AlignCenter);
    layout->addWidget(exitBtn,   0, Qt::AlignCenter);
    layout->addStretch();

    stack->addWidget(victoryScreen);
}

void GameWindow::showVictoryScreen()
{
    stack->setCurrentWidget(victoryScreen);
}

void GameWindow::showTreasureRecoveredPopup()
{
    timer->stop();

    QWidget* dimmer = new QWidget(this);
    dimmer->setGeometry(0, 0, width(), height());
    dimmer->setStyleSheet("background-color:rgba(0,0,0,220);");
    dimmer->show();
    dimmer->raise();

    QWidget* popup = new QWidget(dimmer);
    popup->setFixedSize(760, 560);
    popup->move((dimmer->width() - 760) / 2, (dimmer->height() - 560) / 2);
    popup->setStyleSheet(
        "background-color:rgba(5,3,0,245);"
        "border:2px solid rgba(200,160,60,220);"
        "border-radius:16px;"
        );

    QVBoxLayout* layout = new QVBoxLayout(popup);
    layout->setContentsMargins(40, 30, 40, 30);
    layout->setSpacing(18);

    QLabel* title = new QLabel("THE FINAL VAULT HAS BEEN OPENED");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size:24px; font-weight:bold; letter-spacing:4px;"
        "color:#f5d060; background:transparent; border:none;"
        );

    QLabel* image = new QLabel();
    image->setPixmap(
        QPixmap(":/new/prefix1/images/opened_treasure.png")
            .scaled(420, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        );
    image->setAlignment(Qt::AlignCenter);
    image->setStyleSheet("background:transparent; border:none;");

    QLabel* text = new QLabel(
        "The sacred relic of Tutankhamun has finally been recovered.<br><br>"
        "For centuries it remained hidden beneath the sands of time.<br><br>"
        "Tonight, history survives because of you."
        );
    text->setWordWrap(true);
    text->setAlignment(Qt::AlignCenter);
    text->setStyleSheet("font-size:14px; color:#e8d5a8; background:transparent; border:none;");

    QPushButton* continueBtn = new QPushButton("▶   CONTINUE");
    continueBtn->setFixedHeight(54);
    continueBtn->setCursor(Qt::PointingHandCursor);
    continueBtn->setStyleSheet(
        "QPushButton { background-color:rgba(180,130,40,220); color:#fff8e7;"
        " font-size:15px; font-weight:bold; letter-spacing:3px;"
        " border:2px solid #c8a84b; border-radius:8px; }"
        "QPushButton:hover { background-color:rgba(220,170,60,240); border:2px solid #fff; }"
        );

    layout->addWidget(title);
    layout->addWidget(image);
    layout->addWidget(text);
    layout->addStretch();
    layout->addWidget(continueBtn, 0, Qt::AlignCenter);

    popup->show();

    connect(continueBtn, &QPushButton::clicked, this, [=]() {
        dimmer->deleteLater();
        showVictoryScreen();
    });
}
