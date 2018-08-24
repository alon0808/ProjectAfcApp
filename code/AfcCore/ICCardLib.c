//����ԭ�еĺ�����������ֲ���롣

#include "Macro_Proj.h"
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>

#include "qPBOC.h"
#include "inputmisc/PsamCard.h"

#include "inputmisc/GPIOCtrl.h"


#include "LightColor.h"
#include "GPRS.h"

#include "DemoMain.h"

#include "inputmisc/IcCardCtrlApi.h"

#include "MYDES.h"
#include "CPUCARJTB.h"
#include "CPUCard.h"
#include "Main_City.h"
#include "SlzrTypeDef.h"
#include "PSAMLib.h"
#include "libsm.h"
#include "RecordFile.h"
#include "GprsSocket.h"
#include "xSys_Lantaiyuan.h"
#include "szct.h"
#include "qpboc_head.h"
#include "UtilityProc.h"
#include "RamStorage.h"

#define _debug_ICcard_

typedef struct {
	Parameter6 ErrorDetail[ERROR_DETAIL_SUM];
	unsigned char error_pointer;		  //��������ָ��
}sterrorcardinfo;

sterrorcardinfo gErrorCard;

typedef struct {
	unsigned char Sec;
	unsigned char CardSno[4];
} card_Delay;
card_Delay PCard_delay;

struct pMonthTime_1 MothDelayTime;


stCardInfo gCardinfo;
//----����ˢ����-----
unsigned char cardSound;	//����ˢ��ʶ
unsigned int card_ser;	//����ˢʱ�Ŀ���������

Parameter8 CARD_PUBLIC;
Parameter8 CARD_PUBLIC1;

fFlagVary_1 gBuInfo;	//�豸״̬���Ƿ�ͣ�����Ƿ�ˢ���ÿ�,�Ƿ�ˢ�½���
oldPursIfo old_Infor;
stDeviceParatable gDeviceParaTab;

stcomtime SysTime;

stPsamInfo psamZJB;		//ס����PSAM
stPsamInfo psamJTB;		//��ͨ��SPAM
sam_pub SamPubInf_ZJB;

unsigned int s_sum1, a_sum1, dis_sum2;
unsigned int a_sumR = 0;//�ֻ�Ǯ���е���ʵ���

void end_card(void);



//�����ļ��������������ͣ�0�豸ϵͳ������1�豸����
int File_read(unsigned char mode)
{
	char fullName[300];
	int i, fd = -1, ret = -1, flag = 0;

	unsigned char *dp;
	unsigned int dLen;

	if (mode == _File_DevicePara_mode) {
		dp = (unsigned char *)&gDeviceParaTab;
		dLen = sizeof(stDeviceParatable);
		strcpy(fullName, WorkDir);
		strcat(fullName, _File_DevicePara);
	}
	else if (mode == _File_BuInfo_mode) {
		PRINT_DEBUG("size of fFlagVary_1\n", sizeof(fFlagVary_1));
		dp = (unsigned char *)&gBuInfo;
		dLen = sizeof(fFlagVary_1);
		strcpy(fullName, WorkDir);
		strcat(fullName, _File_BuInfo);
	}
	else {
		return -1;
	}
	ret = access(fullName, F_OK);

	printf("[%s]access file %s ret:%d dLen:%d\n", __FUNCTION__, fullName, ret, dLen);

	if (ret >= 0) {
		fd = open(fullName, O_RDONLY);
		if (fd >= 0) {
			ret = read(fd, dp, dLen);
			if (ret >= 0) {
				printf("[%s]read file %s ret:%d dLen:%d\n", __FUNCTION__, fullName, ret, dLen);
				if (ret != dLen) {
					PRINT_INFOR_LOCATION("�ļ����Ȳ�ƥ��", fullName);
					if (ret < dLen) {
						if (fd >= 0)
							close(fd);
						fd = open(fullName, O_WRONLY);
						memset(dp + ret, 0, dLen - ret);
					}
				}
			}
			else {
				printf("read %s fail!\n", fullName);
				perror("read");
				flag = 1;
			}
		}
		else
		{
			printf("open %s fail!\n", fullName);
			perror("open");
			flag = 2;
		}

	}
	else {
		PRINT_INFOR_LOCATION("�ļ�������,�½�...", fullName);
		memset(dp, 0, dLen);
	}
	if (ret < 0) {
		if (mode == _File_DevicePara_mode) {
			//���ԣ�ֱ�Ӹ���ʼֵ
			memcpy(gDeviceParaTab.busPrice, "\x64\x00\x00\x00", 4);
			memcpy(gDeviceParaTab.plusPrice, "\x00\x00\x00\x00", 4);
			for (i = 0; i < 32; i++) {
				gDeviceParaTab.rate[i] = 1;
				gDeviceParaTab.plusrate[i] = 0;
			}
			memcpy(gDeviceParaTab.DeviceNo, "000000010", 9);
			memcpy(gDeviceParaTab.LineNo, "\x00\x01\x00", 3);
			memcpy(gDeviceParaTab.BussinessNo, CLOUD_BUSSINESSNO, 2);
			gDeviceParaTab.catalogPOST = 0;

			memset((unsigned char*)&gDeviceParaTab.gServerInfo, 0, sizeof(gDeviceParaTab.gServerInfo));
			strcpy(gDeviceParaTab.gServerInfo[0].APN, "CMNET");
			strcpy(gDeviceParaTab.gServerInfo[0].IPaddr, "139.199.213.63");			////139.199.213.63:2020 ���ԡ�
			gDeviceParaTab.gServerInfo[0].port = 2020;
		}
		else if (mode == _File_BuInfo_mode) {
			gBuInfo.stop_flag = 1;
		}
		if (fd >= 0)
			close(fd);
		fd = open(fullName, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);	//д�����������ļ�
	}
	
	if (ret < dLen) {	//�ļ������� ��ǰ�½�һ��
		PRINT_DEBUG("���������޸��ļ�:%d, %d", ret, dLen);
		if (fd >= 0)
		{
			ret = write(fd, dp, dLen);//д��ԭʼ�ļ������ļ���=3�ֽڱ�ʶ_4�ֽڰ汾��
			if (ret < 0) {
				printf("[%s] %sд���ļ���:%d@!\r\n", __FUNCTION__, fullName, ret);

				flag = 3;
			}
		}
		else {
			printf("[%s] %s���� �ļ���@!\r\n", __FUNCTION__, fullName);

			flag = 4;
		}

	}

	if (fd >= 0) {
		close(fd);
	}

#if SWITCH_TEST_SHUAKA
	if (mode == _File_DevicePara_mode) {
		PRINT_DEBUG("IP:%s,port:%d", gDeviceParaTab.gServerInfo[0].IPaddr, gDeviceParaTab.gServerInfo[0].port);
	}
	else if (mode == _File_BuInfo_mode) {
		PRINT_DEBUG_LOCATION("����ǿ��Ϊ����ˢ��", "");
		gBuInfo.stop_flag = 0;
	}
	
#endif

	return flag;
}

//��������ļ�
//�����ļ��������������ͣ�0�豸ϵͳ������1�豸����
int File_save(unsigned char mode)
{
	char fullName[300];
	int fd = -1, ret = -1, flag = 0;

	unsigned char *dp;
	unsigned int dLen;

	if (mode == _File_DevicePara_mode) {
		dp = (unsigned char *)&gDeviceParaTab;
		dLen = sizeof(stDeviceParatable);
		strcpy(fullName, WorkDir);
		strcat(fullName, _File_DevicePara);
	}
	else if (mode == _File_BuInfo_mode) {
		dp = (unsigned char *)&gBuInfo;
		dLen = sizeof(fFlagVary_1);
		strcpy(fullName, WorkDir);
		strcat(fullName, _File_BuInfo);
	}
	else {
		return -1;
	}


	printf("[%s]access file %s ret:%d\n", __FUNCTION__, fullName, dLen);


	fd = open(fullName, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);	//д�����������ļ�
	if (fd >= 0)
	{
		ret = write(fd, dp, dLen);//д��ԭʼ�ļ������ļ���=3�ֽڱ�ʶ_4�ֽڰ汾��
		if (ret < 0) {
			printf("[%s] %sд���ļ���@!ret:%d\r\n", __FUNCTION__, fullName, ret);

			flag = 3;
		}
	}
	else {
		printf("[%s] %s���� �ļ���@! fd:%d\r\n", __FUNCTION__, fullName, fd);

		flag = 4;
	}

	if (fd >= 0)
		close(fd);

	return flag;
}


int  save_file_MothDelayTime(void)
{
	char filename[128];
	int fd, ret = -1;

	strcpy(filename, WorkDir);
	strcat(filename, _File_MothDelayTime);

	fd = file_open_creat(filename);
	if (fd >= 0) {
		ret = write(fd, (unsigned char *)&MothDelayTime, sizeof(pMonthTime_1));
	}
	close(fd);

	return ret;
}

int get_file_MonthDelayTime(void)
{
	char filename[128];

	strcpy(filename, WorkDir);
	strcat(filename, _File_MothDelayTime);
	return GetFileDatac(filename, 0, sizeof(pMonthTime_1), (unsigned char *)&MothDelayTime); //�������ص���Ϣ��������
}

int  save_file_ErrorCard(void)
{
	char filename[128];
	int fd, ret = -1;

	strcpy(filename, WorkDir);
	strcat(filename, _File_ErrorCard);

	fd = file_open_creat(filename);
	if (fd >= 0) {
		ret = write(fd, (unsigned char *)&gErrorCard, sizeof(gErrorCard));
	}
	close(fd);

	return ret;
}

int get_file_ErrorCard(void)
{
	char filename[128];

	strcpy(filename, WorkDir);
	strcat(filename, _File_ErrorCard);
	return GetFileDatac(filename, 0, sizeof(gErrorCard), (unsigned char *)&gErrorCard); //�������ص���Ϣ��������
}

int  save_file_BuInfo(void)
{
	return File_save(_File_BuInfo_mode);
}

int get_file_BuInfo(void)
{
	return File_read(_File_BuInfo_mode);

}

int  save_file_DevicePara(void)
{
	return File_save(_File_DevicePara_mode);
}

int get_file_DevicePara(void)
{
	int ret;

	ret = File_read(_File_DevicePara_mode);
	if (ret != 0) {	//�����ļ����󣬲�Ӧ��ˢ���˰ɡ�
		printf("[%s] %s�����ļ���������\r\n", __FUNCTION__, _File_DevicePara);
		gBuInfo.stop_flag = 1;	//��ͣˢ����
	}
	return ret;
}

void SYSgetbussinessNO(unsigned char *obuf)
{
	memcpy(obuf, CLOUD_BUSSINESSNO, 2);
}

void DisRetry(void)
{
	static unsigned int ucFrePresentC;
	unsigned int tt;

	if ((cardSound == 0xaa) || (cardSound == 0xbb)) {//aaǮ������ bb��Ʊ���� 
		tt = 1;

		ucFrePresentC++;
		if (ucFrePresentC == tt)
		{
			cls();
			beep(3, 20, 10);
			display(3, 3, " �� �� ˢ ", DIS_CENTER);
		}
		else if (ucFrePresentC >= (tt * 2))
		{
			cls();
			ucFrePresentC = 0;
			beep(3, 20, 10);
			display(3, 3, " �� �� ˢ ", DIS_CENTER | DIS_CONVERT);
		}

	}

}




#define KEYBOARD_INPUT 0x55
unsigned char KeyBoardStyle;	//�����Ƿ������룬0x55-�м������룬����-û�м������롣û�м�������ʱʹ�û���Ʊ�ۣ��м�������ʱʹ������Ʊ��


#ifdef KEYBOARD
//�ⲿ��������
void *main_ExKeyBoard(void *arg)
{
	unsigned char rkbuf[32];
	unsigned int i;

	while (1) {
		usleep(50000);

		if (R485ReadData(rkbuf, &i) == 0)
		{
			KeyBoardDeal(rkbuf[0]);
			if (KeyDeal() == ST_OK) {
				if (KeyBoardStyle != KEYBOARD_INPUT) {//Ʊ����Ҫ���¶�ȡ
					KeyBoardStyle = KEYBOARD_INPUT;//2014.7.17

					gBuInfo.restore_flag = 3;
					keyBoardStart();
				}
				KeyBoardStyle = KEYBOARD_INPUT;
			}
		}
	}
	return (void *)0;
}
#endif

//IC������ʱ�ı���������ʼ��
void ICCardInit(void)
{
	printf("[%s] sizeof stDeviceParatable=%d[B]\r\n", __FUNCTION__, sizeof(stDeviceParatable));

	get_file_BuInfo();

	get_file_DevicePara();

#ifdef KEYBOARD
	KeyBoardStyle = 0;
	keyBoardStart();
#endif

	Get_SerialNum(gDeviceParaTab.SN);

	initRecordFilePara();
	GPRSSocketParaINIT();
	get_file_MonthDelayTime();
	get_file_ErrorCard();

	//-----------����SM2�⺯��-----------------------
	// 	unsigned char buff[500],buff1[500],buff2[500];
	// 	char ID[16];
	// 	char buftmp[500];
	// 	unsigned int plen, dlen;
	// 
	// 	struct timeval tv1, tv2;
	// 	struct timezone tz1;
	// 	int kkk;
	// 	
	// 	kkk = gettimeofday(&tv1, &tz1);
	// 
	// 	strcpy(buftmp, "02F6B3C5AAC6A3FDA28310339569BCAD8D8C21B0CFF70A5764233F3AE3000916B7");
	// 
	// 	Ascii2BCD((unsigned char*)buftmp, buff, strlen(buftmp));
	// 
	// 	ret = ECPointDecompress(buff1, buff);
	// 	
	// 	kkk = gettimeofday(&tv2, &tz1);
	// 
	// 	printf("timedly1 = %dus\r\n", (tv2.tv_usec - tv1.tv_usec));
	// 
	// 	debugstring(buftmp);
	// 	debugdata(buff, 50, 1);
	// 	debugdata(buff1, 50, 1);
	// 
	// 	strcpy(ID, "1234567812345678");
	// 
	// 	strcpy(buftmp, "0F4C16B3B2CA00E7F0455359C2A85B698BBCCED26F87EEA6523C9CDC4E42ABC08A3F2761A6DA4C6E9F749F882A8A18267546D488F3F36E1A7686C132704C2512");
	// 	Ascii2BCD((unsigned char*)buftmp, buff, strlen(buftmp));//��Կ
	// 	plen = strlen(buftmp)/2;
	// 	
	// 	strcpy(buftmp, "1201592610124000005604040021033C63C7123D19F16496AD02B86C3A43697D0DD8D7AFDB9492D540062B79DD1CBA");
	// 	Ascii2BCD((unsigned char*)buftmp, buff1, strlen(buftmp));//���Ĺ�Կ��ǩ����
	// 	dlen = strlen(buftmp)/2;
	// 
	// 	strcpy(buftmp, "E3F52D2756C71E10AD4BFB9E2F81E4938DF18AFB1A1D9A4FBB1F3E28C60BADCA5A0C3C99774692A7A937F8258BB52AF166F2A08F15E3282943844C717C2FB27E");
	// 	Ascii2BCD((unsigned char*)buftmp, buff2, strlen(buftmp));//ǩ��ֵ
	// 	
	// 	kkk = gettimeofday(&tv2, &tz1);
	// 
	// 
	// 	ret = PBOC_SM2_Verify(ID, buff, plen, buff1, dlen, buff2);
	// 
	// 	
	// 	kkk = gettimeofday(&tv1, &tz1);
	// 	
	// 	printf("timedly PBOC_SM2_Verify = %dus\r\n", (tv1.tv_usec - tv2.tv_usec)); //timedly PBOC_SM2_Verify = 30575us
	// 
	// 	debugstring(buftmp);
	// 	debugdata(buff, plen, 1);
	// 	debugdata(buff1, dlen, 1);
	// 	debugdata(buff2, 64, 1);
	// 
	// 	printf("ret=%d\r\n", ret);
	// 
	// 	sleep(5);

}

unsigned char BUflag = 0;
void restore_disp(void)
{
	unsigned char pos = 0;
	unsigned char buffer[50];
	unsigned int mmoney;
	//	unsigned int imm;
	unsigned short ii = 0;

	ii = ii;

	if ((gBuInfo.restore_flag) != 0) {
		//led_off(0xFF);	//ȫ��
		led_on(LED_BLUE);
		Light_main(QR_LIGHT, LIGHT_OPEN, QR_W, (char *)buffer);

		gBuInfo.restore_flag = 0;
		//DisplayCls(0);
		pos = 0;
		memset(buffer, 0, 50);

		memcpy(buffer, (unsigned char *)"��·:", 5);
		pos = 5;

		BCD2Ascii(gDeviceParaTab.LineNo, buffer + pos, 2);
		pos += 4;
		buffer[pos] = '-';
		pos += 1;

		BCD2Ascii(gDeviceParaTab.LineNo + 2, buffer + pos, 1);
		pos += 2;
		if (gBuInfo.stop_flag)
			display(3, 1, (char*)buffer, DIS_Cls);

		else
			display(2, 1, (char*)buffer, DIS_Cls);


		memset(buffer, 0, 50);
		strcpy((char*)buffer, SYS_HEAD_STR);

#if defined _debug_ || defined _debug_blk || defined _debug_price || defined debug_GJ_TLVDeal_||defined JT_TES
		strcpy((char*)buffer, "���Գ���");

#endif

		memcpy((unsigned char*)&ii, gBusVerInfo.busProVer, 2);
		sprintf((char*)buffer + 30, " V%d.%02d", (ii / 100), (ii % 100));
		strcat((char*)buffer, (char*)(buffer + 30));

#ifdef _DriverKey_signin_
		if (BUflag != 0) {
			display(0, 0, "��ˢ˾����", DIS_CONVERT | DIS_CENTER);
		}
		else
#endif
			display(0, 0, (char*)buffer, DIS_CONVERT | DIS_CENTER);


		// 		if(gBusDownPro.MISS_flag == 0xEA){
		// 			display(0,0,"                ",0);
		// 			if(gchn_eng == 'E'){
		// 				display(0,0,"program is down",DIS_CENTER|DIS_CONVERT);
		// 			}
		// 			else
		// 				display(0,0,"���³���",DIS_CENTER|DIS_CONVERT);
		// 		}
		if (gBuInfo.stop_flag)
		{
			memset(buffer, 0, 50);
			display(5, 4, "��ͣ��", 0);

			strcpy((char*)buffer, "------");
			LED_Dis3((char*)buffer);

		}
		else
		{
			//			imm = 0;
			memset(buffer, 0, 50);
			memcpy((unsigned char*)&mmoney, gDeviceParaTab.busPrice, 4);
			// 			if(F_FLAG_VARY.PlusSta){
			// 				memcpy((unsigned char*)&imm, NewPriceTab.plusPrice, 4);
			// 			}
			// 			mmoney += imm;
			sprintf((char*)buffer, "Ʊ��:%d.%02dԪ", mmoney / 100, mmoney % 100);
			display(4, 1, (char*)buffer, 0);
#ifdef KEYBOARD
			sprintf((char*)buffer, "%d.%02d ", mmoney / 100, mmoney % 100);
			LED_Dis3((char*)buffer);
			if (mmoney == 0)
				display(4, 1, " ������Ʊ��!    ", 0);
#else
			sprintf((char*)buffer, "%d.%02d ", mmoney / 100, mmoney % 100);
			LED_Dis3((char*)buffer);
#endif
		}

#ifdef _debug_
		printf("�豸��DeviceNo:%s\r\n", (char*)gDeviceParaTab.DeviceNo);
#endif


	}
}

void error(unsigned char code1, unsigned char Index)
{
	char disp[40];

	memset(disp, 0, 32);

	switch (code1)
	{
	case ERR_NO_PSAM:
		strcpy(disp, "PSAM��������!");
		break;
	case ERR_INIT_PSAM:
		strcpy(disp, "��ʼ��PSAM������!");
		break;
	case ERR_READ_TIME:
		strcpy(disp, "ϵͳʱ�䲻��ȷ!");
		break;
	case ERR_CARD_STOP_WORK:
		strcpy(disp, "�ÿ��Ѿ���ͣ��!");
		break;
	case ERR_CARD_NO_SYSTEM:
		strcpy(disp, "�ÿ����Ǳ�ϵͳ�����Ŀ�!");
		break;
	case ERR_CARD_WHITE_BUS:
		strcpy(disp, "�ÿ�����50����δ���ۿ�!");
		break;
	case ERR_CARD_NO_USE:
		strcpy(disp, "�ÿ�δ����!");
		break;
	case ERR_MONEY_SN:
		strcpy(disp, "Ʊ�����Ƿ�!");
		break;
	case ERR_CHARGE_MEONY:
		strcpy(disp, "����!");
		break;
	case ERR_CARD_PUBLIC:
		strcpy(disp, "��������������!");
		break;
	case ERR_MONTH_BAD:
		strcpy(disp, "��Ʊ����Ǯ����ʽ����!");
		break;
	default:
		sprintf(disp, "����:%d ", code1);
		return;
	}

	if (strlen(disp) > 0) {
		MessageBox(1, disp);
	}
	gBuInfo.restore_flag = 1;
	end_card();
}

