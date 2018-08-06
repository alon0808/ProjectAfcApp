#ifndef HEAD_WIDGET_H
#define HEAD_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QMovie>

#include "class/common/define_screen.h"

#include "common_data.h"

enum
{
    kEnumHeadWidgetMainPage,        //主页 head
    kEnumHeadWidgetVideo,           //报站视频 head
    kEnumHeadWidgetMenu,            //菜单　head
    kEnumHeadWidgetSystemManage,    //系统管理
    kEnumHeadWidgetMessage,         //短信
    kEnumHeadWidgetCall,            //通话
    kEnumHeadWidgetAbout,           //关于本机
    kEnumHeadWidgetDriverMenu,       //司机菜单
    kEnumHeadWidgetRunStatus,        //运行状态
    kEnumHeadWidget16Video,           //16视频
    kEnumHeadWidgetPassengerFlow,    //客流
    kEnumHeadWidgetVideoPlayback,    //视频回放
    kEnumHeadWidgetVideotape,         //录像查询
    kEnumHeadWidgetPassageway,
    kEnumHeadWidgetPayInfo,
    kEnumHeadWidgetPassword,
    kEnumHeadWidgetPassenger
};


class CHeadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CHeadWidget(int _type,QWidget *parent = 0);
    

    void head_widget_notify(int _msg,int _event,void *_param);
private:
    void main_head_init();
    void main_head_init_d2();
signals:
    void signal_button_home();
    void signal_button_back();
    void signal_button_menu();
    void signal_button_video(int);
    void signal_pushbutton_passenger_flow();
    void signal_pushbutton_loop_playback(bool);
    void signal_pushbutton_quit();
public slots:
    void slot_1s_timer();

    void slot_pushbutton_1();
    void slot_pushbutton_2();
    void slot_pushbutton_3();
    void slot_pushbutton_4();
    void slot_pushbutton_passenger_flow();
    void slot_pushbutton_loop_playback();
    void slot_passenger_info(const CPassengerInfo & _info);
    void slot_main_head_home_back_show(bool _flag);
public:
    void set_head_widget_title(const QString &_text);
    void set_movie(bool _flag);
    bool get_movie();
private:
    void resizeEvent(QResizeEvent *e);
    void head_widget_button_home_init();
    void head_widget_button_back_init();
    void timer_background_init();
    void check_all_status();
    ////////////////////////////////////////////////////////头部显示
    QWidget *m_widget_head;
    QWidget *m_widget_head1;
    QLabel *m_label_network;            //2G 3G 4G
    QPushButton *m_pushbutton_signal;   //4G 信号
    QLabel *m_label_gps_number; //GPS个数
    QPushButton *m_pushbutton_gps;//GPS
    //QPushButton *m_pushbutton_acc_status;//ACC状态
    //QPushButton *m_pushbutton_device_door;//设备门（机器上）
    QPushButton *m_pushbutton_font_door;//前门
    QPushButton *m_pushbutton_back_door;//后门

    QPushButton *m_pushbutton_device_abnormal;//设备异常
    QPushButton *m_pushbutton_hard_disk_status;//硬盘状态
    QPushButton *m_pushbutton_platform_status;//平台连接状态 (调度平台)
    QPushButton *m_pushbutton_platform_status1;//平台连接状态 (视频服务器)
    //QPushButton *m_pushbutton_message;//消息
    QPushButton *m_pushbutton_sd_status;
    QPushButton *m_pushbutton_menu;

    QLabel *m_label_time; //时间显示
    QTimer *m_timer;

    QPushButton *m_pushbutton_home;
    QPushButton *m_pushbutton_back;
   // QPushButton *m_pushbutton_menu;
    QWidget *m_widget_head_1;
    QLabel *m_munu_title;

    int m_type;

    QPushButton *m_pushbutton_1;
    QPushButton *m_pushbutton_2;
    QPushButton *m_pushbutton_3;
    QPushButton *m_pushbutton_4;
    QPushButton *m_pushbutton_passenger_flow;
    QPushButton *m_pushbutton_loop_playback;

    QPushButton *m_pushbutton_quit;

    QLabel *m_label_total1;
    QLabel *m_label_total2;
    QLabel *m_label_up1;
    QLabel *m_label_up2;
    QLabel *m_label_down1;
    QLabel *m_label_down2;

    QMovie *m_movie_loop;
    QLabel *m_label_loop;
    bool m_movie_run;

    CMyDefineScreen *m_define_screen;

    bool m_status_4g;
    bool m_status_gps;
    bool m_status_harddisk;
    bool m_status_it;
    bool m_status_video;

};

#endif // HEAD_WIDGET_H














