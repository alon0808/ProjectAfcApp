#include "Macro_Proj.h"
#include "TypeDef.h"
#include "ConvertData.h"
#include <string.h>
#include "UtilityProc.h"
//#include <stdio.h>


#ifndef xAssert
#ifdef xImplAssert
#define xAssert  xImplAssert
#else
#define xAssert(x) 
#endif
#endif


TINT32 BytesToVersion(TUINT8 *pSrc, TINT32 srcLen, TUINT8 *pDes, TINT32 *pDesLen)
{
	TINT32 tmpI = Ret_OK;
	TINT32 lenLimit = *pDesLen;
	TINT32 i;


	if (pSrc == NULL || pDes == NULL || pDesLen == NULL)
	{
		return Ret_Err_Param;
	}
	lenLimit = *pDesLen;
	tmpI = BytesToChars(pSrc, srcLen, pDes + 1, lenLimit);
	if (tmpI < 0)
	{
		return Ret_Error;
	}
	// add 'v' at start pos
	pDes[0] = 'v';
	++tmpI;
	for (i = 1; i < (tmpI); )
	{
		if (pDes[i] == 0x30)
		{	// move to left
			--tmpI;
			memcpyE(pDes + i, pDes + (i + 1), tmpI - i);
		}
		else
		{
			++i;
		}
		if ((tmpI + 1) > lenLimit)
		{
			return Ret_Err_Overflow;
		}
		++i;
		// move to right
		memcpyE(pDes + (i + 1), pDes + (i), tmpI - i);
		++tmpI;

		pDes[i] = '.';
		++i;
	}
	--tmpI;
	pDes[tmpI] = '\0';
	*pDesLen = tmpI;

	return Ret_OK;
}
/*******************************************************************/
/*
//将n格式的BYTE数组变成一个长整型,将十进制的转为对应的整型,输入值大于FFFFFFFF的全部变为FFFFFFFF;
*/
/*******************************************************************/
TUINT32 nFormat_to_DWORD(TUINT8 *pIn, TUINT8 in_len)
{
	TUINT32	TempValue = 0;
	TUINT32	RetValue = 0;
	TUINT8 temp = 0;
	TUINT8 i = 0;


	for (i = 0; i < in_len; i++)
	{
		temp = (TUINT8)(pIn[i] >> 4);
		temp *= 10;
		temp = (TUINT8)(temp + (pIn[i] & 0X0F));	//n=>b, 把当前字节变为对应的10进制，如OX14===>0X0E

		TempValue = RetValue * 100 + temp;
		if (TempValue < RetValue)				//保证输入值大于FFFFFFFF的全部变为FFFFFFFF;
		{
			RetValue = 0xFFFFFFFF;
			break;
		}

		RetValue = TempValue;


	}

	return TempValue;
}
//
TUINT32 Binary_To_DWORD(TUINT8 *pIn, TUINT8 inLen)
{
	TUINT32 temp = 0;
	TUINT8 i = 0;

	if (inLen > 4)
		inLen = 4;
	for (i = 0; i < inLen; i++)
	{
		temp |= (((TUINT32)pIn[inLen - i - 1]) << (TUINT32)(i << 3));
	}

	return temp;
}

TUINT32 Int32ToBcd(TUINT32 nUnpredictNumber, TUINT32 nUN)
{
	TUINT32 i;
	TUINT32 nWanted = 0;

	for (i = 0; i < nUN; i++)
	{
		nWanted += (nUnpredictNumber % 10) << (i << 2);
		nUnpredictNumber /= 10;
	}

	return nWanted;
}

