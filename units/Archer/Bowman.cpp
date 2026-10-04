#include "units/Archer/Bowman.h"

namespace {
    // constexpr: константа, есть тип, известна при компиляции
    // namespace { } без имени: видно только внутри этого файла
    constexpr int kHealth = 100;
    constexpr int kArmor = 5;
    constexpr int kAttack = 30;
}

Bowman::Bowman(int unitX, int unitY)
    : Archer(kHealth, kArmor, kAttack, unitX, unitY) {}

std::string Bowman::getType() const {
    return "Bowman";
}

// make_unique — создаёт объект и сразу кладёт его в unique_ptr.
std::unique_ptr<Unit> Bowman::clone() const {
    return std::make_unique<Bowman>(*this);
}
