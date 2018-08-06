#include "arrive_widget.h"
#include "LtyCommonDefine.h"
#include "./class/common_interface.h"
#include "protocol.h"

#include <QDebug>

#define MENU_BUTTON_GROUP_SELECTED_QSS	"\
QPushButton{\
padding: 0px 0px 0px 34px;\
text-align : left;\
font:18px;\
border: 0px;\
border-image:url(:/img/res/zj_ggk.png);\
color:#1dddff;\
}"

#define STATION_NAME_SHOW_FONT_SIZE 68

#define STATION_NAME_SHOW_LENGTH    (290)


CArriveWidget::CArriveWidget(QWidget *parent) :
    CParentWidget(parent)
{
    QString str_sheet = "";

    this->setFixedSize(1024,768);
    this->setObjectName("CArriveWidget");
    this->setStyleSheet("QDialog#CArriveWidget{background:transparent;}");//1f2024
    this->setWindowFlags(Qt::FramelessWindowHint/*|Qt::WindowStaysOnTopHint*/);


    CCommonInterface::printf_debug("CArriveWidget()");
    arrive_widget_init();
    variable_init();

}

void CArriveWidget::arrive_widget_init()
{

    m_widget_bg1 = new QWidget(this);
    m_widget_bg2 = new QWidget(this);

    m_widget_bg1->move(0,0);
    m_widget_bg1->setFixedSize(1024,768);
    m_widget_bg1->setStyleSheet("QWidget{border:0px;background:transparent;}");

    m_widget_bg2->move(0,0);
    m_widget_bg2->setFixedSize(1024,768);
    m_widget_bg2->setStyleSheet("QWidget{border:0px;background:transparent;}");


    m_widget_middle_bg = new QWidget(m_widget_bg1);
    m_widget_middle_bg->setFixedSize(1024,768);
    m_widget_middle_bg->move(0,0);
    m_widget_middle_bg->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/deputy_main_page/secondary_bg.png);}");

    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetVideo,m_widget_bg1);

    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home()));
    connect(m_CHeadWidget,SIGNAL(signal_button_back()),this,SLOT(slot_pushbutton_back()));


    m_pushbutton_station_name = new QPushButton(m_widget_bg1);

    CCommonInterface ::init_new_button(m_pushbutton_station_name,51,175,STATION_NAME_SHOW_LENGTH,90);


    m_label_station_status = new QLabel(m_widget_bg1);
    m_label_station_status->setFixedSize(80,50);
    m_label_station_status->move(51,135);
    m_label_station_status->setStyleSheet("QLabel{border:px;background:transparent;font: 30px;color:#8ea2c0; }");
    m_label_station_status->setAlignment(Qt::AlignCenter);
    m_label_station_status->setText(tr("进站"));

    m_dynamic_text_station_name  = new CDynamicText(m_widget_bg1);
    connect(m_dynamic_text_station_name,SIGNAL(signal_dynamic_text_event(CDynTextParam)),this,SLOT(slot_dynamic_text_event(CDynTextParam)));

    m_label_limit_speed1 = new QLabel(m_widget_bg1);
    m_label_limit_speed1->setFixedSize(80,50);
    m_label_limit_speed1->move(152 - 10,301);
    m_label_limit_speed1->setStyleSheet("QLabel{border:0px;background:transparent;font:bold 61px;color:#f7382a; }");
    m_label_limit_speed1->setAlignment(Qt::AlignCenter);

    m_label_limit_speed2 = new QLabel(m_widget_bg1);
    m_label_limit_speed2->setFixedSize(65,25);
    m_label_limit_speed2->move(158,354);
    m_label_limit_speed2->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#8ea2c0; }");
    m_label_limit_speed2->setText(tr("km/h"));

    m_label_current_speed = new QLabel(m_widget_bg1);
    m_label_current_speed->setFixedSize(100,70);
    m_label_current_speed->move(140 - 5,449);
    m_label_current_speed->setStyleSheet("QLabel{border:0px;background:transparent;font:bold 80px;color:#94fbff; }");
    m_label_current_speed->setAlignment(Qt::AlignCenter);

    m_label_up_vehicle1 = new QLabel(m_widget_bg1);
    m_label_up_vehicle1->setFixedSize(45,25);
    m_label_up_vehicle1->move(77,598);
    m_label_up_vehicle1->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#8ea2c0; }");
    m_label_up_vehicle1->setText(tr("上车"));

    m_label_up_vehicle2 = new QLabel(m_widget_bg1);
    m_label_up_vehicle2->setFixedSize(70,50);
    m_label_up_vehicle2->move(75 - 10,632);
    m_label_up_vehicle2->setStyleSheet("QLabel{border:0px solid #ffffff;background:transparent;font:bold 46px;color:#94c6ff; }");
    m_label_up_vehicle2->setAlignment(Qt::AlignCenter);

    m_label_down_vehicle1 = new QLabel(m_widget_bg1);
    m_label_down_vehicle1->setFixedSize(45,25);
    m_label_down_vehicle1->move(161,598);
    m_label_down_vehicle1->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#8ea2c0; }");
    m_label_down_vehicle1->setText(tr("下车"));


    m_label_down_vehicle2 = new QLabel(m_widget_bg1);
    m_label_down_vehicle2->setFixedSize(70,50);
    m_label_down_vehicle2->move(169 - 20,632);
    m_label_down_vehicle2->setStyleSheet("QLabel{border:0px  solid #ffffff;background:transparent;font:bold 46px;color:#94c6ff; }");
    m_label_down_vehicle2->setAlignment(Qt::AlignCenter);

    m_label_within_vehicle1 = new QLabel(m_widget_bg1);
    m_label_within_vehicle1->setFixedSize(45,25);
    m_label_within_vehicle1->move(248,598);
    m_label_within_vehicle1->setStyleSheet("QLabel{border:0px;background:transparent;font:23px;color:#8ea2c0; }");
    m_label_within_vehicle1->setText(tr("车内"));


    m_label_within_vehicle2 = new QLabel(m_widget_bg1);
    m_label_within_vehicle2->setFixedSize(70,50);
    m_label_within_vehicle2->move(246 - 10,632);
    m_label_within_vehicle2->setStyleSheet("QLabel{border:0px  solid #ffffff;background:transparent;font:bold 46px;color:#94c6ff; }");
    m_label_within_vehicle2->setAlignment(Qt::AlignCenter);


    m_widget_picture1 = new QWidget(m_widget_bg1);
    m_widget_picture1->setFixedSize(614,562);
    m_widget_picture1->move(370,145);
    m_widget_picture1->setStyleSheet("QWidget {border:0px;background-color:#000000;}");
    m_widget_picture1->hide();

    m_pushbutton_amplify = new QPushButton(m_widget_bg1);
    QString     str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/deputy_main_page/full_screen.png").arg(":/img/res/main_page/deputy_main_page/full_screen_hover.png").arg(":/img/res/main_page/deputy_main_page/full_screen_hover.png");

    CCommonInterface ::init_new_button(m_pushbutton_amplify,912,170,52,67,str_sheet);

    connect(m_pushbutton_amplify,SIGNAL(clicked()),this,SLOT(slot_pushbutton_amplify()));

    m_widget_picture2 = new QWidget(m_widget_bg2);
    m_widget_picture2->setFixedSize(1024,768);
    m_widget_picture2->move(0,0);
    m_widget_picture2->setStyleSheet("QWidget {border:0px;background-color:#000000;}");
    m_widget_picture2->hide();

    m_pushbutton_narrow = new QPushButton(m_widget_bg2);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
          .arg(":/img/res/main_page/deputy_main_page/Quit_full_screen.png").arg(":/img/res/main_page/deputy_main_page/Quit_full_screen_hover.png").arg(":/img/res/main_page/deputy_main_page/Quit_full_screen_hover.png");

    CCommonInterface ::init_new_button(m_pushbutton_narrow,952,26,52,67,str_sheet);

    connect(m_pushbutton_narrow,SIGNAL(clicked()),this,SLOT(slot_pushbutton_narrow()));


    m_widget_amplify_limit_speed1 = new QWidget(m_widget_bg2);
    m_widget_amplify_limit_speed1->setFixedSize(80,103);
    m_widget_amplify_limit_speed1->move(30,32);
    m_widget_amplify_limit_speed1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/deputy_main_page/Speed_limit.png);}");

    m_label_amplify_limit_speed1 = new QLabel(m_widget_bg2);
    m_label_amplify_limit_speed1->setFixedSize(80,55);
    m_label_amplify_limit_speed1->move(30,44);
    m_label_amplify_limit_speed1->setStyleSheet("QLabel{border:0px solid #000000;background:transparent;font:bold 40px;color:#444444; }");
    m_label_amplify_limit_speed1->setAlignment(Qt::AlignCenter);

    m_label_amplify_limit_speed2 = new QLabel(m_widget_bg2);
    m_label_amplify_limit_speed2->setFixedSize(45,25);
    m_label_amplify_limit_speed2->move(50 - 2,90 - 2);
    m_label_amplify_limit_speed2->setStyleSheet("QLabel{border:0px;background:transparent;font:18px;color:#444444; }");
    m_label_amplify_limit_speed2->setText(tr("km/h"));


    m_widget_amplify_speed1 = new QWidget(m_widget_bg2);
    m_widget_amplify_speed1->setFixedSize(100,129);
    m_widget_amplify_speed1->move(20,147);
    m_widget_amplify_speed1->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/deputy_main_page/Speed.png);}");

    m_label_amplify_speed1 = new QLabel(m_widget_bg2);
    m_label_amplify_speed1->setFixedSize(100,80);
    m_label_amplify_speed1->move(33 - 13,182 - 10);
    m_label_amplify_speed1->setStyleSheet("QLabel{border:0px solid #000000;background:transparent;font:bold 60px;color:#0a7cde; }");
    m_label_amplify_speed1->setAlignment(Qt::AlignCenter);

    m_widget_bg2->hide();

}

