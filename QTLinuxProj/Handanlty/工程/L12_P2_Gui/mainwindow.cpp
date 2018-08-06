#include "mainwindow.h"


#include "main_page.h"
#include "video_widget.h"
#include "arrive_widget.h"
#include "menu_widget.h"
#include "declaration_dialog.h"
#include "setting_dialog.h"
#include "state_dialog.h"

#include "common_data.h"

#include "class/ltyuicallinterface.h"
#include "class/msglist.h"
#include <QApplication>
#include <QDebug>
#include "class/ltystring.h"

#include "./menu/system_manage.h"
#include "./menu/videotape_widget.h"
#include "./menu/about_widget.h"
#include "./menu/driver_menu_widget.h"
#include "./menu/message_widget.h"
#include "./menu/run_status_widget.h"
#include "./system_manage/videotape_playback.h"
#include "./menu/passageway_widget.h"
#include "./menu/pay_info_widget.h"
#include "./menu/passenger_widget.h"

#include "class/common_interface.h"
#include "shundown_dialog.h"
#include "main_page2.h"
#include <unistd.h>
#include <pthread.h>

pthread_cond_t gui_msg_cond_com2 = 	PTHREAD_COND_INITIALIZER;
pthread_mutex_t gui_msg_mutex_com2 = 	PTHREAD_MUTEX_INITIALIZER;
int screen_flag  = 0;


static MainWindow *gMainPageThis = NULL;

static void *guiMsgDataParse(void *arg);

static bool bfirstCreate = false;
extern bool g_touch_release;



MainWindow *MainWindow::getInstance()
{
    return gMainPageThis;
}


MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent)
{
    gMainPageThis = this;
    this->setFixedSize(1024,768);
    this->setObjectName("MainWindow");
    this->setStyleSheet("QDialog#MainWindow{border:0px;background:transparent;}");
    this->setWindowFlags(Qt::FramelessWindowHint/*|Qt::WindowStaysOnTopHint*/);

    this->setAttribute(Qt::WA_TranslucentBackground, true);
#if 0
     m_button_test = new QPushButton(this);
    m_button_test->setFixedSize(100,50);
    m_button_test->move(431 + 70 + 10,100);
    m_button_test->setFocusPolicy(Qt::NoFocus);
    m_button_test->setText("测试");

    connect(m_button_test,SIGNAL(clicked()),SLOT(slot_pushbutton_test()));
#endif

    m_stackwidget_main = new QStackedWidget(this);
    m_stackwidget_main->move(0,0);
    m_stackwidget_main->setFixedSize(1024,768);
    m_stackwidget_main->setObjectName(QString::fromUtf8("m_stackwidget_main"));
    m_stackwidget_main->setContentsMargins(0, 0, 0, 0);

    m_CMainPage = NULL;
    m_CVideoWidget = NULL;
    m_CArriveWidget = NULL;
    m_CMenuWidget = NULL;
    m_CMessageWidget = NULL;
    m_CVideotapeWidget = NULL;
    m_CVideotapePlayback = NULL;
    m_CDriverMenuWidget = NULL;
    m_CSystemManage = NULL;
    m_CRunStatusWidget = NULL;
    m_CAboutWidget = NULL;
    m_CShundownDialog = NULL;
    m_CPassagewayWidget = NULL;
    m_CPayInfoWidget = NULL;
    m_CMainPage2 = NULL;
    m_CPassengerWidget = NULL;
    m_CDeclarationDiag = NULL;
    m_CSettingDialog =  NULL;
    m_CStateDialog = NULL;


///////////////////////////////////////////////
    m_time_touch_reset = new QTime;
    m_time_touch_reset->start();

    connect(this,SIGNAL(signal_touch_mouse_event(int,int,bool)),this,SLOT(slot_touch_mouse_event(int,int,bool)));
    connect(this,SIGNAL(signal_reset_touch_event1()),this,SLOT(slot_reset_touch_event()),Qt::QueuedConnection);

    CMyMouseEvent::getInstance();

    pthread_create( &thrd_msg_parse, NULL,guiMsgDataParse, this );
///////////////////////////////////////////////

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);


    m_CMainPage = new CMainPage(this);
    m_stackwidget_main->addWidget(m_CMainPage);
    connect( this, SIGNAL(signal_mainwindow_message(unsigned int)), m_CMainPage,SLOT(slot_main_page_message(unsigned int)));
    connect( m_CMainPage, SIGNAL(signal_alg_image_update(int)), this,SLOT(slot_alg_image_update(int)));
    connect(m_CMainPage,SIGNAL(signal_show_main_page(bool)),this,SLOT(slot_mainwindow_show(bool)));

    CProtocol::getInstance()->call_json_starting_screen_flag_set(CProtocol::kEnumGet);
    QTimer::singleShot(1000,this,SLOT(slot_show_main_screen()));


    QTimer::singleShot(1000,this,SLOT(slot_mainwindow_init_timer()));
    QTimer::singleShot(10,this,SLOT(slot_mainwindow_init_quick()));

#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
    CProtocol::getInstance()->call_json_system_passenger_flow(7,2,0);
#endif

}

void MainWindow::slot_alg_image_update(int type)
{
    qDebug()<<"slot_alg_image_update ";

    //1:隐藏主界面显示
    if (1 == type)
    {
    }
    //2:主界面显示
    else if(2 == type)
    {

    }
    else
    {}
    this->update();
}


