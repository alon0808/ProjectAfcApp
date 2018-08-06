#include "message_widget.h"

#include "class/common_interface.h"
#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"
#include "class/ltystring.h"
//预设事件
CPresetEventWidget::CPresetEventWidget(QWidget *parent) :
    QWidget(parent)
{
   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CPresetEventWidget");
   this->setStyleSheet("QWidget#CPresetEventWidget{background-color:#ffffff;}");

   ////////////////////////////////////////////////////////////////////////////////预设事件

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    m_list_width = 650;
    m_list_height = 540;

    m_preset_event_list = new CCommonQueryList(CCommonQueryList::kEnumPresetEventList
                                              ,m_list_width,m_list_height, this,true);
    column_width_init();
    m_preset_event_list->setFixedSize(m_list_width,m_list_height);
    m_preset_event_list->move(290 - 250,158 - MAIN_HEAD_HEIGHT);
    m_preset_event_list->setHorizontalHeaderVisible(false);

    connect(m_preset_event_list,SIGNAL(signalRowDoubleClicked(int,int)),SLOT(slot_pre_event_clicked_event(int,int)));

    //QTimer::singleShot(10,this,SLOT(slot_message_timer_init_1()));
}
#include "class/common/card_dialog.h"
void CPresetEventWidget::slot_pre_event_clicked_event(int _type,int row)
{
    //CCommonInterface::printf_debug("slot_pre_event_clicked_event _type:" + QString::number(_type));
    if(4 != _type)
        return;

    CCommonQueryMode mode;
    if(m_preset_event_list->GetRowSelect(row,mode))
    {
        CPresetEvent *pPresetEvent = (CPresetEvent *)mode.data;
        if(3 == pPresetEvent->m_event_code)
        {
            CCardDialog::getInstance()->move(150 + (1024 - 250 - 540)/2 + 50,166);
            CCardDialog::getInstance()->card_init();
            CCardDialog::getInstance()->set_content(tr("上班签到与签退"),"",CLtyString::getInstance()->m_confirm,CLtyString::getInstance()->m_cancel);
            CCardDialog::getInstance()->exec();

            if(CCardDialog::KEnumButtonOk == CCardDialog::getInstance()->get_result())
            {
                QString str_card_number = CCardDialog::getInstance()->get_card_number();
                CProtocol::getInstance()->call_json_message_set(3,CProtocol::kEnumPresetEvent,pPresetEvent->m_event_code,str_card_number);
            }
        }
        else
        {
            CProtocol::getInstance()->call_json_message_set(2,CProtocol::kEnumPresetEvent,pPresetEvent->m_event_code);
        }

    }



    //CCommonInterface::printf_debug("slot_pre_event_clicked_event row:" + QString::number(row));

}
void CPresetEventWidget::column_width_init()
{
    m_preset_event_list->setColumnCount(2);
    m_preset_event_list->setColumnWidth(0,70,tr(""));
    m_preset_event_list->setColumnWidth(1,m_list_width - 70,tr(""));

}

//重要短信
CImportantMessageWidget::CImportantMessageWidget(QWidget *parent) :
    QWidget(parent)
{
   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CImportantMessageWidget");
   this->setStyleSheet("QWidget#CImportantMessageWidget{background-color:#ffffff;}");

    m_list_width = 350;
    m_list_height = 500;

    m_important_message_list = new CCommonQueryList(CCommonQueryList::kEnumImportantMessageList
                                          ,m_list_width,m_list_height, this);
    column_width_init();
    m_important_message_list->setFixedSize(m_list_width,m_list_height);
    m_important_message_list->move(20,64 + 20);


    QTimer::singleShot(10,this,SLOT(slot_message_timer_init_2()));
    import_message_init();
}
void CImportantMessageWidget::column_width_init()
{
    m_important_message_list->setColumnCount(2);
    m_important_message_list->setColumnWidth(0,70,tr("序号"));
    m_important_message_list->setColumnWidth(1,m_list_width - 70,tr("接收时间"));

}

