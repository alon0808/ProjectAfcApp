


//兼容原有PSAM函数。IC卡，及其它 函数

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/time.h>

#include "inputmisc/PsamCard.h"

#include "inputmisc/GPIOCtrl.h"


#include "LightColor.h"
#include "GPRS.h"

#include "DemoMain.h"

#include "inputmisc/IcCardCtrlApi.h"

#include "SlzrTypeDef.h"
#include "MYDES.h"

//#define _debugPSAM_



void debugstring(const char *str)
{
	printf("%s", str);
}

void debugdata(unsigned char *value, unsigned int uclen, unsigned char mode)
{
	unsigned int i;
	for(i = 0;i < uclen; i++)
	{
		printf("%02X", value[i]);
	}

	if((mode&0x01) == 1)
		printf("\r\n");
}

void over_turn(unsigned char length,unsigned char *ptr)
{
	unsigned char i;
	unsigned char temp[16];
	//memcpy(temp, ptr, length);
	memcpy(temp, ptr, length);
	
	for(i=0;i<length;i++)
	{
		ptr[i]=temp[length-i-1];
	}
}


//字符串转整型数 '9'>=strASC>='0'
unsigned int Ascii2INT(unsigned char *strASC, unsigned char len)
{
	unsigned char i;
	unsigned int t;
	t=0;
	for(i=0;i<len;i++){
		if(strASC[i] == ' ')
			continue;//如果是空格，忽略
		t *= 10;
		t += (strASC[i]-'0');
	}
	return t;
}
///////////////////////////////////////////////////
//
//  ASCII to BCD Code
//
///////////////////////////////////////////////////
void Ascii2BCD(unsigned char *strASC,unsigned char *strBCD, unsigned int lenASC)
{
	unsigned int i,t;
	unsigned char temp;

	t=0;
	for(i=0;i<lenASC;)
	{
		if ((strASC[i] >= '0') && (strASC[i] <= '9'))
			temp = strASC[i] - '0';
		else if ((strASC[i] >= 'a') && (strASC[i] < 'g'))
			temp = strASC[i] - 'a' + 10;
		else if ((strASC[i] >= 'A') && (strASC[i] < 'G'))
			temp = strASC[i] - 'A' + 10;
		else
			temp = strASC[i] - '0';

		strBCD[t] = temp<<4;
		if ((strASC[i+1] >= '0') && (strASC[i+1] <= '9'))
			temp = strASC[i+1] - '0';
		else if ((strASC[i+1] >= 'a') && (strASC[i+1] < 'g'))
			temp = strASC[i+1] - 'a' + 10;
		else if ((strASC[i+1] >= 'A') && (strASC[i+1] < 'G'))
			temp = strASC[i+1] - 'A' + 10;
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
void BCD2Ascii( unsigned char *bcd, unsigned char *Ascii, unsigned int len )
{
	unsigned int i;
	unsigned char k;
	for( i=0 ; i<len ; i++ ){
		k = ((bcd[i]>>4)&0x0f);
		if(k < 10)
			Ascii[2*i] = k + '0';
		else
			Ascii[2*i] = k + 'A' - 10;
		k = bcd[i]&0x0f;
		if(k <10)
			Ascii[2*i+1] = k + '0';
		else
			Ascii[2*i+1] = k + 'A' - 10;
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
	unsigned char temp,temp1;
	unsigned int ulvalue;
	ulvalue = 0;
	
	for(i=0; i<len; i++){
		temp = bcd[i]>>4;
		temp1 = bcd[i]&0x0f;
		ulvalue = ulvalue * 10 + temp; 
		ulvalue = ulvalue * 10 + temp1;
	}
	return ulvalue;
}

//单字节BCD码相加,返回相加后的BCD码
unsigned char bcdADD(unsigned char bcd1, unsigned char bcd2)
{
	unsigned char b1,b2;
	b1 = ((bcd1>>4)*10+(bcd1&0x0f));
	b2 = ((bcd2>>4)*10+(bcd2&0x0f));
	b1 += b2;
	b2 = b1/10;
	b2 <<= 4;
	b2 |= (b1%10);
	return b2;
}
//单字节BCD码相减,返回BCD码
unsigned char bcdDEC(unsigned char bcd1, unsigned char bcd2)
{
	unsigned char b1,b2;
	b1 = ((bcd1>>4)*10+(bcd1&0x0f));
	b2 = ((bcd2>>4)*10+(bcd2&0x0f));
	b1 -= b2;
	b2 = b1/10;
	b2 <<= 4;
	b2 |= (b1%10);
	return b2;
}


unsigned int str2int(unsigned char *pstr, unsigned char len)
{
	unsigned int ii;
	unsigned char index=0;
	
	ii = 0;
	while(index<len){
		ii *= 10;
		ii += (pstr[index]-'0');
		index++;
	}
	return ii;
}

unsigned int BCD2int(unsigned char *pstr, unsigned char len)
{
	unsigned int ii;
	unsigned char index=0;
	
	ii = 0;
	while(index<len){
		ii *= 10;
		ii += ((pstr[index]>>4)&0x0f);
		ii *= 10;
		ii += (pstr[index]&0x0f);
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
	
	
	for(index=0; index<8; index++){//找非0
		if(strin[index] != '0')
			break;
	}
	
	memset(bcdNo, 0, 16);
	memset(ZBbuf, 0, ZBlen);
	pos = 16;
	for(i=index; i<8; i++){
		if(pos > 0)
			pos--;
		else{	//16字节已经用完了,肯定错了
			return 0xFF;
		}
		if((strin[7+index-i] >= '0') && (strin[7+index-i] <= '9')){//
			if((i<7) && ((strin[7+index-i-1] >= '0') && (strin[7+index-i-1] <= '9'))){//前面还有字符,判断是不是数据，是可以合成，不是只能单独处理
				bcdNo[pos] = ((strin[7+index-i-1]<<4)|(strin[7+index-i]&0x0F));
				i++;
			}
			else if(i >= 7){//=7 前面没有字符了，高位为0就行了。
				bcdNo[pos] = strin[7+index-i]&0x0F;
			}
			else{
				bcdNo[pos] = strin[7+index-i] +0xA0;//一个数字，但是前面是字母，没有合成
			}
		}
		else if((strin[7+index-i] >= 'A') && (strin[7+index-i] <= 'Z'))
			bcdNo[pos] = strin[7+index-i] +0xA0;
		else//((strin[7-i] >= 'a') && (strin[7-i] <= 'z'))	//其它的都当小写字母处理
			bcdNo[pos] = strin[7+index-i]-'a'+'A' +0xA0;
	}
	
	memcpy(ZBbuf, bcdNo+16-ZBlen, ZBlen);	//这里全部处理后返回需要长度的字节数。
	return (16-pos);	//返回字符的真实长度
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
	for(i=0; i<ZBlen; i++){
		if(ZBbuf[i] >= 0xA0){//字母
			ss[pos++] = ZBbuf[i]-0xA0;
		}
		else{//BCD
			ss[pos++] = ((ZBbuf[i]>>4)&0x0F)+'0';
			ss[pos++] = (ZBbuf[i]&0x0F)+'0';
		}
	}
	
	if(pos >= 8){
		memcpy(strin, ss, 8);
	}
	else{//if(pos<8){//没有8位，前面补‘0’
		memcpy(strin+(8-pos), ss, pos);
		memset(strin, '0', (8-pos));
	}
	return pos;
}

unsigned int timer0_tick[4];
//最多4组,设置多少ms
void set_timer0(unsigned int tick, unsigned char tID)
{
	if(tID >= 4)
		return;

	timer0_tick[tID] = tick;
}

//最多4组,返回还有多少ms
unsigned int get_timer0(unsigned char tID)
{
	return timer0_tick[tID];
}

volatile unsigned int beep_nNum=0;
volatile unsigned int beep_Ymun=0;
volatile unsigned int beep_enable=0;
volatile unsigned char beep_times=0;
volatile unsigned int beep_delay=0;

void *onemsSecondDly(void *arg)
{	
	unsigned int t;
	unsigned char beepflag;
	
	while(1){
		usleep(1000);	//延时1ms
		for(t=0; t<4; t++){
			if(timer0_tick[t] > 0)
				timer0_tick[t]--;
		}
		
		//----beep---------
		if(beep_enable){//需要响
			set_beer_b(0);
			beep_enable--;
		}
		else{
			if(beep_times > 1){//响了一次，但是还有1次以上。
				if(beep_delay){//不响延时
					set_beer_b(1);
					beep_delay--;
				}
				else{
					beep_times--;
					beep_delay = beep_nNum;
					beep_enable = beep_Ymun;
				}
			}
			else if(beep_times == 1){//已经响了一次了，关闭BEEP
				set_beer_b(1);
				beep_times = 0;
			}
		}
		//----beep---------

	}
	return (void*)0;
}

//输入7个字节的BCD码
void timewrite(unsigned char *bcdDateTime)
{
	int   year,mon,mday,hour,min, sec;
	
	time_t   t;   
	struct     tm   nowtime   ; 

	year = BCD2int(bcdDateTime, 2);
	mon = BCD2int(bcdDateTime+2, 1);
	mday = BCD2int(bcdDateTime+3, 1);
	hour = BCD2int(bcdDateTime+4, 1);
	min = BCD2int(bcdDateTime+5, 1);
	sec = BCD2int(bcdDateTime+6, 1);
	
//	printf("[%s]:%d-%d-%d %d:%d:%d\r\n", __FUNCTION__, year, mon, mday, hour, min, sec);
  
	nowtime.tm_sec=sec;   /*   Seconds.[0-60]   (1   leap   second)*/                             
	nowtime.tm_min=min;  /*   Minutes.[0-59]   */                                       
	nowtime.tm_hour=hour; /*   Hours. [0-23]   */   
	nowtime.tm_mday=mday; /*   Day.[1-31]   */   
	nowtime.tm_mon=mon-1; /*   Month. [0-11]   */   
	nowtime.tm_year=year-1900; /*   Year-   1900.*/   
	nowtime.tm_isdst=-1;  /*   DST.[-1/0/1]*/   
	
//	printf("[%s]:%d-%d-%d %d:%d:%d\r\n", __FUNCTION__, year, mon, mday, hour, min, sec);
	
	t=mktime(&nowtime);   
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

struct tm_slzr {
        short tm_sec;     /* seconds after the minute - [0,59] */
        short tm_min;     /* minutes after the hour - [0,59] */
        short tm_hour;    /* hours since midnight - [0,23] */
        short tm_mday;    /* day of the month - [1,31] */
        short tm_mon;     /* months since January - [0,11] */
        short tm_year;    /* years since 1900 */
        short tm_wday;    /* days since Sunday - [0,6] */
        short tm_yday;    /* days since January 1 - [0,365] */
        short tm_isdst;   /* daylight savings time flag */
        };
/*++------------------------------------------------------------------------  
Function:              localtimes Modification HistoryJurassic   KSLi          
--------------------------------------------------------------------------*/
void localtimes(time_t timew,int timezonew,short *tm_timew)
{
	////////时间计算-----------------------------------------
	char Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	unsigned int n32_Pass4year;              
	int n32_hpery;    //计算时差  
	struct tm_slzr tt;
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
	if(sll < 0)
	{
		sll = 0;
	}    //取秒时间  
	
//	debugdata((unsigned char*)&sll, 4);
	ii=(int)(sll % 60);    //tm_timew->tm_sec
	tt.tm_sec = ii;
//	debugdata((unsigned char*)&tt.tm_sec, 2);
	sll /= 60;    //取分钟时间    
//	debugdata((unsigned char*)&sll, 4);
	ii=(int)(sll % 60);    
	tt.tm_min = ii;
//	debugdata((unsigned char*)&tt.tm_min, 4);
	sll /= 60;    //取过去多少个四年，每四年有 1461*24 小时    
//	debugdata((unsigned char*)&sll, 4);
//	n32_Pass4year=(unsigned int)(time / (1461L * 24L));    //计算年份    
	n32_Pass4year=sll / 35064l;//(1461L * 24L);    //计算年份
//	debugdata((unsigned char*)&n32_Pass4year, 4);
	ii=(int)(n32_Pass4year << 2)+70;    //四年中剩下的小时数
	tt.tm_year = ii;
//	debugdata((unsigned char*)&tt.tm_year, 4);    
	sll %= 35064;//1461L * 24L;    //校正闰年影响的年份，计算一年中剩下的小时数
//	debugdata((unsigned char*)&sll, 4);
	for(;;){        //一年的小时数        
		n32_hpery = 8760;//365 * 24;        //判断闰年        
		if ((ii & 3) == 0){            //是闰年，一年则多24小时，即一天            
			n32_hpery += 24;
		}
		if (sll < n32_hpery){
			break;
		}
// 	debugstring("t0~ ");
		ii++;
		sll -= n32_hpery;
	}    //小时数
	tt.tm_year = ii;
//	debugdata((unsigned char*)&n32_hpery, 4);
	ii=(int)(sll % 24);    //一年中剩下的天数
	tt.tm_hour = ii;
//	debugdata((unsigned char*)&tt.tm_hour, 4);
	sll /= 24;    //假定为闰年
	sll++;    //校正润年的误差，计算月份，日期
//	debugdata((unsigned char*)&sll, 4);
	ii = tt.tm_year;
	if((ii & 3) == 0){
		if (sll > 60){
			sll--;
		}
		else{
			if (sll == 60)
			{
				tt.tm_mon = 1;
				tt.tm_mday = 29;
				return ;
			}
		}
	}    //计算月日
	ii = tt.tm_mon;
	for (ii = 0;Days[ii] < sll;ii++)
	{
//	debugstring("t1~ ");
//	debugdata((unsigned char*)&tt.tm_mon, 1);
//	debugdata((unsigned char*)&Days[tt.tm_mon], 1);
//	debugdata((unsigned char*)&sll, 4);
		sll -= Days[ii];
		if((sll == 0) || (ii > 11))
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


time_t make_time_t (struct tm_slzr *tb)
{
	////////时间计算-----------------------------------------
	char Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	short i;
        int tmptm1, tmptm2, tmptm3;
//        struct tm_slzr *tbtemp;

        /*
         * First, make sure tm_year is reasonably close to being in range.
         */
        if ( ((tmptm1 = tb->tm_year) < _BASE_YEAR - 1) || (tmptm1 > _MAX_YEAR + 1) )
            goto err_mktime;
        /*
         * Adjust month value so it is in the range 0 - 11.  This is because
         * we don't know how many days are in months 12, 13, 14, etc.
         */
        if ( (tb->tm_mon < 0) || (tb->tm_mon > 11) ) {
            /*
             * no danger of overflow because the range check above.
             */
            tmptm1 += (tb->tm_mon / 12);

            if ( (tb->tm_mon %= 12) < 0 ) {
                tb->tm_mon += 12;
                tmptm1--;
            }

            /*
             * Make sure year count is still in range.
             */
            if ( (tmptm1 < _BASE_YEAR - 1) || (tmptm1 > _MAX_YEAR + 1) )
                goto err_mktime;
        }

        /***** HERE: tmptm1 holds number of elapsed years *****/

        /*
         * Calculate days elapsed minus one, in the given year, to the given
         * month. Check for leap year and adjust if necessary.
         */
		tmptm2 = Days[tb->tm_mon];
		if(tmptm2 == 28)
			tmptm2 += 3;
		else if(tmptm2 < 31)
			tmptm2 += 1;
		for(i = 0; i < tb->tm_mon; i++)
			tmptm2 += Days[i];
        if ( !(tmptm1 & 3) && (tb->tm_mon > 1) )
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
        if ( ChkAdd(tmptm1, tmptm3, tmptm2) )
            goto err_mktime;

		tmptm1 -= 32;
//debugstring("tmptm1:0:");
//debugdata((unsigned char*)&tmptm1, 4);
        /***** HERE: tmptm1 holds number of elapsed days *****/

        /*
         * Calculate elapsed hours since base date
         */
        tmptm2 = tmptm1 * 24;
        if ( ChkMul(tmptm2, tmptm1, 24) )
            goto err_mktime;

//debugstring("tmptm2:1:");
//debugdata((unsigned char*)&tmptm2, 4);
        tmptm1 = tmptm2 + (tmptm3 = (int)tb->tm_hour);
        if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
            goto err_mktime;

//debugstring("tmptm1:1:");
//debugdata((unsigned char*)&tmptm1, 4);
        /***** HERE: tmptm1 holds number of elapsed hours *****/

        /*
         * Calculate elapsed minutes since base date
         */

        tmptm2 = tmptm1 * 60;
        if ( ChkMul(tmptm2, tmptm1, 60) )
            goto err_mktime;

//debugstring("tmptm2:3:");
//debugdata((unsigned char*)&tmptm2, 4);
        tmptm1 = tmptm2 + (tmptm3 = (int)tb->tm_min);
        if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
            goto err_mktime;

        /***** HERE: tmptm1 holds number of elapsed minutes *****/

        /*
         * Calculate elapsed seconds since base date
         */

        tmptm2 = tmptm1 * 60;
        if ( ChkMul(tmptm2, tmptm1, 60) )
            goto err_mktime;

//debugstring("tmptm2:4:");
//debugdata((unsigned char*)&tmptm2, 4);
        tmptm1 = tmptm2 + (tmptm3 = (int)tb->tm_sec);
        if ( ChkAdd(tmptm1, tmptm2, tmptm3) )
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
	struct tm_slzr time_check;
	unsigned short uiTemp;

	t_scr = 0l;

	uiTemp = (unsigned short)BCD2HEX(timee,2);
	time_check.tm_year = uiTemp - 1900;
	uiTemp = (unsigned short)BCD2HEX(timee+2,1);
    time_check.tm_mon  = uiTemp - 1;
	uiTemp = (unsigned short)BCD2HEX(timee+3,1);
    time_check.tm_mday = uiTemp;
	uiTemp = (unsigned short)BCD2HEX(timee+4,1);
    time_check.tm_hour = uiTemp;
	uiTemp = (unsigned short)BCD2HEX(timee+5,1);
    time_check.tm_min  = uiTemp;
	uiTemp = (unsigned short)BCD2HEX(timee+6,1);
    time_check.tm_sec  = uiTemp;
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




//打开fileName指定的文件，从中读取第lineNumber行
//返回值：成功返回1，失败返回0
int get_file_line(char *result,char *fileName,int lineNumber)
{
	
#define FILEBUFFER_LENGTH 5000
#define EMPTY_STR ""

    FILE *filePointer;
    int i=0;
    char buffer[FILEBUFFER_LENGTH];
    char *temp;

    memset(buffer,'\0',FILEBUFFER_LENGTH*sizeof(char));
    strcpy(buffer,EMPTY_STR);

    if((fileName==NULL)||(result==NULL))
    {
        return 0;
    }

    if(!(filePointer=fopen(fileName,"rb")))
    {return 0;}

	
    while((!feof(filePointer))&&(i<lineNumber))
    {
        if(!fgets(buffer,FILEBUFFER_LENGTH,filePointer))
        {
            return 0;
        }
        i++;//差点又忘记加这一句了
    }
	
    if(0!=fclose(filePointer))
    {
        return 0;
    }
	
    if(0!=strcmp(buffer,EMPTY_STR))
    {
        while(NULL!=(temp=strstr(buffer,"\n")))
        {
            *temp='\0';
        }

        while(NULL!=(temp=strstr(buffer,"\r")))
        {
            *temp='\0';
        }

		

        strcpy(result,buffer);
    }else
    {
        strcpy(result,EMPTY_STR);
        return 0;
    }
	
    return 1;
}

//读CPU序列号，一只有12字节长度，因系统中只能用4字节，所以把这12字节计算了一个MAC值，当做4字节的唯一序列号。
void Get_SerialNum(unsigned char *ser)
{
    int  icnt;
    
	char FileName[256];
	unsigned char csnbuf[200];

	unsigned char buff[20];
	unsigned char cmac[4];
	unsigned char kbuf[16];
	unsigned char radon[8];

	memset(buff, '1', sizeof(buff));
	buff[19] = 0;

    strcpy(FileName,"/sys/class/sunxi_info/sys_info");
    
    if(1 == get_file_line((char*)csnbuf,FileName,3)){
        for(icnt=0; icnt<10; icnt++)
            buff[icnt] = (unsigned char)csnbuf[icnt + 20];
#ifdef _debugPSAM_
        printf("write00 SN\"%s\"\n",(char*)csnbuf);
        printf("write 22SN\"%s\" Success.\n",(char*)buff);
#endif
    }
    	
	memset(kbuf, 0x11, 16);
	memset(radon, 0x22, 8);
	
#ifdef _debugPSAM_
	printf("readd11 SN\"%s\" Success.\n",(char*)buff);
#endif

	CountMac((char*)kbuf, radon, buff, 16, cmac);

	printf("----SN----:");
	debugdata(cmac, 4, 1);

	memcpy(ser, cmac, 4);
}

//读	F4 CPU序列号，一只有12字节长度，因系统中只能用4字节，所以把这12字节计算了一个MAC值，当做4字节的唯一序列号。
void Get_SerialNumF4(unsigned char *ser)
{
	Get_SerialNum(ser);
}



unsigned char time_invalid(unsigned char *time_buff)
{
	unsigned int year;
	unsigned char i;
	
	for(i=0;i<7;i++){
		if(((time_buff[i]&0x0f) > 0x09)||(((time_buff[i]>>4)&0x0f) > 0x09))
			return 0;
	}
	
	if(time_buff[0]!=0x20)
	{
		if((time_buff[0]!=0x19)||(time_buff[1]!=0x99))
			return 0;
	}
	
	if((!time_buff[2])||(time_buff[2]>0x12))
		return 0;
	
	switch(time_buff[2])
	{
	case 0x01:
	case 0x03:
	case 0x05:
	case 0x07:
	case 0x08:
	case 0x10:
	case 0x12:
		if((!time_buff[3])||(time_buff[3]>0x31))
			return 0;
		break;	
	case 0x04:
	case 0x06:
	case 0x09:
	case 0x11:
		if((!time_buff[3])||(time_buff[3]>0x30))
			return 0;
		break;
	case 0x02:
		year=(unsigned int)(time_buff[0]>>4)*1000+(time_buff[0]&0x0f)*100
			+(time_buff[1]>>4)*10+(time_buff[1]&0x0f);
		
		if(year%4)
		{
			if((!time_buff[3])||(time_buff[3]>0x28))
				return 0;
			break;
		}
		else
		{
			if((!time_buff[3])||(time_buff[3]>0x29))
				return 0;
			break;
		}
	}
	
	if(time_buff[4]>0x23)
		return 0;
	if(time_buff[5]>0x59)
		return 0;
	if(time_buff[6]>0x59)
		return 0;
	return 1;
}


void MessageBox(unsigned char mode, const char *dStr)
{
#warning "need MessageBox code"

	printf("mode:%d, %s\r\n", mode, dStr);
}

/*      LED        */					  
#define LED_RED        ((unsigned char)1)
#define LED_GREEN      ((unsigned char)2)
#define LED_BLUE       ((unsigned char)4)
#define LED_YELLOW     ((unsigned char)8)

void led_on(unsigned char mode)
{
	char buf[8]={0};
	int cl;

	if(mode == LED_GREEN)
		cl = SCREEN_G;
	else if(mode == LED_BLUE)
		cl = SCREEN_B;
	else if(mode == LED_YELLOW)
		cl = SCREEN_R;
	else
		cl = SCREEN_R;
	Light_main(SCREEN_LIGHT, LIGHT_OPEN, cl, (char *)buf);
}

void led_off(unsigned char mode)
{
	char buf[8]={0};
	int cl;
	
	if(mode == LED_GREEN)
		cl = SCREEN_G;
	else if(mode == LED_BLUE)
		cl = SCREEN_B;
	else if(mode == LED_YELLOW)
		cl = SCREEN_R;
	else
		cl = SCREEN_R;

	if(mode == 0xFF){
		Light_main(SCREEN_LIGHT, LIGHT_CLOSE, SCREEN_G, (char *)buf);
		Light_main(SCREEN_LIGHT, LIGHT_CLOSE, SCREEN_B, (char *)buf);
		Light_main(SCREEN_LIGHT, LIGHT_CLOSE, SCREEN_R, (char *)buf);
	}
	else{
		Light_main(SCREEN_LIGHT, LIGHT_CLOSE, cl, (char *)buf);
	}
}


//蜂鸣器,次数、响延时、停延时
void beep(unsigned char ucTimes, unsigned int ulBeepms, unsigned int ulNoms)
{
	if(beep_times > ucTimes)
		return;
	beep_delay = ulNoms*3;
	beep_enable = ulBeepms*3;
	beep_times = ucTimes;
	beep_Ymun = ulBeepms*3;
	beep_nNum = ulNoms*3;
}

//外部键盘输入

unsigned char Keybuffer[10];
volatile unsigned char itKEYB_value;
unsigned char KeyMange(unsigned char kkvv);
extern stDeviceParatable gDeviceParaTab;
extern fFlagVary_1 gBuInfo;	

void KeyBoardDeal(unsigned char vdat)
{
	itKEYB_value = vdat;
}

void LedDisplay(unsigned char *ptr)
{
	unsigned char i=0;
	unsigned char sbuff[64];
	
	sbuff[0] = 0x1B;//	SendCommandkey('C','0');
	sbuff[1] = 'C';
	sbuff[2] = '0';
	R485WriteData(sbuff, 3);
	
	usleep(1000);

	i=0;
	sbuff[i++] = 0x1B;
	sbuff[i++] = 'S';
	sbuff[i++] = '0';
	memcpy(sbuff+i, ptr, 6);
	i += 6;
	sbuff[i++] = 0x0D;

	R485WriteData(sbuff, i);
}
void keyboard_open(void)
{
	unsigned char sbuff[64];
	
	sbuff[0] = 0x1B;//	SendCommandkey('K','0');
	sbuff[1] = 'K';
	sbuff[2] = '0';
	R485WriteData(sbuff, 3);

	usleep(1000);
	
	sbuff[0] = 0x1B;//	SendCommandkey(0x50,0x33);
	sbuff[1] = 0x50;
	sbuff[2] = 0x33;
	R485WriteData(sbuff, 3);

	itKEYB_value = 0xff;
}

unsigned char KeyDeal(void)
{
	unsigned char j=0, dat;

	if(itKEYB_value == 0xFF){
		return ST_ERROR;
	}
	else
		dat = itKEYB_value;
	if((dat>='0')&&(dat<='9'))
	{
		for(j=0;j<5;j++)
		{
			if(memcmp(Keybuffer,"\x20\x20\x20\x20\x20",5)==0)
			if(Keybuffer[5]=='0')
				Keybuffer[5]=0x20;
			Keybuffer[j]=Keybuffer[j+1];
		}
		Keybuffer[5]=dat;
	}
	else if(dat==0x08)//清除键
	{
		for(j=0;j<5;j++)
		{
			Keybuffer[5-j]=Keybuffer[4-j];
		}
		if(Keybuffer[0]!=0x20)
			Keybuffer[0]=0x20;

		if(Keybuffer[5]==0x20)
		{
			memset(Keybuffer,0x20,5);
			Keybuffer[5]='0';
		}
	}
	else if(dat==0x0d)//确定键
	{
		if((Keybuffer[5]&0x80)==0x80)
		{
			memset(Keybuffer,0x20,5);
			Keybuffer[5]='0';
		}
		
	}
	else if(dat==0x2e)
	{
		Keybuffer[5]|=0x80;
	}//  41=F1键  42=F2键
	else{
		itKEYB_value = 0xff;
		return ST_ERROR;
	}
	itKEYB_value = 0xff;
	if(KeyMange(dat) == ST_OK){
		LedDisplay(Keybuffer);
	}
	return ST_OK;
}
void KeyDis(unsigned int value,unsigned char *ptr)
{
	unsigned char i;
	unsigned char buffer1[20];

	memset(buffer1,0,20);
	sprintf((char*)buffer1,"%-7u",value);
	for(i=0;i<14;i++)
	{
		if(buffer1[i]==0x20)
			break;
	}
	
	if(i==2)
	{
		ptr[0]='0';
		ptr[0]|=0x80;
		memcpy(ptr+1,buffer1,2);
		ptr[4]=0x20;
	}
	else if(i==1)
	{
		ptr[0]='0';
		ptr[0]|=0x80;
		ptr[1]='0';
		ptr[2]=buffer1[0];
		ptr[3]=0x20;
	}
	else if(i>2)
	{
//		i-=2;
		memcpy(ptr,buffer1,i);
		i-=3;
		ptr[i]|=0x80;
	}
	memmove(ptr+3,ptr,3);
	memset(ptr,0x20,3);	
}
unsigned char KeyMange(unsigned char kkvv)
{
	unsigned char i=0,a=0,j=0;
	unsigned int ii=0,KeyVa;
	a=0;
	//小数点不能超过两个
	for(i=0;i<6;i++)
	{
		if((Keybuffer[5-i]&0x80)==0x80)
			a++;
	}
	//寻找小数点的位置
	for(i=0;i<6;i++)
	{
		if((Keybuffer[5-i]&0x80)==0x80)
			break;
	}
	if(((i>2)&&(i!=6))||(a>1))
	{
		beep(2,50,50);
		memset(Keybuffer,0x20,5);
		Keybuffer[5]='0';
		KeyVa=0;
	} 
	else
	{
		a=0;
		KeyVa=0;
		for(j=0;j<6;j++)
		{
			if(Keybuffer[5-j]!=0x20)
				a++;
		}
		if(i==1)
		{
			KeyVa=(unsigned int)((Keybuffer[5]-'0')*10);

		}
		else if(i==2)
		{
			KeyVa=(unsigned int)((Keybuffer[5]-'0')+((Keybuffer[4]-'0')*10));

		}
		else if(i==6)
		{
             i=0;
		}
		ii=10;
		for(j=i;j<a;j++)
		{
			ii=ii*10;
			KeyVa+=(unsigned int)(((Keybuffer[5-j]-'0')&0x7f)*ii);
		}

		if(KeyVa>=2000)//不能超过30
		{
			beep(2,50,50);
			memset(Keybuffer,0x20,5);
			Keybuffer[5]='0';
			KeyVa=0;
			KeyDis(KeyVa,Keybuffer);
			LedDisplay(Keybuffer);
			memset(Keybuffer,0x20,5);
			Keybuffer[5]='0';
			KeyVa=0;
			return ST_ERROR;
		}
	}
	if(kkvv==0x0d)	//确定键
	{
		if(KeyVa==0)
		{
			beep(2,50,50);	
			memcpy((unsigned char*)&ii, gDeviceParaTab.busPrice, 4);
			KeyDis(ii,Keybuffer);
			LedDisplay(Keybuffer);

			memset(Keybuffer,0x20,6);
			Keybuffer[5]='0';
			return ST_ERROR;
		}
		else
		{

			memcpy(gDeviceParaTab.busPrice, (unsigned char*)&KeyVa, 4);
//			sysfewrite(BIT_KeyPrice, 4, gDeviceParaTab.busPrice);	////2014.7.17
//			saveDeviceParaTab(1, gDeviceParaTab.busPrice);	//不保存
			beep(1,500,25);
			KeyDis(KeyVa,Keybuffer);
			LedDisplay(Keybuffer);
			memset(Keybuffer,0x20,5);
			Keybuffer[5]='0';
			gBuInfo.restore_flag=1;
			
			return ST_ERROR;
		}
	}
	else
	{
		KeyVa=0;
	}
	return ST_OK;
}

void keyBoardStart(void)
{
	unsigned int ksi;
	keyboard_open();
	memset(Keybuffer,0x20,5);
	// 	memset(NewPriceTab.busPrice, 0, 4);
	// 	memset(NewPriceTab.plusPrice, 0, 4);
	// 	Keybuffer[5]='0';
	memcpy((unsigned char*)&ksi, gDeviceParaTab.busPrice, 4);
	
	KeyDis(ksi,Keybuffer);
	LedDisplay(Keybuffer);
	memset(Keybuffer,0x20,5);
	Keybuffer[5]='0';
}




#define Audio_INVALID		2		//非法卡
#define Audio_NOMONEY		3		//金额不足//余额不足
#define Audio_FAMILY		4		//家属卡
#define Audio_Zilu			5		//子女卡
#define Audio_YWJYu			6		//亲情卡  请年审  义务教育卡（九江）
#define Audio_STUDENT		7		//学生卡
#define Audio_STUFF			9		//员工卡
#define Audio_OLDMAN		10		//老人卡
#define Audio_LOVER			11		//爱心卡
#define Audio_TuanTi		12		//团体卡
#define Audio_Wuxiao		13		//无效卡
#define Audio_zuanxian		14		//专线卡 签到完毕, 请下车
#define Audio_MONTH			15		//月票卡
#define Audio_PuTong		16		//普通卡
#define Audio_YH			17		//优惠卡
#define Audio_DONG			18		//咚
#define Audio_DingDang		19		//咚咚，电信卡
#define Audio_Di			20		//嘀
#define Audio_YongJun		21		//拥军卡
#define Audio_SuaKaOK		22		//刷卡成功
#define Audio_YouDai		23		//优待卡
#define Audio_Siji			24		//司机卡
#define Audio_PLUS_MONEY	25		//请充值..B=请及时充值
#define Audio_BIRTHDAY		26		//敬老卡
#define Audio_CY			27		//优扶卡
#define Audio_FULI			28		//福利卡
#define Audio_GuaShi		29		//挂失卡,请交回
#define Audio_XiaoXueShen	30		//小学生卡
#define Audio_XueShenYP		31		//学生月票卡
#define Audio_YuerDi5		32		//余额低于5元

#define Audio_no_di			0xee	//没语音，只是蜂鸣器响

#define Audio_TRY_AGAIN		1		//请重刷 
#define Audio_FLING_MONEY	8		//请投币

#define voice_invalid		"aplay /mnt/qrdata/wav/invalidcard.wav"		//非法卡
#define voice_studentcard	"aplay /mnt/qrdata/wav/studentcard.wav"		//学生卡
#define voice_jianhangcard	"aplay /mnt/qrdata/wav/jianhangcard.wav"	//建行卡
#define voice_stuffcard		"aplay /mnt/qrdata/wav/stuffcard.wav"		//员工卡
#define voice_jiaotongcard	"aplay /mnt/qrdata/wav/jiaotongcard.wav"	//交通卡
#define voice_jinglaocard	"aplay /mnt/qrdata/wav/jinglaocard.wav"		//敬老卡
#define voice_liantongcard	"aplay /mnt/qrdata/wav/liantongcard.wav"	//联通卡
#define voice_youfucard		"aplay /mnt/qrdata/wav/youfucard.wav"		//优扶卡
#define voice_monthcard		"aplay /mnt/qrdata/wav/monthcard.wav"		//月票卡
#define voice_youhuicard	"aplay /mnt/qrdata/wav/youhuicard.wav"		//优惠卡
#define voice_normalcard	"aplay /mnt/qrdata/wav/normalcard.wav"		//普通卡
#define voice_zhuanxiancard "aplay /mnt/qrdata/wav/zhuanxiancard.wav"	//专线卡
#define voice_plsgeton		"aplay /mnt/qrdata/wav/plsgeton.wav"		//请上车
#define voice_drivercard	"aplay /mnt/qrdata/wav/drivercard.wav"		//司机卡
#define voice_showagain		"aplay /mnt/qrdata/wav/showagain.wav"		//请重刷
#define voice_shuamashangche "aplay /mnt/qrdata/wav/shuamashangche.wav"	//刷码成功,请上车
#define voice_WelCome		"aplay /mnt/qrdata/wav/success.wav"			//欢迎乘车
#define voice_chargemonty	"aplay /mnt/qrdata/wav/chargemonty.wav"		//请充值
#define voice_flingmoney	"aplay /mnt/qrdata/wav/flingmoney.wav"		//请投币

//语音提示
void audio(unsigned char flag)
{
	char wavfile[128];
	int ret, i;

	switch(flag){
	case Audio_INVALID:
		strcpy(wavfile, voice_invalid);
		break;
	case Audio_DONG:
		strcpy(wavfile, voice_WelCome);
		break;
	case Audio_STUDENT:
		strcpy(wavfile, voice_studentcard);
		break;
	case Audio_BIRTHDAY:
		strcpy(wavfile, voice_jinglaocard);
		break;
	case Audio_STUFF:
		strcpy(wavfile, voice_stuffcard);
		break;
	case Audio_MONTH:
		strcpy(wavfile, voice_monthcard);
		break;
	case Audio_zuanxian:		//邯钢专线卡  //增加专线卡 2014.4.8
		strcpy(wavfile, voice_zhuanxiancard);
		break;
	case Audio_FLING_MONEY:
		strcpy(wavfile, voice_flingmoney);
		break;
	case Audio_PLUS_MONEY:
		strcpy(wavfile, voice_chargemonty);
		break;
	case Audio_TRY_AGAIN:
		strcpy(wavfile, voice_showagain);
		break;
	default:
		wavfile[0] = 0;
		break;
	}
	i = strlen(wavfile);

	printf("[%s], flag=%d, i=%d:%s\r\n", __FUNCTION__, flag, i, wavfile);

	if(i != 0)
	{
		ret = system(wavfile);
		printf("[%s], %s,ret=%d\r\n", __FUNCTION__, wavfile, ret);
		usleep(10000);
	}
}

// LCD
#define DIS_CONVERT	(0x1)
#define DIS_CENTER	(0x1 << 1)
#define DIS_RIGHT	(0x1 << 2)
#define DIS_ClsLine	(0x1 << 3)
#define DIS_Cls		(0x1 << 4)
//清显示区
void cls(void)
{
	;
}

void display(unsigned char x, unsigned char y, const char *str, unsigned char mode)
{
#warning "display need code";
	printf("%s ", str);
}
void miniDispstr(unsigned char x, unsigned char y, const char *lcd_string, unsigned char mode)
{
#warning "miniDispstr need code";
	printf("%s ", lcd_string);

}

void dis_time_mini(unsigned char x, stcomtime *time)
{
	unsigned int pos;
	unsigned char buff[30];
	pos = 0;
	memset(buff,0,17);
	
	BCD2Ascii((unsigned char*)&time->year_h, buff+pos, 1); pos+=2;
	BCD2Ascii((unsigned char*)&time->year_l, buff+pos, 1); pos+=2;
	buff[pos++] = '-';
	BCD2Ascii((unsigned char*)&time->month, buff+pos, 1); pos+=2;
	buff[pos++] = '-';
	BCD2Ascii((unsigned char*)&time->day, buff+pos, 1); pos+=2;
	buff[pos++] = ' ';
	BCD2Ascii((unsigned char*)&time->hours, buff+pos, 1); pos+=2;
	buff[pos++] = ':';
	BCD2Ascii((unsigned char*)&time->minutes, buff+pos, 1); pos+=2;
	buff[pos++] = ':';
	BCD2Ascii((unsigned char*)&time->seconds, buff+pos, 1); pos+=2;
	buff[pos++] = 0;
	
	miniDispstr(x, 1, (char*)buff, 0);
}



//后背LED显示,4字符
void LED_Dis3(char *str)
{
	Light_main(LED_LIGHT, 0, 0, (char *)str);
}

//-------------IC卡相关函数---------------
unsigned char MifareWrite(unsigned char cBlockNo, unsigned char *pWrData)
{
	unsigned int ret ;
	unsigned char sta;

	ret = CmdWriteSet(cBlockNo, pWrData, 16, &sta);

	if((ret == MI_OK) && (sta == MI_OK))
	{
		return 1;
		
	}
	else
	{
		printf("[%s] \n", __FUNCTION__);
		return 0;
	}
}

unsigned char MifareAuthKey(unsigned char auth_mode,
								   unsigned char *snr,
								   unsigned char *keys,
								   unsigned char block)
{
//	unsigned char buff[64];
	SLZR_U8 status = 0;
	unsigned int u32KeyMode;
	unsigned int ret;
	
	ret = CmdLoadkeySet(keys,6, &status);
	if((ret == MI_OK) && (status == MI_OK))
	{
		
		block = ((block / 4 + 1) * 4)- 1;
		if(auth_mode == PICC_AUTHENT1B)
			u32KeyMode = KEYB;
		else
			u32KeyMode = KEYA;
		
		printf("[%s] start \n", __FUNCTION__);
		ret = CmdAuthenticationSet(u32KeyMode, block, &status);
		printf("[%s] end ret:%d \n", __FUNCTION__, ret);
		if((ret == MI_OK) && (status == MI_OK))
		{
			return 1;
			
		}
		else
		{
			printf("[%s] CmdAuthenticationSet失败 \n", __FUNCTION__);
			return 0;
		}
	}
	else
	{
		printf("[%s] CmdLoadkeySet失败 \n", __FUNCTION__);
		return 0;
	}

}

unsigned char MifareRead(unsigned char cBlockNo, unsigned char *pRdData)
{
	unsigned int ret, len ;
	unsigned char sta;

	ret = CmdReadSet(cBlockNo, pRdData, &len, &sta);
	if((ret == MI_OK) && (sta == MI_OK))
	{
		return len;		
	}
	else
		return 0;
}


unsigned char MifIsPurse(unsigned char *buffer)	
{
	unsigned char i;
	for(i=0;i<4;i++)
	{
		if((buffer[i]^buffer[4+i])!=0xff)
			return 1;
		if(buffer[i]!=buffer[8+i])
			return 1;
	}
	
	if(buffer[12]!=buffer[14])
		return 1;
	if(buffer[13]!=buffer[15])
		return 1;
	if((buffer[12]^buffer[13])!=0xff)
		return 1;
	return 0;
}

unsigned char MiBlockInvalid(unsigned char mode,unsigned char count,unsigned char *ptr)
{  
	unsigned char i;
	unsigned char check_sum=0;
	for(i=0;i<count;i++)
	{
		check_sum^=ptr[i];
	}
	if(mode)
	{
		if(check_sum==ptr[count])
			return 0;
		else
			return 1;
	}
	else
		return check_sum;
}

unsigned char GetTypeAUID( unsigned char *pUID )
{
	unsigned char buff[64];
	unsigned int len;
	unsigned int ret;
	
	ret = CmdFindCardSet(buff, &len, buff+30);
	if(ret == SLZR_SUCCESS){
#ifdef _debugPSAM_
		printf("[%s] __FindCard OK:", __FUNCTION__);
		debugdata(buff, len, 1);
#endif
		memcpy(pUID, buff, len);
		
		return len;		
	}
	else{
#ifdef _debugPSAM_
		printf("[%s] __FindCard Error:%d\r\n", __FUNCTION__, ret);
#endif
		return 0;
	}


}

void MifareHalt(void)
{
	unsigned char ret;

	CmdHaltSet(&ret);
}

unsigned char MifareValueDebit(unsigned char cSubCommand, unsigned char cBlockNo, unsigned char *pValue)
{
	unsigned int ret ;
	unsigned char sta;

	ret = CmdM1DecrementSet(cBlockNo, pValue, 4, &sta);
		
	if(ret == SLZR_SUCCESS)
		return 4;
	else 
		return 0;
}
// ==================================================================
//
unsigned char MifareTransfer(unsigned char cBlockNo)
{
// 	unsigned int ret;
// 	unsigned char sta;
// 
// 	ret = CmdM1RestoreSet(cBlockNo, &sta);
// 
// 	if(ret == SLZR_SUCCESS)
// 		return 4;
// 	else 
// 		return 0;

	return 4;
}


void deal2or3(unsigned char*sndd)
{
	;
}

unsigned int CPURats(unsigned char *rbuf)
{
	unsigned int ret , rlen;
	unsigned char sta;

	ret = CmdRatsSet(rbuf, &rlen, &sta);
	if(ret ==SLZR_SUCCESS)
		return rlen;
	else
		return 0;
}

unsigned int RfPassthrough( unsigned char *dst, unsigned int iMaxlen, unsigned char *src, unsigned int sendlen)
{
	unsigned int ret , rlen;
	unsigned char sw[8];
	unsigned char sta;

	if((src[0] == 0x02) || (src[0] == 0x03)){		//去掉最前面的02 或 03
		memmove(src, src+1, sendlen-1);
		sendlen--;
	}

	ret = CmdApduSet(src, sendlen, dst, &rlen, &sta, sw);

	if(ret == SLZR_SUCCESS){
		return rlen;
	}
	else
		return 0;
}


//--------------下面是PSAM卡相关函数------------------

#define ERASE_BINARY	0x0E
#define VERIFY			0x20
#define MANAGE_CHANNEL	0x70
#define EXTERNAL_AUTH	0x82
#define GET_CHALLENGE	0x84
#define INTERNAL_AUTH	0x88
#define SELECT_FILE		0xA4
#define READ_BINARY		0xB0
#define GET_RESPONSE	0xC0
#define ENVELOPE		0xC2
#define GET_DATA		0xCA
#define WRITE_BINARY	0xD0
#define WRITE_RECORD	0xD2
#define UPDATE_BINARY	0xD6
#define PUT_DATA		0xDA
#define UPDATE_RECORD	0xDC
#define APPEND_RECORD	0xE2

//****************APDU*******************************//
#define APDU_SEND_DATA_MAX_LEN   300
typedef struct
{
	unsigned char CLA;
	unsigned char INS;
	unsigned char P1;
	unsigned char P2;
	
	unsigned char LC;
	unsigned char DATA[APDU_SEND_DATA_MAX_LEN];
	
	unsigned char LE;
}ISO7816_ADPU_SEND;

typedef struct
{
	unsigned char LE;
	unsigned char DATA[APDU_SEND_DATA_MAX_LEN];
	
	unsigned char SW1;
	unsigned char SW2;
}ISO7816_ADPU_RESPONSE;

typedef struct{
	unsigned char curSlot;		//当前的卡座号,1-4
	unsigned int baud[4];		//波特率，如果切换卡座时波特率也不一样则需要先切换波特率,
}stpsamInfo;

stpsamInfo lPsaminfo;


//卡片复位时调用
void savePsaminfo(unsigned char Slot, unsigned int uiRate)
{
#ifdef _debugPSAM_
	printf("[%s] Slot:%d, Rate:%d\r\n",__FUNCTION__, Slot, uiRate);
#endif
	if((Slot==0) || (Slot>4))
		return;
	lPsaminfo.curSlot = Slot;
	lPsaminfo.baud[Slot-1] = uiRate;
	
	if(uiRate == 38400)
		CmdPSAMbps(1);
	else
		CmdPSAMbps(0);
	usleep(100000);
}

//发APDU指令时调用
void changePsaminfo(unsigned char Slot)
{
#ifdef _debugPSAM_
	printf("[%s] Slot:%d\r\n", __FUNCTION__, Slot);
#endif
	if((Slot==0) || (Slot>4))
		return;
	if(Slot == lPsaminfo.curSlot) //没有换卡座，退出
		return;
	
#ifdef _debugPSAM_
	printf("[%s] curSlot:%d, Slot:%d, curRate:%d, SlotRate:%d\r\n",__FUNCTION__,lPsaminfo.curSlot, Slot, lPsaminfo.baud[lPsaminfo.curSlot-1], lPsaminfo.baud[Slot-1]);
#endif
	if(lPsaminfo.baud[lPsaminfo.curSlot-1] != lPsaminfo.baud[Slot-1]){ //如果要切的卡座和正在使用的卡座的波特率不一样的，要处理。
		if(lPsaminfo.baud[Slot-1] == 38400)	
			CmdPSAMbps(1);
		else
			CmdPSAMbps(0);
		usleep(100000);
	}
	lPsaminfo.curSlot = Slot;	//切换卡座
	
	
}

unsigned char SamColdReset(unsigned char CardSelect, unsigned int uiRate, 
						   unsigned char ucVoltage, unsigned char *atr){

	unsigned int ret, rlen;
	unsigned char sta;

	
	savePsaminfo(CardSelect, uiRate);
	
	
	ret = CmdDetectPsamSet(CardSelect, atr, &rlen, &sta);

	if(ret == 0)
		return rlen;
	return 0;

}

unsigned char SamSendApdu(unsigned char Slot, unsigned char *pTxBuf, unsigned short iLen, unsigned char *pRxBuf)
{
	unsigned int rlen, ret;
	unsigned char sta;
	
#ifdef _debugPSAM_
	printf("[%s]---- \r\n", __FUNCTION__);
	debugdata((unsigned char*)&rlen, 4, 1);
#endif

	ret = CmdPsamApduSet(Slot, pTxBuf, iLen, pRxBuf, &rlen, &sta);//	length = SamdSendAPDUT0(pTxBuf, iLen, pRxBuf);
	if(ret != 0){	//通讯失败
		return 0;
	}

#ifdef _debugPSAM_
	debugstring("SA-len:");
	debugdata((unsigned char*)&rlen, 4, 1);
	debugstring("SA-bf:");
	debugdata(pRxBuf, rlen, 1);
#endif

	if((rlen == 2) && (pRxBuf[0] == 0x61))
	{
		pTxBuf[0] = 0x00;
		pTxBuf[1] = GET_RESPONSE;
		pTxBuf[2] = 0x00;
		pTxBuf[3] = 0x00;
		pTxBuf[4] = pRxBuf[1];
			
		ret = CmdPsamApduSet(Slot, pTxBuf, 5, pRxBuf, &rlen, &sta);
		
		if(ret != 0)
			return 0;
	}


	if((pTxBuf[1] == pRxBuf[0]) && (pRxBuf[rlen-1] == 0x90)){		//有时返回B005600001143390这个数据，前面的INS当作数据了，后面少了个00,这里处理下
		memmove(pRxBuf, pRxBuf+1, rlen);
		pRxBuf[rlen-1] = 0;
	}

	return rlen;

}

unsigned char IccCommand(unsigned char Slot, ISO7816_ADPU_SEND *ApduSend, 
						 ISO7816_ADPU_RESPONSE *ApduRecv)
{
	unsigned char sendbuf[300];
	unsigned char revbuf[300];
	unsigned char rt;
	unsigned char len=0;
	
	changePsaminfo(Slot);	//先看看是否要切换波特率

	memcpy(sendbuf, (unsigned char*)&ApduSend->CLA, 4);
	len = 4;
	if(ApduSend->LC > 0){
		sendbuf[len++] = ApduSend->LC;
		memcpy(sendbuf+len, ApduSend->DATA, ApduSend->LC);
		len = len + ApduSend->LC;
	}
	else{
		sendbuf[len++] = 0;
	}

	sendbuf[len++] = ApduSend->LE;

	
#ifdef _debugPSAM_
  	debugstring("ICC SND:");
	debugdata(sendbuf, len, 1);
#endif
		rt = SamSendApdu(Slot, sendbuf, len, revbuf);
#ifdef _debugPSAM_
		debugstring("IccCommand:");
		debugdata(&rt, 1, 1);
		debugdata(revbuf, rt, 1);
#endif
		if(rt < 2){
			ApduRecv->LE = 0;
			ApduRecv->SW1 = 0x00;
			ApduRecv->SW2 = 0x00;
			return 0;
		}
		
		ApduRecv->LE = rt-2;
		memcpy(ApduRecv->DATA, revbuf, rt-2);
		ApduRecv->SW1 = revbuf[rt-2];
		ApduRecv->SW2 = revbuf[rt-1];
		return 0;
}

unsigned char Get_Challenge(unsigned char Slot, unsigned char *Receivebuf, unsigned char len)
{
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;

	Scmd.CLA = 0x00;
	Scmd.INS = 0x84;
	Scmd.P1 = 0x00;
	Scmd.P2 = 0x00;
	Scmd.LE = len;
	Scmd.LC = 0;
	IccCommand(Slot, &Scmd, &Rcmd);

//	if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0)
	{
		memcpy(Receivebuf,(unsigned char*)&Rcmd.SW1,2);
		return PSAM_FALSE;
	}
	
	memcpy(Receivebuf,Rcmd.DATA,len);

	return len;
}

unsigned char  Select_FileF(unsigned char Slot,unsigned char *file_ID, unsigned char len)
{
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;
	
	Scmd.CLA = 0x00;
	Scmd.INS = 0xA4;
	Scmd.P1 = 0x04;
	Scmd.P2 = 0x00;
	Scmd.LC = len;
	memcpy(Scmd.DATA, file_ID, len);
	Scmd.LE = 0x00;
	IccCommand(Slot, &Scmd, &Rcmd);	
	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0)
		return PSAM_FALSE;
	
	return PSAM_TRUE;
}

unsigned char  Select_File(unsigned char Slot,unsigned int file_ID)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;
		unsigned char i;
		
		for(i=0;i<3;i++){
			Scmd.CLA = 0x00;
			Scmd.INS = 0xA4;
			Scmd.P1 = 0x00;
			Scmd.P2 = 0x00;
			Scmd.LC = 0x02;
			Scmd.DATA[0] = file_ID/256;
			Scmd.DATA[1] = file_ID%256;
			Scmd.LE = 0x00;
			IccCommand(Slot, &Scmd, &Rcmd);
	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
			if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0)
				continue;//return PSAM_FALSE;
			else
				return PSAM_TRUE;
		}
		return PSAM_FALSE;
}

unsigned char Verify_private_PIN(unsigned char Slot, unsigned char length, unsigned char *ptr)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;

		Scmd.CLA = 0x00;
		Scmd.INS = 0x20;
		Scmd.P1 = 0x00;
		Scmd.P2 = 0x00;
		Scmd.LC = length;
		memcpy(Scmd.DATA,ptr,length);
		Scmd.LE = 0;

		IccCommand(Slot, &Scmd, &Rcmd);

//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0){
			ptr[0] = Rcmd.SW1;
			ptr[1] = Rcmd.SW2;
			return PSAM_FALSE;
		}

		return PSAM_TRUE;


}

unsigned char Change_private_PIN(unsigned char Slot, unsigned char length,unsigned char *ptr)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;

		Scmd.CLA = 0x80;
		Scmd.INS = 0x5e;
		Scmd.P1 = 0x01;
		Scmd.P2 = 0x00;
		Scmd.LC = length;
		memcpy(Scmd.DATA,ptr,length);
		Scmd.LE = 0;

		IccCommand(Slot, &Scmd, &Rcmd);

//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0){
			ptr[0] = Rcmd.SW1;
			ptr[1] = Rcmd.SW2;
			return PSAM_FALSE;
		}

		return PSAM_TRUE;
}

unsigned char  Read_Binary(unsigned char Slot, unsigned char file_ID,unsigned char offset,unsigned char length,unsigned char *ptr)
{
	int i=0;
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;
	
	for (i=0; i<3; i++){		//唉，有时通讯会出错，重试3次。
		Scmd.CLA = 0x00;
		Scmd.INS = 0xb0;
		Scmd.P1 = file_ID|0x80;
		Scmd.P2 = offset;
		Scmd.LC = 0;
		Scmd.LE = length;
		
		IccCommand(Slot, &Scmd, &Rcmd);
		memcpy(ptr,Rcmd.DATA,Rcmd.LE);
		memcpy(ptr+Rcmd.LE, (unsigned char *)&Rcmd.SW1, 2);
		//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0){
			memcpy(ptr,(unsigned char*)&Rcmd.SW1,2);
			usleep(10000);
			continue;
		}
		else{
			break;
		}
	}
	if(i>=3)
		return PSAM_FALSE;
	else
		return PSAM_TRUE;
}

