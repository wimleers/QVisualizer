#ifndef TIMELINEVISQGRAPHICSSCENE_H
#define TIMELINEVISQGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "CustomQGraphicsEllipseItem.h"

class TimeLineVisQGraphicsScene: public QGraphicsScene {
    Q_OBJECT

    public:
        TimeLineVisQGraphicsScene() : QGraphicsScene() {}

        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
            CustomQGraphicsEllipseItem* clickedShape = (CustomQGraphicsEllipseItem*) itemAt(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());

            if(clickedShape != NULL && clickedShape->type() == 4) { // 4 being an QGEllipseItem
                emit(eventShapeClicked(clickedShape->getTime(), clickedShape->getEventType()));
            }
        }

    signals:
        void eventShapeClicked(int msecs, const QString & eventType);
};

#endif
