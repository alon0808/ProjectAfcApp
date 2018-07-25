


//����ԭ��PSAM������IC���������� ����

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


//�ַ���ת������ '9'>=strASC>='0'
unsigned int Ascii2INT(unsigned char *strASC, unsigned char len)
{
	unsigned char i;
	unsigned int t;
	t=0;
	for(i=0;i<len;i++){
		if(strASC[i] == ' ')
			continue;//����ǿո񣬺���
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
// ��λ�ڵ�λ
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

//���ֽ�BCD�����,������Ӻ��BCD��
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
//���ֽ�BCD�����,����BCD��
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
*��Ҫ���ڵ�����ѹ������
*4�ֽڴ�����
*ÿ�ֽڴ��ڵ���0xA0��Ҳ���Ǵ��ֽ�ֵ���ڵ���160��ʱ�����ֽڼ�ȥ0xA0���ASCII���ַ��������Ա�ʾ�����ַ�������ĸ������ô˷�����
*ÿ�ֽ�С��0xA0��������BCD���ʾ�����֡���ֻ�ܱ�ʾ0-9��
*2015.1.10
**************************************/
//�ַ�ת4�ֽڣ�
//strin-����8�ֽ��豸�ţ�����8λǰ�油0
//ZBbuf-����DVlen�ֽڵ�ѹ������豸��
//ZBlen-��Ҫ�����ĳ���,���ܴ���16�ֽ�
//���أ���С����Ч�ֽڣ�������ǰ���00��С��ZBlen������������˵�����Ȳ��ԣ�ZBlen�ֽڱ��治����ô����
unsigned char ASCII2ZBcod(char *strin, unsigned char *ZBbuf, unsigned char ZBlen)
{
	unsigned char bcdNo[16];
	unsigned char i, pos, index;
	
	
	for(index=0; index<8; index++){//�ҷ�0
		if(strin[index] != '0')
			break;
	}
	
	memset(bcdNo, 0, 16);
	memset(ZBbuf, 0, ZBlen);
	pos = 16;
	for(i=index; i<8; i++){
		if(pos > 0)
			pos--;
		else{	//16�ֽ��Ѿ�������,�϶�����
			return 0xFF;
		}
		if((strin[7+index-i] >= '0') && (strin[7+index-i] <= '9')){//
			if((i<7) && ((strin[7+index-i-1] >= '0') && (strin[7+index-i-1] <= '9'))){//ǰ�滹���ַ�,�ж��ǲ������ݣ��ǿ��Ժϳɣ�����ֻ�ܵ�������
				bcdNo[pos] = ((strin[7+index-i-1]<<4)|(strin[7+index-i]&0x0F));
				i++;
			}
			else if(i >= 7){//=7 ǰ��û���ַ��ˣ���λΪ0�����ˡ�
				bcdNo[pos] = strin[7+index-i]&0x0F;
			}
			else{
				bcdNo[pos] = strin[7+index-i] +0xA0;//һ�����֣�����ǰ������ĸ��û�кϳ�
			}
		}
		else if((strin[7+index-i] >= 'A') && (strin[7+index-i] <= 'Z'))
			bcdNo[pos] = strin[7+index-i] +0xA0;
		else//((strin[7-i] >= 'a') && (strin[7-i] <= 'z'))	//�����Ķ���Сд��ĸ����
			bcdNo[pos] = strin[7+index-i]-'a'+'A' +0xA0;
	}
	
	memcpy(ZBbuf, bcdNo+16-ZBlen, ZBlen);	//����ȫ������󷵻���Ҫ���ȵ��ֽ�����
	return (16-pos);	//�����ַ�����ʵ����
}
//��ĸBCD��ת�ַ���
//ZBbuf-����ZBlen�ֽڵ�ѹ������豸��
//strin-����8�ֽ��豸�ţ�����8λǰ�油0
//ZBlen-��Ҫ�����ĳ���,���ܴ���16�ֽ�
//���أ�8������������˵�����Ȳ��ԣ�����ų�����8λ�ַ�
unsigned char ZBcod2ASCII(unsigned char *ZBbuf, unsigned char ZBlen, char *strin)
{
	unsigned char ss[32];
	unsigned char i, pos;
	
	pos = 0;
	for(i=0; i<ZBlen; i++){
		if(ZBbuf[i] >= 0xA0){//��ĸ
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
	else{//if(pos<8){//û��8λ��ǰ�油��0��
		memcpy(strin+(8-pos), ss, pos);
		memset(strin, '0', (8-pos));
	}
	return pos;
}

unsigned int timer0_tick[4];
//���4��,���ö���ms
void set_timer0(unsigned int tick, unsigned char tID)
{
	if(tID >= 4)
		return;

	timer0_tick[tID] = tick;
}

//���4��,���ػ��ж���ms
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
		usleep(1000);	//��ʱ1ms
		for(t=0; t<4; t++){
			if(timer0_tick[t] > 0)
				timer0_tick[t]--;
		}
		
		//----beep---------
		if(beep_enable){//��Ҫ��
			set_beer_b(0);
			beep_enable--;
		}
		else{
			if(beep_times > 1){//����һ�Σ����ǻ���1�����ϡ�
				if(beep_delay){//������ʱ
					set_beer_b(1);
					beep_delay--;
				}
				else{
					beep_times--;
					beep_delay = beep_nNum;
					beep_enable = beep_Ymun;
				}
			}
			else if(beep_times == 1){//�Ѿ�����һ���ˣ��ر�BEEP
				set_beer_b(1);
				beep_times = 0;
			}
		}
		//----beep---------

	}
	return (void*)0;
}

//����7���ֽڵ�BCD��
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
	time_t timer;//time_t����long int ����
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
	////////ʱ�����-----------------------------------------
	char Days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	unsigned int n32_Pass4year;              
	int n32_hpery;    //����ʱ��  
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
	}    //ȡ��ʱ��  
	
//	debugdata((unsigned char*)&sll, 4);
	ii=(int)(sll % 60);    //tm_timew->tm_sec
	tt.tm_sec = ii;
//	debugdata((unsigned char*)&tt.tm_sec, 2);
	sll /= 60;    //ȡ����ʱ��    
//	debugdata((unsigned char*)&sll, 4);
	ii=(int)(sll % 60);    
	tt.tm_min = ii;
//	debugdata((unsigned char*)&tt.tm_min, 4);
	sll /= 60;    //ȡ��ȥ���ٸ����꣬ÿ������ 1461*24 Сʱ    
//	debugdata((unsigned char*)&sll, 4);
//	n32_Pass4year=(unsigned int)(time / (1461L * 24L));    //�������    
	n32_Pass4year=sll / 35064l;//(1461L * 24L);    //�������
//	debugdata((unsigned char*)&n32_Pass4year, 4);
	ii=(int)(n32_Pass4year << 2)+70;    //������ʣ�µ�Сʱ��
	tt.tm_year = ii;
//	debugdata((unsigned char*)&tt.tm_year, 4);    
	sll %= 35064;//1461L * 24L;    //У������Ӱ�����ݣ�����һ����ʣ�µ�Сʱ��
//	debugdata((unsigned char*)&sll, 4);
	for(;;){        //һ���Сʱ��        
		n32_hpery = 8760;//365 * 24;        //�ж�����        
		if ((ii & 3) == 0){            //�����꣬һ�����24Сʱ����һ��            
			n32_hpery += 24;
		}
		if (sll < n32_hpery){
			break;
		}
// 	debugstring("t0~ ");
		ii++;
		sll -= n32_hpery;
	}    //Сʱ��
	tt.tm_year = ii;
//	debugdata((unsigned char*)&n32_hpery, 4);
	ii=(int)(sll % 24);    //һ����ʣ�µ�����
	tt.tm_hour = ii;
//	debugdata((unsigned char*)&tt.tm_hour, 4);
	sll /= 24;    //�ٶ�Ϊ����
	sll++;    //У��������������·ݣ�����
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
	}    //��������
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
*����:
*		ת��һ��tm_slzr�ṹ��ʱ�䵽time_t��ʱ��ֵ
*
*����:
*       tm_slzr *tb - ��Ҫת����ʱ��ָ��
*
*���:
*		����ɹ����򷵻�ָ������ʱ����룺time_t��ʽ��
*		���ɹ�������-1
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
	////////ʱ�����-----------------------------------------
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
//ʱ��Ӻ���
//����BCDʱ�䣺bcdTime   Ҫ�Ӻ��룺addSec
//���BCDʱ�䣺outBCDTime
void TimeAdd(unsigned char*outBCDTime, unsigned char*bcdTime, unsigned int addSec)
{
	time_t ttemp;
	ttemp = BCDTime2Long(bcdTime);
	ttemp += addSec;
	Long2BCDTime(ttemp, outBCDTime);
}

