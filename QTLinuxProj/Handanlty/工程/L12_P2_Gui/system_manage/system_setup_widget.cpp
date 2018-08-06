
#include "system_setup_widget.h"

#include <QDateTime>
#include <QListView>

#include "LtyCommonDefine.h"
#include "LtyCommonStyle.h"

#include "class/common_interface.h"
#include "common_data.h"

#include <QDebug>

CSystemSetupInfo::CSystemSetupInfo(int _type,QWidget *parent) :
    QWidget(parent)
{
    this->setObjectName("CSystemSetupInfo");
    this->setStyleSheet("QWidget#CSystemSetupInfo{border:1px solid #24508F;background-color:#ffffff;}");//#03111E

    set_type(_type);

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this,false);
}

void CSystemSetupInfo::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CSystemSetupInfo::common_config_init()  //通用配置
{
#ifdef APPLICATION_TYPE_P2
    int row_count = 10 + 1 - 6;
#else
    int row_count = 9;
#endif
    this->setFixedSize(620,ROW_HEIGHT * row_count);
    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * row_count);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");

    for(int index = 0;index < (row_count + 1);index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("屏幕模式");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_combobox_line1 = new QComboBox(this);
    m_combobox_line1->move(738 - 290 - 15,ROW_HEIGHT * 0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line1->setFixedSize(150 + 15,COMBOBOX_HEIGHT);
    m_combobox_line1->addItem(tr("高清模式"));
    m_combobox_line1->addItem(tr("标清模式"));
    m_combobox_line1->setView(new QListView());

    str_show = tr("DRUSB接口模式");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_combobox_line2 = new QComboBox(this);
    m_combobox_line2->move(738 - 290 - 15,ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->setFixedSize(150 + 15,COMBOBOX_HEIGHT);
    m_combobox_line2->addItem(tr("标准"));
    m_combobox_line2->addItem(tr("灾备"));
    m_combobox_line2->setView(new QListView());

#ifdef APPLICATION_TYPE_P2
    m_label_line1->hide();
    m_combobox_line1->hide();
    m_label_line2->hide();
    m_combobox_line2->hide();
#endif

    str_show = tr("延时关机");
    m_label_line3 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
#endif
    m_checkbox_line3 = new QCheckBox(this);
    m_checkbox_line3->setFixedSize(70,70);
#ifdef APPLICATION_TYPE_P2
    m_checkbox_line3->move(650 - 290,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2);
#else
    m_checkbox_line3->move(650 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
#endif
    m_checkbox_line3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3->setFocusPolicy(Qt::NoFocus);

    m_lineedit3 = new QLineEdit(this);
#ifdef APPLICATION_TYPE_P2
    m_lineedit3->move(724 - 290,ROW_HEIGHT * 0 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#else
    m_lineedit3->move(724 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#endif
    m_lineedit3->setFixedSize(120,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit3,true);

    str_show = tr("min");
    m_label_line3_1 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line3_1,854 - 290, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line3_1,854 - 290, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
#endif
    str_show = tr("设备门开启上报间隔");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_checkbox_line4 = new QCheckBox(this);
    m_checkbox_line4->setFixedSize(70,70);
    m_checkbox_line4->move(650 - 290,ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4->setFocusPolicy(Qt::NoFocus);

    m_lineedit4 = new QLineEdit(this);
    m_lineedit4->move(724 - 290,ROW_HEIGHT * 3 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit4->setFixedSize(120,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit4,true);

    str_show = tr("min");
    m_label_line4_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4_1,854 - 290, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

#ifdef APPLICATION_TYPE_P2
    m_label_line4->hide();
    m_checkbox_line4->hide();
    m_lineedit4->hide();
    m_label_line4_1->hide();
#endif

    str_show = tr("硬盘加热");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_checkbox_line5 = new QCheckBox(this);
    m_checkbox_line5->setFixedSize(70,70);
    m_checkbox_line5->move(650 - 290,ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5->setFocusPolicy(Qt::NoFocus);

    str_show = tr("≤");
    m_label_line5_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_1,703 - 290, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_lineedit5 = new QLineEdit(this);
    m_lineedit5->move(724 - 290,ROW_HEIGHT * 4 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit5->setFixedSize(120,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit5,true);

    str_show = tr("min");
    m_label_line5_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5_2,854 - 290, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

#ifdef APPLICATION_TYPE_P2
    m_label_line5->hide();
    m_checkbox_line5->hide();
    m_label_line5_1->hide();
    m_lineedit5->hide();
    m_label_line5_2->hide();

#endif
    str_show = tr("整点报时");
    m_label_line6 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6,30, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_checkbox_line6 = new QCheckBox(this);
    m_checkbox_line6->setFixedSize(70,70);
    m_checkbox_line6->move(650 - 290 - 20,ROW_HEIGHT * 5 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line6->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line6->setFocusPolicy(Qt::NoFocus);

    m_lineedit6_1 = new QLineEdit(this);
    m_lineedit6_1->move(688 - 290,ROW_HEIGHT * 5 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit6_1->setFixedSize(60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit6_1,true);

    str_show = tr("~");
    m_label_line6_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_1,761 - 290, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

    m_lineedit6_2 = new QLineEdit(this);
    m_lineedit6_2->move(784 - 290,ROW_HEIGHT * 5 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit6_2->setFixedSize(60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit6_2,true);

    str_show = tr("h");
    m_label_line6_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line6_2,854 - 290, ROW_HEIGHT * 5,0,ROW_HEIGHT,str_show);

#ifdef APPLICATION_TYPE_P2
    m_label_line6->hide();
    m_checkbox_line6->hide();
    m_lineedit6_1->hide();
    m_label_line6_1->hide();
    m_lineedit6_2->hide();
    m_label_line6_2->hide();
#endif

    str_show = tr("超电压报警");
    m_label_line7 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line7,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line7,30, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);
#endif
    m_checkbox_line7 = new QCheckBox(this);
    m_checkbox_line7->setFixedSize(70,70);
#ifdef APPLICATION_TYPE_P2
    m_checkbox_line7->move(546 - 290 - 20,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
#else
    m_checkbox_line7->move(546 - 290 - 20,ROW_HEIGHT * 6 + (ROW_HEIGHT - 70)/2);
#endif
    m_checkbox_line7->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line7->setFocusPolicy(Qt::NoFocus);


    m_lineedit7_1 = new QLineEdit(this);
#ifdef APPLICATION_TYPE_P2
    m_lineedit7_1->move(588 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#else
    m_lineedit7_1->move(588 - 290,ROW_HEIGHT * 6 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#endif
    m_lineedit7_1->setFixedSize(60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit7_1,true);

    str_show = tr("~");
    m_label_line7_1 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line7_1,661 - 290, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line7_1,661 - 290, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);
#endif
    m_lineedit7_2 = new QLineEdit(this);
#ifdef APPLICATION_TYPE_P2
    m_lineedit7_2->move(684 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#else
    m_lineedit7_2->move(684 - 290,ROW_HEIGHT * 6 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#endif
    m_lineedit7_2->setFixedSize(60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit7_2,true);

    str_show = tr("V");
    m_label_line7_2 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line7_2,755 - 290, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line7_2,755 - 290, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);
#endif
    m_lineedit7_3 = new QLineEdit(this);
#ifdef APPLICATION_TYPE_P2
    m_lineedit7_3->move(784 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#else
    m_lineedit7_3->move(784 - 290,ROW_HEIGHT * 6 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#endif
    m_lineedit7_3->setFixedSize(60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit7_3,true);

    str_show = tr("min");
    m_label_line7_3 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line7_3,854 - 290, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line7_3,854 - 290, ROW_HEIGHT * 6,0,ROW_HEIGHT,str_show);
#endif
    str_show = tr("超温度报警");
    m_label_line8 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line8,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line8,30, ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);
#endif
    m_checkbox_line8 = new QCheckBox(this);
    m_checkbox_line8->setFixedSize(70,70);
#ifdef APPLICATION_TYPE_P2
    m_checkbox_line8->move(546 - 290 - 20,ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
#else
    m_checkbox_line8->move(546 - 290 - 20,ROW_HEIGHT * 7 + (ROW_HEIGHT - 70)/2);
#endif
    m_checkbox_line8->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line8->setFocusPolicy(Qt::NoFocus);

    m_lineedit8_1 = new QLineEdit(this);
#ifdef APPLICATION_TYPE_P2
    m_lineedit8_1->move(588 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#else
    m_lineedit8_1->move(588 - 290,ROW_HEIGHT * 7 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#endif
    m_lineedit8_1->setFixedSize(60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit8_1,true);

    str_show = tr("~");
    m_label_line8_1 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line8_1,661 - 290, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line8_1,661 - 290, ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);
#endif

    m_lineedit8_2 = new QLineEdit(this);
#ifdef APPLICATION_TYPE_P2
    m_lineedit8_2->move(684 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#else
    m_lineedit8_2->move(684 - 290,ROW_HEIGHT * 7 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#endif
    m_lineedit8_2->setFixedSize(60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit8_2,true);

    str_show = tr("℃");
    m_label_line8_2 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line8_2,755 - 290, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line8_2,755 - 290, ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);
#endif
    m_lineedit8_3 = new QLineEdit(this);
#ifdef APPLICATION_TYPE_P2
    m_lineedit8_3->move(784 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#else
    m_lineedit8_3->move(784 - 290,ROW_HEIGHT * 7 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#endif
    m_lineedit8_3->setFixedSize(60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit8_3,true);

    str_show = tr("min");
    m_label_line8_3 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line8_3,854 - 290, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line8_3,854 - 290, ROW_HEIGHT * 7,0,ROW_HEIGHT,str_show);
#endif
    //add
    str_show = tr("风扇控制");
    m_label_line9 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line9,30, ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);

    m_checkbox_line9 = new QCheckBox(this);
    m_checkbox_line9->setFixedSize(70,70);
    m_checkbox_line9->move(588 - 290,ROW_HEIGHT * 8 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line9->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line9->setFocusPolicy(Qt::NoFocus);


    str_show = tr("≥");
    m_label_line9_0 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line9_0,661 - 290, ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);

    m_lineedit9_1 = new QLineEdit(this);
    m_lineedit9_1->move(688 - 290,ROW_HEIGHT * 8 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit9_1->setFixedSize(60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit9_1,true);

    str_show = tr("℃");
    m_label_line9_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line9_1,761 - 290 - 5, ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);

    m_lineedit9_2 = new QLineEdit(this);
    m_lineedit9_2->move(784 - 290,ROW_HEIGHT * 8 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit9_2->setFixedSize(60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit9_2,true);

    str_show = tr("min");
    m_label_line9_2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line9_2,854 - 290, ROW_HEIGHT * 8,0,ROW_HEIGHT,str_show);

#ifdef APPLICATION_TYPE_P2
    m_label_line9->hide();
    m_checkbox_line9->hide();
    m_label_line9_0->hide();
    m_lineedit9_1->hide();
    m_label_line9_1->hide();
    m_lineedit9_2->hide();
    m_label_line9_2->hide();
#endif

    str_show = tr("图片轮播");
    m_label_line10 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line10,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line10,30, ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show);
#endif
    m_lineedit10_2 = new QLineEdit(this);
#ifdef APPLICATION_TYPE_P2
    m_lineedit10_2->move(784 - 290,ROW_HEIGHT * 3 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#else
    m_lineedit10_2->move(784 - 290,ROW_HEIGHT * 9 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
#endif
    m_lineedit10_2->setFixedSize(60,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit10_2,true,0,255);

    str_show = tr("S");
    m_label_line10_2 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line10_2,854 - 290, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line10_2,854 - 290, ROW_HEIGHT * 9,0,ROW_HEIGHT,str_show);
#endif
    str_show = tr("音量大小");
    m_label_line11 = new QLabel(this);
#ifdef APPLICATION_TYPE_P2
    CCommonInterface::init_label_text(m_label_line11,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);
#else
    CCommonInterface::init_label_text(m_label_line11,30, ROW_HEIGHT * 10,0,ROW_HEIGHT,str_show);
#endif
    m_combobox_line11 = new QComboBox(this);
#ifdef APPLICATION_TYPE_P2
    m_combobox_line11->move(738 - 290 - 15,ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
#else
    m_combobox_line11->move(738 - 290 - 15,ROW_HEIGHT * 10 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
#endif
    m_combobox_line11->setFixedSize(150 + 15,COMBOBOX_HEIGHT);
    m_combobox_line11->addItem(tr("6"));
    m_combobox_line11->addItem(tr("5"));
    m_combobox_line11->addItem(tr("4"));
    m_combobox_line11->addItem(tr("3"));
    m_combobox_line11->addItem(tr("2"));
    m_combobox_line11->addItem(tr("1"));
    m_combobox_line11->addItem(tr("0"));
    m_combobox_line11->setView(new QListView());


#ifdef APPLICATION_TYPE_P2
    m_label_line10->show();
    m_lineedit10_2->show();
    m_label_line10_2->show();
    m_label_line11->show();
    m_combobox_line11->show();
#else
    m_label_line10->hide();
    m_lineedit10_2->hide();
    m_label_line10_2->hide();
    m_label_line11->hide();
    m_combobox_line11->hide();
#endif

}
void CSystemSetupInfo::system_time_init()    //系统时间
{
    this->setFixedSize(620,ROW_HEIGHT * 5);
    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 5);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");

    for(int index = 0;index < 6;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("时区");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_combobox_line1 = new QComboBox(this);
    m_combobox_line1->move(512 - 290,ROW_HEIGHT * 0 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line1->setFixedSize(376,COMBOBOX_HEIGHT);
    //m_combobox_line1->addItem(tr("(UTC+7) 曼谷，河内，雅加达"));
    //m_combobox_line1->addItem(tr("(UTC+7)北京，重庆，香港特别行政区，乌鲁朩齐"));
    m_combobox_line1->addItem(tr("UTC"));
    m_combobox_line1->addItem(tr("UTC+1"));
    m_combobox_line1->addItem(tr("UTC+2"));
    m_combobox_line1->addItem(tr("UTC+3"));
    m_combobox_line1->addItem(tr("UTC+4"));
    m_combobox_line1->addItem(tr("UTC+5"));
    m_combobox_line1->addItem(tr("UTC+6"));
    m_combobox_line1->addItem(tr("UTC+7"));
    m_combobox_line1->addItem(tr("UTC+8"));
    m_combobox_line1->addItem(tr("UTC+9"));
    m_combobox_line1->addItem(tr("UTC+10"));
    m_combobox_line1->addItem(tr("UTC+11"));

    m_combobox_line1->setView(new QListView());

    str_show = tr("时间");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_lineedit2 = new QLineEdit(this);
    m_lineedit2->move(566 - 290 - 40,ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit2->setFixedSize(140 + 40,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit2);
    m_lineedit2->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd"));

    str_show = tr("-");
    m_label_line2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_1,716 - 290, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_lineedit2_1 = new QLineEdit(this);
    m_lineedit2_1->move(748 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit2_1->setFixedSize(140,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit2_1);
    m_lineedit2_1->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));

    str_show = tr("卫星校时");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_checkbox_line3 = new QCheckBox(this);
    m_checkbox_line3->setFixedSize(70,70);
    m_checkbox_line3->move(858 - 290 - 30,ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3->setFocusPolicy(Qt::NoFocus);

    str_show = tr("中心校时");
    m_label_line4 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line4,30, ROW_HEIGHT * 3,0,ROW_HEIGHT,str_show);

    m_checkbox_line4 = new QCheckBox(this);
    m_checkbox_line4->setFixedSize(70,70);
    m_checkbox_line4->move(858 - 290 - 30,ROW_HEIGHT * 3 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line4->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line4->setFocusPolicy(Qt::NoFocus);


    str_show = tr("NTP校时");
    m_label_line5 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line5,30, ROW_HEIGHT * 4,0,ROW_HEIGHT,str_show);

    m_checkbox_line5 = new QCheckBox(this);
    m_checkbox_line5->setFixedSize(70,70);
    m_checkbox_line5->move(648 - 290 - 30 - 50,ROW_HEIGHT * 4 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line5->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line5->setFocusPolicy(Qt::NoFocus);

    m_combobox_line5 = new QComboBox(this);
    m_combobox_line5->move(688 - 290 - 50,ROW_HEIGHT * 4 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line5->setFixedSize(200 + 50,COMBOBOX_HEIGHT);
    m_combobox_line5->addItem(tr("time.windows.com"));
    m_combobox_line5->addItem(tr("time.nist.gov"));
    m_combobox_line5->addItem(tr("time-nw.nist.gov"));
    m_combobox_line5->addItem(tr("time-a.nist.gov"));
    m_combobox_line5->addItem(tr("time-b.nist.gov"));
    m_combobox_line5->setView(new QListView());


}
void CSystemSetupInfo::account_setting_init() //账户设置
{
    this->setFixedSize(620,ROW_HEIGHT * 3);
    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 3);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");

    for(int index = 0;index < 4;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("使用密码");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    m_checkbox_line1 = new QCheckBox(this);
    m_checkbox_line1->setFixedSize(70,70);
    m_checkbox_line1->move(858 - 290 - 30,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line1->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line1->setFocusPolicy(Qt::NoFocus);

    str_show = tr("自动锁定");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_checkbox_line2 = new QCheckBox(this);
    m_checkbox_line2->setFixedSize(70,70);
    m_checkbox_line2->move(650 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line2->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line2->setFocusPolicy(Qt::NoFocus);

    m_combobox_line2 = new QComboBox(this);
    m_combobox_line2->move(724 - 290,ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->setFixedSize(100,COMBOBOX_HEIGHT);
    m_combobox_line2->addItem(tr("1"));
    m_combobox_line2->addItem(tr("2"));
    m_combobox_line2->addItem(tr("3"));
    m_combobox_line2->addItem(tr("5"));
    m_combobox_line2->addItem(tr("10"));
    m_combobox_line2->setView(new QListView());

    str_show = tr("min");
    m_label_line2_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2_1,854 - 290 - 10, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);


    str_show = tr("超时退出");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_checkbox_line3 = new QCheckBox(this);
    m_checkbox_line3->setFixedSize(70,70);
    m_checkbox_line3->move(650 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - 70)/2);
    m_checkbox_line3->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox_line3->setFocusPolicy(Qt::NoFocus);

    m_combobox_line3 = new QComboBox(this);
    m_combobox_line3->move(724 - 290,ROW_HEIGHT * 2 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line3->setFixedSize(100,COMBOBOX_HEIGHT);
    m_combobox_line3->addItem(tr("1"));
    m_combobox_line3->addItem(tr("2"));
    m_combobox_line3->addItem(tr("3"));
    m_combobox_line3->addItem(tr("5"));
    m_combobox_line3->addItem(tr("10"));
    m_combobox_line3->setView(new QListView());

    str_show = tr("min");
    m_label_line3_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3_1,854 - 290 - 10, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

}

void CSystemSetupInfo::set_type(char _type)
{
    switch(_type)
    {
        case kEnumCommonConfig:  //通用配置
        {
            common_config_init();
        }break;
        case kEnumSystemTime:    //系统时间
        {
            system_time_init();
        }break;
        case kEnumAccountSetting: //账户设置
        {
            account_setting_init();
        }break;
    }
}

CSystemSetupWidget::CSystemSetupWidget(QWidget *parent) :
    QWidget(parent)
{
    m_define_screen = new CMyDefineScreen();

    this->setObjectName("CSystemSetupWidget");
    this->setStyleSheet("QWidget#CSystemSetupWidget{background-color:#ffffff;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->move(0,0);

        QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    pWidget->move(0,0);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");

    scroll_layout();
    system_setup_init_new();

    read_data();


    m_define_screen->add_parent(this);
    m_define_screen->add_parent(m_widget_main);

}

void CSystemSetupWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CSystemSetupWidget::scroll_layout()
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



}

void CSystemSetupWidget::system_setup_init_new()
{
    m_button_1 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_1->setDisplayText(tr("通用配置"));

    m_button_1->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_1->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_1->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");


    m_button_2 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_2->setDisplayText(tr("系统时间"));

    m_button_2->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_2->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_2->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_3 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_3->setDisplayText(tr("账户设置"));

    m_button_3->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_3->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_3->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_system_info1 = new CSystemSetupInfo(CSystemSetupInfo::kEnumCommonConfig,m_widget_main);
    m_system_info2 = new CSystemSetupInfo(CSystemSetupInfo::kEnumSystemTime,m_widget_main);
    m_system_info3 = new CSystemSetupInfo(CSystemSetupInfo::kEnumAccountSetting,m_widget_main);


    m_label_space1  = new QLabel(m_widget_main);
    m_label_space1->setFixedHeight(10);
    m_label_space1->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_label_space2  = new QLabel(m_widget_main);
    m_label_space2->setFixedHeight(10);
    m_label_space2->setStyleSheet("QLabel{border:0px;background:transparent;}");

    m_vboxlayout_storage = new QVBoxLayout(m_widget_main);//左 上 右 下
    m_vboxlayout_storage->setContentsMargins(m_define_screen->get_change_factor_x(290 - 250),m_define_screen->get_change_factor_y(158 - MAIN_HEAD_HEIGHT - 50),0,0);
    m_vboxlayout_storage->setSpacing(0);
    m_vboxlayout_storage->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_vboxlayout_storage->addWidget(m_button_1);
    m_vboxlayout_storage->addWidget(m_system_info1);
    m_vboxlayout_storage->addWidget(m_label_space1);
    m_vboxlayout_storage->addWidget(m_button_2);
    m_vboxlayout_storage->addWidget(m_system_info2);
    m_vboxlayout_storage->addWidget(m_label_space2);
    m_vboxlayout_storage->addWidget(m_button_3);
    m_vboxlayout_storage->addWidget(m_system_info3);

    connect(m_button_1,SIGNAL(clicked()),this,SLOT(slot_button_1()));
    connect(m_button_2,SIGNAL(clicked()),this,SLOT(slot_button_2()));
    connect(m_button_3,SIGNAL(clicked()),this,SLOT(slot_button_3()));

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_system_info1->hide();
    m_system_info2->hide();
    m_system_info3->hide();

}

void CSystemSetupWidget::slot_button_1()
{
    if(m_button_press_1)
    {
        m_button_press_1 = false;
        m_button_1->setSelected(false);
        m_system_info1->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_1 = true;
        m_button_1->setSelected(true);
        m_system_info1->show();
#ifdef APPLICATION_TYPE_P2
        char row_count = 2;
#else
        char row_count = 6;
#endif
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height +  ROW_HEIGHT * row_count));
    }
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_system_info2->hide();
    m_system_info3->hide();


}
void CSystemSetupWidget::slot_button_2()
{
    if(m_button_press_2)
    {
        m_button_press_2 = false;
        m_button_2->setSelected(false);
        m_system_info2->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_2 = true;
        m_button_2->setSelected(true);
        m_system_info2->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height +  ROW_HEIGHT * 2));
    }
    m_button_1->setSelected(false);
    m_button_3->setSelected(false);
    m_button_press_1 = false;
    m_button_press_3 = false;
    m_system_info1->hide();
    m_system_info3->hide();
}
void CSystemSetupWidget::slot_button_3()
{
    if(m_button_press_3)
    {
        m_button_press_3 = false;
        m_button_3->setSelected(false);
        m_system_info3->hide();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    else
    {
        m_button_press_3 = true;
        m_button_3->setSelected(true);
        m_system_info3->show();
        m_widget_main->setFixedSize(m_define_screen->get_change_factor_x(m_scroll_widget_width),m_define_screen->get_change_factor_y(m_scroll_widget_height));
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_press_1 = false;
    m_button_press_2 = false;
    m_system_info1->hide();
    m_system_info2->hide();
}

void CSystemSetupWidget::read_data()
{
    //通用配置
    CCommonData::getInstance()->m_CSystemSystemSetup.screen_mode = m_system_info1->m_combobox_line1->currentIndex(); //屏幕模式 0:高清模式 1:标清模式
    CCommonData::getInstance()->m_CSystemSystemSetup.drusb_mode = m_system_info1->m_combobox_line2->currentIndex();  //DRUSB接口模式 0:标准 1:灾备
    CCommonData::getInstance()->m_CSystemSystemSetup.sleep_shutdown_enable = (m_system_info1->m_checkbox_line3->checkState() == Qt::Checked)? 1:0;//延时关机使能

    CCommonData::getInstance()->m_CSystemSystemSetup.sleep_shutdown_value = m_system_info1->m_lineedit3->text().toInt(); //延时关机值　(min)

    CCommonData::getInstance()->m_CSystemSystemSetup.device_door_report_enable = (m_system_info1->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//设备门开启上报间隔使能

    CCommonData::getInstance()->m_CSystemSystemSetup.device_door_report_value = m_system_info1->m_lineedit4->text().toInt();//设备门开启上报间隔值

    CCommonData::getInstance()->m_CSystemSystemSetup.hard_disk_heating_enable = (m_system_info1->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;//硬盘加热使能

    CCommonData::getInstance()->m_CSystemSystemSetup.hard_disk_heating_value = m_system_info1->m_lineedit5->text().toInt();//硬盘加热值

    CCommonData::getInstance()->m_CSystemSystemSetup.now_time_enable = (m_system_info1->m_checkbox_line6->checkState() == Qt::Checked)? 1:0;//整点报时使能

    CCommonData::getInstance()->m_CSystemSystemSetup.now_time_value1 = m_system_info1->m_lineedit6_1->text().toInt();//整点报时值1   h
    CCommonData::getInstance()->m_CSystemSystemSetup.now_time_value2 = m_system_info1->m_lineedit6_2->text().toInt();//整点报时值2   h

    CCommonData::getInstance()->m_CSystemSystemSetup.voltage_enable = (m_system_info1->m_checkbox_line7->checkState() == Qt::Checked)? 1:0;//超电压反警使能

    CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value1 = m_system_info1->m_lineedit7_1->text().toInt();//超电压反警值1   V
    CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value2 = m_system_info1->m_lineedit7_2->text().toInt();//超电压反警值2   V
    CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value3 = m_system_info1->m_lineedit7_3->text().toInt();//超电压反警值3   min

    CCommonData::getInstance()->m_CSystemSystemSetup.temperature_enable = (m_system_info1->m_checkbox_line8->checkState() == Qt::Checked)? 1:0;//超温度报警使能

    CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value1 = m_system_info1->m_lineedit8_1->text().toInt();//超温度报警值1   V
    CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value2 = m_system_info1->m_lineedit8_2->text().toInt();//超温度报警值2   V
    CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value3 = m_system_info1->m_lineedit8_3->text().toInt();//超温度报警值3   min

    //add
    CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_enable = (m_system_info1->m_checkbox_line9->checkState() == Qt::Checked)? 1:0;
    CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_value1 = m_system_info1->m_lineedit9_1->text().toInt();//风扇控制值1
    CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_value2 = m_system_info1->m_lineedit9_2->text().toInt();//风扇控制值2
    CCommonData::getInstance()->m_CSystemSystemSetup.picutre_play_time = m_system_info1->m_lineedit10_2->text().toInt();//图片轮播
    CCommonData::getInstance()->m_CSystemSystemSetup.volumn_size = m_system_info1->m_combobox_line11->currentIndex(); //声音大小

    CCommonData::getInstance()->m_CSystemSystemSetup.m_current_time_zone = m_system_info2->m_combobox_line1->currentIndex();//当前选择的时区


    CCommonData::getInstance()->m_CSystemSystemSetup.m_time1 = m_system_info2->m_lineedit2->text(); //时间1
    CCommonData::getInstance()->m_CSystemSystemSetup.m_time2 = m_system_info2->m_lineedit2_1->text();//时间2

    CCommonData::getInstance()->m_CSystemSystemSetup.satellite_time_enable = (m_system_info2->m_checkbox_line3->checkState() == Qt::Checked)? 1:0;//卫星校时  0:未选择状态 1:选中状态

    CCommonData::getInstance()->m_CSystemSystemSetup.center_time_enable = (m_system_info2->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;

    CCommonData::getInstance()->m_CSystemSystemSetup.ntp_enable = (m_system_info2->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;//NTP校时使能  0:未选择状态 1:选中状态

    CCommonData::getInstance()->m_CSystemSystemSetup.ntp_value = m_system_info2->m_combobox_line5->currentIndex(); //NTP校时值 0:time.windows.com 1:time.nist.gov 2:time-nw.nist.gov 3:time-a.nist.gov 4:time-b.nist.gov

    //账户设置
    CCommonData::getInstance()->m_CSystemSystemSetup.use_password = (m_system_info3->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//使用密码  0:未选择状态 1:选中状态

    CCommonData::getInstance()->m_CSystemSystemSetup.auto_lock_enable = (m_system_info3->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;//自动锁定使能  0:未选择状态 1:选中状态

     CCommonData::getInstance()->m_CSystemSystemSetup.auto_lock_value = m_system_info3->m_combobox_line2->currentIndex(); //自动锁定值(min)  0:1  1:2  2:3  3:5  4:10

    CCommonData::getInstance()->m_CSystemSystemSetup.timeout_exit_enable = (m_system_info3->m_checkbox_line3->checkState() == Qt::Checked)? 1:0; //超时退出使能  0:未选择状态 1:选中状态

    CCommonData::getInstance()->m_CSystemSystemSetup.timeout_exit_value = m_system_info3->m_combobox_line3->currentIndex();//超时退出值(min) 0:1  1:2  2:3  3:5  4:10

}
void CSystemSetupWidget::save_data()
{
    CSystemSystemSetup info;
    //通用配置
   info.screen_mode = m_system_info1->m_combobox_line1->currentIndex(); //屏幕模式 0:高清模式 1:标清模式
   info.drusb_mode = m_system_info1->m_combobox_line2->currentIndex();  //DRUSB接口模式 0:标准 1:灾备
   info.sleep_shutdown_enable = (m_system_info1->m_checkbox_line3->checkState() == Qt::Checked)? 1:0;//延时关机使能

   info.sleep_shutdown_value = m_system_info1->m_lineedit3->text().toInt(); //延时关机值　(min)

   info.device_door_report_enable = (m_system_info1->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;//设备门开启上报间隔使能

   info.device_door_report_value = m_system_info1->m_lineedit4->text().toInt();//设备门开启上报间隔值

   info.hard_disk_heating_enable = (m_system_info1->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;//硬盘加热使能

   info.hard_disk_heating_value = m_system_info1->m_lineedit5->text().toInt();//硬盘加热值

   info.now_time_enable = (m_system_info1->m_checkbox_line6->checkState() == Qt::Checked)? 1:0;//整点报时使能

   info.now_time_value1 = m_system_info1->m_lineedit6_1->text().toInt();//整点报时值1   h
   info.now_time_value2 = m_system_info1->m_lineedit6_2->text().toInt();//整点报时值2   h

   info.voltage_enable = (m_system_info1->m_checkbox_line7->checkState() == Qt::Checked)? 1:0;//超电压反警使能

   info.voltage_value1 = m_system_info1->m_lineedit7_1->text().toInt();//超电压反警值1   V
   info.voltage_value2 = m_system_info1->m_lineedit7_2->text().toInt();//超电压反警值2   V
   info.voltage_value3 = m_system_info1->m_lineedit7_3->text().toInt();//超电压反警值3   min

   info.temperature_enable = (m_system_info1->m_checkbox_line8->checkState() == Qt::Checked)? 1:0;//超温度报警使能

   info.temperature_value1 = m_system_info1->m_lineedit8_1->text().toInt();//超温度报警值1   V
   info.temperature_value2 = m_system_info1->m_lineedit8_2->text().toInt();//超温度报警值2   V
   info.temperature_value3 = m_system_info1->m_lineedit8_3->text().toInt();//超温度报警值3   min

    info.fan_control_enable = (m_system_info1->m_checkbox_line9->checkState() == Qt::Checked)? 1:0;

    info.fan_control_value1 = m_system_info1->m_lineedit9_1->text().toInt();//风扇控制值1
    info.fan_control_value2 = m_system_info1->m_lineedit9_2->text().toInt();//风扇控制值2
    info.picutre_play_time = m_system_info1->m_lineedit10_2->text().toInt();//图片轮播
    info.volumn_size = m_system_info1->m_combobox_line11->currentIndex(); //声音大小

   info.m_current_time_zone = m_system_info2->m_combobox_line1->currentIndex();//当前选择的时区


   info.m_time1 = m_system_info2->m_lineedit2->text(); //时间1
   info.m_time2 = m_system_info2->m_lineedit2_1->text();//时间2

   info.satellite_time_enable = (m_system_info2->m_checkbox_line3->checkState() == Qt::Checked)? 1:0;//卫星校时  0:未选择状态 1:选中状态

   info.center_time_enable = (m_system_info2->m_checkbox_line4->checkState() == Qt::Checked)? 1:0;

   info.ntp_enable = (m_system_info2->m_checkbox_line5->checkState() == Qt::Checked)? 1:0;//NTP校时使能  0:未选择状态 1:选中状态

   info.ntp_value = m_system_info2->m_combobox_line5->currentIndex(); //NTP校时值 0:time.windows.com 1:time.nist.gov 2:time-nw.nist.gov 3:time-a.nist.gov 4:time-b.nist.gov

    //账户设置
   info.use_password = (m_system_info3->m_checkbox_line1->checkState() == Qt::Checked)? 1:0;//使用密码  0:未选择状态 1:选中状态

   info.auto_lock_enable = (m_system_info3->m_checkbox_line2->checkState() == Qt::Checked)? 1:0;//自动锁定使能  0:未选择状态 1:选中状态

   info.auto_lock_value = m_system_info3->m_combobox_line2->currentIndex(); //自动锁定值(min)  0:1  1:2  2:3  3:5  4:10

   info.timeout_exit_enable = (m_system_info3->m_checkbox_line3->checkState() == Qt::Checked)? 1:0; //超时退出使能  0:未选择状态 1:选中状态

   info.timeout_exit_value = m_system_info3->m_combobox_line3->currentIndex();//超时退出值(min) 0:1  1:2  2:3  3:5  4:10

    //CSystemSystemSetup *pSystemSystemSetup = &CCommonData::getInstance()->m_CSystemSystemSetup;

    if(info.screen_mode != CCommonData::getInstance()->m_CSystemSystemSetup.screen_mode ||
       info.drusb_mode != CCommonData::getInstance()->m_CSystemSystemSetup.drusb_mode ||
       info.sleep_shutdown_enable != CCommonData::getInstance()->m_CSystemSystemSetup.sleep_shutdown_enable ||
       info.sleep_shutdown_value != CCommonData::getInstance()->m_CSystemSystemSetup.sleep_shutdown_value ||
       info.device_door_report_enable != CCommonData::getInstance()->m_CSystemSystemSetup.device_door_report_enable ||
       info.device_door_report_value != CCommonData::getInstance()->m_CSystemSystemSetup.device_door_report_value ||
       info.hard_disk_heating_enable != CCommonData::getInstance()->m_CSystemSystemSetup.hard_disk_heating_enable ||
       info.hard_disk_heating_value != CCommonData::getInstance()->m_CSystemSystemSetup.hard_disk_heating_value ||
       info.now_time_enable != CCommonData::getInstance()->m_CSystemSystemSetup.now_time_enable ||
       info.now_time_value1 != CCommonData::getInstance()->m_CSystemSystemSetup.now_time_value1 ||
       info.now_time_value2 != CCommonData::getInstance()->m_CSystemSystemSetup.now_time_value2 ||
       info.voltage_enable != CCommonData::getInstance()->m_CSystemSystemSetup.voltage_enable ||
       info.voltage_value1 != CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value1 ||
       info.voltage_value2 != CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value2 ||
       info.voltage_value3 != CCommonData::getInstance()->m_CSystemSystemSetup.voltage_value3 ||
       info.temperature_enable != CCommonData::getInstance()->m_CSystemSystemSetup.temperature_enable ||
       info.temperature_value1 != CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value1 ||
       info.temperature_value2 != CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value2 ||
       info.temperature_value3 != CCommonData::getInstance()->m_CSystemSystemSetup.temperature_value3 ||
       //add
       info.fan_control_enable != CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_enable ||
       info.fan_control_value1 != CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_value1 ||
       info.fan_control_value2 != CCommonData::getInstance()->m_CSystemSystemSetup.fan_control_value2 ||
#ifdef APPLICATION_TYPE_P2
       info.picutre_play_time != CCommonData::getInstance()->m_CSystemSystemSetup.picutre_play_time ||
       info.volumn_size  != CCommonData::getInstance()->m_CSystemSystemSetup.volumn_size ||
#endif
       info.m_current_time_zone != CCommonData::getInstance()->m_CSystemSystemSetup.m_current_time_zone ||
       info.m_time1 != CCommonData::getInstance()->m_CSystemSystemSetup.m_time1 ||
       info.m_time2 != CCommonData::getInstance()->m_CSystemSystemSetup.m_time2 ||
       info.satellite_time_enable != CCommonData::getInstance()->m_CSystemSystemSetup.satellite_time_enable ||
       info.center_time_enable != CCommonData::getInstance()->m_CSystemSystemSetup.center_time_enable ||
       info.ntp_enable != CCommonData::getInstance()->m_CSystemSystemSetup.ntp_enable ||
       info.ntp_value != CCommonData::getInstance()->m_CSystemSystemSetup.ntp_value ||
       info.use_password != CCommonData::getInstance()->m_CSystemSystemSetup.use_password ||
       info.auto_lock_enable != CCommonData::getInstance()->m_CSystemSystemSetup.auto_lock_enable ||
       info.auto_lock_value != CCommonData::getInstance()->m_CSystemSystemSetup.auto_lock_value ||
       info.timeout_exit_enable != CCommonData::getInstance()->m_CSystemSystemSetup.timeout_exit_enable ||
       info.timeout_exit_value != CCommonData::getInstance()->m_CSystemSystemSetup.timeout_exit_value
    )
    {
        if(CCommonData::getInstance()->messagebox_data_change())
        {


            QStringList date_list = info.m_time1.split("-");
            QStringList time_list = info.m_time2.split(":");
            qDebug() << "NormalSetWidget::sava_data() " << info.m_time1 << "," << date_list.size();
            qDebug() << "NormalSetWidget::sava_data() " << info.m_time2 << "," << time_list.size();
            if(3 == date_list.size() && 3 == time_list.size())
            {
                qDebug() << "NormalSetWidget::sava_data() 2";
                qDebug() << date_list.at(0) << "," << date_list.at(1) << "," << date_list.at(2);
                qDebug() << time_list.at(0) << "," << time_list.at(1) << "," << time_list.at(2);

                if(info.m_time1 == CCommonData::getInstance()->m_CSystemSystemSetup.m_time1 &&
                   info.m_time2 == CCommonData::getInstance()->m_CSystemSystemSetup.m_time2)
                {
                    CCommonData::getInstance()->m_CSystemSystemSetup.user_set_time = 0; //用户设置时间　１：设置 0：不设置
                }
                else
                {
                    CCommonData::getInstance()->m_CSystemSystemSetup.user_set_time = 1; //用户设置时间　１：设置 0：不设置
                }


                CCommonData::getInstance()->m_CSystemSystemSetup.tm_year = date_list.at(0).toInt();
                CCommonData::getInstance()->m_CSystemSystemSetup.tm_mon = date_list.at(1).toInt();
                CCommonData::getInstance()->m_CSystemSystemSetup.tm_mday = date_list.at(2).toInt();
                CCommonData::getInstance()->m_CSystemSystemSetup.tm_hour = time_list.at(0).toInt();
                CCommonData::getInstance()->m_CSystemSystemSetup.tm_min = time_list.at(1).toInt();
                CCommonData::getInstance()->m_CSystemSystemSetup.tm_sec = time_list.at(2).toInt();
            }
            else
            {
                CCommonData::getInstance()->m_CSystemSystemSetup.user_set_time = 0;
            }

            read_data();
            CProtocol::getInstance()->call_json_system_system_setup_set(CProtocol::kEnumSave);
        }
        else
        {
            CCommonData::getInstance()->m_CSystemSystemSetup.user_set_time = 0;
           slot_system_setup_widget_event(CCommonData::getInstance()->m_CSystemSystemSetup);
        }
    }

}

void CSystemSetupWidget::slot_system_setup_widget_event(const CSystemSystemSetup &_info)
{//m_system_info1
     //通用配置
    m_system_info1->m_combobox_line1->setCurrentIndex(_info.screen_mode); //屏幕模式 0:高清模式 1:标清模式
    m_system_info1->m_combobox_line2->setCurrentIndex(_info.drusb_mode);  //DRUSB接口模式 0:标准 1:灾备
    if(0 == _info.sleep_shutdown_enable) //延时关机使能
        m_system_info1->m_checkbox_line3->setChecked(false);
    else
        m_system_info1->m_checkbox_line3->setChecked(true);

    m_system_info1->m_lineedit3->setText(QString::number(_info.sleep_shutdown_value)); //延时关机值　(min)
    if(0 == _info.device_door_report_enable) //设备门开启上报间隔使能
        m_system_info1->m_checkbox_line4->setChecked(false);
    else
        m_system_info1->m_checkbox_line4->setChecked(true);
    m_system_info1->m_lineedit4->setText(QString::number(_info.device_door_report_value));//设备门开启上报间隔值

    if(0 == _info.hard_disk_heating_enable) //硬盘加热使能
        m_system_info1->m_checkbox_line5->setChecked(false);
    else
        m_system_info1->m_checkbox_line5->setChecked(true);

    m_system_info1->m_lineedit5->setText(QString::number(_info.hard_disk_heating_value));//硬盘加热值

    if(0 == _info.now_time_enable) //整点报时使能
        m_system_info1->m_checkbox_line6->setChecked(false);
    else
        m_system_info1->m_checkbox_line6->setChecked(true);

    m_system_info1->m_lineedit6_1->setText(QString::number(_info.now_time_value1));//整点报时值1   h
    m_system_info1->m_lineedit6_2->setText(QString::number(_info.now_time_value2));//整点报时值2   h

    if(0 == _info.voltage_enable) //超电压反警使能
        m_system_info1->m_checkbox_line7->setChecked(false);
    else
        m_system_info1->m_checkbox_line7->setChecked(true);

    m_system_info1->m_lineedit7_1->setText(QString::number(_info.voltage_value1));//超电压反警值1   V
    m_system_info1->m_lineedit7_2->setText(QString::number(_info.voltage_value2));//超电压反警值2   V
    m_system_info1->m_lineedit7_3->setText(QString::number(_info.voltage_value3));//超电压反警值3   min

    if(0 == _info.temperature_enable) //超温度报警使能
        m_system_info1->m_checkbox_line8->setChecked(false);
    else
        m_system_info1->m_checkbox_line8->setChecked(true);

    m_system_info1->m_lineedit8_1->setText(QString::number(_info.temperature_value1));//超温度报警值1   V
    m_system_info1->m_lineedit8_2->setText(QString::number(_info.temperature_value2));//超温度报警值2   V
    m_system_info1->m_lineedit8_3->setText(QString::number(_info.temperature_value3));//超温度报警值3   min

    if(0 == _info.fan_control_enable) //风扇控制使能
        m_system_info1->m_checkbox_line9->setChecked(false);
    else
        m_system_info1->m_checkbox_line9->setChecked(true);

    m_system_info1->m_lineedit9_1->setText(QString::number(_info.fan_control_value1));//风扇控制值1
    m_system_info1->m_lineedit9_2->setText(QString::number(_info.fan_control_value2));//风扇控制值2
    m_system_info1->m_lineedit10_2->setText(QString::number(_info.picutre_play_time));//图片轮播
    m_system_info1->m_combobox_line11->setCurrentIndex(CCommonData::getInstance()->m_CSystemSystemSetup.volumn_size); //声音大小

    //系统时间　　//m_combobox_line1
    m_system_info2->m_combobox_line1->setCurrentIndex(_info.m_current_time_zone); //当前选择的时区

    //m_system_info2->m_lineedit2->setText(_info.m_time1); //时间1
    //m_system_info2->m_lineedit2_1->setText(_info.m_time2);//时间2

    if(0 == _info.satellite_time_enable) //卫星校时  0:未选择状态 1:选中状态
        m_system_info2->m_checkbox_line3->setChecked(false);
    else
        m_system_info2->m_checkbox_line3->setChecked(true);

    if(0 == _info.center_time_enable) //中心校时  0:未选择状态 1:选中状态
        m_system_info2->m_checkbox_line4->setChecked(false);
    else
        m_system_info2->m_checkbox_line4->setChecked(true);

    if(0 == _info.ntp_enable) //NTP校时使能  0:未选择状态 1:选中状态
        m_system_info2->m_checkbox_line5->setChecked(false);
    else
        m_system_info2->m_checkbox_line5->setChecked(true);

    m_system_info2->m_combobox_line5->setCurrentIndex(_info.ntp_value); //NTP校时值 0:time.windows.com 1:time.nist.gov 2:time-nw.nist.gov 3:time-a.nist.gov 4:time-b.nist.gov

    //账户设置
    if(0 == _info.use_password) //使用密码  0:未选择状态 1:选中状态
        m_system_info3->m_checkbox_line1->setChecked(false);
    else
        m_system_info3->m_checkbox_line1->setChecked(true);

    if(0 == _info.auto_lock_enable) //自动锁定使能  0:未选择状态 1:选中状态
        m_system_info3->m_checkbox_line2->setChecked(false);
    else
        m_system_info3->m_checkbox_line2->setChecked(true);

     m_system_info3->m_combobox_line2->setCurrentIndex(_info.auto_lock_value); //自动锁定值(min)  0:1  1:2  2:3  3:5  4:10

    if(0 == _info.timeout_exit_enable) //超时退出使能  0:未选择状态 1:选中状态
        m_system_info3->m_checkbox_line3->setChecked(false);
    else
        m_system_info3->m_checkbox_line3->setChecked(true);

    m_system_info3->m_combobox_line3->setCurrentIndex(_info.timeout_exit_value);//超时退出值(min) 0:1  1:2  2:3  3:5  4:10


}

void CSystemSetupWidget::update_system_time_show()
{
    CCommonData::getInstance()->m_CSystemSystemSetup.m_time1 = QDateTime::currentDateTime().toString("yyyy-MM-dd"); //时间1
    CCommonData::getInstance()->m_CSystemSystemSetup.m_time2 = QDateTime::currentDateTime().toString("hh:mm:ss");//时间2
    m_system_info2->m_lineedit2->setText(CCommonData::getInstance()->m_CSystemSystemSetup.m_time1); //时间1
    m_system_info2->m_lineedit2_1->setText(CCommonData::getInstance()->m_CSystemSystemSetup.m_time2);//时间2


}











