#include "Database.h"

Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./db.sqlite");
    db.open();
}

void Database::close() {
    db = QSqlDatabase();
    db.close();
}