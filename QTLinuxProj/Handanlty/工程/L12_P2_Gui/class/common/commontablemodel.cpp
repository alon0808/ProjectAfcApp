
#include "commontablemodel.h"
#include "LtyCommonDefine.h"
#include <QDebug>

CommonTableModel::CommonTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

    m_bEnableUpdateStatus = true;

    m_isHasPhone = false;
    m_tableMode = NoneManage;
    m_baseModelDataList = NULL;

    m_timerForUpdateCheckStatus.setInterval(50);
    m_timerForUpdateListCount.setInterval(50);

    connect(&m_timerForUpdateCheckStatus, SIGNAL(timeout()), this, SLOT(slotTimeOut()));
    //connect(this, SIGNAL(signalRequestUpdateTopLevelCheckStatus()), this, SLOT(slotTimeOut()));
}

CommonTableModel::~CommonTableModel()
{

}

void *CommonTableModel::baseModelDataList()
{
    return m_baseModelDataList;
}

void CommonTableModel::updateTopLevelCheckStatus()	//更新顶层checkbox状态
{
    if (!m_bEnableUpdateStatus)
    {
        return;
    }

    bool uncheckFlagTmp = false;
    bool checkFlagTmp = false;

    if (NULL == m_baseModelDataList) return;

    QList<BaseModelData > *modelDataPtrListTmp = (QList<BaseModelData> *)m_baseModelDataList;

    int selectedItemCount = 0;
    int itemCount = modelDataPtrListTmp->count();
    for (int i = 0; i < modelDataPtrListTmp->count(); i++)
    {
        if (modelDataPtrListTmp->at(i).ischecked)
        {
            checkFlagTmp = true;
            selectedItemCount++;
        }
        else
        {
            uncheckFlagTmp = true;
        }
    }

    emit signalUpdateCountInfo(selectedItemCount, itemCount);

    if ((uncheckFlagTmp == true) && (checkFlagTmp == true))			//部分选择
    {
        emit signalUpdateTopLevelCheckStatus(Qt::PartiallyChecked);
    }
    else if((uncheckFlagTmp == false) && (checkFlagTmp == true))	//全部选择
    {
        emit signalUpdateTopLevelCheckStatus(Qt::Checked);
    }
    else if ((uncheckFlagTmp == true) && (checkFlagTmp == false))	//全部未选
    {
        emit signalUpdateTopLevelCheckStatus(Qt::Unchecked);
    }
    else
    {
        emit signalUpdateTopLevelCheckStatus(Qt::Unchecked);
    }
}

bool CommonTableModel::setMouseOver(int index,bool _mouseOver)
{

    bool isPaint = false;
    if (NULL == m_baseModelDataList) return isPaint;

    QList<BaseModelData > *baseModelDataListTmp = (QList<BaseModelData> *)m_baseModelDataList;

    if ((-1 == index) || (index >= baseModelDataListTmp->count())) {
        return isPaint;
    }

    if (baseModelDataListTmp->at(index).isMouseOver != _mouseOver)
    {
        BaseModelData baseModelDataTmp = baseModelDataListTmp->at(index);
        baseModelDataTmp.isMouseOver = _mouseOver;
        baseModelDataListTmp->replace(index, baseModelDataTmp);

        isPaint = true;
    }

    if (isPaint)
    {
        reset();
    }

    return isPaint;
}

