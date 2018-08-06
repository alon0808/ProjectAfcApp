
#include "my_messagebox1.h"
#include "../common_interface.h"
#include "mainwindow.h"

#define MESSAGEBOX_BUTTON_WIDTH   160
#define MESSAGEBOX_BUTTON_HEIGHT   65

CMyMessageBox1 *CMyMessageBox1::m_single = NULL;

CMyMessageBox1 *CMyMessageBox1::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CMyMessageBox1(MainWindow::getInstance());
    }
    return m_single;
}

CMyMessageBox1::CMyMessageBox1(QWidget *parent) :
    CParentWidget(parent)
{

    this->setFixedSize(220,220);
    this->setObjectName("CMyMessageBox1");
    this->setStyleSheet("QDialog#CMyMessageBox1{background:transparent;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);
    this->setAttribute(Qt::WA_TranslucentBackground, true);


    m_widget_status = new QWidget(this);
    m_widget_status->setFixedSize(220,220);


    QString str_sheet = "QLabel{border:0px solid #ffffff;background:transparent;color:#ffffff;font:18px;}";
    m_label_content = new QLabel(this);
    CCommonInterface::init_label_text(m_label_content,20,180,180,40,"",str_sheet,Qt::AlignCenter,18);


    m_timer = new QTimer(this);
    m_timer->setInterval(1 * 1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timer_close_window()));
    m_timer_10s = 0;

}



void CMyMessageBox1::set_type(char _type)
{
    QString str_content = "";
    QString str_sheet = "QWidget{border:0px;border-image:url(:/img_p2/res/p2/up_down/pop_bg1.png)}";
    switch(_type)
    {
        case kEnumSuccess:
        {
            str_content = tr("验证成功");
            str_sheet = "QWidget{border:0px;border-image:url(:/img_p2/res/p2/up_down/pop_bg1.png)}";
        }break;
        case kEnumFail:
        {
            str_content = tr("验证失败");
            str_sheet = "QWidget{border:0px;border-image:url(:/img_p2/res/p2/up_down/pop_bg3.png)}";
        }break;
        case kEnumAbnormal:
        {
            str_content = tr("验证异常");
            str_sheet = "QWidget{border:0px;border-image:url(:/img_p2/res/p2/up_down/pop_bg2.png)}";
        }break;
    }
    m_label_content->setText(str_content);
    m_widget_status->setStyleSheet(str_sheet);

}

void CMyMessageBox1::slot_pushbutton_close()
{
    if(m_timer->isActive())
        m_timer->stop();

    this->close();
}



void CMyMessageBox1::set_timer()
{

    m_timer_10s = 3;
    if(m_timer->isActive())
        m_timer->stop();

     m_timer->start();
}

void CMyMessageBox1::slot_timer_close_window()
{
    if(m_timer_10s <= 0)
        slot_pushbutton_close();
    else
    {
        m_timer_10s--;
    }
}


bool CMyMessageBox1::keyboard_value(int _value)
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
            ret_value = true;

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






