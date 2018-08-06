#ifndef COMMONUIINIT_H
#define COMMONUIINIT_H

#include <QObject>

class QToolButton;
class QPushButton;
class QSize;
class QIcon;
class QPoint;
class QString;
class QLabel;
class QProgressBar;
class MyCheckBox;
class QCheckBox;
class NavigateLeftButton;
class QColor;
class QMenu;


#define TEXT_COLOR_BLACK			"#494949"
#define TEXT_COLOR_GRAY				"#999999"
#define TEXT_COLOR_WHITE			"#ffffff"
#define BACKGROUND_COLOR_WHITE		"#ffffff"
#define BACKGROUND_COLOR_GRAY		"#f8f8f8"
#define SPLITE_COLOR_GRAY			"#e5e5e5"

#define FONT_SIZE_16					(24)
#define FONT_SIZE_S					(12)
#define FONT_SIZE_M					(14)
#define FONT_FAMILY					tr("msyh")
#define COM_TITLE_HIGHT				36
#define COM_TITLETEXT_LEFT_MARGIN	14
#define COM_LISTTIPWIDGET_TOP		82


#define MENU_WIDTH			108
#define MENU_ROWHEIGHT		28
#define MENU_MARGIN_LEFT	24
#define MENU_MARGIN_TOP		6
#define MENU_MARGIN_BOTTOM	6

/*背景色-列表*/
#define ALTERNATING_ROW_COLOR_WHITE		"#1d304e"                 //"#ffffff"
#define ALTERNATING_ROW_COLOR_BLUE		"#1d304e"
#define MOUSEOVER_ROW_COLOR				"#3ea2ff"
#define CHECKED_ROW_COLOR               "#3ea2ff"        //"#eaf6fe"
#define TEXT_ROW_COLOR1					"#0897f2"

#define TITLE_FOREGROUND_COLOR		TEXT_COLOR_WHITE

class CommonUiInit : public QObject
{
    Q_OBJECT

public:
    CommonUiInit(QObject *parent);
    ~CommonUiInit();

    static void initToolButtonForMiddle(QToolButton * paramToolButton, const QIcon &paramIcon, const QSize &paramIconSize, const QSize &paramButtonSize, const QPoint &paramTopLeftPoint, const QString &paramText);

    static void setButtonStyleSheet(QToolButton *paramButton);

    static void initSpliterLineStyleSheet(QLabel *paramLabel, bool _left = false, bool _top = false, bool _right = false, bool _bottom = true);

    static void setOkButtonStyleSheet(QPushButton *paramButton, int _pixelSize = FONT_SIZE_M);

    static void setCancelButtonStyleSheet(QPushButton *paramButton, int _pixelSize = FONT_SIZE_M);

    static void setCancelButtonStyleSheet2(QPushButton *paramButton, int _pixelSize = FONT_SIZE_M);

    static void setCloseButtonStyleSheet(QPushButton *paramButton);

    static void initOkButton(QPushButton *paramButton, int _pixelSize = FONT_SIZE_M);

    static void initCancelButton(QPushButton *paramButton, int _pixelSize = FONT_SIZE_M);

    static void initCancelButton2(QPushButton *paramButton, int _pixelSize = FONT_SIZE_M);

    static void initCloseButton(QPushButton *paramButton);

    static QSize okButtonSize();

    static QSize cancelButtonSize();

    static QSize closeButtonSize();

    static void setMsgBoxTitleStyleSheet(QWidget *paramWidget);

    static void setTitleTextStyleSheet(QLabel *paramLabel, int _pixelSize = FONT_SIZE_S, QString _color = TITLE_FOREGROUND_COLOR, bool _bold = false,int _pos = 0);

    static void initTitleTextLabel(QLabel *paramLabel, int _pixelSize = FONT_SIZE_M, QString _color = TEXT_COLOR_BLACK, bool _bold = false);

    static void setMsgProgressBarStyleSheet(QProgressBar *paramProgressBar);

    static void slotUpdateTopLevelCheckStatus(MyCheckBox *paramCheckBox, int paramStatus);

    static void setGrayborderAndWhiteBackgroundStyleSheet(QWidget *paramWidget, int _pixelSize = FONT_SIZE_M, QString _color = TEXT_COLOR_BLACK, bool _left = true, bool _top = true, bool _right = true, bool _bottom = true);

    static void initNavigateMiddleCheckBox(QCheckBox *paramCheckBox);

    static void initNavigateMiddleToolButton(NavigateLeftButton * paramToolButton ,const QString &paramText, const QString &paramNormalIconStr, const QString &paramActiveIconStr, const QString &paramPressedIconStr, const QString &paramDisabledIconStr);

    static void setLabelStyleSheet(QWidget *paramWidget, int _fontSize = FONT_SIZE_S, const QString &_color = TEXT_COLOR_BLACK);

    static void setMenuStyleSheet(QMenu *paramMenu);

    static QSize fmSize(const QString &paramText = QString(), int paramPixelSize = FONT_SIZE_S, const QString &paramFontFamily = QString());



private:

};

#endif // COMMONUIINIT_H
