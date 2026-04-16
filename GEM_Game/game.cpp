#include "game.h"
#include "player.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"
float startTime=0;
Gamestate state;
Game::Game() {}
void Game:: startGame(){
    state=Gamestate::playing;
    timer=startTime;
    player.reset();
    currentLevelIndex =1;
    loadLevel(currentLevelIndex);
    isRunning=true;

}
void Game::pauseGame(){
    state=Gamestate::paused;
}
void Game::resumeGame(){
    state=Gamestate::playing;
}
void Game::restartGame(){
    loadLevel(currentLevelIndex);
    timer=startTime;
    isRunning=true;
    state=Gamestate::playing;
    player.reset();
}
void Game:: exitGame(){
    state=Gamestate::exiting;
    isRunning=false;
}
void Game::update(float diffTime){
    if(state!= Gamestate::playing){
        return;
    }
    else{
        timer=timer+diffTime;
        //update movement of player,enemies, items later
        currentLevel.update();
    }
}
void Game::loadLevel(int levelindex){
    currentLevelIndex=levelindex;
    if(currentLevelIndex==1){
        currentLevel=Level1();
    }
    if(currentLevelIndex==2){
        currentLevel=Level2();
    }
    if(currentLevelIndex==3){
        currentLevel=Level3();
    }
        player.reset();

}
void Game::nextLevel(){

}
void Game:: checkWin(){

}
void checkLose();
void endGame();
Gamestate getstate(){
    return state;
}
Game::~Game(){}