void CArriveWidget::variable_init()
{
    m_current_show_mode = kEnumCurrentStation;
}

void CArriveWidget::slot_pushbutton_amplify()
{
    m_widget_bg1->hide();
    m_widget_bg2->show();
    //m_widget_picture2->show();
    m_widget_picture2->hide();
    current_max_screen = true;
    call_json_data(true);

}

void CArriveWidget::slot_pushbutton_narrow()
{
    m_widget_bg1->show();
    m_widget_bg2->hide();
    //m_widget_picture1->show();
    m_widget_picture1->hide();
    current_max_screen = false;
    call_json_data(false);
}

void CArriveWidget::slot_pushbutton_home()
{
    //close();
    emit signal_pushbutton_home();
}

void CArriveWidget::slot_pushbutton_back()
{
    emit signal_pushbutton_home();
}

void CArriveWidget::slot_pushbutton_arrow()
{
    m_CSteeringWheel->hide();
    m_label_video->setFixedSize(1024,704);
    m_label_video->move(0,64);
    m_label_video->setStyleSheet("QLabel{border:0px;border-image:url(:/img/res/video2.png);}");
    m_pushbutton_arrow->hide();
    m_pushbutton_arrow_left->show();
    m_pushbutton_station_name->hide();

    m_widget_up_vehicle->move(10,768 - 15 - 52);
    m_label_up_vehicle1->move(10 + (120 - 35 - 48)/2,768 - 15 - 52 + (52 - 23)/2);
    m_label_up_vehicle2->move(10 + (120 - 35 - 48)/2 + 35 + 7,768 - 15 - 52 + (52 - 28)/2);
    m_widget_down_vehicle->move(10 + (120 + 6),768 - 15 - 52);
    m_label_down_vehicle1->move(10 + (120 - 35 - 48)/2 + (120 + 6) * 1,768 - 15 - 52 + (52 - 23)/2);
    m_label_down_vehicle2->move(10 + (120 - 35 - 48)/2 + 35 + 7 + (120 + 6) * 1,768 - 15 - 52 + (52 - 28)/2);
    m_widget_within_vehicle->move(10 + (120 + 6) * 2,768 - 15 - 52);
    m_label_within_vehicle1->move(10 + (120 - 35 - 48)/2  + (120 + 6) * 2,768 - 15 - 52 + (52 - 23)/2);
    m_label_within_vehicle2->move(10 + (120 - 35 - 48)/2 + 35 + 7  + (120 + 6) * 2,768 - 15 - 52 + (52 - 28)/2);
}
void CArriveWidget::slot_pushbutton_arrow_left()
{
    m_CSteeringWheel->show();
    m_label_video->setFixedSize(758,704);
    m_label_video->move(266,64);
    m_label_video->setStyleSheet("QLabel{border:0px;border-image:url(:/img/res/video1.jpg);}");
    m_pushbutton_arrow->show();
    m_pushbutton_arrow_left->hide();
    m_pushbutton_station_name->show();

    m_widget_up_vehicle->move(266 + 10,768 - 15 - 52);
    m_label_up_vehicle1->move(266 + 10 + (120 - 35 - 48)/2,768 - 15 - 52 + (52 - 23)/2);
    m_label_up_vehicle2->move(266 + 10 + (120 - 35 - 48)/2 + 35 + 7,768 - 15 - 52 + (52 - 28)/2);
    m_widget_down_vehicle->move(266 + 10 + (120 + 6),768 - 15 - 52);
    m_label_down_vehicle1->move(266 + 10 + (120 - 35 - 48)/2 + (120 + 6) * 1,768 - 15 - 52 + (52 - 23)/2);
    m_label_down_vehicle2->move(266 + 10 + (120 - 35 - 48)/2 + 35 + 7 + (120 + 6) * 1,768 - 15 - 52 + (52 - 28)/2);
    m_widget_within_vehicle->move(266 + 10 + (120 + 6) * 2,768 - 15 - 52);
    m_label_within_vehicle1->move(266 + 10 + (120 - 35 - 48)/2  + (120 + 6) * 2,768 - 15 - 52 + (52 - 23)/2);
    m_label_within_vehicle2->move(266 + 10 + (120 - 35 - 48)/2 + 35 + 7  + (120 + 6) * 2,768 - 15 - 52 + (52 - 28)/2);
}


