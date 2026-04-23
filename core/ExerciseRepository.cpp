// ExerciseRepository.cpp
// Lokalizacja: core/ExerciseRepository.cpp
// UWAGA: Używamy prostego JSON parsingu (bez zewnętrznych bibliotek)

#include "ExerciseRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>

ExerciseRepository::ExerciseRepository(const std::string& filePath)
    : jsonFilePath(filePath) {
}

void ExerciseRepository::addExercise(std::shared_ptr<Exercise> exercise) {
    if(!exercise) {
        throw std::invalid_argument("Nie można dodać pustego ćwiczenia!");
    }

    // Sprawdzenie czy ćwiczenie o takiej nazwie już istnieje
    if(exists(exercise->getName())) {
        throw std::runtime_error("Ćwiczenie o nazwie '" + exercise->getName() + "' już istnieje!");
    }

    exercises.push_back(exercise);
}

std::shared_ptr<Exercise> ExerciseRepository::findByName(const std::string& name) const {
    auto it = std::find_if(exercises.begin(), exercises.end(),
                           [&name](const std::shared_ptr<Exercise>& ex) {
                               return ex->getName() == name;
                           });

    return (it != exercises.end()) ? *it : nullptr;
}

std::vector<std::shared_ptr<Exercise>> ExerciseRepository::searchByName(const std::string& query) const {
    std::vector<std::shared_ptr<Exercise>> results;

    // Szukamy ćwiczeń zawierających query w nazwie (case-insensitive byłoby fajne, ale upraszczamy)
    for(const auto& ex : exercises) {
        if(ex->getName().find(query) != std::string::npos) {
            results.push_back(ex);
        }
    }

    return results;
}

bool ExerciseRepository::updateExercise(const std::string& oldName,
                                        std::shared_ptr<Exercise> newExercise) {
    auto it = std::find_if(exercises.begin(), exercises.end(),
                           [&oldName](const std::shared_ptr<Exercise>& ex) {
                               return ex->getName() == oldName;
                           });

    if(it != exercises.end()) {
        *it = newExercise;
        return true;
    }
    return false;
}

bool ExerciseRepository::removeExercise(const std::string& name) {
    auto it = std::find_if(exercises.begin(), exercises.end(),
                           [&name](const std::shared_ptr<Exercise>& ex) {
                               return ex->getName() == name;
                           });

    if(it != exercises.end()) {
        exercises.erase(it);
        return true;
    }
    return false;
}

bool ExerciseRepository::exists(const std::string& name) const {
    return findByName(name) != nullptr;
}

// === JSON Persistence - PROSTY WŁASNY PARSER ===

// Helper: escape string dla JSON
static std::string escapeJson(const std::string& str) {
    std::string result;
    for(char c : str) {
        if(c == '"') result += "\\\"";
        else if(c == '\\') result += "\\\\";
        else if(c == '\n') result += "\\n";
        else if(c == '\r') result += "\\r";
        else if(c == '\t') result += "\\t";
        else result += c;
    }
    return result;
}

// Helper: unescape string z JSON
static std::string unescapeJson(const std::string& str) {
    std::string result;
    for(size_t i = 0; i < str.size(); ++i) {
        if(str[i] == '\\' && i + 1 < str.size()) {
            char next = str[i + 1];
            if(next == 'n') { result += '\n'; i++; }
            else if(next == 'r') { result += '\r'; i++; }
            else if(next == 't') { result += '\t'; i++; }
            else if(next == '"') { result += '"'; i++; }
            else if(next == '\\') { result += '\\'; i++; }
            else result += str[i];
        } else {
            result += str[i];
        }
    }
    return result;
}

bool ExerciseRepository::saveToJSON() const {
    std::ofstream file(jsonFilePath);
    if(!file.is_open()) {
        std::cerr << "Błąd: Nie można otworzyć pliku do zapisu: " << jsonFilePath << std::endl;
        return false;
    }

    file << "[\n";
    for(size_t i = 0; i < exercises.size(); ++i) {
        const auto& ex = exercises[i];
        file << "  {\n";
        file << "    \"name\": \"" << escapeJson(ex->getName()) << "\",\n";
        file << "    \"description\": \"" << escapeJson(ex->getDescription()) << "\",\n";
        file << "    \"muscles\": \"" << escapeJson(ex->getTargetMuscles()) << "\",\n";
        file << "    \"type\": \"" << Exercise::typeToString(ex->getType()) << "\"\n";
        file << "  }";
        if(i < exercises.size() - 1) file << ",";
        file << "\n";
    }
    file << "]\n";

    file.close();
    return true;
}

bool ExerciseRepository::loadFromJSON() {
    std::ifstream file(jsonFilePath);
    if(!file.is_open()) {
        std::cerr << "Plik nie istnieje lub nie można go otworzyć: " << jsonFilePath << std::endl;
        return false;
    }

    exercises.clear();

    std::string line;
    std::string name, desc, muscles, typeStr;

    // Prosty parser JSON - zakładamy poprawny format!
    while(std::getline(file, line)) {
        // Szukamy kluczy
        if(line.find("\"name\"") != std::string::npos) {
            size_t start = line.find(":") + 1;
            size_t first = line.find("\"", start) + 1;
            size_t last = line.find("\"", first);
            name = unescapeJson(line.substr(first, last - first));
        }
        else if(line.find("\"description\"") != std::string::npos) {
            size_t start = line.find(":") + 1;
            size_t first = line.find("\"", start) + 1;
            size_t last = line.find("\"", first);
            desc = unescapeJson(line.substr(first, last - first));
        }
        else if(line.find("\"muscles\"") != std::string::npos) {
            size_t start = line.find(":") + 1;
            size_t first = line.find("\"", start) + 1;
            size_t last = line.find("\"", first);
            muscles = unescapeJson(line.substr(first, last - first));
        }
        else if(line.find("\"type\"") != std::string::npos) {
            size_t start = line.find(":") + 1;
            size_t first = line.find("\"", start) + 1;
            size_t last = line.find("\"", first);
            typeStr = line.substr(first, last - first);

            // Teraz mamy kompletny obiekt, tworzymy ćwiczenie
            try {
                ExerciseType type = Exercise::stringToType(typeStr);
                auto exercise = ExerciseFactory::createExercise(type, name, desc, muscles);
                exercises.push_back(std::move(exercise));
            } catch(const std::exception& e) {
                std::cerr << "Błąd parsowania ćwiczenia: " << e.what() << std::endl;
            }
        }
    }

    file.close();
    return true;
}
