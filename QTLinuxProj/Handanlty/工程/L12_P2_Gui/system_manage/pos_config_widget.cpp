

#include "pos_config_widget.h"
#include "LtyCommonDefine.h"
#include "common_data.h"
#include "class/common_interface.h"
#include "LtyCommonStyle.h"

#include <QTimer>
#include <QListView>
#include <QDebug>

CPosConfigWidget::CPosConfigWidget(QWidget *parent) :
    QWidget(parent)
{
    m_define_screen = new CMyDefineScreen();

   this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH ,NAVIGATE_RIGHT_WIDGET_HEIGHT);
   this->setObjectName("CPosConfigWidget");
   this->setStyleSheet("QWidget#CPosConfigWidget{background-color:#ffffff;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    scroll_layout();
    widget_init();
    QTimer::singleShot(10,this,SLOT(slot_pos_config_widget_init()));
    read_data();


    m_define_screen->add_parent(this);
    m_define_screen->add_parent(m_widget_main);
    //m_define_screen->add_parent(m_widget_main1);

}

void CPosConfigWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}


void CPosConfigWidget::slot_pos_config_widget_init()
{
    m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_x(m_scroll_widget_height + 200));
    //m_widget_main1->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_x(m_scroll_widget_height + 400));
}

void CPosConfigWidget::slot_pos_config_widget_init1()
{

    m_widget_main1->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_x(m_scroll_widget_height + 400));
}


void CPosConfigWidget::scroll_layout()
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
    m_scrollarea_1->setFixedHeight(m_define_screen->get_change_factor_y(m_scroll_widget_height));
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

    m_support_card_widget = new CSuportCardWidget(m_widget_main1);
    //m_support_card_widget->move(0,0);

    m_widget_main1->hide();

}

