#pragma once
#include "units/Unit.h"

// класс пехоты, наследни Unit
// к классу пехоты относятся мечники и палладины
class Infantry : public Unit{
protected:
    Infantry(int unitHealth, int unitArmor, int unitAttack, int unitX, int unitY);

public:
    // overide: используется для того, чтобы переопределить метод, заданный в Unit
    std::string getCategory() const override;
};
