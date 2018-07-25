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

    #define MSG_HDL_FLAG_ACTIVE    1  //已激活
    #define MSG_HDL_FLAG_PERMANENT 2  //永久注册，不会卸载
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

//永久注册，不允许卸载
extern LTY_RESULT LtyMessageRegister(LTY_MSG_TYPE MsgHdlType,
                                     PFN_MSG_HANDLER pfnMsgHandler,
                                     LTY_U32 u32Priority,    //消息处理优先级，值越大，优先级越高
                                     LTY_LPVOID lpPriv);
extern LTY_RESULT LtyMessageSend(LTY_MSG_TYPE MsgType, LTY_U32 u32Chn, LTY_U32 u32ParaData, LTY_U32 u32ParaSize);

//临时注册初始化
extern LTY_RESULT LtyMessageHdlInit(msg_hdl_t *pMsgHdl);
//临时注册，允许卸载，一次只能注册一种消息类型，必须先使用LtyMessageHdlInit()初始化msg_hdl_t内容，如您的模块工作在非ev_loop
//主循环线程，谨慎使用该系列接口
extern LTY_RESULT LtyMessageRegisterEx(msg_hdl_t *pMsgHdl,
                                       LTY_MSG_TYPE MsgHdlType,
                                       PFN_MSG_HANDLER pfnMsgHandler,
                                       LTY_U32 u32Priority,
                                       LTY_LPVOID lpPriv);
extern LTY_VOID   LtyMessageUnregisterEx(msg_hdl_t *pMsgHdl);

//建议非ev_loop主循环线程给ev_loop主循环线程发送消息调用该接口，调用后消息参数将放入消息缓冲区并立即返回
//如u32ParaSize为一个非0值，则认为u32ParaData是一个指针，Message底层将会拷贝指针指向的地址数据至缓冲区，大小为u32ParaSize，最大限制为1024字节
//如u32ParaSize为0，则认为u32ParaData是一个数值
//如缓冲区满，消息则被丢弃，返回ENOBUFS
extern LTY_RESULT LtyMessagePost(LTY_MSG_TYPE MsgType, LTY_U32 u32Chn, LTY_U32 u32ParaData, LTY_U32 u32ParaSize);

extern LTY_RESULT LtyMessageQueueInit(msg_queue_t *pMsgQueue, LTY_U32 u32MsgQueueSize, LTY_U32 u32MsgSizeMax);
extern LTY_VOID   LtyMessageQueueRelease(msg_queue_t *pMsgQueue);
extern LTY_RESULT LtyMessageQueuePut(msg_queue_t *pMsgQueue, LTY_MSG_TYPE MsgType, LTY_U32 u32Chn, LTY_U32 u32ParaData, LTY_U32 u32ParaSize);
extern LTY_RESULT LtyMessageQueueGet(msg_queue_t *pMsgQueue, msg_data_t *pMsgData);
extern LTY_VOID   LtyMessageQueueClear(msg_queue_t *pMsgQueue);

#endif

