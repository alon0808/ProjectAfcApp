#include "system_maintain_upgrade.h"


#include "../class/common_interface.h"
#include "LtyCommonStyle.h"
#include "class/ltystring.h"

#include "class/common/my_messagebox.h"

#define MESSAGEBOX_BUTTON_WIDTH   140
#define MESSAGEBOX_BUTTON_HEIGHT   65

SystemMaintainUpgrade::SystemMaintainUpgrade(char _type,QWidget *parent) :
    QDialog(parent)
{
    m_type = _type;

    m_define_screen = new CMyDefineScreen();

    this->setFixedSize(600,77 + 538);
    this->setObjectName("SystemMaintainUpgrade");
    this->setStyleSheet("QDialog#SystemMaintainUpgrade{border:1px solid #275ba7;border-radius: 0px;background-color:#03111f;}");//444a59 //ffffff
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog/*|Qt::WindowStaysOnTopHint*/);

    //标题
    QString str_sheet = "";
    str_sheet = "QLabel{border:0px solid #ffffff;font:46px;background:#275ba7;color:#80e3ff;}";
    m_label_title = new QLabel(this);
    CCommonInterface::init_label_text(m_label_title,1,1,this->width() - 2,77,"",str_sheet);
    m_label_title->setText(tr(""));

    int list_width = 52 + 437;
    int list_height = 70 * 5;
    m_upgrade_list = new CCommonQueryList(CCommonQueryList::kEnumUpgrade
                                              ,m_define_screen->get_change_factor_x(list_width),m_define_screen->get_change_factor_y(list_height), this,true);

    m_upgrade_list->setColumnCount(2);
#if 1
    m_upgrade_list->setColumnWidth(0,CMyDefineScreen::getInstance()->get_change_factor_y(52),tr("使能"));
    m_upgrade_list->setColumnWidth(1,437,tr("录像时间"));
#endif
    m_upgrade_list->setFixedSize(list_width,list_height);
    m_upgrade_list->move((this->width() - list_width)/2 ,77 + 10);
    m_upgrade_list->setHorizontalHeaderVisible(false);


    str_sheet = QString("QPushButton {border:0px;border-image:url(%1); }QPushButton:pressed {border-image:url(%2);}QPushButton:hover:!pressed {border-image:url(%3);}")
        .arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png").arg(":/img/res/main_page/menu/system_manage/close.png");

    m_pushbutton_close = new QPushButton(this);
    CCommonInterface ::init_new_button(m_pushbutton_close,this->width() - 10 - 40,(77 - 52)/2,40,52,str_sheet);
//
    m_pushbutton_search = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton_search,268 - 212,535 - 76,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_CANCEL);
    m_pushbutton_search->setText(tr("搜索"));
    m_pushbutton_search->hide();

    m_pushbutton1 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton1,418 - 212 - 100 + 20,535 - 76,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_OK);
    m_pushbutton1->setText(CLtyString::getInstance()->m_confirm);

    m_pushbutton2 = new QPushButton(this);
    CCommonInterface::init_new_button(m_pushbutton2,568 - 212 - 20,535 - 76,MESSAGEBOX_BUTTON_WIDTH,MESSAGEBOX_BUTTON_HEIGHT,LTY_MESSAGEBOX_CANCEL);
    m_pushbutton2->setText(CLtyString::getInstance()->m_cancel);

    m_button_type = KEnumButtonCancel;



    str_sheet = "QLabel{border:0px solid #ffffff;font:23px;background:transparent;color:#80ffc5;}";
    m_label_udisk_status = new QLabel(this);
    CCommonInterface::init_label_text(m_label_udisk_status,244 - 212,649 - 76 - 10,300,40,"",str_sheet);
    //m_label_udisk_status->setText(tr("提示:U盘挂载成功"));
    m_label_udisk_status->setText("");

    connect(m_pushbutton_search,SIGNAL(clicked()),this,SLOT(slot_pushbutton_search()));

    connect(m_pushbutton1,SIGNAL(clicked()),this,SLOT(slot_pushbutton1()));
    connect(m_pushbutton2,SIGNAL(clicked()),this,SLOT(slot_pushbutton2()));

    connect(m_pushbutton_close,SIGNAL(clicked()),this,SLOT(slot_pushbutton_close()));


    //test_init();
    m_CFindUpgradeFile = NULL;


}

