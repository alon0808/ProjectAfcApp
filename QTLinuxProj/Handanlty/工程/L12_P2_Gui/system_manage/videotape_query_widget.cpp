#include "videotape_query_widget.h"

#include <QListView>

#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"

#include "./class/common_interface.h"

#include "videotape_playback.h"
#include "videotape_export.h"
#include "class/ltystring.h"

#include "main_page.h"
//#include "class/common/my_export.h"
#include "class/common/my_messagebox.h"
#include "class/common/progress_box.h"

#include <QDebug>

CVideotapeQueryWidget::CVideotapeQueryWidget(char _type,QWidget *parent) :
    QWidget(parent)
{
    m_type = _type;
     this->setObjectName("CVideotapeQueryWidget");
    this->setStyleSheet("QWidget#CVideotapeQueryWidget{background-color:#000000;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->move(0,0);

        QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    pWidget->move(0,0);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");

    m_pMainLayout = new QVBoxLayout();//左 上 右 下
    m_pMainLayout->setContentsMargins(0,0,0,0);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setAlignment(Qt::AlignLeft/* | Qt::AlignTop*/);
    m_pMainLayout->addStretch(0);
    this->setLayout(m_pMainLayout);

    m_widget_query_date = new QWidget;
    m_widget_query_date->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);

    m_widget_query_date->setStyleSheet("QWidget{background-color:#000000;}");
    m_pMainLayout->addWidget(m_widget_query_date);

    m_widget_query_result = new QWidget;
    m_widget_query_result->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_widget_query_result->setStyleSheet("QWidget{background-color:#000000;}");

    m_widget_bg = new QWidget(m_widget_query_date);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    m_widget_bg1 = new QWidget(m_widget_query_result);
    m_widget_bg1->setFixedSize(744,636);
    m_widget_bg1->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg1->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");


    videotape_query_date();

    videotape_query_init();
    set_current_show(kEnumShowQueryDate);


}

