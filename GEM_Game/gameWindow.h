#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QPushButton>

#include "Game.h"

class GameWindow : public QMainWindow
{
    Q_OBJECT

private:
    Game game;

    QStackedWidget *stack;

    QWidget *startScreen;
    QWidget *gameScreen;
    QWidget *gameOverScreen;

    QLabel *clockLabel;
    QLabel *scoreLabel;
    QLabel *statusLabel;

    QTimer *timer;
    int seconds;

private:
    void startGame();
    void pauseGame();
    void restartGame();
    void exitGame();
    void updateGame();

protected:
    void keyPressEvent(QKeyEvent *event) override;

public:
    GameWindow(QWidget *parent = nullptr);
};

#endif
