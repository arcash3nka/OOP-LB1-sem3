#include "units/Archer/Archer.h"

Archer::Archer(int unitHealth, int unitArmor, int unitAttack, int unitX, int unitY) 
    : Unit(unitHealth, unitArmor, unitAttack, unitX, unitY) {}

std::string Archer::getCategory() const {
    return "Archer";
}
