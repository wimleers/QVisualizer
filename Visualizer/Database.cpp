#include "Database.h"

Database::Database() : QObject() {
    QProcess p;
    this->csvFile = "events.csv";
    this->csvFileForSQLite = "events-prepared-for-sqlite.csv";

    // Parse the .csv file, and write a new .csv file specifically for SQLite,
    // because it is limited in what it can import:
    // http://www.sqlite.org/cvstrac/wiki?p=ImportingFiles
    //
    // The first two lines are typically ignored by spreadsheet apps; they
    // are seen as comments. But SQLite interprets them as single columns. Yet
    // we need some of the info on the first two lines. The third line
    // contains the header names, but SQLite doesn't understand that either.
    // So we parse those first 3 lines, and copy the rest of the .csv file to
    // a new .csv file importable by SQLite.
    QFile csv(this->csvFile);
    if (!csv.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical("Could not open '%s' file for reading: %s.", qPrintable(this->csvFile), qPrintable(csv.errorString()));
        exit(1);
    }
    else {
        QTextStream in(&csv);
        QString line;
        QStringList parts;
        in.readLine(); // First line: date and time.
        line = in.readLine(); // Second line: resolution.
        parts = line.mid(14, line.length() - 13).split('x');
        if (parts.length() == 2)
            this->resolution = QSize(parts[0].toInt(), parts[1].toInt());
        else
            this->resolution = QSize(-1, -1); // In case somebody provides an invalid file.

        // Third line: column headers.
        in.readLine();

        // Write all remaining lines to csvFileForSQLite.
        QFile csvForSQLite(this->csvFileForSQLite);
        if (!csvForSQLite.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            qCritical("Could not open '%s' file for reading: %s.", qPrintable(this->csvFileForSQLite), qPrintable(csvForSQLite.errorString()));
            exit(1);
        }
        else {
            QTextStream out(&csvForSQLite);
            while (!in.atEnd()) {
                out << in.readLine() << endl;
            }
        }
    }

    // Create the SQLite DB.
    /*static QString command = "sqlite3 db.sqlite < sql.sql && sqlite3 db.sqlite";
#ifdef Q_OS_WIN32
    p.start(QString("cmd.exe /c %1").arg(command));
#else
    p.start(QString("sh -c \"%1\"").arg(command));
#endif

    // Import the csvFileForSQLite into the SQLite DB.
    if (p.waitForStarted(500)) {
        p.write(".separator ','\r\n");
        p.write(qPrintable(QString(".import %1 Events\r\n").arg(this->csvFileForSQLite)));
        p.waitForFinished(1000);
    }
    else
       qDebug() << "Failed to import recorded events into SQLite database.";
    p.close();*/

    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName("./db.sqlite");
    if (!this->db.open()) {
        qDebug() << "Database connection could not be established: " << this->db.lastError().text();
        exit(1);
    }


    this->filteredEvents = new QVector<Event*>();
}

Database::~Database() {
    delete this->filteredEvents;
}

QSize Database::getResolution() const {
    return this->resolution;
}

/* This method fetches all events having a timestamp from value 'start' up to 'stop'.
   All such events are stored in a container of the type 'QVector<Event *>*' which is offered to
   all present visualization widgets by passing the mentioned container via emitting the 'eventsLoaded'-signal.
*/
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

/* This method fetches the lowest timestamp that is stored in the database.
   The fetched value will be used to set a lower limit to the sliders that are part of the
   TimeLineVisualization instance.
*/
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

/* This method fetches the highest timestamp that is stored in the database.
   The fetched value will be used to set an upper limit to the sliders that are part of the
   TimeLineVisualization instance.
*/
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
    /*  In one way or another, the following line avoids the obscure error
        'QSqlDatabasePrivate::removeDatabase: connection 'qt_sql_default_connection' is still
        in use, all queries will cease to work.'. */
    this->db = QSqlDatabase();
}