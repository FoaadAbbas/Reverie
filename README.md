Reverie (Live Simulator)
Reverie is a modular Unreal Engine project that explores diverse gameplay mechanics and genres within a single, unified architecture. Built with a hybrid C++ and Blueprint approach, the project acts as a "Live Simulator," allowing seamless transitions between distinct game modes—ranging from intense action combat to narrative-driven exploration.

🌟 Key Features & Gameplay Variants
The project is divided into distinct "Variants," each with its own specialized Game Mode, Player Controller, Character class, and environmental interactions.

⚔️ Combat Variant
A fully-featured action combat system designed for responsive and impactful encounters.

Combo & Charged Attacks: Robust melee system utilizing custom Animation Notifies (CheckCombo, CheckChargedAttack, DoAttackTrace).

Advanced AI: Enemy combatants driven by custom AI Controllers, State Trees, and Environment Query Systems (EQS) for dynamic positioning (flanking, evading, falling back).

Interactive Environment: Features damageable props, training dummies, lava floors, and camera shake feedback for hits.

UI & FX: Custom Niagara systems for damage trails and robust combat HUDs (e.g., enemy/player life bars).

🏃 Platforming Variant
A fast-paced, 3D platforming experience emphasizing precise movement.

Advanced Mobility: Features dashing mechanics integrated closely with animation states (AnimNotify_EndDash).

Visual Polish: Satisfying movement feedback with custom jump trail VFX (Niagara) and tuned air-control mechanics.

🕹️ Side-Scrolling Variant
A 2.5D side-scrolling module showcasing classic platformer mechanics in a modern engine.

Custom Camera Manager: Enforces a strict 2D plane perspective.

Platforming Mechanics: Includes jump pads, moving platforms, and "soft" platforms (allowing players to drop through).

Interactive World: Dedicated interaction system, item pickups, and custom side-scrolling NPC AI.

🧸 Childhood (Narrative) Variant
A slower-paced, narrative-driven exploration mode.

Unique NPCs: Features specialized interactions with character archetypes like KindergartenNPC and DoctorGhostActor.

Atmospheric Audio: Driven by a custom MedicalAudioManager to create a specific, evocative ambiance.

Story Interactions: Custom InteractableObject classes tailored for lore delivery and environment puzzles.

🛠️ Technical Architecture
Engine: Unreal Engine 5 (utilizing Manny/Quinn mannequins and enhanced input systems).

Language: C++ & Blueprints (Hybrid).

Modular Design: Each variant is isolated into its own folder structure (Source/LiveSimulator/Variant_[Name] and Content/Variant_[Name]), ensuring clean code separation and scalability.

Enhanced Input System: Implements UE5's Enhanced Input mapping contexts (IMC) for highly customizable controls tailored to each variant (e.g., IA_ComboAttack, IA_Dash, IA_Drop).

🚀 Getting Started
Prerequisites
Unreal Engine 5 (ensure you have the appropriate version installed).

Visual Studio 2022 (or your preferred IDE setup for UE5 C++ development) with Game Development with C++ workload.

Installation & Setup
Clone the repository:

Bash
git clone https://github.com/foaadabbas/reverie.git
Generate Visual Studio project files:
Right-click LiveSimulator.uproject and select "Generate Visual Studio project files".

Build the project:
Open the generated .sln file in Visual Studio, set your configuration to Development Editor, and build the solution.

Launch the Editor:
Open LiveSimulator.uproject to launch the Unreal Engine editor.

🎮 Controls
Controls automatically map based on the active variant's Input Mapping Context (IMC).

Movement: W A S D / Left Stick

Look: Mouse / Right Stick

Jump: Spacebar / Face Button Bottom

Combat Mode:

Light Attack: Left Click

Charged Attack: Hold Left Click

Toggle Camera Side: Tab / Shoulder Button

Platforming Mode:

Dash: Shift / Trigger

Side-Scrolling Mode:

Drop Through Platform: S + Jump

Interact: E / Face Button Right

📂 Project Structure Overview
Plaintext
Reverie/
├── Content/
│   ├── Characters/         # Base UE Mannequins, Rigs, and generic anims
│   ├── Input/              # Shared Enhanced Input actions and touch interfaces
│   ├── Variant_Combat/     # Blueprints, Anims, Maps, and FX for Combat
│   ├── Variant_Platforming/# Blueprints, Anims, Maps, and FX for Platforming
│   ├── Variant_SideScrolling/# Blueprints, Maps, and UI for Side-Scrolling
│   └── LevelPrototyping/   # Base meshes and materials for blockouts
└── Source/
    └── LiveSimulator/
        ├── Variant_Childhood/     # C++ logic for Narrative mode
        ├── Variant_Combat/        # C++ logic for AI, Interfaces, and Combat
        ├── Variant_Platforming/   # C++ logic for Mobility
        └── Variant_SideScrolling/ # C++ logic for 2D Camera and Actors
📜 License
(Include your license information here, e.g., MIT License, or state if it is proprietary/All Rights Reserved depending on your preference.)
