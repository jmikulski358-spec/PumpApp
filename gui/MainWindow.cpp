// MainWindow.cpp
// Lokalizacja: gui/MainWindow.cpp

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include "ExerciseDialog.h"
#include "WorkoutPlanDialog.h"
#include "ui_ExerciseDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db(DatabaseManager::getInstance())
{
    ui->setupUi(this);

    setWindowTitle("PumpApp - Gym Tracker");

    // Polskie nazwy zakładek
    ui->tabWidget->setTabText(0, QString::fromUtf8("Ćwiczenia"));
    ui->tabWidget->setTabText(1, QString::fromUtf8("Plany treningowe"));

    // Polskie teksty przycisków - zakładka Exercises
    ui->btnAddExercise->setText(QString::fromUtf8("Dodaj"));
    ui->btnEditExercise->setText(QString::fromUtf8("Edytuj"));
    ui->btnDeleteExercise->setText(QString::fromUtf8("Usuń"));
    ui->lineSearchExercises->setPlaceholderText(QString::fromUtf8("Szukaj ćwiczenia..."));

    // Polskie teksty przycisków - zakładka Plans
    ui->btnAddPlan->setText(QString::fromUtf8("Dodaj"));
    ui->btnEditPlan->setText(QString::fromUtf8("Edytuj"));
    ui->btnDeletePlan->setText(QString::fromUtf8("Usuń"));
    ui->btnViewPlan->setText(QString::fromUtf8("Podgląd"));
    ui->lineSearchPlans->setPlaceholderText(QString::fromUtf8("Szukaj planu..."));

    setupConnections();
    refreshExerciseList();
    refreshPlanList();
    updateExerciseButtons();
    updatePlanButtons();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupConnections() {
    connect(ui->btnAddExercise, &QPushButton::clicked, this, &MainWindow::onAddExercise);
    connect(ui->btnEditExercise, &QPushButton::clicked, this, &MainWindow::onEditExercise);
    connect(ui->btnDeleteExercise, &QPushButton::clicked, this, &MainWindow::onDeleteExercise);
    connect(ui->lineSearchExercises, &QLineEdit::textChanged, this, &MainWindow::onSearchExercises);
    connect(ui->listExercises, &QListWidget::itemSelectionChanged, this, &MainWindow::onExerciseSelectionChanged);

    connect(ui->btnAddPlan, &QPushButton::clicked, this, &MainWindow::onAddPlan);
    connect(ui->btnEditPlan, &QPushButton::clicked, this, &MainWindow::onEditPlan);
    connect(ui->btnDeletePlan, &QPushButton::clicked, this, &MainWindow::onDeletePlan);
    connect(ui->btnViewPlan, &QPushButton::clicked, this, &MainWindow::onViewPlan);
    connect(ui->lineSearchPlans, &QLineEdit::textChanged, this, &MainWindow::onSearchPlans);
    connect(ui->listPlans, &QListWidget::itemSelectionChanged, this, &MainWindow::onPlanSelectionChanged);
}

void MainWindow::refreshExerciseList() {
    ui->listExercises->clear();

    const auto& exercises = db.getExerciseRepository().getAllExercises();
    for(const auto& ex : exercises) {
        QString typeIcon = (ex->getType() == ExerciseType::WEIGHTED) ? QString::fromUtf8("🏋️") : QString::fromUtf8("💪");
        QString itemText = QString("%1 %2").arg(typeIcon, QString::fromStdString(ex->getName()));
        ui->listExercises->addItem(itemText);
    }
}

void MainWindow::refreshPlanList() {
    ui->listPlans->clear();

    const auto& plans = db.getWorkoutPlanRepository().getAllPlans();
    for(const auto& plan : plans) {
        QString itemText = QString::fromUtf8("📋 %1 (%2 ćwiczeń)")
                               .arg(QString::fromStdString(plan->getName()))
                               .arg(plan->getEntryCount());
        ui->listPlans->addItem(itemText);
    }
}

void MainWindow::onExerciseSelectionChanged() {
    updateExerciseButtons();
}

void MainWindow::updateExerciseButtons() {
    bool hasSelection = ui->listExercises->currentItem() != nullptr;
    ui->btnEditExercise->setEnabled(hasSelection);
    ui->btnDeleteExercise->setEnabled(hasSelection);
}

void MainWindow::onPlanSelectionChanged() {
    updatePlanButtons();
}

void MainWindow::updatePlanButtons() {
    bool hasSelection = ui->listPlans->currentItem() != nullptr;
    ui->btnEditPlan->setEnabled(hasSelection);
    ui->btnDeletePlan->setEnabled(hasSelection);
    ui->btnViewPlan->setEnabled(hasSelection);
}

void MainWindow::saveDatabase() {
    if(!db.saveAll()) {
        QMessageBox::warning(this, QString::fromUtf8("Błąd"), QString::fromUtf8("Nie udało się zapisać danych!"));
    }
}

// === TYMCZASOWE IMPLEMENTACJE (zadziałają po dodaniu dialogów) ===

void MainWindow::onAddExercise() {
    ExerciseDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
        auto exercise = dialog.getExercise();
        try {
            db.getExerciseRepository().addExercise(exercise);
            refreshExerciseList();
            saveDatabase();
            QMessageBox::information(this, QString::fromUtf8("Sukces"),
                                     QString::fromUtf8("Ćwiczenie dodane!"));
        } catch(const std::exception& e) {
            QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                                 QString::fromUtf8("Nie można dodać ćwiczenia:\n") + e.what());
        }
    }
}

