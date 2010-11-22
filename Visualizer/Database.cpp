#include "Database.h"

Database::Database() {
    #ifdef Q_OS_WIN32
    QProcess p;
    p.start("cmd.exe", QStringList() << "/c sqlite3 db.sqlite < sql.sql && sqlite3 db.sqlite");

    if (p.waitForStarted(500)) {
    p.write(".separator '|'\r\n");
    p.write(".import events.csv Events\r\n");
    p.waitForFinished(1000);
    //qDebug() << p.readAllStandardOutput();
    }
    else
       qDebug() << "Failed to start cmd.exe";
    #endif

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./db.sqlite");
    if (!db.open()) {
        qDebug() << "Database connection could not be established: " << db.lastError().text();
        exit(1);
    }
}

void Database::close() {
    db = QSqlDatabase();
    db.close();
}
