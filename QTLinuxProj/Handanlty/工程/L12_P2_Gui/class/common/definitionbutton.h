#ifndef DEFINITIONBUTTON_H
#define DEFINITIONBUTTON_H

#include <QPixmap>
#include <QRect>
#include <QWidget>
#include <QObject>
#include <QStringList>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>

class QLabel;
class QEvent;
class QPixmap;
class QSize;
class QMovie;
class QButtonGroup;
class QHBoxLayout;
class QVBoxLayout;
class QPropertyAnimation;
class QFrame;

class PhoneBoxCommonBase : public QWidget
{
    Q_OBJECT
public:
    ~PhoneBoxCommonBase();

public:
    void SetTag(int nTag);

    int GetTage() const;

protected:
    void EmitClick();

    void EmitPressed();

    void EmitClikedNoTag();

    void EmitPressedNoTag();

signals:
    void Clicked(int nTage = 0);
    void Pressed(int nTage = 0);

protected:
    PhoneBoxCommonBase(QWidget *pParent = 0);

private:
    int m_nTag;
};

class PhoneBoxIconBtn : public PhoneBoxCommonBase
{
    Q_OBJECT

public:
    PhoneBoxIconBtn();
    ~PhoneBoxIconBtn();

public:
    void SetIconStyleSheet(const QString &styleSheet);

    void SetTextStyleSheet(const QString &text, const QString &styleSheet);

    void SetBackGroundWidget(const QString &styleSheet);

protected:
    bool eventFilter(QObject *pObj, QEvent *pEvent);

private:
    void InitWidget();

    QRect LblBtnGlRect();

private:
    QLabel *m_pImage;		//描述
    QLabel *m_pDescrible; //描述

    bool	m_bMouseButtonDown;
};


class PhoneBoxIconDescBtn : public PhoneBoxCommonBase
{
    Q_OBJECT
public:
    PhoneBoxIconDescBtn();
    ~PhoneBoxIconDescBtn();

public:
    void SetTextBtnText(const QString &strBtnText, const QString &strStyleSheet = QString());

    void SetTextInfo(const QString &strText, const QString &strStyleSheet = QString());

    void SetIconStyleSheet(const QString &strIconStyleSheet);

protected:
    bool eventFilter(QObject *pObj, QEvent *pEvent);

private:
    void InitWidget();

    QRect LblBtnGlRect();

private:
    QLabel *m_pImage;	//图片
    QLabel *m_pTextBtn; //字体按钮
    QLabel *m_pText; //文本描述

    bool	m_bMouseButtonDown;
};


class PhoneBoxPhoneUiBtn : public PhoneBoxCommonBase
{
    Q_OBJECT
public:
    PhoneBoxPhoneUiBtn();
    ~PhoneBoxPhoneUiBtn();

public:
    void SetBtnStyleSheet(const QString &strStyleSheet);

    void SetFixedSize(QSize const &size);

protected:
    bool eventFilter(QObject *pObj, QEvent *pEvent);

private:
    void InitWidget();

    QRect LblBtnGlRect();

private:
    QPushButton *m_pLabelBtn;
    bool	m_bMouseButtonDown;
};

//========================================================================================
class PhoneImagePixmap : public QWidget
{
    Q_OBJECT

public:
    enum ImageViewType
    {
        HB_VIEW,
        VB_VIEW,
        NO_VIEW
    };

public:
    PhoneImagePixmap(QWidget *parent = 0);
    ~PhoneImagePixmap();

public:
    void SetHBBackgroundSize(const QSize &backSize);

    void SetVBBackgroundSize(const QSize &backSize);

    void SetImageVBFrame(const QPixmap &vbFrame);

    void SetImageHBFrame(const QPixmap &hbFrame);

    void SetForegroundPixmap(const QPixmap &foregroundPixmap);

    void SetImageHBFrameForeground(const QRect &foregroundRect);

    void SetImageVBFrameForeground(const QRect &foregroundRect);

    void SetCurrentViewStyle(ImageViewType viewType);

    void SetFramePixmap(const QPixmap &framePixmap); //刷新 布局

    void updateByNoConnect();
    void updateByConnecting();
    void updateByConnected();

private:
    //void InitWidget();

    //QPixmap DrawBackgroundImage(const QPixmap &pixmap);

    //void DrawSvg(const QImage &srcData, QImage *destData, const QSize &dstSize, const QString &tempFileName);

private:
    QPixmap m_HBBackgroundImage; //横屏 背景
    QPixmap m_VBBackgroundImage; //竖屏 背景

    QRect   m_HBForegroundRect; //横屏 前景 显示范围
    QRect   m_VBForegroundRect; //竖屏 前景 显示范围

    QSize  m_HBBackgroundSize; //横屏时 窗口的大小
    QSize  m_VBBackgroundSize; //竖屏时 窗口的大小

    QLabel *m_pPhoneBackgroundImage; //


    QLabel *m_pLabIconNoConnect;
    QLabel *m_pLabTextNoConnect;

