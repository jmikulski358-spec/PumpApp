// test_main.cpp
// Lokalizacja: tests/test_main.cpp
// Autor: Jakub Krzysztof Mikulski (z pomocą Claude.ai)
// Opis: Testy jednostkowe dla projektu PumpApp (Google Test)

#include <gtest/gtest.h>
#include "../core/Exercise.h"
#include "../core/ExerciseFactory.h"
#include "../core/WeightedExercise.h"
#include "../core/BodyweightExercise.h"
#include "../core/WorkoutPlan.h"
#include "../core/ExerciseRepository.h"
#include <memory>

// ===== TEST 1: Factory Pattern - tworzenie ćwiczeń =====
TEST(ExerciseFactoryTest, CreateWeightedExercise) {
    // Test tworzenia ćwiczenia z obciążeniem
    auto exercise = ExerciseFactory::createExercise(
        ExerciseType::WEIGHTED,
        "Bench Press",
        "Lie on bench and press bar",
        "Chest"
        );

    ASSERT_NE(exercise, nullptr);
    EXPECT_EQ(exercise->getName(), "Bench Press");
    EXPECT_EQ(exercise->getType(), ExerciseType::WEIGHTED);
    EXPECT_EQ(exercise->getTargetMuscles(), "Chest");
}

TEST(ExerciseFactoryTest, CreateBodyweightExercise) {
    // Test tworzenia ćwiczenia bez obciążenia
    auto exercise = ExerciseFactory::createExercise(
        ExerciseType::BODYWEIGHT,
        "Push-ups",
        "Standard push-ups",
        "Chest, Triceps"
        );

    ASSERT_NE(exercise, nullptr);
    EXPECT_EQ(exercise->getName(), "Push-ups");
    EXPECT_EQ(exercise->getType(), ExerciseType::BODYWEIGHT);
}

// ===== TEST 2: Polimorfizm - klasy pochodne =====
TEST(ExerciseTest, PolymorphismWeighted) {
    // Test czy polimorfizm działa poprawnie
    std::shared_ptr<Exercise> exercise = std::make_shared<WeightedExercise>(
        "Squat", "Deep squat", "Legs"
        );

    EXPECT_EQ(exercise->getType(), ExerciseType::WEIGHTED);
    EXPECT_EQ(exercise->getName(), "Squat");
}

TEST(ExerciseTest, PolymorphismBodyweight) {
    std::shared_ptr<Exercise> exercise = std::make_shared<BodyweightExercise>(
        "Pull-ups", "Hang and pull", "Back"
        );

    EXPECT_EQ(exercise->getType(), ExerciseType::BODYWEIGHT);
    EXPECT_EQ(exercise->getName(), "Pull-ups");
}

// ===== TEST 3: WorkoutPlan - dodawanie/usuwanie ćwiczeń =====
TEST(WorkoutPlanTest, CreateEmptyPlan) {
    // Test tworzenia pustego planu
    WorkoutPlan plan("FBW Monday");

    EXPECT_EQ(plan.getName(), "FBW Monday");
    EXPECT_TRUE(plan.isEmpty());
    EXPECT_EQ(plan.getEntryCount(), 0);
}

TEST(WorkoutPlanTest, AddEntryToPlan) {
    // Test dodawania ćwiczenia do planu
    WorkoutPlan plan("Push Day");

    auto uniqueEx = ExerciseFactory::createExercise(
        ExerciseType::WEIGHTED, "Bench Press", "Press bar", "Chest"
        );
    std::shared_ptr<Exercise> exercise(std::move(uniqueEx)); // Konwersja unique_ptr → shared_ptr

    plan.addEntry(exercise, 4, 8, 80.0, 120);

    EXPECT_FALSE(plan.isEmpty());
    EXPECT_EQ(plan.getEntryCount(), 1);

    const auto& entries = plan.getEntries();
    EXPECT_EQ(entries[0].sets, 4);
    EXPECT_EQ(entries[0].reps, 8);
    EXPECT_EQ(entries[0].weight, 80.0);
    EXPECT_EQ(entries[0].restTime, 120);
}

TEST(WorkoutPlanTest, AddMultipleEntries) {
    // Test dodawania wielu ćwiczeń
    WorkoutPlan plan("Full Body");

    auto uniqueEx1 = ExerciseFactory::createExercise(
        ExerciseType::WEIGHTED, "Squat", "Squat deep", "Legs"
        );
    auto uniqueEx2 = ExerciseFactory::createExercise(
        ExerciseType::BODYWEIGHT, "Push-ups", "Standard", "Chest"
        );

    std::shared_ptr<Exercise> ex1(std::move(uniqueEx1));
    std::shared_ptr<Exercise> ex2(std::move(uniqueEx2));

    plan.addEntry(ex1, 5, 5, 100.0, 180);
    plan.addEntry(ex2, 3, 15, 0.0, 60);

    EXPECT_EQ(plan.getEntryCount(), 2);
}

