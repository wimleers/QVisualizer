#include "TimeLineVisualization.h"

TimeLineVisualization::TimeLineVisualization(Database *database) : QWidget() {
    timeSlider = new QxtSpanSlider(Qt::Horizontal);
    minEventTime = database->getMinEventTime();
    maxEventTime = database->getMaxEventTime();
    timeSlider->setRange(minEventTime, maxEventTime);
    // The line below forces the upper handle to be drawn correctly.
    timeSlider->setUpperPosition(timeSlider->maximum());

    timer = new QTimer();
    timer->setSingleShot(true);
    timer->setInterval(100);
    connect(timeSlider, SIGNAL(spanChanged(int, int)), timer, SLOT(start()));
    connect(timer, SIGNAL(timeout()), SLOT(onTimeout()));

    minTimeValueLabel = new QLabel(msecsToString(database->getMinEventTime()));
    maxTimeValueLabel = new QLabel(msecsToString(database->getMaxEventTime()));
    timeValueLabelsLayout = new QHBoxLayout();
    timeValueLabelsLayout->addWidget(minTimeValueLabel, 0, Qt::AlignLeft);
    timeValueLabelsLayout->addWidget(maxTimeValueLabel, 0, Qt::AlignRight);

    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    view->setGeometry(0, 0, 470, 200);
    scene->setSceneRect(view->geometry());
    view->setRenderHints(QPainter::Antialiasing);
    view->show();

    highlightedRect = scene->addRect(QRectF(0, 0, scene->width(), scene->height()),
                                     QPen(QColor("red")),
                                     QBrush(QColor(200, 30, 30, 50)));

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(timeValueLabelsLayout);
    mainLayout->addWidget(timeSlider);
    mainLayout->addWidget(view);

    setMinimumWidth(500);
    setMaximumWidth(500);
    setMinimumHeight(300);
    setMaximumHeight(300);

    setLayout(mainLayout);
    show();
}

void TimeLineVisualization::onTimeout() {
    emit timeSpanChanged(timeSlider->lowerValue(), timeSlider->upperValue());
    minTimeValueLabel->setText(msecsToString(timeSlider->lowerValue()));
    maxTimeValueLabel->setText(msecsToString(timeSlider->upperValue()));

    int leftX = ((float) timeSlider->lowerValue() / maxEventTime) * scene->width();
    int rightX = ((float) timeSlider->upperValue() / maxEventTime) * scene->width();
    highlightedRect->setRect(leftX, 0, rightX - leftX, scene->height());
}

void TimeLineVisualization::eventsSequenceChanged(const QVector<Event *> *events) {
    Event *event;

    foreach(event, *events) {
        QString eventType = event->getEventType();
        if(eventType.compare("MouseButtonPress")   *
           eventType.compare("MouseButtonRelease") == 0) {

            QAbstractGraphicsShapeItem *shape;
            int shapeX = ((float) event->getTime() / maxEventTime) * scene->width();

            if(eventType.compare("MouseButtonPress") == 0) {
                shape = new QGraphicsEllipseItem(0, scene);
                QBrush redBrush;
                redBrush.setColor(Qt::red);

                shape->setBrush(redBrush);
                ((QGraphicsEllipseItem*)shape)->setRect(shapeX, 50, 6, 6);
            }
            else if(eventType.compare("MouseButtonRelease") == 0) {
                shape = new QGraphicsRectItem(0, scene);
                ((QGraphicsRectItem*)shape)->setRect(shapeX, 80, 6, 6);
            }
            shape->setToolTip(QString("Time: ") + QString::number(event->getTime()) + "\n" +
                                      "Type: " + event->getEventType() + "\n" +
                                      "Details: " + event->getDetails());
        }
    }
}

/* Converts an integer that represents an amount of millliseconds to a better
   understandable format. E.g.: 1234(ms) to "1.234s" and 71600(ms) to "1m11.6s".
 */
QString TimeLineVisualization::msecsToString(const int mseconds) const {
    return QString::number((mseconds / 1000) / 60) + "m"
         + QString::number((mseconds / 1000) % 60) + "."
         + QString::number(mseconds % 1000) + "s";
}
