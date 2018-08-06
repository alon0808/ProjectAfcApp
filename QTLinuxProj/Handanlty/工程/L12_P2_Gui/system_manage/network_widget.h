#ifndef NETWORK_WIDGET_H
#define NETWORK_WIDGET_H

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>

#include "class/common/scrollcontroller.h"
#include "class/common/navigateleftbutton.h"
#include "class/common/my_pushbutton.h"
#include "protocol.h"

#include "class/common/define_screen.h"

class CNetworkInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CNetworkInfo(char _type,QWidget *parent = 0);
    enum
    {
        kEnumMoveNetwork,   //移动网络
        kEnumCenterServer1, //中心服务器1
        kEnumCenterServer2, //中心服务器2
        kEnumVideoServer,   //视频服务器
        kEnumLocalNetwork,  //本地网络
        kEnumWifi,          //WIFI
        kEnumFtpServer      //FTP服务器
    };
signals:
    void signal_button_line2();
    void signal_button_wifi_search();
public slots:
    void slot_checkbox_changed_line6(int);
    void slot_m_combobox_changed_line3(int);
private:
    void resizeEvent(QResizeEvent *e);
    void Network_info_init(char _type);
    void network_wifi_init(char _type);//网络和WIFI共用一份数据
    void move_network_init();   //移动网络
    void center_server1_init(); //中心服务器1
    void center_server2_init(); //中心服务器2
    void video_server_init();   //视频服务器
    void local_network_init(char _type);  //本地网络
    void wifi_init(char _type);          //WIFI
    void ftp_server_init();      //FTP服务器

    char m_type;
    QWidget *m_widget_bg;
public:
    QLabel *m_label_line1;
    //CMyPushButton *m_pushbutton_line1;
    QCheckBox *m_checkbox_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line5;
    QLabel *m_label_line6;
    QLabel *m_label_line7;
    QLabel *m_label_line8;
    QLabel *m_label_line9;

    //移动网络
    NavigateLeftButton *m_button_line2;
    QComboBox *m_combobox_line3;
    QComboBox *m_combobox_line4;
    QComboBox *m_combobox_line5;
    //CMyPushButton *m_pushbutton_line6;
    QCheckBox *m_checkbox_line6;
    QLineEdit *m_lineedit7;
    QLineEdit *m_lineedit8;
    QLineEdit *m_lineedit9;

    //中心服务器1 //中心服务器2
    QComboBox *m_combobox_line2;
    QLineEdit *m_lineedit3;
    QLineEdit *m_lineedit4;
    QLineEdit *m_lineedit5;
    QLabel *m_label_line5_1;

    //视频服务器
    //CMyPushButton *m_pushbutton_line2;
    QPushButton *m_pushbutton_line2_1;

    //本地网络
    QLineEdit *m_lineedit6;
    QCheckBox *m_checkbox_line2;
    QLabel *m_label_line6_1;

#if 0
    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line2_1;
    QLabel *m_label_line2_2;
    QLabel *m_label_line3;
    QLabel *m_label_line3_1;
    QWidget *m_widget_bg;

    CMyPushButton *m_pushbutton_enable;
    QPushButton *m_pushbutton_ok;
#endif

    CMyDefineScreen *m_define_screen;

};

class CNetworkSelect;
class CWifiSearch;
class CNetworkWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CNetworkWidget(QWidget *parent = 0);

signals:
    void signal_window_close();

public slots:
    void slot_pushbutton_save();
    void slot_pushbutton_reset();
    void slot_timer_init();

    void slot_button_1();
    void slot_button_2();
    void slot_button_3();
    void slot_button_4();
    void slot_button_5();
    void slot_button_6();
    void slot_button_7();

    void slot_button_model_type();
    void slot_button_wifi_search(); //wifi搜索
    void slot_pushbutton_return();

    void slot_network_widget_event(const CSystemNetwork & _info);
public:
    void save_data();
    void set_focus_first();
private:
    void resizeEvent(QResizeEvent *e);
    void scroll_layout();
    void network_init_new();
    void model_type_init();

    void read_data();

    bool networkinfo1_change();
    bool networkinfo2_change();
    bool networkinfo3_change();
    bool networkinfo4_change();
    bool networkinfo5_change();
    bool networkinfo6_change();
    bool networkinfo7_change();

