#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <vector>

using namespace std;

class Player
{
private:
    int score;
    QString currentLocation;
    int x;
    int y;
    int hintsUsed;
    vector<QString> foundItems;

public:
    Player(QString name, int startX, int startY);

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
};

#endif
