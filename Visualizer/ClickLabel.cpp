#include "ClickLabel.h"

void ClickLabel::mousePressEvent(QMouseEvent * event) {
    QLabel::mousePressEvent(event);

    emit clicked(event->pos());
}
