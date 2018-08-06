#include "register_info_widget.h"

#include "LtyCommonDefine.h"
#include "../class/common_interface.h"
#include "LtyCommonStyle.h"

#include "../class/common/comboboxitem.h"
#include "class/ltystring.h"


CRegisterInfoWidget::CRegisterInfoWidget(QWidget *parent) :
    CMyWidget(parent)
{
    m_define_screen = new CMyDefineScreen();

    this->setObjectName("CRegisterInfoWidget");
    this->setStyleSheet("QWidget#CRegisterInfoWidget{background-color:#ffffff;}");
    this->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    this->move(0,0);

    QWidget *pWidget = new QWidget(this);
    pWidget->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH,NAVIGATE_RIGHT_WIDGET_HEIGHT);
    pWidget->move(0,0);
    pWidget->setStyleSheet("QWidget {border:0px;background:#000000;}");

    m_widget_bg = new QWidget(this);
    m_widget_bg->setFixedSize(744,636);
    m_widget_bg->move(250 - 250,107 - MAIN_HEAD_HEIGHT);
    m_widget_bg->setStyleSheet("QWidget{border-image:url(:/img/res/main_page/menu/system_manage/content_background.png);}");


#if 0
    m_pMainLayout = new QVBoxLayout();//左 上 右 下
    m_pMainLayout->setContentsMargins(NAVIGATE_LEFT,24,NAVIGATE_RIGHT,24);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setAlignment(Qt::AlignLeft/* | Qt::AlignTop*/);
    m_pMainLayout->addStretch(0);
    this->setLayout(m_pMainLayout);

    m_widget_main = new QWidget;
    m_widget_main->setFixedSize(NAVIGATE_RIGHT_WIDGET_WIDTH - NAVIGATE_LEFT - NAVIGATE_RIGHT,NAVIGATE_RIGHT_WIDGET_HEIGHT);

    m_widget_main->setStyleSheet("QWidget{background-color:#ffffff;}");
    m_pMainLayout->addWidget(m_widget_main);

    //m_label_test = new QLabel(m_widget_main);
    //m_label_test->setFixedSize(100,30);
    //m_label_test->setText(tr("注册信息"));


#endif
    register_info_init();

    read_data();


    m_focus_index = 0;
    m_define_screen->add_parent(this);

}

void CRegisterInfoWidget::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}


