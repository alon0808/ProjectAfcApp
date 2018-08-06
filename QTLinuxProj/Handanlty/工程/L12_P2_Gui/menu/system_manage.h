#ifndef SYSTEM_MANAGE_H
#define SYSTEM_MANAGE_H

#include <QDialog>
#include <QLabel>

#include "head_widget.h"
#include "navigate_left.h"
#include "./class/common/navigateleftbutton.h"
#include <QStackedWidget>


#include "system_manage/register_info_widget.h"
#include "system_manage/coordinate_collection_widget.h"
#include "system_manage/videotape_query_widget.h"
#include "system_manage/camera_config_widget.h"
#include "system_manage/passenger_flow_widget.h"
#include "system_manage/storage_manage_widget.h"
#include "system_manage/network_widget.h"
#include "system_manage/switch_config_widget.h"

#include "system_manage/senior_setup_widget.h"
#include "system_manage/pos_config_widget.h"
#include "system_manage/system_setup_widget.h"
#include "system_manage/system_maintain_widget.h"
#include "system_manage/history_info_widget.h"

#include "class/common/define_screen.h"

#include "class/common/parent_widget.h"

class CSystemManage : public CParentWidget
{
    Q_OBJECT
public:
    explicit CSystemManage(QWidget *parent = 0);
    
    enum
    {
        kEnumCurrentLeft,
        kEnumCurrentRight
    };

signals:
    void signal_set_default_id(int);
    void signal_videotape_query_user_return();
    void signal_network_user_return();
    void signal_senior_setup_user_return();
    void signal_switch_config_user_return();
    void signal_camera_config_user_return();
    void signal_passenger_flow_user_return();
    void signal_passenger_flow_user_home();
    void signal_pos_config_user_return();
    void signal_pushbutton_home();
    void signal_pushbutton_menu();

public slots:
    void slot_pushbutton_home();
    void slot_pushbutton_back();
    void slot_pushbutton_close();
    void slot_button_pull();
    void slot_system_manager_event(int _id,QString _text);
    void slot_timer_init();

    void slot_pushbutton_up();
    void slot_pushbutton_down();

public:
    void system_manage_init();
    bool goto_main_page() {return m_goto_main_page;}
    void request_data();
private:
    void resizeEvent(QResizeEvent *e);
    bool keyboard_value(int _value);
    void navigate_right_init();
    void navigate_right_key(int _value,bool _first = false);

    void goto_register_info_widget();   //注册信息
    void goto_coordinate_collection();  //坐标采集
    void goto_videotape_query();        //录像查询
    void goto_camera_config();          //摄像机配置
    void goto_passenger_flow();         //客流统计
    void goto_storage_manage();         //存储管理
    void goto_network();                //网络
    void goto_switch_config();          //开关量配置
    void goto_senior_setup();           //高级设置
    void goto_pos_config();             //POS配置
    void goto_system_setup();           //系统设置
    void goto_system_maintain();        //系统维护
    void goto_history_info();           //历史信息

    void check_change();

    CHeadWidget *m_CHeadWidget;
    QWidget *m_widget_bg1;
    QWidget *m_widget_bg2;
    QLabel *m_label_system_manage;


    CNavigateLeft *m_CNavigateLeft;
    NavigateLeftButton *m_button_pull;
    QStackedWidget *m_stackwidget_navigate_right;

    CRegisterInfoWidget *m_CRegisterInfoWidget;                 //注册信息
    CCoordinateCollectionWidget *m_CCoordinateCollectionWidget; //坐标采集
    CVideotapeQueryWidget *m_CVideotapeQueryWidget;             //录像查询
    CCameraConfigWidget *m_CCameraConfigWidget;                 //摄像机配置
    CPassengerFlowWidget *m_CPassengerFlowWidget;               //客流统计
    CStorageManageWidget *m_CStorageManageWidget;               //存储管理
    CNetworkWidget *m_CNetworkWidget;                           //网络
    CSwitchConfigWidget *m_CSwitchConfigWidget;                 //开关量配置
    SeniorSetupWidget *m_SeniorSetupWidget;                     //高级设置
    CSystemMaintainWidget *m_CSystemMaintainWidget;             //系统维护
    CPosConfigWidget *m_CPosConfigWidget;                       //POS配置
    CSystemSetupWidget *m_CSystemSetupWidget;                   //系统设置
    CHistoryInfoWidget *m_CHistoryInfoWidget;                   //历史信息

    QPushButton *m_pushbutton_up;
    QPushButton *m_pushbutton_down;

    bool m_goto_main_page;
    int m_current_select_id;

    CMyDefineScreen *m_define_screen;
    char m_current_left_right;
};

#endif // SYSTEM_MANAGE_H






