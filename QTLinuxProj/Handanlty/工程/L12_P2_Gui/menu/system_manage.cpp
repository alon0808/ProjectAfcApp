#include "system_manage.h"


#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"

//#include "./include/syszuxim.h"
//#include "./include/syszuxpinyin.h"
#include <QDebug>

#include "class/common_interface.h"

#include "mainwindow.h"

CSystemManage::CSystemManage(QWidget *parent) :
    CParentWidget(parent)
{
    qDebug() << "CSystemManage() start";
    this->setFixedSize(1024,768);
    this->setObjectName("CSystemManage");
    this->setStyleSheet("QDialog#CSystemManage{background:transparent;}");//444a59 //ffffff
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);


    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetSystemManage,this);
    connect(m_CHeadWidget,SIGNAL(signal_button_back()),this,SLOT(slot_pushbutton_back()));
    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home()));

    QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(250,768 - 82);
    pWidget->move(0,82);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");


    m_widget_bg2 = new QWidget(this);
    m_widget_bg2->setFixedSize(190,636);
    m_widget_bg2->move(40,107);
    m_widget_bg2->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/menu/system_manage/nav_left_background.png);}");



    m_pushbutton_up = new QPushButton(this);
    QString str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/nav_left_button_up_default.png").arg(":/img/res/main_page/menu/system_manage/nav_left_button_up_selected.png").arg(":/img/res/main_page/menu/system_manage/nav_left_button_up_selected.png");

    CCommonInterface::init_new_button(m_pushbutton_up,106,107,58,58,str_sheet);


    m_pushbutton_down = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/nav_left_button_down_default.png").arg(":/img/res/main_page/menu/system_manage/nav_left_button_down_selected.png").arg(":/img/res/main_page/menu/system_manage/nav_left_button_down_selected.png");

    CCommonInterface::init_new_button(m_pushbutton_down,106,666,58,58,str_sheet);



    int navigate_width = 230;
    int navigate_height = 666 - 106 - 58 - 10 - 18 + 7;
    int button_height = 78 + 18;

    m_CNavigateLeft = new CNavigateLeft(navigate_width,button_height,navigate_height,this);
    m_CNavigateLeft->move(20,106 + 58 + 10);
    m_CNavigateLeft->setFixedSize(navigate_width,navigate_height);







#if 0
    m_button_pull = new NavigateLeftButton(NavigateLeftButton::PullMode,this);
    m_button_pull->setDisplayText(tr("加载更多"));

    m_button_pull->setNormalIconStr(":/img/res/systemManage/icon_up.png");
    m_button_pull->setPressedIconStr(":/img/res/systemManage/icon_up.png");
    m_button_pull->setActiveIconStr(":/img/res/systemManage/icon_up.png");
    m_button_pull->move(0,64 + 76 + 9 * 64);
    connect(m_button_pull,SIGNAL(clicked()),this,SLOT(slot_button_pull()));
#endif
    connect(m_CNavigateLeft,SIGNAL(signalCurrentClassificationChanged(int,QString)),this,SLOT(slot_system_manager_event(int,QString)));

    connect(this,SIGNAL(signal_set_default_id(int)),m_CNavigateLeft,SLOT(slotSetDefaultSelected(int)));


    navigate_right_init();




    connect(m_pushbutton_up,SIGNAL(clicked()),this,SLOT(slot_pushbutton_up()));
    connect(m_pushbutton_down,SIGNAL(clicked()),this,SLOT(slot_pushbutton_down()));

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);

    m_CNavigateLeft->navigate_left_init();
    emit signal_set_default_id(CNavigateLeft::kEnumRegisterInfo);

    m_current_left_right = kEnumCurrentLeft;
}

void CSystemManage::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CSystemManage::system_manage_init()
{
    m_goto_main_page = false;
}

