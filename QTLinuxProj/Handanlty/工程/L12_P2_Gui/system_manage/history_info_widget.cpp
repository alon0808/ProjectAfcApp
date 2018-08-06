#include "history_info_widget.h"

#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"

#include "./class/common_interface.h"

#include <QTimer>

CHistoryInfoWidget::CHistoryInfoWidget(QWidget *parent) :
    QWidget(parent)
{
        this->setObjectName("CHistoryInfoWidget");
    this->setStyleSheet("QWidget#CHistoryInfoWidget{background-color:#ffffff;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT - 24);
    this->move(0,0);

    m_pMainLayout = new QVBoxLayout();//左 上 右 下
    m_pMainLayout->setContentsMargins(20,24,20,24);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setAlignment(Qt::AlignLeft/* | Qt::AlignTop*/);
    m_pMainLayout->addStretch(0);
    this->setLayout(m_pMainLayout);

    m_scrollarea_1=new QScrollArea(this);  //新建了一条滚动条
    m_scrollarea_1->setFixedHeight(NAVIGATE_RIGHT_WIDGET_HEIGHT - 24 - 24);

    m_scrollarea_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //m_scrollarea_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pMainLayout->addWidget(m_scrollarea_1);

    QWidget * pwidget=new QWidget(this);

    pwidget->setAttribute(Qt::WA_StyledBackground);
    pwidget->setObjectName("pwidget");
    pwidget->setStyleSheet("QWidget#pwidget { background: transparent;}");

    m_vboxlayout_widget=new QVBoxLayout(pwidget);
    m_vboxlayout_widget->setContentsMargins(0,0,0,0);
    m_vboxlayout_widget->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    m_vboxlayout_widget->setSpacing(0);

    m_widget_main = new QWidget;
    m_widget_main->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH - 20 - 20,NAVIGATE_RIGHT_WIDGET_HEIGHT + 600);

    m_widget_main->setStyleSheet("QWidget{background-color:#ffffff;}");
    m_vboxlayout_widget->addWidget(m_widget_main);

    pwidget->setLayout(m_vboxlayout_widget);
    m_scrollarea_1->setWidget(pwidget);
    m_scrollarea_1->setWidgetResizable(true);



#if 0
    m_label_test = new QLabel(m_widget_main);
    m_label_test->setFixedSize(100,30);
    m_label_test->setText(tr("历史信息"));
#endif
#if 1
    history_info_init();

    m_pushbutton_save = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_save,(m_widget_main->width() - 180 * 2 - 20)/2,ROW_HEIGHT * 11 + 40,180,64,LTY_PUSHBUTTON_STYLE_SAVE);
    m_pushbutton_save->setText(tr("保存"));

    m_pushbutton_reset = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_reset,(m_widget_main->width() - 180 * 2 - 20)/2 + 20 + 180,ROW_HEIGHT * 11 + 40,180,64,LTY_PUSHBUTTON_STYLE_RESET);
    m_pushbutton_reset->setText(tr("重置"));

    connect(m_pushbutton_save,SIGNAL(clicked()),this,SLOT(slot_pushbutton_save()));
    connect(m_pushbutton_reset,SIGNAL(clicked()),this,SLOT(slot_pushbutton_reset()));
#endif

    m_widget_main->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH - 20 - 20,NAVIGATE_RIGHT_WIDGET_HEIGHT + 600);

}


void CHistoryInfoWidget::slot_pushbutton_save()
{
}
void CHistoryInfoWidget::slot_pushbutton_reset()
{
}

