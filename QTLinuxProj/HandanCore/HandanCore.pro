#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T05:08:32
#
#-------------------------------------------------

QT       -= core gui
QMAKE_CC = arm-linux-gnueabi-g++
TARGET = HandanCore
TEMPLATE = lib
DEFINES += _linux_
QMAKE_CFLAGS = -fvisibility=hidden -Dhi3536 -Wall -std=c++0x -D_THREAD_SAFE -MMD  -Wno-strict-aliasing
QMAKE_CXXFLAGS = $$QMAKE_CFLAGS

INCLUDEPATH += ../../code/public
INCLUDEPATH += ../../code/UtilProc
INCLUDEPATH += ../../code/AfcCore
INCLUDEPATH += ../../code/AfcCore/include
INCLUDEPATH += ../../code/AfcCore/SlDemo
INCLUDEPATH += ../../code/AfcCore/UnionPay
INCLUDEPATH += ../../code/AfcCore/Crypto

LIBS = -lpthread -L./ltyp2/lib

DEFINES += HANDANCORE_LIBRARY

SOURCES += handancore.cpp \
    ../../code/AfcCore/ICCardLib.c \
    ../../code/AfcCore/GprsSocket.c \
    ../../code/AfcCore/CPUCARJTB.c \
    ../../code/AfcCore/CPUCard.c \
    ../../code/AfcCore/SL8583_4GLinux.c \
    ../../code/AfcCore/RecordFile.c \
    ../../code/AfcCore/PSAMLib.c \
    ../../code/AfcCore/main.c \
    ../../code/AfcCore/inputmisc/GPIOCtrl.c \
    ../../code/AfcCore/inputmisc/IcCardCtrlApi.c \
    ../../code/AfcCore/inputmisc/PsamCard.c \
    ../../code/AfcCore/inputmisc/Uart.c \
    ../../code/AfcCore/SlDemo/DemoMain.c \
    ../../code/AfcCore/SlDemo/GPRS.c \
    ../../code/AfcCore/SlDemo/LightColor.c \
    ../../code/AfcCore/xSys_Lantaiyuan.c \
    ../../code/AfcCore/debugInfor.c \
    ../../code/AfcCore/UnionPay/qpboc_8583.c \
    ../../code/AfcCore/UnionPay/qPBOCrsa.c \
    ../../code/AfcCore/Crypto/MD5.c \
    ../../code/AfcCore/Crypto/MYDES.c \
    ../../code/AfcCore/UnionPay/add_2.c \
    ../../code/AfcCore/UnionPay/add.c

HEADERS += handancore.h\
        handancore_global.h \
    ../../code/AfcCore/libsm.h \
    ../../code/AfcCore/ICCardLib.h \
    ../../code/AfcCore/GprsSocket.h \
    ../../code/AfcCore/GPRSdatatype.h \
    ../../code/AfcCore/DEF8583.h \
    ../../code/AfcCore/CPUCard.h \
    ../../code/AfcCore/SL8583.h \
    ../../code/AfcCore/RecordFile.h \
    ../../code/AfcCore/PSAMLib.h \
    ../../code/AfcCore/include/apparel.h \
    ../../code/AfcCore/include/City-handan.h \
    ../../code/AfcCore/include/CPUCARJTB.h \
    ../../code/AfcCore/include/debug_.h \
    ../../code/AfcCore/include/ev.h \
    ../../code/AfcCore/include/InitSystem.h \
    ../../code/AfcCore/include/kfifo.h \
    ../../code/AfcCore/include/LtyCommon.h \
    ../../code/AfcCore/include/ltycommondatatype.h \
    ../../code/AfcCore/include/ltycommonfunction.h \
    ../../code/AfcCore/include/LtyError.h \
    ../../code/AfcCore/include/LtyLog.h \
    ../../code/AfcCore/include/LtyMessage.h \
    ../../code/AfcCore/include/LtyTimer.h \
    ../../code/AfcCore/include/LtyTypeDef.h \
    ../../code/AfcCore/include/Main_City.h \
    ../../code/AfcCore/include/SlzrError.h \
    ../../code/AfcCore/include/SlzrTypeDef.h \
    ../../code/AfcCore/include/StructDef.h \
    ../../code/AfcCore/include/szct.h \
    ../../code/AfcCore/inputmisc/GPIOCtrl.h \
    ../../code/AfcCore/inputmisc/IcCardCtrlApi.h \
    ../../code/AfcCore/inputmisc/LightBeep.h \
    ../../code/AfcCore/inputmisc/PsamCard.h \
    ../../code/AfcCore/inputmisc/Uart.h \
    ../../code/AfcCore/inputmisc/WatchDog.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/sound/asoc.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/sound/asound_fm.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/sound/emu10k1.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/sound/hdsp.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/sound/hdspm.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/sound/sb16_csp.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/sound/sscape_ioctl.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/sound/tlv.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/sound/type_compat.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/alisp.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/asoundef.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/asoundlib.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/conf.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/control.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/control_external.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/error.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/global.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/hwdep.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/input.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/mixer.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/mixer_abst.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/output.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/pcm.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/pcm_external.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/pcm_extplug.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/pcm_ioplug.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/pcm_old.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/pcm_plugin.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/pcm_rate.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/rawmidi.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/seq.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/seq_event.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/seq_midi_event.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/seqmid.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/timer.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/topology.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/use-case.h \
    ../../code/AfcCore/ltyapp/alsa/include/alsa/version.h \
    ../../code/AfcCore/ltyapp/alsa/include/sys/asoundlib.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/sound/asoc.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/sound/asound_fm.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/sound/emu10k1.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/sound/hdsp.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/sound/hdspm.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/sound/sb16_csp.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/sound/sscape_ioctl.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/sound/tlv.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/sound/type_compat.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/alisp.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/asoundef.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/asoundlib.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/conf.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/control.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/control_external.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/error.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/global.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/hwdep.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/input.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/mixer.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/mixer_abst.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/output.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/pcm.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/pcm_external.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/pcm_extplug.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/pcm_ioplug.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/pcm_old.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/pcm_plugin.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/pcm_rate.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/rawmidi.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/seq.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/seq_event.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/seq_midi_event.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/seqmid.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/timer.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/topology.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/use-case.h \
    ../../code/AfcCore/ltyp2/alsa/include/alsa/version.h \
    ../../code/AfcCore/ltyp2/alsa/include/sys/asoundlib.h \
    ../../code/AfcCore/SlDemo/DemoMain.h \
    ../../code/AfcCore/SlDemo/GPRS.h \
    ../../code/AfcCore/SlDemo/LightColor.h \
    ../../code/AfcCore/xSys_Lantaiyuan.h \
    ../../code/AfcCore/UnionPay/qpboc_8583.h \
    ../../code/AfcCore/UnionPay/qpboc_head.h \
    ../../code/AfcCore/Crypto/MD5.h \
    ../../code/AfcCore/Crypto/MYDES.h \
    ../../code/AfcCore/UnionPay/qPBOC.h \
    ../../code/AfcCore/UnionPay/add_2.h \
    ../../code/AfcCore/UnionPay/add.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    ../../code/AfcCore/Makefile.bak \
    ../../code/AfcCore/Makefile
