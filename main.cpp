#include <iostream>
#include <vector>
#include "Vector3i.h"
#include "Unit.h"
#include "Sniper.h"
#include "Cavalry.h"

int main() {
    std::cout << "========================================================\n";
    std::cout << "  ACTION POINT ZERO - TASK 3: CORE COMBAT DEMO\n";
    std::cout << "  Role: The Combat Specialist (Unit Core, Sniper & Cavalry)\n";
    std::cout << "========================================================\n\n";

    // ------------------------------------------------------------------------
    // SETUP: Create Squads for Team Alpha (Team 1) and Team Bravo (Team 2)
    // ------------------------------------------------------------------------
    Sniper sniperAlpha("Ghost (Sniper)", 1, Vector3i(0, 0, 0));
    Cavalry cavalryAlpha("Vanguard (Cavalry)", 1, Vector3i(1, 0, 1));

    // Enemy units (Team 2)
    Unit enemySoldier1("Enemy Grunt A", 2, 40, 15, 2, Vector3i(1, 0, 2));
    Unit enemySoldier2("Enemy Grunt B", 2, 40, 15, 2, Vector3i(2, 0, 2));
    Unit enemySoldier3("Enemy Grunt C", 2, 50, 15, 2, Vector3i(3, 0, 2));
    Unit distantEnemy("Enemy Scout", 2, 80, 20, 2, Vector3i(8, 0, 8)); // Far away

    std::vector<Unit*> battlefieldUnits = {
        &sniperAlpha,
        &cavalryAlpha,
        &enemySoldier1,
        &enemySoldier2,
        &enemySoldier3,
        &distantEnemy
    };

    std::cout << "--- Initial Battlefield Status ---\n";
    for (const auto* unit : battlefieldUnits) {
        unit->displayStatus();
    }
    std::cout << "\n";

    // ------------------------------------------------------------------------
    // TEST 1: Standard 2-AP Attack & Range Checking
    // ------------------------------------------------------------------------
    std::cout << "========================================================\n";
    std::cout << "TEST 1: Standard 2-AP Attack & Range Validation\n";
    std::cout << "========================================================\n";

    // Cavalry tries to attack distant enemy (Out of range check)
    std::cout << "\n[Step 1A] Cavalry tries to attack distant enemy at (8, 0, 8):\n";
    cavalryAlpha.standardAttack(&distantEnemy);

    // Cavalry attacks adjacent enemy (In range check)
    std::cout << "\n[Step 1B] Cavalry performs valid standard attack on adjacent Enemy Grunt A:\n";
    cavalryAlpha.standardAttack(&enemySoldier1);

    // ------------------------------------------------------------------------
    // TEST 2: Strict 3-AP Per Turn Unit Cap
    // ------------------------------------------------------------------------
    std::cout << "\n========================================================\n";
    std::cout << "TEST 2: Strict 3-AP Per Unit Per Turn Cap Enforcement\n";
    std::cout << "========================================================\n";
    std::cout << "Cavalry has already used " << cavalryAlpha.getApSpentThisTurn() 
              << "/3 AP this turn.\n";
    std::cout << "Attempting another 2-AP Standard Attack in the same turn (Total would be 4 AP):\n";
    bool success = cavalryAlpha.standardAttack(&enemySoldier1);
    if (!success) {
        std::cout << ">> VERIFIED: Individual 3-AP limit correctly blocked the action!\n";
    }

    // Reset turns for next tests
    std::cout << "\n--- Ending Turn: Resetting Unit AP pools ---\n";
    for (auto* unit : battlefieldUnits) {
        unit->resetTurn();
    }

    // ------------------------------------------------------------------------
    // TEST 3: Sniper Ultimate - Piercing Shot (Line Tracing & 3x Damage)
    // ------------------------------------------------------------------------
    std::cout << "\n========================================================\n";
    std::cout << "TEST 3: Sniper Piercing Shot (Grid Line Tracing & 3x Dmg)\n";
    std::cout << "========================================================\n";
    
    // Line up targets in a straight row along X axis from (1,0,0) to (3,0,0)
    Unit lineTarget1("Enemy Sentry 1", 2, 100, 10, 1, Vector3i(1, 0, 0));
    Unit lineTarget2("Enemy Sentry 2", 2, 120, 10, 1, Vector3i(2, 0, 0));
    Unit lineTarget3("Enemy Sentry 3", 2, 110, 10, 1, Vector3i(3, 0, 0));

    std::vector<Unit*> sniperLineBattlefield = {
        &sniperAlpha,
        &lineTarget1,
        &lineTarget2,
        &lineTarget3
    };

    std::cout << "Targets lined up along the X-axis:\n";
    lineTarget1.displayStatus();
    lineTarget2.displayStatus();
    lineTarget3.displayStatus();

    // Sniper is at (0, 0, 0). Fires along direction (+1, 0, 0)
    std::cout << "\nSniper Ghost fires Piercing Shot along (+1, 0, 0):\n";
    sniperAlpha.piercingShot(Vector3i(1, 0, 0), 4, sniperLineBattlefield);

    std::cout << "Status of targets after Piercing Shot:\n";
    lineTarget1.displayStatus();
    lineTarget2.displayStatus();
    lineTarget3.displayStatus();

    // ------------------------------------------------------------------------
    // TEST 4: Cavalry Ultimate - Chain Blitz (Recursive Chain On Kill, Max 3)
    // ------------------------------------------------------------------------
    std::cout << "\n========================================================\n";
    std::cout << "TEST 4: Cavalry Chain Blitz (Recursive Chain Kill Loop)\n";
    std::cout << "========================================================\n";

    // Position 3 low-HP enemy units in adjacent clusters
    // Cavalry starts at (0, 0, 0)
    // Target 1 at (1, 0, 0)  [adjacent to (0,0,0)]
    // Target 2 at (2, 0, 0)  [adjacent to (1,0,0)]
    // Target 3 at (2, 0, 1)  [adjacent to (2,0,0)]
    cavalryAlpha.setPosition(Vector3i(0, 0, 0));
    cavalryAlpha.resetTurn();

    Unit chainTarget1("Cluster Enemy 1", 2, 35, 10, 1, Vector3i(1, 0, 0));
    Unit chainTarget2("Cluster Enemy 2", 2, 40, 10, 1, Vector3i(2, 0, 0));
    Unit chainTarget3("Cluster Enemy 3", 2, 45, 10, 1, Vector3i(2, 0, 1));
    Unit chainTarget4("Cluster Enemy 4", 2, 30, 10, 1, Vector3i(2, 0, 2)); // 4th enemy to test max 3 cap

    std::vector<Unit*> chainBattlefield = {
        &cavalryAlpha,
        &chainTarget1,
        &chainTarget2,
        &chainTarget3,
        &chainTarget4
    };

    std::cout << "Cavalry Base Damage is " << cavalryAlpha.getBaseDamage() 
              << ". Chain Blitz 1.5x damage = " 
              << static_cast<int>(cavalryAlpha.getBaseDamage() * 1.5f) << " damage per strike.\n";
    std::cout << "Triggering Chain Blitz on initial target Cluster Enemy 1:\n";

    cavalryAlpha.chainBlitz(&chainTarget1, chainBattlefield);

    std::cout << "Status of cluster enemies after Chain Blitz:\n";
    chainTarget1.displayStatus();
    chainTarget2.displayStatus();
    chainTarget3.displayStatus();
    chainTarget4.displayStatus();

    std::cout << "\n========================================================\n";
    std::cout << "  ALL COMBAT SPECIALIST (ROLE 3) TESTS COMPLETED!\n";
    std::cout << "========================================================\n";

    return 0;
}