void CHistoryInfoWidget::history_info_init()
{
    for(int index = 0;index < 12;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(m_widget_main);
        pWidget->setFixedSize(764,2);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#dddddd;}");
    }

    int str_length = 0;
    int iStart_pos = 0;
    int alignRight = m_widget_main->width() - 77 - 60;
    QString str_sheet = "QLabel{border:1px solid#dddddd;background:transparent;color:#666666;}";

    QString str_show = tr("总行驶里程");
    m_label_line1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    /*
        QLabel *m_label_line1_1;
    QPushButton *m_pushbutton_reset_line1_1;
    QLabel *m_label_line1_2;
    QLabel *m_label_line1_3;
    QPushButton *m_pushbutton_reset_line1_2;
    */
    str_show = tr("2378");
    m_label_line1_1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line1_1,30 + str_length + 10, ROW_HEIGHT * 0 + (ROW_HEIGHT - 38)/2,110,38,str_show,str_sheet);

    iStart_pos = m_label_line1_1->pos().x();

    str_show = tr("m");
    m_label_line1_2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line1_2,iStart_pos + 110 + 10, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);


    m_pushbutton_reset_line1_1 = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_reset_line1_1,alignRight,ROW_HEIGHT * 0 + (ROW_HEIGHT - 38)/2,77,38,LTY_PUSHBUTTON_STYLE_HISTORY);
    m_pushbutton_reset_line1_1->setText(tr("重置"));


    str_show = tr("历史最高速度");
    m_label_line2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    str_show = tr("120");
    m_label_line2_1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line2_1,30 + str_length + 10, ROW_HEIGHT * 1 + (ROW_HEIGHT - 38)/2,110,38,str_show,str_sheet);

    iStart_pos = m_label_line2_1->pos().x();
    str_show = tr("km/h");
    m_label_line2_2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line2_2,iStart_pos + 110 + 10, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);



    m_pushbutton_reset_line2 = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_reset_line2,alignRight,ROW_HEIGHT * 1 + (ROW_HEIGHT - 38)/2,77,38,LTY_PUSHBUTTON_STYLE_HISTORY);
    m_pushbutton_reset_line2->setText(tr("重置"));

    str_show = tr("本次开机时长");
    m_label_line3 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    str_show = tr("120");
    m_label_line3_1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line3_1,30 + str_length + 10, ROW_HEIGHT * 2 + (ROW_HEIGHT - 38)/2,110,38,str_show,str_sheet);

    iStart_pos = m_label_line3_1->pos().x();
    str_show = tr("分");
    m_label_line3_2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line3_2,iStart_pos + 110 + 10, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);


    str_show = tr("历史最低电压");
    m_label_line4 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);
#if 0
    str_show = tr("7V");
    m_label_line4_1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line4_1,30 + str_length + 10, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);
#endif
    iStart_pos = m_label_line4->pos().x();

    str_show = tr("7V 2016-7-22 12:33:23");
    m_label_line4_2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line4_2,alignRight - 35 - 396, ROW_HEIGHT * 3 + (ROW_HEIGHT - 38)/2,396,38,str_show,str_sheet);

    iStart_pos = m_label_line4_2->pos().x();
    m_pushbutton_reset_line4 = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_reset_line4,alignRight,ROW_HEIGHT * 3 + (ROW_HEIGHT - 38)/2,77,38,LTY_PUSHBUTTON_STYLE_HISTORY);
    m_pushbutton_reset_line4->setText(tr("重置"));

    str_show = tr("历史最高电压");
    m_label_line5= new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);
#if 0
    str_show = tr("40V");
    m_label_line5_1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line5_1,30 + str_length + 10, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);
#endif
    iStart_pos = m_label_line5->pos().x();

    str_show = tr("40V 2016-7-22 12:33:55");
    m_label_line5_2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line5_2,alignRight - 35 - 396, ROW_HEIGHT * 4 + (ROW_HEIGHT - 38)/2,396,38,str_show,str_sheet);

    iStart_pos = m_label_line5_2->pos().x();
    m_pushbutton_reset_line5 = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_reset_line5,alignRight,ROW_HEIGHT * 4 + (ROW_HEIGHT - 38)/2,77,38,LTY_PUSHBUTTON_STYLE_HISTORY);
    m_pushbutton_reset_line5->setText(tr("重置"));

    str_show = tr("历史机内最低温度");
    m_label_line6 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);
#if 0
    str_show = tr("-20℃");
    m_label_line6_1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line6_1,30 + str_length + 10, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);
#endif
    iStart_pos = m_label_line6->pos().x();

    str_show = tr("-20℃ 2016-7-22 12:33:23");
    m_label_line6_2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line6_2,alignRight - 35 - 350, ROW_HEIGHT * 5 + (ROW_HEIGHT - 38)/2,350,38,str_show,str_sheet);

    iStart_pos = m_label_line6_2->pos().x();
    m_pushbutton_reset_line6 = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_reset_line6,alignRight,ROW_HEIGHT * 5 + (ROW_HEIGHT - 38)/2,77,38,LTY_PUSHBUTTON_STYLE_HISTORY);
    m_pushbutton_reset_line6->setText(tr("重置"));

    str_show = tr("历史机内最高温度");
    m_label_line7 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line7,30, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);
#if 0
    str_show = tr("70℃");
    m_label_line7_1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line7_1,30 + str_length + 10, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);
