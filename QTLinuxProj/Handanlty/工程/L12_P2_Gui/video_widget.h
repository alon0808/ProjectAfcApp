#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <QDialog>
#include <QStackedWidget>
#include <QMenu>

#include "head_widget.h"

#include "class/common/define_screen.h"
#include "common_data.h"
#include "class/common/parent_widget.h"


//1分频
class CVideoOneWidget :public QWidget
{
    Q_OBJECT
public:
    CVideoOneWidget(QWidget *parent = 0);

public slots:

public:
    void set_output_status(char _number,bool _output);
    void modify_number(char _number);
private:
    QPushButton *m_pushbutton_play;
    //QLabel *m_show_video;
    QLabel *m_show_number;
    QLabel *m_show_text;
    QString m_button_sheet1;

};
//4分频
class CVideoFourWidget :public QWidget
{
    Q_OBJECT
public:
    CVideoFourWidget(QWidget *parent = 0);

public slots:
    void slot_pushbutton_amplify();

    void slot_pushbutton_play1();
    void slot_pushbutton_play2();
    void slot_pushbutton_play3();
    void slot_pushbutton_play4();

public:

    void show_one_status(QString _sheet,const QString &_number);

    void set_output_status(char _number,bool _output);
    void set_full_screen_status(char _number,bool _output);
    void set_show_normal();

private:

    QWidget *m_widget_line1;
    QWidget *m_widget_line2;

    QPushButton *m_pushbutton_play1;
    //QLabel *m_show_video1;
    QLabel *m_show_number1;

    QPushButton *m_pushbutton_play2;
    //QLabel *m_show_video2;
    QLabel *m_show_number2;

    QPushButton *m_pushbutton_play3;
    //QLabel *m_show_video3;
    QLabel *m_show_number3;

    QPushButton *m_pushbutton_play4;
    //QLabel *m_show_video4;
    QLabel *m_show_number4;

    QPushButton *m_pushbutton_amplify;
    QLabel *m_show_amplify;

     QLabel *m_show_amplify_text;

    QString m_save_sheet;
    QString m_save_number;

    QString m_button_sheet1;
    QString m_button_sheet2;
    QString m_button_sheet3;
    QString m_button_sheet4;

    QLabel *m_show_text1;
    QLabel *m_show_text2;
    QLabel *m_show_text3;
    QLabel *m_show_text4;

};
class CVideoWidget;
//9分频
class CVideoNineWidget :public QWidget
{
    Q_OBJECT
public:
    CVideoNineWidget(QWidget *parent = 0);
    enum
    {
        kEnumVideoNinePassengerFlowMode, //客流界面
        kEnumVideoNineVideoMode          //视频界面
    };
public slots:
    void slot_pushbutton_amplify();
    void slot_pushbutton_play1();
    void slot_pushbutton_play2();
    void slot_pushbutton_play3();
    void slot_pushbutton_play4();
    void slot_pushbutton_play5();
    void slot_pushbutton_play6();
    void slot_pushbutton_play7();
    void slot_pushbutton_play8();
    void slot_pushbutton_play9();

public:
    void set_full_screen(bool _flag);
    void set_output_status(char _number,bool _output);
    void set_full_screen_status(char _number,bool _output);
    void set_pushbutton_enable(bool _enable);
    void set_line_hide();
    void set_show_normal();

private:
    void resizeEvent(QResizeEvent *e);
    void show_one_status(QString _sheet,const QString &_number);


    QPushButton *m_pushbutton_play1;
    QLabel *m_show_video1;
    QLabel *m_show_number1;

    QPushButton *m_pushbutton_play2;
    QLabel *m_show_video2;
    QLabel *m_show_number2;

    QPushButton *m_pushbutton_play3;
    QLabel *m_show_video3;
    QLabel *m_show_number3;

    QPushButton *m_pushbutton_play4;
    QLabel *m_show_video4;
    QLabel *m_show_number4;

    QPushButton *m_pushbutton_play5;
    QLabel *m_show_video5;
    QLabel *m_show_number5;

    QPushButton *m_pushbutton_play6;
    QLabel *m_show_video6;
    QLabel *m_show_number6;

    QPushButton *m_pushbutton_play7;
    QLabel *m_show_video7;
    QLabel *m_show_number7;

