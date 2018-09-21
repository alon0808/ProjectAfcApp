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
#include "RamStorage.h"
#include "qpboc_head.h"
#include "szct.h"

//#define _debug_CPU_

#ifdef BUS_CPU_

extern unsigned char gchn_eng;
unsigned int did_sum2 = 0;

typea_public_inf_zaozuang stuPubInfZaoZuang;
pboc_debit_capp_purchase stuDebitPurchase;
pboc_sam_init_purchase stuSamInitPurchase;
pboc_init_capp_purchase_res stuInitPurchaseRet;


extern unsigned char CPUDealCard(unsigned char mode, unsigned char cool);

unsigned char CPUPsamInitialize(void)
{	//������CPU��û���õ�PSAM����

	return ST_OK;
}


void end_close_card(unsigned char moded)
{
	end_card();
	// 	unsigned int i;
	// 	unsigned int sno1;
	// 	unsigned char buffer[10];
	// 	//	unsigned char key;
	// 	
	// 	sno1 = c_serial;
	// 	i = 0;
	// 	do {
	// 		if(moded == 1){
	// 			PcdRfReset(5);
	// 			delayxms(10);
	// 		}
	// 		i++;
	// 		if (GetTypeAUID(buffer)!=0) 
	// 		{
	// 			if (memcmp((unsigned char *)&sno1,buffer,4) ==0) 
	// 			{
	// 				i=0;
	// 			}
	// 		}
	// 	} while(i<5);
	// 	MifareHalt();
	// 	return;
}

unsigned char CPU_Read_Money(unsigned char *remb, unsigned char mode)
{
	unsigned char sndbuf[64];
	unsigned char revbuf[64];
	unsigned char ret;
#ifdef _debug_CPU_
	unsigned int temp;
#endif

	memcpy(sndbuf, "\x02\x80\x5c\x00\x02\x04", 6);
	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 6);
	if (ret == 0) {
		return 1;
	}
	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0) {
		return 2;
	}
#ifdef _debug_CPU_
	debugstring("---remb:====");
	debugdata(revbuf, ret, 1);
	memcpy((unsigned char*)&temp, revbuf + 1, 4);
	RevertTurn(4, (unsigned char*)&temp);
	sprintf((char *)sndbuf, "���:%d.%02d", temp / 100, temp % 100);
	debugstring((char*)sndbuf);
#endif
	memcpy(remb, revbuf + 1, 4);

	return 0;
	//	cmdbuff[0] = 0x03;
	//	picc_b_exchange_block_2(cmdbuff, 1, revbuff+10, 4);
}


