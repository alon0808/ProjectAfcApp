#include "coordinate_collection_widget.h"

#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"
#include "class/common_interface.h"
#include <QListView>
#include "class/common/scrollcontroller.h"

#include "class/ltystring.h"

#define HELP_CONTENT    "1. 司机键盘蓝屏\
       主控模块坏或屏坏或键盘线坏，可以此排查主控模块是否正常工作，键盘背面插针以及主机后面插针是否松动，更换新键盘线测试排除。\
2. 司机键盘黑屏\
       检查设备是否通电，检查电源模块是否正常或主控模块是否正常或司机键盘是否正常或键盘线是否正常，通常黑屏最容易出现的故障为主机无电，或键盘烧坏。\
3. 司机键盘失灵（按一下图标跳几下）\
       检查键盘线针孔是否松动或检查司机键盘是否正常，更换键盘测试。\
4. 司机键盘卡死（按键无用或画面定死）\
       主控模块坏，换新主控模块。\
5. 开机时，司机键盘卡死在摄像头的四分格\
       这是因为读取不到SD卡，换主控模块或SD卡。也有可能是主控板内调度程序损坏用升级键盘升级后应可正常。\
6. 摄像头黑屏\
       先检查摄像头是否供电【可用手护住镜头看红外灯是否正常】，如果有电表示连接线和电源板为正常，更换摄像头即可；如果无电则需要检查连接线和电源板是否正常。\
7. 摄像头白屏\
       摄像头坏，应该换摄像头。\
8. 摄像头蓝屏\
       检查主机后摄像头线是否连接正常或摄像头端摄像头线是否连接，如果连接正常用替换方法逐个排查是摄像头问题还是连接线问题。"

#define STR_LONGITUDE   tr("经度")
#define STR_LATITUDE    tr("纬度")
#define STR_AZIMUTH     tr("方位角")
#define STR_SPEED       tr("时速")

CCoordinateCollectionWidget::CCoordinateCollectionWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setObjectName("CCoordinateCollectionWidget");
    this->setStyleSheet("QWidget#CCoordinateCollectionWidget{background-color:#ffffff;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->move(0,0);

        QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    pWidget->move(0,0);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");

    m_widget_main_1 = new QWidget(this);
    m_widget_main_1->setStyleSheet("QWidget{background-color:#000000;}");
    m_widget_main_1->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_widget_main_1->move(0,0);

    m_widget_main_2 = new QWidget(this);
    m_widget_main_2->setStyleSheet("QWidget{background-color:#000000;}");
    m_widget_main_2->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    m_widget_main_2->move(0,0);
    m_widget_main_2->hide();

    m_widget_bg = new QWidget(m_widget_main_1);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    m_widget_bg1 = new QWidget(m_widget_main_2);
    m_widget_bg1->setFixedSize(744,636);
    m_widget_bg1->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg1->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");

    coordinate_collection_init();
    coordinate_collection_help();

#if 0
    m_pMainLayout = new QVBoxLayout();//左 上 右 下
    m_pMainLayout->setContentsMargins(20,24,20,24);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setAlignment(Qt::AlignLeft/* | Qt::AlignTop*/);
    m_pMainLayout->addStretch(0);
    this->setLayout(m_pMainLayout);

    m_widget_main = new QWidget;
    m_widget_main->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH - 20 - 20,NAVIGATE_RIGHT_WIDGET_HEIGHT);

    m_widget_main->setStyleSheet("QWidget{background-color:#ffffff;}");
    m_pMainLayout->addWidget(m_widget_main);

    m_label_test = new QLabel(m_widget_main);
    m_label_test->setFixedSize(100,30);
    m_label_test->setText(tr("坐标采集"));
#endif

    m_auto_update_data = false;

}

