#include "camera_config_widget.h"

#include "LtyCommonDefine.h"

#include "./class/common_interface.h"
#include "LtyCommonStyle.h"
#include <QListView>
#include <QTimer>

#include "common_data.h"
#include "class/common/my_messagebox.h"
#include "class/ltystring.h"

#define MIDDLE_WIDTH    714
#define MIDDLE_HEIGHT   600

/*
如果是widget->setLayout(QGridLayout);
那设置widget的背景色就好：
setAutoFillBackground(true);
setBackgroundRole(QPalette::Background);
setPalette(QPalette(QColor(255, 255, 0)));
*/
//基本设置
CBaseConfigWidget::CBaseConfigWidget(QWidget *parent) :
    QWidget(parent)
{
    //this->setObjectName("CBaseConfigWidget");
    this->setStyleSheet("QWidget{border:0px solid #24508F;background-color:#ffffff;}");
    this->setFixedSize(620,ROW_HEIGHT * 7);


    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 7);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");

    for(int index = 0;index < 7;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("录像制式");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_combobox_line1 = new QComboBox(this);
    m_combobox_line1->move(688 - 290,ROW_HEIGHT * 0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line1->setFixedSize(200,COMBOBOX_HEIGHT);
    m_combobox_line1->addItem(tr("PAL"));
    m_combobox_line1->addItem(tr("NTSC"));
    m_combobox_line1->setView(new QListView());


    str_show = tr("自动覆盖");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_combobox_line2 = new QComboBox(this);
    m_combobox_line2->move(688 - 290, ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->setFixedSize(200,COMBOBOX_HEIGHT);
    m_combobox_line2->addItem(tr("是"));
    m_combobox_line2->addItem(tr("否"));
    m_combobox_line2->setView(new QListView());

    str_show = tr("录像锁定");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(688 - 290, ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(175,COMBOBOX_HEIGHT);
    for(int index = 1;index <= 7;index++)
        m_combobox_line3->addItem(QString::number(index));
    m_combobox_line3->setView(new QListView());

    str_show = tr("D");
    m_label_line3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_1,688 - 290 + 175 + 10, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    str_show = tr("录像预录");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_checkbox_line4 = new QCheckBox(this);
    m_checkbox_line4->setFixedSize(70,70);
    m_checkbox_line4->move(688 - 290 - 20,ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4->setFocusPolicy(Qt::NoFocus);

    m_lineedit_line4 = new QLineEdit(this);
    m_lineedit_line4->move(724 - 290,ROW_HEIGHT * 3 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_line4->setFixedSize(120,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line4,true);

    str_show = tr("min");
    m_label_line4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_1,854 - 290, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    str_show = tr("网传模式");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_combobox_line5 = new QComboBox(this);
    m_combobox_line5->move(688 - 290, ROW_HEIGHT *4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5->setFixedSize(200,COMBOBOX_HEIGHT);
    m_combobox_line5->addItem(tr("流畅+清晰"));
    m_combobox_line5->addItem(tr("非常清晰"));
    m_combobox_line5->addItem(tr("清晰"));
    m_combobox_line5->addItem(tr("流畅"));
    m_combobox_line5->addItem(tr("非常流畅"));
    m_combobox_line5->setView(new QListView());


    str_show = tr("录像存储器");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_checkbox_line6 = new QCheckBox(this);
    m_checkbox_line6->setFixedSize(70,70);
    m_checkbox_line6->move(614 - 290 - 25,ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6->setFocusPolicy(Qt::NoFocus);

    m_combobox_line6 = new QComboBox(this);
    m_combobox_line6->move(648 - 290, ROW_HEIGHT *5 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line6->setFixedSize(240,COMBOBOX_HEIGHT);
    m_combobox_line6->addItem(tr("EMMC+外置SD卡"));
    m_combobox_line6->addItem(tr("EMMC"));
    m_combobox_line6->addItem(tr("外置SD卡"));
    m_combobox_line6->setView(new QListView());

    str_show = tr("录像模式");
    m_label_line7 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line7,30, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);

    m_combobox_line7 = new QComboBox(this);
    m_combobox_line7->move(648 - 290, ROW_HEIGHT * 6 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line7->setFixedSize(150,COMBOBOX_HEIGHT);
    m_combobox_line7->addItem(tr("镜像录像"));
    m_combobox_line7->addItem(tr("报警备份录像"));
    m_combobox_line7->addItem(tr("子码流录像"));
    m_combobox_line7->setView(new QListView());

    QString str_sheet = BUTTON_CONFIG_SHEET;

    m_pushbutton_line7 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line7,808 - 290,ROW_HEIGHT * 6 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_line7->setText(tr("配置"));
    connect(m_pushbutton_line7,SIGNAL(clicked()),this,SIGNAL(signal_pushbutton_config()));
}



//2.显示设置
CShowConfigWidget::CShowConfigWidget(QWidget *parent) :
    QWidget(parent)
{
    //this->setObjectName("CShowConfigWidget");
    this->setStyleSheet("QWidget{border: 0px;background-color:#ffffff;}");
    this->setFixedSize(620,ROW_HEIGHT * 6);


    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 6);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");

    for(int index = 0;index < 7;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("时间显示");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_checkbox_line1 = new QCheckBox(this);
    m_checkbox_line1->setFixedSize(70,70);
    m_checkbox_line1->move(744 - 290 - 25,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line1->setFocusPolicy(Qt::NoFocus);

    m_combobox_line1 = new QComboBox(this);
    m_combobox_line1->move(778 - 290, ROW_HEIGHT *0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    combobox_init(m_combobox_line1);


    str_show = tr("通道名称");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_checkbox_line2 = new QCheckBox(this);
    m_checkbox_line2->setFixedSize(70,70);
    m_checkbox_line2->move(744 - 290 - 25,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2->setFocusPolicy(Qt::NoFocus);

    m_combobox_line2 = new QComboBox(this);
    m_combobox_line2->move(778 - 290, ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    combobox_init(m_combobox_line2);

    str_show = tr("车辆编号");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_checkbox_line3 = new QCheckBox(this);
    m_checkbox_line3->setFixedSize(70,70);
    m_checkbox_line3->move(744 - 290 - 25,ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3->setFocusPolicy(Qt::NoFocus);

    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(778 - 290, ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    combobox_init(m_combobox_line3);

    str_show = tr("坐标信息");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_checkbox_line4 = new QCheckBox(this);
    m_checkbox_line4->setFixedSize(70,70);
    m_checkbox_line4->move(744 - 290 - 25,ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4->setFocusPolicy(Qt::NoFocus);

    m_combobox_line4 = new QComboBox(this);
    m_combobox_line4->move(778 - 290, ROW_HEIGHT * 3 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    combobox_init(m_combobox_line4);

    str_show = tr("实时速度");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_checkbox_line5 = new QCheckBox(this);
    m_checkbox_line5->setFixedSize(70,70);
    m_checkbox_line5->move(744 - 290 - 25,ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5->setFocusPolicy(Qt::NoFocus);

    m_combobox_line5 = new QComboBox(this);
    m_combobox_line5->move(778 - 290, ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    combobox_init(m_combobox_line5);

    str_show = tr("站点信息");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_checkbox_line6 = new QCheckBox(this);
    m_checkbox_line6->setFixedSize(70,70);
    m_checkbox_line6->move(744 - 290 - 25,ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6->setFocusPolicy(Qt::NoFocus);

    m_combobox_line6 = new QComboBox(this);
    m_combobox_line6->move(778 - 290, ROW_HEIGHT * 5 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    combobox_init(m_combobox_line6);

}

void CShowConfigWidget::combobox_init(QComboBox *_combobox)
{
    _combobox->addItem(tr("正上"));
    _combobox->addItem(tr("左上"));
    _combobox->addItem(tr("右上"));
    _combobox->addItem(tr("正下"));
    _combobox->addItem(tr("左下"));
    _combobox->addItem(tr("右下"));
    _combobox->setFixedSize(107,COMBOBOX_HEIGHT);
    _combobox->setView(new QListView());
}


CCamLineWidget::CCamLineWidget(int _type,QWidget *parent) :
    QWidget(parent)
{
    m_type = _type;

    this->setStyleSheet("QWidget{border: 0px;background:transparent;}");
    this->setFixedSize(620,ROW_HEIGHT);

    QString str_show = tr("");
    m_label_line = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line,30, ROW_HEIGHT * 0,400,ROW_HEIGHT,str_show,"",Qt::AlignLeft | Qt::AlignVCenter);

    QString str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_line = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line,808 - 290,ROW_HEIGHT * 0 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_line->setText(tr("配置"));

    connect(m_pushbutton_line,SIGNAL(clicked()),this,SLOT(slot_pushbutton_line()));

}

void CCamLineWidget::slot_pushbutton_line()
{
    emit signal_button_data(m_type,m_number);
}

void CCamLineWidget::set_text(QString _text,char _number)
{
    m_label_line->setText(_text);
    m_number = _number;
}

//3.通道设置
CPassagewayConfigWidget::CPassagewayConfigWidget(QWidget *parent) :
    QWidget(parent)
{
   // this->setObjectName("CPassagewayConfigWidget");
    this->setStyleSheet("QWidget{border: 0px;background-color:#ffffff;}");
    this->setFixedSize(620,ROW_HEIGHT * 18);


    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 18);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");

    for(int index = 0;index < 18;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }
    //QLabel *m_label_line0;
    //QComboBox *m_combobox_line0;
    QString str_show = tr("模拟数量");
    m_label_line0 = new QLabel(this);
    int str_length = CCommonInterface::init_label_text(m_label_line0,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_combobox_line0 = new QComboBox(this);
    m_combobox_line0->move(620 - 200 - 20,ROW_HEIGHT * 0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line0->setFixedSize(200,COMBOBOX_HEIGHT);
    m_combobox_line0->addItem(tr("0"));
    m_combobox_line0->addItem(tr("1"));
    m_combobox_line0->addItem(tr("2"));
    m_combobox_line0->addItem(tr("3"));
    m_combobox_line0->addItem(tr("4"));
    m_combobox_line0->addItem(tr("5"));
    m_combobox_line0->addItem(tr("6"));
    m_combobox_line0->setView(new QListView());
    m_combobox_line0->setCurrentIndex(6);

    str_show = tr("IPC数量");
    m_label_line0_1 = new QLabel(this);
    str_length = CCommonInterface::init_label_text(m_label_line0_1,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_combobox_line0_1 = new QComboBox(this);
    m_combobox_line0_1->move(620 - 200 - 20,ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line0_1->setFixedSize(200,COMBOBOX_HEIGHT);
    m_combobox_line0_1->addItem(tr("0"));
    m_combobox_line0_1->addItem(tr("1"));
    m_combobox_line0_1->addItem(tr("2"));
    m_combobox_line0_1->addItem(tr("3"));
    m_combobox_line0_1->addItem(tr("4"));
    m_combobox_line0_1->addItem(tr("5"));
    m_combobox_line0_1->addItem(tr("6"));
    m_combobox_line0_1->addItem(tr("7"));
    m_combobox_line0_1->addItem(tr("8"));
    m_combobox_line0_1->addItem(tr("9"));
    m_combobox_line0_1->addItem(tr("10"));
    m_combobox_line0_1->setView(new QListView());
    m_combobox_line0_1->setCurrentIndex(10);

    connect(m_combobox_line0,SIGNAL(currentIndexChanged(int)),this,SLOT(combobox_line0_index_changed(int)));
    connect(m_combobox_line0_1,SIGNAL(currentIndexChanged(int)),this,SLOT(combobox_line1_index_changed(int)));

    m_vboxlayout_widget=new QVBoxLayout(this);
    m_vboxlayout_widget->setContentsMargins(0,ROW_HEIGHT * 2,0,0);
    m_vboxlayout_widget->setAlignment(Qt::AlignTop|Qt::AlignLeft);
    m_vboxlayout_widget->setSpacing(0);

    m_SimulationWidget1 = new CCamLineWidget(CCamLineWidget::kEnumSimulation,this);
    m_SimulationWidget1->set_text("CAM-通道1(CVBS)",1);
    m_SimulationWidget2 = new CCamLineWidget(CCamLineWidget::kEnumSimulation,this);
    m_SimulationWidget2->set_text("CAM-通道2(CVBS)",2);
    m_SimulationWidget3 = new CCamLineWidget(CCamLineWidget::kEnumSimulation,this);
    m_SimulationWidget3->set_text("CAM-通道3(CVBS)",3);
    m_SimulationWidget4 = new CCamLineWidget(CCamLineWidget::kEnumSimulation,this);
    m_SimulationWidget4->set_text("CAM-通道4(CVBS)",4);
    m_SimulationWidget5 = new CCamLineWidget(CCamLineWidget::kEnumSimulation,this);
    m_SimulationWidget5->set_text("CAM-通道5(CVBS)",5);
    m_SimulationWidget6 = new CCamLineWidget(CCamLineWidget::kEnumSimulation,this);
    m_SimulationWidget6->set_text("CAM-通道6(CVBS)",6);

    connect(m_SimulationWidget1,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_SimulationWidget2,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_SimulationWidget3,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_SimulationWidget4,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_SimulationWidget5,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_SimulationWidget6,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));


    m_simulation_list.push_back(m_SimulationWidget1);
    m_simulation_list.push_back(m_SimulationWidget2);
    m_simulation_list.push_back(m_SimulationWidget3);
    m_simulation_list.push_back(m_SimulationWidget4);
    m_simulation_list.push_back(m_SimulationWidget5);
    m_simulation_list.push_back(m_SimulationWidget6);

    m_IPCWidget1 = new CCamLineWidget(CCamLineWidget::kEnumIpc,this);
    m_IPCWidget1->set_text("PON-通道7(IPC)",7);
    m_IPCWidget2 = new CCamLineWidget(CCamLineWidget::kEnumIpc,this);
    m_IPCWidget2->set_text("PON-通道8(IPC)",8);
    m_IPCWidget3 = new CCamLineWidget(CCamLineWidget::kEnumIpc,this);
    m_IPCWidget3->set_text("PON-通道9(IPC)",9);
    m_IPCWidget4 = new CCamLineWidget(CCamLineWidget::kEnumIpc,this);
    m_IPCWidget4->set_text("PON-通道10(IPC)",10);
    m_IPCWidget5 = new CCamLineWidget(CCamLineWidget::kEnumIpc,this);
    m_IPCWidget5->set_text("PON-通道11(IPC)",11);
    m_IPCWidget6 = new CCamLineWidget(CCamLineWidget::kEnumIpc,this);
    m_IPCWidget6->set_text("PON-通道12(IPC)",12);
    m_IPCWidget7 = new CCamLineWidget(CCamLineWidget::kEnumIpc,this);
    m_IPCWidget7->set_text("PON-通道13(IPC)",13);
    m_IPCWidget8 = new CCamLineWidget(CCamLineWidget::kEnumIpc,this);
    m_IPCWidget8->set_text("PON-通道14(IPC)",14);
    m_IPCWidget9 = new CCamLineWidget(CCamLineWidget::kEnumIpc,this);
    m_IPCWidget9->set_text("PON-通道15(IPC)",15);
    m_IPCWidget10 = new CCamLineWidget(CCamLineWidget::kEnumIpc,this);
    m_IPCWidget10->set_text("PON-通道16(IPC)",16);

    connect(m_IPCWidget1,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_IPCWidget2,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_IPCWidget3,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_IPCWidget4,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_IPCWidget5,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_IPCWidget6,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_IPCWidget7,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_IPCWidget8,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_IPCWidget9,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));
    connect(m_IPCWidget10,SIGNAL(signal_button_data(int,int)),this,SLOT(slot_button_clicked(int,int)));

    m_ipc_list.push_back(m_IPCWidget1);
    m_ipc_list.push_back(m_IPCWidget2);
    m_ipc_list.push_back(m_IPCWidget3);
    m_ipc_list.push_back(m_IPCWidget4);
    m_ipc_list.push_back(m_IPCWidget5);
    m_ipc_list.push_back(m_IPCWidget6);
    m_ipc_list.push_back(m_IPCWidget7);
    m_ipc_list.push_back(m_IPCWidget8);
    m_ipc_list.push_back(m_IPCWidget9);
    m_ipc_list.push_back(m_IPCWidget10);

    m_vboxlayout_widget->addWidget(m_SimulationWidget1);
    m_vboxlayout_widget->addWidget(m_SimulationWidget2);
    m_vboxlayout_widget->addWidget(m_SimulationWidget3);
    m_vboxlayout_widget->addWidget(m_SimulationWidget4);
    m_vboxlayout_widget->addWidget(m_SimulationWidget5);
    m_vboxlayout_widget->addWidget(m_SimulationWidget6);

    m_vboxlayout_widget->addWidget(m_IPCWidget1);
    m_vboxlayout_widget->addWidget(m_IPCWidget2);
    m_vboxlayout_widget->addWidget(m_IPCWidget3);
    m_vboxlayout_widget->addWidget(m_IPCWidget4);
    m_vboxlayout_widget->addWidget(m_IPCWidget5);
    m_vboxlayout_widget->addWidget(m_IPCWidget6);
    m_vboxlayout_widget->addWidget(m_IPCWidget7);
    m_vboxlayout_widget->addWidget(m_IPCWidget8);
    m_vboxlayout_widget->addWidget(m_IPCWidget9);
    m_vboxlayout_widget->addWidget(m_IPCWidget10);

}

void CPassagewayConfigWidget::slot_button_clicked(int _type,int _number)
{
    emit signal_button_data(_type,_number,m_combobox_line0->currentIndex(),m_combobox_line0_1->currentIndex());
}

void CPassagewayConfigWidget::combobox_line0_index_changed(int index)
{
    combobox_data_handle();
}
void CPassagewayConfigWidget::combobox_line1_index_changed(int index)
{
    combobox_data_handle();
}
void CPassagewayConfigWidget::combobox_data_handle()
{
    int total_number = 2;
    int cam_number = m_combobox_line0->currentIndex();
    int ipc_number = m_combobox_line0_1->currentIndex();
    int sort_index = 0;

    total_number += cam_number;
    total_number += ipc_number;

    this->setFixedSize(620,ROW_HEIGHT * total_number);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * total_number);

    for(int index = 0;index < m_simulation_list.size();index++)
    {
        m_simulation_list.at(index)->show();
        if(index < cam_number)
        {
            ++sort_index;
            m_simulation_list.at(index)->set_text(QString("CAM-通道%1(CVBS)").arg(sort_index),sort_index);
        }
    }
    for(int index = 0;index < m_ipc_list.size();index++)
    {
        m_ipc_list.at(index)->show();
        if(index < ipc_number)
        {
            ++sort_index;
            m_ipc_list.at(index)->set_text(QString("PON-通道%1(IPC)").arg(sort_index),sort_index);
        }
    }
    for(int index = cam_number;index < m_simulation_list.size();index++)
        m_simulation_list.at(index)->hide();

    for(int index = ipc_number;index < m_ipc_list.size();index++)
        m_ipc_list.at(index)->hide();




    emit signal_current_passageway(total_number,cam_number,ipc_number);
}



CImageSetWidget::CImageSetWidget(QWidget *parent) :
    QWidget(parent)
{
    QWidget *pRectWidget1 = new QWidget(this);
    pRectWidget1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget1->setFixedSize(620,77);
    pRectWidget1->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    QWidget *pWidgetRect2 = new QWidget(this);

    pWidgetRect2->setFixedSize(620,430);
    pWidgetRect2->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + 77);
    pWidgetRect2->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    QString str_show = tr("镜像通道");
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
    str_show = tr("1");
    m_label_line2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line2_2 = new QCheckBox(this);
    m_checkbox_line2_2->setFixedSize(70,70);
    m_checkbox_line2_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("2");
    m_label_line2_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line2_3 = new QCheckBox(this);
    m_checkbox_line2_3->setFixedSize(70,70);
    m_checkbox_line2_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("3");
    m_label_line2_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line2_4 = new QCheckBox(this);
    m_checkbox_line2_4->setFixedSize(70,70);
    m_checkbox_line2_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("4");
    m_label_line2_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    //5~8
    m_checkbox_line3_1 = new QCheckBox(this);
    m_checkbox_line3_1->setFixedSize(70,70);
    m_checkbox_line3_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("5");
    m_label_line3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line3_2 = new QCheckBox(this);
    m_checkbox_line3_2->setFixedSize(70,70);
    m_checkbox_line3_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("6");
    m_label_line3_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line3_3 = new QCheckBox(this);
    m_checkbox_line3_3->setFixedSize(70,70);
    m_checkbox_line3_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("7");
    m_label_line3_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line3_4 = new QCheckBox(this);
    m_checkbox_line3_4->setFixedSize(70,70);
    m_checkbox_line3_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("8");
    m_label_line3_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    //9~12  (pon1 ~ pon10)
    m_checkbox_line4_1 = new QCheckBox(this);
    m_checkbox_line4_1->setFixedSize(70,70);
    m_checkbox_line4_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("9");
    m_label_line4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_1,382 - 290 + 20,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line4_2 = new QCheckBox(this);
    m_checkbox_line4_2->setFixedSize(70,70);
    m_checkbox_line4_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("10");
    m_label_line4_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line4_3 = new QCheckBox(this);
    m_checkbox_line4_3->setFixedSize(70,70);
    m_checkbox_line4_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("11");
    m_label_line4_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line4_4 = new QCheckBox(this);
    m_checkbox_line4_4->setFixedSize(70,70);
    m_checkbox_line4_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4_4->setStyleSheet(CHECK_BOX_SHEET);
    m_checkbox_line4_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("12");
    m_label_line4_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    //13~16
    m_checkbox_line5_1 = new QCheckBox(this);
    m_checkbox_line5_1->setFixedSize(70,70);
    m_checkbox_line5_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_1->setFocusPolicy(Qt::NoFocus);
    str_show = tr("13");
    m_label_line5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line5_2 = new QCheckBox(this);
    m_checkbox_line5_2->setFixedSize(70,70);
    m_checkbox_line5_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_2->setFocusPolicy(Qt::NoFocus);
    str_show = tr("14");
    m_label_line5_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line5_3 = new QCheckBox(this);
    m_checkbox_line5_3->setFixedSize(70,70);
    m_checkbox_line5_3->move(648 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_3->setFocusPolicy(Qt::NoFocus);
    str_show = tr("15");
    m_label_line5_3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_3,691 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line5_4 = new QCheckBox(this);
    m_checkbox_line5_4->setFixedSize(70,70);
    m_checkbox_line5_4->move(802 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5_4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5_4->setFocusPolicy(Qt::NoFocus);
    str_show = tr("16");
    m_label_line5_4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_4,845 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);

    m_checkbox_line6_1 = new QCheckBox(this);
    m_checkbox_line6_1->setFixedSize(70,70);
    m_checkbox_line6_1->move(341 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6_1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6_1->setFocusPolicy(Qt::NoFocus);
    m_checkbox_line6_1->hide();

    str_show = tr("PON9");
    m_label_line6_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_1,382 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);
    m_label_line6_1->hide();

    m_checkbox_line6_2 = new QCheckBox(this);
    m_checkbox_line6_2->setFixedSize(70,70);
    m_checkbox_line6_2->move(495 - 290 + 0 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6_2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6_2->setFocusPolicy(Qt::NoFocus);
    m_checkbox_line6_2->hide();

    str_show = tr("PON10");
    m_label_line6_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_2,537 - 290 + 20 - 10,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,22);
    m_label_line6_2->hide();



    str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_default = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_default,757 - 290 + 50,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,100,60,str_sheet);
    m_pushbutton_default->setText(tr("默认"));

}

CPonCamWidget::CPonCamWidget(char _type,QWidget *parent) :
    QWidget(parent)
{
    //this->setObjectName("CPassagewayConfigWidget");
    this->setStyleSheet("QWidget{border: 0px;background-color:#ffffff;}");
    set_type(_type);
    m_type = _type;
    m_auto_change = false;
}



void CPonCamWidget::pon_init()
{

    QWidget *pRectWidget1 = new QWidget(this);
    pRectWidget1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget1->setFixedSize(620,77);
    pRectWidget1->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    QWidget *pWidgetRect2 = new QWidget(this);

    pWidgetRect2->setFixedSize(620,ROW_HEIGHT * 8 + 2);
    pWidgetRect2->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + 77);
    pWidgetRect2->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < 9;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }

    QString str_show = tr("PON-1通道配置");
    m_label_line1 = new QLabel(this);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,300,77,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,33);

    QString str_sheet1 = "QLabel{border:0px solid #8ea2c0;background:transparent;font:26px;color:#94c6ff;}";
    str_show = tr("IP地址");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_lineedit_line2 = new QLineEdit(this);
    m_lineedit_line2->move(638 - 290 - 20,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit_line2->setFixedSize(160 + 20,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit_line2);

    str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_line2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_line2,808 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - 60)/2,80,60,str_sheet);
    m_pushbutton_line2->setText(tr("搜索"));

    connect(m_pushbutton_line2,SIGNAL(clicked()),this,SLOT(slot_pushbutton_search()));

    str_show = tr("录音");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(768 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(120,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("开启"));
    m_combobox_line3->addItem(tr("关闭"));
    m_combobox_line3->setView(new QListView());

    str_show = tr("分辨率");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    str_show = tr("主");
    m_label_line4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_1,568 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_combobox_line4_1 = new QComboBox(this);
    m_combobox_line4_1->move(598 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line4_1->setFixedSize(120,COMBOBOX_HEIGHT);
    m_combobox_line4_1->addItem(tr("1080P"));
    m_combobox_line4_1->addItem(tr("720P"));
    m_combobox_line4_1->addItem(tr("D1"));
    m_combobox_line4_1->addItem(tr("CIF"));
    m_combobox_line4_1->addItem(tr("QCIF"));
    m_combobox_line4_1->setView(new QListView());

    str_show = tr("子");
    m_label_line4_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_2,738 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_combobox_line4_2 = new QComboBox(this);
    m_combobox_line4_2->move(768 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line4_2->setFixedSize(120,COMBOBOX_HEIGHT);
    m_combobox_line4_2->addItem(tr("1080P"));
    m_combobox_line4_2->addItem(tr("720P"));
    m_combobox_line4_2->addItem(tr("D1"));
    m_combobox_line4_2->addItem(tr("CIF"));
    m_combobox_line4_2->addItem(tr("QCIF"));
    m_combobox_line4_2->setView(new QListView());
    m_combobox_line4_2->setEnabled(false);

    str_show = tr("帧率");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    str_show = tr("主");
    m_label_line5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_1,568 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_combobox_line5_1 = new QComboBox(this);
    m_combobox_line5_1->move(598 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5_1->setFixedSize(120,COMBOBOX_HEIGHT);
    for(int index = 1;index <= 25;index++)
        m_combobox_line5_1->addItem(QString::number(index));
    m_combobox_line5_1->setView(new QListView());

    str_show = tr("子");
    m_label_line5_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_2,738 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);
//  CCommonInterface::init_label_text(m_label_line6_2,738 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);
    m_combobox_line5_2 = new QComboBox(this);
    m_combobox_line5_2->move(768 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5_2->setFixedSize(120,COMBOBOX_HEIGHT);
    for(int index = 1;index <= 25;index++)
        m_combobox_line5_2->addItem(QString::number(index));
    m_combobox_line5_2->setView(new QListView());
    m_combobox_line5_2->setEnabled(false);


    str_show = tr("画质");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    str_show = tr("主");
    m_label_line6_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_1,568 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_combobox_line6_1 = new QComboBox(this);
    m_combobox_line6_1->move(598 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line6_1->setFixedSize(120,COMBOBOX_HEIGHT);
    m_combobox_line6_1->addItem(tr("高"));
    m_combobox_line6_1->addItem(tr("中"));
    m_combobox_line6_1->addItem(tr("低"));
    m_combobox_line6_1->setView(new QListView());

    str_show = tr("子");
    m_label_line6_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_2,738 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_combobox_line6_2 = new QComboBox(this);
    m_combobox_line6_2->move(768 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line6_2->setFixedSize(120,COMBOBOX_HEIGHT);
    m_combobox_line6_2->addItem(tr("高"));
    m_combobox_line6_2->addItem(tr("中"));
    m_combobox_line6_2->addItem(tr("低"));
    m_combobox_line6_2->setView(new QListView());
    m_combobox_line6_2->setEnabled(false);

    str_show = tr("标识");
    m_label_line7 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line7,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);

    m_combobox_line7 = new QComboBox(this);
    m_combobox_line7->move(728 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line7->setFixedSize(160,COMBOBOX_HEIGHT);
    m_combobox_line7->addItem(tr("IPC摄像机"));
    m_combobox_line7->addItem(tr("其他设备"));
    m_combobox_line7->setView(new QListView());

    str_show = tr("复制样式到");
    m_label_line8 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line8,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);
    m_combobox_line8 = new QComboBox(this);
    m_combobox_line8->move(290 - 250 + 620 - 65 - (COMBOBOX_WIDTH - 30) + 3,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 7 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line8->setFixedSize(COMBOBOX_WIDTH - 30,COMBOBOX_HEIGHT);
    m_combobox_line8->addItem(tr("全部PON通道"));
    m_combobox_line8->addItem(tr("PON7"));
    m_combobox_line8->addItem(tr("PON8"));
    m_combobox_line8->addItem(tr("PON9"));
    m_combobox_line8->addItem(tr("PON10"));
    m_combobox_line8->addItem(tr("PON11"));
    m_combobox_line8->addItem(tr("PON12"));
    m_combobox_line8->addItem(tr("PON13"));
    m_combobox_line8->addItem(tr("PON14"));
    m_combobox_line8->addItem(tr("PON15"));
    m_combobox_line8->addItem(tr("PON16"));
    //m_combobox_line8->addItem(tr("PON11"));
    //m_combobox_line8->addItem(tr("PON12"));
    m_combobox_line8->setView(new QListView());

    connect(m_combobox_line8,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_pon_widet_combobox_change(int)));

    str_show = tr("镜像");
    m_label_line8 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line8,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);

    m_checkbox_line9 = new QCheckBox(this);
    m_checkbox_line9->setFixedSize(70,70);
    m_checkbox_line9->move(688 - 290 - 20 + 170,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 8 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line9->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line9->setFocusPolicy(Qt::NoFocus);


    str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_default = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_default,808 - 290 + 42,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 9 + 25,100,60,str_sheet);
    m_pushbutton_default->setText(tr("默认"));

    connect(m_pushbutton_default,SIGNAL(clicked()),this,SLOT(slot_pushbutton_default()));

}
void CPonCamWidget::cam_init()
{
        QWidget *pRectWidget1 = new QWidget(this);
    pRectWidget1->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50);
    pRectWidget1->setFixedSize(620,77);
    pRectWidget1->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#3EA2FF;}");

    QWidget *pWidgetRect2 = new QWidget(this);

    pWidgetRect2->setFixedSize(620,ROW_HEIGHT * 7 + 2);
    pWidgetRect2->move(290 - 250,158 - MAIN_HEAD_HEIGHT - 50 + 77);
    pWidgetRect2->setStyleSheet("QWidget{border:1px solid #1B4279;background-color:#02101D;}");

    for(int index = 0;index < 8;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(290 - 250,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");
    }

    QString str_show = tr("CAM1-1通道配置");
    m_label_line1 = new QLabel(this);
    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#0f1d31;}";
    CCommonInterface::init_label_text(m_label_line1,290 - 250 + 30,159 - MAIN_HEAD_HEIGHT - 50,300,77,str_show,str_sheet,Qt::AlignLeft | Qt::AlignVCenter,33);

    str_show = tr("录音");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(768 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(120,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("开启"));
    m_combobox_line3->addItem(tr("关闭"));
    m_combobox_line3->setView(new QListView());

    str_show = tr("分辨率");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    str_show = tr("主");
    m_label_line4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_1,568 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_combobox_line4_1 = new QComboBox(this);
    m_combobox_line4_1->move(598 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line4_1->setFixedSize(120,COMBOBOX_HEIGHT);
    m_combobox_line4_1->addItem(tr("1080P"));
    m_combobox_line4_1->addItem(tr("720P"));
    m_combobox_line4_1->addItem(tr("D1"));
    m_combobox_line4_1->addItem(tr("CIF"));
    m_combobox_line4_1->addItem(tr("QCIF"));
    m_combobox_line4_1->setView(new QListView());

    str_show = tr("子");
    m_label_line4_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_2,738 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_combobox_line4_2 = new QComboBox(this);
    m_combobox_line4_2->move(768 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line4_2->setFixedSize(120,COMBOBOX_HEIGHT);
    m_combobox_line4_2->addItem(tr("1080P"));
    m_combobox_line4_2->addItem(tr("720P"));
    m_combobox_line4_2->addItem(tr("D1"));
    m_combobox_line4_2->addItem(tr("CIF"));
    m_combobox_line4_2->addItem(tr("QCIF"));
    m_combobox_line4_2->setView(new QListView());
    m_combobox_line4_2->setEnabled(false);

    str_show = tr("帧率");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    str_show = tr("主");
    m_label_line5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_1,568 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_combobox_line5_1 = new QComboBox(this);
    m_combobox_line5_1->move(598 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5_1->setFixedSize(120,COMBOBOX_HEIGHT);
    for(int index = 1;index <= 25;index++)
        m_combobox_line5_1->addItem(QString::number(index));
    m_combobox_line5_1->setView(new QListView());

    str_show = tr("子");
    m_label_line5_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_2,738 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_combobox_line5_2 = new QComboBox(this);
    m_combobox_line5_2->move(768 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 3 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5_2->setFixedSize(120,COMBOBOX_HEIGHT);
    for(int index = 1;index <= 25;index++)
        m_combobox_line5_2->addItem(QString::number(index));
    m_combobox_line5_2->setView(new QListView());
    m_combobox_line5_2->setEnabled(false);

    str_show = tr("画质");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    str_show = tr("主");
    m_label_line6_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_1,568 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_combobox_line6_1 = new QComboBox(this);
    m_combobox_line6_1->move(598 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line6_1->setFixedSize(120,COMBOBOX_HEIGHT);
    m_combobox_line6_1->addItem(tr("高"));
    m_combobox_line6_1->addItem(tr("中"));
    m_combobox_line6_1->addItem(tr("低"));
    m_combobox_line6_1->setView(new QListView());

    str_show = tr("子");
    m_label_line6_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_2,738 - 290, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_combobox_line6_2 = new QComboBox(this);
    m_combobox_line6_2->move(768 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line6_2->setFixedSize(120,COMBOBOX_HEIGHT);
    m_combobox_line6_2->addItem(tr("高"));
    m_combobox_line6_2->addItem(tr("中"));
    m_combobox_line6_2->addItem(tr("低"));
    m_combobox_line6_2->setView(new QListView());
    m_combobox_line6_2->setEnabled(false);

    str_show = tr("标识");
    m_label_line7 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line7,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_combobox_line7 = new QComboBox(this);
    m_combobox_line7->move(728 - 290,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 5 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line7->setFixedSize(160,COMBOBOX_HEIGHT);
    m_combobox_line7->addItem(tr("视频录像"));
    m_combobox_line7->addItem(tr("客流统计"));
    m_combobox_line7->setView(new QListView());

    str_show = tr("复制样式到");
    m_label_line8 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line8,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);
    m_combobox_line8 = new QComboBox(this);
    m_combobox_line8->move(290 - 250 + 620 - 65 - (COMBOBOX_WIDTH - 30) + 3,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 6 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line8->setFixedSize(COMBOBOX_WIDTH - 30,COMBOBOX_HEIGHT);
    m_combobox_line8->addItem(tr("全部CAM通道"));
    m_combobox_line8->addItem(tr("PON1"));
    m_combobox_line8->addItem(tr("PON2"));
    m_combobox_line8->addItem(tr("PON3"));
    m_combobox_line8->addItem(tr("PON4"));
    m_combobox_line8->addItem(tr("PON5"));
    m_combobox_line8->addItem(tr("PON6"));

    m_combobox_line8->setView(new QListView());

    connect(m_combobox_line8,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_cam_widet_combobox_change(int)));

    str_show = tr("镜像");
    m_label_line8 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line8,290 - 250 + 30, 159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);


    m_checkbox_line9 = new QCheckBox(this);
    m_checkbox_line9->setFixedSize(70,70);
    m_checkbox_line9->move(688 - 290 - 20 + 170,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 7 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line9->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line9->setFocusPolicy(Qt::NoFocus);

    str_sheet = BUTTON_CONFIG_SHEET;
    m_pushbutton_default = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_default,808 - 290 + 42,159 - MAIN_HEAD_HEIGHT - 50 - 11 + ROW_HEIGHT * 8 + 25,100,60,str_sheet);
    m_pushbutton_default->setText(tr("默认"));

    connect(m_pushbutton_default,SIGNAL(clicked()),this,SLOT(slot_pushbutton_default()));

}
void CPonCamWidget::set_type(char _type)
{
    switch(_type)
    {
        case kEnumPon:
        {
            pon_init();

            m_number = 1;
            CSystemPonInfo *pon_info = (CSystemPonInfo *)read_data();//由当前界面读到当前pon(n)再进行复制

            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon1);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon2);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon3);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon4);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon5);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon6);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon7);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon8);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon9);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon10);

        }break;
        case kEnumCam:
        {
            cam_init();
            m_number = 1;
            CSystemCamInfo *pon_info = (CSystemCamInfo *)read_data();//由当前界面读到当前cam(n)再进行复制

            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam1);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam2);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam3);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam4);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam5);
            copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam6);

        }break;
    }
}

void CPonCamWidget::modify_title(const QString &_title)
{
    m_label_line1->setText(_title);
}

void CPonCamWidget::slot_pon_widet_combobox_change(int _value)
{
    do
    {
        if(m_auto_change)
            break;

        if(m_number == _value)
        {
            break;
        }



        CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("确认保存?"),CLtyString::getInstance()->m_save,CLtyString::getInstance()->m_cancel);
        CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerCancel);
        CMyMessageBox::getInstance()->exec();

        if(CMyMessageBox::KEnumButtonOk ==  CMyMessageBox::getInstance()->get_result())
        {
             CSystemPonInfo *pon_info = (CSystemPonInfo *)read_data();//由当前界面读到当前pon(n)再进行复制
             if(0 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon1);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon2);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon3);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon4);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon5);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon6);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon7);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon8);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon9);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon10);
             }
             else if(1 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon1);
             }
             else if(2 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon2);
             }
             else if(3 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon3);
             }
             else if(4 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon4);
             }
             else if(5 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon5);
             }
             else if(6 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon6);
             }
             else if(7 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon7);
             }
             else if(8 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon8);
             }
             else if(9 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon9);
             }
             else if(10 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.pon10);
             }
             CProtocol::getInstance()->call_json_system_pon_cam(CProtocol::kEnumSave);
             CProtocol::getInstance()->call_json_system_pon_cam(CProtocol::kEnumGet);
        }

    }while(0);

}

