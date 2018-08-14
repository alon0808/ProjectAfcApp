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


unsigned char getSignXor(unsigned char *pSrc, int len) {
	unsigned char xorCh = 0;
	int i;

	for (i = 0; i < len; i++)		//kid 到xr 值之前的校验
	{
		xorCh ^= pSrc[i];
	}

	return xorCh;
}
///////////////////////////////////////////////////////////////////////

void Long2BCDTime(int Ltime, unsigned char*BCDtime)
{
	//	struct tm_slzr *area;
	short area[20];
	unsigned char buff[16];
	unsigned char pos;

	//	outport( AUXCON,0x0082);
	//	debugstring("L0~ ");
	//	debugdata((unsigned char*)&Ltime, 4);
	pos = 0;
	//	area = localtime(&Ltime);
	localtimes(Ltime, 0, area);
	//	debugstring("L1~ ");
	//	sprintf(buff + pos, "%04d", area ->tm_year + 1900); pos += 4;
	//	sprintf(buff + pos, "%02d", area->tm_mon + 1); pos += 2;
	//	sprintf(buff + pos, "%02d", area->tm_mday); pos += 2;
	//	sprintf(buff + pos, "%02d", area->tm_hour); pos += 2;
	//	sprintf(buff + pos, "%02d", area->tm_min); pos += 2;
	//	sprintf(buff + pos, "%02d", area->tm_sec); pos += 2;

	sprintf((char*)buff + pos, "%04d", area[5] + 1900); pos += 4;
	sprintf((char*)buff + pos, "%02d", area[4] + 1); pos += 2;
	sprintf((char*)buff + pos, "%02d", area[3]); pos += 2;
	sprintf((char*)buff + pos, "%02d", area[2]); pos += 2;
	sprintf((char*)buff + pos, "%02d", area[1]); pos += 2;
	sprintf((char*)buff + pos, "%02d", area[0]); pos += 2;
	//	debugstring("L2~ ");
	Ascii2BCD(buff, BCDtime, 14);

	//	debugstring("LT~ ");

	return;
}
time_t BCDTime2Long(unsigned char*timee)
{
	time_t t_scr;
	tm_slzr time_check;
	unsigned short uiTemp;

	t_scr = 0l;

	uiTemp = (unsigned short)BCD2HEX(timee, 2);
	time_check.tm_year = uiTemp - 1900;
	uiTemp = (unsigned short)BCD2HEX(timee + 2, 1);
	time_check.tm_mon = uiTemp - 1;
	uiTemp = (unsigned short)BCD2HEX(timee + 3, 1);
	time_check.tm_mday = uiTemp;
	uiTemp = (unsigned short)BCD2HEX(timee + 4, 1);
	time_check.tm_hour = uiTemp;
	uiTemp = (unsigned short)BCD2HEX(timee + 5, 1);
	time_check.tm_min = uiTemp;
	uiTemp = (unsigned short)BCD2HEX(timee + 6, 1);
	time_check.tm_sec = uiTemp;
	time_check.tm_isdst = 0;//-1;
	time_check.tm_wday = 0;
	time_check.tm_yday = 0;
	//	t_scr = mktime(&time_check);
	t_scr = make_time_t(&time_check);

	return t_scr;
}
//时间加函数
//输入BCD时间：bcdTime   要加和秒：addSec
//输出BCD时间：outBCDTime
void TimeAdd(unsigned char*outBCDTime, unsigned char*bcdTime, unsigned int addSec)
{
	time_t ttemp;
	ttemp = BCDTime2Long(bcdTime);
	ttemp += addSec;
	Long2BCDTime(ttemp, outBCDTime);
}

//两个时间相减，单位为秒。前面时间减后面时间。
int DataTimeDec(unsigned char *datatime1, unsigned char *datatime2)
{
	int l_time1, l_time2;

	l_time1 = BCDTime2Long(datatime1);
	l_time2 = BCDTime2Long(datatime2);

	l_time1 -= l_time2;

	return l_time1;
}



unsigned char time_invalid(unsigned char *time_buff)
{
	unsigned int year;
	unsigned char i;

	for (i = 0; i < 7; i++) {
		if (((time_buff[i] & 0x0f) > 0x09) || (((time_buff[i] >> 4) & 0x0f) > 0x09))
			return 0;
	}

	if (time_buff[0] != 0x20)
	{
		if ((time_buff[0] != 0x19) || (time_buff[1] != 0x99))
			return 0;
	}

	if ((!time_buff[2]) || (time_buff[2] > 0x12))
		return 0;

	switch (time_buff[2])
	{
	case 0x01:
	case 0x03:
	case 0x05:
	case 0x07:
	case 0x08:
	case 0x10:
	case 0x12:
		if ((!time_buff[3]) || (time_buff[3] > 0x31))
			return 0;
		break;
	case 0x04:
	case 0x06:
	case 0x09:
	case 0x11:
		if ((!time_buff[3]) || (time_buff[3] > 0x30))
			return 0;
		break;
	case 0x02:
		year = (unsigned int)(time_buff[0] >> 4) * 1000 + (time_buff[0] & 0x0f) * 100
			+ (time_buff[1] >> 4) * 10 + (time_buff[1] & 0x0f);

		if (year % 4)
		{
			if ((!time_buff[3]) || (time_buff[3] > 0x28))
				return 0;
			break;
		}
		else
		{
			if ((!time_buff[3]) || (time_buff[3] > 0x29))
				return 0;
			break;
		}
	}

	if (time_buff[4] > 0x23)
		return 0;
	if (time_buff[5] > 0x59)
		return 0;
	if (time_buff[6] > 0x59)
		return 0;
	return 1;
}




