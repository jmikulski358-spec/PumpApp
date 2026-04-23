// WorkoutPlanDialog.cpp
// Lokalizacja: gui/WorkoutPlanDialog.cpp
// Autor: Jakub Krzysztof Mikulski

#include "WorkoutPlanDialog.h"
#include "ui_WorkoutPlanDialog.h"
#include <QMessageBox>
#include <QInputDialog>

// Konstruktor - tryb dodawania
WorkoutPlanDialog::WorkoutPlanDialog(QWidget *parent, DatabaseManager* dbManager)
    : QDialog(parent)
    , ui(new Ui::WorkoutPlanDialog)
    , plan(nullptr)
    , db(dbManager)
    , editMode(false)
{
    ui->setupUi(this);
    setupUI();
    setWindowTitle(QString::fromUtf8("Dodaj plan treningowy"));
}

// Konstruktor - tryb edycji
WorkoutPlanDialog::WorkoutPlanDialog(QWidget *parent, DatabaseManager* dbManager, std::shared_ptr<WorkoutPlan> planToEdit)
    : QDialog(parent)
    , ui(new Ui::WorkoutPlanDialog)
    , plan(planToEdit)
    , db(dbManager)
    , editMode(true)
{
    ui->setupUi(this);
    setupUI();
    loadPlanData();
    setWindowTitle(QString::fromUtf8("Edytuj plan treningowy"));
}

WorkoutPlanDialog::~WorkoutPlanDialog()
{
    delete ui;
}

void WorkoutPlanDialog::setupUI()
{
    // Połączenie sygnałów
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &WorkoutPlanDialog::onAccept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &WorkoutPlanDialog::onReject);
    connect(ui->btnAddExercise, &QPushButton::clicked, this, &WorkoutPlanDialog::onAddExerciseToPlan);
    connect(ui->btnRemoveExercise, &QPushButton::clicked, this, &WorkoutPlanDialog::onRemoveExerciseFromPlan);
    connect(ui->listPlanEntries, &QListWidget::itemSelectionChanged, this, &WorkoutPlanDialog::onPlanEntrySelectionChanged);

    // Załadowanie dostępnych ćwiczeń
    loadAvailableExercises();

    // Początkowy stan przycisków
    ui->btnRemoveExercise->setEnabled(false);
}

void WorkoutPlanDialog::loadAvailableExercises()
{
    ui->comboExercises->clear();

    const auto& exercises = db->getExerciseRepository().getAllExercises();

    if (exercises.empty()) {
        ui->comboExercises->addItem(QString::fromUtf8("Brak ćwiczeń w bazie!"));
        ui->btnAddExercise->setEnabled(false);
        return;
    }

    for (const auto& ex : exercises) {
        QString typeIcon = (ex->getType() == ExerciseType::WEIGHTED) ? QString::fromUtf8("🏋️") : QString::fromUtf8("💪");
        QString itemText = QString("%1 %2").arg(typeIcon, QString::fromStdString(ex->getName()));
        ui->comboExercises->addItem(itemText, QString::fromStdString(ex->getName()));
    }

    ui->btnAddExercise->setEnabled(true);
}

void WorkoutPlanDialog::loadPlanData()
{
    if (!plan) return;

    ui->lineEditPlanName->setText(QString::fromStdString(plan->getName()));
    refreshPlanEntries();
}

void WorkoutPlanDialog::refreshPlanEntries()
{
    ui->listPlanEntries->clear();

    if (!plan) return;

    const auto& entries = plan->getEntries();
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];

        QString itemText = QString::fromUtf8("%1. %2 - %3x%4")
                               .arg(i + 1)
                               .arg(QString::fromStdString(entry.exercise->getName()))
                               .arg(entry.sets)
                               .arg(entry.reps);

        if (entry.weight > 0) {
            itemText += QString(" @ %1kg").arg(entry.weight);
        }

        itemText += QString::fromUtf8(" (przerwa: %1s)").arg(entry.restTime);

        ui->listPlanEntries->addItem(itemText);
    }
}

