#pragma once
#include "units/Unit.h"

// класс кавалерии, наследни Unit
// к классу кавалерии относятся легкая и тяжелая конница
class Cavalry : public Unit{
protected:
    Cavalry(int unitHealth, int unitArmor, int unitAttack, int unitX, int unitY);

public:
    // overide: используется для того, чтобы переопределить метод, заданный в Unit
    std::string getCategory() const override;
};