/////////////////////////////////////////////////////start
    NavigateLeftButton *m_button_1;
    NavigateLeftButton *m_button_2;
    NavigateLeftButton *m_button_3;
    NavigateLeftButton *m_button_4;
    NavigateLeftButton *m_button_5;
    NavigateLeftButton *m_button_6;
    NavigateLeftButton *m_button_7;

    bool m_button_press_1;
    bool m_button_press_2;
    bool m_button_press_3;
    bool m_button_press_4;
    bool m_button_press_5;
    bool m_button_press_6;
    bool m_button_press_7;

    QLabel *m_label_space1;
    QLabel *m_label_space2;
    QLabel *m_label_space3;
    QLabel *m_label_space4;
    QLabel *m_label_space5;
    QLabel *m_label_space6;

    QVBoxLayout *m_vboxlayout_network;

    CNetworkInfo *m_NetworkInfo1;
    CNetworkInfo *m_NetworkInfo2;
    CNetworkInfo *m_NetworkInfo3;
    CNetworkInfo *m_NetworkInfo4;
    CNetworkInfo *m_NetworkInfo5;
    CNetworkInfo *m_NetworkInfo6;
    CNetworkInfo *m_NetworkInfo7;

    QLabel *m_label_main1_line1;
    QLabel *m_label_main1_line2;
    QLabel *m_label_main1_line2_1;

    QLabel *m_label_main1_line3;
    QLabel *m_label_main1_line3_1;

    QLabel *m_label_main1_line4;
    QLabel *m_label_main1_line4_1;

    QLabel *m_label_main1_line5;
    QLabel *m_label_main1_line5_1;


/////////////////////////////////////////////////////end
    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_bg;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    //QScrollArea *m_scrollarea_1;
    QWidget *m_widget_main;
    QWidget *m_widget_main1;
    QLabel *m_label_test;

    QLabel *m_label_line1;
    QLabel *m_label_line1_1;

    QLabel *m_label_line2;
    QComboBox *m_combobox_line2_1;
    QLabel *m_label_line2_1;
    QComboBox *m_combobox_line2_2;

    QLabel *m_label_line3;
    QComboBox *m_combobox_line3_1;
    QLabel *m_label_line3_1;
    QComboBox *m_combobox_line3_2;

    QLabel *m_label_line4;
    QLineEdit *m_lineedit4;
    QLabel *m_label_line4_1;
    QComboBox *m_combobox_line4_1;

    QLabel *m_label_line5;
    QLineEdit *m_lineedit5;
    QLabel *m_label_line5_1;
    QComboBox *m_combobox_line5_1;

    QLabel *m_label_line6;
    QLineEdit *m_lineedit6;
    QLabel *m_label_line6_1;
    QComboBox *m_combobox_line6_1;


    QLabel *m_label_line7;
    QLineEdit *m_lineedit7;
    QLabel *m_label_line7_1;
    QLineEdit *m_lineedit7_1;

    QLabel *m_label_line8;
    QLineEdit *m_lineedit8;

    QLabel *m_label_line9;
    QLineEdit *m_lineedit9;

    QLabel *m_label_line10;
    QLabel *m_label_line10_1;

    QLabel *m_label_line11;
    QComboBox *m_combobox_line11_1;
    QLabel *m_label_line11_1;
    QComboBox *m_combobox_line11_2;

    QLabel *m_label_line12;
    QComboBox *m_combobox_line12_1;
    QLabel *m_label_line12_1;
    QComboBox *m_combobox_line12_2;

    QLabel *m_label_line13;
    QLineEdit *m_lineedit13_1;
    QLabel *m_label_line13_1;
    QComboBox *m_combobox_line13_1;

    QLabel *m_label_line14;
    QLineEdit *m_lineedit14_1;
    QLabel *m_label_line14_1;
    QLineEdit *m_lineedit14_2;

    QLabel *m_label_line15;
    QLineEdit *m_lineedit15_1;
    QLabel *m_label_line15_1;
    QComboBox *m_combobox_line15_1;

    QLabel *m_label_line16;
    QLineEdit *m_lineedit16_1;
    QLabel *m_label_line16_1;
    QLineEdit *m_lineedit16_2;

    QLabel *m_label_line17;
    QLineEdit *m_lineedit17_1;
    QLabel *m_label_line17_1;
    QLineEdit *m_lineedit17_2;

    QLabel *m_label_line18;
    QLabel *m_label_line18_1;
    QLineEdit *m_lineedit18_2;

    QLabel *m_label_line19;
    QLineEdit *m_lineedit19_1;
    QLabel *m_label_line19_1;
    QLineEdit *m_lineedit19_2;

    QLabel *m_label_line20;
    QLineEdit *m_lineedit20_1;
    QLabel *m_label_line20_1;
    QLineEdit *m_lineedit20_2;

    QPushButton *m_pushbutton_save;
    QPushButton *m_pushbutton_reset;

    CNetworkSelect *m_CNetworkSelect;

    CMyDefineScreen *m_define_screen;

};

#endif // NETWORK_WIDGET_H
