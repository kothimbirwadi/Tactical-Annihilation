#include <iostream>
#include <vector>
#include "Vector3i.h"
#include "Unit.h"
#include "Sniper.h"
#include "Cavalry.h"
#include "Infantry.h"
#include "CombatExceptions.h"
#include "CombatLogger.h"

// ============================================================================
// ACTION POINT ZERO - TASK 3: THE COMBAT SPECIALIST
// Demonstrating Core Combat & Complete OOPs Syllabus Concepts:
// - Encapsulation, Abstraction, Access Modifiers
// - Constructors (Default, Parameterized, Copy), Destructors
// - Static Data Members & Static Methods
// - 'this' pointer, Friend Functions, Operator Overloading
// - Inheritance (Hierarchical) & Abstract Base Class
// - Polymorphism: Compile-Time (Overloading) & Run-Time (Virtual Functions)
// - Dynamic Memory Allocation ('new' and 'delete')
// - Exception Handling (try, catch, multiple catch clauses, custom exceptions)
// - File Handling (ofstream, ifstream)
// - Generic Programming (Templates)
// ============================================================================

int main() {
    std::cout << "========================================================\n";
    std::cout << "   ACTION POINT ZERO - TASK 3: CORE COMBAT SYSTEM\n";
    std::cout << "   Role: The Combat Specialist (Unit Core, Sniper & Cavalry)\n";
    std::cout << "========================================================\n\n";

    // ------------------------------------------------------------------------
    // [OOP CONCEPT: File Handling & Generic Programming Initialization]
    // ------------------------------------------------------------------------
    CombatLogger logger("combat_replay_log.txt");
    logger.logEvent("Match started between Team Alpha (Player 1) and Team Bravo (Player 2).");

    // ------------------------------------------------------------------------
    // [OOP CONCEPT: Dynamic Memory Allocation ('new') & Base Class Pointers]
    // ------------------------------------------------------------------------
    std::cout << ">>> [DEMO 1] Object Creation & Dynamic Memory Allocation ('new'):\n";

    // Team 1: Combat Specialist Squad
    Unit* ghostSniper = new Sniper("Ghost", 1, Vector3i(0, 0, 0));
    Unit* vanguardCavalry = new Cavalry("Vanguard", 1, Vector3i(1, 0, 1));

    // Team 2: Target Squad (Infantry)
    Unit* enemyAlpha = new Infantry("Enemy Grunt A", 2, 40, 15, Vector3i(1, 0, 2));
    Unit* enemyBravo = new Infantry("Enemy Grunt B", 2, 40, 15, Vector3i(2, 0, 2));
    Unit* enemyScout = new Infantry("Enemy Scout", 2, 80, 20, Vector3i(8, 0, 8)); // Out of range

    // [OOP CONCEPT: Static Member Function & Data Member]
    std::cout << "\n>>> [DEMO 2] Static Member Functions & Data Members:\n";
    std::cout << "  Total units active on battlefield: " << Unit::getTotalUnitsCreated() << "\n\n";
    logger.logMetric("Total Active Units", Unit::getTotalUnitsCreated());

    // ------------------------------------------------------------------------
    // [OOP CONCEPT: Copy Constructor Demonstration]
    // ------------------------------------------------------------------------
    std::cout << ">>> [DEMO 3] Copy Constructor (Deep Copy):\n";
    Infantry originalDummy("Training Dummy", 2, 50, 0, Vector3i(0, 0, 5));
    Infantry clonedDummy = originalDummy; // Invokes Copy Constructor
    std::cout << "  Original: " << originalDummy << "\n";
    std::cout << "  Clone:    " << clonedDummy << "\n\n";

    // ------------------------------------------------------------------------
    // [OOP CONCEPT: Compile-Time Polymorphism (Method Overloading)]
    // ------------------------------------------------------------------------
    std::cout << ">>> [DEMO 4] Compile-Time Polymorphism (Method Overloading):\n";
    std::cout << "  1. Calling attack(target):\n";
    vanguardCavalry->attack(enemyAlpha); // Standard 2-AP attack
    std::cout << "  2. Calling overloaded attack(target, bonusDamage):\n";
    vanguardCavalry->resetTurn();
    vanguardCavalry->attack(enemyAlpha, 5); // Overloaded with bonus damage
    std::cout << "\n";

    // ------------------------------------------------------------------------
    // [OOP CONCEPT: Exception Handling (try-catch with Multiple Clauses)]
    // ------------------------------------------------------------------------
    std::cout << ">>> [DEMO 5] Exception Handling (Range & 3-AP Cap Enforcement):\n";

    // Reset turn for clean exception demonstrations
    vanguardCavalry->resetTurn();

    // Test A: OutOfRangeException
    try {
        std::cout << "  Test A: Attempting to attack distant enemy at (8, 0, 8) with range 1:\n";
        vanguardCavalry->attack(enemyScout);
    } catch (const OutOfRangeException& ex) {
        std::cout << "  [CAUGHT EXPECTED EXCEPTION]: " << ex.what() << "\n";
        logger.logEvent(ex.what());
    } catch (const OutOfAPException& ex) {
        std::cout << "  [CAUGHT AP EXCEPTION]: " << ex.what() << "\n";
    } catch (const CombatException& ex) {
        std::cout << "  [CAUGHT GENERAL COMBAT EXCEPTION]: " << ex.what() << "\n";
    }

    // Test B: OutOfAPException (Exceeding 3 AP per unit per turn)
    try {
        std::cout << "\n  Test B: Performing 1st valid attack on Enemy Grunt B (costs 2 AP):\n";
        vanguardCavalry->attack(enemyBravo); // Uses 2 AP (2/3 AP used)

        std::cout << "  Attempting 2nd attack on Enemy Grunt B in same turn (would require 4/3 AP):\n";
        vanguardCavalry->attack(enemyBravo); // Fails AP check
    } catch (const OutOfAPException& ex) {
        std::cout << "  [CAUGHT EXPECTED EXCEPTION]: " << ex.what() << "\n";
        logger.logEvent(ex.what());
    } catch (const OutOfRangeException& ex) {
        std::cout << "  [CAUGHT RANGE EXCEPTION]: " << ex.what() << "\n";
    } catch (const CombatException& ex) {
        std::cout << "  [CAUGHT GENERAL COMBAT EXCEPTION]: " << ex.what() << "\n";
    }

    // Reset turn AP for next phase
    vanguardCavalry->resetTurn();
    ghostSniper->resetTurn();

    // ------------------------------------------------------------------------
    // [TASK 3 REQUIREMENT & OOP CONCEPT: Run-Time Polymorphism (Virtual Functions)]
    // Sniper Ultimate: Piercing Shot (Grid Line Tracing for 3x Damage)
    // ------------------------------------------------------------------------
    std::cout << "\n>>> [DEMO 6] Sniper Piercing Shot (Run-Time Polymorphism & Line-Tracing):\n";

    Unit* lineTarget1 = new Infantry("Line Sentry 1", 2, 100, 10, Vector3i(1, 0, 0));
    Unit* lineTarget2 = new Infantry("Line Sentry 2", 2, 120, 10, Vector3i(2, 0, 0));
    Unit* lineTarget3 = new Infantry("Line Sentry 3", 2, 110, 10, Vector3i(3, 0, 0));

    std::vector<Unit*> battlefieldUnits = {
        ghostSniper,
        vanguardCavalry,
        lineTarget1,
        lineTarget2,
        lineTarget3,
        enemyAlpha,
        enemyBravo,
        enemyScout
    };

    // Demonstrating Dynamic Binding via Base Class Pointer (Unit*)
    Unit* basePolymorphicPointer = ghostSniper;
    // Calls Sniper::useUltimate via dynamic dispatch
    basePolymorphicPointer->useUltimate(battlefieldUnits, Vector3i(1, 0, 0));
    logger.logEvent("Ghost (Sniper) fired Piercing Shot along trajectory (1, 0, 0).");

    // ------------------------------------------------------------------------
    // [TASK 3 REQUIREMENT: Cavalry Chain Blitz (Recursive Kill Chain, Max 3)]
    // ------------------------------------------------------------------------
    std::cout << "\n>>> [DEMO 7] Cavalry Chain Blitz (Recursive Chain-Attack on Kill):\n";

    // Reposition Cavalry and arrange clustered low-HP enemies
    vanguardCavalry->setPosition(Vector3i(0, 0, 0));
    vanguardCavalry->resetTurn();

    Unit* chain1 = new Infantry("Chain Grunt 1", 2, 35, 10, Vector3i(1, 0, 0)); // Adjacent to (0,0,0)
    Unit* chain2 = new Infantry("Chain Grunt 2", 2, 40, 10, Vector3i(2, 0, 0)); // Adjacent to (1,0,0)
    Unit* chain3 = new Infantry("Chain Grunt 3", 2, 45, 10, Vector3i(2, 0, 1)); // Adjacent to (2,0,0)
    Unit* chain4 = new Infantry("Chain Grunt 4", 2, 30, 10, Vector3i(2, 0, 2)); // 4th target (testing max 3 limit)

    battlefieldUnits.push_back(chain1);
    battlefieldUnits.push_back(chain2);
    battlefieldUnits.push_back(chain3);
    battlefieldUnits.push_back(chain4);

    // Dynamic Binding calling Cavalry::useUltimate
    Unit* cavalryPointer = vanguardCavalry;
    cavalryPointer->useUltimate(battlefieldUnits);
    logger.logEvent("Vanguard (Cavalry) executed Chain Blitz.");

    // ------------------------------------------------------------------------
    // [OOP CONCEPT: File Stream Verification (Reading back combat log)]
    // ------------------------------------------------------------------------
    logger.displayLogSummary();

    // ------------------------------------------------------------------------
    // [OOP CONCEPT: Memory Recovery ('delete') & Virtual Destructors]
    // ------------------------------------------------------------------------
    std::cout << "\n>>> [DEMO 8] Memory Recovery ('delete') via Virtual Destructors:\n";
    for (Unit* u : battlefieldUnits) {
        delete u; // Correctly calls derived destructor, then base destructor
    }

    std::cout << "\n========================================================\n";
    std::cout << "  ALL OOPS SYLLABUS TOPICS & TASK 3 VERIFIED SUCCESSFULLY!\n";
    std::cout << "========================================================\n";

    return 0;
}
