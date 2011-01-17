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
#include <QFile>
#include <QGraphicsView>
#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include <QGraphicsTextItem>

#include "Event.h"
#include "Database.h"
#include "HeatMapVisQGraphicsScene.h"

class HeatMapVisualization : public QWidget {
    Q_OBJECT

    private:
        QVBoxLayout *mainLayout;
        QImage* heatMapImage;

        HeatMapVisQGraphicsScene *scene;
        QGraphicsView *view;

        QCheckBox *showLeftClicksCheckBox, *showRightClicksCheckBox;
        QCheckBox *showSingleClicksCheckBox, *showDoubleClicksCheckBox;
        QCheckBox *showMouseMoveCheckBox, *showMouseMoveRouteCheckBox;;
        QSpinBox *margeSpinBox, *mouseRouteIntervalSpinBox, *clickThresholdAreaSpinBox;

        QVector<Event*> *lastEvents;
        QHash<int, bool> *availableBackgroundImages;

        int height, width, screenHeight, screenWidth;
        int **heatMap;
        int **clickHeatMap;
        QVector<QPoint> *mouseRoute;

        int maxClicks;
        int marge, mouseRouteInterval, clickThresholdArea;
        int lastEventTime;

        bool showLeftClicks, showRightClicks, showSingleClicks, showDoubleClicks, showMouseMove, showMouseMoveRoute;

        int max(int a, int b);
        void clickOnHeatMap(int x, int y);
        void clearHeatMap();
        void createCheckBoxes();

    public:
        HeatMapVisualization(Database *database);
        ~HeatMapVisualization() {}

        void update(QVector<Event*> *events);
        void renderVisualization();

    private:
        QImage* determineBackgroundImage(int msecs);
        void determineAvailableBackgroundImages(int msecs);

    private slots:
        void updateParameters(int state);
        void updateMarge(int marge);
        void updateMouseRouteInterval(int interval);
        void updateClickThresholdArea(int threshold);
        void pixelSelected(QPointF p);

    public slots:
        void highlightEventLocation(int msec);
};


#endif // HEATMAPVISUALIZATION_H
