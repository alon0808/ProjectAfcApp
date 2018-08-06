#ifndef DRIVERMENUWIDGET_H
#define DRIVERMENUWIDGET_H

#include <QWidget>

#include "head_widget.h"
#include "navigate_left.h"

#include <QStackedWidget>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>

#include <QSlider>
#include "class/common/scrollcontroller.h"
#include "class/common/my_pushbutton.h"

#include "protocol.h"

class CBaseSetInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CBaseSetInfo(char _type,QWidget *parent = 0);
    enum
    {
        kEnumVolumn,
        kEnumlight
    };
signals:

public slots:
    void slot_light_changed(int index);
private:
    void volumn_init();
    void light_init();
    void set_type(char _type);
    char m_type;

    QWidget *m_widget_bg;
    QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;

public:
    QComboBox *m_combobox_line1;
    QComboBox *m_combobox_line2;
    QComboBox *m_combobox_line3;



};


//基本设置
class CBaseSetWidget :public QWidget
{
    Q_OBJECT
public:
    CBaseSetWidget(QWidget *parent = 0);
    enum
    {
        kEnumVolumeSet = 1,  /*1:设置声音*/
        kEnumVolumePlay = 2, /*2:播放声音*/
        kEnumVolumeGet = 3,   /*3:获取声音*/
        kEnumLightSet = 4,   //设置亮度
        kEnumLightGet = 5,   //获取亮度
        kEnumSeniorSet,      //高级设置保存
        kEnumSeniorGet,      //获取高级设置
        kEnumSeniorDefault   //获取高级设置默认
    };
    enum
    {/*1:内音量 2:外音量 3:TTS音*/
        kEnumPlayWithin = 1,
        kEnumPlayOut = 2,
        kEnumPlayTTS = 3
    };
public slots:
    void slot_button_1();
    void slot_button_2();

public:
    void data_init();

public:
    CBaseSetInfo *m_base_set1;
    CBaseSetInfo *m_base_set2;



private:
    void scroll_layout();
    void base_set_init();



    NavigateLeftButton *m_button_1;
    NavigateLeftButton *m_button_2;

    bool m_button_press_1;
    bool m_button_press_2;




    QLabel *m_label_space1;

    QScrollArea *m_scrollarea_1;

    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_vboxlayout_storage;
    QWidget *m_widget_main;
    QWidget *m_widget_bg;
    QWidget *m_widget_scroll;


};


//镜像设置
class CImageSetWidget1 : public QWidget
{
    Q_OBJECT
public:
    explicit CImageSetWidget1(QWidget *parent = 0);

signals:

public slots:
public:

public:
    QLabel *m_label_line1;
    QCheckBox *m_checkbox_line2_1;
    QLabel *m_label_line2_1;
    QCheckBox *m_checkbox_line2_2;
    QLabel *m_label_line2_2;
    QCheckBox *m_checkbox_line2_3;
    QLabel *m_label_line2_3;
    QCheckBox *m_checkbox_line2_4;
    QLabel *m_label_line2_4;

    QCheckBox *m_checkbox_line3_1;
    QLabel *m_label_line3_1;
    QCheckBox *m_checkbox_line3_2;
    QLabel *m_label_line3_2;
    QCheckBox *m_checkbox_line3_3;
    QLabel *m_label_line3_3;
    QCheckBox *m_checkbox_line3_4;
    QLabel *m_label_line3_4;

    QCheckBox *m_checkbox_line4_1;
    QLabel *m_label_line4_1;
    QCheckBox *m_checkbox_line4_2;
    QLabel *m_label_line4_2;
    QCheckBox *m_checkbox_line4_3;
    QLabel *m_label_line4_3;
    QCheckBox *m_checkbox_line4_4;
    QLabel *m_label_line4_4;