void saveDeviceParaTab(unsigned char mode, unsigned char *dat)
{
	unsigned int Crc32;

	switch (mode) {
	case 1:	//������·��Ʊ����Ϣ���Ѿ���ֵ�������ṹ���ˡ�	

		break;
	case 6:	//�����ƿ���Կ
		memcpy(gDeviceParaTab.UserKeyA, dat, 16);
		Crc32 = GenerateCRC32(dat, 16);
		memcpy(gDeviceParaTab.UserKeyACrc32, (unsigned char*)&Crc32, 4);
		memcpy(gDeviceParaTab.UserKeyB, dat + 16, 16);
		Crc32 = GenerateCRC32(dat + 16, 16);
		memcpy(gDeviceParaTab.UserKeyBCrc32, (unsigned char*)&Crc32, 4);
		break;
	case 7://�̻����
		memcpy(gDeviceParaTab.BussinessNo, dat, 2);
		gDeviceParaTab.catalogPOST = 0;
		Crc32 = GenerateCRC32(dat, 2);
		memcpy(gDeviceParaTab.BussinessNo + 2, (unsigned char*)&Crc32, 4);
		break;
	case 2:		//�����豸��
		memcpy(gDeviceParaTab.DeviceNo, dat, 8);
		break;
	case 3:
		printf(gDeviceParaTab.gServerInfo[0].IPaddr, "%d.%d.%d.%d", dat[0], dat[1], dat[2], dat[3]); //memcpy(gDeviceParaTab.gServerInfo[0].IPaddr, dat, 4);
		gDeviceParaTab.gServerInfo[0].port = 0;
		memcpy((unsigned char*)&gDeviceParaTab.gServerInfo[0].port, dat + 4, 2);
		break;
	case 9://stopflag
		gDeviceParaTab.stopflag = dat[0];
		break;
	case 8://����APN
		break;
	default:
		return;
	}

#ifdef _debug_
	debugstring("save cpu ROM:");
	debugdata((unsigned char*)&gDeviceParaTab, sizeof(gDeviceParaTab), 1);
#endif
	save_file_DevicePara();
}


unsigned char GetDateTime(void)
{
	unsigned char i = 0, ii;
	unsigned char buf[32];


	get_datatime((char*)buf);

#ifdef _debug_ICcard_
	printf("[%s]XXX:%s\r\n", __FUNCTION__, (char*)buf);
#endif

	Ascii2BCD(buf, (unsigned char *)&SysTime, 14);

#ifdef _debug_ICcard_
	printf("[%s]:%s\r\n", __FUNCTION__, (char*)buf);
#endif

	memcpy(buf, (unsigned char*)&SysTime, 7);
	if (time_invalid(buf) == 0)
		return ST_ERROR;

	for (ii = 0; ii < 7; ii++) {
		if ((buf[i] & 0x0f) > 0x09)
			break;
		if (((buf[i] >> 4) & 0x0f) > 0x09)
			break;
	}
	if (ii < 7)
		return ST_ERROR;
	else
		return ST_OK;

}


unsigned char profile(unsigned char mode)
{
	unsigned char buff[64];
	unsigned int len;
	unsigned int ret;

	ret = CmdFindCardSet(buff, &len, buff + 30);
	if (ret == SLZR_SUCCESS) {
#ifdef _debug_ICcard_
		printf("[%s] __FindCard OK:", __FUNCTION__);
		debugdata(buff, len, 1);
#endif
		memcpy((unsigned char*)&gCardinfo.c_serial, buff, 4);

		if (buff[4] & 0x20)
			return 0x20;
		else
			return ST_OK;

	}
	else {
		// #ifdef _debug_ICcard_
		// 		printf("[%s] __FindCard Error:%d\r\n", __FUNCTION__, ret);
		// #endif
		return ST_ERROR;
	}

}

void end_card(void)
{
	unsigned int i;
	unsigned int sno1;
	unsigned char buffer[10];
	//	unsigned char key;

	//	debugstring("int-------------\r\n");
	// 	PcdRfReset(5);
	// 	delayxms(10);
	// 	debugstring("444");


	sno1 = gCardinfo.c_serial;
	// 	printf("[%s] c_serial:", __FUNCTION__);
	// 	debugdata((unsigned char *)&gCardinfo.c_serial,4,1);
	i = 0;
	do {
		i++;

		usleep(10000);//delayxms(10);

					  //delayxms(5);
		if (GetTypeAUID(buffer) != 0)
		{
			// 			debugstring("#");
			// 			debugdata((unsigned char *)&sno1,4,1);
			// 			debugdata((unsigned char *)&buffer,4,1);

			//			delayxms(2);		//����Ҫ��ʱ���� ͦ��֣�������ʱ�Ժ�ȷʵ������
			if (memcmp((unsigned char *)&sno1, buffer, 4) == 0)
			{
				i = 0;
			}
		}
		// 		else{
		// 			PcdRfReset(5);
		// 			
		// 		}
		// 		key = getkey(0);
		// 		if((key == KEY_ESC)||(key == KEYP_ESC)||(key == 0x0b)){
		// 			break;
		// 		}
	} while (i < 3);
	// 	debugstring("end i:");
	// 	debugdata((unsigned char *)&i,1,1);

	//	printf("[%s] END :%04X\r\n", __FUNCTION__, i);
	MifareHalt();
	return;
}

//���غ��������ڵ�λ��ID,����0����������
unsigned int IsInBlackList(unsigned char *sno)
{
	char filename[128];
	unsigned char rtmp[16];
	int fd, res, length;
	struct stat stbuf;
	unsigned int i_end, i_start, i;
	unsigned int i_cur, addr;

	strcpy(filename, WorkDir);
	strcat(filename, _File_BlackList_bus);

	MSG_LOG("[%s] sno:", __FUNCTION__);
	BCD_LOG(sno, 8, 1);

	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		MSG_LOG("[%s] OPEN FILES[%s] fail!\r\n", __FUNCTION__, filename);
		perror(__FUNCTION__);
		return 0;
	}

	if ((fstat(fd, &stbuf) != 0) || (!S_ISREG(stbuf.st_mode))) {
		/* Handle error */
		MSG_LOG("[%s] fstat fail!\r\n", __FUNCTION__);
		perror(__FUNCTION__);
		close(fd);
		return 0;
	}

	length = stbuf.st_size;

	MSG_LOG("[%s] file length:%d\r\n", __FUNCTION__, length);
	length -= 10;	//ǰ����10�ֽڲ��Ǻ�����
	length /= BLK_SNO_LEN;	//����������

	MSG_LOG("[%s] blk num:%d\r\n", __FUNCTION__, length);

	i_start = 0;
	i_end = length - 1;

	while (1) {
		i_cur = i_start + (i_end - i_start) / 2;

		addr = 10 + (i_cur*BLK_SNO_LEN);

		lseek(fd, addr, SEEK_SET);

		res = read(fd, rtmp, BLK_SNO_LEN);
		if (res < 0) {
			MSG_LOG("[%s] Read FILES[%s] fail!\r\n", __FUNCTION__, filename);
			perror(__FUNCTION__);
			break;
		}

		MSG_LOG("[%s] rtmp:", __FUNCTION__);
		BCD_LOG(rtmp, 10, 1);

		for (i = 0; i < BLK_SNO_LEN - 2; i++)	//������ǰ8�ֽ���Ч�������ֱ���
		{
			if (rtmp[i] == sno[i]) {
#ifdef _debug_
				debugstring("Y");
#endif
				continue;
			}
			else if (sno[i] > rtmp[i])
			{
				if (i_end == i_cur) {
					close(fd);
					return 0;
				}
				else
				{
					i_start = i_cur + 1;
					break;
				}
			}
			else
			{
				if (i_start == i_cur) {
					close(fd);
					return 0;
				}
				else
				{
					i_end = i_cur;
					break;
				}
			}
		}
		if (i == BLK_SNO_LEN) {
			close(fd);
			return addr;
		}
	}


	close(fd);
	return 0;
}


void setAudioHANDAN(void)//����
{
	//	0 ��ͨ��	1 ѧ����	2 ���꿨	3 Ա����	5 ��������Ʊ��   6 ��ͨB��
	memset(gDeviceParaTab.voice, Audio_DONG, 32);
	gDeviceParaTab.voice[CARD_NORMAL] = Audio_DONG;
	gDeviceParaTab.voice[CARD_STUDENT] = Audio_STUDENT;
	gDeviceParaTab.voice[CARD_OLDMAN] = Audio_BIRTHDAY;
	gDeviceParaTab.voice[CARD_STUFF] = Audio_STUFF;
	gDeviceParaTab.voice[CARD_COUPON] = Audio_MONTH;
	gDeviceParaTab.voice[CARD_RELATION_BUS] = Audio_MONTH;
	gDeviceParaTab.voice[CARD_HANDICAPPED] = Audio_DONG;
	gDeviceParaTab.voice[CARD_CAP_BUS] = Audio_DONG;		//��ͨ�ֻ���
	gDeviceParaTab.voice[CARD_JILIAN_BUS] = Audio_DONG;		//���п�
	gDeviceParaTab.voice[CARD_NORMAL_B] = Audio_zuanxian;		//����ר�߿�  //����ר�߿� 2014.4.8
	gDeviceParaTab.voice[CARD_YOUFU_BUS] = Audio_DONG;		//�����Ÿ����� �۴α����Ÿ�������Ǯ���ˡ�
	gDeviceParaTab.voice[CARD_LIANGTONG_STUF] = Audio_STUFF; //Ա���ֻ���

}

void SoundMessage(unsigned char cmd)
{
	unsigned char css;

	setAudioHANDAN();

	if (gCardinfo.gMCardCand == CARDSTYLE_JTB)
		css = (gCardinfo.card_catalog - 1);
	else
		css = gCardinfo.card_catalog;
	if (css >= 0x40)
		css -= 0x40;
#ifdef BUS_HANDAN_
	if (gCardinfo.card_catalog == CARD_NORMAL_B)		//����ר�߿���ͬ�Ľ��ײ�ͬ�� ��Ǯ dong �۴� ר�߿�
	{
		if ((gDeviceParaTab.rate[gCardinfo.card_catalog] != 203) && (gDeviceParaTab.rate[gCardinfo.card_catalog] >= 100)) {
			gDeviceParaTab.voice[CARD_NORMAL_B] = Audio_zuanxian;		//����ר�߿�  //����ר�߿� 2014.4.8
		}
		else {
			gDeviceParaTab.voice[CARD_NORMAL_B] = Audio_DONG;		//����ר�߿�  //����ר�߿� 2014.4.8
		}

	}
#endif
#ifdef _debug_ICcard_
	debugstring("SoundMessage cmd=");
	debugdata(&cmd, 1, 0);
	debugstring("css=");
	debugdata((unsigned char *)&css, 1, 1);
	debugstring("Sound Port=");
	debugdata(gDeviceParaTab.voice + css, 1, 1);
#endif
	switch (cmd)
	{
	case INVALID_CARD:
		audio(Audio_INVALID);	//�Ƿ���
		break;
	case SOUND_FLING_MONEY:
		audio(Audio_FLING_MONEY);	//��Ͷ��
		break;
	case SOUND_CHARGE_MONEY:
		audio(Audio_PLUS_MONEY);
		break;
	case SOUND_DEAL:
		if (gDeviceParaTab.voice[css] == 0)
			css = 0;
#ifdef _HD_XIAOER_
		audio(Audio_DONG);	//��������
#else
		audio(gDeviceParaTab.voice[css]);	//��������
#endif
//		beep(1,100,50);
		break;
	case SOUND_DI_BUS:			//
		beep(1, 100, 50);
		break;
	case ERR_NO_PSAM:
		beep(8, 400, 40);		//PSAM��������
		break;
	default:
		break;
	}
}

unsigned char CheckSno(unsigned char mode, unsigned char ID, unsigned char *ptr)
{
	unsigned char i = 0, temp = 0;
	unsigned pos = ST_OK;
	//unsigned char buffer[32];
	//clr_dog();

	temp = gErrorCard.error_pointer;
	for (i = 0; i < gErrorCard.error_pointer; i++)
	{
		if ((mode == 1) || (mode == ERROR_MONTH)) {
			if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT_MONTH) continue;
		}
		else if ((mode == 0) || (mode == ERROR_MONEY)) {
			if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT_MONEY) continue;
		}
		else {
			if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT) continue;
		}

		if (memcmp(gErrorCard.ErrorDetail[i].Cardno, (unsigned char *)&gCardinfo.c_serial, 4) == 0)
		{
			if (gErrorCard.ErrorDetail[i].ErrorStep != ID)
			{
				gErrorCard.error_pointer = i;
				pos = ST_ERROR;
				break;
			}
			return ST_ERROR;
		}
	}

	if (gErrorCard.error_pointer == ERROR_DETAIL_LEN)
	{
		for (i = 0; i < ERROR_DETAIL_SUM - 1; i++)
			memcpy((unsigned char *)&gErrorCard.ErrorDetail[i], (unsigned char *)&gErrorCard.ErrorDetail[i + 1], ERROR_DETAIL_LEN);
		gErrorCard.error_pointer = (ERROR_DETAIL_SUM - 1);
	}

	if ((mode == 1) || (mode == ERROR_MONTH))
	{
		gErrorCard.ErrorDetail[gErrorCard.error_pointer].effFlag = IS_EFFECT_MONTH;
		if (pos == ST_OK)
		{
			memcpy(gErrorCard.ErrorDetail[gErrorCard.error_pointer].TollValue, (unsigned char *)&s_sum1, 4);
			memcpy(gErrorCard.ErrorDetail[gErrorCard.error_pointer].AferToll, (unsigned char *)&a_sum1, 4);
		}
	}
	else if ((mode == 0) || (mode == ERROR_MONEY))
	{

		gErrorCard.ErrorDetail[gErrorCard.error_pointer].effFlag = IS_EFFECT_MONEY;
		if (pos == ST_OK)
		{
			memcpy(gErrorCard.ErrorDetail[gErrorCard.error_pointer].TollValue, (unsigned char *)&s_sum1, 4);
			memcpy(gErrorCard.ErrorDetail[gErrorCard.error_pointer].AferToll, (unsigned char *)&a_sum1, 4);
		}
	}
	else
		gErrorCard.ErrorDetail[gErrorCard.error_pointer].effFlag = IS_EFFECT;

	switch (ID)
	{
	case OPER_BLACK:
	case OPER_PUBLIC_MESSAGE:
	case OPER_RECORD_PROFILE:
		memcpy(gErrorCard.ErrorDetail[gErrorCard.error_pointer].ToChange, ptr, 16);
		break;
	default:
		break;
	}
	memcpy(gErrorCard.ErrorDetail[gErrorCard.error_pointer].Cardno, &gCardinfo.c_serial, 4);

	gErrorCard.ErrorDetail[gErrorCard.error_pointer].ErrorStep = ID;
	if (pos == ST_OK)
		gErrorCard.error_pointer += 1;
	else
		gErrorCard.error_pointer = temp;

	//д����������FE2PROM��
	// 	for(i=0;i<ERROR_DETAIL_SUM;i++)
	// 	{
	// 		memset(buffer,0,32);
	// 		memcpy(buffer,(unsigned char *)&gErrorCard.ErrorDetail[i],ERROR_DETAIL_LEN);
	// 		sysfewrite(BIT_ERROR_DETAIL+i*ERROR_DETAIL_LEN,ERROR_DETAIL_LEN,buffer);
	// 	}
	// 	memset(buffer,0,2);
	// 	buffer[0]=gErrorCard.error_pointer;
	// 	sysfewrite(BIT_ERROR_POINT,1,buffer);
	return pos;
}

void ErrorOper(unsigned char flag)
{
	unsigned char i = 0, ii = 0;
	//	unsigned char buffer[32];
	//clr_dog();
	for (i = 0; i < gErrorCard.error_pointer; i++)
	{
		if (gErrorCard.ErrorDetail[i].effFlag != flag) continue;
		else
		{
			if (memcmp(gErrorCard.ErrorDetail[i].Cardno, (unsigned char *)&gCardinfo.c_serial, 4) == 0)
			{
				for (ii = i; ii < gErrorCard.error_pointer; ii++)
					memcpy((unsigned char *)&gErrorCard.ErrorDetail[ii], (unsigned char *)&gErrorCard.ErrorDetail[ii + 1], ERROR_DETAIL_LEN);
				gErrorCard.error_pointer -= 1;
				memset((unsigned char *)&gErrorCard.ErrorDetail[gErrorCard.error_pointer], 0, ERROR_DETAIL_LEN);
			}
		}
	}
	//д����������FE2PROM��
	// 	for(i=0;i<ERROR_DETAIL_SUM;i++)
	// 	{
	// 		memset(buffer,0,32);
	// 		memcpy(buffer,(unsigned char *)&gErrorCard.ErrorDetail[i],ERROR_DETAIL_LEN);
	// 		sysfewrite(BIT_ERROR_DETAIL+i*ERROR_DETAIL_LEN,ERROR_DETAIL_LEN,buffer);
	// 	}
	// 	
	// 	memset(buffer,0,2);
	// 	buffer[0]=gErrorCard.error_pointer;
	// 	sysfewrite(BIT_ERROR_POINT,1,buffer);
}


void save_normal(void)
{
	memcpy(PCard_delay.CardSno, (unsigned char *)&gCardinfo.c_serial, 4);
	PCard_delay.Sec = SysTime.seconds;
}
unsigned char Delay_Normal(void)
{
#if NORM_DLY_TIME==0
	return 0;
#else
	char i;
	if (memcmp(PCard_delay.CardSno, (unsigned char *)&gCardinfo.c_serial, 4))
	{
		memset(PCard_delay.CardSno, 0, 4);
		return 0;
	}
	i = SysTime.seconds - PCard_delay.Sec;
	if (i > NORM_DLY_TIME)
	{
		memset(PCard_delay.CardSno, 0, 4);
		return 0;
	}
	else return 1;
#endif
}
void save_h_month(void)
{
	unsigned char ii = 0;
	unsigned char mcount;
	//	unsigned char temp[100];

#ifdef _debug_ICcard_
	debugstring("save_h_momnth!!!!!!!!!!!!!\r\n");
#endif

	if (MothDelayTime.endptr >= LAST_LIST_LEN)
		MothDelayTime.endptr = LAST_LIST_LEN - 1;
	if (MothDelayTime.startptr >= LAST_LIST_LEN)
		MothDelayTime.startptr = 0;
	mcount = 0;
	if (MothDelayTime.endptr >= MothDelayTime.startptr)
	{
		for (ii = MothDelayTime.startptr; ii < MothDelayTime.endptr; ii++)
		{
			if (!memcmp(MothDelayTime.LastList[ii], (unsigned char *)&gCardinfo.c_serial, 4))
			{
				memset(MothDelayTime.LastList[ii], 0, 7);
				//				temp[mcount]=ii;
				mcount++;
			}
		}
	}
	else
	{
		for (ii = MothDelayTime.startptr; ii < LAST_LIST_LEN; ii++)
		{
			if (!memcmp(MothDelayTime.LastList[ii], (unsigned char *)&gCardinfo.c_serial, 4))
			{
				memset(MothDelayTime.LastList[ii], 0, 7);
				//				temp[mcount]=ii;
				mcount++;
			}
		}
		for (ii = 0; ii < MothDelayTime.endptr; ii++)
		{
			if (!memcmp(MothDelayTime.LastList[ii], (unsigned char *)&gCardinfo.c_serial, 4))
			{
				memset(MothDelayTime.LastList[ii], 0, 7);
				//				temp[mcount]=ii;
				mcount++;
			}
		}
	}


	memcpy(MothDelayTime.LastList[MothDelayTime.endptr], (unsigned char *)&gCardinfo.c_serial, 4);

	MothDelayTime.LastList[MothDelayTime.endptr][4] = SysTime.hours;
	MothDelayTime.LastList[MothDelayTime.endptr][5] = SysTime.minutes;
	MothDelayTime.LastList[MothDelayTime.endptr][6] = SysTime.seconds;

	MothDelayTime.endptr++;

	if (MothDelayTime.endptr == LAST_LIST_LEN)
		MothDelayTime.endptr = 0;

	if (MothDelayTime.startptr == MothDelayTime.endptr)
		MothDelayTime.startptr++;
	if (MothDelayTime.startptr == LAST_LIST_LEN)
		MothDelayTime.startptr = 0;

	save_file_MothDelayTime();
}

