#ifndef CAVALRY_H
#define CAVALRY_H

#include "Unit.h"
#include <vector>
#include <set>

// ============================================================================
// [OOP CONCEPT: Inheritance (Hierarchical Inheritance from Unit)]
// Cavalry operative: Mobile Striker built for flanking and dash chain-strikes.
// ============================================================================
class Cavalry : public Unit {
private:
    static const int MAX_CHAINS = 3; // Maximum 3 chained attacks allowed

    // Helper to locate an alive, unvisited enemy adjacent to a position
    Unit* findNextAdjacentEnemy(const Vector3i& fromPos, 
                                const std::vector<Unit*>& allUnits, 
                                const std::set<Unit*>& visitedTargets);

    // Recursive chain attack loop
    void executeChainBlitzRecursive(Unit* currentTarget, 
                                    const std::vector<Unit*>& allUnits, 
                                    std::set<Unit*>& visitedTargets, 
                                    int currentChain);

public:
    // [OOP CONCEPT: Constructor in Derived Class]
    Cavalry(const std::string& unitName, int team, Vector3i pos = Vector3i(0, 0, 0));

    // [OOP CONCEPT: Method Overriding (Run-Time Polymorphism)]
    virtual void useUltimate(const std::vector<Unit*>& allUnits, const Vector3i& targetParam = Vector3i()) override;

    /**
     * @brief Dedicated Chain Blitz method.
     * Heavy strike for 1.5x damage. If the target dies, automatically chains a 
     * free strike to the next adjacent enemy (recursive loop, max 3 chains).
     */
    bool chainBlitz(Unit* initialTarget, const std::vector<Unit*>& allUnits);
};

#endif // CAVALRY_H
