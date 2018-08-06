#ifndef VIDEOTAPE_QUERY_LIST_H
#define VIDEOTAPE_QUERY_LIST_H

#include <QWidget>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QTableView>

//#include "../AndroidComdef.h"
#include "videotape_query_mode.h"
#include "videotape_query_header.h"
#include "videotape_query_table.h"


//#include "../AndroidPhone/AndroidPhoneDef.h"

#include "./class/common/mytableview.h"
#include "class/common/commonlist.h"

class CVideotapeQueryList : public CommonList
{
    Q_OBJECT
public:
    explicit CVideotapeQueryList(int _listType,int width,int height,QWidget *parent = 0);
    ~CVideotapeQueryList();


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

    void AddData(const CVideotapeQueryMode &_model);
    bool GetRowSelect(int _row,CVideotapeQueryMode &_data);
    void GetRowSelect(QList<CVideotapeQueryMode> &_list_data,bool _get = false);
    void ListSelected(bool _AllSelect);

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

public:


protected:
    //void leaveEvent(QEvent *);
private:

    MyTableView *m_pFileManageTable;
    CVideotapeQueryData *m_ModelData;

    QVBoxLayout* m_pMainLayout;

    CVideotapeQueryHeader *m_Header;
    CVideotapeQueryTable *m_FileMgrTable;

    int m_listType;
};


#endif // VIDEOTAPE_QUERY_LIST_H
