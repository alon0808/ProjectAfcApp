#define SWITCH_LTY_QR	1
#include "Macro_Proj.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

#include "inputmisc/PsamCard.h"

#include "inputmisc/GPIOCtrl.h"

#include "LightColor.h"
#include "GPRS.h"

#include "DemoMain.h"

#include "inputmisc/IcCardCtrlApi.h"

#include "CPUCARJTB.h"
#include "Main_City.h"
#include "SlzrTypeDef.h"
#include "PSAMLib.h"
#include "MYDES.h"

#include "ICCardLib.h"
#if SWITCH_LTY_QR
#include "QRScan.h"
#endif
#include "gps.h"

#include "GprsSocket.h"
#include "RecordFile.h"
#include "xSys_Lantaiyuan.h"
#include "RamStorage.h"
#include "szct.h"
#include "qPBOC.h"
#include "qpboc_8583.h"
#include "qpboc_head.h"
#include "add.h"


// typedef struct Qr_input_info{
// 	unsigned int  BusNo;           //�������
// 	unsigned int  LineNo;          //��·���
// 	unsigned int  CompanyNo;     //��˾����
// 	unsigned char CityID[6];        //����ID
// 	unsigned int  Price;			 //Ʊ��
// 	unsigned char Datetime[7];	     //����ʱ��BCD�룬YYYYMMDDhhmmss
// 	unsigned int  QrcodeLen;		 //��ά�����ݳ��ȡ�//
// 	unsigned char Qrcode[50];	     //��ά������ 
// 	unsigned int  Longitude;       //����
// 	unsigned int  Latitude;         //ά��
// 	unsigned short  u8Angle;      //��λ�ǣ�GPS��λ��
// }StHDQRCinput;
// 
// //libQRScan.a ���ں�������֤��̩Դ��ά��
// extern "C" int HDQRCodeVerify(StHDQRCinput *inData, unsigned char *OutRecord, unsigned int *OutRecLen);

#define _debug_QRCode_



volatile unsigned int gQRCdataLen;
volatile unsigned int gQRCdataFlag;
unsigned char gQRCdatabuff[2048];

