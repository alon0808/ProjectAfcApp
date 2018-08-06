#include "storage_manage_widget.h"

#include "LtyCommonDefine.h"
#include "./class/common_interface.h"
#include "LtyCommonStyle.h"

#include <QListView>
#include "protocol.h"
#include "mainwindow.h"

#include "common_data.h"

#include "class/common/password_dialog.h"
#include "class/ltystring.h"
#include "class/common/progress_box.h"
#include "class/common/my_messagebox.h"
#include "include/myinputpanelcontext.h"

CStorageInfo::CStorageInfo(QWidget *parent) :
    QWidget(parent)
{
    /*
        QLabel *m_label_line1;
    QLabel *m_label_line2;
    QLabel *m_label_line3;
    */
    this->setObjectName("CStorageInfo");
    this->setStyleSheet("QWidget#CStorageInfo{border:1px solid #24508F;background-color:#ffffff;}");//#03111E
    this->setFixedSize(620,ROW_HEIGHT * 4);
   #if 1
    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(620,ROW_HEIGHT * 4);
    m_widget_bg->setStyleSheet("QWidget{border:1px solid #24508F;background-color:#03111E;}");
#endif
    for(int index = 0;index < 4;index++)
    {
        if( 0 == index)
            continue;
        QWidget *pWidget = new QWidget(this);
        pWidget->setFixedSize(620,1);
        pWidget->move(0,0 + ROW_HEIGHT * index);
        pWidget->setStyleSheet("QWidget{background-color:#24508F;}");
    }

    QString str_show = tr("使能");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);
#if 0
    m_pushbutton_enable = new CMyPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_enable,620 - 70,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2,70,70);
    m_pushbutton_enable->set_pushbutton_enable(true);
    m_pushbutton_enable->set_pushbutton_sheet("QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/check.png);}",
                                            "QPushButton {border:0px;background:transparent;border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/Non_check.png);}");
#endif
    m_checkbox = new QCheckBox(this);
    m_checkbox->setFixedSize(70,70);
    m_checkbox->move(620 - 70,ROW_HEIGHT * 0 + (ROW_HEIGHT - 70)/2);
    m_checkbox->setStyleSheet(QString(CHECK_BOX_SHEET).arg(CMyDefineScreen::getInstance()->get_change_factor_x(70)).arg(CMyDefineScreen::getInstance()->get_change_factor_y(70)));
    m_checkbox->setFocusPolicy(Qt::NoFocus);

    str_show = tr("容量");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

          //  QLabel *m_label_line2_1;
    //QLabel *m_label_line2_2;
    QString str_show1 = tr("20GB");
    QString str_show2 = tr("/32GB");
    int str_show1_length = CCommonInterface::get_string_length(str_show1,TEXT_FONT);
    int str_show2_length = CCommonInterface::get_string_length(str_show2,TEXT_FONT);

    m_label_line2_1 = new QLabel(this);
    QString str_sheet = "QLabel{border:0px;background:transparent;font:26px;color:#80e3ff;}";
    CCommonInterface::init_label_text(m_label_line2_1,620 - str_show1_length - str_show2_length - 30, ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show1,str_sheet);
    m_label_line2_1->setText("");
    m_label_line2_1->hide();

    m_label_line2_2 = new QLabel(this);
    str_sheet = "QLabel{border:0px;background:transparent;font:26px;color:#94C6FF;}";
    CCommonInterface::init_label_text(m_label_line2_2,620 - 200 - 20, ROW_HEIGHT * 1,200,ROW_HEIGHT,str_show2,str_sheet,Qt::AlignVCenter | Qt::AlignRight);
    m_label_line2_2->setText("");

    str_show = tr("状态");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    str_show = tr("");
    m_label_line3_1 = new QLabel(this);
    str_sheet = "QLabel{border:0px;background:transparent;font:26px;color:#94C6FF;}";
    CCommonInterface::init_label_text(m_label_line3_1,620 - 200 - 20, ROW_HEIGHT * 2,200,ROW_HEIGHT,str_show,str_sheet,Qt::AlignVCenter | Qt::AlignRight);
    //CCommonInterface::init_label_text(m_label_line3_1,620 - 30 - str_length, ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show,str_sheet);

    str_sheet = "QPushButton {\
      border:0px;\
      background:transparent;\
      border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/format.png);\
      font:34px;\
      color:#80e3ff;\
      } \
      QPushButton:pressed {border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/format_hover.png);} \
      QPushButton:hover:!pressed {border-image:url(:/img/res/main_page/menu/system_manage/storage_manage/format_hover.png);}";

    m_pushbutton_ok = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_ok,(620 - 160)/2,ROW_HEIGHT * 3 + (ROW_HEIGHT - 65)/2,160,65,str_sheet);
    m_pushbutton_ok->setText(tr("格式化"));

    connect(m_pushbutton_ok,SIGNAL(clicked()),this,SIGNAL(signal_pushbutton_format()));

    m_define_screen = new CMyDefineScreen();
    m_define_screen->add_parent(this,false);

}