void CArriveWidget::slot_dynamic_text_event(const CDynTextParam &_param)
{
    QString str_sheet = "";
    switch(_param.m_control_id)
    {
        case CArriveWidget::kEnumPushbuttonStationName:
        {
            switch(_param.m_action)
            {
                case CDynamicText::kEnumAlignmentLeft: //文字左对齐
                {

                       #ifdef CURRENT_ARM_RUN
                       str_sheet = QString("QPushButton {text-align:left;border:0px;background:transparent;font:bold 68px;color:#ffffff; }");
                       #else
                       str_sheet = QString("QPushButton {text-align:left;border:0px;background:transparent;font: 68px;color:#ffffff; }");
                       #endif
                       CCommonInterface::modify_button_sheet((QPushButton *)_param.m_control_this,str_sheet);
                }break;
                case CDynamicText::kEnumAlignmentRight://文字居中对齐
                {
                    #ifdef CURRENT_ARM_RUN
                    str_sheet = QString("QPushButton {text-align:center;border:0px;background:transparent;font:bold 68px;color:#ffffff; }");
                    #else
                    str_sheet = QString("QPushButton {text-align:center;border:0px;background:transparent;font: 68px;color:#ffffff; }");
                    #endif
                    CCommonInterface::modify_button_sheet((QPushButton *)_param.m_control_this,str_sheet);
                }break;
                case CDynamicText::kEnumUpdateText:    //更新文本显示
                {
                    m_pushbutton_station_name->setText(_param.m_control_show_text);

                }break;
            }
        }break;
    }
}

