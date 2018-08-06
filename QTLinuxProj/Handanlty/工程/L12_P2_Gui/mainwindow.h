#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QPushButton>
#include <QStackedWidget>
#include <QResizeEvent>

#include "class/common/my_mouse_event.h"

#include "class/common/define_screen.h"

extern pthread_cond_t gui_msg_cond_com2 ;
extern pthread_mutex_t gui_msg_mutex_com2 ;

extern int screen_flag;

class CMainPage;
class CVideoWidget;
class CArriveWidget;
class CMenuWidget;

class CMessageWidget;
class CVideotapeWidget;
class CDriverMenuWidget;
class CSystemManage;
class CRunStatusWidget;
class CAboutWidget;
class CVideotapePlayback;
class CShundownDialog;
class CPassagewayWidget;
class CPayInfoWidget;
class CMainPage2;
class CPassengerWidget;
class  CDeclarationDialog;
class  CSettingDialog;
class  CStateDialog;

class MainWindow : public QDialog
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    static MainWindow *getInstance();
    enum
    {
        kEnumMainWindow,    //主页
        kEnumPlay,          //播放
        kEnumMenu,          //菜单
        kEnumServiceTerams, //服务用语
        kEnum16Video,        //16分频
        kEnumMenuMessage,       //菜单-短信
        kEnumMenuCameraQuery,   //菜单-录像查询
        kEnumPlayback,          //菜单-录像查询-回放
        kEnumMenuDriverMenu,    //菜单-司机菜单
        kEnumMenuSystemManager, //菜单-系统管理
        kEnumMenuRunStatus,     //菜单-运行状态
        kEnumMenuAboutMachine,   //菜单-关于本机
        kEnumShutDown,            //关机界面
        kEnumPassageway,
        kEnumPayInfo,
        kEnumPassenger,
        kEnumDeclaration, // 开机免责声明画面
        kEnumSetting,  //开机用户设定画面
        kEnumState  //状态显示
    };
signals:
    void signal_reset_touch_event1();
    void signal_mainwindow_message(unsigned int);
    void signal_touch_mouse_event(int,int,bool);
public slots:
    void slot_pushbutton_test();

    void slot_pushbutton_back();
    void slot_pushbutton_home_page();
    void slot_pushbutton_16_frame();
    void slot_pushbutton_play();          //播放界面
    void slot_pushbutton_play_ex();
    void slot_pushbutton_menu();          //菜单
    void slot_decpushbutton_setting_page();
    void slot_decpushbutton_state_page();
    void slot_statePreButtonClicked();
    void slot_stateFinishButtonClicked();

    //菜单下的按钮
    void slot_pushbutton_message();
    void slot_pushbutton_camera_query();
    void slot_pushbutton_playback();
    void slot_pushbutton_driver_menu();
    void slot_pushbutton_system_manager();
    void slot_pushbutton_run_status();
    void slot_pushbutton_about_machine();
    void slot_pushbutton_shutdown();
    void slot_pushbutton_passageway();
    void slot_pushbutton_pay_info();
    void slot_pushbutton_passenger();

    void slot_driver_enter_station(bool _flag);

    void slot_reset_touch_event();
    void slot_touch_mouse_event(int _x,int _y,bool _flag);
    void slot_mainwindow_show(bool _show);

    void slot_playback_return();
    void slot_mainwindow_init_timer();
    void slot_mainwindow_init_quick();
    void slot_back_car_notify(bool _show);
    void slot_show_main_screen();
    void slot_alg_image_update(int type);
public:
    void customEvent(QEvent *e);
    void reset_touch_timer();
    bool current_first_press();
    bool handle_mouse_event(int _x,int _y,bool _flag);
    QPoint get_first_point();
    char  get_current_widget_id(){return m_current_widget_id;}

    void handleKeyEvent(QObject *pObj,QKeyEvent *_event);
    QWidget *get_stack_widget();
    bool is_back_car_show_mode();
private:

    void resizeEvent(QResizeEvent *e);

    void show_mainwindow();
    void show_16video();
    void show_play();
    void show_menu();
    void show_service_terams();
    //菜单下的按钮
    void show_menu_message();
    void show_menu_camera_query();
    void show_play_back();
    void show_menu_driver_menu();
    void show_menu_system_manager();
    void show_menu_run_status();
    void show_menu_about_machine();
    void show_shut_down();
    void show_menu_passageway();
    void show_menu_pay_info();
    void show_main_page2();
    void show_menu_passenger();
    void show_start_declaration();
    void show_start_setting();
    void show_start_state();


    void show_main_window_widget(char _id);
    void play_widget();

    void arrive_widget_init();

    QPushButton *m_button_test;
    QStackedWidget *m_stackwidget_main;

    CMainPage *m_CMainPage;
    CArriveWidget *m_CArriveWidget;
    CVideoWidget *m_CVideoWidget;
    CMenuWidget *m_CMenuWidget;

    CPayInfoWidget *m_CPayInfoWidget;
    CMessageWidget *m_CMessageWidget;
    CVideotapeWidget *m_CVideotapeWidget;
    CDriverMenuWidget *m_CDriverMenuWidget;
    CSystemManage *m_CSystemManage;
    CRunStatusWidget *m_CRunStatusWidget;
    CAboutWidget *m_CAboutWidget;
    CVideotapePlayback *m_CVideotapePlayback;
    CShundownDialog *m_CShundownDialog;
    CPassagewayWidget *m_CPassagewayWidget;
    CMainPage2 *m_CMainPage2;
    CPassengerWidget *m_CPassengerWidget;
    CDeclarationDialog *m_CDeclarationDiag;
    CSettingDialog *m_CSettingDialog;
    CStateDialog *m_CStateDialog;




    QTime *m_time_touch_reset;

    pthread_t thrd_msg_parse;

    char  m_current_widget_id;

    CMyDefineScreen *m_define_screen;

    bool m_is_system_enter;

};

#endif // MAINWINDOW_H







