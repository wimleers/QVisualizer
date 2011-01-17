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
#include <QFile>
#include <QTextStream>

#include <QVector>

#include <QSize>

#include "Event.h"

class Database : public QObject {
    Q_OBJECT

    private:
        QSqlDatabase db;
        QString csvFile;
        QString csvFileForSQLite;
        QSize resolution;

        QVector<Event*> *filteredEvents;

    public:
        Database(bool import = false);
        ~Database();

        int getMinEventTime();
        int getMaxEventTime();
        void close();

        QSize getResolution() const;




    signals:
        void eventsLoaded(QVector<Event*> *);

    public slots:
        void loadEvents(int start, int stop);
};

#endif
