#include "service_dialog.h"
#include "./class/common_interface.h"

#include "LtyCommonDefine.h"

CServiceDialog::CServiceDialog(QWidget *parent) :
    CParentWidget(parent)
{
    this->setFixedSize(723,551);
    this->setObjectName("CServiceDialog");
    this->setStyleSheet("CServiceDialog{background:transparent;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    m_widget_background = new QWidget(this);
    m_widget_background->setFixedSize(712,542);
    m_widget_background->move((723 - 712)/2,(551 - 542)/2);
    m_widget_background->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/popup_dialog/service_bg.png);}");

    QString str_sheet = "";
    m_pushbutton_close = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/popup_dialog/close.png");
    CCommonInterface ::init_new_button(m_pushbutton_close,this->width() - 62,0,62,66,str_sheet);
    connect(m_pushbutton_close,SIGNAL(clicked()),SLOT(slot_pushbutton_close()));

    int start_x = 60 + 4;
    int start_y = 70 - 3;

    str_sheet = "QPushButton {border:0px solid #ffffff;font:38px;background:transparent;color:#bef1ff;}\
        QPushButton:pressed {background:#1c1f26;}\
        QPushButton:hover:pressed {background:#444758;}";

    m_pushbutton_1 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_1,start_x,start_y,198,95,str_sheet);
    m_pushbutton_1->setText(tr("1"));

    m_pushbutton_2 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_2,start_x + 198 * 1,start_y,198,95,str_sheet);
    m_pushbutton_2->setText(tr("2"));

    m_pushbutton_3 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_3,start_x + 198 * 2,start_y,198,95,str_sheet);
    m_pushbutton_3->setText(tr("3"));

    m_pushbutton_4 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_4,start_x,start_y + 95,198,95,str_sheet);
    m_pushbutton_4->setText(tr("4"));

    m_pushbutton_5 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_5,start_x + 198 * 1,start_y + 95,198,95,str_sheet);
    m_pushbutton_5->setText(tr("5"));

    m_pushbutton_6 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_6,start_x + 198 * 2,start_y + 95,198,95,str_sheet);
    m_pushbutton_6->setText(tr("6"));

    m_pushbutton_7 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_7,start_x,start_y + 95 * 2,198,95,str_sheet);
    m_pushbutton_7->setText(tr("7"));

    m_pushbutton_8 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_8,start_x + 198 * 1,start_y + 95 * 2,198,95,str_sheet);
    m_pushbutton_8->setText(tr("8"));

    m_pushbutton_9 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_9,start_x + 198 * 2,start_y + 95 * 2,198,95,str_sheet);
    m_pushbutton_9->setText(tr("9"));
#ifdef CURRENT_ARM_RUN
    str_sheet = "QPushButton {border:0px solid #ffffff;font:bold 38px;background:transparent;color:#d3ff85;}\
    QPushButton:pressed {background:#1c1f26;}\
    QPushButton:hover:pressed {background:#444758;}";
#else
    str_sheet = "QPushButton {border:0px solid #ffffff;font:38px;background:transparent;color:#d3ff85;}\
    QPushButton:pressed {background:#1c1f26;}\
    QPushButton:hover:pressed {background:#444758;}";
#endif
    m_pushbutton_10 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_10,start_x,start_y + 95 * 3,198,132,str_sheet);
    m_pushbutton_10->setText(tr("外音提醒"));

#ifdef CURRENT_ARM_RUN
    str_sheet = "QPushButton {border:0px solid #ffffff;font:bold 38px;background:transparent;color:#ffdb85;}\
    QPushButton:pressed {background:#1c1f26;}\
    QPushButton:hover:pressed {background:#444758;}";
#else
    str_sheet = "QPushButton {border:0px solid #ffffff;font:38px;background:transparent;color:#ffdb85;}\
    QPushButton:pressed {background:#1c1f26;}\
    QPushButton:hover:pressed {background:#444758;}";
