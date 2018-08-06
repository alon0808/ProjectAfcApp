#ifndef NAVIGATELEFTBUTTON_H
#define NAVIGATELEFTBUTTON_H

#include <QWidget>

#include <QToolButton>
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QEvent;

#include "class/common/define_screen.h"

class NavigateLeftButton : public QToolButton
{
    Q_OBJECT

        Q_ENUMS(ButtonMode)

public:
    enum ButtonMode { NavigateLeftMode, NavigateTopMode, PhoneInfoMode, PhoneToolBoxMode, NavigateMiddleMode, BackupRestoreEntryMode, BackupMode,PullMode,BottomMode,StorageMode,NextMode};

    enum
    {
        kEnumNormal,
        kEnumPressed,
        kEnumActive
    };

public:
    NavigateLeftButton(int _width,int _height,ButtonMode paramButtonMode = NavigateLeftMode, QWidget *parent = 0, QString paramNormalIconStr = QString(), QString paramActiveIconStr = QString(), QString paramPressedIconStr = QString(), QString paramDisabledIconStr = QString());
    ~NavigateLeftButton();

    void setSelected(bool paramFlag, bool paramIsForce = false);
    void setNormalIconStr(const QString &);
    void setActiveIconStr(const QString &);
    void setPressedIconStr(const QString &);
    void setDisabledIconStr(const QString &);
    void setDisplayText(const QString &,int paramAddtionMarginRight = 0);
    void setButtonMode(ButtonMode);
    ButtonMode buttonMode();
    void setButtonEnabled(bool);
    void setNavigateMiddleHeight(int);
    void setIconSize(int _w,int _h);
    static void setAllButtonEnable(bool paramFlag, QList<NavigateLeftButton *> paramAllBtnList, QList<NavigateLeftButton *> paramIgnoreList);
    QString getButtonText();
    void setText1Text2(const QString &_text1,const QString &_text2);
    void set_dispersed_align(bool _flag);
    char get_button_status();

    void updatePressStyleSheet(bool _first = false);
    void updateNormalStyleSheet(bool _first = false);
    void updateActiveStyleSheet();

private:
    void focusInEvent (QFocusEvent *e);
    void focusOutEvent (QFocusEvent *e);
    void createWidgets();
    void initSlotConnection();

    void updateDisabledStyleSheet();
    void updateWidth();
    void resizeEvent(QResizeEvent *e);

protected:
    bool eventFilter(QObject *obj, QEvent *e);

private:
    QLabel *m_QLabelBackground;
    QLabel *m_QLabelIcon;
    QLabel *m_QLabelText;
    QHBoxLayout *m_pHMainLayout;
    QVBoxLayout *m_pVMainLayout;
    bool m_isSelected;

    QString m_normalIconStr;
    QString m_activeIconStr;
    QString m_pressedIconStr;
    QString m_disabledIconStr;

    QLabel *m_label_text1;
    QLabel *m_label_text2;

    ButtonMode m_buttonMode;

    int m_addtionMarginRight;

    int m_width;
    int m_height;
    bool m_dispersed;
private:
    CMyDefineScreen *m_define_screen;
    bool m_normal_first;
    bool m_pressed_first;
    char m_button_status;
    char m_save_status;
};

#endif // NAVIGATELEFTBUTTON_H









