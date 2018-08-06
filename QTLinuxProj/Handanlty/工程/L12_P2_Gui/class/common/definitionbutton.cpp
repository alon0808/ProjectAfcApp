#include "definitionbutton.h"



#include <QObject>
#include <QEvent>
#include <QLabel>
#include <QPainter>
#include <QImage>
#include <QFrame>
#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
//#include <QSvgGenerator>
//#include <QSvgRenderer>
#include <QSize>
#include <QMovie>
#include <QPropertyAnimation>
#include <QButtonGroup>

//#include "class/config.h"

#include "definitionbutton.h"

#include "./class/common/commonuiinit.h"
#include "./class/common/mycheckbox.h"

#include "LtyCommonDefine.h"

#define  PHONEBOX_ICONBTN_SIZE			QSize(88,88)
#define  PHONEBOX_ICONTEXT_SIZE			QSize(88, 20)
#define  PHONEBOX_ICON_WIDGET_SIZE		QSize(240-2, 136-2)

#define  PHONEBOX_BTNTEXT_ICON_SIZE		QSize(102,102)
#define  PHONEBOX_TEXTBTN_SIZE			QSize(137, 32)
#define  PHONEBOX_TEXTBTN_DESCRI_SIZE	QSize(137, 40)
#define  PHONEBOX_TEXTBTN_WIDGET_SIZE	QSize(293, 102)

#define PHONEBOX_PHONE_UI_BTN_SIZE QSize(70, 32)

PhoneBoxCommonBase::PhoneBoxCommonBase(QWidget *pParent) :
    QWidget(pParent)
{

}

PhoneBoxCommonBase::~PhoneBoxCommonBase()
{

}

void PhoneBoxCommonBase::SetTag(int nTag)
{
    m_nTag = nTag;
}

int PhoneBoxCommonBase::GetTage() const
{
    return m_nTag;
}

void PhoneBoxCommonBase::EmitClick()
{
    emit Clicked(m_nTag);
}

void PhoneBoxCommonBase::EmitPressed()
{
    emit Pressed(m_nTag);
}

void PhoneBoxCommonBase::EmitClikedNoTag()
{
    emit Clicked();
}

void PhoneBoxCommonBase::EmitPressedNoTag()
{
    emit Pressed();
}

//=======================================================================================
PhoneBoxIconBtn::PhoneBoxIconBtn()
{
    m_bMouseButtonDown = false;
    InitWidget();
    m_pImage->installEventFilter(this);
}

PhoneBoxIconBtn::~PhoneBoxIconBtn()
{

}

void PhoneBoxIconBtn::SetIconStyleSheet(const QString &styleSheet)
{
    m_pImage->setStyleSheet(styleSheet);
}

void PhoneBoxIconBtn::SetTextStyleSheet(const QString &text, const QString &styleSheet)
{
    m_pDescrible->setText(text);
    m_pDescrible->setStyleSheet(styleSheet);
}

void PhoneBoxIconBtn::SetBackGroundWidget(const QString &styleSheet)
{
    this->setStyleSheet(styleSheet);
}

bool PhoneBoxIconBtn::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (NULL != pObj && pObj == m_pImage)
    {
        if (pEvent->type() == QEvent::MouseButtonPress)
        {
            if (!m_bMouseButtonDown)
            {
                EmitPressed();
            }
            m_bMouseButtonDown = true;
        }
        else if (pEvent->type() == QEvent::MouseMove)
        {

        }
        else if (pEvent->type() == QEvent::MouseButtonRelease)
        {
            QPoint mousePs = static_cast<QMouseEvent*>(pEvent)->globalPos();
            if (m_bMouseButtonDown && LblBtnGlRect().contains(mousePs))
            {
                EmitClick();
            }
            m_bMouseButtonDown = false;
        }

        return pObj->event(pEvent);
    }

    return true;
}

void PhoneBoxIconBtn::InitWidget()
{
    m_pImage = new QLabel();
    m_pDescrible = new QLabel();

    m_pImage->setFixedSize(PHONEBOX_ICONBTN_SIZE);
    m_pDescrible->setFixedSize(PHONEBOX_ICONTEXT_SIZE);
    m_pDescrible->setAlignment(Qt::AlignCenter);

    QVBoxLayout *pIconAndDescribleVBoxLayout = new QVBoxLayout();
    pIconAndDescribleVBoxLayout->setMargin(0);
    pIconAndDescribleVBoxLayout->setSpacing(0);

    pIconAndDescribleVBoxLayout->addWidget(m_pImage);
    pIconAndDescribleVBoxLayout->addWidget(m_pDescrible);

    QVBoxLayout *pMainVBoxLayout = new QVBoxLayout();
    pMainVBoxLayout->setSpacing(0);
    pMainVBoxLayout->setMargin(0);

    pMainVBoxLayout->addSpacing(9);
    pMainVBoxLayout->addLayout(pIconAndDescribleVBoxLayout);
    pMainVBoxLayout->addStretch();

    QHBoxLayout *pHBoxLayout = new QHBoxLayout();
    pHBoxLayout->setSpacing(0);
    pHBoxLayout->setMargin(1);
    pHBoxLayout->addStretch();
    pHBoxLayout->addLayout(pMainVBoxLayout);
    pHBoxLayout->addStretch();

    QWidget *pBottomWidget = new QWidget();
    pBottomWidget->setLayout(pHBoxLayout);
    pBottomWidget->setFixedSize(PHONEBOX_ICON_WIDGET_SIZE);

    QHBoxLayout *pMainPhoneBoxIconBtnHboxLayout = new QHBoxLayout();
    pMainPhoneBoxIconBtnHboxLayout->setSpacing(0);
    pMainPhoneBoxIconBtnHboxLayout->setMargin(0);

    pMainPhoneBoxIconBtnHboxLayout->addWidget(pBottomWidget);

    this->setLayout(pMainPhoneBoxIconBtnHboxLayout);
    this->setFixedSize(PHONEBOX_ICON_WIDGET_SIZE);
}

QRect PhoneBoxIconBtn::LblBtnGlRect()
{
    QPoint glTopLeft = m_pImage->mapToGlobal(QPoint(0, 0));

    return QRect(glTopLeft, QSize(m_pImage->width(), m_pImage->height()));
}

//=======================================================================================
PhoneBoxIconDescBtn::PhoneBoxIconDescBtn()
{
    m_bMouseButtonDown = false;
    InitWidget();
    m_pImage->installEventFilter(this);
}

PhoneBoxIconDescBtn::~PhoneBoxIconDescBtn()
{

}

void PhoneBoxIconDescBtn::SetTextBtnText(const QString &strBtnText, const QString &strStyleSheet)
{
    if (!strStyleSheet.isEmpty())
    {
        m_pTextBtn->setStyleSheet(strStyleSheet);
    }

    m_pTextBtn->setText(strBtnText);
}

void PhoneBoxIconDescBtn::SetTextInfo(const QString &strText, const QString &strStyleSheet)
{
    if (!strStyleSheet.isEmpty())
    {
        m_pText->setStyleSheet(strStyleSheet);
    }

    m_pText->setText(strText);
}

void PhoneBoxIconDescBtn::SetIconStyleSheet(const QString &strIconStyleSheet)
{
    m_pImage->setStyleSheet(strIconStyleSheet);
}

bool PhoneBoxIconDescBtn::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (NULL != pObj && pObj == m_pImage)
    {
        if (pEvent->type() == QEvent::MouseButtonPress)
        {
            if (!m_bMouseButtonDown)
            {
                EmitPressed();
            }
            m_bMouseButtonDown = true;
        }
        else if (pEvent->type() == QEvent::MouseMove)
        {

        }
        else if (pEvent->type() == QEvent::MouseButtonRelease)
        {
            QPoint mousePs = static_cast<QMouseEvent*>(pEvent)->globalPos();
            if (m_bMouseButtonDown && LblBtnGlRect().contains(mousePs))
            {
                EmitClick();
            }
            m_bMouseButtonDown = false;
        }

        return pObj->event(pEvent);
    }

    return true;
}

