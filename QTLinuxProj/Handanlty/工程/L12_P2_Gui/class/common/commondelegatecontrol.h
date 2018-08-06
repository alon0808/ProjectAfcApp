#ifndef COMMONDELEGATECONTROL_H
#define COMMONDELEGATECONTROL_H


#include <QObject>
#include <QPainter>
#include <QStyleOptionViewItem>

#include "common_data.h"

#include "./class/common/commonuiinit.h"

#define CHECKBOX_WIDTH	32
#define CHECKBOX_HEIGHT	30


//#include "../common/CommonTableModel.h"
class BaseModelData;

class CommonDelegateControl : public QObject
{
    Q_OBJECT

public:
    explicit CommonDelegateControl(QObject *parent = 0);
    ~CommonDelegateControl();
    /*************************************************
    Function:		drawBackground
    Description:	绘制背景色
    input:
        painter:QPainter *
        opt:QStyleOptionViewItem
        paramIsMouseOver:鼠标是否悬停
        paramIsChecked:是否选中
        paramIsEnableAlternate:是否允许交替色
        paramMouseOverColor:悬停背景色
        paramAlternateColor1:交替背景色1
        paramAlternateColor2:交替背景色2
        paramCheckedColor:选中背景色
    *************************************************/
    static void drawBackground(QPainter *painter, const QStyleOptionViewItem & opt, const QModelIndex & index, bool paramIsMouseOver, bool paramIsChecked, bool paramIsEnableAlternate = true, QColor paramMouseOverColor = MOUSEOVER_ROW_COLOR, QColor paramAlternateColor1 = ALTERNATING_ROW_COLOR_WHITE, QColor paramAlternateColor2 = ALTERNATING_ROW_COLOR_BLUE, QColor paramCheckedColor = CHECKED_ROW_COLOR);


    /*************************************************
    Function:		drawItemText
    Description:	绘制文本
    input:
        painter:QPainter *
        opt:QStyleOptionViewItem
        paramPalette:调色板
        paramText:待绘制文本
        paramFamily:字体家族
        paramRect:绘制区域
        paramPixelSize:字体大小
        paramTextColor:字体颜色
        paramIsEnable:默认true
        paramAlignment:对齐方式
    *************************************************/
    static void drawItemText(QPainter *painter, const QStyleOptionViewItem & opt, const QPalette &paramPalette, const QString &paramText, const QString &paramFamily = QString(), const QRect &paramRect = QRect(), int paramPixelSize = 12, QColor paramTextColor = QColor(TEXT_COLOR_BLACK), bool paramIsEnable = true, Qt::Alignment paramAlignment = Qt::AlignLeft|Qt::AlignVCenter);

    /*************************************************
    Function:		drawItemText2
    Description:	绘制文本
    input:
        painter:QPainter *
        opt:QStyleOptionViewItem
        paramPalette:调色板
        paramText:待绘制文本
        paramOffsetX:X方向偏移量
        paramOffsetY:Y方向偏移量
        paramFamily:字体家族
        paramRect:绘制区域
        paramPixelSize:字体大小
        paramTextColor:字体颜色
        paramIsEnable:默认true
        paramAlignment:对齐方式
    *************************************************/
    static void drawItemText2(QPainter *painter, const QStyleOptionViewItem & opt, const QPalette &paramPalette, const QString &paramText, int paramOffsetX, int paramOffsetY, const QString &paramFamily = QString(), const QRect &paramRect = QRect(), int paramPixelSize = 12, QColor paramTextColor = QColor(TEXT_COLOR_BLACK), bool paramIsEnable = true, Qt::Alignment paramAlignment = Qt::AlignLeft|Qt::AlignVCenter);


    /*************************************************
    Function:		createCheckBoxRect
    Description:	返回一个QCheckBox矩形区域
    input:
        opt:QStyleOptionViewItem
        paramMarginLeft:左边距
        paramCheckBoxWidth:checkBox宽度
        paramCheckBoxHeight:checkBox高度
    *************************************************/
    static QRect createCheckBoxRect(const QStyleOptionViewItem & opt, int paramMarginLeft, int paramCheckBoxWidth = m_checkBoxWidth, int paramCheckBoxHeight = m_checkBoxHeight);

    static QRect createCheckBoxRect2(const QStyleOptionViewItem & opt, int paramOffsetX, int paramOffsetY, int paramCheckBoxWidth = m_checkBoxWidth, int paramCheckBoxHeight = m_checkBoxHeight);

    /*************************************************
    Function:		drawCheckBox
    Description:	绘制QCheckBox
    input:
        painter:QPainter *
        paramRect:绘制区域
        paramIsChecked:是否选中
        paramSelectedImgName:图片路径(选中)
        paramNormalImgName:图片路径(常态)
        paramAlignment:对齐方式
    *************************************************/
    static void drawCheckBox(QPainter *painter, const QRect & paramRect, int paramIsChecked, QString paramSelectedImgName, QString paramNormalImgName, Qt::Alignment paramAlignment = Qt::AlignLeft);

    /*************************************************
    Function:		drawItemPixmap
    Description:	绘制图像
    input:
        painter:QPainter *
        opt:QStyleOptionViewItem
        paramBRet:false：图片读取失败 true：图片读取成功
        paramMarginLeft:左边距
        paramTargetIconName:目标图标路径
        paramDefaultIconPath:默认图标路径
        paramDefaultWidth:图标默认宽度
        paramDefaultHeight:图标默认高度
        paramAlignment:对齐方式
    *************************************************/
    static void drawItemPixmap(QPainter *painter, const QStyleOptionViewItem & opt, bool &paramBRet, int paramMarginLeft, const QString &paramTargetIconName, const QString &paramDefaultIconPath = ":/Img/res/icon_defind.png", int paramDefaultWidth = 35, int paramDefaultHeight = 35, Qt::Alignment paramAlignment = Qt::AlignLeft);

    static void drawItemPixmap2(QPainter *painter, const QStyleOptionViewItem & opt, bool &paramBRet, int paramOffsetX, int paramOffsetY, const QString &paramTargetIconName, const QString &paramDefaultIconPath = ":/Img/res/icon_defind.png", int paramDefaultWidth = 35, int paramDefaultHeight = 35, Qt::Alignment paramAlignment = Qt::AlignLeft);

    static void editorEvent1(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,const QModelIndex &index, BaseModelData &data, bool &isUpdateCheck, int &viewmouseevent, bool &bStartTimer, bool paramIsEnableStartTimer, QPoint &mouseRightClickPs, bool paramIsEnableRightClick,char _type);

    static void editorEvent2(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,const QModelIndex &index, BaseModelData &data, bool &isUpdateCheck, int &viewmouseevent, bool &bStartTimer, bool paramIsEnableStartTimer, int paramMarginLeft,int _checkBoxClickWidth = 0,int _checkBoxClickHeight = 0);

public:
    static const int m_checkBoxWidth = CHECKBOX_WIDTH;
    static const int m_checkBoxHeight = CHECKBOX_HEIGHT;

private:

};


#endif // COMMONDELEGATECONTROL_H
