// DatabaseManager.cpp
// Lokalizacja: core/DatabaseManager.cpp

#include "DatabaseManager.h"
#include <iostream>
#include <filesystem>
#include <QCoreApplication>
#include <QString>
#include <QDir>


// Namespace dla filesystem (C++17)
namespace fs = std::filesystem;

DatabaseManager::DatabaseManager() {
    // Ścieżka do working directory (katalog roboczy)
    QString workDir = QDir::currentPath();
    QString dataDir = workDir + "/data";

    // Tworzenie repozytoriów z pełnymi ścieżkami
    exerciseRepo = std::make_unique<ExerciseRepository>(
        (dataDir + "/exercises.json").toStdString()
        );
    planRepo = std::make_unique<WorkoutPlanRepository>(
        (dataDir + "/plans.json").toStdString(),
        exerciseRepo.get()
        );

    // Powiązanie planRepo z exerciseRepo
    planRepo->setExerciseRepository(exerciseRepo.get());
}

bool DatabaseManager::initialize() {
    std::cout << "[DatabaseManager] Inicjalizacja bazy danych..." << std::endl;

    // Ścieżka do folderu data/ (w working directory)
    QString workDir = QDir::currentPath();
    QString dataDir = workDir + "/data";

    std::cout << "[DatabaseManager] Working directory: "
              << workDir.toStdString() << std::endl;  // <- DEBUG!

    // Sprawdzenie/utworzenie folderu data/
    QDir dir;
    if(!dir.exists(dataDir)) {
        if(dir.mkpath(dataDir)) {
            std::cout << "[DatabaseManager] Utworzono folder: "
                      << dataDir.toStdString() << std::endl;
        } else {
            std::cerr << "[DatabaseManager] Błąd tworzenia folderu!" << std::endl;
            return false;
        }
    } else {
        std::cout << "[DatabaseManager] Folder data/ już istnieje: "
                  << dataDir.toStdString() << std::endl;
    }

    // Próba załadowania danych
    return loadAll();
}


bool DatabaseManager::saveAll() {
    std::cout << "[DatabaseManager] Zapisywanie danych..." << std::endl;

    bool success = true;

    // Zapis ćwiczeń
    if(!exerciseRepo->saveToJSON()) {
        std::cerr << "[DatabaseManager] Błąd zapisu ćwiczeń!" << std::endl;
        success = false;
    } else {
        std::cout << "[DatabaseManager] Zapisano " << exerciseRepo->getCount() << " ćwiczeń" << std::endl;
    }

    // Zapis planów
    if(!planRepo->saveToJSON()) {
        std::cerr << "[DatabaseManager] Błąd zapisu planów!" << std::endl;
        success = false;
    } else {
        std::cout << "[DatabaseManager] Zapisano " << planRepo->getCount() << " planów" << std::endl;
    }

    return success;
}

bool DatabaseManager::loadAll() {
    std::cout << "[DatabaseManager] Ładowanie danych..." << std::endl;

    bool success = true;

    QString workDir = QDir::currentPath();
    QFile exFile(workDir + "/data/exercises.json");
    QFile plFile(workDir + "/data/plans.json");

    // Odczyt ćwiczeń (MUSI być PRZED planami!)
    if(!exerciseRepo->loadFromJSON()) {
        std::cerr << "[DatabaseManager] Nie można załadować ćwiczeń (plik może nie istnieć)" << std::endl;
        success = false;
    } else {
        std::cout << "[DatabaseManager] Załadowano " << exerciseRepo->getCount() << " ćwiczeń" << std::endl;
    }

    // Odczyt planów
    if(!planRepo->loadFromJSON()) {
        std::cerr << "[DatabaseManager] Nie można załadować planów (plik może nie istnieć)" << std::endl;
        success = false;
    } else {
        std::cout << "[DatabaseManager] Załadowano " << planRepo->getCount() << " planów" << std::endl;
    }

    return success;
}

void DatabaseManager::clearAll() {
    std::cout << "[DatabaseManager] UWAGA: Czyszczenie całej bazy danych!" << std::endl;
    exerciseRepo->clear();
    planRepo->clear();
}

void DatabaseManager::printStatus() const {
    std::cout << "\n=== STATUS BAZY DANYCH ===" << std::endl;
    std::cout << "Liczba ćwiczeń: " << exerciseRepo->getCount() << std::endl;
    std::cout << "Liczba planów: " << planRepo->getCount() << std::endl;
    std::cout << "========================\n" << std::endl;
}