#endif
    iStart_pos = m_label_line7->pos().x();

    str_show = tr("70℃ 2016-7-22 12:33:55");
    m_label_line7_2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line7_2,alignRight - 35 - 350, ROW_HEIGHT * 6 + (ROW_HEIGHT - 38)/2,350,38,str_show,str_sheet);

    iStart_pos = m_label_line7_2->pos().x();
    m_pushbutton_reset_line7 = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_reset_line7,alignRight,ROW_HEIGHT * 6 + (ROW_HEIGHT - 38)/2,77,38,LTY_PUSHBUTTON_STYLE_HISTORY);
    m_pushbutton_reset_line7->setText(tr("重置"));

    str_show = tr("历史最高加速度");
    m_label_line8 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line8,30, ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);
#if 0
    str_show = tr("80 Km");
    m_label_line8_1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line8_1,30 + str_length + 10, ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);
#endif

#if 0
    iStart_pos = m_label_line8->pos().x();
    str_show = tr("80 Km 2016-7-22 12:33:55");
    m_label_line8_2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line8_2,iStart_pos + str_length + 10, ROW_HEIGHT * 7 + (ROW_HEIGHT - 38)/2,0,38,str_show,str_sheet);
#endif

    m_pushbutton_reset_line8 = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_reset_line8,alignRight,ROW_HEIGHT * 7 + (ROW_HEIGHT - 38)/2,77,38,LTY_PUSHBUTTON_STYLE_HISTORY);
    m_pushbutton_reset_line8->setText(tr("重置"));


    str_show = tr("X方向:");
    m_label_line9 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line9,150, ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);

    str_show = tr("(+)00.000");
    m_label_line9_1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line9_1,150 + str_length + 10, ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);

    iStart_pos = m_label_line9_1->pos().x();

    str_show = tr("2016-7-22 12:33:55");
    m_label_line9_2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line9_2,iStart_pos + str_length + 10, ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);


    str_show = tr("Y方向:");
    m_label_line10 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line10,150, ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show);

    str_show = tr("(+)00.000");
    m_label_line10_1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line10_1,150 + str_length + 10, ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show);

    iStart_pos = m_label_line10_1->pos().x();

    str_show = tr("2016-7-22 12:33:55");
    m_label_line10_2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line10_2,iStart_pos + str_length + 10, ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show);


    str_show = tr("Z方向:");
    m_label_line11 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line11,150, ROW_HEIGHT * 10,0,ROW_HEIGHT,str_show);

    str_show = tr("(+)00.000");
    m_label_line11_1 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line11_1,150 + str_length + 10, ROW_HEIGHT * 10,0,ROW_HEIGHT,str_show);

    iStart_pos = m_label_line10_1->pos().x();

    str_show = tr("2016-7-22 12:33:55");
    m_label_line11_2 = new QLabel(m_widget_main);
    str_length = CCommonInterface::init_label_text(m_label_line11_2,iStart_pos + str_length + 10, ROW_HEIGHT * 10,0,ROW_HEIGHT,str_show);


    connect(m_pushbutton_reset_line1_1,SIGNAL(clicked()),this,SLOT(slot_pushbutton_reset_line1_1()));
    connect(m_pushbutton_reset_line2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_reset_line2()));
    connect(m_pushbutton_reset_line4,SIGNAL(clicked()),this,SLOT(slot_pushbutton_reset_line4()));
    connect(m_pushbutton_reset_line5,SIGNAL(clicked()),this,SLOT(slot_pushbutton_reset_line5()));
    connect(m_pushbutton_reset_line6,SIGNAL(clicked()),this,SLOT(slot_pushbutton_reset_line6()));
    connect(m_pushbutton_reset_line7,SIGNAL(clicked()),this,SLOT(slot_pushbutton_reset_line7()));
    connect(m_pushbutton_reset_line8,SIGNAL(clicked()),this,SLOT(slot_pushbutton_reset_line8()));

}


void CHistoryInfoWidget::slot_pushbutton_reset_line1_1()
{

}
void CHistoryInfoWidget::slot_pushbutton_reset_line2()
{

}
void CHistoryInfoWidget::slot_pushbutton_reset_line4()
{

}
void CHistoryInfoWidget::slot_pushbutton_reset_line5()
{

}
void CHistoryInfoWidget::slot_pushbutton_reset_line6()
{

}
void CHistoryInfoWidget::slot_pushbutton_reset_line7()
{

}

void CHistoryInfoWidget::slot_pushbutton_reset_line8()
{

}