void CSystemManage::navigate_right_init()
{
    m_stackwidget_navigate_right = new QStackedWidget(this);
    //m_plMain->setGeometry(QRect(NAVIGATElEFT_WIDTH, 0, MAIN_WINDOW_WIDTH - NAVIGATElEFT_WIDTH, MAIN_WINDOW_HEIGHT - MAIN_WINDOW_TOP_HEIGHT));
    m_stackwidget_navigate_right->move(MAIN_NAVIGATE_LEFT,MAIN_HEAD_HEIGHT);
    m_stackwidget_navigate_right->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_stackwidget_navigate_right->setObjectName(QString::fromUtf8("m_stackwidget_navigate_right"));
    m_stackwidget_navigate_right->setContentsMargins(0, 0, 0, 0);

    m_CRegisterInfoWidget = NULL;           //注册信息
    m_CCoordinateCollectionWidget = NULL;   //坐标采集
    m_CVideotapeQueryWidget = NULL;         //录像查询
    m_CCameraConfigWidget = NULL;           //摄像机配置
    m_CPassengerFlowWidget = NULL;          //客流统计
    m_CStorageManageWidget = NULL;          //存储管理
    m_CNetworkWidget = NULL;                //网络
    m_CSwitchConfigWidget = NULL;           //开关量配置
    m_SeniorSetupWidget = NULL;             //高级设置
    m_CSystemMaintainWidget = NULL;         //系统维护
    m_CPosConfigWidget = NULL;
    m_CSystemSetupWidget = NULL;            //系统设置
    m_CHistoryInfoWidget = NULL;            //历史信息


    m_current_select_id = 0;

    //QTimer::singleShot(200,this,SLOT(slot_timer_init()));
}

void CSystemManage::slot_timer_init()
{
    m_CNavigateLeft->set_scroll_value(true);
    //  slot_button_pull();
}

void CSystemManage::slot_pushbutton_home()
{
    if(CNavigateLeft::kEnumPassengerFlow == m_current_select_id)
    {
        emit signal_passenger_flow_user_home();
    }


    check_change();
    CCommonInterface::input_panel_control(false);
    m_goto_main_page = true;
    emit signal_pushbutton_home();
}

void CSystemManage::slot_pushbutton_back()
{
    if(CNavigateLeft::kEnumVideotapeQuery == m_current_select_id)
    {
         emit signal_videotape_query_user_return();
    }
    else if(CNavigateLeft::kEnumSeniorSetUp == m_current_select_id)
    {
        emit signal_senior_setup_user_return();
    }
    else if(CNavigateLeft::kEnumNetwork == m_current_select_id)
    {
        emit signal_network_user_return();
    }
    else if(CNavigateLeft::kEnumSwitchConfig == m_current_select_id)
    {
        emit signal_switch_config_user_return();
    }
    else if(CNavigateLeft::kEnumCameraConfig == m_current_select_id)
    {
        emit signal_camera_config_user_return();
    }
    else if(CNavigateLeft::kEnumPassengerFlow == m_current_select_id)
    {
        emit signal_passenger_flow_user_return();
    }
    else if(CNavigateLeft::kEnumPosConfig == m_current_select_id)
    {
        emit signal_pos_config_user_return();
    }
    else
    {
        check_change();
        CCommonInterface::input_panel_control(false);
        m_goto_main_page = false;
        emit signal_pushbutton_menu();
    }

}

void CSystemManage::slot_pushbutton_close()
{
    check_change();
    CCommonInterface::input_panel_control(false);
    m_goto_main_page = false;
    emit signal_pushbutton_menu();
}


void CSystemManage::slot_button_pull()
{
    static bool test_on = true;

    if(test_on)
    {

        test_on = false;
    }
    else
    {

        test_on = true;
    }
}

void CSystemManage::slot_pushbutton_up()
{
    m_CNavigateLeft->set_scroll_value(false);
}
void CSystemManage::slot_pushbutton_down()
{
    m_CNavigateLeft->set_scroll_value(true);
}