void MainWindow::slot_show_main_screen()
{
       qDebug()<<"time out , show main window screen_falg\n"<< screen_flag;
        printf("slot_show_main_screen>>>>>>\n");
       if(1 == screen_flag)
        {
             printf("slot_show_main_screen>>>>>> screen_flag == 1\n");
            show_main_window_widget(kEnumMainWindow);
        }
        else
        {
             printf("slot_show_main_screen>>>>>> screen_flag == 0\n");
             show_main_window_widget(kEnumDeclaration);
        }
}

void MainWindow::slot_mainwindow_init_quick()
{
    CProtocol::getInstance()->call_json_system_pos_config_set(CProtocol::kEnumGet);
}

void MainWindow::slot_mainwindow_init_timer()
{
    CProtocol::getInstance()->call_json_menu_driver(CBaseSetWidget::kEnumSeniorGet,0);
 #ifdef APPLICATION_TYPE_P2
    CProtocol::getInstance()->request_system_message("gui_picture_play");
    CProtocol::getInstance()->request_system_message("gui_main_page");

 #endif
}



void MainWindow::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
    qDebug() << "MainWindow::resizeEvent() width:" << this->width() << "height:" << this->height();
}

void MainWindow::show_main_window_widget(char _id)
{
    switch(_id)
    {
        case kEnumMainWindow:   //主页
        {
            show_mainwindow();
        }break;
        case kEnumPlay:          //播放
        {
            show_play();
        }break;
        case kEnumMenu:          //菜单
        {
            show_menu();
        }break;
        case kEnumServiceTerams: //服务用语
        {
            show_service_terams();
        }break;
        case kEnum16Video:      //16分频
        {
            show_16video();
        }break;
        case kEnumMenuMessage:       //菜单-短信
        {
            show_menu_message();
        }break;
        case kEnumMenuCameraQuery:   //菜单-录像查询   //CVideotapePlayback
        {
            show_menu_camera_query();
        }break;
        case kEnumPlayback:
        {
            show_play_back();
        }break;
        case kEnumMenuDriverMenu:    //菜单-司机菜单
        {
            show_menu_driver_menu();
        }break;
        case kEnumMenuSystemManager: //菜单-系统管理
        {
            show_menu_system_manager();
        }break;
        case kEnumMenuRunStatus:     //菜单-运行状态
        {
            show_menu_run_status();
        }break;
        case kEnumMenuAboutMachine:   //菜单-关于本机
        {
            show_menu_about_machine();
        }break;
        case kEnumShutDown:
        {
            show_shut_down();
        }break;
        case kEnumPassageway:
        {
            show_menu_passageway();
        }break;
        case kEnumPayInfo:
        {
            show_menu_pay_info();
        }break;
        case kEnumPassenger:
        {
            show_menu_passenger();
        }break;
        case kEnumDeclaration:
        {
            show_start_declaration();
        }break;
        case kEnumSetting:
        {
             show_start_setting();
        }break;
        case kEnumState:
        {
             show_start_state();
        }break;
    }

    m_current_widget_id = _id;
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    if(kEnumMainWindow != m_current_widget_id)
        CProtocol::getInstance()->call_json_system_passenger_flow(7,2,0);
#endif
}


#if 1
//主页
void MainWindow::show_mainwindow()
{
       if (NULL ==  m_CMainPage)
       {
          m_CMainPage = new CMainPage(this);
          m_stackwidget_main->addWidget(m_CMainPage);
       }

      if(false == bfirstCreate)
      {
            bfirstCreate = true;
            connect(m_CMainPage,SIGNAL(signal_pushbutton_play()),this,SLOT(slot_pushbutton_play_ex()));
            connect(m_CMainPage,SIGNAL(signal_pushbutton_menu()),this,SLOT(slot_pushbutton_menu()));
            connect(m_CMainPage,SIGNAL(signal_pushbutton_16_frame()),this,SLOT(slot_pushbutton_16_frame()));
            connect(m_CMainPage,SIGNAL(signal_pushbutton_back()),this,SLOT(slot_pushbutton_back()));
            connect(m_CMainPage,SIGNAL(signal_mainwindow_show(bool)),this,SLOT(slot_mainwindow_show(bool)));

            connect(m_CMainPage,SIGNAL(signal_driver_enter_station(bool)),this,SLOT(slot_driver_enter_station(bool)));
            connect(m_CMainPage,SIGNAL(signal_back_car_notify(bool)),this,SLOT(slot_back_car_notify(bool)));
        }

    m_stackwidget_main->setCurrentWidget(m_CMainPage);
#endif
#if defined(APPLICATION_TYPE_D2)
    show_16video();
    m_CVideoWidget->set_main_page();

    CProtocol::getInstance()->call_json_system_passenger_flow(7,1,0);
#elif defined(APPLICATION_TYPE_P2)
    show_main_page2();
    static bool first_init = true;
    if(first_init)
    {
        first_init = false;
        m_CMainPage->main_page_info_init();
    }

    CProtocol::getInstance()->call_json_system_passenger_flow(7,2,0);
#endif

}

