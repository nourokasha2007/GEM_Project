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
        startScreen->setStyleSheet(
            "background-color: #d7c4a3;"
            "color: #3b2f2f;"
            );

        QVBoxLayout *layout = new QVBoxLayout(startScreen);
        layout->setAlignment(Qt::AlignCenter);

        QLabel *title = new QLabel("GEM : NIGHT SHIFT");
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet(
            "font-size:32px;"
            "font-weight:bold;"
            );

        QPushButton *startBtn = new QPushButton("START");
        startBtn->setFixedSize(220,60);

        connect(
            startBtn,
            &QPushButton::clicked,
            this,
            &GameWindow::startGame
            );

        layout->addWidget(title);
        layout->addWidget(startBtn);

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

    int h = seconds / 3600;
    int m = (seconds % 3600) / 60;
    int s = seconds % 60;

    clockLabel->setText(
        QString("%1:%2:%3")
            .arg(h,2,10,QChar('0'))
            .arg(m,2,10,QChar('0'))
            .arg(s,2,10,QChar('0'))
        );

    scoreLabel->setText(
        "Score: " +
        QString::number(
            game.getPlayer().getScore()
            )
        );

    statusLabel->setText(
        (game.getstate() ==
         Gamestate::playing)
            ? "ALIVE"
            : "PAUSED"
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