    QLabel *m_pLabIconConnecting;
    QLabel *m_pLabTextConnecting2;
    QMovie *m_MovieLoading;

    ImageViewType m_eCurrentViewType;
};


//========================================================================
class PhoneNavigateBtn : public PhoneBoxCommonBase
{
    Q_OBJECT

public:
    enum EImageDir
    {
        RIGHT_TO_LEFT,
        LEFT_TO_RIGHT
    };

public:
    PhoneNavigateBtn();
    ~PhoneNavigateBtn();

public:
    void InstanceLayout(EImageDir dir);

    void SetIconStyleSheet(const QString &styleSheet);

    void SetDescriText(const QString &descriText);

    void SetDescriStyleSheet(const QString &styleSheet);

    void SetBtnStyle(const QString &normalStyle, const QString &hoverStyle, const QString &presseStyle, const QString &disabledStyle);

private:
    void SetLabelStyle(const QString &normalStyle, const QString &hoverStyle, const QString &presseStyle, const QString &disabledStyle);

protected:
    bool eventFilter(QObject *pObj, QEvent *pEvent);

private:
    void InitWidget();

    QRect LblBtnGlRect();

private:
    enum
    {
        E_NORMAL_STYLE,
        E_HOVER_STYLE,
        E_PRESSED_STYLE,
        E_DISABLED_STYLE
    };

private:
    QPushButton *m_pBtnIconImage;
    QLabel *m_pBtnTextDescri;

    bool m_bMouseButtonDown;

    QString m_descriNormalStyle;
    QString m_descriHoverStyle;
    QString m_descriPresseStyle;
    QString m_descriDisabledStyle;

    QString m_btnNormalStyle;
    QString m_btnHoverStyle;
    QString m_btnPresseStyle;
    QString m_btnDisabledStyle;
};

//=========================================================================================
class PhonePushBtn : public PhoneBoxCommonBase
{
    Q_OBJECT

public:
    PhonePushBtn();
    ~PhonePushBtn();

public:
    void SetText(const QString &strText);

    void SetBtnStyleSheet(const QString &strStyleSheet);

protected:
    bool eventFilter(QObject *pObj, QEvent *pEvent);

private:
    void InitWidget();

    QRect LblBtnGlRect();

private:
    bool m_bMouseButtonDown;
    QLabel *m_pPushBtn;
};

class MyCheckBox;

class SelectCheckBoxItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectCheckBoxItemWidget(int nTagId = 0, QWidget *parent = 0);
    ~SelectCheckBoxItemWidget();

signals:
    void signalStateChanged(int);

public:
    int GetTabId() const;

    bool IsChecked() const;

    void SetDesText(const QString &text, const QString &lblStyleSheet);

    void setChecked(bool bChecked);

private slots:
    void OnStateChanged(int nCheckStatues);

private:
    void InitWidget();

private:
    MyCheckBox *m_pCheckBox; //选中按钮
    QLabel *m_pLblDes; //描述信息
    const int m_nTagId; //id标识

    int m_nCheckStatues;
};

class QFrame;
class SpecialBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit SpecialBtn(QWidget *parent = 0);
    ~SpecialBtn();

public:
    void SetLblStyleSheet(const QString &hover, const QString &noraml,  const QString &pressedStylesheet);

    void SetFrameStyleSheet(const QString &hover, const QString &noraml,  const QString &pressedStylesheet);

    void SetDesText(const QString &text);

    void NormalView();

    void ViewPressed();

protected:
    bool eventFilter(QObject *pObj, QEvent *pEvent);

private:
    void InitWidget();

private:
    QLabel *m_pLblDes;
    QFrame *m_pFenGeXian;

    bool m_bPressedView;

private:
    QString m_hoverStyleSheet;
    QString m_normalStyleSheet;
    QString m_pressedStyleSheet;

    QString m_frameHoverStyleSheet;
    QString m_frameNormalStyleSheet;
    QString m_framePressedStyleSheet;

};

class QFrame;
class SpecialVerticalBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit SpecialVerticalBtn(QWidget *parent = 0);
    ~SpecialVerticalBtn();

public:
    void SetLblStyleSheet(const QString &hover, const QString &noraml,  const QString &pressedStylesheet);

    void SetFrameStyleSheet(const QString &hover, const QString &noraml,  const QString &pressedStylesheet);

    void SetDesText(const QString &text);

    void NormalView();

    void ViewPressed();

protected:
    bool eventFilter(QObject *pObj, QEvent *pEvent);

private:
    void InitWidget();

private:
    QLabel *m_pLblDes;
    QFrame *m_pFenGeXian;

    bool m_bPressedView;

private:
    QString m_hoverStyleSheet;
    QString m_normalStyleSheet;
    QString m_pressedStyleSheet;

    QString m_frameHoverStyleSheet;
    QString m_frameNormalStyleSheet;
    QString m_framePressedStyleSheet;
};

class DefineSlidingBtnItem : public QPushButton
{
    Q_OBJECT
public:
    explicit DefineSlidingBtnItem(QWidget *parent = 0);
    ~DefineSlidingBtnItem();

public:
    void SetLblStyleSheet(const QString &hover, const QString &noraml,  const QString &pressedStylesheet);

