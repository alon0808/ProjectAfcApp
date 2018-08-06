#include "common_query_mode.h"


#include <QDebug>

#include <qapplication.h>
#include <QString>

#include "LtyCommonDefine.h"
#include "common_query_list.h"

CCommonQueryMode::CCommonQueryMode()
{

}
CCommonQueryMode::~CCommonQueryMode()
{

}


CCommonQueryData::CCommonQueryData(int paramType, QObject * parent):CommonTableModel(parent)
{
    m_listType = paramType;
    m_baseModelDataList = (void *)(&m_list);
    m_interval.restart();
    m_column_count = 0;
}
// 行数
int CCommonQueryData::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}
// 列数
int CCommonQueryData::columnCount(const QModelIndex &parent) const
{
    return m_column_count;
    if(CCommonQueryList::kEnumPresetEventList == m_listType)//预设事件
        return 4;
    else if(CCommonQueryList::kEnumImportantMessageList == m_listType)//重要短信
        return 2;
    else if(CCommonQueryList::kEnumNormalMessageList == m_listType)//普通短信
        return 2;
    else if(CCommonQueryList::kEnumVehiclePlanList == m_listType)//行车计划
        return 4;


}
// 获取表数据
QVariant CCommonQueryData::data(const QModelIndex &index, int role) const
{
    if(index.isValid()) {
        if(role == Qt::TextAlignmentRole) {
            return int(Qt::AlignRight | Qt::AlignVCenter);
        } else if(role == Qt::DisplayRole) {
            if(index.isValid()) {
                QVariant var = QVariant::fromValue(m_list.at(index.row()));
                return var;
            }
        }
    }
    return QVariant();
}
// 设置表头内容
QVariant CCommonQueryData::headerData(int section, Qt::Orientation d, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(d == Qt::Vertical)
        return QVariant();

    QVariant var = tr("");


    return var;
}

bool CCommonQueryData::setData(bool allchecked, int from, int length, bool clean)	//取消全部反选再设置选中
{
    if (clean)
    {
        setData(false);
    }

    for (int i = from; i < from + length; i++)
    {
        if ((i >= 0) && (i < m_list.count()))
        {
            m_list[i].ischecked = allchecked;
            m_list[i].isMouseOver = false;
        }
    }

    reset();
    emit actionevent(MODELDATA_EVENT_STOP, -1);
    m_timerForUpdateCheckStatus.start();
    return true;
}

bool CCommonQueryData::setData(bool allchecked, QList<int> moveCheckedRowList)
{
    for (int i = 0; i < moveCheckedRowList.count(); i++)
    {
        int target = moveCheckedRowList.at(i);
        if ((target >= 0) && (target < m_list.count()))
        {
            m_list[target].ischecked = allchecked;
            m_list[target].isMouseOver = false;
        }
    }

    reset();
    emit actionevent(MODELDATA_EVENT_STOP, -1);
    m_timerForUpdateCheckStatus.start();
    return true;
}


// 通过表头的复选框，设置全选
bool CCommonQueryData::setData(bool allchecked)
{
    for(int i = 0; i < m_list.count(); i++)
    {
        m_list[i].ischecked = allchecked;
        m_list[i].isMouseOver = false;   //不管是没有全选或合选，鼠标划过事件不处理
    }
    reset();
    emit actionevent(MODELDATA_EVENT_STOP, -1);
    m_timerForUpdateCheckStatus.start();
    return true;
}
bool CCommonQueryData::setSelect(int _index)
{
    if(_index < m_list.size())
    {
        m_list[_index].ischecked = true;
        m_list[_index].isMouseOver = false;   //不管是没有全选或合选，鼠标划过事件不处理
    }
    reset();
    return true;
}


