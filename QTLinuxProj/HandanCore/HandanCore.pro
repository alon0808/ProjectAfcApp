#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T05:08:32
#
#-------------------------------------------------

QT       -= core gui

TARGET = HandanCore
TEMPLATE = lib
DEFINES += _linux_

INCLUDEPATH += ../../code/public
INCLUDEPATH += ../../code/UtilProc

DEFINES += HANDANCORE_LIBRARY

SOURCES += handancore.cpp

HEADERS += handancore.h\
        handancore_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
