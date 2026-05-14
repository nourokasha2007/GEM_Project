/*
#include "enemy.h"
// #include "player.h"


// Enemy passes x, y, and speed UP to the Character class!
Enemy::Enemy(int startX, int startY, int startSpeed, int startDamage)
    : Character(startX, startY, startSpeed)
{
    damageValue = startDamage;
}

// Destructor
Enemy::~Enemy() {
}

// Getter
int Enemy::getDamage() const {
    return damageValue;
}

// Setter
void Enemy::setDamage(int newDamage) {
    damageValue = newDamage;
}

// Collision Logic

#include "player.h"
bool Enemy::checkCollision(const Player& targetPlayer) const {

    // Placeholder for Ahmed's logic later:
    // if (this->x == targetPlayer.getX() && this->y == targetPlayer.getY())
    //{
    //     return true;
    // }

    return false;
}
*/
#include "enemy.h"

Enemy::Enemy(int hp, int dmg, double spd)
    : QObject(), QGraphicsPixmapItem()
{
    this->health = hp;
    this->damage = dmg;
    this->speed = spd;
}

void Enemy::takeDamage(int amount)
{
    health -= amount;
    if (health <= 0) {
        // Beginner-friendly + Qt-safe: delete later instead of deleting immediately.
        // This avoids crashes caused by use-after-free / double delete.
        deleteLater();
    }
}


int Enemy::getDamage() const
{
    return damage;
}