unsigned char  Update_Binary(unsigned char Slot, unsigned char cla,unsigned char file_ID,unsigned char offset,unsigned char length,unsigned char *ptr)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;

		Scmd.CLA = cla;
		Scmd.INS = 0xd6;
		Scmd.P1 = file_ID|0x80;
		Scmd.P2 = offset;
		Scmd.LC = length;
		memcpy(Scmd.DATA,ptr,length);
		Scmd.LE = 0;

		IccCommand(Slot, &Scmd, &Rcmd);

//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0){
			ptr[0] = Rcmd.SW1;
			ptr[1] = Rcmd.SW2;
			return PSAM_FALSE;
		}

		return PSAM_TRUE;
}

unsigned char Get_MifKey(unsigned char Slot, unsigned char p1, unsigned char p2, unsigned char length,unsigned char *ptr)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;

		Scmd.CLA = 0x80;
		Scmd.INS = 0xFC;
		Scmd.P1 = p1;
		Scmd.P2 = p2;
		Scmd.LC = length;
		memcpy(Scmd.DATA,ptr,length);
		Scmd.LE = 0;

		IccCommand(Slot, &Scmd, &Rcmd);

//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0){
			ptr[0] = Rcmd.SW1;
			ptr[1] = Rcmd.SW2;
			return PSAM_FALSE;
		}

		memcpy(ptr,Rcmd.DATA,Rcmd.LE);

		return Rcmd.LE;
}


