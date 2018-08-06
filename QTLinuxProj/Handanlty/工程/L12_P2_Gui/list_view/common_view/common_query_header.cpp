#include "common_query_header.h"

#include "./class/common/commonuiinit.h"
#include "LtyCommonDefine.h"

#include "common_query_list.h"

CCommonQueryHeader::CCommonQueryHeader(int _type,Qt::Orientation orientation, QWidget *parent)
    : QHeaderView(orientation, parent)
    , m_isOn(false)
{
    m_type = _type;
    m_parent = parent;
    setClickable(true);

    m_isUpLogicalIndex0 = E_LOGICAL_NORMAL;
    m_isUpLogicalIndex1 = E_LOGICAL_NORMAL;
    m_isUpLogicalIndex2 = E_LOGICAL_NORMAL;
    m_isUpLogicalIndex3 = E_LOGICAL_NORMAL;

    m_currentLogicalIndex = -1;
    m_prevLogicalIndex = -1;
}

void CCommonQueryHeader::setChecked(bool checked)
{
    if (isEnabled() && m_isOn != checked)
    {
        m_isOn = checked;
        updateSection(0);
        emit toggled(m_isOn);
    }
}

void CCommonQueryHeader::setCheckedWithoutAll(bool checked)
{
    if (isEnabled() && m_isOn != checked)
    {
        m_isOn = checked;
        updateSection(0);
    }
}

void CCommonQueryHeader::resetSort()
{
    m_isUpLogicalIndex0 = E_LOGICAL_NORMAL;
    m_isUpLogicalIndex1 = E_LOGICAL_NORMAL;
    m_isUpLogicalIndex2 = E_LOGICAL_NORMAL;
    m_isUpLogicalIndex3 = E_LOGICAL_NORMAL;
    int countTmp = this->count();
    for (int index = 0; index < countTmp; index++)
    {
        updateSection(index);
    }

}

void CCommonQueryHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{

    QString strShowName = "";
    painter->save();
    painter->setPen(QColor("#24508F"));

//#0E1D30
    if(CCommonQueryList::kEnumDateSelect == m_type)
        painter->fillRect(rect, QColor(ALTERNATING_ROW_COLOR_WHITE));
    else
        painter->fillRect(rect, QColor("#0E1D30"));

    if(CCommonQueryList::kEnumDateSelect == m_type)
    {
        painter->drawLine(rect.x() + rect.width() - 1,0,rect.x() + rect.width() - 1,rect.height());
        painter->drawLine(rect.x(),rect.y() + rect.height() - 1,rect.x() + rect.width(),rect.y() + rect.height() - 1);

    }
    else
    {
        painter->drawLine(rect.x(),rect.y() + rect.height() - 1,rect.x() + rect.width(),rect.y() + rect.height() - 1);
    }



    QStyleOptionButton SortButton;
    SortButton.iconSize = QSize(SORT_ICON_WIDTH,SORT_ICON_HEIGHT);
    SortButton.features=QStyleOptionButton::Flat;
    SortButton.state=QStyle::State_Enabled;

    QStyleOptionViewItem version ;
    version.displayAlignment=Qt::AlignCenter;

    int font_size = 0;
    if(CCommonQueryList::kEnumDateSelect == m_type)
        font_size = FONT_SIZE_16;
    else
        font_size = 20;

    QFont versionFont(FONT_FAMILY);
    versionFont.setPixelSize(font_size);
    painter->setFont(versionFont);
    painter->setPen(QColor(TEXT_COLOR_WHITE));
    version.rect= QRect(rect.x(),rect.y(),rect.width(),rect.height());
    QString show_content = "";


    painter->setPen(QColor("#94c6ff"));


    if(logicalIndex < m_name_list.size())
    {
        //CCommonQueryList *pList = (CCommonQueryList *)m_parent;

        version.rect= QRect(rect.x(),rect.y(),rect.width(),rect.height());

        if(3 == logicalIndex)
        {
            if(CCommonQueryList::kEnumQueryResult == m_type)
            {
                version.displayAlignment=Qt::AlignLeft | Qt::AlignVCenter;
                version.rect.setX(version.rect.x() + 13);
            }
        }

        style()->drawItemText(painter,version.rect,version.displayAlignment,QApplication::palette(),true,m_name_list.at(logicalIndex).m_name);
        //if(m_name_list.at(logicalIndex).m_width != rect.width())
          //  pList->ColumnReWidth(logicalIndex,rect.width(),m_name_list.at(logicalIndex).m_width);
    }



    painter->restore();


}

