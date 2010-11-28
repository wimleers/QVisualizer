#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "Database.h"
#include "TimeLineVisualization.h"

#include <QMainWindow>
#include <QGridLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        Database *database;

        TimeLineVisualization* timeLineVis;
        QGridLayout *mainLayout;

        void closeEvent(QCloseEvent *e);
        void createContent();
    
    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow() {}

    public slots:
        void onEventsLoaded(QVector<Event*> *events);
};

#endif