void CVideotapeQueryWidget::videotape_query_date()
{

    QString str_show = tr("位置:");
    m_label_1 = new QLabel(m_widget_query_date);
    QString str_sheet = "QLabel{border:0px;font:24px;color:#8ea2c0;background:transparent;}";
    CCommonInterface::init_label_text(m_label_1,680 - 250,186 - MAIN_HEAD_HEIGHT,0,30,str_show,str_sheet);

    m_combobox_1 = new QComboBox(m_widget_query_date);
    //m_combobox_1->setGeometry( QRect(m_widget_query_date->width() - MARGIN_RIGHT - COMBOBOX_WIDTH,ROW_HEIGHT * 0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2, COMBOBOX_WIDTH, COMBOBOX_HEIGHT ) );
    m_combobox_1->move(680 - 250,221 - MAIN_HEAD_HEIGHT);
    m_combobox_1->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_1->addItem(tr("硬盘"));
    m_combobox_1->addItem(tr("EMMC"));
    m_combobox_1->addItem(tr("SD卡"));
    m_combobox_1->setView(new QListView());

    str_show = tr("日期:");
    m_label_1 = new QLabel(m_widget_query_date);
    CCommonInterface::init_label_text(m_label_1,680 - 250,315 - MAIN_HEAD_HEIGHT,0,30,str_show,str_sheet);

    m_combobox_2 = new QComboBox(m_widget_query_date);
    m_combobox_2->move(680 - 250,349 - MAIN_HEAD_HEIGHT);
    m_combobox_2->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_2->addItem(tr("2016年"));
    m_combobox_2->addItem(tr("2015年"));
    m_combobox_2->setView(new QListView());
    m_year_list.push_back(2016);
    m_year_list.push_back(2015);

    m_combobox_3 = new QComboBox(m_widget_query_date);
    m_combobox_3->move(680 - 250,426 - MAIN_HEAD_HEIGHT);
    m_combobox_3->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_3->addItem(tr("1月"));
    m_combobox_3->addItem(tr("2月"));
    m_combobox_3->addItem(tr("3月"));
    m_combobox_3->addItem(tr("4月"));
    m_combobox_3->addItem(tr("5月"));
    m_combobox_3->addItem(tr("6月"));
    m_combobox_3->addItem(tr("7月"));
    m_combobox_3->addItem(tr("8月"));
    m_combobox_3->addItem(tr("9月"));
    m_combobox_3->addItem(tr("10月"));
    m_combobox_3->addItem(tr("11月"));
    m_combobox_3->addItem(tr("12月"));
    m_combobox_3->setView(new QListView());

    //bool ret = false;
    connect(m_combobox_1,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_combobox1_index_changed(int)));
    connect(m_combobox_2,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_combobox2_index_changed(int)));
    connect(m_combobox_3,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_combobox3_index_changed(int)));

    str_show = tr("正常录像");
    m_label_3 = new QLabel(m_widget_query_date);
    int str_length = CCommonInterface::init_label_text(m_label_3,680 - 250,516 - MAIN_HEAD_HEIGHT,0,30,str_show,str_sheet);

    str_sheet = "QLabel{border:0px;font:24px;color:#8ea2c0;background-color:#80e3ff;}";
    m_label_4 = new QLabel(m_widget_query_date);
    CCommonInterface::init_label_text(m_label_4,680 - 250 + str_length + 5,516 - MAIN_HEAD_HEIGHT,24,30,"",str_sheet);


    str_sheet = "QLabel{border:0px;font:24px;color:#8ea2c0;background:transparent;}";
    str_show = tr("报警录像");
    m_label_5 = new QLabel(m_widget_query_date);
    str_length = CCommonInterface::init_label_text(m_label_5,680 - 250,559 - MAIN_HEAD_HEIGHT,0,30,str_show,str_sheet);

    str_sheet = "QLabel{border:0px;font:24px;color:#8ea2c0;background-color:#ff2e2e;}";
    m_label_6 = new QLabel(m_widget_query_date);
    str_length = CCommonInterface::init_label_text(m_label_6,680 - 250 + str_length + 5,559 - MAIN_HEAD_HEIGHT,24,30,"",str_sheet);

    m_pushbutton_ok = new QPushButton(m_widget_query_date);
    CCommonInterface::init_new_button(m_pushbutton_ok,435 - 250,642 - MAIN_HEAD_HEIGHT,160,65,LTY_PUSHBUTTON_OK);
    m_pushbutton_ok->setText(CLtyString::getInstance()->m_confirm);

    m_pushbutton_cancel = new QPushButton(m_widget_query_date);
    CCommonInterface::init_new_button(m_pushbutton_cancel,624 - 250,642 - MAIN_HEAD_HEIGHT,160,65,LTY_PUSHBUTTON_OK);
    m_pushbutton_cancel->setText(CLtyString::getInstance()->m_cancel);
    //
    connect(m_pushbutton_ok,SIGNAL(clicked()),this,SLOT(slot_pushbutton_ok()));
    connect(m_pushbutton_cancel,SIGNAL(clicked()),this,SLOT(slot_pushbutton_cancel()));

    int list_width = 362;
    int list_height = 62 * 7;//448;
    int list_column_width = list_width/7;

    m_date_select_list = new CCommonQueryList(CCommonQueryList::kEnumDateSelect
                                              ,list_width,list_height, m_widget_query_date);

    m_date_select_list->setColumnCount(7);
    m_date_select_list->setColumnWidth(0,list_column_width,tr("一"));
    m_date_select_list->setColumnWidth(1,list_column_width,tr("二"));
    m_date_select_list->setColumnWidth(2,list_column_width,tr("三"));
    m_date_select_list->setColumnWidth(3,list_column_width,tr("四"));
    m_date_select_list->setColumnWidth(4,list_column_width,tr("五"));
    m_date_select_list->setColumnWidth(5,list_column_width,tr("六"));
    m_date_select_list->setColumnWidth(6,list_width - list_column_width * 6,tr("日"));

    m_date_select_list->setFixedSize(list_width,list_height);
    m_date_select_list->move(290 - 250,159 - MAIN_HEAD_HEIGHT);
    m_date_select_list->ShowGrid(true);
    m_date_select_list->setVerticalScrollBarVisible(false);

    slot_combobox3_index_changed(11);


}

