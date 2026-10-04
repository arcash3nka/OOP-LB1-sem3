#include "units/Infantry/Swordsman.h"

namespace {
    // constexpr: константа, есть тип, известна при компиляции
    // namespace { } без имени: видно только внутри этого файла
    constexpr int kHealth = 100;
    constexpr int kArmor = 5;
    constexpr int kAttack = 25;
}

Swordsman::Swordsman(int unitX, int unitY)
    : Infantry(kHealth, kArmor, kAttack, unitX, unitY) {}

std::string Swordsman::getType() const {
    return "Swordsman";
}

// make_unique — создаёт объект и сразу кладёт его в unique_ptr.
std::unique_ptr<Unit> Swordsman::clone() const {
    return std::make_unique<Swordsman>(*this);
}
