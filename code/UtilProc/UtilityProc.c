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
void XOR_Bytes(const void *resourse, void *destination, int length)
{
    TUINT8 *prHead = (TUINT8 *)resourse;
    TUINT8 *pdHead = (TUINT8 *)destination;

    if (prHead == NULL || pdHead == NULL || length < 0)
	{
		//printf("字节异或时,发生严重错误!!!");
		return ;
	}

	while (length > 0)
	{
        *pdHead ^= *prHead;
        pdHead++;
        prHead++;
		length--;
	}		
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
TUINT16 NumOf_NonZero_Bits(void *pIn, TUINT8 in_len)
{
	TUINT8 i;
	TUINT16 retValue;
    TUINT8 *piHead = (TUINT8 *)pIn;

    if (piHead == NULL || in_len>63)
	{
		return 0xFFFF;
	}
	for (i=0,retValue=0; i<in_len; i++)
	{
        if ((piHead[i]&0x80) != 0x00)
		{
			retValue++;
		}
        if ((piHead[i]&0x40) != 0x00)
		{
			retValue++;
		}
        if ((piHead[i]&0x20) != 0x00)
		{
			retValue++;
		}
        if ((piHead[i]&0x10) != 0x00)
		{
			retValue++;
		}
        if ((piHead[i]&0x08) != 0x00)
		{
			retValue++;
		}
        if ((piHead[i]&0x04) != 0x00)
		{
			retValue++;
		}
        if ((piHead[i]&0x02) != 0x00)
		{
			retValue++;
		}
        if ((piHead[i]&0x01) != 0x00)
		{
			retValue++;
		}
	}

	return retValue;
}
// 
void memcpyE(void *pDes, void *pSrc, int len)
{
	int i=0;
    TUINT8 *pdHead = (TUINT8 *)pDes;
    TUINT8 *psHead = (TUINT8 *)pSrc;

	// 参数检查
    if (pdHead==NULL || psHead==NULL || len <= 0)
	{
		return;
	}
    if (pdHead<=psHead || (psHead+len) < pdHead)
	{	// copy from left to right
		for (i=0; i < len; ++i)
		{
            pdHead[i] = psHead[i];
		}
	}
	else
	{	// copy from right to left
		i = len - 1;	// len must not be 0
		while(i > 0)
		{
            pdHead[i] = psHead[i];
			--i;
		}	
        pdHead[i] = psHead[i];
	}
}
// 
int replaceE(void *pSrc, char oldCh, char newCh, int len)
{
	int i = 0;
	int ret = 0;
    TUINT8 *pHead = (TUINT8 *)pSrc;

	// 参数检查
    if (pHead == NULL || len <= 0)
	{
		return ret;
	}

	for (i = 0; i < len; ++i)
	{
        if (pHead[i] == oldCh) {
            pHead[i] = newCh;
			++ret;
		}
	}
	return ret;
}





