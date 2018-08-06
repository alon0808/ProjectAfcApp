#ifndef COMMONSHADOWWIDGET_H
#define COMMONSHADOWWIDGET_H

#include <QWidget>

#include <QWidget>
#include <QPoint>
#include <QDialog>
class QGraphicsDropShadowEffect;
class QHBoxLayout;
class QMouseEvent;
class CommonShadowWidgetPrivate;

class CommonShadowWidget : public QDialog
{
    Q_OBJECT

public:
    CommonShadowWidget(QWidget *parent = 0);
    ~CommonShadowWidget();

public:
    void setMouseActivateHeight(int paramHeight);
    int mouseActivateHeight() const;
    void setMiddleWidget(QWidget *, int _width = 0, int _height = 0);
    void updateSize(int _width, int _height);
    void removeMiddleWidget(QWidget *);
    void setHideWhenPhoneDisconnectFlag(bool _flag);
    void setEnableCloseWidgetByKeyEscape(bool _flag);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool eventFilter(QObject *, QEvent *);

private:
    void createWidgets();
    int countFlag(QPoint p, int row);
    void setCursorType(int flag);
    int countRow(QPoint p);

signals:
    void signalShadowWidgetPhoneDisconnect();
    void signalCloseWidgetByKeyEscape();
    void signalNeedShowNormal();

public slots:
    void slotShadowWidgetPhoneDisconnect();

private:
    CommonShadowWidgetPrivate *d;
    int curPos;
    bool m_bMove;
};

#endif // COMMONSHADOWWIDGET_H