unsigned char  Read_Record(unsigned char Slot, unsigned char file_ID,unsigned char recordNO,unsigned char length,unsigned char *ptr)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;

		Scmd.CLA = 0x00;
		Scmd.INS = 0xb2;
		Scmd.P1 = recordNO;
		Scmd.P2 = (file_ID<<3)|0x04;
		Scmd.LC = 0;
		Scmd.LE = length;

		IccCommand(Slot, &Scmd, &Rcmd);

//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0){
			ptr[0] = Rcmd.SW1;
			ptr[1] = Rcmd.SW2;
			return PSAM_FALSE;
		}

		memcpy(ptr,&Rcmd.DATA,Rcmd.LE);

		return PSAM_TRUE;
}
////////////////////////////////////////////////////////////////////////
//
//                   更新记录文件
//
////////////////////////////////////////////////////////////////////////
unsigned char  Update_Record(unsigned char Slot, unsigned char file_ID,unsigned char recordNO,unsigned char length,unsigned char *ptr)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;

		Scmd.CLA = 0x00;
		Scmd.INS = 0xdc;
		Scmd.P1 = recordNO;
		Scmd.P2 = (file_ID<<3)|0x04;
		Scmd.LC = length;

		memcpy(Scmd.DATA,ptr,length);

		Scmd.LE = 0x00;

		IccCommand(Slot, &Scmd, &Rcmd);

