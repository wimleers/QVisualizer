#ifndef TIMELINEVISUALIZATION_H_
#define TIMELINEVISUALIZATION_H_

#include <QWidget>
#include <QxtSpanSlider>
#include <QGridLayout>
#include <QTimer>

#include "Database.h"

class TimeLineVisualization : public QWidget {
    Q_OBJECT

    private:
        QxtSpanSlider *timeSlider;
        QTimer *timer;
        QGridLayout *mainLayout;

    public:
        TimeLineVisualization(Database *database);
        ~TimeLineVisualization() {}

    signals:
        void timeSpanChanged(int, int);

    private slots:
        void onTimeout();
};

#endif
