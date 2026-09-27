#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>
#include <iostream>
#include "Vector3i.h"

/**
 * @brief Base class for all operatives in Action Point Zero.
 * Member 3 (The Combat Specialist): Unit Core & Combat Foundation.
 */
class Unit {
protected:
    std::string name;
    int teamId;         // 1 for Team Alpha, 2 for Team Bravo
    int maxHealth;
    int currentHealth;
    int baseDamage;
    int attackRange;    // Range for standard attacks in grid tiles
    Vector3i position;
    int apSpentThisTurn;

public:
    static const int MAX_AP_PER_TURN = 3;       // Strict rule: no unit can spend > 3 AP/turn
    static const int STANDARD_ATTACK_AP_COST = 2;// Standard attack costs 2 AP
    static const int ULTIMATE_AP_COST = 3;      // Ultimate abilities cost 3 AP

    Unit(const std::string& unitName, int team, int hp, int dmg, int range, Vector3i pos);
    virtual ~Unit() = default;

    // --- Core Getters & State ---
    std::string getName() const { return name; }
    int getTeamId() const { return teamId; }
    int getCurrentHealth() const { return currentHealth; }
    int getMaxHealth() const { return maxHealth; }
    int getBaseDamage() const { return baseDamage; }
    int getAttackRange() const { return attackRange; }
    Vector3i getPosition() const { return position; }
    int getApSpentThisTurn() const { return apSpentThisTurn; }
    int getRemainingUnitAP() const { return MAX_AP_PER_TURN - apSpentThisTurn; }
    bool isAlive() const { return currentHealth > 0; }

    void setPosition(const Vector3i& newPos) { position = newPos; }

    // --- AP Management ---
    bool canSpendAP(int amount) const;
    bool spendAP(int amount);
    void resetTurn(); // Called at start of player's turn to reset AP cap

    // --- Health & Damage Handling ---
    // Returns true if the damage was fatal
    bool takeDamage(int amount);
    void heal(int amount);

    // --- Standard Combat (2 AP) ---
    // Checks range, deducts 2 AP, deals baseDamage to enemy
    virtual bool standardAttack(Unit* target);

    // Virtual hook for Ultimate (implemented in derived classes)
    virtual bool useUltimate(const std::vector<Unit*>& /*allUnits*/, const Vector3i& /*targetParam*/) {
        std::cout << name << " has no default ultimate action.\n";
        return false;
    }

    virtual void displayStatus() const;
};

#endif // UNIT_H