void CVideotapeQueryWidget::slot_combobox1_index_changed(int _index)
{

}
void CVideotapeQueryWidget::slot_combobox2_index_changed(int _index)
{
    int select_month = 0;
    get_month_info(true,select_month);
}
void CVideotapeQueryWidget::slot_combobox3_index_changed(int _index)
{
    do
    {
        int select_month = 0;
        get_month_info(false,select_month);

        m_date_select_list->slotListClean();

        if(_index >= 0 && _index <= 11)
        {
            QDate date;
            int first_day_week = 0;
            int last_day_week = 0;
            int day_month = 0;
            int current_year = 0;
#if 0
            for(int i = 0;i < m_year_list.size();i ++)
            {
                QString str_number = QString::number(m_year_list.at(i));
                 QString str_year = m_combobox_2->currentText();
                if(str_year.contains(str_number))
                {
                    current_year = m_year_list.at(i);
                    break;
                }
            }
#endif
            QString str_year = m_combobox_2->currentText();
            str_year = str_year.mid(0,4);
            current_year = str_year.toInt();

            date.setDate(current_year,select_month,1);
            first_day_week = date.dayOfWeek();

            day_month = date.daysInMonth();

            date.setDate(current_year,select_month,day_month);
            last_day_week = date.dayOfWeek();

            CCommonQueryMode mode_type;

            //上一个月数据
            CDateSelect *pDateSelect = NULL;
            pDateSelect = new CDateSelect;

            if(first_day_week > 1)
            {
                int last_month_days = 0;
                if(select_month - 1 == 0)
                {
                    date.setDate(current_year - 1,12,1);
                }
                else
                {
                    date.setDate(current_year,select_month - 1,1);
                }
                last_month_days = date.daysInMonth();


                for(int index = last_month_days - (first_day_week - 1) + 1;index <= last_month_days;index++)
                {

                    CDay my_day;
                    my_day.m_current_month = false;
                    my_day.m_number = index;
                    pDateSelect->m_list.push_back(my_day);

                }
            }

            //当前月数据
            for(int index = 1;index <= day_month;index++)
            {
                CDay my_day;
                my_day.m_current_month = true;
                my_day.m_number = index;
                pDateSelect->m_list.push_back(my_day);

                if(0 == pDateSelect->m_list.size()%7)
                {
                    mode_type.data = pDateSelect;
                    m_date_select_list->AddData(mode_type);
                    pDateSelect = new CDateSelect;
                }
            }

            //下一个月数据
            for(int index = 1;index < 30;index++)
            {

                CDay my_day;
                my_day.m_current_month = false;
                my_day.m_number = index;
                pDateSelect->m_list.push_back(my_day);

                if(0 == pDateSelect->m_list.size()%7)
                {
                    mode_type.data = pDateSelect;
                    m_date_select_list->AddData(mode_type);

                    if(6 == m_date_select_list->GetCount())
                    {
                        break;
                    }
                    pDateSelect = new CDateSelect;
                }

            }

        }

    }while(0);
}

void CVideotapeQueryWidget::set_current_show(char _type)
{
    switch(_type)
    {
        case kEnumShowQueryDate:
        {
            m_widget_query_date->show();
            m_widget_query_result->hide();
            m_pMainLayout->removeWidget(m_widget_query_result);
            m_pMainLayout->addWidget(m_widget_query_date);
        }break;
        case kEnumShowQueryResult:
        {
            m_combobox_4->setCurrentIndex(0);
            m_lineedit_1->setText(tr("00:00:00"));
            m_lineedit_2->setText(tr("23:59:59"));
            m_widget_query_date->hide();
            m_widget_query_result->show();
            m_pMainLayout->removeWidget(m_widget_query_date);
            m_pMainLayout->addWidget(m_widget_query_result);
        }break;
    }
}
/*
    info.m_position = m_combobox_1->currentIndex(); //位置  0:硬盘 1:EMMC 2:SD卡

    QString str_year = m_combobox_2->currentText();
    str_year = str_year.mid(0,4);
    info.m_year = str_year.toInt(); //年
    info.m_month = m_combobox_2->currentIndex() + 1;//月
*/