    void SetDesText(const QString &text, Qt::Alignment align);

    void NormalView();

    void ViewPressed();

protected:
    bool eventFilter(QObject *pObj, QEvent *pEvent);

private slots:
    void OnClickedEventCreated();

private:
    void InitWidget();

private:
    QLabel *m_pLblDes;

    bool m_bPressedView;

    QString m_hoverStyleSheet;
    QString m_normalStyleSheet;
    QString m_pressedStyleSheet;
};

class MyButtonGroup;

class DefineSlidingBtnGroup : public QWidget
{
    Q_OBJECT
public:
    enum EViewStyle
    {
        E_HBOXLAYOUT_STYLE,
        E_VBOXLAYOUT_SYTLE,
    };

public:
    explicit DefineSlidingBtnGroup(int nStyle, int nSpacingValue, const QString &itemStyleSheet, const QSize &itemSize, QWidget *parent = 0);
    ~DefineSlidingBtnGroup();

signals:
    void DoBtnItemClicked(int nBtnId);

public:
    //设置样式表
    void SetItemStyleSheet(const QString &normal, const QString &hover, const QString &pressed);

    void AddItem(const QString &des, int nBtnId, Qt::Alignment alignment);

    void SetStyleSheet(const QString &styleSheet);

    void SetCurrentViewId(int nBtnId);

    void SetMovieLineStyleSheet(const QString &styleSheet, int nLineWidth);

    void SetCurrentBtnItemClicked(int nBtnId);

    int GetCurrentBtnId();

    void SetCurrentViewIdPage(int nBtnID);

private slots:
    void OnButtonClicked(int nBtenClicked);

    void OnAnimationEnd();

    void OnBtnStatusDeal();

private:
    void AddHBoxItem(int nIndex, const QString &des, int nBtnId, Qt::Alignment alignment);

    void AddVBoxItem(int nIndex, const QString &des, int nBtnId, Qt::Alignment alignment);

private:
    void InitWidget(int nStyle);

private:
    const int m_nSpacingValue;
    const int m_nViewStyleValue;
    const QSize m_itemBtnSize;

    int m_nLineWidth; //动画线条的宽度

    QButtonGroup *m_pBtnButtonGroup;
    QHBoxLayout *m_pMainHBoxLayout;
    QVBoxLayout *m_pMainVBoxLayout;
    const QString m_strItemStyleSheet;

    int m_nPreViewBtnId;

    QLabel *m_pLineSlidding;
    QPropertyAnimation *m_pQPropertyAnimation; //动画效果

    QString m_strNormalStyleSheet;
    QString m_strHoverStyleSheet;
    QString m_strPressedStyleSheet;
};

#include "commonshadowwidget.h"

class TaskSureToolWidget : public CommonShadowWidget
{
    Q_OBJECT
public:
    explicit TaskSureToolWidget(QWidget *parent = NULL);
    ~TaskSureToolWidget();

public:
    void SetTitle(const QString &title);

    void SetDesContent(const QString &strContent);

    void SetCheckBoxDes(const QString &checkDesContent);

    void setOkBtnText(const QString &_text);

    void Exec();

    bool IsSure();

    void GetSelectStatus(bool &bGetSelect);

    void HideCheckBox();

private slots:
    void OnBtnOk();

    void OnClancel();

private:
    void InitWidget();

private:
    QLabel *m_pTitleLbl;
    QLabel *m_pInfoDesLbl;
    MyCheckBox *m_pCheckBox;

    bool m_bSure;

private:
    QPushButton *pOkBtn;
    QPushButton *pCancelBtn;
};

class MyButtonGroup : public QButtonGroup
{
    Q_OBJECT
public:
    explicit MyButtonGroup(QWidget *parent = NULL);
    ~MyButtonGroup();

public:
    void EmitBtnCliked(int nBtnId);
};

class MyRedioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit MyRedioButton(QWidget *parent = NULL);
    ~MyRedioButton();

private:

};

#include <QMap>

class MultiFuncBtn : public QWidget
{
    Q_OBJECT
public:
    enum EAttriType
    {
        E_VIEW_TEXT, //显示字体
        E_STYLE_SHEET, //样式
        E_CURSOR_TYPE, //光标模式
        E_FRAME_SIZE, //大小
    };

public:
    explicit MultiFuncBtn(QWidget *parent = NULL);
    ~MultiFuncBtn();

signals:
    void DoClickedType(int nClickedType);

public:
    void SetCurrentAttri(int nAtttrType);

    void AddAttriTypeValue(int nAttriType, int nAttriItemType, const QVariant &attriItemValue);

    void SetDisplayText(const QString &displayText);

private slots:
    void OnClicked();

private:
    void InitWidget();

private:
    QPushButton *m_pFunBtn;
    QMap<int, QList<QPair<int,QVariant> > > m_lstAttrValueMap;
    int m_nCurrentAttriTypeValue; //当前处理的类别值
};



#endif // DEFINITIONBUTTON_H
