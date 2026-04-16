#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Inventory.h"
#include "Level.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

enum class Gamestate{
    playing,
    paused,
    exiting
};

class Game
{
private:
    Gamestate state;
    bool isRunning;
    float timer;
    float startTime;
    Player player;
    int currentLevelIndex;
    Inventory inventory;
    Level* currentLevel;

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
    void collectItem(int index);
    void checkWin();
    void checkLose();

    Gamestate getstate() const;
    ~Game();
};

#endif