int month_decide(void)
{
	unsigned char i = 0;
	unsigned char buffer[3];
	//	unsigned char temp=0;
	int summin = 0;
	int sT1, sT2;
	int MWTime;
	//clr_dog();
	//get_date(buffer);
	//	temp=SysTime.day;

	//get_time(buffer);
	memcpy(buffer, &SysTime.hours, 3);

	// 	return 1;

	MWTime = WAIT_TIME;

	// 	if (gCardinfo.gMCardCand == CARDQR_CODE)
	// 	{
	// 		MWTime = WAIT_TIME;
	// 	}
	if (MWTime == 0)
		return 0;


	MSG_LOG("month_decide::c_serial:%08X\r\n", gCardinfo.c_serial);

	//	sT1 = (((buffer[0]>>4)*10+(buffer[0]&0x0f))*60+(buffer[1]>>4)*10+(buffer[1]&0x0f));
	sT1 = ((buffer[1] >> 4) * 10 + (buffer[1] & 0x0f)) * 60 + \
		(buffer[2] >> 4) * 10 + (buffer[2] & 0x0f);

	MSG_LOG("wait:%d,sT1:%d\r\n", MWTime, sT1);
	buffer[2] = SysTime.hours;

	if (MothDelayTime.endptr >= MothDelayTime.startptr)
	{
		for (i = MothDelayTime.startptr; i < MothDelayTime.endptr; i++)
		{
			MSG_LOG("M111othDelayTime.LastList:");
			BCD_LOG(MothDelayTime.LastList[i], 7, 1);

			if (!memcmp(MothDelayTime.LastList[i], (unsigned char *)&gCardinfo.c_serial, 4))
			{
				if (buffer[2] != MothDelayTime.LastList[i][4])
					continue;
				else
				{
					sT2 = (((MothDelayTime.LastList[i][5] >> 4) * 10 + (MothDelayTime.LastList[i][5] & 0x0f)) * 60 +
						(MothDelayTime.LastList[i][6] >> 4) * 10 + (MothDelayTime.LastList[i][6] & 0x0f));
					summin = sT1 - sT2;
					if (summin < MWTime)
						return summin;
				}
			}
		}
	}
	else
	{
		for (i = MothDelayTime.startptr; i < 50; i++)
		{
			MSG_LOG("M222othDelayTime.LastList:");
			BCD_LOG(MothDelayTime.LastList[i], 7, 1);

			if (!memcmp(MothDelayTime.LastList[i], (unsigned char *)&gCardinfo.c_serial, 4))
			{
				if (buffer[2] != MothDelayTime.LastList[i][4])
					continue;
				else
				{
					sT2 = (((MothDelayTime.LastList[i][5] >> 4) * 10 + (MothDelayTime.LastList[i][5] & 0x0f)) * 60 +
						(MothDelayTime.LastList[i][6] >> 4) * 10 + (MothDelayTime.LastList[i][6] & 0x0f));
					summin = sT1 - sT2;
					if (summin < MWTime)
						return summin;
				}
			}

		}
		for (i = 0; i < MothDelayTime.endptr; i++)
		{
			MSG_LOG("M333othDelayTime.LastList:");
			BCD_LOG(MothDelayTime.LastList[i], 7, 1);

			if (!memcmp(MothDelayTime.LastList[i], (unsigned char *)&gCardinfo.c_serial, 4))
			{
				if (buffer[2] != MothDelayTime.LastList[i][4])
					continue;
				else
				{
					sT2 = (((MothDelayTime.LastList[i][5] >> 4) * 10 + (MothDelayTime.LastList[i][5] & 0x0f)) * 60 +
						(MothDelayTime.LastList[i][6] >> 4) * 10 + (MothDelayTime.LastList[i][6] & 0x0f));
					summin = sT1 - sT2;
					if (summin < MWTime)
						return summin;
				}
			}

		}
	}
	return 0;
}


unsigned int get_s_sum1(unsigned char mode)
{
	unsigned int temp_s, i;
	int price, plus;
	unsigned char price_ratio;
	unsigned char ctype;
	//	unsigned char buf[7];
	unsigned int Time;
	//	unsigned int Time2;


	if (gCardinfo.card_catalog >= 0x40)
		ctype = gCardinfo.card_catalog - 0x40;
	else
		ctype = gCardinfo.card_catalog;
#ifdef _HD_XIAOER_
	ctype = 0;	//ֻ��һ���ۿ�
#endif

	memcpy((unsigned char*)&price, gDeviceParaTab.busPrice, 4);	//Ʊ��
	memcpy((unsigned char*)&plus, gDeviceParaTab.plusPrice, 4);	//����Ʊ��

	price_ratio = gDeviceParaTab.rate[ctype];			//Ʊ���ۿ�

#ifdef _debug_ICcard_
	debugstring("Ctype0:");
	debugdata((unsigned char*)&ctype, 1, 1);
	debugstring("price:");
	debugdata((unsigned char*)&price, 4, 1);
	debugstring("price_ratio0:");
	debugdata((unsigned char*)&price_ratio, 1, 1);
	debugstring("plus:");
	debugdata((unsigned char*)&plus, 4, 1);
#endif

	//���ж� �Ƿ�Ϊ�����Ż�
	if ((GetDateTime() == ST_ERROR) || (time_invalid(old_Infor.old_Purcs_Time) == ST_OK))//ʱ�䲻�� �����Żݴ���
		goto TimeErr;

	if (gDeviceParaTab.ChangeType == 0xAA)
	{
		//	if((DataTimeDec((unsigned char*)&SysTime, pcard1->getupTime) <= i))//�����ʱ���ڣ�
		Time = gDeviceParaTab.ChangeTime * 60;
#ifdef _debug_ICcard_
		debugstring("���˼���:\r\n");
		debugdata((unsigned char*)&gDeviceParaTab.ChangeType, 3, 1);
		debugdata((unsigned char*)&Time, 4, 1);
		debugdata((unsigned char*)&SysTime, 7, 1);
		debugdata(old_Infor.old_Purcs_Time, 14, 1);
#endif
		//		Ascii2BCD(gDeviceParaTab.DeviceNo,buf,8);
		// 		Get_SerialNum(buf);
		// 		if(memcmp((unsigned char*)&SysTime,old_Infor.old_Purcs_Time,7)>=0)
		// 		{
		// #ifdef _debug_ICcard_
		// 			debugstring("ʱ��1\r\n");
		// #endif
		// 			Time2=DataTimeDec((unsigned char*)&SysTime,old_Infor.old_Purcs_Time);
		// 			if((memcmp(buf,old_Infor.old_Purcs_Device,4)!=0)&&(Time2<Time)&&(Time2>300)&&(gCardinfo.card_catalog==CARD_NORMAL))//��ͬ���豸�� ���Сʱ�����
		// 			{
		// 				price_ratio=gDeviceParaTab.ChangeRate;
		// 				memset(old_Infor.resear,0xAA,2);//�Ż���
		// 			}
		// 
		// 			
		// 		}
		// 		else
		// 		{
		// 			Time2=DataTimeDec(old_Infor.old_Purcs_Time,(unsigned char*)&SysTime);
		// 			if((memcmp(buf,old_Infor.old_Purcs_Device,4)!=0)&&(Time2<Time)&&(Time2>300)&&(gCardinfo.card_catalog==CARD_NORMAL))//��ͬ���豸�� ���Сʱ�����
		// 			{
		// 				price_ratio=gDeviceParaTab.ChangeRate;
		// 					memset(old_Infor.resear,0xAA,2);//�Ż���
		// 			}
		// 		}
	}

#ifdef _debug_ICcard_
	debugstring("�Żݺ��ۿ�:\r\n");
	debugdata((unsigned char*)&price_ratio, 1, 1);
#endif
TimeErr:
	if (price_ratio > 100) {//�������Ʊ����������ۿ�һ���Ǵ���0��
		ctype = CARD_NORMAL;
		if (ctype >= 0x40)
			ctype = ctype - 0x40;
		price_ratio = gDeviceParaTab.rate[ctype];			//Ʊ���ۿ�
	}

#ifdef _debug_ICcard_
	debugstring("Ctype:");
	debugdata((unsigned char*)&ctype, 1, 1);
	debugstring("price_ratio:");
	debugdata((unsigned char*)&price_ratio, 1, 1);
	debugstring("price:");
	debugdata((unsigned char*)&price, 4, 1);
	debugstring("plus:");
	debugdata((unsigned char*)&plus, 4, 1);
#endif
	if (price_ratio > 100)
		price_ratio = 100;
	//���Ʊ��
	//�������Ʊ��Ǯ����ͨ�����ۿ�
	temp_s = price * price_ratio / 10;
	i = temp_s % 10;
	temp_s /= 10;
	if (i > 5)	//��������
		temp_s++;

#ifdef _debug_ICcard_
	debugstring("temp_s:");
	debugdata((unsigned char*)&temp_s, 4, 1);
#endif
	return temp_s;
}

//--------------------------------------------------

unsigned char PsamInitialize(void)
{
	unsigned char ii = 0;
	//	unsigned char pos=0;		
	unsigned char buffer[128];

	ii = ii;
	psamZJB.Flag = 1;

	psamZJB.SLot = PSAM_GJ;

	// 	display(0,0,"���ڼ��PSAM��",0);
	// 	display(2,1,"��ȴ�......",0);

#ifdef _debug_ICcard_
	MSG_LOG("PSAM init9999:%d\r\n", psamZJB.SLot);
#endif

#ifdef BUS_PRO_JSB
	ii = SamColdReset(psamZJB.SLot, 38400, 3, buffer);
#else
	ii = SamColdReset(psamZJB.SLot, 9600, 3, buffer);
#endif  //#ifdef BUS_PRO_JSB

#ifdef _debug_ICcard_
	debugstring("gPSAMin:");
	debugdata((unsigned char*)&psamZJB.SLot, 1, 1);
	debugstring("PSAM art:");
	debugdata(buffer, ii, 1);
#endif
	if (ii == 0) {
		printf("[%s] SAM Reset Error\r\n", __FUNCTION__);
		return ST_ERROR;
	}

	usleep(100000);
	if (Read_Binary(psamZJB.SLot, 0x16, 0, 6, buffer) == 0)
	{
		debugstring("Read_Binary error\r\n");
		debugdata(buffer, 8, 1);
		return ST_ERROR;
	}

#ifdef _debug_ICcard_
	debugstring("read Binary:");
	debugdata(buffer, 6, 1);
#endif
	memcpy(psamZJB.CardNO, buffer, 6);

#ifdef _debug_ICcard_
	debugstring("Select File ");
#endif
#ifdef BUS_PRO_JSB
	if (Select_File(psamZJB.SLot, 0x1001) == 0) //0x3f01
	{
		debugstring("Select_File error\r\n");
		return ST_ERROR;
	}

	memcpy(buffer, "\xBD\xA8\xC9\xE8\xB2\xBF", 6);		//���貿psam��������ͨӦ�ñ�ʶ
	if (Select_FileF(psamZJB.SLot, buffer, 6) == 0) {
		debugstring("Select_File error\r\n");
		return ST_ERROR;
	}
	if (Read_Binary(psamZJB.SLot, 0x17, 0, 25, buffer) == 0)		//��0x17�ļ�25�ֽ�
																	//if(Read_Binary(SAM1, 0x19, 0, 25, buffer)==0) 
	{
		debugstring("GJ_Read_Binary17 error\r\n");
		debugdata(buffer, 8, 1);
		return ST_ERROR;
	}
#ifdef _debug_CPU_
	debugstring("GJ_read 17 Binary:");
	debugdata(buffer, 25, 1);	//01000000000000000100000000000000012011122220161220
	debugstring("OK\r\n");
#endif
	memcpy((unsigned char*)&SamPubInf_ZJB, buffer, sizeof(sam_pub));	//��һ���ֽ�����Կ����
#else	
	if (Select_File(psamZJB.SLot, 0x1003) == 0) //0x1003 
	{
		debugstring("Select_File error\r\n");
		return ST_ERROR;
	}
#ifdef _debug_ICcard_
	debugstring("PSAM PIN:\r\n");
	memcpy(buffer, "\x12\x34\x56", 3);
#endif

	if (Verify_private_PIN(psamZJB.SLot, 3, buffer) == 0) {
		debugstring("verify PIN ERROR!!\r\n");
		return ST_ERROR;
	}
#endif
	//	delayxms(10);
#ifdef _debug_ICcard_
	printf("[%s] OK\r\n", __FUNCTION__);
#endif

	psamZJB.Flag = 0;//PSAM������

	return ST_OK;
}

unsigned char getMiKey(void)
{
#ifdef _M1KEY_3DES_
	SL3DES_MiKEY();
	return ST_OK;
#elif defined _M1KEY_CLOUD_
	unsigned char i;
	if (gCardinfo.card_catalog == CARD_KEY_BUS)
		i = Cloud_miKey(0x11);
	else
		i = Cloud_miKey(0x01);
	return i;
#else
	unsigned char pos, ri = 0;
	unsigned char buffer[30];
	unsigned char tempbuf[10];

	//	if(Select_File(gPSAMin, 0x3f01)==0){
	//		error(ERR_SELECT_MW_ADF3,0);
	//		return ST_ERROR;
	//	}

	if ((psamZJB.SLot == 0) || (psamZJB.SLot > 4)) {
		printf("[%s] ZJB PSAM ������(%d)����\r\n", __FUNCTION__, psamZJB.SLot);
		return ST_ERROR;
	}

	if (psamZJB.Flag) {
		printf("[%s] ZJB PSAM�쳣\r\n", __FUNCTION__);
		// 		cls();
		// 		if(gchn_eng == 'E'){
		// 			display(0,0,"error:",1);
		// 			display(2,0,"PSAM1 error!",0);
		// 		}
		// 		else
		// 		{
		// 			display(0,0,"����:",1);
		// 			display(2,0,"����PSAM����!",0);
		// 		}
		// 		gBuInfo.restore_flag = 3;
		// 		delayxs(1);
		return ST_ERROR;
	}



#ifdef _debug_ICcard_
	debugstring("get mi Key\r\n");
#endif

getmikeyagain:
	pos = 0;

	memcpy(buffer + pos, gCardinfo.CityCode, CITY_CODE_LEN);			//���д���	02

	pos += CITY_CODE_LEN;

#ifdef NO_CPU_PSAM_
	if (gCardinfo.gMCardCand == CARDSTYLE_CPU)
		memcpy(tempbuf, gCardinfo.PublishBicker, 4);//CPU�� ������û��ȡ������ʹ�õ�����ˮ��
	else
		memcpy(tempbuf, (unsigned char*)&gCardinfo.c_serial, 4);//�����к�	04
#else
	memcpy(tempbuf, (unsigned char*)&gCardinfo.c_serial, 4);//�����к�	04
#endif

	buffer[pos++] = tempbuf[0];
	buffer[pos++] = tempbuf[1];
	buffer[pos++] = tempbuf[2];
	buffer[pos++] = tempbuf[3];


	memcpy(buffer + pos, gCardinfo.PublishBicker, 2);					//������ˮ(BCD����λ)	02
	pos += 2;
	memcpy(buffer + pos, gCardinfo.CardCheckCode, CARD_CHECH_CODE_LEN);	//mac		04		
	pos += CARD_CHECH_CODE_LEN;

	buffer[pos++] = 0x02;	//����Ǯ����
	buffer[pos++] = 0x03;	//Ǯ����¼��			//���׼�¼����
	buffer[pos++] = 0x07;	//������Ϣ��

	pos = 0;

#ifdef _debug_ICcard_
	debugstring("get Key1:\r\n");
	debugdata(buffer, 15, 1);
	debugdata((unsigned char*)&psamZJB.SLot, 1, 1);
#endif


	//#ifdef BUS_PRO_JSB
	pos = Get_MifKey(psamZJB.SLot, 0x01, 0x01, 15, buffer);//JSB
// #else
// 		pos = Get_MifKey(gPSAMin, 0x10, 0x40, 15, buffer);//SLZR
// #endif

	if (pos == 0) {
		sprintf((char*)tempbuf, "%02X%02X", buffer[0], buffer[1]);
		printf("[%s] ERROR:%s\r\n", __FUNCTION__, (char*)tempbuf);
		// 		miniDispstr(6,0,(char*)tempbuf, 0);

		if (PsamInitialize() == ST_OK) {//�����ʼ����ȷ�˾���ȡһ�Ρ�
			if (ri++ < 2)
				goto getmikeyagain;
		}

		// 		DeactiveCard(gPSAMin);
		// 		if(PsamInitialize() != ST_OK){//pSAM��ȡ��Կ�����³�ʼ����
		// 			ReStartCPU();//���ɹ�������.
		// 		}
		// 		gBuInfo.restore_flag = 3;
		return ST_ERROR;
	}

	//	debugstring("ȡ��Կ��������:\r\n");
	//	Debugdata(30,30,buffer);

	memcpy(gCardinfo.MoneyKEYA, buffer, 6);				//����Ǯ��,��Ϣ2 6
	memcpy(gCardinfo.RecodeKEYA, buffer + 6, 6);			//���׼�¼	03 04 05
	memcpy(gCardinfo.MonthKEYA, buffer + 12, 6);			//��Ʊ���� 7 8
	memcpy(gCardinfo.PublichKEYA, buffer, 6);

#ifdef _debug_ICcard_
	debugstring(" Money KEYA:\r\n");
	debugdata(gCardinfo.MoneyKEYA, 6, 1);
	debugstring(" Publich KEYA:\r\n");
	debugdata(gCardinfo.PublichKEYA, 6, 1);
	debugstring(" Recode KEYA:\r\n");
	debugdata(gCardinfo.RecodeKEYA, 6, 1);
	debugstring(" Month KEYA:\r\n");
	debugdata(gCardinfo.MonthKEYA, 6, 1);
#endif
	return ST_OK;
#endif
}



unsigned char OperPublicMessage(void)
{
	unsigned char i = 0;
	unsigned char buff[60];
	unsigned char buffer[20];

	gCardinfo.gucRestorFlag = 0;
	//clr_dog();

#ifdef _debug_ICcard_
	debugstring("Oper PublicMessage 1\r\n");
#endif
	if (gCardinfo.publicMessageSector >= 16) {//û�й�����Ϣ��
		memset((unsigned char*)&CARD_PUBLIC1, 0, sizeof(CARD_PUBLIC1));
		CARD_PUBLIC1.DealPo = 1;
		CARD_PUBLIC1.DelaFlag = 2;
		return ST_OK;
	}

	if (MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, gCardinfo.PublichKEYA, gCardinfo.publicMessageSector * 4) == 0)
	{
		return ST_ERROR;
	}

#ifdef _debug_ICcard_
	debugstring("Oper PublicMessage 2\r\n");
#endif

	//��������Ϣ
	if (MifareRead(gCardinfo.publicMessageSector * 4, buff) == 0)
	{
		return ST_ERROR;
	}
	if ((MiBlockInvalid(1, 15, buff) == ST_ERROR) || (buff[0] == 0) || (buff[0] > 9)) {
		gCardinfo.gucRestorFlag |= RES_PUBLIC_Z;
	}

#ifdef _debug_ICcard_
	debugstring("sector 6 0:");
	debugdata(buff, 16, 1);
#endif
	if (MifareRead(gCardinfo.publicMessageSector * 4 + 1, buff + 30) == 0)
	{
		return ST_ERROR;
	}
#ifdef _debug_ICcard_
	debugstring("sector 6 1:");
	debugdata(buff + 30, 16, 1);
#endif
	if (MiBlockInvalid(1, 15, buff + 30) == ST_ERROR) {
		gCardinfo.gucRestorFlag |= RES_PUBLIC_F;
	}
	if ((gCardinfo.gucRestorFlag & RES_PUBLIC_Z) && (gCardinfo.gucRestorFlag & RES_PUBLIC_F)) {
		return CARD_PUBLIC_BAD;
	}
	else if ((gCardinfo.gucRestorFlag & RES_PUBLIC_Z) == 0)
	{
		if (buff[3] == 0x01)
			gCardinfo.gucRestorFlag |= RES_PUBLIC_FLAG;
	}
	for (i = 0; i < 8; i++) {
		if ((buff[i] ^ buff[30 + i]) != 0xff) {
			gCardinfo.gucRestorFlag |= RES_PUBLIC_D;
			break;
		}
	}

#ifdef _debug_ICcard_
	debugstring("������Ϣ00:\r\n");
	debugdata((unsigned char*)&gDeviceParaTab.ChangeType, 3, 1);
#endif
	if (gDeviceParaTab.ChangeType == 0xAA)
	{
		memset(old_Infor.old_Purcs_Time, 0, sizeof(oldPursIfo));
		//�ڶ�����
		if (MifareRead(gCardinfo.publicMessageSector * 4 + 2, buffer) == 0)
		{
#ifdef _debug_ICcard_
			debugstring("6������Ϣ2������\r\n");
#endif
			return ST_ERROR;
		}
		if (MiBlockInvalid(1, 15, buffer) == ST_ERROR)
		{
#ifdef _debug_ICcard_
			debugstring("6������Ϣ2 У���\r\n");
#endif

			return ST_ERROR;
		}

#ifdef _debug_ICcard_
		debugstring("6������Ϣ2:");
		debugdata(buffer, 16, 1);
#endif


		// 	memcpy(old_Infor.old_Purcs_Time,buffer,7);
		// 	memcpy(old_Infor.old_Purcs_LineNum,buffer+7,3);
		memcpy(old_Infor.old_Purcs_Time, buffer, 16);
#ifdef _debug_ICcard_
		debugstring("�ϱ�����ʱ�䣺");
		debugdata(old_Infor.old_Purcs_Time, 7, 1);
		debugstring("�ϱ�������·�ţ�");
		debugdata(old_Infor.old_Purcs_LineNum, 3, 1);
		debugstring("�ϱ������豸�ţ�");
		debugdata(old_Infor.old_Purcs_Device, 4, 1);
#endif

	}//�����Ż�



#ifdef _debug_ICcard_
	debugstring("sector 6:");
	debugdata(buff, 16, 1);
	debugdata(buff + 30, 16, 1);
#endif
	memcpy((unsigned char *)&CARD_PUBLIC, buff, CARD_PULICH_MESSAGE_LEN);
	memcpy((unsigned char *)&CARD_PUBLIC1, buff + 30, CARD_PULICH_MESSAGE_LEN);
	if ((CARD_PUBLIC.BlackFlag == 0x04) || (CARD_PUBLIC1.BlackFlag == 0xfb))
		return CARD_BLACK;

	if (CARD_PUBLIC.DelaFlag == 0x05)
		return CARD_CHAGEERR;
	// 	if(buff[10] == 0){
	// 		if(buff[30] != 0xff){
	// 			buff[10] = ~buff[30];
	// 			buff[11] = ~buff[31];
	// 			buff[12] = ~buff[32];
	// 		}
	// 	}
	if (gCardinfo.gucRestorFlag & RES_PUBLIC_Z) {
		//		memcpy(buff, (unsigned char*)&CARD_PUBLIC1, 8);
		memcpy(buff, buff + 30, 16);
		for (i = 0; i < 16; i++) {
			buff[i] = ~buff[i];//����
		}
		memcpy((unsigned char*)&CARD_PUBLIC, buff, 8);
	}

	return ST_OK;

	// 	i = GethisMessage();
	// 	return i;
}

