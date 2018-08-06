#include "network_select.h"

#include "../class/common_interface.h"
#include "LtyCommonStyle.h"

#include <QListView>
#include "class/common/my_messagebox.h"
#include "class/ltystring.h"

#define MESSAGEBOX_BUTTON_WIDTH   140
#define MESSAGEBOX_BUTTON_HEIGHT   65

#define STR_CONNECT     tr("连接")
#define STR_CONNECTING  tr("连接中")
#define STR_CONNECTED   tr("已连接")

CNetworkSelect::CNetworkSelect(QWidget *parent) :
    QDialog(parent)
{
    this->setFixedSize(540,485);
    this->setObjectName("CNetworkSelect");
    this->setStyleSheet("QDialog#CNetworkSelect{border:1px solid #275ba7;border-radius: 0px;background-color:#03111f;}");//444a59 //ffffff
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    //标题
    QString str_sheet = "";
    str_sheet = "QLabel{border:0px solid #ffffff;font:46px;background:#275ba7;color:#80e3ff;}";
    m_label_title = new QLabel(this);
    CCommonInterface::init_label_text(m_label_title,1,1,this->width() - 2,77,"",str_sheet);
    m_label_title->setText(tr("选择网络"));

    int list_width = 540;
    int list_height = 268;
    m_select_list = new CCommonQueryList(CCommonQueryList::kEnumWifiSearch
                                              ,list_width,list_height, this,true);

    m_select_list->setColumnCount(2);
#if 1
    m_select_list->setColumnWidth(0,320,tr(""));
    m_select_list->setColumnWidth(1,list_width - 300,tr(""));
#endif
    m_select_list->setFixedSize(list_width,list_height);
    m_select_list->move(0,77);
    m_select_list->setHorizontalHeaderVisible(false);
    //m_select_list->ShowGrid(true);

    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png");

    m_pushbutton_close = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_close,this->width() - 10 - 40,(77 - 52)/2,40,52,str_sheet);


    m_pushbutton1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton1,347 - 242,547 - 166,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_OK);
    m_pushbutton1->setText(CLtyString::getInstance()->m_confirm);

    m_pushbutton2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton2,517 - 242,547 - 166,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_CANCEL);
    m_pushbutton2->setText(CLtyString::getInstance()->m_cancel);

    m_button_type = KEnumButtonCancel;

    select_init();

    connect(m_pushbutton1,SIGNAL(clicked()),this,SLOT(slot_pushbutton1()));
    connect(m_pushbutton2,SIGNAL(clicked()),this,SLOT(slot_pushbutton2()));
    connect(m_pushbutton_connect,SIGNAL(clicked()),this,SLOT(slot_pushbutton_connect()));

    connect(m_pushbutton_close,SIGNAL(clicked()),this,SLOT(slot_pushbutton_close()));


    test_init();
}
void CNetworkSelect::select_init()
{

    m_widget_line1 = new QWidget(this);
    m_widget_line1->setFixedSize(this->width(),1);
    m_widget_line1->move(242 - 242,330 - 166);
    m_widget_line1->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    m_widget_line2 = new QWidget(this);
    m_widget_line2->setFixedSize(this->width(),1);
    m_widget_line2->move(242 - 242,330 - 166 + ROW_HEIGHT);
    m_widget_line2->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");

    m_widget_line3 = new QWidget(this);
    m_widget_line3->setFixedSize(this->width(),1);
    m_widget_line3->move(242 - 242,330 - 166 + ROW_HEIGHT * 2);
    m_widget_line3->setStyleSheet("QWidget{border:0px;background-color:#24508F;}");


    QString str_show = tr("SSID名称");
    m_label_line1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1,30, 77 + ROW_HEIGHT * 0,0,ROW_HEIGHT,str_show);

    str_show = tr("");
    m_label_line1_1 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line1_1,this->width() - 30 - 300, 77 + ROW_HEIGHT * 0,300,ROW_HEIGHT,str_show,"",Qt::AlignVCenter | Qt::AlignRight);

    str_show = tr("安全性");
    m_label_line2 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line2,30, 77 + ROW_HEIGHT * 1,0,ROW_HEIGHT,str_show);

    m_combobox_line2 = new QComboBox(this);
    m_combobox_line2->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_line2->move(522 - 242,77 + ROW_HEIGHT * 1 + (ROW_HEIGHT - COMBOBOX_HEIGHT)/2);
    m_combobox_line2->addItem(tr("WEP"));
    m_combobox_line2->addItem(tr("WPA-PSK"));
    m_combobox_line2->addItem(tr("WPA2-PSK"));
    m_combobox_line2->addItem(tr("WAPI"));
    m_combobox_line2->setView(new QListView());


    str_show = tr("密码");
    m_label_line3 = new QLabel(this);
    CCommonInterface::init_label_text(m_label_line3,30, 77 + ROW_HEIGHT * 2,0,ROW_HEIGHT,str_show);

    m_lineedit3 = new QLineEdit(this);
    m_lineedit3->move(522 - 242,77 + ROW_HEIGHT * 2 + (ROW_HEIGHT - EDIT_HEIGHT)/2);
    m_lineedit3->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit3);
    m_lineedit3->setEchoMode(QLineEdit::Password);

    m_pushbutton_connect = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_connect,432 - 242,547 - 166,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_OK);
    m_pushbutton_connect->setText(STR_CONNECT);

    m_timer = new QTimer(this);
    m_timer->setInterval(1 * 1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timer_connect()));

    m_timer_test = new QTimer(this);
    m_timer_test->setInterval(10 * 1000);
    connect(m_timer_test, SIGNAL(timeout()), this, SLOT(slot_timer_connect_test()));

}
void CNetworkSelect::show_select_network()
{
    m_pushbutton1->show();
    m_pushbutton2->show();
    m_select_list->show();

    m_widget_line1->hide();
    m_widget_line2->hide();
    m_widget_line3->hide();
    m_label_line1->hide();
    m_label_line2->hide();
    m_label_line3->hide();
    m_pushbutton_connect->hide();

    m_label_line1_1->hide();
    m_combobox_line2->hide();
    m_lineedit3->hide();

}
void CNetworkSelect::show_select_connect()
{
    m_pushbutton1->hide();
    m_pushbutton2->hide();
    m_select_list->hide();

    m_widget_line1->show();
    m_widget_line2->show();
    m_widget_line3->show();
    m_label_line1->show();
    m_label_line2->show();
    m_label_line3->show();
        m_label_line1_1->show();
    m_combobox_line2->show();
    m_lineedit3->show();

    m_pushbutton_connect->show();
    m_pushbutton_connect->setText(STR_CONNECT);
}