#define LTY_QSCROLLBAR_RIGHT_VERTICAL \
"QScrollArea{border: 0px solid #275ba7;background:transparent;}"\
"QScrollBar:vertical{width:50px;background:transparent;margin:%1px,0px,0px,0px;padding-top:50px;padding-bottom:50px;}"\
"QScrollBar::handle:vertical{width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_scrollbar.png);border-radius:0px;min-height:100px;}"\
"QScrollBar::handle:vertical:hover{width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_scrollbar.png);border-radius:0px;min-height:100px;}"\
"QScrollBar::add-line:vertical{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_down_default.png);subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_up_default.png);subcontrol-position:top;}"\
"QScrollBar::add-line:vertical:hover{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_down_selected.png);subcontrol-position:bottom;}"\
"QScrollBar::sub-line:vertical:hover{height:50px;width:50px;border-image:url(:/img/res/main_page/menu/system_manage/content_up_selected.png);subcontrol-position:top;}"\
"QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:transparent;}"

///////////////////////////////////////////////////////////////////////////帮助 start
void CCoordinateCollectionWidget::coordinate_collection_help()
{//m_label_help

    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#94c6ff;}";
    QString str_show = tr("帮助");
    m_label_help = new QLabel(m_widget_main_2);
    CCommonInterface::init_label_text(m_label_help,596 - 250,159 - MAIN_HEAD_HEIGHT,75,40,str_show,str_sheet,Qt::AlignLeft,33);


   //  上 ,下,,//                margin:0px,0px,-100px,-100px;
    str_sheet = "QTextEdit{\
                background:transparent;\
                border:0px solid #DE5347;\
                font: 20px;\
                color:#8ea2c0;\
                }";

    m_textedit = new QTextEdit(m_widget_main_2);
    m_textedit->setReadOnly(true);
    m_textedit->setStyleSheet(str_sheet);
    m_textedit->move(0,232 - MAIN_HEAD_HEIGHT - 30 - 5 - 2);
    m_textedit->setFixedSize(700 + 44,390 + 60 + 10 + 2);
    //m_textedit->setText(HELP_CONTENT);
    m_textedit->document()->setDocumentMargin(55);

    m_textedit->append(QString("<font size=\"15\"color=\"#94c6ff\">%1</font>")
    .arg(tr("1. 司机键盘蓝屏")));


    m_textedit->append(QString("<font color=\"#8ea2c0\">%1</font>")
    .arg(tr("主控模块坏或屏坏或键盘线坏，可以此排查主控模块是否正常工作，键盘背面插针以及主机后面插针是否松动，更换新键盘线测试排除。")));

    m_textedit->append(QString("<font size=\"15\"color=\"#94c6ff\">%1</font>")
    .arg(tr("2. 司机键盘黑屏")));


    m_textedit->append(QString("<font color=\"#8ea2c0\">%1</font>")
    .arg(tr("检查设备是否通电，检查电源模块是否正常或主控模块是否正常或司机键盘是否正常或键盘线是否正常，通常黑屏最容易出现的故障为主机无电，或键盘烧坏。")));

    m_textedit->append(QString("<font size=\"15\"color=\"#94c6ff\">%1</font>")
    .arg(tr("3. 司机键盘失灵（按一下图标跳几下）")));


    m_textedit->append(QString("<font color=\"#8ea2c0\">%1</font>")
    .arg(tr("检查键盘线针孔是否松动或检查司机键盘是否正常，更换键盘测试。")));



    ScrollController *pScrollController = new ScrollController(this);
    pScrollController->addMonitorWidget((QScrollArea *)m_textedit);
    pScrollController->setVerticalScrollBarStyleSheet(LTY_QSCROLLBAR_RIGHT_VERTICAL,0,50,50);

    m_pushbutton_close = new QPushButton(m_widget_main_2);
    CCommonInterface::init_new_button(m_pushbutton_close,542 - 250,641 - MAIN_HEAD_HEIGHT + 17,160,65,LTY_PUSHBUTTON_OK);
    m_pushbutton_close->setText(tr("关闭"));

    connect(m_pushbutton_close,SIGNAL(clicked()),this,SLOT(slot_pushbutton_close()));
}