void MainWindow::play_widget()
{
    CCommonInterface::printf_debug("MainWindow::play_widget()");

    arrive_widget_init();

    m_stackwidget_main->setCurrentWidget(m_CArriveWidget);

    m_CArriveWidget->slot_arrive_widget_event(CArriveWidget::kEnumSetStationName,0,CCommonData::getInstance()->m_station_info.m_current_station);
    m_CArriveWidget->slot_arrive_widget_event(CArriveWidget::kEnumSetLimitSpeed,CCommonData::getInstance()->m_station_info.m_current_limspeed,"");
    m_CArriveWidget->slot_arrive_widget_event(CArriveWidget::kEnumSetCurrentSpeed,CCommonData::getInstance()->m_station_info.m_current_speed,"");
    m_CArriveWidget->slot_arrive_widget_event(CArriveWidget::kEnumSetUpVehicle,CCommonData::getInstance()->m_passenger_flow.m_up_vehicle,"");
    m_CArriveWidget->slot_arrive_widget_event(CArriveWidget::kEnumSetDownVehicle,CCommonData::getInstance()->m_passenger_flow.m_down_vehicle,"");
    m_CArriveWidget->slot_arrive_widget_event(CArriveWidget::kEnumSetWithinVehicle,CCommonData::getInstance()->m_passenger_flow.m_within_vehicle,"");

    QString str_status = "";
    if(1 == CCommonData::getInstance()->station_status)
         str_status = CLtyString::getInstance()->m_station_in;
    else
         str_status = CLtyString::getInstance()->m_station_out;

    if(m_CArriveWidget->is_back_car_show_mode())
    {
        CCommonInterface::printf_debug("MainWindow::play_widget() slot_pushbutton_amplify");
        m_CArriveWidget->slot_pushbutton_amplify();
    }
    else
    {
        CCommonInterface::printf_debug("MainWindow::play_widget() data_init");
        m_CArriveWidget->data_init();
    }
    m_CArriveWidget->slot_arrive_widget_event(CArriveWidget::kEnumSetVehicleStatus,0,str_status);

}

bool MainWindow::is_back_car_show_mode()
{
    if(m_CArriveWidget)
        return m_CArriveWidget->is_back_car_show_mode();

    return false;
}
void MainWindow::arrive_widget_init()
{
    if(NULL == m_CArriveWidget)
    {
        m_CArriveWidget = new CArriveWidget(this);
        connect(m_CMainPage,SIGNAL(signal_main_page_arrive_widget_event(int,int,QString)),m_CArriveWidget,SLOT(slot_arrive_widget_event(int,int,QString)));
        connect(m_CArriveWidget,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CMainPage,SIGNAL(signal_arrive_widget_notify_event(char,char,bool)),m_CArriveWidget,SLOT(slot_video_notify_event(char,char,bool)));



        m_stackwidget_main->addWidget(m_CArriveWidget);
    }
}

void MainWindow::slot_back_car_notify(bool _show)
{
    arrive_widget_init();
    //CCommonInterface::printf_debug("MainWindow::play_widget() slot_back_car_notify() " + QString::number(_show));
    m_CArriveWidget->slot_back_car_widget(_show);
}

void MainWindow::show_play()//播放
{
    //CCommonInterface::printf_debug("show_play() play_widget()");
    play_widget();

}

void MainWindow::slot_driver_enter_station(bool _flag)
{
    //CCommonInterface::printf_debug("slot_driver_enter_station: " + QString::number(_flag));
    if(_flag)
    {
        //CCommonInterface::printf_debug("slot_driver_enter_station() play_widget");
        slot_pushbutton_play();

    }
    else
    {
        //CCommonInterface::printf_debug("slot_driver_enter_station() slot_mainwindow_show");
        slot_mainwindow_show(true);
    }
}

void MainWindow::show_menu()//菜单
{

    if(NULL == m_CMenuWidget)
    {
        m_CMenuWidget = new CMenuWidget();
        m_stackwidget_main->addWidget(m_CMenuWidget);
        connect(m_CMenuWidget,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));

        connect(m_CMenuWidget,SIGNAL(signal_pushbutton_message()),this,SLOT(slot_pushbutton_message()));
        connect(m_CMenuWidget,SIGNAL(signal_pushbutton_camera_query()),this,SLOT(slot_pushbutton_camera_query()));
        connect(m_CMenuWidget,SIGNAL(signal_pushbutton_driver_menu()),this,SLOT(slot_pushbutton_driver_menu()));
        connect(m_CMenuWidget,SIGNAL(signal_pushbutton_system_manager()),this,SLOT(slot_pushbutton_system_manager()));
        connect(m_CMenuWidget,SIGNAL(signal_pushbutton_run_status()),this,SLOT(slot_pushbutton_run_status()));
        connect(m_CMenuWidget,SIGNAL(signal_pushbutton_about_machine()),this,SLOT(slot_pushbutton_about_machine()));

        connect(m_CMenuWidget,SIGNAL(signal_pushbutton_passenger_flow()),this,SLOT(slot_pushbutton_passageway()));

        connect(m_CMenuWidget,SIGNAL(signal_pushbutton_pay_info()),this,SLOT(slot_pushbutton_pay_info()));
        connect(m_CMenuWidget,SIGNAL(signal_pushbutton_passenger()),this,SLOT(slot_pushbutton_passenger()));


        CProtocol::getInstance()->request_system_message("gui_login_info");
        CProtocol::getInstance()->call_json_system_system_setup_set(CProtocol::kEnumGet);
    }

    m_stackwidget_main->setCurrentWidget(m_CMenuWidget);
    //m_CMenuWidget->set_first_focus();

}

