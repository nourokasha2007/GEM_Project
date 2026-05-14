#include "gameWindow.h"
#include <QDebug>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include "level1enemy.h"
#include "Level2.h"
#include "Level3.h"

/* ================= CONSTRUCTOR ================= */

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //================ DEFAULT VALUES ================//
    mummy = nullptr;
    seconds = 300;

    currentLevel = nullptr;

    view = nullptr;

    playerSprite = nullptr;

    ghost =nullptr;

    level3Music=nullptr;

    horrorMusic = nullptr;

    startMusic = nullptr;
    //================ STACK =================//

    stack =
        new QStackedWidget(this);

    setCentralWidget(stack);

    //================ SETUP SCREENS =================//

    setupStartScreen();

    setupGameScreen();

    //================ INPUT =================//

    setFocusPolicy(
        Qt::StrongFocus
        );

    //================ TIMER =================//

    timer =
        new QTimer(this);

    connect(
        timer,
        &QTimer::timeout,
        this,
        &GameWindow::updateGame
        );

    //================ FIRST SCREEN =================//

    stack->setCurrentWidget(
        startScreen
        );

    showMaximized();

    //================ MUSIC =================//

    startMusic =
        new QSoundEffect(this);

    startMusic->setSource(
        QUrl("qrc:/new/prefix1/sounds/desert sounds.wav")
        );

    startMusic->setVolume(0.5);

    startMusic->setLoopCount(
        QSoundEffect::Infinite
        );

    startMusic->play();
}

/* ================= START SCREEN ================= */

void GameWindow::setupStartScreen()
{
    startScreen =
        new QWidget();

    startScreen->setStyleSheet(
        "QWidget#startScreen {"
        "  background-image: url(:/new/prefix1/images/PHOTO-2026-04-29-21-07-06.jpg);"
        "  background-position: center;"
        "  background-repeat: no-repeat;"
        "  background-color: #000000;"
        "}"
        );

    startScreen->setObjectName(
        "startScreen"
        );

    //================ DARK OVERLAY =================//

    bgOverlay =
        new QLabel(startScreen);

    bgOverlay->setStyleSheet(
        "background-color: rgba(0,0,0,110);"
        );

    bgOverlay->setGeometry(
        0,
        0,
        9999,
        9999
        );

    //================ MAIN LAYOUT =================//

    QVBoxLayout *layout =
        new QVBoxLayout(startScreen);

    layout->setAlignment(
        Qt::AlignCenter
        );

    layout->setSpacing(12);
    //================ CARD =================//

    QWidget *card = new QWidget();

    card->setFixedSize(380, 220);

    card->setStyleSheet(
        "background-color: rgba(10, 5, 0, 185);"
        "border: 1px solid rgba(200, 160, 60, 160);"
        "border-radius: 10px;"
        );

    QVBoxLayout *cardLayout =
        new QVBoxLayout(card);

    cardLayout->setSpacing(12);

    cardLayout->setContentsMargins(
        30,
        24,
        30,
        24
        );

    QLabel *cardTitle =
        new QLabel("ENTER THE MUSEUM");

    cardTitle->setAlignment(
        Qt::AlignCenter
        );

    cardTitle->setStyleSheet(
        "font-size: 14px;"
        "font-weight: bold;"
        "color: #c8a84b;"
        "letter-spacing: 4px;"
        "background: transparent;"
        "border: none;"
        );

    //================ NAME INPUT =================//

    guestNameEdit =
        new QLineEdit();

    guestNameEdit->setPlaceholderText(
        "Your Name"
        );

    guestNameEdit->setFixedHeight(44);

    guestNameEdit->setStyleSheet(
        "QLineEdit {"
        "  background-color: rgba(255,255,255,15);"
        "  border: 1px solid rgba(200,160,60,120);"
        "  border-radius: 4px;"
        "  color: #f5e6c8;"
        "  font-size: 15px;"
        "  padding-left: 12px;"
        "}"
        "QLineEdit:focus {"
        "  border: 1px solid #c8a84b;"
        "  background-color: rgba(255,255,255,25);"
        "}"
        );

    //================ BUTTON =================//

    QPushButton *enterBtn =
        new QPushButton(
            "▶   PROTECT THE HERITAGE"
            );

    enterBtn->setFixedHeight(48);

    enterBtn->setCursor(
        Qt::PointingHandCursor
        );

    enterBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(180,130,40,220);"
        "  color: #fff8e7;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "  letter-spacing: 3px;"
        "  border: 2px solid #c8a84b;"
        "  border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(220,170,60,240);"
        "  border: 2px solid #ffffff;"
        "  color: #ffffff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: rgba(130,90,20,255);"
        "}"
        );

    cardLayout->addWidget(cardTitle);

    cardLayout->addWidget(guestNameEdit);

    cardLayout->addSpacing(4);

    cardLayout->addWidget(enterBtn);

    connect(
        enterBtn,
        &QPushButton::clicked,
        this,
        [=]()
        {
            QString name =
                guestNameEdit->text().trimmed();

            if(name.isEmpty())
            {
                name = "Night Guard";
            }

            showBriefingPopup(name);
        }
        );

    layout->addStretch(3);

    layout->addWidget(
        card,
        0,
        Qt::AlignCenter
        );

    layout->addStretch(1);

    startScreen->setMinimumSize(
        1000,
        700
        );

    stack->addWidget(startScreen);
}
/* ================= BRIEFING POPUP ================= */
void GameWindow::showBriefingPopup(const QString &playerName)
{
    this->playerName = playerName;
    QWidget *dimmer =
        new QWidget(this);

    dimmer->setGeometry(
        0,
        0,
        width(),
        height()
        );

    dimmer->setStyleSheet(
        "background-color: rgba(0,0,0,180);"
        );

    dimmer->show();

    dimmer->raise();

    //================ POPUP =================//

    QWidget *popup =
        new QWidget(dimmer);

    popup->setFixedSize(
        620,
        480
        );

    popup->move(
        (dimmer->width()  - 620) / 2,
        (dimmer->height() - 480) / 2
        );

    popup->setStyleSheet(
        "background-color: rgba(8, 4, 0, 230);"
        "border: 2px solid rgba(200, 160, 60, 200);"
        "border-radius: 12px;"
        );

    QVBoxLayout *pl =
        new QVBoxLayout(popup);

    pl->setContentsMargins(
        40,
        32,
        40,
        28
        );

    pl->setSpacing(14);


    //================ HEADER =================//

    QLabel *header =
        new QLabel("NIGHT SHIFT INITIATED!");

    header->setAlignment(
        Qt::AlignCenter
        );

    header->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #f5d060;"
        "letter-spacing: 4px;"
        "background: transparent;"
        "border: none;"
        );

    //================ WELCOME =================//

    QString welcomeText =
        QString(
            "Welcome aboard, "
            "<b><span style='color:#f5d060;'>%1</span></b>, "
            "to the prestigious "
            "<b>GRAND EGYPTIAN MUSEUM!</b><br><br>"

            "The sun has set, the tourists are gone — "
            "but the halls are far from quiet. "

            "Priceless artifacts have vanished "
            "into the shadows!<br>"

            "As our lead Night Guard, "
            "the museum's legacy rests "
            "on your shoulders."
            ).arg(playerName);

    QLabel *welcome =
        new QLabel(welcomeText);

    welcome->setWordWrap(true);

    welcome->setAlignment(
        Qt::AlignCenter
        );

    welcome->setStyleSheet(
        "font-size: 13px;"
        "color: #e8d5a8;"
        "background: transparent;"
        "border: none;"
        "line-height: 1.5;"
        );

    //================ MISSIONS =================//

    QLabel *missions =
        new QLabel(
            "<span style='color:#c8a84b; font-weight:bold;'>YOUR MISSION BRIEFING</span><br><br>"

            "<span style='color:#f5d060;'>&#9656;</span> "
            "<b>RECOVER THE RELICS</b> — find every missing artifact before dawn.<br>"

            "<span style='color:#f5d060;'>&#9656;</span> "
            "<b>FACE THE UNKNOWN</b> — watch out for enemies lurking in the dark!<br>"

            "<span style='color:#f5d060;'>&#9656;</span> "
            "<b>LEVEL UP</b> — progress through increasingly challenging wings.<br>"

            "<span style='color:#f5d060;'>&#9656;</span> "
            "<b>FOLLOW THE TRAIL</b> — use hidden hints to track what was stolen."
            );

    missions->setWordWrap(true);

    missions->setStyleSheet(
        "font-size: 12px;"
        "color: #d4c090;"
        "background: transparent;"
        "border: none;"
        "line-height:1.6;"
        );

    //================ WARNING =================//

    QLabel *warning =
        new QLabel(
            "The museum opens at 7:00 AM sharp. "
            "Don't let history stay empty!"
            );

    warning->setAlignment(
        Qt::AlignCenter
        );

    warning->setWordWrap(true);

    warning->setStyleSheet(
        "font-size: 12px;"
        "color: rgba(200,160,60,180);"
        "font-style:italic;"
        "background: transparent;"
        "border: none;"
        );

    //================ BUTTON =================//

    QPushButton *beginBtn =
        new QPushButton(
            "▶   PROTECT THE HERITAGE"
            );

    beginBtn->setFixedHeight(50);

    beginBtn->setCursor(
        Qt::PointingHandCursor
        );

    beginBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(180,130,40,220);"
        "  color: #fff8e7;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "  letter-spacing: 3px;"
        "  border: 2px solid #c8a84b;"
        "  border-radius: 6px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(220,170,60,240);"
        "  border: 2px solid #fff;"
        "  color:#fff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: rgba(130,90,20,255);"
        "}"
        );

    pl->addWidget(header);

    pl->addWidget(welcome);

    pl->addWidget(missions);

    pl->addWidget(warning);

    pl->addStretch(1);

    pl->addWidget(
        beginBtn,
        0,
        Qt::AlignCenter
        );

    popup->show();

    //================ START GAME =================//

    connect(
        beginBtn,
        &QPushButton::clicked,
        this,
        [=]()
        {
            dimmer->deleteLater();

            startGame();
        }
        );
}

