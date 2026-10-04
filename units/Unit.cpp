#include "units/Unit.h"
#include <stdexcept>

Unit::Unit(int unitHealth, int unitArmor, int unitAttack, int unitX, int unitY)
    : health_(unitHealth),
    armor_(unitArmor),
    attack_(unitAttack),
    x_(unitX),
    y_(unitY) {
        if (unitX < 0 || unitY < 0) {
            throw std::invalid_argument("Unit->creator: x, y < 0");
        }
    }

void Unit::takeDamage(int damage) {
    health_.decrease(damage);
}
bool Unit::attack(Unit& target) {
    if (!isAlive() || !target.isAlive()) {
        return false;
    }

    int damage = attack_.calculateDamage(target.armor_);
    target.takeDamage(damage);
    
    return true;
}
void Unit::move(int newX, int newY) {
    if (newX < 0 || newY < 0) {
        throw std::invalid_argument("Unit::move: negative coordinates");
    }
    
    x_ = newX;
    y_ = newY;
}

bool Unit::isAlive() const {
    return !health_.isZero();
}
int Unit::health() const {
    return health_.currentHealth();
}
int Unit::maxHealth() const {
    return health_.maxHealth();
}

int Unit::armor() const {
    return armor_.armor();
}

int Unit::attackPower() const {
    return attack_.attack();
}

int Unit::x() const {
    return x_;
}

int Unit::y() const {
    return y_;
}
