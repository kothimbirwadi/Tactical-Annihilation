# Action Point Zero - Task 3: The Combat Specialist

**Assigned Role:** Member 3: The Combat Specialist (Unit Core, Sniper & Cavalry)  
**Academic Alignment:** Computer Engineering OOP Syllabus (Units 1 to 6)

---

## 📌 Mapping Code to OOPs Syllabus Topics

| Syllabus Unit | Key Concept | Implementation in Code |
| :--- | :--- | :--- |
| **Unit 1: Fundamentals of OOP** | Objects, Classes, Encapsulation, Abstraction, Inheritance, Dynamic Binding | Found across `Unit`, `Sniper`, `Cavalry`, and `Vector3i` classes. |
| **Unit 2: Classes and Objects** | Access Modifiers (`private`, `protected`, `public`) | `Unit.h`, `Vector3i.h` |
| | The `this` pointer | Explicitly used in `Unit.cpp` (`this->name = unitName;`, `this->position = ...`) |
| | Static Data Members & Static Methods | `Unit::totalUnitsCreated` and `Unit::getTotalUnitsCreated()` |
| | Method Overloading (Compile-time Polymorphism) | `Unit::attack(Unit* target)` vs `Unit::attack(Unit* target, int bonusDamage)` |
| | Dynamic Memory (`new` / `delete`) | `Unit* ghost = new Sniper(...);` and `delete ghost;` in `main.cpp` |
| | Friend Function & Operator Overloading | Overloaded `operator<<` (friend) and `operator==`, `operator+`, `operator-` in `Vector3i.h` & `Unit.h` |
| **Unit 3: Constructors & Destructors** | Types of Constructors | Default Constructor, Parameterized Constructor with default arguments, and Copy Constructor in `Unit.h` / `Unit.cpp` |
| | Virtual Destructor | `virtual ~Unit()` ensures clean polymorphic deallocation when `delete` is called. |
| **Unit 4: Inheritance & Polymorphism** | Types of Inheritance | Hierarchical Inheritance: `Sniper` and `Cavalry` derive from base class `Unit`. |
| | Abstract Classes & Interfaces | `Unit` has pure virtual function `virtual void useUltimate(...) = 0;` |
| | Run-time Polymorphism (Dynamic Binding) | Calling `basePtr->useUltimate(...)` dynamically dispatches to `Sniper` or `Cavalry`. |
| **Unit 5: File Handling** | `std::ofstream`, `std::ifstream`, File Modes | `CombatLogger.h` records replay logs sequentially to `combat_replay_log.txt` and reads it back. |
| **Unit 6: Exception Handling & Generics** | Custom Exception Hierarchy, `try-catch`, Multiple Catch Clauses | `CombatExceptions.h`: `OutOfAPException`, `OutOfRangeException`, `InvalidTargetException` caught in `main.cpp`. |
| | Generic Programming (Templates) | Template method `logMetric<T>(label, value)` in `CombatLogger.h`. |

---

## ⚔️ Game Mechanics Implemented (GDD Task 3)

1. **Base Unit Core**:
   - Manages operative health, base damage, position (`Vector3i`), and alive/dead state.
   - **Strict 3 AP Unit Cap**: Blocks any action that would cause an operative to spend more than 3 AP in a turn.
   - **Standard 2-AP Attack**: Validates range, deducts 2 AP, and deals damage to target.
2. **Sniper Ultimate (Piercing Shot)**:
   - Costs 3 AP. Traces a straight ray along 3D grid tiles, dealing **3x base damage** to all enemies along the path.
3. **Cavalry Ultimate (Chain Blitz)**:
   - Costs 3 AP. Heavy strike dealing **1.5x damage**. If the target is eliminated, automatically runs a **recursive chain-attack loop** against adjacent enemies (capped at 3 chains).

---

## 🚀 How to Compile and Run

Run in PowerShell:

```powershell
g++ -std=c++11 -Wall -Wextra Vector3i.h CombatExceptions.h CombatLogger.h Unit.cpp Sniper.cpp Cavalry.cpp Infantry.h main.cpp -o combat_specialist.exe
.\combat_specialist.exe
```
