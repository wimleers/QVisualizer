#include "TimeLineVisualization.h"

TimeLineVisualization::TimeLineVisualization(Database *database) : QWidget() {
    this->timeSlider = new QxtSpanSlider(Qt::Horizontal);
    this->timeSlider->setRange(database->getMinEventTime(), database->getMaxEventTime());
    // The line below forces the upper handle to be drawn correctly.
    this->timeSlider->setUpperPosition(this->timeSlider->maximum());

    this->timer = new QTimer();
    this->timer->setSingleShot(true);
    this->timer->setInterval(100);
    connect(this->timeSlider, SIGNAL(spanChanged(int, int)), this->timer, SLOT(start()));
    connect(this->timer, SIGNAL(timeout()), SLOT(onTimeout()));

    this->minTimeValueLabel = new QLabel(msecsToString(database->getMinEventTime()));
    this->maxTimeValueLabel = new QLabel(msecsToString(database->getMaxEventTime()));

    mainLayout = new QGridLayout();
    mainLayout->addWidget(this->minTimeValueLabel, 0, 1, Qt::AlignLeft);
    mainLayout->addWidget(this->maxTimeValueLabel, 0, 2, Qt::AlignRight);
    mainLayout->addWidget(this->timeSlider, 1, 1);

    this->setLayout(mainLayout);
    this->show();
}

void TimeLineVisualization::onTimeout() {
    emit timeSpanChanged(this->timeSlider->lowerValue(), this->timeSlider->upperValue());
    this->minTimeValueLabel->setText(msecsToString(this->timeSlider->lowerValue()));
    this->maxTimeValueLabel->setText(msecsToString(this->timeSlider->upperValue()));
}

/* Converts an integer that represents an amount of millliseconds to a better
   understandable format. E.g.: 1234(ms) to "1.234s" and 71600(ms) to "1m11.6s".
 */
QString TimeLineVisualization::msecsToString(const int mseconds) const {
    return QString::number((mseconds / 1000) / 60) + "m"
         + QString::number((mseconds / 1000) % 60) + "."
         + QString::number(mseconds % 1000) + "s";
}