/* ================= GAME SCREEN ================= */

void GameWindow::setupGameScreen()
{
    gameScreen = new QWidget();

    gameScreen->setStyleSheet(
        "background-color: black;"
        );

    QVBoxLayout* mainLayout =
        new QVBoxLayout(gameScreen);

    setupHUD(mainLayout);

    setupLevel2HUD();

    mainLayout->insertWidget(1, level2HUD);

    scene =
        new QGraphicsScene(this);

    view =
        new QGraphicsView(scene);

    view->setBackgroundBrush(
        QBrush(Qt::black)
        );

    view->setHorizontalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff
        );

    view->setVerticalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff
        );

    view->setFocusPolicy(
        Qt::NoFocus
        );

    view->setFixedSize(
        1400,
        700
        );

    view->scale(0.7, 0.7);

    QHBoxLayout* gameLayout =
        new QHBoxLayout();

    setupInventoryUI(gameLayout);

    gameLayout->addWidget(view);

    mainLayout->addLayout(gameLayout);

    setupButtons(mainLayout);

    stack->addWidget(gameScreen);
}
/* ================= HUD ================= */
void GameWindow::setupHUD(QVBoxLayout* mainLayout)
{
    QHBoxLayout* hud =
        new QHBoxLayout();

    clockLabel =
        new QLabel("00:00:00");

    scoreLabel =
        new QLabel("Score: 0");

    livesLabel =
        new QLabel(QStringLiteral("\u2625 5"));

    livesLabel->setStyleSheet(
        "color: #f5d060;"
        "font-size: 15px;"
        "font-weight: bold;"
        );

    statusLabel =
        new QLabel("ALIVE");

    hud->addWidget(clockLabel);

    hud->addWidget(scoreLabel);

    hud->addWidget(livesLabel);

    hud->addWidget(statusLabel);

    mainLayout->addLayout(hud);
}
void GameWindow::setupLevel2HUD()
{
    level2HUD = new QWidget(gameScreen);

    level2HUD->setFixedHeight(80);

    level2HUD->setStyleSheet(
        "background: rgba(8,4,0,240);"
        "border-bottom: 2px solid #8a6020;"
        );

    level2HUD->hide();

    QHBoxLayout* lay =
        new QHBoxLayout(level2HUD);

    lay->setContentsMargins(16,6,16,6);

    lay->setSpacing(20);

    //================ LEFT SIDE =================//

    QVBoxLayout* leftCol =
        new QVBoxLayout();

    QLabel* levelTitle =
        new QLabel("WING II");

    levelTitle->setStyleSheet(
        "color:#c8a84b;"
        "font-size:12px;"
        "font-weight:bold;"
        "letter-spacing:4px;"
        );

    dangerLabel =
        new QLabel("⚠ WRAITH NEARBY");

    dangerLabel->setStyleSheet(
        "color:#ff3030;"
        "font-size:11px;"
        "font-weight:bold;"
        );

    dangerLabel->hide();

    leftCol->addWidget(levelTitle);

    leftCol->addWidget(dangerLabel);

    //================ SCORE =================//

    level2ScoreLabel =
        new QLabel("SCORE 0");

    level2ScoreLabel->setStyleSheet(
        "color:#f5d060;"
        "font-size:20px;"
        "font-weight:bold;"
        );


    //================ ROCK SLOTS =================//

    QString slotStyle =
        "border:2px solid #c8a84b;"
        "border-radius:8px;"
        "background:rgba(30,18,4,200);";

    //================ ROCK 1 =================//

    rock1Slot = new QLabel();

    rock1Slot->setFixedSize(55,55);

    rock1Slot->setAlignment(Qt::AlignCenter);

    rock1Slot->setStyleSheet(slotStyle);

    //================ ROCK 2 =================//

    rock2Slot = new QLabel();

    rock2Slot->setFixedSize(55,55);

    rock2Slot->setAlignment(Qt::AlignCenter);

    rock2Slot->setStyleSheet(slotStyle);

    //================ ROCK 3 =================//

    rock3Slot = new QLabel();

    rock3Slot->setFixedSize(55,55);

    rock3Slot->setAlignment(Qt::AlignCenter);

    rock3Slot->setStyleSheet(slotStyle);

    //================ ROCK 4 =================//

    rock4Slot = new QLabel();

    rock4Slot->setFixedSize(55,55);

    rock4Slot->setAlignment(Qt::AlignCenter);

    rock4Slot->setStyleSheet(slotStyle);

    //================ ROCK 5 =================//

    rock5Slot = new QLabel();

    rock5Slot->setFixedSize(55,55);

    rock5Slot->setAlignment(Qt::AlignCenter);

    rock5Slot->setStyleSheet(slotStyle);


    //================ ROCK ROW =================//

    QHBoxLayout* rockRow =
        new QHBoxLayout();

    rockRow->addWidget(rock1Slot);

    rockRow->addWidget(rock2Slot);

    rockRow->addWidget(rock3Slot);

    rockRow->addWidget(rock4Slot);

    rockRow->addWidget(rock5Slot);


    //================ TIMER =================//

    level2TimerLabel =
        new QLabel("02:00");

    level2TimerLabel->setStyleSheet(
        "color:#f5d060;"
        "font-size:28px;"
        "font-weight:bold;"
        "font-family:monospace;"
        );

    //================ ADD TO LAYOUT =================//

    lay->addLayout(leftCol);

    lay->addStretch();

    lay->addWidget(level2ScoreLabel);

    lay->addStretch();

    lay->addLayout(rockRow);

    lay->addStretch();

    lay->addWidget(level2TimerLabel);
}
/* ================= INVENTORY ================= */

