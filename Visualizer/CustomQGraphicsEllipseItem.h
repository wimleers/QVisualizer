#ifndef CUSTOMQGRAPHICSELLIPSEITEM_H_
#define CUSTOMQGRAPHICSELLIPSEITEM_H_

#include <QGraphicsEllipseItem>

class CustomQGraphicsEllipseItem : public QGraphicsEllipseItem {
    private:
        int msecs;
        QString eventType;

    public:
        CustomQGraphicsEllipseItem(int msecs, const QString & eventType) : QGraphicsEllipseItem(), msecs(msecs), eventType(eventType){}

        inline int getTime() const { return msecs; }
        inline const QString & getEventType() const { return eventType; }
};

#endif
