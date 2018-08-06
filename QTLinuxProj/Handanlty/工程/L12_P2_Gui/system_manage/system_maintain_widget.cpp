#include "system_maintain_widget.h"

#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"
#include "./class/common_interface.h"
#include "class/ltystring.h"
#include "system_maintain_config.h"

#include "class/common/my_export.h"
#include "common_data.h"

#include <QDebug>

CSystemMaintainInfo::CSystemMaintainInfo(char _type,QWidget *parent) :
    QWidget(parent)
{
    this->setObjectName("CSystemMaintainInfo");
    this->setStyleSheet("QWidget#CSystemMaintainInfo{border:1px solid #24508F;background-color:#ffffff;}");
    m_type = _type;
    set_type(m_type);

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this,false);
}

void CSystemMaintainInfo::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CSystemMaintainInfo::system_upgrade_init()         //系统升级
{
    this->setFixedSize(620,ROW_HEIGHT * 1);

    m_button_line1 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line1->move(0, ROW_HEIGHT * 0);
    m_button_line1->setDisplayText(tr("应用程序"));
    m_button_line1->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line2 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line2->move(0, ROW_HEIGHT * 1);
    m_button_line2->setDisplayText(tr("MCU程序"));
    m_button_line2->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line3 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line3->move(0, ROW_HEIGHT * 2);
    m_button_line3->setDisplayText(tr("UBoot程序"));
    m_button_line3->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line2->hide();
    m_button_line3->hide();

    connect(m_button_line1,SIGNAL(clicked()),this,SIGNAL(signal_application_program()));
    connect(m_button_line2,SIGNAL(clicked()),this,SIGNAL(signal_mcu_program()));
    connect(m_button_line3,SIGNAL(clicked()),this,SIGNAL(signal_uboot_program()));


}
void CSystemMaintainInfo::station_file_init()           //报站文件
{
#ifdef APPLICATION_TYPE_P2
    this->setFixedSize(620,ROW_HEIGHT * 4);
#else
    this->setFixedSize(620,ROW_HEIGHT * 3);
#endif
    m_button_line1 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line1->move(0, ROW_HEIGHT * 0);
    m_button_line1->setDisplayText(tr("导入报站文件"));
    m_button_line1->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line2 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line2->move(0, ROW_HEIGHT * 1);
    m_button_line2->setDisplayText(tr("导出报站文件"));
    m_button_line2->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line3 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line3->move(0, ROW_HEIGHT * 2);
    m_button_line3->setDisplayText(tr("清除报站文件版本"));
    m_button_line3->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
#ifdef APPLICATION_TYPE_P2
    m_button_line4 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line4->move(0, ROW_HEIGHT * 3);
    m_button_line4->setDisplayText(tr("格式化报站分区"));
    m_button_line4->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line4->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line4->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
#endif

    connect(m_button_line1,SIGNAL(clicked()),this,SIGNAL(signal_import_station_file()));
    connect(m_button_line2,SIGNAL(clicked()),this,SIGNAL(signal_export_station_file()));
    connect(m_button_line3,SIGNAL(clicked()),this,SIGNAL(signal_clean_station_file_version()));
#ifdef APPLICATION_TYPE_P2
    connect(m_button_line4,SIGNAL(clicked()),this,SIGNAL(signal_format_station_file()));
#endif
}
void CSystemMaintainInfo::passenger_flow_config_init()   //客流配置
{
    this->setFixedSize(620,ROW_HEIGHT * 2);

    m_button_line1 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line1->move(0, ROW_HEIGHT * 0);
    m_button_line1->setDisplayText(tr("导入客流配置文件"));
    m_button_line1->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line2 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line2->move(0, ROW_HEIGHT * 1);
    m_button_line2->setDisplayText(tr("参数配置"));
    m_button_line2->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    connect(m_button_line1,SIGNAL(clicked()),this,SIGNAL(signal_import_passenger_flow_config_file()));
    connect(m_button_line2,SIGNAL(clicked()),this,SIGNAL(signal_parameter_config()));

}
void CSystemMaintainInfo::log_record_init()             //日志/记录
{
    this->setFixedSize(620,ROW_HEIGHT * 3);

    m_button_line1 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line1->move(0, ROW_HEIGHT * 0);
    m_button_line1->setDisplayText(tr("导出行车记录"));
    m_button_line1->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line2 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line2->move(0, ROW_HEIGHT * 1);
    m_button_line2->setDisplayText(tr("清除行车日志"));
    m_button_line2->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line3 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line3->move(0, ROW_HEIGHT * 2);
    m_button_line3->setDisplayText(tr("导出系统日志"));
    m_button_line3->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line4 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line4->move(0, ROW_HEIGHT * 3);
    m_button_line4->setDisplayText(tr("清除系统日志"));
    m_button_line4->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line4->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line4->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line4->hide();

    m_button_line5 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line5->move(0, ROW_HEIGHT * 4);
    m_button_line5->setDisplayText(tr("导出操作日志"));
    m_button_line5->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line5->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line5->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line5->hide();

    m_button_line6 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line6->move(0, ROW_HEIGHT * 5);
    m_button_line6->setDisplayText(tr("清除操作日志"));
    m_button_line6->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line6->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line6->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line6->hide();

    connect(m_button_line1,SIGNAL(clicked()),this,SIGNAL(signal_export_vehicle_record()));
    connect(m_button_line2,SIGNAL(clicked()),this,SIGNAL(signal_clean_vehicle_log()));
    connect(m_button_line3,SIGNAL(clicked()),this,SIGNAL(signal_export_system_log()));
    connect(m_button_line4,SIGNAL(clicked()),this,SIGNAL(signal_clean_system_log()));
    connect(m_button_line5,SIGNAL(clicked()),this,SIGNAL(signal_export_operator_log()));
    connect(m_button_line6,SIGNAL(clicked()),this,SIGNAL(signal_clean_operator_logd()));

}

void CSystemMaintainInfo::advert_init()                 //广告
{
    this->setFixedSize(620,ROW_HEIGHT * 3);

    m_button_line1 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line1->move(0, ROW_HEIGHT * 0);
    m_button_line1->setDisplayText(tr("导入广告"));
    m_button_line1->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line2 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line2->move(0, ROW_HEIGHT * 1);
    m_button_line2->setDisplayText(tr("导出广告"));
    m_button_line2->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line3 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line3->move(0, ROW_HEIGHT * 2);
    m_button_line3->setDisplayText(tr("清除广告"));
    m_button_line3->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    connect(m_button_line1,SIGNAL(clicked()),this,SIGNAL(signal_import_picture()));
    connect(m_button_line2,SIGNAL(clicked()),this,SIGNAL(signal_export_picture()));
    connect(m_button_line3,SIGNAL(clicked()),this,SIGNAL(signal_clean_picture()));

}

void CSystemMaintainInfo::card_record_init()            //刷卡记录
{
    this->setFixedSize(620,ROW_HEIGHT * 4);

    m_button_line1 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line1->move(0, ROW_HEIGHT * 0);
    m_button_line1->setDisplayText(tr("导入刷卡参数"));
    m_button_line1->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line2 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line2->move(0, ROW_HEIGHT * 1);
    m_button_line2->setDisplayText(tr("导出刷卡记录"));
    m_button_line2->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line3 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line3->move(0, ROW_HEIGHT * 2);
    m_button_line3->setDisplayText(tr("清除刷卡参数"));
    m_button_line3->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line4 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line4->move(0, ROW_HEIGHT * 3);
    m_button_line4->setDisplayText(tr("清除刷卡记录"));
    m_button_line4->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line4->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line4->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    connect(m_button_line1,SIGNAL(clicked()),this,SIGNAL(signal_import_card_param()));
    connect(m_button_line2,SIGNAL(clicked()),this,SIGNAL(signal_export_card_param()));
    connect(m_button_line3,SIGNAL(clicked()),this,SIGNAL(signal_clean_card_param()));
    connect(m_button_line4,SIGNAL(clicked()),this,SIGNAL(signal_clean_card_record()));

}

