#include "common_query_list.h"



#include <QHeaderView>
//#include "Class/toolfun.h"
#include "LtyCommonDefine.h"

#include "class/common/define_screen.h"

#define DEFAULT_ROW_HEIGHT	36


CCommonQueryList::CCommonQueryList(int _listType,int width,int height,QWidget *parent,bool _useMyScroll) :
    CommonList(parent)
{
    m_listType = _listType;


    m_pFileManageTable=new MyTableView(this,_useMyScroll);
    m_pFileManageTable->setIsEnableInlineLeaveEventAndSetMouseOver(true);
    m_ModelData = new CCommonQueryData(m_listType);
    m_ModelData->setTableMode(CommonTableModel::MusicManage);
    m_FileMgrTable = new CCommonQueryTable(this);


    m_pFileManageTable->setModel(m_ModelData);				//设置模型
    m_pFileManageTable->setItemDelegate(m_FileMgrTable);			//设置委托

    m_Header = new CCommonQueryHeader(_listType,Qt::Horizontal,m_pFileManageTable);
    m_Header->setFixedHeight(62);
#if 0
    if(kEnumCallList == _listType
      || kEnumUpgrade == _listType
      || kEnumNormalMessageList == _listType
      || kEnumVehiclePlanList == _listType)
    {
        m_pFileManageTable->horizontalHeader()->setVisible(false);
        m_Header->hide();
    }
    else
#endif
    {
        m_pFileManageTable->setHorizontalHeader(m_Header);
    }

    m_pFileManageTable->move(0,0);
    m_pFileManageTable->setFixedSize(width, height);
    m_pFileManageTable->setOriginTableWidth(width );
    m_pFileManageTable->setOriginTableHeight(height);
    m_pFileManageTable->setOriginRowHeight(CMyDefineScreen::getInstance()->get_change_factor_y(62));

    m_pFileManageTable->setIsRubberBandShow(false);


    //设置默认行列高宽
    m_pFileManageTable->verticalHeader()->setDefaultSectionSize(CMyDefineScreen::getInstance()->get_change_factor_y(62));


    connect(m_Header,SIGNAL(sectionClicked(int)),this,SLOT(slotHeadSectionClicked(int)));
    connect(m_ModelData,SIGNAL(actionevent(int,int)),this,SLOT(slotRowDoubleClicked(int,int)));
    connect(m_ModelData,SIGNAL(signalLibraryTableModelIndex(int)),m_Header,SLOT(slotHeaderViewIndex(int)));
    connect(m_pFileManageTable, SIGNAL(signalCheckRows(int, int)), m_ModelData, SLOT(slotCheckRows(int, int)));
    connect(m_pFileManageTable, SIGNAL(signalShowToolTip(int, int, QPoint)), this, SLOT(slotShowToolTip(int, int, QPoint)));
    connect(m_ModelData, SIGNAL(signalUpdateTopLevelCheckStatus(int)), this, SIGNAL(signalUpdateTopLevelCheckStatus(int)));
    connect(m_ModelData, SIGNAL(signalCurrentListCount(int, int)), this, SIGNAL(signalCurrentListCount(int, int)));
    connect(m_ModelData, SIGNAL(signalUpdateCountInfo(int, int)), this, SIGNAL(signalUpdateCountInfo(int, int)));

#if 0
    m_pMainLayout=new QVBoxLayout(this);
    m_pMainLayout->setSpacing(0);
    m_pMainLayout->setMargin(0);
    m_pMainLayout->addWidget(m_pFileManageTable);
    this->setLayout(m_pMainLayout);
    #endif
}

CCommonQueryList::~CCommonQueryList()
{

}

void CCommonQueryList::ShowGrid(bool _flag)
{
    m_pFileManageTable->setTableViewShowGrid(_flag);
}