//����ʱ���������λΪ�롣ǰ��ʱ�������ʱ�䡣
int DataTimeDec(unsigned char *datatime1, unsigned char *datatime2)
{
	int l_time1, l_time2;

	l_time1 = BCDTime2Long(datatime1);
	l_time2 = BCDTime2Long(datatime2);

	l_time1 -= l_time2;
	
	return l_time1;
}




//��fileNameָ�����ļ������ж�ȡ��lineNumber��
//����ֵ���ɹ�����1��ʧ�ܷ���0
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
        i++;//��������Ǽ���һ����
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

//��CPU���кţ�һֻ��12�ֽڳ��ȣ���ϵͳ��ֻ����4�ֽڣ����԰���12�ֽڼ�����һ��MACֵ������4�ֽڵ�Ψһ���кš�
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

//��	F4 CPU���кţ�һֻ��12�ֽڳ��ȣ���ϵͳ��ֻ����4�ֽڣ����԰���12�ֽڼ�����һ��MACֵ������4�ֽڵ�Ψһ���кš�
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


//������,����������ʱ��ͣ��ʱ
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

//�ⲿ��������

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
	else if(dat==0x08)//�����
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
	else if(dat==0x0d)//ȷ����
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
	}//  41=F1��  42=F2��
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
	//С���㲻�ܳ�������
	for(i=0;i<6;i++)
	{
		if((Keybuffer[5-i]&0x80)==0x80)
			a++;
	}
	//Ѱ��С�����λ��
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

		if(KeyVa>=2000)//���ܳ���30
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
	if(kkvv==0x0d)	//ȷ����
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
//			saveDeviceParaTab(1, gDeviceParaTab.busPrice);	//������
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




