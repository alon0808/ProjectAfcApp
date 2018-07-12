#-------------------------------------------------
#
# Project created by QtCreator 2018-07-05T02:50:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HandanLinux
TEMPLATE = app
QMAKE_CFLAGS += -Wno-psabi
DEFINES += _linux_

INCLUDEPATH += ../../code/public

SOURCES += main.cpp\
        mainwindow.cpp \
    ui_accessinterface.cpp

HEADERS  += mainwindow.h \
    ui_accessinterface.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$PWD/../Bin/ -lUtilProStaic

INCLUDEPATH += $$PWD/../Bin
DEPENDPATH += $$PWD/../Bin

unix:!macx: PRE_TARGETDEPS += $$PWD/../Bin/libUtilProStaic.a
