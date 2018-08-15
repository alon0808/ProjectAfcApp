#include "Macro_Proj.h"
#include "TypeDef.h"
#include "UtilityProc.h"
#include <string.h>
#include <time.h>
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



/***
*static time_t make_time_t(tb, ultflag) -
*
*功能:
*		转换一个tm_slzr结构的时间到time_t的时间值
*
*输入:
*       tm_slzr *tb - 需要转换的时间指针
*
*输出:
*		如果成功，则返回指定日历时间编码：time_t格式。
*		不成功，返回-1
*
*******************************************************************************/

/*++------------------------------------------------------------------------
Function:              localtimes Modification HistoryJurassic   KSLi
--------------------------------------------------------------------------*/
void localtimes(time_t timew, int timezonew, short *tm_timew)
{
	////////时间计算-----------------------------------------
	char Days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	unsigned int n32_Pass4year;
	int n32_hpery;    //计算时差  
	tm_slzr tt;
	int sll;
	short ii;
	timezonew = timezonew;
	sll = timew;
	sll += 28800;
	//	memcpy((unsigned char*)&tt, (unsigned char*)&timew, sizeof(tt));
	//	debugstring("jkl:");
	//	debugdata((unsigned char*)&timew, 4);
	//	debugdata((unsigned char*)&sll, 4);
	//	sll=sll-timezonew;
	//    time += 28800l;
	if (sll < 0)
	{
		sll = 0;
	}    //取秒时间  

		 //	debugdata((unsigned char*)&sll, 4);
	ii = (int)(sll % 60);    //tm_timew->tm_sec
	tt.tm_sec = ii;
	//	debugdata((unsigned char*)&tt.tm_sec, 2);
	sll /= 60;    //取分钟时间    
				  //	debugdata((unsigned char*)&sll, 4);
	ii = (int)(sll % 60);
	tt.tm_min = ii;
	//	debugdata((unsigned char*)&tt.tm_min, 4);
	sll /= 60;    //取过去多少个四年，每四年有 1461*24 小时    
				  //	debugdata((unsigned char*)&sll, 4);
				  //	n32_Pass4year=(unsigned int)(time / (1461L * 24L));    //计算年份    
	n32_Pass4year = sll / 35064l;//(1461L * 24L);    //计算年份
								 //	debugdata((unsigned char*)&n32_Pass4year, 4);
	ii = (int)(n32_Pass4year << 2) + 70;    //四年中剩下的小时数
	tt.tm_year = ii;
	//	debugdata((unsigned char*)&tt.tm_year, 4);    
	sll %= 35064;//1461L * 24L;    //校正闰年影响的年份，计算一年中剩下的小时数
				 //	debugdata((unsigned char*)&sll, 4);
	for (;;) {        //一年的小时数        
		n32_hpery = 8760;//365 * 24;        //判断闰年        
		if ((ii & 3) == 0) {            //是闰年，一年则多24小时，即一天            
			n32_hpery += 24;
		}
		if (sll < n32_hpery) {
			break;
		}
		// 	debugstring("t0~ ");
		ii++;
		sll -= n32_hpery;
	}    //小时数
	tt.tm_year = ii;
	//	debugdata((unsigned char*)&n32_hpery, 4);
	ii = (int)(sll % 24);    //一年中剩下的天数
	tt.tm_hour = ii;
	//	debugdata((unsigned char*)&tt.tm_hour, 4);
	sll /= 24;    //假定为闰年
	sll++;    //校正润年的误差，计算月份，日期
			  //	debugdata((unsigned char*)&sll, 4);
	ii = tt.tm_year;
	if ((ii & 3) == 0) {
		if (sll > 60) {
			sll--;
		}
		else {
			if (sll == 60)
			{
				tt.tm_mon = 1;
				tt.tm_mday = 29;
				return;
			}
		}
	}    //计算月日
	ii = tt.tm_mon;
	for (ii = 0; Days[ii] < sll; ii++)
	{
		//	debugstring("t1~ ");
		//	debugdata((unsigned char*)&tt.tm_mon, 1);
		//	debugdata((unsigned char*)&Days[tt.tm_mon], 1);
		//	debugdata((unsigned char*)&sll, 4);
		sll -= Days[ii];
		if ((sll == 0) || (ii > 11))
			break;
	}
	tt.tm_mon = ii;
	ii = (short)(sll);
	tt.tm_mday = ii;
	//	debugstring("tt:");
	//	debugdata((unsigned char*)&tt.tm_sec, sizeof(tt));
	memcpy((unsigned char*)&tm_timew[0], (unsigned char*)&tt.tm_sec, sizeof(tt));
	//	debugdata((unsigned char*)&tm_timew, sizeof(tt));
	return;
}


