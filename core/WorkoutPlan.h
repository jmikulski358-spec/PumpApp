// WorkoutPlan.h
// Lokalizacja: core/WorkoutPlan.h
// Opis: Klasa reprezentująca plan treningowy składający się z ćwiczeń

#ifndef WORKOUTPLAN_H
#define WORKOUTPLAN_H

#include "Exercise.h"
#include <string>
#include <vector>
#include <memory>

// Struktura reprezentująca pojedynczy wpis w planie treningowym
// Łączy ćwiczenie z parametrami treningowymi (serie, powtórzenia, etc.)
struct PlanEntry {
    std::shared_ptr<Exercise> exercise = nullptr;  // Wskaźnik do ćwiczenia
    int sets = 0;                            // Liczba serii
    int reps = 0;                            // Liczba powtórzeń w serii
    double weight = 0.0;                       // Ciężar (kg), 0 dla bodyweight
    int restTime = 0;                        // Czas odpoczynku między seriami (sekundy)

    // Konstruktor
    PlanEntry(std::shared_ptr<Exercise> ex,
              int s, int r, double w, int rest)
        : exercise(ex), sets(s), reps(r), weight(w), restTime(rest) {}
};

// Klasa reprezentująca cały plan treningowy
class WorkoutPlan {
private:
    std::string name;                    // Nazwa planu (np. "Trening FBW")
    std::vector<PlanEntry> entries;      // Lista ćwiczeń w planie

public:
    // Konstruktor
    explicit WorkoutPlan(const std::string& planName);

    // Dodanie ćwiczenia do planu
    void addEntry(std::shared_ptr<Exercise> exercise,
                  int sets, int reps, double weight, int restTime);

    // Usunięcie ćwiczenia z planu (po indeksie)
    void removeEntry(size_t index);

    // Edycja parametrów ćwiczenia (po indeksie)
    void editEntry(size_t index, int sets, int reps, double weight, int restTime);

    // Gettery
    std::string getName() const { return name; }
    const std::vector<PlanEntry>& getEntries() const { return entries; }
    size_t getEntryCount() const { return entries.size(); }

    // Settery
    void setName(const std::string& n) { name = n; }

    // Sprawdzenie czy plan jest pusty
    bool isEmpty() const { return entries.empty(); }

    // Wyczyszczenie całego planu
    void clear() { entries.clear(); }
};

#endif // WORKOUTPLAN_H
