#ifndef SENIOR_SETUP_WIDGET_H
#define SENIOR_SETUP_WIDGET_H

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QCheckBox>

#include "class/common/navigateleftbutton.h"
#include "class/common/scrollcontroller.h"
#include "class/common/my_pushbutton.h"

#include "protocol.h"

class CSeniorSetupInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CSeniorSetupInfo(int _type,QWidget *parent = 0);
    enum
    {
        kEnumCommonConfig,      //通用配置
        kEnumWarningEnable,       //告警使能
        kEnumCanSetting,    //CAN设置
        SeniorSetup             //高级设置
    };
signals:
    //通用配置
    void signal_location_mode();    //定位方式
    void signal_navigation();       //惯性导航
    //CAN设置
    void signal_can1();
    void signal_can2();
    //高级设置
    void signal_temperature_check(); //温度检测

private:
    void common_config_init();      //通用配置
    void warning_enable_init();     //告警使能
    void can_setting_init();        //CAN设置
    void senior_setup_init();       //高级设置

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
public:
    //通用配置
    QComboBox *m_combobox_line1;
    QPushButton *m_pushbutton_line1_click;//第一行整个区域可点击
    QCheckBox *m_checkbox_line2;
    QPushButton *m_pushbutton_line2_1;
    QComboBox *m_combobox_line3;
    QLineEdit *m_lineedit4;
    QLabel *m_label_line4_1;
    QLineEdit *m_lineedit5;
    QLabel *m_label_line5_1;
    QLineEdit *m_lineedit6;
    QLabel *m_label_line6_1;
    QLineEdit *m_lineedit7;
    QLabel *m_label_line7_1;
    QLineEdit *m_lineedit8;
    QLabel *m_label_line8_1;
    QLineEdit *m_lineedit9;
    QLabel *m_label_line9_1;
    QComboBox *m_combobox_line10;

    //告警使能

    QCheckBox *m_checkbox_line1;
    QLabel *m_label_line1_1;
    QLineEdit *m_lineedit1;
    QLabel *m_label_line1_2;

    QLabel *m_label_line2_1;
    QLineEdit *m_lineedit2;
    QLabel *m_label_line2_2;


    QCheckBox *m_checkbox_line3;
    QLabel *m_label_line3_1;
    QLineEdit *m_lineedit3;
    QLabel *m_label_line3_2;


    QCheckBox *m_checkbox_line4;
    QLabel *m_label_line4_2;


    QCheckBox *m_checkbox_line5;
    QLabel *m_label_line5_2;


    QCheckBox *m_checkbox_line6;
    QLabel *m_label_line6_2;

    //CAN设置
    QPushButton *m_pushbutton_line1_1;

    //高级设置
    QPushButton *m_pushbutton_line3_1;
    QLineEdit *m_lineedit2_1;

};



class SeniorSetupWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SeniorSetupWidget(QWidget *parent = 0);

signals:
    void signal_window_close();
    void signal_pushbutton_send_can_command();
public slots:
    void slot_button_1();
    void slot_button_2();
    void slot_button_3();
    void slot_button_4();

    //通用配置
    void slot_seniorsetupinfo_location_mode();    //定位方式
    void slot_seniorsetupinfo__navigation();       //惯性导航
    void slot_seniorsetupinfo_help();  //帮助
    //CAN设置
    void slot_seniorsetupinfo__can1();
    void slot_seniorsetupinfo__can2();
    //高级设置
    void slot_seniorsetupinfo__temperature_check(); //温度检测

    void slot_pushbutton_return();

    void slot_pushbutton_send_can1_command();//CAN设置　－－　调试1-发送
    void slot_pushbutton_send_can2_command();//CAN设置　－－　调试2-发送

    void slot_senior_setup_widget_event(char _type,const CSystemSeniorSetup &_info);
public:
    void save_data();

private:
    void scroll_layout();
    void senior_setup_init_new();
    void senior_setup_init();

    void submain_1_init(); //通用配置　－－　定位方式
    void submain_2_init();//通用配置　－－　惯性导航（配置）
    void submain_3_init();//通用配置　－－　惯性导航（配置 - 帮助）
    void submain_4_init();//CAN设置　－－　调试1
    void submain_5_init();//CAN设置　－－　调试2
    void submain_6_init();//高级设置　－－　温度检测（配置）

    void submain_45_create(char _type,QWidget *_widget);
    void submain_45_printf_can(char _type,const QString &_data);

    void read_data();


