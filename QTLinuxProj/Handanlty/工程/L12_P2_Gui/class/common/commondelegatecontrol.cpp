#include "commondelegatecontrol.h"


#include <QModelIndex>
#include <QStyledItemDelegate>
#include <QStyleOption>
#include <QTextOption>
#include <QPainter>
#include <QApplication>
#include <QFile>
#include <QMouseEvent>

#include "basemodeldata.h"
#include "commontablemodel.h"
#include "list_view/common_view/common_query_list.h"


CommonDelegateControl::CommonDelegateControl(QObject *parent)
    : QObject(parent)
{

}

CommonDelegateControl::~CommonDelegateControl()
{

}
#include "class/common_interface.h"
void CommonDelegateControl::drawBackground(QPainter *painter, const QStyleOptionViewItem & opt, const QModelIndex & index, bool paramIsMouseOver, bool paramIsChecked, bool paramIsEnableAlternate, QColor paramMouseOverColor, QColor paramAlternateColor1, QColor paramAlternateColor2, QColor paramCheckedColor)
{

    if (paramIsMouseOver)
    {
        painter->fillRect(opt.rect, paramMouseOverColor);
    }
    else
    {
        if (paramIsEnableAlternate)
        {
            if (index.row()%2 == 1)
            {
                painter->fillRect(opt.rect, paramAlternateColor2);
            }
            else
            {
                painter->fillRect(opt.rect, paramAlternateColor1);
            }

        }
        else
        {
            painter->fillRect(opt.rect, paramAlternateColor1);

        }
    }

    if(paramIsChecked)
    {

        painter->fillRect(opt.rect, paramCheckedColor);
    }

}

void CommonDelegateControl::drawItemText(QPainter *painter, const QStyleOptionViewItem & opt, const QPalette &paramPalette, const QString &paramText, const QString &paramFamily,const QRect &paramRect, int paramPixelSize, QColor paramTextColor, bool paramIsEnable, Qt::Alignment paramAlignment)
{

    QFont font;
    if (!paramFamily.isEmpty())
    {
        font.setFamily(paramFamily);
    }

    font.setPixelSize(paramPixelSize);
    painter->setFont(font);
    painter->setPen(paramTextColor);
    QStyleOptionViewItem optTmp = opt;
    optTmp.displayAlignment=paramAlignment;
    if (!paramRect.isEmpty())
    {
        optTmp.rect = paramRect;
    }

    QApplication::style()->drawItemText(painter,optTmp.rect,optTmp.displayAlignment,paramPalette,paramIsEnable,paramText);
}

void CommonDelegateControl::drawItemText2(QPainter *painter, const QStyleOptionViewItem & opt, const QPalette &paramPalette, const QString &paramText, int paramOffsetX, int paramOffsetY, const QString &paramFamily,const QRect &paramRect, int paramPixelSize, QColor paramTextColor, bool paramIsEnable, Qt::Alignment paramAlignment)
{

    QFont fontTmp;
    if (!paramFamily.isEmpty())
    {
        fontTmp.setFamily(paramFamily);
    }
    fontTmp.setPixelSize(paramPixelSize);
    painter->setFont(fontTmp);
    painter->setPen(paramTextColor);
    QStyleOptionViewItem optTmp = opt;
    if (!paramRect.isEmpty())
    {
        optTmp.rect = paramRect;
    }

    QFontMetrics fm(fontTmp);
    int pixelsWide = fm.width(paramText);
    int iheight = fm.height();

    QRect targetRect = QRect(optTmp.rect.x() + paramOffsetX, optTmp.rect.y() + paramOffsetY, pixelsWide, iheight);

    QApplication::style()->drawItemText(painter,targetRect,paramAlignment,paramPalette,paramIsEnable,paramText);
}

QRect CommonDelegateControl::createCheckBoxRect(const QStyleOptionViewItem & opt, int paramMarginLeft, int paramCheckBoxWidth, int paramCheckBoxHeight)
{
    //QStyleOptionButton checkboxstyleoption;
    //QRect checkBoxElementRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, &checkboxstyleoption);
    QPoint checkboxpoint(opt.rect.x() + paramMarginLeft,opt.rect.y() + opt.rect.height() / 2 - paramCheckBoxHeight / 2);
    QRect CheckBoxRect(checkboxpoint, QSize(paramCheckBoxWidth,paramCheckBoxHeight));
    return CheckBoxRect;
}

