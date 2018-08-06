#ifndef SWITCH_CONFIG_WIDGET_H
#define SWITCH_CONFIG_WIDGET_H

#include <QWidget>

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

#include "class/common/navigateleftbutton.h"
#include "class/common/my_pushbutton.h"
#include "class/common/scrollcontroller.h"

#include "protocol.h"

#include "class/common/define_screen.h"

class CSwitchConfigInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CSwitchConfigInfo(char _type,QWidget *parent = 0);
    enum
    {
        kEnumVehicleDoorCheck,  //车门检测
        kEnumOtherSwitchCheck   //其他开关量检测
    };
signals:
    void signal_other_enable();//其它使能
private:
    void resizeEvent(QResizeEvent *e);
     void vehicle_door_check_init();  //车门检测
     void other_enable_init();       //其它使能
     void other_switch_check_init();   //其他开关量检测

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
public:
    //车门检测
    //CMyPushButton *m_pushbutton_line1;
    QCheckBox *m_checkbox_line1;
    QComboBox *m_combobox_line2;
    QComboBox *m_combobox_line3;
    QComboBox *m_combobox_line4;
    QComboBox *m_combobox_line5;
    QComboBox *m_combobox_line6;
    QComboBox *m_combobox_line7;
    QPushButton *m_pushbutton_line7;

    //其他开关量检测
    QComboBox *m_combobox_line1;
#if 0
    CMyPushButton *m_pushbutton_line2;
    CMyPushButton *m_pushbutton_line3;
    CMyPushButton *m_pushbutton_line4;
    CMyPushButton *m_pushbutton_line5;
    CMyPushButton *m_pushbutton_line6;
#endif
    QCheckBox *m_checkbox_line2;
    QCheckBox *m_checkbox_line3;
    QCheckBox *m_checkbox_line4;
    QCheckBox *m_checkbox_line5;
    QCheckBox *m_checkbox_line6;

    CMyDefineScreen *m_define_screen;

};


class CSwitchConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSwitchConfigWidget(QWidget *parent = 0);

signals:
    void signal_window_close();
public slots:
    void slot_button_1();
    void slot_button_2();
    void slot_switchconfiginfo_other_enable(); //其它使能

    void slot_pushbutton_return();
    void slot_switch_config_widget_event(const CSystemIoConfig &_info);
    void slot_timer_init_button1();
public:
    void save_data();
private:
    void resizeEvent(QResizeEvent *e);
    void switch_config_init_new();
    void other_enable_init();
    void read_data();


    QWidget *m_widget_main;
    QWidget *m_widget_other_enable;
    //QLabel *m_label_test;

    void scroll_layout();

    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QVBoxLayout *m_vboxlayout_storage;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

   QWidget *m_widget_bg;

    NavigateLeftButton *m_button_1;
    NavigateLeftButton *m_button_2;


    CSwitchConfigInfo *m_SwitchConfigInfo1;
    CSwitchConfigInfo *m_SwitchConfigInfo2;


    QLabel *m_label_space1;


        bool m_button_press_1;
    bool m_button_press_2;

   //其他使能
   QLabel *m_label_other_enable_line1;
   QLabel *m_label_other_enable_line2;
   QLabel *m_label_other_enable_line3;
   QLabel *m_label_other_enable_line4;
   QLabel *m_label_other_enable_line5;
   QLabel *m_label_other_enable_line6;
   QLabel *m_label_other_enable_line6_1;
   QLabel *m_label_other_enable_line6_2;
   QLineEdit *m_label_other_enablelineedit6;

   QLabel *m_label_other_enable_line7;
   QLabel *m_label_other_enable_line7_2;
   QLineEdit *m_label_other_enablelineedit7;
#if 0
   CMyPushButton *m_pushbutton_other_enable_line2;
   CMyPushButton *m_pushbutton_other_enable_line3;
   CMyPushButton *m_pushbutton_other_enable_line4;
   CMyPushButton *m_pushbutton_other_enable_line5;
   CMyPushButton *m_pushbutton_other_enable_line6;
   CMyPushButton *m_pushbutton_other_enable_line7;
#endif
    QCheckBox *m_checkbox_other_enable_line2;
    QCheckBox *m_checkbox_other_enable_line3;
    QCheckBox *m_checkbox_other_enable_line4;
    QCheckBox *m_checkbox_other_enable_line5;
    QCheckBox *m_checkbox_other_enable_line6;
    QCheckBox *m_checkbox_other_enable_line7;

    CMyDefineScreen *m_define_screen;

};


#endif // SWITCH_CONFIG_WIDGET_H











