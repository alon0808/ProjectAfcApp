#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QEvent>
#include <QTime>

#include "head_widget.h"

#include "steering_wheel.h"

#include "dynamic_text.h"
#include "./class/cJSON.h"

#include "class/common/my_mouse_event.h"
#include "class/common/navigateleftbutton.h"

#include "protocol.h"
#include "list_view/common_view/common_query_mode.h"
//class CCommonQueryMode;


class CMainPage : public QDialog //QWidget
{
    Q_OBJECT
public:
    explicit CMainPage(QWidget *parent = 0);
    static CMainPage *getInstance();

    enum
    {
        kEnumPushbuttonStationName,  //站名显示
        kEnumLabelMessageShow        //短信内容显示

    };
signals:
    void signal_main_page_message(unsigned int _addr);
    void signal_reset_touch_event();

    void signal_main_page_message_upload();
    void signal_main_page_run_status();

    void signal_main_page_arrive_widget_event(int _type,int _param1,QString _param2);
    void signal_touch_mouse_event(int _x,int _y,bool _flag);

    void signal_voice_dialog_event(int _value1,int _value2,int _value3);
    void signal_driver_menu_light_event(int _value);

    void signal_vidoe_widget_event(char _frame,char _number,bool _output);
    void signal_arrive_widget_notify_event(char _frame,char _number,bool _output);
    //关于本机
    void signal_about_machine_widget_event(const CAboutMachineInfo &_info);
    //运行状态
    void signal_run_status_widget_event(const CRunStatusInfo &_runStatusInfo);
    //司机菜单-高级设置
    void signal_driver_menu_widget_event(const CDriverMenuInfo &_driverMenuInfo);
    //消息
    void signal_message_widget_notify_event(int _type,CCommonQueryMode _mode);
    //系统管理-注册信息
    void signal_register_info_widget_event(const CSystemRegisterInfo &_registerInfo);
    //系统管理-坐标采集
    void signal_coordinate_collection_widget_event(char _type,const CSystemCoordinateCollection &_info);
    //系统管理-录像查询
    void signal_videotape_query_widget_event(const CSystemVideotapeQueryEx &_info);
    void signal_menu_videotape_query_widget_event(const CSystemVideotapeQueryEx &_info);
    //系统管理-录像查询-回放
    void signal_videotape_query_playback(int);
    //系统管理-存储管理
    void signal_storage_manage_widget_event(const CSystemStorageManage &_info);
    //菜单-系统管理-网络
    void signal_network_widget_event(const CSystemNetwork & _info);
    //菜单-系统管理-网络-WIFI
    void signal_network_select(char _type,const CWifiSearch &_param);
    //菜单-系统管理-I/O配置
    void signal_switch_config_widget_event(const CSystemIoConfig &_info);
    //菜单-系统管理-高级设置
    void signal_senior_setup_widget_event(char _type,const CSystemSeniorSetup &_info);
    //菜单-系统管理-系统设置
    void signal_system_setup_widget_event(const CSystemSystemSetup &_info);
    //菜单-系统管理-系统维护
    void signal_system_maintain_widget_event(char _type,const CSystemSystemMaintain &_info);
    //菜单-系统管理-CAM配置
    void signal_camera_config_widget_event(const CSystemCameraConfig &_info);
    //菜单-系统管理-CAM配置-PON CAM
    void signal_camera_config_widget_notify(char _type,char _cam_type,char _number);
    //菜单-系统管理-客流统计
    void signal_passenger_flow_widget_event(char _type,void *_param);
    void signal_passenger_info(const CPassengerInfo & _info);
    void signal_pos_config_widget_event(const CSystemPosSetup &_info);

    void signal_storage_manage_widget_event_ex(char _type,void *_param);
    void signal_main_page2_picture(const CPlayPicture & _data);
//////////////////////
    void signal_pushbutton_play();          //播放
    void signal_pushbutton_16_frame();      //16分频
    void signal_pushbutton_service_terams();//服务用语
    void signal_pushbutton_menu();          //菜单
    void signal_mainwindow_show(bool _show);

    void signal_pushbutton_back();
    void signal_main_page_notify(int _msg,int _event,void *_param);

    void signal_driver_enter_station(bool _flag);
    void signal_paycard_record_widget(const CGetQrPayInfo & _info);
    void signal_back_car_notify(bool);
    void signal_playback_quit();
    void signal_playback_notify_event(char _play_type,char _value);
    void signal_pushbutton_passenger();
    void signal_passenger_widget(const CGetPassengerInfo &_info);
    void signal_network_status(int type, const CRunStatusInfo &_runStatusInfo);
    void signal_tmp_status(int type, int status, int errorType);
    void signal_alg_image_update(int type);
    void signal_show_main_page(bool state);
    void signal_g3_network_status(int type, int status);
public slots:
    void slot_pushbutton_back();
    void slot_pushbutton_test();

