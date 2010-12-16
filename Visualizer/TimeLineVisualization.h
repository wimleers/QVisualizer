#ifndef TIMELINEVISUALIZATION_H_
#define TIMELINEVISUALIZATION_H_

#include <QWidget>
#include <QGridLayout>
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
        QxtSpanSlider *timeSlider;
        QTimer *timer;
        QLabel *minTimeValueLabel, *maxTimeValueLabel;
        QGridLayout *mainLayout;

        QString msecsToString(const int mseconds) const;

    public:
        TimeLineVisualization(Database *database);
        ~TimeLineVisualization() {}

    signals:
        void timeSpanChanged(int, int);

    private slots:
        void onTimeout();
};

#endif
