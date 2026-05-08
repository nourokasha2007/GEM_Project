#include "player.h"

/* ================= CONSTRUCTOR ================= */

Player::Player(
    QString name,
    int startX,
    int startY
    )
{
    x =
        startX;

    y =
        startY;

    score =
        0;

    hintsUsed =
        0;

    currentLocation =
        name;
}

/* ================= MOVEMENT ================= */

void Player::moveTo(
    int newX,
    int newY
    )
{
    x =
        newX;

    y =
        newY;
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

/* ================= INVENTORY ================= */

void Player::collectItem(
    QString item
    )
{
    foundItems.push_back(item);
}

std::vector<QString> Player::getFoundItems() const
{
    return foundItems;
}

/* ================= SCORE ================= */

void Player::addScore(
    int points
    )
{
    score += points;
}

void Player::deductScore(
    int points
    )
{
    score -= points;

    if(score < 0)
    {
        score = 0;
    }
}

int Player::getScore() const
{
    return score;
}

/* ================= POSITION ================= */

int Player::getX() const
{
    return x;
}

int Player::getY() const
{
    return y;
}

/* ================= HINTS ================= */

void Player::useHint()
{
    hintsUsed++;
}

/* ================= GAME ================= */

bool Player::isWinner() const
{
    return score >= 100;
}

void Player::reset()
{
    x =
        635;

    y =
        700;

    score =
        0;

    hintsUsed =
        0;

    foundItems.clear();
}
