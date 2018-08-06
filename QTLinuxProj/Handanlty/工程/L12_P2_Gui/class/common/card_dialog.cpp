#include "card_dialog.h"

#include "../common_interface.h"
#include "LtyCommonStyle.h"

#include "mainwindow.h"

#define MESSAGEBOX_BUTTON_WIDTH   160
#define MESSAGEBOX_BUTTON_HEIGHT   65

CCardDialog *CCardDialog::m_single = NULL;

CCardDialog *CCardDialog::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CCardDialog(MainWindow::getInstance());
    }
    return m_single;
}

CCardDialog::CCardDialog(QWidget *parent,char _type) :
    CParentWidget(parent)
{
    m_type = _type;
    this->setFixedSize(540,397);
    this->setObjectName("CCardDialog");
    this->setStyleSheet("QDialog#CCardDialog{border:1px solid #275ba7;border-radius: 0px;background-color:#03111f;}");//444a59 //ffffff
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


    QString str_show = tr("工号:");
    m_label_password = new QLabel(this);
    CCommonInterface::init_label_text(m_label_password,76,157,0,ROW_HEIGHT,str_show);

    m_lineedit_password = new QLineEdit(this);
    m_lineedit_password->move(154,167);
    m_lineedit_password->setFixedSize(315 - 60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_password);


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

}

void CCardDialog::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CCardDialog::set_content(const QString&_title,const QString&_context,
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
    m_define_screen->add_parent(this);
}

void CCardDialog::slot_pushbutton_close()
{
    slot_pushbutton2();
}

void CCardDialog::slot_pushbutton1()
{

    QString str_input = m_lineedit_password->text();

    do
    {
        m_card_number = str_input;

        m_button_type = KEnumButtonOk;
        hide();
    }while(0);



}
void CCardDialog::slot_pushbutton2()
{
     m_button_type = KEnumButtonCancel;
     hide();
}

QString CCardDialog::get_card_number()
{
    return m_card_number;
}

CCardDialog::ButtonType CCardDialog::get_result()
{
    return m_button_type;
}

void CCardDialog::card_init()
{
    m_lineedit_password->clear();
}

bool CCardDialog::keyboard_value(int _value)
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



