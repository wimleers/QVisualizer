#ifndef CUSTOMQGRAPHICSELLIPSEITEM_H_
#define CUSTOMQGRAPHICSELLIPSEITEM_H_

#include <QGraphicsEllipseItem>

class CustomQGraphicsEllipseItem : public QGraphicsEllipseItem {
    private:
        int msecs;
    public:
        CustomQGraphicsEllipseItem(int msecs) : QGraphicsEllipseItem(), msecs(msecs) {}

        inline int getTime() const { return msecs; }
};

#endif
