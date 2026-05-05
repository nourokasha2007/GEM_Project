#include "gameWindow.h"
#include "Level1.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QDebug>

/* ================= CONSTRUCTOR ================= */

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), seconds(0)
{
    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    startScreen = new QWidget();
    gameScreen = new QWidget();
    gameOverScreen = new QWidget();

    currentLevel = nullptr;
    playerSprite = nullptr;

    /* ================= START SCREEN ================= */
    {
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
        bgOverlay->setStyleSheet("background-color: rgba(0,0,0,110);");
        bgOverlay->setGeometry(0, 0, 9999, 9999);

        // --- Main vertical layout ---
        QVBoxLayout *layout = new QVBoxLayout(startScreen);
        layout->setAlignment(Qt::AlignCenter);
        layout->setSpacing(12);

        // ── GUEST-ONLY CARD ───────────────────────────────────────
        QWidget *card = new QWidget();
        card->setFixedSize(380, 220);
        card->setStyleSheet(
            "background-color: rgba(10, 5, 0, 185);"
            "border: 1px solid rgba(200, 160, 60, 160);"
            "border-radius: 10px;"
            );

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setSpacing(12);
        cardLayout->setContentsMargins(30, 24, 30, 24);

        QLabel *cardTitle = new QLabel("ENTER THE MUSEUM");
        cardTitle->setAlignment(Qt::AlignCenter);
        cardTitle->setStyleSheet(
            "font-size: 14px; font-weight: bold; color: #c8a84b;"
            "letter-spacing: 4px; background: transparent; border: none;"
            );

        guestNameEdit = new QLineEdit();
        guestNameEdit->setPlaceholderText("Your Name");
        guestNameEdit->setFixedHeight(44);
        guestNameEdit->setStyleSheet(
            "QLineEdit {"
            "  background-color: rgba(255,255,255,15);"
            "  border: 1px solid rgba(200,160,60,120);"
            "  border-radius: 4px; color: #f5e6c8;"
            "  font-size: 15px; padding-left: 12px;"
            "}"
            "QLineEdit:focus {"
            "  border: 1px solid #c8a84b;"
            "  background-color: rgba(255,255,255,25);"
            "}"
            );

        QPushButton *enterBtn = new QPushButton("\u25B6   PROTECT THE HERITAGE");
        enterBtn->setFixedHeight(48);
        enterBtn->setCursor(Qt::PointingHandCursor);
        enterBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: rgba(180,130,40,220); color: #fff8e7;"
            "  font-size: 15px; font-weight: bold; letter-spacing: 3px;"
            "  border: 2px solid #c8a84b; border-radius: 4px;"
            "}"
            "QPushButton:hover {"
            "  background-color: rgba(220,170,60,240);"
            "  border: 2px solid #ffffff; color: #ffffff;"
            "}"
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

        startScreen->setMinimumSize(1000, 700);
        stack->addWidget(startScreen);
    }

    /* ================= GAME SCREEN ================= */
    {
        gameScreen->setStyleSheet(
            "background-color:black;"
            "color:white;"
            );

        QVBoxLayout *mainLayout =
            new QVBoxLayout(gameScreen);

        //---------------- HUD ----------------
        QHBoxLayout *hud =
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

        //---------------- GAME VIEW ----------------
        scene =
            new QGraphicsScene(this);

        view =
            new QGraphicsView(scene);

        view->setFixedSize(1000,700);

        mainLayout->addWidget(
            view,
            0,
            Qt::AlignCenter
            );

        //---------------- BUTTONS ----------------
        QHBoxLayout *buttons =
            new QHBoxLayout();

        QPushButton *pauseBtn =
            new QPushButton("PAUSE");

        QPushButton *restartBtn =
            new QPushButton("RESTART");

        QPushButton *exitBtn =
            new QPushButton("EXIT");

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

        buttons->addWidget(pauseBtn);
        buttons->addWidget(restartBtn);
        buttons->addWidget(exitBtn);

        mainLayout->addLayout(buttons);

        stack->addWidget(gameScreen);
    }

    /* ================= GAME OVER SCREEN ================= */
    {
        QVBoxLayout *layout =
            new QVBoxLayout(gameOverScreen);

        QLabel *over =
            new QLabel("GAME OVER");

        over->setAlignment(
            Qt::AlignCenter
            );

        QPushButton *restartBtn =
            new QPushButton("RESTART");

        QPushButton *exitBtn =
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

    /* ================= TIMER ================= */
    timer = new QTimer(this);

    connect(
        timer,
        &QTimer::timeout,
        this,
        &GameWindow::updateGame
        );

    stack->setCurrentWidget(
        startScreen
        );

    showMaximized(); // Fill the entire screen
}


/* ================= BRIEFING POPUP ================= */

void GameWindow::showBriefingPopup(const QString &playerName)
{
    // Fullscreen dim overlay
    QWidget *dimmer = new QWidget(this);
    dimmer->setGeometry(0, 0, width(), height());
    dimmer->setStyleSheet("background-color: rgba(0,0,0,180);");
    dimmer->show();
    dimmer->raise();

    // Popup card
    QWidget *popup = new QWidget(dimmer);
    popup->setFixedSize(620, 480);
    popup->move((dimmer->width() - 620) / 2, (dimmer->height() - 480) / 2);
    popup->setStyleSheet(
        "background-color: rgba(8, 4, 0, 230);"
        "border: 2px solid rgba(200, 160, 60, 200);"
        "border-radius: 12px;"
        );

    QVBoxLayout *pl = new QVBoxLayout(popup);
    pl->setContentsMargins(40, 32, 40, 28);
    pl->setSpacing(14);

    // Header
    QLabel *header = new QLabel("NIGHT SHIFT INITIATED!");
    header->setAlignment(Qt::AlignCenter);
    header->setStyleSheet(
        "font-size: 22px; font-weight: bold; color: #f5d060;"
        "letter-spacing: 4px; background: transparent; border: none;"
        );

    // Welcome line
    QString welcomeText = QString(
                              "Welcome aboard, <b><span style='color:#f5d060;'>%1</span></b>, "
                              "to the prestigious <b>GRAND EGYPTIAN MUSEUM!</b><br><br>"
                              "The sun has set, the tourists are gone — but the halls are far from quiet. "
                              "Priceless artifacts have vanished into the shadows!<br>"
                              "As our lead Night Guard, the museum's legacy rests on your shoulders."
                              ).arg(playerName);

    QLabel *welcome = new QLabel(welcomeText);
    welcome->setWordWrap(true);
    welcome->setAlignment(Qt::AlignCenter);
    welcome->setStyleSheet(
        "font-size: 13px; color: #e8d5a8; background: transparent; border: none; line-height: 1.5;"
        );

    // Mission bullets
    QLabel *missions = new QLabel(
        "<span style='color:#c8a84b; font-weight:bold;'>YOUR MISSION BRIEFING</span><br><br>"
        "<span style='color:#f5d060;'>&#9656;</span>  <b>RECOVER THE RELICS</b> — find every missing artifact before dawn.<br>"
        "<span style='color:#f5d060;'>&#9656;</span>  <b>FACE THE UNKNOWN</b> — watch out for enemies lurking in the dark!<br>"
        "<span style='color:#f5d060;'>&#9656;</span>  <b>LEVEL UP</b> — progress through increasingly challenging wings.<br>"
        "<span style='color:#f5d060;'>&#9656;</span>  <b>FOLLOW THE TRAIL</b> — use hidden hints to track what was stolen."
        );
    missions->setWordWrap(true);
    missions->setStyleSheet(
        "font-size: 12px; color: #d4c090; background: transparent; border: none; line-height:1.6;"
        );

    // Clock warning
    QLabel *warning = new QLabel("The museum opens at 7:00 AM sharp. Don't let history stay empty!");
    warning->setAlignment(Qt::AlignCenter);
    warning->setWordWrap(true);
    warning->setStyleSheet(
        "font-size: 12px; color: rgba(200,160,60,180); font-style:italic;"
        "background: transparent; border: none;"
        );

    // Begin button
    QPushButton *beginBtn = new QPushButton("\u25B6   PROTECT THE HERITAGE");
    beginBtn->setFixedHeight(50);
    beginBtn->setCursor(Qt::PointingHandCursor);
    beginBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: rgba(180,130,40,220);"
        "  color: #fff8e7; font-size: 15px; font-weight: bold;"
        "  letter-spacing: 3px; border: 2px solid #c8a84b; border-radius: 6px;"
        "}"
        "QPushButton:hover { background-color: rgba(220,170,60,240); border: 2px solid #fff; color:#fff; }"
        "QPushButton:pressed { background-color: rgba(130,90,20,255); }"
        );

    pl->addWidget(header);
    pl->addWidget(welcome);
    pl->addWidget(missions);
    pl->addWidget(warning);
    pl->addStretch(1);
    pl->addWidget(beginBtn, 0, Qt::AlignCenter);

    popup->show();

    // Tap anywhere on dimmer (outside popup) also closes
    connect(beginBtn, &QPushButton::clicked, this, [=]() {
        dimmer->deleteLater();
        startGame();
    });
}