void CStorageInfo::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}


#include <QTimer>

CStorageManageWidget::CStorageManageWidget(QWidget *parent) :
    QWidget(parent)
{
            this->setObjectName("CCameraConfigWidget");
    this->setStyleSheet("QWidget#CCameraConfigWidget{background-color:#ffffff;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->move(0,0);

    QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    pWidget->move(0,0);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");

    m_define_screen = new CMyDefineScreen();

    scroll_layout();
    storage_manage_init_new();

    m_define_screen->add_parent(this);
    m_define_screen->add_parent(m_widget_main);
}

void CStorageManageWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}

void CStorageManageWidget::slot_timer_init()
{
    slot_button_1();
    //m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 70);
}


void CStorageManageWidget::scroll_layout()
{//m_test_widget

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

}



void CStorageManageWidget::storage_manage_init_new()
{
    m_button_1 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_1->setDisplayText(tr("硬盘"));

    m_button_1->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_1->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_1->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    //m_button_1->move(290 - 250,158 - MAIN_HEAD_HEIGHT);

    m_button_2 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_2->setDisplayText(tr("SD卡"));

    m_button_2->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_2->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_2->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_button_3 = new NavigateLeftButton(620,77,NavigateLeftButton::StorageMode,m_widget_main);
    m_button_3->setDisplayText(tr("EMMC"));

    m_button_3->setNormalIconStr(":/img/res/main_page/menu/system_manage/storage_manage/open.png");
    m_button_3->setPressedIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");
    m_button_3->setActiveIconStr(":/img/res/main_page/menu/system_manage/storage_manage/shrink.png");

    m_sotrage_info1 = new CStorageInfo(m_widget_main);
    m_sotrage_info2 = new CStorageInfo(m_widget_main);
    m_sotrage_info3 = new CStorageInfo(m_widget_main);

    connect(m_sotrage_info1,SIGNAL(signal_pushbutton_format()),this,SLOT(slot_pushbutton_format1()));
    connect(m_sotrage_info2,SIGNAL(signal_pushbutton_format()),this,SLOT(slot_pushbutton_format2()));
    connect(m_sotrage_info3,SIGNAL(signal_pushbutton_format()),this,SLOT(slot_pushbutton_format3()));

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
    m_vboxlayout_storage->addWidget(m_sotrage_info1);
    m_vboxlayout_storage->addWidget(m_label_space1);
    m_vboxlayout_storage->addWidget(m_button_2);
    m_vboxlayout_storage->addWidget(m_sotrage_info2);
    m_vboxlayout_storage->addWidget(m_label_space2);
    m_vboxlayout_storage->addWidget(m_button_3);
    m_vboxlayout_storage->addWidget(m_sotrage_info3);

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    m_button_1->hide();
    m_sotrage_info1->hide();
    m_label_space2->hide();
    m_button_3->hide();
    m_sotrage_info3->hide();
    m_sotrage_info2->show();
#endif

    connect(m_button_1,SIGNAL(clicked()),this,SLOT(slot_button_1()));
    connect(m_button_2,SIGNAL(clicked()),this,SLOT(slot_button_2()));
    connect(m_button_3,SIGNAL(clicked()),this,SLOT(slot_button_3()));

    m_button_press_1 = false;
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_sotrage_info1->hide();
    m_sotrage_info2->hide();
    m_sotrage_info3->hide();

#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
    slot_button_2();
#endif

}