#include "class/common_interface.h"
void CVideotapeQueryWidget::slot_pushbutton_ok()
{
    VideotapeQueryList info;
    CCommonInterface::printf_debug("slot_pushbutton_ok start");
    do
    {
        info.m_position = m_combobox_1->currentIndex(); //位置  0:硬盘 1:EMMC 2:SD卡
        QString str_year = m_combobox_2->currentText();
        str_year = str_year.replace(tr("年"),"");
        info.m_year = str_year.toInt(); //年

        QString str_month = m_combobox_3->currentText();
        str_month = str_month.replace(tr("月"),"");
        info.m_month = str_month.toInt();
        CCommonQueryMode data;
        CDateSelect *pDateSelect = NULL;

        bool is_select = false;
        bool ret = m_date_select_list->GetRowColumnSelect(data);
        bool is_query = false;
        if(ret)
        {
            pDateSelect = (CDateSelect *)data.data;

            CCommonInterface::printf_debug("slot_pushbutton_ok " + QString::number(data.m_column + 1) + "  " + QString::number(pDateSelect->m_list.size()));
            if(data.m_column + 1 > pDateSelect->m_list.size())
            {
                CCommonInterface::printf_debug("slot_pushbutton_ok exit");
                break;
            }

            if(pDateSelect->m_list.at(data.m_column).m_normal || pDateSelect->m_list.at(data.m_column).m_police)
                is_query = true;

            if(is_query && pDateSelect->m_list.at(data.m_column).m_current_month)
            {
                is_select = true;
            }
        }

        if(!is_select)
        {
#if 0
            CCommonInterface::printf_debug("slot_pushbutton_ok m_normal:" + QString::number(pDateSelect->m_list.at(data.m_column).m_normal)
             + "m_police:" + QString::number(pDateSelect->m_list.at(data.m_column).m_police));
             CCommonInterface::printf_debug("slot_pushbutton_ok is_query:" + QString::number(is_query) +
             "m_current_month:" + QString::number(pDateSelect->m_list.at(data.m_column).m_current_month));
#endif
            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("请先选择日期，再点确认"),CLtyString::getInstance()->m_confirm,"");
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
            break;
        }

        info.m_day = pDateSelect->m_list.at(data.m_column).m_number;  //日

        m_year = info.m_year;
        m_month = info.m_month;
        m_day = info.m_day;


        info.start_hour = 0;
        info.start_minute = 0;
        info.start_second = 0;
        info.end_hour = 23;
        info.end_minute = 59;
        info.end_second = 59;
        //info.channel;

        CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumGetDayList,&info);
        set_current_show(kEnumShowQueryResult);

    }while(0);

    CCommonInterface::printf_debug("slot_pushbutton_ok end");
}


void CVideotapeQueryWidget::slot_pushbutton_cancel()
{
    emit signal_window_close();
}

void CVideotapeQueryWidget::slot_pushbutton_return()
{
    if(m_widget_query_result->isVisible())
    {
        set_current_show(kEnumShowQueryDate);
    }
    else
    {
        emit signal_window_close();
    }
}