void GameWindow::setupInventoryUI(
    QHBoxLayout* gameLayout
    )
{
    QVBoxLayout* inventoryLayout =
        new QVBoxLayout();

    inventoryLayout->setSpacing(20);

    inventoryLayout->setAlignment(
        Qt::AlignTop
        );

    //================ ICONS =================//

    coinIcon =
        new QLabel();

    scrollIcon =
        new QLabel();

    maskIcon =
        new QLabel();

    amuletIcon =
        new QLabel();

    timerIcon =
        new QLabel();

    //================ COUNTERS =================//

    coinCounter =
        new QLabel();

    scrollCounter =
        new QLabel();

    maskCounter =
        new QLabel();

    amuletCounter =
        new QLabel();

    timerCounter =
        new QLabel();

    //================ PIXMAPS =================//

    coinIcon->setPixmap(
        QPixmap(":/new/prefix1/images/coint.png")
            .scaled(40, 40)
        );

    scrollIcon->setPixmap(
        QPixmap(":/new/prefix1/images/scrollt.png")
            .scaled(40, 40)
        );

    maskIcon->setPixmap(
        QPixmap(":/new/prefix1/images/maskt.png")
            .scaled(40, 40)
        );

    amuletIcon->setPixmap(
        QPixmap(":/new/prefix1/images/amulett.png")
            .scaled(40, 40)
        );

    timerIcon->setPixmap(
        QPixmap(":/new/prefix1/images/timert.png")
            .scaled(40, 40)
        );

    updateInventoryUI();

    //================ ROWS =================//

    QHBoxLayout* coinRow =
        new QHBoxLayout();

    QHBoxLayout* scrollRow =
        new QHBoxLayout();

    QHBoxLayout* maskRow =
        new QHBoxLayout();

    QHBoxLayout* amuletRow =
        new QHBoxLayout();

    QHBoxLayout* timerRow =
        new QHBoxLayout();

    coinRow->addWidget(coinIcon);
    coinRow->addWidget(coinCounter);

    scrollRow->addWidget(scrollIcon);
    scrollRow->addWidget(scrollCounter);

    maskRow->addWidget(maskIcon);
    maskRow->addWidget(maskCounter);

    amuletRow->addWidget(amuletIcon);
    amuletRow->addWidget(amuletCounter);

    timerRow->addWidget(timerIcon);
    timerRow->addWidget(timerCounter);

    //================ ADD TO INVENTORY =================//

    inventoryLayout->addLayout(coinRow);

    inventoryLayout->addLayout(scrollRow);

    inventoryLayout->addLayout(maskRow);

    inventoryLayout->addLayout(amuletRow);

    inventoryLayout->addLayout(timerRow);

    gameLayout->addLayout(inventoryLayout);
}
/* ================= BUTTONS ================= */

void GameWindow::setupButtons(QVBoxLayout* mainLayout)
{
    QHBoxLayout* buttons =
        new QHBoxLayout();

    QPushButton* pauseBtn =
        new QPushButton("PAUSE");

    QPushButton* restartBtn =
        new QPushButton("RESTART");

    QPushButton* exitBtn =
        new QPushButton("EXIT");

    QPushButton* saveBtn =
        new QPushButton("SAVE");

    QPushButton* loadBtn =
        new QPushButton("LOAD");

    //================ CONNECTS ================//

    connect(
        pauseBtn,
        &QPushButton::clicked,
        this,
        &GameWindow::pauseGame
        );

    connect(
        restartBtn,
        &QPushButton::clicked,
        this,
        &GameWindow::restartGame
        );

    connect(
        exitBtn,
        &QPushButton::clicked,
        this,
        &GameWindow::exitGame
        );

    connect(
        saveBtn,
        &QPushButton::clicked,
        this,
        &GameWindow::saveGame
        );

    connect(
        loadBtn,
        &QPushButton::clicked,
        this,
        &GameWindow::loadGame
        );

    //================ ADD TO LAYOUT ================//

    buttons->addWidget(pauseBtn);

    buttons->addWidget(restartBtn);

    buttons->addWidget(exitBtn);

    buttons->addWidget(saveBtn);

    buttons->addWidget(loadBtn);

    mainLayout->addLayout(buttons);
}
/* ================= START GAME ================= */

void GameWindow::startGame()
{
    game.startGame();

    seconds = 300;

    if (mummy) {
        disconnect(mummy, nullptr, this, nullptr);
        mummy->setPaused(true);
        mummy->discardActiveHomingProjectiles();
    }
    if (ghost) {
        disconnect(ghost, nullptr, this, nullptr);
        ghost->setPaused(true);
    }

    scene->clear();

    mummy = nullptr;
    ghost = nullptr;
    playerSprite = nullptr;

    timer->start(1000);

    //================ LOAD DIRECTIONAL SPRITES =================//

    spriteFront =
        QPixmap(":/new/prefix1/images/player front.png");

    spriteBack =
        QPixmap(":/new/prefix1/images/player back.png");

    spriteLeft =
        QPixmap(":/new/prefix1/images/player left.png");

    spriteRight =
        QPixmap(":/new/prefix1/images/player right.png");

    //================ LOAD LEVEL =================//

    currentLevel =
        game.getCurrentLevel();

    currentLevel->loadScene(scene);

    //================ PLAYER SPRITE =================//

    playerSprite =
        scene->addPixmap(spriteFront);

    playerSprite->setScale(0.12);

    playerSprite->setPos(
        game.getPlayer().getX(),
        game.getPlayer().getY()
        );

    //================ ENEMY =================//

    mummy =
        new Level1Enemy(
            &game.getPlayer(),
            playerSprite
            );

    mummy->setPos(800, 500);

    mummy->setZValue(999);

    scene->addItem(mummy);

    connect(
        mummy,
        &Level1Enemy::reduceScore,
        this,
        [=]()
        {
            game.getPlayer().deductScore(10);

            updateHUD();
        }
        );

    connect(
        mummy,
        &Level1Enemy::playerKilled,
        this,
        &GameWindow::showFireballGameOver
        );

    connect(
        mummy,
        &Level1Enemy::fireballHitRegistered,
        this,
        [this]()
        {
            updateHUD();
        }
        );

    //================ UPDATE UI =================//

    updateInventoryUI();

    updateHUD();

    if (level2HUD) {
        level2HUD->hide();
    }

    //================ SHOW SCREEN =================//

    stack->setCurrentWidget(gameScreen);

    this->setFocus();
}
//================ ENEMY LEVEL 2 =================//

void GameWindow::handleGhostStrike()
{
    playerSpeedStep = playerSpeedStep - 1;
    if (playerSpeedStep < 1) {
        playerSpeedStep = 1;
    }

    QTimer::singleShot(10000, this, [=]() {
        playerSpeedStep = playerSpeedStep + 1;

        if (playerSpeedStep > 3) {
            playerSpeedStep = 3;
        }
    });

    updateHUD();
}
/* ================= GHOST FLASH EFFECT ================= */

/*void GameWindow::showBlankScreen()
{

    static int screechFlashCount = 0;
    screechFlashCount++;

    bool finalHit = (screechFlashCount >= 3);


    timer->stop();
    game.pauseGame();

    if (ghost) ghost->setPaused(true);

    if (horrorMusic) horrorMusic->setVolume(0.0);


    QWidget* flash = new QWidget(this);
    flash->setGeometry(0, 0, width(), height());
    flash->show();
    flash->raise();


    int whiteMs;
    int blackMs;
    int white2Ms;
    int redMs;

    if (finalHit == true) {
        whiteMs = 250;
        blackMs = 200;
        white2Ms = 250;
        redMs = 700;
    } else {
        whiteMs = 120;
        blackMs = 100;
        white2Ms = 120;
        redMs = 250;
    }

    flash->setStyleSheet("background-color: white;");

    QTimer::singleShot(whiteMs, this, [=]() {
        flash->setStyleSheet("background-color: black;");

        QTimer::singleShot(blackMs, this, [=]() {
            flash->setStyleSheet("background-color: white;");

            QTimer::singleShot(white2Ms, this, [=]() {
                flash->setStyleSheet("background-color: #8a2020;");

                QTimer::singleShot(redMs, this, [=]() {
                    flash->deleteLater();



                    if (ghost) {
                        const double px = game.getPlayer().getX();
                        const double py = game.getPlayer().getY();


                        double tx = px + 400;
                        double ty = py + 400;

                        ghost->setPos(tx, ty);
                    }

                    if(finalHit)
                    {
                        flash->deleteLater();

                        showGhostGameOver();

                        return;
                    }
                    else
                    {
                        game.resumeGame();

                        if(ghost)
                        {
                            ghost->setPaused(false);
                        }

                        timer->start(1000);
                    }
                });
            });
        });
    });
}

*/

