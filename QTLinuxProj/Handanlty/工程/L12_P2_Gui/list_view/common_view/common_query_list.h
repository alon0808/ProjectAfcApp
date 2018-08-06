#ifndef COMMON_QUERY_LIST_H
#define COMMON_QUERY_LIST_H

#include <QWidget>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTableView>


#include "common_query_mode.h"
#include "common_query_header.h"
#include "common_query_table.h"

#include "./class/common/mytableview.h"
#include "class/common/commonlist.h"

class CCommonCallback
{
public:
    CCommonCallback(){}
    virtual void column_width_init(){}
};

class CCommonQueryList : public CommonList
{
    Q_OBJECT
public:
    explicit CCommonQueryList(int _listType,int width,int height,QWidget *parent = 0,bool _useMyScroll = false);
    ~CCommonQueryList();

    enum
    {
        kEnumPresetEventList,           //预设事件
        kEnumImportantMessageList,      //重要短信
        kEnumNormalMessageList,         //普通短信
        kEnumVehiclePlanList,           //行车计划
        kEnumCallList,                  //电话簿
        kEnumSimulation,                //模拟摄像机
        kEnumNetworkStatus,              //网络状态
        kEnumDateSelect,               //录像日期查询
        kEnumQueryResult,               //录像查询结果
        kEnumUpgrade,                  //软件升级
        kEnumWifiSearch                //WIFI查找界面
    };


signals:
    void signalRowDoubleClicked(int _type,int row);  //0:双击 1:单击
    //void signalCurrentListCount(int, int);
    //void signalUpdateTopLevelCheckStatus(int);

private slots:

public slots:
    //void slotRemoveTask(const int);
    void slotListClean();					//清空列表

    void slotRowDoubleClicked(int,int);		//用户双击行的响应槽
    void slotHeadSectionClicked(int);		//列表头字段的点击响应槽
    void slotShowToolTip(int _row, int _column, const QPoint &_point);
public:

    void AddData(const CCommonQueryMode &_model);
    bool GetRowSelect(int _row,CCommonQueryMode &_data);
    void GetRowSelect(QList<CCommonQueryMode> &_list_data,bool _get = false);
    bool GetRowColumnSelect(CCommonQueryMode &_data);
    void setSelected(int _index);

    void ListSelected(bool _AllSelect);
    void HandleData(void *_data);
    void list_sort();

    //void setMouseOver(bool _mouseOver,const QModelIndex &index);
    void slotMouseRightClick(const QPoint &clickps, const QModelIndex &index);
    MyTableView *pFileManageTable()
    {
        return m_pFileManageTable;
    }
    int GetListType();
    int GetCount();
    void updateTopLevelCheckStatus();
    void setHorizontalHeaderVisible(bool paramFlag);
    //void ColumnReWidth(int column,int _oldWidth,int _newWidth);
public:
    void setColumnWidth(int column, int width,QString _name);
    void setColumnCount(int _column);

    void ShowGrid(bool _flag);
    void setVerticalScrollBarVisible(bool);
    void list_refresh();
protected:
    //void leaveEvent(QEvent *);
private:

    MyTableView *m_pFileManageTable;
    CCommonQueryData *m_ModelData;

    QVBoxLayout* m_pMainLayout;

    CCommonQueryHeader *m_Header;
    CCommonQueryTable *m_FileMgrTable;

    int m_listType;
    CCommonCallback *m_CCommonCallback;
};


#endif // VIDEOTAPE_QUERY_LIST_H
