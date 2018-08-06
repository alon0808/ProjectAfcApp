#ifndef SYSTEM_MAINTAIN_WIDGET_H
#define SYSTEM_MAINTAIN_WIDGET_H

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>

#include "class/common/scrollcontroller.h"
#include "class/common/navigateleftbutton.h"
#include "protocol.h"

#include "class/common/define_screen.h"

class SystemMaintainUpgrade;

class CSystemMaintainInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CSystemMaintainInfo(char _type,QWidget *parent = 0);
    enum
    {
        kEnumSystemUpgrade,         //系统升级
        kEnumStationFile,           //报站文件
        kEnumPassengerFlowConfig,   //客流配置
        kEnumLogRecord,             //日志/记录
        kEnumAdvert,                //广告
        kEnumCardRecord,            //刷卡记录
        kEnumSenior          //高级
    };
signals:
    //系统升级
    void signal_application_program();//应用程序
    void signal_mcu_program();//MCU程序
    void signal_uboot_program();//UBoot程序
    //报站文件
    void signal_import_station_file();//导入报站文件
    void signal_export_station_file();//导出报站文件
    void signal_clean_station_file_version();//清除报站文件版本
    void signal_format_station_file();   //格式化报站分区
    //客流配置
    void signal_import_passenger_flow_config_file();//导入客流配置文件
    void signal_parameter_config();//参数配置
    //日志/记录
    void signal_export_vehicle_record(); //导出行车记录
    void signal_clean_vehicle_log();//清除行车日志
    void signal_export_system_log();//导出系统日志
    void signal_clean_system_log();//清除系统日志
    void signal_export_operator_log();//导出操作日志
    void signal_clean_operator_logd();//清除操作日志
    //高级
    void signal_import_logo_file();//导入开机LOGO文件
    void signal_recovery_application_program();//恢复应用程序
    void signal_recovery_mcu_program();//恢复MCU程序
    void signal_recovery_uboot_program();//恢复UBoot程序
    void signal_recovery_factory_setting();//恢复出厂设置
    void signal_import_picture();  //导入广告图片
    void signal_export_picture();  //导出广告图片
	void signal_clean_picture();  //清除广告图片
    void signal_import_card_param();  //导入刷卡参数
    void signal_export_card_param();  //导出刷卡参数
    void signal_clean_card_param();   //清除刷卡参数
    void signal_clean_card_record();  //清除刷卡记录

    void signal_reboot_device();    //重启设备

public slots:


private:
    void resizeEvent(QResizeEvent *e);
    void system_upgrade_init();         //系统升级
    void station_file_init();           //报站文件
    void passenger_flow_config_init();   //客流配置
    void log_record_init();             //日志/记录
    void advert_init();                 //广告
    void card_record_init();            //刷卡记录
    void senior_init();           //高级

    void set_type(char _type);
    char m_type;

    NavigateLeftButton *m_button_line1;
    NavigateLeftButton *m_button_line2;
    NavigateLeftButton *m_button_line3;
    NavigateLeftButton *m_button_line4;
    NavigateLeftButton *m_button_line5;
    NavigateLeftButton *m_button_line6;
    NavigateLeftButton *m_button_line7;
    NavigateLeftButton *m_button_line8;
    NavigateLeftButton *m_button_line9;
    NavigateLeftButton *m_button_line10;
    NavigateLeftButton *m_button_line11;  //重启设备

    CMyDefineScreen *m_define_screen;

};


class CSystemMaintainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSystemMaintainWidget(QWidget *parent = 0);

signals:

