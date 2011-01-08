#ifndef CUSTOMQGRAPHICSSCENE_H
#define CUSTOMQGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "CustomQGraphicsEllipseItem.h"

class CustomQGraphicsScene: public QGraphicsScene {
    Q_OBJECT

    public:
        CustomQGraphicsScene() : QGraphicsScene() {}

        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
            CustomQGraphicsEllipseItem* clickedShape = (CustomQGraphicsEllipseItem*) itemAt(mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
            if(clickedShape != NULL) { // TODO: add check on type of object
                emit(eventShapeClicked(clickedShape->getTime()));
            }
        }

    signals:
        void eventShapeClicked(int);
};

#endif