//输入7个字节的BCD码
void timewrite(unsigned char *bcdDateTime)
{
	int   year, mon, mday, hour, min, sec;

	time_t   t;
	struct     tm   nowtime;

	year = BCD2int(bcdDateTime, 2);
	mon = BCD2int(bcdDateTime + 2, 1);
	mday = BCD2int(bcdDateTime + 3, 1);
	hour = BCD2int(bcdDateTime + 4, 1);
	min = BCD2int(bcdDateTime + 5, 1);
	sec = BCD2int(bcdDateTime + 6, 1);

	//	printf("[%s]:%d-%d-%d %d:%d:%d\r\n", __FUNCTION__, year, mon, mday, hour, min, sec);

	nowtime.tm_sec = sec;   /*   Seconds.[0-60]   (1   leap   second)*/
	nowtime.tm_min = min;  /*   Minutes.[0-59]   */
	nowtime.tm_hour = hour; /*   Hours. [0-23]   */
	nowtime.tm_mday = mday; /*   Day.[1-31]   */
	nowtime.tm_mon = mon - 1; /*   Month. [0-11]   */
	nowtime.tm_year = year - 1900; /*   Year-   1900.*/
	nowtime.tm_isdst = -1;  /*   DST.[-1/0/1]*/

							//	printf("[%s]:%d-%d-%d %d:%d:%d\r\n", __FUNCTION__, year, mon, mday, hour, min, sec);

	t = mktime(&nowtime);
	stime(&t);

}

int get_datatime(char *odt)
{
	time_t timer;//time_t就是long int 类型
	struct tm *tblock;
	int pos = 0;

	timer = time(NULL);
	//	timer += (8 * 60 * 60);
	tblock = localtime(&timer);

	sprintf(odt + pos, "%04d", tblock->tm_year + 1900);
	pos += 4;
	sprintf(odt + pos, "%02d", tblock->tm_mon + 1);
	pos += 2;
	sprintf(odt + pos, "%02d", tblock->tm_mday);
	pos += 2;
	sprintf(odt + pos, "%02d", tblock->tm_hour);
	pos += 2;
	sprintf(odt + pos, "%02d", tblock->tm_min);
	pos += 2;
	sprintf(odt + pos, "%02d", tblock->tm_sec);
	pos += 2;
	odt[pos] = 0;
#ifdef _debugPSAM_
	printf("Local time is: %s\n", odt);
#endif
	return 0;
}

#define _DAY_SEC           (24L * 60L * 60L)    /* secs in a day */
#define _YEAR_SEC          (365L * _DAY_SEC)    /* secs in a year */
#define _FOUR_YEAR_SEC     (1461L * _DAY_SEC)   /* secs in a 4 year interval */
#define _DEC_SEC           315532800L           /* secs in 1970-1979 */
#define _BASE_YEAR         70L                  /* 1970 is the base year */
#define _BASE_DOW          4                    /* 01-01-70 was a Thursday */
#define _LEAP_YEAR_ADJUST  17L                  /* Leap years 1900 - 1970 */
#define _MAX_YEAR          138L                 /* 2038 is the max year */
/*
* ChkAdd evaluates to TRUE if dest = src1 + src2 has overflowed
*/
#define ChkAdd(dest, src1, src2)   ( ((src1 >= 0) && (src2 >= 0) \
    && (dest < 0)) || ((src1 < 0) && (src2 < 0) && (dest >= 0)) )

/*
* ChkMul evaluates to TRUE if dest = src1 * src2 has overflowed
*/
#define ChkMul(dest, src1, src2)   ( src1 ? (dest/src1 != src2) : 0 )


