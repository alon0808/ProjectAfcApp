#include "my_messagebox.h"

#include "../common_interface.h"
#include "LtyCommonStyle.h"
#include "mainwindow.h"

#include "common_data.h"

#define MESSAGEBOX_BUTTON_WIDTH   160
#define MESSAGEBOX_BUTTON_HEIGHT   65

CMyMessageBox *CMyMessageBox::m_single = NULL;

CMyMessageBox *CMyMessageBox::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CMyMessageBox(MainWindow::getInstance());
    }
    return m_single;
}

CMyMessageBox::CMyMessageBox(QWidget *parent,char _type) :
    CParentWidget(parent)
{
    m_type = _type;
    this->setFixedSize(540,397);
    this->setObjectName("CMyMessageBox");
    this->setStyleSheet("QDialog#CMyMessageBox{border:1px solid #275ba7;border-radius: 0px;background-color:#03111f;}");//444a59 //ffffff
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

    int show_width = this->width() - 5 - 5;
    int show_height = this->height() - 77 - MESSAGEBOX_BUTTON_HEIGHT - 5 - 5 - 45;
    if(kEnumNormal == m_type)
        str_sheet = "QLabel{border:0px solid #ffffff;font:38px;background:transparent;color:#90A1BF;}";
    else if(kEnumNormal == m_type)
        str_sheet = "QLabel{border:0px solid #ffffff;font:61px;background:transparent;color:#ffffff;}";
    else
        str_sheet = "QLabel{border:0px solid #ffffff;background:transparent;color:#ffffff;}";

    m_label_content = new QLabel(this);
    CCommonInterface::init_label_text(m_label_content,5, 77 + 5,show_width,show_height,"",str_sheet);


    m_pushbutton1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton1,0,0,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_OK);

    m_pushbutton2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton2,0,0,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_CANCEL);


    m_button_type = KEnumButtonCancel;

    connect(m_pushbutton1,SIGNAL(clicked()),this,SLOT(slot_pushbutton1()));
    connect(m_pushbutton2,SIGNAL(clicked()),this,SLOT(slot_pushbutton2()));

    connect(m_pushbutton_close,SIGNAL(clicked()),this,SLOT(slot_pushbutton_close()));

    m_timer = new QTimer(this);
    m_timer->setInterval(1 * 1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timer_close_window()));
    m_timer_10s = 0;
    m_timer_type = KEnumTimerNull;

    m_define_screen = new CMyDefineScreen();


}

void CMyMessageBox::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CMyMessageBox::set_content(const QString&_title,const QString&_context,
                                const QString &buttonText1,
                                const QString &buttonText2)
{
    m_label_title->setText(_title);
    m_label_content->setText(_context);
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

void CMyMessageBox::slot_pushbutton_close()
{
    if(m_timer->isActive())
        m_timer->stop();
    slot_pushbutton2();
}

void CMyMessageBox::slot_pushbutton1()
{
     m_button_type = KEnumButtonOk;
     hide();
     if(m_timer->isActive())
        m_timer->stop();
}
void CMyMessageBox::slot_pushbutton2()
{
     m_button_type = KEnumButtonCancel;
     hide();
     if(m_timer->isActive())
        m_timer->stop();
}

void CMyMessageBox::set_timer(char _timerType)
{
    m_timer_type = _timerType;
    m_timer_10s = CCommonData::getInstance()->get_message_box_quit();

    if(100 != m_timer_10s)
    {
        if(m_timer->isActive())
            m_timer->stop();

         m_timer->start();
    }

}

void CMyMessageBox::slot_timer_close_window()
{
    if(m_timer_10s <= 0)
        slot_pushbutton_close();
    else
    {
        m_timer_10s--;
        if(KEnumTimerOk == m_timer_type)
            m_pushbutton1->setText(m_button_text1 + tr("(") + QString::number(m_timer_10s) + ")");
        else if(KEnumTimerCancel == m_timer_type)
            m_pushbutton2->setText(m_button_text2 + tr("(") + QString::number(m_timer_10s) + ")");
        else
        {
             if(m_timer->isActive())
                m_timer->stop();
        }
    }
}

CMyMessageBox::ButtonType CMyMessageBox::get_result()
{
    return m_button_type;
}

bool CMyMessageBox::keyboard_value(int _value)
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






