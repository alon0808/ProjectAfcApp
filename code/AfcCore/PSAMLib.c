
//兼容原有PSAM函数。IC卡，及其它 函数
#include "Macro_Proj.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#include "inputmisc/PsamCard.h"

#include "inputmisc/GPIOCtrl.h"


#include "LightColor.h"
#include "GPRS.h"

#include "DemoMain.h"

#include "inputmisc/IcCardCtrlApi.h"

#include "SlzrTypeDef.h"
#include "MYDES.h"
#include "szct.h"

//#define _debugPSAM_

unsigned int timer0_tick[4];
//最多4组,设置多少ms
void set_timer0(unsigned int tick, unsigned char tID)
{
	if (tID >= 4)
		return;

	timer0_tick[tID] = tick;
}

//最多4组,返回还有多少ms
unsigned int get_timer0(unsigned char tID)
{
	return timer0_tick[tID];
}

volatile unsigned int beep_nNum = 0;
volatile unsigned int beep_Ymun = 0;
volatile unsigned int beep_enable = 0;
volatile unsigned char beep_times = 0;
volatile unsigned int beep_delay = 0;

void *onemsSecondDly(void *arg)
{
	unsigned int t;

	while (1) {
		usleep(1000);	//延时1ms
		for (t = 0; t < 4; t++) {
			if (timer0_tick[t] > 0)
				timer0_tick[t]--;

		}

		//----beep---------
		if (beep_enable) {//需要响
			//PRINT_DEBUG("onemsSecondDly:%d\n", beep_enable);
			set_beer_b(0);
			beep_enable--;
		}
		else {
			if (beep_times > 1) {//响了一次，但是还有1次以上。
				if (beep_delay) {//不响延时
					//PRINT_DEBUG("onemsSecondDlyset_beer_b(1);:%d\n", beep_delay);
					set_beer_b(1);
					beep_delay--;
				}
				else {
					beep_times--;
					beep_delay = beep_nNum;
					beep_enable = beep_Ymun;
				}
			}
			else if (beep_times == 1) {//已经响了一次了，关闭BEEP

				//PRINT_DEBUG("onemsSecondDlyset_beer_b(1);:%d\n", beep_times);
				set_beer_b(1);
				beep_times = 0;
			}
		}
		//----beep---------

	}
	return (void*)0;
}



/*      LED        */
#define LED_RED        ((unsigned char)1)
#define LED_GREEN      ((unsigned char)2)
#define LED_BLUE       ((unsigned char)4)
#define LED_YELLOW     ((unsigned char)8)

void led_on(unsigned char mode)
{
	char buf[8] = { 0 };
	int cl;

	if (mode == LED_GREEN)
		cl = SCREEN_G;
	else if (mode == LED_BLUE)
		cl = SCREEN_B;
	else if (mode == LED_YELLOW)
		cl = SCREEN_R;
	else
		cl = SCREEN_R;
	Light_main(SCREEN_LIGHT, LIGHT_OPEN, cl, (char *)buf);
}

void led_off(unsigned char mode)
{
	char buf[8] = { 0 };
	int cl;

	if (mode == LED_GREEN)
		cl = SCREEN_G;
	else if (mode == LED_BLUE)
		cl = SCREEN_B;
	else if (mode == LED_YELLOW)
		cl = SCREEN_R;
	else
		cl = SCREEN_R;

	if (mode == 0xFF) {
		Light_main(SCREEN_LIGHT, LIGHT_CLOSE, SCREEN_G, (char *)buf);
		Light_main(SCREEN_LIGHT, LIGHT_CLOSE, SCREEN_B, (char *)buf);
		Light_main(SCREEN_LIGHT, LIGHT_CLOSE, SCREEN_R, (char *)buf);
	}
	else {
		Light_main(SCREEN_LIGHT, LIGHT_CLOSE, cl, (char *)buf);
	}
}


