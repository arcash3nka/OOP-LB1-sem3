#include "units/Infantry/Tank.h"

namespace {
    // constexpr: константа, есть тип, известна при компиляции
    // namespace { } без имени: видно только внутри этого файла
    constexpr int kHealth = 200;
    constexpr int kArmor = 50;
    constexpr int kAttack = 10;
}

Tank::Tank(int unitX, int unitY)
    : Infantry(kHealth, kArmor, kAttack, unitX, unitY) {}

std::string Tank::getType() const {
    return "Tank";
}

// make_unique — создаёт объект и сразу кладёт его в unique_ptr.
std::unique_ptr<Unit> Tank::clone() const {
    return std::make_unique<Tank>(*this);
}
