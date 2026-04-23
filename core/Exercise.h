// Exercise.h
// Lokalizacja: core/Exercise.h
// Autor: Jakub Krzysztof Mikulski
// Data: 2025-01-23
// Opis: Abstrakcyjna klasa bazowa dla wszystkich ćwiczeń

#ifndef EXERCISE_H
#define EXERCISE_H

#include <string>
#include <memory>

// Enum definiujący typ ćwiczenia
enum class ExerciseType {
    BODYWEIGHT,  // Ćwiczenie bez obciążenia (np. pompki, brzuszki)
    WEIGHTED     // Ćwiczenie z obciążeniem (np. wyciskanie sztangi)
};

// Abstrakcyjna klasa bazowa - wzorzec Template Method
class Exercise {
protected:
    std::string name;           // Nazwa ćwiczenia
    std::string description;    // Opis jak wykonać ćwiczenie
    std::string targetMuscles;  // Mięśnie zaangażowane

public:
    // Konstruktor
    Exercise(const std::string& name,
             const std::string& desc,
             const std::string& muscles);

    // Wirtualny destruktor (WAŻNE dla poprawnego polimorfizmu!)
    virtual ~Exercise() = default;

    // Czysto wirtualna metoda - każda klasa pochodna MUSI ją zaimplementować
    // Wzorzec: Template Method Pattern
    virtual ExerciseType getType() const = 0;

    // Metoda do klonowania obiektu (Deep Copy)
    // Wzorzec: Prototype Pattern
    virtual std::unique_ptr<Exercise> clone() const = 0;

    // Gettery
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    std::string getTargetMuscles() const { return targetMuscles; }

    // Settery
    void setName(const std::string& n) { name = n; }
    void setDescription(const std::string& d) { description = d; }
    void setTargetMuscles(const std::string& m) { targetMuscles = m; }

    // Pomocnicza funkcja do konwersji typu na string
    static std::string typeToString(ExerciseType type);
    static ExerciseType stringToType(const std::string& typeStr);
};

#endif // EXERCISE_H
