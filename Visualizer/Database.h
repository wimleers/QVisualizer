#ifndef DATABASE_H_
#define DATABASE_H_

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <QDebug>
#include <QProcess>
#include <QDir>

class Database {
    private:
        QSqlDatabase db;

    public:
        Database();
        ~Database() {}

        void close();
};

#endif