void MainWindow::show_service_terams()//服务用语
{

}

//16分频
void MainWindow::show_16video()
{
    if(NULL == m_CVideoWidget)
    {
        m_CVideoWidget = new CVideoWidget(this);
        m_stackwidget_main->addWidget(m_CVideoWidget);

        connect(m_CVideoWidget,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CMainPage,SIGNAL(signal_vidoe_widget_event(char,char,bool)),m_CVideoWidget,SLOT(slot_vidoe_widget_event(char,char,bool)));

        connect(m_CVideoWidget,SIGNAL(signal_pushbutton_quit()),this,SLOT(slot_pushbutton_back()));
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
        connect(m_CMainPage,SIGNAL(signal_main_page_notify(int,int,void *)),m_CVideoWidget,SLOT(slot_msg_notify(int,int,void *)));

         qRegisterMetaType<CPassengerInfo>("CPassengerInfo");
        connect(m_CMainPage,SIGNAL(signal_passenger_info(CPassengerInfo)),m_CVideoWidget,SIGNAL(signal_passenger_info(CPassengerInfo)));
#endif
#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
        qRegisterMetaType<CPassengerInfo>("CPassengerInfo");
        connect(m_CMainPage,SIGNAL(signal_passenger_info(CPassengerInfo)),m_CVideoWidget,SIGNAL(signal_passenger_info(CPassengerInfo)));
#endif
        connect(m_CVideoWidget,SIGNAL(signal_button_menu()),this,SLOT(slot_pushbutton_menu()));

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
        m_CVideoWidget->set_start_status();
        m_CMainPage->main_page_info_init();
#endif

    }
#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
    m_CVideoWidget->video_init();
#endif
    m_stackwidget_main->setCurrentWidget(m_CVideoWidget);
}
//菜单下的按钮
void MainWindow::show_menu_message()
{
    if(NULL == m_CMessageWidget)
    {
        m_CMessageWidget = new CMessageWidget(this);
        m_stackwidget_main->addWidget(m_CMessageWidget);
        connect(m_CMessageWidget,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CMessageWidget,SIGNAL(signal_pushbutton_menu()),this,SLOT(slot_pushbutton_menu()));

        qRegisterMetaType<CCommonQueryMode>("CCommonQueryMode");
        connect(m_CMainPage,SIGNAL(signal_message_widget_notify_event(int,CCommonQueryMode)),m_CMessageWidget,SLOT(slot_message_widget_notify_event(int,CCommonQueryMode)));
    }
    m_stackwidget_main->setCurrentWidget(m_CMessageWidget);

}
void MainWindow::show_menu_camera_query()
{
    if(NULL == m_CVideotapeWidget)
    {
        m_CVideotapeWidget = new CVideotapeWidget(this);
        m_stackwidget_main->addWidget(m_CVideotapeWidget);
        connect(m_CVideotapeWidget,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CVideotapeWidget,SIGNAL(signal_pushbutton_menu()),this,SLOT(slot_pushbutton_menu()));

    }
    m_is_system_enter = false;
    m_CVideotapeWidget->clean_date_list();
    CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumGetDate,NULL);
    m_stackwidget_main->setCurrentWidget(m_CVideotapeWidget);
}

void MainWindow::show_menu_passageway()
{
    if(NULL == m_CPassagewayWidget)
    {
        m_CPassagewayWidget = new CPassagewayWidget(this);
        m_stackwidget_main->addWidget(m_CPassagewayWidget);
        connect(m_CPassagewayWidget,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CPassagewayWidget,SIGNAL(signal_pushbutton_menu()),this,SLOT(slot_pushbutton_menu()));

    }
    m_stackwidget_main->setCurrentWidget(m_CPassagewayWidget);
}

void MainWindow::show_menu_pay_info()
{
    if(NULL == m_CPayInfoWidget)
    {
        m_CPayInfoWidget = new CPayInfoWidget(this);
        m_stackwidget_main->addWidget(m_CPayInfoWidget);
        connect(m_CPayInfoWidget,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CPayInfoWidget,SIGNAL(signal_pushbutton_menu()),this,SLOT(slot_pushbutton_menu()));

                qRegisterMetaType<CGetQrPayInfo>("CGetQrPayInfo");
        connect(m_CMainPage,SIGNAL(signal_paycard_record_widget(CGetQrPayInfo)),m_CPayInfoWidget,SLOT(slot_paycard_record_widget(CGetQrPayInfo)));

    }
    CProtocol::getInstance()->request_system_message("get_qr_pay_info");
    m_stackwidget_main->setCurrentWidget(m_CPayInfoWidget);
}
#if 1
void MainWindow::show_menu_passenger()
{
    if(NULL == m_CPassengerWidget)
    {
        m_CPassengerWidget = new CPassengerWidget(this);
        m_stackwidget_main->addWidget(m_CPassengerWidget);
        connect(m_CPassengerWidget,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CPassengerWidget,SIGNAL(signal_pushbutton_menu()),this,SLOT(slot_pushbutton_menu()));

        qRegisterMetaType<CGetPassengerInfo>("CGetPassengerInfo");
        connect(m_CMainPage,SIGNAL(signal_passenger_widget(CGetPassengerInfo)),m_CPassengerWidget,SLOT(slot_passenger_widget(CGetPassengerInfo)));

    }
    m_CPassengerWidget->slot_passenger_widget(CCommonData::getInstance()->m_CGetPassengerInfo);
    m_stackwidget_main->setCurrentWidget(m_CPassengerWidget);
}

