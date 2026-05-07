#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <vector>

using namespace std;

class Player
{
private:
    int health; // 1. Add health here
    int score;
    QString currentLocation;
    int x;
    int y;
    int hintsUsed;
    vector<QString> foundItems;

public:
    Player(QString name, int startX, int startY);

    // Health interaction with enemies/projectiles
    int getHealth() const;
    void takeDamage(int amount);


    void moveTo(int newX, int newY);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    void collectItem(QString item);
    void interactCharacter(QString character);
    void useHint();
    void addScore(int points);

    void reset();

    int getScore() const;
    int getX() const;
    int getY() const;
    vector<QString> getFoundItems() const;

    bool isWinner() const;
    void deductScore(int points);
};

#endif