#define Audio_INVALID		2		//�Ƿ���
#define Audio_NOMONEY		3		//����//����
#define Audio_FAMILY		4		//������
#define Audio_Zilu			5		//��Ů��
#define Audio_YWJYu			6		//���鿨  ������  ������������Ž���
#define Audio_STUDENT		7		//ѧ����
#define Audio_STUFF			9		//Ա����
#define Audio_OLDMAN		10		//���˿�
#define Audio_LOVER			11		//���Ŀ�
#define Audio_TuanTi		12		//���忨
#define Audio_Wuxiao		13		//��Ч��
#define Audio_zuanxian		14		//ר�߿� ǩ�����, ���³�
#define Audio_MONTH			15		//��Ʊ��
#define Audio_PuTong		16		//��ͨ��
#define Audio_YH			17		//�Żݿ�
#define Audio_DONG			18		//��
#define Audio_DingDang		19		//���ˣ����ſ�
#define Audio_Di			20		//��
#define Audio_YongJun		21		//ӵ����
#define Audio_SuaKaOK		22		//ˢ���ɹ�
#define Audio_YouDai		23		//�Ŵ���
#define Audio_Siji			24		//˾����
#define Audio_PLUS_MONEY	25		//���ֵ..B=�뼰ʱ��ֵ
#define Audio_BIRTHDAY		26		//���Ͽ�
#define Audio_CY			27		//�ŷ���
#define Audio_FULI			28		//������
#define Audio_GuaShi		29		//��ʧ��,�뽻��
#define Audio_XiaoXueShen	30		//Сѧ����
#define Audio_XueShenYP		31		//ѧ����Ʊ��
#define Audio_YuerDi5		32		//������5Ԫ

#define Audio_no_di			0xee	//û������ֻ�Ƿ�������

#define Audio_TRY_AGAIN		1		//����ˢ 
#define Audio_FLING_MONEY	8		//��Ͷ��

