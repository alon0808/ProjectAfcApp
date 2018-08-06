#ifndef ARRIVE_WIDGET_H
#define ARRIVE_WIDGET_H

#include <QDialog>

#include <QPushButton>
#include <QLabel>

#include "head_widget.h"
#include "steering_wheel.h"

#include "dynamic_text.h"
#include "class/common/parent_widget.h"

class CArriveWidget : public CParentWidget
{
    Q_OBJECT
public:
    explicit CArriveWidget(QWidget *parent = 0);
    enum
    {
        kEnumPushbuttonStationName  //站名显示
    };
    enum
    {
        kEnumSetStationName,    //更新站名
        kEnumSetLimitSpeed,     //更新限速
        kEnumSetCurrentSpeed,   //更新当前速度
        kEnumSetUpVehicle,      //更新上车
        kEnumSetDownVehicle,    //更新下车
        kEnumSetWithinVehicle,  //更新车内
        kEnumSetVehicleStatus   //进站　出站
    };
    void data_init();
    enum
    {
        kEnumCurrentStation,    //进站模式
        kEnumCurrentBackCar     //倒车
    };
signals:
    void signal_pushbutton_home();
public slots:
    void slot_pushbutton_home();
    void slot_pushbutton_back();
    void slot_pushbutton_arrow();
    void slot_pushbutton_arrow_left();
    void slot_pushbutton_amplify();
    void slot_pushbutton_narrow();

    void slot_dynamic_text_event(const CDynTextParam &_param);

    void slot_arrive_widget_event(int _type,int _param1,QString _param2);

    void slot_video_notify_event(char _frame,char _number,bool _output);
    bool keyboard_value(int _value);
    void slot_back_car_widget(bool _show);
public:
    bool is_back_car_show_mode();
private:
    void arrive_widget_init();
    void variable_init();
    void call_json_data(bool _flag);//true:放大 false:缩小


/////////////////////////////////////////////////////////new start
    QWidget *m_widget_middle_bg;

    QLabel *m_label_limit_speed1;
    QLabel *m_label_limit_speed2;
    QLabel *m_label_current_speed;

    QPushButton *m_pushbutton_amplify;  //放大
    QPushButton *m_pushbutton_narrow;   //缩小

    QWidget *m_widget_picture1; //缩小图
    QWidget *m_widget_picture2;//放大图

    QWidget *m_widget_amplify_limit_speed1;
    QLabel *m_label_amplify_limit_speed1;
    QLabel *m_label_amplify_limit_speed2;

    QWidget *m_widget_amplify_speed1;
    QLabel *m_label_amplify_speed1;

    QLabel *m_label_station_status;

    QWidget *m_widget_bg1;
    QWidget *m_widget_bg2;

/////////////////////////////////////////////////////////new end
    QPushButton *m_pushbutton_back;
    CHeadWidget *m_CHeadWidget;

    CSteeringWheel *m_CSteeringWheel;

    QLabel *m_label_video;
    QPushButton *m_pushbutton_arrow;
    QPushButton *m_pushbutton_arrow_left;

    QWidget *m_widget_up_vehicle;       //上车
    QWidget *m_widget_down_vehicle;     //下车
    QWidget *m_widget_within_vehicle;   //车内

    QLabel *m_label_up_vehicle1;
    QLabel *m_label_up_vehicle2;

    QLabel *m_label_down_vehicle1;
    QLabel *m_label_down_vehicle2;

    QLabel *m_label_within_vehicle1;
    QLabel *m_label_within_vehicle2;

    QLabel *m_label_current_station; //本站
    QPushButton *m_pushbutton_station_name;

    CDynamicText *m_dynamic_text_station_name;
    bool current_max_screen;

    char m_current_show_mode;

};

#endif // ARRIVE_WIDGET_H
