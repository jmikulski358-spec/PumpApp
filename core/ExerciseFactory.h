// ExerciseFactory.h
// Lokalizacja: core/ExerciseFactory.h
// Opis: Fabryka do tworzenia odpowiednich typów ćwiczeń
// Design Pattern: Factory Method Pattern

#ifndef EXERCISEFACTORY_H
#define EXERCISEFACTORY_H

#include "Exercise.h"
#include "WeightedExercise.h"
#include "BodyweightExercise.h"
#include <memory>
#include <stdexcept>

// Wzorzec Factory Method - centralne miejsce do tworzenia obiektów
class ExerciseFactory {
public:
    // Statyczna metoda fabrykująca - tworzy odpowiedni typ ćwiczenia
    static std::unique_ptr<Exercise> createExercise(
        ExerciseType type,
        const std::string& name,
        const std::string& description,
        const std::string& muscles
        ) {
        switch(type) {
        case ExerciseType::BODYWEIGHT:
            return std::make_unique<BodyweightExercise>(name, description, muscles);
        case ExerciseType::WEIGHTED:
            return std::make_unique<WeightedExercise>(name, description, muscles);
        default:
            throw std::invalid_argument("Nieznany typ ćwiczenia!");
        }
    }
};

#endif // EXERCISEFACTORY_H