void MainWindow::show_start_declaration()
{
    qDebug() << "MainWindow::show_start_declaration()>>>>>>>>>>>\n";
    if(NULL == m_CDeclarationDiag)
    {
        m_CDeclarationDiag = new CDeclarationDialog(this);
        m_stackwidget_main->addWidget(m_CDeclarationDiag);
        connect(m_CDeclarationDiag, SIGNAL(decButtonClicked()), this, SLOT(slot_decpushbutton_setting_page()));
    }
    m_stackwidget_main->setCurrentWidget(m_CDeclarationDiag);
}

void MainWindow::show_start_setting()
{
    qDebug() << "MainWindow::show_start_setting()>>>>>>>>>>>>";
    if(NULL == m_CSettingDialog)
    {
        m_CSettingDialog = new CSettingDialog(this);
        m_stackwidget_main->addWidget(m_CSettingDialog);
        connect(m_CSettingDialog, SIGNAL(settingButtonClicked()), this, SLOT(slot_decpushbutton_state_page()));
    }
    m_stackwidget_main->setCurrentWidget(m_CSettingDialog);
}

void MainWindow::show_start_state()
{
        qDebug() << "MainWindow::show_start_state()>>>>>>>>>>>";
        if (NULL == m_CStateDialog)
        {
            m_CStateDialog = new CStateDialog(this);
            m_stackwidget_main->addWidget(m_CStateDialog);
            connect(m_CStateDialog, SIGNAL(statePreButtonClicked()), this, SLOT(slot_statePreButtonClicked()));
            connect(m_CStateDialog, SIGNAL(stateFinishButtonClicked()), this, SLOT(slot_stateFinishButtonClicked()));
            // connect(m_CMainPage, SIGNAL(signal_network_status(int, int)), m_CStateDialog, SLOT(slot_signal_network_status(int, int)));
            qRegisterMetaType<CRunStatusInfo>("CRunStatusInfo");
            // 取信号强度的判断，不正确
            connect(m_CMainPage,SIGNAL(signal_network_status(int, CRunStatusInfo)),m_CStateDialog,SLOT(slot_network_status(int, CRunStatusInfo)));
            connect(m_CMainPage,SIGNAL(signal_tmp_status(int, int, int)),m_CStateDialog,SLOT(slot_tmp_status(int, int, int)));
            connect(m_CMainPage,SIGNAL(signal_g3_network_status(int, int)),m_CStateDialog,SLOT(slot_g3_network_status(int, int)));

        }
        m_stackwidget_main->setCurrentWidget(m_CStateDialog);


}
#endif

void MainWindow::show_main_page2()
{
    if(NULL == m_CMainPage2)
    {
        m_CMainPage2 = new CMainPage2(this);
        m_stackwidget_main->addWidget(m_CMainPage2);

        connect(m_CMainPage,SIGNAL(signal_main_page_notify(int,int,void *)),m_CMainPage2,SLOT(slot_head_widget_notify(int,int,void *)));
        connect(m_CMainPage2,SIGNAL(signal_button_menu()),this,SLOT(slot_pushbutton_menu()));
        connect(m_CMainPage2,SIGNAL(signal_button_exit()),this,SLOT(slot_pushbutton_test()));
        qRegisterMetaType<CPlayPicture>("CPlayPicture");
        connect(m_CMainPage,SIGNAL(signal_main_page2_picture(CPlayPicture)),m_CMainPage2,SLOT(slot_main_page2_picture(CPlayPicture)));

    }
    m_stackwidget_main->setCurrentWidget(m_CMainPage2);
}