void CSystemMaintainInfo::senior_init()           //高级
{
#ifdef APPLICATION_TYPE_P2
    char row_number = 2;
#else
    char row_number = 2;
#endif

    this->setFixedSize(620,ROW_HEIGHT * row_number);

    m_button_line1 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line1->move(0, ROW_HEIGHT * 0);
    m_button_line1->setDisplayText(tr("导入开机LOGO文件"));
    m_button_line1->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line1->hide();

    m_button_line2 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line2->move(0, ROW_HEIGHT * 0);
    m_button_line2->setDisplayText(tr("恢复应用程序"));
    m_button_line2->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line2->hide();

    m_button_line3 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line3->move(0, ROW_HEIGHT * 1);
    m_button_line3->setDisplayText(tr("恢复MCU程序"));
    m_button_line3->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line3->hide();

    m_button_line4 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line4->move(0, ROW_HEIGHT * 2);
    m_button_line4->setDisplayText(tr("恢复UBoot程序"));
    m_button_line4->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line4->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line4->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line4->hide();

    m_button_line5 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line5->move(0, ROW_HEIGHT * 0);
    m_button_line5->setDisplayText(tr("恢复出厂设置"));
    m_button_line5->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line5->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line5->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
#if 0//def APPLICATION_TYPE_P2
    m_button_line6 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line6->move(0, ROW_HEIGHT * 1);
    m_button_line6->setDisplayText(tr("导入广告"));
    m_button_line6->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line6->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line6->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line7 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line7->move(0, ROW_HEIGHT * 2);
    m_button_line7->setDisplayText(tr("导出广告"));
    m_button_line7->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line7->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line7->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line8 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line8->move(0, ROW_HEIGHT * 3);
    m_button_line8->setDisplayText(tr("清除广告"));
    m_button_line8->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line8->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line8->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line9 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line9->move(0, ROW_HEIGHT * 4);
    m_button_line9->setDisplayText(tr("导入刷卡参数"));
    m_button_line9->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line9->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line9->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    m_button_line10 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line10->move(0, ROW_HEIGHT * 5);
    m_button_line10->setDisplayText(tr("导出刷卡参数"));
    m_button_line10->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line10->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line10->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");



    connect(m_button_line6,SIGNAL(clicked()),this,SIGNAL(signal_import_picture()));
    connect(m_button_line7,SIGNAL(clicked()),this,SIGNAL(signal_export_picture()));
    connect(m_button_line8,SIGNAL(clicked()),this,SIGNAL(signal_clean_picture()));
    connect(m_button_line9,SIGNAL(clicked()),this,SIGNAL(signal_import_card_param()));
    connect(m_button_line10,SIGNAL(clicked()),this,SIGNAL(signal_export_card_param()));

#endif
    connect(m_button_line1,SIGNAL(clicked()),this,SIGNAL(signal_import_logo_file()));
    connect(m_button_line2,SIGNAL(clicked()),this,SIGNAL(signal_recovery_application_program()));
    connect(m_button_line3,SIGNAL(clicked()),this,SIGNAL(signal_recovery_mcu_program()));
    connect(m_button_line4,SIGNAL(clicked()),this,SIGNAL(signal_recovery_uboot_program()));
    connect(m_button_line5,SIGNAL(clicked()),this,SIGNAL(signal_recovery_factory_setting()));

    m_button_line11 = new NavigateLeftButton(620,ROW_HEIGHT,NavigateLeftButton::NextMode,this);
    m_button_line11->move(0, ROW_HEIGHT * (row_number - 1));
    m_button_line11->setDisplayText(tr("重启设备"));
    m_button_line11->setNormalIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line11->setPressedIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");
    m_button_line11->setActiveIconStr(":/img/res/main_page/menu/system_manage/network/icon_right_arrow.png");

    connect(m_button_line11,SIGNAL(clicked()),this,SIGNAL(signal_reboot_device()));

}
void CSystemMaintainInfo::set_type(char _type)
{
    switch(_type)
    {
       case kEnumSystemUpgrade:         //系统升级
       {
            system_upgrade_init();
       }break;
       case kEnumStationFile:           //报站文件
       {
            station_file_init();
       }break;
        case kEnumPassengerFlowConfig:   //客流配置
        {
            passenger_flow_config_init();
       }break;
        case kEnumLogRecord:             //日志/记录
        {
            log_record_init();
       }break;
       case kEnumAdvert:                //广告
       {
            advert_init();
       }break;
       case kEnumCardRecord:            //刷卡记录
       {
            card_record_init();
       }break;
       case kEnumSenior:           //高级
        {
            senior_init();
       }break;
    }
}

CSystemMaintainWidget::CSystemMaintainWidget(QWidget *parent) :
    QWidget(parent)
{
    m_define_screen = new CMyDefineScreen();

    this->setObjectName("CSystemMaintainWidget");
    this->setStyleSheet("QWidget#CSystemMaintainWidget{background-color:#ffffff;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->move(0,0);

        QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    pWidget->move(0,0);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");

    scroll_layout();
    system_maintain_init_new();


    m_define_screen->add_parent(this);
    m_define_screen->add_parent(m_widget_main);

}

void CSystemMaintainWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CSystemMaintainWidget::scroll_layout()
{

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

#ifdef APPLICATION_TYPE_P2
    m_scroll_widget_width = NAVIGATE_RIGHT_WIDGET_WIDTH - 15;
    m_scroll_widget_height = NAVIGATE_RIGHT_WIDGET_HEIGHT - 50 - 50;
#else
    m_scroll_widget_width = NAVIGATE_RIGHT_WIDGET_WIDTH - 30;
    m_scroll_widget_height = NAVIGATE_RIGHT_WIDGET_HEIGHT - 50 - 50;
#endif
    m_scroll_widget = new QWidget(this);
    m_scroll_widget->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_scroll_widget->move(250 - 250,50);
    m_scroll_widget->setStyleSheet("QWidget{border:0px solid #DD4F43;background: transparent;}");

    m_pMainLayout = new QVBoxLayout();//左 上 右 下
    m_pMainLayout->setContentsMargins(0,0,0,0);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setAlignment(Qt::AlignLeft/* | Qt::AlignTop*/);
    m_pMainLayout->addStretch(0);
    m_scroll_widget->setLayout(m_pMainLayout);

    m_scrollarea_1=new QScrollArea(m_scroll_widget);  //新建了一条滚动条
    m_scrollarea_1->setFixedHeight(m_define_screen->get_change_factor_y(m_scroll_widget_height));
    m_scrollarea_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollarea_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pMainLayout->addWidget(m_scrollarea_1);

    m_ScrollController = new ScrollController(m_scroll_widget);
    m_ScrollController->addMonitorWidget((QScrollArea *)(m_scrollarea_1));
    m_ScrollController->setVerticalScrollBarStyleSheet(CCommonData::getInstance()->get_scroll_sheet(),0,50,50);
    m_ScrollController->set_scroll_right_length(20);

    QWidget * pwidget=new QWidget(this);

    pwidget->setAttribute(Qt::WA_StyledBackground);
    pwidget->setObjectName("pwidget");
    pwidget->setStyleSheet("QWidget#pwidget { background: transparent;}");

    m_vboxlayout_widget=new QVBoxLayout(pwidget);
    m_vboxlayout_widget->setContentsMargins(0,0,0,0);
    m_vboxlayout_widget->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    m_vboxlayout_widget->setSpacing(0);


    pwidget->setLayout(m_vboxlayout_widget);
    m_scrollarea_1->setWidget(pwidget);
    m_scrollarea_1->setWidgetResizable(true);


    m_widget_main = new QWidget;
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);

    m_widget_main->setStyleSheet("QWidget{background: transparent;}");
    m_vboxlayout_widget->addWidget(m_widget_main);



}

