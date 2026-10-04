#include "factories/CavalryFactory.h"
#include <stdexcept>
#include "units/Cavalry/Lightcavalry.h"
#include "units/Cavalry/Heavycavalry.h"

std::unique_ptr<Unit> CavalryFactory::createUnit(UnitKind kind, int x, int y) const {
    switch (kind) {
        case UnitKind::Light: {
            return std::make_unique<Lightcavalry>(x, y);
        }
        case UnitKind::Heavy: {
            return std::make_unique<Heavycavalry>(x, y);
        }
    }

    throw std::logic_error("CavalryFactory -> createUnit: unknown UnitKind");
}