void CSystemManage::slot_system_manager_event(int _id,QString _text)
{
    m_CHeadWidget->set_head_widget_title(_text);

    check_change();

    switch(_id)
    {
        case CNavigateLeft::kEnumRegisterInfo:          //注册信息
        {
            goto_register_info_widget();
        }break;
        case CNavigateLeft::kEnumCoordinateCollection:  //坐标采集
        {
            goto_coordinate_collection();
        }break;
        case CNavigateLeft::kEnumVideotapeQuery:        //录像查询
        {
            goto_videotape_query();
        }break;
        case CNavigateLeft::kEnumCameraConfig:          //摄像机配置
        {
            goto_camera_config();
        }break;
        case CNavigateLeft::kEnumPassengerFlow:         //客流统计
        {
            goto_passenger_flow();
        }break;
        case CNavigateLeft::kEnumStorageManage:         //存储管理
        {
            goto_storage_manage();
        }break;
        case CNavigateLeft::kEnumNetwork:               //网络
        {
            goto_network();
        }break;
        case CNavigateLeft::kEnumSwitchConfig:          //开关量配置
        {
            goto_switch_config();
        }break;
        case CNavigateLeft::kEnumSeniorSetUp:           //高级设置
        {
            goto_senior_setup();
        }break;
        case CNavigateLeft::kEnumPosConfig:           //POS配置
        {
            goto_pos_config();
        }break;
        case CNavigateLeft::kEnumSystemSetUp:           //系统设置
        {
            goto_system_setup();
        }break;
        case CNavigateLeft::kEnumSystemMaintain:        //系统维护
        {
            goto_system_maintain();
        }break;
        case CNavigateLeft::kEnumHistoryInfo:            //历史信息
        {
            goto_history_info();
        }break;
    }
    m_current_select_id = _id;
    request_data();
}

void CSystemManage::check_change()
{
    switch(m_current_select_id)
    {
        case CNavigateLeft::kEnumRegisterInfo:          //注册信息
        {
            if(m_CRegisterInfoWidget)
                m_CRegisterInfoWidget->sava_data();
        }break;
        case CNavigateLeft::kEnumCoordinateCollection:  //坐标采集
        {
            if(m_CCoordinateCollectionWidget)
                m_CCoordinateCollectionWidget->sava_data();
        }break;
        case CNavigateLeft::kEnumVideotapeQuery:        //录像查询
        {
            //if(m_CVideotapeQueryWidget)
            //    m_CVideotapeQueryWidget->sa
        }break;
        case CNavigateLeft::kEnumCameraConfig:          //摄像机配置
        {
            if(m_CCameraConfigWidget)
                m_CCameraConfigWidget->save_data();
        }break;
        case CNavigateLeft::kEnumPassengerFlow:         //客流统计
        {
            if(m_CPassengerFlowWidget)
                m_CPassengerFlowWidget->save_data();
        }break;
        case CNavigateLeft::kEnumStorageManage:         //存储管理
        {

        }break;
        case CNavigateLeft::kEnumNetwork:               //网络
        {
            if(m_CNetworkWidget)
                m_CNetworkWidget->save_data();
        }break;
        case CNavigateLeft::kEnumSwitchConfig:          //开关量配置
        {
            if(m_CSwitchConfigWidget)
                m_CSwitchConfigWidget->save_data();
        }break;
        case CNavigateLeft::kEnumSeniorSetUp:           //高级设置
        {
            if(m_SeniorSetupWidget)
                m_SeniorSetupWidget->save_data();
        }break;
        case CNavigateLeft::kEnumPosConfig:           //POS配置
        {
            if(m_CPosConfigWidget)
                m_CPosConfigWidget->save_data();
        }break;
        case CNavigateLeft::kEnumSystemSetUp:           //系统设置
        {
            if(m_CSystemSetupWidget)
                m_CSystemSetupWidget->save_data();
        }break;
        case CNavigateLeft::kEnumSystemMaintain:        //系统维护
        {

        }break;
        case CNavigateLeft::kEnumHistoryInfo:            //历史信息
        {

        }break;
    }
}


