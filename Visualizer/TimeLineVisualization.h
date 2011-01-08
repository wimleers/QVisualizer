#ifndef TIMELINEVISUALIZATION_H_
#define TIMELINEVISUALIZATION_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>
//#include <QGraphicsScene>
#include "CustomQGraphicsEllipseItem.h"
#include <QGraphicsView>
#include <QLabel>
#include <QxtSpanSlider>

#include "Database.h"
#include "CustomQGraphicsScene.h"
#include "CustomQGraphicsEllipseItem.h"

class TimeLineVisualization : public QWidget {
    Q_OBJECT

    private:
        int minEventTime, maxEventTime;

        QxtSpanSlider *timeSlider;
        QTimer *timer;
        QLabel *minTimeValueLabel, *maxTimeValueLabel;
        QVBoxLayout *mainLayout;
        QHBoxLayout *timeValueLabelsLayout;

        CustomQGraphicsScene *scene;
        QGraphicsView *view;
        QGraphicsRectItem *highlightedRect;
        QBrush *redBrush, *blueBrush, *greenBrush;

        QString msecsToString(const int mseconds) const;

    public:
        TimeLineVisualization(Database *database);
        ~TimeLineVisualization() {}

        void eventsSequenceChanged(const QVector<Event*>* events);

    signals:
        void timeSpanChanged(int, int);
        void onEventShapeClicked(int);

    private slots:
        void onTimeout();
};

#endif
