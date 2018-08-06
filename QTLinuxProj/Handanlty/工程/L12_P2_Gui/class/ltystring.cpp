#include "ltystring.h"


CLtyString *CLtyString::m_single = NULL;

#define STATION_STATUS_IN       tr("进站")
#define STATION_STATUS_OUT      tr("出站")

CLtyString *CLtyString::getInstance()
{
    if(NULL == m_single)
    {
        m_single = new CLtyString;
    }
    return m_single;
}

CLtyString::CLtyString()
{
    m_confirm   = tr("确认");
    m_cancel    = tr("取消");
    m_save = tr("保存");
    m_station_in = STATION_STATUS_IN;
    m_station_out = STATION_STATUS_OUT;

}
