#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include <QEvent>


#define QEVENT_LTY_ROOT		5005


class CustomEvent : public QEvent
{
public:
    explicit CustomEvent(const QEvent::Type typeId, void *param);
    ~CustomEvent();

public:
    void *m_pParam;
    static const QEvent::Type m_lty_root;

};

#endif // CUSTOMEVENT_H
