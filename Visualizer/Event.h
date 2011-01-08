#ifndef EVENT_H_
#define EVENT_H_

#include <QString>

class Event {
    private:
        int time;
        QString inputType;
        QString eventType;
        QString widget;
        QString details;

    public:
        inline Event() {}

        inline Event(int time, QString inputType, QString eventType, QString widget, QString details) {
            this->time = time;
            this->inputType = inputType;
            this->eventType = eventType;
            this->widget = widget;
            this->details = details;
        }

        inline int getTime() { return this->time; }
        inline QString getInputType() { return this->inputType; }
        inline QString getEventType() { return this->eventType; }
        inline QString getWidget() { return this->widget; }
        inline QString getDetails() { return this->details; }
};

#endif
