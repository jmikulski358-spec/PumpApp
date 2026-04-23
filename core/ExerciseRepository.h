// ExerciseRepository.h
// Lokalizacja: core/ExerciseRepository.h
// Opis: Repository Pattern - zarządzanie kolekcją ćwiczeń
// Design Pattern: Repository Pattern, Singleton Pattern

#ifndef EXERCISEREPOSITORY_H
#define EXERCISEREPOSITORY_H

#include "Exercise.h"
#include "ExerciseFactory.h"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

// Wzorzec Repository - separacja logiki dostępu do danych od logiki biznesowej
class ExerciseRepository {
private:
    std::vector<std::shared_ptr<Exercise>> exercises;  // Kontener ćwiczeń
    std::string jsonFilePath;                          // Ścieżka do pliku JSON

public:
    // Konstruktor
    explicit ExerciseRepository(const std::string& filePath = "data/exercises.json");

    // === CRUD Operations ===

    // Create - dodanie nowego ćwiczenia
    void addExercise(std::shared_ptr<Exercise> exercise);

    // Read - pobranie wszystkich ćwiczeń
    const std::vector<std::shared_ptr<Exercise>>& getAllExercises() const {
        return exercises;
    }

    // Read - znalezienie ćwiczenia po nazwie
    std::shared_ptr<Exercise> findByName(const std::string& name) const;

    // Read - wyszukiwanie ćwiczeń po fragmencie nazwy (search)
    std::vector<std::shared_ptr<Exercise>> searchByName(const std::string& query) const;

    // Update - aktualizacja ćwiczenia
    bool updateExercise(const std::string& oldName, std::shared_ptr<Exercise> newExercise);

    // Delete - usunięcie ćwiczenia po nazwie
    bool removeExercise(const std::string& name);

    // === Persistence (JSON) ===

    // Zapis do pliku JSON
    bool saveToJSON() const;

    // Odczyt z pliku JSON
    bool loadFromJSON();

    // Liczba ćwiczeń w repozytorium
    size_t getCount() const { return exercises.size(); }

    // Sprawdzenie czy istnieje ćwiczenie o danej nazwie
    bool exists(const std::string& name) const;

    // Wyczyszczenie całego repozytorium
    void clear() { exercises.clear(); }
};

#endif // EXERCISEREPOSITORY_H
