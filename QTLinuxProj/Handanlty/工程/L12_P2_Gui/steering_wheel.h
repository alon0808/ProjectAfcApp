#ifndef STEERING_WHEEL_H
#define STEERING_WHEEL_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class CSteeringWheel : public QWidget
{
    Q_OBJECT
public:
    explicit CSteeringWheel(QWidget *parent = 0);
    
signals:
    void signal_pushbutton_door_1();
    void signal_pushbutton_door_2();
public slots:

public:
    void steering_wheel_notify(int _msg,int _event,void *_param);
private:
    ////////////////////////////////////////////////////////中间右边显示
    //QWidget *m_widget_middle_right;
    QLabel*m_label_speed1; //限速
    QLabel*m_label_speed2; //速度
    QLabel*m_label_speed3; // km/h

    QLabel*m_label_speed4; // 速度
    QLabel*m_label_speed5; // km/h

    QLabel*m_label_speed6;//超速报警图标
    QLabel*m_label_speed7;//超速报警

    QPushButton *m_pushbutton_door_1;//前门
    QPushButton *m_pushbutton_door_2;//后门

    QWidget *m_widget_head;

};

#endif // STEERING_WHEEL_H
