/**************************************************
* 文件名 :SQD.c
* 描述   :
* 建立   :
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
extern unsigned char gchn_eng;//中英文标志
extern TCP_IP_PACKET1 tcpipPacket;
extern time_t BCDTime2Long(unsigned char*timee);
extern unsigned char gmissflag;//任务标志 0x00 登陆移动,签到.
extern unsigned char GPRSLinkProcess;
extern unsigned char restore_flag;
extern unsigned char *gpboc_8583bitmapaddr;//8583位图到地址指针
extern void tcpipClose(unsigned char mode);
extern int build8583_qpboc_Purse_0200(unsigned char *oDat);
extern int Build_qpboc_8583_HD(unsigned char *Odat, int headType);
extern void setBitmapBits(unsigned char fieldID, unsigned char *sl8583bitmap);
extern unsigned char qPbocBuildRec_hui(unsigned char *qrecbuff, unsigned char transResult);
extern unsigned char qPbocWriteRecord(unsigned char *rec);
extern int Build_qpboc_8583_64(unsigned char *iDat, unsigned int iDatLen, unsigned char *oDat);
extern int build8583_qpboc_Auth(unsigned char *oDat, unsigned inout);
extern void gprs_send_data(unsigned char linkNum, unsigned int len, void *dat);
//冲正报文
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
			MSG_LOG("read_re_infor(冲正记录突然被删除了):%d\n", iret);
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
			MSG_LOG("read_re_infor(冲正记录突然被删除了):%d\n", iret);
			return 0;
		}
		MSG_LOG("infor_temp2:");
		BCD_LOG(infor_temp, inforLen, 1);

		ilen = 0;
		msg6001A = (DEF8583MSG_BIP*)oDat;


		iret = PareseT1L2V_8583(infor_temp, inforLen, tlv8583);
#endif

		if (iret != Ret_OK) {
			MSG_LOG("冲正记录错误:%d\n", iret);
			return 0;
		}
		if (tlv8583[64].tag == 0 || tlv8583[64].len != 1) {
			MSG_LOG("冲正记录无卡交易类型:%d,%d\n", tlv8583[64].tag, tlv8583[64].len);
			return 0;
		}
	}
	else {
		iret = save_ODA_infor(ODA_FeRC_READ, infor_temp);
		if (iret != Ret_OK)
		{
			fieldId = fieldId;
			MSG_LOG("read_re_infor(ODA 冲正记录突然被删除了):%d\n", iret);
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
			MSG_LOG("冲正记录错误:%d\n", iret);
			return 0;
		}
		if (tlv8583[64].tag == 0 || tlv8583[64].len != 1) {
			MSG_LOG("冲正记录无卡交易类型:%d,%d\n", tlv8583[64].tag, tlv8583[64].len);
			return 0;
		}
	}
	cardStyle = tlv8583[64].pValue[0];
	if (cardStyle == CARDSTYLE_UNPAY_ODA) {
		memcpy(oDat + ilen, "\x02\x00", 2); ilen += 2;		//消息类型
		gpboc_8583bitmapaddr = msg6001A->bitMap;
		memset(gpboc_8583bitmapaddr, 0, 8);
		ilen += 8;	//这里是位元表，跳过		
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
		memcpy(oDat + ilen, "\x04\x00", 2); ilen += 2;		//消息类型
	}
	gpboc_8583bitmapaddr = msg6001A->bitMap;
	memset(gpboc_8583bitmapaddr, 0, 8);
	ilen += 8;	//这里是位元表，跳过	

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 3, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 4, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 11, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 22, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 25, ilen, iret);

	BUILD8583_FORM_RECORD(pTlv, tlv8583, 35, ilen, iret);

	iret = 2;
	MSG_LOG("build39--:3938\r\n");

	memcpy(oDat + ilen, "98", 2); ilen += iret;	//POS终端在时限内未能收到POS中心的应答消息而引发，冲正原因码填“98”。
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
//发送数据到模块 数据类型1字节【1=刷卡记录、2=当前记录文件、3=历史记录】+文件长度(4字节）+偏移量（4字节）+数据长度（4字节）+数据（L)
extern  int buildDataSend_0B(unsigned char mode, unsigned int offset, unsigned int Alen, unsigned int datLen, unsigned char *data, unsigned char *obuf, unsigned char *tmb);

unsigned write_linux_re(unsigned char mode)
{
	unsigned char revBuf[600], charbuf[16];


	unsigned int blen = 0, len = 0;
	memset(revBuf, 0, sizeof(revBuf));
	memset(charbuf, 0, sizeof(charbuf));
	revBuf[len++] = 0;	//前面是前导长度。
	revBuf[len++] = 0;	//前面是前导长度。

	blen = Build_qpboc_8583_HD(revBuf + len, mode != MISS_PBOC_LOGIN);
	len += blen;

	blen = build8583_qpboc_re_Purse_0400_sq(revBuf + len, mode);
	if (len > 0)
	{
		len += blen;
		revBuf[0] = (len - 2) / 256;
		revBuf[1] = (len - 2) % 256;//最前面添加总长度
#ifdef YYLinux4G
		BCD2Ascii((unsigned char*)&SysTime, charbuf, 7);
		buildDataSend_0B(1, 0, 512, 512, revBuf, charbuf, charbuf);//把记录发送给Linux模块
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
		memcpy(revBuf, "银行数据", 8);
		qPbocBuildRec_hui(revBuf + 8, transResult);
		MSG_LOG("冲正灰记录\r\n");
		BCD_LOG(revBuf, 256, 1);
		BCD2Ascii((unsigned char*)&SysTime, charbuf, 7);
	}
	//buildDataSend_0B(1, 0, 256, 256, revBuf, charbuf, charbuf);//把记录发送给Linux模块

	WriRecorQRC(revBuf);

	return ST_OK;
}
//保存应答数据

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
		MSG_LOG("长度超64\r\n");
		return;
	}
	memset(rec_buff, 0, sizeof(rec_buff));
	memcpy(rec_buff, data, len);
	BCD2Ascii((unsigned char*)&SysTime, charbuf, 7);
	buildDataSend_0B(1, 0, len, len, rec_buff, charbuf, charbuf);//把记录发送给Linux模块

}
unsigned int Build_qpboc_8583Packge_SQ(unsigned char *revBuf, unsigned char mode)
{
	unsigned int blen = 0, len = 0;

	revBuf[len++] = 0;	//前面是前导长度。
	revBuf[len++] = 0;	//前面是前导长度。

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
		revBuf[1] = (len - 2) % 256;//最前面添加总长度

		return len;
	}
	switch (mode)
	{
	case MISS_PBOC_PURSE:	//1.4.1　消费记录上传-7001
		//blen = build8583UPREC7001(revBuf+len);
		blen = build8583_qpboc_Purse_0200(revBuf + len);
		//		MSG_LOG("blen:%d\r\n",blen);
		if (blen == 0)
			return 0;
		len += blen;
		revBuf[0] = (len - 2) / 256;
		revBuf[1] = (len - 2) % 256;//最前面添加总长度
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
	MSG_LOG("清空msgf-sq-:%02X\r\n", gGprsinfo.gmissflag);
	memset(buffer, 0, sizeof(buffer));//算MAC时候buff里面不够8字节要填\x00 ，先清空
	uilen = Build_qpboc_8583Packge_SQ(buffer, gmissflag_sq);
	if (uilen == 0) {
		MSG_LOG("申请报文未空\r\n");
		return ST_ERROR;
	}
	MSG_LOG("8583 申请报文data: len:%d\r\n", uilen);
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

	debugstring("申请-TaskRecWrite_snd data:");
	debugdata(buffer, uilen, 1);


	if (gmissflag_sq == MISS_PBOC_PURSE && s_isAuthOk != 0)
	{
		save_repurse_infor(FeRC_Write, (unsigned char *)&repurse_infor);
	}

	show_dns_ip();
	gprs_send_data(LINK_PBOC, uilen, buffer);

	return ST_OK;
}
//申请等待接口
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
			tmpI += sprintf(disbuff + tmpI, "通讯中...."STR_NEW_LINE"请稍等");
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
		MSG_LOG("没签到--return--\r\n");
		gGprsinfo.gmissflag = MISS_PBOC_LOGIN;
		ACK_flag = 0;
		MessageBox(1, "设备未签到,请稍后");
		delayxms(500);
		set_timer0(0, 2);
		return -2;
	}


	MSG_LOG("检测网络是否可用\r\n");
	ACK_flag = 0xFF;

	while (1) {//开始申请
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

		if (gGprsinfo.GPRSLinkProcess < 21) {//已经连接
			if (flag != 1) {
				flag = 1;
				display(6, 0, "正在拔号", DIS_ClsLine | DIS_CENTER);
			}
			//set_timer0(5000,2);	

			continue;
		}
		else if (gGprsinfo.GPRSLinkProcess == 0x2F) {
			if (flag != 1) {
				flag = 1;
				display(6, 0, "正在重新连接", DIS_ClsLine | DIS_CENTER);
			}
			//set_timer0(5000,2);	

			continue;
		}


		if (flag != 3) {
			flag = 3;

			display(6, 0, "正在发送消费申请", DIS_ClsLine | DIS_CENTER);
		}

		if (getIsNetOk(LINK_PBOC) != BOOL_FALSE)
		{
			if (gGprsinfo.gmissflag != MISS_PBOC_UPREC_ODA /*&&gmissflag!=MISS_PBOC_RE_PURSE*/) //有其他任务的时候，先不发联机，防止同时收到多个应答	
			{
				gGprsinfo.GPRSLinkProcess = TCPSTARTSTAT;
				MSG_LOG("链路链接上\r\n");
				break;//
			}
		}
		else {
			//		MSG_LOG("链接断开\r\n");	;
			usleep(500000);
		}
	}//while


	for (i = 0; i < 2; i++)
	{
#ifdef _debug_
		MSG_LOG("开始发送申请\r\n");
#endif
		set_flag_aut(0xff);
		ACK_flag = 0xFF;
		// 		//再判断有没有铁电冲正
		if (get_repurse_num() == ST_OK)
		{
#ifdef _debug_
			//beep(3,400,400);
#endif
			MSG_LOG("要冲正-22222-\r\n");
			//gmissflag = MISS_PBOC_RE_PURSE;
			ACK_flag = 0xFE;
			return -2;
		}
#ifdef switch_RE
		//发送申请
		if (over_GPRStR_add2() != ST_OK)
		{
			MSG_LOG("要冲正-3333-\r\n");
			//gmissflag = MISS_PBOC_RE_PURSE;
			ACK_flag = 0xFE;
			return -2;
		}
#endif

		if (gGprsinfo.gmissflag != SQmode) {
			//暂时不去找其他任务
			//	MSG_LOG("暂时不去找其他任务\r\n");
			gGprsinfo.gmissflag = SQmode;
			MSG_LOG("强制切换任务至:%02X\r\n", SQmode);
		}
		if (send_sq(MISS_PBOC_PURSE) == ST_ERROR)
		{
			retcode = -1;
			goto Send_WaitRecvData_OVER;
		}
		if (s_isAuthOk != 0)
		{
			//	MSG_LOG("不需要认证\r\n");
			i = 3;
		}
		while (1) {

			outdly = get_timer0(3);
			if (gCardinfo.gMCardCand != CARDSTYLE_UNPAY_ODA && outdly < TIMEOUT_PBOC_ONLINE - 1000) {
				memset(disbuff, 0, sizeof(disbuff));
				sprintf((char *)disbuff, "剩余%d秒", outdly / 1000);
				display(8, 16, (const char *)disbuff, 0);
			}

			if (gGprsinfo.gmissflag != SQmode) {
				//暂时不去找其他任务
				//	MSG_LOG("暂时不去找其他任务\r\n");
				gGprsinfo.gmissflag = SQmode;
			}

			usleep(500000);

			clr_wdt();
			//main_GPRS(NULL);
			//#if SWITCH_PBOC

			if (get_flag_aut() == 0xad)
			{
				MSG_LOG("认证返回\r\n");
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
					// 进入ODA交易
					shuangmian = 0;

				}
#if defined QK && !defined switch_RE 
				set_pos_infor_1(0xad);  //超时标志，限双通道
#else

#endif
#ifdef switch_RE
				Switch_sign(Switch_sign_OVER);
#endif
				write_linux_re_build(pr_repurse);  //超时保存一条冲正

				retcode = -1;
				goto Send_WaitRecvData_OVER;
			}

			if (ACK_flag != 0xFF)
			{
				// 接受交易
				MSG_LOG("444bit:%d,ACK_flag:%02x   ", msgf[field_ack].bitf, ACK_flag);
				if ((ACK_flag == 0x00) && (msgf[field_ack].bitf)) {
					emv_set_pboc_result(pr_success);
					flag = 0xA5;
					MSG_LOG("444bit:%d,ACK_flag:%02x   ", msgf[field_ack].bitf, ACK_flag);
					break;
				}
				else {
					emv_set_pboc_result(pr_fail);
					MSG_LOG("%s,应答:%02X\r\n", __FUNCTION__, ACK_flag);
					MSG_LOG("444bit:%d,ACK_flag:%02x   ", msgf[field_ack].bitf, ACK_flag);
#ifdef switch_RE
					Switch_sign(Switch_sign_OVER);
#endif
					write_linux_re_build(pr_fail); //保存失败的记录

					MSG_LOG("删冲正111--\r\n");
					memset(repurse_infor, 0, sizeof(repurse_infor));
#ifdef switch_RE
					init_timeout_infor(); //清冲正延时上送
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
					write_linux_re_build(pr_repurse); //超时保存一条冲正

					retcode = -1;
					goto Send_WaitRecvData_OVER;
				}
			}
		}//	while (1) 
	}  //for
	//最后判断
	if (flag == 0xA5 && gCardinfo.gMCardCand != CARDSTYLE_UNPAY_ODA) {	// 收到正确数据 
		//			gSendOverTime = 0;

		MSG_LOG("删冲正22222--\r\n");
		memset(repurse_infor, 0, sizeof(repurse_infor));
#ifdef switch_RE
		init_timeout_infor(); //清冲正延时上送
	//	save_infor_add(FeRC_Dlelt, NULL);
		save_repurse_infor(FeRC_Dlelt, NULL);
#else
		save_repurse_infor(FeRC_Dlelt, NULL);
#endif
		pboc_free_cnt = PBOC_FREE_CNT;
		MSG_LOG("返回正常:%02x\r\n", flag);


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
