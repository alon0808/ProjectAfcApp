#-------------------------------------------------
#
# Project created by QtCreator 2018-07-05T02:50:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HandanLinux
TEMPLATE = app
QMAKE_CFLAGS = -Wno-psabi -fvisibility=hidden
QMAKE_CXXFLAGS = $$QMAKE_CFLAGS
DEFINES += _linux_

INCLUDEPATH += ../../code/public
INCLUDEPATH += ../../code/UtilProc


SOURCES += main.cpp\
        mainwindow.cpp \
    ui_accessinterface.cpp \
    customevent.cpp

HEADERS  += mainwindow.h \
    ui_accessinterface.h \
    customevent.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$PWD/../Bin/ -lUtilPro

INCLUDEPATH += $$PWD/../Bin
DEPENDPATH += $$PWD/../Bin

unix:!macx: PRE_TARGETDEPS += $$PWD/../Bin/libUtilPro.a

unix:!macx: LIBS += -L$$PWD/../Bin/ -lSystemRelation

INCLUDEPATH += $$PWD/../Bin
DEPENDPATH += $$PWD/../Bin

DISTFILES += \
    ../preBuild.sh \
    ../postBuild.sh \
    ../../../../../../appConst.sh \
    ../prebuild.sh \
    ../postbuild.sh
