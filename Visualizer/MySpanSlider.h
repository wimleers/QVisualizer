#ifndef MYSPANSLIDER_H_
#define MYSPANSLIDER_H_

#include <QxtSpanSlider>
#include <QPaintEvent>

class MySpanSlider : public QxtSpanSlider {
    public:
        MySpanSlider(Qt::Orientation orientation) : QxtSpanSlider(orientation) {}

    protected:
        //void paintEvent(QPaintEvent *event) {
            //qDebug() << "blah";
            //qDebug() << this->parent()->event(event);
        //}
};

#endif