bool CommonTableModel::setOneRowSelect(int index,bool _select)
{
    bool isPaint = false;
    if (NULL == m_baseModelDataList) return isPaint;

    QList<BaseModelData > *baseModelDataListTmp = (QList<BaseModelData> *)m_baseModelDataList;

    if ((-1 == index) || (index >= baseModelDataListTmp->count())) {
        return isPaint;
    }

    BaseModelData baseModelDataTmp;
    for (int iRow = 0;iRow < baseModelDataListTmp->size();iRow++)
    {
        if (index  == iRow)
        {
            if (_select != baseModelDataListTmp->at(iRow).ischecked)
            {
                baseModelDataTmp = baseModelDataListTmp->at(iRow);
                baseModelDataTmp.ischecked = _select;
                baseModelDataListTmp->replace(iRow, baseModelDataTmp);
                isPaint = true;
            }
        }
        else
        {
            baseModelDataTmp = baseModelDataListTmp->at(iRow);
            baseModelDataTmp.ischecked = false;
            baseModelDataListTmp->replace(iRow, baseModelDataTmp);
            isPaint = true;
        }
    }
    if (m_tableMode == MusicManage)
    {
        emit actionevent(MODELDATA_EVENT_STOP, -1);	//特殊：除了music其他setOneRowSelect没这句
    }
    if (isPaint)
    {
        reset();
        //updateTopLevelCheckStatus();
        //emit signalRequestUpdateTopLevelCheckStatus();
        slotTimeOut();
    }

    return isPaint;
}
bool CommonTableModel::setOneRowColumnSelect(int _row,int _column)
{
    bool isPaint = false;
    if (NULL == m_baseModelDataList) return isPaint;

    QList<BaseModelData > *baseModelDataListTmp = (QList<BaseModelData> *)m_baseModelDataList;

    if ((-1 == _row) || (_row >= baseModelDataListTmp->count())) {
        return isPaint;
    }

    BaseModelData baseModelDataTmp;
    for (int iRow = 0;iRow < baseModelDataListTmp->size();iRow++)
    {
        if (_row  == iRow)
        {
            baseModelDataTmp = baseModelDataListTmp->at(iRow);
            baseModelDataTmp.m_row = _row;
            baseModelDataTmp.m_column = _column;
            baseModelDataListTmp->replace(iRow, baseModelDataTmp);
            isPaint = true;
        }
        else
        {
            baseModelDataTmp = baseModelDataListTmp->at(iRow);
            baseModelDataTmp.m_row = _row;
            baseModelDataTmp.m_column = -1;
            baseModelDataListTmp->replace(iRow, baseModelDataTmp);
            isPaint = true;
        }
    }

    if (isPaint)
    {
        reset();

        slotTimeOut();
    }

    return isPaint;
}
bool CommonTableModel::setOneRowSelectByRightButton(int index,bool _select)
{
    bool isPaint = false;
    if (NULL == m_baseModelDataList) return isPaint;

    QList<BaseModelData > *baseModelDataListTmp = (QList<BaseModelData> *)m_baseModelDataList;

    if ((-1 == index) || (index >= baseModelDataListTmp->count())) {
        return isPaint;
    }

    BaseModelData baseModelDataTmp;

    if (baseModelDataListTmp->at(index).ischecked)
    {
        isPaint = true;
    }
    else
    {
        for (int iRow = 0;iRow < baseModelDataListTmp->size();iRow++)
        {
            if (index  == iRow)
            {
                if (_select != baseModelDataListTmp->at(iRow).ischecked)
                {
                    baseModelDataTmp = baseModelDataListTmp->at(iRow);
                    baseModelDataTmp.ischecked = _select;
                    baseModelDataListTmp->replace(iRow, baseModelDataTmp);
                    isPaint = true;
                }
            }
            else
            {
                baseModelDataTmp = baseModelDataListTmp->at(iRow);
                baseModelDataTmp.ischecked = false;
                baseModelDataListTmp->replace(iRow, baseModelDataTmp);
                isPaint = true;
            }
        }
    }

    if (m_tableMode == MusicManage)
    {
        emit actionevent(MODELDATA_EVENT_STOP, -1);	//特殊：除了music其他setOneRowSelect没这句
    }
    if (isPaint)
    {
        reset();
        //updateTopLevelCheckStatus();
        //emit signalRequestUpdateTopLevelCheckStatus();
        slotTimeOut();
    }

    return isPaint;
}

void CommonTableModel::ModelDataReSet(void)
{
    reset();
}

void CommonTableModel::setTableMode(TableMode paramTableMode)
{
    m_tableMode = paramTableMode;
}

CommonTableModel::TableMode CommonTableModel::tableMode()
{
    return m_tableMode;
}

