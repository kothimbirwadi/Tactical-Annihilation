#include "Unit.h"

Unit::Unit(const std::string& unitName, int team, int hp, int dmg, int range, Vector3i pos)
    : name(unitName), teamId(team), maxHealth(hp), currentHealth(hp),
      baseDamage(dmg), attackRange(range), position(pos), apSpentThisTurn(0) {}

bool Unit::canSpendAP(int amount) const {
    if (!isAlive()) {
        std::cout << "[AP Error] " << name << " is incapacitated and cannot act.\n";
        return false;
    }
    if (apSpentThisTurn + amount > MAX_AP_PER_TURN) {
        std::cout << "[AP Cap Exceeded] " << name << " cannot spend " << amount 
                  << " AP. Already spent: " << apSpentThisTurn 
                  << "/" << MAX_AP_PER_TURN << " AP this turn.\n";
        return false;
    }
    return true;
}

bool Unit::spendAP(int amount) {
    if (!canSpendAP(amount)) {
        return false;
    }
    apSpentThisTurn += amount;
    std::cout << "[" << name << "] Spent " << amount << " AP (" 
              << apSpentThisTurn << "/" << MAX_AP_PER_TURN << " used this turn).\n";
    return true;
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
        return true; // Target was killed
    } else {
        std::cout << currentHealth << "/" << maxHealth << ")\n";
        return false;
    }
}

void Unit::heal(int amount) {
    if (!isAlive()) return;
    currentHealth += amount;
    if (currentHealth > maxHealth) currentHealth = maxHealth;
    std::cout << "  -> " << name << " was healed for " << amount 
              << " HP! (HP: " << currentHealth << "/" << maxHealth << ")\n";
}

bool Unit::standardAttack(Unit* target) {
    if (!target) {
        std::cout << "[Attack Error] No target specified!\n";
        return false;
    }

    if (!isAlive()) {
        std::cout << "[Attack Error] " << name << " is dead and cannot attack.\n";
        return false;
    }

    if (!target->isAlive()) {
        std::cout << "[Attack Error] Target " << target->getName() << " is already dead.\n";
        return false;
    }

    if (target->getTeamId() == teamId) {
        std::cout << "[Attack Error] Friendly fire is disabled! Cannot attack teammate.\n";
        return false;
    }

    // 1. AP Check (Standard Attack requires 2 AP)
    if (!canSpendAP(STANDARD_ATTACK_AP_COST)) {
        return false;
    }

    // 2. Range Checking
    int dist = position.distanceTo(target->getPosition());
    if (dist > attackRange) {
        std::cout << "[Attack Error] Target " << target->getName() 
                  << " is out of range! (Distance: " << dist 
                  << " tiles, Max Range: " << attackRange << " tiles).\n";
        return false;
    }

    // 3. Deduct AP
    spendAP(STANDARD_ATTACK_AP_COST);

    // 4. Deal Damage
    std::cout << "[" << name << "] performs Standard Attack (2 AP) on " 
              << target->getName() << " for " << baseDamage << " damage!\n";
    target->takeDamage(baseDamage);

    return true;
}

void Unit::displayStatus() const {
    std::cout << name << " [Team " << teamId << "] | HP: " 
              << currentHealth << "/" << maxHealth 
              << " | Base Dmg: " << baseDamage 
              << " | Range: " << attackRange 
              << " | AP used: " << apSpentThisTurn << "/" << MAX_AP_PER_TURN 
              << " | Pos: ";
    position.print();
    if (!isAlive()) std::cout << " (DEAD)";
    std::cout << "\n";
}