void PhoneBoxIconDescBtn::InitWidget()
{
    m_pImage = new QLabel();
    m_pImage->setFixedSize(PHONEBOX_BTNTEXT_ICON_SIZE);

    m_pTextBtn = new QLabel();
    m_pTextBtn->setFixedSize(PHONEBOX_TEXTBTN_SIZE);

    m_pText = new QLabel();
    m_pText->setFixedSize(PHONEBOX_TEXTBTN_DESCRI_SIZE);
    m_pText->setAlignment(Qt::AlignLeft);
    m_pText->setWordWrap(true);

    QVBoxLayout *pRightVBoxLayout = new QVBoxLayout();
    pRightVBoxLayout->setSpacing(0);
    pRightVBoxLayout->setMargin(0);

    pRightVBoxLayout->addStretch();
    pRightVBoxLayout->addWidget(m_pTextBtn);
    pRightVBoxLayout->addWidget(m_pText);
    pRightVBoxLayout->addStretch();

    QHBoxLayout *pMainHBoxLayout =new QHBoxLayout();
    pMainHBoxLayout->setSpacing(0);
    pMainHBoxLayout->setMargin(0);
    pMainHBoxLayout->addWidget(m_pImage);
    pMainHBoxLayout->addSpacing(18);
    pMainHBoxLayout->addLayout(pRightVBoxLayout);
    pMainHBoxLayout->addStretch();

    this->setLayout(pMainHBoxLayout);
    this->setFixedSize(PHONEBOX_TEXTBTN_WIDGET_SIZE);
}

QRect PhoneBoxIconDescBtn::LblBtnGlRect()
{
    QPoint glTopLeft = m_pImage->mapToGlobal(QPoint(0, 0));

    return QRect(glTopLeft, QSize(m_pImage->width(), m_pImage->height()));
}

//=======================================================================================
PhoneBoxPhoneUiBtn::PhoneBoxPhoneUiBtn()
{
    m_bMouseButtonDown = false;
    InitWidget();
    m_pLabelBtn->installEventFilter(this);
}

PhoneBoxPhoneUiBtn::~PhoneBoxPhoneUiBtn()
{

}

void PhoneBoxPhoneUiBtn::SetBtnStyleSheet(const QString &strStyleSheet)
{
    m_pLabelBtn->setStyleSheet(strStyleSheet);
}

void PhoneBoxPhoneUiBtn::SetFixedSize(QSize const &size)
{
    m_pLabelBtn->setFixedSize(size);
    this->setFixedSize(size);
}

bool PhoneBoxPhoneUiBtn::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (NULL != pObj && pObj == m_pLabelBtn)
    {
        if (pEvent->type() == QEvent::MouseButtonPress)
        {
            if (!m_bMouseButtonDown)
            {
                EmitPressed();
            }
            m_bMouseButtonDown = true;
        }
        else if (pEvent->type() == QEvent::MouseMove)
        {

        }
        else if (pEvent->type() == QEvent::MouseButtonRelease)
        {
            QPoint mousePs = static_cast<QMouseEvent*>(pEvent)->globalPos();
            if (m_bMouseButtonDown && LblBtnGlRect().contains(mousePs))
            {
                EmitClick();
            }
            m_bMouseButtonDown = false;
        }

        return pObj->event(pEvent);
    }

    return true;
}

void PhoneBoxPhoneUiBtn::InitWidget()
{
    m_pLabelBtn = new QPushButton();
    m_pLabelBtn->setFixedSize(PHONEBOX_PHONE_UI_BTN_SIZE);
    m_pLabelBtn->setFlat(true);
    m_pLabelBtn->setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *pMainVBoxLayout = new QVBoxLayout();
    pMainVBoxLayout->setSpacing(0);
    pMainVBoxLayout->setMargin(0);

    pMainVBoxLayout->addWidget(m_pLabelBtn);

    this->setLayout(pMainVBoxLayout);

    this->setFixedSize(PHONEBOX_PHONE_UI_BTN_SIZE);
}

QRect PhoneBoxPhoneUiBtn::LblBtnGlRect()
{
    QPoint glTopLeft = m_pLabelBtn->mapToGlobal(QPoint(0, 0));

    return QRect(glTopLeft, QSize(m_pLabelBtn->width(), m_pLabelBtn->height()));
}

//================================================================================
#include <QFrame>
#include <QPalette>

PhoneImagePixmap::PhoneImagePixmap(QWidget *parent) :
    QWidget(parent)
{
    //InitWidget();
}

PhoneImagePixmap::~PhoneImagePixmap()
{

}

void PhoneImagePixmap::SetHBBackgroundSize(const QSize &backSize)
{
    m_HBBackgroundSize = backSize;
}

void PhoneImagePixmap::SetVBBackgroundSize(const QSize &backSize)
{
    m_VBBackgroundSize = backSize;
}

void PhoneImagePixmap::SetImageVBFrame(const QPixmap &vbFrame)
{
    m_VBBackgroundImage = vbFrame;
}

void PhoneImagePixmap::SetImageHBFrame(const QPixmap &hbFrame)
{
    m_HBBackgroundImage = hbFrame;
}

void PhoneImagePixmap::SetForegroundPixmap(const QPixmap &foregroundPixmap)
{
    //m_pPhoneBackgroundImage->setPixmap(DrawBackgroundImage(foregroundPixmap));
}

void PhoneImagePixmap::SetImageHBFrameForeground(const QRect &foregroundRect)
{
    m_HBForegroundRect = foregroundRect;
}

void PhoneImagePixmap::SetImageVBFrameForeground(const QRect &foregroundRect)
{
    m_VBForegroundRect = foregroundRect;
}

void PhoneImagePixmap::SetCurrentViewStyle(ImageViewType viewType)
{
    m_eCurrentViewType = viewType;
}

void PhoneImagePixmap::SetFramePixmap(const QPixmap &framePixmap)
{
    m_pPhoneBackgroundImage->setPixmap(framePixmap);
}

void PhoneImagePixmap::updateByNoConnect()
{
    SetForegroundPixmap(QPixmap());

    m_pLabIconNoConnect->setVisible(true);
    m_pLabTextNoConnect->setVisible(true);

    m_pLabIconConnecting->setVisible(false);
    m_pLabTextConnecting2->setVisible(false);
    m_MovieLoading->stop();

}

void PhoneImagePixmap::updateByConnecting()
{
    SetForegroundPixmap(QPixmap());

    m_pLabIconNoConnect->setVisible(false);
    m_pLabTextNoConnect->setVisible(false);

    m_pLabIconConnecting->setVisible(true);
    m_pLabTextConnecting2->setVisible(true);
    m_MovieLoading->start();
}

void PhoneImagePixmap::updateByConnected()
{
    m_pLabIconNoConnect->setVisible(false);
    m_pLabTextNoConnect->setVisible(false);

    m_pLabIconConnecting->setVisible(false);
    m_pLabTextConnecting2->setVisible(false);
    m_MovieLoading->stop();
}

