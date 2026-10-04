#pragma once
#include <memory>   // std::unique_ptr
#include "units/Unit.h"

// вид юнита внутри типа войск: лёгкий или тяжёлый
// любое значение подходит любой фабрике
// enum class: позволяет избежать конфликта имен
enum class UnitKind {
    Light,   // Swordsman / Bowman / Lightcavalry
    Heavy    // Tank / Crossbowman / Heavycavalry
};

// паттерн «Фабричный метод»
// объявляет createUnit, а КОГО именно создавать решают наследники
class UnitFactory {
public:
    // virtual: фабрики будут использоваться через ссылку/указатель на UnitFactory
    // удаление через базовый указатель должно вызвать деструктор наследника
    virtual ~UnitFactory() = default;

    // = 0: этот метод каждый класс будет реализовывать самостоятельно
    // unique_ptr: вызывающий сразу становится единственным владельцем юнита
    virtual std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const = 0;
};