void CCoordinateCollectionWidget::slot_pushbutton_close()
{
    m_widget_main_2->hide();
    m_widget_main_1->show();
}

///////////////////////////////////////////////////////////////////////////帮助 end

void CCoordinateCollectionWidget::coordinate_collection_init()
{
    QString str_show = tr("线路");
    QString align_text = tr("限速段");
    QFont nameFont1;

    CCommonInterface::text_dispersed_align(align_text,str_show,26,nameFont1);

    m_label_line1 = new QLabel(m_widget_main_1);
    CCommonInterface::init_label_text(m_label_line1,290 - 250 - 5,179 - MAIN_HEAD_HEIGHT,75,40,str_show,"",Qt::AlignLeft);
    m_label_line1->setFont(nameFont1);

    m_combobox_1 = new QComboBox(m_widget_main_1);
    m_combobox_1->move(367 - 250,159 - MAIN_HEAD_HEIGHT);
    m_combobox_1->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    //m_combobox_1->addItem(tr("1路"));
    //m_combobox_1->addItem(tr("2路"));
    m_combobox_1->setView(new QListView());

    connect(m_combobox_1,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_combobox_1_change_index(int)));

    str_show = tr("方向");
    m_label_line1_1 = new QLabel(m_widget_main_1);
    CCommonInterface::init_label_text(m_label_line1_1,613 - 250 - 5,179 - MAIN_HEAD_HEIGHT,75,40,str_show,"",Qt::AlignLeft);
    m_label_line1_1->setFont(nameFont1);

    m_combobox_1_1 = new QComboBox(m_widget_main_1);
    m_combobox_1_1->move(690 - 250,159 - MAIN_HEAD_HEIGHT);
    m_combobox_1_1->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_1_1->addItem(tr("上行"));
    m_combobox_1_1->addItem(tr("下行"));
    m_combobox_1_1->setView(new QListView());
    connect(m_combobox_1_1,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_combobox_1_1_change_index(int)));

    str_show = tr("站点");
    m_label_line2 = new QLabel(m_widget_main_1);
    CCommonInterface::init_label_text(m_label_line2,290 - 250 - 5,256 - MAIN_HEAD_HEIGHT,75,40,str_show,"",Qt::AlignLeft);
    m_label_line2->setFont(nameFont1);

    m_combobox_2 = new QComboBox(m_widget_main_1);
    m_combobox_2->move(367 - 250,159 - MAIN_HEAD_HEIGHT + (COMBOBOX_HEIGHT + 12) * 1);
    m_combobox_2->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    //m_combobox_2->addItem(tr("航天微电机大厦"));
    m_combobox_2->setView(new QListView());

    str_show = tr("限速段");
    m_label_line3 = new QLabel(m_widget_main_1);
    CCommonInterface::init_label_text(m_label_line3,290 - 250 - 5,333 - MAIN_HEAD_HEIGHT,75,40,str_show,"",Qt::AlignLeft);


    m_combobox_3 = new QComboBox(m_widget_main_1);
    m_combobox_3->move(367 - 250,159 - MAIN_HEAD_HEIGHT + (COMBOBOX_HEIGHT + 12) * 2);
    m_combobox_3->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    //m_combobox_3->addItem(tr("航天微电机大厦"));
    m_combobox_3->setView(new QListView());

    str_show = tr("拐点");
    m_label_line4 = new QLabel(m_widget_main_1);
    CCommonInterface::init_label_text(m_label_line4,290 - 250 - 5,410 - MAIN_HEAD_HEIGHT,75,40,str_show,"",Qt::AlignLeft);
    m_label_line4->setFont(nameFont1);

    m_combobox_4 = new QComboBox(m_widget_main_1);
    m_combobox_4->move(367 - 250,159 - MAIN_HEAD_HEIGHT + (COMBOBOX_HEIGHT + 12) * 3);
    m_combobox_4->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
   // m_combobox_4->addItem(tr("第1点"));
    m_combobox_4->setView(new QListView());

    str_show = tr("场站");
    m_label_line5 = new QLabel(m_widget_main_1);
    CCommonInterface::init_label_text(m_label_line5,290 - 250 - 5,486 - MAIN_HEAD_HEIGHT,75,40,str_show,"",Qt::AlignLeft);
    m_label_line5->setFont(nameFont1);

    m_combobox_5 = new QComboBox(m_widget_main_1);
    m_combobox_5->move(367 - 250,159 - MAIN_HEAD_HEIGHT + (COMBOBOX_HEIGHT + 12) * 4);
    m_combobox_5->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    //m_combobox_5->addItem(tr("航天微电机大厦"));
    m_combobox_5->setView(new QListView());

    str_show = tr("场坐标");
    m_label_line5 = new QLabel(m_widget_main_1);
    CCommonInterface::init_label_text(m_label_line5,290 - 250 - 5,563 - MAIN_HEAD_HEIGHT,75,40,str_show,"",Qt::AlignLeft);

    m_combobox_6 = new QComboBox(m_widget_main_1);
    m_combobox_6->move(367 - 250,159 - MAIN_HEAD_HEIGHT + (COMBOBOX_HEIGHT + 12) * 5);
    m_combobox_6->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
   // m_combobox_6->addItem(tr("1"));
   for(int index = 0;index < 8;index++)
   {
        m_combobox_6->addItem(QString::number(index + 1));
   }
    m_combobox_6->setView(new QListView());

    QString str_sheet1 =    "QPushButton {\
                                      border:1px solid #275ba7;\
                                      background:#275ba7;\
                                      font:25px;\
                                      border-radius: 5px;\
                                      color:#94c6ff;\
                                      } \
                                      QPushButton:hover:pressed {background:#275ba7;} \
                                      QPushButton:hover:!pressed {background:#275ba7;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}";
    QString str_sheet2 =    "QPushButton {\
                                      border:1px solid #275ba7;\
                                      background:#0F1D31;\
                                      font:25px;\
                                      border-radius: 5px;\
                                      color:#94c6ff;\
                                      } \
                                      QPushButton:hover:pressed {background:#0F1D31;} \
                                      QPushButton:hover:!pressed {background:#0F1D31;}\
                                      QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}";



    m_pushbutton_1 = new QPushButton(m_widget_main_1);
    CCommonInterface::init_new_button(m_pushbutton_1,613 - 250,236 - MAIN_HEAD_HEIGHT,138,65,str_sheet1);
    m_pushbutton_1->setText(tr("进站点"));

    m_pushbutton_2 = new QPushButton(m_widget_main_1);
    CCommonInterface::init_new_button(m_pushbutton_2,782 - 250,236 - MAIN_HEAD_HEIGHT,138,65,str_sheet2);
    m_pushbutton_2->setText(tr("出站点"));

    m_pushbutton_3 = new QPushButton(m_widget_main_1);
    CCommonInterface::init_new_button(m_pushbutton_3,613 - 250,236 - MAIN_HEAD_HEIGHT + (65 + 12) * 1,138,65,str_sheet2);
    m_pushbutton_3->setText(tr("开始点"));

    m_pushbutton_4 = new QPushButton(m_widget_main_1);
    CCommonInterface::init_new_button(m_pushbutton_4,782 - 250,236 - MAIN_HEAD_HEIGHT + (65 + 12),138,65,str_sheet1);
    m_pushbutton_4->setText(tr("结束点"));

    m_pushbutton_5 = new QPushButton(m_widget_main_1);
    CCommonInterface::init_new_button(m_pushbutton_5,613 - 250,236 - MAIN_HEAD_HEIGHT + (65 + 12) * 2,138,65,str_sheet1);
    m_pushbutton_5->setText(tr("采集"));

    m_pushbutton_6 = new QPushButton(m_widget_main_1);
    CCommonInterface::init_new_button(m_pushbutton_6,613 - 250,236 - MAIN_HEAD_HEIGHT + (65 + 12) * 2 + 65 + 88,138,65,str_sheet1);
    m_pushbutton_6->setText(tr("采集"));


    m_pushbutton_help = new QPushButton(m_widget_main_1);
    CCommonInterface::init_new_button(m_pushbutton_help,352 - 250,643 - MAIN_HEAD_HEIGHT,160,65,LTY_PUSHBUTTON_HELP);
    m_pushbutton_help->setText(tr("帮助"));

    m_pushbutton_ok = new QPushButton(m_widget_main_1);
    CCommonInterface::init_new_button(m_pushbutton_ok,352 - 250 + (160 + 30) * 1,642 - MAIN_HEAD_HEIGHT,160,65,LTY_PUSHBUTTON_OK);
    m_pushbutton_ok->setText(CLtyString::getInstance()->m_confirm);

    m_pushbutton_cancel = new QPushButton(m_widget_main_1);
    CCommonInterface::init_new_button(m_pushbutton_cancel,352 - 250 + (160 + 30) * 2,642 - MAIN_HEAD_HEIGHT,160,65,LTY_PUSHBUTTON_OK);
    m_pushbutton_cancel->setText(CLtyString::getInstance()->m_cancel);

