// WorkoutPlanDialog.h
// Lokalizacja: gui/WorkoutPlanDialog.h
// Autor: Jakub Krzysztof Mikulski
// Opis: Dialog do tworzenia/edycji planów treningowych

#ifndef WORKOUTPLANDIALOG_H
#define WORKOUTPLANDIALOG_H

#include <QDialog>
#include <memory>
#include "../core/WorkoutPlan.h"
#include "../core/DatabaseManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class WorkoutPlanDialog;
}
QT_END_NAMESPACE

class WorkoutPlanDialog : public QDialog
{
    Q_OBJECT

public:
    // Konstruktor - dodawanie nowego planu
    explicit WorkoutPlanDialog(QWidget *parent, DatabaseManager* dbManager);
    
    // Konstruktor - edycja istniejącego planu
    WorkoutPlanDialog(QWidget *parent, DatabaseManager* dbManager, std::shared_ptr<WorkoutPlan> planToEdit);
    
    // Destruktor
    ~WorkoutPlanDialog();

    // Pobranie stworzonego/edytowanego planu
    std::shared_ptr<WorkoutPlan> getWorkoutPlan() const;

private slots:
    void onAddExerciseToPlan();
    void onRemoveExerciseFromPlan();
    void onAccept();
    void onReject();
    void onPlanEntrySelectionChanged();

private:
    Ui::WorkoutPlanDialog *ui;
    std::shared_ptr<WorkoutPlan> plan = nullptr;
    DatabaseManager* db = nullptr;
    bool editMode = false;

    void setupUI();
    void loadAvailableExercises();
    void loadPlanData();
    void refreshPlanEntries();
    bool validateInput();
};

#endif // WORKOUTPLANDIALOG_H
