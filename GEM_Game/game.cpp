#include "game.h"
#include "Level1.h"
#include "artifact.h"
#include "tools.h"

Artifact coin("Coin", false);
Artifact amulet("Amulet", false);
Artifact scroll("Ancient Scroll", false);
Artifact mask("Golden Mask", false);
Artifact nose("Abu El Hol Nose", false);

Tool timerBoost("Timer Boost", false, 1);

Game::Game() : player("Player", 100, 600)
{
    state = Gamestate::paused;
    currentLevel = new Level1();
    currentLevelIndex = 1;
    isRunning = false;
    timer = 0;
    startTime = 0;
}

Game::~Game()
{
    delete currentLevel;
}

void Game::startGame()
{
    state = Gamestate::playing;
    isRunning = true;
}

void Game::pauseGame()
{
    state = Gamestate::paused;
    isRunning = false;
}

void Game::resumeGame()
{
    state = Gamestate::playing;
    isRunning = true;
}

void Game::restartGame()
{
    player.reset();
    state = Gamestate::playing;
    isRunning = true;
    timer = 0;
}

void Game::exitGame()
{
    state = Gamestate::exiting;
    isRunning = false;
}

void Game::update(float diffTime)
{
    if (state == Gamestate::playing) {
        timer += diffTime;
    }
}

void Game::loadLevel(int levelindex)
{
    currentLevelIndex = levelindex;

    delete currentLevel;
    currentLevel = new Level1();
}

void Game::nextLevel()
{
    currentLevelIndex++;
}

void Game::checkWin()
{
    if (player.isWinner()) {
        state = Gamestate::gameOver;
    }
}

void Game::checkLose()
{

}

Gamestate Game::getstate() const
{
    return state;
}

Player& Game::getPlayer()
{
    return player;
}

int Game::getLevelIndex() const
{
    return currentLevelIndex;
}