unsigned char Card_typeProcess(void)
{
	unsigned int startTime, endTime, nowTime, endTimeEn;
	unsigned char pos;
	unsigned char buffer[256];
	unsigned char cNo[8];
	unsigned char i, ii, flag;
	unsigned char BLK_TEMP[30];
	unsigned char BLK_TEMP_1[30];
	unsigned int uitemp;


#ifdef BUS_Cloud_
	unsigned int crc32;

	cpuinfo.catalogPOST = 0;
	gCardinfo.listSector = cpuinfo.catalogPOST;
	memcpy(cNo, (unsigned char *)&gCardinfo.c_serial, 4);
	cNo[0] ^= 0x71;
	cNo[1] ^= 0x30;
	cNo[2] ^= 0x4B;
	cNo[3] ^= 0x19;
	memcpy(buffer, cNo, 4);//0����Ĭ����Կ
	memcpy(buffer + 4, cNo, 2);//0����Ĭ����Կ
#else //#ifdef BUS_Cloud_
	memcpy(cNo, (unsigned char *)&gCardinfo.c_serial, 4);
	gCardinfo.listSector = 0;
	memcpy(buffer, "\xa0\xa1\xa2\xa3\xa4\xa5", 6);//0����Ĭ����Կ
#endif

#ifdef _debug_ICcard_
	printf("[%s]\r\n", __FUNCTION__);
#endif


	flag = 0xff;
	gCardinfo.publishSector = 1;//the bussiness  card used
	for (i = 0; i < 2; i++) {
#ifdef _debug_ICcard_
		debugstring("card type proce:");
		debugdata((unsigned char*)&gCardinfo.listSector, 1, 1);
		debugdata(buffer, 6, 1);
#endif
		pos = ((gCardinfo.listSector * 4) + 1);//��Ŀ¼����һ�顣
		MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, buffer, gCardinfo.listSector * 4);
#ifdef _debug_ICcard_
		debugstring("card type proce2:");
		debugdata((unsigned char*)&pos, 1, 1);
#endif
		if (MifareRead(pos, buffer) == 0)
		{
			//	debugstring("read 1 error\r\n ");
			profile(0);
		}
		else {
			flag = 0;
			break;
		}
	}

	if (flag != 0) {
#ifndef BUS_Cloud_
		return 0xA2;
#else
		profile(0);
#endif
	}
	else {//��1����ȷ

#ifdef _debug_ICcard_
		debugstring("ListSectBUff:");
		debugdata(buffer, 16, 1);
#endif
		memset((unsigned char*)&gCardinfo.listSector, 0xff, 9);
		ii = 0;
		for (i = 0; i < 16; i++)
		{
			switch (buffer[i])
			{
			case LIST_AREA:
				gCardinfo.listSector = i;
				break;
			case PUBLISH_AREA:
				gCardinfo.publishSector = i;
				break;
			case DEALRECORD_AREA:
				gCardinfo.dealRecordSector[ii] = i;
				ii++;
				break;
			case PBULICMESSAGE_AREA:
				gCardinfo.publicMessageSector = i;
				break;
			case ODDMONTH_AREA:
				gCardinfo.oddMonthSector = i;
				break;
			case DODMONTH_AREA:
				gCardinfo.dodMonthSector = i;
				break;
			case PUBLICMONEY_AREA:
				gCardinfo.publicMoneySector = i;
				break;
			default:
				break;
			}
		}
	}

	memcpy(gCardinfo.PublishKEYA, cNo, 4);
	memcpy(gCardinfo.PublishKEYA + 4, cNo, 2);
	memcpy(gCardinfo.PublichKEYA, cNo, 4);
	memcpy(gCardinfo.PublichKEYA + 4, cNo, 2);
	memcpy(gCardinfo.MoneyKEYA, cNo, 4);
	memcpy(gCardinfo.MoneyKEYA + 4, cNo, 2);
	memcpy(gCardinfo.MonthKEYA, cNo, 4);
	memcpy(gCardinfo.MonthKEYA + 4, cNo, 2);
	memcpy(gCardinfo.RecodeKEYA, cNo, 4);
	memcpy(gCardinfo.RecodeKEYA + 4, cNo, 2);

#ifdef _debug_ICcard_
	debugdata((unsigned char *)&gCardinfo.c_serial, 4, 1);
	debugdata((unsigned char *)&gCardinfo.publishSector, 1, 1);
	debugdata(gCardinfo.PublishKEYA, 6, 1);
	debugstring("gCardinfo.PublichKEYA::");
	debugdata(gCardinfo.PublichKEYA, 6, 1);
#endif
	if (MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, gCardinfo.PublishKEYA, gCardinfo.publishSector * 4) == 0)
	{
		debugstring("MifareAuthKey error\r\n ");
		return ST_ERROR;
	}
	//	debugstring("E ");

	if (MifareRead(gCardinfo.publishSector * 4, buffer) == 0)
	{
		debugstring("read 4 error\r\n ");
		return ST_ERROR;
	}

#ifdef _debug_ICcard_
	debugstring("1������Ϣ0��\r\n");
	debugdata(buffer, 16, 1);
#endif	
	if (MiBlockInvalid(1, 15, buffer) == ST_ERROR)
	{
		return ST_ERROR;
	}

	gCardinfo.gMCardCand = CARDSTYLE_NORM1;
	pos = 0;
	memcpy(gCardinfo.CityCode, buffer + pos, CITY_CODE_LEN);			//���д���	2
	pos += CITY_CODE_LEN;
	memcpy(gCardinfo.TradeCode, buffer + pos, TRADE_CODE_LEN);			//��ҵ����	2
	pos += TRADE_CODE_LEN;
	memcpy((unsigned char *)gCardinfo.PublishBicker, buffer + pos, PUBLISH_BICKER_LEN);	//����ˮ��	4	
	memcpy(gCardinfo.CardCheckCode, buffer + 8, CARD_CHECH_CODE_LEN);	//mac��		4
	gCardinfo.card_catalog = buffer[13];								//����		1

#ifdef New_Times_Card__
	gCardinfo.Zicard_catalog = buffer[14];
#else
	gCardinfo.Zicard_catalog = 0;
#endif


#ifdef _debug_ICcard_
	if (gCardinfo.card_catalog == CARD_STUDENT)
	{
		gCardinfo.card_catalog = CARD_YOUFU_BUS;								//����		1  ������
	}
#endif

	if (gCardinfo.card_catalog == CARD_WHITE_BUS)
	{
		return CARD_WHITE_BUS;
	}

#ifdef BUS_Cloud_
	if ((mode == 0xA1) && (gCardinfo.card_catalog < 0x40)) {//��������ͷ���, �����ƿ�����
		return CONTROL_CARD;
	}

	if (gCardinfo.card_catalog == CARD_Business) {
		if (MifareRead(gCardinfo.publishSector * 4 + 1, buffer) == 0)//ȡĿ¼������λ��
		{
			return ST_ERROR;
		}

#ifdef _debug_ICcard_
		debugstring("1������Ϣ1��\r\n");
		debugdata(buffer, 16, 1);
#endif	

		crc32 = Getcrc16(buffer, 8, 0xFF, 0xFF);
		//		crc32 = __cpuCrc32(buffer, 8);
		if (memcmp((unsigned char*)&crc32, buffer + 8, 4) != 0) {
			debugstring("CRCerror:");
			debugdata((unsigned char*)&crc32, 4, 1);
			gCardinfo.catalogPOST = 0;
			return ST_ERROR;
		}
		// 		if(MiBlockInvalid(1,15,buffer)==ST_ERROR) 
		// 		{
		// 			gCardinfo.catalogPOST = 0;
		// 			return ST_ERROR;
		// 		}
		gCardinfo.catalogPOST = buffer[0];//дĿ¼
		return CONTROL_CARD;
	}
	if (gCardinfo.card_catalog == CARD_FENDUAN_Line) {
		return CONTROL_CARD;
	}
#endif //#ifdef BUS_Cloud_

#ifdef _debug_ICcard_
	debugstring("card_catalog:");
	debugdata((unsigned char*)&gCardinfo.card_catalog, 1, 1);
	debugdata((unsigned char*)&gCardinfo.Zicard_catalog, 1, 1);
#endif	

	gCardinfo.nnAudioflag = 0;

	switch (buffer[12])//������־
	{
	case STARTUP_OK:
		break;
	case STARTUP_STOP:
		return CARD_STOPWORK;
	case STARTUP_NO:
		return CARD_NO_USE;
	case STARTUP_BLACK:
		return CARD_BLACK_PRO;
	default:
		return CARD_NO_SYSTEM;
	}

	if (MifareRead(gCardinfo.publishSector * 4 + 1, buffer) == 0)//�������� ��Ч���� �������� ��Ѻ��	����	У��
	{
		return ST_ERROR;
	}

#ifdef _debug_ICcard_
	debugstring("1������Ϣ1:");
	debugdata(buffer, 16, 1);
#endif

	if (MiBlockInvalid(1, 15, buffer) == ST_ERROR)
	{
#ifdef _debug_ICcard_
		debugstring("1������Ϣ1 У���\r\n");
#endif

		return ST_ERROR;
	}
	//ʱ�仺����0
	endTime = 0;
	startTime = 0;
	nowTime = 0;
	endTimeEn = 0;

	memcpy(gCardinfo.stuffNO, buffer, 4);//Ա�����Ŀ���,��Ҫ��˾����ʹ��

	gCardinfo.nnAudioflag = buffer[14];//�Ա�

	memcpy(buffer + 50, buffer + 4, 4);
	buffer[52] = bcdDEC(buffer[52], 1);//��Ч����,��ǰһ������ʾ��
	if (buffer[52] == 0) {
		buffer[52] = 0x12;
		buffer[51] = bcdDEC(buffer[51], 1);
	}

	memcpy((unsigned char*)&endTimeEn, buffer + 50, 4);	//��Ч����,�����ġ�
	memcpy((unsigned char*)&endTime, buffer + 4, 4);//��Ч���ڣ�����ǰ��
	memcpy((unsigned char*)&startTime, buffer + 8, 4);//��������

	if (GetDateTime() == ST_ERROR)
	{
		return ST_ERROR;
	}
#ifdef _debug_ICcard_
	debugstring("systime:");
	debugdata((unsigned char*)&SysTime, 7, 1);
#endif
	memcpy((unsigned char*)&nowTime, (unsigned char *)&SysTime.year_h, 4);//��������

#ifdef _debug_ICcard_
	debugstring("chk:");
	debugdata((unsigned char*)&nowTime, 4, 1);
	debugdata((unsigned char*)&startTime, 4, 1);
	debugdata((unsigned char*)&endTime, 4, 1);
	debugdata((unsigned char*)&endTimeEn, 4, 1);
#endif
	RevertTurn(4, (unsigned char*)&nowTime);
	RevertTurn(4, (unsigned char*)&startTime);
	RevertTurn(4, (unsigned char*)&endTime);
	RevertTurn(4, (unsigned char*)&endTimeEn);

#ifdef _debug_ICcard_
	debugstring("gCardinfo.card_catalog:");
	debugdata((unsigned char*)&gCardinfo.card_catalog, 1, 1);
#endif

	memcpy(&uitemp, gCardinfo.PublishBicker, 4);
	sprintf((char *)BLK_TEMP_1, "%016u", uitemp);//��ʮ������ת��ASCII��ʮ����
	Ascii2BCD(BLK_TEMP_1, BLK_TEMP, 16);

	if (gCardinfo.card_catalog < 0x20) {

		if (getMiKey() == ST_ERROR)
		{
			return ST_ERROR;
		}
	}
	else if ((gCardinfo.card_catalog >= 61) && (gCardinfo.card_catalog < 74))
	{
		if (getMiKey() == ST_ERROR)	//�Ž������ڵĿ��ƿ�Ҳȫ��ȡ��Կ
		{
			return ST_ERROR;
		}
		if (IsInBlackList(BLK_TEMP)) {
			return CARD_BLACK_PRO;
		}
		// // 		return	CONTROL_CARD;
	}
	else
	{
		return CARD_NO_SYSTEM;
	}


#ifdef _debug_ICcard_
	debugstring("Oper ---------\r\n");
	debugdata((unsigned char*)&pos, 1, 1);
#endif
	pos = OperPublicMessage();
#ifdef _debug_ICcard_
	debugstring("OperPublicMessage pos:");
	debugdata((unsigned char*)&pos, 1, 1);
#endif
	//		getCpuInfo((stCpuInfo*)buffer); //�ڶ�����ʱ����	
	if (pos == CARD_PUBLIC_BAD) {
		return CARD_PUBLIC_BAD;
	}
	else if (pos == CARD_BLACK)
	{
		return CARD_BLACK;
	}
	else if (pos == CARD_CHAGEERR) {
		return CARD_CHAGEERR;
	}
	else if (pos == ST_ERROR) {
		if (gCardinfo.card_catalog < 0x20) {//�û������ش��󣬲����û���������
			return ST_ERROR;
		}
	}


#ifdef _debug_ICcard_
	debugstring("gCardinfo.card_catalog:::");
	debugdata(&gCardinfo.card_catalog, 1, 1);
	debugstring("gDeviceParaTab.rate");
	debugdata(gDeviceParaTab.rate + gCardinfo.card_catalog, 1, 1);
#endif


#ifdef BUS_PRO_JSB


	if (gCardinfo.card_catalog < 0x20)//�����Ǻ�������
	{
		if (IsInBlackList(BLK_TEMP)) {
			return CARD_BLACK_PRO;
		}
		if (gDeviceParaTab.rate[gCardinfo.card_catalog] == 0) {//�ۿ�Ϊ0����������
			return ST_OK;
		}
		if (gCardinfo.card_catalog != CARD_NORMAL&&gCardinfo.card_catalog != CARD_NORMAL_B)//���ֿ�������
		{
			if (endTime < nowTime)
			{
#ifdef _debug_ICcard_
				printf("���ڿ�,��Ҫ����\r\n");
#endif
				return CARD_NO_TIME;
			}
		}


		if (gDeviceParaTab.rate[gCardinfo.card_catalog] == 202)//�������
		{
#ifdef _debug_ICcard_
			debugstring("===================202���\r\n");
#endif
			return MONTH_CARD;
		}
		else if ((gDeviceParaTab.rate[gCardinfo.card_catalog] == 104) || (gDeviceParaTab.rate[gCardinfo.card_catalog] == 204)) {
#ifdef _debug_ICcard_
			debugstring("===================204��ֹ\r\n");
#endif
			return CARD_forbid;//�˿�����ֹʹ��
		}//����ר�߿� 2014.4.8

		//�����ο�
		if (gCardinfo.card_catalog == CARD_MOTH_CI)//�ο�
			return MONTH_CARD;
#ifdef _HD_XIAOER_
		return CARD_MONEY;			//Ǯ��
#endif

		if ((gDeviceParaTab.rate[gCardinfo.card_catalog] == 101) || (gDeviceParaTab.rate[gCardinfo.card_catalog] == 201))
		{
#ifdef _debug_ICcard_
			debugstring("===================201����Ʊ\r\n");
#endif
			return MONTH_CARD;//�۴κ���Ҫ��Ǯ�����ӷѣ�����Ʊ����101Ҫ��һ�θ��ӷѣ�110���Ӹ��ӷѣ�111��һ�Σ�112�����Σ����ε�119.

		}
		if ((gDeviceParaTab.rate[gCardinfo.card_catalog] >= 110)\
			&& (gDeviceParaTab.rate[gCardinfo.card_catalog] <= 119))
		{
#ifdef _debug_ICcard_
			debugstring("====110~119 ȥ����Ʊ\r\n");
#endif

			return MONTH_CARD;//�۴κ���Ҫ��Ǯ�����ӷѣ�����Ʊ����101Ҫ��һ�θ��ӷѣ�110���Ӹ��ӷѣ�111��һ�Σ�112�����Σ����ε�119.
		}

		//����ר�߿� 2014.4.8
		if ((gDeviceParaTab.rate[gCardinfo.card_catalog] == 103) || (gDeviceParaTab.rate[gCardinfo.card_catalog] == 203)) {
#ifdef _debug_ICcard_
			debugstring("===================203ֻ�ܿ�Ǯ\r\n");
#endif
			return CARD_MONEY; //�˿����ܿ۴Σ�ֻ�ܿ�Ǯ
		}//����ר�߿� 2014.4.8

		//���µ��ۿ��Ѿ���=100��
		if (gCardinfo.oddMonthSector != 0xff)//��Ʊ
		{
			if (gCardinfo.card_catalog == CARD_NORMAL_B) {
				//ר�߿�
#ifdef _debug_ICcard_
				debugstring("===================<=100 ר�߿���Ǯ\r\n");
#endif
				return MONEY_CARD;
			}
#ifdef _debug_ICcard_
			debugstring("gCardinfo.oddMonthSector!=0xff\r\n");
#endif
			return MONTH_CARD;
		}
		else
		{
			return CARD_MONEY;			//Ǯ��
		}
	}
	else {
		return	CONTROL_CARD;
	}//�����Ǻ�������


////�����Ǻ�������-------------------------------------------------------------------------


#endif

	return CARD_MONEY;
}

void addStatMoney(unsigned char mode, unsigned int moneyv)
{
	// 	unsigned char index;
	// 	stStaeInfo stainto;
	// 	unsigned int it, ii;
	// 	
	// 	if((mode != ID_REC_TOLL)&&(mode != ID_REC_MON))
	// 		return;
	// 	
	// 	for(ii=0;ii<3;ii++){
	// 		sysferead(BIS_PERDat_Info, sizeof(stStaeInfo), (unsigned char*)&stainto);//ÿ�����һ����ʷָ���ͳ������ ����ʱ��7+��ֵ�ۼƽ��4+�����ۼƽ��4+�ۿ�����2+����11+CRC32(4)
	// 		
	// 		it = GenerateCRC32((unsigned char*)&stainto, (sizeof(stStaeInfo)-4));
	// #ifdef _debug_
	// 		debugstring("Ra stStaeInfo:");
	// 		debugdata((unsigned char*)&stainto, sizeof(stStaeInfo), 1);
	// 		debugdata((unsigned char*)&it, 4, 1);
	// #endif
	// 		
	// 		if(stainto.sCrc32 == it)
	// 			break;
	// 	}
	// 	if(ii >= 3){
	// 		memset((unsigned char*)&stainto, 0, sizeof(stStaeInfo));
	// 		memcpy(stainto.DataTime, (unsigned char*)&SysTime, 7);
	// 		debugstring("bͳ�Ƽ�������\r\n");
	// 	}
	// 	
	// 	for(index=0; index<8; index++){//�Ҵ˲���Ա�ı���λ��
	// 		if(memcmp(stainto.infoBuf[index].cardNo, DriveCardNo, 4) == 0){
	// 			break;
	// 		}
	// 		if(memcmp(stainto.infoBuf[index].cardNo, "\x00\x00\x00\x00", 4) == 0){
	// 			break;
	// 		}
	// 	}
	// 	if(index >= 8)
	// 		index = 7;
	// #ifdef _debug_
	// 	debugstring("check index1:");
	// 	debugdata((unsigned char*)&index, 1, 1);
	// 	debugstring("::");
	// 	debugdata((unsigned char*)&stainto.infoBuf[index], sizeof(stSatone), 1);
	// #endif
	// 	it = 0;
	// 	if(mode == ID_REC_TOLL){//Ǯ��
	// 		memcpy(stainto.infoBuf[index].cardNo, DriveCardNo, 4);
	// 		stainto.infoBuf[index].chargeA += moneyv;
	// 		
	// 		stainto.infoBuf[index].SaleA++;
	// 		it = GenerateCRC32((unsigned char*)&stainto, sizeof(stStaeInfo)-4);
	// 		stainto.sCrc32 = it;
	// 		sysfewrite(BIS_PERDat_Info, sizeof(stStaeInfo), (unsigned char*)&stainto);
	// 	}
	// 	else if(mode == ID_REC_MON){//��Ʊ
	// 		memcpy(stainto.infoBuf[index].cardNo, DriveCardNo, 4);
	// 		stainto.infoBuf[index].RseverA += 1;//s_sum1;
	// 		
	// 		stainto.infoBuf[index].SaleA++;
	// 		it = GenerateCRC32((unsigned char*)&stainto, sizeof(stStaeInfo)-4);
	// 		stainto.sCrc32 = it;
	// 		sysfewrite(BIS_PERDat_Info, sizeof(stStaeInfo), (unsigned char*)&stainto);
	// 	}
}
void BuildRecorde(unsigned char delType, unsigned char *recBuf)
{
	DEALRECODE *rRecordDeal;
	unsigned int temp;

	rRecordDeal = (DEALRECODE *)recBuf;

	if ((delType & 0x7F) == ID_REC_TOLL) {
		memcpy(rRecordDeal->rMoneyDealSum, CARD_PUBLIC.MoneyDealSum, 2);
		memcpy(rRecordDeal->rAfterMoney, (unsigned char*)&a_sum1, 4);
		temp = gBuInfo.MoneyDealPointer;
		gBuInfo.MoneyDealPointer++;
		if (gBuInfo.MoneyDealPointer > 0x3B9AC9FF)
			gBuInfo.MoneyDealPointer = 0;
		temp = gBuInfo.MoneyDealPointer;
		memcpy(rRecordDeal->rDealMoney, (unsigned char *)&s_sum1, 3);
	}
	else if ((delType & 0x7F) == ID_REC_MON) {
		memcpy(rRecordDeal->rMoneyDealSum, CARD_PUBLIC.MoneyDealSum, 2);
		memcpy(rRecordDeal->rAfterMoney, (unsigned char*)&dis_sum2, 4);
		gBuInfo.MonthDealPointer++;
		if (gBuInfo.MonthDealPointer > 0x3B9AC9FF)
			gBuInfo.MonthDealPointer = 0;
		temp = gBuInfo.MonthDealPointer;
		memcpy(rRecordDeal->rDealMoney, (unsigned char *)&s_sum1, 3);
	}
	else {
		memcpy(rRecordDeal->rDealTime, (unsigned char*)&SysTime, 7);
		rRecordDeal->rCardType = gCardinfo.card_catalog;
		rRecordDeal->rDealType = delType;
		memcpy(rRecordDeal->rDriverNo, gBuInfo.DriverNO, 4);
		gBuInfo.ControlDealPointer++;
		if (gBuInfo.ControlDealPointer > 0x3B9AC9FF)
			gBuInfo.ControlDealPointer = 0;
		temp = gBuInfo.ControlDealPointer;
		memcpy(rRecordDeal->rCardDealNumb, (unsigned char *)&temp, 4);
		memcpy(rRecordDeal->rPublishNumb, gCardinfo.PublishBicker, 4);
	}
	rRecordDeal->rRemain = 0;
	rRecordDeal->rPublishNumb_a = 0;
	memcpy(rRecordDeal->rCardDealNumb, (unsigned char*)&temp, 4);
	Get_SerialNum(rRecordDeal->rResever1);
	//	memcpy(rRecordDeal->rResever1, "\xea\xef\xea\xef", 4);
	memcpy(rRecordDeal->rDealTime, (unsigned char*)&SysTime, 7);
	rRecordDeal->rDealType = delType;

	//	getCpuDeviceNO(gDeviceParaTab.DeviceNo);	//����ȡ�豸��

	Ascii2BCD(gDeviceParaTab.DeviceNo, rRecordDeal->rDeviceNo, 8);
	rRecordDeal->rCardType = gCardinfo.card_catalog;
	//	ASCII2ZBcod((char *)gDeviceParaTab.DeviceNo, rRecordDeal->rDeviceNo,4);
	memcpy(rRecordDeal->PbocTac, gCardinfo.PbocTac, 4);
	memcpy(rRecordDeal->rDriverNo, gBuInfo.DriverNO, 4);
	memcpy(rRecordDeal->rstuffernum, gBuInfo.DriverNO, 4);
	//	getCpuLineNo();

	memcpy(rRecordDeal->rLineNo, gDeviceParaTab.LineNo, 2);
	memcpy(rRecordDeal->oldLineNO, "\xEE\xEE", 2);
	memcpy(rRecordDeal->rPublishNumb, gCardinfo.PublishBicker, 4);
	rRecordDeal->rCardCand = gCardinfo.gMCardCand;

	memcpy(rRecordDeal->rBussinessNo, gCardinfo.TradeCode, 2);//�̻�����Կ��ڱ��Ϊ׼������ȡˢ�����ڱ��



	memset(rRecordDeal->ruserTimes, 0, 2);

	//	rRecordDeal->rReseverX = 0xFF;
	rRecordDeal->rReseverX = old_Infor.resear[0];
	temp = SOFT_VER_TIME_LOG;
	memcpy(rRecordDeal->rProVer, (unsigned char*)&temp, 2);			//����汾


#ifdef Card_typeM_Min_is0_
	memcpy(rRecordDeal->rTAC + 2, "\xea\xef", 2);
#endif

#ifdef _debug_
	debugstring("BLD REC:");
	debugdata(recBuf, 64, 1);
#endif


	if (((delType & 0x7F) == ID_REC_TOLL) || ((delType & 0x7F) == ID_REC_MON)) {
		addStatMoney((delType & 0x7F), s_sum1);
	}


	save_file_BuInfo();	//�������.

}