///////////////////////////////////////////new start
    NavigateLeftButton *m_button_1;
    NavigateLeftButton *m_button_2;
    NavigateLeftButton *m_button_3;
    NavigateLeftButton *m_button_4;
    QVBoxLayout *m_vboxlayout_storage;

    bool m_button_press_1;
    bool m_button_press_2;
    bool m_button_press_3;
    bool m_button_press_4;

    CSeniorSetupInfo *m_senior_info1;
    CSeniorSetupInfo *m_senior_info2;
    CSeniorSetupInfo *m_senior_info3;
    CSeniorSetupInfo *m_senior_info4;

    QLabel *m_label_space1;
    QLabel *m_label_space2;
    QLabel *m_label_space3;

///////////////////////////////////////////new end
    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_bg;

    int m_scroll_widget_width;
    int m_scroll_widget_height;
    QWidget *m_widget_main;  //主显示
    QWidget *m_widget_submain_1; //通用配置　－－　定位方式
    QWidget *m_widget_submain_2;//通用配置　－－　惯性导航（配置）
    QWidget *m_widget_submain_3;//通用配置　－－　惯性导航（配置 - 帮助）
    QWidget *m_widget_submain_4;//CAN设置　－－　调试1
    QWidget *m_widget_submain_5;//CAN设置　－－　调试2
    QWidget *m_widget_submain_6;//高级设置　－－　温度检测（配置）

/////////////////////////////////////////////////////////通用配置　－－　定位方式
    QLabel *m_label_submain1_line1;
    QLabel *m_label_submain1_line2;
    QLabel *m_label_submain1_line2_1;
    QLabel *m_label_submain1_line3;
    QLabel *m_label_submain1_line3_1;
    QLabel *m_label_submain1_line4;
    QLabel *m_label_submain1_line4_1;

/////////////////////////////////////////////////////////通用配置　－－　惯性导航（配置）
    QLabel *m_label_submain2_line1;

    QLabel *m_label_submain2_line2;
    QLabel *m_label_submain2_line2_1;
    QLineEdit *m_lineedit_submain2_line2;
    QLabel *m_label_submain2_line2_2;

    QLabel *m_label_submain2_line3;
    QLabel *m_label_submain2_line3_1;
    QLineEdit *m_lineedit_submain2_line3;
    QLabel *m_label_submain2_line3_2;

    QLabel *m_label_submain2_line4;
    QLabel *m_label_submain2_line4_1;
    QLineEdit *m_lineedit_submain2_line4;
    QLabel *m_label_submain2_line4_2;

    QLabel *m_label_submain2_line5;
    QLabel *m_label_submain2_line5_1;
    QLineEdit *m_lineedit_submain2_line5;
    QLabel *m_label_submain2_line5_2;

    QPushButton *m_pushbutton_save;
    QPushButton *m_pushbutton_help;

    //帮助
    QLabel *m_label_submain3_line1;
    QWidget *m_widget_submain3_1;
    QWidget *m_widget_submain3_2;

    //CAN设置　－－　调试
    QLabel *m_label_submain4_line1;
    QLabel *m_label_submain4_line2;
    QPushButton *m_pushbutton_submain4_line2;

    QTextEdit *m_textedit1;
    QTextEdit *m_textedit2;

    //高级设置　－－　温度检测（配置）
    QLabel *m_label_submain6_line1;
    QLineEdit *m_lineedit_submain6_line2_1;
    QLabel *m_label_submain6_line2_1;
    QLineEdit *m_lineedit_submain6_line2_2;
    QLabel *m_label_submain6_line2_2;

    QLabel *m_label_submain6_line2;
    QLabel *m_label_submain6_line3;
    QLineEdit *m_lineedit_submain6_line3_1;
    QLabel *m_label_submain6_line3_1;
    QLineEdit *m_lineedit_submain6_line3_2;
    QLabel *m_label_submain6_line3_2;



    QLabel *m_label_submain6_line4;
    QLineEdit *m_lineedit_submain6_line4_1;
    QLabel *m_label_submain6_line4_1;





};


#endif // SENIOR_SETUP_WIDGET_H
