void CStorageManageWidget::slot_button_1()
{
    if(m_button_press_1)
    {
        m_button_press_1 = false;
        m_button_1->setSelected(false);
        m_sotrage_info1->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_1 = true;
        m_button_1->setSelected(true);
        m_sotrage_info1->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 100);
    }
    m_button_2->setSelected(false);
    m_button_3->setSelected(false);
    m_button_press_2 = false;
    m_button_press_3 = false;
    m_sotrage_info2->hide();
    m_sotrage_info3->hide();


}
void CStorageManageWidget::slot_button_2()
{
    if(m_button_press_2)
    {
        m_button_press_2 = false;
        m_button_2->setSelected(false);
        m_sotrage_info2->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_2 = true;
        m_button_2->setSelected(true);
        m_sotrage_info2->show();
#if(defined(APPLICATION_TYPE_D2) || defined(APPLICATION_TYPE_P2))
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
#else
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 100);
#endif

    }
    m_button_1->setSelected(false);
    m_button_3->setSelected(false);
    m_button_press_1 = false;
    m_button_press_3 = false;
    m_sotrage_info1->hide();
    m_sotrage_info3->hide();
}
void CStorageManageWidget::slot_button_3()
{
    if(m_button_press_3)
    {
        m_button_press_3 = false;
        m_button_3->setSelected(false);
        m_sotrage_info3->hide();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height);
    }
    else
    {
        m_button_press_3 = true;
        m_button_3->setSelected(true);
        m_sotrage_info3->show();
        m_widget_main->setFixedSize(m_scroll_widget_width,m_scroll_widget_height + 100);
    }
    m_button_1->setSelected(false);
    m_button_2->setSelected(false);
    m_button_press_1 = false;
    m_button_press_2 = false;
    m_sotrage_info1->hide();
    m_sotrage_info2->hide();
}


void CStorageManageWidget::slot_pushbutton_format1()
{
    do
    {

        if(tr("不正常") == m_sotrage_info1->m_label_line3_1->text())
        {
            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("状态不正常，不能格式化"),
            tr("确认"),tr(""));
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
            break;
        }

        if(!password_pass(CProtocol::kEnumHardDisk))
        {
            break;
        }


    }
    while(0);
}
void CStorageManageWidget::slot_pushbutton_format2()
{
    do
    {

        if(tr("不正常") == m_sotrage_info2->m_label_line3_1->text())
        {
            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("状态不正常，不能格式化"),
            tr("确认"),tr(""));
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
            break;
        }

        if(!password_pass(CProtocol::kEnumSDCard))
        {
            break;
        }


    }
    while(0);
}
void CStorageManageWidget::slot_pushbutton_format3()
{
    do
    {

        if(tr("不正常") == m_sotrage_info3->m_label_line3_1->text())
        {
            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("状态不正常，不能格式化"),
            tr("确认"),tr(""));
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
            break;
        }

        if(!password_pass(CProtocol::kEnumEmmc))
        {
            break;
        }



    }
    while(0);
}


