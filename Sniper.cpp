#include "Sniper.h"
#include <iostream>

Sniper::Sniper(const std::string& unitName, int team, Vector3i pos)
    // Low Health (70), High Base Damage (35), Extreme Range (6 tiles)
    : Unit(unitName, team, 70, 35, 6, pos) {}

bool Sniper::piercingShot(Vector3i direction, int maxDistance, const std::vector<Unit*>& allUnits) {
    if (!isAlive()) {
        std::cout << "[Ultimate Error] " << name << " is down and cannot fire!\n";
        return false;
    }

    // Direction must not be zero vector
    if (direction.x == 0 && direction.y == 0 && direction.z == 0) {
        std::cout << "[Ultimate Error] Direction vector cannot be zero!\n";
        return false;
    }

    // 1. AP Check: Ultimate costs 3 AP
    if (!canSpendAP(ULTIMATE_AP_COST)) {
        return false;
    }

    // Deduct the 3 AP
    spendAP(ULTIMATE_AP_COST);

    int ultDamage = baseDamage * 3; // 3x Damage multiplier
    std::cout << "\n======================================================\n";
    std::cout << "[ULTIMATE ACTIVATION] " << name << " casts PIERCING SHOT!\n";
    std::cout << "  Firing along trajectory ";
    direction.print();
    std::cout << " for up to " << maxDistance << " tiles. (Damage: " << ultDamage << ")\n";
    std::cout << "======================================================\n";

    int unitsHit = 0;
    Vector3i currentRayPos = position;

    // Line-tracing along the grid
    for (int step = 1; step <= maxDistance; ++step) {
        currentRayPos = currentRayPos + direction;
        std::cout << "  Tracing tile " << step << " at ";
        currentRayPos.print();
        std::cout << " ...\n";

        // Check if any unit is standing on this grid tile
        for (Unit* target : allUnits) {
            if (target && target->isAlive() && target->getPosition() == currentRayPos) {
                // Enemies (or any unit in path) caught in the high-velocity piercing round
                if (target->getTeamId() != teamId) {
                    std::cout << "  >>> Direct Hit! Enemy " << target->getName() 
                              << " pierced at ";
                    target->getPosition().print();
                    std::cout << " for 3x damage (" << ultDamage << ")!\n";
                    target->takeDamage(ultDamage);
                    unitsHit++;
                } else {
                    std::cout << "  >>> Piercing round passed ally " << target->getName() 
                              << " (Friendly fire protected).\n";
                }
            }
        }
    }

    std::cout << "[Piercing Shot Complete] Total enemies struck: " << unitsHit << "\n\n";
    return true;
}