//����豸�����Ƿ���ȷ�����豸�Ƿ���ȷ��Ʊ�ۡ���·��ż��������ۿ����Ƿ���ȷ��
unsigned char checkBusInfo(void)
{
	if ((memcmp(gDeviceParaTab.DeviceNo, "\x00\x00\x00\x00\x00\x00\x00\x00", 8) == 0) ||
		memcmp(gDeviceParaTab.LineNo, "\x00\x00", 2) == 0)
		return ST_ERROR;
	else
		return ST_OK;

}

int getCartypeJTB(char *cardD, unsigned char type)
{

	if (type == 1) {
		strcpy(cardD, "��ͨ��");
	}
	else if (type == 2) {
		strcpy(cardD, "ѧ����");
	}
	else if (type == 3) {
		strcpy(cardD, "���꿨");
	}
	else if (type == 4) {
		strcpy(cardD, "���Կ�");
	}
	else if (type == 5)
	{
		strcpy(cardD, "���˿�");
	}
	else if (type == CARD_JTB)
	{
		if (gCardinfo.CardType == LocakCard)
			strcpy(cardD, "����");
		else
			strcpy(cardD, "��ͨ��");
	}
	else
		cardD[0] = 0;
	if (strlen(cardD) == 0)
		return ST_ERROR;
	else
		return ST_OK;
}
int getCardtypeHANDAN(char *cardD, unsigned char type)
{
	if (type == CARD_NORMAL) {
		strcpy(cardD, "��ͨ��");
	}
	else if (type == CARD_STUDENT) {
		strcpy(cardD, "ѧ����");
	}
	else if (type == CARD_COUPON) {
		strcpy(cardD, "��Ʊ��");
	}
	else if (type == CARD_OLDMAN) {
		strcpy(cardD, "���Ͽ�");
	}
	else if (type == CARD_HANDICAPPED) {
		strcpy(cardD, "��ͨ��");
	}
	else if (type == CARD_NORMAL_B) {//����ר�߿� 2014.4.8
		strcpy(cardD, "ר�߿�");
	}
	else if (type == CARD_RELATION_BUS) {
		strcpy(cardD, "��Ʊ��");
	}
	else if (type == CARD_CAP_BUS) {
		strcpy(cardD, "��ͨ��");
	}
	else if (type == CARD_JILIAN_BUS) {
		strcpy(cardD, "���п�");
	}
	else if (type == CARD_YOUFU_BUS)
	{
		strcpy(cardD, "�Ÿ���");
	}
	else if (type == CARD_STUFF) {
		strcpy(cardD, "Ա����");
	}
	else if (type == CARD_DRIVER_BUS) {
		strcpy(cardD, "˾����");

	}
	else if (type == CARD_17RD_BUS) {
		strcpy(cardD, "17ר�߿�");

	}
	else if (type == CARD_JTB)
	{
		strcpy(cardD, "��ͨ��");
	}
	else if (type == CARD_MOTH_CI)
	{
		strcpy(cardD, "�Żݿ�");
	}
	else if (type == CARD_LIANGTONG_STUF)
	{
		strcpy(cardD, "Ա���ֻ���");
	}
	else {
		sprintf(cardD, "%d�ſ�", type);
		//cardD[0] = 0;
		return ST_ERROR;
	}
	return ST_OK;
}

int getCardtype(char *cardD, unsigned char type)
{//�Żݿ���ʾ
	if (gCardinfo.gMCardCand != CARDSTYLE_JTB)
		return getCardtypeHANDAN(cardD, type);
	else
		return getCartypeJTB(cardD, type);

}


//��ʾ��XXXX�������ܴ��˳˳�
void disp_no_swipe(void)
{
	char dnssbuf[20];

	if (gBuInfo.stop_flag)
		return;

	gBuInfo.restore_flag = 1;
	led_off(LED_GREEN | LED_RED | LED_BLUE | LED_YELLOW);
	led_on(LED_RED);
	cls();
	getCardtype(dnssbuf, gCardinfo.card_catalog);
	display(0, 0, dnssbuf, DIS_CENTER);
	display(4, 1, "���ܴ��˳˳�!", DIS_CENTER);

#ifdef _debug_ICcard_
	sleep(5);
#endif

}

//���Ѻ���ʾ��Ϣ��cMode: 0 - M1���� 1 - �ֻ����� 2 - CPU��
void money_msg(unsigned char dmode, unsigned int remM, unsigned int pucM, unsigned char cMOde)
{
	unsigned char disp[50];
	char buffer1[20];


	led_on(LED_GREEN);
	Light_main(QR_LIGHT, LIGHT_OPEN, QR_G, (char *)buffer1);

	gBuInfo.restore_flag = 3;
	// 	if(memcmp(gCardinfo.PublishBicker,(unsigned char*)&last_card_no ,4))
	// 	{
	// 		memcpy((unsigned char*)&last_card_no,gCardinfo.PublishBicker,4);
	// 		gDeviceParaTab.count=1;
	// 	}
	// 	else
	// 	{
	// 		gDeviceParaTab.count++;
	// 	}
	// 
	// 	tmp = gDeviceParaTab.count;
	cls();

	memset(disp, 0, 50);

	if (dmode == ID_REC_TOLL)	//�۷�
	{
		sprintf(buffer1, "���:%d.%02dԪ", remM / 100, remM % 100);
		display(2, 1, buffer1, 0);
		sprintf(buffer1, "�ۿ�:%d.%02dԪ", pucM / 100, pucM % 100);
		display(4, 1, buffer1, 0);

		strcpy(buffer1, "����:");
		if (getCardtype(buffer1 + 5, gCardinfo.card_catalog) == ST_ERROR) {
			sprintf(buffer1 + 5, "%d", gCardinfo.card_catalog);
		}
		display(0, 1, buffer1, 0);

		sprintf(buffer1, "%d.%02d", pucM / 100, pucM % 100);
		LED_Dis3(buffer1);

	}
	else if (dmode == ID_REC_MON)
	{
		if (s_sum1 == 0) {
			display(5, 0, "��ӭ�˳�!", DIS_CENTER);
			if (gCardinfo.card_catalog == CARD_DRIVER_BUS) {
				display(3, 1, "Ա����", DIS_CENTER);
			}
			else {
				getCardtype((char*)buffer1, gCardinfo.card_catalog);
				display(3, 0, buffer1, DIS_CENTER);
			}
		}
		else
		{
			sprintf(buffer1, "���:%d", remM);
			display(2, 1, buffer1, 0);
			sprintf(buffer1, "�۴�:%d", pucM);
			display(4, 1, buffer1, 0);

			sprintf(buffer1, "C%2d", pucM);
			LED_Dis3(buffer1);
		}

		strcpy(buffer1, "����:");
		if (getCardtype(buffer1 + 5, gCardinfo.card_catalog) == ST_ERROR) {
			sprintf(buffer1 + 5, "%d", gCardinfo.card_catalog);
		}
		display(0, 1, buffer1, 0);
	}
	else
	{
		display(0, 5, "���˿�B", DIS_CENTER);
		display(3, 2, "��ӭ�˳�!", 0);
	}
	// 	if(gMCardCand == CARDSTYLE_24CPU){
	// 		display(6, 0, "�ƶ��ֻ���", DIS_CENTER);
	// 	}
	if (gCardinfo.gucEndTimeFlag == 1) {
		display(6, 0, "  �ÿ���Ҫ����  ", 1);
		beep(4, 50, 50);
	}
}


unsigned char OperBlackCard(void)
{
	unsigned char rRecordDeal[170];
	unsigned char i;
	unsigned char buffer[40];
	unsigned char buffer1[16];
	//clr_dog();
	memset(rRecordDeal, 0, 128);
#ifdef _debug_ICcard_
	debugstring("Ente OperBlackCard 1 !!\r\n");
#endif

	for (i = 0; i < gErrorCard.error_pointer; i++)
	{
		if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT) continue;
		else
		{
			if (!memcmp(gErrorCard.ErrorDetail[i].Cardno, (unsigned char *)&gCardinfo.c_serial, 4))
			{
				switch (gErrorCard.ErrorDetail[i].ErrorStep)
				{
				case OPER_BLACK:
					memcpy(buffer, gErrorCard.ErrorDetail[i].ToChange, 16);
					goto error_black;
				default:
					break;
				}
			}
		}
	}

	if (gCardinfo.publicMessageSector >= 16)
	{
		cardSound = 0;
		//WriteRecord((unsigned char*)&rRecordDeal, 0);
		WriRecorJTB(rRecordDeal, 1);
		ErrorOper(IS_EFFECT);
		return ST_OK;
	}

	if (MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, gCardinfo.PublichKEYA, gCardinfo.publicMessageSector * 4) == 0)
	{
		return ST_ERROR;
	}

	//��������Ϣ
	if (MifareRead(gCardinfo.publicMessageSector * 4, buffer) == 0)
	{
		return ST_ERROR;
	}

	buffer[6] = 0x04;
	buffer[15] = MiBlockInvalid(0, 15, buffer);
error_black:
	memcpy(buffer1, buffer, 16);

	if (MifareWrite(gCardinfo.publicMessageSector * 4, buffer) == 0) return ST_ERROR;
	for (i = 0; i < 16; i++)
		buffer[i] = ~buffer1[i];
	if (MifareWrite(gCardinfo.publicMessageSector * 4 + 1, buffer) == 0) return  ST_ERROR;

	BuildRecorde(ID_REC_BLKLIST, rRecordDeal);
#ifdef _debug_ICcard_
	debugstring("�û���¼1\r\n");
	debugdata((unsigned char*)&rRecordDeal, RECORD_LEN, 1);
#endif

	cardSound = 0;

	//WriteRecord((unsigned char*)&rRecordDeal, 0);
	WriRecorJTB(rRecordDeal, 1);
	ErrorOper(IS_EFFECT);
	return ST_OK;
}

void Save_delayCardinfo(unsigned char Cmode)
{
#ifdef _HD_XIAOER_
	save_h_month();
#else

	//����       ��ͨ������ͨ�ֻ��������п�֮�ⲻ����ˢ
	if ((gCardinfo.card_catalog != CARD_NORMAL) && (gCardinfo.card_catalog != CARD_CAP_BUS)
		&& (gCardinfo.card_catalog != CARD_JILIAN_BUS) && (gCardinfo.card_catalog != CARD_MOTH_CI))
	{

		save_h_month();
	}

	else
		save_normal();
#endif
}

//������ʾ����
void Sound_cityCMD(unsigned char sMode)
{
#ifdef _debug_ICcard_
	debugstring("gucSoudnorm1=");
	debugdata((unsigned char*)&gCardinfo.gucSoudnorm, 1, 1);
	debugstring("Sound_cityCMD: MODE::");
	debugdata(&sMode, 1, 1);
#endif

	if (gCardinfo.gucSoudnorm == 0) {
		if (sMode != 0) {
#ifdef _debug_ICcard_
			debugstring("��Ǯ����dong!!!!\r\n");
#endif
			audio(Audio_DONG);//����Ʊ��Ǯ����ͨ����
		}
		else
			SoundMessage(SOUND_DEAL);
	}
	else
		SoundMessage(SOUND_CHARGE_MONEY);	//��ʾ���ֵ

}



void ControlDis(unsigned char type)
{
	char buffer[20];
	unsigned int temp;

	cls();
	memset(buffer, 0, 20);
	memcpy(buffer, (unsigned char *)&"����:", 5);
	memcpy((unsigned char*)&temp, gCardinfo.PublishBicker, 4);

	//	over_turn(4, (unsigned char*)&temp);
	sprintf(buffer + 5, "%010u", temp);
	//	memcpy(buffer+5,BCD2ASC(temp.charbuffer,4),8);
	display(3, 0, buffer, DIS_CENTER);
	led_on(LED_GREEN);
	switch (type)
	{
	case CARD_DATA_GATHER:
		display(0, 2, "���ݲɼ���", 1);
		beep(1, 80, 20);
		break;
	case CARD_LINE_SET:
		display(0, 2, "��·Ʊ�ۿ�", 1);
		beep(1, 80, 20);
		break;
	case CARD_TIME_SET:
		display(0, 2, "ʱ�����ÿ�", 1);
		beep(1, 80, 20);
		break;
	case CARD_LEV_BUS:

		display(0, 3, "������", 1);

		beep(1, 80, 20);
		break;
	case CARD_ARV_BUS:
		display(0, 3, "��վ��", 1);
		beep(1, 80, 20);
		break;
	case CARD_CLERK_BUS:
		display(0, 4, "վ��Ա��", 1);
		beep(1, 80, 20);
		break;
	case CARD_DEV_SET:
		display(0, 3, "�豸���ÿ�", 1);
		beep(1, 80, 20);
		break;

	case CARD_DRIVER_BUS:
		display(0, 5, "˾����", 1);
		beep(1, 80, 20);
		break;
	case CARD_AIR_BUS:
		display(0, 5, "�յ��շѿ�", DIS_CENTER);
		beep(1, 80, 20);
		break;
	default:
		break;
	}

}


//�л������������շѻ�ر��շ� 1 �� 2�ر�
void SwitchBusClose(unsigned char ucClose)
{
	gBuInfo.restore_flag = 1;
	gBuInfo.stop_flag = ucClose;
	save_file_BuInfo();
	return;
}

void wRiteControlRecord(unsigned char ID)
{
	unsigned char rRecord[170];

	memset(rRecord, 0, 128);
	a_sum1 = 0;
	s_sum1 = 0;
#ifdef _debug_
	debugstring("W C R 0:");
#endif
	BuildRecorde(ID, rRecord);
	memset(rRecord + 124, 0xBB, 4);
#ifdef _debug_
	debugstring("W C R 1:");
#endif
	//	WriteRecord(rRecord, 0);
	WriRecorJTB(rRecord, 1);
#ifdef _debug_
	debugstring("W C R 2:");
#endif
	return;
}

unsigned char DriverCardDeal(void)
{
	unsigned int i;

	BUflag = 0;

	if (gBuInfo.stop_flag)
	{	//����
		memcpy(gBuInfo.DriverNO, gCardinfo.PublishBicker, 4);
		memcpy(gBuInfo.StufferNO, gCardinfo.stuffNO, 4);

		beep(2, 50, 50);
		led_on(LED_RED);

		ControlDis(gCardinfo.card_catalog);
		//���ĵ�վ״̬��־
		SwitchBusClose(0);
		wRiteControlRecord(ID_REC_LEVBUS);				//��������¼
		wRiteControlRecord(ID_REC_DRIVENO);				//˾������¼

		return ST_OK;
	}
	else
	{//��վ
		i = memcmp(gBuInfo.DriverNO, gCardinfo.PublishBicker, 4);
		if (i != 0) {//����ͬһ��˾����,����

			return ST_ERROR;
		}

		memcpy(gBuInfo.DriverNO, gCardinfo.PublishBicker, 4);
		memcpy(gBuInfo.StufferNO, gCardinfo.stuffNO, 4);

		SoundMessage(SOUND_DI_BUS);

		ControlDis(gCardinfo.card_catalog);
		wRiteControlRecord(ID_REC_DRIVENO);				//˾������¼
#ifndef _BU_NO_STOP_
		SwitchBusClose(1);
		wRiteControlRecord(ID_REC_ARVBUS);			//��վ����¼
#else
		SwitchBusClose(0);
		beep(2, 100, 300);
		led_on(LED_RED);
#endif

		return ST_OK;
	}
}

void set_line(void)
{
	unsigned char i, j;
	unsigned char s_no;
	unsigned char buffer1[150];
#ifdef line_older_	
	unsigned short temp1 = 0;
#endif
	unsigned char LineNoOld[8];
	unsigned char Key[20];

	cls();

	display(4, 3, "���Ե�......", 0);

	// 	InitRC531();
	// 	profile(0);

	memset(buffer1, 0, 120);
#ifdef BUS_PRO_JSB
#ifndef line_older_//�Ž������ڵĿ��ƿ�Ҳȫ��ȡ��Կ
	memcpy(Key, gCardinfo.RecodeKEYA, 6);
#else
	memcpy(Key, gCardinfo.PublishKEYA, 6);  	//����·��   
#endif
#else
	memcpy(Key, gCardinfo.RecodeKEYA, 6);
#endif
#ifdef _debug_
	debugstring("set Line No: Key: RecodeKEYA:");
	debugdata(Key, 6, 1);
	debugdata((unsigned char*)&gCardinfo.c_serial, 4, 1);
#endif
	s_no = 3;
	for (i = 0; i < 5; i++) {
		MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, Key, s_no * 4);
#ifdef _debug_
		debugstring("set Line No 2 s_no:");
		debugdata(Key, 6, 1);
#endif
		if (MifareRead(s_no * 4, buffer1) != 0) {
			break;
		}

		for (j = 0; j < 5; j++) {
			if (profile(0) != ST_ERROR)
			{
				break;
			}
			else {
				usleep(1000);
			}
		}
	}
	if (i >= 3)
		return;
#ifdef _debug_
	debugstring("set Line No 3 buf:");
	debugdata(buffer1, 16, 1);
#endif
	memcpy(LineNoOld, gDeviceParaTab.LineNo, 3);//�ɵ���·��
#ifndef line_older_	//����·��
	gDeviceParaTab.LineNo[0] = buffer1[0];//��·��  BCD��
	gDeviceParaTab.LineNo[1] = buffer1[1];//
#else
	memcpy((unsigned char*)&temp1, buffer1, 2);	//����·�� 
	sprintf((char*)buffer1 + 60, "%4d", temp1);

	gDeviceParaTab.LineNo[0] = (((buffer1[60] - '0') & 0x0f) << 4) + ((buffer1[61] - '0') & 0x0f);//��·��
	gDeviceParaTab.LineNo[1] = (((buffer1[62] - '0') & 0x0f) << 4) + ((buffer1[63] - '0') & 0x0f);//
