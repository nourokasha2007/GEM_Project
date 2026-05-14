#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "level.h"
#include "inventory.h"

#include <QString>

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

    //================ CORE =================//

    Game();

    ~Game();

    //================ GAME =================//

    void startGame();

    void pauseGame();

    void resumeGame();

    void restartGame();

    void exitGame();

    void update(float diffTime);

    //================ LEVELS =================//

    void loadLevel(int levelindex);

    void nextLevel();

    Level* getCurrentLevel() const;

    int getLevelIndex() const;

    //================ PLAYER =================//

    Player& getPlayer();

    //================ INVENTORY =================//

    void collectArtifact(QString type);

    int getArtifactCount(QString type) const;

    bool hasCollected(QString type) const;

    //================ GAME STATE =================//

    void checkWin();

    void checkLose();

    Gamestate getstate() const;

private:

    //================ CORE =================//

    Player player;

    Inventory inventory;

    Level* currentLevel;

    //================ STATE =================//

    Gamestate state;

    bool isRunning;

    //================ TIMER =================//

    float timer;

    float startTime;

    //================ LEVEL =================//

    int currentLevelIndex;

    //================ COUNTS =================//

    int coinCount;

    int scrollCount;

    int maskCount;

    int amuletCount;

    int timerCount;
};

#endif