void CSystemMaintainWidget::system_maintain_init_new()
{
    m_button_1 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_1->setDisplayText(tr("系统升级"));
    m_button_1->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_1->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_1->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_2 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_2->setDisplayText(tr("报站文件"));
    m_button_2->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_2->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_2->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_3 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_3->setDisplayText(tr("客流配置"));
    m_button_3->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_3->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_3->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_4 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_4->setDisplayText(tr("日志/记录"));
    m_button_4->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_4->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_4->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_4_1 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_4_1->setDisplayText(tr("广告"));
    m_button_4_1->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_4_1->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_4_1->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_4_2 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_4_2->setDisplayText(tr("刷卡记录"));
    m_button_4_2->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_4_2->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_4_2->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_5 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_5->setDisplayText(tr("高级"));
    m_button_5->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_5->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_5->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_SystemMaintainInfo1 = new CSystemMaintainInfo(CSystemMaintainInfo::kEnumSystemUpgrade,m_widget_main);
    m_SystemMaintainInfo2 = new CSystemMaintainInfo(CSystemMaintainInfo::kEnumStationFile,m_widget_main);
    m_SystemMaintainInfo3 = new CSystemMaintainInfo(CSystemMaintainInfo::kEnumPassengerFlowConfig,m_widget_main);
    m_SystemMaintainInfo4 = new CSystemMaintainInfo(CSystemMaintainInfo::kEnumLogRecord,m_widget_main);
    m_SystemMaintainInfo4_1 = new CSystemMaintainInfo(CSystemMaintainInfo::kEnumAdvert,m_widget_main);
    m_SystemMaintainInfo4_2 = new CSystemMaintainInfo(CSystemMaintainInfo::kEnumCardRecord,m_widget_main);
    m_SystemMaintainInfo5 = new CSystemMaintainInfo(CSystemMaintainInfo::kEnumSenior,m_widget_main);
    m_SystemMaintainInfo3->hide();
    //系统升级
    connect(m_SystemMaintainInfo1,SIGNAL(signal_application_program()),this,SLOT(slot_application_program()));
    connect(m_SystemMaintainInfo1,SIGNAL(signal_mcu_program()),this,SLOT(slot_mcu_program()));
    connect(m_SystemMaintainInfo1,SIGNAL(signal_uboot_program()),this,SLOT(slot_uboot_program()));
    //报站文件
    connect(m_SystemMaintainInfo2,SIGNAL(signal_import_station_file()),this,SLOT(slot_import_station_file()));
    connect(m_SystemMaintainInfo2,SIGNAL(signal_export_station_file()),this,SLOT(slot_export_station_file()));
    connect(m_SystemMaintainInfo2,SIGNAL(signal_clean_station_file_version()),this,SLOT(slot_clean_station_file_version()));
    connect(m_SystemMaintainInfo2,SIGNAL(signal_format_station_file()),this,SLOT(slot_format_station_file()));
    //客流配置
    connect(m_SystemMaintainInfo3,SIGNAL(signal_import_passenger_flow_config_file()),this,SLOT(slot_import_passenger_flow_config_file()));
    connect(m_SystemMaintainInfo3,SIGNAL(signal_parameter_config()),this,SLOT(slot_parameter_config()));
    //日志/记录
    connect(m_SystemMaintainInfo4,SIGNAL(signal_export_vehicle_record()),this,SLOT(slot_export_vehicle_record()));
    connect(m_SystemMaintainInfo4,SIGNAL(signal_clean_vehicle_log()),this,SLOT(slot_clean_vehicle_log()));
    connect(m_SystemMaintainInfo4,SIGNAL(signal_export_system_log()),this,SLOT(slot_export_system_log()));
    connect(m_SystemMaintainInfo4,SIGNAL(signal_clean_system_log()),this,SLOT(slot_clean_system_log()));
    connect(m_SystemMaintainInfo4,SIGNAL(signal_export_operator_log()),this,SLOT(slot_export_operator_log()));
    connect(m_SystemMaintainInfo4,SIGNAL(signal_clean_operator_logd()),this,SLOT(slot_clean_operator_logd()));
    //广告
    connect(m_SystemMaintainInfo4_1,SIGNAL(signal_import_picture()),this,SLOT(slot_import_picture()));
    connect(m_SystemMaintainInfo4_1,SIGNAL(signal_export_picture()),this,SLOT(slot_export_picture()));
    connect(m_SystemMaintainInfo4_1,SIGNAL(signal_clean_picture()),this,SLOT(slot_clean_picture()));
    //刷卡记录
    connect(m_SystemMaintainInfo4_2,SIGNAL(signal_import_card_param()),this,SLOT(slot_import_card_param()));
    connect(m_SystemMaintainInfo4_2,SIGNAL(signal_export_card_param()),this,SLOT(slot_export_card_param()));
    connect(m_SystemMaintainInfo4_2,SIGNAL(signal_clean_card_param()),this,SLOT(slot_clean_card_param()));
    connect(m_SystemMaintainInfo4_2,SIGNAL(signal_clean_card_record()),this,SLOT(slot_clean_card_record()));

    //高级
    connect(m_SystemMaintainInfo5,SIGNAL(signal_import_logo_file()),this,SLOT(slot_import_logo_file()));
    connect(m_SystemMaintainInfo5,SIGNAL(signal_recovery_application_program()),this,SLOT(slot_recovery_application_program()));
    connect(m_SystemMaintainInfo5,SIGNAL(signal_recovery_mcu_program()),this,SLOT(slot_recovery_mcu_program()));
    connect(m_SystemMaintainInfo5,SIGNAL(signal_recovery_uboot_program()),this,SLOT(slot_recovery_uboot_program()));
    connect(m_SystemMaintainInfo5,SIGNAL(signal_recovery_factory_setting()),this,SLOT(slot_recovery_factory_setting()));

    //connect(m_SystemMaintainInfo5,SIGNAL(signal_import_picture()),this,SLOT(slot_import_picture()));
    //connect(m_SystemMaintainInfo5,SIGNAL(signal_export_picture()),this,SLOT(slot_export_picture()));
    //connect(m_SystemMaintainInfo5,SIGNAL(signal_clean_picture()),this,SLOT(slot_clean_picture()));
    //connect(m_SystemMaintainInfo5,SIGNAL(signal_import_card_param()),this,SLOT(slot_import_card_param()));
    //connect(m_SystemMaintainInfo5,SIGNAL(signal_export_card_param()),this,SLOT(slot_export_card_param()));

    connect(m_SystemMaintainInfo5,SIGNAL(signal_reboot_device()),this,SLOT(slot_reboot_device()));

    m_label_space1  = new QLabel(m_widget_main);
    m_label_space1->setFixedHeight(10);
    m_label_space1->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space2  = new QLabel(m_widget_main);
    m_label_space2->setFixedHeight(10);
    m_label_space2->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space3  = new QLabel(m_widget_main);
    m_label_space3->setFixedHeight(10);
    m_label_space3->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space4  = new QLabel(m_widget_main);
    m_label_space4->setFixedHeight(10);
    m_label_space4->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space4_1  = new QLabel(m_widget_main);
    m_label_space4_1->setFixedHeight(10);
    m_label_space4_1->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space4_2  = new QLabel(m_widget_main);
    m_label_space4_2->setFixedHeight(10);
    m_label_space4_2->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_vboxlayout_network = new QVBoxLayout(m_widget_main);//左 上 右 下
    m_vboxlayout_network->setContentsMargins(m_define_screen->get_change_factor_x(290 - 250),m_define_screen->get_change_factor_y(158 - MAIN_HEAD_HEIGHT - 50),0,0);
    m_vboxlayout_network->setSpacing(0);
    m_vboxlayout_network->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_vboxlayout_network->addWidget(m_button_1);
    m_vboxlayout_network->addWidget(m_SystemMaintainInfo1);
    m_vboxlayout_network->addWidget(m_label_space1);

    m_vboxlayout_network->addWidget(m_button_2);
    m_vboxlayout_network->addWidget(m_SystemMaintainInfo2);
    m_vboxlayout_network->addWidget(m_label_space2);

    m_vboxlayout_network->addWidget(m_button_3);
    m_vboxlayout_network->addWidget(m_SystemMaintainInfo3);
    m_vboxlayout_network->addWidget(m_label_space3);

    m_vboxlayout_network->addWidget(m_button_4);
    m_vboxlayout_network->addWidget(m_SystemMaintainInfo4);
    m_vboxlayout_network->addWidget(m_label_space4);

    m_vboxlayout_network->addWidget(m_button_4_1);
    m_vboxlayout_network->addWidget(m_SystemMaintainInfo4_1);
    m_vboxlayout_network->addWidget(m_label_space4_1);

    m_vboxlayout_network->addWidget(m_button_4_2);
    m_vboxlayout_network->addWidget(m_SystemMaintainInfo4_2);
    m_vboxlayout_network->addWidget(m_label_space4_2);

    m_vboxlayout_network->addWidget(m_button_5);
    m_vboxlayout_network->addWidget(m_SystemMaintainInfo5);


    connect(m_button_1,SIGNAL(clicked()),this,SLOT(slot_button_1()));
    connect(m_button_2,SIGNAL(clicked()),this,SLOT(slot_button_2()));
    connect(m_button_3,SIGNAL(clicked()),this,SLOT(slot_button_3()));
    connect(m_button_4,SIGNAL(clicked()),this,SLOT(slot_button_4()));
    connect(m_button_4_1,SIGNAL(clicked()),this,SLOT(slot_button_4_1()));
    connect(m_button_4_2,SIGNAL(clicked()),this,SLOT(slot_button_4_2()));
    connect(m_button_5,SIGNAL(clicked()),this,SLOT(slot_button_5()));

#ifdef APPLICATION_TYPE_P2
    m_button_4_1->show();
    m_SystemMaintainInfo4_1->show();
    m_label_space4_1->show();
    m_button_4_2->show();
    m_SystemMaintainInfo4_2->show();
    m_label_space4_2->show();
#else
    m_button_4_1->hide();
    m_SystemMaintainInfo4_1->hide();
    m_label_space4_1->hide();
    m_button_4_2->hide();
    m_SystemMaintainInfo4_2->hide();
    m_label_space4_2->hide();
#endif

    m_button_3->hide();
    m_SystemMaintainInfo3->hide();
    m_label_space3->hide();

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_4_1 = false;
    m_button_press_4_2 = false;
    m_button_press_5 = false;

    m_SystemMaintainInfo1->hide();
    m_SystemMaintainInfo2->hide();
    m_SystemMaintainInfo3->hide();
    m_SystemMaintainInfo4->hide();
    m_SystemMaintainInfo4_1->hide();
    m_SystemMaintainInfo4_2->hide();
    m_SystemMaintainInfo5->hide();


    m_application_list = NULL;
    m_mcu_list = NULL;
    m_uboot_list = NULL;
    m_import_station_list = NULL;
    m_import_logo_list = NULL;

}
void CSystemMaintainWidget::slot_button_1()
{
     if(m_button_press_1)
    {
        m_button_press_1 = false;
        m_button_1->setSelected(false);
        m_SystemMaintainInfo1->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_1 = true;
        m_button_1->setSelected(true);
        m_SystemMaintainInfo1->show();
#ifdef APPLICATION_TYPE_P2
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 1));
#else
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
#endif
    }
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_4_1->setSelected(false);
    m_button_4_2->setSelected(false);
    m_button_5->setSelected(false);

    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_4_1 = false;
    m_button_press_4_2 = false;
    m_button_press_5 = false;

    m_SystemMaintainInfo2->hide();
    m_SystemMaintainInfo3->hide();
    m_SystemMaintainInfo4->hide();
    m_SystemMaintainInfo4_1->hide();
    m_SystemMaintainInfo4_2->hide();
    m_SystemMaintainInfo5->hide();


}
void CSystemMaintainWidget::slot_button_2()
{
    if(m_button_press_2)
    {
        m_button_press_2 = false;
        m_button_2->setSelected(false);
        m_SystemMaintainInfo2->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_2 = true;
        m_button_2->setSelected(true);
        m_SystemMaintainInfo2->show();
#ifdef APPLICATION_TYPE_P2
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 4));
#else
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 2));
#endif
    }
    m_button_1->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_4_1->setSelected(false);
    m_button_4_2->setSelected(false);
    m_button_5->setSelected(false);


    m_button_press_1 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_4_1 = false;
    m_button_press_4_2 = false;
    m_button_press_5 = false;


    m_SystemMaintainInfo1->hide();
    m_SystemMaintainInfo3->hide();
    m_SystemMaintainInfo4->hide();
    m_SystemMaintainInfo4_1->hide();
    m_SystemMaintainInfo4_2->hide();
    m_SystemMaintainInfo5->hide();

}
void CSystemMaintainWidget::slot_button_3()
{
    if(m_button_press_3)
    {
        m_button_press_3 = false;
        m_button_3->setSelected(false);
        m_SystemMaintainInfo3->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_3 = true;
        m_button_3->setSelected(true);
        m_SystemMaintainInfo3->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 1));

    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_4->setSelected(false);
    m_button_4_1->setSelected(false);
    m_button_4_2->setSelected(false);
    m_button_5->setSelected(false);

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_4 = false;
    m_button_press_4_1 = false;
    m_button_press_4_2 = false;
    m_button_press_5 = false;

    m_SystemMaintainInfo1->hide();
    m_SystemMaintainInfo2->hide();
    m_SystemMaintainInfo4->hide();
    m_SystemMaintainInfo4_1->hide();
    m_SystemMaintainInfo4_2->hide();
    m_SystemMaintainInfo5->hide();

}
void CSystemMaintainWidget::slot_button_4()
{
    if(m_button_press_4)
    {
        m_button_press_4 = false;
        m_button_4->setSelected(false);
        m_SystemMaintainInfo4->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_4 = true;
        m_button_4->setSelected(true);
        m_SystemMaintainInfo4->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 3 + 50));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4_1->setSelected(false);
    m_button_4_2->setSelected(false);
    m_button_5->setSelected(false);


    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4_1 = false;
    m_button_press_4_2 = false;
    m_button_press_5 = false;

    m_SystemMaintainInfo1->hide();
    m_SystemMaintainInfo2->hide();
    m_SystemMaintainInfo3->hide();
    m_SystemMaintainInfo4_1->hide();
    m_SystemMaintainInfo4_2->hide();
    m_SystemMaintainInfo5->hide();

}

