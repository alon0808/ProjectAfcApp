#ifndef __LTY_COMMON_H__
#define __LTY_COMMON_H__

#include "LtyTypeDef.h"
#include "LtyError.h"
#include "LtyLog.h"
#include "LtyMessage.h"
#include "ltycommonfunction.h"

#define MKFLAG(a, b, c, d) (a | b<<8 | c<<16 | d<<24)

#define VERSION(a, b, c) ((a<<24) | ((b&0xff)<<16) | (c&0xffff))
#define VER_MAJ(a)  (a>>24)
#define VER_MIN(a)  ((a>>16)&0xff)
#define VER_RLS(a)  (a&0xffff)

#define PACKED(size) __attribute__((packed,aligned(size)))

#define RES_1080P_WIDTH    1920
#define RES_1080P_HEIGHT   1080
#define RES_720P_WIDTH     1280
#define RES_720P_HEIGHT    720
#define PAL_D1_WIDTH   704
#define PAL_D1_HEIGHT  576
#define PAL_HD_WIDTH   PAL_D1_WIDTH
#define PAL_HD_HEIGHT  288
#define PAL_CIF_WIDTH  352
#define PAL_CIF_HEIGHT PAL_HD_HEIGHT
#define MAX_PATH_LEN    128
/*һ��Ӳ�������ķ�������*/
#define MAX_HDDPARTITION_NUM    5
/*Ӳ���豸�ļ�������󳤶�*/
#define MAX_HDDDEVNAME_LEN  80
#define APP_VER_OF_DISPLAY  LTY_VERSION
#define G726_RESERVE_FLAG  0xf8
#define PCM_FLAG           0x8f
#define CTRL_CREATE_DLG   0 //�Ի��򴴽�
#define CTRL_CREATE_WIN   1 //ֱ�Ӵ��ڴ���
#define TIME_BASE 946684800
#define HI3536_SENDBLOCK  (-1)
#define REMOTE_EXPORT_REC_PATH   "/ltyapp/"
#ifndef MAX_URL_LEN
#define MAX_URL	128
#endif
#define NOD_SD "/dev/mmcblk0p13"
#define PATH_MOUNT_SD "/mnt/sdisk"

#define NOD_ADVERT "/dev/mmcblk0p16"
#define PATH_MOUNT_ADVERT "/mnt/message"



extern LTY_U32 LtyVerifyCrc32(LTY_U32 u32Base, LTY_LPVOID pData, LTY_U32 u32Len);
extern LTY_U32 LtyVerifyCrc32_2(LTY_U32 u32Base, LTY_LPVOID pData, LTY_U32 u32Len);
extern LTY_U8  LtyVerifyXOr(LTY_U8 u8Base, LTY_LPVOID pData, LTY_U32 u32Len);
extern LTY_U16 LtyVerifyCrc16(LTY_U16 u16Base, LTY_LPVOID pData, LTY_U32 u32Len);
extern LTY_U8  LtyVerifySum(LTY_U8 u8Base, LTY_LPVOID pData, LTY_U32 u32Len);

extern LTY_U32 LtyStrSpliteAttr(LTY_STRING str, const LTY_STRING flag, LTY_CHAR splite, LTY_STRING attr[], LTY_U32 cnt);
extern LTY_U8  LtyStrAsc2Hex(LTY_U8 u8Asc);

extern void HEXOUT(void *pData, unsigned int len);

unsigned short checksum(unsigned short *buffer, int size);
TBOOL validate_checksum(unsigned short *buffer, int size);

#define MAX_CHANNEL_NUM             16
#define MAX_IPC_NUM             9
#define MAX_ANALOG_NUM              8
#define MAX_FRAME_LEN               (500*1024)

typedef void *(*ThreadEntryPtrType)(void *);

#if defined(OUTPUT_DEBUG_INFO)
    #define DEBUG_INFO(info) do{printf info;}while(0)
#else
    #define DEBUG_INFO(info) do{}while(0)
