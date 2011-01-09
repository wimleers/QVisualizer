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
    timer->setInterval(15);
    connect(timeSlider, SIGNAL(spanChanged(int, int)), timer, SLOT(start()));
    connect(timer, SIGNAL(timeout()), SLOT(onTimeout()));

    minTimeValueLabel = new QLabel(msecsToString(0));
    maxTimeValueLabel = new QLabel(msecsToString(database->getMaxEventTime()));
    timeValueLabelsLayout = new QHBoxLayout();
    timeValueLabelsLayout->addWidget(minTimeValueLabel, 0, Qt::AlignLeft);
    timeValueLabelsLayout->addWidget(maxTimeValueLabel, 0, Qt::AlignRight);

    scene = new TimeLineVisQGraphicsScene();
    connect(scene, SIGNAL(eventShapeClicked(int, const QString &)), SIGNAL(onEventShapeClicked(int, const QString &)));
    view = new QGraphicsView(scene);
    //view->setGeometry(0, 0, 570, 200);
    //scene->setSceneRect(view->geometry());
    view->setRenderHints(QPainter::Antialiasing);
    scene->setSceneRect(0, 0, (QApplication::desktop()->width() == 1920) ? 1050: 570, 200);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(timeValueLabelsLayout);
    mainLayout->addWidget(timeSlider);
    mainLayout->addWidget(view);

    setMinimumWidth(600);
    setMaximumWidth(600);
    setMinimumHeight(300);
    setMaximumHeight(300);

    setLayout(mainLayout);
}

void TimeLineVisualization::onTimeout() {
    emit timeSpanChanged(timeSlider->lowerValue(), timeSlider->upperValue());
    minTimeValueLabel->setText(msecsToString(timeSlider->lowerValue()));
    maxTimeValueLabel->setText(msecsToString(timeSlider->upperValue()));
}

void TimeLineVisualization::eventsSequenceChanged(const QVector<Event *> *events) {
    Event *event;

    redBrush = new QBrush(Qt::red);
    blueBrush = new QBrush(Qt::blue);
    greenBrush = new QBrush(Qt::green);

    /* Clear the scene
     * QGraphicsScene::clear() crashes, so this is a workaround
     */
    QGraphicsItem *item;
    foreach(item, scene->items()) {
        scene->removeItem(item);
    }

    /* Draw the red semi-transparant rectangle that graphically indicates
     * which are the vertical boundaries in the graphics scene below.
     */
    int leftX = ((float) timeSlider->lowerValue() / maxEventTime) * scene->width();
    int rightX = ((float) timeSlider->upperValue() / maxEventTime) * scene->width();
    highlightedRect = scene->addRect(leftX, 0, rightX - leftX, scene->height(),
                                     QPen(QColor("red")),
                                     QBrush(QColor(200, 30, 30, 50)));

    /* Draw four horizontal and labeled lines that will each graphically 'contain'
     * a series of events of the according type.
     */
    QString axisLabels[] = {"Mouse left-click", "Mouse right-click", "Mouse double-click", "Keyboard"};
    for(int i = 0; i < 4; ++i) {
        scene->addLine(2, 33 + (i * 45), scene->width() - 2, 33 + (i * 45));
        QGraphicsSimpleTextItem *label = new QGraphicsSimpleTextItem();
        label->setText(axisLabels[i]);
        label->setPos(4, 10 + (i * 45));
        scene->addItem(label);
    }

    // Iterate over all incoming events
    foreach(event, *events) {

        // Filter interesting events
        QString eventType = event->getEventType();
        // Consider MouseButtonReleases as equivalent to MouseButtonPresses.
        eventType.replace("MouseButtonRelease", "MouseButtonPress");
        if(eventType.compare("MouseButtonPress")  *
           eventType.compare("MouseButtonDblClick") *
           eventType.compare("KeyRelease") == 0) {

            // Create a shape for each event and fill it with a different color for each event type
            QAbstractGraphicsShapeItem *shape;
            int shapeX = (((float) event->getTime() / maxEventTime) * scene->width()) - 3;

            if(eventType.compare("MouseButtonPress") == 0) {
                shape = new CustomQGraphicsEllipseItem(event->getTime(), eventType);
                QString details = event->getDetails();
                details.remove("\"");

                QStringList args = details.split(";");
                if(args.count() < 3)
                    break;

                if(args.at(2).indexOf("L") != -1) {
                    shape->setBrush(*redBrush);
                    ((QGraphicsEllipseItem*)shape)->setRect(shapeX, 30, 6, 6);
                }
                else if(args.at(2).indexOf("R") != -1) {
                    shape->setBrush(*blueBrush);
                    ((QGraphicsEllipseItem*)shape)->setRect(shapeX, 75, 6, 6);
                }
            }
            else if(eventType.compare("MouseButtonDblClick") == 0) {
                shape = new CustomQGraphicsEllipseItem(event->getTime(), eventType);
                shape->setBrush(*greenBrush);
                ((QGraphicsEllipseItem*)shape)->setRect(shapeX, 120, 6, 6);
            }
            else if(eventType.compare("KeyRelease") == 0) {
                shape = new QGraphicsRectItem();
                ((QGraphicsRectItem*)shape)->setRect(shapeX, 165, 5, 5);
            }

            shape->setToolTip(QString("Time: ") + msecsToString(event->getTime()) + "\n" +
                                      "Type: " + event->getEventType() + "\n" +
                                      "Target widget: " + event->getWidget() + "\n" +
                                      "Details: " + event->getDetails());
            scene->addItem(shape);
        }
    }
}


/* Converts an integer that represents an amount of millliseconds to a better
 * understandable format. E.g.: 1234(ms) to "1.234s" and 71600(ms) to "1m11.6s".
 */
QString TimeLineVisualization::msecsToString(const int mseconds) const {
    return QString::number((mseconds / 1000) / 60) + "m"
         + QString::number((mseconds / 1000) % 60) + "."
         + QString::number(mseconds % 1000) + "s";
}
