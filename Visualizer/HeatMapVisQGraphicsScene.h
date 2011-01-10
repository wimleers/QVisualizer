#ifndef HEATMAPVISQGRAPHICSSCENE_H
#define HEATMAPVISQGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>

class HeatMapVisQGraphicsScene: public QGraphicsScene {
    Q_OBJECT

    private:
        QImage *backgroundImage, *foregroundImage;

    public:
        QGraphicsTextItem *numClicksLabel;

        HeatMapVisQGraphicsScene() : QGraphicsScene() {
            backgroundImage = NULL;
            numClicksLabel = NULL;
        }

        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
            emit(clicked(mouseEvent->scenePos()));
        }

        void setBackgroundImage(QImage *background) {
            this->backgroundImage = background;
        }

        void setForegroundImage(QImage *foreground) {
            this->foregroundImage = foreground;
        }

    protected:
        void drawBackground(QPainter *painter, const QRectF &rect) {
            if (backgroundImage != NULL)
                painter->drawImage(rect, backgroundImage->scaled(QSize(sceneRect().width(),sceneRect().height()), Qt::KeepAspectRatio));
            if (foregroundImage != NULL)
                painter->drawImage(rect, foregroundImage->scaled(QSize(sceneRect().width(),sceneRect().height()), Qt::KeepAspectRatio));
        }

        void drawForeground(QPainter *painter, const QRectF &rect) {
;
        }

    signals:
        void clicked(QPointF);
};

#endif // HEATMAPVISQGRAPHICSSCENE_H