#endif
typedef enum _mainthreadstatebitmap_{/*ͨ�õ�״̬λ*/
    /*��MTSB_FIRST��MTSB_LAST֮������״̬λ��������״̬λ���ںϷ����ж�*/
    MTSB_FIRST = 0,/*ע��:NSB_FIRST��ʾ��һ��״̬λ�����������ںϷ��ԱȽ�*/
    MTSB_NEW_SECOND,
    MTSB_NEWSEC_FORGUI,
    /*Ӳ�̸�ʽ��*/
    MTSB_FORMATHDD_DOING,/*���ڽ��и�ʽ��*/
    MTSB_FORMATHDD_SUCC,/*��ʽ���ɹ�*/
    MTSB_FORMATHDD_FAIL, /*��ʽ��ʧ��*/
    MTSB_FORMATHDD_NOHDD, /*�Ҳ���Ӳ��*/
    MTSB_FORMATHDD_NETRELAY, /*�������ڻط�*/
    /*�������*/
    MTSB_SAVEPARA_DOING,
    MTSB_SAVEPARA_SUCC,
    /*����¼���ļ�*/
    MTSB_SEARCHRECFILE_DOING,
    MTSB_SEARCHRECFILE_SUCC,
    /*�ط�¼���ļ�*/
    MTSB_PLAYRECFILE_DOING,
    /*ͨ��U���������*/
    MTSB_UPGRADESW_DOING,
    MTSB_UPGRADESW_SUCC,
    MTSB_UPGRD_FAIL, /*����ʧ��*/
    /*�޸�ϵͳʱ��*/
    MTSB_MODIFYSC_DOING,
    MTSB_MODIFYSC_SUCC,
    /*����¼���ļ�*/
    MTSB_BACKUPREC_DOING,
    MTSB_BACKUPREC_SUCC,
    MTSB_BACKUPREC_CANCEL,
    /*Ӳ��û���㹻�Ŀռ�*/
    MTSB_NOTENOUGH_HDDSPACE,
    /*û��¼��*/
    MTSB_NO_REC,
    /*���ݱ�ȡ��*/
    MTSB_BAK_CANCELED,
    /*ϵͳ������*/
    MTSB_RESTART_MACHINE,
    /*ϵͳ������*/
    MTSB_START_OK,
    /*�����޸��˲���*/
    MTSB_NETWORK_MODIFYPARA,
    /*��ʽ��U��*/
    MTSB_FORMATUSBDISK_DOING,
    /*�µ�һ�죬����¼��ֶΣ�һ��¼���ļ������Կ���*/
    MTSB_NEWDAY_FORREC,
    MTSB_IN_USERINTERFACE,/*�û�������*/
    MTSB_LAST/*ע��:MTSB_LAST��ʾ���һ��״̬λ�����������ںϷ��ԱȽ�*/
}mainthreadstatebitmap_t;
int CreateNormalThread(ThreadEntryPtrType entry, void *para, pthread_t *pid);
void mSleep(unsigned int  MilliSecond);
ssize_t safe_write(int fd, const void *buf, size_t count);
ssize_t safe_read(int fd, void *buf, size_t count);
int copy_file(const char * source,const char * dest,int flags);
typedef enum _osderrorcode_{/*OSD�������ô������*/
    OEC_SUCCESS = 0,
    OEC_INVALIDPARA,
    OEC_INVALIDSTRUCT,
    OEC_HARDWAREERROR,
    OEC_NOTSUPPORTFONT,
    OEC_OUTOFMEMORY,
    OEC_UNINIT,
    OEC_FILEERROR,
    OEC_ERROR,
}osderrorcode_t;
/*���������±�*/
#define ARRAY_INDEX(index, BitNum) ((index) / (BitNum))
/*����BITλ*/
#define BIT_INDEX(index, BitNum) ((index) % (BitNum))

#define CHK_BIT_NUM(addr, index, BitNum, type) \
        (*(addr + ARRAY_INDEX(index, BitNum)) >> BIT_INDEX(index, BitNum)) & ((type)0x01)
