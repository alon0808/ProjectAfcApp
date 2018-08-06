
#ifndef MAIN_PAGE2_H
#define MAIN_PAGE2_H

#include <QDialog>
#include <QLabel>
#include <QTimer>
#include "common_data.h"

class CDynTextParam;
class CDynamicText;


class showImage:public QWidget
{
public:
    showImage(QWidget *parent = 0);
    ~showImage();
    void set_image(const QImage &_image);
    void paintEvent(QPaintEvent *e);
private:
    QImage m_image;
};


class CMainPage2 : public QDialog
{
    Q_OBJECT
public:
    explicit CMainPage2(QWidget *parent = 0);
    static CMainPage2 *getInstance();
    enum
    {
        kEnumLeftRight, //界面左右
        kEnumUpDown     //界面上下
    };
    enum
    {
        kEnumPushbuttonStationName,  //站名显示
        kEnumLabelMessageShow,        //短信内容显示
        kEnumPushbuttonStationName1

    };
signals:
    void signal_button_menu();
    void signal_button_exit();
public slots:
    void slot_head_widget_notify(int _msg,int _event,void *_param);
    void slot_1s_timer();
    void slot_init_time_out();
    void slot_dynamic_text_event(const CDynTextParam &_param);
    void slot_qr_status_timer();
    void slot_main_page2_picture(const CPlayPicture & _data);
    void slot_pushbutton_test();
    void slot_button_menu_test();

public:
    void set_type(char _type);
    void debug_interface(bool _flag);
    bool get_debug_interface();
private:
    void main_page_left_right();
    void main_page_up_down();
    void main_page_demo();
    void set_message_content(const QString &_text);
    void left_right_init1();
    void show_failed_interface();
    QString get_week_string(int _week);
    void show_init_tips(bool _flag); //从导航界面启动时使用
    void req_tbp_status();
	void req_tmp_status();
    void req_sd_status();         // 首次进入画面主动获取ＳＤ状态
    void req_station_and_line(); //首次进入画面主动获取站名和线路

    void show_sd_status(int _event);
    /////////////////////////////////////////////////////////左右
    QLabel *m_label_network;            //2G 3G 4G
    QPushButton *m_pushbutton_signal;   //4G 信号
    QLabel *m_label_gps_number; //GPS个数
    QPushButton *m_pushbutton_gps;//GPS

	QLabel *m_label_tbp;            //tbp文字
    QPushButton *m_pushbutton_platform_status;//平台连接状态 (调度平台)

	QLabel *m_label_tmp;            //tmp文字
    QPushButton *m_pushbutton_platform_status1; //tmp图标
    
    QPushButton *m_pushbutton_sd_status;

    QLabel *m_label_time; //时间显示
    QLabel *m_label_time1; //时间显示
    QTimer *m_timer;

    showImage *m_widget_show_picture1;
    QWidget *m_widget_show_voice;
    QLabel *m_label_scroll_text;
    QWidget *m_widget_qr_status;
    QLabel *m_label_qr_text;
    QLabel *m_label_line_name;
    QLabel *m_label_direction;
    QLabel *m_label_station1;
    QLabel *m_label_station2;

    CDynamicText *m_dynamic_text_station_name;
    CDynamicText *m_dynamic_text_station_name1;
    CDynamicText *m_dynamic_text_message;

    //new
    QWidget *m_widget_bg;
    QLabel *m_label_charge_type;

    QLabel *m_label_ticket; //票价
    QLabel *m_label_ticket1; //金额
    QLabel *m_label_remaining_sum;  //余额
    QLabel *m_label_remaining_sum1; //余额1

    QLabel *m_label_arrive;//到站
    QLabel *m_label_arrive1;//到站

    QLabel *m_label_message_type;//刷卡消息类型
    QWidget *m_widget_message_bg;

    QLabel *m_m_label_station_status;
    QLabel *m_m_label_vehicle_number;

    QWidget *m_m_widget_debug_bg;
    QWidget *m_widget_init_bg;
    QLabel *m_label_debug;
    QLabel *m_label_init;

    QLabel *m_label_debug_tip;
    QWidget *m_widget_line1;
    QWidget *m_widget_line2;
    QWidget *m_widget_icon;
    QWidget *m_widget_init_icon;

    /////////////////////////////////////////////////////////上下
    QWidget *m_widget_bottom;
    QPushButton *m_pushbutton_menu;
    QPushButton *m_pushbutton_exit;
    QPushButton *m_pushbutton_test;
    QLabel *m_label_test_picture;

    QTimer *m_timer_qr;

    char m_type;
    char m_station_font_size;
    int m_station_show_length;
    QLabel *m_label_printf_test;
    bool m_is_demo;
    bool m_current_is_debug;
    QTimer *m_timer_init;
};

#endif // SYSTEM_MANAGE_H






