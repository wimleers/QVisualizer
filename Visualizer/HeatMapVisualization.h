#ifndef HEATMAPVISUALIZATION_H
#define HEATMAPVISUALIZATION_H

#include <QWidget>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QPainter>
#include <QPicture>
#include <QPushButton>
#include <QDialog>

#include "Event.h"

class HeatMapVisualization : public QWidget {
    Q_OBJECT

    private:
        QDialog * dialog;
        QVBoxLayout *mainLayout;
        QImage* image;
        QLabel *heatMapLabel;
        QCheckBox *leftClickCheckBox;
        QCheckBox *rightClickCheckBox;
        QCheckBox *mouseMoveRouteCheckBox;
        QCheckBox *clickCheckBox;
        QCheckBox *doubleClickCheckBox;
        QCheckBox *mouseMoveCheckBox;
        QSpinBox *margeSpinBox;
        QSpinBox *mouseRouteIntervalSpinBox;

        QVector<Event*> *lastEvents;

        int height, width, screenHeight, screenWidth;
        int **heatMap;
        QVector<QPoint> *mouseRoute;

        int maxClicks;
        int marge, mouseRouteInterval;

        bool leftClick, rightClick, click, doubleClick, mouseMove, mouseMoveRoute;

        int max(int a, int b);
        void clickOnHeatMap(int x, int y);
        void clearHeatMap();

    public:
        HeatMapVisualization(QSize resolution);
        ~HeatMapVisualization() {}

        void update(QVector<Event*> *events);
        void renderVisualization();

    private slots:
        void updateParameters(int state);
        void updateMarge(int marge);
        void updateMouseRouteInterval(int interval);
        void showImage();
        void closeDialog();

};


#endif // HEATMAPVISUALIZATION_H