unsigned int get_Month(unsigned char *rev)
{
	unsigned char i, ji;
	unsigned char date[3];
	unsigned char dateSum = 0;
	unsigned char buffer_29[16], buffer_30[16];
	unsigned int temp10, uii, ui30, uinow;
	unsigned int s_sumTmp;
	unsigned int SUMa = 0;
	unsigned int SUMb = 0;
	unsigned int SUMc = 0;
	unsigned long Pm = 0;
	unsigned char block29 = ST_ERROR;
	unsigned char block30 = ST_ERROR;
	unsigned char block2930 = ST_ERROR;
	unsigned int  value;
#ifdef _debug_CPU_
	debugstring("get_Month_purs:");
	debugdata(rev, 64, 1);

#endif
	GetDateTime();
	memcpy(buffer_29, rev, 16);
	memcpy(buffer_30, rev + 16, 16);
	memcpy(date, &SysTime.year_l, 3);          //������
	dateSum = (((date[1] & 0xf0) >> 4) * 10) + (date[1] & 0x0f);//��ǰ�·ݵ�ֵ
	//PRINT_DEBUGBYS("SysTime.year_l:", &SysTime.year_l, 3);

	if (CPU_Read_Money((unsigned char*)&value, 3))
		return ST_ERROR;
	RevertTurn(4, (unsigned char*)&value);
	s_sum1 = get_Month_s_sum1();
	//	s_sum1=1;
	s_sumTmp = s_sum1;//��ֵ ��һ��
	a_sum1 = value;
	did_sum2 = 0;
	if (a_sum1 <= 0)//ȥ��Ǯ
		return 100;

	if (gCardinfo.Zicard_catalog == 0xA1) {//Ϊ��Ʊ
#ifdef _debug_
		debugstring("Month year!!!\r\n");
		debugdata(date, 3, 1);
#endif
		uii = 0;
		memcpy((unsigned char*)&uii, buffer_29, 3);//��Ч��
		RevertTurn(4, (unsigned char*)&uii);
		ui30 = 0;
		memcpy((unsigned char*)&ui30, buffer_30, 3);//��Ч��
		RevertTurn(4, (unsigned char*)&ui30);
		uinow = 0;
		memcpy((unsigned char*)&uinow, (unsigned char*)&SysTime.year_l, 3);
		RevertTurn(4, (unsigned char*)&uinow);


		if (uii < ui30) {	//15 16
			if (uinow < uii) {
				block29 = ST_OK;
				block2930 = ST_OK;//���껹��
			}
			else if (uinow < ui30) {
				block30 = ST_OK;//û����һ����
			}
			else
				return 100;
		}
		else {	//17 16
			if (uinow < ui30) {
				block30 = ST_OK;
				block2930 = ST_OK; //���껹��
			}
			else if (uinow < uii) {//19
				block29 = ST_OK;  //������ԭ����30�ĳ�29û����һ����
			}
			else
				return 100;
		}
		if (block29 == ST_OK) {
			temp10 = 0;
			memcpy((unsigned char*)&temp10, buffer_29 + 13, 2);//��ǰ����ܵ���
			SUMb = temp10;
			SUMa = 0;
			if (block2930 == ST_OK) {//��һ�껹��
				temp10 = 0;
				memcpy((unsigned char*)&temp10, buffer_30 + 13, 2);//��ǰ����ܵ���
				SUMb += temp10;
				SUMa = temp10;
			}
			SUMc = SUMb;
		}
		else {
			temp10 = 0;
			memcpy((unsigned char*)&temp10, buffer_30 + 13, 2);//��ǰ����ܵ���
			SUMb = temp10;
			SUMa = 0;
			if (block2930 == ST_OK) {//��һ�껹��
				temp10 = 0;
				memcpy((unsigned char*)&temp10, buffer_29 + 13, 2);//��ǰ����ܵ���
				SUMb += temp10;
				SUMa = temp10;
			}
			SUMc = SUMb;
		}
	}
	else if (gCardinfo.Zicard_catalog == 0xA2) {//Ϊ��Ʊ
#ifdef _debug_
		debugstring("Month Quarter!!!\r\n");
#endif
		//	date[0]=(((date[0]&0xf0)>>4)*10)+(date[0]&0x0f);//��ǰ��ݷݵ�ֵ
		if (date[0] == buffer_29[0])//
		{
			block29 = ST_OK;
			if (date[0] < buffer_30[0])
			{
				block2930 = ST_OK;//���껹��
			}
		}
		else if (date[0] == buffer_30[0])
		{
			block30 = ST_OK;
			if (date[0] < buffer_29[0])
				block2930 = ST_OK;//���껹��
		}
		else
		{
			return 100;	   //����û�г伾Ʊ
		}

		if (block29 == ST_OK)
		{
			uii = 0;
			ji = ((dateSum - 1) / 3);//���� dataSum=0 - 12
			memcpy((unsigned char*)&uii, buffer_29 + 1 + (ji * 3), 3);
#ifdef _debug_
			debugstring("��29��");
			debugdata((unsigned char*)&ji, 1, 1);
			debugdata((unsigned char*)&uii, 4, 1);
#endif
			if (uii == 0) //����û�г��
			{
				return 100;
			}
			for (i = ji; i < 4; i++) {
				memcpy((unsigned char*)&uii, buffer_29 + 1 + (i * 3), 3);
				SUMb += uii;
#ifdef _debug_
				debugstring("�����ܵ�SUMb��");
				debugdata((unsigned char*)&SUMb, 4, 1);
#endif
			}
			temp10 = 0;
			memcpy((unsigned char*)&temp10, buffer_29 + 13, 2);
			SUMc = temp10;			//�������µ�����

			if (block2930 == ST_OK)
			{
				for (i = 0; i < 4; i++) {
					memcpy((unsigned char*)&uii, buffer_30 + 1 + (i * 3), 3);
					SUMb += uii;
#ifdef _debug_
					debugstring("�����ܵ�sumb��");
					debugdata((unsigned char*)&SUMb, 4, 1);
#endif
				}
				temp10 = 0;
				memcpy((unsigned char*)&temp10, buffer_30 + 13, 2);
				SUMc += temp10;
			}
			memcpy((unsigned char*)&uii, buffer_29 + 1 + (ji * 3), 3);
			SUMa = SUMb - uii;
		}
		else if (block30 == ST_OK)
		{
			uii = 0;
			ji = ((dateSum - 1) / 3);//���� dataSum=0 - 12
			memcpy((unsigned char*)&uii, buffer_30 + 1 + (ji * 3), 3);
#ifdef _debug_
			debugstring("��30��");
			debugdata((unsigned char*)&ji, 1, 1);
			debugdata((unsigned char*)&uii, 4, 1);
#endif
			if (uii == 0) //����û�г��
			{
				return 100;
			}
			for (i = ji; i < 4; i++) {
				memcpy((unsigned char*)&uii, buffer_30 + 1 + (i * 3), 3);
				SUMb += uii;
#ifdef _debug_
				debugstring("�����ܵ�3��");
				debugdata((unsigned char*)&SUMb, 4, 1);
#endif
			}
			temp10 = 0;
			memcpy((unsigned char*)&temp10, buffer_30 + 13, 2);
			SUMc = temp10;			//�������µ�����

			if (block2930 == ST_OK)
			{
				for (i = 0; i < 4; i++) {
					memcpy((unsigned char*)&uii, buffer_29 + 1 + (i * 3), 3);
					SUMb += uii;
#ifdef _debug_
					debugstring("�����ܵ�4��");
					debugdata((unsigned char*)&SUMb, 4, 1);
#endif
				}
				temp10 = 0;
				memcpy((unsigned char*)&temp10, buffer_29 + 13, 2);
				SUMc += temp10;
			}
			memcpy((unsigned char*)&uii, buffer_30 + 1 + (ji * 3), 3);
			SUMa = SUMb - uii;
		}
		if ((SUMb < SUMa) || (SUMb > SUMc) || (SUMa > SUMc))
		{
			//	debugstring("����������\r\n");
			return 100;
		}

	}
	else if (gCardinfo.Zicard_catalog == 0xA4)	//�����ÿ���´�������Ʊ
	{
		//		return ST_OK;
		SUMa = 0;
		SUMb = a_sum1;
	}
	else {//����Ʊ

#ifdef _debug_
		PRINT_DEBUG("Month Month...%02X!!!\r\n", date[0]);
		BCD_LOG(date, 3, 1);
		BCD_LOG(buffer_29, 16, 1);
		BCD_LOG(buffer_30, 16, 1);
#endif
		//	date[0]=(((date[0]&0xf0)>>4)*10)+(date[0]&0x0f);//��ǰ��ݷݵ�ֵ
		if (date[0] == buffer_29[0]) //��ǰ����29
		{
			block29 = ST_OK;
			if (date[0] < buffer_30[0])//���껹�д���
			{
				block2930 = ST_OK;
			}
		}
		else if (date[0] == buffer_30[0]) //��ǰ����30
		{
			block30 = ST_OK;
			if (date[0] < buffer_29[0])//���껹�д���
				block2930 = ST_OK;
		}
		else
		{
			return 100;	   //��Ʊ�黵�ˣ�ȥ��Ǯ								 
		}
#ifdef _debug_
		MSG_LOG("block29, block30:%d, %d\n", block29, block30);
#endif // _debug_

		if (block29 == ST_OK)
		{
			if (!buffer_29[dateSum]) //��ǰ�·�û�г�ֵ
			{
				return 100;
			}
			for (i = dateSum; i < 13; i++)
				SUMb += buffer_29[i];//��ǰ���Ժ� SUMb
			temp10 = 0;
			memcpy((unsigned char*)&temp10, buffer_29 + 13, 2);
			SUMc = temp10;			//�������µ�����
			if (block2930 == ST_OK)
			{
				for (i = 1; i < 13; i++)
					SUMb += buffer_30[i];//�����ܺ�
				temp10 = 0;
				memcpy((unsigned char*)&temp10, buffer_30 + 13, 2);
				SUMc += temp10;
			}
			SUMa = SUMb - buffer_29[dateSum];	//�����µĴ���
		}
		else if (block30 == ST_OK)
		{
			if (!buffer_30[dateSum])
			{
				return 100;
			}
			for (i = dateSum; i < 13; i++)
				SUMb += buffer_30[i];

			temp10 = 0;
			memcpy((unsigned char*)&temp10, buffer_30 + 13, 2);
			SUMc = temp10;
			if (block2930 == ST_OK)
			{
				for (i = 1; i < 13; i++)
					SUMb += buffer_29[i];
				temp10 = 0;
				memcpy((unsigned char*)&temp10, buffer_29 + 13, 2);
				SUMc += temp10;
			}
			SUMa = SUMb - buffer_30[dateSum];
		}
		if ((SUMb < SUMa) || (SUMb > SUMc) || (SUMa > SUMc))
		{
			MSG_LOG("������\r\n");
			return 100;
		}
	}
	Pm = a_sum1;	//��Ч��������ǰ��+�Ժ��´�����
#ifdef _debug_
	debugstring("��һ��SUMa:");
	debugdata((unsigned char*)&SUMa, 4, 1);//�������Ժ�ĵ���
	debugstring("��SUMb:");
	debugdata((unsigned char*)&SUMb, 4, 1);//���¼��Ժ����
	debugstring("SUMc:");
	debugdata((unsigned char*)&SUMc, 4, 1);
#endif

#ifdef _debug_
	debugstring("Month MMM 6 !!!\r\n");
	debugdata((unsigned char*)&a_sum1, 4, 1);
#endif

	if ((Pm - s_sum1) < SUMa)//��ʱ���Ѿ�����
	{
		MSG_LOG("������\r\n");
		return 100;
	}
	if (s_sum1 > 3) {
		MSG_LOG(" �������� \r\n");
		return ST_ERROR;
	}
	//��ǰ�����ڳ����º�ĵ���������С�����е�����
	if ((Pm >= SUMa) && (Pm <= SUMb))
	{
		MSG_LOG("�����۷�\r\n");
		if (a_sum1 >= s_sum1)
		{
			a_sum1 -= s_sum1;
			dis_sum2 = a_sum1 - SUMa;
		}
		else
		{
			MSG_LOG("������1\r\n");
			return 100;
		}
	}
	else if (Pm > SUMb)//������ ���¼��Ժ�������������ǰ�ġ�
	{
		MSG_LOG("�������ǰ�Ŀ۷�\r\n");
		did_sum2 = Pm - SUMb;		//���ڴ���
		s_sum1 = Pm - SUMb + s_sumTmp;
		if (a_sum1 >= s_sum1)
		{
			a_sum1 -= s_sum1;
			if (a_sum1 > SUMa)
				dis_sum2 = a_sum1 - SUMa;	//dis_sum2--����ʣ��
			else
				dis_sum2 = a_sum1;
			MSG_LOG("s_sum1:%d\r\n", s_sum1);
		}
		else
		{
			MSG_LOG("����۷�\r\n");
			return 100;
		}
	}
	else
		return ST_ERROR;

	return ST_OK;

}


unsigned char CPU_month_deal(void)
{

	unsigned char errorf = 0, i = 0;
	unsigned char sndbuf[100];
	unsigned char revbuf[130];
	unsigned char TACC[8];
	unsigned char psRandom[10];
	//	unsigned char PSAMDsno[4];
	unsigned int revLen, ret;
	//	unsigned int value;
	//	unsigned int pmoney;
	//	unsigned int itemp;
	unsigned short ii;
	unsigned char month[2][64];
	unsigned char mothFlag;
	unsigned char monthstyle;
	memset(revbuf, 0, 128);
#ifdef _debug_CPU_
	MSG_LOG("CPU _MONTH _DEAL::%08X,%08X\r\n", gCardinfo.c_serial, card_ser);
#endif
	if (gDeviceParaTab.stopflag)
		return ST_ERROR;

	if (cardSound == 0xaa) {//Ǯ����Ҫ��ˢ��ȥǮ����Ǯ
		if (card_ser != gCardinfo.c_serial) {
			card_ser = 0;
			cardSound = 0;
			//return ST_ERROR;
		}
		else
			return 3;
	}

	if (cardSound == 0xbb) {//��Ʊ��Ҫ��ˢ
		if (card_ser != gCardinfo.c_serial) {
			card_ser = 0;
			cardSound = 0;
			//return ST_ERROR;
		}
	}


	//**************************************
	memcpy(sndbuf, "\x02\x00\xA4\x04\x00\x08\x50\x41\x59\x2E\x59\x50\x2E\x05", 14);
	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 14);
	memcpy(sndbuf, "\x02\x00\xA4\x00\x00\x02\x3F\x02", 8);//�д˿���Ҫ���ļ���������ȡ����ʱ��Ҫ����һ�¡�
	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 8);

