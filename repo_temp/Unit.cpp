#include "Unit.h"

// [OOP CONCEPT: Definition of Static Data Member]
int Unit::totalUnitsCreated = 0;

// 1. Default Constructor
Unit::Unit() 
    : unitId(++totalUnitsCreated), name("Unnamed Operative"), teamId(1), 
      maxHealth(100), currentHealth(100), baseDamage(20), attackRange(2), 
      position(Vector3i(0, 0, 0)), apSpentThisTurn(0) {
    std::cout << "[Constructor] Default Unit created. ID: " << unitId << "\n";
}

// 2. Parameterized Constructor
Unit::Unit(const std::string& unitName, int team, int hp, int dmg, int range, Vector3i pos)
    : unitId(++totalUnitsCreated), name(unitName), teamId(team), 
      maxHealth(hp), currentHealth(hp), baseDamage(dmg), attackRange(range), 
      position(pos), apSpentThisTurn(0) {
    // [OOP CONCEPT: Explicit use of 'this' pointer]
    this->name = unitName;
    std::cout << "[Constructor] Operative '" << this->name << "' deployed (ID: " << this->unitId << ").\n";
}

// 3. Copy Constructor (Deep Copy)
Unit::Unit(const Unit& other)
    : unitId(++totalUnitsCreated), name(other.name + " (Clone)"), teamId(other.teamId),
      maxHealth(other.maxHealth), currentHealth(other.currentHealth), baseDamage(other.baseDamage),
      attackRange(other.attackRange), position(other.position), apSpentThisTurn(0) {
    std::cout << "[Copy Constructor] Cloned " << other.name << " into " << this->name << ".\n";
}

// 4. Destructor
Unit::~Unit() {
    std::cout << "[Destructor] Operative '" << name << "' (ID: " << unitId << ") destroyed.\n";
}

// [OOP CONCEPT: Static Member Function Implementation]
int Unit::getTotalUnitsCreated() {
    return totalUnitsCreated;
}

bool Unit::canSpendAP(int amount) const {
    if (!isAlive()) return false;
    return (apSpentThisTurn + amount <= MAX_AP_PER_TURN);
}

void Unit::spendAP(int amount) {
    if (!isAlive()) {
        throw InvalidTargetException(name + " is incapacitated and cannot act.");
    }
    // [OOP CONCEPT: Throwing Custom Exception]
    if (apSpentThisTurn + amount > MAX_AP_PER_TURN) {
        throw OutOfAPException(name, amount, apSpentThisTurn, MAX_AP_PER_TURN);
    }
    apSpentThisTurn += amount;
    std::cout << "  [" << name << "] Spent " << amount << " AP (" 
              << apSpentThisTurn << "/" << MAX_AP_PER_TURN << " AP used this turn).\n";
}

void Unit::resetTurn() {
    apSpentThisTurn = 0;
}

bool Unit::takeDamage(int amount) {
    if (!isAlive()) return false;

    currentHealth -= amount;
    std::cout << "  -> " << name << " took " << amount << " damage! (HP: ";
    if (currentHealth <= 0) {
        currentHealth = 0;
        std::cout << "0/" << maxHealth << " - KIA!)\n";
        return true; // Fatal damage
    } else {
        std::cout << currentHealth << "/" << maxHealth << ")\n";
        return false;
    }
}

void Unit::heal(int amount) {
    if (!isAlive()) return;
    currentHealth += amount;
    if (currentHealth > maxHealth) currentHealth = maxHealth;
    std::cout << "  -> " << name << " healed for " << amount 
              << " HP (HP: " << currentHealth << "/" << maxHealth << ").\n";
}

// ============================================================================
// [OOP CONCEPT: Compile-Time Polymorphism - Method 1]
// Standard 2-AP Attack
// ============================================================================
bool Unit::attack(Unit* target) {
    if (!target) {
        throw InvalidTargetException("Null target specified for attack!");
    }
    if (!this->isAlive()) {
        throw InvalidTargetException(this->name + " is dead and cannot attack.");
    }
    if (!target->isAlive()) {
        throw InvalidTargetException("Target " + target->getName() + " is already eliminated.");
    }
    if (target->getTeamId() == this->teamId) {
        throw InvalidTargetException("Friendly fire is disabled! Cannot attack teammate.");
    }

    // 1. AP Validation Check (Requires 2 AP)
    if (!canSpendAP(STANDARD_ATTACK_AP_COST)) {
        throw OutOfAPException(this->name, STANDARD_ATTACK_AP_COST, this->apSpentThisTurn, MAX_AP_PER_TURN);
    }

    // 2. Range Validation Check
    int dist = this->position.distanceTo(target->getPosition());
    if (dist > this->attackRange) {
        throw OutOfRangeException(this->name, target->getName(), dist, this->attackRange);
    }

    // 3. Deduct AP and Apply Damage
    spendAP(STANDARD_ATTACK_AP_COST);
    std::cout << "  [" << this->name << "] executes Standard Attack (2 AP) on " 
              << target->getName() << " for " << this->baseDamage << " damage!\n";
    target->takeDamage(this->baseDamage);
    return true;
}

// ============================================================================
// [OOP CONCEPT: Compile-Time Polymorphism - Method Overloading - Method 2]
// Overloaded Attack with Bonus Damage
// ============================================================================
bool Unit::attack(Unit* target, int bonusDamage) {
    std::cout << "  [Special Action] Attack augmented with +" << bonusDamage << " bonus damage!\n";
    if (!target) throw InvalidTargetException("Null target!");
    if (!this->isAlive()) throw InvalidTargetException(this->name + " is dead.");
    if (!target->isAlive()) throw InvalidTargetException("Target " + target->getName() + " is already eliminated.");
    if (target->getTeamId() == this->teamId) throw InvalidTargetException("Cannot attack teammate.");

    // 1. AP Check
    if (!canSpendAP(STANDARD_ATTACK_AP_COST)) {
        throw OutOfAPException(this->name, STANDARD_ATTACK_AP_COST, this->apSpentThisTurn, MAX_AP_PER_TURN);
    }

    // 2. Range Validation
    int dist = this->position.distanceTo(target->getPosition());
    if (dist > this->attackRange) {
        throw OutOfRangeException(this->name, target->getName(), dist, this->attackRange);
    }

    // 3. Deduct AP and Apply Damage
    spendAP(STANDARD_ATTACK_AP_COST);
    int totalDamage = this->baseDamage + bonusDamage;
    std::cout << "  [" << this->name << "] executes Powered Attack (2 AP) on " 
              << target->getName() << " for " << totalDamage << " total damage!\n";
    target->takeDamage(totalDamage);
    return true;
}

void Unit::displayStatus() const {
    std::cout << *this << "\n";
}

// [OOP CONCEPT: Friend Function Implementation]
std::ostream& operator<<(std::ostream& os, const Unit& unit) {
    os << unit.name << " [Team " << unit.teamId << "] | HP: " 
       << unit.currentHealth << "/" << unit.maxHealth 
       << " | Base Dmg: " << unit.baseDamage 
       << " | Range: " << unit.attackRange 
       << " | AP used: " << unit.apSpentThisTurn << "/" << Unit::MAX_AP_PER_TURN 
       << " | Pos: " << unit.position;
    if (!unit.isAlive()) os << " [KIA]";
    return os;
}
