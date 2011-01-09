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
           HeatMapVisualization.h \
           BarTreeVisualization.h \
           CustomQGraphicsEllipseItem.h \
           CustomQGraphicsScene.h \
           ClickLabel.h

SOURCES += MainWindow.cpp \
           Database.cpp \
           Main.cpp \
           TimeLineVisualization.cpp \
           HeatMapVisualization.cpp \
           BarTreeVisualization.cpp

OTHER_FILES += sql.sql


# For temporary simplified debugging purposes: include a sample events.csv
# file in the source code repository.
OTHER_FILES += events.csv

macx {
    CONFIG -= app_bundle
}

# Copy all OTHER_FILES to the build directory when using shadow builds.
macx { # Temporary hack because it won't work on Windows.
!equals($${PWD}, $${OUT_PWD}) {
    unix:COPY  = cp
    win32:COPY = copy /y
    for(other_file, OTHER_FILES) {
          QMAKE_PRE_LINK += $${COPY} $${PWD}/$${other_file} $${OUT_PWD}/$${other_file};
    }
    win32 {
        replace(QMAKE_PRE_LINK, /, \\)
    }
}
}
