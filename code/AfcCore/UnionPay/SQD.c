/**************************************************
* �ļ��� :SQD.c
* ����   :
* ����   :
**************************************************/
#include "Macro_Proj.h"
#include "string.h"
#include "stdio.h"
#include "include.h"
#include "szct.h"
#include "Gszct.h"
#include "citydifferent.h"
#include "SL8583.h"
#include	"tms.h"
#include "qPBOC.h"
#ifdef qPBOC_BUS
#include "qpboc_head.h"
#endif
#include "qpboc_8583.h"
#include "add.h"
#include "add_2.h"
#include "SQD.h"

#include "GprsSocket.h"

extern unsigned int c_serial;
extern stcomtime SysTime;
extern unsigned char gchn_eng;//��Ӣ�ı�־
extern TCP_IP_PACKET1 tcpipPacket;
extern time_t BCDTime2Long(unsigned char*timee);
extern unsigned char gmissflag;//�����־ 0x00 ��½�ƶ�,ǩ��.
extern unsigned char GPRSLinkProcess;
extern unsigned char restore_flag;
extern unsigned char *gpboc_8583bitmapaddr;//8583λͼ����ַָ��
extern void tcpipClose(unsigned char mode);
extern int build8583_qpboc_Purse_0200(unsigned char *oDat);
extern int Build_qpboc_8583_HD(unsigned char *Odat, int headType);
extern void setBitmapBits(unsigned char fieldID, unsigned char *sl8583bitmap);
extern unsigned char qPbocBuildRec_hui(unsigned char *qrecbuff, unsigned char transResult);
extern unsigned char qPbocWriteRecord(unsigned char *rec);
extern int Build_qpboc_8583_64(unsigned char *iDat, unsigned int iDatLen, unsigned char *oDat);
extern int build8583_qpboc_Auth(unsigned char *oDat, unsigned inout);
extern void gprs_send_data(unsigned char linkNum, unsigned int len, void *dat);
//��������
int build8583_qpboc_re_Purse_0400_sq(unsigned char *oDat, unsigned char  mode)
{
#define BUILD8583_FORM_RECORD(pTlv, tlv8583, fieldId, ilen, iret) \
	pTlv = tlv8583 + fieldId - 1;\
	if (pTlv->tag == fieldId){\
		iret = pTlv->len; \
		MSG_LOG("build%d--%d:", fieldId, iret); \
		BCD_LOG(pTlv->pValue, iret, 1); \
		memcpy(oDat + ilen, pTlv->pValue, iret); \
		ilen += iret; \
		setBitmapBits(fieldId, gpboc_8583bitmapaddr);\
	}

	DEF8583MSG_BIP *msg6001A;
	//	int curp, ltemp, headp;
	int iret, ilen;
	unsigned char infor_temp[1000];
	int inforLen = 1000;
	stTlv tlv8583[65] = { 0 };
	stTlv *pTlv = NULL;
	//	int tlvCount = 64;
	unsigned cardStyle = 0;
	int fieldId = 0;
	int i = 0;

	MSG_LOG("build8583_qpboc_re_Purse_0400:\n");
	if (mode == MISS_PBOC_RE_PURSE) {
#ifdef switch_RE
		//	iret = read_re_infor(infor_temp, &inforLen);
		iret = save_infor_add(ODA_FeRC_READ, infor_temp);
		if (iret != Ret_OK)
		{
			fieldId = fieldId;
			MSG_LOG("read_re_infor(������¼ͻȻ��ɾ����):%d\n", iret);
			return 0;
		}
		inforLen = GET_INT16(infor_temp + 2);
		MSG_LOG("infor_temp2:");
		BCD_LOG(infor_temp + 4, inforLen, 1);

		ilen = 0;
		msg6001A = (DEF8583MSG_BIP*)oDat;


		iret = PareseT1L2V_8583(infor_temp + 4, inforLen, tlv8583);
#else
		iret = read_re_infor(infor_temp, &inforLen);
		if (iret != Ret_OK)
		{
			fieldId = fieldId;
			MSG_LOG("read_re_infor(������¼ͻȻ��ɾ����):%d\n", iret);
			return 0;
		}
		MSG_LOG("infor_temp2:");
		BCD_LOG(infor_temp, inforLen, 1);

		ilen = 0;
		msg6001A = (DEF8583MSG_BIP*)oDat;


		iret = PareseT1L2V_8583(infor_temp, inforLen, tlv8583);
#endif

		if (iret != Ret_OK) {
			MSG_LOG("������¼����:%d\n", iret);
			return 0;
		}
		if (tlv8583[64].tag == 0 || tlv8583[64].len != 1) {
			MSG_LOG("������¼�޿���������:%d,%d\n", tlv8583[64].tag, tlv8583[64].len);
			return 0;
		}
	}
	else {
		iret = save_ODA_infor(ODA_FeRC_READ, infor_temp);
		if (iret != Ret_OK)
		{
			fieldId = fieldId;
			MSG_LOG("read_re_infor(ODA ������¼ͻȻ��ɾ����):%d\n", iret);
			return 0;
		}
		//	memcpy(infor_temp,repurse_infor,512);
		inforLen = GET_INT16(infor_temp + 2);
		MSG_LOG("infor_temp1:");
		BCD_LOG(infor_temp, inforLen + 4, 1);

		ilen = 0;
		msg6001A = (DEF8583MSG_BIP*)oDat;

		iret = PareseT1L2V_8583(infor_temp + 4, inforLen, tlv8583);
		if (iret != Ret_OK) {
			MSG_LOG("������¼����:%d\n", iret);
			return 0;
		}
		if (tlv8583[64].tag == 0 || tlv8583[64].len != 1) {
			MSG_LOG("������¼�޿���������:%d,%d\n", tlv8583[64].tag, tlv8583[64].len);
			return 0;
		}
	}
	cardStyle = tlv8583[64].pValue[0];
	if (cardStyle == CARDSTYLE_UNPAY_ODA) {
		memcpy(oDat + ilen, "\x02\x00", 2); ilen += 2;		//��Ϣ����
		gpboc_8583bitmapaddr = msg6001A->bitMap;
		memset(gpboc_8583bitmapaddr, 0, 8);
		ilen += 8;	//������λԪ������		
		for (i = 0; i < 64; i++)
		{
			if (tlv8583[i].tag != 0) {
				BUILD8583_FORM_RECORD(pTlv, tlv8583, tlv8583[i].tag, ilen, iret);
			}
#if 0
			if (tlv8583[i].tag == 11) {
				iret = Build_qpboc_8583_11(oDat + ilen);
				//SetTLV(tlv8583 + tlvCount, 11, iret, oDat + ilen);
				//++tlvCount;
				//BCD2Ascii(oDat + ilen, originData + 8, 3);
				ilen += iret;
				save_deal_count_add(BIT_PBOC_NS_8583);
			}
			else if (tlv8583[i].tag != 0) {
				BUILD8583_FORM_RECORD(pTlv, tlv8583, tlv8583[i].tag, ilen, iret);
			}
#endif
		}
		goto build8583_qpboc_re_Purse_0400_OVER;
	}
	else {
		memcpy(oDat + ilen, "\x04\x00", 2); ilen += 2;		//��Ϣ����
	}
	gpboc_8583bitmapaddr = msg6001A->bitMap;
	memset(gpboc_8583bitmapaddr, 0, 8);
	ilen += 8;	//������λԪ������	

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 3, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 4, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 11, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 22, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 25, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 35, ilen, iret);

	iret = 2;
	MSG_LOG("build39--:3938\r\n");

	memcpy(oDat + ilen, "98", 2); ilen += iret;	//POS�ն���ʱ����δ���յ�POS���ĵ�Ӧ����Ϣ������������ԭ�����98����
	setBitmapBits(39, gpboc_8583bitmapaddr);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 41, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 42, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 48, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 49, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 55, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 57, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 60, ilen, iret);

