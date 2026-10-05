#include <algorithm> // std::count_if
#include <iostream>
#include <stdexcept>
#include <utility> // std::move
#include <limits> // std::numeric_limits — для очистки неправильного ввода

#include "factories/ArcherFactory.h"
#include "factories/CavalryFactory.h"
#include "factories/InfantryFactory.h"
#include "field/GameField.h"

namespace {
    // constexpr: константа, есть тип, известна при компиляции
    // namespace { } без имени: видно только внутри этого файла
    constexpr int kWidth = 6;
    constexpr int kHeight = 4;
    constexpr int kMaxUnits = 5;
    constexpr int kRainAttack = 120; // сила дождя стрел в сценариях 4 и 5
}

/*
По ТЗ необходимо сделать несколько сценариев для демонстрации работы ЛР
1. Создание поля + работы фабрики по наполнению юнитами поля
2. Глубокое копирования
3. Перемещение
4. Массовый урон всем юнитам (проверка итератора)
5. Удаление во время обхода (проверка устойчивости итератора)

Каждый сценарий создаёт СВОЁ поле, поэтому их можно запускать
в любом порядке и сколько угодно раз через меню
*/

// ===================== Вывод действий на экран =====================
// рисует поле сеткой
void printField(const GameField& field) {
    for (int y = 0; y < field.height(); ++y) {
        for (int x = 0; x < field.width(); ++x) {
            const Unit* unit = field.getUnit(x, y);
            std::cout << (unit ? unit->getType()[0] : '.') << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "Юнитов: " << field.size() << " / " << field.maxUnits() << "\n";
}

// вывод списка юнитов
// GameField& без const: begin()/end() у поля не const
void printUnits(GameField& field) {
    // проходит всех юнитов поля по очереди, при этом просто читает их и не может менять
    for (const Unit& unit : field) {
        std::cout << "  " << unit.getType() << " (" << unit.getCategory() << ")"
                  << " в (" << unit.x() << ", " << unit.y() << ")"
                  << "  HP " << unit.health() << "/" << unit.maxHealth()
                  << "  броня " << unit.armor()
                  << "  атака " << unit.attackPower() << "\n";
    }
}

void printTitle(const char* title) {
    std::cout << "\n========== " << title << " ==========\n";
}

// ===================== Подготовка =====================
// собирает стандартную армию через фабрики без вывода на экран
// каждый сценарий начинает со своего свежего поля
GameField buildArmy() {
    InfantryFactory infantry;
    ArcherFactory archers;
    CavalryFactory cavalry;

    GameField field(kWidth, kHeight, kMaxUnits);

    field.spawnUnit(infantry, UnitKind::Light, 0, 0); // Swordsman
    field.spawnUnit(infantry, UnitKind::Heavy, 1, 0); // Tank
    field.spawnUnit(archers, UnitKind::Light, 5, 0); // Bowman
    field.spawnUnit(archers, UnitKind::Heavy, 5, 3); // Crossbowman
    field.spawnUnit(cavalry, UnitKind::Heavy, 2, 2); // Heavycavalry

    return field;
}

// дождь стрел: атака по каждому юниту поля с учётом его брони
void arrowRain(GameField& field) {
    Attack rain(kRainAttack);

    // проходит всех юнитов поля и наносит каждому урон
    for (Unit& unit : field) {
        unit.takeDamage(rain.calculateDamage(Armor(unit.armor())));
    }
}

// ===================== Сценарии =====================
// 1. Создание поля + работы фабрики по наполнению юнитами поля
void scenCreateField() {
    printTitle("1. Создание поля + работы фабрики");

    InfantryFactory infantry;
    ArcherFactory archers;
    CavalryFactory cavalry;

    GameField field(kWidth, kHeight, kMaxUnits);

    field.spawnUnit(infantry, UnitKind::Light, 0, 0); // Swordsman
    field.spawnUnit(infantry, UnitKind::Heavy, 1, 0); // Tank
    field.spawnUnit(archers, UnitKind::Light, 5, 0); // Bowman
    field.spawnUnit(archers, UnitKind::Heavy, 5, 3); // Crossbowman
    field.spawnUnit(cavalry, UnitKind::Heavy, 2, 2); // Heavycavalry

    printField(field);
    printUnits(field);

    // проверки и вывод информации
    std::cout << "\nКлетка (0,0) занята -> spawnUnit вернул "
              << std::boolalpha << field.spawnUnit(cavalry, UnitKind::Light, 0, 0) << "\n";
    std::cout << "Лимит " << kMaxUnits << " юнитов достигнут -> spawnUnit вернул "
              << field.spawnUnit(cavalry, UnitKind::Light, 3, 3) << "\n";

    try {
        field.spawnUnit(cavalry, UnitKind::Light, 10, 10);
    } catch (const std::out_of_range& e) {
        std::cout << "Вне поля -> исключение: " << e.what() << "\n";
    }

    std::cout << "\nДвигаем Heavycavalry (2,2) -> (3,1):\n";
    field.moveUnit(2, 2, 3, 1);
    std::cout << "\n";
    printField(field);
}

// 2. Глубокое копирования
void scenCopy() {
    printTitle("2. Глубокое копирование");

    GameField original = buildArmy();
    GameField copy(original); // конструктор копирования -> clone() каждого юнита

    // бьём всех в КОПИИ и удаляем одного юнита из КОПИИ
    for (Unit& unit : copy) {
        unit.takeDamage(40);
    }
    copy.removeUnit(0, 0);

    std::cout << "Копия (всем -40 HP, Swordsman удалён):\n";
    printField(copy);
    printUnits(copy);

    std::cout << "\nОригинал не пострадал:\n";
    printField(original);
    printUnits(original);

    // адреса разные -> это разные объекты, а не общий юнит
    std::cout << "\nЮнит (1,0) в оригинале: " << original.getUnit(1, 0)
              << ", в копии: " << copy.getUnit(1, 0) << "\n";

    // присваивание копированием (copy-and-swap)
    GameField assigned(2, 2, 1);
    assigned = original;
    std::cout << "После assigned = original: юнитов в assigned " << assigned.size() << "\n";
}

// 3. Перемещение
void scenMove() {
    printTitle("3. Перемещение поля");

    GameField source = buildArmy();
    const Unit* before = source.getUnit(1, 0); // запоминаем адрес юнита до перемещения

    GameField moved(std::move(source)); // конструктор перемещения

    std::cout << "После GameField moved(std::move(source)):\n";
    std::cout << "  source: " << source.size() << " юнитов, размер "
              << source.width() << "x" << source.height() << "\n";
    std::cout << "  moved:  " << moved.size() << " юнитов\n";
    std::cout << "  юнит (1,0) тот же самый объект (адрес не изменился): "
              << std::boolalpha << (moved.getUnit(1, 0) == before) << "\n";

    GameField target(3, 3, 3);
    target = std::move(moved); // присваивание перемещением
    std::cout << "После target = std::move(moved): в target " << target.size()
              << ", в moved " << moved.size() << "\n";
    std::cout << "\n";
    printField(target);
}

// 4. Массовый урон всем юнитам (проверка итератора)
void scenIterate() {
    printTitle("4. Обход итератором: массовое действие");

    GameField field = buildArmy();
    std::cout << "До:\n";
    printUnits(field);

    std::cout << "\nДождь стрел: атака " << kRainAttack
              << " по каждому юниту (с учётом его брони)\n";
    arrowRain(field);
    printUnits(field);

    // итератор совместим со STL — работает std::count_if
    // проходит всех юнитов поля и считает, сколько из них живы
    auto alive = std::count_if(field.begin(), field.end(),
                               [](const Unit& unit) { return unit.isAlive(); });
    std::cout << "Живых (std::count_if): " << alive << " из " << field.size() << "\n";
}

// 5. Удаление во время обхода (проверка устойчивости итератора)
void scenRemoveWhileIterating() {
    printTitle("5. Удаление погибших во время обхода");

    GameField field = buildArmy();
    arrowRain(field); // сначала кого-то убиваем, иначе удалять будет некого

    std::cout << "После дождя стрел:\n";
    printUnits(field);
    std::cout << "\n";

    // проходит всех юнитов поля и удаляет мёртвых
    // сначала сдвигаем итератор (it++ вернёт старую позицию), потом удаляем —
    // так мы никогда не обращаемся к уже удалённому юниту
    for (auto it = field.begin(); it != field.end();) {
        auto current = it++;
        if (!current->isAlive()) {
            std::cout << "  убираем " << current->getType() << " из ("
                      << current->x() << ", " << current->y() << ")\n";
            field.removeUnit(current->x(), current->y());
        }
    }

    std::cout << "\n";
    printField(field);
    printUnits(field);
}

// ===================== Меню =====================
// очистка экрана: ANSI-команда терминалу
// 2J — очистить экран, H — курсор в начало
void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void printMenu() {
    std::cout << "\n--------------------------------------------\n"
              << "1 - создание поля через фабрики\n"
              << "2 - глубокое копирование\n"
              << "3 - перемещение\n"
              << "4 - обход итератором\n"
              << "5 - удаление во время обхода\n"
              << "0 - выход\n"
              << "Выберите сценарий и нажмите Enter: ";
}

// ===================== main =====================
int main() {
    clearScreen();

    // бесконечный цикл меню, выход только по 0
    while (true) {
        printMenu();

        int choice = 0;
        if (!(std::cin >> choice)) {
            // ввод закончился (Ctrl+D)
            if (std::cin.eof()) {
                return 0;
            }

            // ввели не число: сбрасываем ошибку потока и выкидываем строку
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            std::cout << "Нужно ввести число от 0 до 5\n";
            continue;
        }

        clearScreen();
        switch (choice) {
            case 1: scenCreateField(); break;
            case 2: scenCopy(); break;
            case 3: scenMove(); break;
            case 4: scenIterate(); break;
            case 5: scenRemoveWhileIterating(); break;
            case 0: return 0;
            default: std::cout << "Нет сценария " << choice << "\n"; break;
        }
    }
}
