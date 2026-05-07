#include "enemy.h"

Enemy::Enemy(int hp, int dmg, double spd)
    : QObject(), QGraphicsPixmapItem(), Character() {
    this->health = hp;
    this->damage = dmg;
    this->speed = spd;
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) {
        // Remove from game logic
        delete this;
    }
}