#include <QListView>
void CRegisterInfoWidget::register_info_init()
{


    int str_length = 0;
    QString str_show = tr("线路");
    QString align_text = tr("车辆编号");
    QFont nameFont1;

    CCommonInterface::text_dispersed_align(align_text,str_show,m_define_screen->get_change_factor_y(33),nameFont1);

    QString str_sheet = "QLabel{border:0px solid #8ea2c0;background:transparent;font:33px;color:#8ea2c0;}";
    m_label_line1 = new QLabel(this);
    str_length = CCommonInterface::init_label_text(m_label_line1,430 - 250,201 - MAIN_HEAD_HEIGHT + 24,150,40,str_show,str_sheet,Qt::AlignLeft);
    m_label_line1->setFont(nameFont1);

    m_combobox_1 = new QComboBox(this);
    m_combobox_1->move(588 - 250,186 - MAIN_HEAD_HEIGHT + 24);
    m_combobox_1->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_1->addItem(tr("1路"));
    m_combobox_1->addItem(tr("2路"));
    m_combobox_1->setView(new QListView());

    str_show = tr("线路属性");
    m_label_line1_0 = new QLabel(this);
    str_length = CCommonInterface::init_label_text(m_label_line1_0,430 - 250,303 - MAIN_HEAD_HEIGHT,150,40,str_show,str_sheet,Qt::AlignLeft);


    m_combobox_1_0 = new QComboBox(this);
    m_combobox_1_0->move(588 - 250,288 - MAIN_HEAD_HEIGHT);
    m_combobox_1_0->setFixedSize(COMBOBOX_WIDTH,COMBOBOX_HEIGHT);
    m_combobox_1_0->addItem(tr("普通"));
    m_combobox_1_0->addItem(tr("专线"));
    m_combobox_1_0->setView(new QListView());


    str_show = align_text;
    m_label_line2 = new QLabel(this);
    str_length = CCommonInterface::init_label_text(m_label_line2,430 - 250,303 + 80 * 1 - MAIN_HEAD_HEIGHT,150,40,str_show,str_sheet,Qt::AlignLeft);

    m_lineedit2 = new QLineEdit(this);
    m_lineedit2->move(588 - 250,288 - MAIN_HEAD_HEIGHT + (EDIT_HEIGHT + 12) * 1);
    m_lineedit2->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
    CCommonInterface::set_lineedit_sheet(m_lineedit2);
    m_lineedit2->setText(tr(""));


    str_show = tr("厂商编号");
    m_label_line3 = new QLabel(this);
    str_length = CCommonInterface::init_label_text(m_label_line3,430 - 250,303 + 80 * 2 - MAIN_HEAD_HEIGHT,150,40,str_show,str_sheet,Qt::AlignLeft);
    m_lineedit3 = new QLineEdit(this);
    m_lineedit3->move(588 - 250,288 - MAIN_HEAD_HEIGHT + (EDIT_HEIGHT + 12) * 2);
    m_lineedit3->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);  //
    CCommonInterface::set_lineedit_sheet(m_lineedit3);
    m_lineedit3->setText(tr("L"));

    str_show = tr("车牌号码");
    m_label_line4 = new QLabel(this);
    str_length = CCommonInterface::init_label_text(m_label_line4,430 - 250,303 + 80 * 3 - MAIN_HEAD_HEIGHT,150,40,str_show,str_sheet,Qt::AlignLeft);

    m_combobox_4 = new QComboBox(this);
    m_combobox_4->move(588 - 250,288 - MAIN_HEAD_HEIGHT + (EDIT_HEIGHT + 12) * 3);
    m_combobox_4->setFixedSize(100,COMBOBOX_HEIGHT);

    QStringList combox_list;
    combox_list.clear();
    combox_list <<"粤"<<"冀"<<"豫"<<"云"<<"辽"<<"黑"<<"湘"<<"皖"<<"鲁"<<"新"<<"苏"
    <<"赣"<<"鄂"<<"桂"<<"甘"<<"晋"<<"蒙"<<"陕"<<"吉"<<"闽"<<"贵"<<"青"<<"藏"<<"浙"
    <<"川"<<"宁"<<"琼"<<"渝"<<"京"<<"津"<<"沪";

    for(int index = 0;index < combox_list.size();index++)
        m_combobox_4->addItem(combox_list.at(index));

    m_combobox_4->setView(new QListView());

    m_lineedit4 = new QLineEdit(this);
    m_lineedit4->move(588 - 250 + 105,288 - MAIN_HEAD_HEIGHT + (EDIT_HEIGHT + 12) * 3);
    m_lineedit4->setFixedSize(125,EDIT_HEIGHT);  //
    CCommonInterface::set_lineedit_sheet(m_lineedit4);
    m_lineedit4->setText(tr("888888"));


    str_show = tr("设备ID号");
    m_label_line5 = new QLabel(this);
    str_length = CCommonInterface::init_label_text(m_label_line5,430 - 250,303 + 80 * 3 - MAIN_HEAD_HEIGHT,150,40,str_show,str_sheet,Qt::AlignLeft);
    m_label_line5->hide();

    str_show = "";
    str_sheet = "QLabel{border:1px solid #275ba7;background:transparent;font:33px;color:#8ea2c0;}";
    m_lineedit5 = new QLabel(this);
    CCommonInterface::init_label_text(m_lineedit5,588 - 250,288 - MAIN_HEAD_HEIGHT + (EDIT_HEIGHT + 12) * 3,EDIT_WIDTH,EDIT_HEIGHT,str_show,str_sheet);
    m_lineedit5->hide();

#if 0
    m_lineedit5->move(588 - 250,288 - MAIN_HEAD_HEIGHT + (EDIT_HEIGHT + 12) * 3);
    m_lineedit5->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);  //
    CCommonInterface::set_lineedit_sheet(m_lineedit5);
    m_lineedit5->setText(tr("32323"));
    m_lineedit5->setReadOnly(true);
