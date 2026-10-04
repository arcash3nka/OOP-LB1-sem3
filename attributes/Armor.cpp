#include "attributes/Armor.h"
#include <stdexcept>

Armor::Armor(int unitArmor) : value_(unitArmor) {
    if (unitArmor < 0) {
        throw std::invalid_argument("Armor->creator: unitArmor < 0");
    }
}

int Armor::reduceDamage(int damage) const {
    if (damage < 0) {
        throw std::invalid_argument("Armor->reduceDamage: damage < 0");
    }

    int passed = damage - value_;
    if (passed < 0) {
        return 0;
    }
    
    return passed;
}

int Armor::armor() const {
    return value_;
}