/* ================= MOVEMENT ================= */
// Swaps directional sprite before moving

void GameWindow::movePlayer(
    int dx,
    int dy,
    const QPixmap& sprite
    )
{
    if (!playerSprite || !currentLevel) {
        return;
    }

    //================ CHANGE SPRITE =================//

    playerSprite->setPixmap(sprite);

    QPointF newPos =
        playerSprite->pos();

    newPos.setX(
        newPos.x() + dx
        );

    newPos.setY(
        newPos.y() + dy
        );

    if(currentLevel->isWalkable(
            newPos,
            playerSprite->sceneBoundingRect(),
            scene->sceneRect()))
    {
        playerSprite->setPos(newPos);

        game.getPlayer().moveTo(
            newPos.x(),
            newPos.y()
            );

        checkArtifactCollisions();

        //================ LEVEL 3 TREASURE =================//

        Level3* level3 =
            dynamic_cast<Level3*>(currentLevel);

        if(level3)
        {
            level3->checkTreasureCollision(
                playerSprite
                );
        }
    }
}

/* ================= COLLISIONS ================= */

void GameWindow::checkArtifactCollisions()
{
    QGraphicsItem* artifact =
        currentLevel->checkArtifactCollision(
            playerSprite
            );

    if(artifact)
    {
        QString type =
            artifact->data(0).toString();

        currentLevel->removeArtifact(
            artifact,
            scene
            );

        game.collectArtifact(type);

        if(type.contains("rock"))
        {
            rocksCollected++;

            updateLevel2HUD();
        }

        //================ TIMER BONUS =================//

        if(type == "timer")
        {
            seconds += 30;
        }

        updateInventoryUI();

        updateHUD();

        //================ LEVEL COMPLETE =================//

        if(currentLevel->getArtifactCount() == 0)
        {
            timer->stop();

            //================ STOP ENEMY =================//

            if(mummy)
            {
                mummy->setPaused(true);
            }

            //================ LEVEL 1 =================//

            if(game.getLevelIndex() == 1)
            {
                showLevel2BriefingPopup();
            }

            //================ LEVEL 2 =================//

            else if(rocksCollected == 5)
            {
                showHieroglyphScreen();
            }

            return;
        }
    }
}
/* ================= UPDATE HUD ================= */

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

    scoreLabel->setText(
        "Score: "
        + QString::number(
            game.getPlayer().getScore()
            )
        );

    if (livesLabel) {
        const int idx = game.getLevelIndex();
        if (idx == 3) {
            livesLabel->hide();
        } else {
            livesLabel->show();
            int lives = 0;
            if (idx == 1) {
                lives = qMax(0, 5 - game.getPlayer().getFireballHits());
            } else if (idx == 2) {
                const int strikes = ghost ? ghost->getStrikeCount() : 0;
                lives = qMax(0, 3 - strikes);
            }
            livesLabel->setText(
                QStringLiteral("\u2625 %1").arg(lives)
                );
        }
    }
}

/* ================= UPDATE INVENTORY ================= */

void GameWindow::updateInventoryUI()
{
    //================ ENSURE ICONS VISIBLE (reset from L3 hide) =================//

    coinIcon->show();
    scrollIcon->show();
    maskIcon->show();
    amuletIcon->show();
    timerIcon->show();
    coinCounter->show();
    scrollCounter->show();
    maskCounter->show();
    amuletCounter->show();
    timerCounter->show();

    //================ LEVEL 3 — HIDE INVENTORY =================//

    if(game.getLevelIndex() == 3)
    {
        coinIcon->hide();
        scrollIcon->hide();
        maskIcon->hide();
        amuletIcon->hide();
        timerIcon->hide();
        coinCounter->hide();
        scrollCounter->hide();
        maskCounter->hide();
        amuletCounter->hide();
        timerCounter->hide();
        return;
    }

    //================ LEVEL 2 =================//

    if(game.getLevelIndex() == 2)
    {
        //================ ICONS =================//

        coinIcon->setPixmap(
            QPixmap(":/new/prefix1/images/rock_M.png")
                .scaled(40,40)
            );

        scrollIcon->clear();

        maskIcon->clear();

        amuletIcon->clear();

        timerIcon->clear();

        //================ COUNTERS =================//

        coinCounter->setText(
            "x" + QString::number(
                rocksCollected
                )
            );

        scrollCounter->setText("");

        maskCounter->setText("");

        amuletCounter->setText("");

        timerCounter->setText("");

        return;
    }

    //================ LEVEL 1 ICONS =================//

    coinIcon->setPixmap(
        QPixmap(":/new/prefix1/images/coint.png")
            .scaled(40,40)
        );

    scrollIcon->setPixmap(
        QPixmap(":/new/prefix1/images/scrollt.png")
            .scaled(40,40)
        );

    maskIcon->setPixmap(
        QPixmap(":/new/prefix1/images/maskt.png")
            .scaled(40,40)
        );

    amuletIcon->setPixmap(
        QPixmap(":/new/prefix1/images/amulett.png")
            .scaled(40,40)
        );

    timerIcon->setPixmap(
        QPixmap(":/new/prefix1/images/timert.png")
            .scaled(40,40)
        );

    //================ COUNTERS =================//

    coinCounter->setText(
        "x" + QString::number(
            game.getArtifactCount("coin")
            )
        );

    scrollCounter->setText(
        "x" + QString::number(
            game.getArtifactCount("scroll")
            )
        );

    maskCounter->setText(
        "x" + QString::number(
            game.getArtifactCount("mask")
            )
        );

    amuletCounter->setText(
        "x" + QString::number(
            game.getArtifactCount("amulet")
            )
        );

    timerCounter->setText(
        "x" + QString::number(
            game.getArtifactCount("timer")
            )
        );
}
/* ================= LEVEL 2 HUD UPDATE ================= */

void GameWindow::updateLevel2HUD()
{
    if(!level2HUD)
    {
        return;
    }

    //================ TIMER =================//

    int m = seconds / 60;

    int s = seconds % 60;

    level2TimerLabel->setText(
        QString("%1:%2")
            .arg(m,2,10,QChar('0'))
            .arg(s,2,10,QChar('0'))
        );

    //================ SCORE =================//

    level2ScoreLabel->setText(
        "SCORE "
        + QString::number(
            game.getPlayer().getScore()
            )
        );

    //================ WARNING =================//

    if(seconds <= 60)
    {
        dangerLabel->show();
    }
    else
    {
        dangerLabel->hide();
    }

    //================ ROCK 1 =================//

    if(rocksCollected >= 1)
    {
        rock1Slot->setPixmap(
            QPixmap(":/new/prefix1/images/rock_M.png")
                .scaled(
                    28,
                    28,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                    )
            );
    }

    //================ ROCK 2 =================//

    if(rocksCollected >= 2)
    {
        rock2Slot->setPixmap(
            QPixmap(":/new/prefix1/images/rock_A-3.png")
                .scaled(
                    28,
                    28,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                    )
            );
    }

    //================ ROCK 3 =================//

    if(rocksCollected >= 3)
    {
        rock3Slot->setPixmap(
            QPixmap(":/new/prefix1/images/rock_N.png")
                .scaled(
                    28,
                    28,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                    )
            );
    }

    //================ ROCK 4 =================//

    if(rocksCollected >= 4)
    {
        rock4Slot->setPixmap(
            QPixmap(":/new/prefix1/images/rock_N.png")
                .scaled(
                    28,
                    28,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                    )
            );
    }

    //================ ROCK 5 =================//

    if(rocksCollected >= 5)
    {
        rock5Slot->setPixmap(
            QPixmap(":/new/prefix1/images/rock_M.png")
                .scaled(
                    28,
                    28,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                    )
            );
    }
}

/* ================= HIEROGLYPH SCREEN ================= */