#endif

	if (buffer1[2] == 0)
		buffer1[2] = 1;
	sprintf((char*)buffer1 + 60, "%2d", buffer1[2]);
	gDeviceParaTab.LineNo[2] = (((buffer1[60] - '0') & 0x0f) << 4) + ((buffer1[61] - '0') & 0x0f);//��·�ֺ�
																								  //	pFistVary.LineNo[2] = 0; 

	memset(gDeviceParaTab.busPrice, 0, 4);
	memcpy(gDeviceParaTab.busPrice, buffer1 + 3, 2);//Ʊ��

	memset(gDeviceParaTab.plusPrice, 0, 4);
	memcpy(gDeviceParaTab.plusPrice, buffer1 + 5, 2);//���ӷ�

	memcpy((unsigned char*)&gDeviceParaTab.ChangeType, buffer1 + 11, 3);//���������ֽڷֱ��ǣ� �����Żݿ�����ʶ�������ۿ��ʣ�����ʱ��
	if (MifareRead(s_no * 4 + 1, buffer1) == 0) {
		return;
	}
#ifdef _debug_
	debugstring("set Line No 4 buf:");
	debugdata(buffer1, 16, 1);
#endif	
	for (i = 0; i < 8; i++)
	{
		gDeviceParaTab.rate[i] = buffer1[2 * i];//40H~47H��˿Ϳ���Ʊ���ۿ��ʡ����ӷ��ۿ���
		gDeviceParaTab.plusrate[i] = buffer1[2 * i + 1];//40H~47H���ӷ��ۿ��� �ָ�Ϊ�Ƿ���ˢ־
	}
	if (MifareRead(s_no * 4 + 2, buffer1) == 0) {
		memcpy(gDeviceParaTab.LineNo, LineNoOld, 3);//�ɵ���·��
		return;
	}
#ifdef _debug_
	debugstring("set Line No 5 buf:");
	debugdata(buffer1, 16, 1);
#endif
	for (i = 8; i < 16; i++)
	{
		gDeviceParaTab.rate[i] = buffer1[2 * (i - 8)];//48H~4FH��˿Ϳ���Ʊ���ۿ��ʡ����ӷ��ۿ���
		gDeviceParaTab.plusrate[i] = buffer1[2 * (i - 8) + 1];//40H~47H���ӷ��ۿ��� �ָ�Ϊ�Ƿ���ˢ־
	}

	s_no++;
	if (MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, Key, s_no * 4) == 0)
	{
		memcpy(gDeviceParaTab.LineNo, LineNoOld, 3);//�ɵ���·��
		debugstring("MifareAuthKey error\r\n ");
		return;
	}
#ifdef _debug_
	debugstring("set Line No 6 Key:");
	debugdata(Key, 6, 1);
#endif

	if (MifareRead(s_no * 4, buffer1) == 0) {
		memcpy(gDeviceParaTab.LineNo, LineNoOld, 3);//�ɵ���·��
		return;
	}
#ifdef _debug_
	debugstring("set Line No 7 buf:");
	debugdata(buffer1, 16, 1);
#endif
	for (i = 16; i < 24; i++)
	{
		gDeviceParaTab.rate[i] = buffer1[2 * (i - 16)];//50H~57H��˿Ϳ���Ʊ���ۿ��ʡ����ӷ��ۿ���
		gDeviceParaTab.plusrate[i] = buffer1[2 * (i - 16) + 1];//40H~47H���ӷ��ۿ��� �ָ�Ϊ�Ƿ���ˢ־
	}

	//	if(MifareRead(s_no*4+1,buffer1)==0) return;
	MifareRead(s_no * 4 + 1, buffer1);
#ifdef _debug_
	debugstring("set Line No 8 buf:");
	debugdata(buffer1, 16, 1);
#endif
	for (i = 24; i < 32; i++)
	{
		gDeviceParaTab.rate[i] = buffer1[2 * (i - 24)];//58H~5FH��˿Ϳ���Ʊ���ۿ���
		gDeviceParaTab.plusrate[i] = buffer1[2 * (i - 24) + 1];//40H~47H���ӷ��ۿ��� �ָ�Ϊ�Ƿ���ˢ־
	}

	s_no += 1;		//��������������
	if (MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, Key, s_no * 4) == 0)
	{
		debugstring("MifareAuthKey error\r\n ");
		return;
	}

	if (MifareRead(s_no * 4, buffer1) == 0) return;

	for (i = 0; i < 16; i++)
	{
		gDeviceParaTab.voice[i] = i;//40H~4FH��˿Ϳ�������
	}

	if (MifareRead(s_no * 4 + 1, buffer1) == 0) return;
	for (i = 0; i < 16; i++)
	{
		gDeviceParaTab.voice[i + 16] = i + 16;//50H~5FH��˿Ϳ�������
	}

	memcpy(gDeviceParaTab.citycode, "\x07\x55", 2);


	//	SetAudio();
#ifdef _debug_
	debugstring("LineNo1:");
	debugdata(gDeviceParaTab.LineNo, 3, 1);
#endif
	memcpy(buffer1, gDeviceParaTab.LineNo, 3);

#ifdef _debug_
	debugdata((unsigned char*)&gDeviceParaTab, sizeof(gDeviceParaTab), 1);
#endif

	gBuInfo.set_line_status = 0;
	gBuInfo.restore_flag = 1;
	display(4, 3, "�������!", 0);

#ifdef _debug_
	debugstring("LineNo2:");
	debugdata(gDeviceParaTab.LineNo, 3, 1);
#endif

	save_file_DevicePara();
	a_sum1 = 0;
	s_sum1 = 0;
	beep(1, 100, 10);
	SoundMessage(SOUND_DI_BUS);

	memset(buffer1, 0, 128);
	memset(buffer1 + 124, 0xBB, 4);//�����һ��Ҫ��4��BB
	BuildRecorde(ID_REC_LINENO, buffer1);
#ifdef _debug_
	debugstring("LineNo3:");
	debugdata(gDeviceParaTab.LineNo, 3, 1);
#endif

	WriRecorJTB(buffer1, 1);
#ifdef _debug_
	debugstring("LineNo4:");
	debugdata(gDeviceParaTab.LineNo, 3, 1);
#endif
#ifdef _debug_
	debugstring("Set line END!!!\r\n");
#endif
}

void ControlMagane(void)
{

#ifdef _debug_
	debugstring("--control manage---\r\n");
#endif
	gBuInfo.restore_flag = 3;

	switch (gCardinfo.card_catalog)
	{
	case CARD_ARV_BUS:
#ifdef BUS_HANDAN_
		if (gBuInfo.stop_flag)
		{
			led_on(LED_RED);
			return;
		}
#endif
		break;
	case CARD_LEV_BUS:
		break;
	case CARD_DEV_SET:
	case CARD_LINE_SET:
	case CARD_TIME_SET:
	case CARD_AIR_BUS:
	case CARD_CLERK_BUS:
		if (!gBuInfo.stop_flag)
		{
			MessageBox(1, "����ʱ����ˢ�˿�");
			led_on(LED_RED);

			end_card();
			return;
		}
		break;
	case CARD_DRIVER_BUS:
	default:
		break;
	}


	switch (gCardinfo.card_catalog)
	{
		//#ifdef BUS_PRO_JSB
	case CARD_DATA_GATHER:
		beep(1, 100, 10);

		//		get_data(0);
		gBuInfo.restore_flag = 1;
		break;
	case CARD_DEV_SET:
		beep(1, 100, 10);
		//		ShowSetInfo();
		//		set_device();
		break;
	case CARD_LINE_SET:
		if (gBuInfo.set_line_status) {
			set_line();
			//			SetAudio();//��������ǹ̶����ģ���������·������Ҫ���³�ʼ��.����������Ҫ�ٵ���һ��
		}
		else {
			cls();
			display(2, 2, "��ˢվ��Ա��", 0);
		}
		break;
	case CARD_FENDUAN_Line:
		// #ifdef FengDuan_GPS_
		// 		set_FengDuan_line();
		// #endif
		break;
	case CARD_TIME_SET:
		beep(1, 100, 10);
		//		setdatetime();
		break;
	case CARD_ARV_BUS:	//�����ĵ�վ�� ,ֻ�ܵ�վ
	case CARD_LEV_BUS:
		//		debugstring("bb2");

		if (gBuInfo.stop_flag)
		{	//����
			//		debugstring("bb3");
			SoundMessage(SOUND_DI_BUS);
			memcpy(gBuInfo.DriverNO, gCardinfo.PublishBicker, 4);
			save_file_BuInfo();
			ControlDis(gCardinfo.card_catalog);
			//���ĵ�վ״̬��־
			SwitchBusClose(0);
			wRiteControlRecord(ID_REC_LEVBUS);				//��������¼
			wRiteControlRecord(ID_REC_DRIVENO);				//˾������¼
		}
		else
		{//��վ
		 //		debugstring("bb4");
			SoundMessage(SOUND_DI_BUS);
			ControlDis(gCardinfo.card_catalog);
			SwitchBusClose(1);
			wRiteControlRecord(ID_REC_DRIVENO);				//˾������¼
			wRiteControlRecord(ID_REC_ARVBUS);			//��վ����¼

		}
		break;
	case CARD_DRIVER_BUS:
		DriverCardDeal();
		break;
	case CARD_CLERK_BUS:
		if (gBuInfo.stop_flag)
		{
			ControlDis(CARD_CLERK_BUS);
			gBuInfo.restore_flag = 1;
			wRiteControlRecord(ID_REC_CLERK);
			gBuInfo.set_line_status = 1;
			SoundMessage(SOUND_DI_BUS);
		}
		break;
	case CARD_AIR_BUS:
		ControlDis(CARD_AIR_BUS);
		if (gBuInfo.PlusSta == 0)
			gBuInfo.PlusSta = 1;
		else
			gBuInfo.PlusSta = 0;
		if (gBuInfo.PlusSta == 0) {
			display(6, 0, "�յ��ѹر�", DIS_CENTER);
		}
		else {
			display(6, 0, "�յ��Ѵ�", DIS_CENTER);
		}
		gBuInfo.restore_flag = 1;
		save_file_BuInfo();
		wRiteControlRecord(ID_REC_AIRCON);
		break;
	}
	end_card();
	gBuInfo.restore_flag = 1;
	return;
}


unsigned char FreeConsume(void)
{
	unsigned char buffer[170];
	unsigned char mothFlag;
	//	unsigned int ii;
	memset(buffer, 0, 128);

	cardSound = 0;

	if (gBuInfo.stop_flag)
		return ST_ERROR;

	gBuInfo.restore_flag = 1;
	mothFlag = gCardinfo.card_catalog;
	if (mothFlag >= 0x40)
		mothFlag -= 0x40;
	if (month_decide() != 0) // �ڶ���ȥ��Ǯ��(����������ˢ)
	{
		disp_no_swipe();
		return ST_OK;
	}

	s_sum1 = 0;
	dis_sum2 = 0;
	a_sum1 = 0;

	money_msg(ID_REC_MON, 0, 0, 0);
	if (gCardinfo.card_catalog == CARD_DRIVER_BUS)
		audio(Audio_STUFF);
	else
		SoundMessage(SOUND_DEAL);

	save_h_month();	//"�¿����Żݿ�,��������ʹ��!"

	BuildRecorde(ID_REC_MON, buffer);
	//	WriteRecord(buffer, 0);
	WriRecorJTB(buffer, 1);
	gBuInfo.SumMti += s_sum1;

	end_card();
	return ST_OK;
}


//д�����׼�¼
//�޸Ĺ���������־
unsigned char PurchasOk(unsigned char mode)
{
	cardRecord1 card_record;
	unsigned char snPo = 0, s_no = 0;
	unsigned char buffer_1[50];
	unsigned char buffer_2[20];
	unsigned char i, ii;
	unsigned char sector;
	unsigned int itemp;
	unsigned char buffer[16];
	unsigned char buffer_3[16];
	//	unsigned char err_po;
	//	unsigned char err_po1;
	//	unsigned char err=ST_OK;
	//	unsigned char err1=ST_OK;
	unsigned short stemp = 0;

	for (i = 0; i < gErrorCard.error_pointer; i++)
	{
		if (mode == 1) {
			if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT_MONTH) continue;
		}
		else if (mode == 0) {
			if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT_MONEY) continue;
		}
		if (memcmp(gErrorCard.ErrorDetail[i].Cardno, (unsigned char *)&gCardinfo.c_serial, 4) == 0)
		{
			switch (gErrorCard.ErrorDetail[i].ErrorStep)
			{
			case OPER_RECORD_PROFILE:		//д���м�¼����	
//				err_po=i;
//				err=ST_ERROR;          //˵���д�����
				break;
			case OPER_PUBLIC_MESSAGE:
				//				err_po1=i;
				//				err1=ST_ERROR;
				goto continue_pulic_message;
			default:
				break;
			}
		}
	}

#ifdef _debug_ICcard_
	debugstring("PurchasOk --1\r\n");
#endif

#ifdef FengDuan_BU_
	if ((CARD_PUBLIC.DealPo == 0) || (CARD_PUBLIC.DealPo > 6))
#else
	if ((CARD_PUBLIC.DealPo == 0) || (CARD_PUBLIC.DealPo > 9))
#endif
		CARD_PUBLIC.DealPo = 1;
	snPo = CARD_PUBLIC.DealPo;
#ifdef _debug_ICcard_
	debugdata((unsigned char *)&snPo, 1, 1);
#endif
	itemp = a_sum1 + s_sum1;
	memcpy(card_record.Balance, (unsigned char*)&itemp, 4);
	memcpy(card_record.DealMoney, (unsigned char*)&s_sum1, 3);
	memcpy(card_record.DealTime, &SysTime.day, 4);
	card_record.DealType = mode;
	//	Ascii2BCD(gDeviceParaTab.DeviceNo, card_record.DeviceNo, 8);
	Ascii2BCD(gDeviceParaTab.DeviceNo, card_record.DeviceNo, 4);

	if (gCardinfo.dealRecordSector[0] < 16) {
		sector = ((snPo - 1) / 3) + gCardinfo.dealRecordSector[0];
		//		Debugdata(1,1,(unsigned char *)&s_no);	
		if (MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, gCardinfo.RecodeKEYA, sector * 4) == 0)
		{
			CheckSno(mode, OPER_RECORD_PROFILE, (unsigned char*)&card_record);
			//			debugstring("MifareAuthKey error\r\n ");
			return ST_ERROR;
		}

		//		debugstring("PurchasOk --2\r\n");
		s_no = ((((snPo - 1) / 3) + 3) * 4) + (snPo - 1) % 3;
		memset(buffer_1, 0, 16);
		memcpy(buffer_1, (unsigned char*)&card_record, LEN_CARD_RECORD);
		//		debugstring("PurchasOk --3\r\n");

#ifdef _debug_ICcard_
		debugstring("д����¼:\r\n");
		debugdata(buffer_1, 16, 1);
#endif
		if (MifareWrite(s_no, buffer_1) == 0)
		{
			CheckSno(mode, OPER_RECORD_PROFILE, (unsigned char*)&card_record);
			//		debugstring("д����¼ERROR\r\n");
			return ST_ERROR;
		}
	}
	///////////////////////////////////////////////////////////////////////////////
//�޸Ĺ�������
continue_pulic_message:
	if (gCardinfo.publicMessageSector >= 16) {//û�й�����Ϣ��
		return ST_OK;
	}

	CARD_PUBLIC.DelaFlag = 0x02;

	if (MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, gCardinfo.PublichKEYA, gCardinfo.publicMessageSector * 4) == 0)
	{
		debugstring("MifareAuthKey error\r\n ");
		CheckSno(mode, OPER_PUBLIC_MESSAGE, (unsigned char*)&CARD_PUBLIC);
		return ST_ERROR;
	}
#ifdef _debug_ICcard_
	debugstring("PurchasOk --6\r\n");
#endif

	CARD_PUBLIC.DealPo += 1;
#ifdef FengDuan_BU_
	if (CARD_PUBLIC.DealPo > 6)
#else
	if (CARD_PUBLIC.DealPo > 9)
#endif
		CARD_PUBLIC.DealPo = 1;
	// 	if(mode==1)											
	// 	{
	// 		memcpy((unsigned char*)&stemp, CARD_PUBLIC.MonthDealSum, 2);
	// 		stemp+=1;
	// 	}
	// 	if(mode==0)
	// 	{
	memcpy((unsigned char*)&stemp, CARD_PUBLIC.MoneyDealSum, 2);
	stemp += 1;
	// 	}
	memcpy(CARD_PUBLIC.MoneyDealSum, (unsigned char*)&stemp, 2);

	memset(buffer_1, 0, 16);
	memcpy(buffer_1, (unsigned char*)&CARD_PUBLIC, 8);
	// 	buffer_1[Public_dealpo] = CARD_PUBLIC.DealPo;
	// 	buffer_1[Public_blackflag] = CARD_PUBLIC.BlackFlag;
	// 	buffer_1[Public_delaflag] = CARD_PUBLIC.DelaFlag;
	// 	buffer_1[Public_fileflag] = CARD_PUBLIC.FileFlag;
	// 	memcpy(buffer_1+Public_moneydealsum, (unsigned char*)&CARD_PUBLIC.MoneyDealSum, 2);
	// 	memcpy(buffer_1+Public_monthdealsum, (unsigned char*)&CARD_PUBLIC.MonthDealSum, 2);

	buffer_1[15] = MiBlockInvalid(0, 15, buffer_1);
	if (MifareWrite(gCardinfo.publicMessageSector * 4, buffer_1) == 0)
	{
		CheckSno(mode, OPER_PUBLIC_MESSAGE, (unsigned char*)&CARD_PUBLIC);
		return ST_ERROR;
	}
	for (i = 0; i < 16; i++)
	{
		buffer_2[i] = ~buffer_1[i];
	}

	if (MifareWrite(gCardinfo.publicMessageSector * 4 + 1, buffer_2) == 0)
	{
		return ST_ERROR;
	}
#ifdef _debug_ICcard_
	debugstring("Purchase OK End!\r\n");
#endif



	//***********************************************************

	if (gDeviceParaTab.ChangeType == 0xAA)
	{
		// ���к���Ϣ
		for (i = 0; i < 3; i++)
		{
			memset(buffer, 0, 16);
			memset(buffer_2, 0, 16);
			memset(buffer_3, 0, 16);
			memcpy(buffer_3, (unsigned char*)&SysTime, 7);
			memcpy(buffer_3 + 7, buffer_2, 3);//��·��
			Get_SerialNum(buffer_3 + 7 + 3);//�ĳ����кŰ�
		//	Ascii2BCD(gDeviceParaTab.DeviceNo,buffer_3+7+3,8); //�����豸��� 2016-7-28 15:54:49
			if (memcmp(old_Infor.resear, "\xAA\xAA", 2) == 0)//�Ѿ��Żݹ���
				memset(buffer_3, 0, 16);
			buffer_3[15] = MiBlockInvalid(0, 15, buffer_3);//У����
#ifdef _debug_ICcard_
			debugstring("д����ʱ����豸�ŵ�6������");
			debugdata(buffer_3, 16, 1);
#endif
			if (MifareWrite(gCardinfo.publicMessageSector * 4 + 2, buffer_3) == 0)
			{
				CheckSno(mode, OPER_PUBLIC_MESSAGE, (unsigned char*)&CARD_PUBLIC);
				//д��������
#ifdef _debug_ICcard_
				debugstring("��������д ������");
#endif
				return ST_ERROR;
			}
			//д�� �ٶ������Ƚ�
			if (MifareRead(gCardinfo.publicMessageSector * 4 + 2, buffer) == 0)
			{
#ifdef _debug_ICcard_
				debugstring("6������Ϣ2������\r\n");
#endif
				CheckSno(mode, OPER_PUBLIC_MESSAGE, (unsigned char*)&CARD_PUBLIC);
				return ST_ERROR;
			}
			if (MiBlockInvalid(1, 15, buffer) == ST_ERROR)
			{
#ifdef _debug_ICcard_
				debugstring("6������Ϣ2 У���\r\n");
#endif
				CheckSno(mode, OPER_PUBLIC_MESSAGE, (unsigned char*)&CARD_PUBLIC);

				return ST_ERROR;
			}

#ifdef _debug_ICcard_
			debugstring("6������Ϣ2:");
			debugdata(buffer, 16, 1);
#endif
			if (memcmp(buffer, buffer_3, 16) == 0)
			{
#ifdef _debug_ICcard_
				debugstring("6������Ϣ2����У����ȷ:");
				debugdata(buffer, 16, 1);
#endif
				break;
			}

		}
	} //BUS_NanCong_ �ϳ��Ż���Ϣ
		//*********************************************************
	for (i = 0; i < gErrorCard.error_pointer; i++)
	{
		if (mode == 1) {
			if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT_MONTH) continue;
		}
		else if (mode == 0) {
			if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT_MONEY) continue;
		}
		if (memcmp(gErrorCard.ErrorDetail[i].Cardno, (unsigned char *)&gCardinfo.c_serial, 4) == 0)
		{
			switch (gErrorCard.ErrorDetail[i].ErrorStep)
			{
			case OPER_PUBLIC_MESSAGE_W:
			case OPER_RECORD_PROFILE:
			case OPER_PUBLIC_MESSAGE:
				memcpy((unsigned char*)&a_sum1, gErrorCard.ErrorDetail[i].AferToll, 4);
				memcpy((unsigned char*)&s_sum1, gErrorCard.ErrorDetail[i].TollValue, 4);
				break;
			}
			for (ii = i; ii < gErrorCard.error_pointer; ii++)
				memcpy((unsigned char *)&gErrorCard.ErrorDetail[ii], (unsigned char *)&gErrorCard.ErrorDetail[ii + 1], ERROR_DETAIL_LEN);
			gErrorCard.error_pointer -= 1;
			memset((unsigned char *)&gErrorCard.ErrorDetail[gErrorCard.error_pointer], 0, ERROR_DETAIL_LEN);
			break;
		}
	}
	for (i = 0; i < 1; i++)  //ERROR_DETAIL_SUM
	{
		memset(buffer_1, 0, 32);
		memcpy(buffer_1, (unsigned char *)&gErrorCard.ErrorDetail[i], ERROR_DETAIL_LEN);
	}

	memset(buffer_1, 0, 2);
	buffer_1[0] = gErrorCard.error_pointer;
	save_file_ErrorCard();
	return ST_OK;

}


