TEMPLATE = app
TARGET = Visualizer
RESOURCES += BarTree/BarTree.qrc
CONFIG += qt
CONFIG += qxt

QT += sql webkit
QXT += core gui

QMAKE_CXXFLAGS += -O2

HEADERS += MainWindow.h \
           Database.h \
           TimeLineVisualization.h \
           Event.h \
           BarTreeVisualization.h

SOURCES += MainWindow.cpp \
           Database.cpp \
           Main.cpp \
           TimeLineVisualization.cpp \
           BarTreeVisualization.cpp

OTHER_FILES += sql.sql
