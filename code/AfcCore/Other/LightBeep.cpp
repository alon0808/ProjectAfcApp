/// @file LightBeep.cpp
/// @brief 
/// @author Jesse Xu <zhimo811@gmail.com>
/// @version V1.0.0
/// @date 2017-11-07

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

#include "LtyTypeDef.h"
#include "LtyError.h"
#include "LtyMsgUserInput.h"
#include "LtyMsgItsSD.h"
#include "LtyMsgSysCtrl.h"
#include "LtyMsgCfg.h"
#include "LtySerialDevice.h"
#include "LtyTimer.h"

#include "LightBeep.h"
#define LIGHT_BEEP_COM "/dev/tm1650"
#define DISPLAY_TIMER 0
#define DISPLAY_DURATION_MS      10

CLightBeep::CLightBeep()
{
    m_LBComFd = -1;   
    m_BMode = 0;
    m_CMode = 0;
    m_u64TimeB = 0;
    m_u64TimeE = 0;
    m_u64TimeH = 0;
    m_u32Count = 1;
    
    memset(m_szDisplay, 0, sizeof(m_szDisplay));
    memset(&m_evtimerDisplay, 0, sizeof(m_evtimerDisplay));
}

CLightBeep::~CLightBeep()
{
    Release();    
}

LTY_VOID CLightBeep::Release()
{
    m_LBComFd = -1;
    close(m_LBComFd);
    ETTimerStop(&m_evtimerDisplay);
}

LTY_RESULT LB_Msg_Handle(LTY_MSG_TYPE msg, LTY_U32 chan, LTY_U32 buf, LTY_U32 bufsize, LTY_LPVOID lpPriv)
{
    return ((CLightBeep*)lpPriv)->OnMessage(msg, chan, buf, bufsize);
}

LTY_RESULT CLightBeep::Init(struct ev_loop *loop)
{
    LTY_S32 s32Ret = LTY_SUCCESS;  
   
    ETInit(loop);
    
    m_LBComFd = open((const LTY_STRING)LIGHT_BEEP_COM, O_RDWR);
    if ( m_LBComFd <= 0 )
    {
        ERROUT("open %s Failed !\n", LIGHT_BEEP_COM);
        close(m_LBComFd);
        s32Ret = LTY_FAILURE;
    } 
    memcpy(m_szDisplay, "\x15\x15\x15\x15", 4); //"----"
    LBDisplay(m_szDisplay);
    
    ETTimerInit(&m_evtimerDisplay, DISPLAY_TIMER, this);
    
    ETTimerStart(&m_evtimerDisplay, DISPLAY_DURATION_MS, DISPLAY_DURATION_MS);

    LtyMessageRegister(LTY_MSG_CHG_LIGHT_BEEP_MODE, LB_Msg_Handle, 0, this);
    
    return s32Ret;
}

LTY_VOID CLightBeep::ETOnTimer(ev_timer_t *pTimer, LTY_U32 u32Type, LTY_LPVOID pPrivData)
{
    switch(u32Type)
    {
        case DISPLAY_TIMER :
            DisplayLoop();
            break;
        default :
            break;
    }
}