//����ˢ�ָ�����  �����ָ�����
unsigned char  MoneyCoverly(unsigned char *buffer)
{
	unsigned char i;
	//	unsigned char b1,b2;
	unsigned char buffer_9[20];



	memcpy(buffer_9, buffer, 16);
	if (MifIsPurse(buffer_9) == ST_ERROR)
	{
		return ST_ERROR;
	}
#ifdef _debug_
	debugstring("Ǯ���鸱����\r\n");
	debugdata(buffer, 16, 1);
#endif

	for (i = 0; i < 2; i++)
	{
		memset(buffer, 0, 20);
		if (MifareValueDebit(PICC_DECREMENT, gCardinfo.publicMoneySector * 4 + 2, buffer))//����
			break;
	}
	if (i >= 2)
		return ST_ERROR;

	for (i = 0; i < 2; i++)
	{

		if (MifareTransfer(gCardinfo.publicMoneySector * 4 + 1))
			break;
	}

	if (i >= 2)
	{
		//	CheckSno(ERROR_MONEY,OPER_TRANSFER_NO,buffer);
		return 4;
	}

	return ST_OK;

}

//Ǯ�������Ϸ�ʽ
unsigned char PurseProcess(unsigned char mode)
{  //mode=0
	unsigned char i = 0, b1, b2;
	unsigned char buffer[60];
	unsigned char buffer_9[16];
	unsigned char Check_TRANSFER = ST_OK;
	unsigned char buffer_10[16];
	unsigned char block9 = ST_OK;
	unsigned char block10 = ST_ERROR;
	//	unsigned int price,plus;
	//	unsigned char price_ratio,plus_ratio;//,r;
	unsigned char block910 = ST_OK;
	unsigned int eemoney = 0, moneyB2, blockB2;//,temp10,tempp;
	unsigned char count = 0;
#ifdef _debug_ICcard_
	debugstring("Purse Process s\r\n");
#endif
	//���������
	for (i = 0; i < gErrorCard.error_pointer; i++)
	{
		if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT_MONEY) continue;
		else
		{
			if (memcmp(gErrorCard.ErrorDetail[i].Cardno, &gCardinfo.c_serial, 4) == 0)
			{
				switch (gErrorCard.ErrorDetail[i].ErrorStep)
				{
				case OPER_TRANSFER_NO:
				{
					memcpy((unsigned char *)&eemoney, gErrorCard.ErrorDetail[i].AferToll, 4);
					memcpy((unsigned char *)&s_sum1, gErrorCard.ErrorDetail[i].TollValue, 4);
					Check_TRANSFER = ST_ERROR;
					break;
				}
				case OPER_RECORD_PROFILE:
				case OPER_PUBLIC_MESSAGE:
				case OPER_PUBLIC_MESSAGE_W:
					return 5;
				default:
					break;
				}
			}
		}
	}
#ifdef _debug_ICcard_
	debugstring("read Money Block:;");
#endif


	if (cardSound == 0xcc)//������Ϣ��ˢ
		return 5;
	//	for(i=0;i<3;i++){
	if (MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, gCardinfo.MoneyKEYA, gCardinfo.publicMoneySector * 4) == 0)
	{
		debugstring("Purse Process MifareAuthKey error return\r\n ");
		return ST_ERROR;
	}
again:
	//��Ǯ��������
	if (MifareRead(gCardinfo.publicMoneySector * 4 + 1, buffer) == 0)
	{
		debugstring("MifareRead money*4+1 ERROR!\r\n");
		return ST_ERROR;
	}
#ifdef _debug_ICcard_
	debugstring("Ǯ����������\r\n");
	debugdata(buffer, 16, 1);
#endif
	memcpy(buffer_9, buffer, 16);

	if (MifareRead(gCardinfo.publicMoneySector * 4 + 2, buffer) == 0)
	{
		debugstring("MifareRead money*4+2 ERROR!\r\n");
		return ST_ERROR;
	}
	memcpy(buffer_10, buffer, 16);

#ifdef _debug_ICcard_
	debugstring("Ǯ���鸱����\r\n");
	debugdata(buffer, 16, 1);
#endif

	if (MifIsPurse(buffer_9) == ST_ERROR)
	{
		block910 = ST_ERROR;
		block9 = ST_ERROR;
	}
	else {
		block9 = ST_OK;
		block910 = ST_OK;
	}
	if (MifIsPurse(buffer_10) == ST_ERROR)
	{
		block10 = ST_ERROR;
		block910 = ST_OK;
	}
	else {

		block10 = ST_OK;
	}

	//����ˢ  �����Ǻõľ��Ȼָ�
	if ((cardSound == 0xaa) && (card_ser == gCardinfo.c_serial) && (block9 == ST_ERROR) && (block10 == ST_OK))
	{
		if (MoneyCoverly(buffer_10) != ST_OK)
			return ST_ERROR;

		if (count++ < 3)
			goto again;

		return 42;
		//	 block910=ST_ERROR;
	}

	if ((block9 == ST_ERROR) && (block10 == ST_ERROR))
		return RET_MONEY_SN_EROR;

	if ((block9 == ST_OK) && (block10 == ST_OK)) {
		block910 = ST_OK;
		for (i = 0; i < 4; i++)
		{
			if (buffer_9[3 - i] > buffer_10[3 - i])
			{
				block910 = ST_ERROR;
				break;
			}
			if (buffer_9[3 - i] < buffer_10[3 - i])
			{
				block910 = ST_OK;
				break;
			}
		}
	}


	if (block910 == ST_ERROR) //�����쳣
		memcpy(buffer_9, buffer_10, 4);

	if (block910 == ST_ERROR) {

		b1 = gCardinfo.publicMoneySector * 4 + 2;
		b2 = gCardinfo.publicMoneySector * 4 + 1;
		memcpy((unsigned char*)&moneyB2, buffer_9, 4);
		blockB2 = block9;//�ڶ����Ƿ��Ǻõġ�
	}
	else {
		b1 = gCardinfo.publicMoneySector * 4 + 1;
		b2 = gCardinfo.publicMoneySector * 4 + 2;
		memcpy((unsigned char*)&moneyB2, buffer_10, 4);
		blockB2 = block10;
	}

	memcpy((unsigned char*)&a_sum1, buffer_9, 4);

	s_sum1 = get_s_sum1(mode);

	//�û�ԭ��
#ifdef _debug_ICcard_
	debugstring("��ԭ��:");
	debugdata((unsigned char *)&a_sum1, 4, 1);
	debugstring("moneyB2:");
	debugdata((unsigned char *)&moneyB2, 4, 1);
#endif

	if (mode == 100) return ST_OK;

	if ((Check_TRANSFER == ST_ERROR))
	{

		if (a_sumR >= a_sum1)//˵������  
		{
			if (block10 == ST_OK)
				goto mey_step90;
			else
				ErrorOper(IS_EFFECT_MONEY);
			return 5;
		}

	}

	gCardinfo.gucSoudnorm = 0;
#ifdef BUS_JIUJIANG_ //��Ǯ�����С��3Ԫʱ	���ֵ
	if ((a_sum1 >= s_sum1) && (a_sum1 < 300)) {
		gCardinfo.gucSoudnorm = 1;
	}
#elif defined BUS_LICHUANG_
	eemoney = s_sum1 * 4;
	if ((a_sum1 >= s_sum1) && (a_sum1 < eemoney)) {
		gCardinfo.gucSoudnorm = 1;
	}
#else
	if ((a_sum1 >= s_sum1) && (a_sum1 < 500)) {
		gCardinfo.gucSoudnorm = 1;
	}
#endif

	if ((s_sum1 == 0) || (s_sum1 >= 2000))
		return 22;

	if (a_sum1 >= 10000000)
	{
		return 21;
	}

	if (a_sum1 >= s_sum1)
		a_sum1 -= s_sum1;										//�������
	else {
		led_on(LED_RED);
		MessageBox(0, "����!");
		SoundMessage(SOUND_FLING_MONEY);//��Ͷ��
		return 3;
	}
	a_sumR = a_sum1;
	if (s_sum1 == 0)
		return 5;
#ifdef _debug_ICcard_
	debugstring("��b1:");
	debugdata((unsigned char *)&b1, 1, 1);
	debugstring("��b2:");
	debugdata((unsigned char *)&b2, 1, 1);
#endif
	memcpy(buffer, (unsigned char*)&s_sum1, 4);
	for (i = 0; i < 3; i++)
	{
		memcpy(buffer, (unsigned char*)&s_sum1, 4);
		if (MifareValueDebit(PICC_DECREMENT, b1, buffer))
			goto mey_step9;
	}
	//	debugstring("MifareValueDebit ERROR!\r\n");
	return ST_ERROR;
mey_step9:

	//�п��ܿ۳ɹ��˻���� �ظ��� ���صĲ���  
	//	for(i=0;i<1;i++)
	//	{
	//		if(MifareTransfer(b1))
	//			break;
	//	}
	MifareTransfer(b1);
	for (i = 0; i < 1; i++)//ѭ�����ξ͹���
	{
		memset(buffer, 0, sizeof(buffer));
		if (MifareRead(b1, buffer))
		{
			if (MifIsPurse(buffer) == ST_ERROR)
				continue;
			if (memcmp(buffer, (unsigned char*)&a_sum1, 4) == 0)
				goto mey_step90;
		}
	}
	//	if(i==1)
	//	{
	CheckSno(ERROR_MONEY, OPER_TRANSFER_NO, buffer);
	return 4;
	//	}

mey_step90:
	//	eemoney = moneyB2-a_sum1;//��Ǯ����Ҫ����ֵ
	if (blockB2 == ST_ERROR) {//����˱����Ǻõģ�ֻ������һ���ָ�
		for (i = 0; i < 3; i++)
		{
			memset(buffer, 0, sizeof(buffer));
			if (MifareValueDebit(PICC_DECREMENT, b1, buffer))
				break;
		}
	}
	else {//�˱�û�����⣬�������м�ֵ
		for (i = 0; i < 3; i++)
		{
			memcpy(buffer, (unsigned char*)&s_sum1, 4);
			if (MifareValueDebit(PICC_DECREMENT, b2, buffer))
				break;
		}
	}

	//	for(i=0;i<1;i++)//��Ҫѭ����M1������ֵ���ɿ�����ֶ��
	//	{
	//	if(MifareTransfer(b2))
	//		break;
	//	}
	MifareTransfer(b2);
	//���˾ʹ��� ��Ҫ��
	// 	if(i>1)
	// 	{
	// 		CheckSno(ERROR_MONEY,OPER_TRANSFER_NO,buffer);
	// 		return 4;
	// 	}

#ifdef _debug_ICcard_
	debugstring("Purse over!\r\n");
#endif
	ErrorOper(IS_EFFECT_MONEY);
	return 5;
}

unsigned char MoneyResultManage(unsigned char mode)
{
	unsigned char pabuf[172];
	unsigned char ucRnt, mothFlag;
	//	unsigned int ii;
	memset(pabuf, 0, 128);
	mode = mode;

	if (GetDateTime() == ST_ERROR)
	{
		error(ERR_READ_TIME, 0);
		return ST_ERROR;
	}

	if (gBuInfo.stop_flag) {
		PRINT_DEBUG_LOCATION("ͣ��״̬Ŷ", "");
		return ST_OK;
	}

	mothFlag = gCardinfo.card_catalog;
	if (gCardinfo.card_catalog >= 0x40)
		mothFlag -= 0x40;

	if (month_decide() != 0) // (������ˢ)
	{
		disp_no_swipe();
		return ST_OK;
	}

	if (Delay_Normal())	//ͬһ�ſ���ˢ��ʱ1-2��
		return ST_ERROR;

	ucRnt = PurseProcess(mode);			 	//�����׽���
#ifdef _debug_ICcard_
	debugstring("Purse Process = ");
	debugdata(&ucRnt, 1, 1);
#endif
	switch (ucRnt)    //96ms
	{
	case ST_ERROR:						//��ͨ����
		break;
	case 42:
		MessageBox(1, "��Ƭ�ѻ�");
		end_card();
		break;
	case 21:
	case RET_MONEY_SN_EROR:				 //����Ǯ��������
		error(ERR_MONEY_SN, 0);
		return ST_OK;
	case 22:
		MessageBox(1, "Ʊ����Ϣ����ȷ");
		return ST_OK;
	case 2:
	case 3://����

		led_on(LED_RED);
		return ST_OK;
	case 4:
	case 6:
		audio(Audio_TRY_AGAIN);		//����ˢ

		card_ser = gCardinfo.c_serial;
		cardSound = 0xaa;
		break;
	case 5:
	case 7:
	case 8:
		if ((gDeviceParaTab.ChangeType == 0xAA) && (gCardinfo.card_catalog == CARD_NORMAL))
		{
#ifdef _PURSE_OLD_STYLE//����Ϊ�Ϸ�ʽ����Ҫд�빫����Ϣ
			//	PurchasOk_Money(1);
			if (PurchasOk(0) == ST_ERROR)
			{
				card_ser = gCardinfo.c_serial;
				//cardSound=0xaa;
				cardSound = 0xcc;
				SoundMessage(Audio_TRY_AGAIN);
				return ST_ERROR;
			}
#endif
		}

		cardSound = 0;

		Save_delayCardinfo(mode);

		money_msg(ID_REC_TOLL, a_sum1, s_sum1, 0);

		gBuInfo.SumMoney += s_sum1; //��ǰ�����ܶ�
		BuildRecorde(ID_REC_TOLL, pabuf);
		card_ser = gCardinfo.c_serial;

		save_file_BuInfo();


		//	WriteRecord(pabuf, 0);
		WriRecorJTB(pabuf, 1);
		Sound_cityCMD(mode);
		a_sumR = 0;

#ifdef _debug_ICcard_
		debugstring("money puchase ok!!\r\n");
#endif

		memset(old_Infor.resear, 0, 2);

		return ST_OK;
	default:
		break;
	}
	return ST_ERROR;
}


unsigned int get_Month_s_sum1(void)
{
	unsigned int temp_s = 0;

	unsigned char ctype;




	if (gCardinfo.card_catalog >= 0x40)
		ctype = gCardinfo.card_catalog - 0x40;
	else
		ctype = gCardinfo.card_catalog;

#ifdef _debug_ICcard_
	debugstring("card_catalog:");
	debugdata(&ctype, 1, 1);
	debugstring("rat:");
	debugdata(gDeviceParaTab.rate + ctype, 1, 1);
#endif

	if ((gDeviceParaTab.rate[ctype] == 101) \
		|| (gDeviceParaTab.rate[ctype] == 201) \
		|| (gDeviceParaTab.rate[ctype] == 110))
	{
#ifdef _debug_ICcard_
		debugstring("101 || 201 || 110---\r\n");
#endif
		return 1;	//ֱ�ӿ�1��
	}

	if (gCardinfo.card_catalog == CARD_MOTH_CI)
		temp_s = 1;
	if ((gDeviceParaTab.rate[ctype] >= 111) && (gDeviceParaTab.rate[ctype] <= 119)) {//110�Ͳ���Ҫ����
		temp_s = ((gDeviceParaTab.rate[ctype] - 110)); //�Ӹ��ӵ���	
	}
#ifdef _debug_ICcard_
	debugstring("get_Month_s_sum1 temp_s:");
	debugdata((unsigned char*)&temp_s, 4, 1);
#endif

	if (temp_s > 9)
	{
		debugstring("��Ʊ�ۿ�������---\r\n");
		temp_s = 0;
	}
	return temp_s;
}

//
unsigned char MonthManage(void)
{
	unsigned char i = 0;
	unsigned char buffer[25];
	unsigned char date[3];
	unsigned char dateSum = 0;
	unsigned char buffer_29[16], buffer_30[16];
	unsigned int temp10;
	unsigned int temp20 = 0;
	unsigned int SUMa = 0;
	unsigned int SUMb = 0;
	unsigned int SUMc = 0;
	unsigned long Pm = 0;
	unsigned char buffer_33[16];
	unsigned char buffer_34[16];
	unsigned char block29 = ST_ERROR;
	unsigned char block30 = ST_ERROR;
	unsigned char block2930 = ST_ERROR;
	unsigned char block33 = ST_ERROR;
	unsigned char block34 = ST_ERROR;
	unsigned char block3334 = ST_OK;//2016��8��27��14:13:16��
	unsigned char Check_Month = ST_OK;

	//clr_dog();
	if (gBuInfo.stop_flag) {
#ifdef _debug_ICcard_
		debugstring("��ͣ��״̬\r\n");
#endif
		return ST_ERROR;
	}

	for (i = 0; i < gErrorCard.error_pointer; i++)
	{
		if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT_MONTH) continue;
		else
		{
			if (!memcmp(gErrorCard.ErrorDetail[i].Cardno, &gCardinfo.c_serial, 4))
			{
				switch (gErrorCard.ErrorDetail[i].ErrorStep)
				{
				case OPER_TRANSFER_NO:
				{
					memcpy((unsigned char*)&temp20, gErrorCard.ErrorDetail[i].AferToll, 4);
					Check_Month = ST_ERROR;
					break;
				}
				case OPER_RECORD_PROFILE:
				case OPER_PUBLIC_MESSAGE_W:
				case OPER_PUBLIC_MESSAGE:
					return 5;
				default:
					break;
				}
			}
		}
	}
	if ((gDeviceParaTab.rate[gCardinfo.card_catalog] == 202) ||
		(gDeviceParaTab.rate[gCardinfo.card_catalog] == 102))////�������
	{
		dis_sum2 = 0;
		a_sum1 = 0;
		s_sum1 = 0;
		MSG_LOG("�����Ʊ");
		goto month_step10;
	}
#ifdef _HD_XIAOER_ //��Ǯ
	return 100;
#endif
	/*	if((gCardinfo.card_catalog==CARD_OLDMAN_BUS)||(gCardinfo.card_catalog==CARD_STUFF_BUS))
	{
	a_sum2.longbuffer=0l;		//�������
	s_sum2.longbuffer=0l;

	CARD_RECORD.CardRecord.dealOldMoney=a_sum2.longbuffer;
	memcpy(CARD_RECORD.CardRecord.dealMoney,s_sum2.charbuffer,3);
	goto month_step10;
	}
	*/

	memcpy(date, &SysTime.year_l, 3);          //������
	dateSum = (((date[1] & 0xf0) >> 4) * 10) + (date[1] & 0x0f);

#ifdef _debug_ICcard_
	debugstring("Month MMM 1------------------ !!!\r\n");
	debugdata(date, 3, 1);
	debugdata((unsigned char *)&gCardinfo.c_serial, 4, 1);
	debugdata(gCardinfo.MonthKEYA, 6, 1);
	debugdata(&gCardinfo.oddMonthSector, 1, 1);
#endif
	//if(profile(1)==ST_ERROR) return ST_ERROR;

	if (MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, gCardinfo.MonthKEYA, gCardinfo.oddMonthSector * 4) == 0)
	{
		return ST_ERROR;
	}

	if (gCardinfo.card_catalog == CARD_MOTH_CI)//�����ο�  ֱ��ȥ����Ʊ���
		goto ReadMoney;
	//��Ǯ����Ϣ��
	if (MifareRead(gCardinfo.oddMonthSector * 4 + 1, buffer) == 0)
	{
		return ST_ERROR;
	}
	memcpy(buffer_29, buffer, 16);

#ifdef _debug_ICcard_
	debugstring("Month MMM 2 !!!\r\n");
	debugdata(buffer, 16, 1);
#endif
	if (MiBlockInvalid(1, 15, buffer_29) == ST_ERROR)
	{
		return ST_ERROR;
	}
	if (MifareRead(gCardinfo.oddMonthSector * 4 + 2, buffer) == 0)
	{
		return ST_ERROR;
	}
	memcpy(buffer_30, buffer, 16);

#ifdef _debug_ICcard_
	debugstring("Month MMM 3 !!!\r\n");
	debugdata(buffer, 16, 1);
#endif
	if (MiBlockInvalid(1, 15, buffer_30) == ST_ERROR)
	{
		return ST_ERROR;
	}
#ifdef _debug_ICcard_
	debugstring("Month MMM 4 !!!\r\n");
