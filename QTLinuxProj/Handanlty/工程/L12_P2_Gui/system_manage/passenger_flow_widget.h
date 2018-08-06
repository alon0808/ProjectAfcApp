#ifndef PASSENGER_FLOW_WIDGET_H
#define PASSENGER_FLOW_WIDGET_H

#include <QWidget>

#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QDialog>

#include "class/common/scrollcontroller.h"
#include "class/common/navigateleftbutton.h"

#include "class/common/define_screen.h"

class CSelectSingBox : public QDialog
{
    Q_OBJECT
public:
    static CSelectSingBox *getInstance();
    explicit CSelectSingBox(QWidget *parent = 0);
    typedef enum
    {
        KEnumButtonOk,
        KEnumButtonCancel
    }ButtonType;
    enum
    {
        kEnumSelect1,
        kEnumSelect2
    };
    void init();
signals:

public slots:
    void slot_pushbutton_close();
    void slot_pushbutton1();
    void slot_pushbutton2();

    void slot_pushbutton_select1();
    void slot_pushbutton_select2();


public:

    CSelectSingBox::ButtonType get_result();

    char get_select() {return m_current_select;}

    void set_timer(char _timerType);
private:
    void resizeEvent(QResizeEvent *e);

    QPushButton *m_pushbutton_close;

    QPushButton *m_pushbutton1;  //确定
    QPushButton *m_pushbutton2;  //取消
    ButtonType m_button_type;
    QLabel *m_label_title;  //标题


    char m_type;
    QString m_button_text1;
    QString m_button_text2;

    QPushButton *m_pushbutton_select1;
    QPushButton *m_pushbutton_select2;
    QLabel *m_label_select1;
    QLabel *m_label_select2;

    char m_current_select;

    CMyDefineScreen *m_define_screen;

public:
    char m_return_type;
    QString m_return_result;

    static CSelectSingBox *m_single;

};

class CPassengerFlowInfo : public QWidget
{
    Q_OBJECT
public:
    explicit CPassengerFlowInfo(char _type,QWidget *parent = 0);
    enum
    {
        kEnumCam1_1,
        kEnumCam2_1,
        kEnumCam3_1,
        kEnumCam4_1,
        kEnumSeniorSetup
    };

signals:
    void signal_button_line1();
    void signal_button_line12();
public slots:
    void slot_pushbutton_line2();
    void slot_pushbutton_line3();


private:
    void resizeEvent(QResizeEvent *e);
    void set_type(char _type);
    void cam1_1_init();
    void cam2_1_init();
    void senior_setup_init();


    QWidget *m_widget_bg;
public:
    void set_progress(QString _text,bool _show);
public:
    char m_type;
    QLabel *m_label_line1_0;
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

    QLabel *m_label_line1_1;
    QLabel *m_label_line2_1;
    QLabel *m_label_line3_0;
    QLabel *m_label_line3_1;
    QCheckBox *m_checkbox_line1;
    QPushButton *m_pushbutton_line2;
    QPushButton *m_pushbutton_line3;
    QCheckBox *m_checkbox_line4;
    QComboBox *m_combobox_line5;
    QComboBox *m_combobox_line6;
    QLineEdit *m_lineedit_line7;
    QLineEdit *m_lineedit_line8;
    QLineEdit *m_lineedit_line9;
    QLineEdit *m_lineedit_line10;
    QLabel *m_label_line10_1;
    QLineEdit *m_lineedit_line11;
    QLabel *m_label_line11_1;
    NavigateLeftButton *m_button_line12;

    QLineEdit *m_lineedit_line2;
    bool pushbutton_line2_flag;

    //高级设置
    NavigateLeftButton *m_button_line1;
    QCheckBox *m_checkbox_line2;
    QLineEdit *m_lineedit_line3;

    CMyDefineScreen *m_define_screen;

};

class CCheckRegionInfo :public QWidget
{
    Q_OBJECT
public:
    CCheckRegionInfo(QWidget *parent = 0,char _type = 0);

signals:
    void signal_pushbutton_default();
    void signal_pushbutton_bg_save();
    void signal_pushbutton_save();

public slots:

    void slot_pushbutton_line1();
    void slot_pushbutton_line2();
    void slot_pushbutton_line3();
    void slot_pushbutton_line4();

    void slot_pushbutton_left();
    void slot_pushbutton_right();
    void slot_pushbutton_top();
    void slot_pushbutton_bottom();

    void slot_lineedit1_changed(const QString &_text);
    void slot_lineedit2_changed(const QString &_text);
    void slot_lineedit3_changed(const QString &_text);
    void slot_lineedit4_changed(const QString &_text);



public:
    enum
    {
        kEnumLine1,
        kEnumLine2,
        kEnumLine3,
        kEnumLine4
    };
private:
    void resizeEvent(QResizeEvent *e);
    void update_lineedit_line1();
    void update_lineedit_line2();
    void update_lineedit_line3();
    void update_lineedit_line4();
    void hide_button_all();

    QLabel *m_label_line1;
public:
    QLineEdit *m_lineedit_line1;
    QLineEdit *m_lineedit_line2;
    QLineEdit *m_lineedit_line3;
    QLineEdit *m_lineedit_line4;

    QLabel *m_label_line1_1;
    QLabel *m_label_line2_1;
    QLabel *m_label_line3_1;
    QLabel *m_label_line4_1;

