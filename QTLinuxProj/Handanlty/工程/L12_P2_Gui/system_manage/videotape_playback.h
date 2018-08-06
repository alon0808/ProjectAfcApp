#ifndef VIDEOTAPE_PLAYBACK_H
#define VIDEOTAPE_PLAYBACK_H

#include <QDialog>

#include "head_widget.h"
#include "./class/common/my_pushbutton.h"

class CVideotapePlayback : public QDialog
{
    Q_OBJECT
public:
    explicit CVideotapePlayback(QWidget *parent = 0);
    enum
    {
        kEnumButtonBack,
        kEnumButtonHome
    };
signals:
    void signal_button_back();
    void signal_button_home();
    void signal_head_home_back_show(bool);
public slots:
    void slot_button_back();
    void slot_button_home();

    void slot_pushbutton_1();
    void slot_pushbutton_2();
    void slot_pushbutton_3();  //暂停　播放
    void slot_pushbutton_4();
    void slot_pushbutton_5();
    void slot_pushbutton_6();//静音　不静音
    void slot_pushbutton_7();
    void slot_pushbutton_8();
    void slot_pushbutton_prev_passageway(); //上一个通道
    void slot_pushbutton_next_passageway(); //下一个通道

    void slot_videotape_query_playback(int _value);

    void slot_pushbutton_amplify();
    void slot_playback_quit();
    void slot_playback_notify_event(char _play_type,char _value);
public:
    char get_click_button_type();
    bool goto_main_page() {return m_goto_main_page;}
    void playback_init();
private:
    CHeadWidget *m_CHeadWidget;
    char m_type;

    QWidget *m_widget_bottom1;
    QWidget *m_widget_bottom2;

    QLabel *m_label_play_status;

    QWidget *m_widget_line1;
    QWidget *m_widget_line2;

    QPushButton *m_pushbutton_1;
    QPushButton *m_pushbutton_2;
    QPushButton *m_pushbutton_3;
    QPushButton *m_pushbutton_4;
    QPushButton *m_pushbutton_5;

    QPushButton *m_pushbutton_6;
    bool m_play_or_stop;  //true:暂停　false 播放
    bool m_sound;  //true:静音　false 不静音

    QPushButton *m_pushbutton_7;//全屏
    QPushButton *m_pushbutton_8;//不全屏

    QPushButton *m_pushbutton_amplify;

    QPushButton *m_pushbutton_prev_passageway; //上一个通道
    QPushButton *m_pushbutton_next_passageway; //下一个通道

    bool m_goto_main_page;
};

#endif // VIDEOTAPE_PLAYBACK_H








