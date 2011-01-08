#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QPoint>
#include <QLabel>
#include <QMouseEvent>

class ClickLabel : public QLabel {
    Q_OBJECT

    //private:

    public:
        ClickLabel() : QLabel() {}
        ~ClickLabel() {}

    protected:

        void mousePressEvent(QMouseEvent * e);

    signals:
        void clicked(QPoint);
};

#endif
