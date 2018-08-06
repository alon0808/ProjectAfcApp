#include "videotape_widget.h"

#include "class/common_interface.h"
#include "main_page.h"
#include "mainwindow.h"
#include <QDebug>

CVideotapeWidget::CVideotapeWidget(QWidget *parent) :
    QDialog(parent)
{

    this->setFixedSize(1024,768);
    this->setObjectName("CVideotapeWidget");
    this->setStyleSheet("QDialog#CVideotapeWidget{background-color:#000000;}");//444a59
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);


    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetVideotape,this);
    connect(m_CHeadWidget,SIGNAL(signal_button_back()),this,SLOT(slot_pushbutton_back()));
    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home()));


    int button_height = 78 + 18;;
    int navigate_width = 230;
    int navigate_height = 1 * button_height;

    m_widget_bg2 = new QWidget(this);
    m_widget_bg2->setFixedSize(190,636);
    m_widget_bg2->move(40,107);
    m_widget_bg2->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/menu/system_manage/nav_left_background.png);}");


    m_CNavigateLeft = new CNavigateLeft(navigate_width,button_height,navigate_height,this);
    m_CNavigateLeft->move(20,106 + 58 + 10);
    m_CNavigateLeft->setFixedSize(navigate_width,navigate_height);

    m_CNavigateLeft->add_button(CNavigateLeft::kEnumVideotapeQuery,tr("录像查询"));



    m_CVideotapeQueryWidget = NULL;         //录像查询

    m_stackwidget_navigate_right = new QStackedWidget(this);
    m_stackwidget_navigate_right->move(MAIN_NAVIGATE_LEFT,MAIN_HEAD_HEIGHT);
    m_stackwidget_navigate_right->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_stackwidget_navigate_right->setObjectName(QString::fromUtf8("m_stackwidget_navigate_right"));
    m_stackwidget_navigate_right->setContentsMargins(0, 0, 0, 0);

    connect(m_CNavigateLeft,SIGNAL(signalCurrentClassificationChanged(int,QString)),this,SLOT(slot_videotape_widget_event(int,QString)));

    connect(this,SIGNAL(signal_set_default_id(int)),m_CNavigateLeft,SLOT(slotSetDefaultSelected(int)));

    emit signal_set_default_id(CNavigateLeft::kEnumVideotapeQuery);

}

void CVideotapeWidget::slot_videotape_widget_event(int _type,QString _text)
{
    m_CHeadWidget->set_head_widget_title(_text);
    m_current_select_id = _type;
    switch(_type)
    {
        case CNavigateLeft::kEnumVideotapeQuery://录像查询
        {
            goto_videotape_query();
        }break;

    }
}

void CVideotapeWidget::goto_videotape_query()//录像查询
{
    if(NULL == m_CVideotapeQueryWidget)
    {
        m_CVideotapeQueryWidget = new CVideotapeQueryWidget(CVideotapeQueryWidget::kEnumVideotapeQuery,this);
        m_stackwidget_navigate_right->addWidget(m_CVideotapeQueryWidget);
        connect(m_CVideotapeQueryWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(m_CVideotapeQueryWidget,SIGNAL(signal_window_home()),this,SLOT(slot_pushbutton_home()));
        connect(this,SIGNAL(signal_videotape_query_user_return()),m_CVideotapeQueryWidget,SLOT(slot_pushbutton_return()));

        connect(m_CVideotapeQueryWidget,SIGNAL(signal_pushbutton_playback()),MainWindow::getInstance(),SLOT(slot_pushbutton_playback()));

        qRegisterMetaType<CSystemVideotapeQueryEx>("CSystemVideotapeQueryEx");
        connect(CMainPage::getInstance(),SIGNAL(signal_menu_videotape_query_widget_event(CSystemVideotapeQueryEx)),m_CVideotapeQueryWidget,SLOT(slot_videotape_query_widget_event(CSystemVideotapeQueryEx)));
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CVideotapeQueryWidget);
}

void CVideotapeWidget::slot_pushbutton_home()
{
    m_goto_main_page = true;
    CCommonInterface::input_panel_control(false);
    emit signal_pushbutton_home();
}

void CVideotapeWidget::slot_pushbutton_close()
{
    CCommonInterface::input_panel_control(false);
    m_goto_main_page = false;
    emit signal_pushbutton_menu();
}

void CVideotapeWidget::slot_pushbutton_back()
{

    if(CNavigateLeft::kEnumVideotapeQuery == m_current_select_id)
    {
         emit signal_videotape_query_user_return();
    }
    else
    {
        m_goto_main_page = false;
        CCommonInterface::input_panel_control(false);
        emit signal_pushbutton_menu();
    }

}

void CVideotapeWidget::clean_date_list()
{
    if(m_CVideotapeQueryWidget)
        m_CVideotapeQueryWidget->clean_date_list();
}
