void *main_tidgetQRCode(void *arg)
{
	LTY_RESULT ret = LTY_SUCCESS;
	LTY_S8 tmpbuf[6];
	LTY_U8 buf[240];
	LTY_U32 bufsize, i, si;
	//	LTY_S32 m_u32UartQRCodeFd = 0;
	bufsize = 20;
	int QrLen = 0;
	int QrFlag = 0;

	gQRCdataLen = 0;
	gQRCdataFlag = 0;

	Card_QRCodeInit();
	close(m_u32UartQRCodeFd);

	m_u32UartQRCodeFd = open((const LTY_STRING)"/dev/ttyS4", O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fcntl(m_u32UartQRCodeFd, F_SETFL, 0) < 0)
	{
		printf("fcntl failed!\n");
		//return(FALSE);    
	}
	else
	{
		printf("fcntl=%d\n", fcntl(m_u32UartQRCodeFd, F_SETFL, 0));
	}

#if 0	
	memcpy(buf, "\x7E\x00\x00\x05\x33\x48\x30\x33\x30\xB2", 10);
	ret = UartWrite(m_u32UartQRCodeFd, buf, 10);
	if (ret == SLZR_FAILURE)
	{
		printf("SerialSend(%s) failed,%d\n", "/dev/ttyS4", ret);
	}
	else
	{
		printf("��ȡ���к�\n");
	}
	memset(buf, 0, 40);

	ret = UartRead(m_u32UartQRCodeFd, buf, bufsize);

	if (ret == SLZR_FAILURE)
	{
		printf("SerialRead(%s) failed,%d\n", "/dev/ttyS4", ret);
	}
	else
	{
		printf("��ȡ���кų���%d\n", ret);

		for (i = 0; i < bufsize; i++)
		{
			printf("%02x", buf[i]);
		}
		printf("\n");
		memcpy(tmpbuf, buf + 11, 6);
		for (i = 0; i < 6; i++)
		{
			printf("%c", tmpbuf[i]);
		}
		printf("\n");
	}
	memset(buf, 0, 40);
	ret = UartRead(m_u32UartQRCodeFd, buf, 2);
	if (ret == SLZR_FAILURE)
	{
		printf("SerialRead(%s) failed,%d\n", "/dev/ttyS4", ret);
	}
	else
	{
		printf("��ȡ���кų���%d\n", ret);

		for (i = 0; i < 2; i++)
		{
			printf("%02x", buf[i]);
		}
		printf("\n");
		memcpy(tmpbuf + 5, buf, 1);
		for (i = 0; i < 6; i++)
		{
			printf("%c", tmpbuf[i]);
		}
		printf("\n");
	}
	si = atoi((const char*)tmpbuf);
	printf("%d\n", si);

#endif 

	while (1)
	{
		if (gQRCdataFlag == 0xAA) {
			usleep(300000);
			//printf("[%s] AAAAAAAAAAAAAAAA\n", __FUNCTION__);
			continue;			//�ϴε����ݻ�û�����������µġ�
		}
		else if (QrFlag == 0xAA) {
			gQRCdataFlag = 0xAA;
			gQRCdataLen = QrLen;
			QrLen = 0;
			QrFlag = 0;
			usleep(500000);
			continue;
		}
		//		ret = UartRead(m_u32UartQRCodeFd, buf, 1);
		ret = read(m_u32UartQRCodeFd, buf, 1);
		//if(ret == SLZR_FAILURE)
		if (ret < 0)
		{
			usleep(10000);

			printf("main_tidgetQRCode(%s) failed,%d\n", "/dev/ttyS4", ret);
			if (QrLen > 0) {
				gQRCdatabuff[QrLen] = 0;
				QrFlag = 0xAA;
				continue;
			}
		}
		else
		{
			//BCD_LOG(buf, ret, 1);

			memcpy(gQRCdatabuff + QrLen, buf, ret);
			QrLen += ret;

#if 1
			if ((gQRCdatabuff[QrLen - 1] == '\r') || (gQRCdatabuff[QrLen - 1] == '\n')) {
				//PRINT_WARNING("abuff[QrLen - 1] == ) || (gQRCdatabuff[QrLen %d\n", QrLen);
				//BCD_LOG(gQRCdatabuff, QrLen, 1);
				if (QrLen == 1) {
					QrLen = 0;
				}
				else if (QrLen > 1) {
					--QrLen;
					QrFlag = 0xAA;
					gQRCdatabuff[QrLen] = 0;
				}
				continue;
			}
#endif
			if ((ret == 0) && (QrLen > 0)) {
				PRINT_WARNING("main_tidgetQRCode.(ret == 0) && (QrLen > 0\n");
				gQRCdatabuff[QrLen] = 0;
				QrFlag = 0xAA;
				continue;
			}

			if (ret == 0) {
				usleep(10000);
			}
		}

		// 		sleep(5);
		// 		memcpy(gQRCdatabuff, "31304000002447321533802002WZ3BdsgDqw", 36);
		// 		gQRCdataLen = 36;

	}

	close(m_u32UartQRCodeFd);

	return (void *)0;
}


