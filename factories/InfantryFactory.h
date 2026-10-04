#pragma once
#include "factories/UnitFactory.h"

// фабрика пехоты
class InfantryFactory : public UnitFactory {
public:
    std::unique_ptr<Unit> createUnit(UnitKind kind, int x, int y) const override;
};
