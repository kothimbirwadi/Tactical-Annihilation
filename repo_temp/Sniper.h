#ifndef SNIPER_H
#define SNIPER_H

#include "Unit.h"
#include <vector>

// ============================================================================
// [OOP CONCEPT: Inheritance (Derived Class)]
// Sniper inherits from abstract base class Unit.
// High damage, extreme range, low health.
// ============================================================================
class Sniper : public Unit {
private:
    int piercingRange;

public:
    // [OOP CONCEPT: Constructor in Derived Class with Base Constructor Call]
    Sniper(const std::string& unitName, int team, Vector3i pos = Vector3i(0, 0, 0));

    // [OOP CONCEPT: Method Overriding (Run-Time Polymorphism)]
    // Implements pure virtual function from Unit interface
    virtual void useUltimate(const std::vector<Unit*>& allUnits, const Vector3i& targetParam = Vector3i()) override;

    /**
     * @brief Dedicated Piercing Shot function.
     * Fires a high-velocity round in a straight line that penetrates through
     * multiple grid tiles, dealing 3x base damage to ALL enemy units in the line.
     */
    bool piercingShot(Vector3i direction, int maxDistance, const std::vector<Unit*>& allUnits);
};

#endif // SNIPER_H
