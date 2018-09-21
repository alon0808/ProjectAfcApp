#-------------------------------------------------
#
# Project created by QtCreator 2018-07-12T20:47:15
#
#-------------------------------------------------

QT       -= core gui

QMAKE_CC = arm-linux-gnueabi-gcc
TARGET = UtilPro
TEMPLATE = lib
CONFIG += staticlib
DEFINES += _linux_

INCLUDEPATH += ../../code/AfcCore/include/
INCLUDEPATH += ../../code/public/
INCLUDEPATH += ../../code/UtilProc/
INCLUDEPATH += ../../code/xStorage/

SOURCES += ../../code/UtilProc/UtilityProc.c \
    ../../code/UtilProc/ProcTlv.c \
    ../../code/UtilProc/ConvertData.c \
    ../../code/UtilProc/cJSON.c

HEADERS += utilprostaic.h \
    ../../code/UtilProc/UtilityProc.h \
    ../../code/UtilProc/UtilityDef.h \
    ../../code/UtilProc/ConvertData.h \
    ../../code/public/TypeDef.h \
    ../../code/public/TransmitData.h \
    ../../code/public/ManageThread.h \
    ../../code/public/Macro_Proj.h \
    ../../code/public/debugOut.h \
    ../../code/UtilProc/cJSON.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
