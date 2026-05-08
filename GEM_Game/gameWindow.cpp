#include "gameWindow.h"
#include <QDebug>
#include <QPixmap>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
{
    seconds = 300;

    currentLevel = nullptr;

    stack = new QStackedWidget(this);

    setCentralWidget(stack);

    setupStartScreen();

    setupGameScreen();

    setupGameOverScreen();

    timer = new QTimer(this);

    connect(
        timer,
        &QTimer::timeout,
        this,
        &GameWindow::updateGame
        );

    stack->setCurrentWidget(startScreen);

    showMaximized();
}

/* ================= START SCREEN ================= */
void GameWindow::setupStartScreen()
{
    startScreen = new QWidget();

    // --- Background via stylesheet (auto-scales with window) ---
    startScreen->setStyleSheet(
        "QWidget#startScreen {"
        "  background-image: url(:/new/prefix1/images/PHOTO-2026-04-29-21-07-06.jpg);"
        "  background-position: center;"
        "  background-repeat: no-repeat;"
        "  background-color: #000000;"
        "}"
        );

    startScreen->setObjectName("startScreen");

    // --- Dark overlay (member, resizes with window) ---

    bgOverlay = new QLabel(startScreen);

    bgOverlay->setStyleSheet(
        "background-color: rgba(0,0,0,110);"
        );

    bgOverlay->setGeometry(
        0,
        0,
        9999,
        9999
        );

    // --- Main vertical layout ---

    QVBoxLayout *layout =
        new QVBoxLayout(startScreen);

    layout->setAlignment(
        Qt::AlignCenter
        );

    layout->setSpacing(12);

    //================ CARD =================//

    QWidget *card =
        new QWidget();

    card->setFixedSize(
        380,
        220
        );

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
        new QLabel(
            "ENTER THE MUSEUM"
            );

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
                name =
                    "Night Guard";
            }

            showBriefingPopup(name);
        });

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
//=================Briefing Popup======================//
void GameWindow::showBriefingPopup(
    const QString &playerName
    )
{
    //================ FULLSCREEN DIM OVERLAY ================//

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

    //================ POPUP CARD ================//

    QWidget *popup =
        new QWidget(dimmer);

    popup->setFixedSize(
        620,
        480
        );

    popup->move(
        (dimmer->width() - 620) / 2,
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

    //================ HEADER ================//

    QLabel *header =
        new QLabel(
            "NIGHT SHIFT INITIATED!"
            );

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

    //================ WELCOME TEXT ================//

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

    //================ MISSIONS ================//

    QLabel *missions =
        new QLabel(
            "<span style='color:#c8a84b; font-weight:bold;'>"
            "YOUR MISSION BRIEFING"
            "</span><br><br>"

            "<span style='color:#f5d060;'>&#9656;</span> "
            "<b>RECOVER THE RELICS</b> — "
            "find every missing artifact before dawn.<br>"

            "<span style='color:#f5d060;'>&#9656;</span> "
            "<b>FACE THE UNKNOWN</b> — "
            "watch out for enemies lurking in the dark!<br>"

            "<span style='color:#f5d060;'>&#9656;</span> "
            "<b>LEVEL UP</b> — "
            "progress through increasingly challenging wings.<br>"

            "<span style='color:#f5d060;'>&#9656;</span> "
            "<b>FOLLOW THE TRAIL</b> — "
            "use hidden hints to track what was stolen."
            );

    missions->setWordWrap(true);

    missions->setStyleSheet(
        "font-size: 12px;"
        "color: #d4c090;"
        "background: transparent;"
        "border: none;"
        "line-height:1.6;"
        );

    //================ WARNING ================//

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

    //================ BUTTON ================//

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

    //================ LAYOUT ================//

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

    //================ START GAME ================//

    connect(
        beginBtn,
        &QPushButton::clicked,
        this,
        [=]()
        {
            dimmer->deleteLater();

            startGame();
        });
}

/* ================= GAME SCREEN ================= */

void GameWindow::setupGameScreen()
{
    gameScreen = new QWidget();
    gameScreen->setStyleSheet("background-color: black;");

    QVBoxLayout* mainLayout =
        new QVBoxLayout(gameScreen);

    setupHUD(mainLayout);

    scene =new QGraphicsScene(this);

    view = new QGraphicsView(scene);

    view->setBackgroundBrush(
        QBrush(Qt::black)
        );

    view->setHorizontalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff
        );

    view->setVerticalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff
        );

    view->setFocusPolicy(Qt::NoFocus);

    view->setFixedSize(1400, 700);

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
    gameOverScreen = new QWidget();

    QVBoxLayout* layout =
        new QVBoxLayout(gameOverScreen);

    QLabel* over =
        new QLabel("GAME OVER");

    over->setAlignment(Qt::AlignCenter);

    QPushButton* restartBtn =
        new QPushButton("RESTART");

    QPushButton* exitBtn =
        new QPushButton("EXIT");

    connect(restartBtn,
            &QPushButton::clicked,
            this,
            &GameWindow::restartGame);

    connect(exitBtn,
            &QPushButton::clicked,
            this,
            &GameWindow::exitGame);

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

