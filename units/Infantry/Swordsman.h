#pragma once
#include "units/Infantry/Infantry.h"

class Swordsman : public Infantry {
public:
    Swordsman(int unitX, int unitY);

    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};
