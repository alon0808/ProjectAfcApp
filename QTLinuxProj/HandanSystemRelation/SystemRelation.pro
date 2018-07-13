#-------------------------------------------------
#
# Project created by QtCreator 2018-07-13T09:39:25
#
#-------------------------------------------------

QT       -= core gui

TARGET = SystemRelation
TEMPLATE = lib

DEFINES += SYSTEMRELATION_LIBRARY

SOURCES += systemrelation.cpp

HEADERS += systemrelation.h\
        systemrelation_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
