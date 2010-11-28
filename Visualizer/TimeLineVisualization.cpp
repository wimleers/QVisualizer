#include "TimeLineVisualization.h"

TimeLineVisualization::TimeLineVisualization(Database *database) : QWidget() {
   this->timeSlider = new QxtSpanSlider(Qt::Horizontal);
   int minEventTime = database->getMinEventTime();
   int maxEventTime = database->getMaxEventTime();
   qDebug() << minEventTime << " " << maxEventTime;
   this->timeSlider->setSpan(minEventTime, maxEventTime);

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
