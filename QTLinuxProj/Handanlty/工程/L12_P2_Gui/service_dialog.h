#ifndef SERVICE_DIALOG_H
#define SERVICE_DIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QWidget>
#include <QTimer>

#include <QMouseEvent>

#include "class/common/parent_widget.h"


class CServiceDialog : public CParentWidget
{
    Q_OBJECT
public:
    explicit CServiceDialog(QWidget *parent = 0);
    enum
    {
        kEnumPlaySetValue = 1,  //播放值
        kEnumPlaySetType = 2    //播放类型
    };

    enum
    {
        kEnumOutTip = 1,  //外间提醒
        kEnumInWord = 2,  //内喊话
        kEnumOutWord = 3  //外喊话
    };

signals:
    
public slots:
    void slot_pushbutton_close();
    void slot_timer_close_window();
    void slot_pushbutton_1();
    void slot_pushbutton_2();
    void slot_pushbutton_3();
    void slot_pushbutton_4();
    void slot_pushbutton_5();
    void slot_pushbutton_6();
    void slot_pushbutton_7();
    void slot_pushbutton_8();
    void slot_pushbutton_9();
    void slot_pushbutton_10();
    void slot_pushbutton_11();
    void slot_pushbutton_12();
    void call_json_interface(char _action,char _value);
private:

    void reset_close_timer();
private:
    bool keyboard_value(int _value);

    QPushButton *m_pushbutton_close;
    QWidget *m_widget_background;

    QPushButton *m_pushbutton_1;
    QPushButton *m_pushbutton_2;
    QPushButton *m_pushbutton_3;
    QPushButton *m_pushbutton_4;
    QPushButton *m_pushbutton_5;
    QPushButton *m_pushbutton_6;
    QPushButton *m_pushbutton_7;
    QPushButton *m_pushbutton_8;
    QPushButton *m_pushbutton_9;
    QPushButton *m_pushbutton_10;
    QPushButton *m_pushbutton_11;
    QPushButton *m_pushbutton_12;

    QTimer *m_timer;


    bool  m_bLeftButtonPress;
    QPoint m_ptPress;
    QPoint m_ptMove;

};

#endif // SERVICE_DIALOG_H
