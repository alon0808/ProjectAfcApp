#ifndef _INCLUDED_CONVERT_DATA_H
#define _INCLUDED_CONVERT_DATA_H

#include "TypeDef.h"


#ifndef BitMaskMatch
#define BitMaskMatch(Value, Mask) (((Value) &(Mask)) == (Mask))
#define HighByte(x)	((TUINT8)(((x) >>4) & 0x0F))
#define LowByte(x)	((TUINT8)((x) & 0x0F))
#define MakeU16(high, low) ((unsigned short)(((high) <<8) |(low)))

#define ArItemCount(Items) (sizeof(Items)/sizeof(Items[0]))


#endif
//typedef struct 
//{
//	TUINT32 tag;
//	TUINT16 len;
//	UINT8 * pValue;
//	TUINT16 maxSize;
//	UINT8 isPresent;
//}TLV_PTR;
#define Int32ToBytes(buffer, num)	buffer[0] = (num>>24)&0x0FF; \
									buffer[1] = (num>>16)&0x0FF; \
									buffer[2] = (num>>8)&0x0FF; \
									buffer[3] = (num)&0x0FF; 
#define BytesToInt32(buffer)	((buffer[0]<<24)|(buffer[1]<<16)|(pTmpTlv->pValue[2]<<8)|(pTmpTlv->pValue[3]))

#ifdef __cplusplus
extern "C"
{
#endif

	extern TUINT32 Binary_To_DWORD(TUINT8 *pIn, TUINT8 inLen);

	extern TUINT32 Int32ToBcd(TUINT32 nUnpredictNumber, TUINT32 nUN);

	extern TINT32 CharsToBytes(TUINT8 *pSrc, TINT32 srcLen, TUINT8 *pDes, TINT32 desLenLimit);


	extern void DWORD_To_Binary(TUINT32 inData, TUINT8* pOut);

	extern TUINT32 nFormat_to_DWORD(TUINT8 *pIn, TUINT8 in_len);
	//extern TINT32 SMReadFile(TINT8 *fileName, UINT8 *fileBuffer, TUINT16 *length);

	extern TINT32 BytesToChars(const void *pvSrc, TINT32 nSrcLen, void *pvDest, TINT32 nDestLen);

	//extern TINT32 xBinToHexStr(const UINT8 *pSrc, TINT32 nSrcLen, UINT8 *pDest, TINT32 nDestLen);
	extern TINT32 BytesToVersion(TUINT8 *pSrc, TINT32 srcLen, TUINT8 *pDes, TINT32 *pDesLen);

	extern void CurrencyConv(TINT8 *pChars, TUINT16 *pnCharsLen);

	extern void RevertTurn(TUINT8 *pSrc, TINT32 len);

#ifdef __cplusplus
};
#endif


#endif