build8583_qpboc_re_Purse_0400_OVER:
	iret = Build_qpboc_8583_64(oDat, ilen, oDat + ilen);
	ilen += iret;

	MSG_LOG("Build_qpboc_8583MAP:len:8 dat:");
	BCD_LOG(gpboc_8583bitmapaddr, 8, 1);

	return ilen;

#undef BUILD8583_FORM_RECORD
}
//�������ݵ�ģ�� ��������1�ֽڡ�1=ˢ����¼��2=��ǰ��¼�ļ���3=��ʷ��¼��+�ļ�����(4�ֽڣ�+ƫ������4�ֽڣ�+���ݳ��ȣ�4�ֽڣ�+���ݣ�L)
extern  int buildDataSend_0B(unsigned char mode, unsigned int offset, unsigned int Alen, unsigned int datLen, unsigned char *data, unsigned char *obuf, unsigned char *tmb);

unsigned write_linux_re(unsigned char mode)
{
	unsigned char revBuf[600], charbuf[16];


	unsigned int blen = 0, len = 0;
	memset(revBuf, 0, sizeof(revBuf));
	memset(charbuf, 0, sizeof(charbuf));
	revBuf[len++] = 0;	//ǰ����ǰ�����ȡ�
	revBuf[len++] = 0;	//ǰ����ǰ�����ȡ�

	blen = Build_qpboc_8583_HD(revBuf + len, mode != MISS_PBOC_LOGIN);
	len += blen;

	blen = build8583_qpboc_re_Purse_0400_sq(revBuf + len, mode);
	if (len > 0)
	{
		len += blen;
		revBuf[0] = (len - 2) / 256;
		revBuf[1] = (len - 2) % 256;//��ǰ������ܳ���
#ifdef YYLinux4G
		BCD2Ascii((unsigned char*)&SysTime, charbuf, 7);
		buildDataSend_0B(1, 0, 512, 512, revBuf, charbuf, charbuf);//�Ѽ�¼���͸�Linuxģ��
#endif	
	}
	return ST_OK;
}

