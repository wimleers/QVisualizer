#include <QApplication>
#include <QMessageBox>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QT_REQUIRE_VERSION(argc, argv, "4.7.0")

    QApplication app(argc, argv);

    bool import = (argc > 1);
    MainWindow mainWindow(import);
    mainWindow.showMaximized();

    return app.exec();
}