// 字节数组转字符串
TINT32 CharsToBytes(TUINT8 *pSrc, TINT32 srcLen, TUINT8 *pDes, TINT32 desLenLimit)
{
	TUINT16 i;
	TUINT8 tmpBy;
	TINT32 count = 0;
	TINT32 retValue = 0;

	if (pSrc == NULL || pDes == NULL)
	{
		return -1;
	}
	if ((desLenLimit << 1) < srcLen)
	{	// buffer is overflow
		return -2;
	}

	for (i = 0; i < srcLen; ++i)
	{
		tmpBy = pSrc[i];
		if (tmpBy >= '0' && tmpBy <= '9')
		{
			tmpBy = tmpBy - '0';
		}
		else if (tmpBy >= 'A' && tmpBy <= 'F')
		{
			tmpBy = tmpBy - 'A' + 10;
		}
		else if (tmpBy >= 'a' && tmpBy <= 'f')
		{
			tmpBy = tmpBy - 'a' + 10;
		}
		else
		{
			return -3;
		}

		if ((count & 0x01) != 0x00)
		{
			pDes[retValue] |= tmpBy;
			++retValue;
		}
		else
		{
			pDes[retValue] = (tmpBy << 4);
		}
		count ^= 0x01;
	}
	if ((count & 0X01) != 0x00)
	{
		++retValue;
	}

	return retValue;
}

/*******************************************************************/
/*
*/
/*******************************************************************/
void DWORD_To_Binary(TUINT32 inData, TUINT8* pOut)
{
	pOut[0] = (TUINT8)((inData >> 24) & 0x000000FF);
	pOut[1] = (TUINT8)((inData >> 16) & 0x000000FF);
	pOut[2] = (TUINT8)((inData >> 8) & 0x000000FF);
	pOut[3] = (TUINT8)(inData & 0x000000FF);

}
/*******************************************************************/
/*
//TUINT32 to 'n' Format array
*/
/*******************************************************************/
void DWORD_To_nFormat(TUINT32 inData, TUINT8* pOut, TUINT8 pOutLen)
{
	int	i = 0;
	TUINT32	p = 1;
	TUINT32	temp = 0;



	temp = inData;
	for (i = pOutLen - 1; i >= 0; i--)
	{
		pOut[i] = (TUINT8)(temp % 10);
		pOut[i] |= (TUINT8)((temp % 100) / 10 << 4);
		if (p == 100000000)
			break;
		p *= 100;
		temp = inData / p;
		if (temp == 0)
			break;
	}

	//n格式，左边消减，左边补0
	/*for(i=pOutLen-1; i>=0; i--)
	{
	//pOut[i] = (UINT8)((inData % (p*10)) / p);
	//p *= 10;
	//pOut[i] |= (UINT8)(((inData % (p*10)) / p) << 4);
	//p *= 10;


	temp = (inData % (p*100)) / p;
	pOut[i] = (UINT8)(temp % 10);
	pOut[i] |= (UINT8)((temp/10) << 4);
	p *= 100;

	if(inData/p == 0 || )
	break;
	}*/

	if (i > 0)
		memset(pOut, 0, i);

}


TINT32 BytesToChars(const void *pvSrc, TINT32 nSrcLen, void *pvDest, TINT32 nDestLen)
{
	TINT32 i;
	TINT32 nPos;
	TINT32 nLow;
	TINT32 nHigh;
	TINT32 retValue = nSrcLen << 1;
	/*@owned@*/
	static const char *p = "0123456789ABCDEF";
	const TUINT8 *pSrc = (TUINT8 *)pvSrc;
	TUINT8 *pDest = (TUINT8 *)pvDest;

	xAssert(pSrc != NULL);
	xAssert(nSrcLen >= 0);

	xAssert(pDest != NULL);
	xAssert(nDestLen > 0);

	xAssert(retValue + 1 <= nDestLen);

	if (nDestLen < 1)
	{
		return -1;
	}
	pDest[0] = '\0';

	if (nSrcLen == 0)
	{
		return 0;
	}

	if (retValue + 1 > nDestLen)
	{
		return -1;
	}

	for (i = 0; i < nSrcLen; i++)
	{
		nPos = i << 1;

		nHigh = HighByte(pSrc[i]);
		nLow = LowByte(pSrc[i]);

		pDest[nPos] = (TUINT8)p[nHigh];
		pDest[nPos + 1] = (TUINT8)p[nLow];
	}

	pDest[retValue] = 0x00;

	return (retValue);
}