#define voice_invalid		"aplay /mnt/qrdata/wav/invalidcard.wav"		//�Ƿ���
#define voice_studentcard	"aplay /mnt/qrdata/wav/studentcard.wav"		//ѧ����
#define voice_jianhangcard	"aplay /mnt/qrdata/wav/jianhangcard.wav"	//���п�
#define voice_stuffcard		"aplay /mnt/qrdata/wav/stuffcard.wav"		//Ա����
#define voice_jiaotongcard	"aplay /mnt/qrdata/wav/jiaotongcard.wav"	//��ͨ��
#define voice_jinglaocard	"aplay /mnt/qrdata/wav/jinglaocard.wav"		//���Ͽ�
#define voice_liantongcard	"aplay /mnt/qrdata/wav/liantongcard.wav"	//��ͨ��
#define voice_youfucard		"aplay /mnt/qrdata/wav/youfucard.wav"		//�ŷ���
#define voice_monthcard		"aplay /mnt/qrdata/wav/monthcard.wav"		//��Ʊ��
#define voice_youhuicard	"aplay /mnt/qrdata/wav/youhuicard.wav"		//�Żݿ�
#define voice_normalcard	"aplay /mnt/qrdata/wav/normalcard.wav"		//��ͨ��
#define voice_zhuanxiancard "aplay /mnt/qrdata/wav/zhuanxiancard.wav"	//ר�߿�
#define voice_plsgeton		"aplay /mnt/qrdata/wav/plsgeton.wav"		//���ϳ�
#define voice_drivercard	"aplay /mnt/qrdata/wav/drivercard.wav"		//˾����
#define voice_showagain		"aplay /mnt/qrdata/wav/showagain.wav"		//����ˢ
#define voice_shuamashangche "aplay /mnt/qrdata/wav/shuamashangche.wav"	//ˢ��ɹ�,���ϳ�
#define voice_WelCome		"aplay /mnt/qrdata/wav/success.wav"			//��ӭ�˳�
#define voice_chargemonty	"aplay /mnt/qrdata/wav/chargemonty.wav"		//���ֵ
#define voice_flingmoney	"aplay /mnt/qrdata/wav/flingmoney.wav"		//��Ͷ��

//������ʾ
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
	case Audio_zuanxian:		//����ר�߿�  //����ר�߿� 2014.4.8
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
//����ʾ��
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



//��LED��ʾ,4�ַ�
void LED_Dis3(char *str)
{
	Light_main(LED_LIGHT, 0, 0, (char *)str);
}

//-------------IC����غ���---------------
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
			printf("[%s] CmdAuthenticationSetʧ�� \n", __FUNCTION__);
			return 0;
		}
	}
	else
	{
		printf("[%s] CmdLoadkeySetʧ�� \n", __FUNCTION__);
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

	if((src[0] == 0x02) || (src[0] == 0x03)){		//ȥ����ǰ���02 �� 03
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


//--------------������PSAM����غ���------------------

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
	unsigned char curSlot;		//��ǰ�Ŀ�����,1-4
	unsigned int baud[4];		//�����ʣ�����л�����ʱ������Ҳ��һ������Ҫ���л�������,
}stpsamInfo;

stpsamInfo lPsaminfo;


//��Ƭ��λʱ����
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

//��APDUָ��ʱ����
void changePsaminfo(unsigned char Slot)
{
#ifdef _debugPSAM_
	printf("[%s] Slot:%d\r\n", __FUNCTION__, Slot);
#endif
	if((Slot==0) || (Slot>4))
		return;
	if(Slot == lPsaminfo.curSlot) //û�л��������˳�
		return;
	
#ifdef _debugPSAM_
	printf("[%s] curSlot:%d, Slot:%d, curRate:%d, SlotRate:%d\r\n",__FUNCTION__,lPsaminfo.curSlot, Slot, lPsaminfo.baud[lPsaminfo.curSlot-1], lPsaminfo.baud[Slot-1]);
#endif
	if(lPsaminfo.baud[lPsaminfo.curSlot-1] != lPsaminfo.baud[Slot-1]){ //���Ҫ�еĿ���������ʹ�õĿ����Ĳ����ʲ�һ���ģ�Ҫ����
		if(lPsaminfo.baud[Slot-1] == 38400)	
			CmdPSAMbps(1);
		else
			CmdPSAMbps(0);
		usleep(100000);
	}
	lPsaminfo.curSlot = Slot;	//�л�����
	
	
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
	if(ret != 0){	//ͨѶʧ��
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


	if((pTxBuf[1] == pRxBuf[0]) && (pRxBuf[rlen-1] == 0x90)){		//��ʱ����B005600001143390������ݣ�ǰ���INS���������ˣ��������˸�00,���ﴦ����
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
	
	changePsaminfo(Slot);	//�ȿ����Ƿ�Ҫ�л�������

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
	
	for (i=0; i<3; i++){		//������ʱͨѶ���������3�Ρ�
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
//                   ���¼�¼�ļ�
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
//                       ׷�Ӽ�¼�ļ�
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
//                 �ڲ���֤
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
//        init_for_descrypt(uchar *cardNO)˵��
//             ����: cardNO--ָ���û���������(4�ֽ�)
//             ���: PSAM �������ÿ���ָ������Կ,����һ��
//                   ��ʱ��Կ(������Կ)����ʱ��Կ��PSAM��
//                  �µ���Զ���ʧ�����ɶ���
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
//       generate key ��������Կ��������Կ��
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


//���貿��CPU������ָ֤��
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
	debugstring("��֤��֤��:");
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


