void CNetworkSelect::set_interface_type(char _type)
{
    switch(_type)
    {
        case kEnumSelectNetwork:
        {
            show_select_network();
        }break;
        case kEnumSelectConnect:
        {
            show_select_connect();
        }break;
    }
}

void CNetworkSelect::slot_pushbutton_close()
{
    if(kEnumStatusConnecting == m_status)
    {


            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("连接中，是否取消操作!"),CLtyString::getInstance()->m_confirm,CLtyString::getInstance()->m_cancel);
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerCancel);
            CMyMessageBox::getInstance()->exec();

            if(CMyMessageBox::KEnumButtonOk ==  CMyMessageBox::getInstance()->get_result())
            {
                CProtocol::getInstance()->call_json_system_wifi_set(CProtocol::kEnumWifiStop,NULL);
                if(m_timer->isActive())
                    m_timer->stop();

                set_interface_type(kEnumSelectNetwork);
                m_status = kEnumStatusNoConnect;
            }
            {
                return;
            }

    }

    slot_pushbutton2();
}


void CNetworkSelect::slot_pushbutton1()
{
    //获取SSID

    do
    {
        QList<CCommonQueryMode> mode_list;
        m_select_list->GetRowSelect(mode_list);

        if(mode_list.size() <= 0)
        {


            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("请从列表选择后，再点确认!"),CLtyString::getInstance()->m_confirm,"");
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();
            break;
        }

        CWifiSearch *wifi_result = (CWifiSearch *)mode_list.at(0).data;

        m_label_line1_1->setText(wifi_result->m_name);

         m_button_type = KEnumButtonOk;
         set_interface_type(kEnumSelectConnect);

        m_status = kEnumStatusNoConnect;

    }while(0);


     //hide();
}
void CNetworkSelect::slot_pushbutton2()
{
     m_button_type = KEnumButtonCancel;
     hide();
}