void GameWindow::showHieroglyphScreen()
{
    timer->stop();

    if (level2HUD) {
        level2HUD->hide();
    }

    QWidget* dimmer =
        new QWidget(this);

    dimmer->setGeometry(
        0,
        0,
        width(),
        height()
        );

    dimmer->setStyleSheet(
        "background-color:rgba(0,0,0,220);"
        );

    dimmer->show();

    dimmer->raise();

    //================ SCREEN =================//

    QWidget* popup =
        new QWidget(dimmer);

    popup->setFixedSize(900,650);

    popup->move(
        (width()-900)/2,
        (height()-650)/2
        );

    popup->setStyleSheet(
        "background-color:rgba(8,4,0,240);"
        "border:2px solid rgba(200,160,60,220);"
        "border-radius:14px;"
        );

    //================ LAYOUT =================//

    QVBoxLayout* layout =
        new QVBoxLayout(popup);

    layout->setContentsMargins(
        30,
        20,
        30,
        20
        );

    layout->setSpacing(16);

    //================ TITLE =================//

    QLabel* title =
        new QLabel(
            "THE SACRED STONES REVEAL A WORD"
            );

    title->setAlignment(Qt::AlignCenter);

    title->setStyleSheet(
        "font-size:22px;"
        "font-weight:bold;"
        "color:#f5d060;"
        "letter-spacing:4px;"
        );

    //================ IMAGE =================//

    QLabel* image =
        new QLabel();

    image->setPixmap(
        QPixmap(":/new/prefix1/images/hieroglyph_chart-2.png")
            .scaled(
                760,
                430,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
                )
        );

    image->setAlignment(Qt::AlignCenter);

    //================ PASSWORD =================//

    QLabel* password =
        new QLabel(
            "The symbols translate to:\n\n"
            "M   •   A   •   N\n\n"
            "Memorize the ancient password."
            );

    password->setAlignment(Qt::AlignCenter);

    password->setStyleSheet(
        "font-size:18px;"
        "font-weight:bold;"
        "color:#f5d060;"
        "line-height:1.8;"
        );

    //================ BUTTON =================//

    QPushButton* continueBtn =
        new QPushButton(
            "▶ CONTINUE"
            );

    continueBtn->setFixedHeight(50);

    continueBtn->setStyleSheet(
        "QPushButton {"
        "background-color:rgba(180,130,40,220);"
        "color:white;"
        "font-size:15px;"
        "font-weight:bold;"
        "border-radius:8px;"
        "border:2px solid #c8a84b;"
        "padding:10px;"
        "}"

        "QPushButton:hover {"
        "background-color:rgba(220,170,60,240);"
        "}"
        );

    //================ ADD =================//

    layout->addWidget(title);

    layout->addWidget(image);

    layout->addWidget(password);

    layout->addStretch();

    layout->addWidget(
        continueBtn,
        0,
        Qt::AlignCenter
        );

    popup->show();

    //================ CONTINUE =================//

    connect(
        continueBtn,
        &QPushButton::clicked,
        this,
        [=]()
        {
            //================ CLOSE POPUP =================//

            dimmer->deleteLater();

            //================ LOAD LEVEL 3 =================//

            game.loadLevel(3);

            currentLevel = game.getCurrentLevel();

            if (mummy) {
                disconnect(mummy, nullptr, this, nullptr);
                mummy->setPaused(true);
                mummy->discardActiveHomingProjectiles();
            }
            if (ghost) {
                disconnect(ghost, nullptr, this, nullptr);
                ghost->setPaused(true);
            }

            //================ CLEAR OLD SCENE =================//

            scene->clear();

            mummy = nullptr;
            ghost = nullptr;
            playerSprite = nullptr;

            //================ LOAD NEW LEVEL =================//

            currentLevel->loadScene(scene);

            //================ LEVEL 3 COLLISION =================//

            collisionMask = QImage( ":/new/prefix1/images/level3 BW.png");

            //================ SET RESTART CALLBACK =================//

            Level3* lvl3 =
                dynamic_cast<Level3*>(currentLevel);

            if(lvl3)
            {
                lvl3->setRestartCallback(
                    [=]()
                    {
                        // Always restart from Level 1
                        rocksCollected = 0;
                        game.loadLevel(1);
                        restartGame();
                    }
                    );
            }

            //================ CREATE PLAYER =================//

            playerSprite =
                scene->addPixmap(spriteFront);

            playerSprite->setScale(0.12);

            // Start player at middle-bottom of the Level 3 scene
            playerSprite->setPos(605, 816);

            game.getPlayer().moveTo(
                605,
                810
                );


            //================ TIMER =================//

            seconds = 300;

            timer->start(1000);

            //================ UPDATE UI =================//

            updateHUD();

            updateInventoryUI();

            //================ SHOW GAME SCREEN =================//

            stack->setCurrentWidget(
                gameScreen
                );

            this->setFocus();
        }
        );
}
/* ================= UPDATE GAME ================= */

void GameWindow::updateGame()
{
    // Only run game logic while the actual game screen is visible.
    if (stack && stack->currentWidget() != gameScreen)
        return;

    if (!currentLevel || !playerSprite)
        return;

    seconds--;
    updateHUD();
    //================ LEVEL 2 HUD =================//
    updateLevel2HUD();
    //================ GAME LOGIC =================//
    game.update(1.0f);


    //================ LEVEL 2 LIGHT =================//

    Level2* level2 =
        dynamic_cast<Level2*>(currentLevel);

    if(level2)
    {
        level2->updatePlayerGlow(playerSprite);
    }
    if(seconds <= 0)
    {
        timer->stop();

        if(game.getLevelIndex() == 1)
        {
            showFireballGameOver();
        }
        else if(game.getLevelIndex() == 2)
        {
            showGhostGameOver();
        }

        return;
    }

}

/* ================= KEY PRESS ================= */
// Each arrow key uses directional sprites