//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0){
			ptr[0] = Rcmd.SW1;
			ptr[1] = Rcmd.SW2;
			return PSAM_FALSE;
		}

		return PSAM_TRUE;
}
///////////////////////////////////////////////////////////////////////////
//
//                       追加记录文件
//
///////////////////////////////////////////////////////////////////////////
unsigned char Append_Record(unsigned char Slot, unsigned char cla,unsigned char file_ID,unsigned char length,unsigned char *ptr)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;

		Scmd.CLA = cla;
		Scmd.INS = 0xe2;
		Scmd.P1 = 0x00;
		Scmd.P2 = file_ID<<3;
		Scmd.LC = length;

		memcpy(Scmd.DATA,ptr,length);

		Scmd.LE = 0x00;

		IccCommand(Slot, &Scmd, &Rcmd);

//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0)
			return PSAM_FALSE;

		return PSAM_TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//
//                 内部认证
//
/////////////////////////////////////////////////////////////////////////////
unsigned char  Internal_Auth(unsigned char Slot, unsigned char *ptr, unsigned char Keyno)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;

		Scmd.CLA = 0x00;
		Scmd.INS = 0x88;
		Scmd.P1 = 0x00;
		Scmd.P2 = Keyno;
		Scmd.LC = 0x08;
		memcpy(Scmd.DATA,ptr,0x08);

		Scmd.LE = 0x00;

		IccCommand(Slot, &Scmd, &Rcmd);