//普通短信
CNormalMessageWidget::CNormalMessageWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->setObjectName("CNormalMessageWidget");
    this->setStyleSheet("QWidget#CNormalMessageWidget{background-color:#ffffff;}");
#if 0
    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");
#endif

    scroll_layout();

    m_list_width = 650;
    m_list_height = 540;

    m_normal_message_list = new CCommonQueryList(CCommonQueryList::kEnumNormalMessageList
                                              ,m_list_width,m_list_height, m_widget_main,true);
    column_width_init();
    m_normal_message_list->setFixedSize(m_list_width,m_list_height);
    m_normal_message_list->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50);
    m_normal_message_list->setHorizontalHeaderVisible(false);
   // m_normal_message_list->ShowGrid(true);

    //QTimer::singleShot(10,this,SLOT(slot_message_timer_init_3()));
    connect(m_normal_message_list,SIGNAL(signalRowDoubleClicked(int,int)),SLOT(slot_normal_message_clicked_event(int,int)));

    //m_CMessageContentWidget = new CMessageContentWidget(this);
    //m_CMessageContentWidget->move(20 + 350,64 + 20);
    message_content_init();

}

void CNormalMessageWidget::message_content_init()
{
#if 0
    QWidget *pRectWidget1 = new QWidget(m_widget_main1);
    pRectWidget1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget1->setFixedSize(620,77);
    pRectWidget1->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");
#endif

    QString str_show = tr("消息详情");
    m_label_title = new QLabel(m_widget_main1);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background-color:#275BA7;font:38px;color:#80E3FF;}";
    CCommonInterface::init_label_text(m_label_title,290 - 250,159 - MAIN_HEAD_HEIGHT - 50,620,77,str_show,str_sheet,Qt::AlignCenter,33);

    //620,440
    m_textedit_message = new QTextEdit(m_widget_main1);
    m_textedit_message->setFixedSize(620,440);
    m_textedit_message->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1);

    m_textedit_message->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_textedit_message->setStyleSheet("QTextEdit{border:0px solid #ffffff;background:transparent;color:#8EA1BF;}");
    m_textedit_message->setReadOnly(true);

}

#include <QDebug>
void CNormalMessageWidget::slot_normal_message_clicked_event(int _type,int row)
{
    //qDebug() << "CNormalMessageWidget " << row;

    if(3 != _type)
        return;
    m_widget_main->hide();
    m_widget_main1->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_main1);

    QTextCursor tc = m_textedit_message->textCursor();
    tc.select(QTextCursor::Document);

    tc.removeSelectedText();
#if 0
    QString str_content = "1.请各位司机注意了\r\n\
2.请各位司机注意了\r\n\
3.请各位司机注意了\r\n\
4.请各位司机注意了\r\n\
5.请各位司机注意了\r\n\
6.请各位司机注意了\r\n\
7.请各位司机注意了\r\n\
8.请各位司机注意了\r\n\
9.请各位司机注意了\r\n\
10.请各位司机注意了\r\n\
11.请各位司机注意了\r\n\
12.请各位司机注意了\r\n\
13.请各位司机注意了\r\n\
14.请各位司机注意了\r\n\
15.请各位司机注意了\r\n\
16.请各位司机注意了\r\n\
17.请各位司机注意了\r\n\
18.请各位司机注意了\r\n\
19.请各位司机注意了\r\n\
20.请各位司机注意了\r\n\
21.请各位司机注意了";
#endif
    CCommonQueryMode mode;
    m_normal_message_list->GetRowSelect(row,mode);
    CImportantMessage *pMessage = (CImportantMessage *)mode.data;
    if(pMessage)
    {
        m_textedit_message->append(pMessage->m_content);
        int iDocumentHeight = m_textedit_message->document()->size().rheight();

        int height = iDocumentHeight - 470;
        if(height > 0)
        {
            m_textedit_message->setFixedHeight(iDocumentHeight);
            m_widget_main1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + height);
        }
        else
        {
            m_widget_main1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
        }
    }

}