void CVideotapeQueryWidget::videotape_query_init()
{
    /*QLabel *m_label_4;
    QLabel *m_label_5;
    QLabel *m_label_6;*/

    QString str_show = tr("位置:");
    QString str_sheet = "QLabel{border:0px;font:24px;color:#8ea2c0;background:transparent;}";
    m_label_7 = new QLabel(m_widget_query_result);
    CCommonInterface::init_label_text(m_label_7,769 - 250,160 - MAIN_HEAD_HEIGHT,0,30,str_show,str_sheet);

    str_show = tr("开始时间:");
    m_label_8 = new QLabel(m_widget_query_result);
    CCommonInterface::init_label_text(m_label_8,769 - 250,288 - MAIN_HEAD_HEIGHT,0,30,str_show,str_sheet);

    str_show = tr("结束时间:");
    m_label_9 = new QLabel(m_widget_query_result);
    CCommonInterface::init_label_text(m_label_9,769 - 250,413 - MAIN_HEAD_HEIGHT,0,30,str_show,str_sheet);

    m_combobox_4 = new QComboBox(m_widget_query_result);

    //QString passageway_pos[19] = {"all","CAM1-1","CAM1-2","CAM2-1","CAM2-2","CAM3-1","CAM3-2","CAM4-1","CAM4-2"
    //                              ,"PON1","PON2","PON3","PON4","PON5","PON6","PON7","PON8","PON9","PON10"};

    m_combobox_4->move(769 - 250,195 - MAIN_HEAD_HEIGHT);
    m_combobox_4->setFixedSize(150,COMBOBOX_HEIGHT);
    m_combobox_4->addItem(tr("全部"));
    for(int index = 1;index < 17;index++)
        m_combobox_4->addItem(QString::number(index));
        //m_combobox_4->addItem(passageway_pos[index]);
#if 0
    m_combobox_4->addItem(tr("CAM1-1"));
    m_combobox_4->addItem(tr("CAM1-2"));
    m_combobox_4->addItem(tr("CAM2-1"));
    m_combobox_4->addItem(tr("CAM2-2"));
    m_combobox_4->addItem(tr("CAM3-1"));
    m_combobox_4->addItem(tr("CAM3-2"));
    m_combobox_4->addItem(tr("CAM4-1"));
    m_combobox_4->addItem(tr("CAM4-2"));
    m_combobox_4->addItem(tr("PON1"));
    m_combobox_4->addItem(tr("PON2"));
    m_combobox_4->addItem(tr("PON3"));
    m_combobox_4->addItem(tr("PON4"));
    m_combobox_4->addItem(tr("PON5"));
    m_combobox_4->addItem(tr("PON6"));
    m_combobox_4->addItem(tr("PON7"));
    m_combobox_4->addItem(tr("PON8"));
    m_combobox_4->addItem(tr("PON9"));
    m_combobox_4->addItem(tr("PON10"));
#endif
    m_combobox_4->setView(new QListView());

    m_lineedit_1 = new QLineEdit(m_widget_query_result);
    m_lineedit_1->move(769 - 250,323 - MAIN_HEAD_HEIGHT);
    m_lineedit_1->setFixedSize(150,64);
    CCommonInterface::set_lineedit_sheet(m_lineedit_1);


    m_lineedit_2 = new QLineEdit(m_widget_query_result);
    m_lineedit_2->move(769 - 250,447 - MAIN_HEAD_HEIGHT);
    m_lineedit_2->setFixedSize(150,64);
    CCommonInterface::set_lineedit_sheet(m_lineedit_2);

    str_sheet = "QPushButton {\
                                      border:0px;\
                                      background:#3ea2ff;\
                                      color:#03111f;\
                                      font:24px;\
                                      } \
                                      QPushButton:pressed {background:#2C71B2;} \
                                      QPushButton:hover:!pressed {background:#2C71B2;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}";

    m_pushbutton_1 = new QPushButton(m_widget_query_result);
    CCommonInterface::init_new_button(m_pushbutton_1,290 - 250,666 - MAIN_HEAD_HEIGHT,98,65,str_sheet);
    m_pushbutton_1->setText(tr("搜索"));

    m_pushbutton_2 = new QPushButton(m_widget_query_result);
    CCommonInterface::init_new_button(m_pushbutton_2,407 - 250,666 - MAIN_HEAD_HEIGHT,98,65,str_sheet);
    m_pushbutton_2->setText(tr("全选"));

    m_pushbutton_3 = new QPushButton(m_widget_query_result);
    CCommonInterface::init_new_button(m_pushbutton_3,525 - 250,666 - MAIN_HEAD_HEIGHT,98,65,str_sheet);
    m_pushbutton_3->setText(tr("回放"));

    if(kEnumSystemManage == m_type)
    {
        m_pushbutton_4 = new QPushButton(m_widget_query_result);
        CCommonInterface::init_new_button(m_pushbutton_4,641 - 250,666 - MAIN_HEAD_HEIGHT,98,65,str_sheet);
        m_pushbutton_4->setText(tr("导出"));
        connect(m_pushbutton_4,SIGNAL(clicked()),this,SLOT(slot_pushbutton_4()));
    }

    bool ret = false;

    connect(m_pushbutton_1,SIGNAL(clicked()),this,SLOT(slot_pushbutton_1()));
    connect(m_pushbutton_2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_2()));
    ret = connect(m_pushbutton_3,SIGNAL(clicked()),this,SLOT(slot_pushbutton_3()));


    int list_width = 450 + 20;
    int list_height = 70 * 7;


    m_query_result_list = new CCommonQueryList(CCommonQueryList::kEnumQueryResult
                                              ,list_width,list_height, m_widget_query_result,true);

    m_query_result_list->setColumnCount(4);
    m_query_result_list->setColumnWidth(0,60,tr("使能"));
    m_query_result_list->setColumnWidth(1,195,tr("录像时间"));
    m_query_result_list->setColumnWidth(2,75,tr("时长(S)"));
    m_query_result_list->setColumnWidth(3,list_width - 60 - 195 - 75,tr("通道"));

    m_query_result_list->setFixedSize(list_width,list_height);
    m_query_result_list->move(290 - 250 - 20,160 - MAIN_HEAD_HEIGHT);
   // m_query_result_list->ShowGrid(true);


    CCommonQueryMode mode_type;

    CQueryResult *pQueryResult = NULL;

    pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-1";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

    pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-2";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

        pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-3";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

        pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-4";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

        pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-5";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

        pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-6";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

        pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-7";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

        pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-8";//通道
    pQueryResult->m_size = 1024 * 1024;
