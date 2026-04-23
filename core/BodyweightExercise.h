// BodyweightExercise.h
// Lokalizacja: core/BodyweightExercise.h
// Opis: Klasa reprezentująca ćwiczenie bez obciążenia (np. pompki, przysiady)

#ifndef BODYWEIGHTEXERCISE_H
#define BODYWEIGHTEXERCISE_H

#include "Exercise.h"

class BodyweightExercise : public Exercise {
public:
    // Konstruktor
    BodyweightExercise(const std::string& name,
                       const std::string& desc,
                       const std::string& muscles);

    // Override metody abstrakcyjnej
    ExerciseType getType() const override {
        return ExerciseType::BODYWEIGHT;
    }

    // Implementacja klonowania (Prototype Pattern)
    std::unique_ptr<Exercise> clone() const override {
        return std::make_unique<BodyweightExercise>(*this);
    }
};

#endif // BODYWEIGHTEXERCISE_H