/* ================= START GAME ================= */

void GameWindow::startGame()
{
    game.startGame();

    seconds = 0;
    timer->start(1000);

    scene->clear();

    //----------------------------------
    // Load level visuals
    //----------------------------------
    currentLevel =
        game.getCurrentLevel();

    currentLevel->loadScene(scene);

    //----------------------------------
    // Create player sprite
    //----------------------------------
    playerSprite =
        scene->addPixmap(
            QPixmap(":/new/prefix1/images/ChatGPT Image Apr 28, 2026, 05_48_57 PM.png"));
    playerSprite->setScale(0.08);

    playerSprite->setPos(
        game.getPlayer().getX(),
        game.getPlayer().getY()
        );

    stack->setCurrentWidget(
        gameScreen
        );
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

    seconds = 0;
    timer->start(1000);

    startGame();
}

/* ================= EXIT ================= */

void GameWindow::exitGame()
{
    game.exitGame();
    close();
}

/* ================= UPDATE GAME ================= */

void GameWindow::updateGame()
{
    seconds++;
    game.update(1.0f);

    // 1. Update the Clock Display
    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;

    clockLabel->setText(
        QString("%1:%2:%3")
            .arg(h, 2, 10, QChar('0'))
            .arg(m, 2, 10, QChar('0'))
            .arg(s, 2, 10, QChar('0'))
        );

    // 2. Update the Score Display
    scoreLabel->setText("Score: " + QString::number(game.getPlayer().getScore()));

    // 3. CHECK FOR WIN/LOSS CONDITIONS
    // Check if player lost (e.g., hit too many obstacles or time ran out)
    if (game.getstate() == Gamestate::gameOver) {
        timer->stop();
        // If you made 'over' a class member in .h, you can change the text:
        // statusLabel->setText("YOU DIED");
        stack->setCurrentWidget(gameOverScreen);
        return; // Stop processing further
    }

    // Check if player won (e.g., collected enough gems)
    if (game.getPlayer().getScore() >= 100) {
        timer->stop();
        // statusLabel->setText("MISSION ACCOMPLISHED");
        stack->setCurrentWidget(gameOverScreen);
        return;
    }

    // 4. Update HUD Status
    statusLabel->setText(
        (game.getstate() == Gamestate::playing) ? "ALIVE" : "PAUSED"
        );
}