#if 1
    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

            pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-3";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

        pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-4";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

        pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-5";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

        pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-6";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

        pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-7";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);

        pQueryResult = new CQueryResult;
    pQueryResult->m_date = "2016-11-04 13:23:59";//录像时间
    pQueryResult->m_time = 9999;//时长(S)
    pQueryResult->m_passageway = "CAM1-8";//通道
    pQueryResult->m_size = 1024 * 1024;

    mode_type.data = pQueryResult;
    m_query_result_list->AddData(mode_type);
#endif

    m_test_timer = new QTimer(this);
    m_test_timer->setInterval(5000);
    connect(m_test_timer,SIGNAL(timeout()),this,SLOT(slot_timer_test()));

}

void CVideotapeQueryWidget::slot_pushbutton_1()//搜  索
{

    do
    {
        VideotapeQueryList info;

        info.m_year = m_year;
        info.m_month = m_month;
        info.m_day = m_day;

        QString str_start = m_lineedit_1->text();
        QString str_end = m_lineedit_2->text();

        QStringList start_list = str_start.split(":");
        QStringList end_list = str_end.split(":");

        if(3 != start_list.size())
            break;

        if(3 != end_list.size())
            break;

        info.start_hour = start_list.at(0).toInt();
        info.start_minute = start_list.at(1).toInt();
        info.start_second = start_list.at(2).toInt();
        info.end_hour = end_list.at(0).toInt();
        info.end_minute = end_list.at(1).toInt();
        info.end_second = end_list.at(2).toInt();

        info.channel = m_combobox_4->currentIndex();

        CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumGetDayList,&info);

    }while(0);


}
void CVideotapeQueryWidget::slot_pushbutton_2()//全选
{
    if(tr("全选") == m_pushbutton_2->text())
    {
        m_pushbutton_2->setText(tr("全不选"));
        m_query_result_list->ListSelected(true);
    }
    else
    {
        m_pushbutton_2->setText(tr("全选"));
        m_query_result_list->ListSelected(false);
    }

}
void CVideotapeQueryWidget::slot_pushbutton_3()//回放
{


    do
    {
        QList<CCommonQueryMode> mode_list;
        m_query_result_list->GetRowSelect(mode_list);

        if(mode_list.size() <= 0)
        {


            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("请从列表选择视频文件\n后，再回放!"),CLtyString::getInstance()->m_confirm,"");
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
            break;
        }
        CSystemPlayBack play_back;
        for(int index = 0;index < mode_list.size();index++)
        {
            CQueryResult *pCQueryResult = (CQueryResult *)mode_list.at(index).data;

            play_back.m_play_list.push_back(pCQueryResult->m_number);

        }
        play_back.m_operate_type = CSystemPlayBack::kEnumRefreshPlayList; //更新播放列表
        CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumPlayBack,&play_back);
        emit signal_pushbutton_playback();
    }while(0);

}