#define CHK_BIT_LONG(addr, index) (CHK_BIT_NUM(addr, index, 32, unsigned long))

#define VIDEO_OUT_DEV          2
#define VIDEO_OUT_LAYER        3

typedef struct ltyMSG_CONTEXT_S{
    char msgtype;
    LTY_U32 u32context;
}MSG_CONTEXT_S;
struct _MSGQUEUE
{
    MSG_CONTEXT_S* msg;                   /* post message buffer */
    int len;                    /* buffer len */
    int readpos, writepos;      /* positions for reading and writing */
    pthread_mutex_t lock;       // lock
};
typedef struct _MSGQUEUE MSGQUEUE;
typedef MSGQUEUE* PMSGQUEUE;

typedef enum lty_LANGUAGE_E{
	LANGUAGE_CH = 0,
	LANGUAGE_EN,
	LANGUAGE_TAI,
}LANGUAGE_E;

TBOOL GetQueueMessage(PMSGQUEUE msg_que, MSG_CONTEXT_S* pmsg);
TBOOL PutQueueMessage (PMSGQUEUE msg_que, MSG_CONTEXT_S* pmsg);
void Queue_Deinit(PMSGQUEUE pMsgQueue);
TBOOL Queue_Init (PMSGQUEUE pMsgQueue, int iBufferLen);

LTY_RESULT LtyFsScanDir(LTY_STRING pFullPath, LTY_U32 u32FileType,
                        int (*cb)(LTY_STRING pFileName, LTY_U32 u32Type, LTY_LPVOID pCbArg),
                        LTY_LPVOID pPriv);
LTY_VOID CloseAllFd(void);
video_resolution_e ResTranslate(int width, int height);

#define HI3536_AODEV 0
LTY_RESULT LtySysKillProc(LTY_STRING szProcName);

/*
����
    ��һ��U32���͵İ汾��תΪ�ַ���
����
    u32Ver: �汾��
    szVer:  �ַ�����ַ���ɵ�����׼���ڴ�ռ�
    u32Len: �ַ����ռ䳤�ȣ�����16�ֽڣ�(255.255.65535)
����ֵ
    ���szVerΪNULL��u32LenΪ0���򷵻غ����ڲ���һ����̬�ַ�����ַ�����򷵻�szVer
*/
LTY_STRING LtySysVerStr(LTY_U32 u32Ver, LTY_STRING szVer, LTY_U32 u32Len);

void LtyTouchDisable();
void LtyTouchEnable();
#define NETLINK_NONE  -1
#define NETLINK_ETH1  0
#define NETLINK_PPP1  1
#define NETLINK_PPP0  2
#define NETLINK_WIFI  3


LTY_U32 LtyGetStrmServIp();
LTY_U32 LtyGetItsServIp();
int LtyUpdateDomainServIp();

/*
��������/ж��
*/
LTY_RESULT ModInsert(const char* pszMod, const char* option);
LTY_RESULT ModUnload(const LTY_STRING pszModName);
const char *moderror(int err);

void getVideoBitrate(int level, int *bitrate);
#define UDISK_MOUNTDIR "/mnt/udisk"
#define USB_PARTNOD  "/dev/diskUSB1"
#define BEIJING_AREA 8

#define DEVDEFAULTINFO   "/mnt/app/devdefaultinfo"
typedef struct {      
   char IsFist;
   char IsFistConnet;
   unsigned long long FistTime;
   unsigned long long FistConnetcTime;
   int year;
   int mon;
   unsigned long long TXPreTolCnt;
   unsigned long long TXTolCnt;
   unsigned long long RXPreTolCnt;
   unsigned long long RXTolCnt;
}SETTIMEINFO;

typedef struct {      
   unsigned long long TXPreTolCnt;
   unsigned long long TXTolCnt;
   unsigned long long RXPreTolCnt;
   unsigned long long RXTolCnt;
}SZFLOWCNT;
#endif



