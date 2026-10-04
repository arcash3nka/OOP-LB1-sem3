#pragma once

class Armor {
private:
    int value_;

public:
    explicit Armor(int unitArmor);

    int reduceDamage(int damage) const; // уменьшение урона на величину брони

    int armor() const; // получить текущее количество брони
};