    void slot_pushbutton_play();//播放
    void slot_pushbutton_replay();//重播
    void slot_pushbutton_service_terams();//服务用语
    void slot_pushbutton_menu();//菜单
    void slot_pushbutton_16_frame();//16画面分
    void slot_pushbutton_message_upload();//短信上传事件
    void slot_pushbutton_run_status();//查看设备运行状态

    //void slot_text_roll_timer(); //显示文字滚动

    void slot_pushbutton_station_name();//点击站名，弹出设置对话框
    void slot_dynamic_text_event(const CDynTextParam &_param);

    void slot_pushbutton_door_1();
    void slot_pushbutton_door_2();
    int slot_main_page_message(unsigned int addr);
    void slot_reset_touch_event();

    void slot_pushbutton_arrow_left();
    void slot_pushbutton_arrow_right();

    void slot_pushbutton_bianma3();
    void slot_touch_mouse_event(int _x,int _y,bool _flag);

    void slot_timer_vehicle();

    void slot_timer_test();
    void slot_timer_key_5s();
    void slot_timer_collection_data();
    void slot_timer_station();

public:
    //oid recvNewMessage(unsigned char *);
    void reset_touch_timer();
    bool current_first_press();
    bool handle_mouse_event(int _x,int _y,bool _flag);
    void main_page_info_init();
    bool keyboard_value(int);
    void gui_system_key_up(cJSON *root,bool _monitor = false,int _value = 0);
private:

    void gui_system_touch(cJSON *root);
    void gui_system_gps_info(cJSON *root);
    void gui_system_gps_db(cJSON *root);
    void gui_system_itssd_notify_info(cJSON *root);
    void gui_system_notify_station(cJSON *root);
    void gui_system_direction_change(cJSON *root);
    void gui_system_g3_modem_status(cJSON *root);
    void gui_system_itsnw_connect_status(cJSON *root);
    void gui_system_stmdev_connect_status(cJSON *root);
    void gui_system_tbp_connect_status(cJSON *root);
    void gui_system_exception_overspeed(cJSON *root);
    void gui_system_exception_rate_limit(cJSON *root);
    void gui_system_itsio_io_status(cJSON *root);
    void gui_system_hdd_status(cJSON *root);
    void gui_system_userin_card(cJSON *root);
    void gui_system_its_scheluing_plan(cJSON *root);
    void gui_system_its_task_info(cJSON *root);
    void gui_system_station_type(cJSON *root);
    void gui_system_shutdown(cJSON *root);
    void gui_system_exception_analog(cJSON *root);
    void gui_system_working_status(cJSON *root);
    void gui_system_itssd_para(cJSON *root);
    void gui_system_av_videoloss(cJSON *root);
    void gui_voice(cJSON *root);
    void gui_light(cJSON *root);
    void gui_videotape_video(cJSON *root);
    void gui_about_machine(cJSON *root);
    void gui_run_status(cJSON *root);
    void gui_driver_menu(cJSON *root);
    void gui_message(cJSON *root);
    void gui_system_register_info(cJSON *root);
    void gui_system_coordinate_collection(cJSON *root);
    void gui_system_videotape_query(cJSON *root);
    void gui_system_storage_manage(cJSON *root);
    void gui_system_network(cJSON *root);
    void gui_system_wifi(cJSON *root);
    void gui_system_switch_config(cJSON *root);
    void gui_system_senior_setup(cJSON *root);
    void gui_system_system_setup(cJSON *root);
    void gui_system_maintain(cJSON *root);
    void gui_system_camera_config(cJSON *root);
    void gui_system_camera_pon_cam(cJSON *root);
    void gui_system_passenger_flow(cJSON *root);
    void gui_passenger_flow_info(cJSON *root);
    void gui_sdcard_format_progress(cJSON *root);
    void gui_system_bus_interval(cJSON *root);
    void gui_login_info(cJSON *root);
    void gui_two_dimensional_code_info(cJSON *root);
    void gui_picture_play(cJSON *root);
    void gui_get_qr_pay_info(cJSON *root);
    void gui_system_key_up_4(cJSON *root);
    void gui_system_io_raw(cJSON *root);
    void gui_main_page(cJSON *root);
    void gui_get_flash_card_info(cJSON *root);
    void gui_system_reboot(cJSON *root);
    void gui_system_pos_cofnig(cJSON *root);
    void gui_system_heartbeat_notify(cJSON *root);
    void gui_mouse_info(cJSON *root);
    void gui_test_heartbeat(cJSON *root);
    void gui_g3_network_status(cJSON *root);
    void gui_tmp_connect_status(cJSON *root);
    void gui_starting_screen_flag(cJSON *root);
    void gui_tmp_status(cJSON *root);
    void gui_alg_image_update(cJSON *root);
    //void customEvent(QEvent *e);

    void main_signal_slot_init();
    void main_middle_init();
    void main_middle_init_new();
    void main_bottom_init();
    void init_new_button(QPushButton *_button,int _x,int _y,int _w,int _h,QString _typeSheet = "");
    void modify_button_sheet(QPushButton *_button,QString &_typeSheet);
    void main_page_notify(int _msg,int _event,void *_param);

    void set_current_station_name(QString _name);