void CArriveWidget::slot_arrive_widget_event(int _type,int _param1,QString _param2)
{
    switch(_type)
    {
        case kEnumSetStationName://更新站名
        {
            CDynTextParam param_station_name;
            param_station_name.m_control_this = m_pushbutton_station_name;
            param_station_name.m_control_type = CDynamicText::kEnumPushbutton;
            param_station_name.m_control_id   = kEnumPushbuttonStationName;
            param_station_name.m_control_source_text = _param2;
            param_station_name.m_control_font_size = STATION_NAME_SHOW_FONT_SIZE;
            param_station_name.m_control_show_length = STATION_NAME_SHOW_LENGTH;

            m_dynamic_text_station_name->set_text(param_station_name);

        }break;
        case kEnumSetLimitSpeed://更新限速
        {
            m_label_limit_speed1->setText(QString::number(_param1));
            m_label_amplify_limit_speed1->setText(QString::number(_param1));
        }break;
        case kEnumSetCurrentSpeed://更新当前速度
        {
            m_label_current_speed->setText(QString::number(_param1));
            m_label_amplify_speed1->setText(QString::number(_param1));
        }break;
        case kEnumSetUpVehicle://更新上车
        {
            m_label_up_vehicle2->setText(QString::number(_param1));
        }break;
        case kEnumSetDownVehicle:  //更新下车
        {
            m_label_down_vehicle2->setText(QString::number(_param1));
        }break;
        case kEnumSetWithinVehicle: //更新车内
        {
            m_label_within_vehicle2->setText(QString::number(_param1));
        }break;
        case kEnumSetVehicleStatus:
        {
            m_label_station_status->setText(_param2);
        }break;
    }
}



