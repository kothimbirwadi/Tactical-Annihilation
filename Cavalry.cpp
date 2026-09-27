#include "Cavalry.h"
#include <iostream>

Cavalry::Cavalry(const std::string& unitName, int team, Vector3i pos)
    // Moderate Health (90), High Strike Damage (30), Melee/Short Range (1 tile)
    : Unit(unitName, team, 90, 30, 1, pos) {}

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

void Cavalry::executeChainBlitzRecursive(Unit* currentTarget, 
                                         const std::vector<Unit*>& allUnits, 
                                         std::set<Unit*>& visitedTargets, 
                                         int currentChain) {
    if (!currentTarget || !currentTarget->isAlive()) {
        return;
    }

    visitedTargets.insert(currentTarget);

    // 1.5x damage heavy strike
    int strikeDamage = static_cast<int>(baseDamage * 1.5f);

    std::cout << "  [Chain " << currentChain << "/" << MAX_CHAINS << "] " 
              << name << " dashes and strikes " << currentTarget->getName() 
              << " for " << strikeDamage << " damage!\n";

    bool targetDied = currentTarget->takeDamage(strikeDamage);

    // Reposition Cavalry to the struck target's vicinity / position as a dash
    position = currentTarget->getPosition();

    // Check kill condition and chain recursion limit
    if (targetDied) {
        std::cout << "  >>> Target " << currentTarget->getName() << " was ELIMINATED!\n";

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
        std::cout << "  >>> Target survived; chain terminated.\n";
    }
}

bool Cavalry::chainBlitz(Unit* initialTarget, const std::vector<Unit*>& allUnits) {
    if (!isAlive()) {
        std::cout << "[Ultimate Error] " << name << " is down and cannot execute Chain Blitz!\n";
        return false;
    }

    if (!initialTarget || !initialTarget->isAlive()) {
        std::cout << "[Ultimate Error] Invalid or dead initial target!\n";
        return false;
    }

    if (initialTarget->getTeamId() == teamId) {
        std::cout << "[Ultimate Error] Cannot target ally!\n";
        return false;
    }

    // Check if initial target is within reach (adjacent or range)
    if (!position.isAdjacent(initialTarget->getPosition()) && 
        position.distanceTo(initialTarget->getPosition()) > attackRange) {
        std::cout << "[Ultimate Error] Initial target is not adjacent / in range!\n";
        return false;
    }

    // 1. AP Check: Ultimate costs 3 AP
    if (!canSpendAP(ULTIMATE_AP_COST)) {
        return false;
    }

    // Deduct AP
    spendAP(ULTIMATE_AP_COST);

    std::cout << "\n======================================================\n";
    std::cout << "[ULTIMATE ACTIVATION] " << name << " casts CHAIN BLITZ!\n";
    std::cout << "======================================================\n";

    std::set<Unit*> visitedTargets;
    executeChainBlitzRecursive(initialTarget, allUnits, visitedTargets, 1);

    std::cout << "[Chain Blitz Complete] Total chain hits: " << visitedTargets.size() << "\n\n";
    return true;
}
