#include "units/Archer/Crossbowman.h"

namespace {
    // constexpr: константа, есть тип, известна при компиляции
    // namespace { } без имени: видно только внутри этого файла
    constexpr int kHealth = 100;
    constexpr int kArmor = 10;
    constexpr int kAttack = 50;
}

Crossbowman::Crossbowman(int unitX, int unitY)
    : Archer(kHealth, kArmor, kAttack, unitX, unitY) {}

std::string Crossbowman::getType() const {
    return "Crossbowman";
}

// make_unique — создаёт объект и сразу кладёт его в unique_ptr.
std::unique_ptr<Unit> Crossbowman::clone() const {
    return std::make_unique<Crossbowman>(*this);
}