void CVideotapeQueryWidget::slot_pushbutton_4()//导出
{
    static CVideotapeExport *export_dialog = NULL;

    do
    {
        QList<CCommonQueryMode> mode_list;
        m_query_result_list->GetRowSelect(mode_list);
        CSystemVideoExport video_export;

        if(mode_list.size() <= 0)
        {


            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("请从列表选择视频文件\n后，再导出!"),CLtyString::getInstance()->m_confirm,"");
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
            break;
        }

        if(NULL == export_dialog)
        {
            export_dialog = new CVideotapeExport(CMainPage::getInstance());
            export_dialog->move((1024 - 540)/2,(768 - 488)/2);
        }
        qint64 file_size_total = 0;
        for(int index = 0;index < mode_list.size();index++)
        {
            CQueryResult *pCQueryResult = (CQueryResult *)mode_list.at(index).data;

            file_size_total += pCQueryResult->m_size;
            video_export.m_list.push_back(pCQueryResult->m_number);

        }
        QString str_size = CCommonInterface::convert_string_file_size(file_size_total);
        export_dialog->set_data(mode_list.size(),str_size,false);

        export_dialog->exec();

        if(CVideotapeExport::KEnumButtonOk == export_dialog->get_result())
        {
            //通知开始导出

            video_export.m_operate_type = 1;
            video_export.m_export_type = export_dialog->get_current_type();/*1:原始数据  2:AVI数据*/

            QString str_start = m_lineedit_1->text();
            QString str_end = m_lineedit_2->text();

            QStringList start_list = str_start.split(":");
            QStringList end_list = str_end.split(":");

            if(3 != start_list.size())
                break;

            if(3 != end_list.size())
                break;

            video_export.m_year = m_year;
            video_export.m_month = m_month;
            video_export.m_day = m_day;
            video_export.start_hour = start_list.at(0).toInt();
            video_export.start_minute = start_list.at(1).toInt();
            video_export.start_second = start_list.at(2).toInt();
            video_export.end_hour = end_list.at(0).toInt();
            video_export.end_minute = end_list.at(1).toInt();
            video_export.end_second = end_list.at(2).toInt();
            video_export.channel = m_combobox_4->currentIndex();

            CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumExport,&video_export);


            CProgressBox::getInstance()->data_init();
            CProgressBox::getInstance()->set_title(tr("导出视频文件"));

            CProgressBox::getInstance()->exec();

            if(CProgressBox::KEnumStatusSuccess == CProgressBox::getInstance()->get_status())
            {
                QString str_tip = "";
                //if(2 == m_export_value)
                { /*导出成功*/
                    str_tip = tr("导出成功!");
                }



                CMyMessageBox::getInstance()->set_content(tr("温馨提示"),str_tip,CLtyString::getInstance()->m_confirm,"");
                CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
                CMyMessageBox::getInstance()->exec();
            }
            else
            {//用户取消导出
                //通知取消导出

                if(CProgressBox::KEnumStatusStop == CProgressBox::getInstance()->get_status())
                {
                     //通知取消操作
                    video_export.m_operate_type = 2;

                    CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumExport,&video_export);
                     return;
                }

                QString str_tip = tr("导出失败:") + m_export_result;



                CMyMessageBox::getInstance()->set_content(tr("温馨提示"),str_tip,CLtyString::getInstance()->m_confirm,"");
                CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
                CMyMessageBox::getInstance()->exec();
            }

        }

    }while(0);

}

void CVideotapeQueryWidget::slot_timer_test()
{
    //CMyExport::getInstance()->set_content(tr("导出视频文件"),tr("正在导出..."),"",CLtyString::getInstance()->m_cancel);
}

