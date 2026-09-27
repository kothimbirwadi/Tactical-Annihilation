#ifndef CAVALRY_H
#define CAVALRY_H

#include "Unit.h"
#include <vector>
#include <set>

/**
 * @brief Cavalry operative: Mobile Striker built for flanking.
 * Specializes in hit-and-run tactics and the Chain Blitz ultimate.
 */
class Cavalry : public Unit {
private:
    static const int MAX_CHAINS = 3; // Maximum 3 chained attacks allowed

    /**
     * @brief Recursive chain attack loop.
     * If the target is eliminated, finds the next adjacent enemy and strikes again.
     */
    void executeChainBlitzRecursive(Unit* currentTarget, 
                                    const std::vector<Unit*>& allUnits, 
                                    std::set<Unit*>& visitedTargets, 
                                    int currentChain);

    /**
     * @brief Helper to find an alive, unvisited enemy adjacent to a reference position.
     */
    Unit* findNextAdjacentEnemy(const Vector3i& fromPos, 
                                const std::vector<Unit*>& allUnits, 
                                const std::set<Unit*>& visitedTargets);

public:
    Cavalry(const std::string& unitName, int team, Vector3i pos);

    /**
     * @brief Ultimate: Chain Blitz (Costs 3 AP).
     * Strikes target for 1.5x base damage. If the target is killed, 
     * automatically chains a free attack to the next adjacent enemy (up to 3 chains).
     */
    bool chainBlitz(Unit* initialTarget, const std::vector<Unit*>& allUnits);
};

#endif // CAVALRY_H