void CSystemMaintainWidget::slot_button_4_1()
{
    if(m_button_press_4_1)
    {
        m_button_press_4_1 = false;
        m_button_4_1->setSelected(false);
        m_SystemMaintainInfo4_1->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_4_1 = true;
        m_button_4_1->setSelected(true);
        m_SystemMaintainInfo4_1->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 2 + 50));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_4_2->setSelected(false);
    m_button_5->setSelected(false);


    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_4_2 = false;
    m_button_press_5 = false;

    m_SystemMaintainInfo1->hide();
    m_SystemMaintainInfo2->hide();
    m_SystemMaintainInfo3->hide();
    m_SystemMaintainInfo4->hide();
    m_SystemMaintainInfo4_2->hide();
    m_SystemMaintainInfo5->hide();

}
void CSystemMaintainWidget::slot_button_4_2()
{
    if(m_button_press_4_2)
    {
        m_button_press_4_2 = false;
        m_button_4_2->setSelected(false);
        m_SystemMaintainInfo4_2->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_4_2 = true;
        m_button_4_2->setSelected(true);
        m_SystemMaintainInfo4_2->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 3 + 50));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_4_1->setSelected(false);
    m_button_5->setSelected(false);


    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_4_1 = false;
    m_button_press_5 = false;

    m_SystemMaintainInfo1->hide();
    m_SystemMaintainInfo2->hide();
    m_SystemMaintainInfo3->hide();
    m_SystemMaintainInfo4->hide();
    m_SystemMaintainInfo4_1->hide();
    m_SystemMaintainInfo5->hide();
}


