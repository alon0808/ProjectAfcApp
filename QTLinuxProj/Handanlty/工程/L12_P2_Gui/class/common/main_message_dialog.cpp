#include "main_message_dialog.h"

#include "../common_interface.h"
#include "LtyCommonDefine.h"
#include "mainwindow.h"

CMainMessageDialog *CMainMessageDialog::m_single = NULL;

CMainMessageDialog *CMainMessageDialog::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CMainMessageDialog(MainWindow::getInstance());
    }
    return m_single;
}


CMainMessageDialog::CMainMessageDialog(QWidget *parent) :
    CParentWidget(parent)
{

    this->setFixedSize(723,551);
    this->setObjectName("CMainMessageDialog");
    this->setStyleSheet("QDialog#CMainMessageDialog{background:transparent;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    m_widget_background = new QWidget(this);
    m_widget_background->setFixedSize(712,542);
    m_widget_background->move((723 - 712)/2,(551 - 542)/2);
    m_widget_background->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/popup_dialog/popup_bg.png);}");

    QString str_sheet = "";
    m_pushbutton_close = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }").arg(":/img/res/main_page/popup_dialog/close.png");
    CCommonInterface ::init_new_button(m_pushbutton_close,this->width() - 62,0,62,66,str_sheet);
    connect(m_pushbutton_close,SIGNAL(clicked()),SLOT(slot_pushbutton_close()));


    str_sheet = "QLabel{border:0px solid #ffffff;font:31px;color:#8ea2c0;}";
    m_label_start_vehicle1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_start_vehicle1,(this->width() - 240)/2,130,240,40,"",str_sheet);
    m_label_start_vehicle1->setText(tr("下一班发车时间:"));
#ifdef CURRENT_ARM_RUN
    str_sheet = "QLabel{border:0px solid #ffffff;font:bold 51px;color:#80e3ff;}";
#else
    str_sheet = "QLabel{border:0px solid #ffffff;font:51px;color:#80e3ff;}";
#endif
    m_label_start_vehicle2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_start_vehicle2,(this->width() - 600)/2,170,600,70,"",str_sheet);

    str_sheet = "QLabel{border:0px solid #ffffff;font:31px;color:#8ea2c0;}";
    m_label_drive_time1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_drive_time1,(this->width() - 240)/2,255,240,40,"",str_sheet);
    m_label_drive_time1->setText(tr("连续驾驶时间:"));
#ifdef CURRENT_ARM_RUN
    str_sheet = "QLabel{border:0px solid #ffffff;font:bold 51px;color:#80e3ff;}";
#else
    str_sheet = "QLabel{border:0px solid #ffffff;font:51px;color:#80e3ff;}";
#endif
    m_label_drive_time2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_drive_time2,(this->width() - 500)/2,300,500,70,"",str_sheet);


    str_sheet = "QLabel{border:0px solid #ffffff;font:23px;color:#ffff00;}";
    m_label_auto_close1 = new QLabel(this);
    QString str_show = tr("10s");
    int str_length = CCommonInterface::init_label_text(m_label_auto_close1,(this->width() - 220)/2,446,0,40,str_show,str_sheet);
    str_length = CCommonInterface::init_label_text(m_label_auto_close1,(this->width() - 125 - str_length)/2,446,0,40,str_show,str_sheet);
    m_label_auto_close1->setFixedWidth(str_length);

    str_sheet = "QLabel{border:0px solid #ffffff;font:23px;color:#94c6ff;}";
    m_label_auto_close2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_auto_close2,(this->width() - 125 - str_length)/2 + str_length,446,125,40,"",str_sheet,Qt::AlignLeft| Qt::AlignVCenter);
    m_label_auto_close2->setText(tr("后自动关闭"));

/////////////////////////////////////////////////////////////////////////
    m_label_messae = new QLabel(this);
    str_sheet = "QLabel{border:0px solid #ffffff;font:61px;color:#80e3ff;}";
    CCommonInterface::init_label_text(m_label_messae,(this->width() - 546)/2,120,546,262,"",str_sheet);
    m_label_messae->setWordWrap(true);


    m_timer = new QTimer(this);
    m_timer->setInterval(1 * 1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timer_close_window()));
}

void CMainMessageDialog::slot_pushbutton_close()
{
    m_timer->stop();
    close();
}

void CMainMessageDialog::slot_timer_close_window()
{
    if(m_timer_10s <= 0)
        slot_pushbutton_close();
    else
    {
        m_timer_10s--;
        m_label_auto_close1->setText(QString::number(m_timer_10s) + "s");
    }
}

void CMainMessageDialog::set_show_content(int _type,const QString &_show1,const QString &_show2)
{
    switch(_type)
    {
        case kEnumMessageDialog: //来短信时
        {
            m_label_start_vehicle1->hide();
            m_label_start_vehicle2->hide();
            m_label_drive_time1->hide();
            m_label_drive_time2->hide();
            m_label_auto_close1->show();
            m_label_auto_close2->show();

            m_label_messae->show();
            m_label_messae->setText(_show1);
        }break;
        case kEnumOperateDialog:  //点击运营中
        {
            m_label_start_vehicle1->show();
            m_label_start_vehicle2->show();
            m_label_drive_time1->show();
            m_label_drive_time2->show();
            m_label_auto_close1->show();
            m_label_auto_close2->show();

            m_label_messae->hide();

            m_label_start_vehicle2->setText(_show1);
            m_label_drive_time2->setText(_show2);


        }break;
    }
    m_label_auto_close1->setText(tr("10s"));
    if(m_timer->isActive())
        m_timer->stop();
    m_timer->start();
    m_timer_10s = 10;
    this->update();

}

bool CMainMessageDialog::keyboard_value(int _value)
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
