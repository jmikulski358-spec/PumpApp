// ExerciseDialog.h
// Lokalizacja: gui/ExerciseDialog.h
// Autor: Jakub Krzysztof Mikulski
// Opis: Dialog do dodawania/edycji ćwiczeń

#ifndef EXERCISEDIALOG_H
#define EXERCISEDIALOG_H

#include <QDialog>
#include <memory>
#include "../core/Exercise.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ExerciseDialog;
}
QT_END_NAMESPACE

class ExerciseDialog : public QDialog
{
    Q_OBJECT

public:
    // Konstruktor - dodawanie nowego ćwiczenia
    explicit ExerciseDialog(QWidget *parent = nullptr);

    // Konstruktor - edycja istniejącego ćwiczenia
    ExerciseDialog(QWidget *parent, std::shared_ptr<Exercise> exerciseToEdit);

    // Destruktor
    ~ExerciseDialog();

    // Pobranie stworzonego/edytowanego ćwiczenia
    std::shared_ptr<Exercise> getExercise() const;

private slots:
    void onAccept();
    void onReject();

private:
    Ui::ExerciseDialog *ui;
    std::shared_ptr<Exercise> exercise = nullptr;
    bool editMode = false;

    void setupUI();
    void loadExerciseData();
    bool validateInput();
};

#endif // EXERCISEDIALOG_H