QRect CommonDelegateControl::createCheckBoxRect2(const QStyleOptionViewItem & opt, int paramOffsetX, int paramOffsetY, int paramCheckBoxWidth, int paramCheckBoxHeight)
{
    QStyleOptionButton checkboxstyleoption;
    QRect checkBoxElementRect = QApplication::style()->subElementRect( QStyle::SE_CheckBoxIndicator, &checkboxstyleoption);
    QPoint checkboxpoint(opt.rect.x() + paramOffsetX ,opt.rect.y() + paramOffsetY);
    QRect CheckBoxRect(checkboxpoint, QSize(paramCheckBoxWidth,paramCheckBoxHeight));
    return CheckBoxRect;
}

void CommonDelegateControl::drawCheckBox(QPainter *painter, const QRect & paramRect, int paramIsChecked, QString paramSelectedImgName, QString paramNormalImgName, Qt::Alignment paramAlignment)
{
    QPixmap cbox_pixmap;
    if (paramIsChecked) {
        cbox_pixmap.load(paramSelectedImgName);
    } else {
        cbox_pixmap.load(paramNormalImgName);
    }
    QApplication::style()->drawItemPixmap(painter, paramRect, paramAlignment, cbox_pixmap);
}

void CommonDelegateControl::drawItemPixmap(QPainter *painter, const QStyleOptionViewItem & opt, bool &paramBRet, int paramMarginLeft, const QString &paramTargetIconName, const QString &paramDefaultIconPath, int paramDefaultWidth, int paramDefaultHeight, Qt::Alignment paramAlignment)
{
    QPixmap pixmapTmp;
    QString strIconPath = paramTargetIconName;
    if (!QFile::exists(strIconPath))
    {
        strIconPath = paramDefaultIconPath;
    }
    paramBRet = pixmapTmp.load(strIconPath);
    if (!paramBRet)
    {
        QFile file(strIconPath);

        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray pictureContent = file.read(file.size());
            file.close();
            paramBRet = pixmapTmp.loadFromData(pictureContent);
        }
    }

    QStyleOptionViewItem icon =opt;
    icon.displayAlignment = paramAlignment;
    icon.rect = QRect(QPoint(opt.rect.x() + paramMarginLeft, opt.rect.y() + opt.rect.height()/2 - paramDefaultHeight/2), QSize(paramDefaultWidth,paramDefaultHeight));

    if ((pixmapTmp.height() != paramDefaultHeight) || (pixmapTmp.width() != paramDefaultWidth))
    {
        pixmapTmp = pixmapTmp.scaled(paramDefaultWidth,paramDefaultHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);// 缩放图片
    }
    QApplication::style()->drawItemPixmap(painter, icon.rect, icon.displayAlignment, pixmapTmp);
}

void CommonDelegateControl::drawItemPixmap2(QPainter *painter, const QStyleOptionViewItem & opt, bool &paramBRet, int paramOffsetX, int paramOffsetY, const QString &paramTargetIconName, const QString &paramDefaultIconPath, int paramDefaultWidth, int paramDefaultHeight, Qt::Alignment paramAlignment)
{
    QPixmap pixmapTmp;
    QString strIconPath = paramTargetIconName;
    if (!QFile::exists(strIconPath))
    {
        strIconPath = paramDefaultIconPath;
    }
    paramBRet = pixmapTmp.load(strIconPath);
    if (!paramBRet)
    {
        QFile file(strIconPath);

        if(file.open(QIODevice::ReadOnly))
        {
            QByteArray pictureContent = file.read(file.size());
            file.close();
            paramBRet = pixmapTmp.loadFromData(pictureContent);
        }
    }

    QStyleOptionViewItem icon =opt;
    icon.displayAlignment = paramAlignment;
    icon.rect = QRect(QPoint(opt.rect.x() + paramOffsetX, opt.rect.y() + paramOffsetY), QSize(paramDefaultWidth,paramDefaultHeight));

    if ((pixmapTmp.height() != paramDefaultHeight) || (pixmapTmp.width() != paramDefaultWidth))
    {
        pixmapTmp = pixmapTmp.scaled(paramDefaultWidth,paramDefaultHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);// 缩放图片
    }
    QApplication::style()->drawItemPixmap(painter, icon.rect, icon.displayAlignment, pixmapTmp);
}