void CNormalMessageWidget::slot_pushbutton_return()
{
    if(m_widget_main1->isVisible())
    {
        m_widget_main->show();
        m_widget_main1->hide();
        m_vboxlayout_widget->removeWidget(m_widget_main1);
        m_vboxlayout_widget->addWidget(m_widget_main);
    }
    else
    {
        emit signal_window_close();
    }
}


void CNormalMessageWidget::column_width_init()
{
    m_normal_message_list->setColumnCount(2);
    m_normal_message_list->setColumnWidth(0,70,tr("序号"));
    m_normal_message_list->setColumnWidth(1,m_list_width - 70,tr("接收时间"));

}
void CNormalMessageWidget::scroll_layout()
{

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

#ifdef APPLICATION_TYPE_P2
    m_scroll_widget_width = NAVIGATE_RIGHT_WIDGET_WIDTH - 15;
    m_scroll_widget_height = NAVIGATE_RIGHT_WIDGET_HEIGHT - 50 - 50;
#else
    m_scroll_widget_width = NAVIGATE_RIGHT_WIDGET_WIDTH - 30;
    m_scroll_widget_height = NAVIGATE_RIGHT_WIDGET_HEIGHT - 50 - 50;
#endif
    m_scroll_widget = new QWidget(this);
    m_scroll_widget->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_scroll_widget->move(250 - 250,50);
    m_scroll_widget->setStyleSheet("QWidget{border:0px solid #DD4F43;background: transparent;}");

    m_pMainLayout = new QVBoxLayout();//左 上 右 下
    m_pMainLayout->setContentsMargins(0,0,0,0);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setAlignment(Qt::AlignLeft/* | Qt::AlignTop*/);
    m_pMainLayout->addStretch(0);
    m_scroll_widget->setLayout(m_pMainLayout);

    m_scrollarea_1=new QScrollArea(m_scroll_widget);  //新建了一条滚动条
    m_scrollarea_1->setFixedHeight(m_scroll_widget_height);
    m_scrollarea_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollarea_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pMainLayout->addWidget(m_scrollarea_1);

    m_ScrollController = new ScrollController(m_scroll_widget);
    m_ScrollController->addMonitorWidget((QScrollArea *)(m_scrollarea_1));
    m_ScrollController->setVerticalScrollBarStyleSheet(CCommonData::getInstance()->get_scroll_sheet(),0,50,50);
    m_ScrollController->set_scroll_right_length(20);

    QWidget * pwidget=new QWidget(this);

    pwidget->setAttribute(Qt::WA_StyledBackground);
    pwidget->setObjectName("pwidget");
    pwidget->setStyleSheet("QWidget#pwidget { background: transparent;}");

    m_vboxlayout_widget=new QVBoxLayout(pwidget);
    m_vboxlayout_widget->setContentsMargins(0,0,0,0);
    m_vboxlayout_widget->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    m_vboxlayout_widget->setSpacing(0);


    pwidget->setLayout(m_vboxlayout_widget);
    m_scrollarea_1->setWidget(pwidget);
    m_scrollarea_1->setWidgetResizable(true);


    m_widget_main = new QWidget;
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);

    m_widget_main->setStyleSheet("QWidget{background: transparent;}");
    m_vboxlayout_widget->addWidget(m_widget_main);

    m_widget_main1 = new QWidget;
    m_widget_main1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);

    m_widget_main1->setStyleSheet("QWidget{background: transparent;}");
    m_vboxlayout_widget->addWidget(m_widget_main);
    m_widget_main1->hide();

}
//行车计划
CVehiclePlanWidget::CVehiclePlanWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->setObjectName("CVehiclePlanWidget");
    this->setStyleSheet("QWidget#CVehiclePlanWidget{background-color:#ffffff;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");


    //////////////////////////////////////////////////////////////////////////////////行车计划
    m_list_width = 650;
    m_list_height = 540;

    vehicle_plan_list = new CCommonQueryList(CCommonQueryList::kEnumVehiclePlanList
                                          ,m_list_width,m_list_height, this,true);
    column_width_init();
    vehicle_plan_list->setFixedSize(m_list_width,m_list_height);
    vehicle_plan_list->move(290 - 250,158 - MAIN_HEAD_HEIGHT);
    //vehicle_plan_list->ShowGrid(true);
    vehicle_plan_list->setHorizontalHeaderVisible(false);

    //QTimer::singleShot(10,this,SLOT(slot_message_timer_init_4()));

}