void MainWindow::onEditExercise() {
    QListWidgetItem* item = ui->listExercises->currentItem();
    if(!item) return;

    QString fullText = item->text();
    QString exName = fullText.mid(fullText.indexOf(' ') + 1);

    auto exercise = db.getExerciseRepository().findByName(exName.toStdString());
    if(!exercise) {
        QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                             QString::fromUtf8("Nie znaleziono ćwiczenia!"));
        return;
    }

    ExerciseDialog dialog(this, exercise);
    if(dialog.exec() == QDialog::Accepted) {
        auto updatedExercise = dialog.getExercise();
        db.getExerciseRepository().updateExercise(exName.toStdString(), updatedExercise);
        refreshExerciseList();
        saveDatabase();
        QMessageBox::information(this, QString::fromUtf8("Sukces"),
                                 QString::fromUtf8("Ćwiczenie zaktualizowane!"));
    }
}

void MainWindow::onDeleteExercise() {
    QListWidgetItem* item = ui->listExercises->currentItem();
    if(!item) return;

    QString fullText = item->text();
    QString exName = fullText.mid(fullText.indexOf(' ') + 1);

    auto reply = QMessageBox::question(this, QString::fromUtf8("Potwierdzenie"),
                                       QString::fromUtf8("Czy na pewno usunąć ćwiczenie '%1'?").arg(exName),
                                       QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes) {
        db.getExerciseRepository().removeExercise(exName.toStdString());
        refreshExerciseList();
        saveDatabase();
        QMessageBox::information(this, QString::fromUtf8("Sukces"), QString::fromUtf8("Ćwiczenie usunięte!"));
    }
}

void MainWindow::onSearchExercises() {
    QString query = ui->lineSearchExercises->text();

    if(query.isEmpty()) {
        refreshExerciseList();
        return;
    }

    ui->listExercises->clear();
    auto results = db.getExerciseRepository().searchByName(query.toStdString());

    for(const auto& ex : results) {
        QString typeIcon = (ex->getType() == ExerciseType::WEIGHTED) ? QString::fromUtf8("🏋️") : QString::fromUtf8("💪");
        QString itemText = QString("%1 %2").arg(typeIcon, QString::fromStdString(ex->getName()));
        ui->listExercises->addItem(itemText);
    }
}

