#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T14:55:34
#
#-------------------------------------------------

TRANSLATIONS+=app_cn.ts

QT       += core gui

TARGET = LtyL12BGUI
QMAKE_CFLAGS = -Wno-psabi -fvisibility=hidden
QMAKE_CXXFLAGS = $$QMAKE_CFLAGS
DEFINES += _linux_

#/////////////////////////////////////////////config start

 LIBS += -L. -linputpanel_p2 -lssl

#DEFINES += APPLICATION_TYPE_D2
DEFINES += APPLICATION_TYPE_P2

TEMPLATE = app
#TEMPLATE = lib

#/////////////////////////////////////////////config end

contains(DEFINES,APPLICATION_TYPE_D2){
contains(TEMPLATE, "lib") {
 LIBS += -L./lib -linputpanel_d2
 }
 TARGET = LtyD2GUI
}

contains(DEFINES,APPLICATION_TYPE_P2){
contains(TEMPLATE, "lib") {
 LIBS += -L . -linputpanel_p2
}
 TARGET = LtyP2GUI
}

TARGET = HandanLinux

CONFIG += plugin

contains(TEMPLATE, "lib") {
 DEFINES += CURRENT_USE_LIB
 DEFINES += CURRENT_ARM_RUN
}

INCLUDEPATH += ../../../../code/public
INCLUDEPATH += ../../../../code/AfcCore/include
INCLUDEPATH += ../../../../code/UtilProc

SOURCES += main.cpp\
    menu_widget.cpp \
    main_page.cpp \
    class/common_interface.cpp \
    class/common/cycloprogress.cpp \
    service_dialog.cpp \
    voice_dialog.cpp \
    head_widget.cpp \
    arrive_widget.cpp \
    steering_wheel.cpp \
    dynamic_text.cpp \
    navigate_left.cpp \
    system_manage/system_setup_widget.cpp \
    system_manage/register_info_widget.cpp \
    system_manage/passenger_flow_widget.cpp \
    system_manage/network_widget.cpp \
    system_manage/switch_config_widget.cpp \
    system_manage/system_maintain_widget.cpp \
    system_manage/history_info_widget.cpp \
    system_manage/coordinate_collection_widget.cpp \
    system_manage/camera_config_widget.cpp \
    system_manage/storage_manage_widget.cpp \
    class/common/comboboxitem.cpp \
    class/common/definitionbutton.cpp \
    class/common/commonshadowwidget.cpp \
    list_view/videotape_query/videotape_query_header.cpp \
    list_view/videotape_query/videotape_query_list.cpp \
    list_view/videotape_query/videotape_query_mode.cpp \
    list_view/videotape_query/videotape_query_table.cpp \
    common_data.cpp \
    class/common/basemodeldata.cpp \
    class/common/commontablemodel.cpp \
    class/common/mytableview.cpp \
    class/common/commondelegatecontrol.cpp \
    class/common/commonlist.cpp \
    class/common/scrollcontroller.cpp \
    class/common/commonuiinit.cpp \
    class/common/mycheckbox.cpp \
    class/ltyuicallinterface.cpp \
    class/cJSON.cpp \
    class/msglist.cpp \
    class/common/customevent.cpp \
    class/common/my_mouse_event.cpp \
    class/common/navigateleftbutton.cpp \
    list_view/common_view/common_query_table.cpp \
    list_view/common_view/common_query_mode.cpp \
    list_view/common_view/common_query_list.cpp \
    list_view/common_view/common_query_header.cpp \
    menu/message_widget.cpp \
    menu/system_manage.cpp \
    menu/about_widget.cpp \
    class/common/my_pushbutton.cpp \
    menu/driver_menu_widget.cpp \
    menu/run_status_widget.cpp \
    video_widget.cpp \
    class/common/my_messagebox.cpp \
    class/common/progress_box.cpp \
    class/common/main_message_dialog.cpp \
    system_manage/videotape_playback.cpp \
    system_manage/videotape_query_widget.cpp \
    system_manage/videotape_export.cpp \
    mainwindow.cpp \
    class/common/my_export.cpp \
    class/common/password_dialog.cpp \
    shundown_dialog.cpp \
    system_manage/senior_setup_widget.cpp \
    system_manage/system_maintain_upgrade.cpp \
    system_manage/network_select.cpp \
    class/ltystring.cpp \
    system_manage/system_maintain_config.cpp \
    menu/videotape_widget.cpp \
    protocol.cpp \
    menu/passageway_widget.cpp \
    class/common/glappeventfilter.cpp \
    class/common/card_dialog.cpp \
    class/common/parent_widget.cpp \
    menu/pay_info_widget.cpp \
    main_page2.cpp \
    class/common/my_messagebox1.cpp \
    class/common/define_screen.cpp \
    class/common/password1_dialog.cpp \
    menu/passenger_widget.cpp \
    system_manage/pos_config_widget.cpp \
    declaration_dialog.cpp \
    state_dialog.cpp \
    setting_dialog.cpp