//蜂鸣器,次数、响延时、停延时
void beep(unsigned char ucTimes, unsigned int ulBeepms, unsigned int ulNoms)
{
	if (beep_times > ucTimes)
		return;
	beep_delay = ulNoms * 3;
	beep_enable = ulBeepms * 3;
	beep_times = ucTimes;
	beep_Ymun = ulBeepms * 3;
	beep_nNum = ulNoms * 3;
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
	unsigned char i = 0;
	unsigned char sbuff[64];

	sbuff[0] = 0x1B;//	SendCommandkey('C','0');
	sbuff[1] = 'C';
	sbuff[2] = '0';
	R485WriteData(sbuff, 3);

	usleep(1000);

	i = 0;
	sbuff[i++] = 0x1B;
	sbuff[i++] = 'S';
	sbuff[i++] = '0';
	memcpy(sbuff + i, ptr, 6);
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
	unsigned char j = 0, dat;

	if (itKEYB_value == 0xFF) {
		return ST_ERROR;
	}
	else
		dat = itKEYB_value;
	if ((dat >= '0') && (dat <= '9'))
	{
		for (j = 0; j < 5; j++)
		{
			if (memcmp(Keybuffer, "\x20\x20\x20\x20\x20", 5) == 0)
				if (Keybuffer[5] == '0')
					Keybuffer[5] = 0x20;
			Keybuffer[j] = Keybuffer[j + 1];
		}
		Keybuffer[5] = dat;
	}
	else if (dat == 0x08)//清除键
	{
		for (j = 0; j < 5; j++)
		{
			Keybuffer[5 - j] = Keybuffer[4 - j];
		}
		if (Keybuffer[0] != 0x20)
			Keybuffer[0] = 0x20;

		if (Keybuffer[5] == 0x20)
		{
			memset(Keybuffer, 0x20, 5);
			Keybuffer[5] = '0';
		}
	}
	else if (dat == 0x0d)//确定键
	{
		if ((Keybuffer[5] & 0x80) == 0x80)
		{
			memset(Keybuffer, 0x20, 5);
			Keybuffer[5] = '0';
		}

	}
	else if (dat == 0x2e)
	{
		Keybuffer[5] |= 0x80;
	}//  41=F1键  42=F2键
	else {
		itKEYB_value = 0xff;
		return ST_ERROR;
	}
	itKEYB_value = 0xff;
	if (KeyMange(dat) == ST_OK) {
		LedDisplay(Keybuffer);
	}
	return ST_OK;
}
void KeyDis(unsigned int value, unsigned char *ptr)
{
	unsigned char i;
	unsigned char buffer1[20];

	memset(buffer1, 0, 20);
	sprintf((char*)buffer1, "%-7u", value);
	for (i = 0; i < 14; i++)
	{
		if (buffer1[i] == 0x20)
			break;
	}

	if (i == 2)
	{
		ptr[0] = '0';
		ptr[0] |= 0x80;
		memcpy(ptr + 1, buffer1, 2);
		ptr[4] = 0x20;
	}
	else if (i == 1)
	{
		ptr[0] = '0';
		ptr[0] |= 0x80;
		ptr[1] = '0';
		ptr[2] = buffer1[0];
		ptr[3] = 0x20;
	}
	else if (i > 2)
	{
		//		i-=2;
		memcpy(ptr, buffer1, i);
		i -= 3;
		ptr[i] |= 0x80;
	}
	memmove(ptr + 3, ptr, 3);
	memset(ptr, 0x20, 3);
}
unsigned char KeyMange(unsigned char kkvv)
{
	unsigned char i = 0, a = 0, j = 0;
	unsigned int ii = 0, KeyVa;
	a = 0;
	//小数点不能超过两个
	for (i = 0; i < 6; i++)
	{
		if ((Keybuffer[5 - i] & 0x80) == 0x80)
			a++;
	}
	//寻找小数点的位置
	for (i = 0; i < 6; i++)
	{
		if ((Keybuffer[5 - i] & 0x80) == 0x80)
			break;
	}
	if (((i > 2) && (i != 6)) || (a > 1))
	{
		beep(2, 50, 50);
		memset(Keybuffer, 0x20, 5);
		Keybuffer[5] = '0';
		KeyVa = 0;
	}
	else
	{
		a = 0;
		KeyVa = 0;
		for (j = 0; j < 6; j++)
		{
			if (Keybuffer[5 - j] != 0x20)
				a++;
		}
		if (i == 1)
		{
			KeyVa = (unsigned int)((Keybuffer[5] - '0') * 10);

		}
		else if (i == 2)
		{
			KeyVa = (unsigned int)((Keybuffer[5] - '0') + ((Keybuffer[4] - '0') * 10));

		}
		else if (i == 6)
		{
			i = 0;
		}
		ii = 10;
		for (j = i; j < a; j++)
		{
			ii = ii * 10;
			KeyVa += (unsigned int)(((Keybuffer[5 - j] - '0') & 0x7f)*ii);
		}

		if (KeyVa >= 2000)//不能超过30
		{
			beep(2, 50, 50);
			memset(Keybuffer, 0x20, 5);
			Keybuffer[5] = '0';
			KeyVa = 0;
			KeyDis(KeyVa, Keybuffer);
			LedDisplay(Keybuffer);
			memset(Keybuffer, 0x20, 5);
			Keybuffer[5] = '0';
			KeyVa = 0;
			return ST_ERROR;
		}
	}
	if (kkvv == 0x0d)	//确定键
	{
		if (KeyVa == 0)
		{
			beep(2, 50, 50);
			memcpy((unsigned char*)&ii, gDeviceParaTab.busPrice, 4);
			KeyDis(ii, Keybuffer);
			LedDisplay(Keybuffer);

			memset(Keybuffer, 0x20, 6);
			Keybuffer[5] = '0';
			return ST_ERROR;
		}
		else
		{

			memcpy(gDeviceParaTab.busPrice, (unsigned char*)&KeyVa, 4);
			//			sysfewrite(BIT_KeyPrice, 4, gDeviceParaTab.busPrice);	////2014.7.17
			//			saveDeviceParaTab(1, gDeviceParaTab.busPrice);	//不保存
			beep(1, 50, 25);
			KeyDis(KeyVa, Keybuffer);
			LedDisplay(Keybuffer);
			memset(Keybuffer, 0x20, 5);
			Keybuffer[5] = '0';
			gBuInfo.restore_flag = 1;

			return ST_ERROR;
		}
	}
	else
	{
		KeyVa = 0;
	}
	return ST_OK;
}

