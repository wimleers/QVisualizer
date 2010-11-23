#include "Database.h"

Database::Database() {
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

    // Sample query.
    QSqlQuery q(this->db);
    q.exec("SELECT * FROM Events;");
    while (q.next()) {
        qDebug() << "row" << q.at();
        for (int i = 0; i < 5; i++) {
            qDebug() << q.value(i);
        }
    }
}

void Database::close() {
    if (this->db.isOpen())
        this->db.close();
//    this->db.removeDatabase();
}