#if 0
void PhoneImagePixmap::InitWidget()
{
    QHBoxLayout *pHBoxLayout = new QHBoxLayout();
    pHBoxLayout->setMargin(0);
    pHBoxLayout->setSpacing(0);

    m_pPhoneBackgroundImage = new QLabel();

    m_pPhoneBackgroundImage->setScaledContents(true);

    pHBoxLayout->addWidget(m_pPhoneBackgroundImage);
    m_pPhoneBackgroundImage->setScaledContents(true);

    m_pLabIconNoConnect = new QLabel(m_pPhoneBackgroundImage);
    m_pLabIconNoConnect->setFixedSize(120, 120);
    m_pLabIconNoConnect->move(36+48, 68+112);
    m_pLabIconNoConnect->setStyleSheet("border-image:url(:/Img/res/phoneInfo/noConnect.png);");

    m_pLabTextNoConnect = new QLabel(m_pPhoneBackgroundImage);
    CommonUiInit::initTitleTextLabel(m_pLabTextNoConnect, FONT_SIZE_S, TEXT_COLOR_WHITE, true);
    m_pLabTextNoConnect->setFixedSize(120, 36);
    m_pLabTextNoConnect->move(36+48, 68+112+120);
    m_pLabTextNoConnect->setAlignment(Qt::AlignCenter);
    m_pLabTextNoConnect->setText(tr("%1未连接").arg(DEVICE_OR_PHONE_NAME));

    m_pLabIconConnecting = new QLabel(m_pPhoneBackgroundImage);
    m_pLabIconConnecting->setFixedSize(120, 120);
    m_pLabIconConnecting->move(36+48, 68+112);

    m_pLabTextConnecting2 = new QLabel(m_pPhoneBackgroundImage);
    CommonUiInit::initTitleTextLabel(m_pLabTextConnecting2, FONT_SIZE_S, TEXT_COLOR_WHITE, true);
    m_pLabTextConnecting2->setFixedSize(120, 36);
    m_pLabTextConnecting2->move(36+48, 68+112+120);
    m_pLabTextConnecting2->setAlignment(Qt::AlignCenter);
    m_pLabTextConnecting2->setText(tr("%1连接中...").arg(DEVICE_OR_PHONE_NAME));

    m_MovieLoading = new QMovie;
    m_MovieLoading->setFileName(":/Img/res/phoneInfo/loading.gif");
    m_pLabIconConnecting->setMovie(m_MovieLoading);

    this->setLayout(pHBoxLayout);
}
#endif
#if 0
QPixmap PhoneImagePixmap::DrawBackgroundImage(const QPixmap &pixmap)
{
    if (VB_VIEW == m_eCurrentViewType)
    {
        QString strFile = Config::getInstance()->m_TempDir + "/svgTemp.svg";
        QImage tempImage = pixmap.toImage();
        QImage _forgeround = tempImage.scaled(pixmap.width()/2, pixmap.height()/2).scaled(m_VBForegroundRect.size(), Qt::IgnoreAspectRatio,  Qt::SmoothTransformation); //(m_VBForegroundRect.size(), QImage::Format_ARGB32);
        //DrawSvg(pixmap.toImage(), &_forgeround, m_VBForegroundRect.size(), strFile);

        QPixmap _loclapixmap = QPixmap(m_VBBackgroundSize);
        _loclapixmap.fill(Qt::transparent);

        QPainter painter(&_loclapixmap);
        QRect backgroundRect = QRect(QPoint(0, 0), m_VBBackgroundSize);
        painter.drawPixmap(backgroundRect, m_VBBackgroundImage);
        painter.drawPixmap(m_VBForegroundRect, QPixmap::fromImage(_forgeround));

        if (QFile::exists(strFile))
        {
            QFile::remove(strFile);
        }

        return _loclapixmap;
    }
    else if (HB_VIEW == m_eCurrentViewType)
    {
        QString strFile = Config::getInstance()->m_TempDir + "/svgTemp.svg";
        QImage _forgeround = pixmap.toImage().scaled(pixmap.width()/2, pixmap.height()/2).scaled(m_VBForegroundRect.size(), Qt::IgnoreAspectRatio,  Qt::SmoothTransformation);//(m_HBForegroundRect.size(), QImage::Format_ARGB32);
        //DrawSvg(pixmap.toImage(), &_forgeround, m_HBForegroundRect.size(), strFile);

        QPixmap _loclapixmap = QPixmap(m_HBBackgroundSize);
        _loclapixmap.fill(Qt::transparent);

        QPainter painter(&_loclapixmap);
        QRect backgroundRect = QRect(QPoint(0, 0), m_HBBackgroundSize);
        painter.drawPixmap(backgroundRect, m_HBBackgroundImage);
        painter.drawPixmap(m_HBForegroundRect, QPixmap::fromImage(_forgeround));

        if (QFile::exists(strFile))
        {
            QFile::remove(strFile);
        }

        return _loclapixmap;
    }

    return QPixmap();
}
#endif
#if 0
void PhoneImagePixmap::DrawSvg(const QImage &srcData, QImage *destData, const QSize &dstSize, const QString &tempFileName)
{
    if (!srcData.isNull() && NULL != destData)
    {
        QSize srcSize = srcData.size();

        QSvgGenerator svgGenerator;
        svgGenerator.setFileName(tempFileName);
        svgGenerator.setSize(srcSize);
        svgGenerator.setViewBox(QRect(0, 0, srcSize.width(), srcSize.height()));

        QPainter _painter;
        _painter.begin(&svgGenerator);
        _painter.setPen(Qt::NoPen);
        _painter.drawImage(0, 0, srcData);
        _painter.end();

        QSvgRenderer svgRender(tempFileName);
        QPainter destPainter;
        destPainter.begin(destData);
        svgRender.render(&destPainter, QRect(0, 0, dstSize.width(), dstSize.height()));
        destPainter.end();
    }
}
#endif

//==================================
PhoneNavigateBtn::PhoneNavigateBtn()
{
    m_bMouseButtonDown = false;
    InitWidget();

    this->installEventFilter(this);
    m_pBtnIconImage->installEventFilter(this);
    m_pBtnTextDescri->installEventFilter(this);
    m_pBtnTextDescri->setMouseTracking(true);
    m_pBtnIconImage->setMouseTracking(true);

    this->setMouseTracking(true);

    SetLabelStyle("QLabel{color:#494949; font:12px;}","QLabel{color:#0897f2; font:12px;}", "QLabel{color:#007cca; font:12px;}", "QLabel{color:#999999; font:12px;}");
}

PhoneNavigateBtn::~PhoneNavigateBtn()
{

}

void PhoneNavigateBtn::InstanceLayout(EImageDir dir)
{
    if (dir == LEFT_TO_RIGHT)
    {
        QHBoxLayout *pMainHBoxLayout = new QHBoxLayout();
        pMainHBoxLayout->setSpacing(0);
        pMainHBoxLayout->setMargin(0);

        pMainHBoxLayout->addWidget(m_pBtnIconImage);
        pMainHBoxLayout->addWidget(m_pBtnTextDescri);

        this->setLayout(pMainHBoxLayout);
    }
    else if (dir == RIGHT_TO_LEFT)
    {
        QHBoxLayout *pMainHBoxLayout = new QHBoxLayout();
        pMainHBoxLayout->setSpacing(0);
        pMainHBoxLayout->setMargin(0);

        pMainHBoxLayout->addWidget(m_pBtnTextDescri);
        pMainHBoxLayout->addWidget(m_pBtnIconImage);
        this->setLayout(pMainHBoxLayout);
    }
}

void PhoneNavigateBtn::SetIconStyleSheet(const QString &styleSheet)
{
    m_pBtnIconImage->setStyleSheet(styleSheet);
}

void PhoneNavigateBtn::SetDescriText(const QString &descriText)
{
    m_pBtnTextDescri->setText(descriText);
}

void PhoneNavigateBtn::SetDescriStyleSheet(const QString &styleSheet)
{
    m_pBtnTextDescri->setStyleSheet(styleSheet);
}

void PhoneNavigateBtn::SetBtnStyle(const QString &normalStyle, const QString &hoverStyle, const QString &presseStyle, const QString &disabledStyle)
{
    m_btnNormalStyle = normalStyle;
    m_btnHoverStyle = hoverStyle;
    m_btnPresseStyle = presseStyle;
    m_btnDisabledStyle = disabledStyle;

    m_pBtnIconImage->setStyleSheet(normalStyle);
}