void keyBoardStart(void)
{
	unsigned int ksi;
	keyboard_open();
	memset(Keybuffer, 0x20, 5);
	// 	memset(NewPriceTab.busPrice, 0, 4);
	// 	memset(NewPriceTab.plusPrice, 0, 4);
	// 	Keybuffer[5]='0';
	memcpy((unsigned char*)&ksi, gDeviceParaTab.busPrice, 4);

	KeyDis(ksi, Keybuffer);
	LedDisplay(Keybuffer);
	memset(Keybuffer, 0x20, 5);
	Keybuffer[5] = '0';
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

void soundPlay(char * filename)
{
	char wavfile[128];
	int ret, i;

	i = strlen(filename);
	if ((i <= 0) || (i > 120))
		return;

	strcpy(wavfile, "aplay ");
	strcat(wavfile, filename);

	printf("[%s], i=%d:%s\r\n", __FUNCTION__, i, wavfile);

	ret = system(wavfile);
	printf("[%s], %s,ret=%d\r\n", __FUNCTION__, wavfile, ret);
	usleep(10000);
}

//语音提示
void audio(unsigned char flag)
{
	char wavfile[128];
	int ret, i;

	strcpy(wavfile, "aplay ");
	switch (flag) {
	case Audio_INVALID:
		strcat(wavfile, voice_invalid);
		break;
	case Audio_DONG:
		strcat(wavfile, voice_WelCome);
		break;
	case Audio_STUDENT:
		strcat(wavfile, voice_studentcard);
		break;
	case Audio_BIRTHDAY:
		strcat(wavfile, voice_jinglaocard);
		break;
	case Audio_STUFF:
		strcat(wavfile, voice_stuffcard);
		break;
	case Audio_MONTH:
		strcat(wavfile, voice_monthcard);
		break;
	case Audio_zuanxian:		//邯钢专线卡  //增加专线卡 2014.4.8
		strcat(wavfile, voice_zhuanxiancard);
		break;
	case Audio_FLING_MONEY:
		strcat(wavfile, voice_flingmoney);
		break;
	case Audio_PLUS_MONEY:
		strcat(wavfile, voice_chargemonty);
		break;
	case Audio_TRY_AGAIN:
		strcat(wavfile, voice_showagain);
		break;
	default:
		wavfile[0] = 0;
		break;
	}
	i = strlen(wavfile);

	printf("[%s]1111audio , flag=%d, i=%d:%s\r\n", __FUNCTION__, flag, i, wavfile);

	if (i > 0)
	{
		//printf("[%s]2222audio , %s,ret=%d\r\n", __FUNCTION__, wavfile, ret);
		ret = system(wavfile);
		//ret = system("aplay /mnt/qrdata/wav/flingmoney.wav");
		//ret = system(wavfile);
		//printf("[%s]33334audio , %s,ret=%d\r\n", __FUNCTION__, wavfile, ret);
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
	PRINT_DEBUG("display: %s\n", str);
	MessageBox(0, str);
	//#warning "display need code";
	//printf("%s ", str);
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
	memset(buff, 0, 17);

	BCD2Ascii((unsigned char*)&time->year_h, buff + pos, 1); pos += 2;
	BCD2Ascii((unsigned char*)&time->year_l, buff + pos, 1); pos += 2;
	buff[pos++] = '-';
	BCD2Ascii((unsigned char*)&time->month, buff + pos, 1); pos += 2;
	buff[pos++] = '-';
	BCD2Ascii((unsigned char*)&time->day, buff + pos, 1); pos += 2;
	buff[pos++] = ' ';
	BCD2Ascii((unsigned char*)&time->hours, buff + pos, 1); pos += 2;
	buff[pos++] = ':';
	BCD2Ascii((unsigned char*)&time->minutes, buff + pos, 1); pos += 2;
	buff[pos++] = ':';
	BCD2Ascii((unsigned char*)&time->seconds, buff + pos, 1); pos += 2;
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
	unsigned int ret;
	unsigned char sta;

	ret = CmdWriteSet(cBlockNo, pWrData, 16, &sta);

	if ((ret == MI_OK) && (sta == MI_OK))
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

	ret = CmdLoadkeySet(keys, 6, &status);
	if ((ret == MI_OK) && (status == MI_OK))
	{

		block = ((block / 4 + 1) * 4) - 1;
		if (auth_mode == PICC_AUTHENT1B)
			u32KeyMode = KEYB;
		else
			u32KeyMode = KEYA;

		printf("[%s] start \n", __FUNCTION__);
		ret = CmdAuthenticationSet(u32KeyMode, block, &status);
		printf("[%s] end ret:%d \n", __FUNCTION__, ret);
		if ((ret == MI_OK) && (status == MI_OK))
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
	unsigned int ret, len;
	unsigned char sta;

	ret = CmdReadSet(cBlockNo, pRdData, &len, &sta);
	if ((ret == MI_OK) && (sta == MI_OK))
	{
		return len;
	}
	else
		return 0;
}


unsigned char MifIsPurse(unsigned char *buffer)
{
	unsigned char i;
	for (i = 0; i < 4; i++)
	{
		if ((buffer[i] ^ buffer[4 + i]) != 0xff)
			return 1;
		if (buffer[i] != buffer[8 + i])
			return 1;
	}

	if (buffer[12] != buffer[14])
		return 1;
	if (buffer[13] != buffer[15])
		return 1;
	if ((buffer[12] ^ buffer[13]) != 0xff)
		return 1;
	return 0;
}

unsigned char MiBlockInvalid(unsigned char mode, unsigned char count, unsigned char *ptr)
{
	unsigned char i;
	unsigned char check_sum = 0;
	for (i = 0; i < count; i++)
	{
		check_sum ^= ptr[i];
	}
	if (mode)
	{
		if (check_sum == ptr[count])
			return 0;
		else
			return 1;
	}
	else
		return check_sum;
}

unsigned char GetTypeAUID(unsigned char *pUID)
{
#define _debugPSAM_
	unsigned char buff[64];
	unsigned int len = 0;
	unsigned int ret;

	ret = CmdFindCardSet(buff, &len, buff + 30);
	if (ret == SLZR_SUCCESS) {
#ifdef _debugPSAM_
		printf("[%s] __FindCard OK:", __FUNCTION__);
		debugdata(buff, len, 1);
#endif
		memcpy(pUID, buff, len);

		return len;
	}
	else {
#ifdef _debugPSAM_
		printf("[%s] __FindCard Error:%d\r\n", __FUNCTION__, ret);
#endif
		return 0;
	}

#undef  _debugPSAM_
}

void MakeSureCardLeave(int timeout)
{
	unsigned int count;
	unsigned char result, status;
	unsigned int len;
	unsigned char receive[1024] = { 0 };

	count = 0;
	while (1)
	{
		result = CmdFindCardSet(receive, &len, &status);
		if ((result == MI_OK) && (status == MI_OK))
		{
			count = 0;
			timeout--;
		}
		else
		{

			count++;
		}
		if (count >= 1)
		{
			PRINT_DEBUG("卡片已经移开\n");
			break;
		}
		if (timeout < 0) {
			PRINT_ERROR("等待卡片离开时超时\n");
			break;
		}
		usleep(100000);
	}
}

void MifareHalt(void)
{
	unsigned char ret;

	CmdHaltSet(&ret);
}

unsigned char MifareValueDebit(unsigned char cSubCommand, unsigned char cBlockNo, unsigned char *pValue)
{
	unsigned int ret;
	unsigned char sta;

	ret = CmdM1DecrementSet(cBlockNo, pValue, 4, &sta);

	if (ret == SLZR_SUCCESS)
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
	unsigned int ret, rlen;
	unsigned char sta;

	ret = CmdRatsSet(rbuf, &rlen, &sta);
	if (ret == SLZR_SUCCESS)
		return rlen;
	else
		return 0;
}

unsigned int RfPassthrough(unsigned char *dst, unsigned int iMaxlen, unsigned char *src, unsigned int sendlen)
{
	unsigned int ret, rlen;
	unsigned char sw[8];
	unsigned char sta;

	if ((src[0] == 0x02) || (src[0] == 0x03)) {		//去掉最前面的02 或 03
		memmove(src, src + 1, sendlen - 1);
		sendlen--;
	}

	ret = CmdApduSet(src, sendlen, dst, &rlen, &sta, sw);

	if (ret == SLZR_SUCCESS) {
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

typedef struct {
	unsigned char curSlot;		//当前的卡座号,1-4
	unsigned int baud[4];		//波特率，如果切换卡座时波特率也不一样则需要先切换波特率,
}stpsamInfo;

stpsamInfo lPsaminfo;


//卡片复位时调用
void savePsaminfo(unsigned char Slot, unsigned int uiRate)
{
#ifdef _debugPSAM_
	printf("[%s] Slot:%d, Rate:%d\r\n", __FUNCTION__, Slot, uiRate);
#endif
	if ((Slot == 0) || (Slot > 4))
		return;
	lPsaminfo.curSlot = Slot;
	lPsaminfo.baud[Slot - 1] = uiRate;

	if (uiRate == 38400)
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
	if ((Slot == 0) || (Slot > 4))
		return;
	if (Slot == lPsaminfo.curSlot) //没有换卡座，退出
		return;

#ifdef _debugPSAM_
	printf("[%s] curSlot:%d, Slot:%d, curRate:%d, SlotRate:%d\r\n", __FUNCTION__, lPsaminfo.curSlot, Slot, lPsaminfo.baud[lPsaminfo.curSlot - 1], lPsaminfo.baud[Slot - 1]);
#endif
	if (lPsaminfo.baud[lPsaminfo.curSlot - 1] != lPsaminfo.baud[Slot - 1]) { //如果要切的卡座和正在使用的卡座的波特率不一样的，要处理。
		if (lPsaminfo.baud[Slot - 1] == 38400)
			CmdPSAMbps(1);
		else
			CmdPSAMbps(0);
		usleep(100000);
	}
	lPsaminfo.curSlot = Slot;	//切换卡座


}

unsigned char SamColdReset(unsigned char CardSelect, unsigned int uiRate,
	unsigned char ucVoltage, unsigned char *atr) {

	unsigned int ret, rlen;
	unsigned char sta;


	savePsaminfo(CardSelect, uiRate);


	ret = CmdDetectPsamSet(CardSelect, atr, &rlen, &sta);

	if (ret == 0)
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
	if (ret != 0) {	//通讯失败
		return 0;
	}

#ifdef _debugPSAM_
	debugstring("SA-len:");
	debugdata((unsigned char*)&rlen, 4, 1);
	debugstring("SA-bf:");
	debugdata(pRxBuf, rlen, 1);
#endif

	if ((rlen == 2) && (pRxBuf[0] == 0x61))
	{
		pTxBuf[0] = 0x00;
		pTxBuf[1] = GET_RESPONSE;
		pTxBuf[2] = 0x00;
		pTxBuf[3] = 0x00;
		pTxBuf[4] = pRxBuf[1];

		ret = CmdPsamApduSet(Slot, pTxBuf, 5, pRxBuf, &rlen, &sta);

		if (ret != 0)
			return 0;
	}


	if ((pTxBuf[1] == pRxBuf[0]) && (pRxBuf[rlen - 1] == 0x90)) {		//有时返回B005600001143390这个数据，前面的INS当作数据了，后面少了个00,这里处理下
		memmove(pRxBuf, pRxBuf + 1, rlen);
		pRxBuf[rlen - 1] = 0;
	}

	return rlen;

}

unsigned char IccCommand(unsigned char Slot, ISO7816_ADPU_SEND *ApduSend,
	ISO7816_ADPU_RESPONSE *ApduRecv)
{
	unsigned char sendbuf[300];
	unsigned char revbuf[300];
	unsigned char rt;
	unsigned char len = 0;

	changePsaminfo(Slot);	//先看看是否要切换波特率

	memcpy(sendbuf, (unsigned char*)&ApduSend->CLA, 4);
	len = 4;
	if (ApduSend->LC > 0) {
		sendbuf[len++] = ApduSend->LC;
		memcpy(sendbuf + len, ApduSend->DATA, ApduSend->LC);
		len = len + ApduSend->LC;
	}
	else {
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
	if (rt < 2) {
		ApduRecv->LE = 0;
		ApduRecv->SW1 = 0x00;
		ApduRecv->SW2 = 0x00;
		return 0;
	}

	ApduRecv->LE = rt - 2;
	memcpy(ApduRecv->DATA, revbuf, rt - 2);
	ApduRecv->SW1 = revbuf[rt - 2];
	ApduRecv->SW2 = revbuf[rt - 1];
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
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0)
	{
		memcpy(Receivebuf, (unsigned char*)&Rcmd.SW1, 2);
		return PSAM_FALSE;
	}

	memcpy(Receivebuf, Rcmd.DATA, len);

	return len;
}

unsigned char  Select_FileF(unsigned char Slot, unsigned char *file_ID, unsigned char len)
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
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0)
		return PSAM_FALSE;

	return PSAM_TRUE;
}

unsigned char  Select_File(unsigned char Slot, unsigned int file_ID)
{
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;
	unsigned char i;

	for (i = 0; i < 3; i++) {
		Scmd.CLA = 0x00;
		Scmd.INS = 0xA4;
		Scmd.P1 = 0x00;
		Scmd.P2 = 0x00;
		Scmd.LC = 0x02;
		Scmd.DATA[0] = file_ID / 256;
		Scmd.DATA[1] = file_ID % 256;
		Scmd.LE = 0x00;
		IccCommand(Slot, &Scmd, &Rcmd);
		//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0)
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
	memcpy(Scmd.DATA, ptr, length);
	Scmd.LE = 0;

	IccCommand(Slot, &Scmd, &Rcmd);

	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0) {
		ptr[0] = Rcmd.SW1;
		ptr[1] = Rcmd.SW2;
		return PSAM_FALSE;
	}

	return PSAM_TRUE;


}

unsigned char Change_private_PIN(unsigned char Slot, unsigned char length, unsigned char *ptr)
{
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;

	Scmd.CLA = 0x80;
	Scmd.INS = 0x5e;
	Scmd.P1 = 0x01;
	Scmd.P2 = 0x00;
	Scmd.LC = length;
	memcpy(Scmd.DATA, ptr, length);
	Scmd.LE = 0;

	IccCommand(Slot, &Scmd, &Rcmd);

	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0) {
		ptr[0] = Rcmd.SW1;
		ptr[1] = Rcmd.SW2;
		return PSAM_FALSE;
	}

	return PSAM_TRUE;
}

unsigned char  Read_Binary(unsigned char Slot, unsigned char file_ID, unsigned char offset, unsigned char length, unsigned char *ptr)
{
	int i = 0;
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;

	for (i = 0; i < 3; i++) {		//唉，有时通讯会出错，重试3次。
		Scmd.CLA = 0x00;
		Scmd.INS = 0xb0;
		Scmd.P1 = file_ID | 0x80;
		Scmd.P2 = offset;
		Scmd.LC = 0;
		Scmd.LE = length;

		IccCommand(Slot, &Scmd, &Rcmd);
		memcpy(ptr, Rcmd.DATA, Rcmd.LE);
		memcpy(ptr + Rcmd.LE, (unsigned char *)&Rcmd.SW1, 2);
		//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
		if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0) {
			memcpy(ptr, (unsigned char*)&Rcmd.SW1, 2);
			usleep(10000);
			continue;
		}
		else {
			break;
		}
	}
	if (i >= 3)
		return PSAM_FALSE;
	else
		return PSAM_TRUE;
}

unsigned char  Update_Binary(unsigned char Slot, unsigned char cla, unsigned char file_ID, unsigned char offset, unsigned char length, unsigned char *ptr)
{
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;

	Scmd.CLA = cla;
	Scmd.INS = 0xd6;
	Scmd.P1 = file_ID | 0x80;
	Scmd.P2 = offset;
	Scmd.LC = length;
	memcpy(Scmd.DATA, ptr, length);
	Scmd.LE = 0;

	IccCommand(Slot, &Scmd, &Rcmd);

	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0) {
		ptr[0] = Rcmd.SW1;
		ptr[1] = Rcmd.SW2;
		return PSAM_FALSE;
	}

	return PSAM_TRUE;
}

