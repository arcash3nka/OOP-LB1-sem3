#pragma once
#include "units/Archer/Archer.h"

class Bowman : public Archer {
public:
    Bowman(int unitX, int unitY);

    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};