//��֯��Ѷ��ά��ļ�¼�ṹ
void BuildQRCRecorde(unsigned char delType, unsigned char *orecBuf, unsigned char *iQRCdat, unsigned int iQRCdatLen, unsigned char pbocResult)
{
	stOtherQRCrecord *rRecordDeal;
	unsigned int temp;
	stltyQRCandGPS *ltyR;
	unsigned int tmpUI = 0;

	//	unsigned short crc_16;


	rRecordDeal = (stOtherQRCrecord *)orecBuf;

	memcpy(rRecordDeal->rQrcrecHead, "��ά��", 6);
	if (delType == ID_REC_QRC_GZT)
		memcpy(rRecordDeal->rQrcrecHead2, "A6", 2);
	else if (delType == ID_REC_QRC_TX)
		memcpy(rRecordDeal->rQrcrecHead2, "A4", 2);
	else if (delType == ID_REC_QRC_ZFB)
		memcpy(rRecordDeal->rQrcrecHead2, "A5", 2);

	else if (delType == ID_REC_QRC_PBOC)
		memcpy(rRecordDeal->rQrcrecHead2, "A7", 2);
	else if (delType == ID_REC_QRC_UNPAY_BUS)
		memcpy(rRecordDeal->rQrcrecHead2, "AB", 2);
	else if (delType == ID_REC_QRC_LTY)
		memcpy(rRecordDeal->rQrcrecHead2, "AC", 2);
	else
		return;

	//������ˮ��
	temp = gBuInfo.MoneyDealPointer;
	memcpy(rRecordDeal->rCardDealNumb, (unsigned char*)&temp, 4);  //M1

	gBuInfo.MoneyDealPointer++;
	if (gBuInfo.MoneyDealPointer > 999999)	// ��ƽ̨��6��9,��ƽ̨��9��9
		gBuInfo.MoneyDealPointer = 0;
	temp = gBuInfo.MoneyDealPointer;

	Get_SerialNumF4(rRecordDeal->rDevSerial);
	rRecordDeal->rCardType = gCardinfo.card_catalog;
	rRecordDeal->rDealType = delType;
	memset(rRecordDeal->ruserTimes, 0, 2);
	rRecordDeal->ruserTimes[1] = pbocResult;

#ifdef _debug_
	MSG_LOG("__a_sum1:%2X \n", a_sum1);
	MSG_LOG("__s_sum1:%2X \n", s_sum1);
#endif

	if (delType == ID_REC_QRC_UNPAY_BUS)		//�����˳��������
	{
		unsigned int a_sum1_AB = 0;
		memcpy(rRecordDeal->rAfterMoney, (unsigned char*)&a_sum1_AB, 4);
	}
	else {
		memcpy(rRecordDeal->rAfterMoney, (unsigned char*)&a_sum1, 4);
	}

	memcpy(rRecordDeal->rDealMoney, (unsigned char *)&s_sum1, 3);
	memcpy(rRecordDeal->rDealTime, (unsigned char*)&SysTime, 7);
	memcpy(rRecordDeal->rDriverNo, gBuInfo.DriverNO, 4);

	CharsToBytes(gDeviceParaTab.DeviceNo, 8, rRecordDeal->rDeviceNo, 4);
	memcpy(rRecordDeal->rLineNo, gDeviceParaTab.LineNo, 2);
	memcpy(rRecordDeal->rProVer, gBusVerInfo.busProVer, 2);//����汾
	memcpy(rRecordDeal->rTicket, gDeviceParaTab.busPrice, 2);	//Ʊ�ۣ�2�ֽڣ����65536��
	memset(rRecordDeal->rQRCdat, 0, sizeof(rRecordDeal->rQRCdat));

	MSG_LOG("rQRCdat(%d):", iQRCdatLen);
	BCD_LOG(iQRCdat, iQRCdatLen, 1);
	memcpy(rRecordDeal->rQRCdat, iQRCdat, iQRCdatLen);

	if (deal_type == ID_REC_QRC_LTY) {
		ltyR = (stltyQRCandGPS *)rRecordDeal->rQRCdat;
		temp = (unsigned int)(gprmc.longitude * 1000);
		memcpy((unsigned char*)ltyR->longitude, (unsigned char*)&temp, 4);

		temp = (unsigned int)(gprmc.latitude * 1000);
		memcpy((unsigned char*)ltyR->latitude, (unsigned char*)&temp, 4);

		temp = (unsigned int)(gprmc.direction * 100);
		memcpy((unsigned char*)ltyR->direction, (unsigned char*)&temp, 4);

		temp = (unsigned int)(gprmc.speed * 100);
		memcpy((unsigned char*)ltyR->speed, (unsigned char*)&temp, 4);
	}
	else if (delType == ID_REC_QRC_UNPAY_BUS) {
		//getMobileParameter(6, rRecordDeal->rDevId);	//�����豸��

		temp = 0;
		rRecordDeal->rRouteInfo[temp] = 0x00;
		++temp;
		rRecordDeal->rRouteInfo[temp] = (unsigned char)gprmc.direction;
		++temp;
		rRecordDeal->rRouteInfo[temp] = 0;
		++temp;
		tmpUI = (unsigned int)(gprmc.longitude * 1000);
		memcpy(rRecordDeal->rRouteInfo + temp, (unsigned char *)&tmpUI, 4);
		temp += 4;
		tmpUI = (unsigned int)(gprmc.latitude * 1000);
		memcpy(rRecordDeal->rRouteInfo + temp, (unsigned char *)&tmpUI, 4);
#if 0
		if (memcmp(rRecordDeal->rRouteInfo, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 11) == 0) {
			// 110287482-0692DA7A-7ADA9206, 25262823-01817AE7-E77A8101
			memcpy(rRecordDeal->rRouteInfo, "\x01\x01\x05\x7A\xDA\x92\x06\xE7\x7A\x81\x01", 11);
		}
#endif
	}

	memcpy(rRecordDeal->rTAC, "\xEE\xFF\xDD\xCC", 4);

	if (((delType & 0x7F) == ID_REC_TOLL) || ((delType & 0x7F) == ID_REC_MON)) {
		addStatMoney((delType & 0x7F), s_sum1);
#ifdef _Counter_SWcard_times
		addSCtimes(1);//ˢ��������1 
#endif
	}
	else
	{
		addStatMoney(ID_REC_TOLL, s_sum1);
	}

	return;

}

