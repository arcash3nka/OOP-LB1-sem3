#pragma once
#include "units/Infantry/Infantry.h"

class Tank : public Infantry {
public:
    Tank(int unitX, int unitY);

    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};
