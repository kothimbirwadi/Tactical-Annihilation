#include "Sniper.h"
#include <iostream>

// [OOP CONCEPT: Constructor Initialization List calling Base Constructor]
Sniper::Sniper(const std::string& unitName, int team, Vector3i pos)
    // Low Health (70), High Base Damage (35), Extreme Range (6 tiles)
    : Unit(unitName, team, 70, 35, 6, pos), piercingRange(5) {}

// [OOP CONCEPT: Run-Time Polymorphism / Dynamic Binding Implementation]
void Sniper::useUltimate(const std::vector<Unit*>& allUnits, const Vector3i& targetParam) {
    // targetParam represents firing direction (e.g., (1, 0, 0))
    Vector3i direction = targetParam;
    if (direction == Vector3i(0, 0, 0)) {
        direction = Vector3i(1, 0, 0); // Default direction: forward along X
    }
    piercingShot(direction, piercingRange, allUnits);
}

bool Sniper::piercingShot(Vector3i direction, int maxDistance, const std::vector<Unit*>& allUnits) {
    if (!isAlive()) {
        throw InvalidTargetException(name + " is knocked out and cannot fire ultimate!");
    }

    if (direction == Vector3i(0, 0, 0)) {
        throw InvalidTargetException("Sniper shot direction vector cannot be zero (0, 0, 0)!");
    }

    // Spend 3 AP for ultimate ability (throws OutOfAPException if insufficient AP)
    spendAP(ULTIMATE_AP_COST);

    int ultDamage = baseDamage * 3; // 3x Damage multiplier
    std::cout << "\n======================================================\n";
    std::cout << "[ULTIMATE: PIERCING SHOT] " << name << " charges railgun!\n";
    std::cout << "  Trajectory: " << direction << " | Max Penetration: " 
              << maxDistance << " tiles | Damage: " << ultDamage << "\n";
    std::cout << "======================================================\n";

    int unitsHit = 0;
    Vector3i currentRayPos = position;

    // Line-tracing along the 3D grid
    for (int step = 1; step <= maxDistance; ++step) {
        currentRayPos = currentRayPos + direction;
        std::cout << "  Tracing tile " << step << " at " << currentRayPos << " ...\n";

        for (Unit* target : allUnits) {
            if (target && target->isAlive() && target->getPosition() == currentRayPos) {
                if (target->getTeamId() != teamId) {
                    std::cout << "  >>> Direct Hit! Enemy " << target->getName() 
                              << " pierced at " << target->getPosition() 
                              << " for 3x damage (" << ultDamage << ")!\n";
                    target->takeDamage(ultDamage);
                    unitsHit++;
                } else {
                    std::cout << "  >>> Bullet bypasses ally " << target->getName() 
                              << " (Safe from friendly fire).\n";
                }
            }
        }
    }

    std::cout << "[Piercing Shot Complete] Enemies pierced: " << unitsHit << "\n\n";
    return true;
}
