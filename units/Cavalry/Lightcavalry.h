#pragma once
#include "units/Cavalry/Cavalry.h"

class Lightcavalry : public Cavalry {
public:
    Lightcavalry(int unitX, int unitY);

    std::string getType() const override;
    std::unique_ptr<Unit> clone() const override;
};
