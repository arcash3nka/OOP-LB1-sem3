#include "factories/InfantryFactory.h"
#include <stdexcept>
#include "units/Infantry/Swordsman.h"
#include "units/Infantry/Tank.h"

std::unique_ptr<Unit> InfantryFactory::createUnit(UnitKind kind, int x, int y) const {
    switch (kind) {
        case UnitKind::Light: {
            return std::make_unique<Swordsman>(x, y);
        }
        case UnitKind::Heavy: {
            return std::make_unique<Tank>(x, y);
        }
    }

    throw std::logic_error("InfantryFactory -> createUnit: unknown UnitKind");
}
