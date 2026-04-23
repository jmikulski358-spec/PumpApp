// MainWindow.h
// Lokalizacja: gui/MainWindow.h
// Opis: Główne okno aplikacji PumpApp z zakładkami

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <memory>
#include "../core/DatabaseManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // === SLOTY DLA ZAKŁADKI "ĆWICZENIA" ===
    void onAddExercise();
    void onEditExercise();
    void onDeleteExercise();
    void onSearchExercises();
    void onExerciseSelectionChanged();

    // === SLOTY DLA ZAKŁADKI "PLANY TRENINGOWE" ===
    void onAddPlan();
    void onEditPlan();
    void onDeletePlan();
    void onViewPlan();
    void onSearchPlans();
    void onPlanSelectionChanged();

private:
    Ui::MainWindow *ui;
    DatabaseManager& db;

    void setupConnections();
    void refreshExerciseList();
    void refreshPlanList();
    void updateExerciseButtons();
    void updatePlanButtons();
    void saveDatabase();
};

#endif // MAINWINDOW_H
