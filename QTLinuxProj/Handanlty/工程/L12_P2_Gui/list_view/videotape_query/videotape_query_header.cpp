#include "videotape_query_header.h"

#include "./class/common/commonuiinit.h"
#include "LtyCommonDefine.h"



CVideotapeQueryHeader::CVideotapeQueryHeader(int _type,Qt::Orientation orientation, QWidget *parent)
    : QHeaderView(orientation, parent)
    , m_isOn(false)
{
    m_type = _type;
    setClickable(true);

    m_isUpLogicalIndex0 = E_LOGICAL_NORMAL;
    m_isUpLogicalIndex1 = E_LOGICAL_NORMAL;
    m_isUpLogicalIndex2 = E_LOGICAL_NORMAL;
    m_isUpLogicalIndex3 = E_LOGICAL_NORMAL;

    m_currentLogicalIndex = -1;
    m_prevLogicalIndex = -1;
}

void CVideotapeQueryHeader::setChecked(bool checked)
{
    if (isEnabled() && m_isOn != checked)
    {
        m_isOn = checked;
        updateSection(0);
        emit toggled(m_isOn);
    }
}

void CVideotapeQueryHeader::setCheckedWithoutAll(bool checked)
{
    if (isEnabled() && m_isOn != checked)
    {
        m_isOn = checked;
        updateSection(0);
    }
}

void CVideotapeQueryHeader::resetSort()
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

void CVideotapeQueryHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{

    QString strShowName = "";
    painter->save();
    painter->setPen(QColor(TEXT_COLOR_WHITE));

    painter->fillRect(rect, QColor(ALTERNATING_ROW_COLOR_WHITE));
    //painter->drawLine(rect.x() + rect.width() - 1,0,rect.x() + rect.width() - 1,rect.height());

    //painter->drawLine(rect.x(),rect.y() + rect.height() - 1,rect.x() + rect.width(),rect.y() + rect.height() - 1);



    QStyleOptionButton SortButton;
    SortButton.iconSize = QSize(SORT_ICON_WIDTH,SORT_ICON_HEIGHT);
    SortButton.features=QStyleOptionButton::Flat;
    SortButton.state=QStyle::State_Enabled;

    QStyleOptionViewItem version ;
    version.displayAlignment=Qt::AlignCenter;

    QFont versionFont(FONT_FAMILY);
    versionFont.setPixelSize(FONT_SIZE_16);
    painter->setFont(versionFont);
    painter->setPen(QColor(TEXT_COLOR_WHITE));


    switch(logicalIndex)
    {
        case 0:
        {

            QString strShowContent = tr("序号");
            int iContentLength = QFontMetrics(versionFont).width(strShowContent);

            SortButton.rect=QRect(rect.x() + 40 + iContentLength + 3,rect.y() + rect.height()/2 - SORT_ICON_HEIGHT/2,SORT_ICON_WIDTH,SORT_ICON_HEIGHT);
            version.rect= QRect(rect.x(),rect.y(),rect.width(),rect.height());

            style()->drawItemText(painter,version.rect,version.displayAlignment,QApplication::palette(),true,strShowContent);
            if (E_LOGICAL_NORMAL == m_isUpLogicalIndex0)
            {
            }
            else if(E_LOGICAL_UP == m_isUpLogicalIndex0)
            {
                SortButton.icon = QIcon(SORT_ICON_RES_UP);
            }
            else if(E_LOGICAL_DOWN == m_isUpLogicalIndex0)
            {
                SortButton.icon = QIcon(SORT_ICON_RES_DOWN);
            }
            if (E_LOGICAL_NORMAL != m_isUpLogicalIndex0)
            {
               // QApplication::style()->drawControl(QStyle::CE_PushButton,&SortButton,painter);
            }

        }break;
        case 1:
        {

            QString strShowContent = tr("开始时间");
            int iContentLength = QFontMetrics(versionFont).width(strShowContent);

            SortButton.rect=QRect(rect.x() + iContentLength + 3,rect.y() + rect.height()/2 - SORT_ICON_HEIGHT/2,SORT_ICON_WIDTH,SORT_ICON_HEIGHT);
            version.rect= QRect(rect.x(),rect.y(),rect.width(),rect.height());


            style()->drawItemText(painter,version.rect,version.displayAlignment,QApplication::palette(),true,strShowContent);
            if (E_LOGICAL_NORMAL == m_isUpLogicalIndex1)
            {
            }
            else if(E_LOGICAL_UP == m_isUpLogicalIndex1)
            {
                SortButton.icon = QIcon(SORT_ICON_RES_UP);
            }
            else if(E_LOGICAL_DOWN == m_isUpLogicalIndex1)
            {
                SortButton.icon = QIcon(SORT_ICON_RES_DOWN);
            }
            if (E_LOGICAL_NORMAL != m_isUpLogicalIndex1)
            {
               // QApplication::style()->drawControl(QStyle::CE_PushButton,&SortButton,painter);
            }

        }break;
        case 2:
        {

            QString strShowContent = tr("结束时间");
            int iContentLength = QFontMetrics(versionFont).width(strShowContent);

            SortButton.rect=QRect(rect.x() + iContentLength + 3,rect.y() + rect.height()/2 - SORT_ICON_HEIGHT/2,SORT_ICON_WIDTH,SORT_ICON_HEIGHT);
            version.rect= QRect(rect.x(),rect.y(),rect.width(),rect.height());


            style()->drawItemText(painter,version.rect,version.displayAlignment,QApplication::palette(),true,strShowContent);
            if (E_LOGICAL_NORMAL == m_isUpLogicalIndex2)
            {
            }
            else if(E_LOGICAL_UP == m_isUpLogicalIndex2)
            {
                SortButton.icon = QIcon(SORT_ICON_RES_UP);
            }
            else if(E_LOGICAL_DOWN == m_isUpLogicalIndex2)
            {
                SortButton.icon = QIcon(SORT_ICON_RES_DOWN);
            }
            if (E_LOGICAL_NORMAL != m_isUpLogicalIndex2)
            {
               // QApplication::style()->drawControl(QStyle::CE_PushButton,&SortButton,painter);
            }

        }break;
        case 3:
        {

            QString strShowContent = tr("时长（秒）");
            int iContentLength = QFontMetrics(versionFont).width(strShowContent);

            SortButton.rect=QRect(rect.x() + iContentLength + 3,rect.y() + rect.height()/2 - SORT_ICON_HEIGHT/2,SORT_ICON_WIDTH,SORT_ICON_HEIGHT);
            version.rect= QRect(rect.x(),rect.y(),rect.width(),rect.height());
            style()->drawItemText(painter,version.rect,version.displayAlignment,QApplication::palette(),true,strShowContent);

            if (E_LOGICAL_NORMAL == m_isUpLogicalIndex3)
            {
                //btnOnekey.icon = QIcon(":/image/taskMgr/appUnistallActive.png");
            }
            else if(E_LOGICAL_UP == m_isUpLogicalIndex3)
            {
                SortButton.icon = QIcon(SORT_ICON_RES_UP);
            }
            else if(E_LOGICAL_DOWN == m_isUpLogicalIndex3)
            {
                SortButton.icon = QIcon(SORT_ICON_RES_DOWN);
            }
            if (E_LOGICAL_NORMAL != m_isUpLogicalIndex3)
            {
                //QApplication::style()->drawControl(QStyle::CE_PushButton,&SortButton,painter);
            }

        }break;
        case 4:
        {
            QString strShowContent = tr("通道");

            version.rect= QRect(rect.x(),rect.y(),rect.width(),rect.height());
            style()->drawItemText(painter,version.rect,version.displayAlignment,QApplication::palette(),true,strShowContent);

        }break;
        case 5:
        {
            QString strShowContent = tr("使能");
            version.displayAlignment=Qt::AlignVCenter;
            version.rect= QRect(rect.x()+ 5,rect.y(),rect.width(),rect.height());
            style()->drawItemText(painter,version.rect,version.displayAlignment,QApplication::palette(),true,strShowContent);

        }break;

    }

    painter->restore();

}

void CVideotapeQueryHeader::mousePressEvent(QMouseEvent *event)
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

QRect CVideotapeQueryHeader::checkBoxRect(const QRect &sourceRect) const
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

int CVideotapeQueryHeader::slotHeaderViewIndex(int logicalIndex)
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