void CNetworkSelect::slot_pushbutton_connect()
{

    switch(m_status)
    {
        case kEnumStatusNoConnect:
        {

            if(m_lineedit3->text().isEmpty())
            {


                CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("请输入密码!"),CLtyString::getInstance()->m_confirm,"");
                CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
                CMyMessageBox::getInstance()->exec();
                break;
            }

            m_status = kEnumStatusConnecting;
            m_connect_index = 0;
            if(!m_timer->isActive())
            {
               m_timer->start();
            }

            m_CWifiConnectInfo.m_ssid_name = m_label_line1_1->text();//SSID名称
            m_CWifiConnectInfo.m_security = m_combobox_line2->currentIndex();    //安全性 0:WEP  1:WPA-PSK  2:WPA2-PSK  3:WAPI
            m_CWifiConnectInfo.m_password = m_lineedit3->text(); //密码

            CProtocol::getInstance()->call_json_system_wifi_set(CProtocol::kEnumWifiConnect,&m_CWifiConnectInfo);
        }break;
        case kEnumStatusConnecting:
        {


            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("连接中，是否取消操作"),CLtyString::getInstance()->m_confirm,CLtyString::getInstance()->m_cancel);
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerCancel);
            CMyMessageBox::getInstance()->exec();

            if(CMyMessageBox::KEnumButtonOk ==  CMyMessageBox::getInstance()->get_result())
            {
                CProtocol::getInstance()->call_json_system_wifi_set(CProtocol::kEnumWifiStop,NULL);
                if(m_timer->isActive())
                    m_timer->stop();

                set_interface_type(kEnumSelectNetwork);
            }

        }break;
        case kEnumStatusConnected:
        {
            set_interface_type(kEnumSelectNetwork);
        }break;
    }

}

void CNetworkSelect::slot_timer_connect_test()
{
    m_status = kEnumStatusConnected;
    if(m_timer->isActive())
        m_timer->stop();

    m_pushbutton_connect->setText(STR_CONNECTED);
    m_timer_test->stop();
}

void CNetworkSelect::slot_timer_connect()
{
    if(m_connect_index > 3)
    {
        m_connect_index = 0;
    }
    switch(m_connect_index)
    {
        case 0:
            m_pushbutton_connect->setText(STR_CONNECTING);
        break;
        case 1:
            m_pushbutton_connect->setText(STR_CONNECTING + ".");
        break;
        case 2:
            m_pushbutton_connect->setText(STR_CONNECTING + "..");
        break;
        case 3:
            m_pushbutton_connect->setText(STR_CONNECTING + "...");
        break;
    }
    m_connect_index++;
}

CNetworkSelect::ButtonType CNetworkSelect::get_result()
{
    return m_button_type;
}

