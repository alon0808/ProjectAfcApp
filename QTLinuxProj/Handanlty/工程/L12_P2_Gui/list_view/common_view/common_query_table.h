#ifndef COMMON_QUERY_TABLE_H
#define COMMON_QUERY_TABLE_H

#include <QStyledItemDelegate>
#include <QStyleOption>
#include <QTextOption>
#include <QPainter>
#include <QTableView>
#include <QWidget>

#include <QList>

#include "./class/common/mytableview.h"
#include "common_query_mode.h"
#include "./class/common/commondelegatecontrol.h"

class CCommonQueryList;

class CCommonQueryTable: public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum
    {
        kEnumMessagePre,
        kEnumMessageMore
    };
    explicit CCommonQueryTable(QObject * parent = 0);
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    //void ButtonPaint(QPainter * painter, ViewTableModel viewtablemodel, const QStyleOptionViewItem & opt, const QModelIndex & index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,const QModelIndex &index);
    void button_paint(QPainter * painter, CCommonQueryMode viewtablemodel,const QStyleOptionViewItem & opt, const QModelIndex & index,char _type = kEnumMessageMore) const;
private:
    void setCurrentIndex(const QModelIndex &_index,QAbstractItemModel *model);
private slots:

private:
    MyTableView *m_tableview;
    CCommonQueryList *m_common_List;


};


#endif // VIDEOTAPE_QUERY_TABLE_H
