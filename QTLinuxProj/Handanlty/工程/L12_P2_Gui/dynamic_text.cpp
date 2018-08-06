#include "dynamic_text.h"

#include "./class/common_interface.h"

CDynamicText::CDynamicText(QWidget *parent) :
    QWidget(parent)
{
    m_timer_text_roll = NULL;
    m_text_pos = 0;

}

CDynamicText::~CDynamicText()
{
    if(m_timer_text_roll)
        delete m_timer_text_roll;
}

void CDynamicText::set_text(CDynTextParam &_param)
{
    m_CDynTextParam = _param;

    QString str_sheet = "";
    //文字滚动
    if(NULL == m_timer_text_roll)
    {
        m_timer_text_roll = new QTimer(this);
        m_timer_text_roll->setInterval(1000);
        connect(m_timer_text_roll, SIGNAL(timeout()), this, SLOT(slot_text_roll_timer()));
    }

    int real_length = CCommonInterface::get_string_length(_param.m_control_source_text,_param.m_control_font_size/*STATION_NAME_SHOW_FONT_SIZE*/,true);

    if(real_length > _param.m_control_show_length/*STATION_NAME_SHOW_LENGTH*/)
    {
       m_timer_text_roll->start();


       //str_sheet = QString("QPushButton {text-align:left;border:0px;background:transparent;font:31px;color:#49b8ff; }");
       //CCommonInterface::modify_button_sheet(m_pushbutton_station_name,str_sheet);
       //emit signal_text_alignment(kEnumAlignmentLeft);
       m_CDynTextParam.m_action = kEnumAlignmentLeft;
       emit signal_dynamic_text_event(m_CDynTextParam);
    }
    else
    {
        m_timer_text_roll->stop();
        //CEventData eventData;
        //eventData.m_str_data = _name;
        //main_page_notify(kEnumMainPageCurrentStationName,0,&eventData);         //当前站名

        //str_sheet = QString("QPushButton {text-align:center;border:0px;background:transparent;font:31px;color:#49b8ff; }");
        //CCommonInterface::modify_button_sheet(m_pushbutton_station_name,str_sheet);
        //emit signal_text_alignment(kEnumAlignmentRight);

        m_CDynTextParam.m_action = kEnumAlignmentRight;
        emit signal_dynamic_text_event(m_CDynTextParam);
        m_CDynTextParam.m_action = kEnumUpdateText;
        m_CDynTextParam.m_control_show_text = _param.m_control_source_text;
        emit signal_dynamic_text_event(m_CDynTextParam);
    }
    //

}

void CDynamicText::slot_text_roll_timer()
{


    QString scroll_text = m_CDynTextParam.m_control_source_text;
    QString show_text = "";
    int font_size = m_CDynTextParam.m_control_font_size;
    int show_length = m_CDynTextParam.m_control_show_length;
    int real_length = 0;

    //获取显示文字的长度
    real_length = CCommonInterface::get_string_length(scroll_text.mid(m_text_pos),font_size,true);


    CCommonInterface::get_string_limit(scroll_text.mid(m_text_pos),show_length,show_text,font_size,true);

    //CEventData eventData;
    //eventData.m_str_data = show_text;
    //main_page_notify(kEnumMainPageCurrentStationName,0,&eventData);         //当前站名

    m_CDynTextParam.m_action = kEnumUpdateText;
    m_CDynTextParam.m_control_show_text = show_text;
    emit signal_dynamic_text_event(m_CDynTextParam);

    m_text_pos++;
    if(real_length <= show_length)
    {
        m_text_pos = 0;
    }

}