void CCommonQueryHeader::mousePressEvent(QMouseEvent *event)
{
#if 0
      if (isEnabled() && event->pos().x()<=24 && event->pos().x()>=8 && event->pos().y()<=24 && event->pos().y()>=8)
    {
        m_isOn = !m_isOn;
        updateSection(0);
        emit toggled(m_isOn);
    }
    else
#endif
    //int x = event->pos().x();
    //int y = event->pos().y();
    QHeaderView::mousePressEvent(event);

}

QRect CCommonQueryHeader::checkBoxRect(const QRect &sourceRect) const
{
    QStyleOptionButton checkBoxStyleOption;
    QRect checkBoxRect = style()->subElementRect(QStyle::SE_CheckBoxIndicator,&checkBoxStyleOption);
    QPoint checkBoxPoint(sourceRect.x() +
                         sourceRect.width() / 2 -
                         checkBoxRect.width() / 2,
                         sourceRect.y() +
                         sourceRect.height() / 2 -
                         checkBoxRect.height() / 2);
    return QRect(checkBoxPoint, checkBoxRect.size());
}

int CCommonQueryHeader::slotHeaderViewIndex(int logicalIndex)
{
    int iRetVal = 0;
    switch(logicalIndex)
    {
        case 0:
        {
            if (E_LOGICAL_NORMAL == m_isUpLogicalIndex0)
            {
                m_isUpLogicalIndex0 = E_LOGICAL_UP;
            }
            else if(E_LOGICAL_UP == m_isUpLogicalIndex0)
            {
                m_isUpLogicalIndex0 = E_LOGICAL_DOWN;
            }
            else if(E_LOGICAL_DOWN == m_isUpLogicalIndex0)
            {
                m_isUpLogicalIndex0 = E_LOGICAL_UP;
            }
            m_isUpLogicalIndex1 = E_LOGICAL_NORMAL;
            m_isUpLogicalIndex2 = E_LOGICAL_NORMAL;
            m_isUpLogicalIndex3 = E_LOGICAL_NORMAL;
            iRetVal = m_isUpLogicalIndex0;
        }break;
        case 1:
        {
            if (E_LOGICAL_NORMAL == m_isUpLogicalIndex1)
            {
                m_isUpLogicalIndex1 = E_LOGICAL_UP;
            }
            else if(E_LOGICAL_UP == m_isUpLogicalIndex1)
            {
                m_isUpLogicalIndex1 = E_LOGICAL_DOWN;
            }
            else if(E_LOGICAL_DOWN == m_isUpLogicalIndex1)
            {
                m_isUpLogicalIndex1 = E_LOGICAL_UP;
            }
            m_isUpLogicalIndex0 = E_LOGICAL_NORMAL;
            m_isUpLogicalIndex2 = E_LOGICAL_NORMAL;
            m_isUpLogicalIndex3 = E_LOGICAL_NORMAL;
            iRetVal = m_isUpLogicalIndex1;
        }break;
        case 2:
        {
            if (E_LOGICAL_NORMAL == m_isUpLogicalIndex2)
            {
                m_isUpLogicalIndex2 = E_LOGICAL_UP;
            }
            else if(E_LOGICAL_UP == m_isUpLogicalIndex2)
            {
                m_isUpLogicalIndex2 = E_LOGICAL_DOWN;
            }
            else if(E_LOGICAL_DOWN == m_isUpLogicalIndex2)
            {
                m_isUpLogicalIndex2 = E_LOGICAL_UP;
            }
            m_isUpLogicalIndex0 = E_LOGICAL_NORMAL;
            m_isUpLogicalIndex1 = E_LOGICAL_NORMAL;
            m_isUpLogicalIndex3 = E_LOGICAL_NORMAL;
            iRetVal = m_isUpLogicalIndex2;
        }break;
        case 3:
        {
            if (E_LOGICAL_NORMAL == m_isUpLogicalIndex3)
            {
                m_isUpLogicalIndex3 = E_LOGICAL_UP;
            }
            else if(E_LOGICAL_UP == m_isUpLogicalIndex3)
            {
                m_isUpLogicalIndex3 = E_LOGICAL_DOWN;
            }
            else if(E_LOGICAL_DOWN == m_isUpLogicalIndex3)
            {
                m_isUpLogicalIndex3 = E_LOGICAL_UP;
            }
            m_isUpLogicalIndex0 = E_LOGICAL_NORMAL;
            m_isUpLogicalIndex1 = E_LOGICAL_NORMAL;
            m_isUpLogicalIndex2 = E_LOGICAL_NORMAL;
            iRetVal = m_isUpLogicalIndex3;
        }break;

    }

    return iRetVal;
}

void CCommonQueryHeader::add_head_name(int _index,const QString &_name,int _width)
{
    CHeaderInfo info;
    info.m_name = _name;
    info.m_width = _width;
    m_name_list.push_back(info);
}

int CCommonQueryHeader::get_head_name_count()
{
    return m_name_list.size();
}