void SystemMaintainUpgrade::resizeEvent(QResizeEvent *e)
{
    m_define_screen->reset_widget();
}


void SystemMaintainUpgrade::set_title(const QString &_title)
{//move(242,82);
    m_label_title->setText(_title);
    static bool is_first = true;
    if(is_first)
    {
        is_first = false;
        this->move(242,82);
        m_define_screen->add_parent(this);
    }
    else
        this->move(CMyDefineScreen::getInstance()->get_change_factor_x(242),CMyDefineScreen::getInstance()->get_change_factor_y(82));

}


void SystemMaintainUpgrade::slot_pushbutton_close()
{
    slot_pushbutton2();
}

void SystemMaintainUpgrade::slot_pushbutton_search()
{

}

void SystemMaintainUpgrade::slot_pushbutton1()
{
     m_button_type = KEnumButtonOk;
     hide();
}
void SystemMaintainUpgrade::slot_pushbutton2()
{
     m_button_type = KEnumButtonCancel;
     hide();
}

////int CCommonInterface::read_dir_file(QString _dir,QStringList &_file_list,QString _filter_name)


SystemMaintainUpgrade::ButtonType SystemMaintainUpgrade::get_result()
{
    return m_button_type;
}

void SystemMaintainUpgrade::test_init()
{
    CCommonQueryMode mode_type;

    CSoftwareUpgrade *pSoftwareUpgrade = NULL;

    pSoftwareUpgrade = new CSoftwareUpgrade;
    pSoftwareUpgrade->m_number = 1;
    pSoftwareUpgrade->m_file_name = "L12B_ALL_001_20161103.sw";

    mode_type.data = pSoftwareUpgrade;
    m_upgrade_list->AddData(mode_type);

        pSoftwareUpgrade = new CSoftwareUpgrade;
    pSoftwareUpgrade->m_number = 2;
    pSoftwareUpgrade->m_file_name = "L12B_ALL_001_20161103.sw";

    mode_type.data = pSoftwareUpgrade;
    m_upgrade_list->AddData(mode_type);

        pSoftwareUpgrade = new CSoftwareUpgrade;
    pSoftwareUpgrade->m_number = 3;
    pSoftwareUpgrade->m_file_name = "L12B_ALL_001_20161103.sw";

    mode_type.data = pSoftwareUpgrade;
    m_upgrade_list->AddData(mode_type);

        pSoftwareUpgrade = new CSoftwareUpgrade;
    pSoftwareUpgrade->m_number = 4;
    pSoftwareUpgrade->m_file_name = "L12B_ALL_001_20161103.sw";

    mode_type.data = pSoftwareUpgrade;
    m_upgrade_list->AddData(mode_type);


        pSoftwareUpgrade = new CSoftwareUpgrade;
    pSoftwareUpgrade->m_number = 5;
    pSoftwareUpgrade->m_file_name = "L12B_ALL_001_20161103.sw";

    mode_type.data = pSoftwareUpgrade;
    m_upgrade_list->AddData(mode_type);

        pSoftwareUpgrade = new CSoftwareUpgrade;
    pSoftwareUpgrade->m_number = 6;
    pSoftwareUpgrade->m_file_name = "L12B_ALL_001_20161103.sw";

    mode_type.data = pSoftwareUpgrade;
    m_upgrade_list->AddData(mode_type);

        pSoftwareUpgrade = new CSoftwareUpgrade;
    pSoftwareUpgrade->m_number = 7;
    pSoftwareUpgrade->m_file_name = "L12B_ALL_001_20161103.sw";

    mode_type.data = pSoftwareUpgrade;
    m_upgrade_list->AddData(mode_type);
}

void SystemMaintainUpgrade::add_data(CSoftwareUpgrade *_data)
{

    do
    {
        CCommonQueryMode mode_type;

        if(NULL == _data)
        {
            break;
        }
        if(1 == _data->m_number)
        {
            m_upgrade_list->slotListClean();
        }
        mode_type.data = _data;
        m_upgrade_list->AddData(mode_type);

        if(1 == _data->m_number)
        {
            m_upgrade_list->setSelected(0);
        }

    }while(0);


}