void PhoneNavigateBtn::SetLabelStyle(const QString &normalStyle, const QString &hoverStyle, const QString &presseStyle, const QString &disabledStyle)
{
    m_descriNormalStyle = normalStyle;
    m_descriHoverStyle = hoverStyle;
    m_descriPresseStyle = presseStyle;
    m_descriDisabledStyle = disabledStyle;

    m_pBtnTextDescri->setStyleSheet(normalStyle);
}

bool PhoneNavigateBtn::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (NULL != pObj && NULL != pEvent)
    {
        if (pEvent->type() == QEvent::MouseButtonPress)
        {
            if (this->isEnabled())
            {
                if (!m_bMouseButtonDown)
                {
                    EmitPressedNoTag();
                }
                m_bMouseButtonDown = true;

                if (this->isEnabled())
                {
                    m_pBtnTextDescri->setStyleSheet(m_descriPresseStyle);
                    m_pBtnIconImage->setStyleSheet(m_btnPresseStyle);
                }
            }
        }
        else if (pEvent->type() == QEvent::MouseMove)
        {
            if (this->isEnabled())
            {
                m_pBtnTextDescri->setStyleSheet(m_descriHoverStyle);
                m_pBtnIconImage->setStyleSheet(m_btnHoverStyle);
            }
        }
        else if (pEvent->type() == QEvent::MouseButtonRelease)
        {
            if (this->isEnabled())
            {
                QPoint mousePs = static_cast<QMouseEvent*>(pEvent)->globalPos();
                if (m_bMouseButtonDown && LblBtnGlRect().contains(mousePs))
                {
                    EmitClikedNoTag();
                }
                m_bMouseButtonDown = false;
            }
        }
        else if (pEvent->type() == QEvent::EnabledChange)
        {
            if (this->isEnabled())
            {
                m_pBtnTextDescri->setStyleSheet(m_descriNormalStyle);
                m_pBtnIconImage->setStyleSheet(m_btnNormalStyle);
            }
            else
            {
                m_pBtnTextDescri->setStyleSheet(m_descriDisabledStyle);
                m_pBtnIconImage->setStyleSheet(m_btnDisabledStyle);
            }
        }
        else if (pEvent->type() == QEvent::Leave)
        {
            if (this->isEnabled())
            {
                m_pBtnTextDescri->setStyleSheet(m_descriNormalStyle);
                m_pBtnIconImage->setStyleSheet(m_btnNormalStyle);
            }
            else
            {
                m_pBtnTextDescri->setStyleSheet(m_descriDisabledStyle);
                m_pBtnIconImage->setStyleSheet(m_btnDisabledStyle);
            }
        }
        //else if (pEvent->type() == QEvent::Enter)
        //{
        //	if (this->isEnabled())
        //	{
        //		m_pBtnTextDescri->setStyleSheet(m_descriNormalStyle);
        //	}
        //	else
        //	{
        //		m_pBtnTextDescri->setStyleSheet(m_descriDisabledStyle);
        //	}
        //}

        return pObj->event(pEvent);
    }

    return true;
}

void PhoneNavigateBtn::InitWidget()
{
    m_pBtnIconImage = new QPushButton();
    m_pBtnIconImage->setStyleSheet("QPushButton{border:0px;}");
    m_pBtnIconImage->setFocusPolicy(Qt::NoFocus);
    m_pBtnIconImage->setFlat(true);
    m_pBtnIconImage->setFixedSize(26, 26);

    m_pBtnTextDescri = new QLabel();
}

QRect PhoneNavigateBtn::LblBtnGlRect()
{
    QPoint glTopLeft = this->mapToGlobal(QPoint(0, 0));

    return QRect(glTopLeft, QSize(this->width(), this->height()));
}


PhonePushBtn::PhonePushBtn()
{
    m_bMouseButtonDown = false;
    InitWidget();
    m_pPushBtn->installEventFilter(this);
}

PhonePushBtn::~PhonePushBtn()
{

}

void PhonePushBtn::SetText(const QString &strText)
{
    m_pPushBtn->setText(strText);
}

void PhonePushBtn::SetBtnStyleSheet(const QString &strStyleSheet)
{
    m_pPushBtn->setStyleSheet(strStyleSheet);
}

bool PhonePushBtn::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (NULL != pObj && pObj == m_pPushBtn)
    {
        if (pEvent->type() == QEvent::MouseButtonPress)
        {
            if (!m_bMouseButtonDown)
            {
                EmitPressedNoTag();
            }
            m_bMouseButtonDown = true;
        }
        else if (pEvent->type() == QEvent::MouseMove)
        {

        }
        else if (pEvent->type() == QEvent::MouseButtonRelease)
        {
            QPoint mousePs = static_cast<QMouseEvent*>(pEvent)->globalPos();
            if (m_bMouseButtonDown && LblBtnGlRect().contains(mousePs))
            {
                EmitClikedNoTag();
            }
            m_bMouseButtonDown = false;
        }
        else if (pEvent->type() == QEvent::EnabledChange)
        {

        }

        return pObj->event(pEvent);
    }

    return true;
}

void PhonePushBtn::InitWidget()
{
    m_pPushBtn = new QLabel();

    QHBoxLayout *pMainLayout = new QHBoxLayout();
    pMainLayout->setSpacing(0);
    pMainLayout->setMargin(0);

    pMainLayout->addWidget(m_pPushBtn);
    this->setLayout(pMainLayout);
    this->setObjectName("PhonePushBtn");
    this->setStyleSheet("QWidget#PhonePushBtn{border:none;}");
}

QRect PhonePushBtn::LblBtnGlRect()
{
    QPoint glTopLeft = m_pPushBtn->mapToGlobal(QPoint(0, 0));

    return QRect(glTopLeft, QSize(m_pPushBtn->width(), m_pPushBtn->height()));
}

#include "./class/common/mycheckbox.h"

SelectCheckBoxItemWidget::SelectCheckBoxItemWidget(int nTagId, QWidget *parent) :
    m_nTagId(nTagId), QWidget(parent)
{
    InitWidget();
}

SelectCheckBoxItemWidget::~SelectCheckBoxItemWidget()
{

}

int SelectCheckBoxItemWidget::GetTabId() const
{
    return m_nTagId;
}

bool SelectCheckBoxItemWidget::IsChecked() const
{
    return m_nCheckStatues == Qt::Checked;
}

void SelectCheckBoxItemWidget::SetDesText(const QString &text, const QString &lblStyleSheet)
{
    m_pLblDes->setText(text);
    m_pLblDes->setStyleSheet(lblStyleSheet);
}

void SelectCheckBoxItemWidget::setChecked(bool bChecked)
{
    m_pCheckBox->setChecked(bChecked);
    m_nCheckStatues = bChecked ? Qt::Checked : Qt::Unchecked;
}

void SelectCheckBoxItemWidget::OnStateChanged(int nCheckStatues)
{
    m_nCheckStatues = nCheckStatues;
    emit signalStateChanged(nCheckStatues);
}

void SelectCheckBoxItemWidget::InitWidget()
{
    m_pCheckBox = new MyCheckBox();
    m_pLblDes = new QLabel();

    connect(m_pCheckBox, SIGNAL(signalStateChanged(int)), this, SLOT(OnStateChanged(int)));

    QHBoxLayout *pHBoxLayout = new QHBoxLayout();
    pHBoxLayout->setMargin(0);
    pHBoxLayout->setSpacing(0);

    pHBoxLayout->addWidget(m_pCheckBox);
    pHBoxLayout->addWidget(m_pLblDes);
    pHBoxLayout->addStretch();

    this->setLayout(pHBoxLayout);
}

SpecialBtn::SpecialBtn(QWidget *parent) :
    QPushButton(parent)
{
    InitWidget();
    m_bPressedView = false;
}

SpecialBtn::~SpecialBtn()
{

}

void SpecialBtn::SetLblStyleSheet(const QString &hover, const QString &noraml,  const QString &pressedStylesheet)
{
    m_hoverStyleSheet = hover;
    m_normalStyleSheet = noraml;
    m_pressedStyleSheet = pressedStylesheet;
}