void CCommonQueryList::setVerticalScrollBarVisible(bool _flag)
{
    if(_flag)
        m_pFileManageTable->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    else
        m_pFileManageTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void CCommonQueryList::AddData(const CCommonQueryMode &_data)
{
    m_ModelData->addData(_data);
}


void CCommonQueryList::slotRowDoubleClicked(int event,int row)
{
    switch(event)
    {
        case MODELDATA_EVENT_BOOT:
        {
            emit signalRowDoubleClicked(0,row);
        }
        break;
        case MODELDATA_EVENT_STOP:
        {
            emit signalRowDoubleClicked(1,row);
        }
        break;
        case MODELMOUSE_EVENT_RIGHT_CLICK:
        {
            emit signalRowDoubleClicked(2, row); //emit signalsMouseRightClick(row);
        }break;
        case MODELDATA_EVENT_MORE:
        {
            emit signalRowDoubleClicked(3, row);
        }
        break;
        case MODELDATA_EVENT_SEND:
        {
            emit signalRowDoubleClicked(4, row);
        }
        break;
    }
}


void CCommonQueryList::GetRowSelect(QList<CCommonQueryMode> &_list_data,bool _get)
{
    QList<CCommonQueryMode> list = m_ModelData->GetAllData();
    for(int index = 0; index < list.count(); index++)
    {
        if (list.at(index).ischecked || _get)
        {
            _list_data.push_back(list.at(index));
        }
    }
}

bool CCommonQueryList::GetRowColumnSelect(CCommonQueryMode &_data)
{
    bool ret_val = false;
    QList<CCommonQueryMode> list = m_ModelData->GetAllData();

    for(int index = 0; index < list.count(); index++)
    {
        if (list.at(index).m_column >= 0)
        {
            ret_val = true;
            _data = list.at(index);
            break;
        }
    }
    return ret_val;
}

bool CCommonQueryList::GetRowSelect(int _row,CCommonQueryMode &_data)
{
    return m_ModelData->GetData(_row, _data);
}

void CCommonQueryList::slotListClean()
{
    m_ModelData->delAllData();
    //m_Header->resetSort();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//void CCommonQueryList::slotHeadSectionClicked(int logicalIndex)
//单击某字段进行排序
////////////////////////////////////////////////////////////////////////////////////////////////
void CCommonQueryList::slotHeadSectionClicked(int logicalIndex)
{
    m_ModelData->sortModel(logicalIndex);
}

#include <QToolTip>
void CCommonQueryList::slotShowToolTip(int _row, int _column, const QPoint &_point)
{
#if 0
    CVideotapeQueryMode model_data;

    if (GetRowSelect(_row, model_data))
    {
        if (_column == 0)
        {
            QToolTip::showText(_point, libraryTableModel.m_FileMgrModel.m_FileName);
        }
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////
//void CCommonQueryList::ListSelected(bool _AllSelect)
//选中所有或不选择
////////////////////////////////////////////////////////////////////////////////////////////////
void CCommonQueryList::ListSelected(bool _AllSelect)
{
    m_ModelData->setData(_AllSelect);
}

void CCommonQueryList::setSelected(int _index)
{
    m_ModelData->setSelect(_index);
}


int CCommonQueryList::GetListType()
{
    return m_listType;
}

int CCommonQueryList::GetCount()
{
    return m_ModelData->GetCount();
}

void CCommonQueryList::updateTopLevelCheckStatus()
{
    m_ModelData->updateTopLevelCheckStatus();
}

void CCommonQueryList::setHorizontalHeaderVisible(bool paramFlag)
{
   // if (m_pFileManageTable->horizontalHeader()->isVisible() != paramFlag)
    {
        m_pFileManageTable->horizontalHeader()->setVisible(paramFlag);
    }
}

void CCommonQueryList::setColumnWidth(int column, int width,QString _name)
{
    if(kEnumCallList != m_listType)
        m_Header->add_head_name(column,_name,width);


    m_pFileManageTable->setColumnWidth(column,width);
    m_pFileManageTable->horizontalHeader()->setResizeMode(column, QHeaderView::Fixed);
}

void CCommonQueryList::setColumnCount(int _column)
{
    m_ModelData->setColumnCount(_column);
}

#if 0
void CCommonQueryList::ColumnReWidth(int column,int _oldWidth,int _newWidth)
{

     //m_pFileManageTable->setColumnWidth(column,_width);
     //m_pFileManageTable->horizontalHeader()->setResizeMode(column, QHeaderView::Fixed);

}
#endif
#include "class/common_interface.h"
void CCommonQueryList::HandleData(void *_data)
{
    if(kEnumNormalMessageList == m_listType)
    {
        CImportantMessage *pMessage = (CImportantMessage *)_data;
        QString total_show_content = pMessage->m_recv_time + " " + pMessage->m_content;
        if(pMessage)
        {
            int length = CCommonInterface::get_string_length(total_show_content,26);
            if(length > 400)
            {
                CCommonInterface::get_string_limitEx(total_show_content,400,pMessage->m_show_content,26,false,"...");
                pMessage->m_show_content = pMessage->m_show_content.replace("\r\n","");
                pMessage->m_show_content = pMessage->m_show_content.replace("\n","");
                pMessage->m_more = true;
            }
            else
            {
                QString str_content = total_show_content;
                pMessage->m_show_content = str_content.replace("\r\n","");
                pMessage->m_show_content = pMessage->m_show_content.replace("\n","");
            }
            pMessage->m_calculate = true;
        }

    }
}

void CCommonQueryList::list_refresh()
{
    m_ModelData->mode_reset();
}

void CCommonQueryList::list_sort()
{
    m_ModelData->sort_data();
}