bool CCommonQueryData::setData(const QModelIndex &index, bool ischeck, bool _isPressed, eViewTableFocus focus, eViewTableMouseEvent mouseevent,bool _check, bool _isUpdateCheck, const QPoint &ps)
{
    //CCommonQueryMode dataex = index.data().value<CCommonQueryMode>();
    bool ispaint = false; int row = index.row();

    //FileMgrModel data = dataex.m_FileMgrModel;

    if(row < 0)
    {
        qDebug() << "LibraryModelData::setData row = " << QString::number(row);
        return false;
    }

    if(m_list[row].ischecked != ischeck)
    {//选择框被改变
        m_list[row].ischecked = ischeck;
        ispaint = true;
        emit actionevent(MODELDATA_EVENT_STOP, row);
    }
    if(m_list[row].isPressed != _isPressed)
    {//选择框被改变
        m_list[row].isPressed = _isPressed;
        ispaint = true;
    }

    if(m_list[row].curMouseEvent != mouseevent)
    {
        m_list[row].curMouseEvent = mouseevent;
        ispaint = true;
    }
    if(m_list[row].curFocus != focus)
    {
        m_list[row].curFocus = focus;
        ispaint = true;
    }
    if (_check)
    {
        onMouseClicked(row);
        m_MouseClickParam.m_MouseEvent = mouseevent;
        m_MouseClickParam.m_Focus = focus;
        m_MouseClickParam.m_row = row;

    }

    if (!ps.isNull())
    {
        m_list[row].m_mouseRightClickPs = ps;
        emit actionevent(MODELMOUSE_EVENT_RIGHT_CLICK, row);
    }

    if(ispaint)
    {
        reset();
    }

    if (evtmeUp == mouseevent)
    {
        if(CCommonQueryList::kEnumNormalMessageList == m_listType)
        {
            if (evtfMore == focus)
            {
                emit actionevent(MODELDATA_EVENT_MORE,row);
            }
        }
        else if(CCommonQueryList::kEnumPresetEventList == m_listType)
        {
            if (evtfSend == focus)
            {
                emit actionevent(MODELDATA_EVENT_SEND,row);
            }
        }

    }

    if (_isUpdateCheck)
    {
        updateTopLevelCheckStatus();
    }

    return ispaint;
}

bool CCommonQueryData::setData(const QModelIndex &index, bool ischeck, bool _isPressed, eViewTableFocus focus, eViewTableMouseEvent mouseevent,bool _check, const QPoint &ps)
{
    //LibraryTableModel dataex = index.data().value<LibraryTableModel>();
    bool ispaint = false; int row = index.row();

    //FileMgrModel data = dataex.m_FileMgrModel;

    if(row < 0)
    {
        qDebug() << "LibraryModelData::setData row = " << QString::number(row);
        return false;
    }

    if(m_list[row].ischecked != ischeck)
    {//选择框被改变
        m_list[row].ischecked = ischeck;
        ispaint = true;
        emit actionevent(MODELDATA_EVENT_STOP, row);
    }
    if(m_list[row].isPressed != _isPressed)
    {//选择框被改变
        m_list[row].isPressed = _isPressed;
        ispaint = true;
    }

    if(m_list[row].curMouseEvent != mouseevent)
    {
        m_list[row].curMouseEvent = mouseevent;
        ispaint = true;
    }
    if(m_list[row].curFocus != focus)
    {
        m_list[row].curFocus = focus;
        ispaint = true;
    }
    if (_check)
    {
        onMouseClicked(row);
        m_MouseClickParam.m_MouseEvent = mouseevent;
        m_MouseClickParam.m_Focus = focus;
        m_MouseClickParam.m_row = row;
    }

    if (!ps.isNull())
    {
        m_list[row].m_mouseRightClickPs = ps;
        emit actionevent(MODELMOUSE_EVENT_RIGHT_CLICK, row);
    }

    if(ispaint)
    {
        reset();
    }
    return ispaint;
}

