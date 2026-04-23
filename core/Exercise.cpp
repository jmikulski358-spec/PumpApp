// Exercise.cpp
// Lokalizacja: core/Exercise.cpp
// Implementacja klasy Exercise

#include "Exercise.h"
#include <stdexcept>

Exercise::Exercise(const std::string& name,
                   const std::string& desc,
                   const std::string& muscles)
    : name(name), description(desc), targetMuscles(muscles) {
}

// Konwersja typu ćwiczenia na string (do zapisu JSON)
std::string Exercise::typeToString(ExerciseType type) {
    switch(type) {
    case ExerciseType::BODYWEIGHT: return "bodyweight";
    case ExerciseType::WEIGHTED: return "weighted";
    default: return "unknown";
    }
}

// Konwersja string na typ ćwiczenia (z odczytu JSON)
ExerciseType Exercise::stringToType(const std::string& typeStr) {
    if(typeStr == "bodyweight") return ExerciseType::BODYWEIGHT;
    if(typeStr == "weighted") return ExerciseType::WEIGHTED;
    throw std::invalid_argument("Nieznany typ ćwiczenia: " + typeStr);
}