#include "main_page.h"
void CSystemManage::goto_register_info_widget()//注册信息
{
    if(NULL == m_CRegisterInfoWidget)
    {
        m_CRegisterInfoWidget = new CRegisterInfoWidget(this);
        connect(m_CRegisterInfoWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));

        qRegisterMetaType<CSystemRegisterInfo>("CSystemRegisterInfo");
        connect(CMainPage::getInstance(),SIGNAL(signal_register_info_widget_event(CSystemRegisterInfo)),m_CRegisterInfoWidget,SLOT(slot_register_info_widget_event(CSystemRegisterInfo)));
        m_stackwidget_navigate_right->addWidget(m_CRegisterInfoWidget);

    }

    m_stackwidget_navigate_right->setCurrentWidget(m_CRegisterInfoWidget);
}

void CSystemManage::goto_coordinate_collection()//坐标采集
{
    if(NULL == m_CCoordinateCollectionWidget)
    {
        m_CCoordinateCollectionWidget = new CCoordinateCollectionWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CCoordinateCollectionWidget);

        qRegisterMetaType<CSystemCoordinateCollection>("CSystemCoordinateCollection");
        connect(CMainPage::getInstance(),SIGNAL(signal_coordinate_collection_widget_event(char,CSystemCoordinateCollection)),m_CCoordinateCollectionWidget,SLOT(slot_coordinate_collection_widget_event(char,CSystemCoordinateCollection)));
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CCoordinateCollectionWidget);
}
void CSystemManage::goto_videotape_query()//录像查询
{
    //bool ret = false;
    if(NULL == m_CVideotapeQueryWidget)
    {
        m_CVideotapeQueryWidget = new CVideotapeQueryWidget(CVideotapeQueryWidget::kEnumSystemManage,this);
        m_stackwidget_navigate_right->addWidget(m_CVideotapeQueryWidget);
        connect(m_CVideotapeQueryWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_videotape_query_user_return()),m_CVideotapeQueryWidget,SLOT(slot_pushbutton_return()));

        connect(m_CVideotapeQueryWidget,SIGNAL(signal_pushbutton_playback()),MainWindow::getInstance(),SLOT(slot_pushbutton_playback()));

        qRegisterMetaType<CSystemVideotapeQueryEx>("CSystemVideotapeQueryEx");
        connect(CMainPage::getInstance(),SIGNAL(signal_videotape_query_widget_event(CSystemVideotapeQueryEx)),m_CVideotapeQueryWidget,SLOT(slot_videotape_query_widget_event(CSystemVideotapeQueryEx)));
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CVideotapeQueryWidget);
}
void CSystemManage::goto_camera_config()//摄像机配置
{
    if(NULL == m_CCameraConfigWidget)
    {
        m_CCameraConfigWidget = new CCameraConfigWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CCameraConfigWidget);
        connect(m_CCameraConfigWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_camera_config_user_return()),m_CCameraConfigWidget,SLOT(slot_pushbutton_return()));

        qRegisterMetaType<CSystemCameraConfig>("CSystemCameraConfig");
        connect(CMainPage::getInstance(),SIGNAL(signal_camera_config_widget_event(CSystemCameraConfig)),m_CCameraConfigWidget,SLOT(slot_camera_config_widget_event(CSystemCameraConfig)));

        //void signal_camera_config_widget_notify(char _type,char _cam_type,char _number);
        connect(CMainPage::getInstance(),SIGNAL(signal_camera_config_widget_notify(char,char,char)),m_CCameraConfigWidget,SLOT(slot_camera_config_widget_notify(char,char,char)));

    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CCameraConfigWidget);
}
void CSystemManage::goto_passenger_flow()//客流统计
{
    if(NULL == m_CPassengerFlowWidget)
    {
        m_CPassengerFlowWidget = new CPassengerFlowWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CPassengerFlowWidget);

        connect(m_CPassengerFlowWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_passenger_flow_user_return()),m_CPassengerFlowWidget,SLOT(slot_pushbutton_return()));
        connect(this,SIGNAL(signal_passenger_flow_user_home()),m_CPassengerFlowWidget,SLOT(slot_pushbutton_return()));

        connect(CMainPage::getInstance(),SIGNAL(signal_passenger_flow_widget_event(char,void *)),m_CPassengerFlowWidget,SLOT(slot_passenger_flow_widget_event(char,void *)));

    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CPassengerFlowWidget);
}

