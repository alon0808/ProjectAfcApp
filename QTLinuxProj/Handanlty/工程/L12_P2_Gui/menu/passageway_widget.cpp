#include "passageway_widget.h"

#include "class/common_interface.h"

#include "main_page.h"

CPassagewayWidget::CPassagewayWidget(QWidget *parent) :
    QDialog(parent)
{

    this->setFixedSize(1024,768);
    this->setObjectName("CPassagewayWidget");
    this->setStyleSheet("QDialog#CPassagewayWidget{background:transparent;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);


    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetPassageway,this);
    connect(m_CHeadWidget,SIGNAL(signal_button_back()),this,SLOT(slot_pushbutton_back()));
    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home()));


    int button_height = 78 + 18;;
    int navigate_width = 230;
    int navigate_height = 1 * button_height;

    QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(250,768 - 82);
    pWidget->move(0,82);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");

    m_widget_bg2 = new QWidget(this);
    m_widget_bg2->setFixedSize(190,636);
    m_widget_bg2->move(40,107);
    m_widget_bg2->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/menu/system_manage/nav_left_background.png);}");


    m_CNavigateLeft = new CNavigateLeft(navigate_width,button_height,navigate_height,this);
    m_CNavigateLeft->move(20,106 + 58 + 10);
    m_CNavigateLeft->setFixedSize(navigate_width,navigate_height);

    m_CNavigateLeft->add_button(CNavigateLeft::kEnumPassengerFlow,tr("客流统计"));



    m_CPassengerFlowWidget = NULL;

    m_stackwidget_navigate_right = new QStackedWidget(this);
    m_stackwidget_navigate_right->move(MAIN_NAVIGATE_LEFT,MAIN_HEAD_HEIGHT);
    m_stackwidget_navigate_right->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_stackwidget_navigate_right->setObjectName(QString::fromUtf8("m_stackwidget_navigate_right"));
    m_stackwidget_navigate_right->setContentsMargins(0, 0, 0, 0);

    connect(m_CNavigateLeft,SIGNAL(signalCurrentClassificationChanged(int,QString)),this,SLOT(slot_passageway_widget_event(int,QString)));

    connect(this,SIGNAL(signal_set_default_id(int)),m_CNavigateLeft,SLOT(slotSetDefaultSelected(int)));

    m_define_screen = new CMyDefineScreen();
    //m_define_screen->ignore_widget(m_CNavigateLeft);
    m_define_screen->add_parent(this);

    emit signal_set_default_id(CNavigateLeft::kEnumPassengerFlow);

}


void CPassagewayWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}


void CPassagewayWidget::slot_passageway_widget_event(int _type,QString _text)
{
    m_CHeadWidget->set_head_widget_title(_text);
    m_current_select_id = _type;
    switch(_type)
    {
        case CNavigateLeft::kEnumPassengerFlow:
        {
            goto_passenger_flow();
        }break;

    }
}

void CPassagewayWidget::goto_passenger_flow()//客流统计
{
    if(NULL == m_CPassengerFlowWidget)
    {
        m_CPassengerFlowWidget = new CPassengerFlowWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CPassengerFlowWidget);

        connect(m_CPassengerFlowWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_passenger_flow_user_return()),m_CPassengerFlowWidget,SLOT(slot_pushbutton_return()));

        connect(CMainPage::getInstance(),SIGNAL(signal_passenger_flow_widget_event(char,void *)),m_CPassengerFlowWidget,SLOT(slot_passenger_flow_widget_event(char,void *)));

    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CPassengerFlowWidget);
}

void CPassagewayWidget::slot_pushbutton_home()
{
    m_CPassengerFlowWidget->save_data();
    m_goto_main_page = true;
    CCommonInterface::input_panel_control(false);
    emit signal_pushbutton_home();
}

void CPassagewayWidget::slot_pushbutton_close()
{
    m_CPassengerFlowWidget->save_data();
    CCommonInterface::input_panel_control(false);
    m_goto_main_page = false;
    emit signal_pushbutton_menu();
}

void CPassagewayWidget::slot_pushbutton_back()
{

    if(CNavigateLeft::kEnumPassengerFlow == m_current_select_id)
    {
         emit signal_passenger_flow_user_return();
    }
    else
    {
        m_CPassengerFlowWidget->save_data();
        m_goto_main_page = false;
        CCommonInterface::input_panel_control(false);
        emit signal_pushbutton_menu();
    }

}


























