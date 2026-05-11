#include "gameWindow.h"
#include <QDebug>
#include <QPixmap>
#include <QFile>
#include <QTextStream>

#include "level1enemy.h"

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

    //================ STACK =================//

    stack =
        new QStackedWidget(this);

    setCentralWidget(stack);

    //================ SETUP SCREENS =================//

    setupStartScreen();

    setupGameScreen();

    setupGameOverScreen();

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

/* ================= GAME OVER SCREEN ================= */

void GameWindow::setupGameOverScreen()
{
    gameOverScreen =
        new QWidget();

    QVBoxLayout* layout =
        new QVBoxLayout(gameOverScreen);

    QLabel* over =
        new QLabel("GAME OVER");

    over->setAlignment(
        Qt::AlignCenter
        );

    QPushButton* restartBtn =
        new QPushButton("RESTART");

    QPushButton* exitBtn =
        new QPushButton("EXIT");

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

    layout->addWidget(over);

    layout->addWidget(restartBtn);

    layout->addWidget(exitBtn);

    stack->addWidget(gameOverScreen);
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

    statusLabel =
        new QLabel("ALIVE");

    hud->addWidget(clockLabel);

    hud->addWidget(scoreLabel);

    hud->addWidget(statusLabel);

    mainLayout->addLayout(hud);
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

    timer->start(1000);

    scene->clear();

    //================ COLLISION MASK ================//
    collisionMask =QImage(":/new/prefix1/images/level1 BW.png");
    //================ LOAD DIRECTIONAL SPRITES =================//

    spriteFront =
        QPixmap(":/new/prefix1/images/player front.png");

    spriteBack =
        QPixmap(":/new/prefix1/images/player back.png");

    spriteLeft =
        QPixmap(":/new/prefix1/images/player left.png");

    spriteRight =
        QPixmap(":/new/prefix1/images/player right.png");

    //================ COLLISION MASK ================//

    collisionMask =
        QImage(":/new/prefix1/images/level1 BW.png");

    currentLevel =
        game.getCurrentLevel();

    currentLevel->loadScene(scene);

    //================ PLAYER SPRITE =================//
    // Starts facing front

    playerSprite =
        scene->addPixmap(spriteFront);

    playerSprite->setScale(0.12);

    // Use player coordinates from Game/Player
    playerSprite->setPos(
        game.getPlayer().getX(),
        game.getPlayer().getY()
        );

    //================ ENEMY =================//
   mummy =new Level1Enemy(&game.getPlayer(),playerSprite);

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

    stack->setCurrentWidget(gameScreen);

    this->setFocus();
}

/* ================= MOVEMENT ================= */
// Swaps directional sprite before moving

void GameWindow::movePlayer(
    int dx,
    int dy,
    const QPixmap& sprite
    )
{
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

    if(isWalkable(newPos))
    {
        playerSprite->setPos(newPos);

        game.getPlayer().moveTo(
            newPos.x(),
            newPos.y()
            );

        checkArtifactCollisions();
    }
}
/* ================= WALKABLE ================= */

bool GameWindow::isWalkable(QPointF newPos)
{
    QRectF rect =
        playerSprite->sceneBoundingRect();

    QPointF scenePos(
        newPos.x() + rect.width()  / 2,
        newPos.y() + rect.height() - 5
        );

    QRectF sceneRect =
        scene->sceneRect();

    int maskX =
        (scenePos.x() / sceneRect.width())
        * collisionMask.width();

    int maskY =
        (scenePos.y() / sceneRect.height())
        * collisionMask.height();

    //================ PREVENT CRASH =================//
    // Prevent out-of-bounds access at edges

    maskX = qBound(
        0,
        maskX,
        collisionMask.width() - 1
        );

    maskY = qBound(
        0,
        maskY,
        collisionMask.height() - 1
        );

    QColor color =
        collisionMask.pixelColor(
            maskX,
            maskY
            );

    return !(
        color.red()   < 20 &&
        color.green() < 20 &&
        color.blue()  < 20
        );
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

        if(type == "timer")
        {
            seconds += 30;
        }

        updateInventoryUI();

        updateHUD();
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
}