void CSystemManage::goto_storage_manage()//存储管理
{
    if(NULL == m_CStorageManageWidget)
    {
        m_CStorageManageWidget = new CStorageManageWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CStorageManageWidget);

        qRegisterMetaType<CSystemStorageManage>("CSystemStorageManage");
        connect(CMainPage::getInstance(),SIGNAL(signal_storage_manage_widget_event(CSystemStorageManage)),m_CStorageManageWidget,SLOT(slot_storage_manage_widget_event(CSystemStorageManage)));
        connect(CMainPage::getInstance(),SIGNAL(signal_storage_manage_widget_event_ex(char,void *)),m_CStorageManageWidget,SLOT(slot_storage_manage_widget_event_ex(char,void *)));
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CStorageManageWidget);
}
void CSystemManage::goto_network()//网络
{
    if(NULL == m_CNetworkWidget)
    {
        m_CNetworkWidget = new CNetworkWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CNetworkWidget);
        connect(m_CNetworkWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_network_user_return()),m_CNetworkWidget,SLOT(slot_pushbutton_return()));

        qRegisterMetaType<CSystemNetwork>("CSystemNetwork");
        connect(CMainPage::getInstance(),SIGNAL(signal_network_widget_event(CSystemNetwork)),m_CNetworkWidget,SLOT(slot_network_widget_event(CSystemNetwork)));

    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CNetworkWidget);
}
void CSystemManage::goto_switch_config()//开关量配置
{
    if(NULL == m_CSwitchConfigWidget)
    {
        m_CSwitchConfigWidget = new CSwitchConfigWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CSwitchConfigWidget);
        connect(m_CSwitchConfigWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_switch_config_user_return()),m_CSwitchConfigWidget,SLOT(slot_pushbutton_return()));

        qRegisterMetaType<CSystemIoConfig>("CSystemIoConfig");
        connect(CMainPage::getInstance(),SIGNAL(signal_switch_config_widget_event(CSystemIoConfig)),m_CSwitchConfigWidget,SLOT(slot_switch_config_widget_event(CSystemIoConfig)));

    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CSwitchConfigWidget);
}
void CSystemManage::goto_senior_setup()//高级设置
{
    if(NULL == m_SeniorSetupWidget)
    {
        m_SeniorSetupWidget = new SeniorSetupWidget(this);
        m_stackwidget_navigate_right->addWidget(m_SeniorSetupWidget);
        connect(m_SeniorSetupWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_senior_setup_user_return()),m_SeniorSetupWidget,SLOT(slot_pushbutton_return()));

        qRegisterMetaType<CSystemSeniorSetup>("CSystemSeniorSetup");
        connect(CMainPage::getInstance(),SIGNAL(signal_senior_setup_widget_event(char,CSystemSeniorSetup)),m_SeniorSetupWidget,SLOT(slot_senior_setup_widget_event(char,CSystemSeniorSetup)));
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_SeniorSetupWidget);
}

void CSystemManage::goto_pos_config()             //POS配置
{//
    if(NULL == m_CPosConfigWidget)
    {
        m_CPosConfigWidget = new CPosConfigWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CPosConfigWidget);

        connect(m_CPosConfigWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_pos_config_user_return()),m_CPosConfigWidget,SLOT(slot_pushbutton_return()));

        qRegisterMetaType<CSystemPosSetup>("CSystemPosSetup");
        connect(CMainPage::getInstance(),SIGNAL(signal_pos_config_widget_event(CSystemPosSetup)),m_CPosConfigWidget,SLOT(slot_pos_config_widget_event(CSystemPosSetup)));

    }

    m_stackwidget_navigate_right->setCurrentWidget(m_CPosConfigWidget);
}

void CSystemManage::goto_system_setup()//系统设置
{
    if(NULL == m_CSystemSetupWidget)
    {
        m_CSystemSetupWidget = new CSystemSetupWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CSystemSetupWidget);

        qRegisterMetaType<CSystemSystemSetup>("CSystemSystemSetup");
        connect(CMainPage::getInstance(),SIGNAL(signal_system_setup_widget_event(CSystemSystemSetup)),m_CSystemSetupWidget,SLOT(slot_system_setup_widget_event(CSystemSystemSetup)));

    }
    m_CSystemSetupWidget->update_system_time_show();
    m_stackwidget_navigate_right->setCurrentWidget(m_CSystemSetupWidget);
}