/*
char m_position; //位置  0:硬盘 1:EMMC 2:SD卡
    QList<int>  m_year_list;  //日期年列表
    QList<int>  m_month_list; //日期月表
*/
#include <QDebug>
void CVideotapeQueryWidget::slot_videotape_query_widget_event(const CSystemVideotapeQueryEx &_info)
{
    switch(_info.m_type)
    {
        case CSystemVideotapeQueryEx::kEnumGetDate:  //获取年月数据
        {
            if(NULL == _info.m_CSystemVideotapeQuery)
            {
                break;
            }
            //m_combobox_1->setCurrentIndex(_info.m_CSystemVideotapeQuery->m_position);

            if(_info.m_CSystemVideotapeQuery->m_list.size() > 0)
            {//日期年列表
                m_combobox_2->clear();
                for(int index = 0;index < _info.m_CSystemVideotapeQuery->m_list.size();index++)
                {
                    QString str_year = QString::number(_info.m_CSystemVideotapeQuery->m_list.at(index).m_year) + tr("年");
                    m_combobox_2->addItem(str_year);

                    if(0 == index)
                    {
                        m_combobox_3->clear();
                        for(int i = 0;i < _info.m_CSystemVideotapeQuery->m_list.at(index).m_month_list.size();i++)
                        {
                            QString str_month = QString::number(_info.m_CSystemVideotapeQuery->m_list.at(index).m_month_list.at(i)) + tr("月");
                            m_combobox_3->addItem(str_month);
                        }
                    }

                }

            }
#if 0
            if(_info.m_CSystemVideotapeQuery->m_month_list.size() > 0)
            {//日期月表
                m_combobox_3->clear();
                for(int index = 0;index < _info.m_CSystemVideotapeQuery->m_month_list.size();index++)
                {
                    QString str_month = QString::number(_info.m_CSystemVideotapeQuery->m_month_list.at(index)) + tr("月");
                    m_combobox_3->addItem(str_month);
                }
            }
#endif

        }break;
        case CSystemVideotapeQueryEx::kEnumGetMonth: //获取月信息
        {
            //qDebug() << "slot_videotape_query_widget_event 1";
            if(NULL == _info.m_VideotapeQueryDate)
            {
                break;
            }
           // qDebug() << "slot_videotape_query_widget_event 2";
            QList<CCommonQueryMode> date_list;
            m_date_select_list->GetRowSelect(date_list,true);

            for(int date_index = 0;date_index < _info.m_VideotapeQueryDate->m_day_list.size();date_index++)
            {
                char day = _info.m_VideotapeQueryDate->m_day_list.at(date_index).m_day;
                char normal = _info.m_VideotapeQueryDate->m_day_list.at(date_index).normal;
                char alarm = _info.m_VideotapeQueryDate->m_day_list.at(date_index).alarm;

                //qDebug() << "slot_videotape_query_widget_event 2-1";

                for(int index = 0;index < date_list.size();index++)
                {
                    CDateSelect *pDateSelect = (CDateSelect *)date_list.at(index).data;
                    for(int i = 0;i < pDateSelect->m_list.size();i++)
                    {

                       // qDebug() << "slot_videotape_query_widget_event 2-2";
                        if(pDateSelect->m_list.at(i).m_current_month && (pDateSelect->m_list.at(i).m_number == day))
                        {
                            if(1 == normal)
                            {
                                //qDebug() << "slot_videotape_query_widget_event 3";
                                pDateSelect->m_list[i].m_normal = true;
                            }
                            if(1 == alarm)
                            {
                                //qDebug() << "slot_videotape_query_widget_event 4";
                                pDateSelect->m_list[i].m_police = true;
                            }

                        }
                    }
                }
            }

            m_date_select_list->list_refresh();


        }break;
        case CSystemVideotapeQueryEx::kEnumGetDayList://获取列表信息
        {
            if(NULL == _info.m_VideotapeQueryList)
            {
                break;
            }

            CCommonQueryMode mode_type;

            CQueryResult *pQueryResult = (CQueryResult *)_info.m_VideotapeQueryList->param;

            if(1 == pQueryResult->m_number)
            {
                m_query_result_list->slotListClean();
            }

            mode_type.data = pQueryResult;
            m_query_result_list->AddData(mode_type);

            if(pQueryResult->data_end)
                m_query_result_list->list_sort();


        }break;
        case CSystemVideotapeQueryEx::kEnumExport://导出
        {
            if(NULL == _info.m_CSystemVideoExport)
            {
                break;
            }

            if(1 == _info.m_CSystemVideoExport->m_type)
            {
                CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNormal,100);

            }
            else if(2 == _info.m_CSystemVideoExport->m_type)
            {
                m_export_result = _info.m_CSystemVideoExport->result;
                CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessFail,0);
            }
            else if(3 == _info.m_CSystemVideoExport->m_type)
            {
                CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNormal,_info.m_CSystemVideoExport->m_value);
            }
#if 0
            m_export_value = _info.m_CSystemVideoExport->value;
            m_export_result = _info.m_CSystemVideoExport->result;
            CMyExport::getInstance()->nofity_close();
#endif
        }break;
        case CSystemVideotapeQueryEx::kEnumPlayBack:
        {


            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),_info.m_result,CLtyString::getInstance()->m_confirm,"");
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
        }break;

    }

}


void CVideotapeQueryWidget::get_month_info(bool _year_change,int &_month)
{
    VideotapeQueryDate info;

    info.m_position = m_combobox_1->currentIndex(); //位置  0:硬盘 1:EMMC 2:SD卡

    QString str_year =  m_combobox_2->currentText();

    str_year = str_year.replace(tr("年"),"");
    info.m_year = str_year.toInt();   //年

    if(_year_change)
    {
        for(int index = 0;index < CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_CSystemVideotapeQuery->m_list.size();index++)
        {
            if(info.m_year == CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_CSystemVideotapeQuery->m_list.at(index).m_year)
            {
                m_combobox_3->clear();
                for(int i = 0;i < CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_CSystemVideotapeQuery->m_list.at(index).m_month_list.size();i++)
                {
                    QString str_month = QString::number(CCommonData::getInstance()->m_CSystemVideotapeQueryEx.m_CSystemVideotapeQuery->m_list.at(index).m_month_list.at(i)) + tr("月");
                    m_combobox_3->addItem(str_month);
                }
                break;
            }
        }
    }

    QString str_month = m_combobox_3->currentText();
    str_month = str_month.replace(tr("月"),"");


    info.m_month = str_month.toInt(); //月
    _month = info.m_month;

    if(0 != info.m_year && 0 != info.m_month)
    CProtocol::getInstance()->call_json_system_videotape_query_set(CSystemVideotapeQueryEx::kEnumGetMonth,&info);
}

void CVideotapeQueryWidget::clean_date_list()
{
    m_date_select_list->slotListClean();
}
















