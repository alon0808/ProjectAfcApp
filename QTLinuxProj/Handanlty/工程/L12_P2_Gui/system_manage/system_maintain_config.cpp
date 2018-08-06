#include "system_maintain_config.h"

#include "../class/common_interface.h"
#include "LtyCommonStyle.h"
#include "class/ltystring.h"

#include <QListView>
#include <QHeaderView>
#include "class/common/define_screen.h"

#define MESSAGEBOX_BUTTON_WIDTH   140
#define MESSAGEBOX_BUTTON_HEIGHT   65



CConfigWidget::CConfigWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setObjectName("CConfigWidget");
    this->setStyleSheet("QWidget#CConfigWidget{border:1px solid #24508F;background-color:#0F1D31;}");
    this->setFixedSize(489 + 90,ROW_HEIGHT);
    this->move(1,1);
    QString str_show = tr("3.6-1110");
    m_label_filename = new QLabel(this);
    CCommonInterface::init_label_text(m_label_filename,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);


    m_checkbox = new QCheckBox(this);
    m_checkbox->setFixedSize(70,70);
    m_checkbox->move(242,(ROW_HEIGHT - 70)/2);
    m_checkbox->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox->setFocusPolicy(Qt::NoFocus);

    m_combobox = new QComboBox(this);
    m_combobox->move(302,(ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox->setFixedSize(160,COMBOBOX_HEIGHT);
    m_combobox->addItem(tr("CAM1通道"));
    m_combobox->addItem(tr("CAM2通道"));
    m_combobox->setView(new QListView());
}



CSystemMaintainConfig::CSystemMaintainConfig(QWidget *parent) :
    QDialog(parent)
{

    this->setFixedSize(600,77 + 538);
    this->setObjectName("CSystemMaintainConfig");
    this->setStyleSheet("QDialog#CSystemMaintainConfig{border:1px solid #275ba7;border-radius: 0px;background-color:#03111f;}");//444a59 //ffffff
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    //标题
    QString str_sheet = "";
    str_sheet = "QLabel{border:0px solid #ffffff;font:46px;background:#275ba7;color:#80e3ff;}";
    m_label_title = new QLabel(this);
    CCommonInterface::init_label_text(m_label_title,1,1,this->width() - 2,77,"",str_sheet);
    m_label_title->setText(tr("客流参数配置"));


    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png");

    m_pushbutton_close = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_close,this->width() - 10 - 40,(77 - 52)/2,40,52,str_sheet);


    m_pushbutton1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton1,323 - 212,588 - 76,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_OK);
    m_pushbutton1->setText(CLtyString::getInstance()->m_confirm);

    m_pushbutton2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton2,493 - 212,588 - 76,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_CANCEL);
    m_pushbutton2->setText(CLtyString::getInstance()->m_cancel);

    m_button_type = KEnumButtonCancel;

    tablewidget_init();
    CConfigWidget *pConfig = new CConfigWidget;
    add_data(pConfig);
    pConfig = new CConfigWidget;
    add_data(pConfig);
    pConfig = new CConfigWidget;
    add_data(pConfig);
    pConfig = new CConfigWidget;
    add_data(pConfig);
    pConfig = new CConfigWidget;
    add_data(pConfig);
    pConfig = new CConfigWidget;
    add_data(pConfig);

    connect(m_pushbutton1,SIGNAL(clicked()),this,SLOT(slot_pushbutton1()));
    connect(m_pushbutton2,SIGNAL(clicked()),this,SLOT(slot_pushbutton2()));

    connect(m_pushbutton_close,SIGNAL(clicked()),this,SLOT(slot_pushbutton_close()));

}

#define BACKGROUND_COLOR_WHITE		"#0F1D31"//"#ffffff"
#define SPLITE_COLOR_GRAY			"#24508F"//"#dddddd"


void CSystemMaintainConfig::tablewidget_init()
{
    m_tablewidget = new QTableWidget(this);
    m_tablewidget->move((this->width() - 530)/2,192 - 76);
    m_tablewidget->setFixedSize(530,365);

    //m_tablewidget->setFrameShape(QFrame::NoFrame); //设置无边框
    m_tablewidget->setShowGrid(false); //设置不显示格子线
    m_tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    m_tablewidget->horizontalHeader()->setVisible(false);
    m_tablewidget->verticalHeader()->setVisible(false); //隐藏行表头
    m_tablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tablewidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置为可以选中多个目标
    m_tablewidget->setColumnCount(1);

    m_tablewidget->verticalHeader()->setDefaultSectionSize(30);

   // this->setAlternatingRowColors(true);
    m_tablewidget->setFocusPolicy(Qt::NoFocus);
    m_tablewidget->horizontalHeader()->setStretchLastSection(true);
#if 0
    m_tablewidget->setStyleSheet(QString("\
                        QTableView{\
                        background-color:%1;\
                        border:1px solid #24508F;\
                        border-left:0px solid %2;\
                        border-top:0px solid  %3;\
                        border-right:0px solid %4;\
                        border-bottom:0px solid %5;\
                        }\
                        QTableWidget::item:selected {\
                        background-color: %6;\
                        }\
                        ").arg(BACKGROUND_COLOR_WHITE, SPLITE_COLOR_GRAY, SPLITE_COLOR_GRAY, SPLITE_COLOR_GRAY, SPLITE_COLOR_GRAY, BACKGROUND_COLOR_WHITE));
#endif
    m_ScrollController = new ScrollController(this);
    m_ScrollController->addMonitorWidget((QScrollArea *)(m_tablewidget));
    m_tablewidget->setStyleSheet("QTableWidget{border:1px solid #24508F;background-color:#0F1D31;}\
                                 QTableView{}\
                                 QTableWidget::item:selected {\
                                 background-color:#1D304E;\
                                 }"
                                );

}

void CSystemMaintainConfig::add_data(QWidget *_widget)
{
   int row_count = m_tablewidget->rowCount();
   m_tablewidget->insertRow(row_count);
   m_tablewidget->setRowHeight(row_count,ROW_HEIGHT + 2);
   m_tablewidget->setCellWidget(row_count,0,_widget);

}

void CSystemMaintainConfig::slot_pushbutton_close()
{
    slot_pushbutton2();
}


void CSystemMaintainConfig::slot_pushbutton1()
{
     m_button_type = KEnumButtonOk;
     hide();
}
void CSystemMaintainConfig::slot_pushbutton2()
{
     m_button_type = KEnumButtonCancel;
     hide();
}
