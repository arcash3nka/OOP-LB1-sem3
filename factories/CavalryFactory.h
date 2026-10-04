#pragma once
#include "factories/UnitFactory.h"

// фабрика конницы
class CavalryFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const override;
};
