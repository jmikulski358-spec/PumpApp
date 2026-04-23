// WorkoutPlanRepository.cpp
// Lokalizacja: core/WorkoutPlanRepository.cpp

#include "WorkoutPlanRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

WorkoutPlanRepository::WorkoutPlanRepository(const std::string& filePath,
                                             ExerciseRepository* exRepo)
    : jsonFilePath(filePath), exerciseRepo(exRepo) {
}

void WorkoutPlanRepository::addPlan(std::shared_ptr<WorkoutPlan> plan) {
    if(!plan) {
        throw std::invalid_argument("Nie można dodać pustego planu!");
    }

    if(exists(plan->getName())) {
        throw std::runtime_error("Plan o nazwie '" + plan->getName() + "' już istnieje!");
    }

    plans.push_back(plan);
}

std::shared_ptr<WorkoutPlan> WorkoutPlanRepository::findByName(const std::string& name) const {
    auto it = std::find_if(plans.begin(), plans.end(),
                           [&name](const std::shared_ptr<WorkoutPlan>& p) {
                               return p->getName() == name;
                           });

    return (it != plans.end()) ? *it : nullptr;
}

std::vector<std::shared_ptr<WorkoutPlan>> WorkoutPlanRepository::searchByName(const std::string& query) const {
    std::vector<std::shared_ptr<WorkoutPlan>> results;

    for(const auto& plan : plans) {
        if(plan->getName().find(query) != std::string::npos) {
            results.push_back(plan);
        }
    }

    return results;
}

bool WorkoutPlanRepository::updatePlan(const std::string& oldName,
                                       std::shared_ptr<WorkoutPlan> newPlan) {
    auto it = std::find_if(plans.begin(), plans.end(),
                           [&oldName](const std::shared_ptr<WorkoutPlan>& p) {
                               return p->getName() == oldName;
                           });

    if(it != plans.end()) {
        *it = newPlan;
        return true;
    }
    return false;
}

bool WorkoutPlanRepository::removePlan(const std::string& name) {
    auto it = std::find_if(plans.begin(), plans.end(),
                           [&name](const std::shared_ptr<WorkoutPlan>& p) {
                               return p->getName() == name;
                           });

    if(it != plans.end()) {
        plans.erase(it);
        return true;
    }
    return false;
}

bool WorkoutPlanRepository::exists(const std::string& name) const {
    return findByName(name) != nullptr;
}

// === JSON Helpers ===

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

bool WorkoutPlanRepository::saveToJSON() const {
    std::ofstream file(jsonFilePath);
    if(!file.is_open()) {
        std::cerr << "Błąd: Nie można otworzyć pliku do zapisu: " << jsonFilePath << std::endl;
        return false;
    }

    file << "[\n";
    for(size_t i = 0; i < plans.size(); ++i) {
        const auto& plan = plans[i];
        file << "  {\n";
        file << "    \"name\": \"" << escapeJson(plan->getName()) << "\",\n";
        file << "    \"entries\": [\n";

        const auto& entries = plan->getEntries();
        for(size_t j = 0; j < entries.size(); ++j) {
            const auto& entry = entries[j];
            file << "      {\n";
            file << "        \"exerciseName\": \"" << escapeJson(entry.exercise->getName()) << "\",\n";
            file << "        \"sets\": " << entry.sets << ",\n";
            file << "        \"reps\": " << entry.reps << ",\n";
            file << "        \"weight\": " << entry.weight << ",\n";
            file << "        \"restTime\": " << entry.restTime << "\n";
            file << "      }";
            if(j < entries.size() - 1) file << ",";
            file << "\n";
        }

        file << "    ]\n";
        file << "  }";
        if(i < plans.size() - 1) file << ",";
        file << "\n";
    }
    file << "]\n";

    file.close();
    return true;
}

bool WorkoutPlanRepository::loadFromJSON() {
    if(!exerciseRepo) {
        std::cerr << "Błąd: Brak referencji do ExerciseRepository!" << std::endl;
        return false;
    }

    std::ifstream file(jsonFilePath);
    if(!file.is_open()) {
        std::cerr << "Plik nie istnieje lub nie można go otworzyć: " << jsonFilePath << std::endl;
        return false;
    }

    plans.clear();

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();

    // Prosty parser - zakładamy poprawny format
    std::shared_ptr<WorkoutPlan> currentPlan = nullptr;
    std::string planName;
    std::string exerciseName;
    int sets = 0, reps = 0, restTime = 0;
    double weight = 0.0;

    std::istringstream stream(content);
    std::string line;

    while(std::getline(stream, line)) {
        if(line.find("\"name\"") != std::string::npos && line.find("exerciseName") == std::string::npos) {
            // Nazwa planu
            size_t start = line.find(":") + 1;
            size_t first = line.find("\"", start) + 1;
            size_t last = line.find("\"", first);
            planName = unescapeJson(line.substr(first, last - first));
            currentPlan = std::make_shared<WorkoutPlan>(planName);
        }
        else if(line.find("\"exerciseName\"") != std::string::npos) {
            size_t start = line.find(":") + 1;
            size_t first = line.find("\"", start) + 1;
            size_t last = line.find("\"", first);
            exerciseName = unescapeJson(line.substr(first, last - first));
        }
        else if(line.find("\"sets\"") != std::string::npos) {
            size_t pos = line.find(":") + 1;
            sets = std::stoi(line.substr(pos));
        }
        else if(line.find("\"reps\"") != std::string::npos) {
            size_t pos = line.find(":") + 1;
            reps = std::stoi(line.substr(pos));
        }
        else if(line.find("\"weight\"") != std::string::npos) {
            size_t pos = line.find(":") + 1;
            weight = std::stod(line.substr(pos));
        }
        else if(line.find("\"restTime\"") != std::string::npos) {
            size_t pos = line.find(":") + 1;
            restTime = std::stoi(line.substr(pos));

            // Mamy kompletny entry, dodajemy do planu
            if(currentPlan) {
                auto exercise = exerciseRepo->findByName(exerciseName);
                if(exercise) {
                    currentPlan->addEntry(exercise, sets, reps, weight, restTime);
                } else {
                    std::cerr << "Ostrzeżenie: Nie znaleziono ćwiczenia '" << exerciseName << "'" << std::endl;
                }
            }
        }
        else if(line.find("]") != std::string::npos && line.find("entries") == std::string::npos) {
            // Koniec planu
            if(currentPlan && currentPlan->getEntryCount() > 0) {
                plans.push_back(currentPlan);
            }
            currentPlan = nullptr;
        }
    }

    return true;
}
