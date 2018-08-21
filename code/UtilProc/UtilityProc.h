#ifndef _INCLUDED_UTILITY_PROC_H
#define _INCLUDED_UTILITY_PROC_H

#include "TypeDef.h"
#include "ConvertData.h"
#include "cJSON.h"


#ifndef BitMaskMatch
#define BitMaskMatch(Value, Mask) (((Value) &(Mask)) == (Mask))
#define HighByte(x)	((TUINT8)(((x) >>4) & 0x0F))
#define LowByte(x)	((TUINT8)((x) & 0x0F))
#define MakeU16(high, low) ((unsigned short)(((high) <<8) |(low)))

#define ArItemCount(Items) (sizeof(Items)/sizeof(Items[0]))

#endif

typedef struct
{
	TUINT32 tag;
	int len;
	TUINT8 *pValue;
	TUINT8 *pTlvStart;
}StrTlv;

typedef struct {
    unsigned int tag;
    unsigned int len;
    unsigned char *pValue;
    unsigned char *pBegin;
}stTlv;

#ifdef __cplusplus
extern "C"
{
#endif


    extern TUINT16 NumOf_NonZero_Bits(void *pIn, TUINT8 in_len);

    extern void XOR_Bytes(const void *resourse, void *destination, int length);

    extern void memcpyE(void *pDes, void *pSrc, int len);

    extern int replaceE(void *pSrc, char oldCh, char newCh, int len);

	//////////////////////////////////////////////////////////////////////////


	int T4L4VToBytes(StrTlv *pTlvs, int tlvCount, TUINT8 *pDes, int *pDesLen);
	/*
	* @Description get the length of string
	*
	* @Param TU8 *pSrc-resource of byte buffer
	*
	* @Return TI32-
	*/
	int PareseAllT4L4V(TUINT8 *pSrc, int srcLen, StrTlv *pTlvs, int *pTlvCount);

	StrTlv * SearchTlv(StrTlv *pTlvs, int tlvCount, TUINT32 tag);

	int PareseAllTlvs(TUINT8 *pSrc, int srcLen, StrTlv *pTlvs, int *pTlvCount);
	int PareseAllTls(TUINT8 *pSrc, int srcLen, StrTlv *pTlvs, int *pTlvCount);

    /////////////////////////////////////////////////////////////////////////////////////////////////

    extern unsigned char getSignXor(unsigned char *pSrc, int len);
    extern int findT1L2VFirst(void *pSrc, int len, TUINT32 tag, stTlv *pTlvs);
    extern int PareseT1L2V(void *pSrc, int len, stTlv *pTlvs, int *pOLen);
    extern int PareseT1L2V_8583(void *pSrc, int len, stTlv pTlvs[65]);
    extern int BuildT1L2V(stTlv *pTlvs, int len, unsigned char *pSrc, int *pOLen);
    extern int SetTLV(stTlv *pTlv, TUINT32 tag, int len, void *pSrc);

/////////////////////////////////
	// time process

	//����ʱ���������λΪ�롣ǰ��ʱ�������ʱ�䡣
	extern int DataTimeDec(unsigned char *datatime1, unsigned char *datatime2);
	extern time_t BCDTime2Long(unsigned char*timee);
	//ʱ��Ӻ���
	//����BCDʱ�䣺bcdTime   Ҫ�Ӻ��룺addSec
	//���BCDʱ�䣺outBCDTime
	extern void TimeAdd(unsigned char*outBCDTime, unsigned char*bcdTime, unsigned int addSec);

	/********************************************
	* ���ܣ����ʱ���Ƿ���Ч
	* ���룺time-ʱ������ָ�� 7�ֽ� BCD��
	* �����0-��Ч   1-��Ч
	****************************************************/
	extern unsigned char time_invalid(unsigned char *time_buff);
	/********************************************
	* ���ܣ����ʱ���Ƿ���Ч�ַ�
	* ���룺time-ʱ������ָ��  14�ֽ� ASC��
	* �����0-��Ч   1-��Ч
	****************************************************/
	extern unsigned char time_invalid_str(unsigned char *time_buff);

	extern int get_datatime(char *odt);

	extern void timewrite(unsigned char *bcdDateTime);

	////////////////////////////////////

#ifdef __cplusplus
};
#endif

#endif
