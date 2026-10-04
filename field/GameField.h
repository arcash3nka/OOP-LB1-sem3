#pragma once
#include <memory>
#include <vector>
#include "factories/UnitFactory.h"
#include "units/Unit.h"

// поле владеет юнитами через указатели на них
class GameField {
private:   
    int width_;
    int height_;
    int maxUnits_;
    int unitCount_;
    
    // сетка в одном векторе: клетка (x, y) лежит по индексу y * width + x
    // пустая клетка = nullptr.
    std::vector<std::unique_ptr<Unit>> cells_;

    // вспомогательные методы
    // нужны только полю, поэтому в private
    int index (int x, int y) const; // получение индекса
    bool inBounds(int x, int y) const; // проверка внутри ли поля юнит
    void checkBounds(int x, int y) const; // если нет - исключение

public:
    GameField(int width, int height, int maxUnits);


    // &: ссылка на объект, который продолжит жить
    // &&: ссылка на объект, который сейчас исчезнет
    // конструктор копирования нужно писать самостоятельно, потому что unique_ptr нельзя копировать
    // владелец может быть только один
    // 1. конструктор копирования
    GameField(const GameField& other);
    // 2. присваивание копированием
    GameField& operator=(const GameField& other); 
    // noexcept: гарантия того, что не произойдет ошибки
    // 3. конструктор перемещения
    GameField(GameField&& other) noexcept; 
    // 4. присваивание перемещением
    GameField& operator=(GameField&& other) noexcept; 
    // 5. деструктор
    ~GameField(); 

    // добавление юнита
    // если клетка занята, то false
    bool addUnit(std::unique_ptr<Unit> unit);

    // Создать юнита через фабрику и сразу поставить. Поле не знает конкретных классов.
    bool spawnUnit(const UnitFactory& factory, UnitKind kind, int x, int y);

    bool removeUnit(int x, int y); // false — клетка была пуста
    bool moveUnit(int fromX, int fromY, int toX, int toY); // false — откуда пусто / куда занято

    // посмотреть, кто в клетке
    // nullptr — клетка пуста.
    const Unit* getUnit(int x, int y) const;

    int width() const;
    int height() const;
    int maxUnits() const;
    int size() const; // сколько юнитов сейчас
    bool empty() const;
    void clear(); // очистить юнитов
};
