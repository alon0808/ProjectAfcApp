#include "common_data.h"

#include "class/common/my_messagebox.h"
#include "class/ltystring.h"
#include "LtyCommonStyle.h"

CCommonData *CCommonData::m_single = NULL;


/*
    CMobileNetworkInfo *m_CMobileNetworkInfo;
    CCenterServerInfo *m_CCenterServerInfo1;
    CCenterServerInfo *m_CCenterServerInfo2;
    CVideoServerInfo *m_CVideoServerInfo;
    CLocalNetworkInfo *m_CLocalNetworkInfo;
    CWifiInfo *m_CWifiInfo;
    CFtpInfo *m_CFtpInfo;
*/

CCommonData::CCommonData(QWidget *parent) :
    QWidget(parent)
{

    m_CSystemNetwork.m_CMobileNetworkInfo = new CMobileNetworkInfo;
    m_CSystemNetwork.m_CCenterServerInfo1 = new CCenterServerInfo;
    m_CSystemNetwork.m_CCenterServerInfo2 = new CCenterServerInfo;
    m_CSystemNetwork.m_CVideoServerInfo = new CVideoServerInfo;
    m_CSystemNetwork.m_CLocalNetworkInfo = new CLocalNetworkInfo;
    m_CSystemNetwork.m_CWifiInfo = new CWifiInfo;
    m_CSystemNetwork.m_CFtpInfo = new CFtpInfo;


    m_runStatusInfo.m_CRunStatusMobileNetwork = new CRunStatusMobileNetwork;
    m_runStatusInfo.m_CRunStatusIO = new CRunStatusIO;
    m_runStatusInfo.m_CRunStatusGPS = new CRunStatusGPS;
    m_runStatusInfo.m_CRunStatusLocalNetwork = new CRunStatusLocalNetwork;
    m_runStatusInfo.m_CRunStatusDevice = new CRunStatusDevice;
    m_runStatusInfo.m_CStationInfo = new CStationStatus;
    m_runStatusInfo.m_CPeripheralStatus = new CPeripheralStatus;


    m_CSystemVideotapeQueryEx.m_CSystemVideotapeQuery = new CSystemVideotapeQuery;
    m_CSystemVideotapeQueryEx.m_VideotapeQueryDate = new VideotapeQueryDate;
    m_CSystemVideotapeQueryEx.m_VideotapeQueryList = new VideotapeQueryList;
    m_CSystemVideotapeQueryEx.m_CSystemVideoExport = new CSystemVideoExport;

    m_define_screen = new CMyDefineScreen();


    m_play_picture_list.clear();
    m_naviFinish = 0;
    m_snNum.clear();
	m_tmp_status = 3;
}
CCommonData *CCommonData::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CCommonData;
    }
    return m_single;
}

bool CCommonData::messagebox_data_change()
{


    CMyMessageBox::getInstance()->set_content(tr("温馨提示"),tr("数据已变化，是否保存?"),CLtyString::getInstance()->m_save,CLtyString::getInstance()->m_cancel);
    CMyMessageBox::getInstance()->exec();

    if(CMyMessageBox::KEnumButtonOk ==  CMyMessageBox::getInstance()->get_result())
    {
        return true;
    }
    return false;
}
#define SCROLL_SIZE 50
QString CCommonData::get_scroll_sheet()
{
    QString str_number1 = "59";//QString::number(m_define_screen->get_change_factor_y(59));

#ifdef APPLICATION_TYPE_P2
    str_number1 = "37";
#else
    str_number1 = "59";
#endif

    return "QScrollArea{border: 0px solid #275ba7;background:transparent;}"\
    "QScrollBar:vertical{width:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;background:transparent;margin:%1px," + str_number1 + "px,0px,0px;padding-top:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;padding-bottom:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;}"\
    "QScrollBar::handle:vertical{width:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;border-image:url(:/img/res/main_page/menu/system_manage/content_scrollbar.png);border-radius:5px;min-height:100px;}"\
    "QScrollBar::handle:vertical:hover{width:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;border-image:url(:/img/res/main_page/menu/system_manage/content_scrollbar.png);border-radius:5px;min-height:100px;}"\
    "QScrollBar::add-line:vertical{height:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;width:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;border-image:url(:/img/res/main_page/menu/system_manage/content_down_default.png);subcontrol-position:bottom;}"\
    "QScrollBar::sub-line:vertical{height:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;width:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;border-image:url(:/img/res/main_page/menu/system_manage/content_up_default.png);subcontrol-position:top;}"\
    "QScrollBar::add-line:vertical:hover{height:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;width:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;border-image:url(:/img/res/main_page/menu/system_manage/content_down_selected.png);subcontrol-position:bottom;}"\
    "QScrollBar::sub-line:vertical:hover{height:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;width:"+ QString::number(m_define_screen->get_change_factor_y(SCROLL_SIZE)) + "px;border-image:url(:/img/res/main_page/menu/system_manage/content_up_selected.png);subcontrol-position:top;}"\
    "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:transparent;}";


}

int CCommonData::get_message_box_quit()
{
    int ret_value = 5;
    //0:5s 1:10s 2:20s 3:30s 4:60s 5:永不
    if(0 == m_CDriverMenuInfo.message_outtime)
        ret_value = 5;
    else if(1 == m_CDriverMenuInfo.message_outtime)
        ret_value = 10;
    else if(2 == m_CDriverMenuInfo.message_outtime)
        ret_value = 20;
    else if(3 == m_CDriverMenuInfo.message_outtime)
        ret_value = 30;
    else if(4 == m_CDriverMenuInfo.message_outtime)
        ret_value = 60;
    else if(5 == m_CDriverMenuInfo.message_outtime)
        ret_value = 100;

    return ret_value;
}





















