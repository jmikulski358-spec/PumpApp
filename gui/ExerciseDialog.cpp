// ExerciseDialog.cpp
// Lokalizacja: gui/ExerciseDialog.cpp
// Autor: Jakub Krzysztof Mikulski

#include "ExerciseDialog.h"
#include "ui_ExerciseDialog.h"
#include "../core/ExerciseFactory.h"
#include <QMessageBox>

// Konstruktor - tryb dodawania
ExerciseDialog::ExerciseDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ExerciseDialog)
    , exercise(nullptr)
    , editMode(false)
{
    ui->setupUi(this);
    setupUI();
    setWindowTitle(QString::fromUtf8("Dodaj ćwiczenie"));
}

// Konstruktor - tryb edycji
ExerciseDialog::ExerciseDialog(QWidget *parent, std::shared_ptr<Exercise> exerciseToEdit)
    : QDialog(parent)
    , ui(new Ui::ExerciseDialog)
    , exercise(exerciseToEdit)
    , editMode(true)
{
    ui->setupUi(this);
    setupUI();
    loadExerciseData();
    setWindowTitle(QString::fromUtf8("Edytuj ćwiczenie"));
}

ExerciseDialog::~ExerciseDialog()
{
    delete ui;
}

void ExerciseDialog::setupUI()
{
    // Połączenie sygnałów
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ExerciseDialog::onAccept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &ExerciseDialog::onReject);

    // Dodanie listy mięśni do ComboBox
    ui->comboMuscles->addItem("Chest");
    ui->comboMuscles->addItem("Back");
    ui->comboMuscles->addItem("Shoulders");
    ui->comboMuscles->addItem("Biceps");
    ui->comboMuscles->addItem("Triceps");
    ui->comboMuscles->addItem("Forearms");
    ui->comboMuscles->addItem("Abs");
    ui->comboMuscles->addItem("Quads");
    ui->comboMuscles->addItem("Hamstrings");
    ui->comboMuscles->addItem("Glutes");
    ui->comboMuscles->addItem("Calves");
    ui->comboMuscles->addItem("Full Body");
    ui->comboMuscles->setEditable(true);

    // Domyślny typ - bodyweight
    ui->radioBodyweight->setChecked(true);
}

void ExerciseDialog::loadExerciseData()
{
    if (!exercise) return;

    ui->lineEditName->setText(QString::fromStdString(exercise->getName()));
    ui->textEditDescription->setPlainText(QString::fromStdString(exercise->getDescription()));
    ui->comboMuscles->setCurrentText(QString::fromStdString(exercise->getTargetMuscles()));

    if (exercise->getType() == ExerciseType::WEIGHTED) {
        ui->radioWeighted->setChecked(true);
    } else {
        ui->radioBodyweight->setChecked(true);
    }
}

bool ExerciseDialog::validateInput()
{
    if (ui->lineEditName->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                             QString::fromUtf8("Nazwa nie może być pusta!"));
        return false;
    }

    if (ui->textEditDescription->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                             QString::fromUtf8("Opis nie może być pusty!"));
        return false;
    }

    if (ui->comboMuscles->currentText().trimmed().isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("Błąd"),
                             QString::fromUtf8("Wybierz grupę mięśniową!"));
        return false;
    }

    return true;
}

void ExerciseDialog::onAccept()
{
    if (!validateInput()) {
        return;
    }

    QString name = ui->lineEditName->text().trimmed();
    QString desc = ui->textEditDescription->toPlainText().trimmed();
    QString muscles = ui->comboMuscles->currentText().trimmed();

    ExerciseType type = ui->radioWeighted->isChecked()
                            ? ExerciseType::WEIGHTED
                            : ExerciseType::BODYWEIGHT;

    exercise = ExerciseFactory::createExercise(
        type,
        name.toStdString(),
        desc.toStdString(),
        muscles.toStdString()
        );

    accept();
}

void ExerciseDialog::onReject()
{
    reject();
}

std::shared_ptr<Exercise> ExerciseDialog::getExercise() const
{
    return exercise;
}
