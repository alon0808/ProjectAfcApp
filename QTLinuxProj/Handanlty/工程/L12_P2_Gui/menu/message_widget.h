#ifndef MESSAGE_WIDGET_H
#define MESSAGE_WIDGET_H

#include <QDialog>

#include "head_widget.h"
#include "navigate_left.h"

#include <QStackedWidget>
#include <QTextEdit>

#include "../list_view/common_view/common_query_list.h"
#include "class/common/scrollcontroller.h"

class CMessageContentWidget;

//预设事件
class CPresetEventWidget :public QWidget
{
    Q_OBJECT
public:
    CPresetEventWidget(QWidget *parent = 0);
    void column_width_init();
public slots:
    void slot_message_timer_init_1();
    void slot_pre_event_clicked_event(int,int);
private:

    int m_list_width;
    int m_list_height;
    QWidget *m_widget_bg;
public:
    CCommonQueryList *m_preset_event_list;          //预设事件

};
//重要短信
class CImportantMessageWidget :public QWidget
{
    Q_OBJECT
public:
    CImportantMessageWidget(QWidget *parent = 0);
    void column_width_init();
public slots:
    void slot_message_timer_init_2();

private:
    void import_message_init();
    CCommonQueryList *m_important_message_list;     //重要短信
    CMessageContentWidget *m_CMessageContentWidget;

        int m_list_width;
    int m_list_height;

};
//普通短信
class CNormalMessageWidget :public QWidget
{
    Q_OBJECT
public:
    CNormalMessageWidget(QWidget *parent = 0);
    void column_width_init();
public slots:
    void slot_message_timer_init_3();
    void slot_normal_message_clicked_event(int _type,int row);
    void slot_pushbutton_return();
signals:
    void signal_window_close();
private:
    void scroll_layout();
    void message_content_init();

    QVBoxLayout *m_pMainLayout;
    QScrollArea *m_scrollarea_1;
    QWidget *m_scroll_widget;
    ScrollController *m_ScrollController;
    QVBoxLayout *m_vboxlayout_widget;
    QWidget *m_widget_bg;
    QWidget *m_widget_main;
    QWidget *m_widget_main1;

    int m_scroll_widget_width;
    int m_scroll_widget_height;


    CMessageContentWidget *m_CMessageContentWidget;
        int m_list_width;
    int m_list_height;
    QLabel *m_label_title;
    QTextEdit *m_textedit_message;
public:
    CCommonQueryList *m_normal_message_list;        //普通短信

};
//行车计划
class CVehiclePlanWidget :public QWidget
{
    Q_OBJECT
public:
    CVehiclePlanWidget(QWidget *parent = 0);
    void column_width_init();
public slots:
    void slot_message_timer_init_4();
private:

        int m_list_width;
    int m_list_height;
    QWidget *m_widget_bg;
public:
    CCommonQueryList *vehicle_plan_list;            //行车计划

};

class CMessageContentWidget :public QWidget
{
    Q_OBJECT
public:
    CMessageContentWidget(QWidget *parent = 0);

public slots:
    void slot_pushbutton_message_up();
    void slot_pushbutton_message_down();
private:
    QWidget *m_widget_message_bg;
    QLabel *m_label_message_content;
    QTextEdit *m_textedit_message;
    QPushButton *m_pushbutton_message_up;
    QPushButton *m_pushbutton_message_down;

};



class CMessageWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CMessageWidget(QWidget *parent = 0);
    
signals:
    void signal_set_default_id(int);
    void signal_message_content_user_return();
    void signal_pushbutton_home();
    void signal_pushbutton_menu();

public slots:
    void slot_pushbutton_close();
    void slot_pushbutton_home();
    void slot_pushbutton_back();
    void slot_message_widget_event(int,QString _text);
    void slot_message_timer_init();

    void slot_message_widget_notify_event(int _type,CCommonQueryMode _mode);

public:
    bool goto_main_page() {return m_goto_main_page;}

private:
    void message_init();
    void message_test_data();
    void show_preset_event();       //显示预设事件
    void show_important_message();  //显示重要短信
    void show_normal_message();     //显示普通短信
    void show_vehicle_plan();       //显示行车计划

    CHeadWidget *m_CHeadWidget;
    QWidget *m_widget_bg1;
    QWidget *m_widget_bg2;
    QLabel *m_label_system_message;
    CNavigateLeft *m_CNavigateLeft;



    QTimer *m_test_time;
    CPresetEventWidget *m_CPresetEventWidget;
    CImportantMessageWidget *m_CImportantMessageWidget;
    CNormalMessageWidget *m_CNormalMessageWidget;
    CVehiclePlanWidget *m_CVehiclePlanWidget;

    QStackedWidget *m_stackwidget_navigate_right;

    int m_current_index;
    bool m_goto_main_page;

};

#endif // MESSAGE_WIDGET_H
