#ifndef LTY_TIMER_H
#define LTY_TIMER_H

#include <assert.h>

#include "ev.h"
#include "LtyTypeDef.h"
#include "LtyError.h"

typedef struct ev_timer_s
{
    ev_timer evTimer;
    LTY_U32  u32Type;
    LTY_LPVOID pPrivData;
    LTY_BOOL bIsActive;
}ev_timer_t;

class CEvTimer
{
public:
    CEvTimer();
    virtual ~CEvTimer();

    virtual LTY_VOID ETOnTimer(ev_timer_t *pTimer, LTY_U32 u32Type, LTY_LPVOID pPrivData) { assert(0);}

protected:
    LTY_RESULT ETInit(struct ev_loop *loop);
    LTY_RESULT ETTimerInit(ev_timer_t *pTimer, LTY_U32 u32Type, LTY_LPVOID pPrivData);
    LTY_RESULT ETTimerReInit(ev_timer_t *pTimer, LTY_U32 u32Type, LTY_LPVOID pPrivData);
    LTY_RESULT ETTimerStart(ev_timer_t *pTimer, LTY_U32 u32TimeOutMs, LTY_U32 u32DelayMs=0xffffffff);
    LTY_VOID   ETTimerStop(ev_timer_t *pTimer);
    LTY_BOOL   IsStart(ev_timer_t *pTimer);

private:
    struct ev_loop *m_loop;
};

#endif