//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0)
			return PSAM_FALSE;

		return PSAM_TRUE;
}
///////////////////////////////////////////////////////////////
//
//       though  external authen  psam
//
///////////////////////////////////////////////////////////////

unsigned char  External_Auth(unsigned char Slot, unsigned char *ptr, unsigned char Keyno)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;

		Scmd.CLA = 0x00;
		Scmd.INS = 0x82;
		Scmd.P1 = 0x00;
		Scmd.P2 = Keyno;
		Scmd.LC = 0x08;
		memcpy(Scmd.DATA,ptr,0x08);

		Scmd.LE = 0x00;

		IccCommand(Slot, &Scmd, &Rcmd);

//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0){
			memcpy(ptr, (unsigned char*)&Rcmd.SW1, 2);
			return PSAM_FALSE;
		}

		return PSAM_TRUE;
}
   ////////////////////////////////////////////////////////////////////////////
//
//        init_for_descrypt(uchar *cardNO)说明
//             输入: cardNO--指向用户卡物理卡号(4字节)
//             输出: PSAM 卡将利用卡中指定的密钥,产生一个
//                   临时密钥(过程密钥)，临时密钥在PSAM卡
//                  下电后自动消失，不可读。
//
///////////////////////////////////////////////////////////////////////////
unsigned char init_for_descrypt(unsigned char Slot, unsigned char p1,unsigned char p2,unsigned char length,unsigned char *ptr)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;

		Scmd.CLA = 0x80;
		Scmd.INS = 0x1a;
		Scmd.P1 = p1;
		Scmd.P2 = p2;
		Scmd.LC = length;
		memcpy(Scmd.DATA,ptr,length);
		Scmd.LE = 0x00;
		IccCommand(Slot, &Scmd, &Rcmd);