#define STR_LONGITUDE   tr("经度")
#define STR_LATITUDE    tr("纬度")
#define STR_AZIMUTH     tr("方位角")
#define STR_SPEED       tr("时速")

    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:23px;color:#275ba7;}";
    m_label_line10 = new QLabel(m_widget_main_1);
    str_show = STR_LONGITUDE + tr(":0000.00000");
    CCommonInterface::init_label_text(m_label_line10,766 - 250,479 - MAIN_HEAD_HEIGHT,0,30,str_show,str_sheet,Qt::AlignLeft,23);

    str_show = STR_LATITUDE + tr(":0000.00000");
    m_label_line11 = new QLabel(m_widget_main_1);
    CCommonInterface::init_label_text(m_label_line11,766 - 250,479 - MAIN_HEAD_HEIGHT + (30 + 11) * 1,0,30,str_show,str_sheet,Qt::AlignLeft,23);

    str_show = STR_AZIMUTH + tr(":270");
    m_label_line12 = new QLabel(m_widget_main_1);
    CCommonInterface::init_label_text(m_label_line12,766 - 250,479 - MAIN_HEAD_HEIGHT + (30 + 11) * 2,0,30,str_show,str_sheet,Qt::AlignLeft,23);

    str_show = STR_SPEED + tr(":80km/h");
    m_label_line13 = new QLabel(m_widget_main_1);
    CCommonInterface::init_label_text(m_label_line13,766 - 250,479 - MAIN_HEAD_HEIGHT + (30 + 11) * 3,0,30,str_show,str_sheet,Qt::AlignLeft,23);




    connect(m_pushbutton_1,SIGNAL(clicked()),this,SLOT(slot_pushbutton_1()));
    connect(m_pushbutton_2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_2()));
    connect(m_pushbutton_3,SIGNAL(clicked()),this,SLOT(slot_pushbutton_3()));
    connect(m_pushbutton_4,SIGNAL(clicked()),this,SLOT(slot_pushbutton_4()));
    connect(m_pushbutton_5,SIGNAL(clicked()),this,SLOT(slot_pushbutton_5()));
    connect(m_pushbutton_6,SIGNAL(clicked()),this,SLOT(slot_pushbutton_6()));


    connect(m_pushbutton_help,SIGNAL(clicked()),this,SLOT(slot_pushbutton_help()));
    connect(m_pushbutton_ok,SIGNAL(clicked()),this,SLOT(slot_pushbutton_ok()));
    connect(m_pushbutton_cancel,SIGNAL(clicked()),this,SLOT(slot_pushbutton_cancel()));

    read_data();


}

