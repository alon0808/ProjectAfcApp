#include "commonuiinit.h"

#include <QToolButton>
#include <QPushButton>
#include <QSize>
#include <QIcon>
#include <QPoint>
#include <QString>
#include <QLabel>
#include <QProgressBar>
#include <QColor>
#include <QMenu>

#include "mycheckbox.h"
#include "iconLocation.h"
#include "./class/common/navigateleftbutton.h"



CommonUiInit::CommonUiInit(QObject *parent)
    : QObject(parent)
{

}

CommonUiInit::~CommonUiInit()
{

}

void CommonUiInit::initToolButtonForMiddle(QToolButton * paramToolButton, const QIcon &paramIcon, const QSize &paramIconSize, const QSize &paramButtonSize, const QPoint &paramTopLeftPoint, const QString &paramText)
{
    if (paramToolButton)
    {
        paramToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        paramToolButton->setIconSize(paramIconSize);
        paramToolButton->setIcon(paramIcon);
        paramToolButton->setText(paramText);
        paramToolButton->setFixedSize(paramButtonSize);
        paramToolButton->move(paramTopLeftPoint);
        setButtonStyleSheet(paramToolButton);
        //paramToolButton->setFlat(true);	//QPushButton使用
        paramToolButton->setFocusPolicy(Qt::NoFocus);
    }
}

void CommonUiInit::setButtonStyleSheet(QToolButton *paramButton)
{
    do
    {
        if (NULL == paramButton)
        {
            break;
        }

        paramButton->setStyleSheet("\
                               QToolButton {\
                               border:0px;\
                               font:14px;\
                               color:rgb(123,123,123);\
                               background: transparent;\
                               }\
                               QToolButton:hover:pressed{\
                               color:rgb(123,123,123);\
                               }\
                               QToolButton:disabled{\
                               color:#FFFFFF;\
                               background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                               stop: 0 #B4B4B4,\
                               stop: 1 #B4B4B4);\
                               }\
                               QToolButton:hover:!pressed {\
                               color:rgb(62,162,42);\
                               }"
                               );

    } while (0);
}

void CommonUiInit::initSpliterLineStyleSheet(QLabel *paramLabel, bool _left, bool _top, bool _right, bool _bottom)
{
    if (paramLabel)
    {
        paramLabel->setFixedHeight(NAVIGATE_MIDDLE_HEIGHT);
        paramLabel->setObjectName("m_QLabelSpliterLine");

        QString str1 = QString();
        QString str2 = QString();
        QString str3 = QString();
        QString str4 = QString();
        if (_left)
        {
            str1 = "border-left:1px solid #e5e5e5;";
        }
        if (_top)
        {
            str2 = "border-top:1px solid #e5e5e5;";
        }
        if (_right)
        {
            str3 = "border-right:1px solid #e5e5e5;";
        }
        if (_bottom)
        {
            str4 = "border-bottom:1px solid #e5e5e5;";
        }

        QString qssStr = "QLabel#m_QLabelSpliterLine{\
                         background-color:#ffffff;\
                         %1\
                         %2\
                         %3\
                         %4\
                         padding:0px;}";

        qssStr = qssStr.arg(str1, str2, str3, str4);
#if 0
        QString qssStrForLabelLineTmp = "QLabel#m_QLabelSpliterLine{\
                                        background-color:#ffffff;\
                                        border-top:0px solid #e5e5e5;\
                                        border-left:0px;\
                                        border-right:0px;\
                                        border-bottom:1px solid #e5e5e5;\
                                        padding:0px;}";
#endif
        paramLabel->setStyleSheet(qssStr);
    }
}

void CommonUiInit::setOkButtonStyleSheet(QPushButton *paramButton, int _pixelSize)
{
    if (paramButton)
    {
        paramButton->setStyleSheet(tr("QPushButton {\
                                  border:0px;\
                                  background:#22b567;\
                                  font:%1px;\
                                  border-radius: 0px;\
                                  color:#ffffff;\
                                  } \
                                  QPushButton:hover:pressed {background:#189854;} \
                                  QPushButton:hover:!pressed {background:#23ad63;}\
                                  QPushButton:disabled{border:1px solid #cccccc;background:#ffffff;color:#999999}").arg(QString::number(_pixelSize))
                                  );
    }
}