time_t make_time_t(tm_slzr *tb)
{
	////////时间计算-----------------------------------------
	char Days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	short i;
	int tmptm1, tmptm2, tmptm3;
	//        struct tm_slzr *tbtemp;

	/*
	* First, make sure tm_year is reasonably close to being in range.
	*/
	if (((tmptm1 = tb->tm_year) < _BASE_YEAR - 1) || (tmptm1 > _MAX_YEAR + 1))
		goto err_mktime;
	/*
	* Adjust month value so it is in the range 0 - 11.  This is because
	* we don't know how many days are in months 12, 13, 14, etc.
	*/
	if ((tb->tm_mon < 0) || (tb->tm_mon > 11)) {
		/*
		* no danger of overflow because the range check above.
		*/
		tmptm1 += (tb->tm_mon / 12);

		if ((tb->tm_mon %= 12) < 0) {
			tb->tm_mon += 12;
			tmptm1--;
		}

		/*
		* Make sure year count is still in range.
		*/
		if ((tmptm1 < _BASE_YEAR - 1) || (tmptm1 > _MAX_YEAR + 1))
			goto err_mktime;
	}

	/***** HERE: tmptm1 holds number of elapsed years *****/

	/*
	* Calculate days elapsed minus one, in the given year, to the given
	* month. Check for leap year and adjust if necessary.
	*/
	tmptm2 = Days[tb->tm_mon];
	if (tmptm2 == 28)
		tmptm2 += 3;
	else if (tmptm2 < 31)
		tmptm2 += 1;
	for (i = 0; i < tb->tm_mon; i++)
		tmptm2 += Days[i];
	if (!(tmptm1 & 3) && (tb->tm_mon > 1))
		//		if(tb->tm_mon > 1)
		tmptm2++;
	//debugstring("tmptm2:0:");
	//debugdata((unsigned char*)&tmptm2, 4);
	/*
	* Calculate elapsed days since base date (midnight, 1/1/70, UTC)
	*
	*
	* 365 days for each elapsed year since 1970, plus one more day for
	* each elapsed leap year. no danger of overflow because of the range
	* check (above) on tmptm1.
	*/
	tmptm3 = (tmptm1 - _BASE_YEAR) * 365 + ((tmptm1 - 1) >> 2)
		- _LEAP_YEAR_ADJUST;

	//debugstring("tmptm3:0:");
	//debugdata((unsigned char*)&tmptm3, 4);
	/*
	* elapsed days to current month (still no possible overflow)
	*/
	tmptm3 += tmptm2;

	//debugstring("tmptm3:1:");
	//debugdata((unsigned char*)&tmptm3, 4);
	/*
	* elapsed days to current date. overflow is now possible.
	*/
	tmptm1 = tmptm3 + (tmptm2 = (long)(tb->tm_mday));
	if (ChkAdd(tmptm1, tmptm3, tmptm2))
		goto err_mktime;

	tmptm1 -= 32;
	//debugstring("tmptm1:0:");
	//debugdata((unsigned char*)&tmptm1, 4);
	/***** HERE: tmptm1 holds number of elapsed days *****/

	/*
	* Calculate elapsed hours since base date
	*/
	tmptm2 = tmptm1 * 24;
	if (ChkMul(tmptm2, tmptm1, 24))
		goto err_mktime;

	//debugstring("tmptm2:1:");
	//debugdata((unsigned char*)&tmptm2, 4);
	tmptm1 = tmptm2 + (tmptm3 = (int)tb->tm_hour);
	if (ChkAdd(tmptm1, tmptm2, tmptm3))
		goto err_mktime;

	//debugstring("tmptm1:1:");
	//debugdata((unsigned char*)&tmptm1, 4);
	/***** HERE: tmptm1 holds number of elapsed hours *****/

	/*
	* Calculate elapsed minutes since base date
	*/

	tmptm2 = tmptm1 * 60;
	if (ChkMul(tmptm2, tmptm1, 60))
		goto err_mktime;

	//debugstring("tmptm2:3:");
	//debugdata((unsigned char*)&tmptm2, 4);
	tmptm1 = tmptm2 + (tmptm3 = (int)tb->tm_min);
	if (ChkAdd(tmptm1, tmptm2, tmptm3))
		goto err_mktime;

	/***** HERE: tmptm1 holds number of elapsed minutes *****/

	/*
	* Calculate elapsed seconds since base date
	*/

	tmptm2 = tmptm1 * 60;
	if (ChkMul(tmptm2, tmptm1, 60))
		goto err_mktime;

	//debugstring("tmptm2:4:");
	//debugdata((unsigned char*)&tmptm2, 4);
	tmptm1 = tmptm2 + (tmptm3 = (int)tb->tm_sec);
	if (ChkAdd(tmptm1, tmptm2, tmptm3))
		goto err_mktime;

	//debugstring("tmptm1:2:");
	//debugdata((unsigned char*)&tmptm1, 4);
	/***** HERE: tmptm1 holds number of elapsed seconds *****/

	//        if  ( ultflag ) {
	//
	//            /*
	//             * Adjust for timezone. No need to check for overflow since
	//             * localtime() will check its arg value
	//             */
	//
	//#ifdef _WIN32
	//            __tzset();
	//#else  /* _WIN32 */
	//#if defined (_M_MPPC) || defined (_M_M68K)
	//            _tzset();
	//#endif  /* defined (_M_MPPC) || defined (_M_M68K) */
	//#endif  /* _WIN32 */
	//
	//            tmptm1 += _timezone;
	//
	//            /*
	//             * Convert this second count back into a time block structure.
	//             * If localtime returns NULL, return an error.
	//             */
	//            if ( (tbtemp = localtime(&tmptm1)) == NULL )
	//                goto err_mktime;
	//
	//            /*
	//             * Now must compensate for DST. The ANSI rules are to use the
	//             * passed-in tm_isdst flag if it is non-negative. Otherwise,
	//             * compute if DST applies. Recall that tbtemp has the time without
	//             * DST compensation, but has set tm_isdst correctly.
	//             */
	//            if ( (tb->tm_isdst > 0) || ((tb->tm_isdst < 0) &&
	//              (tbtemp->tm_isdst > 0)) ) {
	//                tmptm1 += _dstbias;
	//                tbtemp = localtime(&tmptm1);    /* reconvert, can't get NULL */
	//            }
	//
	//        }
	//        else {
	//            if ( (tbtemp = gmtime(&tmptm1)) == NULL )
	//                goto err_mktime;
	//        }


	//        *tb = *tbtemp;
	tmptm1 -= 28800;	//8  timezone
	return (time_t)tmptm1;

err_mktime:
	// 		debugstring("Error,..........\r\n");
	return (time_t)(-1);
}

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