void SpecialBtn::SetFrameStyleSheet(const QString &hover, const QString &noraml,  const QString &pressedStylesheet)
{
    m_frameHoverStyleSheet = hover;
    m_frameNormalStyleSheet = noraml;
    m_framePressedStyleSheet = pressedStylesheet;
}

void SpecialBtn::SetDesText(const QString &text)
{
    m_pLblDes->setText(text);
}

void SpecialBtn::NormalView()
{
    m_bPressedView = false;
    m_pLblDes->setStyleSheet(m_normalStyleSheet);
    m_pFenGeXian->setStyleSheet(m_frameNormalStyleSheet);
}

void SpecialBtn::ViewPressed()
{
    m_pLblDes->setStyleSheet(m_pressedStyleSheet);
    m_pFenGeXian->setStyleSheet(m_framePressedStyleSheet);
    m_bPressedView = true;
}

bool SpecialBtn::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (m_bPressedView) return QPushButton::eventFilter(pObj, pEvent);

    if (pEvent->type() == QEvent::HoverEnter)
    {
        m_pLblDes->setStyleSheet(m_hoverStyleSheet);
        m_pFenGeXian->setStyleSheet(m_frameHoverStyleSheet);
    }
    else if (pEvent->type() == QEvent::HoverLeave)
    {
        m_pLblDes->setStyleSheet(m_normalStyleSheet);
        m_pFenGeXian->setStyleSheet(m_frameNormalStyleSheet);
    }
    else if (pEvent->type() == QEvent::MouseButtonPress)
    {
        m_pLblDes->setStyleSheet(m_pressedStyleSheet);
        m_pFenGeXian->setStyleSheet(m_framePressedStyleSheet);
        m_bPressedView = true;
    }

    return QPushButton::eventFilter(pObj, pEvent);
}

void SpecialBtn::InitWidget()
{
    QHBoxLayout *pHBoxLayout = new QHBoxLayout();
    pHBoxLayout->setMargin(0);
    pHBoxLayout->setSpacing(0);

    m_pLblDes = new QLabel();
    //m_pLblDes->setMargin(Qt::AlignCenter);
    m_pFenGeXian = new QFrame();
    m_pFenGeXian->setFixedWidth(4);

    pHBoxLayout->addWidget(m_pFenGeXian);
    pHBoxLayout->addWidget(m_pLblDes);

    this->setLayout(pHBoxLayout);

    this->installEventFilter(this);
}


SpecialVerticalBtn::SpecialVerticalBtn(QWidget *parent) :
QPushButton(parent)
{
    InitWidget();
    m_bPressedView = false;
}

SpecialVerticalBtn::~SpecialVerticalBtn()
{

}

void SpecialVerticalBtn::SetLblStyleSheet(const QString &hover, const QString &noraml,  const QString &pressedStylesheet)
{
    m_hoverStyleSheet = hover;
    m_normalStyleSheet = noraml;
    m_pressedStyleSheet = pressedStylesheet;
}

void SpecialVerticalBtn::SetFrameStyleSheet(const QString &hover, const QString &noraml,  const QString &pressedStylesheet)
{
    m_frameHoverStyleSheet = hover;
    m_frameNormalStyleSheet = noraml;
    m_framePressedStyleSheet = pressedStylesheet;
}

void SpecialVerticalBtn::SetDesText(const QString &text)
{
    m_pLblDes->setText(text);
}

void SpecialVerticalBtn::NormalView()
{
    m_bPressedView = false;
    m_pLblDes->setStyleSheet(m_normalStyleSheet);
    m_pFenGeXian->setStyleSheet(m_frameNormalStyleSheet);
}

void SpecialVerticalBtn::ViewPressed()
{
    m_pLblDes->setStyleSheet(m_pressedStyleSheet);
    m_pFenGeXian->setStyleSheet(m_framePressedStyleSheet);
    m_bPressedView = true;
}

bool SpecialVerticalBtn::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (m_bPressedView) return QPushButton::eventFilter(pObj, pEvent);

    if (pEvent->type() == QEvent::HoverEnter)
    {
        m_pLblDes->setStyleSheet(m_hoverStyleSheet);
        m_pFenGeXian->setStyleSheet(m_frameHoverStyleSheet);
    }
    else if (pEvent->type() == QEvent::HoverLeave)
    {
        m_pLblDes->setStyleSheet(m_normalStyleSheet);
        m_pFenGeXian->setStyleSheet(m_frameNormalStyleSheet);
    }
    else if (pEvent->type() == QEvent::MouseButtonPress)
    {
        m_pLblDes->setStyleSheet(m_pressedStyleSheet);
        m_pFenGeXian->setStyleSheet(m_framePressedStyleSheet);
        m_bPressedView = true;
    }

    return QPushButton::eventFilter(pObj, pEvent);
}

void SpecialVerticalBtn::InitWidget()
{
    QVBoxLayout *pVBoxLayout = new QVBoxLayout();
    pVBoxLayout->setMargin(0);
    pVBoxLayout->setSpacing(0);

    m_pLblDes = new QLabel();
    //m_pLblDes->setMargin(Qt::AlignCenter);
    m_pFenGeXian = new QFrame();
    m_pFenGeXian->setFixedWidth(2);

    pVBoxLayout->addWidget(m_pLblDes);
    pVBoxLayout->addWidget(m_pFenGeXian);

    this->setLayout(pVBoxLayout);
    this->installEventFilter(this);
}

DefineSlidingBtnItem::DefineSlidingBtnItem(QWidget *parent) :
    QPushButton(parent)
{
    InitWidget();
    m_bPressedView = false;
    this->installEventFilter(this);

    connect(this, SIGNAL(clicked()), this, SLOT(OnClickedEventCreated()));
}

DefineSlidingBtnItem::~DefineSlidingBtnItem()
{

}

void DefineSlidingBtnItem::SetLblStyleSheet(const QString &hover, const QString &noraml,  const QString &pressedStylesheet)
{
    m_hoverStyleSheet = hover;
    m_normalStyleSheet = noraml;
    m_pressedStyleSheet = pressedStylesheet;
    m_pLblDes->setStyleSheet(noraml);
}

void DefineSlidingBtnItem::SetDesText(const QString &text, Qt::Alignment align)
{
    //m_pLblDes->setAlignment(align);i
    m_pLblDes->setText(text);
    m_pLblDes->setAlignment(align);
}

void DefineSlidingBtnItem::NormalView()
{
    m_pLblDes->setStyleSheet(m_normalStyleSheet);
    m_bPressedView = false;
}

void DefineSlidingBtnItem::ViewPressed()
{
    m_pLblDes->setStyleSheet(m_pressedStyleSheet);
    m_bPressedView = true;
}

void DefineSlidingBtnItem::OnClickedEventCreated()
{
    m_pLblDes->setStyleSheet(m_pressedStyleSheet);
    m_bPressedView = true;
}

bool DefineSlidingBtnItem::eventFilter(QObject *pObj, QEvent *pEvent)
{
    if (m_bPressedView) return QPushButton::eventFilter(pObj, pEvent);

    if (pEvent->type() == QEvent::HoverEnter)
    {
        m_pLblDes->setStyleSheet(m_hoverStyleSheet);
    }
    else if (pEvent->type() == QEvent::HoverLeave)
    {
        m_pLblDes->setStyleSheet(m_normalStyleSheet);
    }
    //else if (pEvent->type() == QEvent::MouseButtonPress)
    //{
    //
    //}

    return QPushButton::eventFilter(pObj, pEvent);
}

void DefineSlidingBtnItem::InitWidget()
{
    QHBoxLayout *pHBoxLayout = new QHBoxLayout();
    pHBoxLayout->setMargin(0);
    pHBoxLayout->setMargin(0);

    m_pLblDes = new QLabel();
    pHBoxLayout->addWidget(m_pLblDes, Qt::AlignCenter);

    this->setLayout(pHBoxLayout);
}

