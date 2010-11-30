#include "TimeLineVisualization.h"

TimeLineVisualization::TimeLineVisualization(Database *database) : QWidget() {
   this->timeSlider = new MySpanSlider(Qt::Horizontal);
   this->timeSlider->setSpan(database->getMinEventTime(), database->getMaxEventTime());

   /*
   QGraphicsScene scene;
   QGraphicsProxyWidget *proxy = scene.addWidget(this->timeSlider);
   qDebug() << "Is visible?" << proxy->isVisible();
   qDebug() << "Position" << proxy->pos().x() << proxy->pos().y();
   proxy->setPos();

   QGraphicsView view(&scene);
   view.show();
   */

   this->timer = new QTimer();
   this->timer->setSingleShot(true);
   this->timer->setInterval(200);

   connect(this->timeSlider, SIGNAL(spanChanged(int, int)), this->timer, SLOT(start()));
   connect(this->timer, SIGNAL(timeout()), SLOT(onTimeout()));

   mainLayout = new QGridLayout();
   mainLayout->addWidget(this->timeSlider, 0, 0);

   this->setLayout(mainLayout);
   this->show();
}

void TimeLineVisualization::onTimeout() {
    emit timeSpanChanged(this->timeSlider->lowerValue(), this->timeSlider->upperValue());
}
