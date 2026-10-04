#include "units/Cavalry/Lightcavalry.h"

namespace {
    // constexpr: константа, есть тип, известна при компиляции
    // namespace { } без имени: видно только внутри этого файла
    constexpr int kHealth = 200;
    constexpr int kArmor = 10;
    constexpr int kAttack = 20;
}

Lightcavalry::Lightcavalry(int unitX, int unitY)
    : Cavalry(kHealth, kArmor, kAttack, unitX, unitY) {}

std::string Lightcavalry::getType() const {
    return "Lightcavalry";
}

// make_unique — создаёт объект и сразу кладёт его в unique_ptr.
std::unique_ptr<Unit> Lightcavalry::clone() const {
    return std::make_unique<Lightcavalry>(*this);
}