void CommonUiInit::setCancelButtonStyleSheet(QPushButton *paramButton, int _pixelSize)
{
    if (paramButton)
    {
        paramButton->setStyleSheet(tr("QPushButton {\
                                        border:1px solid #cccccc;\
                                          background:#ffffff;\
                                          font:%1px;\
                                          border-radius: 0px;\
                                          color:#999999;\
                                          } \
                                          QPushButton:hover:pressed {background:#f2f2f2;color:#28b825;} \
                                          QPushButton:hover:!pressed {color:#28b825;}\
                                          QPushButton:disabled{background:#ffffff;color:#999999;}").arg(QString::number(_pixelSize))
                                          );
    }
}

void CommonUiInit::setCancelButtonStyleSheet2(QPushButton *paramButton, int _pixelSize)
{
    if (paramButton)
    {
        paramButton->setStyleSheet(tr("QPushButton {\
                                      border:1px solid #cccccc;\
                                      background:#ffffff;\
                                      font:%1px;\
                                      border-radius: 0px;\
                                      color:#494949;\
                                      } \
                                      QPushButton:hover:pressed {background:#f2f2f2;color:#28b825;} \
                                      QPushButton:hover:!pressed {color:#28b825;}\
                                      QPushButton:disabled{background:#ffffff;color:#999999;}").arg(QString::number(_pixelSize))
                                      );
    }
}

void CommonUiInit::setCloseButtonStyleSheet(QPushButton *paramButton)
{
    if (paramButton)
    {
        paramButton->setStyleSheet("QPushButton {border:0px;background:transparent;border-image:url(:/Img/res/x_normal.png); } \
                                     QPushButton:hover:pressed {border-image:url(:/Img/res/x_pressed.png);} \
                                     QPushButton:hover:!pressed {border-image:url(:/Img/res/x_active.png);}"
                                     );
    }
}


void CommonUiInit::initOkButton(QPushButton *paramButton, int _pixelSize)
{
    if (paramButton)
    {
        paramButton->setFixedSize(okButtonSize());
        paramButton->setText(tr("确定"));
        setOkButtonStyleSheet(paramButton, _pixelSize);
        paramButton->setFocusPolicy(Qt::NoFocus);
        paramButton->setFlat(true);
    }
}

void CommonUiInit::initCancelButton(QPushButton *paramButton, int _pixelSize)
{
    if (paramButton)
    {
        paramButton->setFixedSize(cancelButtonSize());
        paramButton->setText(tr("取消"));
        setCancelButtonStyleSheet(paramButton, _pixelSize);
        paramButton->setFocusPolicy(Qt::NoFocus);
        paramButton->setFlat(true);
    }
}

void CommonUiInit::initCancelButton2(QPushButton *paramButton, int _pixelSize)
{
    if (paramButton)
    {
        paramButton->setFixedSize(cancelButtonSize());
        paramButton->setText(tr("取消"));
        setCancelButtonStyleSheet2(paramButton, _pixelSize);
        paramButton->setFocusPolicy(Qt::NoFocus);
        paramButton->setFlat(true);
    }
}

void CommonUiInit::initCloseButton(QPushButton *paramButton)
{
    if (paramButton)
    {
        paramButton->setFixedSize(closeButtonSize());
        setCloseButtonStyleSheet(paramButton);
        paramButton->setFocusPolicy(Qt::NoFocus);
        paramButton->setFlat(true);
    }
}

QSize CommonUiInit::okButtonSize()
{
    return QSize(90, 30);
}

QSize CommonUiInit::cancelButtonSize()
{
    return QSize(90, 30);
}

QSize CommonUiInit::closeButtonSize()
{
    return QSize(36, 36);
}

void CommonUiInit::setMsgBoxTitleStyleSheet(QWidget *paramWidget)
{
    if (paramWidget)
    {
        QString objNameTmp = paramWidget->objectName();
        if (objNameTmp.isEmpty())
        {
            objNameTmp = "msgTitle";
            paramWidget->setObjectName(objNameTmp);
        }
        paramWidget->setStyleSheet(tr("\
                                      QWidget#%1{\
                               background-color:#f8f8f8;\
                               border-left:0px;\
                               border-top:0px;\
                               border-right:0px;\
                               border-bottom:1px solid #e5e5e5;\
                               }").arg(objNameTmp)
                               );
    }
}


