#ifndef HEATMAPVISUALIZATION_H
#define HEATMAPVISUALIZATION_H

#include <QWidget>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>

#include "Event.h"

class HeatMapVisualization : public QWidget {
    Q_OBJECT

    private:
        QGridLayout *mainLayout;
        QImage* image;
        QLabel *heatMapLabel;

        int height, width;
        int **heatMap;

        int maxClicks;
        int marge;

        bool leftClick, rightClick, click, doubleClick;

        int max(int a, int b);
        void clickOnHeatMap(int x, int y);
        void clearHeatMap();

        //testfuncties
        void printHeatMap();

    public:
        HeatMapVisualization(QSize resolution);
        ~HeatMapVisualization() {}

        void update(QVector<Event*> *events);
        void renderVisualization();

};


#endif // HEATMAPVISUALIZATION_H