//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0){
			memcpy(ptr, (unsigned char*)&Rcmd.SW1, 2);
			return PSAM_FALSE;
		}

		return PSAM_TRUE;
}
///////////////////////////////////////////////////////////////
//
//       generate key 生成子密钥（过程密钥）
//
//////////////////////////////////////////////////////////////
unsigned char MAC(unsigned char Slot, unsigned char p1,unsigned char p2,unsigned char length,unsigned char *ptr)
{
		ISO7816_ADPU_SEND		Scmd;
		ISO7816_ADPU_RESPONSE	Rcmd;

		Scmd.CLA = 0x80;
		Scmd.INS = 0xfa;
		Scmd.P1 = p1;
		Scmd.P2 = p2;
		Scmd.LC = length;
		memcpy(Scmd.DATA,ptr,length);
		Scmd.LE = 0x00;
		IccCommand(Slot, &Scmd, &Rcmd);

//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0){
			memcpy(ptr, (unsigned char*)&Rcmd.SW1, 2);
			return PSAM_FALSE;
		}

		memcpy(ptr,Rcmd.DATA,Rcmd.LE);

		return PSAM_TRUE;
}


//建设部对CPU卡的认证指令
unsigned char CpuCardCheck(unsigned char slot, unsigned char *pMac, unsigned char *pData, unsigned char len)
{
	ISO7816_ADPU_SEND Scmd;
	ISO7816_ADPU_RESPONSE Rcmd;
	
	Scmd.CLA = 0x80;
	Scmd.INS = 0xca;
	Scmd.P1 = 0x00;
	Scmd.P2 = 0x00;
	Scmd.LC = len;
	
	memcpy(Scmd.DATA, pData, len);
	
	Scmd.LE = 0x00;
#ifdef _debug_CPU_
	debugstring("验证认证码:");
	debugstring("scmd:");
	debugdata(pData, len, 1);
#endif

	IccCommand(slot, &Scmd, &Rcmd);
#ifdef _debug_CPU_
	debugstring("rcmd:");
	debugdata((unsigned char*)&Rcmd, 10, 1);
	debugdata((unsigned char*)&Rcmd.SW1, 2, 1);
#endif
	if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0)// *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	{
		memcpy(pMac, &Rcmd.SW1, 2);
		return ST_ERROR;
	}
	memcpy(pMac, Rcmd.DATA, Rcmd.LE);
	return ST_OK;
}

