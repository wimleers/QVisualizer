#ifndef TIMELINEVISUALIZATION_H_
#define TIMELINEVISUALIZATION_H_

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <QGraphicsView>
#include <QLabel>
#include <QxtSpanSlider>

#include "Database.h"
#include "TimeLineVisQGraphicsScene.h"
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

        TimeLineVisQGraphicsScene *scene;
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
        void onEventShapeClicked(int msecs, const QString & eventType);

    private slots:
        void onTimeout();
};

#endif
