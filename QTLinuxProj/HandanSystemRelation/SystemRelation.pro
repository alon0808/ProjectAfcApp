#-------------------------------------------------
#
# Project created by QtCreator 2018-07-13T09:39:25
#
#-------------------------------------------------

QT       -= core gui

TARGET = SystemRelation
TEMPLATE = lib
DEFINES += _linux_

# add include file directory
INCLUDEPATH += ../../code/public
INCLUDEPATH += ../../code/UtilProc
INCLUDEPATH += ../../code/xSys
INCLUDEPATH += ../../code/xStorage
INCLUDEPATH += ../../code/xConfig
INCLUDEPATH += ../../code/peripheral
INCLUDEPATH += ../../code/xCardOpera
INCLUDEPATH += ../../code/transaction

DEFINES += SYSTEMRELATION_LIBRARY

SOURCES += systemrelation.cpp \
    ../../code/application/Application.c \
    ../../code/peripheral/modulePeripheral.c \
    ../../code/peripheral/xDev_linux.c \
    ../../code/peripheral/xDev_win32.c \
    ../../code/ProcessThread/ManageThread.c \
    ../../code/transaction/SecuritySam.c \
    ../../code/transaction/Transaction.c \
    ../../code/transaction/Transaction_M1.c \
    ../../code/TransmitData/TransmitData.c \
    ../../code/TransmitData/xCom_linux.c \
    ../../code/TransmitData/xCom_win32.c \
    ../../code/TransmitData/xSharedMemory_linux.c \
    ../../code/TransmitData/xSharedMemory_win32.c \
    ../../code/TransmitData/xSocketClient.c \
    ../../code/TransmitData/xSocketSrv.c \
    ../../code/TransmitData/xThreadQueue.c \
    ../../code/xConfig/xAppConfig.c \
    ../../code/xStorage/RamStorage.c \
    ../../code/xStorage/xStorage.c \
    ../../code/xSys/xSys_linux.c \
    ../../code/xSys/xSys_win32.c

HEADERS += systemrelation.h\
        systemrelation_global.h \
    ../../code/application/Application.h \
    ../../code/peripheral/modulePeripheral.h \
    ../../code/peripheral/xDev.h \
    ../../code/ProcessThread/ExchThread.h \
    ../../code/transaction/SecuritySam.h \
    ../../code/transaction/Transaction.h \
    ../../code/transaction/Transaction_M1.h \
    ../../code/TransmitData/xCom.h \
    ../../code/TransmitData/xSharedMemory.h \
    ../../code/TransmitData/xSocketClient.h \
    ../../code/TransmitData/xSocketSrv.h \
    ../../code/TransmitData/xThreadQueue.h \
    ../../code/xConfig/xAppConfig.h \
    ../../code/xStorage/xStorage.h \
    ../../code/xSys/xSys.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../Bin
DEPENDPATH += $$PWD/../Bin
