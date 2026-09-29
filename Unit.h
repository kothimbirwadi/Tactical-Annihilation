#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <vector>
#include <iostream>
#include "Vector3i.h"
#include "CombatExceptions.h"

// [OOP CONCEPT: Forward Declaration]
class Unit;

// ============================================================================
// [OOP CONCEPT: Abstract Base Class & Interface]
// Serves as the foundation for all operatives in Action Point Zero.
// Member 3: The Combat Specialist (Unit Core, Sniper & Cavalry)
// ============================================================================
class Unit {
private:
    // [OOP CONCEPT: Encapsulation & Data Hiding (Private Member)]
    int unitId;

protected:
    // [OOP CONCEPT: Protected Visibility for Inheritance]
    std::string name;
    int teamId;          // 1 = Team Alpha, 2 = Team Bravo
    int maxHealth;
    int currentHealth;
    int baseDamage;
    int attackRange;     // Max distance in grid tiles
    Vector3i position;
    int apSpentThisTurn;

    // [OOP CONCEPT: Static Data Member]
    static int totalUnitsCreated;

public:
    // Game Rules Constants
    static const int MAX_AP_PER_TURN = 3;        // Strict 3-AP per unit turn cap
    static const int STANDARD_ATTACK_AP_COST = 2;// Standard attack costs 2 AP
    static const int ULTIMATE_AP_COST = 3;       // Ultimate abilities cost 3 AP

    // ========================================================================
    // [OOP CONCEPT: Constructors & Destructors]
    // ========================================================================
    // 1. Default Constructor
    Unit();

    // 2. Parameterized Constructor with Default Arguments
    Unit(const std::string& unitName, int team, int hp, int dmg, int range, Vector3i pos = Vector3i(0, 0, 0));

    // 3. Copy Constructor (Deep Copy)
    Unit(const Unit& other);

    // 4. Virtual Destructor (Ensures safe cleanup of derived class objects)
    virtual ~Unit();

    // ========================================================================
    // [OOP CONCEPT: Static Member Function]
    // ========================================================================
    static int getTotalUnitsCreated();

    // ========================================================================
    // Core Getters & State (Encapsulation)
    // ========================================================================
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

    // ========================================================================
    // AP Management System
    // ========================================================================
    bool canSpendAP(int amount) const;
    void spendAP(int amount); // Throws OutOfAPException if limit exceeded
    void resetTurn();         // Resets AP cap at beginning of turn

    // ========================================================================
    // Health & Combat Mechanics
    // ========================================================================
    bool takeDamage(int amount); // Returns true if fatal
    void heal(int amount);

    // ========================================================================
    // [OOP CONCEPT: Compile-Time Polymorphism (Method Overloading)]
    // ========================================================================
    // Standard 2-AP attack (checks AP and range, throws exceptions on errors)
    virtual bool attack(Unit* target);

    // Overloaded attack with additional bonus damage modifier
    virtual bool attack(Unit* target, int bonusDamage);

    // ========================================================================
    // [OOP CONCEPT: Run-Time Polymorphism (Pure Virtual Function)]
    // Makes Unit an Abstract Class. Derived classes (Sniper, Cavalry) MUST implement it.
    // ========================================================================
    virtual void useUltimate(const std::vector<Unit*>& allUnits, const Vector3i& targetParam = Vector3i()) = 0;

    virtual void displayStatus() const;

    // ========================================================================
    // [OOP CONCEPT: Friend Function for Stream Output]
    // ========================================================================
    friend std::ostream& operator<<(std::ostream& os, const Unit& unit);
};

#endif // UNIT_H