#endif
    m_pushbutton_ok = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_ok,435 - 250,642 - MAIN_HEAD_HEIGHT,160,65,LTY_PUSHBUTTON_OK);
    m_pushbutton_ok->setText(CLtyString::getInstance()->m_confirm);

    m_pushbutton_cancel = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_cancel,624 - 250,642 - MAIN_HEAD_HEIGHT,160,65,LTY_PUSHBUTTON_OK);
    m_pushbutton_cancel->setText(CLtyString::getInstance()->m_cancel);
    //
    connect(m_pushbutton_ok,SIGNAL(clicked()),this,SLOT(slot_pushbutton_ok()));
    connect(m_pushbutton_cancel,SIGNAL(clicked()),this,SLOT(slot_pushbutton_cancel()));

    m_focus_widget_list.push_back(m_combobox_1);
    m_focus_widget_list.push_back(m_combobox_1_0);
    m_focus_widget_list.push_back(m_lineedit2);
    m_focus_widget_list.push_back(m_lineedit3);
    m_focus_widget_list.push_back(m_combobox_4);
    m_focus_widget_list.push_back(m_lineedit4);
    m_focus_widget_list.push_back(m_pushbutton_ok);
    m_focus_widget_list.push_back(m_pushbutton_cancel);

}
#include "class/common/my_messagebox.h"
#include "class/common/progress_box.h"

void CRegisterInfoWidget::slot_pushbutton_ok()
{
#if 0
    CMyMessageBox message_dialog(this);
    message_dialog.move((1024 - message_dialog.width())/2,(768 - message_dialog.height())/2);
    //message_dialog.move(10,(768 - message_dialog.height())/2);
    message_dialog.set_content(tr("升级提示"),tr("清除行车记录，\n系统无法恢复，请谨慎操作。"),tr("确定"),tr("取消"));
    message_dialog.set_timer(CMyMessageBox::KEnumTimerCancel);
    message_dialog.exec();

    if(CMyMessageBox::KEnumButtonOk ==  message_dialog.get_result())
    {
        CProgressBox box;
        box.move((1024 - box.width())/2,(768 - message_dialog.height())/2);
        box.exec();

    }
#endif
    emit signal_window_close();

}
void CRegisterInfoWidget::slot_pushbutton_cancel()
{
    emit signal_window_close();
}
#include "common_data.h"
void CRegisterInfoWidget::sava_data()
{
    CSystemRegisterInfo info;

    info.m_select_line = m_combobox_1->currentIndex(); //用户选择的线路
    info.m_line_attribute = m_combobox_1_0->currentIndex();
    info.m_car_number = m_lineedit2->text();//车辆编号
    info.m_manufacturer_number  = m_lineedit3->text();//厂商编号
    info.m_select_rovince = m_combobox_4->currentIndex();
    info.m_license_plate = m_lineedit4->text();//车牌号码
    info.m_device_id = m_lineedit5->text();//设备ID号

    if(info.m_select_line != CCommonData::getInstance()->m_CSystemRegisterInfo.m_select_line ||
#ifdef APPLICATION_TYPE_P2
        info.m_line_attribute != CCommonData::getInstance()->m_CSystemRegisterInfo.m_line_attribute ||
#endif
        info.m_car_number != CCommonData::getInstance()->m_CSystemRegisterInfo.m_car_number ||
        info.m_manufacturer_number  != CCommonData::getInstance()->m_CSystemRegisterInfo.m_manufacturer_number ||
        info.m_select_rovince != CCommonData::getInstance()->m_CSystemRegisterInfo.m_select_rovince ||
        info.m_license_plate != CCommonData::getInstance()->m_CSystemRegisterInfo.m_license_plate
        //info.m_device_id != CCommonData::getInstance()->m_CSystemRegisterInfo.m_device_id
    )
    {
        if(CCommonData::getInstance()->messagebox_data_change())
        {
            read_data();
            CCommonData::getInstance()->m_sd_info.m_bus_number = info.m_car_number;
            CProtocol::getInstance()->call_json_system_register_info_set(CProtocol::kEnumSave);
        }
        else
        {
            slot_register_info_widget_event(CCommonData::getInstance()->m_CSystemRegisterInfo);
        }
    }


}

void CRegisterInfoWidget::read_data()
{
    CCommonData::getInstance()->m_CSystemRegisterInfo.m_select_line = m_combobox_1->currentIndex(); //用户选择的线路
    CCommonData::getInstance()->m_CSystemRegisterInfo.m_line_attribute = m_combobox_1_0->currentIndex();
    CCommonData::getInstance()->m_CSystemRegisterInfo.m_car_number = m_lineedit2->text();//车辆编号
    CCommonData::getInstance()->m_CSystemRegisterInfo.m_manufacturer_number  = m_lineedit3->text();//厂商编号
    CCommonData::getInstance()->m_CSystemRegisterInfo.m_select_rovince = m_combobox_4->currentIndex();
    CCommonData::getInstance()->m_CSystemRegisterInfo.m_license_plate = m_lineedit4->text();//车牌号码
    CCommonData::getInstance()->m_CSystemRegisterInfo.m_device_id = m_lineedit5->text();//设备ID号

}

