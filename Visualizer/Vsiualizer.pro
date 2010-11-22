TEMPLATE = app
TARGET = Visualizer
DEPENDPATH += .
INCLUDEPATH += .
#RESOURCES += resources.qrc
CONFIG += qt

QT += sql

QMAKE_CXXFLAGS += -O2

HEADERS += MainWindow.h \
           Database.h

SOURCES += MainWindow.cpp \
           Database.cpp \
           Main.cpp
