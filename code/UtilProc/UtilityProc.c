#include "Macro_Proj.h"
#include "TypeDef.h"
#include "UtilityProc.h"
#include <string.h>
//#include <stdio.h>


#ifndef xAssert
#ifdef xImplAssert
#define xAssert  xImplAssert
#else
#define xAssert(x) 
#endif
#endif


/*
@ resourse       [IN]: input data 1;
@ destination [INOUT]: IN,input data 2; OUT, xor result of data1 and data2;
*/
void XOR_Bytes(const TUINT8 *resourse, TUINT8 *destination, int length)
{
	if (resourse == NULL || destination == NULL || length < 0)
	{
		//printf("字节异或时,发生严重错误!!!");
		return ;
	}

	while (length > 0)
	{
		*destination ^= *resourse;
		destination++;
		resourse++;
		length--;
	}		
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
TUINT16 NumOf_NonZero_Bits(TUINT8 *pIn, TUINT8 in_len)
{
	TUINT8 i;
	TUINT16 retValue;

	if (pIn == NULL || in_len>63)
	{
		return 0xFFFF;
	}
	for (i=0,retValue=0; i<in_len; i++)
	{
		if ((pIn[i]&0x80) != 0x00)
		{
			retValue++;
		}
		if ((pIn[i]&0x40) != 0x00)
		{
			retValue++;
		}
		if ((pIn[i]&0x20) != 0x00)
		{
			retValue++;
		}
		if ((pIn[i]&0x10) != 0x00)
		{
			retValue++;
		}
		if ((pIn[i]&0x08) != 0x00)
		{
			retValue++;
		}
		if ((pIn[i]&0x04) != 0x00)
		{
			retValue++;
		}
		if ((pIn[i]&0x02) != 0x00)
		{
			retValue++;
		}
		if ((pIn[i]&0x01) != 0x00)
		{
			retValue++;
		}
	}

	return retValue;
}
// 
void memcpyE(TUINT8 *pDes, TUINT8 *pSrc, int len)
{
	int i=0;

	// 参数检查
	if (pDes==NULL || pSrc==NULL || len <= 0)
	{
		return;
	}
	if (pDes<=pSrc || (pSrc+len) < pDes)
	{	// copy from left to right
		for (i=0; i < len; ++i)
		{
			pDes[i] = pSrc[i];
		}
	}
	else
	{	// copy from right to left
		i = len - 1;	// len must not be 0
		while(i > 0)
		{
			pDes[i] = pSrc[i];
			--i;
		}	
		pDes[i] = pSrc[i];
	}
}
// 
int replaceE(TUINT8 *pSrc, char oldCh, char newCh, int len)
{
	int i = 0;
	int ret = 0;

	// 参数检查
	if (pSrc == NULL || len <= 0)
	{
		return ret;
	}

	for (i = 0; i < len; ++i)
	{
		if (pSrc[i] == oldCh) {
			pSrc[i] = newCh;
			++ret;
		}
	}
	return ret;
}