void MainWindow::show_play_back()
{
    if(NULL == m_CVideotapePlayback)
    {
        m_CVideotapePlayback = new CVideotapePlayback(this);
        m_stackwidget_main->addWidget(m_CVideotapePlayback);
        connect(m_CVideotapePlayback,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CVideotapePlayback,SIGNAL(signal_button_back()),this,SLOT(slot_playback_return()));

        connect(m_CMainPage,SIGNAL(signal_videotape_query_playback(int)),m_CVideotapePlayback,SLOT(slot_videotape_query_playback(int)));

        connect(m_CMainPage,SIGNAL(signal_playback_quit()),m_CVideotapePlayback,SLOT(slot_playback_quit()));
        connect(m_CMainPage,SIGNAL(signal_playback_notify_event(char,char)),m_CVideotapePlayback,SLOT(slot_playback_notify_event(char,char)));
    }
    m_CVideotapePlayback->playback_init();
    m_stackwidget_main->setCurrentWidget(m_CVideotapePlayback);

}
void MainWindow::slot_playback_return()
{
    if(m_is_system_enter)
        slot_pushbutton_system_manager();
    else
        slot_pushbutton_camera_query();
}
void MainWindow::show_menu_driver_menu()
{
    if(NULL == m_CDriverMenuWidget)
    {
        m_CDriverMenuWidget = new CDriverMenuWidget(this);
        m_stackwidget_main->addWidget(m_CDriverMenuWidget);

        connect(m_CMainPage,SIGNAL(signal_voice_dialog_event(int,int,int)),m_CDriverMenuWidget,SLOT(slot_driver_menu_voice_event(int,int,int)));
        connect(m_CMainPage,SIGNAL(signal_driver_menu_light_event(int)),m_CDriverMenuWidget,SLOT(slot_driver_menu_light_event(int)));

        qRegisterMetaType<CDriverMenuInfo>("CDriverMenuInfo");
        connect(m_CMainPage,SIGNAL(signal_driver_menu_widget_event(CDriverMenuInfo)),m_CDriverMenuWidget,SLOT(slot_driver_menu_widget_event(CDriverMenuInfo)));

        connect(m_CDriverMenuWidget,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CDriverMenuWidget,SIGNAL(signal_pushbutton_menu()),this,SLOT(slot_pushbutton_menu()));



    }
    m_stackwidget_main->setCurrentWidget(m_CDriverMenuWidget);

}
void MainWindow::show_menu_system_manager()
{
    if(NULL == m_CSystemManage)
    {
        m_CSystemManage = new CSystemManage(this);
        m_stackwidget_main->addWidget(m_CSystemManage);
        connect(m_CSystemManage,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CSystemManage,SIGNAL(signal_pushbutton_menu()),this,SLOT(slot_pushbutton_menu()));
    }
    m_is_system_enter = true;
    m_CSystemManage->system_manage_init();
    m_CSystemManage->request_data();
    m_stackwidget_main->setCurrentWidget(m_CSystemManage);

}
void MainWindow::show_menu_run_status()
{
    if(NULL == m_CRunStatusWidget)
    {
        m_CRunStatusWidget = new CRunStatusWidget(this);
        m_stackwidget_main->addWidget(m_CRunStatusWidget);
        connect(m_CRunStatusWidget,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CRunStatusWidget,SIGNAL(signal_pushbutton_menu()),this,SLOT(slot_pushbutton_menu()));

        qRegisterMetaType<CRunStatusInfo>("CRunStatusInfo");
        connect(m_CMainPage,SIGNAL(signal_run_status_widget_event(CRunStatusInfo)),m_CRunStatusWidget,SLOT(slot_run_status_widget_event(CRunStatusInfo)));
    }
    m_stackwidget_main->setCurrentWidget(m_CRunStatusWidget);

}
void MainWindow::show_menu_about_machine()
{
    if(NULL == m_CAboutWidget)
    {
        m_CAboutWidget = new CAboutWidget(this);
        m_stackwidget_main->addWidget(m_CAboutWidget);
        connect(m_CAboutWidget,SIGNAL(signal_pushbutton_home()),this,SLOT(slot_pushbutton_home_page()));
        connect(m_CAboutWidget,SIGNAL(signal_pushbutton_menu()),this,SLOT(slot_pushbutton_menu()));

        qRegisterMetaType<CAboutMachineInfo>("CAboutMachineInfo");
        connect(m_CMainPage,SIGNAL(signal_about_machine_widget_event(CAboutMachineInfo)),m_CAboutWidget,SLOT(slot_about_machine_widget_event(CAboutMachineInfo)));
    }
    CProtocol::getInstance()->call_json_about_machine_set(1);
    m_stackwidget_main->setCurrentWidget(m_CAboutWidget);
}

void MainWindow::show_shut_down()
{
    if(NULL == m_CShundownDialog)
    {
        m_CShundownDialog = new CShundownDialog(this);
        m_stackwidget_main->addWidget(m_CShundownDialog);
    }
    m_stackwidget_main->setCurrentWidget(m_CShundownDialog);
}

void MainWindow::slot_pushbutton_back()
{
    close();
}

void MainWindow::slot_pushbutton_home_page()
{
    show_main_window_widget(kEnumMainWindow);
}

void MainWindow::slot_pushbutton_16_frame()
{
    show_main_window_widget(kEnum16Video);
}

void MainWindow::slot_pushbutton_play()          //播放
{
    CCommonInterface::printf_debug("MainWindow::slot_pushbutton_play()");
    show_main_window_widget(kEnumPlay);
}
void MainWindow::slot_pushbutton_play_ex()
{
    do
    {
        CCommonInterface::printf_debug( "MainWindow::slot_pushbutton_play_ex()");
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }
        cJSON_AddStringToObject(root, "msg_type", "gui_station_info");

        cJSON_AddNumberToObject(root, "station_play", 1);

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);

}

void MainWindow::slot_pushbutton_menu()          //菜单
{

    show_main_window_widget(kEnumMenu);

}

void MainWindow::slot_decpushbutton_setting_page()
{
    qDebug() << "MainWindow::slot_decpushbutton_setting_page>>>>>>>>>>>";
    show_main_window_widget(kEnumSetting);
}

void MainWindow::slot_decpushbutton_state_page()
{
     qDebug() << "MainWindow::slot_decpushbutton_state_page>>>>>>>>>>>";
     show_main_window_widget(kEnumState);
}


void MainWindow::slot_statePreButtonClicked()
{
     qDebug() << "MainWindow::slot_statePreButtonClicked>>>>>>>>>>>";
     show_main_window_widget(kEnumSetting);
}