void CArriveWidget::slot_video_notify_event(char _frame,char _number,bool _output)
{
    switch(_frame)
    {
        case CProtocol::kEnumFrame1:      //1画面
        {
            if(_output)
                m_widget_picture2->hide();
            else
                m_widget_picture2->show();

        }break;
        case CProtocol::kEnumFrameDefine: //自定义1画面的位置
        {
           if(_output)
                m_widget_picture1->hide();
            else
                m_widget_picture1->show();

        }break;
    }
}


void CArriveWidget::call_json_data(bool _flag)//true:放大 false:缩小
{
    if(_flag)
    {
        CVideoOutput _videoOutput;
        _videoOutput.m_frame = CProtocol::kEnumFrame1;
        if(is_back_car_show_mode())
            _videoOutput.m_number = CCommonData::getInstance()->m_CDriverMenuInfo.back_car_video_passageway + 1;
        else
            _videoOutput.m_number = CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway + 1;
        CProtocol::getInstance()->call_json_camera_set(_videoOutput);
    }
    else
    {
        CVideoOutput _videoOutput;
        _videoOutput.m_frame = CProtocol::kEnumFrameDefine;
        _videoOutput.m_number = CCommonData::getInstance()->m_CDriverMenuInfo.out_door_video_passageway + 1;
        _videoOutput.m_x = 370;
        _videoOutput.m_y = 145;
        _videoOutput.m_width = 614;
        _videoOutput.m_height = 562;
        CProtocol::getInstance()->call_json_camera_set(_videoOutput);
    }
}

bool CArriveWidget::keyboard_value(int _value)
{
    bool ret_value = false;
    switch(_value)
    {
        case KEYPAD_KEY7_F0:
        {

        }break;
        case KEYPAD_KEY7_F1:
        {

        }break;
        case KEYPAD_KEY7_F2:
        {

        }break;
        case KEYPAD_KEY7_F3:
        {

        }break;
        case KEYPAD_KEY7_F4:
        {

        }break;
        case KEYPAD_KEY7_F5:
        {

        }break;
        case KEYPAD_KEY7_F6:
        {

        }break;
        case KEYPAD_KEY7_F7:
        {

        }break;
        case KEYPAD_KEY7_F8:
        {

        }break;
        case KEYPAD_KEY7_F9:
        {

        }break;
        case KEYPAD_KEY7_PLAY:
        {

        }break;
        case KEYPAD_KEY7_REPLAY:
        {

        }break;
        case KEYPAD_KEY7_MENU:
        {

        }break;
        case KEYPAD_KEY7_EXIT:
        {
            if(is_back_car_show_mode())
                slot_back_car_widget(false);
            else
                slot_pushbutton_back();

            ret_value = true;
        }break;
        case KEYPAD_KEY7_FUN:
        {

        }break;
        case KEYPAD_KEY7_UP:
        {

        }break;
        case KEYPAD_KEY7_LEFT:
        {

        }break;
        case KEYPAD_KEY7_RIGHT:
        {

        }break;
        case KEYPAD_KEY7_DOWN:
        {

        }break;
    }
    return ret_value;
}

void CArriveWidget::data_init()
{

    if(1 == CCommonData::getInstance()->m_CDriverMenuInfo.deputy_page_show_mode)
    {
        current_max_screen = true;
        slot_pushbutton_amplify();
    }
    else
    {
        current_max_screen = false;
        slot_pushbutton_narrow();
    }


}

bool CArriveWidget::is_back_car_show_mode()
{
    return (kEnumCurrentBackCar == m_current_show_mode);
}

void CArriveWidget::slot_back_car_widget(bool _show)
{
    if(_show)
    {
        m_pushbutton_narrow->hide();
        m_current_show_mode = kEnumCurrentBackCar;
    }
    else
    {
        m_pushbutton_narrow->show();
        m_current_show_mode = kEnumCurrentStation;
        slot_pushbutton_back();
    }
}
















