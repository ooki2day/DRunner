#-------------------------------------------------
#
# Project created by QtCreator 2016-08-12T14:47:03
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = drunner
TEMPLATE = app


SOURCES += qt-solutions/qtservice/src/qtservice.cpp \
    qt-solutions/qtservice/src/qtservice_unix.cpp \
    qt-solutions/qtservice/src/qtservice_win.cpp \
    qt-solutions/qtservice/src/qtunixserversocket.cpp \
    qt-solutions/qtservice/src/qtunixsocket.cpp \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS  += qt-solutions/qtservice/src/QtService \
    qt-solutions/qtservice/src/qtservice.h \
    qt-solutions/qtservice/src/qtservice_p.h \
    qt-solutions/qtservice/src/QtServiceBase \
    qt-solutions/qtservice/src/QtServiceController \
    qt-solutions/qtservice/src/qtunixserversocket.h \
    qt-solutions/qtservice/src/qtunixsocket.h \
    src/mainwindow.h

FORMS    += src/mainwindow.ui
