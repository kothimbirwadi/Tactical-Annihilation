# Action Point Zero - Task 3: The Combat Specialist

**Assigned Role:** Member 3: The Combat Specialist (Unit Core, Sniper & Cavalry)

---

## 📌 Requirements Implemented

### 1. C++ Backend (`Unit.h`, `Unit.cpp`, `Vector3i.h`)
- **Base `Unit` Class**:
  - Encapsulates Operative name, team ID, health (current & max), base damage, attack range, and 3D grid position (`Vector3i`).
  - **Strict 3 AP Cap**: Each unit cannot spend more than 3 AP per turn (`apSpentThisTurn + cost <= 3`).
  - **Standard 2-AP Attack**: Checks range, deducts 2 AP from unit, deals base damage, checks friendly fire and target status.
  - Turn reset function (`resetTurn()`) to reset the 3 AP cap when turn passes.

### 2. Sniper Ultimate: Piercing Shot (`Sniper.h`, `Sniper.cpp`)
- **High Damage / Long Range / Low HP**: Operative stats tuned according to GDD.
- **Piercing Shot (3 AP)**:
  - Line-tracing along the specified 3D grid vector up to `maxDistance` tiles.
  - Penetrates through every tile, dealing **3x base damage** to all enemies along the bullet path.

### 3. Cavalry Ultimate: Chain Blitz (`Cavalry.h`, `Cavalry.cpp`)
- **Mobile Striker**: High speed/flanking operative.
- **Chain Blitz (3 AP)**:
  - Heavy initial strike dealing **1.5x damage**.
  - **Recursive Chain Attack Loop**: If the struck target is killed, automatically locates an adjacent living enemy and dashes to strike again.
  - Recursion limit strictly capped at **3 chains**.

### 4. Godot Frontend Companion (`UnitView.gd`)
- Reusable 3D Unit scene script (ready to attach Blender `.glb` models).
- 3D floating health bar with color shifts (Green -> Orange -> Red / KIA).
- VFX hooks for:
  - Standard attack animations and recoil.
  - Sniper Piercing Shot glowing line beam VFX.
  - Cavalry Chain Blitz rapid dash tweening.

---

## 🚀 How to Compile and Run the C++ Demo

Open PowerShell in this folder (`e:\Oops Cp`) and run:

```powershell
g++ -std=c++11 -Wall -Wextra Vector3i.h Unit.cpp Sniper.cpp Cavalry.cpp main.cpp -o combat_specialist.exe
.\combat_specialist.exe
```