#ifdef _debug_CPU_
	debugdata(sndbuf, 15, 1);
	debugdata(revbuf, ret, 1);
#endif

	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
	{
		//return ST_ERROR;
		memcpy(sndbuf, "\x02\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x02", 15);
		ret = RfPassthrough(revbuf, 2, sndbuf, 15);

#ifdef _debug_CPU_
		debugdata(sndbuf, 15, 1);
		debugdata(revbuf, ret, 1);
#endif
		if ((ret == 0) || (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)) {
			MessageBox(1, "��Ʊ��������!");
			led_on(LED_RED);
			return ST_OK;
		}//��Ʊ���ļ�������

	}

	//��ȡ15�ļ��������ӿ���
	memcpy(sndbuf, "\x02\x00\xB0\x95\x00\x1E", 6);//��Ӧ�����к���Ϊ��ɢ����
	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 6);
	if (ret == 0)
		return ST_ERROR;
#ifdef _debug_CPU_
	debugstring("G24 read 95:");
	debugdata(revbuf, ret, 1);
	debugdata((unsigned char*)&ret, 4, 1);
#endif

	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
		return ST_ERROR;

	//A0����Ʊ��A1����ƱƱ A2�ǻ�Ʊ 03 ��Ǯ��
	gCardinfo.Zicard_catalog = revbuf[29];//�ӿ���
#ifdef _debug_CPU_
	printf("zic:%02X\r\n", gCardinfo.Zicard_catalog);
#endif

	//if (gCardinfo.Zicard_catalog != 0xA4)	{	}
	//0003�ļ�	//0~15	//16~31	//32_48
	memcpy(sndbuf, "\x02\x00\xB0\x83\x00\x20", 6);//����Ʊ32���ֽ�
	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 6);
#ifdef _debug_CPU_
	debugstring("0003�ļ�:");	//19000000000000000000000000000019180000000000000000780000007800189000
	debugdata(revbuf, ret, 1);
#endif
	if (ret == 0)
		return ST_ERROR;

	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
		return 3;	//���û�д��ļ���ȥ��Ǯ
					//*********************************

	ret = get_Month(revbuf + 1);
	MSG_LOG("get_month ret:%d\r\n", ret);
	//#ifndef _debug_
	if (ret == ST_ERROR)
		return ST_ERROR;

	if (ret == 100)
		return 3;//ȥ��Ǯ
				 //#endif

				 //#ifdef BUS_LiuPanSui_
	ret = CPUDealCard(MONTH_CARD, 0);
	//#else
	//		ret = CPUDealCard(MONTH_CARD, 0);
	//#endif
	return ret;
}


unsigned char ZJB_CARD;//ס������׼�Ŀ�
unsigned char Safe_Code[9];		//��ȫ��֤��
unsigned char CPUcardType(void)
{
	//	unsigned char PSAMDsno[4];
	unsigned int pmoney, itemp, value;
	unsigned int ret;
	stJSBF15 JSBFile15;
	unsigned char sndbuf[100];
	unsigned char revbuf[100];
	unsigned char uc1;

	gCardinfo.gMCardCand = CARDSTYLE_CPU;

	ZJB_CARD = 0;

	// 	PcdRfReset(5);
	// 	delayxms(10);
	// 	profile(0);
	// 	memcpy(sndbuf,"\xe0\x50",2);
	// 	ret = RfPassthrough(revbuf, 2, sndbuf, 2);
	ret = CPURats(revbuf);

	usleep(2000);

#ifdef _debug_CPU_
	printf("CPU start!ret=%d\r\n", ret);
	if (ret < 100) {
		debugdata(revbuf, ret, 1);
	}
#endif

	////Select PSE 00A404000E315041592E5359532E4444463033
#ifdef CPU_PSE
#ifdef _debug_CPU_
	debugstring("ѡ��Ӧ�� PSE:\r\n");
#endif
	memcpy(sndbuf, CPU_PSE, 20);
	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 20);
	if (ret == 0)
		return ST_ERROR;
#ifdef _debug_CPU_
	debugstring("ѡ��PSE:");
	debugdata(sndbuf, 20, 1);
	debugdata(revbuf, ret, 1);
#endif
	// 	if(memcmp(revbuf+ret-2, "\x90\x00", 2) != 0)	//���ѡ����Ҳû�£����г��Ŀ���û������ļ��ġ�
	// 		return ST_ERROR;

#endif //#ifdef CPU_PSE

	//ѡ��Ӧ��Ŀ¼ 3F01   03 6F 0C 84 06 5A 42 47 47 51 42 A5 04 9F 08 90 00
#ifdef _debug_CPU_
	debugstring("ѡ��Ӧ�� 3F01:::��ʼ\r\n");
#endif

	memcpy(sndbuf, "\x02\x00\xA4\x00\x00\x02\x3F\x01", 8);//�д˿���Ҫ���ļ���������ȡ����ʱ��Ҫ����һ�¡�
	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 8);
#ifdef _debug_CPU_
	debugdata(sndbuf, 8, 1);
	debugstring("RCV::");
	debugdata(revbuf, ret, 1);
#endif
	if (ret == 0)
		return ST_ERROR;

	if (memcmp(revbuf + ret - 2, "\x6A\x82", 2) == 0) {//����������ļ������Ͱ�AIDѡ�ļ�

		memcpy(sndbuf, "\x02\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01", 15);
		deal2or3(sndbuf);
#ifdef _debug_CPU_
		debugstring("select AID1:");
		debugdata(sndbuf, 15, 1);
#endif
		ret = RfPassthrough(revbuf, 2, sndbuf, 15);
#ifdef _debug_CPU_
		debugdata(revbuf, ret, 1);
#endif
		if (ret == 0)
			return ST_ERROR;
	}

	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
		return ST_ERROR;

	memcpy(sndbuf, "\x02\x00\xB0\x95\x00\x1E", 6);//��Ӧ�����к���Ϊ��ɢ����
	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 6);
	if (ret == 0)
		return ST_ERROR;

#ifdef _debug_CPU_
	debugstring("G24 read 95:");
	debugdata(revbuf, ret, 1);
	debugdata((unsigned char*)&ret, 4, 1);
#endif
	//03A0000000030000010301020000000000201100162011052420110830A9389000 
	//03010136201E201211212015010110000000100000000000000000000000009000

	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
		return ST_ERROR;

	if (memcmp(revbuf + 1, "\xA0\x00", 2) != 0) {
		ZJB_CARD = 1;//ס�����Ŀ�
		printf("����������");
		return ST_ERROR;
	}

	if (ZJB_CARD)
		gCardinfo.card_catalog = revbuf[7];//����//ס����
	else
		gCardinfo.card_catalog = revbuf[11];//����

	if ((gCardinfo.card_catalog >= CARD_WHITE_BUS) && (gCardinfo.card_catalog <= 60))//��û���۳��Ŀ�
		return 8;

	if (revbuf[12] == 0x04) {			//�ڿ�
#ifdef _debug_CPU_
		debugstring("revbuf[12] == 0x04!!!\r\n");
#endif
		return CARD_BLACK;
	}


	debugstring("gCardinfo.card_catalog111:");
	debugdata((unsigned char*)&gCardinfo.card_catalog, 1, 1);

#ifdef BUS_Cloud_
	if (gCardinfo.card_catalog < 32)
		gCardinfo.card_catalog += CARD_NORMAL;