DefineSlidingBtnGroup::DefineSlidingBtnGroup(int nStyle, int nSpacingValue, const QString &itemStyleSheet, const QSize &itemSize, QWidget *parent) :
    m_nViewStyleValue(nStyle), m_nSpacingValue(nSpacingValue), m_strItemStyleSheet(itemStyleSheet), m_itemBtnSize(itemSize), QWidget(parent)
{
    m_nPreViewBtnId = -1;
    InitWidget(nStyle);
    connect(m_pBtnButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(OnButtonClicked(int)));

    m_pQPropertyAnimation = NULL;
}



DefineSlidingBtnGroup::~DefineSlidingBtnGroup()
{

}

void DefineSlidingBtnGroup::SetItemStyleSheet(const QString &normal, const QString &hover, const QString &pressed)
{
    m_strNormalStyleSheet = normal;
    m_strHoverStyleSheet = hover;
    m_strPressedStyleSheet = pressed;
}

void DefineSlidingBtnGroup::AddItem(const QString &des, int nBtnId, Qt::Alignment alignment)
{
    int nCurrentBtnNums = m_pBtnButtonGroup->buttons().count();
    if (E_HBOXLAYOUT_STYLE == m_nViewStyleValue)
    {
        AddHBoxItem(nCurrentBtnNums, des, nBtnId, alignment);
    }
    else if (E_VBOXLAYOUT_SYTLE == m_nViewStyleValue)
    {
        AddVBoxItem(nCurrentBtnNums, des, nBtnId, alignment);
    }

    int nFixedVlaue =  m_pBtnButtonGroup->buttons().size() * m_itemBtnSize.width() + (m_pBtnButtonGroup->buttons().size() - 1) * (m_nSpacingValue);
    this->setFixedWidth(nFixedVlaue);
}

void DefineSlidingBtnGroup::AddHBoxItem(int nIndex, const QString &des, int nBtnId, Qt::Alignment alignment)
{
    if (!m_pBtnButtonGroup->button(nBtnId))
    {
        DefineSlidingBtnItem *pBtnItem = new DefineSlidingBtnItem();
        pBtnItem->setFixedSize(m_itemBtnSize);
        pBtnItem->SetLblStyleSheet(m_strHoverStyleSheet, m_strNormalStyleSheet, m_strPressedStyleSheet);
        pBtnItem->SetDesText(des, alignment);
        m_pMainHBoxLayout->insertWidget(nIndex, pBtnItem);
        m_pBtnButtonGroup->addButton(pBtnItem, nBtnId);
    }
}

void DefineSlidingBtnGroup::AddVBoxItem(int nIndex, const QString &des, int nBtnId, Qt::Alignment alignment)
{
    if (!m_pBtnButtonGroup->button(nBtnId))
    {
        DefineSlidingBtnItem *pBtnItem = new DefineSlidingBtnItem();
        pBtnItem->setFixedSize(m_itemBtnSize);
        pBtnItem->SetLblStyleSheet(m_strHoverStyleSheet, m_strNormalStyleSheet, m_strPressedStyleSheet);
        pBtnItem->SetDesText(des, alignment);
        m_pMainVBoxLayout->insertWidget(nIndex, pBtnItem);
        m_pBtnButtonGroup->addButton(pBtnItem, nBtnId);
    }
}

//设置窗体的样式表
void DefineSlidingBtnGroup::SetStyleSheet(const QString &styleSheet)
{
    if (!styleSheet.isEmpty())
    {
        this->setStyleSheet(styleSheet);
    }
}

void DefineSlidingBtnGroup::SetCurrentViewId(int nBtnId)
{
    if (m_pBtnButtonGroup->button(nBtnId))
    {
        m_nPreViewBtnId = nBtnId;
        DefineSlidingBtnItem *pBtnItem = static_cast<DefineSlidingBtnItem*>(m_pBtnButtonGroup->button(nBtnId));

        m_pLineSlidding->raise();

        if (E_HBOXLAYOUT_STYLE == m_nViewStyleValue)
        {
            QPoint prePointTopLeft = pBtnItem->pos();
            QRect startRect = QRect(prePointTopLeft.x(), prePointTopLeft.y() + pBtnItem->height() - m_nLineWidth, m_pLineSlidding->width(), m_pLineSlidding->height());

            m_pLineSlidding->move(prePointTopLeft);
            m_pLineSlidding->show();

            m_pQPropertyAnimation->stop();
            m_pQPropertyAnimation->setStartValue(startRect);
            m_pQPropertyAnimation->setEndValue(startRect);
            m_pQPropertyAnimation->setDuration(100);
            m_pQPropertyAnimation->start();
        }
        else if (E_VBOXLAYOUT_SYTLE == m_nViewStyleValue)
        {
            QPoint prePointTopLeft = pBtnItem->pos();
            QRect startRect = QRect(pBtnItem->pos().x(), pBtnItem->pos().y(), m_pLineSlidding->width(), m_pLineSlidding->height());

            m_pLineSlidding->move(prePointTopLeft);
            m_pLineSlidding->show();

            m_pQPropertyAnimation->stop();
            m_pQPropertyAnimation->setStartValue(startRect);
            m_pQPropertyAnimation->setEndValue(startRect);
            m_pQPropertyAnimation->setDuration(100);
            m_pQPropertyAnimation->start();
        }

        pBtnItem->ViewPressed();
    }

    //m_pLineSlidding->raise();
}

void DefineSlidingBtnGroup::SetCurrentBtnItemClicked(int nBtnId)
{
    OnButtonClicked(nBtnId);
}

int DefineSlidingBtnGroup::GetCurrentBtnId()
{
    return m_nPreViewBtnId;
}

void DefineSlidingBtnGroup::SetCurrentViewIdPage(int nBtnID)
{
    DefineSlidingBtnItem *pBtn = (DefineSlidingBtnItem*)m_pBtnButtonGroup->button(nBtnID);
    if (NULL != pBtn)
    {
        if (E_HBOXLAYOUT_STYLE == m_nViewStyleValue)
        {
            QPoint movePs = pBtn->mapToParent(pBtn->geometry().bottomLeft());
            m_pLineSlidding->move(movePs.x(), movePs.y() - m_pLineSlidding->height());
        }
        else
        {
            QPoint movePs = pBtn->mapToParent(pBtn->geometry().topLeft());
            m_pLineSlidding->move(movePs.x(), movePs.y());
        }

        m_nPreViewBtnId = nBtnID;
    }
}

void DefineSlidingBtnGroup::SetMovieLineStyleSheet(const QString &styleSheet, int nLineWidth)
{
    m_nLineWidth = nLineWidth;
    m_pLineSlidding = new QLabel(this);
    m_pQPropertyAnimation = new QPropertyAnimation(m_pLineSlidding, "geometry", this);
    connect(m_pQPropertyAnimation, SIGNAL(finished()), this, SLOT(OnAnimationEnd()));

    if (m_nViewStyleValue == E_HBOXLAYOUT_STYLE)
    {
        m_pLineSlidding->setFixedSize(QSize(m_itemBtnSize.width(), nLineWidth));
    }
    else if (m_nViewStyleValue == E_VBOXLAYOUT_SYTLE)
    {
        m_pLineSlidding->setFixedSize(QSize(nLineWidth, m_itemBtnSize.height()));
    }

    m_pLineSlidding->setStyleSheet(styleSheet);
    m_pLineSlidding->hide();
}

#include <QAbstractAnimation>

void DefineSlidingBtnGroup::OnAnimationEnd()
{
    emit DoBtnItemClicked(m_nPreViewBtnId);
}

