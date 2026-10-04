#pragma once

class Health {
private:
    int max_;
    int cur_;

public:
    explicit Health(int unitHealth);

    void decrease(int amount); // получить урон
    void increase(int amount); // исцелиться
    bool isZero() const; // проверка на 0hp

    int currentHealth() const; // получить текущее количество hp
    int maxHealth() const; // получить максимальное количество hp
};
