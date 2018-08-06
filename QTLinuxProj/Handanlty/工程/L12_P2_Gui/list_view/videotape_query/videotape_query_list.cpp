#include "videotape_query_list.h"



#include <QHeaderView>
//#include "Class/toolfun.h"
#include "LtyCommonDefine.h"

#define DEFAULT_ROW_HEIGHT	36


CVideotapeQueryList::CVideotapeQueryList(int _listType,int width,int height,QWidget *parent) :
    CommonList(parent)
{
    m_listType = _listType;


    m_pFileManageTable=new MyTableView(this);
    m_pFileManageTable->setIsEnableInlineLeaveEventAndSetMouseOver(true);
    m_ModelData = new CVideotapeQueryData(m_listType);
    m_ModelData->setTableMode(CommonTableModel::MusicManage);
    m_FileMgrTable = new CVideotapeQueryTable(this);


    m_pFileManageTable->setModel(m_ModelData);				//设置模型
    m_pFileManageTable->setItemDelegate(m_FileMgrTable);			//设置委托

    m_Header = new CVideotapeQueryHeader(_listType,Qt::Horizontal,m_pFileManageTable);
    m_Header->setFixedHeight(60);
    m_pFileManageTable->setHorizontalHeader(m_Header);
    //m_pFileManageTable->horizontalHeader()->setVisible(false);

    m_pFileManageTable->setColumnWidth(0,50);                       //序号
    m_pFileManageTable->setColumnWidth(1,70);                       //开始时间
    m_pFileManageTable->setColumnWidth(2,70);                       //结束时间
    m_pFileManageTable->setColumnWidth(3,70);                       //时长（秒）
    m_pFileManageTable->setColumnWidth(4,50);                       //通道
    m_pFileManageTable->setColumnWidth(5,width - 50 - 70 * 3 - 50);	//使能

    m_pFileManageTable->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    m_pFileManageTable->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    m_pFileManageTable->horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    m_pFileManageTable->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    m_pFileManageTable->horizontalHeader()->setResizeMode(4, QHeaderView::Fixed);
    m_pFileManageTable->horizontalHeader()->setResizeMode(5, QHeaderView::Fixed);

    //m_pFileManageTable->hideColumn(4);

    //m_pFileManageTable->setGeometry(0, 0, width, height);
    m_pFileManageTable->move(0,0);
    m_pFileManageTable->setFixedSize(width, height);
    m_pFileManageTable->setOriginTableWidth(width);
    m_pFileManageTable->setOriginTableHeight(height);
    m_pFileManageTable->setOriginRowHeight(60);

    m_pFileManageTable->setIsRubberBandShow(false);


    //设置默认行列高宽
    m_pFileManageTable->verticalHeader()->setDefaultSectionSize(60);


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

CVideotapeQueryList::~CVideotapeQueryList()
{

}

void CVideotapeQueryList::AddData(const CVideotapeQueryMode &_data)
{
    m_ModelData->addData(_data);
}


void CVideotapeQueryList::slotRowDoubleClicked(int event,int row)
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
            }
            break;
    }
}


void CVideotapeQueryList::GetRowSelect(QList<CVideotapeQueryMode> &_list_data,bool _get)
{
    QList<CVideotapeQueryMode> list = m_ModelData->GetAllData();
    for(int index = 0; index < list.count(); index++)
    {
        if (list.at(index).ischecked || _get)
        {
            _list_data.push_back(list.at(index));
        }
    }
}

bool CVideotapeQueryList::GetRowSelect(int _row,CVideotapeQueryMode &_data)
{
    return m_ModelData->GetData(_row, _data);
}

void CVideotapeQueryList::slotListClean()
{
    m_ModelData->delAllData();
    m_Header->resetSort();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//void CVideotapeQueryList::slotHeadSectionClicked(int logicalIndex)
//单击某字段进行排序
////////////////////////////////////////////////////////////////////////////////////////////////
void CVideotapeQueryList::slotHeadSectionClicked(int logicalIndex)
{
    m_ModelData->sortModel(logicalIndex);
}

#include <QToolTip>
void CVideotapeQueryList::slotShowToolTip(int _row, int _column, const QPoint &_point)
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
//void CVideotapeQueryList::ListSelected(bool _AllSelect)
//选中所有或不选择
////////////////////////////////////////////////////////////////////////////////////////////////
void CVideotapeQueryList::ListSelected(bool _AllSelect)
{
    m_ModelData->setData(_AllSelect);
}

int CVideotapeQueryList::GetListType()
{
    return m_listType;
}

int CVideotapeQueryList::GetCount()
{
    return m_ModelData->GetCount();
}

void CVideotapeQueryList::updateTopLevelCheckStatus()
{
    m_ModelData->updateTopLevelCheckStatus();
}

void CVideotapeQueryList::setHorizontalHeaderVisible(bool paramFlag)
{
    if (m_pFileManageTable->horizontalHeader()->isVisible() != paramFlag)
    {
        m_pFileManageTable->horizontalHeader()->setVisible(paramFlag);
    }
}