/* ================= MOVEMENT ================= */

void GameWindow::keyPressEvent(
    QKeyEvent *event
    )
{
    if(
        stack->currentWidget()
        != gameScreen
        )
        return;

    int step = 20;

    QPointF oldPos =
        playerSprite->pos();

    QPointF newPos =
        oldPos;

    if(event->key()==Qt::Key_W)
        newPos.setY(
            newPos.y()-step
            );

    if(event->key()==Qt::Key_S)
        newPos.setY(
            newPos.y()+step
            );

    if(event->key()==Qt::Key_A)
        newPos.setX(
            newPos.x()-step
            );

    if(event->key()==Qt::Key_D)
        newPos.setX(
            newPos.x()+step
            );

    playerSprite->setPos(
        newPos
        );

    //-----------------------------------
    // Collision with statues/walls
    //-----------------------------------
    for(
        auto obstacle :
        currentLevel->obstacles
        )
    {
        if(
            playerSprite
                ->collidesWithItem(
                    obstacle
                    )
            )
        {
            playerSprite->setPos(
                oldPos
                );

            game.getPlayer()
                .deductScore(5);

            break;
        }
    }

    //-----------------------------------
    // Artifact collection
    //-----------------------------------
    for(
        auto artifact :
        currentLevel->artifacts
        )
    {
        if(
            playerSprite
                ->collidesWithItem(
                    artifact
                    )
            )
        {
            scene->removeItem(
                artifact
                );

            game.getPlayer()
                .addScore(10);
        }
    }

    //-----------------------------------
    // Sync logical player position
    //-----------------------------------
    game.getPlayer().moveTo(
        newPos.x(),
        newPos.y()
        );

    //-----------------------------------
    // Escape
    //-----------------------------------
    if(
        event->key() ==
        Qt::Key_Escape
        )
    {
        pauseGame();
        stack->setCurrentWidget(
            startScreen
            );
    }
}

/* ================= RESIZE EVENT ================= */

void GameWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (bgOverlay)
        bgOverlay->setGeometry(0, 0, width(), height());
    if (startScreen)
        startScreen->setGeometry(0, 0, width(), height());
}
void GameWindow::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = view->mapToScene(event->pos());
    qDebug() << "CLICK:" << pos;
}