TEST(WorkoutPlanTest, RemoveEntry) {
    // Test usuwania ćwiczenia z planu
    WorkoutPlan plan("Test Plan");

    auto uniqueEx = ExerciseFactory::createExercise(
        ExerciseType::BODYWEIGHT, "Dips", "Dip down", "Triceps"
        );
    std::shared_ptr<Exercise> ex(std::move(uniqueEx));

    plan.addEntry(ex, 3, 10, 0.0, 90);
    EXPECT_EQ(plan.getEntryCount(), 1);

    plan.removeEntry(0);
    EXPECT_TRUE(plan.isEmpty());
    EXPECT_EQ(plan.getEntryCount(), 0);
}

// ===== TEST 4: ExerciseRepository - CRUD operations =====
TEST(ExerciseRepositoryTest, AddExercise) {
    // Test dodawania ćwiczenia do repozytorium
    ExerciseRepository repo("test_exercises.json");
    repo.clear(); // Wyczyść na początek

    auto uniqueEx = ExerciseFactory::createExercise(
        ExerciseType::WEIGHTED, "Deadlift", "Lift bar", "Back"
        );
    std::shared_ptr<Exercise> exercise(std::move(uniqueEx));

    repo.addExercise(exercise);
    EXPECT_EQ(repo.getCount(), 1);
}

TEST(ExerciseRepositoryTest, FindByName) {
    // Test wyszukiwania ćwiczenia po nazwie
    ExerciseRepository repo("test_exercises2.json");
    repo.clear();

    auto uniqueEx = ExerciseFactory::createExercise(
        ExerciseType::BODYWEIGHT, "Plank", "Hold position", "Abs"
        );
    std::shared_ptr<Exercise> exercise(std::move(uniqueEx));

    repo.addExercise(exercise);

    auto found = repo.findByName("Plank");
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->getName(), "Plank");
    EXPECT_EQ(found->getTargetMuscles(), "Abs");
}

TEST(ExerciseRepositoryTest, FindNonExistent) {
    // Test wyszukiwania nieistniejącego ćwiczenia
    ExerciseRepository repo("test_exercises3.json");
    repo.clear();

    auto found = repo.findByName("NonExistent");
    EXPECT_EQ(found, nullptr);
}

TEST(ExerciseRepositoryTest, RemoveExercise) {
    // Test usuwania ćwiczenia
    ExerciseRepository repo("test_exercises4.json");
    repo.clear();

    auto uniqueEx = ExerciseFactory::createExercise(
        ExerciseType::WEIGHTED, "OHP", "Press overhead", "Shoulders"
        );
    std::shared_ptr<Exercise> ex(std::move(uniqueEx));

    repo.addExercise(ex);
    EXPECT_EQ(repo.getCount(), 1);

    bool removed = repo.removeExercise("OHP");
    EXPECT_TRUE(removed);
    EXPECT_EQ(repo.getCount(), 0);
}

// ===== TEST 5: Edge cases - walidacja =====
TEST(WorkoutPlanTest, InvalidSetsAndReps) {
    // Test czy rzuca wyjątek przy nieprawidłowych wartościach
    WorkoutPlan plan("Test");

    auto uniqueEx = ExerciseFactory::createExercise(
        ExerciseType::BODYWEIGHT, "Test", "Test", "Test"
        );
    std::shared_ptr<Exercise> ex(std::move(uniqueEx));

    // Serie i reps muszą być > 0
    EXPECT_THROW(plan.addEntry(ex, 0, 10, 0.0, 60), std::invalid_argument);
    EXPECT_THROW(plan.addEntry(ex, 3, 0, 0.0, 60), std::invalid_argument);
}

TEST(ExerciseRepositoryTest, DuplicateExercise) {
    // Test czy nie pozwala dodać duplikatu
    ExerciseRepository repo("test_duplicates.json");
    repo.clear();

    auto uniqueEx1 = ExerciseFactory::createExercise(
        ExerciseType::WEIGHTED, "Squat", "Desc", "Legs"
        );
    auto uniqueEx2 = ExerciseFactory::createExercise(
        ExerciseType::WEIGHTED, "Squat", "Different desc", "Legs"
        );

    std::shared_ptr<Exercise> ex1(std::move(uniqueEx1));
    std::shared_ptr<Exercise> ex2(std::move(uniqueEx2));

    repo.addExercise(ex1);
    EXPECT_THROW(repo.addExercise(ex2), std::runtime_error);
}

// ===== MAIN - uruchomienie testów =====
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