void CSystemMaintainWidget::slot_button_5()
{
    if(m_button_press_5)
    {
        m_button_press_5 = false;
        m_button_5->setSelected(false);
        m_SystemMaintainInfo5->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_5 = true;
        m_button_5->setSelected(true);
        m_SystemMaintainInfo5->show();
#ifdef APPLICATION_TYPE_P2
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height + ROW_HEIGHT * 2));
#else
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
#endif
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_4_1->setSelected(false);
    m_button_4_2->setSelected(false);

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_button_press_4_1 = false;
    m_button_press_4_2 = false;

    m_SystemMaintainInfo1->hide();
    m_SystemMaintainInfo2->hide();
    m_SystemMaintainInfo3->hide();
    m_SystemMaintainInfo4->hide();
    m_SystemMaintainInfo4_1->hide();
    m_SystemMaintainInfo4_2->hide();

}
#include "system_maintain_upgrade.h"
#include "mainwindow.h"
#include "class/common/progress_box.h"
#include "class/common/my_messagebox.h"
       //系统升级
void CSystemMaintainWidget::slot_application_program()//应用程序
{
    QString str_title = tr("应用程序升级");
    if(NULL == m_application_list)
    {
        m_application_list = new SystemMaintainUpgrade(CSystemSystemMaintain::kEnumApplicationProgram,MainWindow::getInstance());
    }
    //int CCommonInterface::read_dir_file(QString _dir,QStringList &_file_list,QString _filter_name)
#if 0
    QStringList file_list;
    QString file_name;
    file_list.clear();
    if(0 == CCommonInterface::read_dir_file(UDISK_PATH,file_list,"*.sw"))
    {
        for(int index = 0;index < file_list.size();index++)
        {
            CSoftwareUpgrade *pSoftwareUpgrade = new CSoftwareUpgrade;
            pSoftwareUpgrade->m_number = index + 1;
            file_name = file_list.at(index);
            pSoftwareUpgrade->m_file_name = file_name.remove(0,file_name.lastIndexOf('/')+1);
            m_application_list->add_data(pSoftwareUpgrade);

        }
    }
#endif
    m_application_list->start_get_upgrade();


    m_application_list->set_title(str_title);

   // CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainList,
   //                                                         CSystemSystemMaintain::kEnumApplicationProgram);

    m_application_list->exec();

    do
    {
        if(SystemMaintainUpgrade::KEnumButtonOk != m_application_list->get_result())
        {
            break;
        }



        CSoftwareUpgrade *pCSoftwareUpgrade = (CSoftwareUpgrade *)m_application_list->get_row_info();
        if(pCSoftwareUpgrade)
        {
            user_select_tip(CSystemSystemMaintain::kEnumApplicationProgram,str_title,false,pCSoftwareUpgrade);
        }


    }while(0);


}
void CSystemMaintainWidget::slot_mcu_program()//MCU程序
{
    QString str_title = tr("MCU程序升级");
    if(NULL == m_mcu_list)
    {
        m_mcu_list = new SystemMaintainUpgrade(CSystemSystemMaintain::kEnumMcuProgram,MainWindow::getInstance());
    }


    m_mcu_list->set_title(str_title);

    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainList,
                                                            CSystemSystemMaintain::kEnumMcuProgram);

    m_mcu_list->exec();

    if(SystemMaintainUpgrade::KEnumButtonOk == m_mcu_list->get_result())
    {
       user_select_tip(CSystemSystemMaintain::kEnumMcuProgram,str_title);
    }
}
void CSystemMaintainWidget::slot_uboot_program()//UBoot程序
{
    QString str_title = tr("UBoot程序升级");
    if(NULL == m_uboot_list)
    {
        m_uboot_list = new SystemMaintainUpgrade(CSystemSystemMaintain::kEnumUBootProgram,MainWindow::getInstance());
    }

        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainList,
                                                            CSystemSystemMaintain::kEnumUBootProgram);


    m_uboot_list->set_title(str_title);
    m_uboot_list->exec();

    if(SystemMaintainUpgrade::KEnumButtonOk == m_uboot_list->get_result())
    {
       user_select_tip(CSystemSystemMaintain::kEnumUBootProgram,str_title);
    }
}
//报站文件
void CSystemMaintainWidget::slot_import_station_file()//导入报站文件
{
    user_operate_tip(CSystemSystemMaintain::kEnumImportStationFile,tr("导入报站文件"));
#if 0
    QString str_title = tr("导入报站文件");
    if(NULL == m_import_station_list)
    {
        m_import_station_list = new SystemMaintainUpgrade(CSystemSystemMaintain::kEnumImportStationFile,MainWindow::getInstance());
    }

            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainList,
                                                            CSystemSystemMaintain::kEnumImportStationFile);


    m_import_station_list->set_title(str_title);
    m_import_station_list->exec();

    if(SystemMaintainUpgrade::KEnumButtonOk == m_import_station_list->get_result())
    {
        user_select_tip(CSystemSystemMaintain::kEnumImportStationFile,str_title);
    }
#endif
}




void CSystemMaintainWidget::slot_export_station_file()//导出报站文件
{
    user_operate_tip(CSystemSystemMaintain::kEnumExportStationFile,tr("导出报站文件"));
}
void CSystemMaintainWidget::slot_clean_station_file_version()//清除报站文件版本
{
    user_operate_tip(CSystemSystemMaintain::kEnumClearStationFile,tr("清除报站文件版本"));
}

void CSystemMaintainWidget::slot_format_station_file()//格式化报站分区
{
    user_operate_tip(CSystemSystemMaintain::kEnumFormatStationFile,tr("格式化报站分区"));
}


//客流配置
void CSystemMaintainWidget::slot_import_passenger_flow_config_file()//导入客流配置文件
{
}
void CSystemMaintainWidget::slot_parameter_config()//参数配置
{
#if 0
    CSystemMaintainConfig *config_dialog = NULL;

    if(NULL == config_dialog)
    {
        config_dialog = new CSystemMaintainConfig(CMainPage::getInstance());
    }

    config_dialog->move(212,76);
    config_dialog->exec();
#endif
}
//日志/记录
void CSystemMaintainWidget::slot_export_vehicle_record() //导出行车记录
{
    user_operate_tip(CSystemSystemMaintain::kEnumExportVehicleRecord,tr("导出行车记录"));
}
void CSystemMaintainWidget::slot_clean_vehicle_log()//清除行车日志
{
    user_operate_tip(CSystemSystemMaintain::kEnumClearVehicleRecord,tr("清除行车日志"));
}
void CSystemMaintainWidget::slot_export_system_log()//导出系统日志
{
    user_select_tip(CSystemSystemMaintain::kEnumExportSystemLog,tr("导出系统日志"),false);
}
void CSystemMaintainWidget::slot_clean_system_log()//清除系统日志
{
    user_operate_tip(CSystemSystemMaintain::kEnumClearSystemLog,tr("清除系统日志"));
}
void CSystemMaintainWidget::slot_export_operator_log()//导出操作日志
{
    user_operate_tip(CSystemSystemMaintain::kEnumExportOperationLog,tr("导出操作日志"));
}
void CSystemMaintainWidget::slot_clean_operator_logd()//清除操作日志
{
    user_operate_tip(CSystemSystemMaintain::kEnumClearOperationLog,tr("清除操作日志"));
}
//高级
void CSystemMaintainWidget::slot_import_logo_file()//导入开机LOGO文件
{
    QString str_title = tr("导入开机LOGO文件");
    if(NULL == m_import_logo_list)
    {
        m_import_logo_list = new SystemMaintainUpgrade(CSystemSystemMaintain::kEnumImportLogoFile,MainWindow::getInstance());
    }
                CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainList,
                                                            CSystemSystemMaintain::kEnumImportLogoFile);


    m_import_logo_list->set_title(str_title);
    m_import_logo_list->exec();

    if(SystemMaintainUpgrade::KEnumButtonOk == m_import_logo_list->get_result())
    {
        user_select_tip(CSystemSystemMaintain::kEnumImportLogoFile,str_title);
    }
}
void CSystemMaintainWidget::slot_recovery_application_program()//恢复应用程序
{
    user_operate_tip(CSystemSystemMaintain::kEnumResumeApplicationProgram,tr("恢复应用程序"));
}
void CSystemMaintainWidget::slot_recovery_mcu_program()//恢复MCU程序
{
    user_operate_tip(CSystemSystemMaintain::kEnumResumeMcuProgram,tr("恢复MCU程序"));
}
void CSystemMaintainWidget::slot_recovery_uboot_program()//恢复UBoot程序
{
    user_operate_tip(CSystemSystemMaintain::kEnumResumeUBootProgram,tr("恢复UBoot程序"));
}
void CSystemMaintainWidget::slot_recovery_factory_setting()//恢复出厂设置
{
    user_operate_tip(CSystemSystemMaintain::kEnumResumeFactoryConfig,tr("恢复出厂设置"));
}