#endif

	if (ZJB_CARD == 1) {
		memcpy(gCardinfo.TradeCode, revbuf + 1, 2);//��������ʶ
		memcpy(gCardinfo.CityCode, revbuf + 3, 2);//���д���
		memcpy(gCardinfo.CardCheckCode, revbuf + 31, CARD_CHECH_CODE_LEN);

		memcpy((unsigned char*)&JSBFile15, revbuf + 1, sizeof(stJSBF15));
		memcpy(gCardinfo.PublishBickerH, JSBFile15.YYNO, 4);//��4�ֽڿ���
		memcpy(gCardinfo.PublishBicker, JSBFile15.YYNO + 4, 4);//������ˮ��	
		gCardinfo.card_catalog = JSBFile15.recre[0];			//
		gCardinfo.Zicard_catalog = 0;

		switch (JSBFile15.recre[1])//������־
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

		memcpy((unsigned char*)&stuPubInfZaoZuang, JSBFile15.YYNO, 8);//�����к�
		memcpy(stuPubInfZaoZuang.cApp_sn, JSBFile15.FYFlag, 10);	//��Ӧ�����кţ������ģ�����
		memcpy(stuPubInfZaoZuang.cCitycode, JSBFile15.citycode, 2);	//

	}
	else {
		memcpy((unsigned char*)&stuPubInfZaoZuang, revbuf + 13, 8);//�����к�
		memcpy(stuPubInfZaoZuang.cApp_sn, revbuf + 12, 10);	//��Ӧ�����кţ������ģ�����
		memcpy(gCardinfo.CityCode, revbuf + 3, 2);//���д���
		memcpy(gCardinfo.CardCheckCode, revbuf + 5, 4);//MAC��
	}

	if (ZJB_CARD == 0)
		memcpy(gCardinfo.PublishBicker, revbuf + 13, 4);//������ˮ��

#ifndef NO_CPU_PSAM_
	pmoney = BCD2HEX(stuPubInfZaoZuang.cPublisherSign, 8);
	memcpy(gCardinfo.PublishBicker, (unsigned char*)&pmoney, 4);
#endif

#ifdef NO_CPU_PSAM_
#ifdef BUS_PRO_JSB
	//memcpy((unsigned char*)&gCardinfo.c_serial, gCardinfo.PublishBicker, 4);//������û��ȡ������ʹ�õ�����ˮ��
#endif
#endif

#ifdef _debug_CPU_
	debugstring("gCardinfo.card_catalog:");
	debugdata((unsigned char*)&gCardinfo.card_catalog, 1, 1);
	debugdata((unsigned char*)&stuPubInfZaoZuang, 50, 1);
	debugdata(gDeviceParaTab.rate + gCardinfo.card_catalog, 1, 1);
#endif

	memcpy((unsigned char*)&itemp, (unsigned char *)&SysTime, 4);//��ǰ����
	RevertTurn(4, (unsigned char*)&itemp);
	memcpy((unsigned char*)&value, revbuf + 21, 4);//��������
	RevertTurn(4, (unsigned char*)&value);
	memcpy((unsigned char*)&pmoney, revbuf + 25, 4);//��Ч����
	RevertTurn(4, (unsigned char*)&pmoney);

#ifdef _debug_CPU_
	debugdata(gCardinfo.PublishBicker, 4, 1);
	debugdata((unsigned char*)&value, 4, 1);
	debugdata((unsigned char*)&pmoney, 4, 1);
#endif
	// 	if((itemp < value)||(itemp > pmoney)){
	// 		return CARD_NO_TIME;//���ڻ�ĩ����
	// 	}

	if (IsInBlackList(gCardinfo.PublishBicker)) {
		return CARD_BLACK_PRO;
	}


	if (ZJB_CARD == 1) {
		memcpy(sndbuf, "\x80\xca\x00\x00\x09", 5);
#ifdef _debug_CPU_
		debugstring("ȡ��ȫ��֤��:");
		debugdata(sndbuf, 5, 1);
#endif
		ret = RfPassthrough(revbuf, 2, sndbuf, 5);//��ȡ�û�����ȫ��֤ʶ����

		if (ret == 0)
			return 4;
		if (memcmp(revbuf + ret - 2, "\x90\x00", 2) == 0) {
			memcpy(Safe_Code, revbuf + 1, 9);
		}
#ifdef _debug_CPU_
		debugdata((unsigned char*)&ret, 1, 1);
		debugdata(revbuf, ret, 1);
		debugstring("Safe_Code:");
		debugdata(Safe_Code, 9, 1);
#endif

		memcpy(sndbuf, revbuf + 1, 9);

		if (gCardinfo.gMCardCand == CARDSTYLE_24Mobile)
		{
			uc1 = PSAM_YD;
		}
		else {
			uc1 = PSAM_GJ;
		}
		if (CpuCardCheck(uc1, revbuf, sndbuf, 9) == ST_ERROR)//PSAM ����֤��ȫ��֤�����,�¼�
			return 4;

	}

	if (gCardinfo.card_catalog >= 0x40)
		ret = gCardinfo.card_catalog - 0x40;
	else
		ret = gCardinfo.card_catalog;

	PRINT_DEBUG("gDeviceParaTab.rate[gCardin11111:%d\n", gDeviceParaTab.rate[gCardinfo.card_catalog]);
	if (gDeviceParaTab.rate[gCardinfo.card_catalog] == 0) {//�ۿ�Ϊ0����������
		return ST_OK;
	}

	if (gDeviceParaTab.rate[gCardinfo.card_catalog] == 202)//�������
	{
		return CARD_FREE;
	}
	else if ((gDeviceParaTab.rate[gCardinfo.card_catalog] == 104) || (gDeviceParaTab.rate[gCardinfo.card_catalog] == 204)) {
		return CARD_forbid;//�˿�����ֹʹ��
	}
	if (gCardinfo.oddMonthSector != 0xff)//��Ʊ
	{
		return MONTH_CARD;
	}
	else
	{
		return CARD_MONEY;			//Ǯ��
	}


	if ((gDeviceParaTab.rate[ret] == 204) || (gDeviceParaTab.rate[ret] == 104))//��ֹ
	{
		return CARD_forbid;
	}
	else if ((gDeviceParaTab.rate[gCardinfo.card_catalog] == 101) || (gDeviceParaTab.rate[gCardinfo.card_catalog] == 201))
	{//ֻˢ��Ʊ��
		return CARD_forbid;// CPU��û�д�, �ͽ�ֹˢ����
	}//����ר�߿� 2014.4.8

	return CARD_MONEY;			//Ǯ��
}
unsigned char cpuWriteFile03(unsigned char mode)
{
	unsigned char sndbuf[100];
	unsigned char revbuf[128];
	unsigned char buffer_2[20];
	unsigned char buffer_3[64];
	unsigned char psRandom[16];
	unsigned char cMac[16];
	unsigned int ret;

	if (gDeviceParaTab.ChangeType != 0xAA)
	{
		MSG_LOG("%s No ChangeType:%02X\r\n", __FUNCTION__, gDeviceParaTab.ChangeType);
		return ST_ERROR;
	}

	if (mode == MONTH_CARD) {
		return ST_ERROR;	//�ο�����д��
	}

	if ((gCardinfo.card_catalog != CARD_NORMAL) || (gDeviceParaTab.ChangeType != 0xAA)) {	//ֻ����ͨ����д
		return ST_ERROR;
	}

	memset(buffer_3, 0, 16);

	GetDateTime();//ȡʱ��
	sysferead(BIT_LINENO, 3, buffer_2);//ȡ��·��
	memcpy(buffer_3, (unsigned char*)&SysTime, 7);
	memcpy(buffer_3 + 7, buffer_2, 3);//��·��
#ifdef _debug_
	memset(buffer_3 + 7 + 3, 0x11, 4);//�ĳ����кŰ�,,����ʱ��ͬһ̨����Ҳ���Ż� 
#else
	Get_SerialNum(buffer_3 + 7 + 3);//�ĳ����кŰ�
#endif
									//	Ascii2BCD(gDeviceParaTab.DeviceNo,buffer_3+7+3,8); //�����豸��� 2016-7-28 15:54:49
	if (memcmp(old_Infor.resear, "\xAA\xAA", 2) == 0)//�Ѿ��Żݹ���
		memset(buffer_3, 0, 16);
	buffer_3[15] = MiBlockInvalid(0, 15, buffer_3);//У����


	clr_wdt();
	memcpy(sndbuf, "\x02\x00\xB0\x83\x00\x20", 6);//��03�ļ�������д�����Żݻ�����Ϣ��
	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 6);
	if (ret == 0)
		return ST_ERROR;

#ifdef _debug_CPU_
	debugstring("CPU read 03:");
	debugdata(revbuf, ret, 1);
	debugdata((unsigned char*)&ret, 4, 1);

	memcpy(buffer_3, "\x19\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x19\x18\x00\x00\x00\x00\x00\x00\x00\x00\x78\x00\x00\x00\x78\x00\x18", 0x20);
