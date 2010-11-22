#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <QMainWindow>
#include "Database.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        Database* database;
        void closeEvent(QCloseEvent *e);
    
    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow() {}
};

#endif
