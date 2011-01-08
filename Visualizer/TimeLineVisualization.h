#ifndef TIMELINEVISUALIZATION_H_
#define TIMELINEVISUALIZATION_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QLabel>
#include <QxtSpanSlider>

#include "Database.h"

class TimeLineVisualization : public QWidget {
    Q_OBJECT

    private:
        int minEventTime, maxEventTime;

        QxtSpanSlider *timeSlider;
        QTimer *timer;
        QLabel *minTimeValueLabel, *maxTimeValueLabel;
        QVBoxLayout *mainLayout;
        QHBoxLayout *timeValueLabelsLayout;

        QGraphicsScene *scene;
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

    private slots:
        void onTimeout();
};

#endif
