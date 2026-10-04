#pragma once
#include <memory>   // std::unique_ptr
#include <string>   // std::string
#include "attributes/Armor.h"
#include "attributes/Attack.h"
#include "attributes/Health.h"

// абстрактный класс для всех юнитов игры
class Unit {
private:
    // общие параметры всех юнитов. private: наследники не трогают их напрямую,
    // а передают значения через конструктор Unit.
    Health health_;
    Armor armor_;
    Attack attack_;

    // позиция юнита на карте
    int x_;
    int y_;

protected:
    // конструктор классам могут вызвать только наследники
    Unit(int unitHealth, int unitArmor, int unitAttack, int unitX, int unitY);

public:
    // = default: генерация обычного деструктора, который будет удалять все созданное автоматически
    // virtual: возможность сделать так, чтобы поле хранило юнитов как указатель на Unit
    // на самом же деле там будет лежать другой класс
    
    // из-за того, что класс абстрактный, то наследники будут удаляться через указатель на этот класс
    // поэтому деструктор виртуальный
    virtual ~Unit() = default;

    void takeDamage(int damage); // обработка урона
    bool attack(Unit& target); // обработка атки
    void move(int newX, int newY); // обработка движения

    bool isAlive() const; // проверка здоровья юнита

    int health() const; // получить текущее количество hp
    int maxHealth() const; // получить максимальное количество hp
    int armor() const; // получить количество брони
    int attackPower() const; // получить значение атаки юнита
    int x() const; // получить x координату
    int y() const; // получить y координату

    // = 0: означает, что реализацию каждый наследник делает самостоятельно
    // возвращает тип юнита
    virtual std::string getType() const = 0;
    
    // возвращается класс юнита
    virtual std::string getCategory() const = 0;

    // = 0: означает, что реализацию каждый наследник делает самостоятельно
    // когда указатель уничтожается, он сам вызывает delete, поэтому утечек нет
    // std::unique_ptr<Unit>: нужен для глубого копирования поля, то есть копирования юнитов 
    // поле видит только Unit* и не знает, какой объект создавать
    // Через virtual вызовется clone() нужного класса, и тот создаст копию правильного типа
    virtual std::unique_ptr<Unit> clone() const = 0;
};
