#pragma once
#include "units/Archer/Archer.h"

class Crossbowman : public Archer {
public:
    Crossbowman(int unitX, int unitY);

    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};
