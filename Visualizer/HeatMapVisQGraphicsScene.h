#ifndef HEATMAPVISQGRAPHICSSCENE_H
#define HEATMAPVISQGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

class HeatMapVisQGraphicsScene: public QGraphicsScene {
    Q_OBJECT

    public:
        HeatMapVisQGraphicsScene() : QGraphicsScene() {}

        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
            emit(clicked(mouseEvent->scenePos()));
        }

    signals:
        void clicked(QPointF);
};

#endif // HEATMAPVISQGRAPHICSSCENE_H