void DefineSlidingBtnGroup::OnBtnStatusDeal()
{
    QList<QAbstractButton *> m_lstAbsTractBtn = m_pBtnButtonGroup->buttons();
    for (int index = 0; index < m_lstAbsTractBtn.size(); ++index)
    {
        DefineSlidingBtnItem *pBtnItem = static_cast<DefineSlidingBtnItem*>(m_lstAbsTractBtn.at(index));
        if (NULL != pBtnItem)
        {
            if (m_pBtnButtonGroup->id(pBtnItem) != m_nPreViewBtnId)
            {
                pBtnItem->NormalView();
            }
            else
            {
                pBtnItem->ViewPressed();
            }
        }
    }
}

void DefineSlidingBtnGroup::OnButtonClicked(int nBtenClicked)
{
    do
    {
        if (m_nPreViewBtnId == nBtenClicked) break; //点击同一个按钮 直接返回

        DefineSlidingBtnItem *pBtn = (DefineSlidingBtnItem*)m_pBtnButtonGroup->button(nBtenClicked);
        if (NULL != pBtn)
        {
            if (m_nPreViewBtnId != -1)
            {
                DefineSlidingBtnItem *pPreBtn = (DefineSlidingBtnItem*)m_pBtnButtonGroup->button(m_nPreViewBtnId);
                //pPreBtn->NormalView();
                if (NULL != pPreBtn)
                {
                    //pPreBtn->NormalView();
                    //pBtn->ViewPressed();

                    if (m_pQPropertyAnimation->state() == QAbstractAnimation::Running)
                    {
                        m_pQPropertyAnimation->pause();
                        m_pQPropertyAnimation->stop();
                    }
                    else if (m_pQPropertyAnimation->state() == QAbstractAnimation::Paused)
                    {
                        m_pQPropertyAnimation->stop();
                    }

                    //这里产生动画
                    if (E_HBOXLAYOUT_STYLE == m_nViewStyleValue)
                    {
                        QPoint prePointTopLeft = pPreBtn->pos();
                        QPoint endPointTopLeft = pBtn->pos();

                        QRect startRect = QRect(prePointTopLeft.x(), prePointTopLeft.y() + pPreBtn->height() - m_nLineWidth, m_pLineSlidding->width(), m_pLineSlidding->height());
                        QRect endRect = QRect(pBtn->pos().x(), pBtn->pos().y() + pBtn->height() - m_nLineWidth, m_pLineSlidding->width(), m_pLineSlidding->height());
                        m_pQPropertyAnimation->setStartValue(startRect);
                        m_pQPropertyAnimation->setEndValue(endRect);
                        m_pQPropertyAnimation->setDuration(100);
                        m_pQPropertyAnimation->setEasingCurve(QEasingCurve::OutQuad);
                        m_pQPropertyAnimation->start();
                    }
                    else if (E_VBOXLAYOUT_SYTLE == m_nViewStyleValue)
                    {
                        QRect startRect = QRect(pPreBtn->pos().x(), pPreBtn->pos().y(), m_pLineSlidding->width(), m_pLineSlidding->height());
                        QRect endRect = QRect(pBtn->pos().x(), pBtn->pos().y(), m_pLineSlidding->width(), m_pLineSlidding->height());
                        m_pQPropertyAnimation->setStartValue(startRect);
                        m_pQPropertyAnimation->setEndValue(endRect);
                        m_pQPropertyAnimation->setDuration(100);
                        m_pQPropertyAnimation->setEasingCurve(QEasingCurve::OutQuad);
                        m_pQPropertyAnimation->start();
                    }
                }

                //pPreBtn->NormalView();
            }
        }

        m_nPreViewBtnId = nBtenClicked;

    } while (false);

    OnBtnStatusDeal();

}

void DefineSlidingBtnGroup::InitWidget(int nStyle)
{
    if (E_HBOXLAYOUT_STYLE == nStyle)
    {
        m_pMainHBoxLayout = new QHBoxLayout();
        m_pMainHBoxLayout->setMargin(0);
        m_pMainHBoxLayout->setSpacing(m_nSpacingValue);

        this->setLayout(m_pMainHBoxLayout);
    }
    else if (E_VBOXLAYOUT_SYTLE == nStyle)
    {
        m_pMainVBoxLayout = new QVBoxLayout();
        m_pMainVBoxLayout->setMargin(0);
        m_pMainVBoxLayout->setSpacing(m_nSpacingValue);

        this->setLayout(m_pMainVBoxLayout);
    }

    m_pBtnButtonGroup = new QButtonGroup(this);
}


TaskSureToolWidget::TaskSureToolWidget(QWidget *parent) :
    CommonShadowWidget(parent)
{
    m_bSure = false;

    InitWidget();
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
}

TaskSureToolWidget::~TaskSureToolWidget()
{

}

void TaskSureToolWidget::SetTitle(const QString &title)
{
    m_pTitleLbl->setText(title);
}

void TaskSureToolWidget::SetDesContent(const QString &strContent)
{
    m_pInfoDesLbl->setText(strContent);
}

void TaskSureToolWidget::SetCheckBoxDes(const QString &checkDesContent)
{
    m_pCheckBox->setText(checkDesContent);
}

void TaskSureToolWidget::setOkBtnText(const QString &_text)
{
    pOkBtn->setText(_text);
}

void TaskSureToolWidget::Exec()
{
    pCancelBtn->setFocus();

    this->exec();
}

bool TaskSureToolWidget::IsSure()
{
    return m_bSure;
}

void TaskSureToolWidget::GetSelectStatus(bool &bGetSelect)
{
    bool bTempGetSelect = m_pCheckBox->checkState() == Qt::Checked;
    bGetSelect = bTempGetSelect;
}

void TaskSureToolWidget::HideCheckBox()
{
    m_pCheckBox->hide();
}

void TaskSureToolWidget::OnBtnOk()
{
    m_bSure = true;
    hide();
}

void TaskSureToolWidget::OnClancel()
{
    m_bSure = false;
    hide();
}