void GameWindow::keyPressEvent(
    QKeyEvent *event
    )
{
    if(stack->currentWidget() != gameScreen)
        return;

    if (event->key() == Qt::Key_Escape) {
        pauseGame();
        return;
    }

    if (!playerSprite || !currentLevel) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    const int step = playerSpeedStep;

    switch(event->key())
    {
    case Qt::Key_Up:

        movePlayer(
            0,
            -step,
            spriteBack
            );

        break;

    case Qt::Key_Down:

        movePlayer(
            0,
            +step,
            spriteFront
            );

        break;

    case Qt::Key_Left:

        movePlayer(
            -step,
            0,
            spriteLeft
            );

        break;

    case Qt::Key_Right:

        movePlayer(
            +step,
            0,
            spriteRight
            );

        break;

    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

/* ================= FIREBALL GAME OVER ================= */

void GameWindow::showFireballGameOver()
{
    // Stop the game timer and pause everything
    timer->stop();
    game.pauseGame();
    if (mummy) {
        mummy->discardActiveHomingProjectiles();
        mummy->setPaused(true);
    }

    //================ FULL-SCREEN DARK OVERLAY =================//

    QWidget* dimmer = new QWidget(this);
    dimmer->setGeometry(0, 0, width(), height());
    dimmer->setStyleSheet("background-color: rgba(0, 0, 0, 210);");
    dimmer->show();
    dimmer->raise();

    //================ POPUP CARD =================//

    QWidget* popup = new QWidget(dimmer);
    popup->setFixedSize(600, 420);
    popup->move(
        (dimmer->width()  - 600) / 2,
        (dimmer->height() - 420) / 2
        );
    popup->setStyleSheet(
        "background-color: rgba(60, 5, 0, 240);"
        "border: 3px solid rgba(220, 60, 20, 200);"
        "border-radius: 16px;"
        );

    QVBoxLayout* layout = new QVBoxLayout(popup);
    layout->setContentsMargins(50, 40, 50, 40);
    layout->setSpacing(18);

    //================ FIRE EMOJI ROW =================//

    QLabel* flames = new QLabel("🔥  🔥  🔥");
    flames->setAlignment(Qt::AlignCenter);
    flames->setStyleSheet(
        "font-size: 36px;"
        "background: transparent;"
        "border: none;"
        );

    //================ TITLE =================//

    QLabel* title = new QLabel("GAME OVER");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 52px;"
        "font-weight: bold;"
        "letter-spacing: 10px;"
        "color: #ff3300;"
        "background: transparent;"
        "border: none;"
        );

    //================ SUBTITLE =================//

    QLabel* subtitle = new QLabel("You were consumed by the RA's flames!");
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setWordWrap(true);
    subtitle->setStyleSheet(
        "font-size: 15px;"
        "color: #f5c89a;"
        "background: transparent;"
        "border: none;"
        "letter-spacing: 2px;"
        );

    //================ HIT COUNTER =================//

    QLabel* hitInfo = new QLabel("The fireball struck you 5 times.");
    hitInfo->setAlignment(Qt::AlignCenter);
    hitInfo->setStyleSheet(
        "font-size: 13px;"
        "color: rgba(255,120,60,200);"
        "font-style: italic;"
        "background: transparent;"
        "border: none;"
        );

    //================ BUTTON STYLE =================//

    QString btnStyle =
        "QPushButton {"
        "  background-color: rgba(180,130,40,220);"
        "  color: #fff8e7;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "  letter-spacing: 3px;"
        "  border: 2px solid #c8a84b;"
        "  border-radius: 8px;"
        "  padding: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(220,170,60,240);"
        "  border: 2px solid #ffffff;"
        "  color: #ffffff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: rgba(130,90,20,255);"
        "}";

    //================ RESTART BUTTON =================//

    QPushButton* restartBtn = new QPushButton("↺   TRY AGAIN");
    restartBtn->setFixedHeight(52);
    restartBtn->setCursor(Qt::PointingHandCursor);
    restartBtn->setStyleSheet(btnStyle);

    //================ EXIT BUTTON =================//

    QPushButton* exitBtn = new QPushButton("✕   EXIT GAME");
    exitBtn->setFixedHeight(52);
    exitBtn->setCursor(Qt::PointingHandCursor);
    exitBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(100,30,20,220);"
        "  color: #fff8e7;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "  letter-spacing: 3px;"
        "  border: 2px solid #8a2020;"
        "  border-radius: 8px;"
        "  padding: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(160,40,30,240);"
        "}"
        );

    //================ ASSEMBLE =================//

    layout->addWidget(flames);
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addWidget(hitInfo);
    layout->addStretch();
    layout->addWidget(restartBtn);
    layout->addWidget(exitBtn);

    popup->show();

    //================ RESTART =================//

    connect(restartBtn, &QPushButton::clicked, this, [=]() {
        dimmer->deleteLater();
        restartGame();
    });

    //================ EXIT =================//

    connect(exitBtn, &QPushButton::clicked, this, [=]() {
        dimmer->deleteLater();
        exitGame();
    });
}

void GameWindow::showGhostGameOver()
{
    //================ STOP GAME =================//

    timer->stop();

    game.pauseGame();

    if(ghost)
    {
        ghost->setPaused(true);
    }

    //================ FULL SCREEN DIM =================//

    QWidget* dimmer =
        new QWidget(this);

    dimmer->setGeometry(
        0,
        0,
        width(),
        height()
        );

    dimmer->setStyleSheet(
        "background-color: rgba(0, 0, 0, 210);"
        );

    dimmer->show();

    dimmer->raise();

    //================ POPUP CARD =================//

    QWidget* popup =
        new QWidget(dimmer);

    popup->setFixedSize(600,420);

    popup->move(
        (dimmer->width()  - 600) / 2,
        (dimmer->height() - 420) / 2
        );

    popup->setStyleSheet(
        "background-color: rgba(10, 10, 25, 240);"
        "border: 3px solid rgba(120, 180, 255, 200);"
        "border-radius: 16px;"
        );

    //================ LAYOUT =================//

    QVBoxLayout* layout =
        new QVBoxLayout(popup);

    layout->setContentsMargins(
        50,
        40,
        50,
        40
        );

    layout->setSpacing(18);

    //================ SOUL ICONS =================//

    QLabel* souls =
        new QLabel("👻  👻  👻");

    souls->setAlignment(Qt::AlignCenter);

    souls->setStyleSheet(
        "font-size: 36px;"
        "background: transparent;"
        "border: none;"
        );

    //================ TITLE =================//

    QLabel* title =
        new QLabel("GAME OVER");

    title->setAlignment(Qt::AlignCenter);

    title->setStyleSheet(
        "font-size: 52px;"
        "font-weight: bold;"
        "letter-spacing: 10px;"
        "color: #9fd0ff;"
        "background: transparent;"
        "border: none;"
        );

    //================ SUBTITLE =================//

    QLabel* subtitle =
        new QLabel(
            "You were hit 3 times by the soul of Tutankhamun!"
            );

    subtitle->setAlignment(Qt::AlignCenter);

    subtitle->setWordWrap(true);

    subtitle->setStyleSheet(
        "font-size: 15px;"
        "color: #d8ebff;"
        "background: transparent;"
        "border: none;"
        "letter-spacing: 2px;"
        );

    //================ EXTRA TEXT =================//

    QLabel* hitInfo =
        new QLabel(
            "The spirit reclaimed the sacred chamber."
            );

    hitInfo->setAlignment(Qt::AlignCenter);

    hitInfo->setStyleSheet(
        "font-size: 13px;"
        "color: rgba(180,220,255,200);"
        "font-style: italic;"
        "background: transparent;"
        "border: none;"
        );

    //================ BUTTON STYLE =================//

    QString btnStyle =
        "QPushButton {"
        "  background-color: rgba(70,110,180,220);"
        "  color: #fff8e7;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "  letter-spacing: 3px;"
        "  border: 2px solid #9fd0ff;"
        "  border-radius: 8px;"
        "  padding: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(110,160,240,240);"
        "  border: 2px solid #ffffff;"
        "  color: #ffffff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: rgba(40,70,130,255);"
        "}";

    //================ TRY AGAIN =================//

    QPushButton* restartBtn =
        new QPushButton(
            "↺   TRY AGAIN"
            );

    restartBtn->setFixedHeight(52);

    restartBtn->setCursor(
        Qt::PointingHandCursor
        );

    restartBtn->setStyleSheet(btnStyle);

    //================ EXIT =================//

    QPushButton* exitBtn =
        new QPushButton(
            "✕   EXIT GAME"
            );

    exitBtn->setFixedHeight(52);

    exitBtn->setCursor(
        Qt::PointingHandCursor
        );

    exitBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(40,50,90,220);"
        "  color: #fff8e7;"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "  letter-spacing: 3px;"
        "  border: 2px solid #4d6fb3;"
        "  border-radius: 8px;"
        "  padding: 14px;"
        "}"
        "QPushButton:hover {"
        "  background-color: rgba(70,90,150,240);"
        "}"
        );

    //================ ADD =================//

    layout->addWidget(souls);

    layout->addWidget(title);

    layout->addWidget(subtitle);

    layout->addWidget(hitInfo);

    layout->addStretch();

    layout->addWidget(restartBtn);

    layout->addWidget(exitBtn);

    popup->show();

    //================ TRY AGAIN =================//

    connect(
        restartBtn,
        &QPushButton::clicked,
        this,
        [=]()
        {
            dimmer->deleteLater();

            rocksCollected = 0;

            restartGame();
        }
        );

    //================ EXIT =================//

    connect(
        exitBtn,
        &QPushButton::clicked,
        this,
        [=]()
        {
            dimmer->deleteLater();

            exitGame();
        }
        );
}

/* ================= PAUSE GAME ================= */

