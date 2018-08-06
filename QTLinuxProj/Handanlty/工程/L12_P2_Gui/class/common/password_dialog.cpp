#include "password_dialog.h"

#include "../common_interface.h"
#include "LtyCommonStyle.h"

#include "mainwindow.h"
#include "common_data.h"

#define MESSAGEBOX_BUTTON_WIDTH   160
#define MESSAGEBOX_BUTTON_HEIGHT   65

CPasswordDialog *CPasswordDialog::m_single = NULL;

CPasswordDialog *CPasswordDialog::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CPasswordDialog(MainWindow::getInstance());
    }
    return m_single;
}

CPasswordDialog::CPasswordDialog(QWidget *parent,char _type) :
    CParentWidget(parent)
{
    m_type = _type;
    this->setFixedSize(540,397);
    this->setObjectName("CPasswordDialog");
    this->setStyleSheet("QDialog#CPasswordDialog{border:1px solid #275ba7;border-radius: 0px;background-color:#03111f;}");//444a59 //ffffff
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    //标题
    QString str_sheet = "";
    str_sheet = "QLabel{border:0px solid #ffffff;font:46px;background:#275ba7;color:#80e3ff;}";
    m_label_title = new QLabel(this);
    CCommonInterface::init_label_text(m_label_title,1,1,this->width() - 2,77,"",str_sheet);

    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png");

    m_pushbutton_close = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_close,this->width() - 10 - 40,(77 - 52)/2,40,52,str_sheet);


    //QLabel *m_label_password;
    //QLineEdit *m_lineedit_password;
    QString str_show = tr("密码:");
    m_label_password = new QLabel(this);
    CCommonInterface::init_label_text(m_label_password,76,157,0,ROW_HEIGHT,str_show);

    m_lineedit_password = new QLineEdit(this);
    m_lineedit_password->move(154,167);
    m_lineedit_password->setFixedSize(315,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_password);
    m_lineedit_password->setEchoMode(QLineEdit::Password);

    m_pushbutton1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton1,0,0,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_OK);

    m_pushbutton2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton2,0,0,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_CANCEL);


    m_button_type = KEnumButtonCancel;

    connect(m_pushbutton1,SIGNAL(clicked()),this,SLOT(slot_pushbutton1()));
    connect(m_pushbutton2,SIGNAL(clicked()),this,SLOT(slot_pushbutton2()));

    connect(m_pushbutton_close,SIGNAL(clicked()),this,SLOT(slot_pushbutton_close()));

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);

    m_password_type = kEnumTypeSystem;
}

void CPasswordDialog::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CPasswordDialog::set_content(const QString&_title,const QString&_context,
                                const QString &buttonText1,
                                const QString &buttonText2)
{
    m_label_title->setText(_title);
    m_button_text1 = buttonText1;
    m_button_text2 = buttonText2;

    if (buttonText1.isEmpty() && buttonText2.isEmpty())
    {
        m_pushbutton1->hide();
        m_pushbutton2->hide();
    }
    else if (buttonText2.isEmpty())
    {
        int button_x = (this->width() - m_pushbutton1->width())/2;
        int button_y = this->height() - m_pushbutton1->height() - 45;

        m_pushbutton1->move(button_x,button_y);
        m_pushbutton1->setText(buttonText1);
        m_pushbutton1->show();
        m_pushbutton2->hide();
    }
    else if ((!buttonText1.isEmpty()) && (!buttonText2.isEmpty()))
    {
        int button1_x = (this->width() - m_pushbutton1->width() - m_pushbutton2->width() - 20)/2;
        int button2_x = button1_x + 20 + m_pushbutton1->width();
        int button1_y = this->height() - m_pushbutton1->height() - 45;
        m_pushbutton1->show();
        m_pushbutton2->show();

        m_pushbutton1->move(button1_x,button1_y);
        m_pushbutton1->setText(buttonText1);

        m_pushbutton2->move(button2_x,button1_y);
        m_pushbutton2->setText(buttonText2);
    }

    static bool is_first = true;
    if(is_first)
    {
        is_first = false;
        this->move(242,166);
        m_define_screen->add_parent(this);
    }
    else
        this->move(CMyDefineScreen::getInstance()->get_change_factor_x(242),CMyDefineScreen::getInstance()->get_change_factor_y(166));
}

void CPasswordDialog::slot_pushbutton_close()
{
    slot_pushbutton2();
}

void CPasswordDialog::slot_pushbutton1()
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
                if(kEnumTypeSystem == m_password_type)
                    str_password = CCommonData::getInstance()->m_CUserLoginInfo.sd_admin_password;
                else
                    str_password = CCommonData::getInstance()->m_CUserLoginInfo.sd_format_password;
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
            str_password = "1654321";

        if(str_password != str_input)
        {

            CCommonInterface::printf_debug("str_input:" + str_input);
            CCommonInterface::printf_debug("str_password:" + str_password);
            break;
        }

        m_button_type = KEnumButtonOk;
        hide();
    }while(0);



}
void CPasswordDialog::slot_pushbutton2()
{
     m_button_type = KEnumButtonCancel;
     hide();
}


CPasswordDialog::ButtonType CPasswordDialog::get_result()
{
    return m_button_type;
}

void CPasswordDialog::password_init()
{
    m_lineedit_password->clear();
}

void CPasswordDialog::set_password_type(char _type)
{
    m_password_type = _type;
}

bool CPasswordDialog::keyboard_value(int _value)
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