void CommonDelegateControl::editorEvent1(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,const QModelIndex &index, BaseModelData &data, bool &isUpdateCheck, int &viewmouseevent, bool &bStartTimer, bool paramIsEnableStartTimer, QPoint &mouseRightClickPs, bool paramIsEnableRightClick,char _type)
{
    QMouseEvent *mouse_event = static_cast<QMouseEvent*>(event);
    if (event->type() == QEvent::MouseMove)
    {
        viewmouseevent = evtmeMove;
    }
    else if(event->type() == QEvent::MouseButtonPress)
    {
        viewmouseevent = evtmePress;
        isUpdateCheck = true;
        if (paramIsEnableRightClick)
        {
            if (mouse_event->button() == Qt::RightButton && option.rect.contains(mouse_event->pos()))
            {
                data.ischecked = true;
                mouseRightClickPs = mouse_event->globalPos();
                //((CommonTableModel*)model)->setOneRowSelect(index.row(),true);
                ((CommonTableModel*)model)->setOneRowSelectByRightButton(index.row(),true);
            }
        }
        if(CCommonQueryList::kEnumDateSelect == _type)
            ((CommonTableModel*)model)->setOneRowColumnSelect(index.row(),index.column());
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        viewmouseevent = evtmeUp;
        isUpdateCheck = true;
        if (paramIsEnableStartTimer)
        {
            bStartTimer = true;
        }
    }
    else if(event->type() == QEvent::MouseButtonDblClick)
    {
        viewmouseevent = evtmeDouble;
        isUpdateCheck = true;
    }
}



void CommonDelegateControl::editorEvent2(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,const QModelIndex &index, BaseModelData &data, bool &isUpdateCheck, int &viewmouseevent, bool &bStartTimer, bool paramIsEnableStartTimer, int paramMarginLeft,int _checkBoxClickWidth,int _checkBoxClickHeight)
{
    QMouseEvent *mouse_event = static_cast<QMouseEvent*>(event);
    if (0 == index.column())
    {
        QRect checkBoxRect;
        if (_checkBoxClickWidth > 0)
        {
            if (0 == _checkBoxClickHeight)
                _checkBoxClickHeight = _checkBoxClickWidth;

            checkBoxRect = QRect(option.rect.x(),option.rect.y(),_checkBoxClickWidth,_checkBoxClickHeight);
        }
        else
            checkBoxRect = QRect(option.rect.x() + paramMarginLeft,option.rect.y() + option.rect.height() / 2 - m_checkBoxHeight/2,m_checkBoxWidth,m_checkBoxHeight);
        if((evtmeUp == viewmouseevent) && checkBoxRect.contains(mouse_event->pos()))
        {//点击的是复选框区域
            if (data.isPressed)
            {
                data.isPressed = false;
                data.ischecked = !data.ischecked;
                if (paramIsEnableStartTimer)
                {
                    bStartTimer = false;
                }
            }
        }
        else if(evtmeUp == viewmouseevent)
        {//点击的是第0列，但不是筛选框区域
            if (data.isPressed)
            {
                data.isPressed = false;
                data.ischecked = true;
                if (paramIsEnableStartTimer)
                {
                    bStartTimer = true;
                }
                ((CommonTableModel*)model)->setOneRowSelect(index.row(),true);
            }
        }
        else if (evtmePress == viewmouseevent)
        {
            data.isPressed = true;
        }
    }
    else
    {
        if(evtmePress == viewmouseevent)
        {
            if (mouse_event->button() != Qt::RightButton)
            {
                ((CommonTableModel*)model)->setOneRowSelect(index.row(),true);
            }
            data.ischecked = true;
            data.isPressed = true;
        }
        else if (evtmeUp == viewmouseevent)
        {
            data.isPressed = false;
        }
    }
}