//------------------------------------------------------------------------------------------
bool CCommonQueryData::setData(const QModelIndex &index, bool ischeck, eViewTableFocus focus, eViewTableMouseEvent mouseevent,bool _check, const QPoint &ps)
{
    //LibraryTableModel dataex = index.data().value<LibraryTableModel>();
    bool ispaint = false; int row = index.row();

    //FileMgrModel data = dataex.m_FileMgrModel;

    if(row < 0)
    {
        qDebug() << "LibraryModelData::setData row = " << QString::number(row);
        return false;
    }

    if(m_list[row].ischecked != ischeck)
    {//选择框被改变
        m_list[row].ischecked = ischeck;
        ispaint = true;
        emit actionevent(MODELDATA_EVENT_STOP, row);
    }
    if(m_list[row].curMouseEvent != mouseevent)
    {
        m_list[row].curMouseEvent = mouseevent;
        ispaint = true;
    }
    if(m_list[row].curFocus != focus)
    {
        m_list[row].curFocus = focus;
        ispaint = true;
    }
    if (_check)
    {
        onMouseClicked(row);
        m_MouseClickParam.m_MouseEvent = mouseevent;
        m_MouseClickParam.m_Focus = focus;
        m_MouseClickParam.m_row = row;

    }

    if (!ps.isNull())
    {
        m_list[row].m_mouseRightClickPs = ps;
        emit actionevent(MODELMOUSE_EVENT_RIGHT_CLICK, row);
    }

    if(ispaint)
    {
        reset();
    }

    return ispaint;
}

void CCommonQueryData::onMouseClicked(int _row)
{
    if (timer.isActive())
    {
        disconnect( &timer, SIGNAL( timeout() ), 0, 0 );  //终止发送单击事件
        timer.stop();       //停止timer
        //执行双击事件
        if (m_MouseClickParam.m_row == _row)
        {
            emit actionevent(MODELDATA_EVENT_BOOT, m_MouseClickParam.m_row);
        }
        else
        {
            timer.start(qApp->doubleClickInterval());
            connect(&timer, SIGNAL(timeout()), this, SLOT( toClicked()));
        }

    }
    else
    {
        timer.start(qApp->doubleClickInterval());
        connect(&timer, SIGNAL(timeout()), this, SLOT( toClicked()));
    }
}

void CCommonQueryData::toClicked()
{
    disconnect( &timer, SIGNAL( timeout() ), 0, 0 );  //终止发送单击事件
    timer.stop();       //停止timer
    //执行单击事件

    if(evtmeUp == m_MouseClickParam.m_MouseEvent)
    {


#if 0
        if(evtfBoot == m_MouseClickParam.m_Focus)
        {
            m_list[m_MouseClickParam.m_row].curState = evtsBoot;
            m_list[m_MouseClickParam.m_row].curFocus = evtfStop;
            emit actionevent(MODELDATA_EVENT_BOOT, m_MouseClickParam.m_row);
        }
        if(evtfStop == m_MouseClickParam.m_Focus)
        {
            m_list[m_MouseClickParam.m_row].curState = evtsStop;
            m_list[m_MouseClickParam.m_row].curFocus = evtfBoot;
            emit actionevent(MODELDATA_EVENT_STOP, m_MouseClickParam.m_row);
        }
        if(evtfExit == m_MouseClickParam.m_Focus)
        {
            emit actionevent(MODELDATA_EVENT_EXIT, m_MouseClickParam.m_row);
        }
#endif
    }

}

int CCommonQueryData::GetCount()
{
    return m_list.count();
}
#if 0
static bool librarySort(CCommonQueryMode First, CCommonQueryMode Second)
{
    return ToolFun::CompareStringAScend(First.m_FileMgrModel.m_FileName, Second.m_FileMgrModel.m_FileName);
}
#endif
void CCommonQueryData::addData(const CCommonQueryMode &_data)
{
#if 0
    for(int i=0; i<m_list.count(); i++)
    {
        if(m_list[i].m_FileMgrModel.m_FilePath == model.m_FilePath)
        {
            return;
        }
    }
#endif

    m_list.push_back(_data);//m_list.push_front(_data);
#if 0
    if (model.m_bGetEnd)
    {
        qSort(m_list.begin(), m_list.end(), librarySort);
    }
#endif
    emit signalCurrentListCount(m_list.count(), m_listType);
    m_timerForUpdateCheckStatus.start();
    reset();
}