// ����д256
unsigned char WriRecorQRC(unsigned char *rec)
{
	unsigned char i = 0;
	unsigned char buff[TXQRCRECORD_LEN + 1];
	unsigned char buffer[TXQRCRECORD_LEN + 1];
	//	unsigned char *tempbuf;
	unsigned int temp;
	//	unsigned char len;

	memset(buff, 0x00, TXQRCRECORD_LEN);
	memcpy(buffer, rec, TXQRCRECORD_LEN);


	temp = Get_Record_point(buff, 0);


#ifdef _debug_QRCode_
	debugstring("-------------------\r\ncur:");
	debugdata((unsigned char*)&temp, 4, 1);
	FR_flashread(temp, buff, TXQRCRECORD_LEN);
	debugdata(buff, TXQRCRECORD_LEN, 1);
#endif

	for (i = 0; i < 3; i++)
	{

		FR_flashwrite(temp, buffer, TXQRCRECORD_LEN);
#ifdef _debug_QRCode_
		debugstring("WriRecorQRC : \r\n");
		debugdata(buffer, TXQRCRECORD_LEN, 1);
#endif

		FR_flashread(temp, buff, TXQRCRECORD_LEN);
#ifdef _debug_QRCode_
		debugstring("WriteRecordCheck ulofset2: \r\n");
		debugdata(buff, TXQRCRECORD_LEN, 1);
#endif

		if (memcmp(buffer, buff, TXQRCRECORD_LEN) == 0)
		{
			writeBackRec(buffer, TXQRCRECORD_LEN, 0);
			ADD_Record_point(0, TXQRCRECORD_LEN);
			MSG_LOG("WRITRC_OK!");
			return ST_OK;
		}
		else {//д������дȫ0������дʵ���ݣ���д����һ���ݵ�ַ
#ifdef _debug_QRCode_
			debugstring("write record ERROR!!!\r\n");
#endif
			memset(buff, 0x01, TXQRCRECORD_LEN);
			FR_flashwrite(temp, buff, TXQRCRECORD_LEN);
			temp += TXQRCRECORD_LEN;
			ADD_Record_point(0, TXQRCRECORD_LEN);

		}
	}
	cls();

	writeBackRec(buffer, TXQRCRECORD_LEN, 0);
	//	writeBackRec(buffer+128, RECORD_LEN, 0);
	display(2, 2, "E2PROMд����,��Ҫά��!", 1);
	memset(buffer, 0, 20);
	memcpy(buffer, "ADDR:", 5);
	BCD2Ascii((unsigned char *)&temp, buffer + 5, 4);
	display(6, 0, (char*)buffer, 0);

	led_on(LED_RED | LED_GREEN);
	for (;;)
		usleep(50000);

	return ST_OK;
}

