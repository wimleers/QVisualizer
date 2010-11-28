#include "Database.h"

Database::Database() : QObject() {
    QProcess p;
    static QString command = "sqlite3 db.sqlite < sql.sql; sqlite3 db.sqlite";

#ifdef Q_OS_WIN32
    p.start(QString("cmd.exe /c %1").arg(command));
#else
    p.start(QString("sh -c \"%1\"").arg(command));
#endif

    if (p.waitForStarted(500)) {
        p.write(".separator ','\r\n");
        p.write(".import events.csv Events\r\n");
        p.waitForFinished(1000);
        qDebug() << p.readAllStandardOutput();
    }
    else
       qDebug() << "Failed to import recorded events into SQLite database.";
    p.close();

    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName("./db.sqlite");
    if (!this->db.open()) {
        qDebug() << "Database connection could not be established: " << this->db.lastError().text();
        exit(1);
    }

    filteredEvents = new QVector<Event*>();
}

void Database::loadEvents(int start, int stop) {
    QSqlQuery query("SELECT time_, input_type, event_type, widget, details FROM Events WHERE time_ >= ? AND time_ <= ?");

    query.addBindValue(start);
    query.addBindValue(stop);

    if (query.exec()) {
        filteredEvents->clear();

        while (query.next()) {
            int time = query.value(0).toInt();
            QString inputType = query.value(1).toString();
            QString eventType = query.value(2).toString();
            QString widget = query.value(3).toString();
            QString details = query.value(4).toString();
            Event *event = new Event(time, inputType, eventType, widget, details);
            filteredEvents->append(event);
        }

        emit eventsLoaded(filteredEvents);
    }
    else
        qDebug() << "Failed to execute query: " << query.lastError().text();
}

int Database::getMinEventTime() {
    QSqlQuery query("SELECT MIN(time_) FROM Events;");

    if (query.exec()) {
        if(query.next())
            return query.value(0).toInt();
    }
    else
        qDebug() << "Failed to execute query: " << query.lastError().text();

    return 0;
}

int Database::getMaxEventTime() {
    QSqlQuery query("SELECT MAX(time_) FROM Events;");

    if (query.exec()) {
        if(query.next())
            return query.value(0).toInt();
    }
    else
        qDebug() << "Failed to execute query: " << query.lastError().text();

    return 0;
}

void Database::close() {
    if (this->db.isOpen())
        this->db.close();
//    this->db.removeDatabase();
}
