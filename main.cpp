#include <QApplication>
#include "gui/MainWindow.h"
#include "core/DatabaseManager.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Inicjalizacja bazy danych
    auto& db = DatabaseManager::getInstance();
    db.initialize();

    // Otwarcie głównego okna
    MainWindow w;
    w.show();

    return a.exec();
}
