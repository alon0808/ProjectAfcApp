
#ifndef __PROC_CHARACTER_HEAD_
#define __PROC_CHARACTER_HEAD_



#define TEXT_LINE	"**********************************************************\n"
#define TEXT_LINE1	"==========================================================\n"
#define TEXT_LINE2	"----------------------------------------------------------\n"

#ifndef NULL
#define NULL	(void *)0
#endif

#ifndef SET_INT16
#define SET_INT16(buf, len)	(buf)[0] = ((len) >> 8) & 0x0FF;\
							(buf)[1] = ((len)) & 0x0FF
#define GET_INT16(buf) ((buf)[0]<<8)|((buf)[1]&0x0FF)
//
#define SET_INT32(buf, len)	(buf)[0] = ((len) >> 24) & 0x0FF;\
							(buf)[1] = ((len) >> 16) & 0x0FF;\
							(buf)[2] = ((len) >> 8) & 0x0FF;\
							(buf)[3] = ((len) ) & 0x0FF
#define GET_INT32(buf) ((buf)[0]<<24)|((buf)[1]<<16|(buf)[2]<<8)|((buf)[3]&0x0FF)

#endif

#define ALIGN_4K_CUR(addr)	(addr)&0xFFFFF000
#define ALIGN_4K_NEXT(addr)	((addr)+0x0FFF)&0xFFFFF000
#define ALIGN_2_NEXT(addr)	((addr)+1)&0xFFFFFFFE

typedef struct {
	unsigned int tag;
	unsigned int len;
	unsigned char *pValue;
	unsigned char *pBegin;
}stTlv;


extern unsigned char getSignXor(unsigned char *pSrc, int len);
extern int findT1L2VFirst(void *pSrc, int len, unsigned int tag, stTlv *pTlvs);
extern int PareseT1L2V(void *pSrc, int len, stTlv *pTlvs, int *pOLen);
extern int PareseT1L2V_8583(void *pSrc, int len, stTlv pTlvs[65]);
extern int BuildT1L2V(stTlv *pTlvs, int len, unsigned char *pSrc, int *pOLen);
extern int SetTLV(stTlv *pTlv, int tag, int len, void *pSrc);

#endif