void CPonCamWidget::slot_pushbutton_default()
{
    CProtocol::getInstance()->call_json_system_pon_cam(CProtocol::kEnumDefault,m_type,m_number);
}

void CPonCamWidget::slot_pushbutton_search()
{
    CProtocol::getInstance()->call_json_system_pon_cam(CProtocol::kEnumDefault,m_type,m_number);
}

void CPonCamWidget::slot_cam_widet_combobox_change(int _value)
{
    do
    {
       if(m_auto_change)
            break;

        if(m_number == _value)
        {
            break;
        }



        CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("确认保存?"),CLtyString::getInstance()->m_save,CLtyString::getInstance()->m_cancel);
        CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerCancel);
        CMyMessageBox::getInstance()->exec();

        if(CMyMessageBox::KEnumButtonOk ==  CMyMessageBox::getInstance()->get_result())
        {
             CSystemCamInfo *pon_info = (CSystemCamInfo *)read_data();//由当前界面读到当前pon(n)再进行复制
             if(0 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam1);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam2);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam3);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam4);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam5);
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam6);

             }
             else if(1 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam1);
             }
             else if(2 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam2);
             }
             else if(3 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam3);
             }
             else if(4 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam4);
             }
             else if(5 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam5);
             }
             else if(6 == _value)
             {
                copy_data(pon_info,&CCommonData::getInstance()->m_CSystemCameraConfig.cam6);
             }

            CProtocol::getInstance()->call_json_system_pon_cam(CProtocol::kEnumSave);
            CProtocol::getInstance()->call_json_system_pon_cam(CProtocol::kEnumGet);
        }

    }while(0);
}


