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
        // Background image label (fills the widget)
        QLabel *bg = new QLabel(startScreen);
        bg->setPixmap(
            QPixmap(":/new/prefix1/images/Gemini_Generated_Image_ibun6yibun6yibun.png")
                .scaled(1200, 800, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)
            );
        bg->setAlignment(Qt::AlignCenter);
        bg->setGeometry(0, 0, 1200, 800);

        // Semi-transparent dark overlay for readability
        QLabel *overlay = new QLabel(startScreen);
        overlay->setGeometry(0, 0, 1200, 800);
        overlay->setStyleSheet("background-color: rgba(0, 0, 0, 120);");

        // Layout sitting on top
        QVBoxLayout *layout = new QVBoxLayout(startScreen);
        layout->setAlignment(Qt::AlignCenter);
        layout->setSpacing(20);

        // Title
        QLabel *title = new QLabel("GEM : NIGHT SHIFT");
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(
            "font-size: 52px;"
            "font-weight: bold;"
            "color: #f5d78e;"
            "letter-spacing: 8px;"
            "background: transparent;"
            "font-family: \'Palatino Linotype\', serif;"
            );

        // Subtitle
        QLabel *subtitle = new QLabel("A Museum After Midnight");
        subtitle->setAlignment(Qt::AlignCenter);
        subtitle->setStyleSheet(
            "font-size: 18px;"
            "color: #d4b483;"
            "letter-spacing: 4px;"
            "background: transparent;"
            "font-style: italic;"
            "font-family: \'Palatino Linotype\', serif;"
            );

        // START Button
        QPushButton *startBtn = new QPushButton("\u25B6   ENTER THE MUSEUM");
        startBtn->setFixedSize(320, 65);
        startBtn->setCursor(Qt::PointingHandCursor);
        startBtn->setStyleSheet(
            "QPushButton {"
            "  background-color: rgba(180, 130, 60, 200);"
            "  color: #fff8e7;"
            "  font-size: 18px;"
            "  font-weight: bold;"
            "  letter-spacing: 3px;"
            "  border: 2px solid #f5d78e;"
            "  border-radius: 4px;"
            "}"
            "QPushButton:hover {"
            "  background-color: rgba(210, 160, 80, 230);"
            "  color: #ffffff;"
            "  border: 2px solid #ffffff;"
            "}"
            "QPushButton:pressed {"
            "  background-color: rgba(140, 100, 40, 255);"
            "}"
            );

        connect(
            startBtn,
            &QPushButton::clicked,
            this,
            &GameWindow::startGame
            );

        layout->addStretch(3);
        layout->addWidget(title,    0, Qt::AlignCenter);
        layout->addWidget(subtitle, 0, Qt::AlignCenter);
        layout->addSpacing(16);
        layout->addWidget(startBtn, 0, Qt::AlignCenter);
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
void GameWindow::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = view->mapToScene(event->pos());
    qDebug() << "CLICK:" << pos;
}
