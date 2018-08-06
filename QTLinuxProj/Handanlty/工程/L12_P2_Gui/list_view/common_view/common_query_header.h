#ifndef COMMON_QUERY_HEADER_H
#define COMMON_QUERY_HEADER_H

#include <QWidget>
#include <QHeaderView>
#include <QPainter>
#include <QMouseEvent>
#include<QApplication>


class CHeaderInfo
{
public:
    QString m_name;
    int m_width;
};

class CCommonQueryHeader : public QHeaderView
{
    Q_OBJECT
public:
    explicit CCommonQueryHeader(int _type,Qt::Orientation orientation, QWidget *parent = 0);
    void setChecked(bool checked);
    void setCheckedWithoutAll(bool checked);
    void resetSort();
signals:
    void toggled(bool checked);
public slots:
    int slotHeaderViewIndex(int);

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    void mousePressEvent(QMouseEvent *event);

public:
    void add_head_name(int _index,const QString &_name,int _width);
    int get_head_name_count();

private:
    QRect checkBoxRect(const QRect &sourceRect) const;
    bool m_isOn;

    int m_isUpLogicalIndex0;
    int m_isUpLogicalIndex1;
    int m_isUpLogicalIndex2;
    int m_isUpLogicalIndex3;

    int m_prevLogicalIndex;
    int m_currentLogicalIndex;
    int m_type;

    QList<CHeaderInfo> m_name_list;
    void *m_parent;
};


#endif // VIDEOTAPE_QUERY_HEADER_H
