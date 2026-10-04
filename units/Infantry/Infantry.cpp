#include "units/Infantry/Infantry.h"

Infantry::Infantry(int unitHealth, int unitArmor, int unitAttack, int unitX, int unitY) 
    : Unit(unitHealth, unitArmor, unitAttack, unitX, unitY) {}

std::string Infantry::getCategory() const {
    return "Infantry";
}
