#include "customevent.h"

const QEvent::Type CustomEvent::m_lty_root = static_cast<QEvent::Type>(QEvent::registerEventType(QEVENT_LTY_ROOT));


CustomEvent::CustomEvent(const QEvent::Type typeId, void *param) : QEvent(typeId)
{
    m_pParam = param;
}

CustomEvent::~CustomEvent()
{

}
