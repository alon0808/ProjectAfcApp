#include "Macro_Proj.h"
#include "RamStorage.h"

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "RecordFile.h"
#include "StructDef.h"
#include "szct.h"
#include "include.h"
#include "ff.h"
#include "Gszct.h"

unsigned int g24GDisFlash = 0;//24G卡界面刷新
unsigned int endTime = 0;
unsigned int GPRSLinkProcess = 0;
unsigned char nnAudioflag = 0;
unsigned char gucRestorFlag = 0;
unsigned char gchn_eng = 'C';//中英文标志
TCP_IP_PACKET1 tcpipPacket;
unsigned int c_serial;
unsigned char irda_rxBuf[MaxPackLen];
unsigned char deal_type;
unsigned int gSendGLogin;//登陆命令
unsigned int nowTime = 0;
unsigned int startTime = 0;
unsigned int endTimeEn = 0;
stGPrsDataStyle gGPRS_data_style;
unsigned char gprsErrorTimes = 0;
unsigned char isNetOK[MAX_RCV_PACKET_NUM];//两路链接是否成功;
unsigned char StufferNO[4];
unsigned short gErrorFlag;//错误标识,高字节表示错误编号，低字节表示错次数据 详见"*错误编码标识定义"
unsigned int ghisCur;
pFistVary_1 pFistVary;
unsigned char restore_flag = 0;
stPricetable NewPriceTab;
int gOverTimes = 0;


void COM_it_enable(unsigned char com) {
	return 0;
}

void delayxs(unsigned int ms) {
	usleep(ms);
	return 0;
}

void flashread(unsigned int addr, unsigned char *rec_data, unsigned int length) {
	FR_flashread(addr, rec_data, length);
}

unsigned char getkey(unsigned char beepE) {
	return 0;
}

void flasherase(unsigned char mode, unsigned int addr) {

}

void SetUSART1mode(unsigned char mode) {

}

extern stDeviceParatable gDeviceParaTab;

static void *getConfigPointer(unsigned int Address_f) {
	void *pConfig = NULL;

	switch (Address_f)
	{
	case BIT_LINENO:
		pConfig = gDeviceParaTab.LineNo;
		break;
	case BIS_BLK_63:
		pConfig = &gDeviceParaTab.unionPayInof.TYPE_63;
		break;
	case BIT_PBOC_NS_BIG:	// 批次号
		pConfig = &gDeviceParaTab.unionPayInof.batchNo;
		break;
	case BIT_PBOC_NS_8583:	// 流水号
		pConfig = &gDeviceParaTab.unionPayInof.sysTrackNo;
		break;
	case BIS_PAY_MODE:
		pConfig = &gDeviceParaTab.unionPayInof.shuangmian;
		break;
	case BIT_repurse_infor:
		pConfig = &gDeviceParaTab.unionPayInof.repurseRecord;
		break;
	case BIT_DEVICE_NO:
		pConfig = gDeviceParaTab.DeviceNo;
		break;
	case BIT_PBOC_NS:
		pConfig = gDeviceParaTab.unionPayInof.gj_pboc_trackNo;
		break;
	default:
		break;
	}

	return pConfig;
}

void sysfewrite(unsigned int Address_f, unsigned int length, unsigned char *Framdata) {

	void *pConfig = getConfigPointer(Address_f);

	if (pConfig == NULL) {
		MSG_LOG("sysfewrite is NULL:%08X\n", Address_f);
		return;
	}
	memcpy(pConfig, Framdata, length);
}

void sysferead(unsigned int Address_f, unsigned int length, unsigned char *rec_data) {

	void *pConfig = getConfigPointer(Address_f);

	if (pConfig == NULL) {
		MSG_LOG("sysfewrite is NULL:%08X\n", Address_f);
		return;
	}
	memcpy(rec_data, pConfig, length);
}


void sysfereadOffset(unsigned int Address_f, unsigned int offsetAddr, unsigned int length, unsigned char *rec_data) {

	void *pConfig = getConfigPointer(Address_f);

	if (pConfig == NULL) {
		MSG_LOG("sysfewrite is NULL:%08X\n", Address_f);
		return;
	}
	memcpy(rec_data, (unsigned char *)pConfig + offsetAddr, length);
}

void feread(unsigned int Address_f, unsigned int length, unsigned char *rec_data) {
	void *pConfig = getConfigPointer(Address_f);

	if (pConfig == NULL) {
		MSG_LOG("sysfewrite is NULL:%08X\n", Address_f);
		return;
	}
	memcpy(rec_data, pConfig, length);
}

void flashwrite(unsigned int addr, unsigned char *writebuf, unsigned int length) {

}

void dis_messgebox(char *istr, int code, unsigned char row) {
	UNUSED_VAR(code);
	UNUSED_VAR(row);
	PRINT_DEBUG("dis_messgebox: %s\n", istr);
	
	MessageBox((unsigned char)0, (const char *)istr);
}


void SetTcpPacketTTLTime(unsigned char val)
{
#ifndef NO_GJ_SERVISE
#ifdef WIFI_TLH_
	tcpipPacket.TTL = 0;
#else
	tcpipPacket.TTL = val;
#endif
	if (tcpipPacket.packetStatus != RECORD_DEAL)
		tcpipPacket.packetStatus = 0;
	//	tcpipClose(0);
#endif	//#ifndef NO_GJ_SERVISE
}

void display_3232(unsigned char x, unsigned char y, const char *str, unsigned char mode) {

}

//下面是文件系统。
int _Fread(void *rbuf, unsigned int rlen, FIL*rfile)
{
	return 0;
}

void StatsMoney(void) {

}

int MenuFrame(char menu[][20], void(*function[])(void), char *titlestr, unsigned char ucMENU_NUM, unsigned int dlywaite) {
	return 0;
}

void dispageInfo_L(unsigned char maxROW, char disbuf[][30]) {

}

void com_init(unsigned char com, unsigned int uibaud) {

}

char PcdRfReset(unsigned char ms) {
	return 0;
}

unsigned char ValidDevNo(unsigned char *dvcbuf) {
	return 1;
}

void clr_wdt(void) {

}

void saveCpuInfo(unsigned char mode, unsigned char *dat) {

}

void getCpuInfo(stCpuInfo *inP) {

}

void tcpipClose(unsigned char mode) {

}

unsigned int __cpuCrc32(unsigned char *val, unsigned int len) {

	return cal_crc32(val, len);
}

void ReStartCPU(void) {

}

void savesysbupara(void) {

}

unsigned char SetupClientIP(unsigned char mode) {

}

void com_snd(unsigned char com, unsigned int uiLen, unsigned char *SendBuf) {

}

int buildSeverInfo_06tls(char cmd, char link, unsigned char mode) {//mode//银联选用那个ip
}

int buildDataSend_0B(unsigned char mode, unsigned int offset, unsigned int Alen, unsigned int datLen, unsigned char *data, unsigned char *obuf, unsigned char *tmb) {

}


