#ifndef SYSTEM_SETUP_WIDGET_H
#define SYSTEM_SETUP_WIDGET_H

#include <QWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>

#include "class/common/my_pushbutton.h"
#include "class/common/scrollcontroller.h"
#include "class/common/navigateleftbutton.h"

#include "protocol.h"

#include "class/common/define_screen.h"

class CSystemSetupInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CSystemSetupInfo(int _type,QWidget *parent = 0);
    enum
    {
        kEnumCommonConfig,  //通用配置
        kEnumSystemTime,    //系统时间
        kEnumAccountSetting //账户设置
    };
signals:

private:
    void common_config_init();  //通用配置
    void system_time_init();    //系统时间
    void account_setting_init(); //账户设置
    void set_type(char _type);
    char m_type;

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
public:
    void resizeEvent(QResizeEvent *e);
    //通用配置
    QComboBox *m_combobox_line1;
    //CMyPushButton *m_pushbutton_line3;
    QCheckBox *m_checkbox_line3;
    QLineEdit *m_lineedit3;
    QLabel *m_label_line3_1;
    //CMyPushButton *m_pushbutton_line4;
    QCheckBox *m_checkbox_line4;
    QLineEdit *m_lineedit4;
    QLabel *m_label_line4_1;
    //CMyPushButton *m_pushbutton_line5;
    QCheckBox *m_checkbox_line5;
    QLabel *m_label_line5_1;
    QLineEdit *m_lineedit5;
    QLabel *m_label_line5_2;
    //CMyPushButton *m_pushbutton_line6;
    QCheckBox *m_checkbox_line6;
    QLineEdit *m_lineedit6_1;
    QLabel *m_label_line6_1;
    QLineEdit *m_lineedit6_2;
    QLabel *m_label_line6_2;

    //CMyPushButton *m_pushbutton_line7;
    QCheckBox *m_checkbox_line7;
    QLineEdit *m_lineedit7_1;
    QLabel *m_label_line7_1;
    QLineEdit *m_lineedit7_2;
    QLabel *m_label_line7_2;
    QLineEdit *m_lineedit7_3;
    QLabel *m_label_line7_3;

    //CMyPushButton *m_pushbutton_line8;
    QCheckBox *m_checkbox_line8;
    QLineEdit *m_lineedit8_1;
    QLabel *m_label_line8_1;
    QLineEdit *m_lineedit8_2;
    QLabel *m_label_line8_2;
    QLineEdit *m_lineedit8_3;
    QLabel *m_label_line8_3;

    QCheckBox *m_checkbox_line9;
    QLabel *m_label_line9_0;
    QLineEdit *m_lineedit9_1;
    QLabel *m_label_line9_1;
    QLineEdit *m_lineedit9_2;
    QLabel *m_label_line9_2;


    QLineEdit *m_lineedit10_2;
    QLabel *m_label_line10_2;

    QComboBox *m_combobox_line11;

    //系统时间
    QLineEdit *m_lineedit2_1;
    QComboBox *m_combobox_line5;
    QLineEdit *m_lineedit2;
    QLabel *m_label_line2_1;

    //账户设置
    //CMyPushButton *m_pushbutton_line1;
    QCheckBox *m_checkbox_line1;
    QComboBox *m_combobox_line2;
    QComboBox *m_combobox_line3;
    //CMyPushButton *m_pushbutton_line2;
    QCheckBox *m_checkbox_line2;

    CMyDefineScreen *m_define_screen;

};

class CSystemSetupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSystemSetupWidget(QWidget *parent = 0);
    
signals:
    
public slots:

   // void slot_pushbutton_save();//保存
   // void slot_pushbutton_reset();//重置

    void slot_button_1();
    void slot_button_2();
    void slot_button_3();

    void slot_system_setup_widget_event(const CSystemSystemSetup &_info);

public:
    void update_system_time_show();
private:
    void system_setup_init_new();
public:
    void save_data();

private:
    void resizeEvent(QResizeEvent *e);
    void scroll_layout();
    void read_data();


//////////////////////////////////////////new start
    NavigateLeftButton *m_button_1;
    NavigateLeftButton *m_button_2;
    NavigateLeftButton *m_button_3;
    QVBoxLayout *m_vboxlayout_storage;

    bool m_button_press_1;
    bool m_button_press_2;
    bool m_button_press_3;

    CSystemSetupInfo *m_system_info1;
    CSystemSetupInfo *m_system_info2;
    CSystemSetupInfo *m_system_info3;

    QLabel *m_label_space1;
    QLabel *m_label_space2;
//////////////////////////////////////////new end


    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_bg;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QWidget *m_widget_main;

    QLabel *m_label_line1;
    QLineEdit *m_lineedit_1;
    QLabel *m_label_line1_1;

    QLabel *m_label_line2;
    QLineEdit *m_lineedit_2;
    QPushButton *m_pushbutton_line2;//时间校准

    QLabel *m_label_line3;
    CMyPushButton *m_pushbutton_line3;//整点报点使能
    bool m_enble_line3;

    QLabel *m_label_line4;
    CMyPushButton *m_pushbutton_line4;//低电压保护
    bool m_enble_line4;

    QHBoxLayout *m_hboxlayout_5;
    QLabel *m_label_line5;
    QLineEdit *m_lineedit_5;  //低电压值
    QLabel *m_label_line1_5;

    QLabel *m_label_line5_1;
    QLineEdit *m_lineedit_5_1;  //上报中心时间
    QLabel *m_label_line1_5_1;


    QLabel *m_label_line6;
    QLineEdit *m_lineedit6;  //设备门开启上报时间
    QLabel *m_label_line6_1;

    QLabel *m_label_line7;
    QLabel *m_label_line7_1;
    QLineEdit *m_lineedit7;  //硬盘加热
    QLabel *m_label_line7_2;

    QLabel *m_label_line8;
    QLineEdit *m_lineedit8_1;  //机内温度范围
    QLabel *m_label_line8_1;
    QLineEdit *m_lineedit8_2;
    QLabel *m_label_line8_2;


    //QLabel *m_label_line9;
    QPushButton *m_pushbutton_save;//保存
    QPushButton *m_pushbutton_reset;//重置

    CMyDefineScreen *m_define_screen;

};

#endif // SYSTEM_SETUP_WIDGET_H