void CPosConfigWidget::widget_init()
{
    int row_number = 8;
    for(int index = 0;index < (row_number + 1);index++)
    {

        QWidget *pWidget = new QWidget(m_widget_main);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#1B4279;}");
    }
    QWidget *pWidgetLine1 = new QWidget(m_widget_main);
    QWidget *pWidgetLine2 = new QWidget(m_widget_main);

    pWidgetLine1->setFixedSize(1,ROW_HEIGHT * row_number);
    pWidgetLine1->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50);
    pWidgetLine1->setStyleSheet("QWidget{background-color:#1B4279;}");

    pWidgetLine2->setFixedSize(1,ROW_HEIGHT * row_number);
    pWidgetLine2->move(290 - 250 + 620,158 - MAIN_HEAD_HEIGHT - 50);
    pWidgetLine2->setStyleSheet("QWidget{background-color:#1B4279;}");

    int startX = 320 - 250;
    int startY = 158 - MAIN_HEAD_HEIGHT - 50;

    QString str_show = tr("终端模式");
    m_label_line1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line1,startX, startY + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line1 = new QComboBox(m_widget_main);
    m_combobox_line1->move(650 - 250,startY + ROW_HEIGHT * 0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line1->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT - 10);//CMyDefineScreen::getInstance()->get_change_factor_x(
    m_combobox_line1->addItem(tr("一体机"));
    m_combobox_line1->addItem(tr("主机"));
    m_combobox_line1->addItem(tr("从机"));
    m_combobox_line1->setView(new QListView());
    connect(m_combobox_line1, SIGNAL(currentIndexChanged(int)), SLOT(slot_currentIndex_changed(int)));
    str_show = tr("收费模式");
    m_label_line2 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line2,startX, startY + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line2 = new QComboBox(m_widget_main);
    m_combobox_line2->move(650 - 250,startY + ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line2->addItem(tr("一票制"));
    m_combobox_line2->addItem(tr("分段收费"));
    m_combobox_line2->setView(new QListView());
    if(NULL != m_combobox_line1)
    {
        slot_currentIndex_changed(m_combobox_line1->currentIndex());
    }

    str_show = tr("PSAM卡数量");
    m_label_line3 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line3,startX, startY + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line3 = new QComboBox(m_widget_main);
    m_combobox_line3->move(650 - 250,startY + ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("1"));
    m_combobox_line3->addItem(tr("2"));
    m_combobox_line3->addItem(tr("3"));
    m_combobox_line3->addItem(tr("4"));
    m_combobox_line3->setView(new QListView());

    str_show = tr("支持卡类");
    m_label_line4 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line4,startX, startY + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    QString str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_line4 = new QPushButton(m_widget_main);
    CCommonInterface::init_new_button(m_pushbutton_line4,650 - 250 + 150,startY + ROW_HEIGHT * 3 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_line4->setText(tr("配置"));
    connect(m_pushbutton_line4,SIGNAL(clicked()),this,SLOT(slot_pushbutton_config()));

    str_show = tr("卡扇区");
    m_label_line5 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line5,startX, startY + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_combobox_line5 = new QComboBox(m_widget_main);
    m_combobox_line5->move(650 - 250,startY + ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);

    for(int index = 1;index <= 16;index++)
        m_combobox_line5->addItem(QString::number(index));
    m_combobox_line5->setView(new QListView());

    str_show = tr("刷卡间隔时长");
    m_label_line6 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6,startX, startY + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_lineedit6 = new QLineEdit(m_widget_main);
    m_lineedit6->move(650 - 250,startY + ROW_HEIGHT * 5 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit6->setFixedSize(EDIT_WIDTH - 50,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit6);

    str_show = tr("Min");
    m_label_line6_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line6_1,650 - 250 + (EDIT_WIDTH - 50) + 10, startY + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("逃票判断时长");
    m_label_line7 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7,startX, startY + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_lineedit7 = new QLineEdit(m_widget_main);
    m_lineedit7->move(650 - 250,startY + ROW_HEIGHT * 6 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit7->setFixedSize(EDIT_WIDTH - 50,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit7);

    str_show = tr("Min");
    m_label_line7_1 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line7_1,650 - 250 + (EDIT_WIDTH - 50) + 10, startY + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    str_show = tr("主机IP");
    m_label_line8 = new QLabel(m_widget_main);
    CCommonInterface::init_label_text(m_label_line8,startX, startY + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show,"",Qt::AlignCenter,23);

    m_lineedit8 = new QLineEdit(m_widget_main);
    m_lineedit8->move(650 - 250,startY + ROW_HEIGHT * 7 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit8->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit8);

}

void CPosConfigWidget::slot_currentIndex_changed(int index)
{
    qDebug()<<"save_data : m_combobox_line1->currentIndex() m_combobox_line1->currentIndex(): index"<<index;
    // 一体机模式
    if (0 == index)
    {
       m_combobox_line2->setCurrentIndex(0);
       m_combobox_line2->setEnabled(false);
    }
    // 主机和从机模式
    else if(1 == index || 2 == index)
    {
        m_combobox_line2->setCurrentIndex(1);
        m_combobox_line2->setEnabled(false);
    }
    else
    {
        m_combobox_line2->setEnabled(true);
    }
}

void CPosConfigWidget::slot_pushbutton_config()
{
    m_widget_main->hide();
    m_widget_main1->show();

    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_main1);
    static bool first_init = true;
    if(first_init)
    {
        first_init = false;
        QTimer::singleShot(10,this,SLOT(slot_pos_config_widget_init1()));
    }


}

CSuportCardWidget::CSuportCardWidget(QWidget *parent) :
    QWidget(parent)
{
    QWidget *pRectWidget1 = new QWidget(this);
    pRectWidget1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget1->setFixedSize(620,77);
    pRectWidget1->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    QWidget *pWidgetRect2 = new QWidget(this);

    pWidgetRect2->setFixedSize(620,430 + 300);
    pWidgetRect2->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + 77);
    pWidgetRect2->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    QString str_show = tr("卡类选择");
    m_label_line1 = new QLabel(this);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,300,77,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,33);


    //1~4
    str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:22px;color:#8ea2c0;}";
    m_checkbox_line2_1 = new QCheckBox(this);
    m_checkbox_line2_1->setFixedSize(70,70);
    m_checkbox_line2_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("0");
    m_label_line2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line2_2 = new QCheckBox(this);
    m_checkbox_line2_2->setFixedSize(70,70);
    m_checkbox_line2_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("1");
    m_label_line2_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line2_3 = new QCheckBox(this);
    m_checkbox_line2_3->setFixedSize(70,70);
    m_checkbox_line2_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("2");
    m_label_line2_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line2_4 = new QCheckBox(this);
    m_checkbox_line2_4->setFixedSize(70,70);
    m_checkbox_line2_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("3");
    m_label_line2_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    //5~8
    m_checkbox_line3_1 = new QCheckBox(this);
    m_checkbox_line3_1->setFixedSize(70,70);
    m_checkbox_line3_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("4");
    m_label_line3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line3_2 = new QCheckBox(this);
    m_checkbox_line3_2->setFixedSize(70,70);
    m_checkbox_line3_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("5");
    m_label_line3_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line3_3 = new QCheckBox(this);
    m_checkbox_line3_3->setFixedSize(70,70);
    m_checkbox_line3_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("6");
    m_label_line3_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line3_4 = new QCheckBox(this);
    m_checkbox_line3_4->setFixedSize(70,70);
    m_checkbox_line3_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("7");
    m_label_line3_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    //9~12  (pon1 ~ pon10)
    m_checkbox_line4_1 = new QCheckBox(this);
    m_checkbox_line4_1->setFixedSize(70,70);
    m_checkbox_line4_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("8");
    m_label_line4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line4_2 = new QCheckBox(this);
    m_checkbox_line4_2->setFixedSize(70,70);
    m_checkbox_line4_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("9");
    m_label_line4_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line4_3 = new QCheckBox(this);
    m_checkbox_line4_3->setFixedSize(70,70);
    m_checkbox_line4_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("10");
    m_label_line4_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line4_4 = new QCheckBox(this);
    m_checkbox_line4_4->setFixedSize(70,70);
    m_checkbox_line4_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("11");
    m_label_line4_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    //13~16
    m_checkbox_line5_1 = new QCheckBox(this);
    m_checkbox_line5_1->setFixedSize(70,70);
    m_checkbox_line5_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("12");
    m_label_line5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line5_2 = new QCheckBox(this);
    m_checkbox_line5_2->setFixedSize(70,70);
    m_checkbox_line5_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("13");
    m_label_line5_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line5_3 = new QCheckBox(this);
    m_checkbox_line5_3->setFixedSize(70,70);
    m_checkbox_line5_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("14");
    m_label_line5_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line5_4 = new QCheckBox(this);
    m_checkbox_line5_4->setFixedSize(70,70);
    m_checkbox_line5_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("15");
    m_label_line5_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);
