#ifndef LTY_MESSAGE_H
#define LTY_MESSAGE_H

#include <pthread.h>

#include "LtyTypeDef.h"
#include "LtyError.h"
#include "kfifo.h"

typedef LTY_U32 LTY_MSG_TYPE;

typedef LTY_RESULT (*PFN_MSG_HANDLER)(LTY_MSG_TYPE, LTY_U32, LTY_U32, LTY_U32, LTY_LPVOID);

#define MSG_TYPE_MAX 1024

typedef struct msg_hdl_s
{
    PFN_MSG_HANDLER  pfnMsgHandler;
    LTY_LPVOID       lpPriv;
    LTY_U32          u32Priority;
    LTY_MSG_TYPE     u32MsgType;

    #define MSG_HDL_FLAG_ACTIVE    1  //�Ѽ���
    #define MSG_HDL_FLAG_PERMANENT 2  //����ע�ᣬ����ж��
    LTY_U32          u32Flag;

    struct msg_hdl_s *prev;
    struct msg_hdl_s *next;
}msg_hdl_t;

typedef struct msg_data_s
{
    LTY_MSG_TYPE MsgType;
    LTY_U32      u32Chn;
    LTY_U32      u32ParaData;
    LTY_U32      u32ParaSize;
}msg_data_t;

typedef struct msg_queue_s
{
    struct kfifo    kfMsgQueue;
    LTY_U8          *pMsgData;
    LTY_U32         u32MsgMax;
    pthread_mutex_t lock;
}msg_queue_t;

extern LTY_RESULT LtyMessageInit(LTY_LPVOID loop);
extern LTY_VOID   LtyMessageRelease();

//����ע�ᣬ������ж��
extern LTY_RESULT LtyMessageRegister(LTY_MSG_TYPE MsgHdlType,
                                     PFN_MSG_HANDLER pfnMsgHandler,
                                     LTY_U32 u32Priority,    //��Ϣ�������ȼ���ֵԽ�����ȼ�Խ��
                                     LTY_LPVOID lpPriv);
extern LTY_RESULT LtyMessageSend(LTY_MSG_TYPE MsgType, LTY_U32 u32Chn, LTY_U32 u32ParaData, LTY_U32 u32ParaSize);

//��ʱע���ʼ��
extern LTY_RESULT LtyMessageHdlInit(msg_hdl_t *pMsgHdl);
//��ʱע�ᣬ����ж�أ�һ��ֻ��ע��һ����Ϣ���ͣ�������ʹ��LtyMessageHdlInit()��ʼ��msg_hdl_t���ݣ�������ģ�鹤���ڷ�ev_loop
//��ѭ���̣߳�����ʹ�ø�ϵ�нӿ�
extern LTY_RESULT LtyMessageRegisterEx(msg_hdl_t *pMsgHdl,
                                       LTY_MSG_TYPE MsgHdlType,
                                       PFN_MSG_HANDLER pfnMsgHandler,
                                       LTY_U32 u32Priority,
                                       LTY_LPVOID lpPriv);
extern LTY_VOID   LtyMessageUnregisterEx(msg_hdl_t *pMsgHdl);

//�����ev_loop��ѭ���̸߳�ev_loop��ѭ���̷߳�����Ϣ���øýӿڣ����ú���Ϣ������������Ϣ����������������
//��u32ParaSizeΪһ����0ֵ������Ϊu32ParaData��һ��ָ�룬Message�ײ㽫�´��ָ��ָ��ĵ�ַ����������������СΪu32ParaSize���������Ϊ1024�ֽ�
//��u32ParaSizeΪ0������Ϊu32ParaData��һ����ֵ
//�绺����������Ϣ�򱻶���������ENOBUFS
extern LTY_RESULT LtyMessagePost(LTY_MSG_TYPE MsgType, LTY_U32 u32Chn, LTY_U32 u32ParaData, LTY_U32 u32ParaSize);

extern LTY_RESULT LtyMessageQueueInit(msg_queue_t *pMsgQueue, LTY_U32 u32MsgQueueSize, LTY_U32 u32MsgSizeMax);
extern LTY_VOID   LtyMessageQueueRelease(msg_queue_t *pMsgQueue);
extern LTY_RESULT LtyMessageQueuePut(msg_queue_t *pMsgQueue, LTY_MSG_TYPE MsgType, LTY_U32 u32Chn, LTY_U32 u32ParaData, LTY_U32 u32ParaSize);
extern LTY_RESULT LtyMessageQueueGet(msg_queue_t *pMsgQueue, msg_data_t *pMsgData);
extern LTY_VOID   LtyMessageQueueClear(msg_queue_t *pMsgQueue);

#endif