#endif
	//03A0000000030000010301020000000000201100162011052420110830A9389000 
	//03010136201E201211212015010110000000100000000000000000000000009000

	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
		return ST_ERROR;


#ifdef _debug_CPU_
	debugstring("д03�ļ��Ż���Ϣ::\r\n");
#endif

	memcpy(sndbuf, "\x02\x00\x84\x00\x00\x04", 6);//��Ӧ�����к���Ϊ��ɢ����

	deal2or3(sndbuf);
#ifdef _debug_CPU_
	debugstring("Send data:");
	debugdata(sndbuf, 6, 1);
#endif

	ret = RfPassthrough(revbuf, 2, sndbuf + 1, 5);
	if (ret == 0)
		return ST_ERROR;

#ifdef _debug_CPU_
	debugstring("get random:");
	debugdata(revbuf, ret, 1);
	debugdata((unsigned char*)&ret, 4, 1);
#endif
	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
		return ST_ERROR;
	memset(psRandom, 0, 8);
	memcpy(psRandom, revbuf + 1, 4);

	memcpy(sndbuf, "\x02\x04\xD6\x83\x00\x20", 6);//д�������20�ֽ�+MAC 4�ֽ�
												  //	memcpy(sndbuf, "\x02\x04\xD6\x95\x19\x05\x50", 7);//�޸���Ч��
	memcpy(sndbuf + 6, buffer_3, 0x20);
#ifdef _debug_CPU_
	debugstring("get MAC:");
	debugdata(psRandom, 8, 1);
	debugdata(sndbuf, 0x20 + 6, 1);
#endif
	getCPUMAC(2, psRandom, sndbuf + 1, 6 + 0x20 - 1, cMac);		//����

#ifdef _debug_CPU_
	debugstring("get MACOKK:");
	debugdata(cMac, 4, 1);
#endif
	memcpy(sndbuf + 0x20 + 6, cMac, 4);

	deal2or3(sndbuf);
#ifdef _debug_CPU_
	debugstring("write 03 CPU snd:");
	debugdata(sndbuf, 0x20 + 6 + 4, 1);
#endif
	ret = RfPassthrough(revbuf, 2, sndbuf + 1, 0x20 + 6 + 4 - 1);
	if (ret == 0)
		return ST_ERROR;
#ifdef _debug_CPU_
	debugstring("write 03 end:");
	debugdata(revbuf, ret, 1);
#endif
	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
		return ST_ERROR;
	else
		return ST_OK;

}


