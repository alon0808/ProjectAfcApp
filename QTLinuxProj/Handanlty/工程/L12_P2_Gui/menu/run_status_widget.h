#ifndef RUN_STATUS_WIDGET_H
#define RUN_STATUS_WIDGET_H

#include <QDialog>

#include "head_widget.h"
#include "navigate_left.h"
#include "list_view/common_view/common_query_list.h"
#include <QStackedWidget>

#include "class/common/scrollcontroller.h"
#include "protocol.h"

#include "class/common/define_screen.h"

class CConnectDetails :public QWidget
{
    Q_OBJECT
public:
    CConnectDetails(QWidget *parent = 0);

public slots:

public:
    void set_title(const QString &_title);
private:
    void resizeEvent(QResizeEvent *e);
    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;

public:
    QLabel *m_label_line2_1;
    QLabel *m_label_line3_1;
    QLabel *m_label_line4_1;
    QLabel *m_label_line5_1;
    QLabel *m_label_line6_1;

    CMyDefineScreen *m_define_screen;

};

//1.移动网络
class CMobileNetworkWidget :public QWidget
{
    Q_OBJECT
public:
    CMobileNetworkWidget(QWidget *parent = 0);

public slots:
    void slot_ablout_machine_init();
    void slot_pushbutton_line6();
    void slot_pushbutton_line7();
    void slot_pushbutton_line8();
    void slot_pushbutton_return();
signals:
    void signal_window_close();
private:
    void resizeEvent(QResizeEvent *e);
     void scroll_layout();
     void widget_init();

    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_main;
    QWidget *m_widget_main1;
    QWidget *m_widget_main2;
    QWidget *m_widget_main3;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QWidget *m_widget_bg;
public:
    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;
    QLabel *m_label_line7;
    QLabel *m_label_line8;
    QLabel *m_label_line9;
    QLabel *m_label_line10;
    QLabel *m_label_line11;
    QLabel *m_label_line12;
    QLabel *m_label_line13;
    QLabel *m_label_line14;
    QLabel *m_label_line1_1;
    QLabel *m_label_line1_2;
    QLabel *m_label_line1_3;
    QLabel *m_label_line3_2;
    QLabel *m_label_line1_4;
    QLabel *m_label_line1_5;
    QLabel *m_label_line1_6;
    QLabel *m_label_line1_7;
    QLabel *m_label_line1_8;
    QLabel *m_label_line1_9;
    QLabel *m_label_line1_10;
    QLabel *m_label_line1_11;
    QLabel *m_label_line1_12;
    QLabel *m_label_line1_13;
    QLabel *m_label_line1_14;

    NavigateLeftButton *m_pushbutton_line6;
    NavigateLeftButton *m_pushbutton_line7;
    NavigateLeftButton *m_pushbutton_line8;

    CConnectDetails *mCConnectDetails1;
    CConnectDetails *mCConnectDetails2;
    CConnectDetails *mCConnectDetails3;

    CMyDefineScreen *m_define_screen;

};
//2.开关量
class CSwitchStatusWidget :public QWidget
{
    Q_OBJECT
public:
    CSwitchStatusWidget(QWidget *parent = 0);

public slots:
    void slot_ablout_machine_init();
private:
    void resizeEvent(QResizeEvent *e);
     void scroll_layout();
     void widget_init();

    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_main;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QWidget *m_widget_bg;

    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;
    QLabel *m_label_line7;
    QLabel *m_label_line8;
    QLabel *m_label_line9;
    QLabel *m_label_line10;
    QLabel *m_label_line11;
    QLabel *m_label_line12;
public:
    QLabel *m_label_line1_1;
    QLabel *m_label_line1_2;
    QLabel *m_label_line1_3;
    QLabel *m_label_line1_4;
    QLabel *m_label_line1_5;
    QLabel *m_label_line1_6;
    QLabel *m_label_line1_7;
    QLabel *m_label_line1_8;
    QLabel *m_label_line1_9;
    QLabel *m_label_line1_10;
    QLabel *m_label_line1_11;
    QLabel *m_label_line1_12;

