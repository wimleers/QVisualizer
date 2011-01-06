#ifndef HEATMAPVISUALIZATION_H
#define HEATMAPVISUALIZATION_H

#include <QWidget>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>

#include "Event.h"

class HeatMapVisualization : public QWidget {
    Q_OBJECT

    private:
        QVBoxLayout *mainLayout;
        QImage* image;
        QLabel *heatMapLabel;
        QCheckBox *leftClickCheckBox;
        QCheckBox *rightClickCheckBox;
        QCheckBox *clickCheckBox;
        QCheckBox *doubleClickCheckBox;
        QSpinBox *margeSpinBox;

        QVector<Event*> *lastEvents;

        int height, width, screenHeight, screenWidth;
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

    private slots:
        void updateParameters(int state);
        void updateMarge(int marge);

};


#endif // HEATMAPVISUALIZATION_H