void CVehiclePlanWidget::column_width_init()
{
    vehicle_plan_list->setColumnCount(2);
    vehicle_plan_list->setColumnWidth(0,50,tr(""));
    vehicle_plan_list->setColumnWidth(1,m_list_width -50,tr(""));
    //vehicle_plan_list->setColumnWidth(2,150,tr("发车时间"));
    //vehicle_plan_list->setColumnWidth(3,180,tr("属性"));

}

CMessageWidget::CMessageWidget(QWidget *parent) :
    QDialog(parent)
{

    this->setFixedSize(1024,768);
    this->setObjectName("CMessageWidget");
    this->setStyleSheet("QDialog#CMessageWidget{background-color:#000000;}");
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);


    m_CHeadWidget = new CHeadWidget(kEnumHeadWidgetMessage,this);
    connect(m_CHeadWidget,SIGNAL(signal_button_back()),this,SLOT(slot_pushbutton_back()));
    connect(m_CHeadWidget,SIGNAL(signal_button_home()),this,SLOT(slot_pushbutton_home()));

    int button_height = 78 + 18;;
    int navigate_width = 230;
    int navigate_height = 3 * button_height;

    m_widget_bg2 = new QWidget(this);
    m_widget_bg2->setFixedSize(190,636);
    m_widget_bg2->move(40,107);
    m_widget_bg2->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/main_page/menu/system_manage/nav_left_background.png);}");


    m_CNavigateLeft = new CNavigateLeft(navigate_width,button_height,navigate_height,this);
    m_CNavigateLeft->move(20,106 + 58 + 10);
    m_CNavigateLeft->setFixedSize(navigate_width,navigate_height);


    m_CNavigateLeft->add_button(CNavigateLeft::kEnumPresetEvent,tr("预设事件"));
    //m_CNavigateLeft->add_button(CNavigateLeft::kEnumImportantMessage,tr("重要短信"));
    m_CNavigateLeft->add_button(CNavigateLeft::kEnumNormalMessage,tr("消息"));
    m_CNavigateLeft->add_button(CNavigateLeft::kEnumVehiclePlan,tr("行车计划"));




    connect(m_CNavigateLeft,SIGNAL(signalCurrentClassificationChanged(int,QString)),this,SLOT(slot_message_widget_event(int,QString)));

    connect(this,SIGNAL(signal_set_default_id(int)),m_CNavigateLeft,SLOT(slotSetDefaultSelected(int)));


    m_CPresetEventWidget = NULL;
    m_CImportantMessageWidget = NULL;
    m_CNormalMessageWidget = NULL;
    m_CVehiclePlanWidget = NULL;

    message_init();




}

void CMessageWidget::slot_pushbutton_home()
{
    m_goto_main_page = true;
    CCommonInterface::input_panel_control(false);
    emit signal_pushbutton_home();
}

void CMessageWidget::slot_pushbutton_close()
{
    m_goto_main_page = false;
    CCommonInterface::input_panel_control(false);
    emit signal_pushbutton_menu();
}

void CMessageWidget::slot_pushbutton_back()
{
    if(CNavigateLeft::kEnumNormalMessage == m_current_index)
    {
        emit signal_message_content_user_return();
    }
    else
    {
        m_goto_main_page = false;
        CCommonInterface::input_panel_control(false);
        emit signal_pushbutton_menu();
    }
}