void MainWindow::onAddPlan() {
    WorkoutPlanDialog dialog(this, &db);
    if (dialog.exec() == QDialog::Accepted) {
        auto plan = dialog.getWorkoutPlan();
        try {
            db.getWorkoutPlanRepository().addPlan(plan);
            refreshPlanList();
            saveDatabase();
            QMessageBox::information(this, QString::fromUtf8("Sukces"),
                                     QString::fromUtf8("Plan treningowy dodany!"));
        } catch (const std::exception& e) {
            QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                                 QString::fromUtf8("Nie można dodać planu:\n") + e.what());
        }
    }
}

void MainWindow::onEditPlan() {
    QListWidgetItem* item = ui->listPlans->currentItem();
    if (!item) return;

    QString fullText = item->text();
    int start = fullText.indexOf(' ') + 1;
    int end = fullText.indexOf(" (");
    QString planName = fullText.mid(start, end - start);

    auto plan = db.getWorkoutPlanRepository().findByName(planName.toStdString());
    if (!plan) {
        QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                             QString::fromUtf8("Nie znaleziono planu!"));
        return;
    }

    WorkoutPlanDialog dialog(this, &db, plan);
    if (dialog.exec() == QDialog::Accepted) {
        auto updatedPlan = dialog.getWorkoutPlan();
        db.getWorkoutPlanRepository().updatePlan(planName.toStdString(), updatedPlan);
        refreshPlanList();
        saveDatabase();
        QMessageBox::information(this, QString::fromUtf8("Sukces"),
                                 QString::fromUtf8("Plan zaktualizowany!"));
    }
}

void MainWindow::onDeletePlan() {
    QListWidgetItem* item = ui->listPlans->currentItem();
    if(!item) return;

    QString fullText = item->text();
    int start = fullText.indexOf(' ') + 1;
    int end = fullText.indexOf(" (");
    QString planName = fullText.mid(start, end - start);

    auto reply = QMessageBox::question(this, QString::fromUtf8("Potwierdzenie"),
                                       QString::fromUtf8("Czy na pewno usunąć plan '%1'?").arg(planName),
                                       QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes) {
        db.getWorkoutPlanRepository().removePlan(planName.toStdString());
        refreshPlanList();
        saveDatabase();
        QMessageBox::information(this, QString::fromUtf8("Sukces"),
                                 QString::fromUtf8("Plan usunięty!"));
    }
}

void MainWindow::onViewPlan() {
    QListWidgetItem* item = ui->listPlans->currentItem();
    if(!item) return;

    QString fullText = item->text();
    int start = fullText.indexOf(' ') + 1;
    int end = fullText.indexOf(" (");
    QString planName = fullText.mid(start, end - start);

    auto plan = db.getWorkoutPlanRepository().findByName(planName.toStdString());
    if(!plan) return;

    QString details = QString::fromUtf8("Plan: %1\n\n").arg(planName);
    const auto& entries = plan->getEntries();

    for(size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        details += QString("%1. %2\n")
                       .arg(i + 1)
                       .arg(QString::fromStdString(entry.exercise->getName()));
        details += QString::fromUtf8("   Serie: %1 | Powtórzenia: %2 | Ciężar: %3 kg | Odpoczynek: %4s\n\n")
                       .arg(entry.sets)
                       .arg(entry.reps)
                       .arg(entry.weight)
                       .arg(entry.restTime);
    }

    QMessageBox::information(this, QString::fromUtf8("Szczegóły planu"), details);
}

void MainWindow::onSearchPlans() {
    QString query = ui->lineSearchPlans->text();

    if(query.isEmpty()) {
        refreshPlanList();
        return;
    }

    ui->listPlans->clear();
    auto results = db.getWorkoutPlanRepository().searchByName(query.toStdString());

    for(const auto& plan : results) {
        QString itemText = QString::fromUtf8("📋 %1 (%2 ćwiczeń)")
                               .arg(QString::fromStdString(plan->getName()))
                               .arg(plan->getEntryCount());
        ui->listPlans->addItem(itemText);
    }
}