unsigned char CPUDealCard(unsigned char mode, unsigned char cool)
{

	unsigned char i;
	unsigned char sndbuf[100];
	unsigned char revbuf[128];
	unsigned char TACC[4];
	//	unsigned char PSAMDsno[4];
	unsigned int revLen, ret;
	unsigned int value;
	unsigned short ii;
#ifdef NO_CPU_PSAM_
	unsigned char CPUPKEY[16];
	unsigned char CPUpInData1[8];
	unsigned char CPUinitdat[8];
	unsigned char snddat[64];
#endif


	//ѡ��Ӧ��Ŀ¼ 3F01   03 6F 0C 84 06 5A 42 47 47 51 42 A5 04 9F 08 90 00
#ifdef _debug_CPU_
	debugstring("ѡ��Ӧ�� 3F01:��ʼ\r\n");
#endif

	if (cardSound == 0xaa) {
		if (card_ser != gCardinfo.c_serial) {
			card_ser = 0;
			cardSound = 0;
		}
	}

	if (Delay_Normal()) {
		return ST_ERROR;
	}
#ifndef _HD_XIAOER_OPEN
	if (month_decide()) // �ڶ���ȥ��Ǯ��(����������ˢ)
	{
		//��ʽ����ע�͵�----(ע�͵���������ˢ��)
		disp_no_swipe();
		return ST_OK;
	}
#endif

	CPU_Read_Money((unsigned char*)&value, 3);
	RevertTurn(4, (unsigned char*)&value);
	if (cardSound == 0xaa) {//��Ҫ��ˢ
#ifdef _debug_CPU_
		debugstring("aa���:");
		debugdata((unsigned char*)&value, 4, 1);
		debugdata((unsigned char*)&a_sumR, 4, 1);
#endif
		if (value < a_sumR) {//�ϴ��Ѿ��۳ɹ�
			memcpy(sndbuf, "\x02\x80\x5A\x00\x06\x02", 6);//memcpy(sndbuf, "\x02\x80\x5A\x00\x05\x02", 6);
			memcpy((unsigned char*)&ii, stuInitPurchaseRet.cSnq, 2);
			RevertTurn(2, (unsigned char*)&ii);
			ii += 1;
			RevertTurn(2, (unsigned char*)&ii);
			memcpy(sndbuf + 6, (unsigned char*)&ii, 2);
			sndbuf[8] = 8;
#ifdef _debug_CPU_
			debugstring("����:");
			debugdata(stuInitPurchaseRet.cSnq, 2, 1);
			debugdata(sndbuf, 9, 1);
#endif
			deal2or3(sndbuf);
			revLen = RfPassthrough(revbuf, 2, sndbuf, 9);
			if (revLen == 0)
				return ST_ERROR;
#ifdef _debug_CPU_
			debugdata(revbuf, revLen, 1);//02 00 0C 00 00 ED F2 97 9B 6E 94 37 BC 90 00 F2 03
#endif
			if (revLen == 0)//���س�ʱ,����
				return ST_ERROR;
			if ((memcmp(revbuf + revLen - 2, "\x90\x00", 2) != 0) && \
				(memcmp(revbuf + revLen - 2, "\x6C\x08", 2) != 0))
				goto CPUMAC2deal;//ȡ���ɹ�02 69 85 

			memcpy(TACC, revbuf + revLen - 6, 4);//���ص�TAC��
			memcpy(sndbuf, revbuf + revLen - 10, 4);//��MAC2��
													//			return ST_ERROR;
			goto CPUMAC2deal0;
		}

	}
	a_sumR = value;

#ifdef _debug_CPU_
	debugstring("a_sumR:");
	debugdata((unsigned char*)&a_sumR, 4, 1);
#endif


	// 	memcpy(sndbuf, "\x80\xca\x00\x00\x09", 5);
	// #ifdef _debug_CPU_
	// 	debugstring("ȡ��ȫ��֤��:");
	// 	debugdata(sndbuf, 5, 1);
	// #endif
	// 	ret = RfPassthrough(revbuf, 2, sndbuf, 5);//��ȡ�û�����ȫ��֤ʶ����
	// 	
	// 	if(ret == 0)
	// 		return ST_ERROR;
	// 	if(memcmp(revbuf+ret-2, "\x90\x00", 2)==0){
	// 		//		memcpy(Safe_Code,revbuf+1,9);
	// 	}
	// #ifdef _debug_CPU_
	// 	debugdata((unsigned char*)&ret, 1, 1);
	// 	debugdata(revbuf, ret, 1);
	// 	debugstring("Safe_Code:");
	// 	debugdata(revbuf,9,1);
	// #endif


#ifdef NO_CPU_PSAM_
	memset(psamZJB.CardNO, 0, 6);
	Get_SerialNum(psamZJB.CardNO + 2);
#ifdef _debug_CPU_
	debugstring("G24 psamZJB.CardNO:");
	debugdata(psamZJB.CardNO, 6, 1);
#endif
#endif
	//8050 0101 0B||��Կ��ʶ�� [1]��0x01��||���׽��[4](0x00000001)||�ն˻����[6]
	memcpy(sndbuf, "\x02\x80\x50\x01\x02\x0B", 6); i = 6;//11 INITIALIZE FOR PURCHASE����
	sndbuf[i++] = 0x01;
	//	gCardinfo.card_catalog = CARD_NORMAL; CPU�����п���
	if (mode == MONTH_CARD)
		s_sum1 = 1;
	else {
		s_sum1 = get_s_sum1(0);//ȡ�����ѽ��
#ifdef _debug_CPU_
		//PRINT_DEBUG("get_s_sum1:%d\n", s_sum1);
#endif
	}
	// #warning "����۷�-----"
	// 	if(s_sum1 == 0)//ttttttttttttttttt
	// 		s_sum1 = 1;

	//	if ((s_sum1 == 0) || (s_sum1 > 20000))
	if (s_sum1 > 10000)
		return 22;
	value = s_sum1;
#ifdef _debug_CPU_
	PRINT_DEBUG("value:%d\n", value);
#endif
	RevertTurn(4, (unsigned char*)&value);
	memcpy(sndbuf + i, (unsigned char*)&value, 4); i += 4;
	if (ZJB_CARD)
		memcpy(sndbuf + i, psamZJB.CardNO, 6);
	else {
		memcpy(sndbuf + i, psamZJB.CardNO, 6);
	}
	i += 6;
	//	sndbuf[i++] = 0x0f;
	deal2or3(sndbuf);
#ifdef _debug_CPU_
	debugstring("���ѳ�ʼ��:");
	printf("s_sum1:%d\r\n", s_sum1);
	debugdata(sndbuf, i, 1);
#endif
	ret = RfPassthrough(revbuf, 2, sndbuf, i);
#ifdef _debug_CPU_
	debugdata((unsigned char*)&ret, 4, 1);
	debugdata(revbuf, ret, 1);

	//	return ST_ERROR;
#endif
	if (ret == 0)
		return ST_ERROR;
	else {
		if (memcmp(revbuf + ret - 2, "\x94\x01", 2) == 0) {
			if (cool == 4)//��Ʊ��ˢ����������ʾ����
				return 4;
			else
				return 3;
		}
		else if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
			return 5;
	}

	memcpy((unsigned char*)&stuInitPurchaseRet, revbuf + 1, sizeof(stuInitPurchaseRet));
#ifdef _debug_CPU_
	debugdata((unsigned char*)&stuInitPurchaseRet, sizeof(stuInitPurchaseRet), 1);
	debugstring("MAC1����ָ��:");
#endif
	memcpy(stuSamInitPurchase.cRand, stuInitPurchaseRet.cRand, 4);
	memcpy(stuSamInitPurchase.cSnq, stuInitPurchaseRet.cSnq, 2);
	memcpy(stuSamInitPurchase.cMoney, (unsigned char*)&value, 4);

	memcpy((unsigned char*)&a_sum1, stuInitPurchaseRet.cBalance, 4);//���
	RevertTurn(4, (unsigned char*)&a_sum1);
	if (a_sum1 >= 5000000)
	{
		return 21;
	}

	gCardinfo.gucSoudnorm = 0;
#ifdef BUS_JIUJIANG_ //��Ǯ�����С��3Ԫʱ	���ֵ
	if ((a_sum1 >= s_sum1) && (a_sum1 < 300)) {
		gCardinfo.gucSoudnorm = 1;
	}
#else
	if ((a_sum1 >= s_sum1) && (a_sum1 < 500)) {
		gCardinfo.gucSoudnorm = 1;
	}
#endif

	stuSamInitPurchase.cType = 0x06;

	memcpy(stuSamInitPurchase.cDateTime, (unsigned char*)&SysTime, 7);

	stuSamInitPurchase.cKeyVer = 0x01;//stuInitPurchaseRet.cKeyVer;//0x05

	stuSamInitPurchase.cKeyEn = stuInitPurchaseRet.cKeyEn;

#ifdef FUJIAN_EKT_CPU_
	memcpy(stuSamInitPurchase.cPublisherSign, "\x36\x20\x80\x00\x00\x00\x00\x00", 8);//��28������
	memcpy(stuSamInitPurchase.cKeyFactor, stuPubInfZaoZuang.cPublisherSign, 8);//36������
#ifdef _debug_CPU_
	debugdata((unsigned char*)&stuSamInitPurchase, (36), 1);//+12
#endif
#else
	memcpy(stuSamInitPurchase.cKeyFactor, stuPubInfZaoZuang.cPublisherSign, 8);//��28������
	if (ZJB_CARD) {
		memcpy(stuSamInitPurchase.cPublisherSign, stuPubInfZaoZuang.cCitycode, 2);//���д���
		memcpy(stuSamInitPurchase.cPublisherSign + 2, "\xFF\x00\x00\x00\x00\x00", 6);//��36������
	}

	//	memcpy(stuSamInitPurchase.cPublisherSign, stuPubInfZaoZuang.cPublisherSign, 8);//���...
#ifdef _debug_CPU_
	debugdata((unsigned char*)&stuSamInitPurchase, (36), 1);//+12
#endif
#endif

															//52B58B730005000000010620110402154217 05000000000090002030
															//4ACF30AF 0005 00000001 06 20110402154422 01000000000090002030 
	memset(revbuf, 0xff, 50);
	memset((unsigned char*)&stuDebitPurchase, 0, sizeof(stuDebitPurchase));

	//--------------------------
	if (ZJB_CARD == 0) {
#ifdef NO_CPU_PSAM_
		if (getMiKey() != ST_OK)	//��֤������֤��
			return 8;
		//----------����KEY----------
#ifdef BUS_PRO_JSB
		memcpy(sndbuf, gCardinfo.CardCheckCode, 4);//����Դ��MAC+����ˮ+����ˮǰ2�ֽ�+6��0
		memcpy(sndbuf + 4, gCardinfo.PublishBicker, 4);
		memcpy(sndbuf + 8, gCardinfo.PublishBicker, 2);
		memset(sndbuf + 10, 0, 6);
		getJiuJiang_CPUKey(sndbuf, CPUPKEY);
#else
		memcpy(CPUPKEY, gCardinfo.MoneyKEYA, 6);
		memcpy(CPUPKEY + 6, gCardinfo.RecodeKEYA, 6);
		memcpy(CPUPKEY + 12, gCardinfo.MonthKEYA, 4);
#endif
		//----------����KEY----------

		//	memcpy(CPUPKEY, "\x0C\x62\x9F\x90\x31\xCC\x94\x43\x91\xD4\x7B\xF7\xEA\xD3\xEF\x34", 16);//����ʢ��13.56MHZ�ֻ���������Կ
		//	memcpy(CPUPKEY, "\x1B\xE5\xF1\x63\xF5\x08\x43\x69\x9E\x12\x0D\x74\xFA\xF0\x8E\x76", 16);
		memcpy(CPUpInData1, stuInitPurchaseRet.cRand, 4);//�����
		memcpy(CPUpInData1 + 4, stuInitPurchaseRet.cSnq, 2);
		memcpy(CPUpInData1 + 6, "\x00\x25", 2);
		memset(CPUinitdat, 0, 8);

		memcpy(snddat, (unsigned char*)&s_sum1, 4);
		RevertTurn(4, snddat);
		snddat[4] = 0x06;
		memcpy(snddat + 5, psamZJB.CardNO, 6);
		memcpy(snddat + 11, (unsigned char*)&SysTime, 7);//��18
#ifdef _debug_CPU_
		debugstring("Mac --:");
		debugdata(CPUPKEY, 16, 1);
#endif
		Mac(CPUPKEY, CPUpInData1, snddat, 18, CPUinitdat, revbuf);//1BE5F163F50843699E120D74FAF08E76 ����
		memmove(revbuf + 4, revbuf, 4);
		memcpy(revbuf, "\x00\x00\x00\x25", 4);
#else	//#ifdef NO_CPU_PSAM_

#ifdef FUJIAN_EKT_CPU_
		if (CpuCardSetMac(PSAM_YD, revbuf, (unsigned char*)&stuSamInitPurchase, 36) == ST_ERROR)
		{
			//		DeactiveCard(PSAM_YD);
			return 6;
		}
#else//#ifdef FUJIAN_EKT_CPU_
		if (CpuCardSetMac(PSAM_YD, revbuf, (unsigned char*)&stuSamInitPurchase, 28) == ST_ERROR)
		{
			//		DeactiveCard(PSAM_YD);
			return 6;
		}
#endif//#ifdef FUJIAN_EKT_CPU_
#endif	//#ifdef NO_CPU_PSAM_

	}
	else//ס�����Ŀ�
	{
		if (CpuCardSetMac(PSAM_GJ, revbuf, (unsigned char*)&stuSamInitPurchase, 36) == ST_ERROR)
		{
			//		DeactiveCard(PSAM_YD);
			return 6;
		}
	}
#ifdef _debug_CPU_
	debugstring("MAC OK!:");
	debugdata(revbuf, 8, 1);
#endif
	memcpy(stuDebitPurchase.cTermSnq, revbuf, 4);
	//	memcpy(stuDebitPurchase.cTermSnq,"\x00\x00\x00\x02",4);
	memcpy(stuDebitPurchase.cMac, revbuf + 4, 4);
	memcpy(stuDebitPurchase.cDateTime, (unsigned char*)&SysTime, 7);
	memcpy(psamZJB.PSAMLiuShou, revbuf, 4);//--4�ֽڵ��ն��ѻ��������  �� MAC1

	memcpy(sndbuf, "\x03\x80\x54\x01\x00\x0F", 6); i = 6;
	memcpy(sndbuf + i, (unsigned char*)&stuDebitPurchase, 0x0f); i += 0x0f;
	memset(revbuf, 0xff, 50);
	deal2or3(sndbuf);
#ifdef _debug_CPU_
	debugstring("chek Mac1:");
	debugdata(sndbuf, i, 1);
#endif

	ret = RfPassthrough(revbuf, 2, sndbuf, i);
	if (ret == 0) {
		//		debugdata(revbuf, 20, 1);
		//		debugdata(revbuf, 20, 1);
#ifndef BUS_PINGXIANG_
#ifdef AUDIO_SOUND_OLD
		audio_old(Audio_TRY_AGAIN);		//����ˢ
#else
		audio(Audio_TRY_AGAIN);		//����ˢ
#endif
#endif 
		cardSound = 0xaa;//������Ҫ��ˢ��
		card_ser = gCardinfo.c_serial;
		return 7;
	}

#ifdef _debug_CPU_
	debugstring("chek Mac1 OK:");
	debugdata((unsigned char*)&ret, 4, 1);
	debugdata(revbuf, 20, 1);
#endif
	if (memcmp(revbuf + ret - 2, "\x93\x02", 2) == 0)
		return 9;
	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0) {
		//		debugdata(revbuf, 20, 1);
		cardSound = 0xaa;//������Ҫ��ˢ��
		card_ser = gCardinfo.c_serial;
		return 7;
	}//��������

	memcpy(TACC, revbuf + ret - 10, 4);//���ص�TAC�� SPI -13
	memcpy(sndbuf, revbuf + ret - 6, 4);