unsigned char Get_MifKey(unsigned char Slot, unsigned char p1, unsigned char p2, unsigned char length, unsigned char *ptr)
{
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;

	Scmd.CLA = 0x80;
	Scmd.INS = 0xFC;
	Scmd.P1 = p1;
	Scmd.P2 = p2;
	Scmd.LC = length;
	memcpy(Scmd.DATA, ptr, length);
	Scmd.LE = 0;

	IccCommand(Slot, &Scmd, &Rcmd);

	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0) {
		ptr[0] = Rcmd.SW1;
		ptr[1] = Rcmd.SW2;
		return PSAM_FALSE;
	}

	memcpy(ptr, Rcmd.DATA, Rcmd.LE);

	return Rcmd.LE;
}


unsigned char  Read_Record(unsigned char Slot, unsigned char file_ID, unsigned char recordNO, unsigned char length, unsigned char *ptr)
{
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;

	Scmd.CLA = 0x00;
	Scmd.INS = 0xb2;
	Scmd.P1 = recordNO;
	Scmd.P2 = (file_ID << 3) | 0x04;
	Scmd.LC = 0;
	Scmd.LE = length;

	IccCommand(Slot, &Scmd, &Rcmd);

	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0) {
		ptr[0] = Rcmd.SW1;
		ptr[1] = Rcmd.SW2;
		return PSAM_FALSE;
	}

	memcpy(ptr, &Rcmd.DATA, Rcmd.LE);

	return PSAM_TRUE;
}
////////////////////////////////////////////////////////////////////////
//
//                   更新记录文件
//
////////////////////////////////////////////////////////////////////////
unsigned char  Update_Record(unsigned char Slot, unsigned char file_ID, unsigned char recordNO, unsigned char length, unsigned char *ptr)
{
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;

	Scmd.CLA = 0x00;
	Scmd.INS = 0xdc;
	Scmd.P1 = recordNO;
	Scmd.P2 = (file_ID << 3) | 0x04;
	Scmd.LC = length;

	memcpy(Scmd.DATA, ptr, length);

	Scmd.LE = 0x00;

	IccCommand(Slot, &Scmd, &Rcmd);

	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0) {
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
unsigned char Append_Record(unsigned char Slot, unsigned char cla, unsigned char file_ID, unsigned char length, unsigned char *ptr)
{
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;

	Scmd.CLA = cla;
	Scmd.INS = 0xe2;
	Scmd.P1 = 0x00;
	Scmd.P2 = file_ID << 3;
	Scmd.LC = length;

	memcpy(Scmd.DATA, ptr, length);

	Scmd.LE = 0x00;

	IccCommand(Slot, &Scmd, &Rcmd);

	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0)
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
	memcpy(Scmd.DATA, ptr, 0x08);

	Scmd.LE = 0x00;

	IccCommand(Slot, &Scmd, &Rcmd);

	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0)
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
	memcpy(Scmd.DATA, ptr, 0x08);

	Scmd.LE = 0x00;

	IccCommand(Slot, &Scmd, &Rcmd);

	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0) {
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
unsigned char init_for_descrypt(unsigned char Slot, unsigned char p1, unsigned char p2, unsigned char length, unsigned char *ptr)
{
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;

	Scmd.CLA = 0x80;
	Scmd.INS = 0x1a;
	Scmd.P1 = p1;
	Scmd.P2 = p2;
	Scmd.LC = length;
	memcpy(Scmd.DATA, ptr, length);
	Scmd.LE = 0x00;
	IccCommand(Slot, &Scmd, &Rcmd);

	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0) {
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
unsigned char MAC(unsigned char Slot, unsigned char p1, unsigned char p2, unsigned char length, unsigned char *ptr)
{
	ISO7816_ADPU_SEND		Scmd;
	ISO7816_ADPU_RESPONSE	Rcmd;

	Scmd.CLA = 0x80;
	Scmd.INS = 0xfa;
	Scmd.P1 = p1;
	Scmd.P2 = p2;
	Scmd.LC = length;
	memcpy(Scmd.DATA, ptr, length);
	Scmd.LE = 0x00;
	IccCommand(Slot, &Scmd, &Rcmd);

	//		if( *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0) {
		memcpy(ptr, (unsigned char*)&Rcmd.SW1, 2);
		return PSAM_FALSE;
	}

	memcpy(ptr, Rcmd.DATA, Rcmd.LE);

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
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0)// *(unsigned int*)&Rcmd.SW1 != 0x9000 )
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
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0)// *(unsigned int*)&Rcmd.SW1 != 0x9000 )
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
	debugdata(&Scmd.CLA, len + 5, 1);
#endif
	IccCommand(PsamID, &Scmd, &Rcmd);
#ifdef _debug_
	debugstring("CpuCardSetMac rcmd:");
	debugdata((unsigned char*)&Rcmd, 10, 1);
	debugdata((unsigned char*)&Rcmd.SW1, 2, 1);
#endif
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0)// *(unsigned int*)&Rcmd.SW1 != 0x9000 )
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
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0)// *(unsigned int*)&Rcmd.SW1 != 0x9000 )
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
	if (memcmp((unsigned char*)&Rcmd.SW1, "\x90\x00", 2) != 0)// *(unsigned int*)&Rcmd.SW1 != 0x9000 )
	{
		memcpy(pMac, &Rcmd.SW1, 2);
		return ST_ERROR;
	}
	memcpy(pMac, Rcmd.DATA, Rcmd.LE);
	return ST_OK;
}


