#endif

    m_pushbutton_11 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_11,start_x + 198 * 1,start_y + 95 * 3,198,132,str_sheet);
    m_pushbutton_11->setText(tr("内喊话"));

    m_pushbutton_12 = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_12,start_x + 198 * 2,start_y + 95 * 3,198,132,str_sheet);
    m_pushbutton_12->setText(tr("外喊话"));



    m_timer = new QTimer(this);
    m_timer->setInterval(10 * 1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timer_close_window()));
    m_timer->start();

    connect(m_pushbutton_1,SIGNAL(clicked()),this,SLOT(slot_pushbutton_1()));
    connect(m_pushbutton_2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_2()));
    connect(m_pushbutton_3,SIGNAL(clicked()),this,SLOT(slot_pushbutton_3()));
    connect(m_pushbutton_4,SIGNAL(clicked()),this,SLOT(slot_pushbutton_4()));
    connect(m_pushbutton_5,SIGNAL(clicked()),this,SLOT(slot_pushbutton_5()));
    connect(m_pushbutton_6,SIGNAL(clicked()),this,SLOT(slot_pushbutton_6()));
    connect(m_pushbutton_7,SIGNAL(clicked()),this,SLOT(slot_pushbutton_7()));
    connect(m_pushbutton_8,SIGNAL(clicked()),this,SLOT(slot_pushbutton_8()));
    connect(m_pushbutton_9,SIGNAL(clicked()),this,SLOT(slot_pushbutton_9()));
    connect(m_pushbutton_10,SIGNAL(clicked()),this,SLOT(slot_pushbutton_10()));
    connect(m_pushbutton_11,SIGNAL(clicked()),this,SLOT(slot_pushbutton_11()));
    connect(m_pushbutton_12,SIGNAL(clicked()),this,SLOT(slot_pushbutton_12()));


}

void CServiceDialog::slot_pushbutton_close()
{
    close();
}

void CServiceDialog::slot_timer_close_window()
{
    m_timer->stop();
    slot_pushbutton_close();
}


void CServiceDialog::slot_pushbutton_1()
{
    call_json_interface(kEnumPlaySetValue,1);
    reset_close_timer();
}
void CServiceDialog::slot_pushbutton_2()
{
    call_json_interface(kEnumPlaySetValue,2);
    reset_close_timer();
}
void CServiceDialog::slot_pushbutton_3()
{
    call_json_interface(kEnumPlaySetValue,3);
    reset_close_timer();
}
void CServiceDialog::slot_pushbutton_4()
{
    call_json_interface(kEnumPlaySetValue,4);
    reset_close_timer();
}
void CServiceDialog::slot_pushbutton_5()
{
    call_json_interface(kEnumPlaySetValue,5);
    reset_close_timer();
}
void CServiceDialog::slot_pushbutton_6()
{
    call_json_interface(kEnumPlaySetValue,6);
    reset_close_timer();
}
void CServiceDialog::slot_pushbutton_7()
{
    call_json_interface(kEnumPlaySetValue,7);
    reset_close_timer();
}
void CServiceDialog::slot_pushbutton_8()
{
    call_json_interface(kEnumPlaySetValue,8);
    reset_close_timer();
}
void CServiceDialog::slot_pushbutton_9()
{
    call_json_interface(kEnumPlaySetValue,9);
    reset_close_timer();
}

void CServiceDialog::slot_pushbutton_10()
{
    call_json_interface(kEnumPlaySetType,kEnumOutTip);
}
void CServiceDialog::slot_pushbutton_11()
{
    call_json_interface(kEnumPlaySetType,kEnumInWord);
}
void CServiceDialog::slot_pushbutton_12()
{
    call_json_interface(kEnumPlaySetType,kEnumOutWord);
}

void CServiceDialog::reset_close_timer()
{
    if(m_timer->isActive())
    {
       m_timer->stop();
    }
    m_timer->start();
}
//////////////////////////////////////////////////////////////////////////////////////////
//数据交互区

#include "class/cJSON.h"
#include "class/ltyuicallinterface.h"

void CServiceDialog::call_json_interface(char _action,char _value)
{
    do
    {
        cJSON *root = NULL;
        root = cJSON_CreateObject();

        if(!root)
        {
            break;
        }
        cJSON_AddStringToObject(root, "msg_type", "gui_service_term");/*服务用语*/
        switch(_action)
        {
            case kEnumPlaySetValue:  //播放值
            {
                cJSON_AddNumberToObject(root, "type", kEnumPlaySetValue);
                cJSON_AddNumberToObject(root, "value", _value);
            }break;
            case kEnumPlaySetType:    //播放类型
            {
                cJSON_AddNumberToObject(root, "type", kEnumPlaySetType);
                cJSON_AddNumberToObject(root, "value", _value);
            }break;
        }

        gui_send_message2system(root);

        //cJSON_Delete(root);

    }while(0);
}

bool CServiceDialog::keyboard_value(int _value)
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
            slot_pushbutton_close();
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



