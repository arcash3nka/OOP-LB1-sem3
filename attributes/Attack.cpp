#include "attributes/Attack.h"
#include <stdexcept>

Attack::Attack(int unitAttack) : value_(unitAttack) {
    if (unitAttack < 0) {
        throw std::invalid_argument("Attack->creator: unitAttack < 0");
    }
}

int Attack::calculateDamage(const Armor& targetArmor) const {
    return targetArmor.reduceDamage(value_);
}

int Attack::attack() const {
    return value_;
}