#ifdef _debug_CPU_
	//	debugdata((unsigned char*)&pmoney, 4, 1);
	debugdata(sndbuf, 4, 1);
	debugstring("���ѳɹ�:\r\n");
#endif

#ifdef _debug_CPU_
	debugstring("TAC:");
	debugdata(TACC, 4, 1);
	debugstring("MAC2:");
	debugdata(sndbuf, 4, 1);
	debugdata((unsigned char*)&a_sum1, 4, 1);
#endif
	a_sum1 -= s_sum1;//Ҫ�ŵ�����������Ȼ ��ˢ��ʱ�� ��ʾ���ۿ� ��ʵû��
CPUMAC2deal0:
	if (ZJB_CARD)
		CPUMAC2(PSAM_YD, revbuf, sndbuf, 4);


CPUMAC2deal:
	memcpy(gCardinfo.PbocTac, TACC, 4);	// TAC��
#ifdef _debug_CPU_
	debugstring("����MAC2:");
	debugdata(revbuf, 10, 1);
#endif
	card_ser = 0;
	cardSound = 0;
	if (mode == MONTH_CARD)
	{
#ifdef _debug_CPU_
		MSG_LOG("MONTH_CARD:%2X, ", MONTH_CARD);
		BCD_LOG(&mode, 1, 1);
#endif
		gCardinfo.gucSoudnorm = 0;
		money_msg(ID_REC_MON, a_sum1, s_sum1 - did_sum2, 2);
	}
	else
		money_msg(ID_REC_TOLL, a_sum1, s_sum1, 2);

	Save_delayCardinfo(0);

	memset(revbuf, 0, 128);

	if (mode == MONTH_CARD)
	{
		BuildRecorde(0x80 | ID_REC_MON, revbuf);
	}
	else
	{
		BuildRecorde(0x80 | ID_REC_TOLL, revbuf);
	}

	//	WriteRecord(revbuf, 0);
	WriRecorJTB(revbuf, 1);

	if (cool == 4)//��Ʊ��ˢ����������ʾ�ŻݵĿ���
#ifdef AUDIO_SOUND_OLD
		audio_old(Audio_DONG);
#else
		audio(Audio_DONG);
#endif
	else {
		Sound_cityCMD(0);
	}
#ifdef _debug_CPU_
	debugstring("-------\r\n");
	debugstring("���ѳɹ�:\r\n");
	sprintf((char*)revbuf, "���:%d.%02dԪ\r\n", (a_sum1 / 100), (a_sum1 % 100));
	debugstring((char*)revbuf);
	sprintf((char*)revbuf, "�ۿ�:%d.%02dԪ\r\n", (s_sum1 / 100), (s_sum1 % 100));
	debugstring((char*)revbuf);
	debugstring("-------\r\n");
#endif


	cpuWriteFile03(mode);
	return ST_OK;//���ѳɹ�

}


//mode 0=Ƽ��   1=�Ž� 2=���������˺�����MAC���㡣
extern void getCPUMAC(unsigned char mode, unsigned char *inittext, unsigned char *Source, unsigned int le, unsigned char *destin);

unsigned char OperCPUBlackCard(void)
{
	unsigned char sndbuf[100];
	unsigned char revbuf[100];
	unsigned char psRandom[8];
	unsigned char cMac[4];
	unsigned char ret;

#ifdef _debug_CPU_
	debugstring("OperCPUBlackCard::::\r\n");
	debugstring("CPU��д������::\r\n");
#endif

	memcpy(sndbuf, "\x02\x00\x84\x00\x00\x04", 6);//��Ӧ�����к���Ϊ��ɢ����

	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf + 1, 5);
	if (ret == 0)
		return ST_ERROR;

#ifdef _debug_CPU_
	debugstring("get random:");
	debugdata(revbuf, ret, 1);
	debugdata((unsigned char*)&ret, 4, 1);
#endif
	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
		return ST_ERROR;
	memset(psRandom, 0, 8);
	memcpy(psRandom, revbuf + 1, 4);

	memcpy(sndbuf, "\x02\x04\xD6\x95\x0B\x05\x04", 7);//д��ĺڿ���־��04
	//	memcpy(sndbuf, "\x02\x04\xD6\x95\x19\x05\x50", 7);//�޸���Ч��
#ifdef _debug_CPU_
	debugstring("get MAC:");
	debugdata(psRandom, 8, 1);
	debugdata(sndbuf, 7, 1);
#endif
	getCPUMAC(2, psRandom, sndbuf + 1, 6, cMac);		//����

#ifdef _debug_CPU_
	debugstring("get MACOKK:");
	debugdata(cMac, 4, 1);
#endif
	memcpy(sndbuf + 7, cMac, 4);

	deal2or3(sndbuf);
#ifdef _debug_CPU_
	debugstring("black CPU snd:");
	debugdata(sndbuf, 11, 1);
#endif
	ret = RfPassthrough(revbuf, 2, sndbuf + 1, 10);
	if (ret == 0)
		return ST_ERROR;
#ifdef _debug_CPU_
	debugstring("write blk end:");
	debugdata(revbuf, ret, 1);
#endif
	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
		return ST_ERROR;
	else
		return ST_OK;
}


unsigned char CPU3F01(void)
{
	unsigned int ret;
	unsigned char sndbuf[100];
	unsigned char revbuf[100];

#ifdef _debug_CPU_
	debugstring("ѡ��Ӧ�� 3F01:::��ʼ\r\n");
#endif

	memcpy(sndbuf, "\x02\x00\xA4\x00\x00\x02\x3F\x01", 8);//�д˿���Ҫ���ļ���������ȡ����ʱ��Ҫ����һ�¡�
	deal2or3(sndbuf);
	ret = RfPassthrough(revbuf, 2, sndbuf, 8);
#ifdef _debug_CPU_
	debugdata(sndbuf, 8, 1);
	debugstring("RCV::");
	debugdata(revbuf, ret, 1);
#endif
	if (ret == 0)
		return ST_ERROR;

	if (memcmp(revbuf + ret - 2, "\x6A\x82", 2) == 0) {//����������ļ������Ͱ�AIDѡ�ļ�

		memcpy(sndbuf, "\x02\x00\xA4\x04\x00\x09\xA0\x00\x00\x00\x03\x86\x98\x07\x01", 15);
		deal2or3(sndbuf);
#ifdef _debug_CPU_
		debugstring("select AID1:");
		debugdata(sndbuf, 15, 1);
#endif
		ret = RfPassthrough(revbuf, 2, sndbuf, 15);
#ifdef _debug_CPU_
		debugdata(revbuf, ret, 1);
#endif
		if (ret == 0)
			return ST_ERROR;
	}

	if (memcmp(revbuf + ret - 2, "\x90\x00", 2) != 0)
		return ST_ERROR;

	return ST_OK;
}



