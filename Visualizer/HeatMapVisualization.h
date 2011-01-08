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
//#include "ClickPixmap.h"

class HeatMapVisualization : public QWidget {
    Q_OBJECT

    private:
        QDialog * dialog;
        QVBoxLayout *mainLayout;
        QImage* image;
        /*QPixmap *pixmap;
        ClickPixmap *clickPixmap;*/
        QLabel *heatMapLabel;
        QCheckBox *showLeftClicksCheckBox, *showRightClicksCheckBox;
        QCheckBox *showSingleClicksCheckBox, *showDoubleClicksCheckBox;
        QCheckBox *showMouseMoveCheckBox, *showMouseMoveRouteCheckBox;;
        QSpinBox *margeSpinBox, *mouseRouteIntervalSpinBox;

        QVector<Event*> *lastEvents;

        int height, width, screenHeight, screenWidth;
        int **heatMap;
        QVector<QPoint> *mouseRoute;

        int maxClicks;
        int marge, mouseRouteInterval;

        bool showLeftClicks, showRightClicks, showSingleClicks, showDoubleClicks, showMouseMove, showMouseMoveRoute;

        int max(int a, int b);
        void clickOnHeatMap(int x, int y);
        void clearHeatMap();
        void createCheckBoxes();

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
        void pixelSelected(QPoint p);

    public slots:
        void highlightEventLocation(int msec);
};


#endif // HEATMAPVISUALIZATION_H
