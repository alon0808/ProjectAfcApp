#ifndef LTY_WATCH_DOG_H
#define LTY_WATCH_DOG_H

#include "LtyTimer.h"

class CWatchDog : private CEvTimer
{
public:
    CWatchDog();
    virtual ~CWatchDog();

    LTY_RESULT Init(struct ev_loop *loop);
    LTY_VOID   Release();
	LTY_RESULT SetTimeOut(LTY_U32 u32Sec);

private:
    LTY_VOID ETOnTimer(ev_timer_t *pTimer, LTY_U32 u32Type, LTY_LPVOID pPrivData);

    LTY_RESULT Open();
    LTY_RESULT Feed();
    LTY_VOID   Close();
    LTY_RESULT Disable();
    LTY_RESULT Enable();
    
	
    struct ev_loop* m_loop;
    int             m_fd;
    ev_timer_t      m_tmrFeed;
};

#endif

