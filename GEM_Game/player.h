#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

#include <vector>

class Player
{
private:

    //================ POSITION =================//

    int x;

    int y;

    //================ PLAYER DATA =================//

    int score;

    int hintsUsed;

    QString currentLocation;

    std::vector<QString> foundItems;

public:

    //================ CONSTRUCTOR =================//

    Player(
        QString name,
        int startX,
        int startY
        );

    //================ MOVEMENT =================//

    void moveTo(
        int newX,
        int newY
        );

    void moveUp();

    void moveDown();

    void moveLeft();

    void moveRight();

    //================ INVENTORY =================//

    void collectItem(
        QString item
        );

    std::vector<QString> getFoundItems() const;

    //================ SCORE =================//

    void addScore(
        int points
        );

    void deductScore(
        int points
        );

    int getScore() const;

    //================ POSITION GETTERS =================//

    int getX() const;

    int getY() const;

    //================ HINTS =================//

    void useHint();

    //================ GAME =================//

    bool isWinner() const;

    void reset();
};

#endif