//amount is 12 chars 00 00 00 00 00 10
void CurrencyConv(TINT8 *pChars, TUINT16 *pnCharsLen)
{
	TINT8 tempChar[14] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	TUINT16 tempLen = *pnCharsLen;
	TUINT16 i = 0;
	TUINT16 len = 0;


	*pnCharsLen = 0;
	for (i = 0; i < tempLen; ++i)
	{
		if (pChars[i] == '0' && len == 0)
		{
			continue;
		}
		tempChar[len++] = pChars[i];
	}
	if (len == 0)
	{
		memcpy(pChars, "0.00", 4);
		//pChars[3] = tempChar[0];
		len = 4;
	}
	else if (len == 1)
	{
		memcpy(pChars, "0.0", 3);
		pChars[3] = tempChar[0];
		len = 4;
	}
	else if (len == 2)
	{
		memcpy(pChars, "0.", 2);
		memcpy(pChars + 2, tempChar, 2);
		len = 4;
	}
	else
	{
		//memcpy(tempChar+(len-1), tempChar + (len -2), 2);
		tempChar[len] = tempChar[len - 1];
		tempChar[len - 1] = tempChar[len - 2];
		tempChar[len - 2] = '.';
		++len;
		memcpy(pChars, tempChar, len);
	}
	pChars[len] = '\0';
	*pnCharsLen = len;

	return;
}


void Int32To4Byte(TINT32 n, TUINT8 buf[])
{
	buf[0] = (TUINT8)((n >> 24) & 0xFF);
	buf[1] = (TUINT8)((n >> 16) & 0xFF);
	buf[2] = (TUINT8)((n >> 8) & 0xFF);
	buf[3] = (TUINT8)(n & 0xFF);
}
// 4 bytes to int32
TINT32 x4ByteToInt32(const TUINT8 buf[])
{
	TINT32 n;

	n = (buf[0] << 24)
		| (buf[1] << 16)
		| (buf[2] << 8)
		| buf[3];

	return n;
}

void xInt32ToBCD6(TINT32 n, TUINT8 buf[])
{
	TINT32 nCurPos = 5;
	TINT32 nTemp;
	TINT32 nHigh;
	TINT32 nLow;

	xAssert(buf != NULL);

	memset(buf, 0x00, 6);

	while (n > 0)
	{
		nTemp = n % 100;

		nHigh = nTemp / 10;
		nLow = nTemp % 10;

		buf[nCurPos--] = (TUINT8)((nHigh << 4) | nLow);

		n /= 100;
	}
}

TINT32 xInt32ToBCD2(TINT32 b)
{
	TINT32 nHigh;
	TINT32 nLow;

	if (b >= 100 || b < 0)
	{
		return -1;
	}

	nHigh = b / 10;
	nLow = b % 10;

	return ((nHigh << 4) | nLow);
}

TINT32 xBCD2ToInt32(TINT32 n)
{
	TINT32 nHigh;
	TINT32 nLow;

	nHigh = HighByte(n);
	nLow = LowByte(n);

	if (nHigh < 0 || nHigh >9)
	{
		return -1;
	}

	if (nLow < 0 || nLow >9)
	{
		return -2;
	}

	return (nHigh * 10 + nLow);
}

/*TINT32 xBCDToString( const UINT8 *pBCD, TINT32 nBCDLen, UINT8 *pDest, TINT32 *pnDestLen )
{
	TINT32 i;
	TINT32 nCurLen =0;
	UINT8 ch;

	xAssert(pBCD !=NULL);
	xAssert(nBCDLen >0);

	xAssert(pDest !=NULL);

	xAssert(pnDestLen !=NULL);
	xAssert(*pnDestLen >=nBCDLen *2 +1);

	if(*pnDestLen <1)
	{
		return -1;
	}
	pDest[0] ='\0';

	if(nBCDLen <0)
	{
		return -1;
	}

	if(*pnDestLen <nBCDLen *2 +1)
	{
		return -1;
	}

	for(i =0; i <nBCDLen; i++)
	{
		ch =HighByte(pBCD[i]);

		if(ch <=9)//ch >=0 &&
		{
			pDest[nCurLen++] =(UINT8)('0' +ch);
		}
		else if(ch ==0x0F)
		{
			break;
		}
		else
		{
			return -1;
		}

		ch =LowByte(pBCD[i]);

		if(ch <=9)//ch >=0 &&
		{
			pDest[nCurLen++] =(UINT8)('0' +ch);
		}
		else if(ch ==0x0F)
		{
			break;
		}
		else
		{
			return -1;
		}
	}

	pDest[nCurLen] ='\0';

	*pnDestLen =nCurLen;

	return nCurLen;
}*/

