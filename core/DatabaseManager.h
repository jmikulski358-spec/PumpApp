// DatabaseManager.h
// Lokalizacja: core/DatabaseManager.h
// Opis: Singleton Pattern - centralny punkt dostępu do wszystkich repozytoriów
// Design Pattern: Singleton Pattern, Facade Pattern

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "ExerciseRepository.h"
#include "WorkoutPlanRepository.h"
#include <memory>
#include <string>

// Wzorzec Singleton - tylko jedna instancja managera bazy danych w całej aplikacji
// Wzorzec Facade - uproszczony interfejs do zarządzania wieloma repozytoriami
class DatabaseManager {
private:
    // Prywatny konstruktor (Singleton)
    DatabaseManager();

    // Usunięcie copy constructor i assignment operator (Singleton)
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // Repozytoria
    std::unique_ptr<ExerciseRepository> exerciseRepo;
    std::unique_ptr<WorkoutPlanRepository> planRepo;

public:
    // Statyczna metoda dostępu do jedynej instancji (Singleton Pattern)
    static DatabaseManager& getInstance() {
        static DatabaseManager instance;  // Thread-safe w C++11+
        return instance;
    }

    // Destruktor
    ~DatabaseManager() = default;

    // === Dostęp do repozytoriów ===

    ExerciseRepository& getExerciseRepository() {
        return *exerciseRepo;
    }

    WorkoutPlanRepository& getWorkoutPlanRepository() {
        return *planRepo;
    }

    // === Operacje na całej bazie danych ===

    // Inicjalizacja - tworzenie folderów, ładowanie danych
    bool initialize();

    // Zapis wszystkich danych do plików
    bool saveAll();

    // Odczyt wszystkich danych z plików
    bool loadAll();

    // Wyczyszczenie całej bazy (UWAGA!)
    void clearAll();

    // Status bazy danych (do debugowania)
    void printStatus() const;
};

#endif // DATABASEMANAGER_H
