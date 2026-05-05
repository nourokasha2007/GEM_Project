#include "game.h"
#include "Level1.h"
#include <iostream>
#include "artifact.h"
#include "tools.h"
using namespace std;

Game::Game() : player("Start", 0, 0)
{
    startTime = 0;
    timer = startTime;
    state = Gamestate::paused;
    currentLevelIndex = 1;
    isRunning = false;
    currentLevel = nullptr;
}

void Game::startGame()
{
    cout << "Game Started\n";

    state = Gamestate::playing;
    timer = startTime;

    player.reset();

    currentLevelIndex = 1;
    loadLevel(currentLevelIndex);

    isRunning = true;
}

void Game::pauseGame()
{
    cout << "Game Paused\n";
    state = Gamestate::paused;
}

void Game::resumeGame()
{
    cout << "Game Resumed\n";
    state = Gamestate::playing;
}

void Game::restartGame()
{
    cout << "Game Restarted\n";

    timer = startTime;
    player.reset();

    loadLevel(currentLevelIndex);

    state = Gamestate::playing;
    isRunning = true;
}

void Game::exitGame()
{
    cout << "Game Exiting\n";

    state = Gamestate::exiting;
    isRunning = false;
}

void Game::update(float diffTime)
{
    if (state != Gamestate::playing)
        return;

    timer += diffTime;

    // If your Level class later supports update, you can enable this:
    // if (currentLevel)
    //     currentLevel->update(diffTime);

    checkWin();
    checkLose();
}

void Game::loadLevel(int levelindex)
{
    if (currentLevel)
    {
        delete currentLevel;
        currentLevel = nullptr;
    }

    currentLevelIndex = levelindex;

    if (currentLevelIndex == 1)
        currentLevel = new Level1();
    else
        currentLevel = new Level1(); // fallback

    player.reset();

    cout << "Loaded Level " << levelindex << endl;
}

void Game::nextLevel()
{
    currentLevelIndex++;
    loadLevel(currentLevelIndex);
    timer = startTime;
}

void Game::checkWin()
{
    if (player.getScore() >= 100)
    {
        cout << "Level Complete!\n";
        nextLevel();
    }
}

void Game::checkLose()
{
    if (timer > 300)
    {
        cout << "You Lost!\n";
        restartGame();
    }
}

Gamestate Game::getstate() const
{
    return state;
}

Game::~Game()
{
    delete currentLevel;
}

Player& Game::getPlayer()
{
    return player;
}

int Game::getLevelIndex() const
{
    return currentLevelIndex;
}
Artifact coin("Coin", false);
Artifact statue("Statue", false);
Artifact scroll("Ancient Scroll", false);
Artifact mask("Golden Mask", false);
Artifact nose("Abu El Hol Nose", false);

Tool flashlight("Flashlight", false, 3);
Tool map("Map", false, 1);
Tool timerBoost("Timer Boost", false, 1);