TINT32 xBinToHexStrSpace(const TUINT8 *pSrc, TINT32 nSrcLen, TUINT8 *pDest, TINT32 nDestLen)
{
	TINT32 i;

	TINT32 nPos;

	TINT32 nLow;
	TINT32 nHigh;
	static const char *p = "0123456789ABCDEF";

	xAssert(pSrc != NULL);
	xAssert(nSrcLen >= 0);

	xAssert(pDest != NULL);
	xAssert(nDestLen > 0);

	xAssert(nSrcLen * 3 <= nDestLen);

	if (nDestLen < 1)
	{
		return -1;
	}
	pDest[0] = (TUINT8)'\0';

	if (nSrcLen == 0)
	{
		return 0;
	}

	if (nSrcLen * 3 > nDestLen)
	{
		return -1;
	}

	for (i = 0; i < nSrcLen; i++)
	{
		nPos = 3 * i;

		nHigh = (pSrc[i] >> 4) & 0x0F;
		nLow = pSrc[i] & 0x0F;

		pDest[nPos] = (TUINT8)p[nHigh];
		pDest[nPos + 1] = (TUINT8)p[nLow];
		pDest[nPos + 2] = (TUINT8)' ';
	}

	pDest[i * 3 - 1] = (TUINT8)'\0';
	return (i * 3 - 1);
}

// 
//void memcpyE(UINT8 *pDes, UINT8 *pSrc, TUINT32 len)
//{
//	TUINT32 i=0;
//
//	// check parameter
//	if (pDes==NULL || pSrc==NULL || len == 0)
//	{
//		return;
//	}
//	if (pDes<=pSrc || (pSrc+len) < pDes)
//	{	// copy from left to right
//		for (i=0; i < len; ++i)
//		{
//			pDes[i] = pSrc[i];
//		}
//	}
//	else
//	{	// copy from right to left
//		i = len - 1;	// len must not be 0
//		while(i > 0)
//		{
//			pDes[i] = pSrc[i];
//			--i;
//		}	
//		pDes[i] = pSrc[i];
//	}
//}
//BOOL XPPPIsPayPassAidByCmp(const UINT8 *pAdfName)
//{
//	const UINT8 bufPPRID[]="\xA0\x00\x00\x00\x04";
//	
//	xAssert(pAdfName !=NULL);
//	
//	if(memcmp(pAdfName, bufPPRID, sizeof(bufPPRID) -1) ==0)
//	{
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}
//
//BOOL XPPPIsPayPassAid(void)
//{
//	const TXPDataObject *pdoAID;
//	
//	pdoAID =XPDolGetItem(XP_TAG_AID_ICC);
//	xAssert(pdoAID !=NULL);
//	xAssert(pdoAID->Value !=NULL);
//	xAssert(pdoAID->Length >=XP_TAG_LEN_AID_MIN);
//	
//	return XPPPIsPayPassAidByCmp(pdoAID->Value);
//}

void RevertTurn(TINT32 len, void *pData) {
	int tmpI, i;
	TUINT8 tmpU8;
	TUINT8 *pSrc = (TUINT8*)pData;

	tmpI = len >> 1;
	--len;
	for (i = 0; i < tmpI; ++i)
	{
		tmpU8 = pSrc[i];
		pSrc[i] = pSrc[len - i];
		pSrc[len - i] = tmpU8;
	}

	//return Ret_OK;
}