void CSystemManage::goto_system_maintain()        //系统维护
{
    if(NULL == m_CSystemMaintainWidget)
    {
        m_CSystemMaintainWidget = new CSystemMaintainWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CSystemMaintainWidget);

        qRegisterMetaType<CSystemSystemMaintain>("CSystemSystemMaintain");
        connect(CMainPage::getInstance(),SIGNAL(signal_system_maintain_widget_event(char,CSystemSystemMaintain)),m_CSystemMaintainWidget,SLOT(slot_system_maintain_widget_event(char,CSystemSystemMaintain)));

    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CSystemMaintainWidget);
}

void CSystemManage::goto_history_info()//历史信息
{
    if(NULL == m_CHistoryInfoWidget)
    {
        m_CHistoryInfoWidget = new CHistoryInfoWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CHistoryInfoWidget);
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CHistoryInfoWidget);
}

void CSystemManage::request_data()
{
    switch(m_current_select_id)
    {
        case CNavigateLeft::kEnumRegisterInfo:          //注册信息
        {
            CProtocol::getInstance()->call_json_system_register_info_set(CProtocol::kEnumGet);
        }break;
        case CNavigateLeft::kEnumCoordinateCollection:  //坐标采集
        {
            CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumGet,1);
        }break;
        case CNavigateLeft::kEnumVideotapeQuery:        //录像查询
        {
            if(m_CVideotapeQueryWidget)
                m_CVideotapeQueryWidget->clean_date_list();
            CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumGetDate,NULL);
        }break;
        case CNavigateLeft::kEnumCameraConfig:          //摄像机配置
        {
            CProtocol::getInstance()->call_json_system_camera_config(CProtocol::kEnumGet);
            CProtocol::getInstance()->call_json_system_pon_cam(CProtocol::kEnumGet);
        }break;
        case CNavigateLeft::kEnumPassengerFlow:         //客流统计
        {
            CProtocol::getInstance()->call_json_system_passenger_flow(CProtocol::kEnumGet);
        }break;
        case CNavigateLeft::kEnumStorageManage:         //存储管理
        {
            CProtocol::getInstance()->call_json_system_storage_manage_set(1,0);
        }break;
        case CNavigateLeft::kEnumNetwork:               //网络
        {
            CProtocol::getInstance()->call_json_system_network_set(CProtocol::kEnumGet);
        }break;
        case CNavigateLeft::kEnumSwitchConfig:          //开关量配置
        {
            CProtocol::getInstance()->call_json_system_switch_config_set(CProtocol::kEnumGet);
        }break;
        case CNavigateLeft::kEnumSeniorSetUp:           //高级设置
        {
            CProtocol::getInstance()->call_json_system_senior_setup_set(CProtocol::kEnumGet);
        }break;
        case CNavigateLeft::kEnumPosConfig:           //POS配置
        {
            CProtocol::getInstance()->call_json_system_pos_config_set(CProtocol::kEnumGet);
        }break;
        case CNavigateLeft::kEnumSystemSetUp:           //系统设置
        {
            CProtocol::getInstance()->call_json_system_system_setup_set(CProtocol::kEnumGet);
        }break;
        case CNavigateLeft::kEnumSystemMaintain:        //系统维护
        {

        }break;
        case CNavigateLeft::kEnumHistoryInfo:            //历史信息
        {

        }break;
    }
}