void GameWindow::pauseGame()
{
    game.pauseGame();
    if(mummy)
    {
        mummy->setPaused(true);
    }

    timer->stop();

    //================ DIM BACKGROUND =================//

    QWidget *dimmer =
        new QWidget(this);

    dimmer->setGeometry(
        0,
        0,
        width(),
        height()
        );

    dimmer->setStyleSheet(
        "background-color: rgba(0,0,0,170);"
        );

    dimmer->show();

    dimmer->raise();

    //================ POPUP CARD =================//

    QWidget *popup =
        new QWidget(dimmer);

    popup->setFixedSize(
        520,
        360
        );

    popup->move(
        (width()  - popup->width())  / 2,
        (height() - popup->height()) / 2
        );

    popup->setStyleSheet(
        "background-color: rgba(8,4,0,235);"
        "border: 2px solid rgba(200,160,60,200);"
        "border-radius: 14px;"
        );

    //================ LAYOUT =================//

    QVBoxLayout *layout =
        new QVBoxLayout(popup);

    layout->setContentsMargins(
        40,
        35,
        40,
        35
        );

    layout->setSpacing(20);

    //================ TITLE =================//

    QLabel *title =
        new QLabel("GAME PAUSED");

    title->setAlignment(
        Qt::AlignCenter
        );

    title->setStyleSheet(
        "font-size: 28px;"
        "font-weight: bold;"
        "letter-spacing: 6px;"
        "color: #f5d060;"
        "background: transparent;"
        "border: none;"
        );

    //================ MESSAGE =================//

    QLabel *message =
        new QLabel(
            "The museum remains in silence...\n"
            "Press continue when you're ready."
            );

    message->setAlignment(
        Qt::AlignCenter
        );

    message->setWordWrap(true);

    message->setStyleSheet(
        "font-size: 15px;"
        "color: #e8d5a8;"
        "background: transparent;"
        "border: none;"
        "line-height: 1.5;"
        );

    //================ BUTTON STYLE =================//

    QString buttonStyle =
        "QPushButton {"
        " background-color: rgba(180,130,40,220);"
        " color: #fff8e7;"
        " font-size: 15px;"
        " font-weight: bold;"
        " letter-spacing: 3px;"
        " border: 2px solid #c8a84b;"
        " border-radius: 8px;"
        " padding: 14px;"
        "}"
        "QPushButton:hover {"
        " background-color: rgba(220,170,60,240);"
        " border: 2px solid #ffffff;"
        " color: #ffffff;"
        "}"
        "QPushButton:pressed {"
        " background-color: rgba(130,90,20,255);"
        "}";

    //================ CONTINUE BUTTON =================//

    QPushButton *continueBtn =
        new QPushButton(
            "▶   CONTINUE"
            );

    continueBtn->setCursor(
        Qt::PointingHandCursor
        );

    continueBtn->setFixedHeight(55);

    continueBtn->setStyleSheet(
        buttonStyle
        );

    //================ EXIT BUTTON =================//

    QPushButton *exitBtn =
        new QPushButton(
            "✕   EXIT GAME"
            );

    exitBtn->setCursor(
        Qt::PointingHandCursor
        );

    exitBtn->setFixedHeight(55);

    exitBtn->setStyleSheet(
        "QPushButton {"
        " background-color: rgba(100,30,20,220);"
        " color: #fff8e7;"
        " font-size: 15px;"
        " font-weight: bold;"
        " letter-spacing: 3px;"
        " border: 2px solid #8a2020;"
        " border-radius: 8px;"
        " padding: 14px;"
        "}"
        "QPushButton:hover {"
        " background-color: rgba(160,40,30,240);"
        "}");

    //================ ADD WIDGETS =================//

    layout->addWidget(title);

    layout->addWidget(message);

    layout->addStretch();

    layout->addWidget(continueBtn);

    layout->addWidget(exitBtn);

    popup->show();

    //================ CONTINUE =================//

    connect(
        continueBtn,
        &QPushButton::clicked,
        this,
        [=]()
        {
            dimmer->deleteLater();

            game.resumeGame();

            if(mummy)
            {
                mummy->setPaused(false);
            }

            if(ghost)
            {
                ghost->setPaused(false);
            }

            timer->start(1000);

            stack->setCurrentWidget(
                gameScreen
                );

            this->setFocus();
        }
        );
    //================ EXIT =================//

    connect(
        exitBtn,
        &QPushButton::clicked,
        this,
        [=]()
        {
            dimmer->deleteLater();

            exitGame();
        }
        );
}

void GameWindow::showLevel2BriefingPopup()
{
    //================ STOP GAME ONLY =================//

    timer->stop();

    if(mummy)
    {
        mummy->setPaused(true);
    }

    //================ DIMMER =================//

    QWidget* dimmer =
        new QWidget(this);

    dimmer->setGeometry(
        0,
        0,
        width(),
        height()
        );

    dimmer->setStyleSheet(
        "background-color:rgba(0,0,0,200);"
        );

    dimmer->show();

    dimmer->raise();

    //================ POPUP =================//

    QWidget* popup =
        new QWidget(dimmer);

    popup->setFixedSize(760,620);

    popup->move(
        (dimmer->width()-760)/2,
        (dimmer->height()-620)/2
        );

    popup->setStyleSheet(
        "background-color:rgba(5,3,0,240);"
        "border:2px solid rgba(200,160,60,220);"
        "border-radius:14px;"
        );

    //================ LAYOUT =================//

    QVBoxLayout* pl =
        new QVBoxLayout(popup);

    pl->setContentsMargins(
        44,
        36,
        44,
        32
        );

    pl->setSpacing(18);

    //================ TITLE =================//

    QLabel* title =
        new QLabel(
            "WING II — THE HIEROGLYPH VAULT"
            );

    title->setAlignment(
        Qt::AlignCenter
        );

    title->setStyleSheet(
        "font-size:24px;"
        "font-weight:bold;"
        "color:#f5d060;"
        "letter-spacing:4px;"
        "background:transparent;"
        "border:none;"
        );

    //================ STORY =================//

    QString storyText =
        QString(
            "You secured the first hall — well done, <b>%1</b>.<br><br>"

            "You now enter the sacred Room of Tutankhamun.<br><br>"

            "But the east wing has gone dark. "
            "Something ancient has awakened in the deepest vault "
            "of the museum... and it is not alone."
            ).arg(playerName);

    QLabel* story =
        new QLabel(storyText);

    story->setWordWrap(true);

    story->setAlignment(
        Qt::AlignCenter
        );

    story->setStyleSheet(
        "font-size:15px;"
        "color:#e8d5a8;"
        "background:transparent;"
        "border:none;"
        "line-height:1.7;"
        );

    //================ OBJECTIVES =================//

    QLabel* objective =
        new QLabel(
            "<span style='color:#c8a84b; font-weight:bold; font-size:15px;'>NEW OBJECTIVE</span><br><br>"

            "The vault contains <b>5 Sacred Stones</b>, each carved "
            "with an ancient hieroglyph — a symbol whose meaning "
            "has been lost to time.<br><br>"

            "The stones have been scattered across the chamber. "
            "<b>Collect all five</b> to reveal their secret.<br><br>"

            "<span style='color:#f5d060;'>&#9656;</span> "
            "The room is <b>pitch black</b> — trust your torch.<br><br>"

            "<span style='color:#f5d060;'>&#9656;</span> "
            "A dark spirit roams the chamber. Avoid it or lose score.<br><br>"

            "<span style='color:#f5d060;'>&#9656;</span> "
            "Collect all three stones to unlock the ancient secret."
            );

    objective->setWordWrap(true);

    objective->setStyleSheet(
        "font-size:13px;"
        "color:#d4c090;"
        "background:transparent;"
        "border:none;"
        "line-height:1.8;"
        );

    //================ WARNING =================//

    QLabel* warning =
        new QLabel(
            "The hieroglyphs whisper in the dark..."
            );

    warning->setAlignment(
        Qt::AlignCenter
        );

    warning->setStyleSheet(
        "font-size:13px;"
        "color:rgba(200,160,60,180);"
        "font-style:italic;"
        "background:transparent;"
        "border:none;"
        );

    //================ BUTTON =================//

    QPushButton* enterBtn =
        new QPushButton(
            "▶   ENTER THE ROOM OF TUTANKHAMUN "
            );

    enterBtn->setFixedHeight(58);

    enterBtn->setCursor(
        Qt::PointingHandCursor
        );

    enterBtn->setStyleSheet(
        "QPushButton {"
        " background-color:rgba(180,130,40,220);"
        " color:#fff8e7;"
        " font-size:16px;"
        " font-weight:bold;"
        " letter-spacing:3px;"
        " border:2px solid #c8a84b;"
        " border-radius:8px;"
        "}"
        "QPushButton:hover {"
        " background-color:rgba(220,170,60,240);"
        " border:2px solid #fff;"
        "}"
        "QPushButton:pressed {"
        " background-color:rgba(130,90,20,255);"
        "}"
        );

    //================ ADD =================//

    pl->addWidget(title);

    pl->addWidget(story);

    pl->addWidget(objective);

    pl->addWidget(warning);

    pl->addStretch();

    pl->addWidget(
        enterBtn,
        0,
        Qt::AlignCenter
        );

    popup->show();

    //================ ENTER LEVEL 2 =================//

    connect(
        enterBtn,
        &QPushButton::clicked,
        this,
        [=]()
        {
            dimmer->deleteLater();

            //================ LOAD LEVEL 2 =================//

            game.loadLevel(2);

            rocksCollected = 0;
            updateInventoryUI();
            updateLevel2HUD();
            if (level2HUD) {
                level2HUD->show();
            }
            seconds = 180;
            timer->start(1000);
            currentLevel =
                game.getCurrentLevel();

            if (mummy) {
                disconnect(mummy, nullptr, this, nullptr);
                mummy->setPaused(true);
                mummy->discardActiveHomingProjectiles();
            }
            if (ghost) {
                disconnect(ghost, nullptr, this, nullptr);
                ghost->setPaused(true);
            }

            scene->clear();

            mummy = nullptr;
            ghost = nullptr;
            playerSprite = nullptr;

            currentLevel->loadScene(scene);

            //================ PLAYER (before ghost — ghost holds this pointer) =================//

            playerSprite = scene->addPixmap(spriteFront);

            playerSprite->setScale(0.12);

            playerSprite->setPos(500, 720);

            game.getPlayer().moveTo(500, 720);

            //================ GHOST =================//

            ghost = new Level2Enemy(
                &game.getPlayer(),
                playerSprite
                );

            ghost->setPos(700, 400);

            ghost->setZValue(999);

            scene->addItem(ghost);
            connect(
                ghost,
                &Level2Enemy::reduceSpeed,
                this,
                &GameWindow::handleGhostStrike
                );

            connect(
                ghost,
                &Level2Enemy::ghostScreech,
                this,
                &GameWindow::showGhostGameOver
                );

            stack->setCurrentWidget(gameScreen);

            this->setFocus();
        }
        );
}