//字符串转整型数 '9'>=strASC>='0'
unsigned int Ascii2INT(unsigned char *strASC, unsigned char len)
{
	unsigned char i;
	unsigned int t;
	t = 0;
	for (i = 0; i < len; i++) {
		if (strASC[i] == ' ')
			continue;//如果是空格，忽略
		t *= 10;
		t += (strASC[i] - '0');
	}
	return t;
}
///////////////////////////////////////////////////
//
//  ASCII to BCD Code
//
///////////////////////////////////////////////////
void Ascii2BCD(const void* pDataASC, const void* pDataBCD, unsigned int lenASC)
{
	unsigned int i, t;
	unsigned char temp;
	unsigned char *strASC = (unsigned char *)pDataASC;
	unsigned char *strBCD = (unsigned char *)pDataBCD;

	t = 0;
	for (i = 0; i < lenASC;)
	{
		if ((strASC[i] >= '0') && (strASC[i] <= '9'))
			temp = strASC[i] - '0';
		else if ((strASC[i] >= 'a') && (strASC[i] < 'g'))
			temp = strASC[i] - 'a' + 10;
		else if ((strASC[i] >= 'A') && (strASC[i] < 'G'))
			temp = strASC[i] - 'A' + 10;
		else
			temp = strASC[i] - '0';

		strBCD[t] = temp << 4;
		if ((strASC[i + 1] >= '0') && (strASC[i + 1] <= '9'))
			temp = strASC[i + 1] - '0';
		else if ((strASC[i + 1] >= 'a') && (strASC[i + 1] < 'g'))
			temp = strASC[i + 1] - 'a' + 10;
		else if ((strASC[i + 1] >= 'A') && (strASC[i + 1] < 'G'))
			temp = strASC[i + 1] - 'A' + 10;
		else
			temp = strASC[i] - '0';
		strBCD[t] |= temp;
		i += 2;
		t++;
	}

	//	printf("[%s]::t=%d\r\n", __FUNCTION__, t);
}
////////////////////////////////////
//
// BCD to ASCII Code
//
////////////////////////////////////
void BCD2Ascii(unsigned char *bcd, unsigned char *Ascii, unsigned int len)
{
	unsigned int i;
	unsigned char k;
	for (i = 0; i < len; i++) {
		k = ((bcd[i] >> 4) & 0x0f);
		if (k < 10)
			Ascii[2 * i] = k + '0';
		else
			Ascii[2 * i] = k + 'A' - 10;
		k = bcd[i] & 0x0f;
		if (k < 10)
			Ascii[2 * i + 1] = k + '0';
		else
			Ascii[2 * i + 1] = k + 'A' - 10;
	}
}

unsigned char Hex2BCD(unsigned char hex)
{
	char buf[5];
	unsigned char bcd;
	sprintf(buf, "%02d", hex);
	Ascii2BCD((unsigned char*)buf, (unsigned char*)&bcd, 2);
	return bcd;
}


////////////////////////////////////
// BCD to HEX
// 高位在低位
////////////////////////////////////
unsigned int BCD2HEX(unsigned char *bcd, unsigned char len)
{
	unsigned char i;
	unsigned char temp, temp1;
	unsigned int ulvalue;
	ulvalue = 0;

	for (i = 0; i < len; i++) {
		temp = bcd[i] >> 4;
		temp1 = bcd[i] & 0x0f;
		ulvalue = ulvalue * 10 + temp;
		ulvalue = ulvalue * 10 + temp1;
	}
	return ulvalue;
}

//单字节BCD码相加,返回相加后的BCD码
unsigned char bcdADD(unsigned char bcd1, unsigned char bcd2)
{
	unsigned char b1, b2;
	b1 = ((bcd1 >> 4) * 10 + (bcd1 & 0x0f));
	b2 = ((bcd2 >> 4) * 10 + (bcd2 & 0x0f));
	b1 += b2;
	b2 = b1 / 10;
	b2 <<= 4;
	b2 |= (b1 % 10);
	return b2;
}
//单字节BCD码相减,返回BCD码
unsigned char bcdDEC(unsigned char bcd1, unsigned char bcd2)
{
	unsigned char b1, b2;
	b1 = ((bcd1 >> 4) * 10 + (bcd1 & 0x0f));
	b2 = ((bcd2 >> 4) * 10 + (bcd2 & 0x0f));
	b1 -= b2;
	b2 = b1 / 10;
	b2 <<= 4;
	b2 |= (b1 % 10);
	return b2;
}


unsigned int str2int(unsigned char *pstr, unsigned char len)
{
	unsigned int ii;
	unsigned char index = 0;

	ii = 0;
	while (index < len) {
		ii *= 10;
		ii += (pstr[index] - '0');
		index++;
	}
	return ii;
}