    QCheckBox *m_checkbox_line5_1;
    QLabel *m_label_line5_1;
    QCheckBox *m_checkbox_line5_2;
    QLabel *m_label_line5_2;
    QCheckBox *m_checkbox_line5_3;
    QLabel *m_label_line5_3;
    QCheckBox *m_checkbox_line5_4;
    QLabel *m_label_line5_4;

    QCheckBox *m_checkbox_line6_1;
    QLabel *m_label_line6_1;
    QCheckBox *m_checkbox_line6_2;
    QLabel *m_label_line6_2;
    QCheckBox *m_checkbox_line6_3;
    QLabel *m_label_line6_3;
    QCheckBox *m_checkbox_line6_4;
    QLabel *m_label_line6_4;

    QCheckBox *m_checkbox_line7_1;
    QLabel *m_label_line7_1;

    QPushButton *m_pushbutton_default;

};

//高级设置
class CSeniorSetWidget :public QWidget
{
    Q_OBJECT
public:
    CSeniorSetWidget(QWidget *parent = 0);

public slots:
    void slot_ablout_machine_init();
    void slot_pushbutton_config();
    void slot_pushbutton_return();
    void slot_pushbutton_default();

    void slot_timer_init();

signals:
    void signal_window_close();
private:
    void scroll_layout();

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
public:
    QComboBox *m_combobox_line1;
    //CMyPushButton *m_pushbutton_line2;
    QComboBox *m_combobox_line2;
    QComboBox *m_combobox_line3;
    QComboBox *m_combobox_line4;
    QComboBox *m_combobox_line4_0;
    QCheckBox *m_checkbox_line4; //add
    QComboBox *m_combobox_line5;
    QCheckBox *m_checkbox_line5; //add

    QComboBox *m_combobox_line6;
    QComboBox *m_combobox_line7;
    QComboBox *m_combobox_line8;
    QComboBox *m_combobox_line9;

    QCheckBox *m_checkbox_line10;

    QComboBox *m_combobox_line11;
    QPushButton *m_pushbutton_line11;

    QComboBox *m_combobox_line12;

    QLabel *m_label_line2;
    QLabel *m_label_line3;
    QLabel *m_label_line4;
    QLabel *m_label_line4_0;
    QLabel *m_label_line5;
    QLabel *m_label_line6;
    QLabel *m_label_line7;
    QLabel *m_label_line8;
    QLabel *m_label_line9;
    QLabel *m_label_line10;
    QLabel *m_label_line11;
    QLabel *m_label_line12;

    QPushButton *m_pushbutton_line13;

    CImageSetWidget1 *m_image_set_widget;

};


class CDriverMenuWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CDriverMenuWidget(QWidget *parent = 0);
    
signals:
    void signal_set_default_id(int);
    void signal_driver_menu_user_return();
    void signal_pushbutton_home();
    void signal_pushbutton_menu();
public slots:
    void slot_driver_menu_widget_event(int,QString);
    void slot_pushbutton_home();
    void slot_pushbutton_close();
    void slot_pushbutton_back();



    void slot_driver_menu_voice_event(int _value1,int _value2,int _value3);
    void slot_driver_menu_light_event(int _value);

    void slot_driver_menu_widget_event(const CDriverMenuInfo &_driverMenuInfo);
public:
    bool goto_main_page() {return m_goto_main_page;}
private:
    void show_base_set();    //基本设置
    void show_senior_set();  //高级设置
    void save_data();
    void read_senior_set_info();//保存高级设置到临时数据

    CBaseSetWidget *m_CBaseSetWidget;
    CSeniorSetWidget *m_CSeniorSetWidget;

    CHeadWidget *m_CHeadWidget;
    QWidget *m_widget_bg1;
    QWidget *m_widget_bg2;
    QLabel *m_label_set;
    CNavigateLeft *m_CNavigateLeft;


    QStackedWidget *m_stackwidget_navigate_right;

    bool m_goto_main_page;
    char m_current_index;
};

#endif // DRIVERMENUWIDGET_H
