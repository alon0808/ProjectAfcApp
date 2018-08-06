#ifndef COMMONTABLEMODEL_H
#define COMMONTABLEMODEL_H

#include <QAbstractTableModel>

#include <QTime>
#include <QTimer>

#include "common_data.h"
#include "./class/common/basemodeldata.h"

class CommonTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_ENUMS(TableMode)

public:
    /*
    MusicManage:音乐管理
    MessageManage:短信管理
    FileManage:文件管理
    DownloadManage:下载管理
    ContactManage:通讯录管理
    AppManage:应用管理
    */
    enum TableMode { NoneManage, MusicManage, MessageManage, FileManage, DownloadManage, ContactManage, AppManage};

    CommonTableModel(QObject *parent = 0);
    ~CommonTableModel();

    void *baseModelDataList();
    void updateTopLevelCheckStatus();	//更新顶层checkbox状态
    bool setMouseOver(int index,bool _mouseOver);
    bool setOneRowSelect(int index,bool _select);
    bool setOneRowColumnSelect(int _row,int _column);
    bool setOneRowSelectByRightButton(int index,bool _select);
    void ModelDataReSet(void);
    void setTableMode(TableMode paramTableMode);
    TableMode tableMode();
    bool getCheckInfo(int paramRow, bool &paramIsChecked);
    void setEnableUpdateStatus(bool _bIsEnable);
    int	selectedRowCount();
    int firstSelectedRow();

public:
    void *m_baseModelDataList;


    bool m_isHasPhone;	// 是否有手机连接

protected:
    QTime m_interval;	// 记录两次点击的间隔, 间隔小于100为双击， 超过150为单击
    QTimer timer;		//利用QTimer的超时来触发单击事件
    sMouseClickParam m_MouseClickParam;
    QTimer m_timerForUpdateCheckStatus;
    QTimer m_timerForUpdateListCount;

public slots:
    void slotCheckRows(int from, int length);

private slots:
    void slotTimeOut();

signals:
    void actionevent(int event,int row);
    //void signalViewTableModelClear();	//中间这两个的命名要统一
    //int signalViewTableModelIndex(int);//中间这两个的命名要统一
    void signalRequestUpdateTopLevelCheckStatus();
    void signalUpdateTopLevelCheckStatus(int);
    void signalUpdateCountInfo(int, int);

private:
    TableMode m_tableMode;
    bool m_bEnableUpdateStatus;
    //QList <void *> multiTableModelList;	//根据enum的值确定各个位置放的是什么类型;
};

#endif // COMMONTABLEMODEL_H
