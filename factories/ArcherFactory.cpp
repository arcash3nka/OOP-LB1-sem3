#include "factories/ArcherFactory.h"
#include <stdexcept>
#include "units/Archer/Bowman.h"
#include "units/Archer/Crossbowman.h"

std::unique_ptr<Unit> ArcherFactory::createUnit(UnitKind kind, int x, int y) const {
    switch (kind) {
        case UnitKind::Light: {
            return std::make_unique<Bowman>(x, y);
        }
        case UnitKind::Heavy: {
            return std::make_unique<Crossbowman>(x, y);
        }
    }

    throw std::logic_error("ArcherFactory -> createUnit: unknown UnitKind");
}