    QLabel *m_label_line1_2;
    QLabel *m_label_line2_2;
    QLabel *m_label_line3_2;
    QLabel *m_label_line4_2;

    QPushButton *m_pushbutton_default;
    QPushButton *m_pushbutton_bg_save;
    QPushButton *m_pushbutton_save;


    QPushButton *m_pushbutton_line1;
    QPushButton *m_pushbutton_line2;
    QPushButton *m_pushbutton_line3;
    QPushButton *m_pushbutton_line4;

    QPushButton *m_pushbutton_left;
    QPushButton *m_pushbutton_right;
    QPushButton *m_pushbutton_top;
    QPushButton *m_pushbutton_bottom;

    char m_current_line;
    QWidget *m_widget_no_video;

    bool m_auto_input;

    CMyDefineScreen *m_define_screen;
    char m_type;
    QTimer *m_timer_init;

};

class CPassengerFlowWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPassengerFlowWidget(QWidget *parent = 0);
    
signals:
    void signal_window_close();
public slots:
    void slot_button_1();
    void slot_button_2();
    void slot_button_3();
    void slot_button_4();
    void slot_button_5();

    void slot_button_count_enable();
    void slot_button_check_region1();
    void slot_button_check_region2();
    void slot_button_check_region3();
    void slot_button_check_region4();
    void slot_pushbutton_return();
    void slot_pushbutton_home();

    void slot_timer_init();

    void slot_passenger_flow_widget_event(char _type,void *_param);

    void slot_check_region_cam1_1_button_default();
    void slot_check_region_cam1_1_button_bg_save();
    void slot_check_region_cam1_1_button_save();
    void slot_check_region_cam2_1_button_default();
    void slot_check_region_cam2_1_button_bg_save();
    void slot_check_region_cam2_1_button_save();

    void slot_check_region_cam3_1_button_default();
    void slot_check_region_cam3_1_button_bg_save();
    void slot_check_region_cam3_1_button_save();
    void slot_check_region_cam4_1_button_default();
    void slot_check_region_cam4_1_button_bg_save();
    void slot_check_region_cam4_1_button_save();

public:
    void save_data();

private:
    void resizeEvent(QResizeEvent *e);
    void scroll_layout();
    void init();
    void count_enable_init();

    void read_data();

    void save_bg_progress(int _type);


    NavigateLeftButton *m_button_1;
    NavigateLeftButton *m_button_2;
    NavigateLeftButton *m_button_3;
    NavigateLeftButton *m_button_4;
    NavigateLeftButton *m_button_5;


    bool m_button_press_1;
    bool m_button_press_2;
    bool m_button_press_3;
    bool m_button_press_4;
    bool m_button_press_5;

    CPassengerFlowInfo *m_passenger_flow_info1;
    CPassengerFlowInfo *m_passenger_flow_info2;
    CPassengerFlowInfo *m_passenger_flow_info3;
    CPassengerFlowInfo *m_passenger_flow_info4;
    CPassengerFlowInfo *m_passenger_flow_info5;

    CCheckRegionInfo *m_CCheckRegionInfo1;
    CCheckRegionInfo *m_CCheckRegionInfo2;
    CCheckRegionInfo *m_CCheckRegionInfo3;
    CCheckRegionInfo *m_CCheckRegionInfo4;

    QLabel *m_label_space1;
    QLabel *m_label_space2;
    QLabel *m_label_space3;
    QLabel *m_label_space4;

    QScrollArea *m_scrollarea_1;

    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;

    int m_scroll_widget_width;
    int m_scroll_widget_height;

    QWidget *m_widget_bg_color;
    QWidget *m_widget_bg_color_1;

    QVBoxLayout *m_pMainLayout;
    QVBoxLayout *m_vboxlayout_storage;
    QWidget *m_widget_main;
    QWidget *m_widget_bg;
    QWidget *m_widget_scroll;

    QWidget *m_widget_count_enable;
    QWidget *m_widget_check_region1;
    QWidget *m_widget_check_region2;
    QWidget *m_widget_check_region3;
    QWidget *m_widget_check_region4;

    QLabel *m_label_count_enable_line1;
    QLabel *m_label_count_enable_line2;
    QCheckBox *m_checkbox_count_enable_line2;
    QLabel *m_label_count_enable_line3;
    QCheckBox *m_checkbox_count_enable_line3;
    QLabel *m_label_count_enable_line4;
    QCheckBox *m_checkbox_count_enable_line4;
    QLabel *m_label_count_enable_line5;
    QCheckBox *m_checkbox_count_enable_line5;

    QLabel *m_label_count_enable_line6;
    QLabel *m_label_count_enable_line6_1;
    QCheckBox *m_checkbox_count_enable_line6;
    QLineEdit *m_lineedit_count_enable_line6;
    QLabel *m_label_count_enable_line6_2;

    QLabel *m_label_count_enable_line7;
    QLineEdit *m_lineedit_count_enable_line7;
    QLabel *m_label_count_enable_line7_1;

    QLabel *m_label_count_enable_line8;
    QLineEdit *m_lineedit_count_enable_line8;
    QLabel *m_label_count_enable_line8_1;

    QLabel *m_label_count_enable_line9;
    QLineEdit *m_lineedit_count_enable_line9;
    QLabel *m_label_count_enable_line9_1;

    CMyDefineScreen *m_define_screen;

};

#endif // PASSENGER_FLOW_WIDGET_H



