    QPushButton *m_pushbutton_play8;
    QLabel *m_show_video8;
    QLabel *m_show_number8;

    QPushButton *m_pushbutton_play9;
    QLabel *m_show_video9;
    QLabel *m_show_number9;

        QPushButton *m_pushbutton_amplify;
    QLabel *m_show_amplify;
    QLabel *m_show_amplify_text;

    QString m_save_sheet;
    QString m_save_number;

    QWidget *m_widget_H_line1;
    QWidget *m_widget_H_line2;

    QWidget *m_widget_V_line1;
    QWidget *m_widget_V_line2;

    QString m_button_sheet1;
    QString m_button_sheet2;
    QString m_button_sheet3;
    QString m_button_sheet4;
    QString m_button_sheet5;
    QString m_button_sheet6;
    QString m_button_sheet7;
    QString m_button_sheet8;
    QString m_button_sheet9;

    QLabel *m_show_text1;
    QLabel *m_show_text2;
    QLabel *m_show_text3;
    QLabel *m_show_text4;
    QLabel *m_show_text5;
    QLabel *m_show_text6;
    QLabel *m_show_text7;
    QLabel *m_show_text8;
    QLabel *m_show_text9;

    CMyDefineScreen *m_define_screen;
    CVideoWidget *m_parent;

};
//16分频
class CVideoSixteenWidget :public QWidget
{
    Q_OBJECT
public:
    CVideoSixteenWidget(QWidget *parent = 0);

public slots:
    void slot_pushbutton_amplify();
    void slot_pushbutton_play1();
    void slot_pushbutton_play2();
    void slot_pushbutton_play3();
    void slot_pushbutton_play4();
    void slot_pushbutton_play5();
    void slot_pushbutton_play6();
    void slot_pushbutton_play7();
    void slot_pushbutton_play8();
    void slot_pushbutton_play9();
    void slot_pushbutton_play10();
    void slot_pushbutton_play11();
    void slot_pushbutton_play12();
    void slot_pushbutton_play13();
    void slot_pushbutton_play14();
    void slot_pushbutton_play15();
    void slot_pushbutton_play16();

public:
    void set_full_screen(bool _flag);
    void set_output_status(char _number,bool _output);
    void set_full_screen_status(char _number,bool _output);
    void set_show_normal();
private:

    void show_one_status(QString _sheet,const QString &_number);


    QPushButton *m_pushbutton_play1;
    QLabel *m_show_video1;
    QLabel *m_show_number1;

    QPushButton *m_pushbutton_play2;
    QLabel *m_show_video2;
    QLabel *m_show_number2;

    QPushButton *m_pushbutton_play3;
    QLabel *m_show_video3;
    QLabel *m_show_number3;

    QPushButton *m_pushbutton_play4;
    QLabel *m_show_video4;
    QLabel *m_show_number4;

    QPushButton *m_pushbutton_play5;
    QLabel *m_show_video5;
    QLabel *m_show_number5;

    QPushButton *m_pushbutton_play6;
    QLabel *m_show_video6;
    QLabel *m_show_number6;

    QPushButton *m_pushbutton_play7;
    QLabel *m_show_video7;
    QLabel *m_show_number7;

    QPushButton *m_pushbutton_play8;
    QLabel *m_show_video8;
    QLabel *m_show_number8;

    QPushButton *m_pushbutton_play9;
    QLabel *m_show_video9;
    QLabel *m_show_number9;

    QPushButton *m_pushbutton_play10;
    QLabel *m_show_video10;
    QLabel *m_show_number10;

    QPushButton *m_pushbutton_play11;
    QLabel *m_show_video11;
    QLabel *m_show_number11;

    QPushButton *m_pushbutton_play12;
    QLabel *m_show_video12;
    QLabel *m_show_number12;

    QPushButton *m_pushbutton_play13;
    QLabel *m_show_video13;
    QLabel *m_show_number13;

    QPushButton *m_pushbutton_play14;
    QLabel *m_show_video14;
    QLabel *m_show_number14;

    QPushButton *m_pushbutton_play15;
    QLabel *m_show_video15;
    QLabel *m_show_number15;

    QPushButton *m_pushbutton_play16;
    QLabel *m_show_video16;
    QLabel *m_show_number16;

    QPushButton *m_pushbutton_amplify;
    QLabel *m_show_amplify;
    QLabel *m_show_amplify_text;