void CCoordinateCollectionWidget::slot_pushbutton_1()    //进站点
{
    read_data();
    CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumSave,1);
}
void CCoordinateCollectionWidget::slot_pushbutton_2()    //出站点
{
    read_data();
    CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumSave,2);
}
void CCoordinateCollectionWidget::slot_pushbutton_3()    //开始点
{
    read_data();
    CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumSave,3);
}
void CCoordinateCollectionWidget::slot_pushbutton_4()    //结束点
{
    read_data();
    CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumSave,4);
}
void CCoordinateCollectionWidget::slot_pushbutton_5()    //采集(拐点)
{
    read_data();
    CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumSave,5);
}
void CCoordinateCollectionWidget::slot_pushbutton_6()    //采集(场坐标)
{
    read_data();
    CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumSave,6);
}

void CCoordinateCollectionWidget::slot_pushbutton_help()
{
    m_widget_main_2->show();
    m_widget_main_1->hide();
}
void CCoordinateCollectionWidget::slot_pushbutton_ok()
{
    read_data();
    CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumSave,7);
}
void CCoordinateCollectionWidget::slot_pushbutton_cancel()
{
}

/*
    QList<QString> m_line_list; //线路列表
    QList<QString> m_site_list;//站点列表
    QList<QString> m_speed_limiting_section_list;//限速段列表
    QList<QString> m_inflection_point_list; //拐点列表
    QList<QString> m_station_list;  //场站列表
    QList<QString> m_field_coordinate_list; //场坐标列表
    char m_direction; //0:上行 1:下行

    QString m_longitude;//经度
    QString m_latitude; //纬度
    int m_azimuth;//方位角l
    int m_speed; //时速
*/
#include "common_data.h"
void CCoordinateCollectionWidget::sava_data()
{
    //CCommonData::getInstance()->m_CSystemCoordinateCollection;
    CSystemCoordinateCollection info;

    info.m_current_line = m_combobox_1->currentIndex();
    info.m_current_site = m_combobox_2->currentIndex();
    info.m_current_speed_limiting_section = m_combobox_3->currentIndex();
    info.m_current_inflection_point = m_combobox_4->currentIndex();
    info.m_current_station = m_combobox_5->currentIndex();
    info.m_current_field_coordinate = m_combobox_6->currentIndex();
    info.m_direction = m_combobox_1_1->currentIndex();

    info.m_longitude = m_label_line10->text().toInt();//经度
    info.m_latitude = m_label_line11->text().toInt(); //纬度
    info.m_azimuth = m_label_line12->text().toInt();//方位角
    info.m_speed = m_label_line13->text().toInt(); //时速

    if(info.m_current_line != CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_line ||
        info.m_current_site != CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_site ||
        info.m_current_speed_limiting_section != CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_speed_limiting_section ||
        info.m_current_inflection_point !=  CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_inflection_point ||
        info.m_current_station != CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_station ||
        info.m_current_field_coordinate != CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_field_coordinate ||
        info.m_direction != CCommonData::getInstance()->m_CSystemCoordinateCollection.m_direction ||
        info.m_longitude != CCommonData::getInstance()->m_CSystemCoordinateCollection.m_longitude ||
        info.m_latitude != CCommonData::getInstance()->m_CSystemCoordinateCollection.m_latitude ||
        info.m_azimuth != CCommonData::getInstance()->m_CSystemCoordinateCollection.m_azimuth ||
        info.m_speed != CCommonData::getInstance()->m_CSystemCoordinateCollection.m_speed
        )
    {
        if(CCommonData::getInstance()->messagebox_data_change())
        {
            read_data();
            CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumSave,0);
        }
        else
        {
            slot_coordinate_collection_widget_event(1,CCommonData::getInstance()->m_CSystemCoordinateCollection);
        }
    }


}

