#ifndef DATABASE_H_
#define DATABASE_H_

#include <QSqlDatabase>
#include <QSqlQuery>

class Database {
    private:
        QSqlDatabase db;

    public:
        Database();
        ~Database() {}

        void close();
};

#endif
