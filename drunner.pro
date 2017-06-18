#-------------------------------------------------
#
# Project created by QtCreator 2016-08-12T14:47:03
#
#-------------------------------------------------

QT       += core network
CONFIG += console qt

TARGET = drunner
TEMPLATE = app


SOURCES += qt-solutions/qtservice/src/qtservice.cpp \
    src/main.cpp \
    src/server.cpp \
    src/drunner.cpp \
    src/parser.cpp \
    src/processlauncher.cpp \
    qt-solutions/qtservice/src/qtunixserversocket.cpp \
    qt-solutions/qtservice/src/qtunixsocket.cpp \
    src/logcollector.cpp

HEADERS  += qt-solutions/qtservice/src/QtService \
    qt-solutions/qtservice/src/qtservice.h \
    qt-solutions/qtservice/src/qtservice_p.h \
    qt-solutions/qtservice/src/QtServiceBase \
    qt-solutions/qtservice/src/QtServiceController \
    src/server.h \
    src/drunner.h \
    src/parser.h \
    src/processlauncher.h \
    src/utils.h \
    qt-solutions/qtservice/src/qtunixserversocket.h \
    qt-solutions/qtservice/src/qtunixsocket.h \
    src/logcollector.h

win32-g++: CONFIG(release, debug|release): {
    SOURCES += qt-solutions/qtservice/src/qtservice_win.cpp
}

unix: {
    SOURCES += qt-solutions/qtservice/src/qtservice_unix.cpp
}
