# PumpApp 💪

A desktop application for managing gym workout plans, written in C++ with Qt framework.

## About

PumpApp lets you build and manage a personal exercise database and create custom workout plans with detailed training parameters (sets, reps, weight, rest time). Data is automatically saved to JSON after every operation.

## Features

- ➕ Add, edit, delete and search exercises (Weighted / Bodyweight)
- 📋 Create and manage workout plans with full parameter control
- 💾 Automatic JSON persistence (`data/exercises.json`, `data/plans.json`)
- ✅ Multi-level data validation (GUI, logic, repository layers)
- 🧪 14 unit tests (Google Test) — all passing

## Tech Stack

- **Language:** C++17 (smart pointers, RAII)
- **GUI:** Qt 6.10.1 (Qt Widgets)
- **Build:** CMake 3.30+, MinGW 13.1.0
- **Testing:** Google Test 1.12.1
- **Platform:** Windows 10/11 (64-bit), Linux compatible

## Design Patterns

- **Factory Method** — `ExerciseFactory::createExercise()`
- **Repository Pattern** — `ExerciseRepository`, `WorkoutPlanRepository`
- **Singleton** — `DatabaseManager`
- **Inheritance hierarchy** — abstract `Exercise` → `WeightedExercise`, `BodyweightExercise`

## Getting Started

1. Open Qt Creator
2. `File → Open File or Project` → select `CMakeLists.txt`
3. Configure: `Desktop Qt 6.10.1 MinGW 64-bit`
4. Hit `Run` (Ctrl+R)

> Alternatively: run `PumpApp.exe` directly from the `build/` folder (Windows)