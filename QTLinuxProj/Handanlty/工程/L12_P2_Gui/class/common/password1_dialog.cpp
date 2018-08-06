#include "password1_dialog.h"

#include "../common_interface.h"
#include "LtyCommonStyle.h"

#include "mainwindow.h"
#include "common_data.h"
#include "head_widget.h"

#define MESSAGEBOX_BUTTON_WIDTH   145
#define MESSAGEBOX_BUTTON_HEIGHT   65

CPassword1Dialog *CPassword1Dialog::m_single = NULL;

CPassword1Dialog *CPassword1Dialog::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CPassword1Dialog(MainWindow::getInstance());
    }
    return m_single;
}

CPassword1Dialog::CPassword1Dialog(QWidget *parent,char _type) :
    CParentWidget(parent)
{
    m_type = _type;
    this->move(MainWindow::getInstance()->pos());
    this->setFixedSize(1024,768);
    this->setObjectName("CPassword1Dialog");
    this->setStyleSheet("QDialog#CPassword1Dialog{border:1px solid #275ba7;border-radius: 0px;background-color:#03111f;}");//444a59 //ffffff
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);


    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetPassword,this);

    QString str_show = tr("密码:");
    m_label_password = new QLabel(this);
    CCommonInterface::init_label_text(m_label_password,335 - 10,386,0,30,str_show);

    m_lineedit_password = new QLineEdit(this);
    m_lineedit_password->move(390,364);
    m_lineedit_password->setFixedSize(300,65);
    CCommonInterface::set_lineedit_sheet(m_lineedit_password);
    m_lineedit_password->setEchoMode(QLineEdit::Password);

    m_pushbutton1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton1,390,467,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_OK);

    m_pushbutton2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton2,545,467,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_CANCEL);


    m_button_type = KEnumButtonCancel;

    connect(m_pushbutton1,SIGNAL(clicked()),this,SLOT(slot_pushbutton1()));
    connect(m_pushbutton2,SIGNAL(clicked()),this,SLOT(slot_pushbutton2()));



    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);


}

void CPassword1Dialog::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CPassword1Dialog::set_content(const QString&_title,const QString&_context,
                                const QString &buttonText1,
                                const QString &buttonText2)
{

    m_button_text1 = buttonText1;
    m_button_text2 = buttonText2;

    m_pushbutton1->setText(buttonText1);
    m_pushbutton2->setText(buttonText2);

}



void CPassword1Dialog::slot_pushbutton1()
{
    QString str_password = "";
    QString str_input = m_lineedit_password->text();

    do
    {
#if 0
        CCommonInterface::printf_debug("admin_password:" + CCommonData::getInstance()->m_CUserLoginInfo.admin_password);
        CCommonInterface::printf_debug("sd_admin_password:" + CCommonData::getInstance()->m_CUserLoginInfo.sd_admin_password);
        CCommonInterface::printf_debug("sd_format_password:" + CCommonData::getInstance()->m_CUserLoginInfo.sd_format_password);
        CCommonInterface::printf_debug("use_sd_data:" + QString::number(CCommonData::getInstance()->m_CUserLoginInfo.use_sd_data));
#endif
        if(0 == CCommonData::getInstance()->m_CUserLoginInfo.use_dynamic)
        {
            if(0 == CCommonData::getInstance()->m_CUserLoginInfo.use_sd_data)
                str_password = CCommonData::getInstance()->m_CUserLoginInfo.admin_password;
            else
            {
                str_password = CCommonData::getInstance()->m_CUserLoginInfo.sd_admin_password;
            }
        }
        else
        {
            time_t now;
            char aszdynamicpwd[8];
            struct tm tm;
            now = time(0);
            localtime_r(&now, &tm);
            sprintf(aszdynamicpwd, "%d%d%d%d%d%d",(tm.tm_mon + 1)%10,
            (tm.tm_mon + 1)/10, tm.tm_mday%10, tm.tm_mday/10, tm.tm_hour%10,
            tm.tm_hour/10);
            str_password = aszdynamicpwd;
        }
        if(str_password.isEmpty())
            str_password = "165432";

        if(str_password != str_input)
        {

            CCommonInterface::printf_debug("str_input:" + str_input);
            CCommonInterface::printf_debug("str_password:" + str_password);
            break;
        }

        m_button_type = KEnumButtonOk;
        hide();
    }while(0);

    emit signal_user_exit_password();

}
void CPassword1Dialog::slot_pushbutton2()
{
     m_button_type = KEnumButtonCancel;
     hide();

     emit signal_user_exit_password();
}


CPassword1Dialog::ButtonType CPassword1Dialog::get_result()
{
    return m_button_type;
}

void CPassword1Dialog::password_init()
{
    m_lineedit_password->clear();
}



bool CPassword1Dialog::keyboard_value(int _value)
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
            slot_pushbutton1();
            ret_value = true;

        }break;
        case KEYPAD_KEY7_EXIT:
        {
            slot_pushbutton2();
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



