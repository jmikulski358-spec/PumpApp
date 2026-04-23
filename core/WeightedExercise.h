// WeightedExercise.h
// Lokalizacja: core/WeightedExercise.h
// Opis: Klasa reprezentująca ćwiczenie z obciążeniem (np. wyciskanie sztangi)

#ifndef WEIGHTEDEXERCISE_H
#define WEIGHTEDEXERCISE_H

#include "Exercise.h"

class WeightedExercise : public Exercise {
public:
    // Konstruktor
    WeightedExercise(const std::string& name,
                     const std::string& desc,
                     const std::string& muscles);

    // Override metody abstrakcyjnej
    ExerciseType getType() const override {
        return ExerciseType::WEIGHTED;
    }

    // Implementacja klonowania (Prototype Pattern)
    std::unique_ptr<Exercise> clone() const override {
        return std::make_unique<WeightedExercise>(*this);
    }
};

#endif // WEIGHTEDEXERCISE_H