void CPonCamWidget::copy_data(CSystemPonInfo *_src,CSystemPonInfo *_dst)
{
    _dst->config_sound_recording = _src->config_sound_recording;//录音  0:开启　　1:关闭
    _dst->resolving_power_main = _src->resolving_power_main;  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    _dst->resolving_power_sub = _src->resolving_power_sub;   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    _dst->frame_rate_main = _src->frame_rate_main;       //帧率 主 0~24 对应值是 1~25
    _dst->frame_rate_sub = _src->frame_rate_sub;        //帧率 子 0~24 对应值是 1~25
    _dst->picture_main = _src->picture_main;          //画质　主 0:高 1:中 2:低
    _dst->picture_sub = _src->picture_sub;           //画质　子 0:高 1:中 2:低
    _dst->identification = _src->identification;        //标识 0:IPC摄像机 1:其他设备
    _dst->m_image = _src->m_image;
}

void CPonCamWidget::copy_data(CSystemCamInfo *_src,CSystemCamInfo *_dst)
{
    _dst->config_sound_recording = _src->config_sound_recording;//录音  0:开启　　1:关闭
    _dst->resolving_power_main = _src->resolving_power_main;  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    _dst->resolving_power_sub = _src->resolving_power_sub;   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    _dst->frame_rate_main = _src->frame_rate_main;       //帧率 主 0~24 对应值是 1~25
    _dst->frame_rate_sub = _src->frame_rate_sub;        //帧率 子 0~24 对应值是 1~25
    _dst->picture_main = _src->picture_main;          //画质　主 0:高 1:中 2:低
    _dst->picture_sub = _src->picture_sub;           //画质　子 0:高 1:中 2:低
    _dst->identification = _src->identification;        //标识 0:视频录像 1:客流录像
    _dst->m_image = _src->m_image;
}

