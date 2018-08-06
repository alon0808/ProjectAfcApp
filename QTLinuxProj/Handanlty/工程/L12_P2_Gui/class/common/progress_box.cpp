#include "progress_box.h"

#include "commonuiinit.h"


#include "class/common_interface.h"
#include "LtyCommonStyle.h"
#include "main_page.h"

#include "class/ltystring.h"

#define MESSAGEBOX_BUTTON_WIDTH   140
#define MESSAGEBOX_BUTTON_HEIGHT   65

CProgressBox *CProgressBox::m_single = NULL;

CProgressBox *CProgressBox::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CProgressBox(CMainPage::getInstance());
    }
    return m_single;
}

CProgressBox::CProgressBox(QWidget *parent) :
    CParentWidget(parent)
{

    this->setFixedSize(540,397);
    this->setObjectName("CProgressBox");
    this->setStyleSheet("QDialog#CProgressBox{border:1px solid #275ba7;border-radius: 0px;background-color:#03111f;}");//444a59 //ffffff
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    //标题
    QString str_sheet = "";
    str_sheet = "QLabel{border:0px solid #ffffff;font:46px;background:#275ba7;color:#80e3ff;}";
    m_label_title = new QLabel(this);
    CCommonInterface::init_label_text(m_label_title,1,1,this->width() - 2,77,"",str_sheet);
    m_label_title->setText(tr("升级进度"));

    m_progressbar_1 = new QProgressBar(this);
    CommonUiInit::setMsgProgressBarStyleSheet(m_progressbar_1);
    m_progressbar_1->setFixedSize(376,65);
    m_progressbar_1->move((540 - 376)/2,77 + (397 - 77 - 65)/2);
    m_progressbar_1->setRange(0,100);
    m_progressbar_1->setTextVisible(false);
    m_progressbar_1->setValue(20);


    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png");

    m_pushbutton_close = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_close,this->width() - 10 - 40,(77 - 52)/2,40,52,str_sheet);


    m_pushbutton1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton1,418 - 212,535 - 76,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_OK);
    m_pushbutton1->setText(CLtyString::getInstance()->m_confirm);

    m_pushbutton2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton2,568 - 212,535 - 76,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_CANCEL);
    m_pushbutton2->setText(CLtyString::getInstance()->m_cancel);

    m_button_type = KEnumButtonCancel;

    m_pushbutton1->hide();
    m_pushbutton2->hide();




    connect(m_pushbutton1,SIGNAL(clicked()),this,SLOT(slot_pushbutton1()));
    connect(m_pushbutton2,SIGNAL(clicked()),this,SLOT(slot_pushbutton2()));

    connect(m_pushbutton_close,SIGNAL(clicked()),this,SLOT(slot_pushbutton_close()));

    m_timer = new QTimer(this);
    m_timer->setInterval(1 * 1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_test_timer()));
    m_timer_10s = 0;

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this);

}

void CProgressBox::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

#include "my_messagebox.h"

void CProgressBox::slot_pushbutton_close()
{
    //slot_pushbutton2();

    do
    {


        CMyMessageBox::getInstance()->set_content(tr("温馨提示"),"有任务在执行，是否结束?",CLtyString::getInstance()->m_confirm,CLtyString::getInstance()->m_cancel);
        CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerCancel);
        CMyMessageBox::getInstance()->exec();

        if(CMyMessageBox::KEnumButtonOk != CMyMessageBox::getInstance()->get_result())
        {
            break;
        }
        m_status = KEnumStatusStop;
        slot_pushbutton2();

    }while(0);

}

void CProgressBox::slot_progress_box_event(char _type,int _percent)
{

    switch(_type)
    {
        case kEnumProgreessNormal:
        {
            m_progressbar_1->setValue(_percent);
            if(100 == _percent)
            {
                m_status = KEnumStatusSuccess;
                slot_pushbutton2();
            }
        }break;
        case kEnumProgreessFail:
        {
            m_status = KEnumStatusFail;
            slot_pushbutton2();
        }break;
        case kEnumProgreessNull:
        {
            m_status = KEnumStatusNull;
            slot_pushbutton2();
        }break;
        case kEnumProgreessUmount:
        {
            m_status = KEnumStatusUmount;
            slot_pushbutton2();
        }break;

    }
}

void CProgressBox::data_init()
{
    static bool is_first = true;
    m_status = KEnumStatusNull;
    m_progressbar_1->setValue(0);

    if(is_first)
    {
        is_first = false;
        this->move(242,166);
        m_define_screen->add_parent(this);
    }
    else
        this->move(CMyDefineScreen::getInstance()->get_change_factor_x(242),CMyDefineScreen::getInstance()->get_change_factor_y(166));
}

CProgressBox::StatusType CProgressBox::get_status()
{

    return m_status;
 //   return  KEnumStatusNull;
}

void CProgressBox::slot_pushbutton1()
{
     m_button_type = KEnumButtonOk;
     hide();
}
void CProgressBox::slot_pushbutton2()
{
     m_button_type = KEnumButtonCancel;
     hide();
}

void CProgressBox::test_init()
{
    m_timer_10s = 0;
    m_timer->start();
}

void CProgressBox::slot_test_timer()
{
    if(m_timer_10s >= 10)
    {
        m_timer->stop();
        m_status = KEnumStatusSuccess;
        slot_pushbutton2();
    }
    m_timer_10s++;
    m_progressbar_1->setValue(m_timer_10s * 10);
}

void CProgressBox::set_title(QString _title)
{
    m_label_title->setText(_title);

}

bool CProgressBox::keyboard_value(int _value)
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