/* ================= RESTART ================= */
void GameWindow::restartGame()
{
    timer->stop();

    if (mummy) {
        disconnect(mummy, nullptr, this, nullptr);
        mummy->setPaused(true);
        mummy->discardActiveHomingProjectiles();
    }
    if (ghost) {
        disconnect(ghost, nullptr, this, nullptr);
        ghost->setPaused(true);
    }

    if (!scene) {
        mummy = nullptr;
        ghost = nullptr;
        playerSprite = nullptr;
        return;
    }

    // Destroys all QGraphicsItems; do not deleteLater() enemies separately — that double-frees.
    scene->clear();

    mummy = nullptr;
    ghost = nullptr;
    playerSprite = nullptr;

    if (horrorMusic) {
        horrorMusic->stop();
    }
    if (level3Music) {
        level3Music->stop();
    }
    if (startMusic) {
        startMusic->play();
    }

    game.restartGame();
    game.loadLevel(1);
    rocksCollected = 0;
    currentLevel = game.getCurrentLevel();
    currentLevel->loadScene(scene);

    spriteFront = QPixmap(":/new/prefix1/images/player front.png");
    spriteBack  = QPixmap(":/new/prefix1/images/player back.png");
    spriteLeft  = QPixmap(":/new/prefix1/images/player left.png");
    spriteRight = QPixmap(":/new/prefix1/images/player right.png");

    playerSprite = scene->addPixmap(spriteFront);
    playerSprite->setScale(0.12);
    playerSprite->setPos(game.getPlayer().getX(), game.getPlayer().getY());
    playerSpeedStep = 3;

    if (level2HUD) {
        level2HUD->hide();
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

    connect(
        mummy,
        &Level1Enemy::fireballHitRegistered,
        this,
        [this]()
        {
            updateHUD();
        }
        );

    seconds = 300;
    timer->start(1000);
    updateInventoryUI();
    updateHUD();
    stack->setCurrentWidget(gameScreen);
    this->setFocus();
}
//=================SAVE=====================//
void GameWindow::saveGame()
{
    QFile file("savegame.txt");

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << QDir::currentPath();

        QTextStream out(&file);

        out << "PlayerX: " << game.getPlayer().getX() << "\n";
        out << "PlayerY: " << game.getPlayer().getY() << "\n";

        out << "Score: " << game.getPlayer().getScore() << "\n";

        out << "Seconds: " << seconds << "\n";

        out << "Coins: " << coinCount << "\n";
        out << "Scrolls: " << scrollCount << "\n";
        out << "Masks: " << maskCount << "\n";
        out << "Amulets: " << amuletCount << "\n";
        out << "Timers: " << timerCount << "\n";

        file.close();
        QMessageBox::information(
            this,
            "Saved",
            "Game saved successfully!"
            );
    }
}

void GameWindow::loadGame()
{
    QFile file("savegame.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(
            this,
            "Load",
            "No save file found (savegame.txt)."
            );
        return;
    }

    QTextStream in(&file);
    int px = 0;
    int py = 0;
    int savedScore = -1;
    int savedSeconds = 0;
    bool haveX = false;
    bool haveY = false;
    bool haveSeconds = false;

    while (!in.atEnd()) {
        const QString line = in.readLine();
        const int colon = line.indexOf(':');
        if (colon < 0) {
            continue;
        }

        const QString key = line.left(colon).trimmed();
        const QString value = line.mid(colon + 1).trimmed();
        bool ok = false;
        const int v = value.toInt(&ok);

        if (!ok) {
            continue;
        }

        if (key == "PlayerX") {
            px = v;
            haveX = true;
        } else if (key == "PlayerY") {
            py = v;
            haveY = true;
        } else if (key == "Score") {
            savedScore = v;
        } else if (key == "Seconds") {
            savedSeconds = v;
            haveSeconds = true;
        } else if (key == "Coins") {
            coinCount = v;
        } else if (key == "Scrolls") {
            scrollCount = v;
        } else if (key == "Masks") {
            maskCount = v;
        } else if (key == "Amulets") {
            amuletCount = v;
        } else if (key == "Timers") {
            timerCount = v;
        }
    }

    file.close();

    if (!haveX || !haveY || !haveSeconds) {
        QMessageBox::warning(
            this,
            "Load",
            "Save file is missing position or time data."
            );
        return;
    }

    seconds = savedSeconds;

    game.getPlayer().moveTo(px, py);

    if (savedScore >= 0) {
        const int cur = game.getPlayer().getScore();
        const int delta = savedScore - cur;
        if (delta > 0) {
            game.getPlayer().addScore(delta);
        } else if (delta < 0) {
            game.getPlayer().deductScore(-delta);
        }
    }

    if (playerSprite) {
        playerSprite->setPos(px, py);
    }

    updateHUD();
    updateInventoryUI();

    QMessageBox::information(
        this,
        "Loaded",
        "Game state restored from savegame.txt."
        );
}

void GameWindow::setupGameOverScreen()
{
}

/* ================= RESIZE ================= */

void GameWindow::resizeEvent(
    QResizeEvent *event
    )
{
    QMainWindow::resizeEvent(event);
}
void GameWindow::mousePressEvent(QMouseEvent *event)
{
    if (!view) {
        QMainWindow::mousePressEvent(event);
        return;
    }

    QPoint viewPos =
        view->mapFrom(
            this,
            event->pos()
            );

    QPointF scenePos =
        view->mapToScene(viewPos);

    qDebug()
        << "X:"
        << scenePos.x()
        << "Y:"
        << scenePos.y();

    QMainWindow::mousePressEvent(event);
}
void GameWindow::exitGame()
{
    game.exitGame();

    close();
}