    CMyDefineScreen *m_define_screen;

};
//3.定位模块
class CLocationModularWidget :public QWidget
{
    Q_OBJECT
public:
    CLocationModularWidget(QWidget *parent = 0);

public slots:
    void slot_ablout_machine_init();
private:
    void widget_init();
    void scroll_layout();

    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_main;

    int m_scroll_widget_width;
    int m_scroll_widget_height;


    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;
    //add
    QLabel *m_label_line7;
    QLabel *m_label_line8;
    QLabel *m_label_line9;
    QLabel *m_label_line10;
    QLabel *m_label_line11;
    QLabel *m_label_line12;
    QLabel *m_label_line13;
    QLabel *m_label_line14;
    QLabel *m_label_line15;
    QLabel *m_label_line16;
    QLabel *m_label_line17;
    QLabel *m_label_line18;
    QLabel *m_label_line19;
public:
    void resizeEvent(QResizeEvent *e);


    QLabel *m_label_line1_1;
    QLabel *m_label_line1_2;
    QLabel *m_label_line1_3;
    QLabel *m_label_line1_4;
    QLabel *m_label_line1_5;
    QLabel *m_label_line1_6;
    //add
    QLabel *m_label_line1_7;
    QLabel *m_label_line1_8;
    QLabel *m_label_line1_9;
    QLabel *m_label_line1_10;
    QLabel *m_label_line1_11;
    QLabel *m_label_line1_12;
    QLabel *m_label_line1_13;
    QLabel *m_label_line1_14;
    QLabel *m_label_line1_15;
    QLabel *m_label_line1_16;
    QLabel *m_label_line1_17;
    QLabel *m_label_line1_18;
    QLabel *m_label_line1_19;

    QWidget *m_widget_bg;
    CMyDefineScreen *m_define_screen;

};
//4.本地网络
class CLocalNetworkWidget :public QWidget
{
    Q_OBJECT
public:
    CLocalNetworkWidget(QWidget *parent = 0);

public slots:
    void slot_ablout_machine_init();


private:
    void resizeEvent(QResizeEvent *e);
     void scroll_layout();
     void widget_init();

    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_main;
    QWidget *m_widget_main1;


    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QWidget *m_widget_bg;

    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;
    QLabel *m_label_line7;
    QLabel *m_label_line8;
    QLabel *m_label_line9;
    QLabel *m_label_line10;
public:
    QLabel *m_label_line1_1;
    QLabel *m_label_line1_2;
    QLabel *m_label_line1_3;
    QLabel *m_label_line1_4;
    QLabel *m_label_line1_5;
    QLabel *m_label_line1_6;
    QLabel *m_label_line1_7;
    QLabel *m_label_line1_8;
    QLabel *m_label_line1_9;
    QLabel *m_label_line1_10;

    CMyDefineScreen *m_define_screen;

};
//5.设备状态
class CDeviceStatusWidget :public QWidget
{
    Q_OBJECT
public:
    CDeviceStatusWidget(QWidget *parent = 0);

public slots:
    void slot_ablout_machine_init();

    void slot_button_serial_port_status();
    void slot_button_can_status();
    void slot_pushbutton_return();
signals:
    void signal_window_close();

private:
    void resizeEvent(QResizeEvent *e);
     void scroll_layout();
     void widget_init();
     void serial_port_init();
     void can_init();

    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_main;
    QWidget *m_widget_main1;
    QWidget *m_widget_main2;


    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QWidget *m_widget_bg;

    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;
public:
    QLabel *m_label_line1_1;
    QLabel *m_label_line1_2;
    QLabel *m_label_line1_3;
    QLabel *m_label_line1_4;
    QLabel *m_label_line1_5;
    QLabel *m_label_line1_6;
    NavigateLeftButton *m_button_line7;
    NavigateLeftButton *m_button_line8;

    QLabel *m_label_main1_line1;
    QLabel *m_label_main1_line2;
    QLabel *m_label_main1_line2_1;

    QLabel *m_label_main1_line3;
    QLabel *m_label_main1_line3_1;

    QLabel *m_label_main1_line4;
    QLabel *m_label_main1_line4_1;

    QLabel *m_label_main1_line5;
    QLabel *m_label_main1_line5_1;