void CSystemMaintainWidget::slot_import_picture()  //导入广告
{
    user_select_tip(CSystemSystemMaintain::kEnumImportPicture,tr("导入广告"),false);
}

void CSystemMaintainWidget::slot_export_picture()  //导出广告
{
    user_select_tip(CSystemSystemMaintain::kEnumExportPicture,tr("导出广告"),false);
}

void CSystemMaintainWidget::slot_clean_picture()  //清除广告
{
    user_select_tip(CSystemSystemMaintain::kEnumCleanPicture,tr("清除广告"),false);
}

void CSystemMaintainWidget::slot_import_card_param()  //导入刷卡参数
{
    user_operate_tip(CSystemSystemMaintain::kEnumImportCardParam,tr("导入刷卡参数"));
}
void CSystemMaintainWidget::slot_export_card_param()  //导出刷卡参数
{
    user_operate_tip(CSystemSystemMaintain::kEnumExportCardParam,tr("导出刷卡参数"));
}

void CSystemMaintainWidget::slot_clean_card_param()   //清除刷卡参数
{
    user_operate_tip(CSystemSystemMaintain::kEnumCleanCardParam,tr("清除刷卡参数"));
}
void CSystemMaintainWidget::slot_clean_card_record()  //清除刷卡记录
{
    user_operate_tip(CSystemSystemMaintain::kEnumCleanCardRecord,tr("清除刷卡记录"));
}

void CSystemMaintainWidget::slot_reboot_device()  //重启设备
{
    CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("确认重启设备？"),CLtyString::getInstance()->m_confirm,CLtyString::getInstance()->m_cancel);
    CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerCancel);
    CMyMessageBox::getInstance()->exec();

    if(CMyMessageBox::KEnumButtonOk == CMyMessageBox::getInstance()->get_result())
    {
        CProtocol::getInstance()->call_json_system_operate_set(1);
        MainWindow::getInstance()->slot_pushbutton_shutdown();
    }

}

void CSystemMaintainWidget::user_operate_tip(char _type,const QString &_tip)
{
    do
    {

        if(CSystemSystemMaintain::kEnumResumeFactoryConfig == _type)
        {
            CMyMessageBox::getInstance()->set_content(_tip,tr("请提前备份用户数据,\n继续恢复出厂设置") +tr("") + tr("?"),CLtyString::getInstance()->m_confirm,CLtyString::getInstance()->m_cancel);
        }
        else
        {
            CMyMessageBox::getInstance()->set_content(_tip,tr("确认") + _tip + tr("?"),CLtyString::getInstance()->m_confirm,CLtyString::getInstance()->m_cancel);
        }
        CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerCancel);
        CMyMessageBox::getInstance()->exec();

        if(CMyMessageBox::KEnumButtonOk == CMyMessageBox::getInstance()->get_result())
        {

            switch(_type)
            {
                //报站文件
                case CSystemSystemMaintain::kEnumImportStationFile: //导入报站文件
                {
                     CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumImportStationFile);
                }break;
                case CSystemSystemMaintain::kEnumExportStationFile://导出报站文件
                {
                     CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumExportStationFile);
                }break;
                case CSystemSystemMaintain::kEnumClearStationFile://清除报站文件版本
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumClearStationFile);
                }break;
                //日志/记录
                case CSystemSystemMaintain::kEnumExportVehicleRecord://导出行车记录
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumExportVehicleRecord);
                }break;
                case CSystemSystemMaintain::kEnumClearVehicleRecord://清除行车日志
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumClearVehicleRecord);
                }break;
                case CSystemSystemMaintain::kEnumExportSystemLog://导出系统日志
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumExportSystemLog);
                }break;
                case CSystemSystemMaintain::kEnumClearSystemLog://清除系统日志
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumClearSystemLog);
                }break;
                case CSystemSystemMaintain::kEnumExportOperationLog://导出操作日志
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumExportOperationLog);
                }break;
                case CSystemSystemMaintain::kEnumClearOperationLog://清除操作日志
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumClearOperationLog);
                }break;
                //高级
                case CSystemSystemMaintain::kEnumResumeApplicationProgram://恢复应用程序
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumResumeApplicationProgram);
                }break;
                case CSystemSystemMaintain::kEnumResumeMcuProgram://恢复MCU程序
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumResumeMcuProgram);
                }break;
                case CSystemSystemMaintain::kEnumResumeUBootProgram://恢复UBoot程序
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumResumeUBootProgram);
                }break;
                case CSystemSystemMaintain::kEnumResumeFactoryConfig://恢复出厂设置
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumResumeFactoryConfig);
                }break;
                case CSystemSystemMaintain::kEnumImportCardParam:
                {

                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumImportCardParam);
                }break;
                case CSystemSystemMaintain::kEnumExportCardParam:
                {

                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumExportCardParam);
                }break;
                case CSystemSystemMaintain::kEnumCleanCardParam:
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumCleanCardParam);
                }break;
                case CSystemSystemMaintain::kEnumCleanCardRecord:
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumCleanCardRecord);
                }break;
                case CSystemSystemMaintain::kEnumFormatStationFile:
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                            CSystemSystemMaintain::kEnumFormatStationFile);
                }break;
            }

            //


            CMyExport::getInstance()->set_content(_tip,tr("准备") + _tip + tr("..."),"",CLtyString::getInstance()->m_cancel);
            CMyExport::getInstance()->exec();
            if(CMyExport::KEnumButtonNotify == CMyExport::getInstance()->get_result())
            {
                QString str_tip = "";
                if(CSystemSystemMaintain::kEnumSuccess == m_return_type)
                {
                    qDebug() << "CSystemMaintainWidget::user_operate_tip() KEnumButtonNotify ok buttton\n";
                    str_tip = _tip + tr("成功!");
                }
                else if(CSystemSystemMaintain::kEnumFail == m_return_type)
                {
                    qDebug() << "CSystemMaintainWidget::user_operate_tip() KEnumButtonNotify cancle buton\n";
                    str_tip = _tip + tr("失败:\n") + m_return_result;
                }

                CMyMessageBox::getInstance()->set_content(tr("温馨提示"),str_tip,CLtyString::getInstance()->m_confirm,"");
                CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
                CMyMessageBox::getInstance()->exec();

               if(CMyExport::KEnumButtonNotify == CMyExport::getInstance()->get_result())
               {
                    if(CSystemSystemMaintain::kEnumSuccess == m_return_type)
                    {
                        qDebug() << "CSystemMaintainWidget::user_operate_tip() KEnumButtonNotify ok(center) button\n";
                        if(CSystemSystemMaintain::kEnumResumeFactoryConfig == _type)
                        {
                            qDebug() << "CSystemMaintainWidget::user_operate_tip() KEnumButtonNotify ok(center) button for kEnumResumeFactoryConfig\n";
                            CProtocol::getInstance()->call_json_system_operate_set(1);
                            MainWindow::getInstance()->slot_pushbutton_shutdown();
                        }
                     }
                }
             }
            else
            {//用户取消
                //通知取消  //kEnumBehaviorStop

                switch(_type)
                {
                    //报站文件
                    case CSystemSystemMaintain::kEnumImportStationFile: //导入报站文件
                    {
                         CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumImportStationFile,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumExportStationFile://导出报站文件
                    {
                         CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumExportStationFile,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumClearStationFile://清除报站文件版本
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumClearStationFile,CProtocol::kEnumBehaviorStop);
                    }break;
                    //日志/记录
                    case CSystemSystemMaintain::kEnumExportVehicleRecord://导出行车记录
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumExportVehicleRecord,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumClearVehicleRecord://清除行车日志
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumClearVehicleRecord,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumExportSystemLog://导出系统日志
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumExportSystemLog,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumClearSystemLog://清除系统日志
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumClearSystemLog,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumExportOperationLog://导出操作日志
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumExportOperationLog,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumClearOperationLog://清除操作日志
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumClearOperationLog,CProtocol::kEnumBehaviorStop);
                    }break;
                    //高级
                    case CSystemSystemMaintain::kEnumResumeApplicationProgram://恢复应用程序
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumResumeApplicationProgram,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumResumeMcuProgram://恢复MCU程序
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumResumeMcuProgram,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumResumeUBootProgram://恢复UBoot程序
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumResumeUBootProgram,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumResumeFactoryConfig://恢复出厂设置
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumResumeFactoryConfig,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumImportCardParam:
                    {

                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumImportCardParam,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumExportCardParam:
                    {

                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumExportCardParam,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumCleanCardParam:
                    {

                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumCleanCardParam,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumCleanCardRecord:
                    {

                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                                CSystemSystemMaintain::kEnumCleanCardRecord,CProtocol::kEnumBehaviorStop);
                    }break;
                    case CSystemSystemMaintain::kEnumFormatStationFile:
                    {
                        CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                        CSystemSystemMaintain::kEnumFormatStationFile,CProtocol::kEnumBehaviorStop);
                    }break;

                }
            }
        }


    }while(0);
}

