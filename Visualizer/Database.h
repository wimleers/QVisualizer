#ifndef DATABASE_H_
#define DATABASE_H_

#include <QObject>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <QDebug>
#include <QProcess>
#include <QDir>

#include <QVector>

#include "Event.h"

class Database : public QObject {
    Q_OBJECT

    private:
        QSqlDatabase db;

        QVector<Event*> *filteredEvents;

    public:
        Database();
        ~Database() {}

        int getMinEventTime();
        int getMaxEventTime();
        void close();

    signals:
        void eventsLoaded(QVector<Event*> *);

    public slots:
        void loadEvents(int start, int stop);
};

#endif
