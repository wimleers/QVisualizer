TEMPLATE = app
TARGET = Visualizer
DEPENDPATH += .
INCLUDEPATH += .
#RESOURCES += resources.qrc
CONFIG += qt
CONFIG += qxt

QT += sql
QXT += core gui

QMAKE_CXXFLAGS += -O2

HEADERS += MainWindow.h \
           Database.h \
           TimeLineVisualization.h \
    Event.h

SOURCES += MainWindow.cpp \
           Database.cpp \
           Main.cpp \
           TimeLineVisualization.cpp

OTHER_FILES += \
    sql.sql