void CCoordinateCollectionWidget::read_data()
{
    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_line = m_combobox_1->currentIndex();
    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_site = m_combobox_2->currentIndex();
    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_speed_limiting_section = m_combobox_3->currentIndex();
    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_inflection_point = m_combobox_4->currentIndex();
    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_station = m_combobox_5->currentIndex();
    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_field_coordinate = m_combobox_6->currentIndex();
    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_direction = m_combobox_1_1->currentIndex();

    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_longitude = m_label_line10->text().toInt();//经度
    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_latitude = m_label_line11->text().toInt(); //纬度
    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_azimuth = m_label_line12->text().toInt();//方位角
    CCommonData::getInstance()->m_CSystemCoordinateCollection.m_speed = m_label_line13->text().toInt(); //时速
}

void CCoordinateCollectionWidget::slot_coordinate_collection_widget_event(char _type,const CSystemCoordinateCollection &_info)
{
    switch(_type)
    {
        case 1:
        {//更新数据
            m_auto_update_data = true;
            if(_info.m_line_list.size() > 0)
            {//线路列表
                m_combobox_1->clear();
                for(int index = 0;index < _info.m_line_list.size();index++)
                {
                    m_combobox_1->addItem(_info.m_line_list.at(index));
                }
                m_combobox_1->setCurrentIndex(CCommonData::getInstance()->m_CSystemCoordinateCollection.m_current_line);
            }

            if(_info.m_site_list.size() > 0)
            {//站点列表
                m_combobox_2->clear();
                for(int index = 0;index < _info.m_site_list.size();index++)
                {
                    m_combobox_2->addItem(_info.m_site_list.at(index));
                }
            }
            if(_info.m_speed_limiting_section_list.size() > 0)
            {//限速段列表
                m_combobox_3->clear();
                for(int index = 0;index < _info.m_speed_limiting_section_list.size();index++)
                {
                    m_combobox_3->addItem(_info.m_speed_limiting_section_list.at(index));
                }
            }
            if(_info.m_inflection_point_list.size() > 0)
            {//拐点列表
                m_combobox_4->clear();
                for(int index = 0;index < _info.m_inflection_point_list.size();index++)
                {
                    m_combobox_4->addItem(_info.m_inflection_point_list.at(index));
                }
            }
            if(_info.m_station_list.size() > 0)
            {//场站列表
                m_combobox_5->clear();
                for(int index = 0;index < _info.m_station_list.size();index++)
                {
                    m_combobox_5->addItem(_info.m_station_list.at(index));
                }
            }
            if(_info.m_field_coordinate_list.size() > 0)
            {//场坐标列表
                m_combobox_6->clear();
                for(int index = 0;index < _info.m_field_coordinate_list.size();index++)
                {
                    m_combobox_6->addItem(_info.m_field_coordinate_list.at(index));
                }
            }

            m_combobox_1_1->setCurrentIndex(_info.m_direction);
            m_auto_update_data = false;
        }break;
        case 2:
        {//获取GPS信息
            m_label_line10->setText(STR_LONGITUDE + ":" + _info.m_longitude);
            m_label_line11->setText(STR_LATITUDE + ":" + _info.m_latitude);
            m_label_line12->setText(STR_AZIMUTH + ":" + QString::number(_info.m_azimuth));
            m_label_line13->setText(STR_SPEED + ":" + QString::number(_info.m_speed) + "km/h");

        }break;
    }
    this->update();
}

void CCoordinateCollectionWidget::slot_combobox_1_change_index(int _index)
{
    do
    {
        if(m_auto_update_data)
            break;

        read_data();
        CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumSave,8);

    }while(0);
}
void CCoordinateCollectionWidget::slot_combobox_1_1_change_index(int _index)
{
    do
    {
        if(m_auto_update_data)
            break;

        read_data();
        CProtocol::getInstance()->call_json_system_coordinate_collection_set(CProtocol::kEnumSave,9);

    }while(0);
}










