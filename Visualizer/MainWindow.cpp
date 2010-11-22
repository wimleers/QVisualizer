#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    database = new Database();
}

void MainWindow::closeEvent(QCloseEvent *e) {
    database->close();
    close();
}