void CMessageWidget::slot_message_widget_event(int _type,QString _text)
{
    m_CHeadWidget->set_head_widget_title(_text);
    m_current_index = _type;
    switch(_type)
    {
        case CNavigateLeft::kEnumPresetEvent:           //预设事件
        {
            show_preset_event();
        }break;
        case CNavigateLeft::kEnumImportantMessage:      //重要短信
        {
            //show_important_message();
        }break;
        case CNavigateLeft::kEnumNormalMessage:         //普通短信
        {
            show_normal_message();
        }break;
        case CNavigateLeft::kEnumVehiclePlan:           //行车计划
        {
            show_vehicle_plan();
        }break;
    }
}

void CMessageWidget::show_preset_event()       //显示预设事件
{
    if(NULL == m_CPresetEventWidget)
    {
        m_CPresetEventWidget = new CPresetEventWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CPresetEventWidget);

    }
    CProtocol::getInstance()->call_json_message_set(1,CProtocol::kEnumPresetEvent);

    m_stackwidget_navigate_right->setCurrentWidget(m_CPresetEventWidget);
}
void CMessageWidget::show_important_message()  //显示重要短信
{
    if(NULL == m_CImportantMessageWidget)
    {
        m_CImportantMessageWidget = new CImportantMessageWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CImportantMessageWidget);
    }
    m_stackwidget_navigate_right->setCurrentWidget(m_CImportantMessageWidget);
}
void CMessageWidget::show_normal_message()     //显示普通短信
{
    if(NULL == m_CNormalMessageWidget)
    {
        m_CNormalMessageWidget = new CNormalMessageWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CNormalMessageWidget);
        connect(m_CNormalMessageWidget,SIGNAL(signal_window_close()),this,SLOT(slot_pushbutton_close()));
        connect(this,SIGNAL(signal_message_content_user_return()),m_CNormalMessageWidget,SLOT(slot_pushbutton_return()));


    }
    CProtocol::getInstance()->call_json_message_set(1,CProtocol::kEnumMessage);
    m_stackwidget_navigate_right->setCurrentWidget(m_CNormalMessageWidget);
}
void CMessageWidget::show_vehicle_plan()       //显示行车计划
{
    if(NULL == m_CVehiclePlanWidget)
    {
        m_CVehiclePlanWidget = new CVehiclePlanWidget(this);
        m_stackwidget_navigate_right->addWidget(m_CVehiclePlanWidget);

    }
    CProtocol::getInstance()->call_json_message_set(1,CProtocol::kEnumVehiclePlan);

    m_stackwidget_navigate_right->setCurrentWidget(m_CVehiclePlanWidget);
}