public slots:
    void slot_button_1();
    void slot_button_2();
    void slot_button_3();
    void slot_button_4();
    void slot_button_4_1();
    void slot_button_4_2();
    void slot_button_5();

       //系统升级
    void slot_application_program();//应用程序
    void slot_mcu_program();//MCU程序
    void slot_uboot_program();//UBoot程序
    //报站文件
    void slot_import_station_file();//导入报站文件
    void slot_export_station_file();//导出报站文件
    void slot_clean_station_file_version();//清除报站文件版本
    void slot_format_station_file();   //格式化报站分区
    //客流配置
    void slot_import_passenger_flow_config_file();//导入客流配置文件
    void slot_parameter_config();//参数配置
    //日志/记录
    void slot_export_vehicle_record(); //导出行车记录
    void slot_clean_vehicle_log();//清除行车日志
    void slot_export_system_log();//导出系统日志
    void slot_clean_system_log();//清除系统日志
    void slot_export_operator_log();//导出操作日志
    void slot_clean_operator_logd();//清除操作日志
    //高级
    void slot_import_logo_file();//导入开机LOGO文件
    void slot_recovery_application_program();//恢复应用程序
    void slot_recovery_mcu_program();//恢复MCU程序
    void slot_recovery_uboot_program();//恢复UBoot程序
    void slot_recovery_factory_setting();//恢复出厂设置
    void slot_import_picture();  //导入广告图片
    void slot_export_picture();  //导出广告图片
	void slot_clean_picture();  //清除广告图片
    void slot_import_card_param();  //导入刷卡参数
    void slot_export_card_param();  //导出刷卡参数
    void slot_clean_card_param();   //清除刷卡参数
    void slot_clean_card_record();  //清除刷卡记录
    void slot_reboot_device();  //重启设备

    void slot_system_maintain_widget_event(char _type,const CSystemSystemMaintain &_info);
private:
    void resizeEvent(QResizeEvent *e);
    void system_maintain_init();
    void system_maintain_init_new();
    void user_operate_tip(char _type,const QString &_tip);
    void user_select_tip(char _type,const QString &_tip,bool _reboot = true,void *_param = NULL);

//////////////////////////////////////////////////////////////////////////new start
    void scroll_layout();

    QWidget *m_widget_bg;
    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    NavigateLeftButton *m_button_1;
    NavigateLeftButton *m_button_2;
    NavigateLeftButton *m_button_3;
    NavigateLeftButton *m_button_4;
    NavigateLeftButton *m_button_4_1;
    NavigateLeftButton *m_button_4_2;
    NavigateLeftButton *m_button_5;

    CSystemMaintainInfo *m_SystemMaintainInfo1;
    CSystemMaintainInfo *m_SystemMaintainInfo2;
    CSystemMaintainInfo *m_SystemMaintainInfo3;
    CSystemMaintainInfo *m_SystemMaintainInfo4;
    CSystemMaintainInfo *m_SystemMaintainInfo4_1;
    CSystemMaintainInfo *m_SystemMaintainInfo4_2;
    CSystemMaintainInfo *m_SystemMaintainInfo5;

    QLabel *m_label_space1;
    QLabel *m_label_space2;
    QLabel *m_label_space3;
    QLabel *m_label_space4;
    QLabel *m_label_space4_1;
    QLabel *m_label_space4_2;

    bool m_button_press_1;
    bool m_button_press_2;
    bool m_button_press_3;
    bool m_button_press_4;
    bool m_button_press_4_1;
    bool m_button_press_4_2;
    bool m_button_press_5;

    QVBoxLayout *m_vboxlayout_network;
    QWidget *m_widget_main;

    SystemMaintainUpgrade *m_application_list;//应用程序
    SystemMaintainUpgrade *m_mcu_list;//MCU程序
    SystemMaintainUpgrade *m_uboot_list;//UBoot程序
    SystemMaintainUpgrade *m_import_station_list;//导入报站文件
    SystemMaintainUpgrade *m_import_logo_list;//导入开机LOGO文件

    char m_return_type;
    QString m_return_result;
//////////////////////////////////////////////////////////////////////////new end

    CMyDefineScreen *m_define_screen;

};

#endif // SYSTEM_MAINTAIN_WIDGET_H