void CCommonQueryData::delAllData()
{
    int listTotal = m_list.count();
    for(int i=listTotal - 1; i >= 0; i--)
    {

            if(m_list.at(i).data)
            {
                delete m_list.at(i).data;
                m_list[i].data = NULL;
            }

        m_list.removeAt(i);
    }

   // emit signalViewTableModelClear();
   // emit signalCurrentListCount(m_list.count(), m_listType);
    m_timerForUpdateCheckStatus.start();
    reset();
}
void CCommonQueryData::delData(QString _FileName)
{
#if 0
    for(int i=0; i<m_list.count(); i++)
    {
        if(m_list[i].m_FileMgrModel.m_FileName == _FileName)
        {
            m_list.removeAt(i);
            reset();
            break;
        }
    }
#endif
    emit signalCurrentListCount(m_list.count(), m_listType);
    m_timerForUpdateCheckStatus.start();
}
void CCommonQueryData::delData(int row)
{
    if (row >= 0 && row < m_list.size())
    {
        m_list.removeAt(row);
    }

    emit signalCurrentListCount(m_list.count(), m_listType);
    m_timerForUpdateCheckStatus.start();
    reset();
}

bool CCommonQueryData::GetData(int row, CCommonQueryMode &_data)
{
    if (row >= 0 && row < m_list.size())
    {
        _data = m_list.at(row);
        return true;
    }
    else
    {
        return false;
    }
}
QList<CCommonQueryMode> CCommonQueryData::GetAllData()
{
    return m_list;
}

#if 0
static int gSortIndex = 0;
static int gSortStatus;
static bool CapitySort(const LibraryTableModel First,const LibraryTableModel Second)
{
    bool retVal = false;
    if (0 == gSortIndex)
    {
        if (gSortStatus == E_LOGICAL_UP)
        {
            retVal = (First.m_FileMgrModel.m_FileName > Second.m_FileMgrModel.m_FileName);
        }
        else
        {
            retVal = (First.m_FileMgrModel.m_FileName < Second.m_FileMgrModel.m_FileName);
        }

    }
    if (1 == gSortIndex)
    {
        if (gSortStatus == E_LOGICAL_UP)
        {
            retVal = (First.m_FileMgrModel.m_UpdateTime > Second.m_FileMgrModel.m_UpdateTime);
        }
        else
        {
            retVal = (First.m_FileMgrModel.m_UpdateTime < Second.m_FileMgrModel.m_UpdateTime);
        }

    }
    if (2 == gSortIndex)
    {
        if (gSortStatus == E_LOGICAL_UP)
        {
            retVal = (First.m_FileMgrModel.m_extName > Second.m_FileMgrModel.m_extName);
        }
        else
        {
            retVal = (First.m_FileMgrModel.m_extName < Second.m_FileMgrModel.m_extName);
        }

    }
    if (3 == gSortIndex)
    {
        if (gSortStatus == E_LOGICAL_UP)
        {
            retVal = (First.m_FileMgrModel.m_Size > Second.m_FileMgrModel.m_Size);
        }
        else
        {
            retVal = (First.m_FileMgrModel.m_Size < Second.m_FileMgrModel.m_Size);
        }

    }

    return retVal;
}
#endif

void CCommonQueryData::sortModel(int logicalIndex)
{
#if 0
    int iResult;
    gSortIndex = logicalIndex;
    gSortStatus = signalLibraryTableModelIndex(logicalIndex);
    qSort(m_list.begin(),m_list.end(),CapitySort);
    reset();
#endif
}


void CCommonQueryData::mode_reset()
{
    reset();
}

static bool compare_sort(const CCommonQueryMode &_data1, const CCommonQueryMode &_data2)
{

        CQueryResult *pQueryResult1 = (CQueryResult *)_data1.data;
        CQueryResult *pQueryResult2 = (CQueryResult *)_data2.data;

        if (pQueryResult1->m_date < pQueryResult2->m_date)
            return true;



    return false;
}

void CCommonQueryData::sort_data()
{
    if(CCommonQueryList::kEnumQueryResult == m_listType)
        qSort(m_list.begin(),m_list.end(),compare_sort);

    reset();
}












