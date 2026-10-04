#pragma once
#include "units/Cavalry/Cavalry.h"

class Heavycavalry : public Cavalry {
public:
    Heavycavalry(int unitX, int unitY);

    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};
