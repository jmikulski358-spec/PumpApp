// WorkoutPlan.cpp
// Lokalizacja: core/WorkoutPlan.cpp

#include "WorkoutPlan.h"
#include <stdexcept>

WorkoutPlan::WorkoutPlan(const std::string& planName)
    : name(planName) {
}

void WorkoutPlan::addEntry(std::shared_ptr<Exercise> exercise,
                           int sets, int reps, double weight, int restTime) {
    if(!exercise) {
        throw std::invalid_argument("Nie można dodać pustego ćwiczenia!");
    }
    if(sets <= 0 || reps <= 0) {
        throw std::invalid_argument("Serie i powtórzenia muszą być większe od 0!");
    }

    entries.emplace_back(exercise, sets, reps, weight, restTime);
}

void WorkoutPlan::removeEntry(size_t index) {
    if(index >= entries.size()) {
        throw std::out_of_range("Nieprawidłowy indeks ćwiczenia!");
    }
    entries.erase(entries.begin() + index);
}

void WorkoutPlan::editEntry(size_t index, int sets, int reps,
                            double weight, int restTime) {
    if(index >= entries.size()) {
        throw std::out_of_range("Nieprawidłowy indeks ćwiczenia!");
    }
    if(sets <= 0 || reps <= 0) {
        throw std::invalid_argument("Serie i powtórzenia muszą być większe od 0!");
    }

    entries[index].sets = sets;
    entries[index].reps = reps;
    entries[index].weight = weight;
    entries[index].restTime = restTime;
}