void CPresetEventWidget::slot_message_timer_init_1()
{
        CCommonQueryMode mode_type;

////////////////////////////////////////////////////////////////////////////////预设事件
            CPresetEvent *pPresetEvent = NULL;
            pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 1;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 1;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 2;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 2;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 3;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 3;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 4;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 4;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 5;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 5;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 6;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 6;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 7;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 7;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 8;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 8;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 9;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 9;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 10;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 10;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 11;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 11;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 12;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 12;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 13;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 13;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 14;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 14;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 15;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 15;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 16;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 16;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 17;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 17;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 18;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 18;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 19;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 19;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
                pPresetEvent = new CPresetEvent;
            pPresetEvent->m_number = 20;
            pPresetEvent->m_content = tr("紧急报警开始");
            pPresetEvent->m_event_code = 20;
            pPresetEvent->m_shortcut_key = tr("F8+F1");
            mode_type.data = pPresetEvent;
            m_preset_event_list->AddData(mode_type);
}
void CImportantMessageWidget::slot_message_timer_init_2()
{
        CCommonQueryMode mode_type;
        CImportantMessage *pImportantMessage = NULL;
            ///////////////////////////////////////////////////////////////////////////////////重要短信



            pImportantMessage = new CImportantMessage;
            pImportantMessage->m_number = 1;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 2;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 3;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 4;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 5;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 6;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 7;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 8;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 9;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);


            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 10;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 11;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 12;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 13;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 14;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 15;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 16;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_important_message_list->AddData(mode_type);
}
void CNormalMessageWidget::slot_message_timer_init_3()
{
        CCommonQueryMode mode_type;
    CImportantMessage *pImportantMessage = NULL;
            //////////////////////////////////////////////////////////////////////////////////普通短信

            pImportantMessage = new CImportantMessage;
            pImportantMessage->m_number = 1;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            pImportantMessage->m_content = "1.请各位司机注意了\r\n\
2.请各位司机注意了\r\n\
3.请各位司机注意了\r\n\
4.请各位司机注意了\r\n\
5.请各位司机注意了\r\n\
6.请各位司机注意了\r\n\
7.请各位司机注意了\r\n\
8.请各位司机注意了\r\n\
9.请各位司机注意了\r\n\
10.请各位司机注意了\r\n\
11.请各位司机注意了\r\n\
12.请各位司机注意了\r\n\
13.请各位司机注意了\r\n\
14.请各位司机注意了\r\n\
15.请各位司机注意了\r\n\
16.请各位司机注意了\r\n\
17.请各位司机注意了\r\n\
18.请各位司机注意了\r\n\
19.请各位司机注意了\r\n\
20.请各位司机注意了\r\n\
21.请各位司机注意了";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 2;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            pImportantMessage->m_content = tr("明天休息一天");
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 3;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            pImportantMessage->m_content = tr("雨天路滑，请小心驾驶，下班后来财务室领取礼品1。");
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 4;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            pImportantMessage->m_content = tr("雨天路滑，请小心驾驶，下班后来财务室领取礼品2。");
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 5;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 6;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 7;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 8;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 9;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);


            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 10;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 11;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 12;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 13;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 14;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 15;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);

            pImportantMessage = new CImportantMessage;
                pImportantMessage->m_number = 16;
            pImportantMessage->m_recv_time = "2016/07/25/17:55:55";
            mode_type.data = pImportantMessage;
            m_normal_message_list->AddData(mode_type);
}
void CVehiclePlanWidget::slot_message_timer_init_4()
{
        CCommonQueryMode mode_type;

            //////////////////////////////////////////////////////////////////////////////////行车计划


            CVehiclePlan *pVehiclePlan = NULL;

            pVehiclePlan = new CVehiclePlan;
            pVehiclePlan->m_number = 1;
            pVehiclePlan->m_recv_time = "2016/07/25/17:55:55";
            pVehiclePlan->m_start_time = "17:00:00";
            pVehiclePlan->m_attribute = tr("执行完毕");

            mode_type.data = pVehiclePlan;
            vehicle_plan_list->AddData(mode_type);

                pVehiclePlan = new CVehiclePlan;
            pVehiclePlan->m_number = 2;
            pVehiclePlan->m_recv_time = "2016/07/25/17:55:55";
            pVehiclePlan->m_start_time = "17:00:00";
            pVehiclePlan->m_attribute = tr("执行完毕");

            mode_type.data = pVehiclePlan;
            vehicle_plan_list->AddData(mode_type);

                pVehiclePlan = new CVehiclePlan;
            pVehiclePlan->m_number = 3;
            pVehiclePlan->m_recv_time = "2016/07/25/17:55:55";
            pVehiclePlan->m_start_time = "17:00:00";
            pVehiclePlan->m_attribute = tr("执行完毕");

            mode_type.data = pVehiclePlan;
            vehicle_plan_list->AddData(mode_type);

                pVehiclePlan = new CVehiclePlan;
            pVehiclePlan->m_number = 4;
            pVehiclePlan->m_recv_time = "2016/07/25/17:55:55";
            pVehiclePlan->m_start_time = "17:00:00";
            pVehiclePlan->m_attribute = tr("执行完毕");

            mode_type.data = pVehiclePlan;
            vehicle_plan_list->AddData(mode_type);

                pVehiclePlan = new CVehiclePlan;
            pVehiclePlan->m_number = 5;
            pVehiclePlan->m_recv_time = "2016/07/25/17:55:55";
            pVehiclePlan->m_start_time = "17:00:00";
            pVehiclePlan->m_attribute = tr("执行完毕");

            mode_type.data = pVehiclePlan;
            vehicle_plan_list->AddData(mode_type);

                pVehiclePlan = new CVehiclePlan;
            pVehiclePlan->m_number = 6;
            pVehiclePlan->m_recv_time = "2016/07/25/17:55:55";
            pVehiclePlan->m_start_time = "17:00:00";
            pVehiclePlan->m_attribute = tr("执行完毕");

            mode_type.data = pVehiclePlan;
            vehicle_plan_list->AddData(mode_type);

                pVehiclePlan = new CVehiclePlan;
            pVehiclePlan->m_number = 7;
            pVehiclePlan->m_recv_time = "2016/07/25/17:55:55";
            pVehiclePlan->m_start_time = "17:00:00";
            pVehiclePlan->m_attribute = tr("执行完毕");

            mode_type.data = pVehiclePlan;
            vehicle_plan_list->AddData(mode_type);

                pVehiclePlan = new CVehiclePlan;
            pVehiclePlan->m_number = 8;
            pVehiclePlan->m_recv_time = "2016/07/25/17:55:55";
            pVehiclePlan->m_start_time = "17:00:00";
            pVehiclePlan->m_attribute = tr("执行完毕");

            mode_type.data = pVehiclePlan;
            vehicle_plan_list->AddData(mode_type);

                pVehiclePlan = new CVehiclePlan;
            pVehiclePlan->m_number = 9;
            pVehiclePlan->m_recv_time = "2016/07/25/17:55:55";
            pVehiclePlan->m_start_time = "17:00:00";
            pVehiclePlan->m_attribute = tr("执行完毕");

            mode_type.data = pVehiclePlan;
            vehicle_plan_list->AddData(mode_type);

                pVehiclePlan = new CVehiclePlan;
            pVehiclePlan->m_number = 10;
            pVehiclePlan->m_recv_time = "2016/07/25/17:55:55";
            pVehiclePlan->m_start_time = "17:00:00";
            pVehiclePlan->m_attribute = tr("执行完毕");

            mode_type.data = pVehiclePlan;
            vehicle_plan_list->AddData(mode_type);

            pVehiclePlan = new CVehiclePlan;
            pVehiclePlan->m_number = 11;
            pVehiclePlan->m_recv_time = "2016/07/25/17:55:55";
            pVehiclePlan->m_start_time = "17:00:00";
            pVehiclePlan->m_attribute = tr("执行完毕");

            mode_type.data = pVehiclePlan;
            vehicle_plan_list->AddData(mode_type);
}



