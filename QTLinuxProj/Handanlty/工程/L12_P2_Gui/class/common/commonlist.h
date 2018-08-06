#ifndef COMMONLIST_H
#define COMMONLIST_H

#include <QWidget>

class CommonList : public QWidget
{
    Q_OBJECT

public:
    CommonList(QWidget *parent);
    ~CommonList();

signals:
    void signalCurrentListCount(int);
    void signalCurrentListCount(int, int);
    void signalUpdateTopLevelCheckStatus(int);
    void signalUpdateCountInfo(int, int);
    void signalRightClickEmptyArea(const QPoint &);
    void signalClickEmptyArea(const QPoint &);
    void signalShowToolTip(int, int, const QPoint &);
private:

};


#endif // COMMONLIST_H