void MainWindow::slot_stateFinishButtonClicked()
{
     qDebug() << "MainWindow::slot_stateFinishButtonClicked>>>>>>>>>>>";
     show_main_window_widget(kEnumMainWindow);
}

//菜单下的按钮
void MainWindow::slot_pushbutton_message()
{
    show_main_window_widget(kEnumMenuMessage);
}
void MainWindow::slot_pushbutton_camera_query()
{
    show_main_window_widget(kEnumMenuCameraQuery);
}

void MainWindow::slot_pushbutton_passageway()
{
    show_main_window_widget(kEnumPassageway);
}

void MainWindow::slot_pushbutton_pay_info()
{
    show_main_window_widget(kEnumPayInfo);
}

void MainWindow::slot_pushbutton_passenger()
{
    show_main_window_widget(kEnumPassenger);
}

void MainWindow::slot_pushbutton_playback()
{
    show_main_window_widget(kEnumPlayback);
}

void MainWindow::slot_pushbutton_driver_menu()
{
    show_main_window_widget(kEnumMenuDriverMenu);
}
void MainWindow::slot_pushbutton_system_manager()
{
    show_main_window_widget(kEnumMenuSystemManager);
}
void MainWindow::slot_pushbutton_run_status()
{
    show_main_window_widget(kEnumMenuRunStatus);
}
void MainWindow::slot_pushbutton_about_machine()
{
    show_main_window_widget(kEnumMenuAboutMachine);
}

void MainWindow::slot_pushbutton_shutdown()
{
    show_main_window_widget(kEnumShutDown);
}

void MainWindow::slot_pushbutton_test()
{
    close();
}

////////////////////////////////////////////////////////////////

void MainWindow::reset_touch_timer()
{
#if 0
    int time_move = 0;

    time_move = m_time_touch_reset->elapsed();

    //if(time_move >= 1/*20*/)
    {
       emit signal_reset_touch_event();
        m_time_touch_reset->restart();
    }
#endif

    emit signal_reset_touch_event1();
}

void MainWindow::slot_reset_touch_event()
{
    //CCommonInterface::printf_debug("MainWindow::slot_reset_touch_event()");
    CMyMouseEvent::getInstance()->reset_timer();
}

bool MainWindow::current_first_press()
{
    return CMyMouseEvent::getInstance()->m_first_press;
}

#include <QDebug>
static TOUCH_ATTR_S gTouchAttr;
#include "./class/common/customevent.h"
#define INVALID_TOUCH_DATA  30000
static void *guiMsgDataParse(void *arg)
{
    pthread_detach( pthread_self());

    unsigned int len = 0,addr = 0;
    MainWindow *mWiget = (MainWindow *)arg;
    char message_type;
    while(1)
    {
        gTouchAttr.x = INVALID_TOUCH_DATA;
        if(GuiMsgListGet(&gTouchAttr,&len,&addr,0,&message_type))
        {
            if(mWiget)
            {

                if(kEnumUiMessageTouch == message_type)
                {
                    //CCommonInterface::printf_debug("guiMsgDataParse kEnumUiMessageTouch");
#if 1
                    if(mWiget->current_first_press())
                    {

                        //CCommonInterface::printf_debug("guiMsgDataParse kEnumUiMessageTouch 1");
                        static QTime gui_msg_tick;
                        static bool first_start = true;
                        if(first_start)
                        {
                            gui_msg_tick.start();
                            first_start = false;
                        }
                        //CCommonInterface::printf_debug("guiMsgDataParse 1");
                        int tick_count = gui_msg_tick.elapsed();
                        //if(tick_count >= 5)
                        {
                           // g_time_tick.begin();
                            gui_msg_tick.restart();
                            //QApplication::postEvent((QObject *)mWiget, new CustomEvent(CustomEvent::m_lty_root, (void *)addr));
                            if(INVALID_TOUCH_DATA != gTouchAttr.x)
                            {
                                //CCommonInterface::printf_debug("guiMsgDataParse 2");
                                bool release = false;
                                if(1 == gTouchAttr.up)
                                {
                                    QPoint first_point = mWiget->get_first_point();
                                    gTouchAttr.x = first_point.x();
                                    gTouchAttr.y = first_point.y();
                                    release = true;
                                    g_touch_release = true;
                                }
                                mWiget->handle_mouse_event(gTouchAttr.x,gTouchAttr.y,release);
                                //CCommonInterface::printf_debug("guiMsgDataParse 3");
                            }

                        }


                    }
                    else
                    {
                       // CCommonInterface::printf_debug("guiMsgDataParse kEnumUiMessageTouch  2");
                       //CCommonInterface::printf_debug("guiMsgDataParse 3");
                       bool release = false;
                        if(1 == gTouchAttr.up)
                        {
                            QPoint first_point = mWiget->get_first_point();
                            gTouchAttr.x = first_point.x();
                            gTouchAttr.y = first_point.y();
                            release = true;
                            g_touch_release = true;
                        }
                        if(INVALID_TOUCH_DATA != gTouchAttr.x)
                        {
                          //  CCommonInterface::printf_debug("guiMsgDataParse kEnumUiMessageTouch  3");
                            mWiget->handle_mouse_event(gTouchAttr.x,gTouchAttr.y,release);
                        }
                        //CCommonInterface::printf_debug("guiMsgDataParse 4");
                        //QApplication::postEvent((QObject *)mWiget, new CustomEvent(CustomEvent::m_lty_root, (void *)addr));
                    }
#endif
                }
                else
                {
                    QApplication::postEvent((QObject *)mWiget, new CustomEvent(CustomEvent::m_lty_root, (void *)addr));
                }

            }
        }
        if(!GuiMsgHasTask())
            usleep(1000 * 10);
    }

    pthread_exit(0);
}

