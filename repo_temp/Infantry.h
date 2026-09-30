#ifndef INFANTRY_H
#define INFANTRY_H

#include "Unit.h"

// ============================================================================
// [OOP CONCEPT: Inheritance & Polymorphism]
// Frontline Tank Operative: High health, close-quarters combat.
// Provides a concrete implementation of Unit for grunts/tanks.
// ============================================================================
class Infantry : public Unit {
public:
    Infantry(const std::string& unitName, int team, int hp = 100, int dmg = 20, Vector3i pos = Vector3i(0, 0, 0))
        : Unit(unitName, team, hp, dmg, 1, pos) {}

    virtual void useUltimate(const std::vector<Unit*>& /*allUnits*/, const Vector3i& /*targetParam*/ = Vector3i()) override {
        spendAP(ULTIMATE_AP_COST);
        std::cout << "\n[ULTIMATE] " << name << " activates Shield Fortification / Taunt!\n";
    }
};

#endif // INFANTRY_H