void *SystemMaintainUpgrade::get_row_info()
{
    void *ret_value = NULL;

    do
    {
        QList<CCommonQueryMode> list_data;

        m_upgrade_list->GetRowSelect(list_data);

        if(list_data.size() <= 0)
        {


            CMyMessageBox::getInstance()->set_content("温馨提示","请选择需要升级的文件!",CLtyString::getInstance()->m_confirm,CLtyString::getInstance()->m_cancel);
            CMyMessageBox::getInstance()->set_timer(CMyMessageBox::KEnumTimerCancel);
            CMyMessageBox::getInstance()->exec();

            break;
        }

        ret_value = list_data.at(0).data;

    }while(0);

    return ret_value;

}

void SystemMaintainUpgrade::slot_upgrade_notify_event(char _type,void *_param)
{
    switch(_type)
    {
        case kEnumMountCreateFail:
        {
            m_label_udisk_status->setText(tr("提示:创建挂载失败"));
        }break;
        case kEnumMountSuccess://挂载U盘成功
        {
            m_label_udisk_status->setText(tr("提示:U盘挂载成功"));
        }break;
        case kEnumMountFail:   //挂载U盘失败
        {
            m_label_udisk_status->setText(tr("提示:U盘挂载失败"));
        }break;
        case kEnumNoFile:      //U盘没有升级文件
        {
            m_label_udisk_status->setText(tr("提示:U盘没有升级文件"));
        }break;
        case kEnumGetFile:      //获取升级文件
        {
            add_data((CSoftwareUpgrade *)_param);
        }break;
        case kEnumClean:
        {
            m_upgrade_list->slotListClean();
        }break;
    }
}

void SystemMaintainUpgrade::start_get_upgrade()
{
    if(NULL == m_CFindUpgradeFile)
    {
        m_CFindUpgradeFile = new CFindUpgradeFile;
        connect(m_CFindUpgradeFile,SIGNAL(signal_upgrade_notify_event(char,void *)),this,SLOT(slot_upgrade_notify_event(char,void *)));
    }
    if(!m_CFindUpgradeFile->isRunning())
    {
        m_CFindUpgradeFile->start();
    }
}

CFindUpgradeFile::CFindUpgradeFile()
{

}
//#define  UDISK_PATH    "/mnt/udisk"
//#define  UPGRADE_PATH  "/mnt/udisk/ltyd2"
/*
        kEnumMountSuccess,//挂载U盘成功
        kEnumMountFail,   //挂载U盘失败
        kEnumNoFile,      //U盘没有升级文件
        kEnumGetFile      //获取升级文件
*/
#include <QDir>

#include <sys/mount.h>

void CFindUpgradeFile::run()
{
    do
    {



        emit signal_upgrade_notify_event(SystemMaintainUpgrade::kEnumClean,NULL);

        int ret = 0;
        //MakeDirectory(UDISK_PATH);
        QDir dir;
        if(!dir.exists(UDISK_PATH))
        {
            if(!dir.mkdir(UDISK_PATH))
            {
                emit signal_upgrade_notify_event(SystemMaintainUpgrade::kEnumMountCreateFail,NULL);
                break;
            }
        }

        umount(UDISK_PATH);
        ret = mount(UDISK_MOUNT_NOD, UDISK_PATH, "vfat", MS_NOSUID | MS_NODEV, "utf8");
        if(ret != 0)
        {
            emit signal_upgrade_notify_event(SystemMaintainUpgrade::kEnumMountFail,NULL);
            break;
        }
        emit signal_upgrade_notify_event(SystemMaintainUpgrade::kEnumMountSuccess,NULL);


        QStringList file_list;
        QString file_name;
        file_list.clear();
        if(0 == CCommonInterface::read_dir_file(UPGRADE_PATH,file_list,"*.sw"))
        {
            for(int index = 0;index < file_list.size();index++)
            {
                CSoftwareUpgrade *pSoftwareUpgrade = new CSoftwareUpgrade;
                pSoftwareUpgrade->m_number = index + 1;
                file_name = file_list.at(index);
                pSoftwareUpgrade->m_file_name = file_name.remove(0,file_name.lastIndexOf('/')+1);

                emit signal_upgrade_notify_event(SystemMaintainUpgrade::kEnumGetFile,pSoftwareUpgrade);
            }

            if(file_list.size() <= 0)
            {
                emit signal_upgrade_notify_event(SystemMaintainUpgrade::kEnumNoFile,NULL);
            }

        }
        else
        {
            emit signal_upgrade_notify_event(SystemMaintainUpgrade::kEnumNoFile,NULL);
        }

    }while(0);
}











