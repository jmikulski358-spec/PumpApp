// WorkoutPlanRepository.h
// Lokalizacja: core/WorkoutPlanRepository.h
// Opis: Repository Pattern - zarządzanie kolekcją planów treningowych

#ifndef WORKOUTPLANREPOSITORY_H
#define WORKOUTPLANREPOSITORY_H

#include "WorkoutPlan.h"
#include "ExerciseRepository.h"
#include <vector>
#include <memory>
#include <string>

// Repository dla planów treningowych
class WorkoutPlanRepository {
private:
    std::vector<std::shared_ptr<WorkoutPlan>> plans;  // Kontener planów
    std::string jsonFilePath;                         // Ścieżka do pliku JSON
    ExerciseRepository* exerciseRepo;                 // Referencja do repo ćwiczeń (potrzebne do odczytu JSON)

public:
    // Konstruktor
    explicit WorkoutPlanRepository(const std::string& filePath = "data/plans.json",
                                   ExerciseRepository* exRepo = nullptr);

    // Ustawienie referencji do ExerciseRepository (potrzebne przed loadFromJSON)
    void setExerciseRepository(ExerciseRepository* repo) { exerciseRepo = repo; }

    // === CRUD Operations ===

    // Create - dodanie nowego planu
    void addPlan(std::shared_ptr<WorkoutPlan> plan);

    // Read - pobranie wszystkich planów
    const std::vector<std::shared_ptr<WorkoutPlan>>& getAllPlans() const {
        return plans;
    }

    // Read - znalezienie planu po nazwie
    std::shared_ptr<WorkoutPlan> findByName(const std::string& name) const;

    // Read - wyszukiwanie planów po fragmencie nazwy
    std::vector<std::shared_ptr<WorkoutPlan>> searchByName(const std::string& query) const;

    // Update - aktualizacja planu
    bool updatePlan(const std::string& oldName, std::shared_ptr<WorkoutPlan> newPlan);

    // Delete - usunięcie planu po nazwie
    bool removePlan(const std::string& name);

    // === Persistence (JSON) ===

    // Zapis do pliku JSON
    bool saveToJSON() const;

    // Odczyt z pliku JSON (wymaga wcześniej ustawionego exerciseRepo!)
    bool loadFromJSON();

    // Liczba planów w repozytorium
    size_t getCount() const { return plans.size(); }

    // Sprawdzenie czy istnieje plan o danej nazwie
    bool exists(const std::string& name) const;

    // Wyczyszczenie całego repozytorium
    void clear() { plans.clear(); }
};

#endif // WORKOUTPLANREPOSITORY_H