void CMessageWidget::message_init()
{
    m_stackwidget_navigate_right = new QStackedWidget(this);
    m_stackwidget_navigate_right->move(MAIN_NAVIGATE_LEFT,MAIN_HEAD_HEIGHT);
    m_stackwidget_navigate_right->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_stackwidget_navigate_right->setObjectName(QString::fromUtf8("m_stackwidget_navigate_right"));
    m_stackwidget_navigate_right->setContentsMargins(0, 0, 0, 0);

    QTimer::singleShot(100,this,SLOT(slot_message_timer_init()));

}
void CMessageWidget::slot_message_timer_init()
{
    emit signal_set_default_id(CNavigateLeft::kEnumPresetEvent);
}

CMessageContentWidget::CMessageContentWidget(QWidget *parent) :
    QWidget(parent)
{
        m_widget_message_bg = new QWidget(this);
    m_widget_message_bg->setFixedSize(434,514);
    m_widget_message_bg->move(0,0);
    m_widget_message_bg->setStyleSheet("QWidget {border:0px;border-image:url(:/img/res/message/message_bg.png);}");

     //   QPushButton *m_pushbutton_message_up;
    //QPushButton *m_pushbutton_message_down;

    m_pushbutton_message_up = new QPushButton(this);
    QString str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
            .arg(":/img/res/message/message_1.png");

    CCommonInterface ::init_new_button(m_pushbutton_message_up,0 + (434 - 82)/2 + 20,0 + 5,82,68,str_sheet);
    connect(m_pushbutton_message_up,SIGNAL(clicked()),this,SLOT(slot_pushbutton_message_up()));

        m_pushbutton_message_down = new QPushButton(this);
    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }")
            .arg(":/img/res/message/message_2.png");

    CCommonInterface ::init_new_button(m_pushbutton_message_down,0 + (434 - 82)/2 + 20,0 + 514 - 68 - 20,82,68,str_sheet);
    connect(m_pushbutton_message_down,SIGNAL(clicked()),this,SLOT(slot_pushbutton_message_down()));

    QString show_content = tr("消息内容");
    m_label_message_content = new QLabel(this);

    int str_length = CCommonInterface::get_string_length(show_content,26);
    m_label_message_content->move(0 + (434 - str_length)/2 + 30,0 + 5 + 68);
    m_label_message_content->setFixedSize(str_length, 50);
    m_label_message_content->setText(show_content);

    m_label_message_content->setStyleSheet("QLabel{border:0px;background:transparent;font:35;}");

    m_textedit_message = new QTextEdit(this);
    m_textedit_message->setFixedSize(300,270);
    m_textedit_message->move(0 + (434 - 300)/2 + 20,0 + 5 + 68 + 50 + 20);
    m_textedit_message->setText(tr("1.请各位司机注意了，下班后请到公司财务部领取当月奖金，请准时到场，过时不候！\
2.请各位司机注意了，下班后请到公司财务部领取当月奖金，请准时到场，过时不候！\
3.请各位司机注意了，下班后请到公司财务部领取当月奖金，请准时到场，过时不候！\
4.请各位司机注意了，下班后请到公司财务部领取当月奖金，请准时到场，过时不候！\
5.请各位司机注意了，下班后请到公司财务部领取当月奖金，请准时到场，过时不候！"));


    m_textedit_message->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_textedit_message->setStyleSheet("QTextEdit{border:0px;background:transparent;}");
}

