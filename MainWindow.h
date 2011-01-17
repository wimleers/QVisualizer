#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "Database.h"
#include "TimeLineVisualization.h"
#include "HeatMapVisualization.h"
#include "BarTreeVisualization.h"

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        Database *database;

        TimeLineVisualization* timeLineVis;
        HeatMapVisualization* heatMapVis;
        BarTreeVisualization* barTreeVis;

        QHBoxLayout *mainLayout;
        QVBoxLayout *vbox;

        void closeEvent(QCloseEvent *e);
        void createContent();

    public:
        MainWindow(bool import, QWidget *parent = 0);
        ~MainWindow() {}

    public slots:
        void onEventsLoaded(QVector<Event*> *events);
};

#endif
