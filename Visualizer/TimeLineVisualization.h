#ifndef TIMELINEVISUALIZATION_H_
#define TIMELINEVISUALIZATION_H_

#include <QWidget>
#include <QGridLayout>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsProxyWidget>

#include "Database.h"
#include "MySpanSlider.h"

class TimeLineVisualization : public QWidget {
    Q_OBJECT

    private:
        MySpanSlider *timeSlider;
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