void TaskSureToolWidget::InitWidget()
{
    //============================================================================================
    QWidget *pHeadWidget = new QWidget();
    pHeadWidget->setFixedHeight(COM_TITLE_HIGHT);
    m_pTitleLbl = new QLabel();
    m_pTitleLbl->setStyleSheet("QLabel{font-size:12px; color:#fff;}");
    m_pTitleLbl->setFixedHeight(COM_TITLE_HIGHT);
    m_pTitleLbl->setAlignment(Qt::AlignLeft | Qt::AlignCenter);

    QPushButton *pCloseBtn = new QPushButton();
    CommonUiInit::initCloseButton(pCloseBtn);
    connect(pCloseBtn, SIGNAL(clicked()), this, SLOT(OnClancel()));

    QHBoxLayout *pHeadHBoxLayout = new QHBoxLayout();
    pHeadHBoxLayout->setMargin(0);
    pHeadHBoxLayout->setSpacing(0);
    pHeadHBoxLayout->addSpacing(COM_TITLETEXT_LEFT_MARGIN);
    pHeadHBoxLayout->addWidget(m_pTitleLbl);
    pHeadHBoxLayout->addStretch();
    pHeadHBoxLayout->addWidget(pCloseBtn, 0, Qt::AlignTop);
    pHeadHBoxLayout->addSpacing(COM_CLOSEBTN_RIGHT_MARGIN);

    pHeadWidget->setLayout(pHeadHBoxLayout);
    pHeadWidget->setObjectName("HeadWidget");
    pHeadWidget->setStyleSheet("QWidget#HeadWidget{background-color:#33a9f4;}");

    //============================================================================================
    QLabel *pToolIcon = new QLabel();
    pToolIcon->setFixedSize(44,44);
    pToolIcon->setStyleSheet("QLabel{border-image:url(:/Img/res/MsgTipDialog/46x46_determine.png);}");

    m_pInfoDesLbl = new QLabel();
    m_pInfoDesLbl->setStyleSheet("QLabel{font-size:12px; color:#494949;}");

    QHBoxLayout *pDesHBoxLayout = new QHBoxLayout();
    pDesHBoxLayout->setMargin(0);
    pDesHBoxLayout->setSpacing(0);

    pDesHBoxLayout->addSpacing(MSGBOX_BTN_MARGIN);
    pDesHBoxLayout->addWidget(pToolIcon);
    pDesHBoxLayout->addSpacing(10);
    pDesHBoxLayout->addWidget(m_pInfoDesLbl);
    pDesHBoxLayout->addStretch();

    //============================================================================================
    m_pCheckBox = new MyCheckBox();

    QString strBtnQSS = QString("QPushButton{border:none; background-color:#33a9f4; font-size:12px; color:white;}QPushButton:hover{background-color:#4dbeff;}QPushButton:pressed{background-color:#0897f2;}");

    pOkBtn = new QPushButton();
    pOkBtn->setFixedSize(CommonUiInit::okButtonSize());
    pOkBtn->setStyleSheet(strBtnQSS);
    pOkBtn->setText(tr("删除"));
    connect(pOkBtn, SIGNAL(clicked()), this, SLOT(OnBtnOk()));

    pCancelBtn = new QPushButton();
    pCancelBtn->setFixedSize(CommonUiInit::cancelButtonSize());
    pCancelBtn->setStyleSheet(QString("QPushButton{border:1px solid #dddddd;  background-color:white; font-size:14px; color:#494949;}QPushButton:hover{border:1px solid #33a9f4; color:#33a9f4;}QPushButton:pressed{border:1px solid #0897f2; background-color:#e6f4fe; color:#0897f2;}"));
    pCancelBtn->setText(tr("取消"));
    connect(pCancelBtn, SIGNAL(clicked()), this, SLOT(OnClancel()));

    QHBoxLayout *pCheckHBoxLayout = new QHBoxLayout();
    pCheckHBoxLayout->setMargin(0);
    pCheckHBoxLayout->setSpacing(0);

    //pCheckHBoxLayout->addSpacing(11);
    pCheckHBoxLayout->addSpacing(MSGBOX_BTN_MARGIN - 12);
    pCheckHBoxLayout->addWidget(m_pCheckBox);
    pCheckHBoxLayout->addStretch();
    pCheckHBoxLayout->addWidget(pOkBtn);
    pCheckHBoxLayout->addSpacing(14);
    pCheckHBoxLayout->addWidget(pCancelBtn);
    pCheckHBoxLayout->addSpacing(MSGBOX_BTN_MARGIN);

    //============================================================================================
    QVBoxLayout *pMainBoxLayout = new QVBoxLayout();
    pMainBoxLayout->setSpacing(0);
    pMainBoxLayout->setMargin(0);

    pMainBoxLayout->addWidget(pHeadWidget);
    pMainBoxLayout->addSpacing(38);
    pMainBoxLayout->addLayout(pDesHBoxLayout);
    //pMainBoxLayout->addSpacing(93);
    pMainBoxLayout->addStretch();
    pMainBoxLayout->addLayout(pCheckHBoxLayout);
    pMainBoxLayout->addSpacing(MSGBOX_BTN_MARGIN);

    QWidget *pBottomWidget = new QWidget();
    pBottomWidget->setLayout(pMainBoxLayout);

    pBottomWidget->setObjectName("BottomWidget");
    pBottomWidget->setStyleSheet("QWidget#BottomWidget{background-color:white;}");

    setMiddleWidget(pBottomWidget);
    this->setFixedSize(COM_MSGBOX_WIDTH, COM_MSGBOX_HEIGHT);
}

MyButtonGroup::MyButtonGroup(QWidget *parent) :
    QButtonGroup(parent)
{

}

MyButtonGroup::~MyButtonGroup()
{

}

void MyButtonGroup::EmitBtnCliked(int nBtnId)
{
    emit buttonClicked(nBtnId);
}
#include "./class/common/iconLocation.h"


MyRedioButton::MyRedioButton(QWidget *parent) :
    QRadioButton(parent)
{
    QString qssStr = "\
                     QRadioButton::indicator {\
                     width: %1px;\
                     height: %2px;\
                     }\
                     QRadioButton{\
                     background:transparent;\
                     spacing: 0px;\
                     font:12px;\
                     color:%3;\
                     }\
                     QRadioButton:hover{color:%4;}\
                     QRadioButton::indicator:unchecked {\
                     image: url(%5);\
                     }\
                     QRadioButton::indicator:checked {\
                     image: url(%6);\
                     }\
                     QRadioButton::indicator:unchecked:hover{\
                     image: url(%7);\
                     }\
                     QRadioButton:!enabled{\
                     color:#999999; \
                     }\
                     QRadioButton::indicator!enabled{\
                     image: url(%8);\
                     }";

    setStyleSheet(qssStr.arg(QString::number(NAVIGATE_MIDDLE_CHECKBOX_ICON_WIDTH),QString::number(NAVIGATE_MIDDLE_CHECKBOX_ICON_HEIGHT),TEXT_COLOR_BLACK, BASE_COLOR2, COM_CIRCLE_UNSELECT, COM_CIRCLE_SELECT, COM_CIRCLE_UNSELECT, COM_CIRCLE_UNSELECT));
}

MyRedioButton::~MyRedioButton()
{

}

MultiFuncBtn::MultiFuncBtn(QWidget *parent /* = NULL */)
    : QWidget(parent)
{
    m_nCurrentAttriTypeValue = -1;
    InitWidget();

    connect(m_pFunBtn, SIGNAL(clicked()), this, SLOT(OnClicked()));
}

MultiFuncBtn::~MultiFuncBtn()
{

}

void MultiFuncBtn::SetCurrentAttri(int nAtttrType)
{
    if (m_lstAttrValueMap.contains(nAtttrType))
    {
        m_nCurrentAttriTypeValue = nAtttrType;
        QList<QPair<int, QVariant> > lstAttriItem = m_lstAttrValueMap.value(nAtttrType);

        for (int index = 0; index < lstAttriItem.size(); ++index)
        {
            QPair<int,QVariant> pairAttriButeValue = lstAttriItem.at(index);

            switch(pairAttriButeValue.first)
            {
            case E_VIEW_TEXT:
                {
                    m_pFunBtn->setText(pairAttriButeValue.second.toString());
                    break;
                }

            case E_STYLE_SHEET:
                {
                    m_pFunBtn->setStyleSheet(pairAttriButeValue.second.toString());
                    break;
                }

            case E_CURSOR_TYPE:
                {
                    m_pFunBtn->setCursor(Qt::CursorShape(pairAttriButeValue.second.toInt()));
                    break;
                }

            case E_FRAME_SIZE:
                {
                    m_pFunBtn->setFixedSize(pairAttriButeValue.second.toSize());
                    this->setFixedSize(pairAttriButeValue.second.toSize());
                    break;
                }

            default:
                break;
            }
        }
    }
}

void MultiFuncBtn::AddAttriTypeValue(int nAttriType, int nAttriItemType, const QVariant &attriItemValue)
{
    if (m_lstAttrValueMap.contains(nAttriType))
    {
        m_lstAttrValueMap[nAttriType].append(QPair<int, QVariant>(nAttriItemType, attriItemValue));
    }
    else
    {
        QList<QPair<int, QVariant> > lstAttriItem;
        lstAttriItem.append(QPair<int, QVariant>(nAttriItemType, attriItemValue));

        m_lstAttrValueMap.insert(nAttriType, lstAttriItem);
    }
}

void MultiFuncBtn::SetDisplayText(const QString &displayText)
{
    m_pFunBtn->setText(displayText);
}

void MultiFuncBtn::OnClicked()
{
    emit DoClickedType(m_nCurrentAttriTypeValue);
}

void MultiFuncBtn::InitWidget()
{
    QHBoxLayout *pHBoxLayout = new QHBoxLayout();
    pHBoxLayout->setSpacing(0);
    pHBoxLayout->setMargin(0);

    m_pFunBtn = new QPushButton();
    pHBoxLayout->addWidget(m_pFunBtn);

    this->setLayout(pHBoxLayout);
}