void CSystemMaintainWidget::user_select_tip(char _type,const QString &_tip,bool _reboot,void *_param)
{

    //CProgressBox::getInstance()->test_init();
    CProgressBox::getInstance()->data_init();
    CProgressBox::getInstance()->set_title(_tip);

    switch(_type)
    {
        case CSystemSystemMaintain::kEnumApplicationProgram: //应用程序
        {
            CSoftwareUpgrade *pCSoftwareUpgrade = (CSoftwareUpgrade *)_param;

            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                    CSystemSystemMaintain::kEnumApplicationProgram,CProtocol::kEnumBehaviorNormal,
                                    QString(UPGRADE_PATH) + "/" + pCSoftwareUpgrade->m_file_name);
        }break;
        case CSystemSystemMaintain::kEnumMcuProgram:  //MCU程序
        {
            CSoftwareUpgrade *pCSoftwareUpgrade = (CSoftwareUpgrade *)_param;
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                    CSystemSystemMaintain::kEnumMcuProgram,CProtocol::kEnumBehaviorNormal,
                                    pCSoftwareUpgrade->m_file_name);
        }break;
        case CSystemSystemMaintain::kEnumUBootProgram: //UBOOT程序
        {
            CSoftwareUpgrade *pCSoftwareUpgrade = (CSoftwareUpgrade *)_param;
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                    CSystemSystemMaintain::kEnumUBootProgram,CProtocol::kEnumBehaviorNormal,
                                    pCSoftwareUpgrade->m_file_name);
        }break;
        //报站文件
        case CSystemSystemMaintain::kEnumImportStationFile://导入报站文件
        {
            CSoftwareUpgrade *pCSoftwareUpgrade = (CSoftwareUpgrade *)_param;
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                    CSystemSystemMaintain::kEnumImportStationFile,CProtocol::kEnumBehaviorNormal,
                                    pCSoftwareUpgrade->m_file_name);
        }break;
        //高级
        case CSystemSystemMaintain::kEnumImportLogoFile://导入开机LOGO文件
        {
            CSoftwareUpgrade *pCSoftwareUpgrade = (CSoftwareUpgrade *)_param;
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                    CSystemSystemMaintain::kEnumImportLogoFile,CProtocol::kEnumBehaviorNormal,
                                    pCSoftwareUpgrade->m_file_name);
        }break;
        case CSystemSystemMaintain::kEnumExportSystemLog://导出系统日志
        {
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                        CSystemSystemMaintain::kEnumExportSystemLog);
        }break;
        case CSystemSystemMaintain::kEnumImportPicture://导入广告
        {
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                            CSystemSystemMaintain::kEnumImportPicture);
        }break;
        case CSystemSystemMaintain::kEnumExportPicture://导出广告
        {
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                            CSystemSystemMaintain::kEnumExportPicture);
        }break;
        case CSystemSystemMaintain::kEnumCleanPicture://清除广告
        {
            CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                        CSystemSystemMaintain::kEnumCleanPicture);
        }break;
    }


    CProgressBox::getInstance()->exec();

    if(CProgressBox::KEnumStatusSuccess == CProgressBox::getInstance()->get_status())
    {
        QString str_message = "";

        if(_reboot)
            str_message = tr("成功，是否\n立即重启设备。");
        else
            str_message = tr("成功!");

        CMyMessageBox::getInstance()->set_content(_tip, _tip + str_message,CLtyString::getInstance()->m_confirm,tr(""));
        CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
        CMyMessageBox::getInstance()->exec();
        if(_reboot)
        {
            //CProtocol::getInstance()->call_json_system_operate_set(1);
            //MainWindow::getInstance()->slot_pushbutton_shutdown();
        }

    }
    else
    {
        if(CProgressBox::KEnumStatusStop == CProgressBox::getInstance()->get_status())
        {
            //用户停止
            switch(_type)
            {
                case CSystemSystemMaintain::kEnumApplicationProgram: //应用程序
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                            CSystemSystemMaintain::kEnumApplicationProgram,CProtocol::kEnumBehaviorStop);
                }break;
                case CSystemSystemMaintain::kEnumMcuProgram:  //MCU程序
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                            CSystemSystemMaintain::kEnumMcuProgram,CProtocol::kEnumBehaviorStop);
                }break;
                case CSystemSystemMaintain::kEnumUBootProgram: //UBOOT程序
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                            CSystemSystemMaintain::kEnumUBootProgram,CProtocol::kEnumBehaviorStop);
                }break;
                //报站文件
                case CSystemSystemMaintain::kEnumImportStationFile://导入报站文件
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                            CSystemSystemMaintain::kEnumImportStationFile,CProtocol::kEnumBehaviorStop);
                }break;
                //高级
                case CSystemSystemMaintain::kEnumImportLogoFile://导入开机LOGO文件
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                            CSystemSystemMaintain::kEnumImportLogoFile,CProtocol::kEnumBehaviorStop);
                }break;
                case CSystemSystemMaintain::kEnumImportPicture://导入广告
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                    CSystemSystemMaintain::kEnumImportPicture,CProtocol::kEnumBehaviorStop);
                }break;
                case CSystemSystemMaintain::kEnumExportPicture://导出广告
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                    CSystemSystemMaintain::kEnumExportPicture,CProtocol::kEnumBehaviorStop);
                }break;
                case CSystemSystemMaintain::kEnumCleanPicture://清除广告
                {
                    CProtocol::getInstance()->call_json_system_maintain_set(CProtocol::kEnumSystemMaintainOperate,
                                                CSystemSystemMaintain::kEnumCleanPicture,CProtocol::kEnumBehaviorStop);
                }break;

            }
            return;
        }



        CMyMessageBox::getInstance()->set_content(_tip + tr("提示"), _tip + tr("失败:\n") + CCommonData::getInstance()->m_CSystemSystemMaintain.m_result,CLtyString::getInstance()->m_confirm,tr(""));
        CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
        CMyMessageBox::getInstance()->exec();
    }
}