LTY_VOID CLightBeep::DisplayLoop()
{
    struct timeval tr;
   
    memset(&tr, 0, sizeof(tr));
    gettimeofday(&tr,NULL);
    if(m_CMode == 1)
    {
        if(m_BMode == 2)
            m_u64TimeH = tr.tv_sec*1000000+ tr.tv_usec;
        else
            m_u64TimeH = 0;
        m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
        m_u32Count = 1;
        m_CMode = 0;
    }
    m_u64TimeE = tr.tv_sec*1000000 + tr.tv_usec;
    switch(m_BMode)
    {
        case 0:
            if(((m_u64TimeE - m_u64TimeB) >= 1000000*3)&&(m_u32Count == 2))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count--;
            }
            if(((m_u64TimeE - m_u64TimeB) >= 1000000)&&(m_u32Count == 1))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            if(((m_u64TimeE - m_u64TimeH) >= 1000000*60)&&(m_u64TimeH != 0))
            {
                memcpy(m_szDisplay,"\x14\x14\x14\x14",4);
                LBDisplay(m_szDisplay);
            }
            //sleep(1);
            break;
        case 1: 			//短声
            if(m_u32Count == 1)
            {
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 100000)&&(m_u32Count == 2))
            {
                m_u32Count = 1;
                m_BMode = 0;
            }
            break;
        case 2: 			//长声
            if(m_u32Count == 1)
            {
                LBDisplay(m_szDisplay);
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 300000)&&(m_u32Count == 2))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 300000)&&(m_u32Count == 3))
            {
                m_u32Count = 1;
                m_BMode = 0;
            }
            break;
        case 3:
            if(m_u32Count == 1)
            {
                m_u32Count++;
            }
            //LBDisplay(m_szDisplay);
            else if(((m_u64TimeE - m_u64TimeB) >= 100000)&&(m_u32Count == 2))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 80000)&&(m_u32Count == 3))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 100000)&&(m_u32Count == 4))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 80000)&&(m_u32Count == 5))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 100000)&&(m_u32Count == 6))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count = 1;
                m_BMode = 0;
            }

            break;
        case 4:
            if(m_u32Count == 1)
            {
                m_u32Count++;
            }
            if(((m_u64TimeE - m_u64TimeB) >= 100000)&&(m_u32Count == 2))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            if(((m_u64TimeE - m_u64TimeB) >= 80000)&&(m_u32Count == 3))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            if(((m_u64TimeE - m_u64TimeB) >= 100000)&&(m_u32Count == 4))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count = 1;
                m_BMode = 0;
            }
            break;
        case 5: 			//长声
            if(m_u32Count == 1)
            {
                m_u32Count++;
            }
            //usleep(600000);
            if(((m_u64TimeE - m_u64TimeB) >= 60000)&&(m_u32Count == 2))
            {
                m_u32Count = 1;
                m_BMode = 0;
            }
            break;
        case 6: 		
            if(m_u32Count == 1)
            {
                m_u32Count++;
            }
            if(((m_u64TimeE - m_u64TimeB) >= 100000)&&(m_u32Count == 2))
            {
                m_u32Count = 1;
                m_BMode = 0;
            }
            break;
        case 10:
            if(m_u32Count == 1)
            {
                m_u32Count++;
            }
            //LBDisplay(m_szDisplay);
            else if(((m_u64TimeE - m_u64TimeB) >= 100000)&&(m_u32Count == 2))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 80000)&&(m_u32Count == 3))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 100000)&&(m_u32Count == 4))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 80000)&&(m_u32Count == 5))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 100000)&&(m_u32Count == 6))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 80000)&&(m_u32Count == 7))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 100000)&&(m_u32Count == 8))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
                //m_BMode = 0;
            }
            break;

        case 11:
            m_u32Count = 1;
            m_BMode = 0;
            break;
        case 12:	//初始化晶体管
            memcpy(m_szDisplay,"\x14\x14\x14\x14",4);
            LBDisplay(m_szDisplay);
            m_BMode = 0;
            break;
        case 13:
            if(m_u32Count == 1)
            {
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 150000)&&(m_u32Count == 2))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 80000)&&(m_u32Count == 3))
            {
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count++;
            }
            else if(((m_u64TimeE - m_u64TimeB) >= 150000)&&(m_u32Count == 4))
            {
                sleep(1);
                gettimeofday(&tr,NULL);
                m_u64TimeB = tr.tv_sec*1000000 + tr.tv_usec;
                m_u32Count = 1;
            }
            break;
        case 14:
            m_u32Count = 1;
            m_BMode = 0;
            break;
        default :
            break;
    }
    return;
}

LTY_RESULT CLightBeep::OnMessage(LTY_MSG_TYPE MsgType, LTY_U32 u32Chn, LTY_U32 u32ParaData, LTY_U32 u32ParaSize)
{
    LTY_S32 s32Ret = LTY_SUCCESS;  
    switch(MsgType) {
        case LTY_MSG_CHG_LIGHT_BEEP_MODE :
            LBChangeMode((LBDisplayInfo_t *)u32ParaData);
            break;
        default :
            break;
    }     
    
    return s32Ret;
}

LTY_RESULT CLightBeep::LBDisplay(LTY_U8 *pBuf)
{
    LTY_S32 s32Ret = LTY_SUCCESS;  

    s32Ret = write(m_LBComFd, pBuf, 4);
    return s32Ret;
}

LTY_VOID CLightBeep::LBChangeMode(LBDisplayInfo_t *stDisplayInfo)
{
    m_BMode = stDisplayInfo->u32Mode;
    m_CMode = 1;
    
    if ( stDisplayInfo->szBuf[0] != 0 )
    {
        memset(m_szDisplay, 0, sizeof(m_szDisplay));
        memcpy(m_szDisplay, stDisplayInfo->szBuf, 4);
    } else {
        memset(m_szDisplay, 0, sizeof(m_szDisplay));
        memcpy(m_szDisplay, "\x14\x14\x14\x14", 4);
    }
    
    // LBDisplay(m_szDisplay);
    return ;
}