#if 0
    m_checkbox_list.push_back(m_checkbox_line6_1); //6
    m_checkbox_list.push_back(m_checkbox_line6_2);
    m_checkbox_list.push_back(m_checkbox_line6_3);
    m_checkbox_list.push_back(m_checkbox_line6_4);
    m_checkbox_list.push_back(m_checkbox_line7_1); //7
    m_checkbox_list.push_back(m_checkbox_line7_2);
    m_checkbox_list.push_back(m_checkbox_line7_3);
    m_checkbox_list.push_back(m_checkbox_line7_4);
    m_checkbox_list.push_back(m_checkbox_line8_1); //8
    m_checkbox_list.push_back(m_checkbox_line8_2);
    m_checkbox_list.push_back(m_checkbox_line8_3);
    m_checkbox_list.push_back(m_checkbox_line8_4);
    m_checkbox_list.push_back(m_checkbox_line9_1); //9
    m_checkbox_list.push_back(m_checkbox_line9_2);
    m_checkbox_list.push_back(m_checkbox_line9_3);
    m_checkbox_list.push_back(m_checkbox_line9_4);

    m_label_list.push_back(m_label_line6_1);  //6
    m_label_list.push_back(m_label_line6_2);
    m_label_list.push_back(m_label_line6_3);
    m_label_list.push_back(m_label_line6_4);
    m_label_list.push_back(m_label_line7_1);  //7
    m_label_list.push_back(m_label_line7_2);
    m_label_list.push_back(m_label_line7_3);
    m_label_list.push_back(m_label_line7_4);
    m_label_list.push_back(m_label_line8_1);  //8
    m_label_list.push_back(m_label_line8_2);
    m_label_list.push_back(m_label_line8_3);
    m_label_list.push_back(m_label_line8_4);
    m_label_list.push_back(m_label_line9_1);  //9
    m_label_list.push_back(m_label_line9_2);
    m_label_list.push_back(m_label_line9_3);
    m_label_list.push_back(m_label_line9_4);

    char show_label_number = 16;
    char line_number = 5;
    for(int index = 0;index < m_checkbox_list.size()/4;index++)
    {
        QCheckBox *checkbox = m_checkbox_list[index];
        QCheckBox **pcheckbox = &checkbox;
        (*pcheckbox) = new QCheckBox(this);
        (*pcheckbox)->setFixedSize(70,70);
        (*pcheckbox)->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        (*pcheckbox)->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        (*pcheckbox)->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_list[index] = new QLabel(this);
        CCommonInterface::init_label_text(m_label_list[index],382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_list[index + 1] = new QCheckBox(this);
        m_checkbox_list[index + 1]->setFixedSize(70,70);
        m_checkbox_list[index + 1]->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_list[index + 1]->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_list[index + 1]->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_list[index + 1] = new QLabel(this);
        CCommonInterface::init_label_text(m_label_list[index + 1],537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_list[index + 2] = new QCheckBox(this);
        m_checkbox_list[index + 2]->setFixedSize(70,70);
        m_checkbox_list[index + 2]->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_list[index + 2]->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_list[index + 2]->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_list[index + 2] = new QLabel(this);
        CCommonInterface::init_label_text(m_label_list[index + 2],691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_list[index + 3] = new QCheckBox(this);
        m_checkbox_list[index + 3]->setFixedSize(70,70);
        m_checkbox_list[index + 3]->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_list[index + 3]->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_list[index + 3]->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_list[index + 3] = new QLabel(this);
        CCommonInterface::init_label_text(m_label_list[index + 3],845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    }
#endif
        char show_label_number = 15;
        char line_number = 5;
        int index = 0;
        m_checkbox_line6_1 = new QCheckBox(this);
        m_checkbox_line6_1->setFixedSize(70,70);
        m_checkbox_line6_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line6_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line6_1->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line6_1 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line6_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line6_2 = new QCheckBox(this);
        m_checkbox_line6_2->setFixedSize(70,70);
        m_checkbox_line6_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line6_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line6_2->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line6_2 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line6_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line6_3 = new QCheckBox(this);
        m_checkbox_line6_3->setFixedSize(70,70);
        m_checkbox_line6_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line6_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line6_3->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line6_3 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line6_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line6_4 = new QCheckBox(this);
        m_checkbox_line6_4->setFixedSize(70,70);
        m_checkbox_line6_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line6_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line6_4->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line6_4 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line6_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        index++;
        m_checkbox_line7_1 = new QCheckBox(this);
        m_checkbox_line7_1->setFixedSize(70,70);
        m_checkbox_line7_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line7_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line7_1->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line7_1 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line7_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line7_2 = new QCheckBox(this);
        m_checkbox_line7_2->setFixedSize(70,70);
        m_checkbox_line7_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line7_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line7_2->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line7_2 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line7_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line7_3 = new QCheckBox(this);
        m_checkbox_line7_3->setFixedSize(70,70);
        m_checkbox_line7_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line7_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line7_3->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line7_3 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line7_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line7_4 = new QCheckBox(this);
        m_checkbox_line7_4->setFixedSize(70,70);
        m_checkbox_line7_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line7_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line7_4->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line7_4 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line7_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        index++;
        m_checkbox_line8_1 = new QCheckBox(this);
        m_checkbox_line8_1->setFixedSize(70,70);
        m_checkbox_line8_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line8_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line8_1->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line8_1 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line8_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line8_2 = new QCheckBox(this);
        m_checkbox_line8_2->setFixedSize(70,70);
        m_checkbox_line8_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line8_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line8_2->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line8_2 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line8_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line8_3 = new QCheckBox(this);
        m_checkbox_line8_3->setFixedSize(70,70);
        m_checkbox_line8_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line8_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line8_3->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line8_3 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line8_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line8_4 = new QCheckBox(this);
        m_checkbox_line8_4->setFixedSize(70,70);
        m_checkbox_line8_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line8_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line8_4->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line8_4 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line8_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        index++;
        m_checkbox_line9_1 = new QCheckBox(this);
        m_checkbox_line9_1->setFixedSize(70,70);
        m_checkbox_line9_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line9_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line9_1->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line9_1 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line9_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line9_2 = new QCheckBox(this);
        m_checkbox_line9_2->setFixedSize(70,70);
        m_checkbox_line9_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line9_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line9_2->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line9_2 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line9_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line9_3 = new QCheckBox(this);
        m_checkbox_line9_3->setFixedSize(70,70);
        m_checkbox_line9_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line9_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line9_3->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line9_3 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line9_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_checkbox_line9_4 = new QCheckBox(this);
        m_checkbox_line9_4->setFixedSize(70,70);
        m_checkbox_line9_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index) + (ROW_HEIGHT - 70)/2);
        m_checkbox_line9_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
        m_checkbox_line9_4->setFocusPolicy(Qt::NoFocus);
        str_show = QString::number(++show_label_number);
        m_label_line9_4 = new QLabel(this);
        CCommonInterface::init_label_text(m_label_line9_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * (line_number + index),0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

        m_define_screen = new CMyDefineScreen();
        m_define_screen->add_parent(this,false);

}

void CSuportCardWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CPosConfigWidget::slot_pushbutton_return()
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
        save_data();
        emit signal_window_close();

        #if 0
        save_data();
        CCommonInterface::input_panel_control(false);
        emit signal_pushbutton_menu();
        #endif

    }

}

