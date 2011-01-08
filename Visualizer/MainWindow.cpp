#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    database = new Database();
    connect(database, SIGNAL(eventsLoaded(QVector<Event*> *)), SLOT(onEventsLoaded(QVector<Event*> *)));

    createContent();
}

void MainWindow::closeEvent(QCloseEvent *e) {
    Q_UNUSED(e);

    database->close();
    close();
}

/* The general layout of the application is as follows:
   ---------
   | 1 | 2 |        1 = Inneke's heatmap
   ----+ 2 |        2 = Wim's input ratio vis
   | 3 | 2 |        3 = Nick's interactive time line
   ---------
*/
void MainWindow::createContent() {
    heatMapVis = new HeatMapVisualization(database->getResolution());

    timeLineVis = new TimeLineVisualization(database);
    connect(timeLineVis, SIGNAL(timeSpanChanged(int, int)), database, SLOT(loadEvents(int, int)));
    connect(timeLineVis, SIGNAL(onEventShapeClicked(int)), heatMapVis, SLOT(highlightEventLocation(int)));

    barTreeVis = new BarTreeVisualization();

    vbox = new QVBoxLayout();
    vbox->addWidget(heatMapVis);
    vbox->addWidget(timeLineVis, 1);

    mainLayout = new QHBoxLayout();
    mainLayout->addLayout(vbox);
    mainLayout->addWidget(barTreeVis);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Initially, load *all* events.
    database->loadEvents(database->getMinEventTime(), database->getMaxEventTime());
}

/* After the database had fetched the valid events, given a time frame, it passes those events
   to this method, using a container of the type 'QVector<Event*>*'. All visualizations / widgets
   that are outputting information should invoke e.g. their update()-method here..
*/
void MainWindow::onEventsLoaded(QVector<Event*> *events) {
    heatMapVis->update(events);
    barTreeVis->eventSequenceChanged(events);
    timeLineVis->eventsSequenceChanged(events);
}