unsigned int BCD2int(unsigned char *pstr, unsigned char len)
{
	unsigned int ii;
	unsigned char index = 0;

	ii = 0;
	while (index < len) {
		ii *= 10;
		ii += ((pstr[index] >> 4) & 0x0f);
		ii *= 10;
		ii += (pstr[index] & 0x0f);
		index++;
	}
	return ii;
}

/************************************
*主要用于调备号压缩保存
*4字节存贮。
*每字节大于等于0xA0（也就是此字节值大于等于160）时，此字节减去0xA0后的ASCII码字符。（可以表示所有字符，而字母必须采用此方法）
*每字节小于0xA0，则是以BCD码表示的数字。（只能表示0-9）
*2015.1.10
**************************************/
//字符转4字节：
//strin-传入8字节设备号，不够8位前面补0
//ZBbuf-传出DVlen字节的压缩后的设备号
//ZBlen-需要传出的长度,不能大于16字节
//返回：最小的有效字节，不包括前面的00，小于ZBlen正常，其它：说明长度不对，ZBlen字节保存不了这么多数
unsigned char ASCII2ZBcod(char *strin, unsigned char *ZBbuf, unsigned char ZBlen)
{
	unsigned char bcdNo[16];
	unsigned char i, pos, index;


	for (index = 0; index < 8; index++) {//找非0
		if (strin[index] != '0')
			break;
	}

	memset(bcdNo, 0, 16);
	memset(ZBbuf, 0, ZBlen);
	pos = 16;
	for (i = index; i < 8; i++) {
		if (pos > 0)
			pos--;
		else {	//16字节已经用完了,肯定错了
			return 0xFF;
		}
		if ((strin[7 + index - i] >= '0') && (strin[7 + index - i] <= '9')) {//
			if ((i < 7) && ((strin[7 + index - i - 1] >= '0') && (strin[7 + index - i - 1] <= '9'))) {//前面还有字符,判断是不是数据，是可以合成，不是只能单独处理
				bcdNo[pos] = ((strin[7 + index - i - 1] << 4) | (strin[7 + index - i] & 0x0F));
				i++;
			}
			else if (i >= 7) {//=7 前面没有字符了，高位为0就行了。
				bcdNo[pos] = strin[7 + index - i] & 0x0F;
			}
			else {
				bcdNo[pos] = strin[7 + index - i] + 0xA0;//一个数字，但是前面是字母，没有合成
			}
		}
		else if ((strin[7 + index - i] >= 'A') && (strin[7 + index - i] <= 'Z'))
			bcdNo[pos] = strin[7 + index - i] + 0xA0;
		else//((strin[7-i] >= 'a') && (strin[7-i] <= 'z'))	//其它的都当小写字母处理
			bcdNo[pos] = strin[7 + index - i] - 'a' + 'A' + 0xA0;
	}

	memcpy(ZBbuf, bcdNo + 16 - ZBlen, ZBlen);	//这里全部处理后返回需要长度的字节数。
	return (16 - pos);	//返回字符的真实长度
}
//字母BCD码转字符：
//ZBbuf-传入ZBlen字节的压缩后的设备号
//strin-传出8字节设备号，不够8位前面补0
//ZBlen-需要传出的长度,不能大于16字节
//返回：8正常，其它：说明长度不对，传入号超过了8位字符
unsigned char ZBcod2ASCII(unsigned char *ZBbuf, unsigned char ZBlen, char *strin)
{
	unsigned char ss[32];
	unsigned char i, pos;

	pos = 0;
	for (i = 0; i < ZBlen; i++) {
		if (ZBbuf[i] >= 0xA0) {//字母
			ss[pos++] = ZBbuf[i] - 0xA0;
		}
		else {//BCD
			ss[pos++] = ((ZBbuf[i] >> 4) & 0x0F) + '0';
			ss[pos++] = (ZBbuf[i] & 0x0F) + '0';
		}
	}

	if (pos >= 8) {
		memcpy(strin, ss, 8);
	}
	else {//if(pos<8){//没有8位，前面补‘0’
		memcpy(strin + (8 - pos), ss, pos);
		memset(strin, '0', (8 - pos));
	}
	return pos;
}

