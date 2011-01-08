#include "TimeLineVisualization.h"

TimeLineVisualization::TimeLineVisualization(Database *database) : QWidget() {
    timeSlider = new QxtSpanSlider(Qt::Horizontal);
    minEventTime = database->getMinEventTime();
    maxEventTime = database->getMaxEventTime();
    timeSlider->setRange(0, maxEventTime);
    // The line below forces the upper handle to be drawn correctly.
    timeSlider->setUpperPosition(timeSlider->maximum());

    timer = new QTimer();
    timer->setSingleShot(true);
    timer->setInterval(50);
    connect(timeSlider, SIGNAL(spanChanged(int, int)), timer, SLOT(start()));
    connect(timer, SIGNAL(timeout()), SLOT(onTimeout()));

    minTimeValueLabel = new QLabel(msecsToString(0));
    maxTimeValueLabel = new QLabel(msecsToString(database->getMaxEventTime()));
    timeValueLabelsLayout = new QHBoxLayout();
    timeValueLabelsLayout->addWidget(minTimeValueLabel, 0, Qt::AlignLeft);
    timeValueLabelsLayout->addWidget(maxTimeValueLabel, 0, Qt::AlignRight);

    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    view->setGeometry(0, 0, 570, 200);
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

    setMinimumWidth(600);
    setMaximumWidth(600);
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

    redBrush = new QBrush(Qt::red);
    blueBrush = new QBrush(Qt::blue);
    greenBrush = new QBrush(Qt::green);

    foreach(event, *events) {
        QString eventType = event->getEventType();
        if(eventType.compare("MouseButtonPress")   *
           eventType.compare("MouseButtonRelease") *
           eventType.compare("MouseButtonDblClick") *
           eventType.compare("KeyRelease") == 0) {

            QAbstractGraphicsShapeItem *shape;
            int shapeX = (((float) event->getTime() / maxEventTime) * scene->width()) - 3;

            if(eventType.compare("MouseButtonPress") == 0) {
                shape = new QGraphicsEllipseItem(0, scene);
                shape->setBrush(*redBrush);
                ((QGraphicsEllipseItem*)shape)->setRect(shapeX, 30, 6, 6);
            }
            else if(eventType.compare("MouseButtonRelease") == 0) {
                shape = new QGraphicsEllipseItem(0, scene);
                shape->setBrush(*blueBrush);
                ((QGraphicsEllipseItem*)shape)->setRect(shapeX, 75, 6, 6);
            }
            else if(eventType.compare("MouseButtonDblClick") == 0) {
                shape = new QGraphicsEllipseItem(0, scene);
                shape->setBrush(*greenBrush);
                ((QGraphicsEllipseItem*)shape)->setRect(shapeX, 120, 6, 6);
            }
            else if(eventType.compare("KeyRelease") == 0) {
                shape = new QGraphicsRectItem(0, scene);
                ((QGraphicsRectItem*)shape)->setRect(shapeX, 165, 5, 5);
            }

            shape->setToolTip(QString("Time: ") + QString::number(event->getTime()) + "\n" +
                                      "Type: " + event->getEventType() + "\n" +
                                      "Target widget: " + event->getWidget() + "\n" +
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
