#pragma once
#include "attributes/Armor.h"

class Attack {
private:
    int value_;

public:
    explicit Attack(int attackValue);

    int calculateDamage(const Armor& targetArmor) const; // вычисление получаемого урона

    int attack() const; // получить значение атаки
};