HEADERS  += \
    menu_widget.h \
    main_page.h \
    class/common_interface.h \
    class/common/cycloprogress.h \
    service_dialog.h \
    voice_dialog.h \
    head_widget.h \
    LtyCommonDefine.h \
    arrive_widget.h \
    steering_wheel.h \
    dynamic_text.h \
    navigate_left.h \
    system_manage/system_setup_widget.h \
    system_manage/register_info_widget.h \
    system_manage/passenger_flow_widget.h \
    system_manage/network_widget.h \
    system_manage/switch_config_widget.h \
    system_manage/system_maintain_widget.h \
    system_manage/history_info_widget.h \
    system_manage/coordinate_collection_widget.h \
    system_manage/camera_config_widget.h \
    system_manage/storage_manage_widget.h \
    LtyCommonStyle.h \
    class/common/comboboxitem.h \
    class/common/definitionbutton.h \
    class/common/commonshadowwidget.h \
    list_view/videotape_query/videotape_query_header.h \
    list_view/videotape_query/videotape_query_list.h \
    list_view/videotape_query/videotape_query_mode.h \
    list_view/videotape_query/videotape_query_table.h \
    common_data.h \
    class/common/basemodeldata.h \
    class/common/commontablemodel.h \
    class/common/mytableview.h \
    class/common/commondelegatecontrol.h \
    class/common/commonlist.h \
    class/common/scrollcontroller.h \
    class/common/commonuiinit.h \
    class/common/mycheckbox.h \
    class/common/iconLocation.h \
    class/ltyuicallinterface.h \
    class/cJSON.h \
    class/msglist.h \
    class/common.h \
    class/common/customevent.h \
    class/common/my_mouse_event.h \
    class/common/navigateleftbutton.h \
    list_view/common_view/common_query_table.h \
    list_view/common_view/common_query_mode.h \
    list_view/common_view/common_query_list.h \
    list_view/common_view/common_query_header.h \
    menu/message_widget.h \
    menu/system_manage.h \
    menu/about_widget.h \
    class/common/my_pushbutton.h \
    menu/driver_menu_widget.h \
    menu/run_status_widget.h \
    video_widget.h \
    class/common/my_messagebox.h \
    class/common/progress_box.h \
    class/common/main_message_dialog.h \
    system_manage/videotape_playback.h \
    system_manage/videotape_query_widget.h \
    system_manage/videotape_export.h \
    system_manage/senior_setup_widget.h \
    system_manage/system_maintain_upgrade.h \
    system_manage/network_select.h \
    class/ltystring.h \
    system_manage/system_maintain_config.h \
    menu/videotape_widget.h \
    protocol.h \
    mainwindow.h \
    class/common/my_export.h \
    class/common/password_dialog.h \
    shundown_dialog.h \
    menu/passageway_widget.h \
    class/common/glappeventfilter.h \
    class/common/card_dialog.h \
    class/common/parent_widget.h \
    menu/pay_info_widget.h \
    main_page2.h \
    class/common/my_messagebox1.h \
    class/common/define_screen.h \
    class/common/password1_dialog.h \
    menu/passenger_widget.h \
    system_manage/pos_config_widget.h \
    declaration_dialog.h \
    state_dialog.h \
    setting_dialog.h

 contains(TEMPLATE, "lib") {
    #HEADERS  += include/syszuxpinyin.h \
   #include/syszuxim.h
    HEADERS  += include/myinputpanelcontext.h \
       include/myinputpanel.h
   }



RESOURCES += \
    LtyApp.qrc


contains(DEFINES,APPLICATION_TYPE_P2){
    RESOURCES += \
     LtyP2.qrc
}


debug{
   #TARGET = $$join(TARGET,,,d)

   DESTDIR = .
   DLLDESTDIR = .

   OBJECTS_DIR = ./temp/debug/obj
   MOC_DIR = ./temp/debug/moc

}else{
   DESTDIR = ./output/release
   DLLDESTDIR = ./output/release

   OBJECTS_DIR = ./temp/release/obj
   MOC_DIR = ./temp/release/moc
}

unix:!macx: LIBS += -L$$PWD/../../../Bin/ -lHandanCore

INCLUDEPATH += $$PWD/../../../Bin
DEPENDPATH += $$PWD/../../../Bin

unix:!macx: LIBS += -L$$PWD/../../../Bin/ -lUtilPro

INCLUDEPATH += $$PWD/../../../Bin
DEPENDPATH += $$PWD/../../../Bin

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../Bin/libUtilPro.a

unix:!macx: LIBS += -L$$PWD/../../../../code/AfcCore/lib/ -lQrcode

INCLUDEPATH += $$PWD/../../../../code/AfcCore/include
DEPENDPATH += $$PWD/../../../../code/AfcCore/include

#unix:!macx: LIBS += -L$$PWD/../Bin/ -lcrypto -lssl

#INCLUDEPATH += $$PWD/../Bin
#DEPENDPATH += $$PWD/../Bin


unix:!macx: LIBS += -L$$PWD/../Bin/ -lcurl

INCLUDEPATH += $$PWD/../Bin
DEPENDPATH += $$PWD/../Bin
