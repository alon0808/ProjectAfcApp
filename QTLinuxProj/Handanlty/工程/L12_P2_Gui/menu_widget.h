#ifndef MENU_WIDGET_H
#define MENU_WIDGET_H

#include <QWidget>
#include <QDialog>

#include <QPushButton>
#include <QLabel>
#include <QToolButton>
#include <QEvent>

#include "head_widget.h"
#include "class/common/define_screen.h"
#include "class/common/parent_widget.h"

class CMenuWidget : public  CParentWidget //QWidget
{
    Q_OBJECT
public:
    explicit CMenuWidget(QDialog *parent = 0);
    
signals:
    void signal_menu_widget_message_upload();
    void signal_menu_widget_run_status();
    void signal_pushbutton_home();

    void signal_pushbutton_message();
    void signal_pushbutton_camera_query();
    void signal_pushbutton_driver_menu();
    void signal_pushbutton_system_manager();
    void signal_pushbutton_run_status();
    void signal_pushbutton_about_machine();
    void signal_pushbutton_passenger_flow();
    void signal_pushbutton_pay_info();
    void signal_pushbutton_passenger();
public slots:
    void slot_pushbutton_home();
    void slot_pushbutton_back();

    void slot_pushbutton_message();
    void slot_pushbutton_camera_query();
    void slot_pushbutton_driver_menu();
    void slot_pushbutton_system_manager();
    void slot_pushbutton_run_status();
    void slot_pushbutton_about_machine();
    void slot_pushbutton_passenger_flow();
    void slot_pushbutton_pay_info();
    void slot_pushbutton_passenger();

    void slot_user_exit_password();

public:
    void set_first_focus();
    bool keyboard_value(int _value);
private:
    void create_new_button(QPushButton *_button,QLabel*_label,int _x,int _y,int _w,int _h,QString &_typeSheet,QString _text);
    void resizeEvent(QResizeEvent *e);

    bool get_current_focus();

    QPushButton *m_pushbutton_pay_info;
    QPushButton *m_pushbutton_passenger;
    QPushButton *m_pushbutton_message;
    QPushButton *m_pushbutton_call;
    QPushButton *m_pushbutton_driver_menu;
    QPushButton *m_pushbutton_system_manager;
    QPushButton *m_pushbutton_run_status;
    QPushButton *m_pushbutton_about_machine;

    QPushButton *m_pushbutton_passenger_flow;

    QLabel *m_label_pay_info;
    QLabel *m_label_passenger;
    QLabel *m_label_message;
    QLabel *m_label_call;
    QLabel *m_label_driver_menu;
    QLabel *m_label_system_manager;
    QLabel *m_label_run_status;
    QLabel *m_label_about_machine;

    QLabel *m_label_passenger_flow;

    CHeadWidget *m_CHeadWidget;

    CMyDefineScreen *m_define_screen;

    bool m_login_success;
    QTime *m_login_success_tick;

};

#endif // MENU_WIDGET_H