void CNetworkSelect::test_init()
{
    CCommonQueryMode mode_type;

    CWifiSearch *pWifiSearch = NULL;

    pWifiSearch = new CWifiSearch;
    pWifiSearch->m_name = "WXLTY1";
    pWifiSearch->m_signal = "-70Db";

    mode_type.data = pWifiSearch;
    m_select_list->AddData(mode_type);

        pWifiSearch = new CWifiSearch;
    pWifiSearch->m_name = "WXLTY2";
    pWifiSearch->m_signal = "-70Db";
    pWifiSearch->m_lock = false;
    mode_type.data = pWifiSearch;
    m_select_list->AddData(mode_type);

        pWifiSearch = new CWifiSearch;
    pWifiSearch->m_name = "WXLTY3";
    pWifiSearch->m_signal = "-70Db";

    mode_type.data = pWifiSearch;
    m_select_list->AddData(mode_type);

        pWifiSearch = new CWifiSearch;
    pWifiSearch->m_name = "WXLTY4";
    pWifiSearch->m_signal = "-70Db";

    mode_type.data = pWifiSearch;
    m_select_list->AddData(mode_type);

        pWifiSearch = new CWifiSearch;
    pWifiSearch->m_name = "WXLTY5";
    pWifiSearch->m_signal = "-70Db";

    mode_type.data = pWifiSearch;
    m_select_list->AddData(mode_type);

        pWifiSearch = new CWifiSearch;
    pWifiSearch->m_name = "WXLTY6";
    pWifiSearch->m_signal = "-70Db";

    mode_type.data = pWifiSearch;
    m_select_list->AddData(mode_type);

}

void CNetworkSelect::slot_network_select(char _type,const CWifiSearch &_param)
{

    switch(_type)
    {
        case kEnumGetData:
        {
            if(1 == _param.m_number)
            {
                m_select_list->slotListClean();
            }
            CCommonQueryMode mode_type;
            CWifiSearch *pCWifiSearch = new CWifiSearch;
            pCWifiSearch->m_number = _param.m_number;
            pCWifiSearch->m_name = _param.m_name;
            pCWifiSearch->m_signal = _param.m_signal;
            pCWifiSearch->m_lock = _param.m_lock;

            mode_type.data = pCWifiSearch;
            m_select_list->AddData(mode_type);
        }break;
        case kEnumConnectSuccess:
        {
            m_status = kEnumStatusConnected;

            QList<CCommonQueryMode> mode_list;
            m_select_list->GetRowSelect(mode_list,true);
            for(int index = 0;index < mode_list.size();index++)
            {
                CWifiSearch *pCWifiSearch = (CWifiSearch *)mode_list.at(index).data;
                if(_param.m_name == pCWifiSearch->m_name)
                {
                    pCWifiSearch->m_number = _param.m_number;
                    pCWifiSearch->m_name = _param.m_name;
                    pCWifiSearch->m_signal = _param.m_signal;
                    pCWifiSearch->m_lock = _param.m_lock;
                    break;
                }
            }
            if(m_timer->isActive())
                m_timer->stop();



            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("连接成功!"),CLtyString::getInstance()->m_confirm,"");
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();

            set_interface_type(kEnumSelectNetwork);

        }break;
        case kEnumConnectFail:
        {
            m_status = kEnumStatusFail;

            QList<CCommonQueryMode> mode_list;
            m_select_list->GetRowSelect(mode_list,true);
            for(int index = 0;index < mode_list.size();index++)
            {
                CWifiSearch *pCWifiSearch = (CWifiSearch *)mode_list.at(index).data;
                if(_param.m_name == pCWifiSearch->m_name)
                {
                    pCWifiSearch->m_number = _param.m_number;
                    pCWifiSearch->m_name = _param.m_name;
                    pCWifiSearch->m_signal = _param.m_signal;
                    pCWifiSearch->m_lock = _param.m_lock;
                    break;
                }
            }
            if(m_timer->isActive())
                m_timer->stop();



            CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("连接失败!"),CLtyString::getInstance()->m_confirm,"");
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerOk);
            CMyMessageBox::getInstance()->exec();

            set_interface_type(kEnumSelectNetwork);

        }break;
    }
}