void CRegisterInfoWidget::slot_register_info_widget_event(const CSystemRegisterInfo &_registerInfo)
{
    //int current_index = 0;
    if(_registerInfo.m_line.size() > 0)
    {//线路列表
        m_combobox_1->clear();
        for(int index = 0;index < _registerInfo.m_line.size();index++)
        {
            m_combobox_1->addItem(_registerInfo.m_line.at(index));
            #if 0
            if(_registerInfo.m_line.at(index) == _registerInfo.m_select_line)
            {
                current_index = index;
            }
            #endif
        }
    }
    m_combobox_1->setCurrentIndex(_registerInfo.m_select_line);
    m_combobox_1_0->setCurrentIndex(_registerInfo.m_line_attribute);

    m_lineedit2->setText(_registerInfo.m_car_number);//车辆编号
    m_lineedit3->setText(_registerInfo.m_manufacturer_number);//厂商编号

#if 0
    current_index = 0;
    if(_registerInfo.m_rovince_list.size() > 0) //省份列表 粤　湘
    {
        m_combobox_4->clear();
        for(int index = 0;index < _registerInfo.m_rovince_list.size();index++)
        {
            m_combobox_4->addItem(_registerInfo.m_rovince_list.at(index));
            if(_registerInfo.m_rovince_list.at(index) == _registerInfo.m_select_rovince)
            {
                current_index = index;
            }
        }
    }
#endif

    m_combobox_4->setCurrentIndex(_registerInfo.m_select_rovince);

    m_lineedit4->setText(_registerInfo.m_license_plate);//车牌号码
    m_lineedit5->setText(_registerInfo.m_device_id);//设备ID号

}

void CRegisterInfoWidget::change_focus(int key_code)
{
    switch(key_code)
    {
        case KEYPAD_KEY7_UP:
        {
            m_focus_index--;

            if(m_focus_index < 0)
                m_focus_index = m_focus_widget_list.size()-1;
            m_focus_widget_list.at(m_focus_index)->setFocus();
        }
        break;
        case KEYPAD_KEY7_DOWN:
        {
            m_focus_index++;
            if(m_focus_index >= m_focus_widget_list.size())
                m_focus_index = 0;

            m_focus_widget_list.at(m_focus_index)->setFocus();
        }
        break;
    }
}

void CRegisterInfoWidget::set_focus_first()
{
    m_focus_widget_list.at(0)->setFocus();
}

bool CRegisterInfoWidget::keyboard_value(int _value)
{
    bool ret_value = false;
    switch(_value)
    {
        case KEYPAD_KEY7_F0:
        {

        }break;
        case KEYPAD_KEY7_F1:
        {

        }break;
        case KEYPAD_KEY7_F2:
        {

        }break;
        case KEYPAD_KEY7_F3:
        {

        }break;
        case KEYPAD_KEY7_F4:
        {

        }break;
        case KEYPAD_KEY7_F5:
        {

        }break;
        case KEYPAD_KEY7_F6:
        {

        }break;
        case KEYPAD_KEY7_F7:
        {

        }break;
        case KEYPAD_KEY7_F8:
        {

        }break;
        case KEYPAD_KEY7_F9:
        {

        }break;
        case KEYPAD_KEY7_PLAY:
        {

        }break;
        case KEYPAD_KEY7_REPLAY:
        {

        }break;
        case KEYPAD_KEY7_MENU:
        {


        }break;
        case KEYPAD_KEY7_EXIT:
        {


        }break;
        case KEYPAD_KEY7_FUN:
        {

        }break;
        case KEYPAD_KEY7_UP:
        {
            change_focus(_value);

        }break;
        case KEYPAD_KEY7_LEFT:
        {

        }break;
        case KEYPAD_KEY7_RIGHT:
        {

        }break;
        case KEYPAD_KEY7_DOWN:
        {
            change_focus(_value);
        }break;
    }
    return ret_value;
}












