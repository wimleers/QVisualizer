#ifndef MYAPPLICATION_H_
#define MYAPPLICATION_H_

#include <QApplication>
#include "QEventLogger.h"

class MyApplication : public QApplication {
    Q_OBJECT

    public:
        explicit MyApplication(int argv, char *args[]) : QApplication(argv, args) {}
};

#endif