/* ================= UPDATE INVENTORY ================= */

void GameWindow::updateInventoryUI()
{
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

/* ================= UPDATE GAME ================= */

void GameWindow::updateGame()
{
    seconds--;

    updateHUD();

    game.update(1.0f);

    if(
        seconds <= 0 ||
        game.getstate() == Gamestate::gameOver
        )
    {
        timer->stop();

        stack->setCurrentWidget(
            gameOverScreen
            );
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

    const int step = 3;

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

    case Qt::Key_Escape:

        pauseGame();

        stack->setCurrentWidget(
            startScreen
            );

        break;

    default:
        break;
    }
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
    if(mummy)
    {
        mummy->setPaused(false);
    }

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
/* ================= RESTART ================= */

void GameWindow::restartGame()
{
    timer->stop();

    //================ CLEAR SCENE ================//

    scene->clear();

    //================ RESET GAME =================//

    game.restartGame();

    game.loadLevel(
        game.getLevelIndex()
        );

    currentLevel =
        game.getCurrentLevel();

    currentLevel->loadScene(scene);

    //================ RELOAD SPRITES ================//

    spriteFront =
        QPixmap(":/new/prefix1/images/player front.png");

    spriteBack =
        QPixmap(":/new/prefix1/images/player back.png");

    spriteLeft =
        QPixmap(":/new/prefix1/images/player left.png");

    spriteRight =
        QPixmap(":/new/prefix1/images/player right.png");

    //================ RECREATE PLAYER =================//

    playerSprite =
        scene->addPixmap(spriteFront);

    playerSprite->setScale(0.12);

    playerSprite->setPos(
        game.getPlayer().getX(),
        game.getPlayer().getY()
        );

    //================ RESET TIMER ================//

    seconds = 300;

    timer->start(1000);

    //================ UPDATE UI ================//

    updateInventoryUI();

    updateHUD();

    stack->setCurrentWidget(
        gameScreen
        );

    this->setFocus();
}

/* ================= EXIT ================= */

void GameWindow::exitGame()
{
    game.exitGame();

    close();
}

/* ================= SAVE GAME ================= */

void GameWindow::saveGame()
{
    QFile file("savegame.txt");

    if(
        file.open(
            QIODevice::WriteOnly |
            QIODevice::Text
            )
        )
    {
        QTextStream out(&file);

        out << game.getPlayer().getX()     << "\n";
        out << game.getPlayer().getY()     << "\n";
        out << game.getPlayer().getScore() << "\n";

        out << seconds << "\n";

        out << coinCount   << "\n";
        out << scrollCount << "\n";
        out << maskCount   << "\n";
        out << amuletCount << "\n";
        out << timerCount  << "\n";

        file.close();
    }
}

/* ================= LOAD GAME ================= */

void GameWindow::loadGame()
{
    QFile file("savegame.txt");

    if(
        file.open(
            QIODevice::ReadOnly |
            QIODevice::Text
            )
        )
    {
        QTextStream in(&file);

        int x;
        int y;
        int score;

        in >> x;
        in >> y;
        in >> score;

        in >> seconds;

        in >> coinCount;
        in >> scrollCount;
        in >> maskCount;
        in >> amuletCount;
        in >> timerCount;

        file.close();

        //================ PLAYER =================//

        game.getPlayer().moveTo(x, y);

        if(playerSprite != nullptr)
        {
            playerSprite->setPos(x, y);
        }

        //================ INVENTORY =================//

        coinCounter->setText(
            "x" + QString::number(coinCount)
            );

        scrollCounter->setText(
            "x" + QString::number(scrollCount)
            );

        maskCounter->setText(
            "x" + QString::number(maskCount)
            );

        amuletCounter->setText(
            "x" + QString::number(amuletCount)
            );

        timerCounter->setText(
            "x" + QString::number(timerCount)
            );
    }
}

/* ================= RESIZE ================= */

void GameWindow::resizeEvent(
    QResizeEvent *event
    )
{
    QMainWindow::resizeEvent(event);
}