void CPosConfigWidget::save_data()
{
    CSystemPosSetup info;
    qDebug()<<"save_data : m_combobox_line1->currentIndex() m_combobox_line1->currentIndex():"<<m_combobox_line1->currentIndex();
    qDebug()<<"save_data : m_combobox_line1->currentIndex() m_combobox_line2->currentIndex():"<<m_combobox_line2->currentIndex();
    info.DEVMODE = m_combobox_line1->currentIndex() + 1;   //终端模式
    info.CHARGMODE = m_combobox_line2->currentIndex()+1; //收费模式
    info.PSAMNUM = m_combobox_line3->currentIndex()+1;   //PSAM个数
    //支持卡类型

    info.SUPCARD_TYPE[0] = (m_support_card_widget->m_checkbox_line2_1->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[1] = (m_support_card_widget->m_checkbox_line2_2->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[2] = (m_support_card_widget->m_checkbox_line2_3->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[3] = (m_support_card_widget->m_checkbox_line2_4->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[4] = (m_support_card_widget->m_checkbox_line3_1->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[5] = (m_support_card_widget->m_checkbox_line3_2->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[6] = (m_support_card_widget->m_checkbox_line3_3->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[7] = (m_support_card_widget->m_checkbox_line3_4->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[8] = (m_support_card_widget->m_checkbox_line4_1->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[9] = (m_support_card_widget->m_checkbox_line4_2->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[10] = (m_support_card_widget->m_checkbox_line4_3->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[11] = (m_support_card_widget->m_checkbox_line4_4->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[12] = (m_support_card_widget->m_checkbox_line5_1->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[13] = (m_support_card_widget->m_checkbox_line5_2->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[14] = (m_support_card_widget->m_checkbox_line5_3->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[15] = (m_support_card_widget->m_checkbox_line5_4->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[16] = (m_support_card_widget->m_checkbox_line6_1->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[17] = (m_support_card_widget->m_checkbox_line6_2->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[18] = (m_support_card_widget->m_checkbox_line6_3->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[19] = (m_support_card_widget->m_checkbox_line6_4->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[20] = (m_support_card_widget->m_checkbox_line7_1->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[21] = (m_support_card_widget->m_checkbox_line7_2->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[22] = (m_support_card_widget->m_checkbox_line7_3->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[23] = (m_support_card_widget->m_checkbox_line7_4->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[24] = (m_support_card_widget->m_checkbox_line8_1->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[25] = (m_support_card_widget->m_checkbox_line8_2->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[26] = (m_support_card_widget->m_checkbox_line8_3->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[27] = (m_support_card_widget->m_checkbox_line8_4->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[28] = (m_support_card_widget->m_checkbox_line9_1->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[29] = (m_support_card_widget->m_checkbox_line9_2->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[30] = (m_support_card_widget->m_checkbox_line9_3->checkState() == Qt::Checked)? 1:0;

    info.SUPCARD_TYPE[31] = (m_support_card_widget->m_checkbox_line9_4->checkState() == Qt::Checked)? 1:0;

    info.SECTOR = m_combobox_line5->currentIndex() + 1;     //支持扇区
    info.TIMEINTER = m_lineedit6->text().toInt();   //刷卡时间间隔
    info.TICKETTIME = m_lineedit7->text().toInt();  //逃票最大时间
    info.m_host_ip = m_lineedit8->text();
    if(info.DEVMODE != CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE ||
    info.CHARGMODE != CCommonData::getInstance()->m_CSystemPosSetup.CHARGMODE ||
    info.PSAMNUM != CCommonData::getInstance()->m_CSystemPosSetup.PSAMNUM ||
    info.SUPCARD_TYPE[0] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[0] ||
    info.SUPCARD_TYPE[1] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[1] ||
    info.SUPCARD_TYPE[2] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[2] ||
    info.SUPCARD_TYPE[3] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[3] ||
    info.SUPCARD_TYPE[4] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[4] ||
    info.SUPCARD_TYPE[5] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[5] ||
    info.SUPCARD_TYPE[6] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[6] ||
    info.SUPCARD_TYPE[7] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[7] ||
    info.SUPCARD_TYPE[8] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[8] ||
    info.SUPCARD_TYPE[9] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[9] ||
    info.SUPCARD_TYPE[10] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[10] ||
    info.SUPCARD_TYPE[11] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[11] ||
    info.SUPCARD_TYPE[12] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[12] ||
    info.SUPCARD_TYPE[13] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[13] ||
    info.SUPCARD_TYPE[14] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[14] ||
    info.SUPCARD_TYPE[15] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[15] ||
    info.SUPCARD_TYPE[16] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[16] ||
    info.SUPCARD_TYPE[17] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[17] ||
    info.SUPCARD_TYPE[18] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[18] ||
    info.SUPCARD_TYPE[19] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[19] ||
    info.SUPCARD_TYPE[20] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[20] ||
    info.SUPCARD_TYPE[21] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[21] ||
    info.SUPCARD_TYPE[22] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[22] ||
    info.SUPCARD_TYPE[23] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[23] ||
    info.SUPCARD_TYPE[24] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[24] ||
    info.SUPCARD_TYPE[25] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[25] ||
    info.SUPCARD_TYPE[26] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[26] ||
    info.SUPCARD_TYPE[27] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[27] ||
    info.SUPCARD_TYPE[28] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[28] ||
    info.SUPCARD_TYPE[29] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[29] ||
    info.SUPCARD_TYPE[30] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[30] ||
    info.SUPCARD_TYPE[31] != CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[31] ||
    info.SECTOR != CCommonData::getInstance()->m_CSystemPosSetup.SECTOR ||
    info.TIMEINTER != CCommonData::getInstance()->m_CSystemPosSetup.TIMEINTER ||
    info.TICKETTIME != CCommonData::getInstance()->m_CSystemPosSetup.TICKETTIME ||
    info.m_host_ip != CCommonData::getInstance()->m_CSystemPosSetup.m_host_ip
    )
    {
        if(CCommonData::getInstance()->messagebox_data_change())
        {
            //1.保存到缓存
            read_data();
            //2.通知保存到本地文件
            CProtocol::getInstance()->call_json_system_pos_config_set(CProtocol::kEnumSave);
        }
        else
        {
            //恢复到原来数据
            slot_pos_config_widget_event(CCommonData::getInstance()->m_CSystemPosSetup);
        }
    }

}
void CPosConfigWidget::read_data()
{
    CCommonData::getInstance()->m_CSystemPosSetup.DEVMODE = m_combobox_line1->currentIndex() + 1;   //终端模式
    CCommonData::getInstance()->m_CSystemPosSetup.CHARGMODE = m_combobox_line2->currentIndex()+1; //收费模式
    CCommonData::getInstance()->m_CSystemPosSetup.PSAMNUM = m_combobox_line3->currentIndex() +1;   //PSAM个数
    //支持卡类型

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[0] = (m_support_card_widget->m_checkbox_line2_1->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[1] = (m_support_card_widget->m_checkbox_line2_2->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[2] = (m_support_card_widget->m_checkbox_line2_3->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[3] = (m_support_card_widget->m_checkbox_line2_4->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[4] = (m_support_card_widget->m_checkbox_line3_1->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[5] = (m_support_card_widget->m_checkbox_line3_2->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[6] = (m_support_card_widget->m_checkbox_line3_3->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[7] = (m_support_card_widget->m_checkbox_line3_4->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[8] = (m_support_card_widget->m_checkbox_line4_1->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[9] = (m_support_card_widget->m_checkbox_line4_2->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[10] = (m_support_card_widget->m_checkbox_line4_3->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[11] = (m_support_card_widget->m_checkbox_line4_4->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[12] = (m_support_card_widget->m_checkbox_line5_1->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[13] = (m_support_card_widget->m_checkbox_line5_2->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[14] = (m_support_card_widget->m_checkbox_line5_3->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[15] = (m_support_card_widget->m_checkbox_line5_4->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[16] = (m_support_card_widget->m_checkbox_line6_1->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[17] = (m_support_card_widget->m_checkbox_line6_2->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[18] = (m_support_card_widget->m_checkbox_line6_3->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[19] = (m_support_card_widget->m_checkbox_line6_4->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[20] = (m_support_card_widget->m_checkbox_line7_1->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[21] = (m_support_card_widget->m_checkbox_line7_2->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[22] = (m_support_card_widget->m_checkbox_line7_3->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[23] = (m_support_card_widget->m_checkbox_line7_4->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[24] = (m_support_card_widget->m_checkbox_line8_1->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[25] = (m_support_card_widget->m_checkbox_line8_2->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[26] = (m_support_card_widget->m_checkbox_line8_3->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[27] = (m_support_card_widget->m_checkbox_line8_4->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[28] = (m_support_card_widget->m_checkbox_line9_1->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[29] = (m_support_card_widget->m_checkbox_line9_2->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[30] = (m_support_card_widget->m_checkbox_line9_3->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SUPCARD_TYPE[31] = (m_support_card_widget->m_checkbox_line9_4->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemPosSetup.SECTOR = m_combobox_line5->currentIndex() + 1;     //支持扇区
    CCommonData::getInstance()->m_CSystemPosSetup.TIMEINTER = m_lineedit6->text().toInt();   //刷卡时间间隔
    CCommonData::getInstance()->m_CSystemPosSetup.TICKETTIME = m_lineedit7->text().toInt();  //逃票最大时间
    CCommonData::getInstance()->m_CSystemPosSetup.m_host_ip = m_lineedit8->text();           //主机IP

}


void CPosConfigWidget::slot_pos_config_widget_event(const CSystemPosSetup &_info)
{
    qDebug()<<"slot_pos_config_widget_event() before set m_combobox_line1->currentIndex:"<<m_combobox_line1->currentIndex();
    qDebug()<<"slot_pos_config_widget_event() before set m_combobox_line2->currentIndex:"<<m_combobox_line2->currentIndex();
    m_combobox_line1->setCurrentIndex(_info.DEVMODE - 1);   //终端模式
    m_combobox_line2->setCurrentIndex(_info.CHARGMODE-1); //收费模式
    m_combobox_line3->setCurrentIndex(_info.PSAMNUM -1);   //PSAM个数
    qDebug()<<"slot_pos_config_widget_event() after set m_combobox_line1->currentIndex:"<<m_combobox_line1->currentIndex();
    qDebug()<<"slot_pos_config_widget_event() after set m_combobox_line2->currentIndex:"<<m_combobox_line2->currentIndex();
    //支持卡类型
    bool get_value = false;
    for(int index = 0;index < 32;index++)
    {
        get_value = (_info.SUPCARD_TYPE[index] == 0)? false:true;
        if(0 == index)
            m_support_card_widget->m_checkbox_line2_1->setChecked(get_value);
        else if(1 == index)
            m_support_card_widget->m_checkbox_line2_2->setChecked(get_value);
        else if(2 == index)
            m_support_card_widget->m_checkbox_line2_3->setChecked(get_value);
        else if(3 == index)
            m_support_card_widget->m_checkbox_line2_4->setChecked(get_value);
        else if(4 == index)
            m_support_card_widget->m_checkbox_line3_1->setChecked(get_value);
        else if(5 == index)
            m_support_card_widget->m_checkbox_line3_2->setChecked(get_value);
        else if(6 == index)
            m_support_card_widget->m_checkbox_line3_3->setChecked(get_value);
        else if(7 == index)
            m_support_card_widget->m_checkbox_line3_4->setChecked(get_value);
        else if(8 == index)
            m_support_card_widget->m_checkbox_line4_1->setChecked(get_value);
        else if(9 == index)
            m_support_card_widget->m_checkbox_line4_2->setChecked(get_value);
        else if(10 == index)
            m_support_card_widget->m_checkbox_line4_3->setChecked(get_value);
        else if(11 == index)
            m_support_card_widget->m_checkbox_line4_4->setChecked(get_value);
        else if(12 == index)
            m_support_card_widget->m_checkbox_line5_1->setChecked(get_value);
        else if(13 == index)
            m_support_card_widget->m_checkbox_line5_2->setChecked(get_value);
        else if(14 == index)
            m_support_card_widget->m_checkbox_line5_3->setChecked(get_value);
        else if(15 == index)
            m_support_card_widget->m_checkbox_line5_4->setChecked(get_value);
        else if(16 == index)
            m_support_card_widget->m_checkbox_line6_1->setChecked(get_value);
        else if(17 == index)
            m_support_card_widget->m_checkbox_line6_2->setChecked(get_value);
        else if(18 == index)
            m_support_card_widget->m_checkbox_line6_3->setChecked(get_value);
        else if(19 == index)
            m_support_card_widget->m_checkbox_line6_4->setChecked(get_value);
        else if(20 == index)
            m_support_card_widget->m_checkbox_line7_1->setChecked(get_value);
        else if(21 == index)
            m_support_card_widget->m_checkbox_line7_2->setChecked(get_value);
        else if(22 == index)
            m_support_card_widget->m_checkbox_line7_3->setChecked(get_value);
        else if(23 == index)
            m_support_card_widget->m_checkbox_line7_4->setChecked(get_value);
        else if(24 == index)
            m_support_card_widget->m_checkbox_line8_1->setChecked(get_value);
        else if(25 == index)
            m_support_card_widget->m_checkbox_line8_2->setChecked(get_value);
        else if(26 == index)
            m_support_card_widget->m_checkbox_line8_3->setChecked(get_value);
        else if(27 == index)
            m_support_card_widget->m_checkbox_line8_4->setChecked(get_value);
        else if(28 == index)
            m_support_card_widget->m_checkbox_line9_1->setChecked(get_value);
        else if(29 == index)
            m_support_card_widget->m_checkbox_line9_2->setChecked(get_value);
        else if(30 == index)
            m_support_card_widget->m_checkbox_line9_3->setChecked(get_value);
        else if(31 == index)
            m_support_card_widget->m_checkbox_line9_4->setChecked(get_value);
    }
    m_combobox_line5->setCurrentIndex(_info.SECTOR - 1);     //支持扇区
    m_lineedit6->setText(QString::number(_info.TIMEINTER));   //刷卡时间间隔
    m_lineedit7->setText(QString::number(_info.TICKETTIME));  //逃票最大时间
    m_lineedit8->setText(_info.m_host_ip); //主机IP

}


