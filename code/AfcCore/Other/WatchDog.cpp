#include <stdio.h>
#include <stdlib.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "WatchDog.h"
#include "LtyMessage.h"
//#include "LtyMsgSysCtrl.h"
#define WATCHDOG_DEV "/dev/watchdog"

#define LTY_MSG_SYSCTRL_BASE    420 
#define LTY_MSG_SYSCTRL_SETWDT_TIMEOUT  (LTY_MSG_SYSCTRL_BASE + 14)   


#define WATCHDOG_IOCTL_BASE     'W'
#define WDIOC_GETSUPPORT        _IOR(WATCHDOG_IOCTL_BASE, 0, struct watchdog_info)
#define WDIOC_GETSTATUS         _IOR(WATCHDOG_IOCTL_BASE, 1, int)
#define WDIOC_GETBOOTSTATUS     _IOR(WATCHDOG_IOCTL_BASE, 2, int)
#define WDIOC_KEEPALIVE         _IOR(WATCHDOG_IOCTL_BASE, 5, int)
#define WDIOC_SETTIMEOUT        _IOWR(WATCHDOG_IOCTL_BASE, 6, int)
#define WDIOC_GETTIMEOUT        _IOR(WATCHDOG_IOCTL_BASE, 7, int)
#define WDIOC_SETOPTIONS        _IOR(WATCHDOG_IOCTL_BASE, 4, int)
#define WDIOS_DISABLECARD       0x0001  /* Turn off the watchdog timer */
#define WDIOS_ENABLECARD        0x0002  /* Turn on the watchdog timer */

CWatchDog::CWatchDog()
{
    m_fd = -1;
    ETTimerInit(&m_tmrFeed, 0, NULL);
}

CWatchDog::~CWatchDog()
{
    Release();
}

LTY_RESULT WDMsgHandle(LTY_MSG_TYPE msg, LTY_U32 u32Chn, LTY_U32 u32ParaData, 
						     LTY_U32 u32Size, LTY_LPVOID pPriv)
{
	CWatchDog* pWd = (CWatchDog*)pPriv;
	return pWd->SetTimeOut(u32Chn);
}


LTY_RESULT CWatchDog::Init(struct ev_loop *loop)
{
    LTY_RESULT ret;

    ETInit(loop);

    ret = Open();
    if(LTY_SUCCESS != ret)
        return ret;
	/* 全志T3最多16s超时*/
    SetTimeOut(16);

    ret = Enable();
    if(LTY_SUCCESS != ret)
        goto ERROR;
    LtyMessageRegister(LTY_MSG_SYSCTRL_SETWDT_TIMEOUT, WDMsgHandle, 0, this);
    m_loop = loop;

    return LTY_SUCCESS;

ERROR:
    Release();
    return ret;
}

LTY_VOID CWatchDog::Release()
{
    Close();
}

LTY_VOID CWatchDog::ETOnTimer(ev_timer_t *pTimer, LTY_U32 u32Type, LTY_LPVOID pPrivData)
{
    Feed();
}

LTY_RESULT CWatchDog::Open()
{
    m_fd = open(WATCHDOG_DEV, O_RDWR);
    if(m_fd < 0)
        return errno;

    return LTY_SUCCESS;
}

LTY_RESULT CWatchDog::Feed()
{
    if(m_fd < 0)
        return ENOTINIT;
    return ioctl(m_fd, WDIOC_KEEPALIVE, 0);
}

LTY_VOID CWatchDog::Close()
{
    if(m_fd > 0)
        close(m_fd);
    m_fd = -1;
    ETTimerStop(&m_tmrFeed);
}

LTY_RESULT CWatchDog::Disable()
{
    unsigned int disable_flag_tmp;
    disable_flag_tmp = WDIOS_DISABLECARD;
    if(m_fd < 0)
        return ENOTINIT;

    ETTimerStop(&m_tmrFeed);

    return ioctl(m_fd, WDIOC_SETOPTIONS, (unsigned long *)&disable_flag_tmp);
}

LTY_RESULT CWatchDog::Enable()
{
    int ret;
    unsigned int enable_flag_tmp;
    enable_flag_tmp = WDIOS_ENABLECARD;
    if(m_fd < 0)
        return ENOTINIT;
    
    ETTimerStart(&m_tmrFeed, 3000);

    ret = ioctl(m_fd, WDIOC_SETOPTIONS, (unsigned long *)&enable_flag_tmp);
    if (0 != ret){
        printf("set dog option error %s...\n", strerror(errno));
    }
    return ret;
}

LTY_RESULT CWatchDog::SetTimeOut(LTY_U32 u32Sec)
{
    if(m_fd < 0)
        return ENOTINIT;

    return ioctl(m_fd, WDIOC_SETTIMEOUT, &u32Sec);
}

