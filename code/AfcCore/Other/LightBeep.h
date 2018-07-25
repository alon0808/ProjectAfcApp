/// @file LightBeep.h
/// @brief 
/// @author Jesse Xu <zhimo811@gmail.com>
/// @version V1.0.0
/// @date 2017-11-07

#pragma once

#include "LtyCommon.h"
#include "LtyTimer.h"
#include "LtyError.h"
#include "LtyMsgUserInput.h"

class CLightBeep : private CEvTimer
{
    public:
        CLightBeep();
        virtual ~CLightBeep();

        LTY_RESULT Init(struct ev_loop *loop);
        LTY_VOID   Release();

        LTY_RESULT OnMessage(LTY_MSG_TYPE MsgType, LTY_U32 u32Chn, LTY_U32 u32ParaData, LTY_U32 u32ParaSize);

        LTY_VOID   LBChangeMode(LBDisplayInfo_t *stDisplayInfo); 
    private:
        LTY_RESULT LBDisplay(LTY_U8 *pBuf);
        LTY_VOID   ETOnTimer(ev_timer_t *pTimer, LTY_U32 u32Type, LTY_LPVOID pPrivData);
        LTY_VOID   DisplayLoop();
        
    private:
        LTY_S32     m_LBComFd;
        LTY_U32     m_BMode;
        LTY_U32     m_CMode;
        LTY_U8      m_szDisplay[4]; 
        LTY_U64     m_u64TimeB;
        LTY_U64     m_u64TimeE;
        LTY_U64     m_u64TimeH;
        LTY_U32     m_u32Count;
        ev_timer_t  m_evtimerDisplay;

};
