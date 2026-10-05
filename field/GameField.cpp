#include "field/GameField.h"
#include <stdexcept>
#include <utility>   // std::move

// ================= Вспомогательные =================
int GameField::index(int x, int y) const {
    return y * width_ + x;
}

bool GameField::inBounds(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

void GameField::checkBounds(int x, int y) const {
    if (!inBounds(x, y)) {
        throw std::out_of_range("GameField: coordinates out of field");
    }
}

// ================= Конструктор =================
GameField::GameField(int width, int height, int maxUnits)
    : width_(width), height_(height), maxUnits_(maxUnits), unitCount_(0), cells_() {
        if (width <= 0 || height <= 0) {
            throw std::invalid_argument("GameField->creator: width and height must be > 0");
        }
        if (maxUnits <= 0) {
            throw std::invalid_argument("GameField->creator: maxUnits must be > 0");
        }
        
        // размер задаём ПОСЛЕ проверки
        // все клетки = nullptr
        cells_.resize(width * height);
    }

// ================= 5 пунктов =================
// 1. Глубокое копирование: каждому юниту своя копия через clone
GameField::GameField(const GameField& other) 
    : width_(other.width_),
      height_(other.height_),
      maxUnits_(other.maxUnits_),
      unitCount_(other.unitCount_),
      cells_(other.cells_.size()) {  // столько же пустых клеток
        for (std::size_t i = 0; i < other.cells_.size(); ++i) {
            if (other.cells_[i]) { // клетка не пустая
                cells_[i] = other.cells_[i]->clone(); // виртуальный вызов -> копия нужного типа
            }
        }
    }

// 2. Присваивание копированием
GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        GameField temp(other); // копирование во временный объект
        *this = std::move(temp); // забираем содержимое
    }

    return *this;
}

// 3. Конструктор перемещения: берем векторы, юнитов не трогаем
GameField::GameField(GameField&& other) noexcept
    : width_(other.width_),
      height_(other.height_),
      maxUnits_(other.maxUnits_),
      unitCount_(other.unitCount_),
      cells_(std::move(other.cells_)) {
        other.width_ = 0;
        other.height_ = 0;
        other.maxUnits_ = 0;
        other.unitCount_ = 0;
        other.cells_.clear();
    }

// 4. Присваивание перемещением
GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        maxUnits_ = other.maxUnits_;
        unitCount_ = other.unitCount_;
        cells_ = std::move(other.cells_);   // старые юниты *this удалятся автоматически

        other.width_ = 0;
        other.height_ = 0;
        other.maxUnits_ = 0;
        other.unitCount_ = 0;
        other.cells_.clear();
    }
    return *this;
}

// 5. Деструктор: unique_ptr в каждой клетке сам удалит своего юнита
GameField::~GameField() = default;

// ================= Работа с юнитами =================
bool GameField::addUnit(std::unique_ptr<Unit> unit) {
    if (!unit) {
        throw std::invalid_argument("GameField::addUnit: unit is null");
    }
    
    // вне поля — исключение
    checkBounds(unit->x(), unit->y());

    // лимит
    if (unitCount_ >= maxUnits_) {
        return false;
    }
    
    // & — ссылка на саму клетку в векторе, а не её копия
    std::unique_ptr<Unit>& cell = cells_[index(unit->x(), unit->y())];
    // клетка занята
    if (cell) {
        return false;
    }
    
    // поле стало владельцем
    cell = std::move(unit);
    ++unitCount_;
    
    return true;
}

bool GameField::spawnUnit(const UnitFactory& factory, UnitKind kind, int x, int y) {
    return addUnit(factory.createUnit(kind, x, y));
}

bool GameField::removeUnit(int x, int y) {
    checkBounds(x, y);
    
    std::unique_ptr<Unit>& cell = cells_[index(x, y)];
    // клетка свободна
    if (!cell) {
        return false;
    }
    
    // удаляет юнита, клетка становится nullptr
    cell.reset();
    --unitCount_;
    
    return true;
}

bool GameField::moveUnit(int fromX, int fromY, int toX, int toY) {
    checkBounds(fromX, fromY);
    checkBounds(toX, toY);

    std::unique_ptr<Unit>& from = cells_[index(fromX, fromY)];
    std::unique_ptr<Unit>& to = cells_[index(toX, toY)];
    // некого двигать или место занято
    if (!from || to) {
        return false;
    }
    
    from->move(toX, toY); // координаты в самом юните
    to = std::move(from); // и в сетке; from теперь nullptr
    
    return true;
}

const Unit* GameField::getUnit(int x, int y) const {
    checkBounds(x, y);
    return cells_[index(x, y)].get(); // .get() — обычный указатель, владение остаётся у поля
}

// ================= Прочее =================

int GameField::width() const { return width_; }
int GameField::height() const { return height_; }
int GameField::maxUnits() const { return maxUnits_; }
int GameField::size() const { return unitCount_; }
bool GameField::empty() const { return unitCount_ == 0; }

void GameField::clear() {
    for (std::unique_ptr<Unit>& cell : cells_) {
        cell.reset();
    }
    
    unitCount_ = 0;
}

// ================= Итератор =================
GameField::Iterator::Iterator(std::vector<std::unique_ptr<Unit>>* data, std::size_t pos)
    : data_(data), pos_(pos) {
    // сразу пропускаем пустые клетки
    skipEmpty();
}

void GameField::Iterator::skipEmpty() {
    // пока не дошли до конца И клетка пустая — шагаем дальше
    while (pos_ < data_->size() && !(*data_)[pos_]) {
        ++pos_;
    }
}

// (*data_)[pos_]: unique_ptr в клетке
// ещё одна * — сам юнит
Unit& GameField::Iterator::operator*() const {
    return *(*data_)[pos_];
}

Unit* GameField::Iterator::operator->() const {
    return (*data_)[pos_].get();
}

// ++it: сдвинулись, пропустили пустые клетки, вернули себя же
GameField::Iterator& GameField::Iterator::operator++() {
    ++pos_;
    skipEmpty();
    return *this;
}

// it++: запомнили старое положение, сдвинулись, вернули старую версию себя
GameField::Iterator GameField::Iterator::operator++(int) {
    Iterator old = *this;
    ++(*this);
    return old;
}

bool GameField::Iterator::operator==(const Iterator& other) const {
    return data_ == other.data_ && pos_ == other.pos_;
}

bool GameField::Iterator::operator!=(const Iterator& other) const {
    return !(*this == other);
}

GameField::Iterator GameField::begin() {
    // с начала (конструктор сам найдёт первого юнита)
    return Iterator(&cells_, 0);
}

GameField::Iterator GameField::end() {
    // за концом
    return Iterator(&cells_, cells_.size());
}