void GameWindow::setupInventoryUI(QHBoxLayout* gameLayout)
{
    QVBoxLayout* inventoryLayout =
        new QVBoxLayout();

    inventoryLayout->setSpacing(20);

    inventoryLayout->setAlignment(Qt::AlignTop);

    coinIcon = new QLabel();
    scrollIcon = new QLabel();
    maskIcon = new QLabel();
    amuletIcon = new QLabel();
    timerIcon = new QLabel();

    coinCounter = new QLabel();
    scrollCounter = new QLabel();
    maskCounter = new QLabel();
    amuletCounter = new QLabel();
    timerCounter = new QLabel();

    coinIcon->setPixmap(QPixmap(":/new/prefix1/images/coin2.png").scaled(40,40));
    scrollIcon->setPixmap(QPixmap(":/new/prefix1/images/scroll2.png").scaled(40,40));
    maskIcon->setPixmap(QPixmap(":/new/prefix1/images/mask2.png").scaled(40,40));
    amuletIcon->setPixmap(QPixmap(":/new/prefix1/images/amulet2.png").scaled(40,40));
    timerIcon->setPixmap(QPixmap(":/new/prefix1/images/timer.png").scaled(40,40));

    updateInventoryUI();

    QHBoxLayout* coinRow = new QHBoxLayout();
    coinRow->addWidget(coinIcon);
    coinRow->addWidget(coinCounter);

    QHBoxLayout* scrollRow = new QHBoxLayout();
    scrollRow->addWidget(scrollIcon);
    scrollRow->addWidget(scrollCounter);

    QHBoxLayout* maskRow = new QHBoxLayout();
    maskRow->addWidget(maskIcon);
    maskRow->addWidget(maskCounter);

    QHBoxLayout* amuletRow = new QHBoxLayout();
    amuletRow->addWidget(amuletIcon);
    amuletRow->addWidget(amuletCounter);

    QHBoxLayout* timerRow = new QHBoxLayout();
    timerRow->addWidget(timerIcon);
    timerRow->addWidget(timerCounter);

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

    connect(pauseBtn,
            &QPushButton::clicked,
            this,
            &GameWindow::pauseGame);

    connect(restartBtn,
            &QPushButton::clicked,
            this,
            &GameWindow::restartGame);

    connect(exitBtn,
            &QPushButton::clicked,
            this,
            &GameWindow::exitGame);

    buttons->addWidget(pauseBtn);
    buttons->addWidget(restartBtn);
    buttons->addWidget(exitBtn);

    mainLayout->addLayout(buttons);
}

/* ================= START GAME ================= */

void GameWindow::startGame()
{
    game.startGame();

    seconds = 300;

    timer->start(1000);

    scene->clear();

    collisionMask =
        QImage(":/new/prefix1/images/level1 BW.png");

    currentLevel =
        game.getCurrentLevel();

    currentLevel->loadScene(scene);

    playerSprite =
        scene->addPixmap(
            QPixmap(":/new/prefix1/images/ChatGPT Image Apr 28, 2026, 05_48_57 PM.png")
            );

    playerSprite->setScale(0.08);

    playerSprite->setPos(
        game.getPlayer().getX(),
        game.getPlayer().getY()
        );

    stack->setCurrentWidget(gameScreen);

    setFocus();
}

/* ================= MOVEMENT ================= */

void GameWindow::movePlayer(int dx, int dy)
{
    QPointF newPos =
        playerSprite->pos();

    newPos.setX(newPos.x() + dx);

    newPos.setY(newPos.y() + dy);

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
        newPos.x() + rect.width() / 2,
        newPos.y() + rect.height() - 5
        );

    QRectF sceneRect =
        scene->sceneRect();

    int maskX =
        (scenePos.x() / sceneRect.width()) * collisionMask.width();

    int maskY =
        (scenePos.y() / sceneRect.height()) * collisionMask.height();

    QColor color =
        collisionMask.pixelColor(maskX, maskY);

    return !(color.red() < 20 &&
             color.green() < 20 &&
             color.blue() < 20);
}

/* ================= COLLISIONS ================= */

void GameWindow::checkArtifactCollisions()
{
    QGraphicsItem* artifact =
        currentLevel->checkArtifactCollision(playerSprite);

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
        "Score: " +
        QString::number(
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

    if(seconds <= 0 ||
        game.getstate() == Gamestate::gameOver)
    {
        timer->stop();

        stack->setCurrentWidget(gameOverScreen);
    }
}

/* ================= KEYS ================= */

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if(stack->currentWidget() != gameScreen)
        return;

    int step = 3;

    if(event->key() == Qt::Key_Up)
        movePlayer(0, -step);

    else if(event->key() == Qt::Key_Down)
        movePlayer(0, step);

    else if(event->key() == Qt::Key_Left)
        movePlayer(-step, 0);

    else if(event->key() == Qt::Key_Right)
        movePlayer(step, 0);

    else if(event->key() == Qt::Key_Escape)
    {
        pauseGame();

        stack->setCurrentWidget(startScreen);
    }
}

/* ================= PAUSE ================= */

void GameWindow::pauseGame()
{
    game.pauseGame();

    timer->stop();
}

/* ================= RESTART ================= */

void GameWindow::restartGame()
{
    game.restartGame();

    startGame();
}

/* ================= EXIT ================= */

void GameWindow::exitGame()
{
    game.exitGame();

    close();
}

/* ================= RESIZE ================= */

void GameWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
}
