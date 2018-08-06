#include "videotape_query_table.h"


#include <QtGui>
#include <QDebug>

#include "videotape_query_list.h"
#include "videotape_query_mode.h"
#include <QPalette>




CVideotapeQueryTable::CVideotapeQueryTable(QObject * parent) : QStyledItemDelegate(parent)
{
    //m_tableview = (MyTableView*)parent;
    m_musicManageList = (CVideotapeQueryList *)parent;
    m_tableview = m_musicManageList->pFileManageTable();
}

void CVideotapeQueryTable::paint(QPainter * painter, const QStyleOptionViewItem & opt, const QModelIndex & index ) const
{
    painter->save();

    CVideotapeQueryMode data_type = index.data().value<CVideotapeQueryMode>();
    int colindex = index.column();
    bool ischeck = data_type.ischecked;

    CommonDelegateControl::drawBackground(painter, opt, index, data_type.isMouseOver, data_type.ischecked, false);

    QRect CheckBoxRect = CommonDelegateControl::createCheckBoxRect(opt, 3);
#if 0
    painter->setPen(QColor(TEXT_COLOR_BLACK));
    QRect rect = opt.rect;
    painter->drawLine(rect.x() + rect.width() - 1,2,rect.x() + rect.width() - 1,rect.height() - 2);
    painter->drawLine(rect.x(),rect.y() + 1,rect.x() + rect.width(),rect.y() + 1);
    painter->drawLine(rect.x(),rect.y() + rect.height() - 1,rect.x() + rect.width(),rect.y() + rect.height() - 1);
#endif

    QColor font_color;
    if(data_type.ischecked)
        font_color = "#000000";
    else
        font_color = TEXT_COLOR_WHITE;

    if(colindex == 0) //第一列    显示复选框、名称
    {
        //QRect rect = opt.rect;
        //painter->setPen(QColor(TEXT_COLOR_BLACK));
        //painter->drawLine(rect.x(),rect.y(),rect.x(),rect.height());
        QFont nameFont(FONT_FAMILY);
        nameFont.setPixelSize(FONT_SIZE_16);
        painter->setFont(nameFont);
        painter->setPen(QColor(TEXT_COLOR_BLACK));
        CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), QString::number(data_type.model_data.m_number), FONT_FAMILY, opt.rect, FONT_SIZE_16, font_color, true, Qt::AlignCenter);
    }
    else if(colindex==1)//第二列	//开始时间
    {
        QFont nameFont(FONT_FAMILY);
        nameFont.setPixelSize(FONT_SIZE_16);
        painter->setFont(nameFont);
        painter->setPen(QColor(TEXT_COLOR_BLACK));
        CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), data_type.model_data.m_start_time, FONT_FAMILY, opt.rect, FONT_SIZE_16, font_color, true, Qt::AlignCenter);
    }
    else if(colindex==2)//第三列	结束时间
    {
        QFont nameFont(FONT_FAMILY);
        nameFont.setPixelSize(FONT_SIZE_16);
        painter->setFont(nameFont);
        painter->setPen(QColor(TEXT_COLOR_BLACK));
        CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), data_type.model_data.m_end_time, FONT_FAMILY, opt.rect, FONT_SIZE_16, font_color, true, Qt::AlignCenter);
    }
    else if(colindex==3)//第四列	时长（秒）
    {
        QFont nameFont(FONT_FAMILY);
        nameFont.setPixelSize(FONT_SIZE_16);
        painter->setFont(nameFont);
        painter->setPen(QColor(TEXT_COLOR_BLACK));
        CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), data_type.model_data.m_duration, FONT_FAMILY, opt.rect, FONT_SIZE_16, font_color, true, Qt::AlignCenter);
    }
    else if(colindex==4)//第五列	通道
    {
        QFont nameFont(FONT_FAMILY);
        nameFont.setPixelSize(FONT_SIZE_16);
        painter->setFont(nameFont);
        painter->setPen(QColor(TEXT_COLOR_BLACK));
        CommonDelegateControl::drawItemText(painter, opt, QApplication::palette(), QString::number(data_type.model_data.m_passageway), FONT_FAMILY, opt.rect, FONT_SIZE_16, font_color, true, Qt::AlignCenter);
    }
    else if(colindex==5)//第六列	使能
    {
        CommonDelegateControl::drawCheckBox(painter, CheckBoxRect, ischeck, ":/img/res/checkbox_selected.png", ":/img/res/checkbox_normal.png", Qt::AlignLeft);
    }

    painter->restore();

}


bool CVideotapeQueryTable::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,const QModelIndex &index)
{
    if (NULL == event || NULL == model) return false;

    //QMouseEvent *mouse_event = static_cast<QMouseEvent*>(event);
    //QRect btnOnekeyRect = QRect(option.rect.x(),option.rect.y()-12+option.rect.height()/2,69,24);
    //QRect btnDelRect = QRect(option.rect.x()+92,option.rect.y()-8+option.rect.height()/2,16,16);

    CVideotapeQueryMode data = index.data().value<CVideotapeQueryMode>();
    bool bStartTimer = false;
    bool isUpdateCheck = false;

    // 按键事件
    QPoint mouseRightClickPs;
    eViewTableMouseEvent viewmouseevent = evtmeNone;
    int viewmouseeventTmp = evtmeNone;
    eViewTableFocus viewfocus = evtfNone;
    bool isOver = false;

    CommonDelegateControl::editorEvent1(event, model, option, index, data, isUpdateCheck, viewmouseeventTmp, bStartTimer, true, mouseRightClickPs, true,0);

    CommonDelegateControl::editorEvent2(event, model, option, index, data, isUpdateCheck, viewmouseeventTmp, bStartTimer, true, 0,option.rect.width(),option.rect.height());

    viewmouseevent = static_cast<eViewTableMouseEvent>(viewmouseeventTmp);
    if (viewmouseevent == evtmeMove || data.isPressed)
    {
        isOver = true;
    }
    if(data.ischecked)
    {
        isOver = false;
    }

    m_tableview->setMouseOver(isOver, index);

    return ((CVideotapeQueryData*)model)->setData(index, data.ischecked, data.isPressed, viewfocus, viewmouseevent,bStartTimer, isUpdateCheck, mouseRightClickPs);
}