extern unsigned char WriRecorQRC(unsigned char *rec);
unsigned char write_linux_re_build(unsigned char transResult)
{
	unsigned char revBuf[300], charbuf[20];
	int qrinfor_len = 0;
	int tmpI = 0;
	memset(revBuf, 0, sizeof(revBuf));
	memset(charbuf, 0, sizeof(charbuf));

	if (emv_get_card_type() == QR_CARD) {
		BuildUnionpayQrRecord(revBuf, transResult);
	}
	else {
		memcpy(revBuf, "��������", 8);
		qPbocBuildRec_hui(revBuf + 8, transResult);
		MSG_LOG("�����Ҽ�¼\r\n");
		BCD_LOG(revBuf, 256, 1);
		BCD2Ascii((unsigned char*)&SysTime, charbuf, 7);
	}
	//buildDataSend_0B(1, 0, 256, 256, revBuf, charbuf, charbuf);//�Ѽ�¼���͸�Linuxģ��

	WriRecorQRC(revBuf);

	return ST_OK;
}
//����Ӧ������

oda_ack ODAACK;
void ODA_ACK_INIT(void)
{
	memset((unsigned char *)&ODAACK, 0, sizeof(oda_ack));

}

void ODA_ack(unsigned char *data, unsigned int len)
{
	unsigned char rec_buff[64];
	unsigned char charbuf[16];
	if (len > 64)
	{
		MSG_LOG("���ȳ�64\r\n");
		return;
	}
	memset(rec_buff, 0, sizeof(rec_buff));
	memcpy(rec_buff, data, len);
	BCD2Ascii((unsigned char*)&SysTime, charbuf, 7);
	buildDataSend_0B(1, 0, len, len, rec_buff, charbuf, charbuf);//�Ѽ�¼���͸�Linuxģ��

}
unsigned int Build_qpboc_8583Packge_SQ(unsigned char *revBuf, unsigned char mode)
{
	unsigned int blen = 0, len = 0;

	revBuf[len++] = 0;	//ǰ����ǰ�����ȡ�
	revBuf[len++] = 0;	//ǰ����ǰ�����ȡ�

	blen = Build_qpboc_8583_HD(revBuf + len, mode != MISS_PBOC_LOGIN);
	len += blen;

	if (s_isAuthOk == 0)
	{
		len = 2;
		blen = build8583_qpboc_Auth(revBuf + len, 0);
		if (blen == 0)
			return 0;
		len += blen;
		revBuf[0] = (len - 2) / 256;
		revBuf[1] = (len - 2) % 256;//��ǰ������ܳ���

		return len;
	}
	switch (mode)
	{
	case MISS_PBOC_PURSE:	//1.4.1�����Ѽ�¼�ϴ�-7001
		//blen = build8583UPREC7001(revBuf+len);
		blen = build8583_qpboc_Purse_0200(revBuf + len);
		//		MSG_LOG("blen:%d\r\n",blen);
		if (blen == 0)
			return 0;
		len += blen;
		revBuf[0] = (len - 2) / 256;
		revBuf[1] = (len - 2) % 256;//��ǰ������ܳ���
		break;
	default:
		len = 0;
		break;
	}
	return len;
}

