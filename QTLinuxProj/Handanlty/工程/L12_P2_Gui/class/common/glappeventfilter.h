#ifndef GLAPPEVENTFILTER_H
#define GLAPPEVENTFILTER_H

#include <QObject>
#include <QEvent>


class GlAppEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit GlAppEventFilter(QObject *parent = 0);
    static GlAppEventFilter *getInstance();
signals:
    
public slots:
    
private:
    bool eventFilter(QObject *pObj, QEvent *pEvent);
    static GlAppEventFilter *m_single;
};

#endif // GLAPPEVENTFILTER_H