void CImportantMessageWidget::import_message_init()
{
    m_CMessageContentWidget = new CMessageContentWidget(this);
    m_CMessageContentWidget->move(20 + 350,64 + 20);


}

void CMessageContentWidget::slot_pushbutton_message_up()
{
    int page_size = m_textedit_message->verticalScrollBar()->pageStep();
    int current_value = m_textedit_message->verticalScrollBar()->value();

    m_textedit_message->verticalScrollBar()->setValue(current_value - page_size);
}
void CMessageContentWidget::slot_pushbutton_message_down()
{
    int page_size = m_textedit_message->verticalScrollBar()->pageStep();
    int current_value = m_textedit_message->verticalScrollBar()->value();

    m_textedit_message->verticalScrollBar()->setValue(current_value + page_size);
}

void CMessageWidget::slot_message_widget_notify_event(int _type,CCommonQueryMode _mode)
{
    //qDebug() << "slot_message_widget_notify_event start";
    switch(_type)
    {
        case 1:
        {
            CPresetEvent *pPresetEvent = (CPresetEvent *)_mode.data;
            //qDebug() << "slot_message_widget_notify_event 1";
            if(1 == pPresetEvent->m_number)
            {
                m_CPresetEventWidget->m_preset_event_list->slotListClean();
            }
            m_CPresetEventWidget->m_preset_event_list->AddData(_mode);
            //qDebug() << "slot_message_widget_notify_event 2";

        }break;
        case 2:
        {

            CImportantMessage *pMessage = (CImportantMessage *)_mode.data;
            if(1 == pMessage->m_number)
            {
                m_CNormalMessageWidget->m_normal_message_list->slotListClean();
            }

            m_CNormalMessageWidget->m_normal_message_list->AddData(_mode);

        }break;
        case 3:
        {
            CVehiclePlan *pVehiclePlan = (CVehiclePlan *)_mode.data;
            if(1 == pVehiclePlan->m_number)
            {
                m_CVehiclePlanWidget->vehicle_plan_list->slotListClean();
            }

            m_CVehiclePlanWidget->vehicle_plan_list->AddData(_mode);
        }break;
    }
    //qDebug() << "slot_message_widget_notify_event end";
}






