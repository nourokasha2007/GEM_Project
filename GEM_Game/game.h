#ifndef GAME_H
#define GAME_H
#include "player.h"
#include "level.h"
enum class Gamestate{
    Menu,
    playing,
    paused,
    won,
    lost,
    exiting
};

class Game
{
private:
    Gamestate state;
    bool isRunning;
    float timer;
    player player;
    level currentLevel;
    int currentLevelIndex;

public:
    Game();
    void startGame();
    void pauseGame();
    void resumeGame();
    void restartGame();
    void exitGame();
    void update(float diffTime);
    void loadLevel(int levelindex);
    void nextLevel();
    void checkWin();
    void checkLose();
    void endGame();
    Gamestate getstate();
    ~Game();
};

#endif // GAME_H