unsigned char  send_sq(unsigned char gmissflag_sq)
{

	unsigned char buffer[1025];//[2100];
	unsigned int uilen;
#if HTTP_HEAD
	char http_head[256];
	unsigned int http_len = 0;
#endif

	memset(msgf, 0, sizeof(msgf));
	MSG_LOG("���msgf-sq-:%02X\r\n", gGprsinfo.gmissflag);
	memset(buffer, 0, sizeof(buffer));//��MACʱ��buff���治��8�ֽ�Ҫ��\x00 �������
	uilen = Build_qpboc_8583Packge_SQ(buffer, gmissflag_sq);
	if (uilen == 0) {
		MSG_LOG("���뱨��δ��\r\n");
		return ST_ERROR;
	}
	MSG_LOG("8583 ���뱨��data: len:%d\r\n", uilen);
	debugdata(buffer, uilen, 1);

#if HTTP_HEAD
	memset(http_head, 0, sizeof(http_head));
	http_len = Build_http_pack(http_head, "", 0, uilen);
	MSG_LOG("httphead len:%d\r\n", http_len);
	//MSG_LOG("httphead len:%d\r\n%s\r\n", http_len, http_head);
	memmove(buffer + http_len, buffer, uilen);
	memcpy(buffer, http_head, http_len);
	uilen += http_len;
#endif

	debugstring("����-TaskRecWrite_snd data:");
	debugdata(buffer, uilen, 1);


	if (gmissflag_sq == MISS_PBOC_PURSE && s_isAuthOk != 0)
	{
		save_repurse_infor(FeRC_Write, (unsigned char *)&repurse_infor);
	}

	show_dns_ip();
	gprs_send_data(LINK_PBOC, uilen, buffer);

	return ST_OK;
}
//����ȴ��ӿ�
unsigned char flag_aut = 0xff;
void set_flag_aut(unsigned char data)
{
	flag_aut = data;
}
unsigned get_flag_aut(void)
{
	return flag_aut;
}