bool CStorageManageWidget::password_pass(char _type)
{
    bool ret_value = false;
    MyInputPanelContext *pInputPanel = (MyInputPanelContext *)CCommonInterface::get_method_widget();

    CPasswordDialog::getInstance()->password_init();
    CPasswordDialog::getInstance()->set_password_type(CPasswordDialog::kEnumTypeFormat);
    CPasswordDialog::getInstance()->set_content(tr("格式化密码"),"",CLtyString::getInstance()->m_confirm,CLtyString::getInstance()->m_cancel);
#ifdef APPLICATION_TYPE_P2
#ifdef CURRENT_ARM_RUN
    pInputPanel->set_model_modal(true);
#endif
#endif
    CPasswordDialog::getInstance()->exec();
#ifdef APPLICATION_TYPE_P2
#ifdef CURRENT_ARM_RUN
    pInputPanel->set_model_modal(false);
#endif
#endif
    if(CPasswordDialog::KEnumButtonOk == CPasswordDialog::getInstance()->get_result())
    {
        ret_value = true;

        CProtocol::getInstance()->call_json_system_storage_manage_set(2,_type);


        //CProgressBox::getInstance()->test_init();
        CProgressBox::getInstance()->data_init();
        CProgressBox::getInstance()->set_title(tr("格式化状态"));
        CProgressBox::getInstance()->exec();

        if(CProgressBox::KEnumStatusSuccess == CProgressBox::getInstance()->get_status())
        {

            CMyMessageBox::getInstance()->set_content(tr("格式化状态"),tr("格式化成功，是否\n立即重启设备。"),
            tr("立即重启"),tr("手动重启"));
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
            if(CMyMessageBox::KEnumButtonOk == CMyMessageBox::getInstance()->get_result())
            {
                //调用重启界面及重启
                CProtocol::getInstance()->call_json_system_operate_set(1);
                MainWindow::getInstance()->slot_pushbutton_shutdown();
            }
        }
        else
        {
            if(CProgressBox::KEnumStatusStop == CProgressBox::getInstance()->get_status())
            {
                 //通知取消操作
                 CProtocol::getInstance()->call_json_system_storage_manage_set(3,_type);
            }

            CMyMessageBox::getInstance()->set_content(tr("格式化状态"),tr("格式化失败，请\n重格式化。"),
            tr("重新格式化"),tr("稍后再说"));
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerCancel);
            CMyMessageBox::getInstance()->exec();
            if(CMyMessageBox::KEnumButtonOk == CMyMessageBox::getInstance()->get_result())
            {
                //重新格式化
                password_pass(_type);
            }
        }
    }
    return ret_value;
}
#include <QDebug>
void CStorageManageWidget::slot_storage_manage_widget_event(const CSystemStorageManage &_info)
{
    //qDebug() << "slot_storage_manage_widget_event() start type:" << QString::number(_info.m_type);
    switch(_info.m_type)
    {
        case CProtocol::kEnumHardDisk:
        {
            if(0 == _info.m_enable)
            {
               m_sotrage_info1->m_checkbox->setCheckState(Qt::Unchecked);
               m_sotrage_info1->m_checkbox->setEnabled(true);
            }
            else if(1 == _info.m_enable)
            {
                m_sotrage_info1->m_checkbox->setCheckState(Qt::Checked);
                m_sotrage_info1->m_checkbox->setEnabled(true);
            }
            else if(2 == _info.m_enable)
            {
                m_sotrage_info1->m_checkbox->setEnabled(false);
            }

            //m_sotrage_info1->m_label_line2_1->setText(_info.m_surplus_storage);
            //m_sotrage_info1->m_label_line2_2->setText("/" + _info.m_total_storage);
            m_sotrage_info1->m_label_line2_2->setText(_info.m_total_storage);

            if(1 == _info.m_status)
                m_sotrage_info1->m_label_line3_1->setText(tr("正常"));
            else
                m_sotrage_info1->m_label_line3_1->setText(tr("不正常"));
        }break;
        case CProtocol::kEnumSDCard:
        {
            if(0 == _info.m_enable)
            {
               m_sotrage_info2->m_checkbox->setCheckState(Qt::Unchecked);
               m_sotrage_info2->m_checkbox->setEnabled(true);
            }
            else if(1 == _info.m_enable)
            {
                m_sotrage_info2->m_checkbox->setCheckState(Qt::Checked);
                m_sotrage_info2->m_checkbox->setEnabled(true);
            }
            else if(2 == _info.m_enable)
            {
                m_sotrage_info2->m_checkbox->setEnabled(false);
            }
            //

            //m_sotrage_info2->m_label_line2_1->setText(_info.m_surplus_storage);
            //m_sotrage_info2->m_label_line2_2->setText("/" + _info.m_total_storage);
            m_sotrage_info2->m_label_line2_2->setText(_info.m_total_storage);
            if(1 == _info.m_status)
                m_sotrage_info2->m_label_line3_1->setText(tr("正常"));
            else
                m_sotrage_info2->m_label_line3_1->setText(tr("不正常"));
        }break;
        case CProtocol::kEnumEmmc:
        {
            if(0 == _info.m_enable)
            {
               m_sotrage_info3->m_checkbox->setCheckState(Qt::Unchecked);
               m_sotrage_info3->m_checkbox->setEnabled(true);
            }
            else if(1 == _info.m_enable)
            {
                m_sotrage_info3->m_checkbox->setCheckState(Qt::Checked);
                m_sotrage_info3->m_checkbox->setEnabled(true);
            }
            else if(2 == _info.m_enable)
            {
                m_sotrage_info3->m_checkbox->setEnabled(false);
            }
            //

            m_sotrage_info3->m_label_line2_1->setText(_info.m_surplus_storage);
            m_sotrage_info3->m_label_line2_2->setText("/" + _info.m_total_storage);
            if(1 == _info.m_status)
                m_sotrage_info3->m_label_line3_1->setText(tr("正常"));
            else
                m_sotrage_info3->m_label_line3_1->setText(tr("不正常"));
        }break;
    }
    //qDebug() << "slot_storage_manage_widget_event() end";
}

void CStorageManageWidget::slot_storage_manage_widget_event_ex(char _type,void *_param)
{
    SystemProgressInfo *pSystemProgressInfo = (SystemProgressInfo *)_param;
    ////0 conitue; 1 finish; -1 failed.
    if(1 == pSystemProgressInfo->status)
    {
        CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNormal,100);
    }
    else if(-1 == pSystemProgressInfo->status)
    {
        CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessFail,0);
    }
    else if(0 == pSystemProgressInfo->status)
    {
        qint64 percent = pSystemProgressInfo->current_size * 100/pSystemProgressInfo->total_size;
        CProgressBox::getInstance()->slot_progress_box_event(CProgressBox::kEnumProgreessNormal,percent);
    }
}



