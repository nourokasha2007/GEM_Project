#ifndef CHARACTER_H
#define CHARACTER_H

class Character
{
protected:
    int health;

public:
    Character();
    virtual ~Character() = default;

    int getHealth() const;
    virtual void takeDamage(int amount);
    bool isDead() const { return health <= 0; }
};

#endif // CHARACTER_H

