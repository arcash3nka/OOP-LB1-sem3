#include "units/Cavalry/Heavycavalry.h"

namespace {
    // constexpr: константа, есть тип, известна при компиляции
    // namespace { } без имени: видно только внутри этого файла
    constexpr int kHealth = 200;
    constexpr int kArmor = 30;
    constexpr int kAttack = 30;
}

Heavycavalry::Heavycavalry(int unitX, int unitY)
    : Cavalry(kHealth, kArmor, kAttack, unitX, unitY) {}

std::string Heavycavalry::getType() const {
    return "Heavycavalry";
}

// make_unique — создаёт объект и сразу кладёт его в unique_ptr.
std::unique_ptr<Unit> Heavycavalry::clone() const {
    return std::make_unique<Heavycavalry>(*this);
}
