#include "game.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

#include <iostream>
using namespace std;

/* ================= CONSTRUCTOR ================= */

Game::Game()
    : player("Player", 650, 810)
{
    state             = Gamestate::paused;
    currentLevel      = new Level1();
    currentLevelIndex = 1;
    isRunning         = false;
    timer             = 0;
    startTime         = 0;

    coinCount   = 2;
    scrollCount = 2;
    maskCount   = 2;
    amuletCount = 2;
    timerCount  = 2;
}

/* ================= DESTRUCTOR ================= */

Game::~Game()
{
    delete currentLevel;
}

/* ================= START ================= */

void Game::startGame()
{
    state     = Gamestate::playing;
    isRunning = true;
}

/* ================= PAUSE ================= */

void Game::pauseGame()
{
    state     = Gamestate::paused;
    isRunning = false;
}

/* ================= RESUME ================= */

void Game::resumeGame()
{
    state     = Gamestate::playing;
    isRunning = true;
}

/* ================= RESTART ================= */

void Game::restartGame()
{
    player.reset();
    state     = Gamestate::playing;
    isRunning = true;
    timer     = 0;

    coinCount   = 2;
    scrollCount = 2;
    maskCount   = 2;
    amuletCount = 2;
    timerCount  = 2;
}

/* ================= EXIT ================= */

void Game::exitGame()
{
    state     = Gamestate::exiting;
    isRunning = false;
}

/* ================= UPDATE ================= */

void Game::update(float diffTime)
{
    if (state != Gamestate::playing)
        return;

    timer += diffTime;

    checkWin();
    checkLose();
}

/* ================= LOAD LEVEL ================= */

void Game::loadLevel(int levelindex)
{
    currentLevelIndex = levelindex;
    delete currentLevel;

    if (levelindex == 1)
        currentLevel = new Level1();
    else if (levelindex == 2)
        currentLevel = new Level2();
    else if (levelindex == 3)
        currentLevel = new Level3();
    else
        currentLevel = new Level1();

    player.reset();
}

/* ================= NEXT LEVEL ================= */

void Game::nextLevel()
{
    currentLevelIndex++;
}

/* ================= COLLECT ARTIFACT ================= */

void Game::collectArtifact(QString type)
{
    player.addScore(10);
    player.collectItem(type);

    if      (type == "coin"   && coinCount   > 0) coinCount--;
    else if (type == "scroll" && scrollCount > 0) scrollCount--;
    else if (type == "mask"   && maskCount   > 0) maskCount--;
    else if (type == "amulet" && amuletCount > 0) amuletCount--;
    else if (type == "timer"  && timerCount  > 0) timerCount--;

    checkWin();
}

/* ================= GET COUNTS ================= */

int Game::getArtifactCount(QString type) const
{
    if (type == "coin")   return coinCount;
    if (type == "scroll") return scrollCount;
    if (type == "mask")   return maskCount;
    if (type == "amulet") return amuletCount;
    if (type == "timer")  return timerCount;
    return 0;
}

/* ================= HAS COLLECTED ================= */

bool Game::hasCollected(QString type) const
{
    vector<QString> items = player.getFoundItems();
    for (size_t i = 0; i < items.size(); i++)
        if (items[i] == type) return true;
    return false;
}

/* ================= WIN ================= */

void Game::checkWin()
{
    if (player.getScore() >= 150)
        state = Gamestate::gameOver;
}

/* ================= LOSE ================= */

void Game::checkLose()
{
    // Handled by timer countdown in GameWindow
}

/* ================= GETTERS ================= */

Gamestate Game::getstate()        const { return state; }
Player&   Game::getPlayer()             { return player; }
Level*    Game::getCurrentLevel() const { return currentLevel; }
int       Game::getLevelIndex()   const { return currentLevelIndex; }
