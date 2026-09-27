#ifndef SNIPER_H
#define SNIPER_H

#include "Unit.h"
#include <vector>

/**
 * @brief Sniper operative: High damage, extreme range, low health.
 * Specializes in long-distance combat and the Piercing Shot ultimate.
 */
class Sniper : public Unit {
public:
    Sniper(const std::string& unitName, int team, Vector3i pos);

    /**
     * @brief Ultimate: Piercing Shot (Costs 3 AP).
     * Fires a high-velocity round in a straight line that penetrates through 
     * multiple grid tiles, dealing 3x base damage to ALL units caught in its path.
     * 
     * @param direction Unit vector for firing direction (e.g., (1,0,0), (-1,0,0), (0,0,1))
     * @param maxDistance Number of tiles the bullet penetrates
     * @param allUnits List of all units on the battlefield to check for collisions
     * @return true if successfully fired
     */
    bool piercingShot(Vector3i direction, int maxDistance, const std::vector<Unit*>& allUnits);
};

#endif // SNIPER_H