//��ά�봦��
void main_QRCode_Deal(void)
{
	int ret;

	unsigned char outbuf[1024];
	unsigned int len;
	int t, t1;
#if SWITCH_LTY_QR
	StHDQRCinput verifyInput;
	//CQR classCQR;
#endif
	unsigned char pubkey1[256];
	unsigned int c_serial_mac;
	stltyQRCandGPS *ltyp;
	int i = 0;
	unsigned int qrLen = 0;

	unsigned char *pQrBuf = gQRCdatabuff;
	unsigned char deal_type;


	if (gQRCdataFlag == 0xAA) {
		qrLen = gQRCdataLen;
#ifdef _debug_QRCode_
		printf("[%s] QRCode OK,qrLen:%d\n", __FUNCTION__, qrLen);
#endif
#if SWITCH_LTY_QR
		if ((qrLen == 36) && ((pQrBuf[0] == '3') || (pQrBuf[0] == '4'))) {	//����̩Դ���롣

		//���⹫��������ʶ���̶�Ϊ3��4��+���⿨ID��6�ֽڳ��д���+9�ֽ�ע��Ψһ�룩+����ʱ�䣨10�ֽڣ�+��֤�루10�ֽ�У�飩
		//������3 130400 000244732 1534244118 PNPBWC9Eyh
			gCardinfo.card_catalog = CARD_QRC_LTY;
			gCardinfo.gMCardCand = CARDQR_CODE;


			verifyInput.BusNo = 0;
			verifyInput.LineNo = 101;
			verifyInput.CompanyNo = 1;
			memcpy(verifyInput.CityID, "\x00\x00\x00\x00\x07\x55", 6);
			memcpy((unsigned char*)&verifyInput.Price, gDeviceParaTab.busPrice, 4);
			memcpy(verifyInput.Datetime, (unsigned char*)&SysTime, 7);
			verifyInput.QrcodeLen = LTY_QRCODE_LEN;
			memcpy(verifyInput.Qrcode, pQrBuf, LTY_QRCODE_LEN/*sizeof(verifyInput.Qrcode)*/);
			verifyInput.Longitude = 0;
			verifyInput.Latitude = 0;
			verifyInput.u8Angle = 0;

			len = 0;
			ret = HDQRCodeVerify(&verifyInput, outbuf, &len);

			printf("[%s] ret = %d, len = %d\r\n", __FUNCTION__, ret, len);

			if (ret != 0) {
				led_on(LED_RED);
				memset(pubkey1, 0, 32);

				strcpy((char *)pubkey1, "��Ч��ά��");

				MessageBox(1, (char *)pubkey1);

				gBuInfo.restore_flag = 3;

				ret = -1;
				goto main_QRCode_Deal_OVER;
			}

			a_sum1 = 0;
			s_sum1 = get_s_sum1(0);


			memset(pubkey1, 0x11, 16);
			memset(pubkey1 + 20, 0x22, 8);

			if (gBuInfo.stop_flag != 0) {
				MSG_LOG("[%s] ��վ״̬Ŷ!\r\n", __FUNCTION__);


				ret = 0;
				goto main_QRCode_Deal_OVER;
			}

			CountMac((char*)pubkey1, pubkey1 + 20, (unsigned char*)verifyInput.Qrcode, 26, (unsigned char*)&c_serial_mac);//ǰ��16�ֽڣ����ܴ��˳˳�

			gCardinfo.c_serial = c_serial_mac;
			t = month_decide();
#ifdef _debug_QRCode_
			printf("[%s] month_decide t:%d\n", __FUNCTION__, t);
#endif
			if (t != 0) // ͬһ����
			{
				//if (t > 2) {
					led_on(LED_RED);
					memset(pubkey1, 0, 32);

					strcpy((char *)pubkey1, "��ά����ˢ");

					MessageBox(1, (char *)pubkey1);

					gBuInfo.restore_flag = 3;
				//}

				ret = 0;

				soundPlay((char*)voice_refleshRQC);

				goto main_QRCode_Deal_OVER;
			}

			ltyp = (stltyQRCandGPS*)verifyInput.Qrcode;

			t = BCDTime2Long((unsigned char *)&SysTime);
			memcpy(outbuf, ltyp->datetime, 10);
			outbuf[10] = 0;
			t1 = atol((char*)outbuf);
			printf("[%s] t:%d, t1:%d\n", __FUNCTION__, t, t1);

			if ((t - t1) > limitTime_LTYQRC) {//3����
				led_on(LED_RED);
				memset(pubkey1, 0, 32);

				strcpy((char *)pubkey1, "��ˢ�¶�ά��");

				MessageBox(1, (char *)pubkey1);

				soundPlay((char*)voice_refleshRQC);

				gBuInfo.restore_flag = 3;

				ret = 0;
				goto main_QRCode_Deal_OVER;
			}

			if (memcmp(ltyp->citycode, "130400", 4) != 0) {
				led_on(LED_RED);
				memset(pubkey1, 0, 32);

				strcpy((char *)pubkey1, "���Ǳ��̻���ά��");

				MessageBox(1, (char *)pubkey1);

				gBuInfo.restore_flag = 3;

				ret = 0;
				goto main_QRCode_Deal_OVER;
			}

			BuildQRCRecorde(ID_REC_QRC_LTY, outbuf, verifyInput.Qrcode, LTY_QRCODE_LEN, pr_success);
			WriRecorQRC(outbuf);
			money_msg(ID_REC_TOLL, INFINITE, s_sum1, 0);

			soundPlay((char*)voice_WelCome);

			save_h_month();


			ret = 1;
		}
		else
#endif
		{
			// check all the buffer is numberic
			for (i = 0; i < qrLen; i++)
			{
				if ((pQrBuf[i] < '0') || (pQrBuf[i] > '9'))
				{
					break;
				}
			}
			PRINT_DEBUG("main_QRCode_Deal.if.else.PRINT_DEBUG:%d\n", i);
			if (/*(memcmp(QRCdat, "62", 2) == 0*/ i == qrLen && qrLen < 30) {

				switch (pQrBuf[0])
				{
				case 0x31:
					MSG_LOG("΢�Ÿ�����\n");
					deal_type = ID_REC_QRC_TX;
					break;
				case 0x32:
					MSG_LOG("֧����������\n");
					deal_type = ID_REC_QRC_ZFB;
					break;
				case 0x36:
					MSG_LOG("������\n");
					deal_type = ID_REC_QRC_PBOC;
					break;
				default:
					break;
				}


#if SWITCH_PBOC
				if ((g_supportQR&qr_weiXin) == 0) {


					if (pQrBuf[0] == '1') {
						MessageBox(1, "��֧��΢�Ÿ�����");
						ret = -999;
					}
				}
				if ((g_supportQR&qr_aliPay) == 0) {

					if (pQrBuf[0] == '2') {
						MessageBox(1, "��֧��֧����������");
						ret = -999;
					}
				}
				if ((g_supportQR&qr_unPay) == 0) {

					MessageBox(1, "��֧������������");
					ret = -999;

				}
				if (ret != -999) {

					ret = qpboc_qr_main(pQrBuf, pubkey1);
					MSG_LOG("qpboc_qr_mainret:%d\n", ret);
				}
#else	// ��ʱ��֧�ָ�����
				ret = -1;
#endif
			}
			else {
				BCD_LOG(pQrBuf, qrLen, 1);
				ret = -1;
			}

			if (ret > 0) {
				MSG_LOG("money_msg:%d\n", s_sum1);
				//debugdata(rrrecd, ret, 1);
				//a_sum1 = a_sum1 - s_sum1;
				money_msg(ID_REC_TOLL, INFINITE, s_sum1, 0);
				MSG_LOG("SoundMessage:\n");
				SoundMessage(SOUND_DEAL);
				set_delay_send(set_delay_send_val); //��ʱ������
				MSG_LOG("QTXQRCWriteRecord:\n");
				WriRecorQRC(pubkey1);

				goto main_QRCode_Deal_OVER;
			}

			if (ret == -1) {
				MessageBox(1, "��ά���ʽ����");
			}
			else if (ret == -2) {
				MessageBox(1, "��ά���ʽ�汾����");
			}
			else if (ret == -3) {
				MessageBox(1, "��ά��ʱ�����");
			}
			else if (ret == -4) {
				MessageBox(1, "��ά��TAC����");
			}
			else if (ret == -5) {
				MessageBox(1, "��ά������");
			}
			else if (ret == -6) {
				MessageBox(1, "��ά�����ʹ���");
			}
			else if (ret == -7) {
				MessageBox(1, "��ά����ҵʹ�ô���");
			}
		}
		goto main_QRCode_Deal_OVER;
	}
	return;
main_QRCode_Deal_OVER:
	if (ret < 0) {
		SoundMessage(SOUND_FLING_MONEY);
		led_off(LED_YELLOW);
	}

	sleep(1);
	//gBuInfo.restore_flag = 3;
	tcflush(m_u32UartQRCodeFd, TCIFLUSH);
	gQRCdataFlag = 0;
	qrLen = 0;
	return;
}




