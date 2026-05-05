#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "level.h"

enum class Gamestate
{
    playing,
    paused,
    exiting,
    gameOver
};

class Game
{
public:
    Game();
    ~Game();

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

    Gamestate getstate() const;

    Player& getPlayer();
    int getLevelIndex() const;

    Level* getCurrentLevel() const { return currentLevel; }

private:
    Player player;

    Level* currentLevel;

    Gamestate state;

    float timer;
    float startTime;

    int currentLevelIndex;

    bool isRunning;
};

#endif