unsigned char CpuBlackMac(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len)
{
	ISO7816_ADPU_SEND Scmd;
	ISO7816_ADPU_RESPONSE Rcmd;
	
	Scmd.CLA = 0x80;
	Scmd.INS = 0xFA;
	
	Scmd.P1 = 0x05;//0x01;//
	
	Scmd.P2 = 0x00;
	Scmd.LC = len;
	
	//memset(Scmd.DATA, 0, 0x24);
	//Scmd.DATA[10] = 0x05;
	//Scmd.DATA[18] = 0x01;
	
	memcpy(Scmd.DATA, pData, len);
	
	Scmd.LE = 0x00;
#ifdef _debug_
	debugstring("CpuCardSetMac scmd:");
	debugdata(pData, len, 1);
#endif
	IccCommand(PsamID, &Scmd, &Rcmd);
#ifdef _debug_
	debugstring("CpuCardSetMac rcmd:");
	debugdata((unsigned char*)&Rcmd, 10, 1);
	debugdata((unsigned char*)&Rcmd.SW1, 2, 1);
#endif
	if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0)// *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	{
		memcpy(pMac, &Rcmd.SW1, 2);
		return ST_ERROR;
	}
	memcpy(pMac, Rcmd.DATA, Rcmd.LE);
	return ST_OK;
}
unsigned char CpuCardSetMac2(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len)
{
	ISO7816_ADPU_SEND Scmd;
	ISO7816_ADPU_RESPONSE Rcmd;
	
	Scmd.CLA = 0x80;
	Scmd.INS = 0x1A;
	
	Scmd.P1 = 0x45;//0x01;//
	
	Scmd.P2 = 0x02;
	Scmd.LC = len;
	
	//memset(Scmd.DATA, 0, 0x24);
	//Scmd.DATA[10] = 0x05;
	//Scmd.DATA[18] = 0x01;
	
	memcpy(Scmd.DATA, pData, len);
	
	Scmd.LE = 0x00;
#ifdef _debug_
	debugstring("MAC=scmd:");
	debugdata(&Scmd.CLA, len+5, 1);
#endif
	IccCommand(PsamID, &Scmd, &Rcmd);
#ifdef _debug_
	debugstring("CpuCardSetMac rcmd:");
	debugdata((unsigned char*)&Rcmd, 10, 1);
	debugdata((unsigned char*)&Rcmd.SW1, 2, 1);
#endif
	if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0)// *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	{
		memcpy(pMac, &Rcmd.SW1, 2);
		return ST_ERROR;
	}
	memcpy(pMac, Rcmd.DATA, Rcmd.LE);
	return ST_OK;
}

