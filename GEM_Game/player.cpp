#include "player.h"

using namespace std;

Player::Player(QString name, int startX, int startY)
{
    x = startX;
    y = startY;
    score = 0;
    health = 100; // Start with full health
    hintsUsed = 0;
    currentLocation = name;
}

// Health
int Player::getHealth() const {
    return health;
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0; // Prevent health from going below zero
}

// Score
int Player::getScore() const {
    return score;
}

void Player::addScore(int points) {
    score += points;
}

void Player::deductScore(int points) {
    score -= points;
    if (score < 0) score = 0;
}

void Player::reset()
{
    x = 100;
    y = 600;
    score = 0;
    health = 100; // Reset health when restarting the game
    hintsUsed = 0;
    foundItems.clear();
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

vector<QString> Player::getFoundItems() const {
    return foundItems;
}

bool Player::isWinner() const {
    return score >= 100;
}

void Player::moveTo(int newX, int newY) {
    x = newX;
    y = newY;
}

void Player::moveUp() { y -= 5; }
void Player::moveDown() { y += 5; }
void Player::moveLeft() { x -= 5; }
void Player::moveRight() { x += 5; }

void Player::collectItem(QString item) {
    foundItems.push_back(item);
}

void Player::interactCharacter(QString character) {
    currentLocation = character;
}

void Player::useHint() {
    hintsUsed++;
}

// ... rest of your existing player.cpp code ...
