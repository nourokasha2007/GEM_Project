#include "character.h"

Character::Character() {
    health = 100; // Default health for everyone
}

int Character::getHealth() const {
    return health;
}

void Character::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}