void WorkoutPlanDialog::onPlanEntrySelectionChanged()
{
    bool hasSelection = ui->listPlanEntries->currentItem() != nullptr;
    ui->btnRemoveExercise->setEnabled(hasSelection);
}

void WorkoutPlanDialog::onAddExerciseToPlan()
{
    if (ui->comboExercises->currentIndex() < 0) {
        QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                             QString::fromUtf8("Wybierz ćwiczenie z listy!"));
        return;
    }

    // Pobranie nazwy ćwiczenia z userData (bez emoji)
    QString exerciseName = ui->comboExercises->currentData().toString();
    auto exercise = db->getExerciseRepository().findByName(exerciseName.toStdString());

    if (!exercise) {
        QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                             QString::fromUtf8("Nie znaleziono wybranego ćwiczenia!"));
        return;
    }

    // Pytanie o parametry treningu
    bool ok;

    // Serie
    int sets = QInputDialog::getInt(this, QString::fromUtf8("Serie"),
                                    QString::fromUtf8("Liczba serii:"),
                                    3, 1, 20, 1, &ok);
    if (!ok) return;

    // Powtórzenia
    int reps = QInputDialog::getInt(this, QString::fromUtf8("Powtórzenia"),
                                    QString::fromUtf8("Liczba powtórzeń w serii:"),
                                    10, 1, 100, 1, &ok);
    if (!ok) return;

    // Ciężar (tylko dla weighted exercises)
    double weight = 0.0;
    if (exercise->getType() == ExerciseType::WEIGHTED) {
        weight = QInputDialog::getDouble(this, QString::fromUtf8("Ciężar"),
                                         QString::fromUtf8("Ciężar (kg):"),
                                         20.0, 0.0, 500.0, 1, &ok);
        if (!ok) return;
    }

    // Czas odpoczynku (ZAWSZE pytamy!)
    int restTime = QInputDialog::getInt(this, QString::fromUtf8("Odpoczynek"),
                                        QString::fromUtf8("Czas odpoczynku między seriami (sekundy):"),
                                        60, 10, 600, 10, &ok);
    if (!ok) return;

    // Utworzenie planu jeśli nie istnieje
    if (!plan) {
        plan = std::make_shared<WorkoutPlan>("Temporary");
    }

    // Dodanie ćwiczenia do planu
    try {
        plan->addEntry(exercise, sets, reps, weight, restTime);
        refreshPlanEntries();
        QMessageBox::information(this, QString::fromUtf8("Sukces"),
                                 QString::fromUtf8("Ćwiczenie dodane do planu!"));
    } catch (const std::exception& e) {
        QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                             QString::fromUtf8("Nie można dodać ćwiczenia:\n") + e.what());
    }
}

void WorkoutPlanDialog::onRemoveExerciseFromPlan()
{
    QListWidgetItem* item = ui->listPlanEntries->currentItem();
    if (!item || !plan) return;

    int index = ui->listPlanEntries->currentRow();

    auto reply = QMessageBox::question(this, QString::fromUtf8("Potwierdzenie"),
                                       QString::fromUtf8("Czy na pewno usunąć to ćwiczenie z planu?"),
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        plan->removeEntry(index);
        refreshPlanEntries();
    }
}

bool WorkoutPlanDialog::validateInput()
{
    if (ui->lineEditPlanName->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                             QString::fromUtf8("Nazwa planu nie może być pusta!"));
        return false;
    }

    if (!plan || plan->isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                             QString::fromUtf8("Plan musi zawierać przynajmniej jedno ćwiczenie!"));
        return false;
    }

    return true;
}

void WorkoutPlanDialog::onAccept()
{
    if (!validateInput()) {
        return;
    }

    QString planName = ui->lineEditPlanName->text().trimmed();
    plan->setName(planName.toStdString());

    accept();
}

void WorkoutPlanDialog::onReject()
{
    reject();
}

std::shared_ptr<WorkoutPlan> WorkoutPlanDialog::getWorkoutPlan() const
{
    return plan;
}
