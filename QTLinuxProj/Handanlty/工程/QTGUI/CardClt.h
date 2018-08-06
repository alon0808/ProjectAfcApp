#ifndef LTY_CARD_CLT_H
#define LTY_TTS_CLT_H

#include "LtyCommon.h"
#include "LtySocketClient.h"
//#include "LtyMsgTTS.h"
//#include "TTSProto.h"

#define TTSCLT_AUDIO_MAX 16

#define CARD_SOCKET_FILE "/var/run/CARD_SOCKET"

#include "CardSvr.h"

#if 0
typedef struct ttsclt_audio_s
{
    LTY_BOOL    bActive;
    LTY_U32     u32Tag;
    tts_audio_t stAdo;
}ttsclt_audio_t;
#endif
class CCardClt : private CSocketClient
{
public:
    CCardClt();
    virtual ~CCardClt();

    LTY_RESULT Init(struct ev_loop *loop,int _test_type);
    LTY_VOID   Release();

    LTY_RESULT OnMessage(LTY_MSG_TYPE MsgType, LTY_U32 u32Chn, LTY_U32 u32ParaData, LTY_U32 u32ParaSize);

private:
    LTY_VOID SCOnTimer();
    LTY_VOID SCOnError(LTY_RESULT ret);
    LTY_VOID SCOnConnect(LTY_RESULT ret);
    LTY_U32  SCOnRecv(LTY_U8 *pRcvData, LTY_U32 u32DataLen);

	void card_cmd_send_rsp(card_cmd_head_t *cmd_head);
	LTY_BOOL check_command_head(card_cmd_head_t *pcmd_head);
	void test_data();
#if 0
    LTY_RESULT OnMsgGetAudio(tts_audio_t* pstAudio);

    LTY_RESULT AudioGet(LTY_U32* pu32Idx);
    LTY_RESULT AudioSend(LTY_U32 u32Idx);
    LTY_RESULT AudioPush(LTY_U32 u32Idx, LTY_U32 u32Tag, LTY_LPVOID pData, LTY_U32 u32Len);
    LTY_RESULT AudioDone(LTY_U32 u32Idx, LTY_U32 u32Tag, LTY_BOOL bForce=LTY_FALSE);
#endif
    LTY_VOID CltClose();

    LTY_BOOL       m_bIsConn;

	char send_buf[1024];
	int m_test_type;
    //ttsclt_audio_t m_stAudio[TTSCLT_AUDIO_MAX];
};

#endif