    QLabel *m_label_main1_line6;
    QLabel *m_label_main1_line6_1;

    QLabel *m_label_main2_line1;
    QLabel *m_label_main2_line2;
    QLabel *m_label_main2_line2_1;

    QLabel *m_label_main2_line3;
    QLabel *m_label_main2_line3_1;

    CMyDefineScreen *m_define_screen;

};
//外设(L12)
class CPeripheralWidget :public QWidget
{
    Q_OBJECT
public:
    CPeripheralWidget(QWidget *parent = 0);

public slots:

    void slot_pushbutton_return();
    void slot_peripheral_init();
signals:
    void signal_window_close();

private:
    void resizeEvent(QResizeEvent *e);
     void scroll_layout();
     void widget_init();


    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_main;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QWidget *m_widget_bg;


    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;
    QLabel *m_label_line7;
    QLabel *m_label_line8;
    QLabel *m_label_line9;
    QLabel *m_label_line10;

public:
    QLabel *m_label_line1_1;
    QLabel *m_label_line2_1;
    QLabel *m_label_line3_1;
    QLabel *m_label_line4_1;
    QLabel *m_label_line5_1;
    QLabel *m_label_line6_1;
    QLabel *m_label_line7_1;
    QLabel *m_label_line8_1;
    QLabel *m_label_line9_1;
    QLabel *m_label_line10_1;

    CMyDefineScreen *m_define_screen;

};


#if 1
//6.站点信息
class CStationInfoWidget :public QWidget
{
    Q_OBJECT
public:
    CStationInfoWidget(QWidget *parent = 0);

public slots:

    void slot_pushbutton_return();
signals:
    void signal_window_close();

private:
    void resizeEvent(QResizeEvent *e);
     void scroll_layout();
     void widget_init();


    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_main;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QWidget *m_widget_bg;


    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
public:
    QLabel *m_label_line1_1;
    QLabel *m_label_line2_1;
    QLabel *m_label_line3_1;
    QLabel *m_label_line4_1;
    QLabel *m_label_line5_1;

    CMyDefineScreen *m_define_screen;

};
#endif
class CRunStatusWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CRunStatusWidget(QWidget *parent = 0);
    
signals:
    void signal_set_default_id(int);
    void signal_mobile_network_user_return();
    void signal_device_status_user_return();
    void signal_pushbutton_home();
    void signal_pushbutton_menu();
public slots:
    void slot_pushbutton_home();
    void slot_pushbutton_back();
    void slot_run_status_widget_event(int _type,QString _text);
    void slot_pushbutton_close();

    void slot_run_status_widget_event(const CRunStatusInfo &_runStatusInfo);

    void slot_pushbutton_up();
    void slot_pushbutton_down();

public:
    bool goto_main_page() {return m_goto_main_page;}
private:
    void resizeEvent(QResizeEvent *e);
    void show_mobile_network();
    void show_switch_status();
    void show_location_modular();
    void show_local_network();
    void show_device_status();
    void show_peripheral();
    void show_station_info();

    CHeadWidget *m_CHeadWidget;
    QWidget *m_widget_bg1;
    QLabel *m_label_about;
    QWidget *m_widget_bg2;

    CNavigateLeft *m_CNavigateLeft;

    QStackedWidget *m_stackwidget_navigate_right;

    CMobileNetworkWidget *m_CMobileNetworkWidget;       //1.移动网络
    CSwitchStatusWidget *m_CSwitchStatusWidget;         //2.开关量
    CLocationModularWidget *m_CLocationModularWidget;   //3.定位模块
    CLocalNetworkWidget *m_CLocalNetworkWidget;         //4.本地网络
    CDeviceStatusWidget *m_CDeviceStatusWidget;         //5.设备
    CPeripheralWidget *m_CPeripheralWidget;
    CStationInfoWidget *m_CStationInfoWidget;           //6.站点信息

    QPushButton *m_pushbutton_up;
    QPushButton *m_pushbutton_down;

    char m_current_index;
    bool m_goto_main_page;

    CMyDefineScreen *m_define_screen;

};














#endif // RUN_STATUS_WIDGET_H
