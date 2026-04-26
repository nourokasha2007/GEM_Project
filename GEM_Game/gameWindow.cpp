#include "gameWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

/* ================= CONSTRUCTOR ================= */

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent), seconds(0)
{
    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    startScreen = new QWidget();
    gameScreen = new QWidget();
    gameOverScreen = new QWidget();

    /* ================= START SCREEN ================= */
    {
        startScreen->setStyleSheet(
            "background-color: #d7c4a3;"   // beige
            "color: #3b2f2f;"
            );

        QVBoxLayout *layout = new QVBoxLayout(startScreen);
        layout->setAlignment(Qt::AlignCenter);

        QLabel *title = new QLabel("GEM : NIGHT SHIFT");
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("font-size: 32px; font-weight: bold;");

        QPushButton *startBtn = new QPushButton("START");

        startBtn->setFixedSize(220, 60);
        startBtn->setStyleSheet(
            "background-color: #6f4e37;"
            "color: white;"
            "font-size: 18px;"
            "border-radius: 10px;"
            );

        connect(startBtn, &QPushButton::clicked, this, &GameWindow::startGame);

        layout->addWidget(title);
        layout->addSpacing(30);
        layout->addWidget(startBtn, 0, Qt::AlignCenter);

        stack->addWidget(startScreen);
    }

    /* ================= GAME SCREEN ================= */
    {
        gameScreen->setStyleSheet(
            "background-color: #1e1e1e;"
            "color: white;"
            );

        QVBoxLayout *mainLayout = new QVBoxLayout(gameScreen);

        /* HUD */
        QHBoxLayout *hud = new QHBoxLayout();

        clockLabel = new QLabel("00:00:00");
        scoreLabel = new QLabel("Score: 0");
        statusLabel = new QLabel("ALIVE");

        hud->addWidget(clockLabel);
        hud->addWidget(scoreLabel);
        hud->addWidget(statusLabel);

        mainLayout->addLayout(hud);

        /* GAME AREA */
        QLabel *canvas = new QLabel();
        canvas->setFixedSize(600, 400);
        canvas->setStyleSheet("background-color: black;");
        mainLayout->addWidget(canvas, 0, Qt::AlignCenter);

        /* CONTROL BUTTONS */
        QHBoxLayout *buttons = new QHBoxLayout();

        QPushButton *pauseBtn = new QPushButton("PAUSE");
        QPushButton *restartBtn = new QPushButton("RESTART");
        QPushButton *exitBtn = new QPushButton("EXIT");

        pauseBtn->setStyleSheet("padding:6px;");
        restartBtn->setStyleSheet("padding:6px;");
        exitBtn->setStyleSheet("padding:6px;");

        connect(pauseBtn, &QPushButton::clicked, this, &GameWindow::pauseGame);
        connect(restartBtn, &QPushButton::clicked, this, &GameWindow::restartGame);
        connect(exitBtn, &QPushButton::clicked, this, &GameWindow::exitGame);

        buttons->addWidget(pauseBtn);
        buttons->addWidget(restartBtn);
        buttons->addWidget(exitBtn);

        mainLayout->addLayout(buttons);

        stack->addWidget(gameScreen);
    }

    /* ================= GAME OVER SCREEN ================= */
    {
        QVBoxLayout *layout = new QVBoxLayout(gameOverScreen);

        QLabel *over = new QLabel("GAME OVER");
        over->setAlignment(Qt::AlignCenter);
        over->setStyleSheet("font-size: 24px;");

        QPushButton *restartBtn = new QPushButton("RESTART");
        QPushButton *exitBtn = new QPushButton("EXIT");

        connect(restartBtn, &QPushButton::clicked, this, &GameWindow::restartGame);
        connect(exitBtn, &QPushButton::clicked, this, &GameWindow::exitGame);

        layout->addWidget(over);
        layout->addWidget(restartBtn);
        layout->addWidget(exitBtn);

        stack->addWidget(gameOverScreen);
    }

    /* ================= TIMER ================= */
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWindow::updateGame);

    stack->setCurrentWidget(startScreen);
}

/* ================= START GAME ================= */

void GameWindow::startGame()
{
    game.startGame();

    seconds = 0;
    timer->start(1000);

    stack->setCurrentWidget(gameScreen);
}

/* ================= PAUSE GAME ================= */

void GameWindow::pauseGame()
{
    game.pauseGame();
    timer->stop();
}

/* ================= RESTART GAME ================= */

void GameWindow::restartGame()
{
    game.restartGame();

    seconds = 0;
    timer->start(1000);

    stack->setCurrentWidget(gameScreen);
}

/* ================= EXIT GAME ================= */

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
        "Score: " + QString::number(game.getPlayer().getScore())
        );

    statusLabel->setText(
        (game.getstate() == Gamestate::playing) ? "ALIVE" : "PAUSED"
        );
}

/* ================= KEYBOARD ================= */

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    if (stack->currentWidget() != gameScreen)
        return;

    switch (event->key())
    {
    case Qt::Key_W: game.getPlayer().moveUp(); break;
    case Qt::Key_S: game.getPlayer().moveDown(); break;
    case Qt::Key_A: game.getPlayer().moveLeft(); break;
    case Qt::Key_D: game.getPlayer().moveRight(); break;

    case Qt::Key_Escape:
        pauseGame();
        stack->setCurrentWidget(startScreen);
        break;
    }
}