void CSystemManage::navigate_right_key(int _value,bool _first)
{
    switch(m_current_select_id)
    {
        case CNavigateLeft::kEnumRegisterInfo:          //注册信息
        {
            if(_first)
                m_CRegisterInfoWidget->set_focus_first();
            else
                m_CRegisterInfoWidget->keyboard_value(_value);
        }break;
        case CNavigateLeft::kEnumCoordinateCollection:  //坐标采集
        {

        }break;
        case CNavigateLeft::kEnumVideotapeQuery:        //录像查询
        {

        }break;
        case CNavigateLeft::kEnumCameraConfig:          //摄像机配置
        {

        }break;
        case CNavigateLeft::kEnumPassengerFlow:         //客流统计
        {

        }break;
        case CNavigateLeft::kEnumStorageManage:         //存储管理
        {

        }break;
        case CNavigateLeft::kEnumNetwork:               //网络
        {
            if(_first)
                m_CNetworkWidget->set_focus_first();
        }break;
        case CNavigateLeft::kEnumSwitchConfig:          //开关量配置
        {

        }break;
        case CNavigateLeft::kEnumSeniorSetUp:           //高级设置
        {

        }break;
        case CNavigateLeft::kEnumSystemSetUp:           //系统设置
        {

        }break;
        case CNavigateLeft::kEnumSystemMaintain:        //系统维护
        {

        }break;
        case CNavigateLeft::kEnumHistoryInfo:            //历史信息
        {

        }break;
    }
}

bool CSystemManage::keyboard_value(int _value)
{
    bool ret_value = false;
    switch(_value)
    {
        case KEYPAD_KEY7_F0:
        {

        }break;
        case KEYPAD_KEY7_F1:
        {

        }break;
        case KEYPAD_KEY7_F2:
        {

        }break;
        case KEYPAD_KEY7_F3:
        {

        }break;
        case KEYPAD_KEY7_F4:
        {

        }break;
        case KEYPAD_KEY7_F5:
        {

        }break;
        case KEYPAD_KEY7_F6:
        {

        }break;
        case KEYPAD_KEY7_F7:
        {

        }break;
        case KEYPAD_KEY7_F8:
        {

        }break;
        case KEYPAD_KEY7_F9:
        {

        }break;
        case KEYPAD_KEY7_PLAY:
        {

        }break;
        case KEYPAD_KEY7_REPLAY:
        {

        }break;
        case KEYPAD_KEY7_MENU:
        {
            if(kEnumCurrentLeft == m_current_left_right)
                m_CNavigateLeft->active_button_clicked();
            else
                navigate_right_key(_value);

        }break;
        case KEYPAD_KEY7_EXIT:
        {


        }break;
        case KEYPAD_KEY7_FUN:
        {

        }break;
        case KEYPAD_KEY7_UP:  //上
        {
            if(kEnumCurrentLeft == m_current_left_right)
                m_CNavigateLeft->navigate_up_down(true);
            else
            {
                  QWidget *pFocusWidget = QApplication::focusWidget();
                 bool handle_up_down = true;
                 if (pFocusWidget && pFocusWidget->inherits("QListView"))
                 {
                    QWidget *parentWidget = pFocusWidget->parentWidget();
                    if (parentWidget && parentWidget->inherits("QComboBoxPrivateContainer"))
                    {
                        handle_up_down = false;
                    }
                 }

                 if(handle_up_down)
                    navigate_right_key(_value);
            }

        }break;
        case KEYPAD_KEY7_LEFT: //左
        {
            m_current_left_right = kEnumCurrentLeft;
            m_CNavigateLeft->set_focus_first();

        }break;
        case KEYPAD_KEY7_RIGHT://右
        {
            m_current_left_right = kEnumCurrentRight;
            navigate_right_key(_value,true);
        }break;
        case KEYPAD_KEY7_DOWN://下
        {
           if(kEnumCurrentLeft == m_current_left_right)
               m_CNavigateLeft->navigate_up_down(false);
           else
           {
                 QWidget *pFocusWidget = QApplication::focusWidget();
                 bool handle_up_down = true;
                 if (pFocusWidget && pFocusWidget->inherits("QListView"))
                 {
                    QWidget *parentWidget = pFocusWidget->parentWidget();
                    if (parentWidget && parentWidget->inherits("QComboBoxPrivateContainer"))
                    {
                        handle_up_down = false;
                    }
                 }

                 if(handle_up_down)
                    navigate_right_key(_value);


           }

        }break;
    }
    return ret_value;
}