void CommonUiInit::initTitleTextLabel(QLabel *paramLabel, int _pixelSize, QString _color, bool _bold)
{
    if (paramLabel)
    {
        paramLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        paramLabel->setFixedSize(170,COM_TITLE_HIGHT);
        paramLabel->move(COM_TITLETEXT_LEFT_MARGIN,0);
        setTitleTextStyleSheet(paramLabel, _pixelSize, _color, _bold);
    }
}

void CommonUiInit::setMsgProgressBarStyleSheet(QProgressBar *paramProgressBar)
{
    if (paramProgressBar)
    {
        paramProgressBar->setStyleSheet("\
                                      QProgressBar{\
                                      border: 1px solid #ffff00;\
                                      background:#03111f;\
                                      border-radius:0px;\
                                      }\
                                      QProgressBar::chunk {\
                                      background-color:#ffff00;\
                                      border-radius:0px;\
                                      }"
                                      );
    }
}

void CommonUiInit::slotUpdateTopLevelCheckStatus(MyCheckBox *paramCheckBox, int paramStatus)
{
    //	qDebug()<<"paramStatus:"<<paramStatus;
    if (paramCheckBox)
    {
        int restoreFlagTmp = false;
        if (Qt::Unchecked == paramStatus)
        {
            if (!paramCheckBox->isChecked())
            {
                restoreFlagTmp = true;
            }
            paramCheckBox->setEnableUpdate(false);
            paramCheckBox->setChecked(false);
        }
        else if (Qt::Checked == paramStatus)
        {
            if (paramCheckBox->isChecked())
            {
                restoreFlagTmp = true;
            }
            paramCheckBox->setEnableUpdate(false);
            paramCheckBox->setChecked(true);
        }
        else if (Qt::PartiallyChecked == paramStatus)
        {
            if (!paramCheckBox->isChecked())
            {
                restoreFlagTmp = true;
            }
            paramCheckBox->setEnableUpdate(false);
            paramCheckBox->setChecked(false);
        }

        if (restoreFlagTmp)
        {
            paramCheckBox->setEnableUpdate(true);
        }
    }
}

void CommonUiInit::setGrayborderAndWhiteBackgroundStyleSheet(QWidget *paramWidget, int _pixelSize, QString _color, bool _left, bool _top, bool _right, bool _bottom)
{
    if (paramWidget)
    {
#if 1
        QString str1 = _color;
        QString str2 = QString::number(_pixelSize);
        QString str3 = QString();
        QString str4 = QString();
        QString str5 = QString();
        QString str6 = QString();

        if (_left)
        {
            str3 = tr("border-left: 1px solid %1;").arg(SPLITE_COLOR_GRAY);
        }
        if (_top)
        {
            str4 = tr("border-top: 1px solid %1;").arg(SPLITE_COLOR_GRAY);
        }
        if (_right)
        {
            str5 = tr("border-right: 1px solid %1;").arg(SPLITE_COLOR_GRAY);
        }
        if (_bottom)
        {
            str6 = tr("border-bottom: 1px solid %1;").arg(SPLITE_COLOR_GRAY);
        }

        QString qssStr = tr("QWidget{background-color:#ffffff;\
                            color:%1;\
                            font:%2px;\
                            border:1px;\
                            %3\
                            %4\
                            %5\
                            %6\
                            }\
                            ");
        qssStr = qssStr.arg(str1, str2, str3, str4, str5, str6);
        paramWidget->setStyleSheet(qssStr);
#endif
    }
}

void CommonUiInit::initNavigateMiddleCheckBox(QCheckBox *paramCheckBox)
{
    if (paramCheckBox)
    {
        paramCheckBox->setText(tr("全选"));

        QFont fontTmp;
        fontTmp.setPixelSize(12);
        QFontMetrics fm(fontTmp);
        int pixelsWide = fm.width(paramCheckBox->text());
        paramCheckBox->setFixedSize(NAVIGATE_MIDDLE_CHECKBOX_ICON_WIDTH + pixelsWide,NAVIGATE_MIDDLE_CHECKBOX_ICON_HEIGHT);
    }
}