void CPonCamWidget::clean_copy_sheet_list()
{
    m_combobox_line8->clear();
}

void CPonCamWidget::add_copy_sheet_list(const QString &_text)
{
    m_combobox_line8->addItem(_text);
}

void *CPonCamWidget::read_data()
{
    if(kEnumPon == m_type)
    {
        CSystemPonInfo *pTemp = NULL;
        CSystemPonInfo *pon_info = new CSystemPonInfo;
        if(1 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon1;
        else if(2 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon2;
        else if(3 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon3;
        else if(4 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon4;
        else if(5 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon5;
        else if(6 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon6;
        else if(7 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon7;
        else if(8 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon8;
        else if(9 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon9;
        else if(10 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon10;

        pon_info->config_sound_recording = m_combobox_line3->currentIndex();//录音  0:开启　　1:关闭
        pon_info->resolving_power_main = m_combobox_line4_1->currentIndex();  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        pon_info->resolving_power_sub = m_combobox_line4_2->currentIndex();   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        pon_info->frame_rate_main = m_combobox_line5_1->currentIndex();       //帧率 主 0~24 对应值是 1~25
        pon_info->frame_rate_sub = m_combobox_line5_2->currentIndex();        //帧率 子 0~24 对应值是 1~25
        pon_info->picture_main = m_combobox_line6_1->currentIndex();          //画质　主 0:高 1:中 2:低
        pon_info->picture_sub = m_combobox_line6_2->currentIndex();           //画质　子 0:高 1:中 2:低
        pon_info->identification = m_combobox_line7->currentIndex();        //标识 0:IPC摄像机 1:其他设备
        pon_info->m_image = (m_checkbox_line9->checkState() == Qt::Checked)? 1:0;

        pTemp->config_sound_recording = pon_info->config_sound_recording;//录音  0:开启　　1:关闭
        pTemp->resolving_power_main = pon_info->resolving_power_main;  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        pTemp->resolving_power_sub = pon_info->resolving_power_sub;   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        pTemp->frame_rate_main = pon_info->frame_rate_main;       //帧率 主 0~24 对应值是 1~25
        pTemp->frame_rate_sub = pon_info->frame_rate_sub;        //帧率 子 0~24 对应值是 1~25
        pTemp->picture_main = pon_info->picture_main;          //画质　主 0:高 1:中 2:低
        pTemp->picture_sub = pon_info->picture_sub;           //画质　子 0:高 1:中 2:低
        pTemp->identification = pon_info->identification;        //标识 0:IPC摄像机 1:其他设备
        pTemp->m_image = pon_info->m_image;        //标识 0:IPC摄像机 1:其他设备

        delete pon_info;
        return  pTemp;
    }
    else
    {
        CSystemCamInfo *pTemp = NULL;
        CSystemCamInfo *cam_info = new CSystemCamInfo;

        if(1 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam1;
        else if(2 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam2;
        else if(3 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam3;
        else if(4 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam4;
        else if(5 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam5;
        else if(6 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam6;


        cam_info->config_sound_recording = m_combobox_line3->currentIndex();//录音  0:开启　　1:关闭
        cam_info->resolving_power_main = m_combobox_line4_1->currentIndex();  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        cam_info->resolving_power_sub = m_combobox_line4_2->currentIndex();   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        cam_info->frame_rate_main = m_combobox_line5_1->currentIndex();       //帧率 主 0~24 对应值是 1~25
        cam_info->frame_rate_sub = m_combobox_line5_2->currentIndex();        //帧率 子 0~24 对应值是 1~25
        cam_info->picture_main = m_combobox_line6_1->currentIndex();          //画质　主 0:高 1:中 2:低
        cam_info->picture_sub = m_combobox_line6_2->currentIndex();           //画质　子 0:高 1:中 2:低
        cam_info->identification = m_combobox_line7->currentIndex();        //标识 0:视频录像 1:客流录像
        cam_info->m_image = (m_checkbox_line9->checkState() == Qt::Checked)? 1:0;

        pTemp->config_sound_recording = cam_info->config_sound_recording;//录音  0:开启　　1:关闭
        pTemp->resolving_power_main = cam_info->resolving_power_main;  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        pTemp->resolving_power_sub = cam_info->resolving_power_sub;   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        pTemp->frame_rate_main = cam_info->frame_rate_main;       //帧率 主 0~24 对应值是 1~25
        pTemp->frame_rate_sub = cam_info->frame_rate_sub;        //帧率 子 0~24 对应值是 1~25
        pTemp->picture_main = cam_info->picture_main;          //画质　主 0:高 1:中 2:低
        pTemp->picture_sub = cam_info->picture_sub;           //画质　子 0:高 1:中 2:低
        pTemp->identification = cam_info->identification;        //标识 0:视频录像 1:客流录像
        pTemp->m_image = cam_info->m_image;

        delete cam_info;

        return  pTemp;
    }
}
void CPonCamWidget::save_data()
{

    if(kEnumPon == m_type)
    {
        CSystemPonInfo *pTemp = NULL;
        CSystemPonInfo *pon_info = new CSystemPonInfo;
        char ipc_index = 0;
        ipc_index = m_number;
        if(1 == ipc_index)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon1;
        else if(2 == ipc_index)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon2;
        else if(3 == ipc_index)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon3;
        else if(4 == ipc_index)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon4;
        else if(5 == ipc_index)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon5;
        else if(6 == ipc_index)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon6;
        else if(7 == ipc_index)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon7;
        else if(8 == ipc_index)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon8;
        else if(9 == ipc_index)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon9;
        else if(10 == ipc_index)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon10;

        pon_info->config_sound_recording = m_combobox_line3->currentIndex();//录音  0:开启　　1:关闭
        pon_info->resolving_power_main = m_combobox_line4_1->currentIndex();  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        pon_info->resolving_power_sub = m_combobox_line4_2->currentIndex();   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        pon_info->frame_rate_main = m_combobox_line5_1->currentIndex();       //帧率 主 0~24 对应值是 1~25
        pon_info->frame_rate_sub = m_combobox_line5_2->currentIndex();        //帧率 子 0~24 对应值是 1~25
        pon_info->picture_main = m_combobox_line6_1->currentIndex();          //画质　主 0:高 1:中 2:低
        pon_info->picture_sub = m_combobox_line6_2->currentIndex();           //画质　子 0:高 1:中 2:低
        pon_info->identification = m_combobox_line7->currentIndex();        //标识 0:IPC摄像机 1:其他设备
        pon_info->m_image = (m_checkbox_line9->checkState() == Qt::Checked)? 1:0;

        if(pon_info->config_sound_recording != pTemp->config_sound_recording ||
            pon_info->resolving_power_main != pTemp->resolving_power_main ||
            pon_info->resolving_power_sub != pTemp->resolving_power_sub ||
            pon_info->frame_rate_main != pTemp->frame_rate_main ||
            pon_info->frame_rate_sub != pTemp->frame_rate_sub ||
            pon_info->picture_main != pTemp->picture_main ||
            pon_info->picture_sub != pTemp->picture_sub ||
            pon_info->identification != pTemp->identification ||
            pon_info->m_image != pTemp->m_image
         )
        {
            if(CCommonData::getInstance()->messagebox_data_change())
            {
                emit signal_notify_save_data();
                read_data();
                CProtocol::getInstance()->call_json_system_pon_cam(CProtocol::kEnumSave);
                CProtocol::getInstance()->call_json_system_pon_cam(CProtocol::kEnumGet);
            }
            else
            {
                #if 0
                pTemp->config_sound_recording = pon_info->config_sound_recording;//录音  0:开启　　1:关闭
                pTemp->resolving_power_main = pon_info->resolving_power_main;  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
                pTemp->resolving_power_sub = pon_info->resolving_power_sub;   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
                pTemp->frame_rate_main = pon_info->frame_rate_main;       //帧率 主 0~24 对应值是 1~25
                pTemp->frame_rate_sub = pon_info->frame_rate_sub;        //帧率 子 0~24 对应值是 1~25
                pTemp->picture_main = pon_info->picture_main;          //画质　主 0:高 1:中 2:低
                pTemp->picture_sub = pon_info->picture_sub;           //画质　子 0:高 1:中 2:低
                pTemp->identification = pon_info->identification;        //标识 0:IPC摄像机 1:其他设备
                #endif
               //slot_camera_config_widget_event(CCommonData::getInstance()->m_CSystemCameraConfig);
            }
        }
        delete pon_info;

    }
    else
    {
        CSystemCamInfo *pTemp = NULL;
        CSystemCamInfo *cam_info = new CSystemCamInfo;

        if(1 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam1;
        else if(2 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam2;
        else if(3 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam3;
        else if(4 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam4;
        else if(5 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam5;
        else if(6 == m_number)
            pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.cam6;


        cam_info->config_sound_recording = m_combobox_line3->currentIndex();//录音  0:开启　　1:关闭
        cam_info->resolving_power_main = m_combobox_line4_1->currentIndex();  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        cam_info->resolving_power_sub = m_combobox_line4_2->currentIndex();   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
        cam_info->frame_rate_main = m_combobox_line5_1->currentIndex();       //帧率 主 0~24 对应值是 1~25
        cam_info->frame_rate_sub = m_combobox_line5_2->currentIndex();        //帧率 子 0~24 对应值是 1~25
        cam_info->picture_main = m_combobox_line6_1->currentIndex();          //画质　主 0:高 1:中 2:低
        cam_info->picture_sub = m_combobox_line6_2->currentIndex();           //画质　子 0:高 1:中 2:低
        cam_info->identification = m_combobox_line7->currentIndex();        //标识 0:视频录像 1:客流录像
        cam_info->m_image = (m_checkbox_line9->checkState() == Qt::Checked)? 1:0;

        if(cam_info->config_sound_recording != pTemp->config_sound_recording ||
            cam_info->resolving_power_main != pTemp->resolving_power_main ||
            cam_info->resolving_power_sub != pTemp->resolving_power_sub ||
            cam_info->frame_rate_main != pTemp->frame_rate_main ||
            cam_info->frame_rate_sub != pTemp->frame_rate_sub ||
            cam_info->picture_main != pTemp->picture_main ||
            cam_info->picture_sub != pTemp->picture_sub ||
            cam_info->identification != pTemp->identification ||
            cam_info->m_image != pTemp->m_image
        )
        {
            if(CCommonData::getInstance()->messagebox_data_change())
            {
                read_data();
                CProtocol::getInstance()->call_json_system_pon_cam(CProtocol::kEnumSave);
                CProtocol::getInstance()->call_json_system_pon_cam(CProtocol::kEnumGet);
            }
            else
            {
                #if 0
                pTemp->config_sound_recording = cam_info->config_sound_recording;
                pTemp->resolving_power_main = cam_info->resolving_power_main;
                pTemp->resolving_power_sub = cam_info->resolving_power_sub;
                pTemp->frame_rate_main = cam_info->frame_rate_main;
                pTemp->frame_rate_sub = cam_info->frame_rate_sub;
                pTemp->picture_main = cam_info->picture_main;
                pTemp->picture_sub = cam_info->picture_sub;
                pTemp->identification = cam_info->identification;
                #endif
               //slot_camera_config_widget_event(CCommonData::getInstance()->m_CSystemCameraConfig);
            }
        }
        delete cam_info;
    }


}



void CPonCamWidget::update_data(const CSystemPonInfo &_info,char _number)
{
    //PON通道-配置
    m_lineedit_line2->setText(_info.config_ip);             //IP地址
    m_combobox_line3->setCurrentIndex(_info.config_sound_recording);//录音  0:开启　　1:关闭
    m_combobox_line4_1->setCurrentIndex(_info.resolving_power_main);  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    m_combobox_line4_2->setCurrentIndex(_info.resolving_power_sub);   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    m_combobox_line5_1->setCurrentIndex(_info.frame_rate_main);       //帧率 主 0~24 对应值是 1~25
    m_combobox_line5_2->setCurrentIndex(_info.frame_rate_sub);        //帧率 子 0~24 对应值是 1~25
    m_combobox_line6_1->setCurrentIndex(_info.picture_main);          //画质　主 0:高 1:中 2:低
    m_combobox_line6_2->setCurrentIndex(_info.picture_sub);           //画质　子 0:高 1:中 2:低
    m_combobox_line7->setCurrentIndex(_info.identification);        //标识 0:IPC摄像机 1:其他设备

    m_number = _number;
    m_combobox_line8->setCurrentIndex(_number);

}

void CPonCamWidget::update_data(const CSystemCamInfo &_info,char _number)
{//
    m_combobox_line3->setCurrentIndex(_info.config_sound_recording);//录音  0:开启　　1:关闭
    m_combobox_line4_1->setCurrentIndex(_info.resolving_power_main);  //分辨率　主 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    m_combobox_line4_2->setCurrentIndex(_info.resolving_power_sub);   //分辨率　子 0:1080P  1:720P 2:D1  3:CIF  4:QCIF
    m_combobox_line5_1->setCurrentIndex(_info.frame_rate_main);       //帧率 主 0~24 对应值是 1~25
    m_combobox_line5_2->setCurrentIndex(_info.frame_rate_sub);        //帧率 子 0~24 对应值是 1~25
    m_combobox_line6_1->setCurrentIndex(_info.picture_main);          //画质　主 0:高 1:中 2:低
    m_combobox_line6_2->setCurrentIndex(_info.picture_sub);           //画质　子 0:高 1:中 2:低
    m_combobox_line7->setCurrentIndex(_info.identification);        //标识 0:视频录像 1:客流录像

    m_number = _number;
    m_combobox_line8->setCurrentIndex(_number);

}

CCameraConfigWidget::CCameraConfigWidget(QWidget *parent) :
    QWidget(parent)
{
        //this->setObjectName("CCameraConfigWidget");
    this->setStyleSheet("QWidget{background-color:#000000;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->move(0,0);

        QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    pWidget->move(0,0);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");

    scroll_layout();
    widget_init();

    read_data();


}

void CCameraConfigWidget::scroll_layout()
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

    m_widget_submain_1 = new QWidget;
    m_widget_submain_1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_submain_1->setStyleSheet("QWidget{background: transparent;}");
    m_widget_submain_1->hide();

    m_widget_submain_2 = new QWidget;
    m_widget_submain_2->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 300);
    m_widget_submain_2->setStyleSheet("QWidget{background: transparent;}");
    m_widget_submain_2->hide();

    m_widget_submain_3 = new QWidget;
    m_widget_submain_3->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    m_widget_submain_3->setStyleSheet("QWidget{background: transparent;}");
    m_widget_submain_3->hide();


}

void CCameraConfigWidget::slot_timer_init()
{
    m_widget_submain_1->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 400);
}

void CCameraConfigWidget::widget_init()
{
    m_button_1 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_1->setDisplayText(tr("通用设置"));

    m_button_1->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_1->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_1->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    //m_button_1->move(290 - 250,158 - MAIN_HEAD_HEIGHT);

    m_button_2 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_2->setDisplayText(tr("显示设置"));

    m_button_2->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_2->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_2->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_3 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_3->setDisplayText(tr("通道设置"));

    m_button_3->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_3->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_3->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_4 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_4->setDisplayText(tr("模拟摄像机通道设置"));

    m_button_4->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_4->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_4->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_4->hide();

    m_CBaseConfigWidget = new CBaseConfigWidget(m_widget_main);
    m_CShowConfigWidget = new CShowConfigWidget(m_widget_main);
    m_CPassagewayConfigWidget = new CPassagewayConfigWidget(m_widget_main);


    connect(m_CBaseConfigWidget,SIGNAL(signal_pushbutton_config()),this,SLOT(slot_pushbutton_image_set()));
#if 0
    connect(m_CPassagewayConfigWidget,SIGNAL(signal_pushbutton_line1_1()),this,SLOT(slot_pushbutton_config1()));
    connect(m_CPassagewayConfigWidget,SIGNAL(signal_pushbutton_line2_1()),this,SLOT(slot_pushbutton_config2()));
    connect(m_CPassagewayConfigWidget,SIGNAL(signal_pushbutton_line3_1()),this,SLOT(slot_pushbutton_config3()));
    connect(m_CPassagewayConfigWidget,SIGNAL(signal_pushbutton_line4_1()),this,SLOT(slot_pushbutton_config4()));
    connect(m_CPassagewayConfigWidget,SIGNAL(signal_pushbutton_line5_1()),this,SLOT(slot_pushbutton_config5()));

    connect(m_CPassagewayConfigWidget,SIGNAL(signal_pushbutton_line6_1()),this,SLOT(slot_pushbutton_config6()));
    connect(m_CPassagewayConfigWidget,SIGNAL(signal_pushbutton_line7_1()),this,SLOT(slot_pushbutton_config7()));
    connect(m_CPassagewayConfigWidget,SIGNAL(signal_pushbutton_line8_1()),this,SLOT(slot_pushbutton_config8()));
    connect(m_CPassagewayConfigWidget,SIGNAL(signal_pushbutton_line9_1()),this,SLOT(slot_pushbutton_config9()));
    connect(m_CPassagewayConfigWidget,SIGNAL(signal_pushbutton_line10_1()),this,SLOT(slot_pushbutton_config10()));
#endif

    connect(m_CPassagewayConfigWidget,SIGNAL(signal_button_data(int,int,int,int)),this,SLOT(slot_button_config_data(int,int,int,int)));

    //slot_current_passageway
    connect(m_CPassagewayConfigWidget,SIGNAL(signal_current_passageway(int,int,int)),this,SLOT(slot_current_passageway(int,int,int)));


    m_label_space1  = new QLabel(m_widget_main);
    m_label_space1->setFixedHeight(10);
    m_label_space1->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space2  = new QLabel(m_widget_main);
    m_label_space2->setFixedHeight(10);
    m_label_space2->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space3  = new QLabel(m_widget_main);
    m_label_space3->setFixedHeight(10);
    m_label_space3->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_vboxlayout_storage = new QVBoxLayout(m_widget_main);//左 上 右 下
    m_vboxlayout_storage->setContentsMargins(290 - 250,158 - MAIN_HEAD_HEIGHT - 50,0,0);
    m_vboxlayout_storage->setSpacing(0);
    m_vboxlayout_storage->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_vboxlayout_storage->addWidget(m_button_1);
    m_vboxlayout_storage->addWidget(m_CBaseConfigWidget);
    m_vboxlayout_storage->addWidget(m_label_space1);
    m_vboxlayout_storage->addWidget(m_button_2);
    m_vboxlayout_storage->addWidget(m_CShowConfigWidget);
    m_vboxlayout_storage->addWidget(m_label_space2);
    m_vboxlayout_storage->addWidget(m_button_3);
    m_vboxlayout_storage->addWidget(m_CPassagewayConfigWidget);
    m_vboxlayout_storage->addWidget(m_label_space3);


    connect(m_button_1,SIGNAL(clicked()),this,SLOT(slot_button_1()));
    connect(m_button_2,SIGNAL(clicked()),this,SLOT(slot_button_2()));
    connect(m_button_3,SIGNAL(clicked()),this,SLOT(slot_button_3()));
    connect(m_button_4,SIGNAL(clicked()),this,SLOT(slot_button_4()));

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_CBaseConfigWidget->hide();
    m_CShowConfigWidget->hide();
    m_CPassagewayConfigWidget->hide();


    m_pon_widget = new CPonCamWidget(CPonCamWidget::kEnumPon,m_widget_submain_1);
    m_pon_widget->move(0,0);
    m_cam_widget = new CPonCamWidget(CPonCamWidget::kEnumCam,m_widget_submain_2);
    m_cam_widget->move(0,0);

    m_image_set_widget = new CImageSetWidget(m_widget_submain_3);
    m_image_set_widget->move(0,0);

    connect(m_pon_widget,SIGNAL(signal_notify_save_data()),this,SLOT(slot_save_data()));
    connect(m_cam_widget,SIGNAL(signal_notify_save_data()),this,SLOT(slot_save_data()));

}

void CCameraConfigWidget::slot_button_1()
{
    if(m_button_press_1)
    {
        m_button_press_1 = false;
        m_button_1->setSelected(false);
        m_CBaseConfigWidget->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_1 = true;
        m_button_1->setSelected(true);
        m_CBaseConfigWidget->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 400 + ROW_HEIGHT * 1);
    }
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_CShowConfigWidget->hide();
    m_CPassagewayConfigWidget->hide();


}
void CCameraConfigWidget::slot_button_2()
{
    if(m_button_press_2)
    {
        m_button_press_2 = false;
        m_button_2->setSelected(false);
        m_CShowConfigWidget->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_2 = true;
        m_button_2->setSelected(true);
        m_CShowConfigWidget->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 300 + ROW_HEIGHT * 1);
    }
    m_button_1->setSelected(false);
    m_button_3->setSelected(false);
    m_button_4->setSelected(false);
    m_button_press_1 = false;
    m_button_press_3 = false;
    m_button_press_4 = false;
    m_CBaseConfigWidget->hide();
    m_CPassagewayConfigWidget->hide();

}
void CCameraConfigWidget::slot_button_3()
{
    if(m_button_press_3)
    {
        m_button_press_3 = false;
        m_button_3->setSelected(false);
        m_CPassagewayConfigWidget->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_3 = true;
        m_button_3->setSelected(true);
        m_CPassagewayConfigWidget->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + ROW_HEIGHT *
        (CCommonData::getInstance()->m_CSystemCameraConfig.monitor_passageway_number +
        CCommonData::getInstance()->m_CSystemCameraConfig.network_passageway_number));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_4->setSelected(false);
    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_4 = false;
    m_CBaseConfigWidget->hide();
    m_CShowConfigWidget->hide();

}

void CCameraConfigWidget::slot_button_4()
{
    if(m_button_press_4)
    {
        m_button_press_4 = false;
        m_button_4->setSelected(false);
        //m_CPassagewayConfigWidget1->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_4 = true;
        m_button_4->setSelected(true);
        //m_CPassagewayConfigWidget1->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + ROW_HEIGHT *8);
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_CBaseConfigWidget->hide();
    m_CShowConfigWidget->hide();
    m_CPassagewayConfigWidget->hide();
}

void CCameraConfigWidget::slot_pushbutton_return()
{
    if(m_widget_submain_1->isVisible())
    {
        m_widget_main->show();
        m_widget_submain_1->hide();
        m_vboxlayout_widget->removeWidget(m_widget_submain_1);
        m_vboxlayout_widget->addWidget(m_widget_main);
        m_pon_widget->save_data();
    }
    else if(m_widget_submain_2->isVisible())
    {
        m_widget_main->show();
        m_widget_submain_2->hide();
        m_vboxlayout_widget->removeWidget(m_widget_submain_2);
        m_vboxlayout_widget->addWidget(m_widget_main);
        m_cam_widget->save_data();
    }
    else if(m_widget_submain_3->isVisible())
    {
        m_widget_main->show();
        m_widget_submain_3->hide();
        m_vboxlayout_widget->removeWidget(m_widget_submain_3);
        m_vboxlayout_widget->addWidget(m_widget_main);

    }
    else
    {
        save_data();
        emit signal_window_close();
    }
}

void CCameraConfigWidget::show_pon(const CSystemPonInfo &_info,const QString &_title,char _number)
{
    static bool first_init = false;
    m_pon_widget->modify_title(_title + tr("配置"));
    m_pon_widget->update_data(_info,_number);
    m_widget_main->hide();
    m_widget_submain_1->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_submain_1);

    if(!first_init)
    {
        first_init = true;
        QTimer::singleShot(10,this,SLOT(slot_timer_init()));
    }
}
void CCameraConfigWidget::show_cam(const CSystemCamInfo &_info,const QString &_title,char _number)
{
    m_cam_widget->modify_title(_title + tr("配置"));
    m_cam_widget->update_data(_info,_number);
    m_widget_main->hide();
    m_widget_submain_2->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_submain_2);
}

void CCameraConfigWidget::slot_pushbutton_image_set()
{
    m_widget_main->hide();
    m_widget_submain_3->show();
    m_vboxlayout_widget->removeWidget(m_widget_main);
    m_vboxlayout_widget->addWidget(m_widget_submain_3);
}

void CCameraConfigWidget::slot_button_config_data(int _type,int _number,int _cam_number,int _ipc_number)
{
    int ipc_index = 0;
    m_type = _type;
    m_number = _number;
    m_cam_number = _cam_number;
    m_ipc_number = _ipc_number;
    if(CCamLineWidget::kEnumSimulation == _type)
    {
        if(1 == _number)
            show_cam(CCommonData::getInstance()->m_CSystemCameraConfig.cam1,"PON-通道1",1);
        if(2 == _number)
            show_cam(CCommonData::getInstance()->m_CSystemCameraConfig.cam2,"PON-通道2",2);
        if(3 == _number)
            show_cam(CCommonData::getInstance()->m_CSystemCameraConfig.cam3,"PON-通道3",3);
        if(4 == _number)
            show_cam(CCommonData::getInstance()->m_CSystemCameraConfig.cam4,"PON-通道4",4);
        if(5 == _number)
            show_cam(CCommonData::getInstance()->m_CSystemCameraConfig.cam5,"PON-通道5",5);
        if(6 == _number)
            show_cam(CCommonData::getInstance()->m_CSystemCameraConfig.cam6,"PON-通道6",6);
    }
    else
    {
        QString str_title = QString("PON-通道%1").arg(_number);

        ipc_index = _number - _cam_number;

         if(1 == ipc_index)
            show_pon(CCommonData::getInstance()->m_CSystemCameraConfig.pon1,str_title,1);
         if(2 == ipc_index)
            show_pon(CCommonData::getInstance()->m_CSystemCameraConfig.pon2,str_title,2);
         if(3 == ipc_index)
            show_pon(CCommonData::getInstance()->m_CSystemCameraConfig.pon3,str_title,3);
         if(4 == ipc_index)
            show_pon(CCommonData::getInstance()->m_CSystemCameraConfig.pon4,str_title,4);
         if(5 == ipc_index)
            show_pon(CCommonData::getInstance()->m_CSystemCameraConfig.pon5,str_title,5);
         if(6 == ipc_index)
            show_pon(CCommonData::getInstance()->m_CSystemCameraConfig.pon6,str_title,6);
         if(7 == ipc_index)
            show_pon(CCommonData::getInstance()->m_CSystemCameraConfig.pon7,str_title,7);
         if(8 == ipc_index)
            show_pon(CCommonData::getInstance()->m_CSystemCameraConfig.pon8,str_title,8);
         if(9 == ipc_index)
            show_pon(CCommonData::getInstance()->m_CSystemCameraConfig.pon9,str_title,9);
         if(10 == ipc_index)
            show_pon(CCommonData::getInstance()->m_CSystemCameraConfig.pon10,str_title,10);
    }
}

void CCameraConfigWidget::save_data(bool _dialog_tip)
{
    CSystemCameraConfig info;

    //通用设置---------------
    info.videotape_standard = m_CBaseConfigWidget->m_combobox_line1->currentIndex();//录像制式  0:PAL 1:NTSC
    info.auto_cover = m_CBaseConfigWidget->m_combobox_line2->currentIndex();//自动覆盖 0:是  1:否
    info.videotape_lock = m_CBaseConfigWidget->m_combobox_line3->currentIndex();//录像锁定　0:1 1:2 2:3 3:4 4:5 5:6 6:7
    info.videotape_prev = m_CBaseConfigWidget->m_lineedit_line4->text().toInt();//录像预录    min
    info.network_transmission_mode = m_CBaseConfigWidget->m_combobox_line5->currentIndex();//网传模式  0:流畅+清晰 1:非常清晰 2:清晰 3:流畅 4:非常流畅
    info.videotape_storage = m_CBaseConfigWidget->m_combobox_line6->currentIndex();//录像存储器 0:EMMC+外置SD卡 1:EMMC  2:外置SD卡
    info.videotape_mode = m_CBaseConfigWidget->m_combobox_line7->currentIndex();//录像模式  0:镜像录像  1:报警备份录像 2:子码流录像
    //配置－镜像通道
    info.m_passageway_list.clear();
     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line2_1->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line2_2->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line2_3->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line2_4->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line3_1->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line3_2->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line3_3->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line3_4->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line4_1->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line4_2->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line4_3->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line4_4->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line5_1->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line5_2->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line5_3->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line5_4->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line6_1->checkState() == Qt::Checked)? 1:0);

     info.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line6_2->checkState() == Qt::Checked)? 1:0);

    //

    //显示设置---------------

    info.time_show_enable = (m_CShowConfigWidget->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//时间显示使能
    info.time_show_value = m_CShowConfigWidget->m_combobox_line1->currentIndex();//时间显示值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    info.passageway_name_enable = (m_CShowConfigWidget->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;//通道名称使能
    info.passageway_name_value = m_CShowConfigWidget->m_combobox_line2->currentIndex();//通道名称值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    info.vehicle_number_enable = (m_CShowConfigWidget->m_checkbox_line3->checkState() == Qt::Checked)? 1:0;//车辆编号使能
    info.vehicle_number_value = m_CShowConfigWidget->m_combobox_line3->currentIndex();//车辆编号值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    info.coordinate_info_enable = (m_CShowConfigWidget->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//坐标信息使能
    info.coordinate_info_value = m_CShowConfigWidget->m_combobox_line4->currentIndex();//车辆编号值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    info.real_speed_enable = (m_CShowConfigWidget->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;//实时速度使能
    info.real_speed_value = m_CShowConfigWidget->m_combobox_line5->currentIndex();//实时速度值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    info.station_info_enable = (m_CShowConfigWidget->m_checkbox_line6->checkState() == Qt::Checked)? 1:0;//站点信息使能
    info.station_info_value = m_CShowConfigWidget->m_combobox_line6->currentIndex();//站点信息值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    info.monitor_passageway_number = m_CPassagewayConfigWidget->m_combobox_line0->currentIndex();//模拟数量
    info.network_passageway_number = m_CPassagewayConfigWidget->m_combobox_line0_1->currentIndex();//IPC数量


    if( info.videotape_standard != CCommonData::getInstance()->m_CSystemCameraConfig.videotape_standard ||
    info.auto_cover != CCommonData::getInstance()->m_CSystemCameraConfig.auto_cover ||
    info.videotape_lock != CCommonData::getInstance()->m_CSystemCameraConfig.videotape_lock ||
    info.videotape_prev != CCommonData::getInstance()->m_CSystemCameraConfig.videotape_prev ||
    info.network_transmission_mode != CCommonData::getInstance()->m_CSystemCameraConfig.network_transmission_mode ||
    info.videotape_storage != CCommonData::getInstance()->m_CSystemCameraConfig.videotape_storage ||
    info.videotape_mode != CCommonData::getInstance()->m_CSystemCameraConfig.videotape_mode ||
     info.m_passageway_list.at(0) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(0) ||
     info.m_passageway_list.at(1) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(1) ||
     info.m_passageway_list.at(2) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(2) ||
     info.m_passageway_list.at(3) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(3) ||
     info.m_passageway_list.at(4) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(4) ||
     info.m_passageway_list.at(5) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(5) ||
     info.m_passageway_list.at(6) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(6) ||
     info.m_passageway_list.at(7) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(7) ||
     info.m_passageway_list.at(8) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(8) ||
     info.m_passageway_list.at(9) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(9) ||
     info.m_passageway_list.at(10) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(10) ||
     info.m_passageway_list.at(11) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(11) ||
     info.m_passageway_list.at(12) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(12) ||
     info.m_passageway_list.at(13) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(13) ||
     info.m_passageway_list.at(14) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(14) ||
     info.m_passageway_list.at(15) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(15) ||
     info.m_passageway_list.at(16) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(16) ||
     info.m_passageway_list.at(17) != CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.at(17) ||
    info.time_show_enable != CCommonData::getInstance()->m_CSystemCameraConfig.time_show_enable ||
    info.time_show_value != CCommonData::getInstance()->m_CSystemCameraConfig.time_show_value ||
    info.passageway_name_enable != CCommonData::getInstance()->m_CSystemCameraConfig.passageway_name_enable ||
    info.passageway_name_value != CCommonData::getInstance()->m_CSystemCameraConfig.passageway_name_value ||
    info.vehicle_number_enable != CCommonData::getInstance()->m_CSystemCameraConfig.vehicle_number_enable ||
    info.vehicle_number_value != CCommonData::getInstance()->m_CSystemCameraConfig.vehicle_number_value ||
    info.coordinate_info_enable != CCommonData::getInstance()->m_CSystemCameraConfig.coordinate_info_enable ||
    info.coordinate_info_value != CCommonData::getInstance()->m_CSystemCameraConfig.coordinate_info_value ||
    info.real_speed_enable != CCommonData::getInstance()->m_CSystemCameraConfig.real_speed_enable ||
    info.real_speed_value != CCommonData::getInstance()->m_CSystemCameraConfig.real_speed_value ||
    info.station_info_enable != CCommonData::getInstance()->m_CSystemCameraConfig.station_info_enable ||
    info.station_info_value != CCommonData::getInstance()->m_CSystemCameraConfig.station_info_value ||
    info.monitor_passageway_number != CCommonData::getInstance()->m_CSystemCameraConfig.monitor_passageway_number ||
    info.network_passageway_number != CCommonData::getInstance()->m_CSystemCameraConfig.network_passageway_number
    )
    {
        if(!_dialog_tip)
        {
            read_data();
            CProtocol::getInstance()->call_json_system_camera_config(CProtocol::kEnumSave);
        }
        else
        {
            if(CCommonData::getInstance()->messagebox_data_change())
            {
                read_data();
                CProtocol::getInstance()->call_json_system_camera_config(CProtocol::kEnumSave);
            }
            else
            {
               slot_camera_config_widget_event(CCommonData::getInstance()->m_CSystemCameraConfig);
            }
        }

    }


}
void CCameraConfigWidget::read_data()
{
    //通用设置---------------
    CCommonData::getInstance()->m_CSystemCameraConfig.videotape_standard = m_CBaseConfigWidget->m_combobox_line1->currentIndex();//录像制式  0:PAL 1:NTSC
    CCommonData::getInstance()->m_CSystemCameraConfig.auto_cover = m_CBaseConfigWidget->m_combobox_line2->currentIndex();//自动覆盖 0:是  1:否
    CCommonData::getInstance()->m_CSystemCameraConfig.videotape_lock = m_CBaseConfigWidget->m_combobox_line3->currentIndex();//录像锁定　0:1 1:2 2:3 3:4 4:5 5:6 6:7
    CCommonData::getInstance()->m_CSystemCameraConfig.videotape_prev = m_CBaseConfigWidget->m_lineedit_line4->text().toInt();//录像预录    min
    CCommonData::getInstance()->m_CSystemCameraConfig.network_transmission_mode = m_CBaseConfigWidget->m_combobox_line5->currentIndex();//网传模式  0:流畅+清晰 1:非常清晰 2:清晰 3:流畅 4:非常流畅
    CCommonData::getInstance()->m_CSystemCameraConfig.videotape_storage = m_CBaseConfigWidget->m_combobox_line6->currentIndex();//录像存储器 0:EMMC+外置SD卡 1:EMMC  2:外置SD卡
    CCommonData::getInstance()->m_CSystemCameraConfig.videotape_mode = m_CBaseConfigWidget->m_combobox_line7->currentIndex();//录像模式  0:镜像录像  1:报警备份录像 2:子码流录像
    //配置－镜像通道
    CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.clear();
     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line2_1->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line2_2->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line2_3->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line2_4->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line3_1->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line3_2->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line3_3->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line3_4->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line4_1->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line4_2->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line4_3->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line4_4->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line5_1->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line5_2->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line5_3->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line5_4->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line6_1->checkState() == Qt::Checked)? 1:0);

     CCommonData::getInstance()->m_CSystemCameraConfig.m_passageway_list.push_back((m_image_set_widget->m_checkbox_line6_2->checkState() == Qt::Checked)? 1:0);

    //

    //显示设置---------------

    CCommonData::getInstance()->m_CSystemCameraConfig.time_show_enable = (m_CShowConfigWidget->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//时间显示使能
    CCommonData::getInstance()->m_CSystemCameraConfig.time_show_value = m_CShowConfigWidget->m_combobox_line1->currentIndex();//时间显示值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    CCommonData::getInstance()->m_CSystemCameraConfig.passageway_name_enable = (m_CShowConfigWidget->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;//通道名称使能
    CCommonData::getInstance()->m_CSystemCameraConfig.passageway_name_value = m_CShowConfigWidget->m_combobox_line2->currentIndex();//通道名称值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    CCommonData::getInstance()->m_CSystemCameraConfig.vehicle_number_enable = (m_CShowConfigWidget->m_checkbox_line3->checkState() == Qt::Checked)? 1:0;//车辆编号使能
    CCommonData::getInstance()->m_CSystemCameraConfig.vehicle_number_value = m_CShowConfigWidget->m_combobox_line3->currentIndex();//车辆编号值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    CCommonData::getInstance()->m_CSystemCameraConfig.coordinate_info_enable = (m_CShowConfigWidget->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//坐标信息使能
    CCommonData::getInstance()->m_CSystemCameraConfig.coordinate_info_value = m_CShowConfigWidget->m_combobox_line4->currentIndex();//车辆编号值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    CCommonData::getInstance()->m_CSystemCameraConfig.real_speed_enable = (m_CShowConfigWidget->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;//实时速度使能
    CCommonData::getInstance()->m_CSystemCameraConfig.real_speed_value = m_CShowConfigWidget->m_combobox_line5->currentIndex();//实时速度值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    CCommonData::getInstance()->m_CSystemCameraConfig.station_info_enable = (m_CShowConfigWidget->m_checkbox_line6->checkState() == Qt::Checked)? 1:0;//站点信息使能
    CCommonData::getInstance()->m_CSystemCameraConfig.station_info_value = m_CShowConfigWidget->m_combobox_line6->currentIndex();//站点信息值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下


    //通道设置---------------
    CCommonData::getInstance()->m_CSystemCameraConfig.monitor_passageway_number = m_CPassagewayConfigWidget->m_combobox_line0->currentIndex();
    CCommonData::getInstance()->m_CSystemCameraConfig.network_passageway_number = m_CPassagewayConfigWidget->m_combobox_line0_1->currentIndex();//IPC数量


}

void CCameraConfigWidget::slot_camera_config_widget_event(const CSystemCameraConfig &_info)
{
    //通用设置---------------
    m_CBaseConfigWidget->m_combobox_line1->setCurrentIndex(_info.videotape_standard);//录像制式  0:PAL 1:NTSC
    m_CBaseConfigWidget->m_combobox_line2->setCurrentIndex(_info.auto_cover);//自动覆盖 0:是  1:否
    m_CBaseConfigWidget->m_combobox_line3->setCurrentIndex(_info.videotape_lock);//录像锁定　0:1 1:2 2:3 3:4 4:5 5:6 6:7
    m_CBaseConfigWidget->m_lineedit_line4->setText(QString::number(_info.videotape_prev));//录像预录    min
    m_CBaseConfigWidget->m_combobox_line5->setCurrentIndex(_info.network_transmission_mode);//网传模式  0:流畅+清晰 1:非常清晰 2:清晰 3:流畅 4:非常流畅
    m_CBaseConfigWidget->m_combobox_line6->setCurrentIndex(_info.videotape_storage);//录像存储器 0:EMMC+外置SD卡 1:EMMC  2:外置SD卡
    m_CBaseConfigWidget->m_combobox_line7->setCurrentIndex(_info.videotape_mode);//录像模式  0:镜像录像  1:报警备份录像 2:子码流录像
    //配置－镜像通道
    bool get_value = false;
    for(int index = 0;index < _info.m_passageway_list.size();index++)
    {
        get_value = (_info.m_passageway_list.at(index) == 0)? false:true;
        if(0 == index)
            m_image_set_widget->m_checkbox_line2_1->setChecked(get_value);
        else if(1 == index)
            m_image_set_widget->m_checkbox_line2_2->setChecked(get_value);
        else if(2 == index)
            m_image_set_widget->m_checkbox_line2_3->setChecked(get_value);
        else if(3 == index)
            m_image_set_widget->m_checkbox_line2_4->setChecked(get_value);
        else if(4 == index)
            m_image_set_widget->m_checkbox_line3_1->setChecked(get_value);
        else if(5 == index)
            m_image_set_widget->m_checkbox_line3_2->setChecked(get_value);
        else if(6 == index)
            m_image_set_widget->m_checkbox_line3_3->setChecked(get_value);
        else if(7 == index)
            m_image_set_widget->m_checkbox_line3_4->setChecked(get_value);
        else if(9 == index)
            m_image_set_widget->m_checkbox_line4_1->setChecked(get_value);
        else if(10 == index)
            m_image_set_widget->m_checkbox_line4_2->setChecked(get_value);
        else if(11 == index)
            m_image_set_widget->m_checkbox_line4_3->setChecked(get_value);
        else if(12 == index)
            m_image_set_widget->m_checkbox_line4_4->setChecked(get_value);
        else if(13 == index)
            m_image_set_widget->m_checkbox_line5_1->setChecked(get_value);
        else if(14 == index)
            m_image_set_widget->m_checkbox_line5_2->setChecked(get_value);
        else if(15 == index)
            m_image_set_widget->m_checkbox_line5_3->setChecked(get_value);
        else if(16 == index)
            m_image_set_widget->m_checkbox_line5_4->setChecked(get_value);
        else if(17 == index)
            m_image_set_widget->m_checkbox_line6_1->setChecked(get_value);
        else if(18 == index)
            m_image_set_widget->m_checkbox_line6_2->setChecked(get_value);
    }
    //

    //显示设置---------------

    if(0 == _info.time_show_enable) //时间显示使能
        m_CShowConfigWidget->m_checkbox_line1->setChecked(false);
    else
        m_CShowConfigWidget->m_checkbox_line1->setChecked(true);
    m_CShowConfigWidget->m_combobox_line1->setCurrentIndex(_info.time_show_value);//时间显示值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    if(0 == _info.passageway_name_enable) //通道名称使能
        m_CShowConfigWidget->m_checkbox_line2->setChecked(false);
    else
        m_CShowConfigWidget->m_checkbox_line2->setChecked(true);
    m_CShowConfigWidget->m_combobox_line2->setCurrentIndex(_info.passageway_name_value);//通道名称值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    if(0 == _info.vehicle_number_enable) //车辆编号使能
        m_CShowConfigWidget->m_checkbox_line3->setChecked(false);
    else
        m_CShowConfigWidget->m_checkbox_line3->setChecked(true);
     m_CShowConfigWidget->m_combobox_line3->setCurrentIndex(_info.vehicle_number_value);//车辆编号值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    if(0 == _info.coordinate_info_enable) //坐标信息使能
        m_CShowConfigWidget->m_checkbox_line4->setChecked(false);
    else
        m_CShowConfigWidget->m_checkbox_line4->setChecked(true);
    m_CShowConfigWidget->m_combobox_line4->setCurrentIndex(_info.coordinate_info_value);//坐标信息值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    if(0 == _info.real_speed_enable) //实时速度使能
        m_CShowConfigWidget->m_checkbox_line5->setChecked(false);
    else
        m_CShowConfigWidget->m_checkbox_line5->setChecked(true);
    m_CShowConfigWidget->m_combobox_line5->setCurrentIndex(_info.real_speed_value);//实时速度值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下

    if(0 == _info.station_info_enable) //站点信息使能
        m_CShowConfigWidget->m_checkbox_line6->setChecked(false);
    else
        m_CShowConfigWidget->m_checkbox_line6->setChecked(true);
    m_CShowConfigWidget->m_combobox_line6->setCurrentIndex(_info.station_info_value);//站点信息值　0:正上 1:左上 2:右上 3:正下 4:左下 5:右下


    //通道设置---------------
    m_CPassagewayConfigWidget->m_combobox_line0->setCurrentIndex(_info.monitor_passageway_number);//模拟数量
    m_CPassagewayConfigWidget->m_combobox_line0_1->setCurrentIndex(_info.network_passageway_number);//IPC数量



}

void CCameraConfigWidget::slot_camera_config_widget_notify(char _type,char _cam_type,char _number)
{
    switch(_type)
    {
        case 1:  //获取到所有数据
        {
            if(m_widget_submain_1->isVisible() || m_widget_submain_2->isVisible())
            {
                slot_button_config_data(m_type,m_number,m_cam_number,m_ipc_number);
            }
            int sort_index = CCommonData::getInstance()->m_CSystemCameraConfig.monitor_passageway_number;
            QString str_show_ip = "";
            for(int index = 0;index < m_CPassagewayConfigWidget->m_ipc_list.size();index++)
            {
                 CSystemPonInfo *pTemp = NULL;

                if(0 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon1;
                else if(1 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon2;
                else if(2 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon3;
                else if(3 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon4;
                else if(4 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon5;
                else if(5 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon6;
                else if(6 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon7;
                else if(7 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon8;
                else if(8 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon9;
                else if(9 == index)
                    pTemp = &CCommonData::getInstance()->m_CSystemCameraConfig.pon10;

                if(NULL == pTemp)
                    continue;

                //if(m_CPassagewayConfigWidget->m_ipc_list.at(index)->isVisible())
                {

                    ++sort_index;
                    if(pTemp->config_ip.isEmpty())
                        str_show_ip = "IPC";
                    else
                        str_show_ip = pTemp->config_ip;

                    m_CPassagewayConfigWidget->m_ipc_list.at(index)->set_text(QString("PON-通道%1(%2)").arg(sort_index).arg(str_show_ip),sort_index);

                }
            }

        }break;
        case 2: //获取到默认数据
        {
            if(1 == _cam_type)
            {//pon
                m_pon_widget->update_data(CCommonData::getInstance()->m_CSystemCameraConfig.pon_default,_number);
            }
            else if(2 == _cam_type)
            {//cam
                m_cam_widget->update_data(CCommonData::getInstance()->m_CSystemCameraConfig.cam_default,_number);
            }

        }break;
        case 3://搜索
        {
            m_pon_widget->update_data(CCommonData::getInstance()->m_CSystemCameraConfig.pon_default,_number);
        }break;

    }


}

void CCameraConfigWidget::slot_current_passageway(int _total,int _cam_number,int _pon_number)
{

    int pon_number = 0;
    m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + ROW_HEIGHT * (_total - 2));

#if 1

    m_cam_widget->m_auto_change = true;
    m_pon_widget->m_auto_change = true;

    m_cam_widget->clean_copy_sheet_list();
    m_cam_widget->add_copy_sheet_list("全部CAM通道");



    for(int index = 0;index < _cam_number;index++)
    {
        pon_number++;
        m_cam_widget->add_copy_sheet_list(QString("PON%1").arg(pon_number));
    }

    m_pon_widget->clean_copy_sheet_list();
    m_pon_widget->add_copy_sheet_list("全部IPC通道");

    for(int index = 0;index < _pon_number;index++)
    {
        pon_number++;
        m_pon_widget->add_copy_sheet_list(QString("PON%1").arg(pon_number));
    }

    m_cam_widget->m_auto_change = false;
    m_pon_widget->m_auto_change = false;

#endif
}

void CCameraConfigWidget::slot_save_data()
{
    save_data(false);
}
