int Send_WaitRecvData(unsigned char SQmode, int msecends)
{
	//	unsigned short us1;
	unsigned char flag = 0xFF;
	unsigned char 	ret = 0;
	unsigned int outdly = 0;
	unsigned int i;
	int retcode = -2;


	//	unsigned int i=0;
		//unsigned long long read_dat=0;
		//unsigned int count=0;
	unsigned char disbuff[50];
	int tmpI = 0;
	//	BER_TVL TempTVL;

#if 0
	msecends = Q_QPBOC_para.shuangmian_OVER_TIME;//get_over_time_shuang();
	msecends = msecends * 1000;
	msecends += 250;
#else
	msecends = 5000;
#endif
	msecends = TIMEOUT_PBOC_ONLINE;

	if (msecends > 0) {
		flag = 0;
		if (gCardinfo.gMCardCand != CARDSTYLE_UNPAY_ODA)
		{
			cls();
			tmpI = 0;
			tmpI += sprintf(disbuff + tmpI, "ͨѶ��...."STR_NEW_LINE"���Ե�");
			display(8, 0, disbuff, DIS_ClsLine | DIS_CENTER);
		}
		MSG_LOG("do %s:0x%02X\r\n", __FUNCTION__, SQmode);
		MSG_LOG("set_timer0(10000,2)--:%d\r\n", msecends);
		set_timer0(msecends, 3);
	}
	memset(irda_rxBuf, 0, 22);
	memset(pakege_8583, 0, sizeof(pakege_8583));

	if (Sign_Infor.ISOK == 0)
	{
		MSG_LOG("ûǩ��--return--\r\n");
		gGprsinfo.gmissflag = MISS_PBOC_LOGIN;
		ACK_flag = 0;
		MessageBox(1, "�豸δǩ��,���Ժ�");
		delayxms(500);
		set_timer0(0, 2);
		return -2;
	}


	MSG_LOG("��������Ƿ����\r\n");
	ACK_flag = 0xFF;

	while (1) {//��ʼ����
		clr_wdt();
#ifdef WIFI_TLH_
		wifiTlh_main();
#else
		//main_GPRS(NULL);
#endif

		outdly = get_timer0(3);
		if (outdly == 0)
		{
			goto Send_WaitRecvData_OVER;
		}
		ret = getkey(1);
		if (ret == SLZRKEY_ESC) {
			goto Send_WaitRecvData_OVER;
		}

		if (gGprsinfo.GPRSLinkProcess < 21) {//�Ѿ�����
			if (flag != 1) {
				flag = 1;
				display(6, 0, "���ڰκ�", DIS_ClsLine | DIS_CENTER);
			}
			//set_timer0(5000,2);	

			continue;
		}
		else if (gGprsinfo.GPRSLinkProcess == 0x2F) {
			if (flag != 1) {
				flag = 1;
				display(6, 0, "������������", DIS_ClsLine | DIS_CENTER);
			}
			//set_timer0(5000,2);	

			continue;
		}


		if (flag != 3) {
			flag = 3;

			display(6, 0, "���ڷ�����������", DIS_ClsLine | DIS_CENTER);
		}

		if (getIsNetOk(LINK_PBOC) != BOOL_FALSE)
		{
			if (gGprsinfo.gmissflag != MISS_PBOC_UPREC_ODA /*&&gmissflag!=MISS_PBOC_RE_PURSE*/) //�����������ʱ���Ȳ�����������ֹͬʱ�յ����Ӧ��	
			{
				gGprsinfo.GPRSLinkProcess = TCPSTARTSTAT;
				MSG_LOG("��·������\r\n");
				break;//
			}
		}
		else {
			//		MSG_LOG("���ӶϿ�\r\n");	;
			usleep(500000);
		}
	}//while


	for (i = 0; i < 2; i++)
	{
#ifdef _debug_
		MSG_LOG("��ʼ��������\r\n");
#endif
		set_flag_aut(0xff);
		ACK_flag = 0xFF;
		// 		//���ж���û���������
		if (get_repurse_num() == ST_OK)
		{
#ifdef _debug_
			//beep(3,400,400);
#endif
			MSG_LOG("Ҫ����-22222-\r\n");
			//gmissflag = MISS_PBOC_RE_PURSE;
			ACK_flag = 0xFE;
			return -2;
		}
#ifdef switch_RE
		//��������
		if (over_GPRStR_add2() != ST_OK)
		{
			MSG_LOG("Ҫ����-3333-\r\n");
			//gmissflag = MISS_PBOC_RE_PURSE;
			ACK_flag = 0xFE;
			return -2;
		}
#endif

		if (gGprsinfo.gmissflag != SQmode) {
			//��ʱ��ȥ����������
			//	MSG_LOG("��ʱ��ȥ����������\r\n");
			gGprsinfo.gmissflag = SQmode;
			MSG_LOG("ǿ���л�������:%02X\r\n", SQmode);
		}
		if (send_sq(MISS_PBOC_PURSE) == ST_ERROR)
		{
			retcode = -1;
			goto Send_WaitRecvData_OVER;
		}
		if (s_isAuthOk != 0)
		{
			//	MSG_LOG("����Ҫ��֤\r\n");
			i = 3;
		}
		while (1) {

			outdly = get_timer0(3);
			if (gCardinfo.gMCardCand != CARDSTYLE_UNPAY_ODA && outdly < TIMEOUT_PBOC_ONLINE - 1000) {
				memset(disbuff, 0, sizeof(disbuff));
				sprintf((char *)disbuff, "ʣ��%d��", outdly / 1000);
				display(8, 16, (const char *)disbuff, 0);
			}

			if (gGprsinfo.gmissflag != SQmode) {
				//��ʱ��ȥ����������
				//	MSG_LOG("��ʱ��ȥ����������\r\n");
				gGprsinfo.gmissflag = SQmode;
			}

			usleep(500000);

			clr_wdt();
			//main_GPRS(NULL);
			//#if SWITCH_PBOC

			if (get_flag_aut() == 0xad)
			{
				MSG_LOG("��֤����\r\n");
				set_flag_aut(0xff);
				break;
			}
			//#endif
			if (outdly == 0)
			{
				emv_set_pboc_result(pr_repurse);
				MSG_LOG("time out-3-\r\n");
				tcpipClose(LINK_PBOC);
				if (switch_both)
				{
					// ����ODA����
					shuangmian = 0;

				}
#if defined QK && !defined switch_RE 
				set_pos_infor_1(0xad);  //��ʱ��־����˫ͨ��
#else

#endif
#ifdef switch_RE
				Switch_sign(Switch_sign_OVER);
#endif
				write_linux_re_build(pr_repurse);  //��ʱ����һ������

				retcode = -1;
				goto Send_WaitRecvData_OVER;
			}

			if (ACK_flag != 0xFF)
			{
				// ���ܽ���
				MSG_LOG("444bit:%d,ACK_flag:%02x   ", msgf[field_ack].bitf, ACK_flag);
				if ((ACK_flag == 0x00) && (msgf[field_ack].bitf)) {
					emv_set_pboc_result(pr_success);
					flag = 0xA5;
					MSG_LOG("444bit:%d,ACK_flag:%02x   ", msgf[field_ack].bitf, ACK_flag);
					break;
				}
				else {
					emv_set_pboc_result(pr_fail);
					MSG_LOG("%s,Ӧ��:%02X\r\n", __FUNCTION__, ACK_flag);
					MSG_LOG("444bit:%d,ACK_flag:%02x   ", msgf[field_ack].bitf, ACK_flag);
#ifdef switch_RE
					Switch_sign(Switch_sign_OVER);
#endif
					write_linux_re_build(pr_fail); //����ʧ�ܵļ�¼

					MSG_LOG("ɾ����111--\r\n");
					memset(repurse_infor, 0, sizeof(repurse_infor));
#ifdef switch_RE
					init_timeout_infor(); //�������ʱ����
										  //	save_infor_add(FeRC_Dlelt, NULL);
					save_repurse_infor(FeRC_Dlelt, NULL);
#else
					save_repurse_infor(FeRC_Dlelt, NULL);
#endif

					retcode = -1;
					goto Send_WaitRecvData_OVER;
				}

			}
			else {
				ret = getkey(1);
				if (ret == SLZRKEY_ESC) {
					emv_set_pboc_result(pr_repurse);
#ifdef switch_RE
					Switch_sign(Switch_sign_OVER);
#endif
					write_linux_re_build(pr_repurse); //��ʱ����һ������

					retcode = -1;
					goto Send_WaitRecvData_OVER;
				}
			}
		}//	while (1) 
	}  //for
	//����ж�
	if (flag == 0xA5 && gCardinfo.gMCardCand != CARDSTYLE_UNPAY_ODA) {	// �յ���ȷ���� 
		//			gSendOverTime = 0;

		MSG_LOG("ɾ����22222--\r\n");
		memset(repurse_infor, 0, sizeof(repurse_infor));
#ifdef switch_RE
		init_timeout_infor(); //�������ʱ����
	//	save_infor_add(FeRC_Dlelt, NULL);
		save_repurse_infor(FeRC_Dlelt, NULL);
#else
		save_repurse_infor(FeRC_Dlelt, NULL);
#endif
		pboc_free_cnt = PBOC_FREE_CNT;
		MSG_LOG("��������:%02x\r\n", flag);


		retcode = 0;
		goto Send_WaitRecvData_OVER;
	}

Send_WaitRecvData_OVER:

	if (gGprsinfo.GPRSLinkProcess == GPRS_SENDING_CMD)
		gGprsinfo.GPRSLinkProcess = TCPSTARTSTAT;

	gGprsinfo.gmissflag = MISS_G_FREE;

	restore_flag = 3;

	PRINT_DEBUG("Send_WaitRecvData over:%d\n", retcode);

	return retcode;

}