void CommonUiInit::initNavigateMiddleToolButton(NavigateLeftButton * paramToolButton ,const QString &paramText, const QString &paramNormalIconStr, const QString &paramActiveIconStr, const QString &paramPressedIconStr, const QString &paramDisabledIconStr)
{
    if (paramToolButton)
    {
        paramToolButton->setNormalIconStr(paramNormalIconStr);
        paramToolButton->setActiveIconStr(paramActiveIconStr);
        paramToolButton->setPressedIconStr(paramPressedIconStr);
        paramToolButton->setDisabledIconStr(paramDisabledIconStr);
        paramToolButton->setDisplayText(paramText);
        paramToolButton->setSelected(false, true);
    }
}

void CommonUiInit::setLabelStyleSheet(QWidget *paramWidget, int _fontSize, const QString &_color)
{
    if (paramWidget)
    {
        QString str1 = QString::number(_fontSize);
        QString str2 = _color;
        QString qssStr ="font:%1px;\
                        color:%2;\
                        ";
        qssStr = qssStr.arg(str1, str2);
        paramWidget->setStyleSheet(qssStr);
    }
}

#define MENU_STYLE_SHEET "QMenu{\
border:1px solid #e5e5e5;\
background-color:white; \
padding-top:6px;\
padding-bottom:6px;\
padding-left:0px;\
padding-right:0px;\
}\
\
QMenu::item{\
background-color:#ffffff;\
font-size:12px;\
margin:0px;\
padding-top:6px;\
padding-bottom:6px;\
padding-left:23px;\
padding-right:30px;\
color:#494949;\
}\
\
QMenu::indicator{\
width:24px;\
height:28px;\
}\
\
QMenu::item:hover{\
background:rgb(34,181,102); \
color:white;\
}\
QMenu::item:selected{\
background:rgb(34,181,102); \
color:white;\
}\
\
QMenu:item:disabled{color:#999999; background:rgb(250,250,250);}\
QMenu::separator{\
height:1px;\
background-color:#e5e5e5;  \
margin:0px 1px 0px 1px;\
}"

void CommonUiInit::setMenuStyleSheet(QMenu *paramMenu)
{
    if (paramMenu)
    {
        paramMenu->setStyleSheet(MENU_STYLE_SHEET);


#if 0
        paramMenu->setStyleSheet(
            "QMenu{\
            background-color:#ffffff; \
            border:1px solid rgb(200,200,200); \
            padding:1px;\
            margin:0.1px;\
            }\
            \
            QMenu::item{\
            background-color:#ffffff;\
            }\
            \
            QMenu::indicator{\
            width:13px;\
            height:13px;\
            }\
            \
            QMenu::item:selected{\
            background:rgb(0,183,46); \
            }\
            \
            QMenu:item:disabled{background:rgb(250,250,250);}\
            QMenu::separator{\
            height:0.5px;\
            background-color:rgb(188,188,188);  \
            margin-left:0.1px;\
            margin-right:1px;\
            }");
#endif
    }
}

QSize CommonUiInit::fmSize(const QString &paramText, int paramPixelSize, const QString &paramFontFamily)
{
    QString fontFamily = paramFontFamily;
    if (fontFamily.isEmpty())
    {
        fontFamily = FONT_FAMILY;
    }

    QFont nameFont(paramFontFamily);
    nameFont.setPixelSize(paramPixelSize);
    //显示应用名称
    QFontMetrics fm(nameFont);
    int pixelsWide = fm.width(paramText);
    int pixelsHeight = fm.height();
    return QSize(pixelsWide, pixelsHeight);
}
void CommonUiInit::setTitleTextStyleSheet(QLabel *paramLabel, int _pixelSize, QString _color, bool _bold,int _pos)
{
    if (paramLabel)
    {

        QString str1 = QString();
        QString str2 = QString::number(_pixelSize);
        QString str4 = "left";
        if (_bold)
        {
            str1 = "bold";
        }
        if(0 == _pos)
        {
            str4 = "left";
        }
        else if(1 == _pos)
        {
            str4 = "center";
        }
        else if(2 == _pos)
        {
            str4 = "right";
        }
        QString str3 = _color;

        paramLabel->setStyleSheet(tr("\
                                   QLabel{\
                                   border:0px solid #ffffff;\
                                   text-align :%1;\
                                   font:%2 %3px;\
                                   color:%4;\
                                   background:transparent;\
                                   }").arg(str4,str1, str2, str3)
                                   );
    }
}
