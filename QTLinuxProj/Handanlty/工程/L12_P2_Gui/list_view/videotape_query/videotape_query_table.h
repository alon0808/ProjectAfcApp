#ifndef VIDEOTAPE_QUERY_TABLE_H
#define VIDEOTAPE_QUERY_TABLE_H

#include <QStyledItemDelegate>
#include <QStyleOption>
#include <QTextOption>
#include <QPainter>
#include <QTableView>
#include <QWidget>

#include <QList>

#include "./class/common/mytableview.h"

#include "./class/common/commondelegatecontrol.h"

class CVideotapeQueryList;

class CVideotapeQueryTable: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit CVideotapeQueryTable(QObject * parent = 0);
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    //void ButtonPaint(QPainter * painter, ViewTableModel viewtablemodel, const QStyleOptionViewItem & opt, const QModelIndex & index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,const QModelIndex &index);
private:
    void setCurrentIndex(const QModelIndex &_index,QAbstractItemModel *model);
private slots:

private:
    MyTableView *m_tableview;
    CVideotapeQueryList *m_musicManageList;


};


#endif // VIDEOTAPE_QUERY_TABLE_H