void CSystemMaintainWidget::slot_system_maintain_widget_event(char _type,const CSystemSystemMaintain &_info)
{
    //qDebug() << "CSystemMaintainWidget::slot_system_maintain_widget_event start";
    switch(_type)
    {
        //系统升级
        case CSystemSystemMaintain::kEnumApplicationProgram: //应用程序
        case CSystemSystemMaintain::kEnumMcuProgram:  //MCU程序
        case CSystemSystemMaintain::kEnumUBootProgram: //UBOOT程序
        case CSystemSystemMaintain::kEnumExportSystemLog://导出系统日志
        //高级
        case CSystemSystemMaintain::kEnumImportLogoFile://导入开机LOGO文件
        case CSystemSystemMaintain::kEnumImportPicture: //导入广告
        case CSystemSystemMaintain::kEnumExportPicture:  //导出广告
        case CSystemSystemMaintain::kEnumCleanPicture:   //清除广告
        {
            switch(_info.m_operate_type)
            {
                case CSystemSystemMaintain::kEnumGetData://获取数据
                {
                    #if 0
                    if(CSystemSystemMaintain::kEnumApplicationProgram == _type && m_application_list)
                        m_application_list->add_data((CSoftwareUpgrade*)_info.m_param);
                    else if(CSystemSystemMaintain::kEnumMcuProgram == _type && m_mcu_list)
                        m_mcu_list->add_data((CSoftwareUpgrade*)_info.m_param);
                    else if(CSystemSystemMaintain::kEnumUBootProgram == _type && m_uboot_list)
                        m_uboot_list->add_data((CSoftwareUpgrade*)_info.m_param);
                    else if(CSystemSystemMaintain::kEnumImportStationFile == _type && m_import_station_list)
                        m_import_station_list->add_data((CSoftwareUpgrade*)_info.m_param);
                    else if(CSystemSystemMaintain::kEnumImportLogoFile == _type && m_import_logo_list)
                        m_import_logo_list->add_data((CSoftwareUpgrade*)_info.m_param);
                    #endif
                }break;
                case CSystemSystemMaintain::kEnumGetProgress://获取进度
                {
                     //qDebug() << "CSystemMaintainWidget::slot_system_maintain_widget_event 1";
                    CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNormal,_info.percent);

                }break;
                case CSystemSystemMaintain::kEnumSuccess:    //操作成功
                {
                    CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNormal,100);
                }break;
                case CSystemSystemMaintain::kEnumFail:        //操作失败
                {
                    CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessFail,0);
                }break;
            }

        }break;
        //报站文件
        case CSystemSystemMaintain::kEnumImportStationFile://导入报站文件
        case CSystemSystemMaintain::kEnumExportStationFile://导出报站文件
        case CSystemSystemMaintain::kEnumClearStationFile://清除报站文件版本
        //日志/记录
        case CSystemSystemMaintain::kEnumExportVehicleRecord://导出行车记录
        case CSystemSystemMaintain::kEnumClearVehicleRecord://清除行车日志
        case CSystemSystemMaintain::kEnumClearSystemLog://清除系统日志
        case CSystemSystemMaintain::kEnumExportOperationLog://导出操作日志
        case CSystemSystemMaintain::kEnumClearOperationLog://清除操作日志
        //高级
        case CSystemSystemMaintain::kEnumResumeApplicationProgram://恢复应用程序
        case CSystemSystemMaintain::kEnumResumeMcuProgram://恢复MCU程序
        case CSystemSystemMaintain::kEnumResumeUBootProgram://恢复UBoot程序
        case CSystemSystemMaintain::kEnumResumeFactoryConfig://恢复出厂设置
        case CSystemSystemMaintain::kEnumImportCardParam:
        case CSystemSystemMaintain::kEnumExportCardParam:
        case CSystemSystemMaintain::kEnumCleanCardParam:
        case CSystemSystemMaintain::kEnumCleanCardRecord:
        case CSystemSystemMaintain::kEnumFormatStationFile:
        {
            switch(_info.m_operate_type)
            {
                case CSystemSystemMaintain::kEnumOperate:////正在操作
                {
                    if(_type == CSystemSystemMaintain::kEnumExportStationFile)
                        CMyExport::getInstance()->set_content(tr("导出报站文件"),tr("正在导出报站文件..."),"",CLtyString::getInstance()->m_cancel);
                    else if(_type == CSystemSystemMaintain::kEnumClearStationFile)
                        CMyExport::getInstance()->set_content(tr("清除报站文件"),tr("正在清除报站文件..."),"",CLtyString::getInstance()->m_cancel);
                    else if(_type == CSystemSystemMaintain::kEnumExportVehicleRecord)
                        CMyExport::getInstance()->set_content(tr("导出行车记录"),tr("正在导出行车记录..."),"",CLtyString::getInstance()->m_cancel);
                    else if(_type == CSystemSystemMaintain::kEnumClearVehicleRecord)
                        CMyExport::getInstance()->set_content(tr("清除行车日志"),tr("正在清除行车日志..."),"",CLtyString::getInstance()->m_cancel);
                    else if(_type == CSystemSystemMaintain::kEnumExportSystemLog)
                        CMyExport::getInstance()->set_content(tr("导出系统日志"),tr("正在导出系统日志..."),"",CLtyString::getInstance()->m_cancel);
                    else if(_type == CSystemSystemMaintain::kEnumClearSystemLog)
                        CMyExport::getInstance()->set_content(tr("清除系统日志"),tr("正在清除系统日志..."),"",CLtyString::getInstance()->m_cancel);
                    else if(_type == CSystemSystemMaintain::kEnumExportOperationLog)
                        CMyExport::getInstance()->set_content(tr("导出操作日志"),tr("正在导出操作日志..."),"",CLtyString::getInstance()->m_cancel);
                    else if(_type == CSystemSystemMaintain::kEnumClearOperationLog)
                        CMyExport::getInstance()->set_content(tr("清除操作日志"),tr("正在清除操作日志..."),"",CLtyString::getInstance()->m_cancel);
                    else if(_type == CSystemSystemMaintain::kEnumResumeApplicationProgram)
                        CMyExport::getInstance()->set_content(tr("恢复应用程序"),tr("正在恢复应用程序..."),"",CLtyString::getInstance()->m_cancel);
                    else if(_type == CSystemSystemMaintain::kEnumResumeMcuProgram)
                        CMyExport::getInstance()->set_content(tr("恢复MCU程序"),tr("正在恢复MCU程序..."),"",CLtyString::getInstance()->m_cancel);
                    else if(_type == CSystemSystemMaintain::kEnumResumeMcuProgram)
                        CMyExport::getInstance()->set_content(tr("恢复UBoot程序"),tr("正在恢复UBoot程序..."),"",CLtyString::getInstance()->m_cancel);
                    else if(_type == CSystemSystemMaintain::kEnumResumeFactoryConfig)
                        CMyExport::getInstance()->set_content(tr("恢复出厂设置"),tr("正在恢复出厂设置..."),"",CLtyString::getInstance()->m_cancel);
                }break;
                case CSystemSystemMaintain::kEnumSuccess:    //操作成功
                {
                    m_return_type = CSystemSystemMaintain::kEnumSuccess;
                    m_return_result = _info.m_result;
                    CMyExport::getInstance()->nofity_close();
                }break;
                case CSystemSystemMaintain::kEnumFail:        //操作失败
                {
                    m_return_type = CSystemSystemMaintain::kEnumFail;
                    m_return_result = _info.m_result;
                    CMyExport::getInstance()->nofity_close();
                }break;
            }
        }break;
        //客流配置
        case CSystemSystemMaintain::kEnumImportPassengerFlowFile://导入客流配置文件
        {
        }break;
        case CSystemSystemMaintain::kEnumParameterConfig://参数配置
        {
        }break;

    }
    //qDebug() << "CSystemMaintainWidget::slot_system_maintain_widget_event end";
}









