#pragma once
#include <memory>
#include <vector>
#include <cstddef>    // std::size_t, std::ptrdiff_t
#include <iterator>   // std::forward_iterator_tag
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

    // ============== Класс итератора ==============
    // вложенный, потому что классу итератора нужно поле для обхода
    // + есть доступ к private
    // сам класс независим и по сути лежит как будто в папке, только в поле gamefield

    // по сути своей итератор - это закладка, которая бегает по полю
    // операторы сравнения также сравнивают не юнитов, а итераторы
    // по сути каждый итератор отвечает за КАЖДОЕ действие на поле
    // то есть если два юнита двигаются, то цикл будет создавать два итератора
    class Iterator {
    private:
        // конструктор private: создавать итераторы может только лишь поле через begin/end
        // friend: дает GameField доступ к private Iterator
        friend class GameField;
        Iterator(std::vector<std::unique_ptr<Unit>>*, std::size_t pos);

        void skipEmpty();
        std::vector<std::unique_ptr<Unit>>* data_; // список тех, на кого смотрим при проходе
        std::size_t pos_; // индекс

    public:
        //  для совместимости с STL
        // std::count_if и др: спрашивают у итератора, что он умеет и что выдаёт
        // using: обозначение типа данных. то есть вместо value_type можно писать Unit
        using iterator_category = std::forward_iterator_tag; // умеет только вперёд
        using value_type = Unit; // возвращает тип юнита
        using difference_type = std::ptrdiff_t; // возвращает количество шагов между двумя итераторами
        using pointer = Unit*; // что возвращает operator->
        using reference = Unit&; // что возвращает operator*

        // продолжение создания частей типа данных
        // тут задаются операторы для объектов класса Unit
        Unit& operator*() const; // *it  — юнит в текущей клетке
        Unit* operator->() const;  // it-> — обратиться к методу юнита
        Iterator& operator++();  // ++it — префиксный
        Iterator operator++(int); // it++ — постфиксный (int — просто метка)
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };
    
    Iterator begin(); // первый юнит
    Iterator end(); // позиция за последней клеткой
};
