#include "units/Cavalry/Cavalry.h"

Cavalry::Cavalry(int unitHealth, int unitArmor, int unitAttack, int unitX, int unitY) 
    : Unit(unitHealth, unitArmor, unitAttack, unitX, unitY) {}

std::string Cavalry::getCategory() const {
    return "Cavalry";
}