void MainWindow::customEvent(QEvent *e)
{
    CustomEvent *pCustom = static_cast<CustomEvent *>(e);
    if (NULL ==  pCustom)
    {
        return;
    }

    switch (e->type())
    {
        case QEVENT_LTY_ROOT:
        {
            emit signal_mainwindow_message((unsigned int)pCustom->m_pParam);
            //delete pData;

        }break;
    }
}

bool MainWindow::handle_mouse_event(int _x,int _y,bool _flag)
{
    if(CCommonInterface::notify_ui_printf())
        CCommonInterface::printf_debug("MainWindow::handle_mouse_event()");
    emit signal_touch_mouse_event(_x,_y,_flag);

    return true;
}
#include <QWSServer>
void MainWindow::slot_touch_mouse_event(int _x,int _y,bool _flag)
{
    if(CCommonInterface::notify_ui_printf())
        CCommonInterface::printf_debug("MainWindow::slot_touch_mouse_event() start");
    if(_flag)
        CMyMouseEvent::getInstance()->stop_timer();

    CMyMouseEvent::getInstance()->touch_click_event(_x,_y,_flag);


    if(CCommonInterface::notify_ui_printf())
        CCommonInterface::printf_debug("MainWindow::slot_touch_mouse_event() end");
}

QPoint MainWindow::get_first_point()
{
    return CMyMouseEvent::getInstance()->get_first_point();
}

#include "class/common_interface.h"
void MainWindow::handleKeyEvent(QObject *pObj,QKeyEvent *_event)
{
    do
    {

           // CCommonInterface::printf_debug("MainWindow::handleKeyEvent m_CMenuWidget");
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(_event);

        switch(keyEvent->key())
        {
            case Qt::Key_Up:
            {
                CCommonInterface::printf_debug("MainWindow::handleKeyEvent() Qt::Key_Up");
                m_CMainPage->keyboard_value(KEYPAD_KEY7_UP);
            }break;
            case Qt::Key_Down:
            {
                CCommonInterface::printf_debug("MainWindow::handleKeyEvent() Qt::Key_Down");
                m_CMainPage->keyboard_value(KEYPAD_KEY7_DOWN);
            }break;
            case Qt::Key_Left:
            {
                CCommonInterface::printf_debug("MainWindow::handleKeyEvent() Qt::Key_Left");
                m_CMainPage->keyboard_value(KEYPAD_KEY7_LEFT);
            }break;
            case Qt::Key_Right:
            {
                CCommonInterface::printf_debug("MainWindow::handleKeyEvent() Qt::Key_Right");
                m_CMainPage->keyboard_value(KEYPAD_KEY7_RIGHT);
            }break;
            case Qt::Key_F1: //返回
            {
                m_CMainPage->gui_system_key_up(NULL,true,KEYPAD_KEY7_EXIT);
            }break;
            case Qt::Key_F2: //菜单\ENTER
            //case Qt::Key_Return:
            {
                if(kEnumMainWindow == get_current_widget_id())
                {
                    m_CMainPage->gui_system_key_up(NULL,true,KEYPAD_KEY7_MENU);
                }
                else
                {
                    QWidget *pFocusWidget = QApplication::focusWidget();
                    CCommonInterface::printf_debug("MainWindow::handleKeyEvent() 1");
                    if(pFocusWidget && pFocusWidget->inherits("QComboBox"))
                    {
                        QWidget *parent_widget = pFocusWidget->parentWidget();
                        CCommonInterface::printf_debug("MainWindow::handleKeyEvent() 2");
                        if(NULL == parent_widget)
                            break;

                        qDebug() << "mainwindow:" << this->pos();
                        qDebug() << pFocusWidget->pos();
                        QPoint point = parent_widget->mapToGlobal(pFocusWidget->pos());
                        qDebug() << point;
                        if(!((QComboBox *)pFocusWidget)->view()->isVisible())
                        {
                            CMyMouseEvent::getInstance()->touch_click_event(point.x(),point.y(),false);
                            CMyMouseEvent::getInstance()->touch_click_event(point.x(),point.y(),true);
                        }

                    }
                    else
                    {
                        CCommonInterface::printf_debug("MainWindow::handleKeyEvent() 3");
                        m_CMainPage->gui_system_key_up(NULL,true,KEYPAD_KEY7_MENU);
                    }
                }


                CCommonInterface::printf_debug("MainWindow::handleKeyEvent() Qt::Key_Return");

            }break;
        }


        //CCommonInterface::printf_debug("MainWindow::handleKeyEvent end");

    }while(0);
}

void MainWindow::slot_mainwindow_show(bool _show)
{
    if(_show)
    {
        slot_pushbutton_home_page();
        this->show();
    }
    else
        this->hide();
}

QWidget *MainWindow::get_stack_widget()
{
    return m_stackwidget_main->currentWidget();
}