    QString m_save_sheet;
    QString m_save_number;

    QWidget *m_widget_V_line1;
    QWidget *m_widget_V_line2;
    QWidget *m_widget_V_line3;
    QWidget *m_widget_H_line1;
    QWidget *m_widget_H_line2;
    QWidget *m_widget_H_line3;

    QString m_button_sheet1;
    QString m_button_sheet2;
    QString m_button_sheet3;
    QString m_button_sheet4;
    QString m_button_sheet5;
    QString m_button_sheet6;
    QString m_button_sheet7;
    QString m_button_sheet8;
    QString m_button_sheet9;
    QString m_button_sheet10;
    QString m_button_sheet11;
    QString m_button_sheet12;
    QString m_button_sheet13;
    QString m_button_sheet14;
    QString m_button_sheet15;
    QString m_button_sheet16;

    QLabel *m_show_text1;
    QLabel *m_show_text2;
    QLabel *m_show_text3;
    QLabel *m_show_text4;
    QLabel *m_show_text5;
    QLabel *m_show_text6;
    QLabel *m_show_text7;
    QLabel *m_show_text8;
    QLabel *m_show_text9;
    QLabel *m_show_text10;
    QLabel *m_show_text11;
    QLabel *m_show_text12;
    QLabel *m_show_text13;
    QLabel *m_show_text14;
    QLabel *m_show_text15;
    QLabel *m_show_text16;

};

class CVideoWidget :public CParentWidget
{
    Q_OBJECT
public:
    CVideoWidget(QWidget *parent = 0);

    enum
    {
        kEnumVideoOne = 0,      //1分频
        kEnumVideoFour = 1,     //4分频
        kEnumVideoNine = 2,     //9分频
        kEnumVideoSixteen = 3   //16分频
    };

    enum
    {
        kEnumPassengerFlowMode, //客流界面
        kEnumVideoMode          //视频界面
    };

    enum
    {
        kEnumNormalMode = 1, //普通模式
        kEnumMaxMode     //最大模式
    };
signals:
    void signal_pushbutton_home();
    void signal_pushbutton_quit();
    void signal_button_menu();

    void signal_passenger_info(const CPassengerInfo & _info);
public slots:
    //void slot_pushbutton_back();
    //void slot_pushbutton_menu();
    void slot_pushbutton_home();
    void slot_button_video(int _type);
    void slot_menu_one();
    void slot_menu_four();
    void slot_menu_nine();
    void slot_menu_sixteen();

    void slot_pushbutton_home_flow();
    void slot_pushbutton_back_flow();

    void slot_pushbutton_shrink();   //收缩
    void slot_pushbutton_open();     //展开

    void slot_pushbutton_passenger_flow();
    void slot_pushbutton_loop_playback(bool);

    void slot_vidoe_widget_event(char _frame,char _number,bool _output);
    void slot_msg_notify(int _msg,int _event,void *_param);
    void slot_timer_play_video();
public:
    void set_start_status();
    void set_main_page();
    void video_init();
    bool keyboard_value(int);
    char get_current_mode() {return m_current_mode;}
private:
    void resizeEvent(QResizeEvent *e);
    void show_video_event(int _type,int _number = 1);
    void show_video_one(int _number);      //1分频
    void show_video_four();     //4分频
    void show_video_nine();     //9分频
    void show_video_sixteen();   //16分频

    void start_play_timer(bool _start);
    bool get_current_play(char &_value);
    //void call_json_camera_set(char _frame,char _number = 1);


    CHeadWidget *m_CHeadWidget;
    CHeadWidget *m_CHeadWidget_flow;

    QStackedWidget *m_stackwidget_video;

    CVideoOneWidget *m_CVideoOneWidget;
    CVideoFourWidget *m_CVideoFourWidget;
    CVideoNineWidget *m_CVideoNineWidget;
    CVideoSixteenWidget *m_CVideoSixteenWidget;

    QMenu *m_menu;

    QPushButton *m_pushbutton_shrink;   //收缩
    QPushButton *m_pushbutton_open;     //展开
    char m_prev_video;
    char m_current_video;
    char m_current_mode;

    CMyDefineScreen *m_define_screen;
    char m_current_default_one;

    QTimer *m_timer;
    char m_current_play_video;

};

#endif // VIDEO_WIDGET_H
