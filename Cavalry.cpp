#include "Cavalry.h"
#include <iostream>

// [OOP CONCEPT: Constructor Initialization List calling Base Constructor]
Cavalry::Cavalry(const std::string& unitName, int team, Vector3i pos)
    // Moderate Health (90), High Strike Damage (30), Melee Range (1 tile)
    : Unit(unitName, team, 90, 30, 1, pos) {}

// [OOP CONCEPT: Run-Time Polymorphism / Dynamic Binding Implementation]
void Cavalry::useUltimate(const std::vector<Unit*>& allUnits, const Vector3i& /*targetParam*/) {
    // Finds the closest alive enemy to launch Chain Blitz
    Unit* closestEnemy = nullptr;
    int minDistance = 99999;
    for (Unit* u : allUnits) {
        if (u && u->getTeamId() != teamId && u->isAlive()) {
            int d = position.distanceTo(u->getPosition());
            if (d < minDistance) {
                minDistance = d;
                closestEnemy = u;
            }
        }
    }
    if (closestEnemy) {
        chainBlitz(closestEnemy, allUnits);
    } else {
        std::cout << "[Chain Blitz Error] No living enemies in sight to blitz!\n";
    }
}

Unit* Cavalry::findNextAdjacentEnemy(const Vector3i& fromPos, 
                                     const std::vector<Unit*>& allUnits, 
                                     const std::set<Unit*>& visitedTargets) {
    for (Unit* unit : allUnits) {
        if (!unit) continue;
        if (unit->getTeamId() != teamId && unit->isAlive()) {
            if (visitedTargets.find(unit) == visitedTargets.end()) {
                if (fromPos.isAdjacent(unit->getPosition())) {
                    return unit;
                }
            }
        }
    }
    return nullptr;
}

// [OOP CONCEPT: Recursion / Algorithm for Chain Blitz Loop]
void Cavalry::executeChainBlitzRecursive(Unit* currentTarget, 
                                         const std::vector<Unit*>& allUnits, 
                                         std::set<Unit*>& visitedTargets, 
                                         int currentChain) {
    if (!currentTarget || !currentTarget->isAlive()) return;

    visitedTargets.insert(currentTarget);

    // 1.5x damage heavy strike
    int strikeDamage = static_cast<int>(baseDamage * 1.5f);

    std::cout << "  [Chain " << currentChain << "/" << MAX_CHAINS << "] " 
              << name << " dashes and strikes " << currentTarget->getName() 
              << " for " << strikeDamage << " damage!\n";

    bool targetDied = currentTarget->takeDamage(strikeDamage);

    // Move Cavalry position to struck enemy location (Tactical dash)
    this->position = currentTarget->getPosition();

    // Check kill condition and chain recursion limit
    if (targetDied) {
        std::cout << "  >>> Target " << currentTarget->getName() << " ELIMINATED!\n";

        if (currentChain < MAX_CHAINS) {
            Unit* nextEnemy = findNextAdjacentEnemy(currentTarget->getPosition(), allUnits, visitedTargets);
            if (nextEnemy) {
                std::cout << "  >>> CHAIN TRIGGERED! Chaining free blitz to adjacent target " 
                          << nextEnemy->getName() << "...\n";
                // Recursive step
                executeChainBlitzRecursive(nextEnemy, allUnits, visitedTargets, currentChain + 1);
            } else {
                std::cout << "  >>> No adjacent enemies found to continue chain.\n";
            }
        } else {
            std::cout << "  >>> Maximum chain limit (" << MAX_CHAINS << ") reached!\n";
        }
    } else {
        std::cout << "  >>> Target survived; chain ended.\n";
    }
}

bool Cavalry::chainBlitz(Unit* initialTarget, const std::vector<Unit*>& allUnits) {
    if (!isAlive()) {
        throw InvalidTargetException(name + " is knocked out and cannot use Chain Blitz!");
    }
    if (!initialTarget || !initialTarget->isAlive()) {
        throw InvalidTargetException("Invalid or already defeated target for Chain Blitz!");
    }
    if (initialTarget->getTeamId() == teamId) {
        throw InvalidTargetException("Cannot blitz allied unit!");
    }
    if (!position.isAdjacent(initialTarget->getPosition()) && 
        position.distanceTo(initialTarget->getPosition()) > attackRange) {
        throw OutOfRangeException(name, initialTarget->getName(), 
                                  position.distanceTo(initialTarget->getPosition()), attackRange);
    }

    // Spend 3 AP for ultimate ability (throws OutOfAPException if insufficient)
    spendAP(ULTIMATE_AP_COST);

    std::cout << "\n======================================================\n";
    std::cout << "[ULTIMATE: CHAIN BLITZ] " << name << " dashes into combat!\n";
    std::cout << "======================================================\n";

    std::set<Unit*> visitedTargets;
    executeChainBlitzRecursive(initialTarget, allUnits, visitedTargets, 1);

    std::cout << "[Chain Blitz Complete] Total chain strikes: " << visitedTargets.size() << "\n\n";
    return true;
}