void CPUMainCard(void)
{

	unsigned char ret;

	char disbuf[20];

	if (gBuInfo.stop_flag)
		return;

	gCardinfo.gMCardCand = CARDSTYLE_CPU;
#ifndef BUS_PINGXIANG_
	if ((cardSound == 0xaa) || (cardSound == 0xbb)) {//aaǮ������ bb��Ʊ����
		DisRetry();
	}
#endif 

	//BlightOpen();

#ifdef _debug_CPU_
	debugstring("ѯ���ɹ�:CSNO:");
	debugdata((unsigned char*)&gCardinfo.c_serial, 4, 1);
#endif

#if 0
	if (gBuInfo.gPsamErrorFlag & 2) {
		cls();
		if (gchn_eng == 'E') {
			display(4, 0, "PSAM2 error!", 0);
		}
		else {
			display(0, 0, "����:", 1);
			display(3, 0, "CPUPSAM����!", 0);
		}
		restore_flag = 3;
		delayxs(1);
		return;
	}
#endif

	ret = CPUcardType();
	if (ret == ST_ERROR) {
		ret = CPUcardType_JTB(0);
	}
	MSG_LOG("CPUcardType ret:%d\r\n", ret);
	if (gchn_eng == 'E') {
		sprintf(disbuf, "code:%02d ", ret);
	}
	else {
		sprintf(disbuf, "����:%02d ", ret);
	}

	switch (ret)
	{
	case APP_LOCK:
	case NO_JTB_UNTION:
#if 1
#ifdef qPBOC_BUS
		MSG_LOG("��������������\r\n");
		cpuPBOCmain();
#endif
#else
		cls();
		display(0, 0, "���棺Ӧ������!", 0);
		display(2, 4, "��Ͷ��!!", 0);
		SoundMessage(INVALID_CARD);
		restore_flag = 3;
#endif
		break;
		break;
	case NO_WriteNUM:
		MessageBox(0, "���Ǳ��ؿ�");
		end_close_card(1);
		break;
	case ST_OK:
		gBuInfo.g24GDisFlash = 3;
		gBuInfo.restore_flag = 0;
		SetTcpPacketTTLTime(RECORD_TTL);
		end_card();//����������á�
		return;
	case MONTH_CARD:
		ret = CPU_month_deal();//��˾��Ʊ
#ifdef _debug_CPU_
		debugstring("ret:");
		debugdata((unsigned char*)&ret, 1, 1);
#endif
		if ((ret == 3) || (ret == 4))//����
		{
			ret = CPU3F01();
			if (ret != ST_OK)
				break;

			ret = CPUDealCard(MONEY_CARD, ret);//��Ǯ, ����ʹ�ã���Ϊ���Ż���Ϣ���������Ʊ�Ļ����Ʊ��Ϣ���ǡ�
			break;
		}
		break;
	case MONEY_CARD:
		if (gCardinfo.gMCardCand == CARDSTYLE_CPU)
			ret = CPUDealCard(MONEY_CARD, 0);
		else if (gCardinfo.gMCardCand == CARDSTYLE_JTB)
			ret = CPUDealCard_JTB(MONEY_CARD, 0);
		break;
	case CARD_forbid:
		MessageBox(1, "�˿���ֹˢ��!");
		led_on(LED_RED);
		SoundMessage(INVALID_CARD);//����ר�߿� 2014.4.8
		gBuInfo.restore_flag = 3;
		goto CPUMEnd;
	case 9:
	case 8:
		cls();
		display(0, 0, disbuf, 0);
		if (gchn_eng == 'E') {
			display(3, 0, "illegal card", DIS_CENTER);
		}
		else
			display(3, 0, "�Ƿ���", DIS_CENTER);
		SoundMessage(INVALID_CARD);
		goto CPUMEnd;
	case CARD_NO_TIME:
		cls();
		display(0, 0, disbuf, 0);
		if (gchn_eng == 'E') {
			display(3, 0, "out date card", DIS_CENTER);
		}
		else {
			display(3, 0, "�����ڻ�û����", DIS_CENTER);
		}
		audio(Audio_FLING_MONEY);	//��Ͷ��

		goto CPUMEnd;
	case CARD_BLACK_PRO:
		if (gCardinfo.gMCardCand == CARDSTYLE_CPU)
			OperCPUBlackCard();
		else if (gCardinfo.gMCardCand == CARDSTYLE_JTB)
			operat_Black();
	case CARD_BLACK:
		cls();
		display(0, 0, "���棺��������!", 0);
		display(2, 4, "��Ͷ��!!", 0);
		SoundMessage(INVALID_CARD);
		gBuInfo.restore_flag = 3;
		goto CPUMEnd;
	case CARD_FREE:
		FreeConsume();
		goto CPUMEnd;
	case ST_ERROR:
		return;
	default:
		return;
	}
	if (gchn_eng == 'E') {
		sprintf(disbuf, "code:%02d ", ret);
	}
	else {
		sprintf(disbuf, "����:%02d ", ret);
	}
	switch (ret)
	{
	case ST_ERROR:
		return;
	case CARD_BLACK_PRO:
		cls();
		display(0, 0, "���棺��������!", 0);
		display(2, 4, "��Ͷ��!!", 0);
		SoundMessage(INVALID_CARD);
		gBuInfo.restore_flag = 3;
		end_card();//����������á�
		break;
	case 21:
	case RET_MONEY_SN_EROR:			//����Ǯ��������
		error(ERR_MONEY_SN, 0);
		end_card();//����������á�
		break;
	case 22:
		if (gchn_eng == 'E') {
			display(0, 0, "hint:", DIS_Cls);
			display(2, 0, "pos price error", DIS_CENTER);
		}
		else {
			display(0, 0, "��ʾ:", DIS_Cls);
			display(2, 0, "Ʊ����Ϣ����ȷ", DIS_CENTER);
		}
		end_card();//����������á�
		gBuInfo.restore_flag = 3;
		break;
	case CARD_NO_TIME:
		cls();
		display(0, 0, disbuf, 0);
		if (gchn_eng == 'E') {
			display(3, 0, "out date card", DIS_CENTER);
		}
		else {
			display(3, 0, "�����ڻ�û����", DIS_CENTER);
		}
		audio(Audio_FLING_MONEY);	//��Ͷ��

		end_card();//����������á�
		break;
	case 3:
		cls();
		display(0, 0, disbuf, 0);
		if (gchn_eng == 'E') {
			display(3, 0, "NO SAVE ACCOUNT", DIS_CENTER);
		}
		else {
			display(3, 0, "����", DIS_CENTER);
		}

		SoundMessage(SOUND_FLING_MONEY);
		end_card();//����������á�
		break;
	case 4:
		cls();
		display(0, 0, disbuf, 0);
		display(3, 0, "�۴β�����ˢ", DIS_CENTER);
		SoundMessage(SOUND_FLING_MONEY);
		end_card();//����������á�
		break;
	case 6:
		cls();
		display(0, 0, disbuf, 0);
		if (gchn_eng == 'E') {
			display(3, 0, "MAC1 eror", DIS_CENTER);
		}
		else {
			display(3, 0, "MAC1�������", DIS_CENTER);
		}
		//		CPUPsamInitialize();
		break;
	case 9:
	case 8:
		cls();
		display(0, 0, disbuf, 0);
		if (gchn_eng == 'E') {
			display(3, 0, "illegal card", DIS_CENTER);
		}
		else
			display(3, 0, "�Ƿ���", DIS_CENTER);
		SoundMessage(INVALID_CARD);
		end_card();//����������á�
		break;
	case ST_OK://�ɹ�
		gBuInfo.restore_flag = 3;
		SetTcpPacketTTLTime(RECORD_TTL);
		break;
	default:
		// 		cls();
		// 		display(0, 0, disbuf, 0);
		// 		display(3, 0, "ĩ�������", DIS_CENTER);
		break;
	}
CPUMEnd:
	end_close_card(1);
	PRINT_DEBUG("CPUMainCard over\n");
	//gBuInfo.g24GDisFlash = 3;
	//gBuInfo.restore_flag = 0;

	PRINT_DEBUG("CPUMainCard over111\n");
}
#endif // #ifdef BUS_CPU_