    int main_page_handle(cJSON *root);
    void quit_interface();



    ////////////////////////////////////////////////////////中间左边显示

    QWidget *m_widget_middle1; //多点站背景图
    QWidget *m_widget_vehicle1; //车1
    QWidget *m_widget_vehicle1_1; //车1_1
    QWidget *m_widget_vehicle2; //车2
    QWidget *m_widget_vehicle2_1; //车2_1
    QWidget *m_widget_vehicle3; //车3

    QWidget *m_widget_bianma1; //车的编号1
    QLabel *m_label_bianma1; //车的编号2

    QWidget *m_widget_line1;
    QLabel *m_label_bianma2;
    QWidget *m_widget_line2;
    //QLabel *m_label_bianma3;
    QPushButton *m_pushbutton_bianma3;

    CHeadWidget *m_CHeadWidget;

    ////////////////////////////////////////////////////////////////站台信息start
    QWidget *m_widget_middle2; //显示到站背景
    QLabel *m_label_local_station; //本站



    QLabel *m_label_left_distance; //左边距离
    QLabel *m_label_right_distance; //右边距离
    QLabel *m_label_left_distance1; //左边距离(泰国版才用到)
    QLabel *m_label_right_distance1; //右边距离(泰国版才用到)



    ////////////////////////////////////////////////////////////////站台信息end

    QWidget *m_widget_middle3;//显示普通短信背景
    QLabel *m_label_message_content;//显示短信内容

    QLabel *m_label_next_start_time1;   //下一趟发车时间1
    QLabel *m_label_next_start_time2;   //下一趟发车时间2

    QWidget *m_widget_up_vehicle;       //上车
    QWidget *m_widget_down_vehicle;     //下车
    QWidget *m_widget_within_vehicle;   //车内



    QLabel *m_label_driver;     //驾驶员
    QLabel *m_label_driver_name;//名字

    QWidget *m_widget_line;

    QLabel *m_label_vehicle1;     //车版编号1
    QLabel *m_label_vehicle2;     //车版编号2

    CSteeringWheel *m_CSteeringWheel;


    ////////////////////////////////////////////////////////中间显示(new)
    QWidget *m_widget_middle_left;
    QWidget *m_widget_middle_right;

    ///////////////////////////////////图一　start
    QWidget *m_widget_front_vehicle;
    QLabel *m_label_front_vehicle1;
    QLabel *m_label_front_vehicle2;

    QWidget *m_widget_back_vehicle;
    QLabel *m_label_back_vehicle1;
    QLabel *m_label_back_vehicle2;
    ///////////////////////////////////图一　end

    ///////////////////////////////////图二　start
    QWidget *m_widget_station_bg;     //5个站点背景
    QLabel *m_label_front_station_1; //上一个站1
    QLabel *m_label_front_station_2;//上一个站2

    QLabel *m_label_back_station_1;//下一个站1
    QLabel *m_label_back_station_2;//下一个站2

    QLabel *m_label_vehicle_1; //前车
    QLabel *m_label_vehicle_1_distance;//前车到下站的距离
    QLabel *m_label_vehicle_2;//当前车
    QLabel *m_label_vehicle_2_distance;//当前车到下站的距离
    QLabel *m_label_vehicle_3;//后车
    QLabel *m_label_vehicle_3_distance;//后车到下站的距离


    QLabel *m_label_station_status;

    ///////////////////////////////////图二　end


    QPushButton *m_pushbutton_arrow_left; //箭头左
    QPushButton *m_pushbutton_arrow_right;//箭头右
    QPushButton *m_pushbutton_station_name;//站台名称
    QString m_current_station_name;     //当前站名

    ////////////////////////////////////////////////////////底部显示
    QWidget *m_widget_bottom;
    QPushButton *m_pushbutton_play;//播放
    QPushButton *m_pushbutton_replay;//重播
    QPushButton *m_pushbutton_service_terams;//服务用语
    QPushButton *m_pushbutton_menu;//菜单
    QPushButton *m_pushbutton_16_frame;//16画面分

    QLabel *m_label_up_vehicle1;
    QLabel *m_label_up_vehicle2;

    QLabel *m_label_down_vehicle1;
    QLabel *m_label_down_vehicle2;

    QLabel *m_label_within_vehicle1;
    QLabel *m_label_within_vehicle2;

    QLabel *m_label_limit_speed1;
    QLabel *m_label_limit_speed2;

    QLabel *m_label_current_speed;


    QPushButton *m_pushbutton_back;
    QPushButton *m_pushbutton_test;

    CDynamicText *m_dynamic_text_station_name;
    CDynamicText *m_dynamic_text_message;

    QTimer *m_timer_test;
    QTimer *m_timer_vehicle;
    QTimer *m_timer_station;

    pthread_t thrd_msg_parse;

    QTime *m_time_touch_reset;

    QLabel *m_label_test_picture;

    QTimer *m_timer_key_5s;
    int m_key_times;
    int m_key_back_times;
    int net_type ;

    char m_current_key_event_type;

};

#endif // MAIN_PAGE_H