bool CommonTableModel::getCheckInfo(int paramRow, bool &paramIsChecked)
{
    bool retTmp = false;
    if (NULL == m_baseModelDataList) return retTmp;
    QList<BaseModelData > *baseModelDataListTmp = (QList<BaseModelData> *)m_baseModelDataList;

    if (paramRow < 0 || paramRow >= baseModelDataListTmp->count())
    {
        retTmp = false;
        return retTmp;
    }

    paramIsChecked = baseModelDataListTmp->at(paramRow).ischecked;
    retTmp = true;

    return retTmp;

}

void CommonTableModel::setEnableUpdateStatus(bool _bIsEnable)
{
    if (m_bEnableUpdateStatus != _bIsEnable)
    {
        m_bEnableUpdateStatus = _bIsEnable;
    }
}

int	CommonTableModel::selectedRowCount()
{
    if (NULL == m_baseModelDataList) return -1;

    QList<BaseModelData > *baseModelDataListTmp = (QList<BaseModelData> *)m_baseModelDataList;


    int countTmp = baseModelDataListTmp->count();
    int ret = 0;
    for (int i = 0; i < countTmp; i++)
    {
        if (baseModelDataListTmp->at(i).ischecked)
        {
            ret++;
        }
    }
    return ret;
}

int CommonTableModel::firstSelectedRow()
{
    if (NULL == m_baseModelDataList) return -1;

    QList<BaseModelData > *baseModelDataListTmp = (QList<BaseModelData> *)m_baseModelDataList;


    int countTmp = baseModelDataListTmp->count();
    int ret = -1;
    for (int i = 0; i < countTmp; i++)
    {
        if (baseModelDataListTmp->at(i).ischecked)
        {
            ret = i;
            break;
        }
    }
    return ret;
}

void CommonTableModel::slotCheckRows(int from, int length)
{
    if (NULL != m_baseModelDataList)
    {
        QList<BaseModelData > *baseModelDataListTmp = (QList<BaseModelData> *)m_baseModelDataList;

        if (NULL == baseModelDataListTmp) return;

        int iSelectRow;
        bool bRepaint = false;
        QList<int> parentRowListTmp;

        for (iSelectRow = 0; iSelectRow < baseModelDataListTmp->count(); iSelectRow++)
        {
            BaseModelData baseModelDataTmp = baseModelDataListTmp->at(iSelectRow);
            if (baseModelDataTmp.ischecked)
            {
                baseModelDataTmp.ischecked = false;
                bRepaint = true;
            }
            if (baseModelDataTmp.isMouseOver)
            {
                baseModelDataTmp.isMouseOver = false;
                bRepaint = true;
            }
            if (baseModelDataTmp.isPressed)
            {
                baseModelDataTmp.isPressed = false;
                bRepaint = true;
            }
            if (bRepaint)
            {
                baseModelDataListTmp->replace(iSelectRow, baseModelDataTmp);
            }
        }

        for (iSelectRow = from; iSelectRow < from + length; iSelectRow++)
        {
            if (iSelectRow < baseModelDataListTmp->count())
            {
                BaseModelData baseModelDataTmp = baseModelDataListTmp->at(iSelectRow);
                if (!baseModelDataTmp.ischecked)
                {
                    baseModelDataTmp.ischecked = true;
                    bRepaint = true;
                }
                if (baseModelDataTmp.isMouseOver)
                {
                    baseModelDataTmp.isMouseOver = false;
                    bRepaint = true;
                }
                if (baseModelDataTmp.isPressed)
                {
                    baseModelDataTmp.isPressed = false;
                    bRepaint = true;
                }
                if (bRepaint)
                {
                    baseModelDataListTmp->replace(iSelectRow, baseModelDataTmp);
                }
            }
        }

        if (bRepaint)
        {
            slotTimeOut();
            //emit signalRequestUpdateTopLevelCheckStatus();
            //updateTopLevelCheckStatus();
            reset();
        }
    }
}

void CommonTableModel::slotTimeOut()
{
    m_timerForUpdateCheckStatus.stop();
    updateTopLevelCheckStatus();
    emit signalRequestUpdateTopLevelCheckStatus();
}