unsigned char CPUMAC2(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len)
{
	ISO7816_ADPU_SEND Scmd;
	ISO7816_ADPU_RESPONSE Rcmd;
	
	Scmd.CLA = 0x80;
	Scmd.INS = 0x72;
	Scmd.P1 = 0x00;
	Scmd.P2 = 0x00;
	Scmd.LC = len;
	
	memcpy(Scmd.DATA, pData, len);
	
	Scmd.LE = 0x00;
#ifdef _debug_CPU_
	debugstring("scmd:");
	debugdata(pData, len, 1);
#endif
	IccCommand(PsamID, &Scmd, &Rcmd);
#ifdef _debug_CPU_
	debugstring("rcmd M2:");
	debugdata((unsigned char*)&Rcmd, 10, 1);
	debugdata((unsigned char*)&Rcmd.SW1, 2, 1);
#endif
	if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0)// *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	{
		memcpy(pMac, &Rcmd.SW1, 2);
		return ST_ERROR;
	}
	memcpy(pMac, Rcmd.DATA, Rcmd.LE);
	return ST_OK;
}

unsigned char CpuCardSetMac(unsigned char PsamID, unsigned char *pMac, unsigned char *pData, unsigned char len)
{
	ISO7816_ADPU_SEND Scmd;
	ISO7816_ADPU_RESPONSE Rcmd;
	
	Scmd.CLA = 0x80;
	Scmd.INS = 0x70;
	
	Scmd.P1 = 0;//0x01;//
	
	Scmd.P2 = 0x00;
	Scmd.LC = len;
	
	//memset(Scmd.DATA, 0, 0x24);
	//Scmd.DATA[10] = 0x05;
	//Scmd.DATA[18] = 0x01;
	
	memcpy(Scmd.DATA, pData, len);
	
	Scmd.LE = 0x00;
#ifdef _debug_
	debugstring("CpuCardSetMac scmd:");
	debugdata(pData, len, 1);
#endif
	IccCommand(PsamID, &Scmd, &Rcmd);
#ifdef _debug_
	debugstring("CpuCardSetMac rcmd:");
	debugdata((unsigned char*)&Rcmd, 10, 1);
	debugdata((unsigned char*)&Rcmd.SW1, 2, 1);
#endif
	if(memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2)!=0)// *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	{
		memcpy(pMac, &Rcmd.SW1, 2);
		return ST_ERROR;
	}
	memcpy(pMac, Rcmd.DATA, Rcmd.LE);
	return ST_OK;
}


















