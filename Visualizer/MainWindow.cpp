#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    this->database = new Database();
    connect(this->database, SIGNAL(eventsLoaded(QVector<Event*> *)), SLOT(onEventsLoaded(QVector<Event*> *)));

    setGeometry(30, 30, 800, 600);
    createContent();
}

void MainWindow::closeEvent(QCloseEvent *e) {
    Q_UNUSED(e);

    this->database->close();
    this->close();
}

void MainWindow::createContent() {
    mainLayout = new QGridLayout();

    this->timeLineVis = new TimeLineVisualization(this->database);
    connect(this->timeLineVis, SIGNAL(timeSpanChanged(int, int)), this->database, SLOT(loadEvents(int, int)));

    //this->heatMapVis = new HeatMapVisualization(this->database);

    //mainLayout->addWidget(this->heatMapVis, 0, 0);
    mainLayout->addWidget(this->timeLineVis, 1, 0);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    this->setCentralWidget(centralWidget);
}

void MainWindow::onEventsLoaded(QVector<Event*> *events) {
    for (int i = 0; i < events->count(); ++i) {
        qDebug() << events->at(i)->getTime() << " "
                 << events->at(i)->getEventType() << " "
                 << events->at(i)->getInputType() << " "
                 << events->at(i)->getWidget() << " "
                 << events->at(i)->getDetails();
    }
    //this->heatMapVis->update(events);
}