#endif

	i = (((date[0] & 0xf0) >> 4) * 10) + (date[0] & 0x0f);
	date[0] = i;

	if (date[0] == buffer_29[0])
	{
		block29 = ST_OK;
		if (date[0] < buffer_30[0])
		{
			block2930 = ST_OK;
		}
	}
	else if (date[0] == buffer_30[0])
	{
		block30 = ST_OK;
		if (date[0] < buffer_29[0])
			block2930 = ST_OK;
	}
	else
	{
		return 100;	   //��Ʊ�黵�ˣ�ȥ��Ǯ								 
	}

	if (block29 == ST_OK)
	{
		if (!buffer_29[dateSum])
		{
			return 100;
		}
		for (i = dateSum; i < 13; i++)
			SUMb += buffer_29[i];
		memcpy((unsigned char*)&temp10, buffer_29 + 13, 2);
		SUMc = temp10;			//�������µ�����

		if (block2930 == ST_OK)
		{
			for (i = 1; i < 13; i++)
				SUMb += buffer_30[i];
			memcpy((unsigned char*)&temp10, buffer_30 + 13, 2);
			SUMc += temp10;
		}
		SUMa = SUMb - buffer_29[dateSum];
	}
	else if (block30 == ST_OK)
	{
		if (!buffer_30[dateSum])
		{
			return 100;
		}
		for (i = dateSum; i < 13; i++)
			SUMb += buffer_30[i];

		memcpy((unsigned char*)&temp10, buffer_30 + 13, 2);
		SUMc = temp10;
		if (block2930 == ST_OK)
		{
			for (i = 1; i < 13; i++)
				SUMb += buffer_29[i];

			memcpy((unsigned char*)&temp10, buffer_29 + 13, 2);
			SUMc += temp10;
		}
		SUMa = SUMb - buffer_30[dateSum];
	}
	if ((SUMb < SUMa) || (SUMb > SUMc) || (SUMa > SUMc))
	{
		return 100;
	}
#ifdef _debug_ICcard_
	debugstring("Month MMM 6 !!!\r\n");

	debugstring("��SUMa:");
	debugdata((unsigned char *)&SUMa, 4, 1);
	debugstring("��+��SUMb:");
	debugdata((unsigned char *)&SUMb, 4, 1);

#endif

ReadMoney:
	//if(profile(1)==ST_ERROR) return ST_ERROR;
	if (MifareAuthKey(PICC_AUTHENT1A, (unsigned char *)&gCardinfo.c_serial, gCardinfo.MonthKEYA, gCardinfo.dodMonthSector * 4) == 0)
	{
		return ST_ERROR;
	}
	//��Ǯ����Ϣ��
	if (MifareRead(gCardinfo.dodMonthSector * 4 + 2, buffer) == 0)
	{
		return ST_ERROR;
	}
	memcpy(buffer_34, buffer, 16);
#ifdef _debug_ICcard_
	debugstring("Month MMM 7 !!!\r\n");
	debugdata(buffer, 16, 1);
#endif
	if (MifareRead(gCardinfo.dodMonthSector * 4 + 1, buffer) == 0)
	{
		return ST_ERROR;
	}
	memcpy(buffer_33, buffer, 16);

	if (MifIsPurse(buffer_33) == ST_ERROR)
	{
		block3334 = ST_ERROR;
		MSG_LOG("MifIsPurse 33 ERR\r\n");
		goto month_step6;
	}
	else
		block33 = ST_OK;
	if (MifIsPurse(buffer_34) == ST_ERROR)
	{
		block3334 = ST_OK;
		MSG_LOG("��������\r\n");
		goto month_step6;
	}
	else
		block34 = ST_OK;
#ifdef _debug_ICcard_
	debugstring("Month MMM 8 !!!\r\n");
	debugdata(buffer_33, 16, 1);
	debugdata(buffer_34, 16, 1);
#endif
	if ((block33 == ST_ERROR) && (block34 == ST_ERROR)) return 120;

	for (i = 0; i < 4; i++) //
	{
		//clr_dog();
		if (buffer_33[3 - i] > buffer_34[3 - i])
		{
			block3334 = ST_ERROR;
			MSG_LOG("33>34 ERRO");
			break;
		}
		if (buffer_33[3 - i] < buffer_34[3 - i])
		{
			MSG_LOG("33<34 OK");
			block3334 = ST_OK;
			break;
		}
	}
month_step6:
	if (block3334 == ST_ERROR) memcpy(buffer_33, buffer_34, 4);

	s_sum1 = get_Month_s_sum1();

	memcpy((unsigned char*)&a_sum1, buffer_33, 4);

#ifdef _debug_ICcard_
	debugstring("Month MMM 9 !!!\r\n");
	debugdata((unsigned char*)&a_sum1, 4, 1);
#endif
	gCardinfo.gucSoudnorm = 0;
	if ((a_sum1 >= 1) && (a_sum1 <= 3))		//��Ʊ������3��
	{
		//		SoundMessage(DEAL_CHARGE_MONEY);
		gCardinfo.gucSoudnorm = 1;
	}

	if (Check_Month == ST_ERROR)
	{
		if (temp20 == a_sum1)
		{
			MSG_LOG("��ˢ�ѿۿ�");
			goto month_step10;
		}
	}
	Pm = a_sum1;

#ifdef _debug_ICcard_
	debugstring("��Pm:");
	debugdata((unsigned char *)&Pm, 4, 1);
	debugdata((unsigned char*)&dis_sum2, 4, 1);
#endif

	if (gCardinfo.card_catalog == CARD_MOTH_CI)//3 �������ο�  ���ж�
	{
		temp10 = s_sum1;
		//alon
		if (a_sum1 >= s_sum1)
		{
			a_sum1 -= s_sum1;
			dis_sum2 = a_sum1;
		}
		else
		{
			return 100;
		}

		goto MonthDebit;//���ж� ֱ��ȥ�۴�
	}

	if (Pm < SUMa) return 100;

	if (Pm == SUMa)
	{
		return 100;
	}
	if ((Pm >= SUMa) && (Pm <= SUMb))
	{
		if (a_sum1 >= s_sum1)
		{
			if ((a_sum1 - SUMa) < s_sum1)//��β����� ȥ��Ǯ
				return 100;
			a_sum1 -= s_sum1;
			dis_sum2 = a_sum1 - SUMa;
			temp10 = s_sum1;
		}
		else
		{
			return 100;
		}
	}
	else if (Pm > SUMb)
	{

		temp10 = s_sum1;		//Ҫ��Ȼs_sum�������ʾ�ۿ���е������ˣ���������������
		if (Pm - SUMb < temp10)//��β����� ȥ��Ǯ
			return 100;
		temp10 = Pm - SUMb + temp10;
#ifdef _debug_ICcard_
		debugstring("����------\r\n");
		debugstring("s_sum----\r\n");
		debugdata((unsigned char *)&temp10, 4, 1);
#endif

		if (a_sum1 >= temp10)
		{
			a_sum1 -= temp10;
			dis_sum2 = a_sum1;
		}
		else
		{
			return 100;
		}
	}
	else
		return ST_ERROR;



MonthDebit:

	memcpy(buffer_33 + 4, (unsigned char*)&temp10, 4);
	// 	buffer_33[4]=s_sum2.charbuffer[0];
	// 	buffer_33[5]=s_sum2.charbuffer [1];
	// 	buffer_33[6]=s_sum2.charbuffer [2];
	// 	buffer_33[7]=s_sum2.charbuffer [3];
#ifdef _debug_ICcard_
	//	goto month_step10;

	debugstring("�۴�temp10:");
	debugdata((unsigned char*)&dis_sum2, 4, 1);
	debugdata((unsigned char *)&temp10, 4, 1);
#endif

	if (dis_sum2 > 3000)//�Ӹ��ж�, ���²����۵Ļ� �����һ���ܴ����
	{
		MSG_LOG("ȥ��Ǯ:");
		//��ʵ���ᵽ��������   �������ж���
		return 100;
	}

	for (i = 0; i < 3; i++)
	{
		//clr_dog();
		if (block3334 == ST_ERROR)
		{
			MSG_LOG("block3334=ERRO Debit 2\r\n");
			memcpy(buffer, (unsigned char*)&temp10, 4);
			//debugdata(buffer,4,1);
			if (MifareValueDebit(PICC_DECREMENT, gCardinfo.dodMonthSector * 4 + 2, buffer))
				goto month_step9;
		}
		else
		{
			MSG_LOG("block3334=ERRO Debit 1\r\n");
			memcpy(buffer, (unsigned char*)&temp10, 4);
			//	debugdata(buffer,4,1);
			if (MifareValueDebit(PICC_DECREMENT, gCardinfo.dodMonthSector * 4 + 1, buffer))
				goto month_step9;
		}
	}
	return ST_ERROR;

month_step9:
	for (i = 0; i < 1; i++)
	{
		//clr_dog();
		if (block3334 == ST_ERROR)
		{
			MSG_LOG("block3334=ERRO Transer 1\r\n");
			if (MifareTransfer(gCardinfo.dodMonthSector * 4 + 1))
				break;
		}
		else
		{
			MSG_LOG("block3334=ERRO Transer 2\r\n");

			if (MifareTransfer(gCardinfo.dodMonthSector * 4 + 2))
				break;
		}
	}




	if (block3334 == ST_ERROR)
	{
		MSG_LOG("block3334=ERRO Read\r\n");
		for (i = 0; i < 3; i++)
		{
			//clr_dog();
			if (MifareRead(gCardinfo.dodMonthSector * 4 + 1, buffer))
			{
#ifdef _debug_ICcard_
				debugstring("��ˢ8-1:\r\n");
				debugdata(buffer, 4, 1);
				debugdata((unsigned char *)&a_sum1, 4, 1);
#endif
				if (memcmp(buffer, (unsigned char*)&a_sum1, 4) == 0)
					goto month_step10;
			}
		}
		if (i == 3)
		{
			if (CheckSno(ERROR_MONTH, OPER_TRANSFER_NO, buffer) == ST_ERROR) return ST_ERROR;
			return 4;
		}

	}
	else
	{
		for (i = 0; i < 3; i++)
		{
			//clr_dog();
			if (MifareRead(gCardinfo.dodMonthSector * 4 + 2, buffer))
			{

#ifdef _debug_ICcard_
				debugstring("��ˢ8-4:");
				debugdata(buffer, 4, 1);
				debugdata((unsigned char *)&a_sum1, 4, 1);
#endif
				if (memcmp(buffer, (unsigned char*)&a_sum1, 4) == 0)
					goto month_step10;
			}
		}
		if (i == 3)
		{

			if (CheckSno(ERROR_MONTH, OPER_TRANSFER_NO, buffer) == ST_ERROR) return ST_ERROR;
			return 4;
		}

	}
month_step10:
	ErrorOper(IS_EFFECT_MONTH);
	return 5;
}

unsigned char MonthResultManage(void)
{
	unsigned char rRecord[170];
	unsigned char i = 0, mothFlag;
	//	unsigned int ii;
	//	unsigned char pabuf[50];

	//clr_dog();
	memset(rRecord, 0, 128);
#ifdef _debug_ICcard_
	debugstring("MonthResultManage Programer!\r\n");
#endif
	mothFlag = gCardinfo.card_catalog;
	if (mothFlag >= 0x40)
		mothFlag -= 0x40;

	if (month_decide() != 0) // �ڶ���ȥ��Ǯ��(����������ˢ)
	{
		for (i = 0; i < gErrorCard.error_pointer; i++)
		{
			if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT_MONTH) continue;
			else
			{
				if (memcmp(gErrorCard.ErrorDetail[i].Cardno, &gCardinfo.c_serial, 4) == 0)
				{
					goto kousum_continue;
				}
			}
		}
		disp_no_swipe();
		return ST_OK;
	}

	for (i = 0; i < gErrorCard.error_pointer; i++)
	{
		if (gErrorCard.ErrorDetail[i].effFlag != IS_EFFECT_MONEY) continue;
		else
		{
			if (memcmp(gErrorCard.ErrorDetail[i].Cardno, &gCardinfo.c_serial, 4) == 0)
			{
				if (MoneyResultManage(3) == ST_ERROR)
					return ST_ERROR;
				else
					return ST_OK;
			}
		}

	}

kousum_continue:
	i = gCardinfo.card_catalog;
	if (i >= 0x40)
		i -= 0x40;
	if (gDeviceParaTab.rate[i] == 203)//��ֹʹ����Ʊ
	{
		if (MoneyResultManage(3) == ST_ERROR)
			return ST_ERROR;
		else
			return ST_OK;
	}
#ifdef _debug_ICcard_
	debugstring("MonthResultManage Programer 3 !!!\r\n");
	debugdata(&i, 1, 1);
#endif

	i = MonthManage();
#ifdef _debug_ICcard_
	debugstring("MonthResultManage Programer 4 !!!\r\n");
	debugdata(&i, 1, 1);
#endif
	switch (i)
	{
	case 5:
	{
		// 			if(PurchasOk(1)==ST_ERROR) 
		// 			{
		// #ifdef CARD_SOUND
		// 				card_ser=c_serial;
		// 				cardSound=0xaa;
		// #endif
		// //				SoundMessage(Audio_TRY_AGAIN);
		// 				return ST_ERROR;
		// 			}
		cardSound = 0;
		if (gCardinfo.card_catalog == CARD_YOUFU_BUS)
		{
#ifdef _debug_ICcard_
			debugstring("�Ÿ���:::::�۴α�:�Ÿ�\r\n");
#endif
			audio(Audio_CY);
		}
		else if (gCardinfo.card_catalog == CARD_MOTH_CI)
		{
			audio(Audio_DONG);
		}
		else {
			SoundMessage(SOUND_DEAL);
		}
		BuildRecorde(ID_REC_MON, rRecord);
		money_msg(ID_REC_MON, dis_sum2, s_sum1, 0);
		memset(old_Infor.old_Purcs_Time, 0, 16);
		//if(gCardinfo.card_catalog != CARD_COUPON)
		//			debugstring("WARNING::: NO save_h_month!!!!");

		if (gCardinfo.card_catalog == CARD_MOTH_CI)
			save_normal();
		else
			save_h_month();

		//	WriteRecord(rRecord, 0);
		WriRecorJTB(rRecord, 1);
		gBuInfo.SumMti += s_sum1;
		save_file_BuInfo();

		return ST_OK;
	}
	case 120:
		MessageBox(1, "��ƱǮ������");
		return ST_OK;
	case 121://�۴�������
		MessageBox(1, "����Ʊ�۴���!");
		return ST_OK;
	case 100:
#ifdef _debug_ICcard_
		debugstring("��Ʊ����----\r\n");
#endif
		if (gCardinfo.card_catalog == CARD_NORMAL_B) {// ����ר�߿�������ˢǮ����  ����ר�߿� 2014.4.8
			led_on(LED_RED);
#ifdef _debug_ICcard_
			debugstring("ר�߿� ��β���!\r\n");
#endif
			MessageBox(0, "����!");
			SoundMessage(SOUND_FLING_MONEY);//��Ͷ��
			gBuInfo.restore_flag = 1;
			return ST_OK;
		}
		else {
			if (MoneyResultManage(3) == ST_ERROR)
				return ST_ERROR;
			else
				return ST_OK;
		}
	case 4:
		card_ser = gCardinfo.c_serial;
		cardSound = 0xaa;
	default:
		break;
	}
	return ST_ERROR;
}


void main_card(void)
{
	unsigned char resPonse = 0;
	// 	unsigned int ltemp;
	// 	int headp, curp;

	resPonse = profile(0);

	if (resPonse == ST_ERROR) //�����������
	{
		return;
	}

#ifdef _debug_ICcard_
	printf("response:%02X, c_serial:%08X\r\n", resPonse, gCardinfo.c_serial);
#endif

	if (resPonse == 0x20) {//��һ����־ CPU�� ����������
		gCardinfo.gMCardCand = CARDSTYLE_CPU;
	}
	else {
		gCardinfo.gMCardCand = CARDSTYLE_NORM1;
	}

	if (resPonse == 0x20) {//CPU 0x20,0x28

#ifdef BUS_CPU_
		CPUMainCard();//CPU��,Ӧ�ò���2.4G��
#endif
#ifdef qPBOC_BUS
		cpuPBOCmain();
		return;
#endif
		return;
	}


	if ((cardSound == 0xaa) || (cardSound == 0xbb))//aaǮ������ bb��Ʊ����
	{
		if (card_ser != gCardinfo.c_serial) {

			cardSound = 0;
			card_ser = 0;

		}
	}

	resPonse = Card_typeProcess();	 //120ms

#ifdef _debug_ICcard_
	printf("resPonse=");
	debugdata(&resPonse, 1, 1);
	debugstring("  �ۿ�:");
#ifdef BUS_PRO_JSB
	debugdata((unsigned char*)&gDeviceParaTab.rate[gCardinfo.card_catalog], 1, 1);
#else
	debugdata((unsigned char*)&gDeviceParaTab.rate[gCardinfo.card_catalog - 0x40], 1, 1);
#endif
#endif

#ifdef FengDuan_BU_			//�ֶ��շ�
	if ((resPonse == MONTH_CARD) || (resPonse == CARD_MONEY) || (resPonse == CARD_YEAR) || (resPonse == CARD_STUFF_BUS)) {
		FengDuan_BUS_card();
		return;
	}
#endif	//#ifdef FengDuan_BU_


	// 	if((cal_space()<5)&&(gCardinfo.card_catalog!=CARD_DATA_GATHER))		//�������ˣ�����ˢ��
	// 	{
	// 		cls();
	// 		curp = Get_Record_point((unsigned char*)&headp, 0);
	// 		ltemp=0;
	// 		ltemp=curp - headp;
	// 		ltemp/=RECORD_LEN;
	// 		if(ltemp == 0)					//��ǰ��¼����Ϊ0�����¼��
	// 		{
	// 			lib_clr_Record_point();
	// 		}
	// 		else
	// 		{
	// 			if(gchn_eng == 'E'){
	// 				display(0,0,"warring:",1);
	// 				display(2,0,"eeprom full!",DIS_CENTER);
	// 			}
	// 			else
	// 			{
	// 				display(0, 0, "����:", 0);
	// 				display(2, 2, "���ϴ�����!", 0);
	// 			}
	// 			end_card();
	// 			gBuInfo.restore_flag = 3;
	// 			return;
	// 		}	
	// 	}

	switch (resPonse) {
	case ST_ERROR:
		return;
	case CARD_NO_Busins:
		gBuInfo.restore_flag = 1;
		led_on(LED_RED);
		MessageBox(1, "�Ǳ��̻���!");
		// 		sprintf((char*)irda_rxBuf, "BN:%02X%02X", gCardinfo.TradeCode[0], gCardinfo.TradeCode[1]);
		// 		display(4, 0, (char*)irda_rxBuf, DIS_CENTER);
		end_card();
		break;
	case CARD_CHAGEERR:
		gBuInfo.restore_flag = 1;
		led_on(LED_RED);
		MessageBox(1, "������,�뵽��ֵ��ָ�.");
	case ST_OK:
		end_card();
		break;
	case CARD_PUBLIC_BAD:		//����Ҫ�ָ�
// 		if(RepairPublicSector() == ST_ERROR)
// 			error(ERR_CARD_PUBLIC, 0);
// 		gBuInfo.restore_flag=3;
		break;
	case CARD_NO_TIME:
		if (gBuInfo.stop_flag) return;

		MessageBox(1, "�����ڻ�δ����");

		audio(Audio_FLING_MONEY);	//��Ͷ��

		gBuInfo.restore_flag = 3;
		end_card();
		break;
	case CARD_BLACK_PRO:
		OperBlackCard();
		MessageBox(1, "�ÿ��ѹ�ʧ!");
		SoundMessage(INVALID_CARD);
		gBuInfo.restore_flag = 3;
		end_card();
		break;
	case CARD_forbid:
		MessageBox(1, "�˿���ֹˢ��!");
		led_on(LED_RED);
		gBuInfo.restore_flag = 3;
		end_card();
		break;
	case CARD_BLACK:
		MessageBox(1, "�ÿ��ѹ�ʧ!");
		SoundMessage(INVALID_CARD);
		gBuInfo.restore_flag = 3;
		end_card();
		break;
	case CARD_STOPWORK:
		error(ERR_CARD_STOP_WORK, 0);
		break;
	case CARD_NO_SYSTEM:
		error(ERR_CARD_NO_SYSTEM, 0);
		break;
	case CARD_WHITE_BUS:				//�׿�
		error(ERR_CARD_WHITE_BUS, 0);
		break;
	case CARD_NO_USE:
		error(ERR_CARD_NO_USE, 0);
		break;
	case INVALID_CARD:
		SoundMessage(INVALID_CARD);
		gBuInfo.restore_flag = 3;
		end_card();
		break;
	case CARD_HUIFU:
		error(ERR_CARD_PUBLIC, 0);
		gBuInfo.restore_flag = 3;
		end_card();
		break;
	case CARD_YEAR:
		printf("[%s]Year Card!!!!!!!!!\r\n", __FUNCTION__);
		// 		if(YearResultManage(0)==ST_ERROR) 
		// 			break;   //��ƱǮ�������
		// 		gBuInfo.restore_flag=3;
		end_card();
		break;
	case CARD_MONEY:
		if (checkBusInfo() != ST_OK)
		{
			MessageBox(0, "�豸��Ϣ����");
		}
		else {
			if (MoneyResultManage(0) == ST_ERROR)
				break;   //Ǯ����
		}

		end_card();
		gBuInfo.restore_flag = 1;
		break;
	case MONTH_CARD:
		if (checkBusInfo() != ST_OK)
		{
			MessageBox(0, "�豸��Ϣ����");
		}
		else {
			if (MonthResultManage() == ST_ERROR)
				break;	//��Ʊ�����´�
		}
		end_card();
		gBuInfo.restore_flag = 0;
		break;
	case 150:
		gBuInfo.restore_flag = 3;
#if 1
		end_card();
#endif
		break;
	case CONTROL_CARD:
		ControlMagane();
		gBuInfo.restore_flag = 3;
		break;
	default:
		break;
	}
	return;
}










