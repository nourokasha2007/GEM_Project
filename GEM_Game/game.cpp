#include "Game.h"
#include <iostream>

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
    if (state != Gamestate::playing){
        return;}else{
        timer += diffTime;}
    if (currentLevel)
        currentLevel->update();
    checkWin();
    checkLose();
}

void Game::loadLevel(int levelindex)
{
    // 🔥 prevent memory leak
    if (currentLevel)
    {
        delete currentLevel;
        currentLevel = nullptr;
    }

    currentLevelIndex = levelindex;

    if (currentLevelIndex == 1)
        currentLevel = new Level1();
    /*
    else if (currentLevelIndex == 2)
        currentLevel = new Level2();
    else if (currentLevelIndex == 3)
        currentLevel = new Level3();
    else
        currentLevel = new Level1();
    */
    player.reset();

    if (currentLevel)
        currentLevel->setup();

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
