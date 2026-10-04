#pragma once
#include "units/Unit.h"

// класс лучников, наследни Unit
// к классу лучников относятся лучники и арбалетчики
class Archer : public Unit{
protected:
    Archer(int unitHealth, int unitArmor, int unitAttack, int unitX, int unitY);

public:
    // overide: используется для того, чтобы переопределить метод, заданный в Unit
    std::string getCategory() const override;
};
