#include "menu_widget.h"


#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   768



#include "main_page.h"

#include "class/common_interface.h"
#include "LtyCommonDefine.h"
#include "class/common/password1_dialog.h"
#include "class/ltystring.h"
#include "include/myinputpanelcontext.h"

CMenuWidget::CMenuWidget(QDialog *parent) :
    CParentWidget(parent)
{
    this->setFixedSize(1024,768);
    this->setObjectName("CMenuWidget");
    this->setStyleSheet("QDialog#CMenuWidget{background-color:#1f2024;}");
    this->setWindowFlags(Qt::FramelessWindowHint/*|Qt::WindowStaysOnTopHint*/);

    m_define_screen = new CMyDefineScreen();

    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetMenu,this);
    connect(m_CHeadWidget,SIGNAL(signal_button_back()),this,SLOT(slot_pushbutton_back()));
    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home()));

    QString str_sheet = "";

#ifdef APPLICATION_TYPE_P2
    m_pushbutton_pay_info = new QPushButton(this);
    m_label_pay_info = new QLabel(this);

    m_define_screen->ignore_widget(m_pushbutton_pay_info);
    m_define_screen->ignore_widget(m_label_pay_info);

    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
    QPushButton:focus{border:0px solid #ffffff;border-image:url(%4);}")
        .arg(":/img_p2/res/p2/icon_operate_default.png").arg(":/img_p2/res/p2/icon_operate_pressed.png").arg(":/img_p2/res/p2/icon_operate_pressed.png").arg(":/img_p2/res/p2/icon_operate_pressed.png");

    create_new_button(m_pushbutton_pay_info,m_label_pay_info,100,103,104,104,str_sheet,tr("交易信息"));

    m_pushbutton_passenger = new QPushButton(this);
    m_label_passenger = new QLabel(this);

    m_define_screen->ignore_widget(m_pushbutton_passenger);
    m_define_screen->ignore_widget(m_label_passenger);

    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
    QPushButton:focus{border:0px solid #ffffff;border-image:url(%4);}")
        .arg(":/img_p2/res/p2/icon_statistics_default.png").arg(":/img_p2/res/p2/icon_statistics_pressed.png").arg(":/img_p2/res/p2/icon_statistics_pressed.png").arg(":/img_p2/res/p2/icon_statistics_pressed.png");

    create_new_button(m_pushbutton_passenger,m_label_passenger,268,103,104,104,str_sheet,tr("客流统计"));



#else
    m_pushbutton_pay_info = NULL;
    m_label_pay_info = NULL;
    m_pushbutton_passenger = NULL;
    m_label_passenger = NULL;
#endif




#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_pushbutton_message = NULL;
    m_pushbutton_call = NULL;
    m_pushbutton_driver_menu = NULL;
#else
    //1.消息
    m_pushbutton_message = new QPushButton(this);
    m_label_message = new QLabel(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
    QPushButton:focus{border:0px;border-image:url(%4);}")
            .arg(":/img/res/button_news.png").arg(":/img/res/button_news_selected.png").arg(":/img/res/button_news_selected.png").arg(":/img/res/button_news_selected.png");

    create_new_button(m_pushbutton_message,m_label_message,234 - 70,64 + 102 - 10,130,166,str_sheet,tr("消息"));

    //2.录像查询
    m_pushbutton_call = new QPushButton(this);
    m_label_call = new QLabel(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
    QPushButton:focus{border:0px;border-image:url(%4);}")
            .arg(":/img/res//Video_query.png").arg(":/img/res/Video_query_selected.png").arg(":/img/res/Video_query_selected.png").arg(":/img/res/Video_query_selected.png");

    create_new_button(m_pushbutton_call,m_label_call,234 + (130 + 82) * 1,64 + 102 - 10,130,166,str_sheet,tr("录像查询"));

    //3.司机菜单
    m_pushbutton_driver_menu = new QPushButton(this);
    m_label_driver_menu = new QLabel(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
    QPushButton:focus{border:0px;border-image:url(%4);}")
            .arg(":/img/res//button_driver.png").arg(":/img/res/button_driver_selected.png").arg(":/img/res/button_driver_selected.png").arg(":/img/res/button_driver_selected.png");

    create_new_button(m_pushbutton_driver_menu,m_label_driver_menu,234 + (130 + 82) * 2 + 70,64 + 102 - 10,130,166,str_sheet,tr("司机菜单"));

#endif

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_pushbutton_passenger_flow = new QPushButton(this);
    m_label_passenger_flow = new QLabel(this);

#ifdef APPLICATION_TYPE_P2
    m_define_screen->ignore_widget(m_pushbutton_passenger_flow);
    m_define_screen->ignore_widget(m_label_passenger_flow);
        str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
        QPushButton:focus{border:0px;border-image:url(%4);}")
            .arg(":/img_p2/res/p2/icon_passageway_default.png").arg(":/img_p2/res/p2/icon_passageway_pressed.png").arg(":/img_p2/res/p2/icon_passageway_pressed.png").arg(":/img_p2/res/p2/icon_passageway_pressed.png");

        create_new_button(m_pushbutton_passenger_flow,m_label_passenger_flow,436,103,104,104,str_sheet,tr("通道配置"));
#else
        str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
        QPushButton:focus{border:0px;border-image:url(%4);}")
            .arg(":/img/res/Video_query.png").arg(":/img/res/Video_query_selected.png").arg(":/img/res/Video_query_selected.png").arg(":/img/res/Video_query_selected.png");

        create_new_button(m_pushbutton_passenger_flow,m_label_passenger_flow,132,306,130,166,str_sheet,tr("通道配置"));
#endif


#else
    m_pushbutton_passenger_flow = NULL;
#endif

    //4.系统管理
    m_pushbutton_system_manager = new QPushButton(this);
    m_label_system_manager = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
    QPushButton:focus{border:0px;border-image:url(%4);}")
            .arg(":/img_p2/res/p2/icon_system_default.png").arg(":/img_p2/res/p2/icon_system_pressed.png").arg(":/img_p2/res/p2/icon_system_pressed.png").arg(":/img_p2/res/p2/icon_system_pressed.png");
#else
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
    QPushButton:focus{border:0px;border-image:url(%4);}")
            .arg(":/img/res//button_system.png").arg(":/img/res/button_system_selected.png").arg(":/img/res/button_system_selected.png").arg(":/img/res/button_system_selected.png");
#endif

#if defined(APPLICATION_TYPE_P2)
    m_define_screen->ignore_widget(m_pushbutton_system_manager);
    m_define_screen->ignore_widget(m_label_system_manager);
    create_new_button(m_pushbutton_system_manager,m_label_system_manager,100,271,104,104,str_sheet,tr("系统管理"));
#elif defined(APPLICATION_TYPE_D2)
    create_new_button(m_pushbutton_system_manager,m_label_system_manager,342,306,130,166,str_sheet,tr("系统管理"));
#else
    create_new_button(m_pushbutton_system_manager,m_label_system_manager,234 - 70,64 + 102 + 166 + 20 + 22 + 60 + 10,130,166,str_sheet,tr("系统管理"));
#endif


    //5.运行状态
    m_pushbutton_run_status = new QPushButton(this);
    m_label_run_status = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
    QPushButton:focus{border:0px;border-image:url(%4);}")
            .arg(":/img_p2/res/p2/icon_function_default.png").arg(":/img_p2/res/p2/icon_function_pressed.png").arg(":/img_p2/res/p2/icon_function_pressed.png").arg(":/img_p2/res/p2/icon_function_pressed.png");
#else
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
    QPushButton:focus{border:0px;border-image:url(%4);}")
            .arg(":/img/res//button_motion.png").arg(":/img/res/button_motion_selected.png").arg(":/img/res/button_motion_selected.png").arg(":/img/res/button_motion_selected.png");
#endif
#if defined(APPLICATION_TYPE_P2)
    m_define_screen->ignore_widget(m_pushbutton_run_status);
    m_define_screen->ignore_widget(m_label_run_status);
    create_new_button(m_pushbutton_run_status,m_label_run_status,268,271,104,104,str_sheet,tr("运行状态"));
#elif defined(APPLICATION_TYPE_D2)
    create_new_button(m_pushbutton_run_status,m_label_run_status,552,306,130,166,str_sheet,tr("运行状态"));
#else
    create_new_button(m_pushbutton_run_status,m_label_run_status,234 + (130 + 82) * 1,64 + 102 + 166 + 20 + 22 + 60 + 10,130,166,str_sheet,tr("运行状态"));
#endif
    //6.关于本机
    m_pushbutton_about_machine = new QPushButton(this);
    m_label_about_machine = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
    QPushButton:focus{border:0px;border-image:url(%4);}")
            .arg(":/img_p2/res/p2/icon_about_default.png").arg(":/img_p2/res/p2/icon_about_pressed.png").arg(":/img_p2/res/p2/icon_about_pressed.png").arg(":/img_p2/res/p2/icon_about_pressed.png");
#else
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}\
    QPushButton:focus{border:0px;border-image:url(%4);}")
            .arg(":/img/res//button_local.png").arg(":/img/res/button_local_selected.png").arg(":/img/res/button_local_selected.png").arg(":/img/res/button_local_selected.png");
#endif

#if defined(APPLICATION_TYPE_P2)
    m_define_screen->ignore_widget(m_pushbutton_about_machine);
    m_define_screen->ignore_widget(m_label_about_machine);
    create_new_button(m_pushbutton_about_machine,m_label_about_machine,436,271,104,104,str_sheet,tr("关于本机"));
#elif defined(APPLICATION_TYPE_D2)
    create_new_button(m_pushbutton_about_machine,m_label_about_machine,762,306,130,166,str_sheet,tr("关于本机"));
#else
    create_new_button(m_pushbutton_about_machine,m_label_about_machine,234 + (130 + 82) * 2 + 70,64 + 102 + 166 + 20 + 22 + 60 + 10,130,166,str_sheet,tr("关于本机"));
#endif

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    connect(m_pushbutton_passenger_flow,SIGNAL(clicked()),this,SLOT(slot_pushbutton_passenger_flow()));
#else
    connect(m_pushbutton_message,SIGNAL(clicked()),this,SLOT(slot_pushbutton_message()));
    connect(m_pushbutton_call,SIGNAL(clicked()),this,SLOT(slot_pushbutton_camera_query()));
    connect(m_pushbutton_driver_menu,SIGNAL(clicked()),this,SLOT(slot_pushbutton_driver_menu()));
#endif

#ifdef APPLICATION_TYPE_P2
    connect(m_pushbutton_pay_info,SIGNAL(clicked()),this,SLOT(slot_pushbutton_pay_info()));
    connect(m_pushbutton_passenger,SIGNAL(clicked()),this,SLOT(slot_pushbutton_passenger()));
#endif
    connect(m_pushbutton_system_manager,SIGNAL(clicked()),this,SLOT(slot_pushbutton_system_manager()));
    connect(m_pushbutton_run_status,SIGNAL(clicked()),this,SLOT(slot_pushbutton_run_status()));
    connect(m_pushbutton_about_machine,SIGNAL(clicked()),this,SLOT(slot_pushbutton_about_machine()));

    connect(CPassword1Dialog::getInstance(),SIGNAL(signal_user_exit_password()),this,SLOT(slot_user_exit_password()));

    m_define_screen->add_parent(this);
    m_login_success = false;
    m_login_success_tick = new QTime;
}

void CMenuWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CMenuWidget::create_new_button(QPushButton *_button,QLabel*_label,int _x,int _y,int _w,int _h,QString &_typeSheet,QString _text)
{
    _button->setFixedSize(_w,_h);
    _button->move(_x,_y);
    _button->setStyleSheet(_typeSheet);

    _button->setFocusPolicy(Qt::StrongFocus);

    //_button->setFocusPolicy(Qt::NoFocus);
    _button->setFlat(true);
#ifdef APPLICATION_TYPE_P2
    int str_length = CCommonInterface::get_string_length(_text,17);
    _label->setFixedSize(str_length,29 + 6);
    _label->move((_x + (_w - str_length)/2),_y + _h + 5);
#else
    int str_length = CCommonInterface::get_string_length(_text,TEXT_FONT);
    _label->setFixedSize(str_length,29 + 6);
#ifdef APPLICATION_TYPE_D2
    _label->move((_x + (_w - str_length)/2) + 15,_y + _h + 20);
#else
    _label->move((_x + (_w - str_length)/2),_y + _h + 20);
#endif

#endif

#ifdef APPLICATION_TYPE_P2
    _label->setStyleSheet("QLabel{border:0px;background:transparent;color:#ffffff;font:17px;}");
#else
    _label->setStyleSheet("QLabel{border:0px;background:transparent;color:#ffffff;font:26px;}");
#endif
    _label->setText(_text);
}

void CMenuWidget::slot_pushbutton_home()
{
    //close();
    emit signal_pushbutton_home();
}

void CMenuWidget::slot_pushbutton_back()
{
    //close();
    emit signal_pushbutton_home();
}

void CMenuWidget::slot_pushbutton_message()
{
    emit signal_pushbutton_message();
    m_pushbutton_message->clearFocus();
}
void CMenuWidget::slot_pushbutton_camera_query()
{
    emit signal_pushbutton_camera_query();
    m_pushbutton_call->clearFocus();
}
void CMenuWidget::slot_pushbutton_driver_menu()
{
    emit signal_pushbutton_driver_menu();
    m_pushbutton_driver_menu->clearFocus();
}

void CMenuWidget::slot_user_exit_password()
{
    bool ret_value = false;

    if(CPassword1Dialog::KEnumButtonOk == CPassword1Dialog::getInstance()->get_result())
    {
        m_login_success = true;
        ret_value = true;
        m_login_success_tick->restart();
    }
    else
    {
        m_login_success = false;
    }

    if(ret_value)
        emit signal_pushbutton_system_manager();

    m_pushbutton_system_manager->clearFocus();

}

void CMenuWidget::slot_pushbutton_system_manager()
{
    bool ret_value = false;
//#if(!defined(APPLICATION_TYPE_D2) && !defined(APPLICATION_TYPE_P2))
    if(1 == CCommonData::getInstance()->m_CSystemSystemSetup.use_password)
    {
        if(m_login_success)
        {
            if(m_login_success_tick->elapsed() >= 2 * 60 * 1000)
            {
                m_login_success = false;
            }
            else
            {
                ret_value = true;
            }
        }


        if(!m_login_success)
        {
            CPassword1Dialog::getInstance()->password_init();

            CPassword1Dialog::getInstance()->set_content(tr("管理员密码"),"",CLtyString::getInstance()->m_confirm,CLtyString::getInstance()->m_cancel);
            MyInputPanelContext *pInputPanel = (MyInputPanelContext *)CCommonInterface::get_method_widget();
            CPassword1Dialog::getInstance()->show();
            //pInputPanel->set_model_modal(true);
            //CPassword1Dialog::getInstance()->exec();
            //pInputPanel->set_model_modal(false);
            return;
        }

    }
    else
    {
        ret_value = true;
    }
//    ret_value = true;
//#else
//#endif

    if(ret_value)
        emit signal_pushbutton_system_manager();

    m_pushbutton_system_manager->clearFocus();

}
void CMenuWidget::slot_pushbutton_run_status()
{
    emit signal_pushbutton_run_status();
    m_pushbutton_run_status->clearFocus();
}
void CMenuWidget::slot_pushbutton_about_machine()
{
    emit signal_pushbutton_about_machine();
    m_pushbutton_about_machine->clearFocus();
}
void CMenuWidget::slot_pushbutton_passenger_flow()
{
    CProtocol::getInstance()->call_json_system_passenger_flow(CProtocol::kEnumGet);
    emit signal_pushbutton_passenger_flow();
    m_pushbutton_passenger_flow->clearFocus();
}

void CMenuWidget::slot_pushbutton_pay_info()
{
    emit signal_pushbutton_pay_info();
    m_pushbutton_pay_info->clearFocus();
}

void CMenuWidget::slot_pushbutton_passenger()
{
    emit signal_pushbutton_passenger();
    m_pushbutton_passenger->clearFocus();
}

void CMenuWidget::set_first_focus()
{
#if(defined(APPLICATION_TYPE_P2))
    m_pushbutton_pay_info->setFocus();
#else
    m_pushbutton_message->setFocus();
#endif
}

bool CMenuWidget::keyboard_value(int _value)
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


        }break;
        case KEYPAD_KEY7_FUN:
        {

        }break;
        case KEYPAD_KEY7_UP:
        {
            if(!get_current_focus())
            {
                set_first_focus();
                ret_value = true;
            }

        }break;
        case KEYPAD_KEY7_LEFT:
        {
            if(!get_current_focus())
            {
                set_first_focus();
                ret_value = true;
            }
        }break;
        case KEYPAD_KEY7_RIGHT:
        {
             if(!get_current_focus())
            {
                set_first_focus();
                ret_value = true;
            }
        }break;
        case KEYPAD_KEY7_DOWN:
        {
             if(!get_current_focus())
            {
                set_first_focus();
                ret_value = true;
            }
        }break;
    }
    return ret_value;
}
#include <QApplication>
bool CMenuWidget::get_current_focus()
{
    bool ret_value = false;
    do
    {
        QWidget *pFocusWidget = QApplication::focusWidget();

        if(NULL == pFocusWidget)
        {
            break;
        }

        if(m_pushbutton_pay_info && m_pushbutton_pay_info == pFocusWidget)
            ret_value = true;
        else if(m_pushbutton_passenger && m_pushbutton_passenger == pFocusWidget)
            ret_value = true;
        else if(m_pushbutton_passenger_flow && m_pushbutton_passenger_flow == pFocusWidget)
            ret_value = true;
        else if(m_pushbutton_message && m_pushbutton_message == pFocusWidget)
            ret_value = true;
        else if(m_pushbutton_call && m_pushbutton_call == pFocusWidget)
            ret_value = true;
        else if(m_pushbutton_driver_menu && m_pushbutton_driver_menu == pFocusWidget)
            ret_value = true;
        else if(m_pushbutton_system_manager && m_pushbutton_system_manager == pFocusWidget)
            ret_value = true;
        else if(m_pushbutton_run_status && m_pushbutton_run_status == pFocusWidget)
            ret_value = true;
        else if(m_pushbutton_about_machine && m_pushbutton_about_machine == pFocusWidget)
            ret_value = true;

    }while(0);

    return ret_value;
}








