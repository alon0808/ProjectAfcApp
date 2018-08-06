#ifndef COMMON_QUERY_MODE_H
#define COMMON_QUERY_MODE_H

#include <QWidget>


#include <QWidget>
#include <QString>
#include <QLabel>
#include <QMovie>
#include <QAbstractTableModel>
#include <QTime>
#include <QTimer>

#include "common_data.h"

#include "./class/common/basemodeldata.h"
#include "./class/common/commontablemodel.h"


class CCommonQueryMode : public BaseModelData
{
public:
    CCommonQueryMode();
    ~CCommonQueryMode();

public:
    /*CVideotape model_data;*/
    void *data;

};

Q_DECLARE_METATYPE(CCommonQueryMode)






class CCommonQueryData : public CommonTableModel
{
    Q_OBJECT
public:
    CCommonQueryData(int paramType, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation d, int role) const;


    bool setData(bool allchecked, int from, int length, bool clean);	//取消全部反选再设置选中
    bool setData(bool allchecked, QList<int> moveCheckedRowList);
    bool setData(bool allchecked);
    bool setData(const QModelIndex &index, bool ischeck,  bool _isPressed, eViewTableFocus focus, eViewTableMouseEvent mouseevent,bool _check, bool _isUpdateCheck, const QPoint &ps);
    bool setData(const QModelIndex &index, bool ischeck, bool _isPressed, eViewTableFocus focus, eViewTableMouseEvent mouseevent,bool _check, const QPoint &ps);
    bool setData(const QModelIndex &index, bool ischeck, eViewTableFocus focus, eViewTableMouseEvent mouseevent,bool _check, const QPoint &ps);
    bool setSelect(int _index);

    void addData(const CCommonQueryMode &model);


    void delData(int row);
    void delData(QString _FileName);
    void delAllData();

    bool GetData(int row, CCommonQueryMode &_data);

    QList<CCommonQueryMode> GetAllData();
    int GetCount();
    void sort_data();

    void sortModel(int logicalIndex);
    QList<CCommonQueryMode>* baseTableModelList()
    {
        return &m_list;
    }
    void setColumnCount(int _count){ m_column_count = _count;}
    void mode_reset();
protected:
    QList<CCommonQueryMode> m_list;

signals:
    void signalViewTableModelClear();
    int signalLibraryTableModelIndex(int);
    void signalCurrentListCount(int, int);

private slots:
    void toClicked();

private:
    void onMouseClicked(int _row);
private:
    int m_listType;
    int m_column_count;

};

#endif // VIDEOTAPE_QUERY_MODE_H
