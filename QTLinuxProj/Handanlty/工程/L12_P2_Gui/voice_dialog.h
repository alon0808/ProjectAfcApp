#ifndef VOICE_DIALOG_H
#define VOICE_DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QSlider>
#include "class/common/parent_widget.h"

class CVoiceDialog : public CParentWidget
{
    Q_OBJECT
public:
    explicit CVoiceDialog(QWidget *parent = 0);
    static CVoiceDialog *getInstance();
    static bool getIsEmpty();
    enum
    {
        kEnumVolumeSet = 1,  /*1:设置声音*/
        kEnumVolumePlay = 2, /*2:播放声音*/
        kEnumVolumeGet = 3   /*3:获取声音*/
    };
    enum
    {/*1:内音量 2:外音量 3:TTS音*/
        kEnumPlayWithin = 1,
        kEnumPlayOut = 2,
        kEnumPlayTTS = 3
    };

signals:

public slots:
    void slot_pushbutton_close();
    void slot_value_change_slider1(int);
    void slot_value_change_slider2(int);
    void slot_value_change_slider3(int);

    void slot_pushbutton_voice1();
    void slot_pushbutton_voice2();
    void slot_pushbutton_voice3();

    void slot_voice_dialog_event(int _value1,int _value2,int _value3);

public:
    void data_init();
    bool keyboard_value(int _value);
private:
    void call_json_interface(char _action,char _play);

private:
    QPushButton *m_pushbutton_close;
    QWidget *m_widget_background;

    QWidget *m_widget_voice_background1;
    QWidget *m_widget_voice_background2;
    QWidget *m_widget_voice_background3;

    QLabel *m_label_voice_icon1;
    QLabel *m_label_voice_icon2;
    QLabel *m_label_voice_icon3;

    QLabel *m_label_voice_icon4;
    QLabel *m_label_voice_icon5;
    QLabel *m_label_voice_icon6;

    QPushButton *m_pushbutton_voice1;
    QPushButton *m_pushbutton_voice2;
    QPushButton *m_pushbutton_voice3;

    QLabel *m_label_voice_icon10;
    QLabel *m_label_voice_icon11;
    QLabel *m_label_voice_icon12;

    QSlider *m_slider1;
    QSlider *m_slider2;
    QSlider *m_slider3;

    int m_value1;
    int m_value2;
    int m_value3;

    static CVoiceDialog *m_single;

};

#endif // VOICE_DIALOG_H
