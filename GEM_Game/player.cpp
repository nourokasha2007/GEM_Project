#include "player.h"




using namespace std;

Player::Player(QString name, int startX, int startY)
{
    x = startX;
    y = startY;
    score = 0;
    hintsUsed = 0;
    currentLocation = name;
}

void Player::moveTo(int newX, int newY)
{
    x = newX;
    y = newY;
}
void Player::moveUp()
{
    y--;
}

void Player::moveDown()
{
    y++;
}

void Player::moveLeft()
{
    x--;
}

void Player::moveRight()
{
    x++;
}
void Player::collectItem(QString item)
{
    foundItems.push_back(item);
}

/*void Player::interactCharacter(QString character)
{
}
*/
void Player::useHint()
{
    hintsUsed++;
}

void Player::addScore(int points)
{
    score += points;
}

void Player::reset()
{
    x = 100;
    y = 600;
    score = 0;
    hintsUsed = 0;
    foundItems.clear();
}

int Player::getScore() const
{
    return score;
}

int Player::getX() const
{
    return x;
}

int Player::getY() const
{
    return y;
}

vector<QString> Player::getFoundItems() const
{
    return foundItems;
}

bool Player::isWinner() const
{
    return score >= 100;
}
void Player::deductScore(int points)
{
    score -= points;

    if(score < 0)
        score = 0;
}
