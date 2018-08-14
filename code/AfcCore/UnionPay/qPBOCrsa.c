/*==========================================================================*
 *    Copyright(c) 2001-2004, SANLIAN., Ltd.
 *                     ALL RIGHTS RESERVED
 *
 *  PRODUCT  : HD300
 *
 *  FILENAME : qPBOC.c
 *  CREATOR  : 	                DATE: 2007-10-29 10:11
 *  VERSION  : V1.00
 *  PURPOSE  :
 *
 *
 *  HISTORY  :
 *
 *==========================================================================*/
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "szct.h"
#include "Font.h"

#include "qPBOC.h"
#include "qpboc_head.h"
#include "add.h"
#define	LEN_QBOCTVL_DATA		1280
#include "ProcCharacter.h"
#ifdef qPBOC_BUS
#include "RSA.h"

#include "qpboc_8583.h"
#include "Gszct.h"
#include "TxnRecord.h"
#include "SQD.h"
 //#define PBOC_loan  //交易开关借贷记

 //如果定义了就说明需要判断是否和此行一致
 //#define _BankNameLimit "ICBC"
 //#define _BankNameLimit "BC"
 //中国银行可能有两个简称
 //#define _BankNameLimit2 "BOC"

 //	如果定义了，没有公钥的卡将刷不成功
#define _CheckPublicVer_ 

#define STRNOQPBOCC "非受理银行卡"

// #ifdef NO_MOBILE_SERVISE
// unsigned int gMobilREC_Start;//记录指针头
// unsigned int gMobilREC_End;//移动指针尾
// #endif

#ifdef _debug_
extern void dprintf(const char *fmt, ...);
#endif
extern unsigned char StufferNO[4];
extern unsigned char DriveCardNo[4];
extern unsigned int s_sum1, a_sum1, dis_sum2;
extern unsigned int a_sumR;//手机钱包中的真实金额
extern unsigned int c_serial;
extern unsigned char cardSound;			//刷卡没有成功标志
extern unsigned int card_ser;
extern unsigned int ghisCur;//历史当前指针
extern TCP_IP_PACKET1 tcpipPacket;
extern Parameter3 ComSumeFile;
extern unsigned char restore_flag;						//显示复原
extern Parameter5 cardMessage;
extern unsigned int g24GDisFlash;//24G卡界面刷新
extern pFistVary_1 pFistVary;
extern stBusVerIfo gBusVerInfo;
extern unsigned int get_s_sum1(void);
extern void dis_time_mini(unsigned char x, stcomtime *time);
extern unsigned char CheckSno(unsigned char mode, unsigned char ID, unsigned char *ptr);
extern void money_msg(unsigned char dmode, unsigned int remM, unsigned int pucM, unsigned char cMOde);
extern void SetTcpPacketTTLTime(unsigned char val);
extern unsigned char WriteRecord(unsigned char *buffer, unsigned char mode);
extern void SoundMessage(unsigned char cmd);
extern unsigned char checkBusInfo(void);
extern int SQDataFromSVT(unsigned char SQmode, int msecends);
extern int QpbocOfflineAuten(void);
extern char *ASC2BCD(char *strASC, int lenASC);
extern unsigned char month_decide(void);
extern unsigned char gchn_eng;//中英文标志
extern void disp_no_swipe(void);
int getqPbocMoney(void);
extern unsigned char GPRSLinkProcess;

extern unsigned char gMCardCand;

#define GPRSCOMMAX 1600
extern unsigned char gprsRevBuf[1600];//GPRS接收缓冲
extern volatile unsigned short gprsIndex;
extern unsigned char gprsErrorTimes;//出现ＥＲＲＯＲ返回错误次数
extern PACKET_RCV gPacketRcvBuf[MAX_RCV_PACKET_NUM];	//接收帧缓冲

extern stPricetable NewPriceTab;
extern unsigned char gmissflag;


extern BIT_TVL_TAG QpbocTVLFCI;
extern INT8U	QpbocTVLData[LEN_QBOCTVL_DATA];

//SAM卡中能存放最多的公钥文件数量
//共有文件15个，有一个文件需要算IC卡的签名认证。
//#define SAM_KEY_FILE_MAX 14

//unsigned char gPublicKeyVer[16];

//此标识判断是否存在LKT4201，不存在为0.
//unsigned char LKT4201exist=1;


static EMV_CARDTYPE emv_cardtype;
static PBOC_CHANNEL pay_channel;

void emv_set_card_type(EMV_CARDTYPE type)
{
	emv_cardtype = type;
}

EMV_CARDTYPE emv_get_card_type(void)
{
	return emv_cardtype;
}

void emv_set_pay_channel(PBOC_CHANNEL channel)
{
	pay_channel = channel;
}

PBOC_CHANNEL emv_get_pay_channel(void)
{
	return pay_channel;
}


unsigned char shuangmian = 0;	//双免交易成功标记
unsigned char online_funtion = 0;		//先走电子现金，余额不足时候走双免交易，需填写终端属性为9F66为：28000000


struct {
	struct {
		INT8U 	bAID_9F06[16];			//4 应用标识符AID=RID+PIX
		INT8U 	bASI;					//4 应用选择指示器
		INT8S 	aPPSEName[20];			//4 PPSE选择文件名
	}AppSelect;
	struct {
		INT8U 	bPosCapabilty[3];		//4 终端性能
		INT8U 	bAddPosCapabity[5];		//4 附加终端性能
		INT8U 	TSI[2];					//4 交易状态信息
		INT8U 	TVR[5];					//4 终端验证结果
	}AutenData;
	struct {
		INT8U 	VerNum[2];				//4 应用版本号
		INT8U 	PosContyCode[2];		//4 终端国家代码
	}ManageLimist;
	struct {
		INT8U 	CVMRstu[3];				//4 CVM结果
		INT8U 	PosType;				//4 终端类型
	}CVM;
	struct {
		INT8U 	bPosAttr9F66[4];			//4 终端交易属性
		INT32U 	DebitMoney;				//4 交易金额
		INT32U 	SecDebitMoney;			//4 与交易相关的第二金额，表示返现金额
		INT8U	nContryCode[2];			//4 国家代码
	}PDOL;
}PosOfferData;
unsigned char qpbocrevbuf[258];

QPBOC_RESULT_TYPE TradeResult;


static KEY_PUBLICMSG AuterCerKey;
static KEY_PUBLICMSG PublishKey;
static KEY_PUBLICMSG IcKey;
//static unsigned char AuterCerKeySamFile;

INT8U 	QpbocAppSum = 0;
static FCI_MODULE QpbocAppList[SUM_APP_LIST_AID];
static FCI_MODULE TempAIDTable[SUM_APP_LIST_AID];
static INT32U ValidAppNum = 0;					//4 有效的目录索引

extern stcomtime SysTime;

static INT32U	bit_TVLData = 1;
static INT8U 	PCBSerial = 0x02;
#define 	LEN_REPEAT_TAG_SUM		100
static 	INT32U 	RepeatPo = 0;
static INT16U 	RepeatTag[LEN_REPEAT_TAG_SUM];
static INT32U 	PosTradeCout = 1;
static INT32U 	MSDOrQpboc = 0;				// 执行MSG 或者Qpboc的交易
//static INT32U 	QPBOC_type = 0;
static INT8U 	SDADataAuten[512];
static INT32U 	SDADataALen;
static INT32U	SDADataValid = 0;

//----工商银行------
#if defined  BUS_DK_FENGGAN
#define HandListlen 0
const unsigned int gHandList[HandListlen + 1] = {
	0
};
// #define HandListlen 18
// const unsigned int gHandList[HandListlen+1] = {
// 	621225,621226,622236,623062,625331,625330,622235,622233,
// 		622237,622215,622208,628288,622232,622231,621281,621823,621670,625650,0
// 		
// };

// #elif defined BUS_XINGRE//取消限制
// #define HandListlen 19
// const unsigned int gHandList[HandListlen+1] = {
// 622848,622845,622846,622847,622825,621723,622208,6212263,621081,621700,621467,621499,621466,625955,6259564,
// 621798,621799,623208,621756,0
// 		
// };

//增加卡丙号：621758.621757.621668.621661.621669.621569/
// #elif defined BUS_QIANNAN// 取消限制
// #define HandListlen 16
// const unsigned int gHandList[HandListlen+1] = {
// 	621785,621786,621787,621788,621789,621790,621756,621757,620513,621725,623208,621758,621668,621661,621669,621569,0
// 		
// };
#elif defined BUS_YUQING
#define HandListlen 1
const unsigned int gHandList[HandListlen + 1] = {
	621460,0

};
#elif defined BUS_LIUZHI
#define HandListlen 1
const unsigned int gHandList[HandListlen + 1] = {
	621460,0

};
// #elif defined BUS_SINAN
// #define HandListlen 32
// const unsigned int gHandList[HandListlen+1] = {
// 	625997,625996,625998,625336,628269,622208,621226,621723,622237,628288,625859,\
// 	463231,622230,621700,623668,621284,621700,621283,621661,621787,621790,621756,\
// 	621789,623208,621786,621788,623208,621568,623569,623586,621779,0
// 		
// };
#elif defined BUS_SONGTAO
#define HandListlen 1
const unsigned int gHandList[HandListlen + 1] = {
	621460,0

};
#elif defined BUS_DUSHAN
#define HandListlen 1
const unsigned int gHandList[HandListlen + 1] = {
	621779,0

};
#elif defined BUS_JIANGKOU
#define HandListlen 1
const unsigned int gHandList[HandListlen + 1] = {
	621779,0
};//621779
#elif defined BUS_YILONG
#define HandListlen 1
const unsigned int gHandList[HandListlen + 1] = {
	621460,0

};
#elif defined BUS_PUAN
#define HandListlen 1
const unsigned int gHandList[HandListlen + 1] = {
	621779,0
};//621779
#elif defined BUS_CEHENG
#define HandListlen 9
const unsigned int gHandList[HandListlen + 1] = {
	622848,622845,622846,622849,625997,625996,625998,625336,628269,0
};//621779
#else
#define HandListlen 0
const unsigned int gHandList[HandListlen + 1] = {
	0
};
#endif



int WHT_BANK_Load(void)
{

	#warning "需要时候放开"
		printf("%s NULL:!!!!\r\n", __FUNCTION__);
	/*
		unsigned int flash_Addr = FLASH_WHT_BANK;
		unsigned char buftemp[32];
		unsigned int i,crc32,len,flashcrc,crdHand;
		unsigned short Ver=0;

	//	char *ptemp=NULL;//,*p1=NULL;	//开始先找这个
	//	char * p1 =NULL,p2=NULL;

		flashread(flash_Addr,buftemp,12);
		memcpy(&Ver,buftemp,4);
		memcpy(&len,buftemp+4,4);
		memcpy(&flashcrc,buftemp+8,4);

		MSG_LOG("load,whtinfor:");
		BCD_LOG(buftemp,12,1);


		memset(gBusVerInfo.WHT_Bank,0,2);
		HandListlen = 0;


		if ((Ver ==0) || (Ver == 0xffff))
		{
			return ST_ERROR;
		}
		if (len >MAX_WHT_BANK_NUM)
		{
			MSG_LOG("TOO Long\r\n");
			return ST_ERROR;
		}

		crc32 = calFileCRC(flash_Addr+12,len*4);
		MSG_LOG("cal crc32:%08X,crd32%08X\r\n",crc32,flashcrc);
		if (crc32 != flashcrc)
		{
			return ST_ERROR;
		}

		MSG_LOG("CRCok--\r\n");
		for (i=0;i<len;i++)
		{
			flashread(flash_Addr+12+i*4,(unsigned char *)&crdHand,4);
			gHandList[HandListlen++] = crdHand;
		}

		memcpy(gBusVerInfo.WHT_Bank,&Ver,2);




		MSG_LOG("HandListlen:%d\r\n",HandListlen);
	#ifdef _debug_
		for (i=0;i<HandListlen;i++)
		{
			MSG_LOG("gHandList[%d]:%u,",i,gHandList[i]);

		}
	#endif*/


	return ST_OK;

}
int WHT_BANK_Save(unsigned int flash_Addr)
{

	#warning "需要时候放开"
		printf("%s NULL:!!!!\r\n", __FUNCTION__);

	/*
		unsigned char buftemp[32];
		unsigned int i,j,crc32,len,flashcrc,crdHand;
		unsigned short Ver=0;
		unsigned char buffer[512];
	//	unsigned int fileoff=0,templen=0;

	// 	char *ptemp=NULL;//,*p1=NULL;	//开始先找这个
	// 	char * p1 =NULL,p2=NULL;

		flashread(flash_Addr,buftemp,12);
		memcpy(&Ver,buftemp,4);
		memcpy(&len,buftemp+4,4);
		memcpy(&flashcrc,buftemp+8,4);

		memset(gBusVerInfo.WHT_Bank,0,2);
		HandListlen = 0;


		MSG_LOG("whtinfor:");
		BCD_LOG(buftemp,12,1);




		if ((Ver ==0) || (Ver == 0xffff))
		{
			return ST_ERROR;
		}

		if (len >1024)
		{
			MSG_LOG("save TOO Long%d\r\n",len);
			return ST_ERROR;
		}




		crc32 = calFileCRC(flash_Addr+12,len);
		MSG_LOG("cal crc32:%08X,crd32%08X\r\n",crc32,flashcrc);
		if (crc32 != flashcrc)
		{
			return ST_ERROR;
		}


		flashread(flash_Addr+12,buffer,sizeof(buffer));




		j = 0;
		for (i=0;i<len;)
		{


			if (i>=len)
			{
				break;
			}


			if ((buffer[i]>='0') &&((buffer[i]<='9')))
			{
				buftemp[j++] = buffer[i];
			}
			i++;

			if ((buffer[i] == 0x0d) &&(buffer[i+1]==0x0a))
			{
				buftemp[j] = 0;
				crdHand = (unsigned int )__atol(( char *)buftemp);
	//			MSG_LOG("crdHand:%d\r\n",crdHand);
				gHandList[HandListlen++] = crdHand;
				j = 0;
				i+=2;
				continue;
			}
		}

		if (j != 0)
		{
			//最后一个，有可能没有回车
			crdHand = (unsigned int )__atol(( char *)buftemp);
	// 		MSG_LOG("crdHand:%d\r\n",crdHand);
			gHandList[HandListlen++] = crdHand;


		}



		flasherase(ERASE4KBYTESECTOR, FLASH_WHT_BANK);//先擦除 4K



		for (i=0;i<HandListlen;i++)
		{
			memcpy(buffer,&gHandList[i],4);
			MSG_LOG("crdHand:%d ",gHandList[i]);
			BCD_LOG(buffer,4,1);
			flashwrite(FLASH_WHT_BANK+12 +i*4,buffer,4);
		}


		crc32 = calFileCRC(FLASH_WHT_BANK+12,HandListlen*4);


		memcpy(buftemp,&Ver,4);
		memcpy(buftemp+4,&HandListlen,4);
		memcpy(buftemp+8,&crc32,4);

		MSG_LOG("whtinfor2:");
		BCD_LOG(buftemp,12,1);


		flashwrite(FLASH_WHT_BANK,buftemp,12);








		if (WHT_BANK_Load() != ST_OK)
		{
			return ST_ERROR;
		}
		*/


	return ST_OK;

}



//static char gPKIsamErrflag;
//------------------

//static char gPKIsamErrflag;
//static char strbuff[300];
static char *BCD2ASC(unsigned char *strBCD, int lenBCD)
{
	int  i, pos;
	memset(irda_rxBuf, 0, sizeof(irda_rxBuf));
	if (lenBCD > sizeof(irda_rxBuf))
		lenBCD = sizeof(irda_rxBuf);
	pos = 0;
	for (i = 0; i < lenBCD; i++) {
		sprintf((char*)irda_rxBuf + pos, "%02X", strBCD[i]);
		pos += 2;
	}
	irda_rxBuf[pos] = 0x00;
	return (char*)irda_rxBuf;
}

//void get_BER_TVL(IN INT32U bitPos,OUT BER_TVL *OutTVL);
//比较句柄 允许消费返回st_OK;
unsigned char qbpoc_check_hand(void)
{
	unsigned int crdHand;
	int i;
	BER_TVL TempTVL;

	if (HandListlen == 0) {	//没有限制
		return ST_OK;
	}
	else {
		get_BER_TVL(QpbocTVLFCI.t_57.SizeOff, &TempTVL);

		memset(irda_rxBuf, 0, 20);
		BCD2Ascii(TempTVL.V, irda_rxBuf, 3);
		crdHand = __atol((char*)irda_rxBuf);//转成16进制。

		MSG_LOG("crdHand:: %d\r\n", crdHand);
		for (i = 0; i < HandListlen; i++) {
			if (crdHand == gHandList[i])
				return ST_OK;
		}
		return ST_ERROR;
	}
}

static int save_Tag_bit(IN INT32U Tag, IN INT32U bitPos)
{
	unsigned char tagflag;
	tagflag = 0;
	switch (Tag) {
	case 0x4F:
		// 			if(QpbocTVLFCI.t_4F.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_4F.TagValue = 0x4F;
		QpbocTVLFCI.t_4F.SizeOff = bitPos;
		break;
	case 0x50:
		// 			if(QpbocTVLFCI.t_50.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_50.TagValue = 0x50;
		QpbocTVLFCI.t_50.SizeOff = bitPos;
		break;
	case 0x57:
		// 			if(QpbocTVLFCI.t_57.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_57.TagValue = 0x57;
		QpbocTVLFCI.t_57.SizeOff = bitPos;
		break;
	case 0x5A:
		// 			if(QpbocTVLFCI.t_5A.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_5A.TagValue = 0x5A;
		QpbocTVLFCI.t_5A.SizeOff = bitPos;
		break;
	case 0x61:
		// 			if(QpbocTVLFCI.t_61.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_61.TagValue = 0x61;
		QpbocTVLFCI.t_61.SizeOff = bitPos;
		break;
	case 0x6F:
		// 			if(QpbocTVLFCI.t_6F.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_6F.TagValue = 0x6F;
		QpbocTVLFCI.t_6F.SizeOff = bitPos;
		break;
	case 0x82:
		// 			if(QpbocTVLFCI.t_82.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_82.TagValue = 0x82;
		QpbocTVLFCI.t_82.SizeOff = bitPos;
		break;
	case 0x84:
		// 			if(QpbocTVLFCI.t_84.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_84.SizeOff = 0x84;
		QpbocTVLFCI.t_84.SizeOff = bitPos;
		break;
	case 0x8C:
		// 			if(QpbocTVLFCI.t_84.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_8C.SizeOff = 0x8C;
		QpbocTVLFCI.t_8C.SizeOff = bitPos;
		break;
	case 0x8E:
		// 			if(QpbocTVLFCI.t_8E.SizeOff != 0) {
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_8E.TagValue = 0x8E;
		QpbocTVLFCI.t_8E.SizeOff = bitPos;
		break;
	case 0x8F:
		// 			if(QpbocTVLFCI.t_8F.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_8F.TagValue = 0x8F;
		QpbocTVLFCI.t_8F.SizeOff = bitPos;
		break;
	case 0x90:
		// 			if(QpbocTVLFCI.t_90.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_90.TagValue = 0x90;
		QpbocTVLFCI.t_90.SizeOff = bitPos;
		break;
	case 0x92:
		// 			if(QpbocTVLFCI.t_92.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_92.TagValue = 0x92;
		QpbocTVLFCI.t_92.SizeOff = bitPos;
		break;
	case 0x93:
		// 			if(QpbocTVLFCI.t_93.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_93.TagValue = 0x93;
		QpbocTVLFCI.t_93.SizeOff = bitPos;
		break;
	case 0x94:
		// 			if(QpbocTVLFCI.t_94.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_94.TagValue = 0x94;
		QpbocTVLFCI.t_94.SizeOff = bitPos;
		break;
	case 0xA5:
		// 			if(QpbocTVLFCI.t_A5.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_A5.TagValue = 0xA5;
		QpbocTVLFCI.t_A5.SizeOff = bitPos;
		break;
	case 0x5F24:
		// 			if(QpbocTVLFCI.t_5F24.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_5F24.TagValue = 0x5F24;
		QpbocTVLFCI.t_5F24.SizeOff = bitPos;
		break;
	case 0x5F25:
		// 			if(QpbocTVLFCI.t_5F25.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_5F25.TagValue = 0x5F25;
		QpbocTVLFCI.t_5F25.SizeOff = bitPos;
		break;
	case 0x5F28:
		// 			if(QpbocTVLFCI.t_5F28.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_5F28.TagValue = 0x5F28;
		QpbocTVLFCI.t_5F28.SizeOff = bitPos;
		break;
	case 0x5F2D:
		// 			if(QpbocTVLFCI.t_5F2D.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_5F2D.TagValue = 0x5F2D;
		QpbocTVLFCI.t_5F2D.SizeOff = bitPos;
		break;
	case 0x5F30:
		// 			if(QpbocTVLFCI.t_5F30.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_5F30.TagValue = 0x5F30;
		QpbocTVLFCI.t_5F30.SizeOff = bitPos;
		break;
	case 0x5F34:
		// 			if(QpbocTVLFCI.t_5F34.SizeOff != 0){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_5F34.TagValue = 0x5F34;
		QpbocTVLFCI.t_5F34.SizeOff = bitPos;
		break;
	case 0x9F07:
		// 			if(QpbocTVLFCI.t_9F07.TagValue==0x9F07){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F07.TagValue = 0x9F07;
		QpbocTVLFCI.t_9F07.SizeOff = bitPos;
		break;
	case 0x9F08:
		// 			if(QpbocTVLFCI.t_9F08.TagValue==0X9F08){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F08.TagValue = 0X9F08;
		QpbocTVLFCI.t_9F08.SizeOff = bitPos;
		break;
	case 0x9F0B:
		// 			if(QpbocTVLFCI.t_9F0B.TagValue==0x9F0B){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F0B.TagValue = 0x9F0B;
		QpbocTVLFCI.t_9F0B.SizeOff = bitPos;
		break;
	case 0x9F0D:
		// 			if(QpbocTVLFCI.t_9F0D.TagValue==0x9F0D){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F0D.TagValue = 0x9F0D;
		QpbocTVLFCI.t_9F0D.SizeOff = bitPos;
		break;
	case 0x9F0E:
		// 			if(QpbocTVLFCI.t_9F0E.TagValue==0x9F0E){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F0E.TagValue = 0x9F0E;
		QpbocTVLFCI.t_9F0E.SizeOff = bitPos;
		break;
	case 0x9F0F:
		// 			if(QpbocTVLFCI.t_9F0F.TagValue==0x9F0F){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F0F.TagValue = 0x9F0F;
		QpbocTVLFCI.t_9F0F.SizeOff = bitPos;
		break;
	case 0x9F10:
		// 			if(QpbocTVLFCI.t_9F10.TagValue==0x9F10){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F10.TagValue = 0x9F10;
		QpbocTVLFCI.t_9F10.SizeOff = bitPos;
		break;
	case 0x9F11:
		// 			if(QpbocTVLFCI.t_9F11.TagValue==0x9F11){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F11.TagValue = 0x9F11;
		QpbocTVLFCI.t_9F11.SizeOff = bitPos;
		break;
	case 0x9F12:
		// 			if(QpbocTVLFCI.t_9F12.TagValue==0x9F12){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F12.TagValue = 0x9F12;
		QpbocTVLFCI.t_9F12.SizeOff = bitPos;
		break;
	case 0x9F26:
		// 			if(QpbocTVLFCI.t_9F26.TagValue==0x9F26){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F26.TagValue = 0x9F26;
		QpbocTVLFCI.t_9F26.SizeOff = bitPos;
		break;
	case 0x9F27:
		// 			if(QpbocTVLFCI.t_9F26.TagValue==0x9F26){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F27.TagValue = 0x9F27;
		QpbocTVLFCI.t_9F27.SizeOff = bitPos;
		break;
	case 0x9F32:
		// 			if(QpbocTVLFCI.t_9F32.TagValue==0x9F32){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F32.TagValue = 0x9F32;
		QpbocTVLFCI.t_9F32.SizeOff = bitPos;
		break;
	case 0x9F36:
		// 			if(QpbocTVLFCI.t_9F36.TagValue==0x9F36){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F36.TagValue = 0x9F36;
		QpbocTVLFCI.t_9F36.SizeOff = bitPos;
		break;
	case 0x9F38:
		// 			if(QpbocTVLFCI.t_9F38.TagValue==0x9F38){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F38.TagValue = 0x9F38;
		QpbocTVLFCI.t_9F38.SizeOff = bitPos;
		break;
	case 0x9F46:
		// 			if(QpbocTVLFCI.t_9F46.TagValue==0x9F46){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F46.TagValue = 0x9F46;
		QpbocTVLFCI.t_9F46.SizeOff = bitPos;
		break;
	case 0x9F47:
		// 			if(QpbocTVLFCI.t_9F47.TagValue==0x9F47){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F47.TagValue = 0x9F47;
		QpbocTVLFCI.t_9F47.SizeOff = bitPos;
		break;
	case 0x9F48:
		// 			if(QpbocTVLFCI.t_9F48.TagValue==0x9F48){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F48.TagValue = 0x9F48;
		QpbocTVLFCI.t_9F48.SizeOff = bitPos;
		break;
	case 0x9F49:
		// 			if(QpbocTVLFCI.t_9F49.TagValue==0x9F49){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F49.TagValue = 0x9F49;
		QpbocTVLFCI.t_9F49.SizeOff = bitPos;
		break;
	case 0x9F4A:
		// 			if(QpbocTVLFCI.t_9F4A.TagValue==0x9F4A) {
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F4A.TagValue = 0x9F4A;
		QpbocTVLFCI.t_9F4A.SizeOff = bitPos;
		break;
	case 0x9F4B:
		// 			if(QpbocTVLFCI.t_9F4B.TagValue==0x9F4B){
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F4B.TagValue = 0x9F4B;
		QpbocTVLFCI.t_9F4B.SizeOff = bitPos;
		break;
	case 0x9F4D:
		// 			if(QpbocTVLFCI.t_9F4D.TagValue==0x9F4D) {
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F4D.TagValue = 0x9F4D;
		QpbocTVLFCI.t_9F4D.SizeOff = bitPos;
		break;
	case 0x9F51:
		QpbocTVLFCI.t_9F51.TagValue = T_MONEYCODE;
		QpbocTVLFCI.t_9F51.SizeOff = bitPos;
		break;
	case 0x9F5D:
		// 			if(QpbocTVLFCI.t_9F5D.TagValue==0x9F5D) {
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F5D.TagValue = 0x9F5D;
		QpbocTVLFCI.t_9F5D.SizeOff = bitPos;
		break;
	case 0x9F6C:
		// 			if(QpbocTVLFCI.t_9F6C.TagValue==0x9F6C) {
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F6C.TagValue = 0x9F6C;
		QpbocTVLFCI.t_9F6C.SizeOff = bitPos;
		break;
	case 0x9F74:
		if (QpbocTVLFCI.t_9F74.TagValue == 0x9F74) {
			tagflag = 1;
			break;
		}
		QpbocTVLFCI.t_9F74.TagValue = 0x9F74;
		QpbocTVLFCI.t_9F74.SizeOff = bitPos;
		break;
	case 0x9F79:
		// 			if(QpbocTVLFCI.t_9F79.TagValue==0x9F79) {
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_9F79.TagValue = 0x9F79;
		QpbocTVLFCI.t_9F79.SizeOff = bitPos;
		break;
	case 0xBF0C:
		// 			if(QpbocTVLFCI.t_BF0C.TagValue==0xBF0C) {
		// 				tagflag = 1;
		// 				break;
		// 			}
		QpbocTVLFCI.t_BF0C.TagValue = 0xBF0C;
		QpbocTVLFCI.t_BF0C.SizeOff = bitPos;
		break;
	case 0xDF61:
		QpbocTVLFCI.t_DF61.TagValue = 0xDF61;
		QpbocTVLFCI.t_DF61.SizeOff = bitPos;
		break;
	case 0xDF71:
		QpbocTVLFCI.t_DF71.TagValue = 0xDF71;
		QpbocTVLFCI.t_DF71.SizeOff = bitPos;
		break;

	default:
		break;

	}
	if (tagflag == 1) {
		MSG_LOG("TAG-:%04X 重复了\r\n", Tag);
		return -1;
	}
	return 0;
}
static int get_TVL_Tag(IN INT8U mode, IN INT8U *InMsg, IN INT32U Offaddr, IN INT32U len)
{
	INT32U i;
	INT8U *p = InMsg;
	INT32U TempBit;
	INT8U Tag[2], LenBit;
	INT32U lTag = 0;
	TYPE_CHAR_LONG lTempLong;
	//	INT32U 	IFistFlag =0;
	INT8U 	Buffer[4];

	while ((INT32U)(p - InMsg) < len) {
		if ((*p == 0xFF) || (*p == 0x00)) {
			p += 1;
			continue;
		}
		if ((*p & 0x20) == 0x20) {
			// 			if(p == InMsg) IFistFlag =1;
			// 			else IFistFlag =0;

			Tag[0] = *p++;
			if ((Tag[0] & 0x1F) == 0x1F) {
				Tag[1] = *p++;
				// save tag bit
				lTag = (INT32U)Tag[0] * 256 + Tag[1];

				TempBit = (p - InMsg - 2 + Offaddr);
				lTempLong.LongBuff = 0;
				if ((*p & 0x80) == 0x80) {
					LenBit = *p++ & 0x7F;
					if (LenBit == 1)
						lTempLong.CharBuff[0] = *p++;
					else if (LenBit == 2) {
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}
					else if (LenBit == 3) {
						lTempLong.CharBuff[2] = *p++;
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}
					else if (LenBit == 4) {
						lTempLong.CharBuff[3] = *p++;
						lTempLong.CharBuff[2] = *p++;
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}

				}
				else {
					lTempLong.LongBuff = (INT32U)*p++;
				}

				// 				if(IFistFlag == 1){
				// 					if((lTempLong.LongBuff +(p-InMsg)) != len){
				// 						ERROR("a=%d,(p-Insmg)=%d,len=%d\r\n",lTempLong.LongBuff,(p-InMsg),len);
				// 						return qPBOC_TVL_INVALID;
				// 					}
				// 				}
				if (lTempLong.LongBuff != 0)
					save_Tag_bit(lTag, TempBit);
				MSG_LOG("TAG-:%04X  %d  %s\r\n", lTag, lTempLong.LongBuff, BCD2ASC(p, lTempLong.LongBuff));
				//	if(lTempLong.LongBuff == 0) return qPBOC_TVL_INVALID;

			}
			else {
				lTag = (INT32U)Tag[0];
				//save_Tag_bit(lTag,(p-InMsg-1+Offaddr));
				TempBit = (p - InMsg - 1 + Offaddr);
				lTempLong.LongBuff = 0;
				if ((*p & 0x80) == 0x80) {
					LenBit = *p++ & 0x7F;

					MSG_LOG("LenBit-:%d  %d %04X\r\n", LenBit, (*p), lTempLong.LongBuff);

					if (LenBit == 1)
						lTempLong.CharBuff[0] = *p++;
					else if (LenBit == 2) {
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}
					else if (LenBit == 3) {
						lTempLong.CharBuff[2] = *p++;
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}
					else if (LenBit == 4) {
						lTempLong.CharBuff[3] = *p++;
						lTempLong.CharBuff[2] = *p++;
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}
					else {
						if (InMsg[0] == 0x70) {
							p--;
							lTempLong.LongBuff = (INT32U)*p++;
						}
						else
							lTempLong.LongBuff = (lTempLong.LongBuff);
					}
				}
				else {
					lTempLong.LongBuff = (INT32U)*p++;
				}
				// 				if(IFistFlag == 1){
				// 					if((lTempLong.LongBuff +(p-InMsg)) != len){
				// 						ERROR("a=%d,(p-Insmg)=%d,len=%d\r\n",lTempLong.LongBuff,(p-InMsg),len);
				// 						return qPBOC_TVL_INVALID;
				// 					}
				// 				}
								//save_Tag_bit(lTag,(TempP-InMsg-2+Offaddr));
				if (lTempLong.LongBuff != 0)
					save_Tag_bit(lTag, TempBit);
				MSG_LOG("TAG-:%04X  %d  %s\r\n", lTag, lTempLong.LongBuff, BCD2ASC(p, lTempLong.LongBuff));
				//				if(lTempLong.LongBuff == 0) return qPBOC_TVL_INVALID;
			}
		}
		else {
			Tag[0] = *p++;
			if ((Tag[0] & 0x1F) == 0x1F) {
				Tag[1] = *p++;
				// save tag bit
				lTag = (INT32U)Tag[0] * 256 + Tag[1];
				if (mode == 1) {
					if ((lTag != 0x85) && (lTag != 0x5F34)) {
						for (i = 0; i < RepeatPo; i++) {
							if (RepeatTag[i] == lTag)
								break;//return qPBOC_REPEAT_TAG;
						}
						if (i >= RepeatPo)//没有重复
						{
							if (RepeatPo >= LEN_REPEAT_TAG_SUM) {
								ERROR("Repeat Tag is Over\r\n");
								return -1;
							}
							RepeatTag[RepeatPo++] = lTag;
						}
					}
				}
				//save_Tag_bit(lTag,(p-InMsg-2+Offaddr));
				TempBit = (p - InMsg - 1 + Offaddr);
				lTempLong.LongBuff = 0;
				if ((*p & 0x80) == 0x80) {
					LenBit = *p & 0x7F;
					p += 1;
					if (LenBit == 1)
						lTempLong.CharBuff[0] = *p++;
					else if (LenBit == 2) {
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}
					else if (LenBit == 3) {
						lTempLong.CharBuff[2] = *p++;
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}
					else if (LenBit == 4) {
						lTempLong.CharBuff[3] = *p++;
						lTempLong.CharBuff[2] = *p++;
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}
					MSG_LOG("TAG-:%04X  %d  %s\r\n", lTag, lTempLong.LongBuff, BCD2ASC(p, lTempLong.LongBuff));


				}
				else {
					lTempLong.LongBuff = (INT32U)*p++;
					MSG_LOG("TAG-:%04X  %d  %s\r\n", lTag, lTempLong.LongBuff, BCD2ASC(p, lTempLong.LongBuff));

				}
				if (lTempLong.LongBuff != 0)
					save_Tag_bit(lTag, TempBit);
				//				if(lTempLong.LongBuff == 0) return qPBOC_TVL_INVALID;
				if (lTag == 0x5F25) {
					memset(Buffer, 0, 4);
					memcpy(Buffer + 1, p, 3);
					if (*p >= 0x50) Buffer[0] = 0x19;
					else Buffer[0] = 0x20;
					if (memcmp(Buffer, (INT8U *)&SysTime.year_h, 4) > 0) {
						ERROR("TVR:QpbocManageLimist-应用生效日期> 当期日期:%02X%02X%02X%02X,%02X%02X%02X\r\n",
							Buffer[0], Buffer[1], Buffer[2], Buffer[3], SysTime.year_l, SysTime.month, SysTime.day);
						ERROR("TVR:QpbocManageLimist-应用生效日期> 当期日期:%02X%02X%02X%02X,%02X%02X%02X\r\n",
							*p, *(p + 1), *(p + 2), *(p + 3), SysTime.year_l, SysTime.month, SysTime.day);
						//PosOfferData.AutenData.TVR[1] |=TVR_APP_INVALID;
						//return qPBOC_STARTTIME_INVLAID;
					}
				}
				if (lTag == 0x5F24) {
					memset(Buffer, 0, 4);
					memcpy(Buffer + 1, p, 3);
					if (*p >= 0x50) Buffer[0] = 0x19;
					else Buffer[0] = 0x20;
					if (memcmp(Buffer, (INT8U *)&SysTime.year_h, 4) < 0) {
						ERROR("TVR:QpbocManageLimist-应用失效日期< 当前日期:%02X%02X%02X,%02X%02X%02X\r\n",
							Buffer[0], Buffer[1], Buffer[2], SysTime.year_l, SysTime.month, SysTime.day);
						PosOfferData.AutenData.TVR[1] |= TVR_APP_EXP;
						return qPBOC_ENDTIME_INVLIAD;
					}
				}
				p += lTempLong.LongBuff;
			}
			else {
				// save tag bit
				lTag = (INT32U)Tag[0];
				if (mode == 1) {
					if ((lTag != 0x85) && (lTag != 0x5F34)) {
						for (i = 0; i < RepeatPo; i++) {
							if (RepeatTag[i] == lTag)
								break;//return qPBOC_REPEAT_TAG;
						}
						if (i >= RepeatPo) {
							if (RepeatPo >= LEN_REPEAT_TAG_SUM) {
								ERROR("Repeat Tag is Over\r\n");
								return -1;
							}
							RepeatTag[RepeatPo++] = lTag;
						}
					}
				}
				//save_Tag_bit(lTag,(p-InMsg-1+Offaddr));
				TempBit = (p - InMsg - 1 + Offaddr);
				lTempLong.LongBuff = 0;
				if ((*p & 0x80) == 0x80) {
					LenBit = *p++ & 0x7F;
					if (LenBit == 1)
						lTempLong.CharBuff[0] = *p++;
					else if (LenBit == 2) {
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}
					else if (LenBit == 3) {
						lTempLong.CharBuff[2] = *p++;
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}
					else if (LenBit == 4) {
						lTempLong.CharBuff[3] = *p++;
						lTempLong.CharBuff[2] = *p++;
						lTempLong.CharBuff[1] = *p++;
						lTempLong.CharBuff[0] = *p++;
					}
					MSG_LOG("TAG-:%04X  %d  %s\r\n", lTag, lTempLong.LongBuff, BCD2ASC(p, lTempLong.LongBuff));
					//					p +=lTempLong.LongBuff;

				}
				else {
					lTempLong.LongBuff = (INT32U)*p++;
					MSG_LOG("TAG-:%04X  %d  %s\r\n", lTag, lTempLong.LongBuff, BCD2ASC(p, lTempLong.LongBuff));
					//					p +=lTempLong.LongBuff;

				}
				if (lTempLong.LongBuff != 0)
					save_Tag_bit(lTag, TempBit);
				//				if(lTempLong.LongBuff == 0) return qPBOC_TVL_INVALID;
				if (lTag == 0x5F25) {
					memset(Buffer, 0, 4);
					memcpy(Buffer + 1, p + 1, 3);
					if (*p >= 0x50) Buffer[0] = 0x19;
					else Buffer[0] = 0x20;
					if (memcmp(Buffer, (INT8U *)&SysTime.year_h, 4) > 0) {
						ERROR("TVR:QpbocManageLimist-应用生效日期> 当期日期:%02X%02X%02X,%02X%02X%02X\r\n",
							Buffer[0], Buffer[1], Buffer[2], SysTime.year_l, SysTime.month, SysTime.day);
						PosOfferData.AutenData.TVR[1] |= TVR_APP_INVALID;
						return qPBOC_STARTTIME_INVLAID;
					}
				}
				if (lTag == 0x5F24) {
					memset(Buffer, 0, 4);
					memcpy(Buffer + 1, p, 3);
					if (*p >= 0x50) Buffer[0] = 0x19;
					else Buffer[0] = 0x20;
					if (memcmp(Buffer, (INT8U *)&SysTime.year_h, 4) < 0) {
						ERROR("TVR:QpbocManageLimist-应用失效日期< 当前日期:%02X%02X%02X,%02X%02X%02X\r\n",
							Buffer[0], Buffer[1], Buffer[2], SysTime.year_l, SysTime.month, SysTime.day);
						PosOfferData.AutenData.TVR[1] |= TVR_APP_EXP;
						return qPBOC_ENDTIME_INVLIAD;
					}
				}
				p += lTempLong.LongBuff;
			}
		}
	}
	return 0;
}
static INT8U *Find_CharData(IN INT8U *InMsg, IN INT32U SrcLen, IN INT8U *ObjMsg, IN INT32U ObjLen)
{
	INT32U i, j;
	INT8U *p = InMsg;
	for (i = 0; i < SrcLen; i++) {
		if (p[i] == ObjMsg[0]) {
			if ((SrcLen == 1) && (ObjLen == 1)) {
				return InMsg;
			}
			i++;
			for (j = 1; j < ObjLen; i++, j++) {
				if (p[i] != ObjMsg[j]) break;
			}
			if (j == ObjLen) break;
		}
	}
	if (i == SrcLen) return NULL;
	else return (InMsg + i - ObjLen);
}

int qPBOC_INIT(void)
{
	QpbocAppSum = 0;
	ValidAppNum = 0;					//4 有效的目录索引
	bit_TVLData = 1;
	PCBSerial = 0x02;
	RepeatPo = 0;
	PosTradeCout = 1;
	MSDOrQpboc = 0;// 执行MSG 或者Qpboc的交易
	SDADataValid = 0;


// #if SWITCH_PRIOR_ODA == 0
// 	shuangmian = 1;
// #else
// 	shuangmian = 0;
// #endif
	return 0;
}
static int Analytic_FCIMoudle(IN INT8U *InMsg, IN INT32U Len, OUT INT8U *AppListCnt)
{
	INT8U *p, i, *tempP, *temp2P;
	INT32U BFOC_Len = 0, Len_61 = 0;
	INT8U  temp;
	TYPE_CHAR_LONG lTempLong;
	INT8U objbuf[4];

	*AppListCnt = 0;
	for (i = 0; i < SUM_APP_LIST_AID; i++) memset((INT8U *)&TempAIDTable[i], 0, sizeof(FCI_MODULE));

	p = NULL;
	memcpy(objbuf, "\xBF\x0C", 2);
	if ((p = Find_CharData(InMsg, Len, objbuf, 2)) == NULL) {
		return -1;
	}

	p += 2;
	if ((*p & 0x80) == 0x80) {
		temp = *p++ & 0x7F;
		lTempLong.LongBuff = 0;
		if (temp == 1)
			lTempLong.CharBuff[0] = *p++;
		else if (temp == 2) {
			lTempLong.CharBuff[1] = *p++;
			lTempLong.CharBuff[0] = *p++;
		}
		else if (temp == 3) {
			lTempLong.CharBuff[2] = *p++;
			lTempLong.CharBuff[1] = *p++;
			lTempLong.CharBuff[0] = *p++;
		}
		else if (temp == 4) {
			lTempLong.CharBuff[3] = *p++;
			lTempLong.CharBuff[2] = *p++;
			lTempLong.CharBuff[1] = *p++;
			lTempLong.CharBuff[0] = *p++;
		}
		BFOC_Len = lTempLong.LongBuff;
	}
	else {
		BFOC_Len = *p++;
	}
	tempP = p;
	while ((INT32U)(p - tempP) < BFOC_Len) {
		if (*p++ == T_DIR_ENTER) {
			if ((*p & 0x80) == 0x80) {
				temp = *p++ & 0x7F;
				lTempLong.LongBuff = 0;
				if (temp == 1)
					lTempLong.CharBuff[0] = *p++;
				else if (temp == 2) {
					lTempLong.CharBuff[1] = *p++;
					lTempLong.CharBuff[0] = *p++;
				}
				else if (temp == 3) {
					lTempLong.CharBuff[2] = *p++;
					lTempLong.CharBuff[1] = *p++;
					lTempLong.CharBuff[0] = *p++;
				}
				else if (temp == 4) {
					lTempLong.CharBuff[3] = *p++;
					lTempLong.CharBuff[2] = *p++;
					lTempLong.CharBuff[1] = *p++;
					lTempLong.CharBuff[0] = *p++;
				}
				Len_61 = lTempLong.LongBuff;
			}
			else {
				Len_61 = *p++;
			}
			temp2P = p;
			while ((INT32U)(p - temp2P) < Len_61) {
				switch (*p) {
				case T_APP_FILE_AID:
					p += 1;
					lTempLong.LongBuff = 0;
					if ((*p & 0x80) == 0x80) {
						temp = *p++ & 0x7F;
						if (temp == 1)
							lTempLong.CharBuff[0] = *p++;
						else if (temp == 2) {
							lTempLong.CharBuff[1] = *p++;
							lTempLong.CharBuff[0] = *p++;
						}
						else if (temp == 3) {
							lTempLong.CharBuff[2] = *p++;
							lTempLong.CharBuff[1] = *p++;
							lTempLong.CharBuff[0] = *p++;
						}
						else if (temp == 4) {
							lTempLong.CharBuff[3] = *p++;
							lTempLong.CharBuff[2] = *p++;
							lTempLong.CharBuff[1] = *p++;
							lTempLong.CharBuff[0] = *p++;
						}
					}
					else {
						lTempLong.LongBuff = *p++;
					}
					TempAIDTable[*AppListCnt].LenAID = lTempLong.LongBuff;
					memcpy(TempAIDTable[*AppListCnt].DF_AID, p, lTempLong.LongBuff);
					p += lTempLong.LongBuff;
					break;
				case T_APP_TARGE:
					p += 1;
					lTempLong.LongBuff = 0;
					if ((*p & 0x80) == 0x80) {
						temp = *p++ & 0x7F;
						if (temp == 1)
							lTempLong.CharBuff[0] = *p++;
						else if (temp == 2) {
							lTempLong.CharBuff[1] = *p++;
							lTempLong.CharBuff[0] = *p++;
						}
						else if (temp == 3) {
							lTempLong.CharBuff[2] = *p++;
							lTempLong.CharBuff[1] = *p++;
							lTempLong.CharBuff[0] = *p++;
						}
						else if (temp == 4) {
							lTempLong.CharBuff[3] = *p++;
							lTempLong.CharBuff[2] = *p++;
							lTempLong.CharBuff[1] = *p++;
							lTempLong.CharBuff[0] = *p++;
						}
					}
					else {
						lTempLong.LongBuff = *p++;
					}
					TempAIDTable[*AppListCnt].LenTag = lTempLong.LongBuff;
					memcpy(TempAIDTable[*AppListCnt].AppTag, p, lTempLong.LongBuff);
					p += lTempLong.LongBuff;
					break;
				case T_APP_USER_PRIOR:
					p += 1;
					lTempLong.LongBuff = 0;
					if ((*p & 0x80) == 0x80) {
						temp = *p++ & 0x7F;
						if (temp == 1)
							lTempLong.CharBuff[0] = *p++;
						else if (temp == 2) {
							lTempLong.CharBuff[1] = *p++;
							lTempLong.CharBuff[0] = *p++;
						}
						else if (temp == 3) {
							lTempLong.CharBuff[2] = *p++;
							lTempLong.CharBuff[1] = *p++;
							lTempLong.CharBuff[0] = *p++;
						}
						else if (temp == 4) {
							lTempLong.CharBuff[3] = *p++;
							lTempLong.CharBuff[2] = *p++;
							lTempLong.CharBuff[1] = *p++;
							lTempLong.CharBuff[0] = *p++;
						}
					}
					else {
						lTempLong.LongBuff = *p++;
					}
					TempAIDTable[*AppListCnt].LenPro = lTempLong.LongBuff;
					memcpy(TempAIDTable[*AppListCnt].AppPrio, p, lTempLong.LongBuff);
					p += lTempLong.LongBuff;
					break;
				default:
					ERROR("ERROR-:FCI Format is %02X\r\n", *p);
					return -2;
				}
			}
			*AppListCnt += 1;
		}
		else {
			p += 1;
		}
	}
	if (*AppListCnt == 0) return -3;
	return 0;
}
static void DOL_DataFill(OUT INT8U *PDOLData, OUT INT32U *PDLen, BER_TVL *pTempTVL)
{
	INT8U 	i;
	INT8U 	Buff[100];
	INT8S 	Buffer[20];
	INT8U 	RamData[4];
	INT8U 	*p, temp;
	INT32U 	len;
	INT16U 	Tag;
	TYPE_CHAR_LONG lTempLong;



	memcpy(Buff, (INT8U *)&SysTime.day, 4);
	Buff[0] += 0xAD;
	Buff[1] += 0x89;
	Buff[2] += 0xC3;
	Buff[3] += 0x93;
	QpbocTVLFCI.t_9F37.TagValue = 0x9F37;
	QpbocTVLFCI.t_9F37.SizeOff = bit_TVLData;
	memcpy(QpbocTVLData + bit_TVLData, "\x9F\x37\x04", 3);
	bit_TVLData += 3;
	memcpy(QpbocTVLData + bit_TVLData, Buff, 4);
	memcpy(RamData, Buff, 4);
	bit_TVLData += 4;

	*PDLen = 0;
	if (pTempTVL == NULL) {
		return;
	}
	//get_BER_TVL(QpbocTVLFCI.t_9F38.SizeOff, &TempTVL);

#ifdef _debug_
	//	MSG_LOG("9F38多写几个!!!!;\r\n");
	//	BCD_LOG(pTempTVL->V,pTempTVL->L,1);
	// 	Ascii2BCD("9F66049F02069F03069F1A0295055F2A029A039C019F3704",pTempTVL->V,48);
	// 	pTempTVL->L = 24;
	//  	MSG_LOG("9F38--\r\n");
	//  	BCD_LOG(pTempTVL->V,pTempTVL->L,1);

#endif

	p = pTempTVL->V;
	while ((INT32U)(p - pTempTVL->V) < pTempTVL->L) {
		len = 0;
		temp = *p;
		if ((*p++ & 0x1F) == 0x1F) {		// 说明TAG 后面还有一个字节
			Tag = (short)temp * 256 + *p++;
			if ((*p & 0x80) == 0x80) {
				temp = *p++ & 0x7F;
				lTempLong.LongBuff = 0;
				if (temp == 1)
					lTempLong.CharBuff[0] = *p++;
				else if (temp == 2) {
					lTempLong.CharBuff[1] = *p++;
					lTempLong.CharBuff[0] = *p++;
				}
				else if (temp == 3) {
					lTempLong.CharBuff[2] = *p++;
					lTempLong.CharBuff[1] = *p++;
					lTempLong.CharBuff[0] = *p++;
				}
				else if (temp == 4) {
					lTempLong.CharBuff[3] = *p++;
					lTempLong.CharBuff[2] = *p++;
					lTempLong.CharBuff[1] = *p++;
					lTempLong.CharBuff[0] = *p++;
				}
				len = lTempLong.LongBuff;
			}
			else {
				len = *p++;
			}
		}
		else {
			Tag = (INT16U)temp;
			if ((*p & 0x80) == 0x80) {
				temp = *p++ & 0x7F;
				lTempLong.LongBuff = 0;
				if (temp == 1)
					lTempLong.CharBuff[0] = *p++;
				else if (temp == 2) {
					lTempLong.CharBuff[1] = *p++;
					lTempLong.CharBuff[0] = *p++;
				}
				else if (temp == 3) {
					lTempLong.CharBuff[2] = *p++;
					lTempLong.CharBuff[1] = *p++;
					lTempLong.CharBuff[0] = *p++;
				}
				else if (temp == 4) {
					lTempLong.CharBuff[3] = *p++;
					lTempLong.CharBuff[2] = *p++;
					lTempLong.CharBuff[1] = *p++;
					lTempLong.CharBuff[0] = *p++;
				}
				len = lTempLong.LongBuff;
			}
			else {
				len = *p++;
			}
		}

		MSG_LOG("$PDOL_FILL:TAG=%02X\r\n", Tag);
		switch (Tag) {
			// $PDOL_FILL:TAG=9F66
			// $PDOL_FILL:TAG=9F02
			// $PDOL_FILL:TAG=9F03
			// $PDOL_FILL:TAG=9F1A
			// $PDOL_FILL:TAG=95
			// $PDOL_FILL:TAG=5F2A
			// $PDOL_FILL:TAG=9A
			// $PDOL_FILL:TAG=9C
			// $PDOL_FILL:TAG=9F37
			//2800000000000000000100000000000001560000000000015610100100AE9BF3C8
						//4 下列是ASICII
			//			9F1E 9F16 
		case 0x9F1E:				// 8 ASCII

			memset(Buff, 0, 100);
			memset(Buff, 0x30, 8);
			memcpy(PDOLData + *PDLen, Buff, len);
			*PDLen += len;
			break;
		case 0x9F16:				//ASCII 15
			memset(Buff, 0, 100);
			memset(Buff, 0x30, 15);
			memcpy(PDOLData + *PDLen, Buff, len);
			*PDLen += len;
			break;
		case 0x9F40:
			memset(Buff, 0, 100);
			memcpy(Buff, "\x60\x00\x00\x00\x00", 5);
			memcpy(PDOLData + *PDLen, Buff, len);
			*PDLen += len;
			break;
		case 0x9F41: //4       
			memset(Buff, 0, 20);
			sprintf((char *)Buff, "%08d", PosTradeCout);
			memset(PDOLData + *PDLen, 0, len);
			if (len >= 4)
				memcpy(PDOLData + *PDLen + len - 4, ASC2BCD((char *)Buff, 8), 4);
			else
				memcpy(PDOLData + *PDLen, ASC2BCD((char *)Buff, 8) + 4 - len, len);
			*PDLen += len;
			break;

		case 0x81:		//4 
			lTempLong.LongBuff = SWAP32(PosOfferData.PDOL.DebitMoney);
			memset(Buff, 0, 20);
			memcpy(Buff, lTempLong.CharBuff, 4);
			if (len <= 4) {
				for (i = 0; i < len; i++)
					PDOLData[*PDLen + i] = Buff[i];
			}
			else {
				memset(PDOLData + *PDLen, 0, len);
				memcpy(PDOLData + *PDLen, Buff, 4);
			}
			*PDLen += len;
			break;
		case 0x9F06:		//4  
			memset(Buff, 0, 20);
			memcpy(Buff, QpbocAppList[ValidAppNum].DF_AID, QpbocAppList[ValidAppNum].LenAID);
			memcpy(PDOLData + *PDLen, Buff, len);
			*PDLen += len;
			break;
		case T_DEV_TRADE_AATR://9F66
			Buff[0] = PosOfferData.PDOL.bPosAttr9F66[0];
			Buff[1] = PosOfferData.PDOL.bPosAttr9F66[1];
			Buff[2] = PosOfferData.PDOL.bPosAttr9F66[2];
			Buff[3] = PosOfferData.PDOL.bPosAttr9F66[3];
			QpbocTVLFCI.t_9F66.TagValue = 0x9F66;
			QpbocTVLFCI.t_9F66.SizeOff = bit_TVLData;
			memcpy(QpbocTVLData + bit_TVLData, "\x9F\x66\x04", 3);
			bit_TVLData += 3;
			memcpy(QpbocTVLData + bit_TVLData, Buff, 4);
			bit_TVLData += 4;
			if (len <= 4) {
				for (i = 0; i < len; i++)
					PDOLData[*PDLen + i] = Buff[i];
			}
			else {
				memset(PDOLData + *PDLen, 0, len);
				memcpy(PDOLData + *PDLen, Buff, 4);
			}
			*PDLen += len;
			// 				debugstring("pdol 9F66:");
			// 				debugdata(PDOLData, *PDLen, 1);
			break;
		case T_ACCREDIT_MONEY:			//4   9F02
			memset(Buffer, 0, sizeof(Buffer));
			sprintf((char *)Buffer, "%012d", PosOfferData.PDOL.DebitMoney);
			memcpy(Buff, ASC2BCD((char *)Buffer, 12), 6);
			QpbocTVLFCI.t_9F02.TagValue = 0x9F02;
			QpbocTVLFCI.t_9F02.SizeOff = bit_TVLData;
			memcpy(QpbocTVLData + bit_TVLData, "\x9F\x02\x06", 3);
			bit_TVLData += 3;
			memcpy(QpbocTVLData + bit_TVLData, Buff, 6);
			bit_TVLData += 6;

			if (len <= 6) {
				for (i = 0; i < len; i++)
					PDOLData[*PDLen + i] = Buff[6 - len + i];
			}
			else {
				memset(PDOLData + *PDLen, 0, 6);
				memcpy(PDOLData + *PDLen, Buff, 6);
			}
			*PDLen += len;
			break;
		case T_OTHER_MONEY:		//4 
			memset(Buffer, 0, sizeof(Buffer));
			sprintf((char *)Buffer, "%012d", PosOfferData.PDOL.SecDebitMoney);
			memcpy(Buff, ASC2BCD((char *)Buffer, 12), 6);
			QpbocTVLFCI.t_9F03.TagValue = 0x9F03;
			QpbocTVLFCI.t_9F03.SizeOff = bit_TVLData;
			memcpy(QpbocTVLData + bit_TVLData, "\x9F\x03\x06", 3);
			bit_TVLData += 3;
			memcpy(QpbocTVLData + bit_TVLData, Buff, 6);
			bit_TVLData += 6;
			if (len <= 6) {
				for (i = 0; i < len; i++)
					PDOLData[*PDLen + i] = Buff[6 - len + i];
			}
			else {
				memset(PDOLData + *PDLen, 0, len);
				memcpy(PDOLData + *PDLen, Buff, 6);
			}
			*PDLen += len;
			break;
		case T_DEV_COUNTY_CODE:
			memcpy(Buff, PosOfferData.PDOL.nContryCode, 2);
			QpbocTVLFCI.t_9F1A.TagValue = 0x9F1A;
			QpbocTVLFCI.t_9F1A.SizeOff = bit_TVLData;
			memcpy(QpbocTVLData + bit_TVLData, "\x9F\x1A\x02", 3);
			bit_TVLData += 3;
			memcpy(QpbocTVLData + bit_TVLData, Buff, 2);
			bit_TVLData += 2;
			if (len <= 2) {
				for (i = 0; i < len; i++)
					PDOLData[*PDLen + i] = Buff[2 - len + i];
			}
			else {
				memset(PDOLData + *PDLen, 0, 2);
				memcpy(PDOLData + *PDLen, Buff, len);
			}
			*PDLen += len;
			break;
		case T_DEV_TVR:		//b   95
			memset(Buff, 0, len);
			Buff[0] = 0;
			Buff[1] = 0;
			Buff[2] = 0;
			Buff[3] = 0;
			Buff[4] = 0;

			QpbocTVLFCI.t_95.TagValue = 0x95;
			QpbocTVLFCI.t_95.SizeOff = bit_TVLData;
			memcpy(QpbocTVLData + bit_TVLData, "\x95\x05", 2);
			bit_TVLData += 2;
			memcpy(QpbocTVLData + bit_TVLData, Buff, 5);
			bit_TVLData += 5;
			memcpy(PDOLData + *PDLen, Buff, len);
			*PDLen += len;
			break;
		case T_TRADE_COIN_CODE:   //n  5F2A
			Buff[0] = 0x01;
			Buff[1] = 0x56;
			QpbocTVLFCI.t_5F2A.TagValue = 0x5F2A;
			QpbocTVLFCI.t_5F2A.SizeOff = bit_TVLData;
			memcpy(QpbocTVLData + bit_TVLData, "\x5F\x2A\x02", 3);
			bit_TVLData += 3;
			memcpy(QpbocTVLData + bit_TVLData, Buff, 2);
			bit_TVLData += 2;
			if (len <= 2) {
				for (i = 0; i < len; i++)
					PDOLData[*PDLen + i] = Buff[2 - len + i];
			}
			else {
				memset(PDOLData + *PDLen, 0, len);
				memcpy(PDOLData + *PDLen, Buff, 2);
			}
			*PDLen += len;
			break;
		case 0x9F21:				//HHMMSS
			memcpy(Buff, (INT8U *)&SysTime.hours, 3);
			//				Buff[0] =0x10;
			//				Buff[1] =0x09;
			//				Buff[2] =0x20;
			//				QpbocTVLFCI.t_9A.TagValue =0x9A;
			//				QpbocTVLFCI.t_9A.SizeOff =bit_TVLData;
			//				memcpy(QpbocTVLData+bit_TVLData,"\x9A\x03",2);
			//				bit_TVLData +=2;
			//				memcpy(QpbocTVLData+bit_TVLData,Buff,3);
			//				bit_TVLData +=3;
			if (len <= 3) {
				for (i = 0; i < len; i++)
					PDOLData[*PDLen + i] = Buff[3 - len + i];
			}
			else {
				memset(PDOLData + *PDLen, 0, len);
				memcpy(PDOLData + *PDLen, Buff, 3);
			}
			*PDLen += len;
			break;
		case T_TRADE_TIME:		//n YYMMDD  9A
			memcpy(Buff, (INT8U *)&SysTime.year_l, 3);
			//				Buff[0] =0x10;
			//				Buff[1] =0x09;
			//				Buff[2] =0x20;
			QpbocTVLFCI.t_9A.TagValue = 0x9A;
			QpbocTVLFCI.t_9A.SizeOff = bit_TVLData;
			memcpy(QpbocTVLData + bit_TVLData, "\x9A\x03", 2);
			bit_TVLData += 2;
			memcpy(QpbocTVLData + bit_TVLData, Buff, 3);
			bit_TVLData += 3;
			if (len <= 3) {
				for (i = 0; i < len; i++)
					PDOLData[*PDLen + i] = Buff[3 - len + i];
			}
			else {
				memset(PDOLData + *PDLen, 0, len);
				memcpy(PDOLData + *PDLen, Buff, 3);
			}
			*PDLen += len;
			break;
		case T_TRADE_TYPE:			 //  n   9c
			Buff[0] = 0x00;
			QpbocTVLFCI.t_9C.TagValue = 0x9C;
			QpbocTVLFCI.t_9C.SizeOff = bit_TVLData;
			memcpy(QpbocTVLData + bit_TVLData, "\x9C\x01", 2);
			bit_TVLData += 2;
			memcpy(QpbocTVLData + bit_TVLData, Buff, 1);
			bit_TVLData += 1;
			if (len <= 1) {
				for (i = 0; i < len; i++)
					PDOLData[*PDLen + i] = Buff[1 - len + i];
			}
			else {
				memset(PDOLData + *PDLen, 0, len);
				memcpy(PDOLData + *PDLen, Buff, 1);
			}
			*PDLen += len;
			break;
		case T_FORECAST_DATA:		//b
			memcpy(PDOLData + *PDLen, RamData, len);
			*PDLen += len;
			break;
		default:
			memset(PDOLData + *PDLen, 0, len);
			*PDLen += len;
			break;
		}
	}

	return;
}

int QpbocPosActionAnalyse(void)
{
	BER_TVL TempTVL;

	get_BER_TVL(QpbocTVLFCI.t_9F10.SizeOff, &TempTVL);

	//	printf("密文版本=%d\r\n",TempTVL.V[2]);
	//	printf("TVR =%s\r\n",BCD2ASC(PosOfferData.AutenData.TVR,5));

	MSG_LOG("9F10[4]:");
	BCD_LOG(TempTVL.V, 4, 1);

	if (((TempTVL.V[4] >> 4) & 0x03) == 0) {
		return qPBOC_RESUL_AAC;			//4 交易拒绝
	}
	else if (((TempTVL.V[4] >> 4) & 0x03) == 1) {

		return qPBOC_RESUL_TC;			//4 交易批准
	}
	else if (((TempTVL.V[4] >> 4) & 0x03) == 2) {

		return qPBOC_RESUL_ARQC;//;	    //4 联机批准
	}
	else {
		return qPBOC_RESUL_RFU;
	}
}
int qPbocTradeAhead(IN INT32U InputMoney)
{
	PosOfferData.PDOL.DebitMoney = InputMoney;
	memcpy(TradeResult.TradeMoney, (INT8U *)&InputMoney, 4);
	if (PosOfferData.PDOL.DebitMoney == 0) {
		PosOfferData.PDOL.bPosAttr9F66[0] &= ~0x20;
		if ((PosOfferData.PDOL.bPosAttr9F66[0] & A1_OFFLINE) == A1_OFFLINE) {
			ERROR("ERROR-DebitMoney =0 And only OffLine\r\n");
			return qPBOC_SUCESS_ECU;
		}
	}
	else {
		PosOfferData.PDOL.bPosAttr9F66[1] &= ~A2_CVM;
	}
	MSG_LOG("Ahead Pos Trade Trade=%s\r\n", BCD2ASC(PosOfferData.PDOL.bPosAttr9F66, 4));
	return qPBOC_SUCESS_ECU;
}
int Pro_QpbocInt(void)
{
	bit_TVLData = 0;

	memset((INT8U *)&QpbocTVLFCI, 0, sizeof(BIT_TVL_TAG));

	//4  终端的应用AID
//	bzero(DevAID,sizeof(DevAID));--
//	strcat(DevAID,"A000000333");--
	memset(PosOfferData.AppSelect.bAID_9F06, 0, 16);
	//	memcpy(PosOfferData.AppSelect.bAID_9F06,);--
	memset(PosOfferData.AppSelect.aPPSEName, 0, sizeof(PosOfferData.AppSelect.aPPSEName));
	strcat((char *)PosOfferData.AppSelect.aPPSEName, "2PAY.SYS.DDF01");

	//4 终端属性
#if SWITCH_EC_CASH
	MSG_LOG("先电子现金\r\n");
	PosOfferData.PDOL.bPosAttr9F66[0] = A1_QPBOC | A1_OFFLINE;//A1_MSD|A1_QPBOC|A1_OFFLINE|A1_SIGN;

//	shuangmian_funtion = 1;
// 	if (shuangmian_funtion )
// 	{
// 		MSG_LOG("双免交易PDO_9F66\r\n");
// 		memcpy(PosOfferData.PDOL.bPosAttr9F66,"\x26\x80\x00\x00",4);	//银联技术员指导，说填这个
// 	}
// 	shuangmian_funtion = 0;


//	memcpy(PosOfferData.PDOL.bPosAttr9F66,"\x26\x00\x00\x00",4);

	//	*****ODA/电子现金********
// 	MSG_LOG("ODA电子现金交易---\r\n");
// 	PosOfferData.PDOL.bPosAttr9F66[0] =A1_QPBOC|A1_OFFLINE|A1_ODA;//A1_MSD|A1_QPBOC|A1_OFFLINE|A1_SIGN;
// 	PosOfferData.PDOL.bPosAttr9F66[1] =A2_ONLINEKEY;//0//A2_CVM;
#else
//	借贷记
	MSG_LOG("借贷记---\r\n");
	PosOfferData.PDOL.bPosAttr9F66[0] = A1_NPBOC | A1_QPBOC | A1_OFFLINE | A1_ODA;//A1_MSD|A1_QPBOC|A1_OFFLINE|A1_SIGN;
	PosOfferData.PDOL.bPosAttr9F66[1] = 0;//A1_MSD;//A2_CVM;
	PosOfferData.PDOL.bPosAttr9F66[2] = 0;
	PosOfferData.PDOL.bPosAttr9F66[3] = 0;//0x80
#endif

	MSG_LOG("9F66:");
	BCD_LOG(PosOfferData.PDOL.bPosAttr9F66, 4, 1);

	PosOfferData.PDOL.DebitMoney = 0;		//4 交易金额
	PosOfferData.PDOL.SecDebitMoney = 0;
	// 国家代码
	memcpy(PosOfferData.PDOL.nContryCode, "\x01\x56", 2);

	memset(PosOfferData.AutenData.bPosCapabilty, 0, 3);
	PosOfferData.AutenData.bPosCapabilty[1] = CA_NO_CVM;
	PosOfferData.AutenData.bPosCapabilty[2] = /*CA_DDA | */CA_SDA;

	//应用版本号
	memset(PosOfferData.ManageLimist.VerNum, 0, 2);
	//终端国家代码
	memset(PosOfferData.ManageLimist.PosContyCode, 0, 2);

	// CVM 结果
	memset(PosOfferData.CVM.CVMRstu, 0, 3);

	//终端类型
	PosOfferData.CVM.PosType = PT_ORG_SHOP | PT_NOMAN_OFFLINE;

	//终端附加信息
	memset(PosOfferData.AutenData.bAddPosCapabity, 0, 5);
	PosOfferData.AutenData.bAddPosCapabity[0] = PTA_CASH | PTA_DEBIT;

	return 0;
}
int  QpbocAppSelect(void)
{
	int ret, send_len, i, pos, j;
	INT8U Buff[254];
	INT8U AppListCnt = 0;
	INT32U len = 0;
	BER_TVL TempTVL, TempTVL1;
	INT8U objbuf[4];
	const unsigned char YL_AID_PREFIX[] = { 0xA0, 0x00, 0x00, 0x03, 0x33 };
	char tmpCardType = 0;	// 1.存量卡,2.新增卡,3.特定的移动设备卡

	const unsigned char E_CASHFIX[] = {0x01,0x01,0x01};
#ifdef _BankNameLimit
	char *p;
#endif
	//	INT32U 	bit_TVLDataTemp =0;

	GetDateTime();
	MSG_LOG("Syster =%s\r\n", BCD2ASC((unsigned char *)&SysTime, 7));
	bit_TVLData = 1;
	memset((INT8U *)&QpbocTVLFCI, 0, sizeof(BIT_TVL_TAG));
	memset(QpbocTVLData, 0, LEN_QBOCTVL_DATA);
	RepeatPo = 0;
	for (i = 0; i < LEN_REPEAT_TAG_SUM; i++) RepeatTag[i] = 0;

	send_len = 20;
	memcpy(Buff, "\x02\x00\xA4\x04\x00\x0E\x32\x50\x41\x59\x2E\x53\x59\x53\x2E\x44\x44\x46\x30\x31", send_len);//2PAY.SYS.DDF01
	Buff[0] = PCBSerial;
	PCBSerial ^= 1;
	memset(qpbocrevbuf, 0, 256);
	MSG_LOG("select snd =%s\r\n", BCD2ASC(Buff, send_len));
	ret = RfPassthrough(qpbocrevbuf, 2, Buff + 1, send_len - 1);
	MSG_LOG("select qpbocrevbuf =%s\r\n", BCD2ASC(qpbocrevbuf, ret));
	if (ret == 0) {
		return qPBOC_APPFILE_FAILE;
	}
	if (memcmp(qpbocrevbuf + ret - 2, "\x90\x00", 2) != 0)
	{
		return qPBOC_APPFILE_FAILE;
	}
	MSG_LOG("APUD =%d,%s\r\n", ret, BCD2ASC(qpbocrevbuf, ret));

	memcpy(QpbocTVLData + bit_TVLData, qpbocrevbuf + 1, ret - 3);
	get_TVL_Tag(0, qpbocrevbuf + 1, bit_TVLData, ret - 3);
	bit_TVLData += ret - 3;
	if ((QpbocTVLFCI.t_6F.SizeOff == 0) || (QpbocTVLFCI.t_84.SizeOff == 0) ||
		(QpbocTVLFCI.t_A5.SizeOff == 0) || (QpbocTVLFCI.t_BF0C.SizeOff == 0) ||
		(QpbocTVLFCI.t_61.SizeOff == 0) || (QpbocTVLFCI.t_4F.SizeOff == 0)) {
		ERROR("ERROR-:FCI Module lost data\r\n");
		return qPBOC_FCI_LOSTDATA;
	}
	AppListCnt = 0;
	if (Analytic_FCIMoudle(qpbocrevbuf + 1, ret - 3, &AppListCnt)) {
		return qPBOC_SELECT_NO_APP;
	}
	for (i = 0; i < AppListCnt; i++) {
		MSG_LOG("MSG:AID Length=%d,Data =%s\r\n", TempAIDTable[i].LenAID, BCD2ASC(TempAIDTable[i].DF_AID, TempAIDTable[i].LenAID));
		MSG_LOG("MSG:Tag Length=%d,Data =%s\r\n", TempAIDTable[i].LenTag, BCD2ASC(TempAIDTable[i].AppTag, TempAIDTable[i].LenTag));
		MSG_LOG("MSG:Tag Length=%d,Data =%s\r\n", TempAIDTable[i].LenPro, BCD2ASC(TempAIDTable[i].AppPrio, TempAIDTable[i].LenPro));
		if (memcmp(TempAIDTable[i].DF_AID, YL_AID_PREFIX, 5) != 0) //check if it's yinlian IC card   2017.06.29 pfquan
		{
			MSG_LOG("no  yinlian IC card\r\n");
			return qPBOC_SELECT_NO_APP;
		}
		else {
			break;
		}
	}
	pos = 0;
	for (j = 0; j < 16; j++) {
		for (i = 0; i < AppListCnt; i++) {
			if ((TempAIDTable[i].AppPrio[0] & 0x0F) == j) {
				if (TempAIDTable[i].LenAID != 0) {
					QpbocAppList[pos++] = TempAIDTable[i];
					TempAIDTable[i].LenAID = 0;

				}
			}
		}
	}
	for (i = 0; i < AppListCnt; i++) {
		clr_wdt();
		ValidAppNum = i;
		memcpy(Buff, "\x00\x00\xA4\x04\x00", 5);
		Buff[0] = PCBSerial;
		PCBSerial ^= 1;
		Buff[5] = QpbocAppList[i].LenAID;
		memcpy(Buff + 6, QpbocAppList[i].DF_AID, QpbocAppList[i].LenAID);
		send_len = 6 + QpbocAppList[i].LenAID;

				//add by zhgfan 2018-1-6
		if(memcmp(TempAIDTable[i].DF_AID+5, E_CASHFIX, 3)==0){ //check if it's a pre-paid card  2018.01.5 zhgfan		
			emv_set_card_type(PREPAID_CARD);//储值卡
			MSG_LOG("储蓄卡\r\n");
		}
		else{
			emv_set_card_type(CREDIT_CARD);//信用卡
				MSG_LOG("信用卡\r\n");
		}
		//	


		MSG_LOG("#select snd =%s\r\n", BCD2ASC(Buff, send_len));
		ret = RfPassthrough(qpbocrevbuf, 2, Buff + 1, send_len - 1);
		if (ret == 0) continue;

		MSG_LOG("#select REV =%s\r\n", BCD2ASC(qpbocrevbuf, ret));

		if (memcmp(qpbocrevbuf + ret - 2, "\x90\x00", 2) != 0) continue;

		memcpy(QpbocTVLData + bit_TVLData, qpbocrevbuf + 1, ret - 3);
		get_TVL_Tag(0, qpbocrevbuf + 1, bit_TVLData, ret - 3);
		bit_TVLData += ret - 3;
		if ((QpbocTVLFCI.t_6F.SizeOff == 0) || (QpbocTVLFCI.t_84.SizeOff == 0) ||
			(QpbocTVLFCI.t_A5.SizeOff == 0)) {
			ERROR("ERROR-:FCI Module lost data\r\n");
			continue;
		}

#ifdef _BankNameLimit
		memset(Buff, 0, sizeof(Buff));
		if (QpbocTVLFCI.t_9F12.SizeOff != 0) {//判断厂家信息
			get_BER_TVL(QpbocTVLFCI.t_9F12.SizeOff, &TempTVL);
			memcpy(Buff, TempTVL.V, TempTVL.L);
			p = strstr((char*)Buff, "ICBC");
			if (p == NULL) {//不是工行卡不能刷
				return qPBOC_CONTRY_OUTTRADE;
			}
		}
#endif

		if (QpbocTVLFCI.t_9F38.SizeOff == 0) {
			ERROR("ERROR-:PDOL 9F38 not Find\r\n");
			return qPBOC_9F38_NOEXIT;
		}
		get_BER_TVL(QpbocTVLFCI.t_9F38.SizeOff, &TempTVL);

		memcpy(objbuf, "\x9F\x66", 2);
		if (Find_CharData(TempTVL.V, TempTVL.L, objbuf, 2) == NULL) {
			ERROR("ERROR-:PDOL  not Find 9F66\r\n");
			return qPBOC_9F66_NOEXIT;
		}
		memset(Buff, 0, sizeof(Buff));
		memset(PosOfferData.AutenData.TSI, 0, 2);
		memset(PosOfferData.AutenData.TVR, 0, 5);


		if (QpbocTVLFCI.t_DF61.SizeOff != 0) {
			get_BER_TVL(QpbocTVLFCI.t_DF61.SizeOff, &TempTVL);
			MSG_LOG("t_DF61-0:%04X::", TempTVL.L);
			BCD_LOG(TempTVL.V, TempTVL.L, 1);
			if ((TempTVL.V[0] & 0x40) == 0) {//DF61的第1字节的第7位为1，则说明该卡为新增支持联机ODA 的卡片.
				MSG_LOG("有DF61，卡片不支持ODA\r\n");
				tmpCardType = 1;
			}
			else {	// 2. 卡片返回 FCI 信息， DF61 第 1 字节第 7 位置为 1
				MSG_LOG("有DF61，卡片支持ODA\r\n");
				tmpCardType = 2;
			}
		}
		else {	//没有DF61不支持ODA,0.2.1 存量卡
			MSG_LOG("没有DF61，卡片不支持ODA\r\n");
			tmpCardType = 1;
		}


		memcpy(Buff, "\x00\x80\xCA\x9F\x51\x00", 5);
		Buff[0] = PCBSerial;
		PCBSerial ^= 1;

		send_len = 6;

		MSG_LOG("#select snd =%s\r\n", BCD2ASC(Buff, send_len));
		ret = RfPassthrough(qpbocrevbuf, 2, Buff + 1, send_len - 1);
		if (ret == 0) continue;

		MSG_LOG("#select 9F51 =%s\r\n", BCD2ASC(qpbocrevbuf, ret));

		if (memcmp(qpbocrevbuf + ret - 2, "\x90\x00", 2) != 0) continue;

		memcpy(QpbocTVLData + bit_TVLData, qpbocrevbuf + 1, ret - 3);
		get_TVL_Tag(0, qpbocrevbuf + 1, bit_TVLData, ret - 3);
		bit_TVLData += ret - 3;


		get_BER_TVL(QpbocTVLFCI.t_9F51.SizeOff, &TempTVL);
		MSG_LOG("9f51:");
		BCD_LOG(TempTVL.V, TempTVL.L, 1);
		if (memcmp(TempTVL.V, "\x01\x56", 2) != 0)	//外币
		{
			MSG_LOG("非0156外币\r\n");
			//return qPBOC_CARD_forbid;
		}

#if SWITCH_EC_CASH
		ret = getqPbocMoney();//如果没返回9F5D，就用指令取余额
		MSG_LOG("取电子现金余额:%d\r\n", ret);
		if (ret < 0)
		{

			MSG_LOG("取不到9F5D 只能走双免交易PDO_9F66\r\n");
			memcpy(PosOfferData.PDOL.bPosAttr9F66, "\x20\x80\x00\x00", 4);	//银联技术员指导，说填这个

			online_funtion = 1;
			shuangmian = 1;
		}
		if (ret < s_sum1)
#endif
		{
			if (shuangmian != 0) {
				gMCardCand = CARDSTYLE_PBOC;
				MSG_LOG("1双免交易PDO_9F66\r\n");
				memcpy(PosOfferData.PDOL.bPosAttr9F66, "\x26\x80\x00\x00", 4);	//银联技术员指导，说填这个

				online_funtion = 1;
			}
			else {
				MSG_LOG("走脱机数据认证过程PDO_9F66\r\n");
				memcpy(PosOfferData.PDOL.bPosAttr9F66, "\x00\x00\x00\x00", 4);	//银联技术员指导，说填这个
				//shuangmian = 0;
				//add hbg
				switch (tmpCardType)
				{
				case 1:
					// 3. 终端将 tag 9F66 第 1 字节第 7 位置 1（非接 PBOC/非接 UICS），进入非接标准借贷记流程并执行脱机数据认证
				//	PosOfferData.PDOL.bPosAttr9F66[0] |= 0x40;
					PosOfferData.PDOL.bPosAttr9F66[0] = 0x61;
					PosOfferData.PDOL.bPosAttr9F66[1] = 0x00;
					PosOfferData.PDOL.bPosAttr9F66[2] = 0;
					PosOfferData.PDOL.bPosAttr9F66[3] = 0x80;
						
					break;
				case 2:
					// 3. 终端将 tag 9F66 第 1 字节第 7 位置 0（qPBOC/qUICS）、第 1 字节第 1 位置 1(联机ODA)、 第 1 字节第 6 位置 1、 第 2 字节第 8 位置 1（要求联机）， 进入非接快速借贷记流程
				//	PosOfferData.PDOL.bPosAttr9F66[0] &= 0xDF;
// 					PosOfferData.PDOL.bPosAttr9F66[0] = 0x21;
// 					PosOfferData.PDOL.bPosAttr9F66[1] = 0x80;
// 					PosOfferData.PDOL.bPosAttr9F66[2] = 0;
// 					PosOfferData.PDOL.bPosAttr9F66[3] = 0x80;
					if(CREDIT_CARD == emv_get_card_type())
					{
						// 				PosOfferData.PDOL.bPosAttr9F66[0] = 0x21;
						// 				PosOfferData.PDOL.bPosAttr9F66[1] |= 0x80;
						PosOfferData.PDOL.bPosAttr9F66[0] = 0x21;
						PosOfferData.PDOL.bPosAttr9F66[1] = 0x80;
						PosOfferData.PDOL.bPosAttr9F66[2] = 0;
						PosOfferData.PDOL.bPosAttr9F66[3] = 0x80;
						
					}
					else
					{
						
						// 				PosOfferData.PDOL.bPosAttr9F66[0] = 0x21;
						// 				PosOfferData.PDOL.bPosAttr9F66[1] |= 0x80;
						// 				PosOfferData.PDOL.bPosAttr9F66[3] |= 0x80;
						// 				PosOfferData.PDOL.bPosAttr9F66[0] |= 0x06;
						PosOfferData.PDOL.bPosAttr9F66[0] = 0x27;
						PosOfferData.PDOL.bPosAttr9F66[1] = 0x80;
						PosOfferData.PDOL.bPosAttr9F66[2] = 0;
						PosOfferData.PDOL.bPosAttr9F66[3] = 0x80;
						
					}
					break;
				default:
						PosOfferData.PDOL.bPosAttr9F66[0] |= 0x40;
					MSG_LOG("未知卡类型:%d\r\n", tmpCardType);
					return qPBOC_DEFAULT;
					//break;
		}
			}
		}


    	MSG_LOG("shuangmian=%d == g_supportQR=%x\r\n",shuangmian,g_supportQR);
			if(CREDIT_CARD != emv_get_card_type()&& shuangmian == 0 &&(g_supportQR&car_) == 0) 
			{
				return qPBOC_CARD_forbid_oda;
			}
		/*
				memcpy(Buff,"\x00\x80\xCA\xDF\x71\x00",5);
				Buff[0] =PCBSerial;
				PCBSerial ^=1;

				send_len = 6;

				MSG_LOG("#select snd =%s\r\n",BCD2ASC(Buff,send_len));
				ret=RfPassthrough(qpbocrevbuf,2,Buff+1,send_len-1);
				if(ret == 0) continue;

				MSG_LOG("#select DF71 =%s\r\n",BCD2ASC(qpbocrevbuf,ret));

				if(memcmp(qpbocrevbuf+ret-2, "\x90\x00", 2) != 0) continue;

				memcpy(QpbocTVLData+bit_TVLData,qpbocrevbuf+1,ret-3);
				get_TVL_Tag(0,qpbocrevbuf+1,bit_TVLData,ret-3);
				bit_TVLData +=ret-3;


				get_BER_TVL(QpbocTVLFCI.t_DF71.SizeOff,&TempTVL);
				MSG_LOG("t_DF71:");
				BCD_LOG(TempTVL.V,TempTVL.L,1);
		*/

		memcpy(Buff, "\x00\x80\xA8\x00\x00\x00", 6);
		Buff[0] = PCBSerial;
		PCBSerial ^= 1;
		memcpy(Buff + 6, "\x83\x00", 2);
		pos = 2;
		memset(qpbocrevbuf, 0, sizeof(qpbocrevbuf));
		get_BER_TVL(QpbocTVLFCI.t_9F38.SizeOff, &TempTVL);
		DOL_DataFill(qpbocrevbuf, &len, &TempTVL);
		if (len < 0x80) {
			Buff[5] = len + 2;
			Buff[7] = len;
			memcpy(Buff + 8, qpbocrevbuf, len);
		}
		else {
			Buff[5] = len + 3;
			Buff[7] = 0x81;
			Buff[8] = len;
			memcpy(Buff + 9, qpbocrevbuf, len);
			len += 1;
		}
		// CPU_APDU sndDATA=0280A800002383212800000000000000000100000000000001560000000000015610100100AE9BF3C8,0
		// CPU_APDU sndDATA=0280A800002383212800000000000000000100000000000001560000000000015610100100AE9BF5E5,0
		// CPU_APDU sndDATA=0280A800002383212800000000000000000100000000000001560000000000015611092000CD9FE7AC,0 $CPU_APDU:
		send_len = len + 8;
		MSG_LOG("CPU_APDU sndDATA=%s,%d\r\n", BCD2ASC(Buff, send_len), i);
		ret = RfPassthrough(qpbocrevbuf, 2, Buff + 1, send_len - 1);
		clr_wdt();
		if (ret == 0) {
			ERROR("ERROR:CPU_APDU is %d\r\n\r\n\r\n\r\n", ret);
			return qPBOC_APDU_CMD;
		}
		MSG_LOG("$CPU_APDU:\r\n");
		MSG_LOG("\tLength=%d,DATA=%s,\r\n", ret, BCD2ASC(qpbocrevbuf, ret));
		if (memcmp(qpbocrevbuf + ret - 2, "\x90\x00", 2) != 0)
		{
			ERROR("ERROR-PDOL:stuApduRet.SW =%02X%02X\r\n", qpbocrevbuf[ret - 2], qpbocrevbuf[ret - 1]);
			if (memcmp(qpbocrevbuf + ret - 2, "\x69\x85", 2) == 0)
				continue;
			else
				return qPBOC_APDU_CMD;
		}
		if ((qpbocrevbuf[0 + 1] != 0x77) && (qpbocrevbuf[0 + 1] != 0x80)) {
			return qPBOC_DATA_NO70;
		}


		pos = qpbocrevbuf[1];
		if (qpbocrevbuf[1] == 0x80) {	//借贷记返回的是80模板,应用交互特征AIP  和 应用文件定位器AFL
			j = qpbocrevbuf[2];
			memcpy(qpbocrevbuf + 1, "\x82\x02", 2);//加入T，L
			memmove(qpbocrevbuf + 7, qpbocrevbuf + 5, ret);

			qpbocrevbuf[5] = 0x94;	//T
			qpbocrevbuf[6] = j - 2;//L

			ret += 2;	//多加了两个字节

		}

		MSG_LOG("::::::\tLength=%d,DATA=%s,\r\n", ret, BCD2ASC(qpbocrevbuf, ret));

		memcpy(QpbocTVLData + bit_TVLData, qpbocrevbuf + 1, ret - 3);

		if ((j = get_TVL_Tag(1, qpbocrevbuf + 1, bit_TVLData, ret - 3)) != 0) {
			ERROR(" *** GPO Respone \r\n");
			return j;
		}

		bit_TVLData += ret - 3;
		break;
	}

	if (i == AppListCnt) {
		return qPBOC_NOOK_PATH;
	}
	get_BER_TVL(QpbocTVLFCI.t_9F66.SizeOff, &TempTVL);
	get_BER_TVL(QpbocTVLFCI.t_82.SizeOff, &TempTVL1);
	MSDOrQpboc = 0;


	if (pos == 0x80) {
		return qPBOC_RESUL_TC;//可退出
	}


	if (QpbocTVLFCI.t_9F10.SizeOff == 0)
		return qPBOC_NOFIND_9F10;

	get_BER_TVL(QpbocTVLFCI.t_9F10.SizeOff, &TempTVL);
	MSG_LOG("t_9F10-5:%02X\r\n", TempTVL.V[4]);
	MSG_LOG("t_9F10-10:%02X\r\n", TempTVL.V[9]);
	if (((TempTVL.V[4] >> 4) & 0x03) == 0) {
		if ((QpbocTVLFCI.t_82.SizeOff == 0) ||
			(QpbocTVLFCI.t_9F36.SizeOff == 0) ||
			(QpbocTVLFCI.t_57.SizeOff == 0) ||
			(QpbocTVLFCI.t_9F10.SizeOff == 0) ||
			(QpbocTVLFCI.t_9F26.SizeOff == 0)) {
			return qPBOC_AACARQC_NOM;
		}

		if (TempTVL.V[9] == 0x07)
		{
			MSG_LOG("NFC手机卡\r\n");
			return qPBOC_RESUL_TC;
		}

		debugstring("-------\r\n");
		return qPBOC_RESUL_AAC;
	}
	else if (((TempTVL.V[4] >> 4) & 0x03) == 1) {
		if (MSDOrQpboc == 0) {
			if ((QpbocTVLFCI.t_82.SizeOff == 0) ||
				(QpbocTVLFCI.t_94.SizeOff == 0) ||
				(QpbocTVLFCI.t_9F36.SizeOff == 0) ||
				(QpbocTVLFCI.t_9F26.SizeOff == 0)) {
				ERROR("ERROR-:GPO Respne  not Find AIP,AFL,ATC,AAC,IAD\r\n");
				return qPBOC_TC_NOM;
			}
		}
		else {
			if ((QpbocTVLFCI.t_82.SizeOff == 0) ||
				(QpbocTVLFCI.t_57.SizeOff == 0) ||
				(QpbocTVLFCI.t_9F36.SizeOff == 0) ||
				(QpbocTVLFCI.t_9F26.SizeOff == 0) ||
				(QpbocTVLFCI.t_5F20.SizeOff == 0)) {
				ERROR("ERROR-:GPO Respne  not Find AIP,AFL,ATC,AAC,IAD\r\n");
				return qPBOC_TC_NOM;
			}
		}
		return qPBOC_RESUL_TC;
	}
	else if (((TempTVL.V[4] >> 4) & 0x03) == 2) {
		if ((QpbocTVLFCI.t_82.SizeOff == 0) ||
			(QpbocTVLFCI.t_9F36.SizeOff == 0) ||
			(QpbocTVLFCI.t_57.SizeOff == 0) ||
			(QpbocTVLFCI.t_9F10.SizeOff == 0) ||
			(QpbocTVLFCI.t_9F26.SizeOff == 0)) {

			return qPBOC_AACARQC_NOM;
		}
		return qPBOC_RESUL_ARQC;//; 这里应该是只允许联机交易
	}
	else {
		return qPBOC_RESUL_RFU;
	}
}

//判断公钥是否在设备中存在,返回值：大于0=找了版本号，并返回位置序号，否则存在
int qPBOC_KeyVerExist(unsigned char pKeyVer);

static int QpbocReadRecordData(void)
{
	INT32U j, ii = 0;
	INT8U 	Buff[100], temp, AFL;
	BER_TVL	TempTVL;
#ifdef _CheckPublicVer_
	BER_TVL	TVL2;
#endif
	int iRe, send_len = 0, re;
	INT8U 	AutenDataCnt = 0;
	//	unsigned char i;
	INT32U ErrFlag = 0;

#ifdef _CheckPublicVer_
	TVL2.V[0] = 0xFF;//没有判断公钥版时，判断了就不是0xFF了。
#endif

	SDADataALen = 0;
	get_BER_TVL(QpbocTVLFCI.t_94.SizeOff, &TempTVL);
	if(QpbocTVLFCI.t_94.SizeOff==0)
	{
		MSG_LOG("没有94域\r\n");
		 return qPBOC_AFL_SFI_ERROR;;
	}
	//	printf("TempTVL.L =%d\r\n",TempTVL.L);
	//	printf("TempTVL.V=%s\r\n",BCD2ASC(TempTVL.V,TempTVL.L));
	for (j = 0; j < (TempTVL.L / 4); j++) {
		MSG_LOG("TempTVL.V=%s\r\n", BCD2ASC(TempTVL.V + j * 4, 4));
		if (((TempTVL.V[j * 4] >> 3) < 01) || ((TempTVL.V[j * 4] >> 3) > 30)) {
			ERROR("ERROR-:AFL SFI(%d) is EROOR\r\n", TempTVL.V[j * 4]);
			ErrFlag = qPBOC_AFL_SFI_ERROR;
			goto errorEndflag;// qPBOC_AFL_SFI_ERROR;
		}
		if ((TempTVL.V[j * 4 + 1] == 0) || (TempTVL.V[j * 4 + 1] > TempTVL.V[j * 4 + 2]) ||
			(TempTVL.V[j * 4 + 3] > (TempTVL.V[j * 4 + 2] - TempTVL.V[j * 4 + 1] + 1))) {
			ERROR("ERROR-:AFL S:(%d),E:(%d) is EROOR\r\n", TempTVL.V[j * 4 + 1], TempTVL.V[j * 4 + 2]);
			ErrFlag = qPBOC_AFL_SFI_ERROR;
			goto errorEndflag;// qPBOC_AFL_SFI_ERROR;
		}
		AutenDataCnt = TempTVL.V[j * 4 + 3];
		SDADataValid = 1;
		for (ii = TempTVL.V[j * 4 + 1]; ii <= TempTVL.V[j * 4 + 2]; ii++) {

			MSG_LOG("AFL=%02X,Saddr=%02X\r\n", TempTVL.V[j * 4], ii);

			memcpy(Buff, "\x00\x00\xA4\x04\x00", 5);
			Buff[0] = PCBSerial;
			PCBSerial ^= 1;
			Buff[1] = 0x00;
			Buff[2] = 0xB2;
			Buff[3] = ii;
			Buff[4] = (TempTVL.V[j * 4] & 0xF8) | 0x04;
			Buff[5] = 0;
			send_len = 6;
			iRe = RfPassthrough(qpbocrevbuf, 2, Buff + 1, send_len - 1);
			MSG_LOG("\tsend DATA=%s\r\n", BCD2ASC(Buff, send_len));
			if (iRe == 0) {
				ERROR("ERROR:CPU_APDU is %d\r\n\r\n\r\n\r\n", iRe);
				ErrFlag = qPBOC_AFL_SFI_ERROR;
				goto errorEndflag;// qPBOC_APDU_CMD;
			}
			MSG_LOG("$QpbocReadRecordData:CPU_APDU:\r\n");
			MSG_LOG("\tLength=%d,DATA=%s\r\n\r\n\r\n\r\n", iRe, BCD2ASC(qpbocrevbuf, iRe));
			if (memcmp(qpbocrevbuf + iRe - 2, "\x90\x00", 2) != 0) {
				ERROR("ERROR-RECORD:stuApduRet.SW =%02X%02X\r\n", qpbocrevbuf[iRe - 2], qpbocrevbuf[iRe - 1]);

				ErrFlag = qPBOC_AFL_SFI_ERROR;
				goto errorEndflag;// qPBOC_APDU_CMD;
			}

			AFL = (TempTVL.V[j * 4] >> 3) & 0x1F;

			if ((TempTVL.V[j * 4 + 3] != 0) && (AutenDataCnt != 0)) {
				if (qpbocrevbuf[1] != 0x70) {
					ErrFlag = qPBOC_AFL_SFI_ERROR;
					goto errorEndflag;// qPBOC_DATA_SIGAN_NO70;
				}
				if ((AFL >= 1) && (AFL <= 10))
				{
					if ((qpbocrevbuf[2] & 0x80) == 0x80) {
						temp = qpbocrevbuf[2] & 0x7F;
						memcpy(SDADataAuten + SDADataALen, qpbocrevbuf + 1 + 1 + 1 + temp, iRe - 3 - 1 - 1 - temp);
						SDADataALen += iRe - 3 - 1 - 1 - temp;
					}
					else {
						memcpy(SDADataAuten + SDADataALen, qpbocrevbuf + 1 + 1 + 1, iRe - 3 - 1 - 1);
						SDADataALen += iRe - 3 - 1 - 1;
					}
					MSG_LOG("SDADataAuten:");
					BCD_LOG(SDADataAuten, SDADataALen, 1);
				}
				else if ((AFL >= 11) && (AFL <= 30)) {
					memcpy(SDADataAuten + SDADataALen, qpbocrevbuf + 1, iRe - 3);
					SDADataALen += iRe - 3;

				}
				else {
					SDADataValid = 0;
				}
				AutenDataCnt -= 1;
			}
			memcpy(QpbocTVLData + bit_TVLData, qpbocrevbuf + 1, iRe - 3);
			if ((re = get_TVL_Tag(1, qpbocrevbuf + 1, bit_TVLData, iRe - 3)) != 0) {
				ERROR(" *** Read Record \r\n");
				ErrFlag = re;
				goto errorEndflag;// re;
			}
			bit_TVLData += iRe - 3;
			MSG_LOG("bit_TVLData =%d\r\n", bit_TVLData);

#ifdef _CheckPublicVer_
			if (TVL2.V[0] == 0xFF) {//没有判断，需要判断是否有公钥
				MSG_LOG("QpbocTvlFci.t_8F:%04X,%04X", QpbocTVLFCI.t_8F.TagValue, QpbocTVLFCI.t_8F.SizeOff);
				if (QpbocTVLFCI.t_8F.SizeOff != 0) {//读出了公钥版本
					get_BER_TVL(QpbocTVLFCI.t_8F.SizeOff, &TVL2);
					MSG_LOG("QpbocTVLFCI.t_8FVer =%s\r\n", BCD2ASC(TVL2.V, TVL2.L));
					re = qPBOC_KeyVerExist(TVL2.V[0]);
					if ((re < 0) || (re > 120)) {//没有公钥,返回, 不执行消费
						return qPBOC_CAPKEY_NOEXIT;
					}
				}
			}
#endif
		}
	}
errorEndflag:
	//	MSG_LOG("errorEndflag j=%d ii=%d tL=%d tV=%d\r\n",j, ii, ((TempTVL.L/4)-1), TempTVL.V[j*4+2]);
	if ((j == (TempTVL.L / 4) - 1) && (ii == TempTVL.V[j * 4 + 2])) {//最后一条指令没有成功，需要请重刷，并在开始时判断是不是已经扣钱成功了。
		return qPBOC_QpboC_tryAgain;			//2014.4.29
	}
	return ErrFlag;		//2014.4.29
}

//----------2014.3.18-------
int getqPbocMoney(void)
{
	unsigned char Buff[64];
	unsigned char gqmBuff[64];
	unsigned int ret, sLen;

	memset(gqmBuff, 0, sizeof(gqmBuff));
	memcpy(Buff + 1, "\x80\xCA\x9F\x79\x00", 5);
	Buff[0] = PCBSerial;
	PCBSerial ^= 1;

	sLen = 6;
	MSG_LOG("getqPbocMoney snd =%s\r\n", BCD2ASC(Buff, sLen));
	ret = RfPassthrough(gqmBuff, 2, Buff + 1, sLen - 1);
	MSG_LOG("getqPbocMoney b =%s\r\n", BCD2ASC(gqmBuff, ret));
	if (ret == 0) {
		return -1;
	}
	if (memcmp(gqmBuff + ret - 2, "\x90\x00", 2) != 0)
	{
		return -2;
	}
	//	getqPbocMoney snd =0380CA9F7900
	//	getqPbocMoney b =039F79060000000037809000
	memset(Buff, 0, 40);
	BCD2Ascii(gqmBuff + 4, Buff, gqmBuff[3]);
	ret = __atol((char *)Buff);
	//	debugstring("qpboc Money:");
	//	debugdata((unsigned char*)&ret, 4, 1);
	return ret;
}

static int GenerateAc() {
	int ret, send_len, i, j;
	INT8U Buff[254];
	//	INT8U AppListCnt = 0;
	INT32U len = 0;
	BER_TVL TempTVL;//, TempTVL1;
//	INT8U objbuf[4];
	//const unsigned char YL_AID_PREFIX[] = { 0xA0, 0x00, 0x00, 0x03, 0x33 };
//	char tmpCardType = 0;	// 1.存量卡,2.新增卡,3.特定的移动设备卡
	int tmpI = 0;

	memcpy(Buff, "\x00\x80\xAE\x80\x00\x00", 6);
	Buff[0] = PCBSerial;
	PCBSerial ^= 1;
	//memcpy(Buff + 6, "\x83\x00", 2);
	memset(qpbocrevbuf, 0, sizeof(qpbocrevbuf));
	get_BER_TVL(QpbocTVLFCI.t_8C.SizeOff, &TempTVL);

		 MSG_LOG("do %s\r\n",__FUNCTION__);
	if(QpbocTVLFCI.t_8C.SizeOff==0)
	{
		MSG_LOG("没有8c域\r\n");
	 return qPBOC_RESUL_ARQC;;
	}
	 BCD_LOG((unsigned char * )&TempTVL,TempTVL.L,1);
	 	MSG_LOG("t_8C: %s\r\n",BCD2ASC(TempTVL.V,TempTVL.L));
	DOL_DataFill(Buff + 6, &len, &TempTVL);
	Buff[5] = len;
	// CPU_APDU sndDATA=0280A800002383212800000000000000000100000000000001560000000000015610100100AE9BF3C8,0
	// CPU_APDU sndDATA=0280A800002383212800000000000000000100000000000001560000000000015610100100AE9BF5E5,0
	// CPU_APDU sndDATA=0280A800002383212800000000000000000100000000000001560000000000015611092000CD9FE7AC,0 $CPU_APDU:
	send_len = len + 6;
	MSG_LOG("CPU_APDU sndDATA=%s,%d\r\n", BCD2ASC(Buff, send_len), send_len);
	ret = RfPassthrough(qpbocrevbuf, 2, Buff + 1, send_len - 1);
	clr_wdt();
	if (ret == 0) {
		ERROR("ERROR:CPU_APDU is %d\r\n\r\n\r\n\r\n", ret);
		return qPBOC_APDU_CMD;
	}
	MSG_LOG("$CPU_APDU:\r\n");
	MSG_LOG("\tLength=%d,DATA=%s,\r\n", ret, BCD2ASC(qpbocrevbuf, ret));
	if (memcmp(qpbocrevbuf + ret - 2, "\x90\x00", 2) != 0)
	{
		ERROR("ERROR-PDOL:stuApduRet.SW =%02X%02X\r\n", qpbocrevbuf[ret - 2], qpbocrevbuf[ret - 1]);

		return qPBOC_APDU_CMD;
	}
	if ((qpbocrevbuf[0 + 1] != 0x77) && (qpbocrevbuf[0 + 1] != 0x80)) {
		return qPBOC_DATA_NO70;
	}

	//pos = qpbocrevbuf[1];
	if (qpbocrevbuf[1] == 0x80) {	//借贷记返回的是80模板,应用交互特征AIP  和 应用文件定位器AFL
		j = qpbocrevbuf[2] + 3;
		tmpI = 0;
		i = 3;
		memcpy(QpbocTVLData + bit_TVLData + tmpI, "\x9F\x27\x01", 3);//加入T，L
		tmpI += 3;
		memcpy(QpbocTVLData + bit_TVLData + tmpI, qpbocrevbuf + i, 1);
		tmpI += 1;
		i += 1;
		memcpy(QpbocTVLData + bit_TVLData + tmpI, "\x9F\x36\x02", 3);//加入T，L
		tmpI += 3;
		memcpy(QpbocTVLData + bit_TVLData + tmpI, qpbocrevbuf + i, 2);
		tmpI += 2;
		i += 2;
		memcpy(QpbocTVLData + bit_TVLData + tmpI, "\x9F\x26\x08", 3);//加入T，L
		tmpI += 3;
		memcpy(QpbocTVLData + bit_TVLData + tmpI, qpbocrevbuf + i, 8);
		tmpI += 8;
		i += 8;
		if (j > i) {
			memcpy(QpbocTVLData + bit_TVLData + tmpI, "\x9F\x10", 2);//加入T，L
			tmpI += 2;
			QpbocTVLData[bit_TVLData + tmpI] = j - i;
			++tmpI;
			memcpy(QpbocTVLData + bit_TVLData + tmpI, qpbocrevbuf + i, j - i);
			tmpI += j - i;
			//i += j - i;
		}

		ret += 2;	//多加了两个字节
	}
	else {
		memcpy(QpbocTVLData + bit_TVLData, qpbocrevbuf + 1, ret - 3);
	}

	if ((j = get_TVL_Tag(0, QpbocTVLData + bit_TVLData, bit_TVLData, ret - 3)) != 0) {
		ERROR(" *** GAC Respone \r\n");
		return j;
	}

	bit_TVLData += ret - 3;
	// 5. 若脱机数据认证成功，则认定卡片为真卡，终端判断卡片是否按要求返回了 ARQC， 若未返回 ARQC， 则拒绝交易
	get_BER_TVL(QpbocTVLFCI.t_9F27.SizeOff, &TempTVL);
	MSG_LOG("t_9F27-5:%04X,%02X\r\n", TempTVL.T, TempTVL.V[0]);

	if (TempTVL.V[0] == 0x80) {
		//return qPBOC_RESUL_ARQC;
	}
	else {
		if (shuangmian != 0 && TempTVL.V[0] == 0x40) {
			return qPBOC_RESUL_TC;
		}
		else {
			return qPBOC_RESUL_AAC;
		}
	}
	// 6. 当确认卡片已按要求返回 ARQC 后，终端判断卡片 ATC 是否大于 0， 即卡片是否已做过交易， 若 ATC 为 0， 则拒绝交易
	get_BER_TVL(QpbocTVLFCI.t_9F36.SizeOff, &TempTVL);
//	MSG_LOG("t_9F36-5:%04X,", TempTVL.T, TempTVL.V[0]);
	BCD_LOG(TempTVL.V, TempTVL.L, 1);
	tmpI = GET_INT16(TempTVL.V);
	if (tmpI > 0) {
		return qPBOC_RESUL_ARQC;
	}
	else {
		return qPBOC_RESUL_AAC;
	}
	// 7. 将卡号与黑名单进行比对， 若卡号在黑名单中，则拒绝交易
	// 8. 终端可根据行业商户要求，进行其他检查，若不通过，则拒绝交易
	// 9. 在通过各项检测后，交易成功，终端对联机交易数据以及相关交易要素进行留存
}


int qPbocTradeProc(IN INT32U InputMoney, OUT INT32U *Len, OUT INT8U *pOutMsg)
{
	int ret;//, i = 0;
	//	INT32U len=0;
//	INT8U Buffer[256];
	BER_TVL TempTVL;

	if (cardSound != 0xaa) {//请重刷时不清除 //----------2014.3.18-------
		memset((INT8U *)&TradeResult, 0, sizeof(QPBOC_RESULT_TYPE));
	}
#ifdef _debug_
	MSG_LOG("DebitMoney:%d\r\n", InputMoney);
#endif
	PosOfferData.PDOL.DebitMoney = InputMoney;
	memcpy(TradeResult.TradeMoney, (INT8U *)&InputMoney, 4);

	// 	if((ret=GetTypeAUID(Buffer)) == 0){
	// 		TradeResult.TradeType =qPBOC_TRADE_ABORT;
	// 		return qPBOC_NO_CARD;
	// 	}

	//	miniDispstr(6, 0, "Q6", 0);

	//	memcpy(Buffer,"\x02\x00\xA4\x04\x00\x0E\x32\x50\x41\x59\x2E\x53\x59\x53\x2E\x44\x44\x46\x30\x31",20);
	// 	memset(qpbocrevbuf,0,256);
	// 	ret=RfPassthrough(qpbocrevbuf,2,Buffer,20);
	// 	if(ret == 0)
	// 		return 0;
	// 	//   6F30840E325041592E5359532E4444463031A51EBF0C1B61194F08A000000333010106500A50424F432044454249548701019000
	// 	//02 6F 30 84 0E 32 50 41 59 2E 53 59 53 2E 44 44 46 30 31 A5 1E BF 0C 1B 61 19 4F 08 A0 00 00 03 33 01 01 06 50 0A 50 42 4F 43 20 44 45 42 49 54 87 01 01 90 00
	// 	//02 6F 30 84 0E 32 50 41 59 2E 53 59 53 2E 44 44 46 30 31 A5 1E BF 0C 1B 61 19 4F 08 A0 00 00 03 33 01 01 06 50 0A 
	// 
	// 	debugstring("selectFile:");
	// 	debugdata(qpbocrevbuf, ret, 1);

	//	miniDispstr(6, 0, "Q7", 0);

	if ((NewPriceTab.rate[cardMessage.card_catalog - 0x40] == 104) || (NewPriceTab.rate[cardMessage.card_catalog - 0x40] == 204)) {

		audio(Audio_FLING_MONEY);	//请投币
		MessageBox(1, "此卡禁止刷卡!");
		led_on(LED_RED);
		restore_flag = 3;
		end_card();

		return ST_OK;
	}
	ret = QpbocAppSelect();

	MSG_LOG("QpbocAppSelect END =%d\r\n", ret);

	MSG_LOG("等会传数据\r\n");
#ifndef NO_GJ_SERVISE
	SetTcpPacketTTLTime(RECORD_TTL);
#endif



	if ((ret != qPBOC_RESUL_TC) && (ret != qPBOC_RESUL_ARQC) &(ret != qPBOC_RESUL_AAC)) {
		TradeResult.TradeType = qPBOC_TRADE_ABORT;
		return ret;
	}

#if SWITCH_EC_CASH
	//------------------------取余额---------------------------//----------2014.3.18-------
	if (QpbocTVLFCI.t_9F5D.SizeOff != 0) {
		get_BER_TVL(QpbocTVLFCI.t_9F5D.SizeOff, &TempTVL);
		MSG_LOG("9F5D=%s\r\n", BCD2ASC(TempTVL.V, TempTVL.L));
		memset(Buffer, 0, 40);
		memcpy(Buffer, BCD2ASC(TempTVL.V, TempTVL.L), TempTVL.L * 2);
		i = __atol((char *)Buffer);
	}
	else {
		i = getqPbocMoney();//如果没返回9F5D，就用指令取余额
		if (i < 0)
			return 0;
	}
	memcpy(TradeResult.BefMoney, (unsigned char*)&i, 4); // 余额
	a_sum1 = i;

	MSG_LOG("余额:%d\r\n", a_sum1);
#endif
	//---------------------------------------------------------


	if (online_funtion)
	{
	shuangmian_funtion_begin:
		online_funtion = 0;

		if (shuangmian != 0) {
			gMCardCand = CARDSTYLE_QPBOC;
			emv_set_pay_channel(PAY_Online);
			MSG_LOG("账户上没有余额,走双免流程\r\n");
		}
		else {
			emv_set_pay_channel(ODA);
			gMCardCand = CARDSTYLE_UNPAY_ODA;
			MSG_LOG("账户上没有余额,走ODA流程\r\n");

			TradeResult.TradeType = qPBOC_RESUL_TC;
			return 0;//可退出
		}


		ret = -1;
	//	ret = SQDataFromSVT(MISS_PBOC_PURSE, 6000);
		ret = Send_WaitRecvData(MISS_PBOC_PURSE, 6000);
		if (ret == 0)
		{
			TradeResult.TradeType = qPBOC_RESUL_TC;
			//shuangmian = 1;
			return 0;//可退出

		}
		else
		{
			return qPBOC_CARD_forbid;
		}
	}
    else
	{
		emv_set_pay_channel(E_cash);
	}


	//	miniDispstr(6, 0, "Q8", 0);
	MSG_LOG("QpbocPosActionAnalyse befor\r\n");
	get_BER_TVL(QpbocTVLFCI.t_57.SizeOff, &TempTVL);
	//	debugdata(TempTVL.V, TempTVL.L, 1);

	//if (qbpoc_check_hand() == ST_ERROR) //判断银行名柄
	//	return qPBOC_CONTRY_OUTTRADE;	// 不是要的卡


	if ((ret = QpbocReadRecordData()) != qPBOC_SUCESS_ECU) {
		if (ret == qPBOC_DATA_SIGAN_NO70) {
			TradeResult.TradeType = qPBOC_RESUL_AAC;
		}
		else
			TradeResult.TradeType = qPBOC_TRADE_ABORT;
		//		return ret;
		return qPBOC_TRADE_ABORT;
	}




	
// #if defined QK && !defined switch_RE 
// 	if(qpoc_nfc_(1)==ST_OK)
// 	{
// 		sercel();
// 		// 	if(month_decide()) // 第二次去扣钱包(次数不能连刷)
// 		// 	{
// 		// 	
// 		// 			MSG_LOG("手机连刷\r\n");
// 		// 		return qPBOC_NFC_ERR;	
// 		// 	}	
// 	}
// 	if (get_q_(2)==ST_ERROR) {//请重刷时不清除 //----------2014.3.18-------
// 		cls();
// 		display(0, 0, "警告:", 0);
// 		display(2, 2, "上一笔在规定时间内没有重刷!", 0);
// 		end_card();
// 			return qPBOC_CARD_forbid;
// 	}
// #endif

	// 4. 终端根据脱机数据认证结果对卡片真伪进行判断，若脱机数据认证失败则为伪卡，则拒绝交易
	ret = QpbocOfflineAuten();
	if (ret != qPBOC_SUCESS_ECU) {
		TradeResult.TradeType = qPBOC_RESUL_AAC;
		return qPBOC_RESUL_AAC;
	}

	ret = QpbocPosActionAnalyse();			//4   行为分析
	MSG_LOG("QpbocInitPosApp =%d\r\n", ret);
	if (ret == qPBOC_RESUL_AAC) {
		TradeResult.TradeType = qPBOC_RESUL_AAC;
		return qPBOC_RESUL_AAC;
	}
	ret = GenerateAc();
	MSG_LOG("GenerateAc =%d\r\n", ret);
	if (ret == qPBOC_RESUL_ARQC) {
		TradeResult.TradeType = qPBOC_RESUL_ARQC;
	MSG_LOG("跳转\r\n");
		goto shuangmian_funtion_begin;
	}
	else {
		TradeResult.TradeType = qPBOC_LINERESUL_AAC;

		//return qPBOC_OFFNOLINE;
	}

	//TradeResult.TradeType = qPBOC_RESUL_TC;
	if (cardSound == 0xcc) {//有卡需要重刷 //----------2014.3.18-------
		MSG_LOG("Try again a_sum1=%d, a_sumR=%d\r\n", a_sum1, a_sumR);
		if (a_sum1 < a_sumR) {//当前余额小于前一次余额，扣款成功
			memcpy(TradeResult.BefMoney, (unsigned char*)&a_sumR, 4);
			PosOfferData.PDOL.DebitMoney = s_sum1;
			goto qqPPuceOK;
		}
	}

	memcpy((unsigned char*)&a_sumR, TradeResult.BefMoney, 4); //----------2014.3.18-------

qqPPuceOK: //----------2014.3.18-------
	PosTradeCout++;

	MSG_LOG("#MSG-:****交易结果:\r\n");
	memcpy((INT8U *)&ret, TradeResult.BefMoney, 4);
	MSG_LOG("MSG-:交易后金额:%5d.%02d元\r\n", ret / 100, ret % 100);
	MSG_LOG("#MSG-:交易金额=%5d.%02d元\r\n", PosOfferData.PDOL.DebitMoney / 100, PosOfferData.PDOL.DebitMoney % 100);
	return 0;
	}
#if 0
unsigned char qPbocDisp_history(unsigned char mode, unsigned int Index)
{
	unsigned char buffer[80];
	unsigned int temp;//,temp1;
//	unsigned char i;
	stPbocRec historyCheck;

	unsigned int addr;
	int curp;

#ifdef _debug_
	debugstring("qFIND:Index:");
	debugdata((unsigned char*)&Index, 4, 1);
#endif
	if (mode == 1)
		Index += 2;
	else {
		if (Index > 2)
			Index -= 2;
		else
			Index = 2;
	}
	if ((ghisCur <= FLASH2_START1) || (ghisCur >= FLASH2_START2END)) {
		writeBackRec(buffer, 0, 0xA5);
	}
	curp = ghisCur;
	addr = (Index + 1) * RECORD_LEN;
#ifdef _debug_
	debugstring("qFIND:curp:");
	debugdata((unsigned char*)&curp, 4, 1);
	debugstring("qFIND:addr:");
	debugdata((unsigned char*)&addr, 4, 1);
#endif
	if (addr > (curp - FLASH2_START1))
	{
		addr = FLASH2_START2END - (addr - (curp - FLASH2_START1));
	}
	else
		addr = curp - addr;

	flashread(addr, buffer, 8);
#ifdef _debug_
	debugstring("qHistory:");
	debugdata((unsigned char*)&addr, 4, 1);
	debugdata(buffer, 64, 1);
#endif

	if (memcmp(buffer, "银行数据", 8) != 0)
		return ST_ERROR;//不是银行数据

	flashread(addr + 8, (unsigned char *)&historyCheck, 132);

#ifdef _debug_
	debugstring("find:");
	debugdata((unsigned char*)&Index, 4, 1);
	debugdata((unsigned char*)&historyCheck, 132, 1);
	debugdata((unsigned char*)&historyCheck.rdealType, 1, 1);
#endif
	memset(buffer, 0xff, 70);
	if (memcmp(buffer, (unsigned char*)&historyCheck, RECORD_LEN) == 0) {
		return 0xEE;
	}
	temp = 0;
	switch ((historyCheck.rdealType))
	{
	case 0:
		cls();
		memset(buffer, 0, 20);
		temp = 0;
		memcpy((unsigned char*)&temp, historyCheck.rDealMoney, 2);
		RevertTurn(2, (unsigned char*)&temp);
		sprintf((char*)buffer, _Font_KouKuandian, temp / 100, temp % 100);
		display(2, 0, (char*)buffer, 0);
		memset(buffer, 0, 20);
		temp = 0;
		memcpy((unsigned char*)&temp, historyCheck.rAferMoney, 4);
		RevertTurn(4, (unsigned char*)&temp);

		// 		memcpy((unsigned char*)&temp1, historyCheck.rDealMoney, 3);
		// 		temp -= temp1;
		sprintf((char*)buffer, _Font_Balancedian, temp / 100, temp % 100);
		display(4, 0, (char*)buffer, 0);
		dis_time_mini(6, (stcomtime*)&historyCheck.rDatatiem);
		break;
	case qPBOC_RESUL_AAC:
		cls();
		display(3, 0, "脱机拒绝", DIS_CENTER);
		break;
	case qPBOC_TRADE_ABORT:
		cls();
		display(3, 0, "交易终止", DIS_CENTER);
		break;
	case ID_REC_CLERK:
		cls();
		display(3, 0, "站务员卡", DIS_CENTER);
		break;
	case ID_REC_DRIVENO:
		cls();
		display(3, 0, "司机卡", DIS_CENTER);
		break;
	case ID_REC_LEVBUS:
		cls();
		display(3, 0, "发车记录", DIS_CENTER);
		break;
	case ID_REC_ARVBUS:
		cls();
		display(3, 0, "到站记录", DIS_CENTER);
		break;
	case ID_REC_BLKLIST:
		cls();
		display(3, 0, "黑卡记录", DIS_CENTER);
		break;
	default:
		return ST_ERROR;
	}
	sprintf((char*)buffer, "%d ", Index + 1);
	miniDispstr(7, (21 - strlen((char*)buffer)), (char*)buffer, 0);
	temp = 0;

	memset(buffer, 0, 22);
	BCD2Ascii(historyCheck.rcardMainNO, buffer, 10);
	miniDispstr(0, 0, (char*)buffer, 0);
	return ST_OK;
}
#endif
unsigned char qPbocDisp_history(unsigned char mode, unsigned int Index)
{
	unsigned char buffer[80];
	unsigned int temp;//,temp1;
//	unsigned char i;
	stPbocRec historyCheck;
	
	unsigned int addr;
	int curp;
	
#ifdef _debug_
	debugstring("qFIND:Index:");
	debugdata((unsigned char*)&Index, 4, 1);
#endif
// 	if(mode == 1)
// 		Index += 2;
// 	else{
// 		if(Index > 2)
// 			Index -= 2;
// 		else
// 			Index = 2;
// 	}
	if((ghisCur <= FLASH2_START1)||(ghisCur >= FLASH2_START2END)){
		writeBackRec(buffer, 0, 0xA5);
	}
	curp = ghisCur;
	addr=(Index) * RECORD_LEN;
#ifdef _debug_
	debugstring("qFIND:curp:");
	debugdata((unsigned char*)&curp, 4, 1);
	debugstring("qFIND:addr:");
	debugdata((unsigned char*)&addr, 4, 1);
#endif
	if(addr>(curp-FLASH2_START1))
	{
		addr=FLASH2_START2END-(addr-(curp-FLASH2_START1));
	}
	else
		addr=curp-addr;
	
	flashread(addr, buffer, 8);	
#ifdef _debug_
	debugstring("qHistory:");
	debugdata((unsigned char*)&addr, 4, 1);
	debugdata(buffer, 64, 1);
#endif

	if(memcmp(buffer, "银行数据", 8) != 0)
		return ST_ERROR;//不是银行数据
	
	flashread(addr+8, (unsigned char *)&historyCheck, 132);

#ifdef _debug_
	debugstring("find:");
	debugdata((unsigned char*)&Index, 4, 1);
	debugdata((unsigned char*)&historyCheck, 132, 1);
	debugdata((unsigned char*)&historyCheck.rdealType, 1, 1);
#endif
	memset(buffer, 0xff, 70);
	if(memcmp(buffer, (unsigned char*)&historyCheck, RECORD_LEN) == 0){
		return 0xEE;
	}
	temp=0;
	switch((historyCheck.rdealType))
	{
	case 0:
		cls();
		memset(buffer,0,20);
		temp=0;
		memcpy((unsigned char*)&temp,historyCheck.rDealMoney,2);
		RevertTurn(2, (unsigned char*)&temp);
		sprintf((char*)buffer, _Font_KouKuandian, temp/100, temp%100);
		display(2,0,(char*)buffer,0);
		memset(buffer,0,20);
		temp=0;
		memcpy((unsigned char*)&temp, historyCheck.rAferMoney, 4);
		RevertTurn(4, (unsigned char*)&temp);
		
// 		memcpy((unsigned char*)&temp1, historyCheck.rDealMoney, 3);
// 		temp -= temp1;
		sprintf((char*)buffer, _Font_Balancedian, temp/100, temp%100);
		display(4,0,(char*)buffer,0);
		dis_time_mini(6, (stcomtime*)&historyCheck.rDatatiem);
			display(6, 0,"时间:",0);
		break;
	case qPBOC_RESUL_AAC:
		cls();
		display(3, 0, "脱机拒绝", DIS_CENTER);
		break;
	case qPBOC_TRADE_ABORT:
		cls();
		display(3, 0, "交易终止", DIS_CENTER);
		break;
	case ID_REC_CLERK:
		cls();
		display(3, 0, "站务员卡", DIS_CENTER);
		break;
	case ID_REC_DRIVENO:
		cls();
		display(3, 0, "司机卡", DIS_CENTER);
		break;
	case ID_REC_LEVBUS:
		cls();
		display(3, 0, "发车记录", DIS_CENTER);
		break;
	case ID_REC_ARVBUS:
		cls();
		display(3, 0, "到站记录", DIS_CENTER);
		break;
	case ID_REC_BLKLIST:
		cls();
		display(3, 0, "黑卡记录", DIS_CENTER);
		break;
	default:
		return ST_ERROR;
	}
	sprintf((char*)buffer, "%d ", Index+1);
	miniDispstr(7, (40-strlen((char*)buffer)), (char*)buffer, 0);
	temp=0;
	
	memset(buffer,0,44);
	memcpy(buffer,_Font_showCardNo,strlen(_Font_showCardNo));
	BCD2Ascii(historyCheck.rcardMainNO, buffer+strlen(_Font_showCardNo), 10);
	display(0,0,(char*)buffer,0);
	return ST_OK;
}
extern unsigned int c_serial;
void Pboc_delay_card(void)
{
	unsigned int i;
	unsigned int sno1;
	unsigned char buffer[10];

	//	InitRC531();
	PcdRfReset(5);
	delayxms(5);
	sno1 = c_serial;
	i = 0;
	do {
		i++;
		if (GetTypeAUID(buffer) != 0)
		{
			if (memcmp((unsigned char *)&sno1, buffer, 4) == 0)
			{
				i = 0;
			}
		}
	} while (i < 3);
	return;
}
extern unsigned char SYSgetbussinessNO(unsigned char *dat);
extern void addStatMoney(unsigned char mode, unsigned int moneyv,unsigned char type);
extern unsigned char DriveCardNo[4];
extern void INT2BCD(unsigned int ii, unsigned char *bcdbuf, unsigned char bcdbuflen);
unsigned char get_rcardMainNO(unsigned char *outdata)
{

	BER_TVL TempTVL;
	// 	unsigned char rcardMainNO[10];
	unsigned char temp[50];
	char *p = NULL;
	unsigned char cardNolen = 0;
		
	get_BER_TVL(QpbocTVLFCI.t_57.SizeOff, &TempTVL);
		
		// 	memset(rcardMainNO, 0xFF, 10);
		// 
		MSG_LOG("卡帐号:");
		BCD_LOG(TempTVL.V, TempTVL.L, 1);
		memset(temp, 0, sizeof(temp));
		BCD2Ascii(TempTVL.V, temp, TempTVL.L);
		MSG_LOG("temp:%s\r\n", temp);

		p = strstr((const char *)temp, "D");

		if (p != NULL)
		{
			cardNolen = (unsigned char)(p - (char *)temp);
		}
		else {
			cardNolen = sizeof(temp);
		}

	// 	MSG_LOG("cardNolen:%d :%s\r\n",cardNolen,temp);


	if (cardNolen % 2)
	{
		temp[cardNolen] = 'F';		//后面填充0 6223670100004522768 --> 62236701000045227680
		cardNolen++;

	}
	temp[cardNolen] = 0;
	Ascii2BCD(temp,outdata, cardNolen);
	  return (cardNolen/2);
}


unsigned char qPbocBuildRec_hui(unsigned char *qrecbuff)
{
	BER_TVL TempTVL;
	stPbocRec pbocrec;
	unsigned int uiTemp;
	INT16U usI;
	unsigned char buff[50];
		unsigned char buffer[10];
	//unsigned int i=0;
//	char *p=NULL;
	unsigned char len=0;

	 memset(qrecbuff, 0x5A, 256);
	feread(BIT_PBOC_NS, 4, (unsigned char*)&uiTemp);

	if(uiTemp == 0)
		uiTemp = 1;
	sprintf((char*)buff, "%012ui", uiTemp);
// 	uiTemp ++;
// 	sysfewrite(BIT_PBOC_NS, 4, (unsigned char*)&uiTemp);


  //1.记录流水号
	memcpy(pbocrec.rpurSerial, ASC2BCD((char*)buff, 12), 6);
	//2.银联卡编号 3. 卡主帐户 4.卡有效日期 YYMM
	memset(pbocrec.rBankCardNo, '0', 18);//银联卡编号---------------------
// 	if(QpbocTVLFCI.t_5A.SizeOff != 0)	//2014.4.29
// 	{
// 
// 
// 		get_BER_TVL(QpbocTVLFCI.t_5A.SizeOff,&TempTVL);
// 
// 		memset(pbocrec.rcardMainNO, 0xFF, 10);
// 		if(TempTVL.L <= 10)
// 			memcpy(pbocrec.rcardMainNO+(10-TempTVL.L), TempTVL.V, TempTVL.L);//卡主帐户
// 		else
// 			memcpy(pbocrec.rcardMainNO, TempTVL.V, TempTVL.L);//卡主帐户
// 		
// 		get_BER_TVL(QpbocTVLFCI.t_5F24.SizeOff,&TempTVL);
// 		memcpy(pbocrec.rcardlimitdata, TempTVL.V, 2);
// 		
// 	}else{
// 
// 		memset(pbocrec.rcardMainNO, 0xFF, 10);
// 		get_BER_TVL(QpbocTVLFCI.t_57.SizeOff,&TempTVL);
// 		
// 		MSG_LOG("2磁道数据:");
// 		BCD_LOG(TempTVL.V, TempTVL.L, 1);
// 		memset(buff,0,sizeof(buff));
// 		BCD2Ascii(TempTVL.V,buff,19);
// 		p = strtok((char *)buff,"D");
// 		len = strlen(p);
// 		if (len%2)
// 		{
// 			memmove(p+1,p,len);
// 			p[len++] = 'F';		//
// 		}
// 
// 		MSG_LOG("len:%d,:%s\r\n",len,p);
// 		Ascii2BCD((unsigned char *)p,(pbocrec.rcardMainNO+10-len/2),len);
// 		BCD_LOG(pbocrec.rcardMainNO,10,1);
	// 	}
	memset(pbocrec.rcardMainNO, 0xFF, 10);
	len=get_rcardMainNO(pbocrec.rcardMainNO);
	MSG_LOG("len:%d\r\n",len);
	BCD_LOG(pbocrec.rcardMainNO,10,1);
 		BCD_LOG(pbocrec.rcardMainNO,10,1);
//5.卡片序列号
	memset(pbocrec.rcardseial, 0, 2);
	get_BER_TVL(QpbocTVLFCI.t_5F34.SizeOff,&TempTVL);
	MSG_LOG("%04XPIN序号:",QpbocTVLFCI.t_5F34.TagValue);
	// 	BCD_LOG(TempTVL.V,TempTVL.L,1);
	
	if(TempTVL.L == 1)
		pbocrec.rcardseial[1] = TempTVL.V[0];//卡片序列号
	else
		memcpy(pbocrec.rcardseial, TempTVL.V, 2);//卡片序列号	

//6.应用密文

	get_BER_TVL(QpbocTVLFCI.t_9F26.SizeOff,&TempTVL);
	memcpy(pbocrec.rUseKeytext, TempTVL.V, 8);//应用密文
//7.应用信息数据 	
	if(PosTradeCout > 0)
		pbocrec.rUsemsginfo = PosTradeCout-1;//应用信息数据
	else
		pbocrec.rUsemsginfo = PosTradeCout;//应用信息数据
	//8.发卡行应用数据
	memset(pbocrec.rpublisBanduse, 0, 32);//发卡行应用数据
	get_BER_TVL(QpbocTVLFCI.t_9F10.SizeOff,&TempTVL);
	memcpy(pbocrec.rpublisBanduse, TempTVL.V, TempTVL.L);//发卡行应用数据
	
	get_BER_TVL(QpbocTVLFCI.t_9F37.SizeOff,&TempTVL);
	memcpy(pbocrec.rRand, TempTVL.V, 4);//不可预知数

	get_BER_TVL(QpbocTVLFCI.t_9F36.SizeOff,&TempTVL);
	memcpy(pbocrec.rdealTimer, TempTVL.V, 2);//应用交易计数器HEX

	memcpy(pbocrec.rchekend, PosOfferData.AutenData.TVR, 5);//终端验证结果
	get_BER_TVL(QpbocTVLFCI.t_82.SizeOff,&TempTVL);
	memcpy(pbocrec.ryyjfType, TempTVL.V, 2);//应用交互特征
	
// 	debugstring("######t_9F10.L:");
// 	debugdata((unsigned char*)&QpbocTVLFCI.t_9F08.SizeOff, 4, 0);
// 	debugdata((unsigned char*)&TempTVL.L, 4, 0);
// 	debugdata((unsigned char*)TempTVL.V, 4, 1);

	if(QpbocTVLFCI.t_9F08.SizeOff == 0){
		memset(pbocrec.ruseVer, 0, 2);//应用版本号
	}
	else{
		get_BER_TVL(QpbocTVLFCI.t_9F08.SizeOff,&TempTVL);
		memcpy(pbocrec.ruseVer, TempTVL.V, 2);//应用版本号
	}

	if(PosTradeCout > 0)
		uiTemp = PosTradeCout-1;//交易序列计数器---------------------
	else
		uiTemp = PosTradeCout; 
	memcpy(pbocrec.rSrlTimer, (unsigned char*)&uiTemp, 2);//交易序列计数器---------------------
	RevertTurn(2, pbocrec.rSrlTimer);

	get_BER_TVL(QpbocTVLFCI.t_9F74.SizeOff,&TempTVL);
	memcpy(pbocrec.rLicense, TempTVL.V, 6);//电子现金发卡行授权码
	get_BER_TVL(QpbocTVLFCI.t_9F74.SizeOff,&TempTVL);
	memcpy(pbocrec.rLicenseACK, "Y3", 2);//授权响应码
	
	sysferead(BIT_DEVICE_NO,9,buff);
	memcpy(pbocrec.rBuPOSSerial, ASC2BCD((char*)buff+2, 6), 3);//接口设备序列号
	pbocrec.rDealMoney[0] = TradeResult.TradeMoney[1];//交易金额
	pbocrec.rDealMoney[1] = TradeResult.TradeMoney[0];
	pbocrec.rAferMoney[0] = TradeResult.BefMoney[3];//交易后余额
	pbocrec.rAferMoney[1] = TradeResult.BefMoney[2];
	pbocrec.rAferMoney[2] = TradeResult.BefMoney[1];
	pbocrec.rAferMoney[3] = TradeResult.BefMoney[0];
	memcpy(pbocrec.rDatatiem, (INT8U *)&SysTime, 7);//交易日期时间

// 	if(QpbocTVLFCI.t_9C.SizeOff == 0)
// 		pbocrec.rdealType = 0;//TradeResult.TradeType;//交易类型
// 	else{
// 		get_BER_TVL(QpbocTVLFCI.t_9C.SizeOff,&TempTVL);
// 		pbocrec.rdealType = TempTVL.V[0];//电子现金发卡行授权码
// 	}
// 	debugstring("######t_9F10.L:");
// 	debugdata((unsigned char*)&QpbocTVLFCI.t_9C.SizeOff, 4, 0);
// 	debugdata((unsigned char*)&TempTVL.L, 4, 0);
// 	debugdata((unsigned char*)TempTVL.V, 4, 1);
		pbocrec.rdealType = 0xad;
	SYSgetbussinessNO(pbocrec.rbusinessCode);//两字节商户编号
//	memcpy(pbocrec.rbusinessCode, "\x00\x00", 2);//运营商户代码

	memcpy(pbocrec.rBuLineDevNo,pFistVary.LineNo, 3);//线路编号
	memcpy(pbocrec.rBuLineDevNo+3, ASC2BCD((char*)buff+2, 6), 3);//汽车编号
	pbocrec.rNull = 0xad;
	
	usI = 0;
	usI = cal_crc16((unsigned char *)&pbocrec, 130);
	memcpy(pbocrec.rCrc16, (unsigned char*)&usI, 2);//CRC16
	memset(qrecbuff, 0x5A, qPbocRECORD_LEN);
	memcpy(qrecbuff, (unsigned char*)&pbocrec, 132);



	uiTemp = 132;	//紧接着是公交方用数据
	memcpy(qrecbuff+uiTemp, DriveCardNo, 4);//司机卡号
	uiTemp += 4;



	//交易类型
	switch(emv_get_pay_channel())
	{
	case ODA:
	case UICS:
	qrecbuff[uiTemp] = ID_ODA;
		break;
	case E_cash:
	//	qrecbuff[uiTemp] = ID_Electronic;
		MSG_LOG("PosOfferData.AutenData.TVR=\r\n");
	debugdata(PosOfferData.AutenData.TVR,5,1);
	if((PosOfferData.AutenData.TVR[0] != 0) || (PosOfferData.AutenData.TVR[1] != 0))
		qrecbuff[uiTemp] = 0x10;
	else
		qrecbuff[uiTemp] = 0xE0;//E0表示正常记录，10表示不正常记录
		break;
	case PAY_Online:
	 qrecbuff[uiTemp]= ID_ONLINE;
		break;
	default:
		break;
	}
// 	if (shuangmian)
// 	{
// 		qrecbuff[uiTemp] = ID_REC_SHUANGMIAN;
// 	}
	uiTemp++;
	
	qrecbuff[uiTemp++] = cardMessage.card_catalog;	//卡类

	Get_SerialNumF4(qrecbuff+uiTemp);	//设备序列号4
	uiTemp += 4;

	get_BER_TVL(QpbocTVLFCI.t_84.SizeOff,&TempTVL);
	memset(qrecbuff+uiTemp, 0, 8);
	memcpy(qrecbuff+uiTemp, TempTVL.V, TempTVL.L);//专用文件名称
	uiTemp+=8;
	debugstring("专用文件名称:");
	debugdata(TempTVL.V, TempTVL.L, 1);
	
	get_BER_TVL(QpbocTVLFCI.t_57.SizeOff,&TempTVL);
	qrecbuff[uiTemp++] = TempTVL.L;
	memset(qrecbuff+uiTemp, 0, 19);
	memcpy(qrecbuff+uiTemp, TempTVL.V, TempTVL.L);//2磁道数据
	uiTemp+=19;
	debugstring("2磁道数据:");
	debugdata(TempTVL.V, TempTVL.L, 1);

	switch(emv_get_pay_channel())
	{
	case ODA:
	case UICS:
	case PAY_Online:
		MSG_LOG("双免记录\r\n");
		getMobileParameter(6,qrecbuff+uiTemp);	//银联设备号
		uiTemp += 8;
		
		getMobileParameter(1,qrecbuff+uiTemp);	//银联设备号
		uiTemp += 15;
		//	sprintf(disbuf[index++],"银联商户号:%s",buff);
		
		memset(buff,0,sizeof(buff));
		sprintf((char*)buff, "%06u", POS_8583_COUNT);
		MSG_LOG("pos流水:%s\r\n",buff);
		
		memcpy(qrecbuff+uiTemp, ASC2BCD((char*)buff, 6), 3);
		uiTemp += 3;
		break;
	default:
uiTemp+=26;
		break;
	}
  
// 	if (shuangmian)
// 	{
// 		MSG_LOG("双免记录\r\n");
// 		getMobileParameter(6,qrecbuff+uiTemp);	//银联设备号
// 		uiTemp += 8;
// 		
// 		getMobileParameter(1,qrecbuff+uiTemp);	//银联设备号
// 		uiTemp += 15;
// 	//	sprintf(disbuf[index++],"银联商户号:%s",buff);
// 		
// 		memset(buff,0,sizeof(buff));
// 		sprintf((char*)buff, "%06u", POS_8583_COUNT);
// 		MSG_LOG("pos流水:%s\r\n",buff);
// 
// 		memcpy(qrecbuff+uiTemp, ASC2BCD((char*)buff, 6), 3);
// 		uiTemp += 3;
// 
// 	}
	//add by zhgfan 增加请款金额
	sprintf((char *)buff,"%012d",PosOfferData.PDOL.DebitMoney);
	memcpy(buffer,ASC2BCD((char *)buff,12),6);
	memcpy(qrecbuff+uiTemp,buffer,6);
	MSG_LOG("请款金额:%s\r\n",(char *)buff);
	uiTemp+=6;
	
	qrecbuff[uiTemp] = 0;//Off_authen;  //存储脱机认证码
	uiTemp++;
	memcpy(qrecbuff+uiTemp,StufferNO,4);
	uiTemp+=4;
	debugstring("qPBOC 灰记录 Record1:");
	debugdata(qrecbuff, 256, 1);
	debugstring("\r\n");  

//	memcpy((unsigned char*)&uiTemp, TradeResult.TradeMoney, 4);
//	addStatMoney(ID_REC_TOLL, uiTemp);

	return ST_OK;
}


unsigned char qPbocBuildRec(unsigned char *qrecbuff)
{
	BER_TVL TempTVL;
	stPbocRec pbocrec;
	unsigned int uiTemp;
	INT16U usI;
	unsigned char buff[50];
		unsigned char buffer[10];
	//unsigned int i=0;
//	char *p=NULL;
	unsigned char len=0;

	 memset(qrecbuff, 0x5A, 256);
	feread(BIT_PBOC_NS, 4, (unsigned char*)&uiTemp);

	if(uiTemp == 0)
		uiTemp = 1;
	sprintf((char*)buff, "%012ui", uiTemp);
	uiTemp ++;
	sysfewrite(BIT_PBOC_NS, 4, (unsigned char*)&uiTemp);


  //1.记录流水号
	memcpy(pbocrec.rpurSerial, ASC2BCD((char*)buff, 12), 6);
	//2.银联卡编号 3. 卡主帐户 4.卡有效日期 YYMM
	memset(pbocrec.rBankCardNo, '0', 18);//银联卡编号---------------------
// 	if(QpbocTVLFCI.t_5A.SizeOff != 0)	//2014.4.29
// 	{
// 
// 
// 		get_BER_TVL(QpbocTVLFCI.t_5A.SizeOff,&TempTVL);
// 
// 		memset(pbocrec.rcardMainNO, 0xFF, 10);
// 		if(TempTVL.L <= 10)
// 			memcpy(pbocrec.rcardMainNO+(10-TempTVL.L), TempTVL.V, TempTVL.L);//卡主帐户
// 		else
// 			memcpy(pbocrec.rcardMainNO, TempTVL.V, TempTVL.L);//卡主帐户
// 		
// 		get_BER_TVL(QpbocTVLFCI.t_5F24.SizeOff,&TempTVL);
// 		memcpy(pbocrec.rcardlimitdata, TempTVL.V, 2);
// 		
// 	}else{
// 
// 		memset(pbocrec.rcardMainNO, 0xFF, 10);
// 		get_BER_TVL(QpbocTVLFCI.t_57.SizeOff,&TempTVL);
// 		
// 		MSG_LOG("2磁道数据:");
// 		BCD_LOG(TempTVL.V, TempTVL.L, 1);
// 		memset(buff,0,sizeof(buff));
// 		BCD2Ascii(TempTVL.V,buff,19);
// 		p = strtok((char *)buff,"D");
// 		len = strlen(p);
// 		if (len%2)
// 		{
// 			memmove(p+1,p,len);
// 			p[len++] = 'F';		//
// 		}
// 
// 		MSG_LOG("len:%d,:%s\r\n",len,p);
// 		Ascii2BCD((unsigned char *)p,(pbocrec.rcardMainNO+10-len/2),len);
// 		BCD_LOG(pbocrec.rcardMainNO,10,1);
// 	}

		memset(pbocrec.rcardMainNO, 0xFF, 10);
	len=get_rcardMainNO(pbocrec.rcardMainNO);
	MSG_LOG("len:%d\r\n",len);
	BCD_LOG(pbocrec.rcardMainNO,10,1);
 		BCD_LOG(pbocrec.rcardMainNO,10,1);
//5.卡片序列号
	memset(pbocrec.rcardseial, 0, 2);
	get_BER_TVL(QpbocTVLFCI.t_5F34.SizeOff,&TempTVL);
	MSG_LOG("%04XPIN序号:",QpbocTVLFCI.t_5F34.TagValue);
	// 	BCD_LOG(TempTVL.V,TempTVL.L,1);
	
	if(TempTVL.L == 1)
		pbocrec.rcardseial[1] = TempTVL.V[0];//卡片序列号
	else
		memcpy(pbocrec.rcardseial, TempTVL.V, 2);//卡片序列号	

//6.应用密文

	get_BER_TVL(QpbocTVLFCI.t_9F26.SizeOff,&TempTVL);
	memcpy(pbocrec.rUseKeytext, TempTVL.V, 8);//应用密文
//7.应用信息数据 	
	if(PosTradeCout > 0)
		pbocrec.rUsemsginfo = PosTradeCout-1;//应用信息数据
	else
		pbocrec.rUsemsginfo = PosTradeCout;//应用信息数据
	//8.发卡行应用数据
	memset(pbocrec.rpublisBanduse, 0, 32);//发卡行应用数据
	get_BER_TVL(QpbocTVLFCI.t_9F10.SizeOff,&TempTVL);
	memcpy(pbocrec.rpublisBanduse, TempTVL.V, TempTVL.L);//发卡行应用数据
	
	get_BER_TVL(QpbocTVLFCI.t_9F37.SizeOff,&TempTVL);
	memcpy(pbocrec.rRand, TempTVL.V, 4);//不可预知数

	get_BER_TVL(QpbocTVLFCI.t_9F36.SizeOff,&TempTVL);
	memcpy(pbocrec.rdealTimer, TempTVL.V, 2);//应用交易计数器HEX

	memcpy(pbocrec.rchekend, PosOfferData.AutenData.TVR, 5);//终端验证结果
	get_BER_TVL(QpbocTVLFCI.t_82.SizeOff,&TempTVL);
	memcpy(pbocrec.ryyjfType, TempTVL.V, 2);//应用交互特征
	
// 	debugstring("######t_9F10.L:");
// 	debugdata((unsigned char*)&QpbocTVLFCI.t_9F08.SizeOff, 4, 0);
// 	debugdata((unsigned char*)&TempTVL.L, 4, 0);
// 	debugdata((unsigned char*)TempTVL.V, 4, 1);

	if(QpbocTVLFCI.t_9F08.SizeOff == 0){
		memset(pbocrec.ruseVer, 0, 2);//应用版本号
	}
	else{
		get_BER_TVL(QpbocTVLFCI.t_9F08.SizeOff,&TempTVL);
		memcpy(pbocrec.ruseVer, TempTVL.V, 2);//应用版本号
	}

	if(PosTradeCout > 0)
		uiTemp = PosTradeCout-1;//交易序列计数器---------------------
	else
		uiTemp = PosTradeCout; 
	memcpy(pbocrec.rSrlTimer, (unsigned char*)&uiTemp, 2);//交易序列计数器---------------------
	RevertTurn(2, pbocrec.rSrlTimer);

	get_BER_TVL(QpbocTVLFCI.t_9F74.SizeOff,&TempTVL);
	memcpy(pbocrec.rLicense, TempTVL.V, 6);//电子现金发卡行授权码
	get_BER_TVL(QpbocTVLFCI.t_9F74.SizeOff,&TempTVL);
	memcpy(pbocrec.rLicenseACK, "Y3", 2);//授权响应码
	
	sysferead(BIT_DEVICE_NO,9,buff);
	memcpy(pbocrec.rBuPOSSerial, ASC2BCD((char*)buff+2, 6), 3);//接口设备序列号
	pbocrec.rDealMoney[0] = TradeResult.TradeMoney[1];//交易金额
	pbocrec.rDealMoney[1] = TradeResult.TradeMoney[0];
	pbocrec.rAferMoney[0] = TradeResult.BefMoney[3];//交易后余额
	pbocrec.rAferMoney[1] = TradeResult.BefMoney[2];
	pbocrec.rAferMoney[2] = TradeResult.BefMoney[1];
	pbocrec.rAferMoney[3] = TradeResult.BefMoney[0];
	memcpy(pbocrec.rDatatiem, (INT8U *)&SysTime, 7);//交易日期时间

	if(QpbocTVLFCI.t_9C.SizeOff == 0)
		pbocrec.rdealType = 0;//TradeResult.TradeType;//交易类型
	else{
		get_BER_TVL(QpbocTVLFCI.t_9C.SizeOff,&TempTVL);
		pbocrec.rdealType = TempTVL.V[0];//电子现金发卡行授权码
	}
// 	debugstring("######t_9F10.L:");
// 	debugdata((unsigned char*)&QpbocTVLFCI.t_9C.SizeOff, 4, 0);
// 	debugdata((unsigned char*)&TempTVL.L, 4, 0);
// 	debugdata((unsigned char*)TempTVL.V, 4, 1);
	
	SYSgetbussinessNO(pbocrec.rbusinessCode);//两字节商户编号
//	memcpy(pbocrec.rbusinessCode, "\x00\x00", 2);//运营商户代码

	memcpy(pbocrec.rBuLineDevNo,pFistVary.LineNo, 3);//线路编号
	memcpy(pbocrec.rBuLineDevNo+3, ASC2BCD((char*)buff+2, 6), 3);//汽车编号
	pbocrec.rNull = 0;
	
	usI = 0;
	usI = cal_crc16((unsigned char *)&pbocrec, 130);
	memcpy(pbocrec.rCrc16, (unsigned char*)&usI, 2);//CRC16
	memset(qrecbuff, 0x5A, qPbocRECORD_LEN);
	memcpy(qrecbuff, (unsigned char*)&pbocrec, 132);



	uiTemp = 132;	//紧接着是公交方用数据
	memcpy(qrecbuff+uiTemp, DriveCardNo, 4);//司机卡号
	uiTemp += 4;



	//交易类型
	switch(emv_get_pay_channel())
	{
	case ODA:
	case UICS:
	qrecbuff[uiTemp] = ID_ODA;
		break;
	case E_cash:
	//	qrecbuff[uiTemp] = ID_Electronic;
		MSG_LOG("PosOfferData.AutenData.TVR=\r\n");
	debugdata(PosOfferData.AutenData.TVR,5,1);
	if((PosOfferData.AutenData.TVR[0] != 0) || (PosOfferData.AutenData.TVR[1] != 0))
		qrecbuff[uiTemp] = 0x10;
	else
		qrecbuff[uiTemp] = 0xE0;//E0表示正常记录，10表示不正常记录
		break;
	case PAY_Online:
	 qrecbuff[uiTemp]= ID_ONLINE;
		break;
	default:
		break;
	}
// 	if (shuangmian)
// 	{
// 		qrecbuff[uiTemp] = ID_REC_SHUANGMIAN;
// 	}
	uiTemp++;
	
	qrecbuff[uiTemp++] = cardMessage.card_catalog;	//卡类

	Get_SerialNumF4(qrecbuff+uiTemp);	//设备序列号4
	uiTemp += 4;

	get_BER_TVL(QpbocTVLFCI.t_84.SizeOff,&TempTVL);
	memset(qrecbuff+uiTemp, 0, 8);
	memcpy(qrecbuff+uiTemp, TempTVL.V, TempTVL.L);//专用文件名称
	uiTemp+=8;
	debugstring("专用文件名称:");
	debugdata(TempTVL.V, TempTVL.L, 1);
	
	get_BER_TVL(QpbocTVLFCI.t_57.SizeOff,&TempTVL);
	qrecbuff[uiTemp++] = TempTVL.L;
	memset(qrecbuff+uiTemp, 0, 19);
	memcpy(qrecbuff+uiTemp, TempTVL.V, TempTVL.L);//2磁道数据
	uiTemp+=19;
	debugstring("2磁道数据:");
	debugdata(TempTVL.V, TempTVL.L, 1);

	switch(emv_get_pay_channel())
	{
	case ODA:
	case UICS:
	case PAY_Online:
		MSG_LOG("双免记录\r\n");
		getMobileParameter(6,qrecbuff+uiTemp);	//银联设备号
		uiTemp += 8;
		
		getMobileParameter(1,qrecbuff+uiTemp);	//银联设备号
		uiTemp += 15;
		//	sprintf(disbuf[index++],"银联商户号:%s",buff);
		
		memset(buff,0,sizeof(buff));
		sprintf((char*)buff, "%06u", POS_8583_COUNT);
		MSG_LOG("pos流水:%s\r\n",buff);
		
		memcpy(qrecbuff+uiTemp, ASC2BCD((char*)buff, 6), 3);
		uiTemp += 3;
		break;
	default:
uiTemp+=26;
		break;
	}
  
// 	if (shuangmian)
// 	{
// 		MSG_LOG("双免记录\r\n");
// 		getMobileParameter(6,qrecbuff+uiTemp);	//银联设备号
// 		uiTemp += 8;
// 		
// 		getMobileParameter(1,qrecbuff+uiTemp);	//银联设备号
// 		uiTemp += 15;
// 	//	sprintf(disbuf[index++],"银联商户号:%s",buff);
// 		
// 		memset(buff,0,sizeof(buff));
// 		sprintf((char*)buff, "%06u", POS_8583_COUNT);
// 		MSG_LOG("pos流水:%s\r\n",buff);
// 
// 		memcpy(qrecbuff+uiTemp, ASC2BCD((char*)buff, 6), 3);
// 		uiTemp += 3;
// 
// 	}
	//add by zhgfan 增加请款金额
	sprintf((char *)buff,"%012d",PosOfferData.PDOL.DebitMoney);
	memcpy(buffer,ASC2BCD((char *)buff,12),6);
	memcpy(qrecbuff+uiTemp,buffer,6);
	MSG_LOG("请款金额:%s\r\n",(char *)buff);
	uiTemp+=6;
	
	qrecbuff[uiTemp] = 0;//Off_authen;  //存储脱机认证码
	uiTemp++;
	memcpy(qrecbuff+uiTemp,StufferNO,4);
	uiTemp+=4;
	debugstring("qPBOC Record1:");
	debugdata(qrecbuff, sizeof(stPbocRec), 1);
	debugstring("\r\n");  

	memcpy((unsigned char*)&uiTemp, TradeResult.TradeMoney, 4);
	addStatMoney(ID_REC_TOLL, uiTemp,emv_get_pay_channel());

	return ST_OK;
}

//写入公交存贮器
unsigned char qPbocWriteRecord(unsigned char *rec)
{
	//	unsigned char i=0;
	//	unsigned char buff[RECORD_LEN];
	unsigned char buffer[qPbocRECORD_LEN + 1];
	//	unsigned char *tempbuf;
	//	unsigned int temp;

	memset(buffer, 0x5A, qPbocRECORD_LEN);

	memcpy(buffer, "银行数据", 8);
	memcpy(buffer + 8, rec, qPbocRECORD_LEN - 8);


	MSG_LOG("write QBOC REC:");
	BCD_LOG(buffer, qPbocRECORD_LEN, 1);

	WriteRecord(buffer, 0);

	return ST_OK;


}


#if (qPBOC_HAND==2)
//只有钟国营处用了公交二次发卡，加入了卡类
unsigned char Read2PublishInfo(void)
{
	int ret, send_len;
	INT8U Buff[254];

	send_len = 14;
	memcpy(Buff, "\x02\x00\xA4\x04\x00\x07\xA0\x00\x00\x03\x33\x01\x01\x00", send_len);//先公交二次发卡目录
	Buff[0] = PCBSerial;
	PCBSerial ^= 1;
	memset(qpbocrevbuf, 0, 256);
	MSG_LOG("Read2PublishInfo1 snd =%s\r\n", BCD2ASC(Buff, send_len));
	ret = RfPassthrough(qpbocrevbuf, 2, Buff + 1, send_len - 1);
	MSG_LOG("select Read2 qpbocrevbuf1 =%s\r\n", BCD2ASC(qpbocrevbuf, ret));
	if (ret == 0) {
		return 0;
	}
	if (memcmp(qpbocrevbuf + ret - 2, "\x90\x00", 2) != 0)
	{
		return 0;
	}
	MSG_LOG("Read2APUD1 =%d,%s\r\n", ret, BCD2ASC(qpbocrevbuf, ret));
	//80b400b0020001


	send_len = 8;
	memcpy(Buff, "\x02\x80\xb4\x00\xb0\x02\x00\x01", send_len);//读二次发卡文件
	Buff[0] = PCBSerial;
	PCBSerial ^= 1;
	memset(qpbocrevbuf, 0, 256);
	MSG_LOG("Read2PublishInfo2 snd =%s\r\n", BCD2ASC(Buff, send_len));
	ret = RfPassthrough(qpbocrevbuf, 2, Buff + 1, send_len - 1);
	MSG_LOG("select Read2 qpbocrevbuf2 =%s\r\n", BCD2ASC(qpbocrevbuf, ret));
	if (ret == 0) {
		return 0;
	}
	if (memcmp(qpbocrevbuf + ret - 2, "\x90\x00", 2) != 0)
	{
		return 0;
	}
	MSG_LOG("Read2APUD2 =%d,%s\r\n", ret, BCD2ASC(qpbocrevbuf, ret));
	return qpbocrevbuf[5];
}
#endif

extern void face_time(void);
int QpbocOfflineAuten(void);

void cpuPBOCmain(void)
{
	BER_TVL *TempTVL;
	int len;
	unsigned char Buffer[qPbocRECORD_LEN + 1];
		unsigned char Buffer_1[512];
	int price;
	int ret;
	unsigned char cType;

#ifdef Center_Ctrl_BU_ENABEL
	if (gBuCtrlFlag & BUCTRL_FLAG_qPBOC) {//禁止刷银联卡
		return;
	}
#endif
	//	InitRC531();
	//	PKI_INIT();
	// 	do_cpu_card();
	// 	return;
	//	miniDispstr(6, 0, "Q1", 0);
	//	debugstring("---Cpupboc Main---\r\n");
	//	ret = qPbocGet_Record_point((unsigned char*)&price, 0x5a);
	//	if(ret != 0xFF){
	//		MessageBox(0, "银联记录已满!");
	//		MifareHalt();
	//		Pboc_delay_card();
	//		g24GDisFlash = 20;
	//		restore_flag = 0;
	//		return;
	//	}

		//	miniDispstr(6, 0, "Q2", 0);

	MSG_LOG("cpuP  BO  Cmain:\r\n");

	if (checkBusInfo() > 10)
	{
		cls();
		if (gchn_eng == 'E') {
			display(0, 0, "warring:", 1);
			display(2, 0, "pos info error!", DIS_CENTER);
		}
		else
		{
			display(0, 0, "提示:", 0);
			display(3, 2, "设备信息错误", 0);
		}
		end_card();
		return;
	}
#if defined switch_RE && ! defined QK  //冲正

	Judge_timeout_re(2,shuangmian);
#elif defined switch_RE && defined QK
	Judge_timeout_re(3,shuangmian);  //暂时不写到铁电
#elif defined QK  && ! defined switch_RE
		if (get_q_(2)==ST_ERROR) {//请重刷时不清除 //----------2014.3.18-------
		cls();
		display(0, 0, "警告:", 0);
		display(2, 2, "上一笔在规定时间内没有重刷!", 0);
		end_card();
		return ;
	}
#endif


	if ((cal_space_O() < 5) && (cardMessage.card_catalog != CARD_DATA_GATHER))
	{
		cls();
		if (gchn_eng == 'E') {
			display(0, 0, "warring:", 1);
			display(2, 0, "eeprom full!", DIS_CENTER);
		}
		else
		{
			display(0, 0, "警告:", 0);
			display(2, 2, "请上传数据ODA!", 0);
		}
		end_card();
		restore_flag = 3;
		return;
	}


	if (cardSound != 0xcc) {//请重刷时不清除 //----------2014.3.18-------

		Pro_QpbocInt();
	}
	else
	{
		if (c_serial != card_ser) {//不同的卡，取消重刷 //----------2014.3.18-------
#ifdef BUS_GFT
			return ST_ERROR;//请重刷
#else
			cardSound = 0;
			card_ser = 0;
#endif
		}
		}

	//	miniDispstr(6, 0, "Q3", 0);
#if !defined BUS_DK_GZHUAIREN_ && !defined BUS_GZDK_ZLS_
	cardMessage.card_catalog = CARD_qPBOC_BUS;
#else
#ifdef CARD_NORMAL_BUS
	cardMessage.card_catalog = CARD_NORMAL_BUS;
#else
	cardMessage.card_catalog = CARD_NORMAL;
#endif
#endif

	gMCardCand = CARDSTYLE_PBOC;

	PCBSerial = 0x02;

	//	debugstring("---1---\r\n");

	MSG_LOG("cpuP  BO  Cmain:%d!!\r\n", cardMessage.card_catalog);

#ifndef BUS_GUILIN_
	//	debugstring("---2---\r\n");
	memcpy(Buffer, "\xe0\x50", 2);
	//	memcpy(Buffer,"\xe0\x80",2);
	ret = RfPassthrough(qpbocrevbuf, 2, Buffer, 2);
	if (ret == 0) {
		TradeResult.TradeType = qPBOC_TRADE_ABORT;
		return;
	}
	MSG_LOG("CPU start=%s\r\n", BCD2ASC(Buffer, ret));
#endif



	cType = 0;

#if defined SLZR_TEST_ || defined QPBOC_DEBUG
	price = 1;
#elif defined BUS_GZDK_ZLS_
	price = 1;
#else
	price = get_s_sum1();//0;//
#endif

// #ifdef _debug_
// 	price = 0;
// #endif

// 	price = 1;

	if (month_decide()) // 第二次去扣钱包(次数不能连刷)
	{
		disp_no_swipe();
		return ;
	}

	s_sum1 = price;
#ifndef _debug_
	if((s_sum1 == 0) || (s_sum1 > 2000))
	{
		if(gchn_eng == 'E'){
			display(0,0,"hint:", DIS_Cls);
			display(2,0,"price is error",DIS_CENTER);
		}
		else{
			display(0,0,"提示:", DIS_Cls);
			display(2,0,"票价不正确",DIS_CENTER);
		}
		Pboc_delay_card();
		MifareHalt();
		g24GDisFlash = 1;
		restore_flag = 0;
		card_ser = 0;
		cardSound=0;
		return ;
	}

#endif


	MSG_LOG("cType=%02X\r\n", cType);

	//	miniDispstr(6, 0, "Q4", 0);
	clr_wdt();
	ret = qPbocTradeProc(price, (INT32U*)&len, Buffer);


	//	miniDispstr(6, 0, "Q5", 0);
	MSG_LOG("qPbocTradeProc END:%d,TradeType:%d\r\n", ret, TradeResult.TradeType);


	if (ret == qPBOC_RESUL_AAC) {
		display(0, 0, "拒绝交易:", DIS_Cls);
		memcpy((unsigned char*)&len, TradeResult.BefMoney, 4);
		if (len < price) {
			display(3, 2, "余额不足", 0);
			audio(Audio_FLING_MONEY);
		}
			delayxms(200);
		MifareHalt();
		Pboc_delay_card();
	
		g24GDisFlash = 2;
		restore_flag = 0;
		card_ser = 0;
		cardSound = 0;
	}
	else if (ret == qPBOC_CARD_forbid_oda) {
		display(0, 0, "拒绝交易:", DIS_Cls);

			display(3, 2, "ODA禁止刷借记卡", 0);
		//	audio(Audio_FLING_MONEY);
	
		MifareHalt();
		Pboc_delay_card();
		g24GDisFlash = 2;
		restore_flag = 0;
		card_ser = 0;
		cardSound = 0;
	}
		
	else if (ret == qPBOC_QpboC_tryAgain) {//需要重刷 //----------2014.3.18-------
		if (qPbocBuildRec(Buffer) == ST_ERROR) {//临时保存在铁电中，再次刷成功后使用。  //请重刷2 --2014.4.22--
			return; //如果里需字段有错，说明没取到，直接退出 2014.4.29
		}
		audio(Audio_TRY_AGAIN);
		PcdRfReset(2);
		card_ser = c_serial;
		cardSound = 0xcc;
		debugstring("close qPBOC_QpboC_tryAgain!!\r\n");
		MSG_LOG(" s_sum1=%d, a_sum1=%d \r\n", s_sum1, a_sum1);
		CheckSno(ERROR_MONEY, OPER_TRANSFER_NO, Buffer);

		sysfewrite(BIT_qPBOC_Rec_Temp, 192, Buffer);
		MSG_LOG("重刷记录 %s\r\n", BCD2ASC(Buffer, 192));  //请重刷2 --2014.4.22--
	}
	else if ((ret == qPBOC_CONTRY_OUTTRADE) || (ret == qPBOC_QpboC_SIGAN_NO)) {//不是工行的卡
		display(0, 0, "拒绝交易:", DIS_Cls);
		if (ret == qPBOC_QpboC_SIGAN_NO)
			display(3, 0, "无银行标志9F12", DIS_CENTER);
		else
			display(3, 0, STRNOQPBOCC, DIS_CENTER);
		audio(Audio_INVALID);
		if (QpbocTVLFCI.t_9F12.SizeOff != 0) {
			TempTVL = (BER_TVL *)Buffer;
			get_BER_TVL(QpbocTVLFCI.t_9F12.SizeOff, (BER_TVL *)Buffer);

			memset(irda_rxBuf, 0, 20);
			memcpy(irda_rxBuf, TempTVL->V, TempTVL->L);
			miniDispstr(15, 10, (char*)irda_rxBuf, 0);
		}
		Pboc_delay_card();
		MifareHalt();
		g24GDisFlash = 1;
		restore_flag = 0;
		card_ser = 0;
		cardSound = 0;
	}
	else if (ret == qPBOC_CARD_forbid) {
		if (ACK_flag != 0)
		{
		//	SoundMessage(SOUND_FLING_MONEY);
		//	audio(Audio_TRY_AGAIN);	//非法卡
			dis_qboc_ack_code(ACK_flag);
		}
//	if (switch_both==1 &&ACK_flag ==0xFF)
//		{
//			audio(Audio_TRY_AGAIN);  //s双通道
//		}
		else
			SoundMessage(SOUND_FLING_MONEY);
		//display(3, 0, "请稍候再试!", DIS_CENTER);
		delayxms(500);
		Pboc_delay_card();
			
		g24GDisFlash = 1;
		restore_flag = 3;
		card_ser = 0;
		cardSound = 0;
	}
	else if ((ret == 0) && (TradeResult.TradeType == qPBOC_RESUL_TC)) {
		if (cType == 0)
			audio(Audio_DONG);
		else if (cType == 0xA4)
			audio(Audio_STUDENT);
		else if (cType == 0xB0)
			audio(Audio_STUFF);
		clr_wdt();
		//显示结果
		cls();
		led_on(LED_GREEN);
		beep(1, 100, 50);
		// 		memset(Buffer,0,sizeof(Buffer));
		// 		display(0,0,"交易结果:",0);
		// 		memcpy((unsigned char *)&len, TradeResult.BefMoney, 4);
		// 		sprintf((char *)Buffer, "余额:%d.%02d元", len/100, len%100);
		// #ifdef PBOC_loan
		// 		display(2, 0, "刷卡成功", 0);
		// #else
		// 		display(2, 0, (char*)Buffer, 0);
		// #endif
		// 		memcpy((unsigned char *)&len, TradeResult.TradeMoney, 4);
		// 		sprintf((char *)Buffer, "扣款:%d.%02d元", len/100, len%100);
		// 		display(4, 0, (char *)Buffer, 0);
#if SWITCH_EC_CASH
		display_3232(2, 0, "刷卡成功", 0);
#else
   //display(2, 0, (char*)Buffer, 0);
		money_msg(ID_REC_TOLL, a_sum1, s_sum1, 0);
#endif
		MSG_LOG("刷卡成功************\r\n");



		PosTradeCout++;
		if (QpbocTVLFCI.t_9F12.SizeOff != 0) {
			TempTVL = (BER_TVL *)Buffer;
			get_BER_TVL(QpbocTVLFCI.t_9F12.SizeOff, (BER_TVL *)Buffer);

			memset(irda_rxBuf, 0, 20);
			memcpy(irda_rxBuf, TempTVL->V, TempTVL->L);
			miniDispstr(15, 0, (char*)irda_rxBuf, 0);
		}
		//		MSG_LOG( "刷卡成功************2\r\n");

		ComSumeFile.SumMoney += price; //当前消费总额
		len = (((unsigned char*)&ComSumeFile.SumMoney) - ((unsigned char*)&ComSumeFile));
		sysfewrite(BIT_COMSUME_FILE + len, 4, (unsigned char*)&ComSumeFile.SumMoney);


		clr_wdt();
		if (cardSound != 0) {//重刷成功
			sysferead(BIT_qPBOC_Rec_Temp, 192, Buffer);   //请重刷2 --2014.4.22--
			//			debugstring("----------------------------\r\n");
			MSG_LOG(" 重刷成功记录 %s\r\n", BCD2ASC(Buffer, 192));
		}
		else {
			//			MSG_LOG( "刷卡成功************3\r\n");

			// 			if(qPbocBuildRec(Buffer) == ST_ERROR){//临时保存在铁电中，再次刷成功后使用。  //请重刷2 --2014.4.22--
			// 				goto qpbocMainEnd;//记录错误,不用写了. 2014.4.29
			// 			}
			qPbocBuildRec(Buffer);
			MSG_LOG(" NO重刷记录 %s\r\n", BCD2ASC(Buffer, 192));   //请重刷2 --2014.4.22--
			//			debugstring("++++++++++++++++++++++++++++\r\n");
		}
		//add hbg
		if (gMCardCand == CARDSTYLE_UNPAY_ODA) {
			build8583_qpboc_Purse_0200(Buffer_1);
			save_ODA_infor(ODA_FeRC_Write, repurse_infor);
			delayxms(3);
			write_linux_re(MISS_PBOC_UPREC_ODA);//备份0DA
			}
// 		if (gMCardCand == CARDSTYLE_UNPAY_ODA) {
// 			ret = SQDataFromSVT(MISS_PBOC_UPREC_ODA_first, 6000);
// 		}
		//		MSG_LOG( "刷卡成功************4\r\n");

		qPbocWriteRecord(Buffer);
		SetTcpPacketTTLTime(RECORD_TTL);
	
		set_delay_send(set_delay_send_val); //延时找任务
		//shuangmian = 0;
		//		MSG_LOG( "刷卡成功************5\r\n");

		// 		debugstring("\r\nqPBOC Record2:"); 
		// 		debugstring(BCD2ASC(Buffer, 132)); 
		// 		debugstring("\r\n");
	qpbocMainEnd:
		MifareHalt();
		Pboc_delay_card();
		g24GDisFlash = 2;
		restore_flag = 0;

		card_ser = 0;
		cardSound = 0;   //请重刷2 --2014.4.22--
#if defined QK && !defined switch_RE 
		MSG_LOG( "Q_pos_infor.flge==%x\r\n",Q_pos_infor.flge);
		if(Q_pos_infor.flge==0xad&&switch_both==1) //双通道才有的
		{
				MSG_LOG("再请款删冲正--\r\n");
				//memset(repurse_infor, 0, sizeof(repurse_infor));
				save_repurse_infor(FeRC_Dlelt, NULL);
		}
		init_Q_pos_infor();//成功后清空
#elif defined switch_RE && defined QK
		if(timeout_infor.flge==Switch_sign_OVER&&switch_both==1) //双通道才有的
		{
				MSG_LOG("	延时再请款删冲正--\r\n");
				//memset(repurse_infor, 0, sizeof(repurse_infor));
				save_repurse_infor(FeRC_Dlelt, NULL);
		}
		init_timeout_infor();

#endif
			MSG_LOG( "刷卡成功************4\r\n");
	}
	else {
		debugstring("close RFTX!!\r\n");
		PcdRfReset(2);
	}
	}



//--------------------消费认证－－－－－－－－－
int qPBOCgetPublicKey(unsigned char pKeyVer, KEY_PUBLICMSG *pKeyOut);
int	RSA_SLF_public_decrypt(unsigned char *from, unsigned char *to, KEY_PUBLICMSG Key, int padding);
static int SHA1(unsigned char *d, int n, unsigned char *md);

int QpbocGetAutenCerPKEY(void)
{
	BER_TVL TempTVL;
	int i;

	if (QpbocTVLFCI.t_8F.SizeOff == 0) {
		ERROR("ERROR-:Table 8F is not find\r\n");
		//return -1;
	}
	MSG_LOG("QpbocTvlFci.t_8F:%04X,%04X", QpbocTVLFCI.t_8F.TagValue, QpbocTVLFCI.t_8F.SizeOff);
	AuterCerKey.e_Key = 3;
	memcpy(AuterCerKey.n_Key, ASC2BCD("EB374DFC5A96B71D2863875EDA2EAFB96B1B439D3ECE0B1826A2672EEEFA7990286776F8BD989A15141A75C384DFC14FEF9243AAB32707659BE9E4797A247C2F0B6D99372F384AF62FE23BC54BCDC57A9ACD1D5585C303F201EF4E8B806AFB809DB1A3DB1CD112AC884F164A67B99C7D6E5A8A6DF1D3CAE6D7ED3D5BE725B2DE4ADE23FA679BF4EB15A93D8A6E29C7FFA1A70DE2E54F593D908A3BF9EBBD760BBFDC8DB8B54497E6C5BE0E4A4DAC29E5", 352), 176);
	AuterCerKey.n_KeyLen = 144;

	get_BER_TVL(QpbocTVLFCI.t_8F.SizeOff, &TempTVL);
	MSG_LOG("QpbocTVLFCI.t_8FVer =%s\r\n", BCD2ASC(TempTVL.V, TempTVL.L));

	i = qPBOCgetPublicKey(TempTVL.V[0], &AuterCerKey);
	//	i = findPulicKey(TempTVL.V[0]);
	if (i != 0)	//设备中没有此公钥
		return -1;
	else
		return 0;
}

static int QpbocGetPublishPKEY(void)
{
	int ret = 0, i = 0;
	INT32U OutLen;
	INT8U Buffer[500], EnDataBuffer[280], ShaData[20];
	BER_TVL PublishPublicCertify, TempTVL;
	TYPE_CHAR_LONG lTempLong;

	MSG_LOG("********************   QpbocGetPublishPKEY **************\r\n");
	ret = ret;
	get_BER_TVL(QpbocTVLFCI.t_90.SizeOff, &PublishPublicCertify);

	MSG_LOG("PublishPublicCertify Len =%d\r\n", PublishPublicCertify.L);
	MSG_LOG("PublishPublicCertify =%s\r\n", BCD2ASC(PublishPublicCertify.V, PublishPublicCertify.L));
	MSG_LOG("Auten E=%d\r\n", AuterCerKey.e_Key);
	MSG_LOG("Auten Key =%s\r\n", BCD2ASC(AuterCerKey.n_Key, AuterCerKey.n_KeyLen));
	AuterCerKey.n_KeyLen = PublishPublicCertify.L;
	if (PublishPublicCertify.L != AuterCerKey.n_KeyLen) {
		ERROR("ERROR-:Len :AutenCenterKeyLen(%d) != PublishPublicCertify(%d)\r\n", AuterCerKey.n_KeyLen, PublishPublicCertify.L);
		return qPBOC_PUBLISHKEY_LEN;
	}

	memset(EnDataBuffer, 0, 260);
	if (RSA_SLF_public_decrypt(PublishPublicCertify.V, EnDataBuffer, AuterCerKey, RSA_NO_PADDING) != 0) {
		//	if((ret =  RSA_public_decrypt_LOCK(PublishPublicCertify.V, EnDataBuffer, AuterCerKey,  RSA_NO_PADDING)) != 0){
		ERROR("ERROR-:RSA_public_decrypt  is %d\r\n", ret);
		return qPBOC_RSA_ERROR;
	}
	MSG_LOG("Publish RSA Result=%s\r\n", BCD2ASC(EnDataBuffer, AuterCerKey.n_KeyLen));

	if ((EnDataBuffer[0] != 0x6A) || (EnDataBuffer[1] != 0x02) || (EnDataBuffer[AuterCerKey.n_KeyLen - 1] != 0xBC)) {
		ERROR("ERROR-:Retrieval of Publish Certification Format EnDataBuffer[0]=%02X,EnDataBuffer[1]=%02X,EnDataBuffer[AutenCenterKeyLen-1]=%02X\r\n",
			EnDataBuffer[0], EnDataBuffer[1], EnDataBuffer[AuterCerKey.n_KeyLen - 1]);
		return qPBOC_PUBLISHC_FORMAT;
	}
	memset(Buffer, 0, sizeof(Buffer));
	OutLen = 0;
	memcpy(Buffer + OutLen, EnDataBuffer + 1, (AuterCerKey.n_KeyLen - 1 - 1 - 20));
	OutLen += (AuterCerKey.n_KeyLen - 1 - 1 - 20);
	if (QpbocTVLFCI.t_92.SizeOff != 0) {
		get_BER_TVL(QpbocTVLFCI.t_92.SizeOff, &TempTVL);
		memcpy(Buffer + OutLen, TempTVL.V, TempTVL.L);
		OutLen += TempTVL.L;
	}
	get_BER_TVL(QpbocTVLFCI.t_9F32.SizeOff, &TempTVL);
	if (TempTVL.L == 1) {
		Buffer[OutLen] = 3;
		OutLen += 1;
	}
	else {
		memcpy(Buffer + OutLen, "\x01\x00\x01", 3);
		OutLen += 3;
	}
	MSG_LOG("Sha Len =%d\r\n", OutLen);
	MSG_LOG("Sha Souce =%s\r\n", BCD2ASC(Buffer, OutLen));
	// 计算哈希结果
	memset(ShaData, 0, 20);
	SHA1(Buffer, OutLen, ShaData);
	MSG_LOG("QFPPK SHA1 =%s\r\n", BCD2ASC(ShaData, 20));

	//比较SHA结果
	if (memcmp(ShaData, EnDataBuffer + AuterCerKey.n_KeyLen - 1 - 20, 20) != 0) {
		ERROR("ERROR-:aaa:Retrieval of Publish Certification SHA is error\r\n");
		return qPBOC_PUBLISHC_SIGN;
	}
	get_BER_TVL(QpbocTVLFCI.t_92.SizeOff, &TempTVL);
	PublishKey.n_KeyLen = EnDataBuffer[B_PUBLISHC_PKEYLEN];
	if (PublishKey.n_KeyLen <= (AuterCerKey.n_KeyLen - 36)) {
		memcpy(PublishKey.n_Key, EnDataBuffer + B_PUBLISHC_PKEY, PublishKey.n_KeyLen);
	}
	else {
		memcpy(PublishKey.n_Key, EnDataBuffer + B_PUBLISHC_PKEY, (AuterCerKey.n_KeyLen - 15 - 1 - 20));
		memcpy(PublishKey.n_Key + (AuterCerKey.n_KeyLen - 15 - 1 - 20), TempTVL.V, TempTVL.L);
	}

	get_BER_TVL(QpbocTVLFCI.t_5A.SizeOff, &TempTVL);

	memset(Buffer, 0, sizeof(Buffer));
	memcpy(Buffer, BCD2ASC(EnDataBuffer + B_PUBLISHC_ISSID, 4), 8);
	MSG_LOG("Publish Cer PAN =%s\r\n", Buffer);
	for (i = 0; i < 8; i++) if (Buffer[7 - i] != 'F') break;
	MSG_LOG("Valid Sum =%d\r\n", (8 - i));
	MSG_LOG("PAN =%s\r\n", BCD2ASC(TempTVL.V, TempTVL.L));
	if (memcmp(Buffer, BCD2ASC(TempTVL.V, TempTVL.L), (INT32U)(8 - i)) != 0) {
		ERROR("ERROR-:Retrieval of Publish Certification SHA is error\r\n");
		return qPBOC_PUBLISHC_FLAG;
	}


	memcpy(Buffer, EnDataBuffer + B_PUBLISHC_EXPTIME, 2);
	RevertTurn(2, Buffer);
	MSG_LOG("Now Time =%s\r\n", BCD2ASC((INT8U *)&SysTime.year_h, 7));
	MSG_LOG("Exp Time =%02X%02X\r\n", Buffer[0], Buffer[1]);
	if (memcmp(Buffer, (INT8U *)&SysTime.year_l, 2) <= 0) {
		ERROR("ERROR-:Retrieval of Publish Certification Exp Time is error\r\n");
		return qPBOC_PUBLISHC_EXP;
	}

	if (EnDataBuffer[B_PUBLISHC_PKEYFLAG] != 0x01) {
		ERROR("ERROR-:Retrieval of Publish Certification PKEY flag <> 1\r\n");
		return qPBOC_PUBLISHC_PKEYF;
	}

	MSG_LOG("PublishPkeyLen =%d\r\n", PublishKey.n_KeyLen);
	MSG_LOG("PublishPkey =%s\r\n", BCD2ASC(PublishKey.n_Key, PublishKey.n_KeyLen));

	get_BER_TVL(QpbocTVLFCI.t_9F32.SizeOff, &TempTVL);
	lTempLong.LongBuff = 0;
	for (i = 0; i < EnDataBuffer[B_PUBLISHC_PKEYELEN]; i++) lTempLong.CharBuff[i] = TempTVL.V[TempTVL.L - 1 - i];
	PublishKey.e_Key = lTempLong.LongBuff;
	MSG_LOG("PublishPkeyE =%d\r\n", PublishKey.e_Key);
	return qPBOC_SUCESS_ECU;
}
int QpbocGetICPKEY(void)
{
	int ret = 0, i = 0;
	INT32U OutLen;
	INT8U Buffer[500], EnDataBuffer[280], ShaData[20];
	BER_TVL TempTVL;
	TYPE_CHAR_LONG lTempLong;

	MSG_LOG("************* QpbocGetICPKEY **********\r\n");
	ret = ret;
	get_BER_TVL(QpbocTVLFCI.t_9F46.SizeOff, &TempTVL);
	if (TempTVL.L != PublishKey.n_KeyLen) {
		ERROR("ERROR-:PIC Certification Len != Publish Publich Key Len\r\n");
		return qPBOC_ICC_LEN;
	}
	memset(EnDataBuffer, 0, 280);
	if ((ret = RSA_SLF_public_decrypt(TempTVL.V, EnDataBuffer, PublishKey, RSA_NO_PADDING)) != 0) {
		ERROR("ERROR-:RSA_IC public_decrypt  is %d\r\n", ret);
		return qPBOC_RSA_ERROR;
	}
	//	debugdata((unsigned char*)&PublishKey.n_KeyLen, 4, 1);
	MSG_LOG("IC RSA Result=%s\r\n", BCD2ASC(EnDataBuffer, PublishKey.n_KeyLen));

	if ((EnDataBuffer[0] != 0x6A) || (EnDataBuffer[1] != 0x04) || (EnDataBuffer[PublishKey.n_KeyLen - 1] != 0xBC)) {
		ERROR("ERROR-:Retrieval of IC Certification Format EnDataBuffer[0]=%02X,EnDataBuffer[1]=%02X,EnDataBuffer[PublishPkeyLen-1]=%02X\r\n",
			EnDataBuffer[0], EnDataBuffer[1], EnDataBuffer[PublishKey.n_KeyLen - 1]);
		return qPBOC_ICC_FORMAT;
	}

	memset(Buffer, 0, sizeof(Buffer));
	OutLen = 0;

	memcpy(Buffer + OutLen, EnDataBuffer + 1, (PublishKey.n_KeyLen - 1 - 1 - 20));
	OutLen += (PublishKey.n_KeyLen - 1 - 1 - 20);

	if (QpbocTVLFCI.t_9F48.SizeOff != 0) {
		get_BER_TVL(QpbocTVLFCI.t_9F48.SizeOff, &TempTVL);
		memcpy(Buffer + OutLen, TempTVL.V, TempTVL.L);
		OutLen += TempTVL.L;
	}

	get_BER_TVL(QpbocTVLFCI.t_9F47.SizeOff, &TempTVL);
	memcpy(Buffer + OutLen, TempTVL.V, TempTVL.L);
	OutLen += TempTVL.L;

	if (SDADataValid == 0) {
	}
	else {

		memcpy(Buffer + OutLen, SDADataAuten, SDADataALen);
		OutLen += SDADataALen;
	}

	if (QpbocTVLFCI.t_9F4A.SizeOff != 0) {
		get_BER_TVL(QpbocTVLFCI.t_9F4A.SizeOff, &TempTVL);
		get_BER_TVL(QpbocTVLFCI.t_82.SizeOff, &TempTVL);
		memcpy(Buffer + OutLen, TempTVL.V, TempTVL.L);
		OutLen += TempTVL.L;
	}

	//	ERROR("Sha Souce =%d %d %s\r\n",SDADataValid, SDADataALen,BCD2ASC(SDADataAuten,SDADataALen));

	MSG_LOG("Sha Len =%d\r\n", OutLen);
	MSG_LOG("Sha Souce =%s\r\n", BCD2ASC(Buffer, OutLen));

	// 计算哈希结果
	memset(ShaData, 0, 20);
	SHA1(Buffer, OutLen, ShaData);
	MSG_LOG("IC KEY SHA1 A=%s\r\n", BCD2ASC(ShaData, 20));
	MSG_LOG("IC KEY SHA1 B=%s\r\n", BCD2ASC(EnDataBuffer + PublishKey.n_KeyLen - 1 - 20, 20));

	//比较SHA结果
	if (memcmp(ShaData, EnDataBuffer + PublishKey.n_KeyLen - 1 - 20, 20) != 0) {
		ERROR("ERROR-:IC KEY Retrieval of Publish Certification SHA is error\r\n");
		return qPBOC_ICC_SIGN;
	}

	get_BER_TVL(QpbocTVLFCI.t_5A.SizeOff, &TempTVL);
	memset(Buffer, 0, sizeof(Buffer));
	memcpy(Buffer, BCD2ASC(EnDataBuffer + B_ICC_APPPAN, 10), 20);
	//MSG_LOG("IC Cer PAN = %s == \r\n",Buffer);
	//BCD_LOG(TempTVL.V, TempTVL.L, 1);
	for (i = 0; i < 20; i++) if (Buffer[19 - i] != 'F') break;
	if (memcmp(Buffer, BCD2ASC(TempTVL.V, TempTVL.L), (INT32U)(20 - i)) != 0) {
		ERROR("ERROR-:Retrieval of IC Certification SHA is error\r\n");
		return qPBOC_ICC_PANFLAG;
	}

	memcpy(Buffer, EnDataBuffer + B_ICC_EXPTIME, 2);
	RevertTurn(2, Buffer);
	//	MSG_LOG("Now Time =%s\r\n",BCD2ASC((INT8U *)&SysTime.cer,7));
	//	MSG_LOG("Exp Time =%02X%02X\r\n",Buffer[0],Buffer[1]);
	if (memcmp(Buffer, (INT8U *)&SysTime.year_l, 2) <= 0) {
		ERROR("ERROR-:Retrieval of IC Certification Exp Time is error\r\n");
		return qPBOC_ICC_EXP;
	}


	if (EnDataBuffer[B_ICC_ICPKEYFLAG] != 0x01) {
		ERROR("ERROR-:IC KEY Retrieval of Publish Certification PKEY flag <> 1\r\n");
		return qPBOC_ICC_PKEYF;
	}

	if (EnDataBuffer[B_ICC_SHAFLAG] != 0x01) {
		ERROR("ERROR-:IC KEY Retrieval of Publish Certification PKEY flag <> 1\r\n");
		return qPBOC_SHA_FLAG;
	}

	IcKey.n_KeyLen = EnDataBuffer[B_ICC_PKEYLEN];
	if (IcKey.n_KeyLen <= (PublishKey.n_KeyLen - 42)) {
		memcpy(IcKey.n_Key, EnDataBuffer + B_ICC_ICPKEYLEFT, IcKey.n_KeyLen);
	}
	else {
		memcpy(IcKey.n_Key, EnDataBuffer + B_ICC_ICPKEYLEFT, (PublishKey.n_KeyLen - 21 - 21));
		if (QpbocTVLFCI.t_9F48.SizeOff != 0) {
			get_BER_TVL(QpbocTVLFCI.t_9F48.SizeOff, &TempTVL);
			memcpy(IcKey.n_Key + (PublishKey.n_KeyLen - 21 - 21), TempTVL.V, TempTVL.L);
		}
	}

	get_BER_TVL(QpbocTVLFCI.t_9F47.SizeOff, &TempTVL);
	lTempLong.LongBuff = 0;
	for (i = 0; i < EnDataBuffer[B_ICC_PKEYELEN]; i++) lTempLong.CharBuff[i] = TempTVL.V[TempTVL.L - 1 - i];
	IcKey.e_Key = lTempLong.LongBuff;
	return qPBOC_SUCESS_ECU;
}

static int QpbocfDDAOK(void)
{
	int ret = 0;
	INT32U i;
	INT8U Buffer[500], EnDataBuffer[260], ShaData[20], *p, temp, *q;
	INT16U Tag;
	BER_TVL TempTVL, TagTVL;
	TAG_OFFSTU pTagBit;
	INT32U OutLen;


	MSG_LOG("**************  QpbocfDDAOK ************\r\n");
	ret = ret;
	if (QpbocTVLFCI.t_9F4B.SizeOff == 0) return qPBOC_DDA_DATAFORMAT;

	get_BER_TVL(QpbocTVLFCI.t_9F4B.SizeOff, &TempTVL);
	//	printf("ICC_PkeyLen =%d\r\n",ICC_PkeyLen);
	if (TempTVL.L != IcKey.n_KeyLen) {
		ERROR("ERROR-:fDDA Data Len(%d) != IC Publich Key Len\r\n", TempTVL.L);
		return qPBOC_DDA_DATAKEYLEN;
	}

	memset(EnDataBuffer, 0, 260);
	//	debugstring("QpbocfDDAOK\r\n");
	if ((ret = RSA_SLF_public_decrypt(TempTVL.V, EnDataBuffer, IcKey, RSA_NO_PADDING)) != 0) {
		ERROR("ERROR-:RSA_IC public_decrypt  is %d\r\n", ret);
		return qPBOC_RSA_ERROR;
	}
	MSG_LOG("fDDA RSA Result=%s\r\n", BCD2ASC(EnDataBuffer, IcKey.n_KeyLen));
	if ((EnDataBuffer[0] != 0x6A) || (EnDataBuffer[1] != 0x05) || (EnDataBuffer[IcKey.n_KeyLen - 1] != 0xBC)) {
		ERROR("ERROR-:Retrieval of fDDA Certification Format EnDataBuffer[0]=%02X,EnDataBuffer[1]=%02X,EnDataBuffer[AutenCenterKeyLen-1]=%02X\r\n",
			EnDataBuffer[0], EnDataBuffer[1], EnDataBuffer[IcKey.n_KeyLen - 1]);
		return qPBOC_PUBLISHC_FORMAT;
	}
	// 哈希算法标识
	if (EnDataBuffer[2] != 0x01) {
		ERROR(" SHA Flag is error\r\n");
		return qPBOC_SHA_FLAG;
	}

	memset(Buffer, 0, sizeof(Buffer));
	OutLen = 0;
	memcpy(Buffer + OutLen, EnDataBuffer + 1, (IcKey.n_KeyLen - 1 - 1 - 20));
	OutLen += (IcKey.n_KeyLen - 1 - 1 - 20);
	if (QpbocTVLFCI.t_9F49.SizeOff != 0) {
		get_BER_TVL(QpbocTVLFCI.t_9F49.SizeOff, &TempTVL);
		p = TempTVL.V;
		while ((INT32U)(p - TempTVL.V) < TempTVL.L) {
			temp = *p;
			if ((*p++ & 0x1F) == 0x1F) {
				Tag = (short)temp * 256 + *p++;
				if ((*p & 0x80) == 0x80) {
					temp = *p++ & 0x7F;
					p += temp;
				}
				else {
					p += 1;
				}
			}
			else {
				Tag = (INT16U)temp;
				if ((*p & 0x80) == 0x80) {
					temp = *p++ & 0x7F;
					p += temp;
				}
				else {
					p += 1;
				}
			}
			q = (INT8U *)&QpbocTVLFCI;
			for (i = 0; i < (sizeof(BIT_TVL_TAG) / 8); i++) {
				memcpy((INT8U *)&pTagBit, q + sizeof(TAG_OFFSTU)*i, sizeof(TAG_OFFSTU));
				//			printf("Tag =%4X\r\n",pTagBit.TagValue);
				if (pTagBit.TagValue == Tag) {
					get_BER_TVL(pTagBit.SizeOff, &TagTVL);
					memcpy(Buffer + OutLen, TagTVL.V, TagTVL.L);
					OutLen += TagTVL.L;
				}
			}
		}
	}
	else {
		if (QpbocTVLFCI.t_9F37.SizeOff != 0) {
			get_BER_TVL(QpbocTVLFCI.t_9F37.SizeOff, &TempTVL);
			memcpy(Buffer + OutLen, TempTVL.V, TempTVL.L);
		}
		OutLen += TempTVL.L;
	}
	MSG_LOG("Sha Len =%d\r\n", OutLen);
	MSG_LOG("Sha Souce =%s\r\n", BCD2ASC(Buffer, OutLen));


	// 计算哈希结果
	memset(ShaData, 0, 20);
	SHA1(Buffer, OutLen, ShaData);
	MSG_LOG("QfDDAOK SHA1 =%s\r\n", BCD2ASC(ShaData, 20));

	//比较SHA结果
	if (memcmp(ShaData, EnDataBuffer + IcKey.n_KeyLen - 1 - 20, 20) != 0) {
		ERROR("ERROR-:Retrieval of Publish Certification SHA is error\r\n");
		return qPBOC_DDA_SIGN;
	}
	QpbocTVLFCI.t_9F4C.TagValue = 0x9F4C;
	QpbocTVLFCI.t_9F4C.SizeOff = bit_TVLData;
	memcpy(QpbocTVLData + bit_TVLData, "\x9F\x4C", 2);
	bit_TVLData += 2;
	QpbocTVLData[bit_TVLData++] = EnDataBuffer[3];
	memcpy(QpbocTVLData + bit_TVLData, EnDataBuffer + 4, EnDataBuffer[3]);
	memcpy(TradeResult.D_9F4C, EnDataBuffer + 4, EnDataBuffer[3]);
	bit_TVLData += EnDataBuffer[3];
	miniDispstr(15, 0, "OK", 0);
	//	debugstring("************OK\r\n");
	return 0;
}


static int QpbocDDAOK(void)
{
	int ret = 0;
	INT32U i;
	INT8U Buff[500], EnDataBuffer[260], ShaData[20], *p, temp, *q;
	INT16U Tag;
	BER_TVL TempTVL, TagTVL;
	TAG_OFFSTU pTagBit;
	INT32U OutLen;
	int len = 0;//, tmpI = 0;
	int send_len = 0;


	MSG_LOG("**************  QpbocDDAOK ************\r\n");

	memcpy(Buff, "\x00\x00\x88\x00\x00\x00", 6);
	Buff[0] = PCBSerial;
	PCBSerial ^= 1;
	//memcpy(Buff + 6, "\x83\x00", 2);
	memset(qpbocrevbuf, 0, sizeof(qpbocrevbuf));
	if (QpbocTVLFCI.t_9F49.SizeOff != 0) {
		get_BER_TVL(QpbocTVLFCI.t_9F49.SizeOff, &TempTVL);
		DOL_DataFill(Buff + 6, (INT32U*)&len, &TempTVL);
	}
	else
	{	// 缺省DDOL
		get_BER_TVL(QpbocTVLFCI.t_9F37.SizeOff, &TempTVL);
		//DOL_DataFill(Buff + 6, &len, &TempTVL);
		memcpy(Buff + 6, TempTVL.V, TempTVL.L);
		len = TempTVL.L;
	}
	Buff[5] = len;
	// CPU_APDU sndDATA=0280A800002383212800000000000000000100000000000001560000000000015610100100AE9BF3C8,0
	// CPU_APDU sndDATA=0280A800002383212800000000000000000100000000000001560000000000015610100100AE9BF5E5,0
	// CPU_APDU sndDATA=0280A800002383212800000000000000000100000000000001560000000000015611092000CD9FE7AC,0 $CPU_APDU:
	send_len = len + 6;
	MSG_LOG("CPU_APDU sndDATA=%s,%d\r\n", BCD2ASC(Buff, send_len), send_len);
	ret = RfPassthrough(qpbocrevbuf, 2, Buff + 1, send_len - 1);
	clr_wdt();
	if (ret == 0) {
		ERROR("ERROR:CPU_APDU is %d\r\n\r\n\r\n\r\n", ret);
		return qPBOC_APDU_CMD;
	}
	MSG_LOG("$CPU_APDU:\r\n");
	MSG_LOG("\tLength=%d,DATA=%s,\r\n", ret, BCD2ASC(qpbocrevbuf, ret));
	if (memcmp(qpbocrevbuf + ret - 2, "\x90\x00", 2) != 0)
	{
		ERROR("ERROR-PDOL:stuApduRet.SW =%02X%02X\r\n", qpbocrevbuf[ret - 2], qpbocrevbuf[ret - 1]);

		return qPBOC_APDU_CMD;
	}
	if (qpbocrevbuf[0 + 1] != 0x80) {
		return qPBOC_DATA_NO70;
	}
	if (qpbocrevbuf[0 + 2] == 0x81) {
		//return qPBOC_DATA_NO70;
		TempTVL.L = qpbocrevbuf[0 + 3];
		memcpy(TempTVL.V, qpbocrevbuf + 4, TempTVL.L);
	}
	else {
		TempTVL.L = qpbocrevbuf[0 + 2];
		memcpy(TempTVL.V, qpbocrevbuf + 3, TempTVL.L);
	}

	//if (QpbocTVLFCI.t_9F4B.SizeOff == 0) return qPBOC_DDA_DATAFORMAT;

	//get_BER_TVL(QpbocTVLFCI.t_9F4B.SizeOff, &TempTVL);
	//	printf("ICC_PkeyLen =%d\r\n",ICC_PkeyLen);
	if (TempTVL.L != IcKey.n_KeyLen) {
		ERROR("ERROR-:DDA Data Len(%d) != IC Publich Key Len\r\n", TempTVL.L);
		return qPBOC_DDA_DATAKEYLEN;
	}

	memset(EnDataBuffer, 0, 260);
	//	debugstring("QpbocDDAOK\r\n");
	if ((ret = RSA_SLF_public_decrypt(TempTVL.V, EnDataBuffer, IcKey, RSA_NO_PADDING)) != 0) {
		ERROR("ERROR-:RSA_IC public_decrypt  is %d\r\n", ret);
		return qPBOC_RSA_ERROR;
	}
	MSG_LOG("DDA RSA Result=%s\r\n", BCD2ASC(EnDataBuffer, IcKey.n_KeyLen));
	if ((EnDataBuffer[0] != 0x6A) || (EnDataBuffer[1] != 0x05) || (EnDataBuffer[IcKey.n_KeyLen - 1] != 0xBC)) {
		ERROR("ERROR-:Retrieval of DDA Certification Format EnDataBuffer[0]=%02X,EnDataBuffer[1]=%02X,EnDataBuffer[AutenCenterKeyLen-1]=%02X\r\n",
			EnDataBuffer[0], EnDataBuffer[1], EnDataBuffer[IcKey.n_KeyLen - 1]);
		return qPBOC_PUBLISHC_FORMAT;
	}
	// 哈希算法标识
	if (EnDataBuffer[2] != 0x01) {
		ERROR(" SHA Flag is error\r\n");
		return qPBOC_SHA_FLAG;
	}

	memset(Buff, 0, sizeof(Buff));
	OutLen = 0;
	memcpy(Buff + OutLen, EnDataBuffer + 1, (IcKey.n_KeyLen - 1 - 1 - 20));
	OutLen += (IcKey.n_KeyLen - 1 - 1 - 20);
	if (QpbocTVLFCI.t_9F49.SizeOff != 0) {
		get_BER_TVL(QpbocTVLFCI.t_9F49.SizeOff, &TempTVL);
		p = TempTVL.V;
		while ((INT32U)(p - TempTVL.V) < TempTVL.L) {
			temp = *p;
			if ((*p++ & 0x1F) == 0x1F) {
				Tag = (short)temp * 256 + *p++;
				if ((*p & 0x80) == 0x80) {
					temp = *p++ & 0x7F;
					p += temp;
				}
				else {
					p += 1;
				}
			}
			else {
				Tag = (INT16U)temp;
				if ((*p & 0x80) == 0x80) {
					temp = *p++ & 0x7F;
					p += temp;
				}
				else {
					p += 1;
				}
			}
			q = (INT8U *)&QpbocTVLFCI;
			for (i = 0; i < (sizeof(BIT_TVL_TAG) / 8); i++) {
				memcpy((INT8U *)&pTagBit, q + sizeof(TAG_OFFSTU)*i, sizeof(TAG_OFFSTU));
				//			printf("Tag =%4X\r\n",pTagBit.TagValue);
				if (pTagBit.TagValue == Tag) {
					get_BER_TVL(pTagBit.SizeOff, &TagTVL);
					memcpy(Buff + OutLen, TagTVL.V, TagTVL.L);
					OutLen += TagTVL.L;
				}
			}
		}
	}
	else {
		if (QpbocTVLFCI.t_9F37.SizeOff != 0) {
			get_BER_TVL(QpbocTVLFCI.t_9F37.SizeOff, &TempTVL);
			memcpy(Buff + OutLen, TempTVL.V, TempTVL.L);
		}
		OutLen += TempTVL.L;
	}
	MSG_LOG("Sha Len =%d\r\n", OutLen);
	MSG_LOG("Sha Souce =%s\r\n", BCD2ASC(Buff, OutLen));


	// 计算哈希结果
	memset(ShaData, 0, 20);
	SHA1(Buff, OutLen, ShaData);
	MSG_LOG("QDDAOK SHA1 =%s\r\n", BCD2ASC(ShaData, 20));

	//比较SHA结果
	if (memcmp(ShaData, EnDataBuffer + IcKey.n_KeyLen - 1 - 20, 20) != 0) {
		ERROR("ERROR-:Retrieval of Publish Certification SHA is error\r\n");
		return qPBOC_DDA_SIGN;
	}
	QpbocTVLFCI.t_9F4C.TagValue = 0x9F4C;
	QpbocTVLFCI.t_9F4C.SizeOff = bit_TVLData;
	memcpy(QpbocTVLData + bit_TVLData, "\x9F\x4C", 2);
	bit_TVLData += 2;
	QpbocTVLData[bit_TVLData++] = EnDataBuffer[3];
	memcpy(QpbocTVLData + bit_TVLData, EnDataBuffer + 4, EnDataBuffer[3]);
	memcpy(TradeResult.D_9F4C, EnDataBuffer + 4, EnDataBuffer[3]);
	bit_TVLData += EnDataBuffer[3];
	miniDispstr(15, 0, "OK", 0);
	//	debugstring("************OK\r\n");
	return 0;
}


static int QpbocSDAOK(void)
{
	int ret = 0;
	int i;
	INT8U Buffer[500], EnDataBuffer[260], ShaData[20];//, *p, temp;//, *q;
//	INT16U Tag;
	BER_TVL TempTVL;//, TagTVL;
//	TAG_OFFSTU pTagBit;
	INT32U OutLen;


	MSG_LOG("**************  QpbocSDAOK ************\r\n");
	ret = ret;
	if (QpbocTVLFCI.t_93.SizeOff == 0) return qPBOC_SDA_DATAFORMAT;

	get_BER_TVL(QpbocTVLFCI.t_93.SizeOff, &TempTVL);
	//	printf("ICC_PkeyLen =%d\r\n",ICC_PkeyLen);
	if (TempTVL.L != PublishKey.n_KeyLen) {
		ERROR("ERROR-:SDA Data Len(%d) != SSD Len\r\n", TempTVL.L);
		return qPBOC_DDA_DATAKEYLEN;
	}

	memset(EnDataBuffer, 0, 260);
	//	debugstring("QpbocDDAOK\r\n");
	if ((ret = RSA_SLF_public_decrypt(TempTVL.V, EnDataBuffer, PublishKey, RSA_NO_PADDING)) != 0) {
		ERROR("ERROR-:RSA_ISS public_decrypt  is %d\r\n", ret);
		return qPBOC_RSA_ERROR;
	}
	MSG_LOG("SDA RSA Result=%s\r\n", BCD2ASC(EnDataBuffer, PublishKey.n_KeyLen));
	if ((EnDataBuffer[0] != 0x6A) || (EnDataBuffer[1] != 0x03) || (EnDataBuffer[PublishKey.n_KeyLen - 1] != 0xBC)) {
		ERROR("ERROR-:Retrieval of SSD Format EnDataBuffer[0]=%02X,EnDataBuffer[1]=%02X,EnDataBuffer[AutenCenterKeyLen-1]=%02X\r\n",
			EnDataBuffer[0], EnDataBuffer[1], EnDataBuffer[PublishKey.n_KeyLen - 1]);
		return qPBOC_SDA_DATAFORMAT;
	}
	// 哈希算法标识
	if (EnDataBuffer[2] != 0x01) {
		ERROR(" SHA Flag is error\r\n");
		return qPBOC_SHA_FLAG;
	}
	// 检查填充的BB
	for (i = 5; i < PublishKey.n_KeyLen - 26; ++i)
	{
		if (EnDataBuffer[i] != 0xBB) {
			ERROR(" Padding 0xBB is error:%02X\r\n", EnDataBuffer[i]);
			return qPBOC_SHA_FLAG;
		}
	}

	OutLen = 0;
	memcpy(Buffer + OutLen, EnDataBuffer + 1, (PublishKey.n_KeyLen - 1 - 1 - 20));
	OutLen += (PublishKey.n_KeyLen - 1 - 1 - 20);

	memcpy(Buffer + OutLen, SDADataAuten, SDADataALen);
	OutLen += SDADataALen;

	if (QpbocTVLFCI.t_9F4A.SizeOff != 0) {
		get_BER_TVL(QpbocTVLFCI.t_9F4A.SizeOff, &TempTVL);
		get_BER_TVL(QpbocTVLFCI.t_82.SizeOff, &TempTVL);
		memcpy(Buffer + OutLen, TempTVL.V, TempTVL.L);
		OutLen += TempTVL.L;
	}

	MSG_LOG("Sha Len =%d\r\n", OutLen);
	MSG_LOG("Sha Souce =%s\r\n", BCD2ASC(Buffer, OutLen));


	// 计算哈希结果
	memset(ShaData, 0, 20);
	SHA1(Buffer, OutLen, ShaData);
	MSG_LOG("QSDAOK SHA1 =%s\r\n", BCD2ASC(ShaData, 20));

	//比较SHA结果
	if (memcmp(ShaData, EnDataBuffer + PublishKey.n_KeyLen - 1 - 20, 20) != 0) {
		ERROR("ERROR-:Retrieval of SSD SHA is error\r\n");
		return qPBOC_SDA_SIGN;
	}

	QpbocTVLFCI.t_9F45.TagValue = 0x9F45;
	QpbocTVLFCI.t_9F45.SizeOff = bit_TVLData;
	memcpy(QpbocTVLData + bit_TVLData, "\x9F\x45\x02", 3);
	bit_TVLData += 3;
	memcpy(QpbocTVLData + bit_TVLData, EnDataBuffer + 3, 2);
	//memcpy(TradeResult.D_9F4C, EnDataBuffer + 4, EnDataBuffer[3]);
	bit_TVLData += 2;

	miniDispstr(15, 0, "OK", 0);
	//	debugstring("************OK\r\n");
	return 0;
}

int QpbocOfflineAuten(void)
{//计算时长：41ms、17ms。不一样的卡不一样。最长的为41ms
	int Retn = 0;
	INT32U AutenType = 0;
	BER_TVL TempTVL;

	//	PKI_Reset();
	get_BER_TVL(QpbocTVLFCI.t_82.SizeOff, &TempTVL);
	if ((TempTVL.V[0] & AIP_DDA) == AIP_DDA) {
		if ((PosOfferData.AutenData.bPosCapabilty[2] & CA_DDA) == CA_DDA) {
			AutenType = TYPE_AUTEN_DDA;
			goto start_pos_offline_auten;
		}
	}
	if ((TempTVL.V[0] & AIP_SDA) == AIP_SDA) {
		if ((PosOfferData.AutenData.bPosCapabilty[2] & CA_SDA) == CA_SDA) {
			AutenType = TYPE_AUTEN_SDA;
			goto start_pos_offline_auten;
		}
	}

	ERROR("TVR:QpbocOfflineAuten-AUTEN   is NO\r\n");
	PosOfferData.AutenData.TVR[0] |= TVR_NO_OFFLINEAUTEN;
	AutenType = TYPE_AUTEN_NO;
	return qPBOC_NOFIND_AUTEN;

start_pos_offline_auten:
	if (AutenType == TYPE_AUTEN_DDA) {
		MSG_LOG("脱机数据认证类型:DDA\n");
	}
	else {
		MSG_LOG("脱机数据认证类型:SDA\n");
	}
	if (QpbocTVLFCI.t_8F.SizeOff == 0) {			// 认证中心公钥索引
		ERROR("TVR:QpbocOfflineAuten-8F  is NO\r\n");
		PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		ERROR("ERROR-:qPBOC_CAINDEX_NOEXIT\r\n");
		Retn = qPBOC_CAINDEX_NOEXIT;
		return Retn;
	}

	get_BER_TVL(QpbocTVLFCI.t_82.SizeOff, &TempTVL);
	if (QpbocTVLFCI.t_90.SizeOff == 0) {		// 发卡行公钥证书
		ERROR("TVR:QpbocOfflineAuten-90  is NO\r\n");
		PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		ERROR("ERROR-:qPBOC_PUBLISHC_NOEXIT\r\n");
		Retn = qPBOC_PUBLISHC_NOEXIT;
		return Retn;
	}
	if (QpbocTVLFCI.t_9F32.SizeOff == 0) {		// 发卡行公钥指数
		ERROR("TVR:QpbocOfflineAuten-32  is NO\r\n");
		PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
		ERROR("ERROR-:qPBOC_PUBLISHE_NOEXIT\r\n");
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		Retn = qPBOC_PUBLISHE_NOEXIT;
		return Retn;
	}


	MSG_LOG("QpbocAppList[ValidAppNum].DF_AID=%s\r\n", BCD2ASC(QpbocAppList[ValidAppNum].DF_AID, 5));
	if (memcmp(QpbocAppList[ValidAppNum].DF_AID, "\x00\x00\x00\x00\x00", 5) == 0) {
		ERROR("TVR:QpbocOfflineAuten-AID  is NO\r\n");
		PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		ERROR("ERROR-:qPBOC_CAKEYRID_NOEXIT\r\n");
		Retn = qPBOC_CAKEYRID_NOEXIT;
		return Retn;
	}
	if (QpbocTVLFCI.t_9F4A.SizeOff != 0) {
		get_BER_TVL(QpbocTVLFCI.t_9F4A.SizeOff, &TempTVL);
		if ((TempTVL.L != 1) || (TempTVL.V[0] != 0x82)) {
			ERROR("TVR:QpbocOfflineAuten-9F49 82  is No:%s\r\n", BCD2ASC(TempTVL.V, TempTVL.L));
			PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
			PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
			ERROR("ERROR-:qPBOC_SIGNSTATIC_NOEXIT\r\n");
			Retn = qPBOC_SIGNSTATIC_NOEXIT;
			return Retn;
		}
	}

	if (QpbocTVLFCI.t_94.SizeOff == 0) {
		ERROR("TVR:QpbocOfflineAuten-94  is NO\r\n");
		PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		ERROR("ERROR-:qPBOC_AFL_NOEXIT\r\n");
		Retn = qPBOC_AFL_NOEXIT;
		return Retn;
	}
	if (QpbocGetAutenCerPKEY() != 0) {
		ERROR("TVR22:QpbocOfflineAuten-CAKEY  is NO\r\n");
		PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		ERROR("ERROR22-qPBOC_CAPKEY_NOEXIT\r\n");
		Retn = qPBOC_CAPKEY_NOEXIT;
		return Retn;
	}

	if (AuterCerKey.n_KeyLen == 0) {
		ERROR("TVR:QpbocOfflineAuten-CAKEY  is NO\r\n");
		PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		ERROR("ERROR-qPBOC_CAPKEY_NOEXIT\r\n");
		Retn = qPBOC_CAPKEY_NOEXIT;
		return Retn;
	}

	if ((Retn = QpbocGetPublishPKEY()) != qPBOC_SUCESS_ECU) {
		ERROR("TVR:QpbocOfflineAuten-PKEY  is NO:%d\r\n", Retn);
		PosOfferData.AutenData.TVR[0] |= TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		return Retn;
	}
	if (AutenType == TYPE_AUTEN_SDA) {		//SDA			
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		if (QpbocTVLFCI.t_93.SizeOff == 0) {
			ERROR("TVR:QpbocOfflineAuten-93  is NO\r\n");
			PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
			PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
			ERROR("ERROR-:签名的静态应用数据\r\n");
			Retn = qPBOC_SIGNSTATIC_NOEXIT;
			return Retn;
		}
		if ((Retn = QpbocSDAOK()) != qPBOC_SUCESS_ECU) {
			ERROR("TVR:QpbocOfflineAuten-SDA is FAIL:%d\r\n", Retn);
			PosOfferData.AutenData.TVR[0] |= TVR_OFFLINEAUTEN_FAILE;
			//PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
			return Retn;
		}
		return qPBOC_SUCESS_ECU;
	}
	// 进行有效的脱机认证
	if (AutenType == TYPE_AUTEN_CDA) {		//CDA
		ERROR("TVR:QpbocOfflineAuten-CDA is FAIL\r\n");
		PosOfferData.AutenData.TVR[0] |= TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		Retn = qPBOC_NO_CDA;
		return Retn;
	}
	if (QpbocTVLFCI.t_9F46.SizeOff == 0) {
		ERROR("TVR:QpbocOfflineAuten-9F46  is NO\r\n");
		PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		Retn = qPBOC_ICC_NOEXIT;
		return Retn;
	}
	if (QpbocTVLFCI.t_9F47.SizeOff == 0) {
		ERROR("TVR:QpbocOfflineAuten-9F47  is NO\r\n");
		PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		Retn = qPBOC_ICCE_NOEXIT;
		return Retn;
	}
	if (QpbocTVLFCI.t_9F4B.SizeOff == 0 && (PosOfferData.PDOL.bPosAttr9F66[0] & 0x60) == 0x20) {
		ERROR("TVR:QpbocOfflineAuten-9F4B  is NO\r\n");
		PosOfferData.AutenData.TVR[0] |= TVR_ICDATA_LOST | TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		Retn = qPBOC_ICSIGNDYDATA_NOEXIT;
		return Retn;
	}
	if (QpbocTVLFCI.t_9F37.SizeOff == 0) {
		ERROR("TVR:QpbocOfflineAuten-9F37  is NO\r\n");
		//PosOfferData.AutenData.TVR[0] |=TVR_ICDATA_LOST|TVR_OFFLINEAUTEN_FAILE;
		//PosOfferData.AutenData.TSI[0] |=TSI_OFFAUTEN_EXE;
		//Retn =qPBOC_NOUNDOWNDATA_NOEXIT;
		//return Retn;
	}

	if ((Retn = QpbocGetICPKEY()) != qPBOC_SUCESS_ECU) {
		ERROR("TVR:QpbocOfflineAuten-ICKEY is FAIL:%d\r\n", Retn);
		PosOfferData.AutenData.TVR[0] |= TVR_OFFLINEAUTEN_FAILE;
		PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
		return Retn;
	}
	// 进行有效的脱机认证
	if (AutenType == TYPE_AUTEN_DDA && (PosOfferData.PDOL.bPosAttr9F66[0] & 0x60) == 0x20) {		//DDA
		// fDDA
		if ((Retn = QpbocfDDAOK()) != qPBOC_SUCESS_ECU) {
			ERROR("TVR:QpbocOfflineAuten-fDDA is FAIL:%d\r\n", Retn);
			PosOfferData.AutenData.TVR[0] |= TVR_OFFLINEAUTEN_FAILE;
			PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
			return Retn;
		}
	}
	else {	//DDA
		if ((Retn = QpbocDDAOK()) != qPBOC_SUCESS_ECU) {
			ERROR("TVR:QpbocOfflineAuten-DDA is FAIL:%d\r\n", Retn);
			PosOfferData.AutenData.TVR[0] |= TVR_OFFLINEAUTEN_FAILE;
			PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
			return Retn;
		}
	}
	PosOfferData.AutenData.TSI[0] |= TSI_OFFAUTEN_EXE;
	// 	AutenType = get_timer0(2);	//检测计算时长：41ms
	// 	printf("qPbocOfflineA Times:%d,%d\r\n", AutenType, (5000-AutenType));
	return qPBOC_SUCESS_ECU;
}

//---------------------公钥处理---------------------

#define PKEY_Mulu_offset 128
#define PKEY_e_offset 256
#define PKEY_MOZHI_offset 768
#define PKEY_e_len 4

//公钥解密
int	RSA_SLF_public_decrypt(unsigned char *from,
	unsigned char *to,
	KEY_PUBLICMSG Key,
	int padding)
{
	unsigned int rlen;
	int i;
	R_RANDOM_STRUCT randomStruct;
	R_RSA_PUBLIC_KEY rrk;

	rrk.bits = Key.n_KeyLen * 8;
	memcpy(rrk.modulus, Key.n_Key, Key.n_KeyLen);
	rrk.exponent[0] = ((Key.e_Key >> 16) & 0xFF);
	rrk.exponent[1] = ((Key.e_Key >> 8) & 0xFF);
	rrk.exponent[2] = (Key.e_Key & 0xFF);

	BCD_LOG((unsigned char*)&Key.e_Key, 4, 1);

	MSG_LOG("rrk.exponent:%02X%02X%02X\r\n", rrk.exponent[0], rrk.exponent[1], rrk.exponent[2]);

	i = RSAPublicEncrypt(to, &rlen, from, Key.n_KeyLen, &rrk, &randomStruct);
	if (i != 0) {
		ERROR("SIM APUD-2 is ERROR:%d\r\n", i);
		return qPBOC_RSA_ERROR;
	}
	return qPBOC_SUCESS_ECU;
}

static int SHA1(unsigned char *d, int n, unsigned char *md)
{
	return DigestInfo(d, n, md);

}

//装入默认公钥,在第一次刷卡时，如果没有公钥，就会载入。
void qPbocLoadDefaultPublicKey(void)
{	//这段代码占用空间为3044字节。
	unsigned char pbuff[256];
	flasherase(ERASE32KBYTESECTOR, FLASH_PUBLICKEY_START);
	Ascii2BCD("D2F8D0D0B9ABD4BF0100000020151123150315FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCDAD2764",
		pbuff, 128 * 2);
	flashwrite(FLASH_PUBLICKEY_START, pbuff, 128);//参数区
	Ascii2BCD("02030408090A0B00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000069A9022A",
		pbuff, 128 * 2);
	flashwrite(FLASH_PUBLICKEY_START + PKEY_Mulu_offset, pbuff, 128);//目录区,当前为7组公钥
	Ascii2BCD("000390000003B0000003F800000390000003B000000380000003F800", pbuff, (7 * 4 * 2));
	flashwrite(FLASH_PUBLICKEY_START + PKEY_e_offset, pbuff, (7 * 4));	//指数区
	//下面是7组模值
	Ascii2BCD("A3767ABD1B6AA69D7F3FBF28C092DE9ED1E658BA5F0909AF7A1CCD907373B7210FDEB16287BA8E78E1529F443976FD27F991EC67D95E5F4E96B127CAB2396A94D6E45CDA44CA4C4867570D6B07542F8D4BF9FF97975DB9891515E66F525D2B3CBEB6D662BFB6C3F338E93B02142BFC44173A3764C56AADD202075B26DC2F9F7D7AE74BD7D00FD05EE430032663D27A57",
		pbuff, 144 * 2);
	flashwrite(FLASH_PUBLICKEY_START + PKEY_MOZHI_offset, pbuff, 144);
	Ascii2BCD("B0627DEE87864F9C18C13B9A1F025448BF13C58380C91F4CEBA9F9BCB214FF8414E9B59D6ABA10F941C7331768F47B2127907D857FA39AAF8CE02045DD01619D689EE731C551159BE7EB2D51A372FF56B556E5CB2FDE36E23073A44CA215D6C26CA68847B388E39520E0026E62294B557D6470440CA0AEFC9438C923AEC9B2098D6D3A1AF5E8B1DE36F4B53040109D89B77CAFAF70C26C601ABDF59EEC0FDC8A99089140CD2E817E335175B03B7AA33D",
		pbuff, 176 * 2);
	flashwrite(FLASH_PUBLICKEY_START + PKEY_MOZHI_offset + 256, pbuff, 176);
	Ascii2BCD("BC853E6B5365E89E7EE9317C94B02D0ABB0DBD91C05A224A2554AA29ED9FCB9D86EB9CCBB322A57811F86188AAC7351C72BD9EF196C5A01ACEF7A4EB0D2AD63D9E6AC2E7836547CB1595C68BCBAFD0F6728760F3A7CA7B97301B7E0220184EFC4F653008D93CE098C0D93B45201096D1ADFF4CF1F9FC02AF759DA27CD6DFD6D789B099F16F378B6100334E63F3D35F3251A5EC78693731F5233519CDB380F5AB8C0F02728E91D469ABD0EAE0D93B1CC66CE127B29C7D77441A49D09FCA5D6D9762FC74C31BB506C8BAE3C79AD6C2578775B95956B5370D1D0519E37906B384736233251E8F09AD79DFBE2C6ABFADAC8E4D8624318C27DAF1",
		pbuff, 248 * 2);
	flashwrite(FLASH_PUBLICKEY_START + PKEY_MOZHI_offset + (2 * 256), pbuff, 248);
	Ascii2BCD("B61645EDFD5498FB246444037A0FA18C0F101EBD8EFA54573CE6E6A7FBF63ED21D66340852B0211CF5EEF6A1CD989F66AF21A8EB19DBD8DBC3706D135363A0D683D046304F5A836BC1BC632821AFE7A2F75DA3C50AC74C545A754562204137169663CFCC0B06E67E2109EBA41BC67FF20CC8AC80D7B6EE1A95465B3B2657533EA56D92D539E5064360EA4850FED2D1BF",
		pbuff, 144 * 2);
	flashwrite(FLASH_PUBLICKEY_START + PKEY_MOZHI_offset + (3 * 256), pbuff, 144);
	Ascii2BCD("EB374DFC5A96B71D2863875EDA2EAFB96B1B439D3ECE0B1826A2672EEEFA7990286776F8BD989A15141A75C384DFC14FEF9243AAB32707659BE9E4797A247C2F0B6D99372F384AF62FE23BC54BCDC57A9ACD1D5585C303F201EF4E8B806AFB809DB1A3DB1CD112AC884F164A67B99C7D6E5A8A6DF1D3CAE6D7ED3D5BE725B2DE4ADE23FA679BF4EB15A93D8A6E29C7FFA1A70DE2E54F593D908A3BF9EBBD760BBFDC8DB8B54497E6C5BE0E4A4DAC29E5",
		pbuff, 176 * 2);
	flashwrite(FLASH_PUBLICKEY_START + PKEY_MOZHI_offset + (4 * 256), pbuff, 176);
	Ascii2BCD("B2AB1B6E9AC55A75ADFD5BBC34490E53C4C3381F34E60E7FAC21CC2B26DD34462B64A6FAE2495ED1DD383B8138BEA100FF9B7A111817E7B9869A9742B19E5C9DAC56F8B8827F11B05A08ECCF9E8D5E85B0F7CFA644EFF3E9B796688F38E006DEB21E101C01028903A06023AC5AAB8635F8E307A53AC742BDCE6A283F585F48EF",
		pbuff, 128 * 2);
	flashwrite(FLASH_PUBLICKEY_START + PKEY_MOZHI_offset + (5 * 256), pbuff, 128);
	Ascii2BCD("CF9FDF46B356378E9AF311B0F981B21A1F22F250FB11F55C958709E3C7241918293483289EAE688A094C02C344E2999F315A72841F489E24B1BA0056CFAB3B479D0E826452375DCDBB67E97EC2AA66F4601D774FEAEF775ACCC621BFEB65FB0053FC5F392AA5E1D4C41A4DE9FFDFDF1327C4BB874F1F63A599EE3902FE95E729FD78D4234DC7E6CF1ABABAA3F6DB29B7F05D1D901D2E76A606A8CBFFFFECBD918FA2D278BDB43B0434F5D45134BE1C2781D157D501FF43E5F1C470967CD57CE53B64D82974C8275937C5D8502A1252A8A5D6088A259B694F98648D9AF2CB0EFD9D943C69F896D49FA39702162ACB5AF29B90BADE005BC157",
		pbuff, 248 * 2);
	flashwrite(FLASH_PUBLICKEY_START + PKEY_MOZHI_offset + (6 * 256), pbuff, 248);
}

//判断公钥是否在设备中存在,返回值：大于0=找了版本号，并返回位置序号，否则存在
int qPBOC_KeyVerExist(unsigned char pKeyVer)
{
	unsigned char PkVerBuf[128];
	unsigned short t;
	unsigned int i;

	MSG_LOG("----公钥信息输出----\r\n");
	MSG_LOG("参数区:\r\n");
	for (t = 0; t < 3; t++) {
		flashread(FLASH_PUBLICKEY_START, PkVerBuf, 128);
		BCD_LOG(PkVerBuf, 128, 1);
		i = __cpuCrc32(PkVerBuf, 124);
		MSG_LOG("crc i=%08X\r\n", i);
		if (memcmp((unsigned char*)&i, PkVerBuf + 124, 4) != 0) {
			MSG_LOG("校验错误!\r\n");
		}
		else {
			break;
		}
	}
	if (t >= 3) {
		MSG_LOG("校验错误，没有公钥\r\n");
		MSG_LOG("把默认公钥保存到FLASH中\r\n");
		qPbocLoadDefaultPublicKey();	//这里写完后，目录区也正常。
	}
	MSG_LOG("参数区正常\r\n");

	MSG_LOG("目录区:保存的是公钥版本<=120组\r\n");
	for (t = 0; t < 3; t++) {
		flashread(FLASH_PUBLICKEY_START + PKEY_Mulu_offset, PkVerBuf, 128);
		BCD_LOG(PkVerBuf, 128, 1);
		i = __cpuCrc32(PkVerBuf, 124);
		MSG_LOG("crc i=%08X\r\n", i);
		if (memcmp((unsigned char*)&i, PkVerBuf + 124, 4) != 0) {
			MSG_LOG("校验错误!\r\n");
		}
		else {
			break;
		}
	}
	if (t >= 3) {
		MSG_LOG("校验错误，没有公钥\r\n");
		return -2;
	}
	MSG_LOG("目录区正常\r\n");

	for (i = 0; i < 120; i++) {
		if ((PkVerBuf[i] == 0xFF) || (PkVerBuf[i] == 0)) {//没有了
			MSG_LOG("--结束---\r\n");
			return -2;
		}
		else if (PkVerBuf[i] == pKeyVer) {
			MSG_LOG("  找到:%d,%d\r\n", i, PkVerBuf[i]);
			break;	//找到需要的版本号。
		}
	}
	if (i >= 120) {
		return -3;
	}
	return i;
}

//取出公钥信息：
//输入需要取的公钥版本号。
//输出此版本的公钥信息。
//如果没有此公钥，则返回-1，有则返回0
int qPBOCgetPublicKey(unsigned char pKeyVer, KEY_PUBLICMSG *pKeyOut)
{
	unsigned char buff[256];
	unsigned short t, e;
	int i;

	i = qPBOC_KeyVerExist(pKeyVer);
	if ((i < 0) || (i > 120)) {	//公钥给数量大120个
		return -1;
	}

	//找到了，读出指数 和 模值
	flashread(FLASH_PUBLICKEY_START + PKEY_e_offset + (i*PKEY_e_len), buff, PKEY_e_len);
	memcpy((unsigned char*)&e, buff, 2);	//指数
	RevertTurn(2, (unsigned char*)&e);
	memcpy((unsigned char*)&t, buff + 2, 2);	//模长
	MSG_LOG("   指数:%02X%02X,模长:%d\r\n", buff[0], buff[1], t);

	flashread(FLASH_PUBLICKEY_START + PKEY_MOZHI_offset + (i * 256), buff, t);	//读出模值
	MSG_LOG("    模值:\r\n");
	BCD_LOG(buff, t, 1);

	pKeyOut->e_Key = e;
	pKeyOut->n_KeyLen = t;
	memcpy(pKeyOut->n_Key, buff, t);
	return 0;
}

//测试-公钥信息输出
int checkSamKey(unsigned char mode)
{
	unsigned char buff[256];
	unsigned char buff1[256];
	unsigned short t;
	unsigned int i;

	debugstring("----公钥信息输出----\r\n");
	debugstring("参数区:\r\n");
	for (t = 0; t < 5; t++) {
		flashread(FLASH_PUBLICKEY_START, buff1, 128);
		debugdata(buff1, 128, 1);
		i = __cpuCrc32(buff1, 124);
		printf("crc i=%08X\r\n", i);
		if (memcmp((unsigned char*)&i, buff1 + 124, 4) != 0) {
			debugstring("校验错误，没有公钥\r\n");
		}
		else {
			break;
		}
	}

	if (t >= 5)
	{
		debugstring("校验错误，没有公钥\r\n");
		debugstring("刷一次闪付卡后载入默认公钥\r\n");
		return -1;
	}

	debugstring("参数区正常\r\n");

	debugstring("目录区:");
	flashread(FLASH_PUBLICKEY_START + PKEY_Mulu_offset, buff1, 128);
	debugdata(buff1, 128, 1);
	i = __cpuCrc32(buff1, 124);
	printf("crc i=%08X\r\n", i);
	if (memcmp((unsigned char*)&i, buff1 + 124, 4) != 0) {
		debugstring("校验错误，没有公钥目录\r\n");
		return -1;
	}
	debugstring("目录区正常\r\n");

	debugstring("指数+模长区:");
	flashread(FLASH_PUBLICKEY_START + PKEY_e_offset, buff, 256);
	debugdata(buff, 256, 1);

	for (i = 0; i < 120; i++) {
		if ((buff1[i] != 0xFF) && (buff1[i] != 0)) {//有版本信息
			printf("--版本号:%d", buff1[i]);
			flashread(FLASH_PUBLICKEY_START + PKEY_e_offset + (i*PKEY_e_len), buff, PKEY_e_len);
			memcpy((unsigned char*)&t, buff + 2, 2);	//模长
			printf("  指数:%02X%02X,模长:%d\r\n", buff[0], buff[1], t);

			flashread(FLASH_PUBLICKEY_START + PKEY_MOZHI_offset + (i * 256), buff, t);	//读出模值
			printf("    模值:\r\n");
			debugdata(buff, t, 1);
		}
		else {
			break;	//不合法，则结束。
		}
	}

	debugstring("\r\n----输出完成----\r\n");
	return 0;
}

//把下载到临时区的公钥移到公钥存贮区，用软件算RSA。
void qPbocDealPkey(void)
{
	unsigned int addr;
	char buff[1024];
	unsigned char FlashVerbuf[128];
	INT8U cbuf[20];
	char Ver[16];
	unsigned int i, keylen, keye, pkVer, pIndex;
	KEY_PUBLICMSG key;
	unsigned char keyVer;
	char *p, *p1;
	//	unsigned int crc;
	unsigned int tt;

	memset(FlashVerbuf, 0xFF, sizeof(FlashVerbuf));

	cls();
	display(0, 0, "公钥检测!", DIS_CENTER | DIS_CONVERT);
	flashread(FLASH_GPS_SOUND_START, (unsigned char*)buff, 32);
	if (memcmp(buff, "需要更新", 8) != 0) {//文件已经使用，不再更新
		return;
	}

#ifdef _debug_
	debugstring("OtheerFiles Ver:");
	debugstring((char*)buff);
#endif
	p1 = strstr((char*)buff, "文件版本");
	if (p1 == NULL)//文件没有版本，不合法
		return;
	memset(Ver, 0, sizeof(Ver));
	p = Ver;
	for (i = 0; i < 32; i++) {
		if ((*p1 >= '0') && (*p1 <= '9')) {//有效值
			*p++ = *p1;
		}
		else if (p != Ver) {//已经取得版本号，这里是版本号结束，如果没取到，说明还没有到版本号的地方
			break;
		}
		p1++;
	}
	i = __atol(Ver);
	pkVer = i;
	memcpy(gBusVerInfo.busVoiceVer, (unsigned char*)&i, 2);

	//有合法版本号，需要更新文件。覆盖方式，全部删除再更新
	flasherase(ERASE32KBYTESECTOR, FLASH_PUBLICKEY_START);
	pIndex = 0;

	for (addr = FLASH_GPS_SOUND_START; addr < FLASH_GPS_SOUND_END;)
	{
		memset(buff, 0, sizeof(buff));
		flashread(addr, (unsigned char*)buff, 1000);
		//		debugstring(buff);
		if (memcmp(buff, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 10) == 0)
			break;
		p = strstr(buff, "公钥索引");
		if (p == NULL) {//没有，找下一批1000字节
			MSG_LOG("E1 ");
			addr += 1000;
			continue;
		}
		p += 8;
		keyVer = 0;
		for (i = 0; i < 20; i++) {//取公钥索引
			if ((*p >= '0') && (*p <= '9')) {
				keyVer *= 10;
				keyVer += (*p - '0');
			}
			else {
				if (keyVer != 0)//如果不等0，说明已经找到索引，可以退出
					break;
			}
			p++;
		}
		if (keyVer == 0) {//索引不存在，下一循环
			addr += (p - buff);
			MSG_LOG("E2 ");
			continue;
		}
		keyVer /= 2;

		keylen = 0;//取公钥长度
		memcpy(cbuf, "公钥模长", 8);
		p1 = (char*)Find_CharData((INT8U*)p, 20, cbuf, 8);
		if (p1 == NULL) {//没有模长
			addr += (p - buff);
			MSG_LOG("E3 ");
			continue;
		}
		p = p1 + 8;
		for (i = 0; i < 20; i++) {//取公钥长度
			if ((*p >= '0') && (*p <= '9')) {
				keylen *= 10;
				keylen += (*p - '0');
			}
			else {
				if (keylen != 0)//如果不等0，说明已经找到长度，可以退出
					break;
			}
			p++;
		}
		if (keylen == 0) {//取公钥长度=0
			addr += (p - buff);
			MSG_LOG("E4 ");
			continue;
		}

		memcpy(cbuf, "公钥模值", 8);
		p1 = (char*)Find_CharData((INT8U*)p, 20, cbuf, 8);
		if (p1 == NULL) {
			addr += (p - buff);
			MSG_LOG("E5 ");
			debugstring("公钥模值ERROR!!\r\n");
			continue;
		}
		p = p1 + 8;
		for (i = 0; i < 20; i++) {
			if (((*p == 0x0d) && (*(p + 1) == 0x0a)) || ((*p == 0x0a) && (*(p + 1) == 0x0d))) {//找到0d0a
				p++;
				break;
			}
		}
		if (i >= 20)
			p += 2;
		p++;
		key.n_KeyLen = (keylen / 2);
		memcpy(key.n_Key, ASC2BCD(p, keylen), (key.n_KeyLen));
		p += keylen;

		//		debugstring(p);

		keye = 0;
		memcpy(cbuf, "钥指数值", 8);
		p1 = (char*)Find_CharData((INT8U*)p, 40, cbuf, 8);
		if (p1 == NULL) {//公钥指数值
			addr += (p - buff);
			MSG_LOG("E6 ");
			continue;
		}
		p = p1 + 8;
		for (i = 0; i < 20; i++) {//公钥指数值
			if ((*p >= '0') && (*p <= '9')) {
				keye *= 10;
				keye += (*p - '0');
			}
			else {
				if (keye != 0)//如果不等0，说明已经找到长度，可以退出
					break;
			}
			p++;
		}
		key.e_Key = keye;
		MSG_LOG("keyver:%d,key.e_key:%d\r\n", keyVer, key.e_Key);
		// 		debugstring("keyData:");
		// 		debugdata(key.n_Key, key.n_KeyLen, 1);
		// 		debugstring("\r\n");
		addr += (p - buff);//此版本完全取出

		MSG_LOG("\r\n必须更新!!");	//需要更新写入
		//先写入1字节的目录位置。
		FlashVerbuf[pIndex] = keyVer;	//保存
		BCD_LOG(FlashVerbuf, pIndex + 1, 1);

		//e 指数区
		memcpy(buff, (unsigned char*)&key.e_Key, 2);
		RevertTurn(2, (unsigned char*)buff);				//需要高位在前
		memcpy(buff + 2, (unsigned char*)&key.n_KeyLen, 2);	//模值长度
		flashwrite(FLASH_PUBLICKEY_START + PKEY_e_offset + (pIndex*PKEY_e_len), (unsigned char*)buff, PKEY_e_len);	//写入公钥指数2字节+模值长度2字节
		//公钥模值区
		memcpy(buff, key.n_Key, key.n_KeyLen);
		if (key.n_KeyLen % 2) {//不是2的整数倍，加个FF
			buff[key.n_KeyLen] = 0xFF;
			key.n_KeyLen++;
		}
		flashwrite(FLASH_PUBLICKEY_START + PKEY_MOZHI_offset + (pIndex * 256), (unsigned char*)buff, key.n_KeyLen);	//写入公钥
		pIndex++;
	}

	//处理完了所有值，把目录区加上校验 
	flashread(FLASH_PUBLICKEY_START + PKEY_Mulu_offset, (unsigned char*)buff, 124);
	tt = __cpuCrc32((unsigned char*)FlashVerbuf, 124);
	memcpy(FlashVerbuf + 124, (unsigned char*)&tt, 4);	//加入CRC32校验
	MSG_LOG("目录区写入:\r\n");
	BCD_LOG((unsigned char*)FlashVerbuf, 128, 1);
	flashwrite(FLASH_PUBLICKEY_START + PKEY_Mulu_offset, FlashVerbuf, 128);	//写入目录校验

	memset(buff, 0xFF, 128);		//写入参数信息
	memcpy(buff, "银行公钥", 8);
	memcpy(buff + 8, (unsigned char*)&pkVer, 4);
	memcpy(buff + 8 + 4, (unsigned char*)&SysTime, 7);
	tt = __cpuCrc32((unsigned char*)buff, 124);
	memcpy(buff + 124, (unsigned char*)&tt, 4);	//加入CRC32校验
	flashwrite(FLASH_PUBLICKEY_START, (unsigned char*)buff, 128);	//全部公钥处理完了再写入参数。

	MSG_LOG("参数区写入:\r\n");
	BCD_LOG((unsigned char*)buff, 128, 1);
	MSG_LOG("\r\n更新标志!!\r\n");
	memcpy(buff, "已经写入", 8);
	flashwrite(FLASH_GPS_SOUND_START, (unsigned char*)buff, 8);//更新完成就不再更新了

}
#endif

BIT_TVL_TAG QpbocTVLFCI;
static INT8U	QpbocTVLData[LEN_QBOCTVL_DATA];

char *ASC2BCD(char *strASC, int lenASC)
{
	unsigned int i, t;
	unsigned char temp;

	t = 0;
	for (i = 0; i < lenASC;)
	{
		if ((strASC[i] >= '0') && (strASC[i] <= '9'))
			temp = strASC[i] - '0';
		else if ((strASC[i] >= 'a') && (strASC[i] < 'g'))
			temp = strASC[i] - 'a' + 10;
		else if ((strASC[i] >= 'A') && (strASC[i] < 'G'))
			temp = strASC[i] - 'A' + 10;
		else
			temp = strASC[i] - '0';

		irda_rxBuf[t] = temp << 4;
		if ((strASC[i + 1] >= '0') && (strASC[i + 1] <= '9'))
			temp = strASC[i + 1] - '0';
		else if ((strASC[i + 1] >= 'a') && (strASC[i + 1] < 'g'))
			temp = strASC[i + 1] - 'a' + 10;
		else if ((strASC[i + 1] >= 'A') && (strASC[i + 1] < 'G'))
			temp = strASC[i + 1] - 'A' + 10;
		else
			temp = strASC[i] - '0';
		irda_rxBuf[t] |= temp;
		i += 2;
		t++;
	}
	return (char*)irda_rxBuf;
}


void get_BER_TVL(IN INT32U bitPos, OUT BER_TVL *OutTVL)
{
	INT8U temp[2];
	INT8U *p = QpbocTVLData + bitPos;
	TYPE_CHAR_LONG lTempLong;
	//tag

	memset(temp, 0, 2);
	temp[0] = *p++;
	if ((temp[0] & 0x1F) == 0x1F) {
		temp[1] = *p++;
		OutTVL->T = (short)temp[0] * 256 + temp[1];
		if ((*p & 0x80) == 0) {
			OutTVL->L = (INT32U)*p++;
			memcpy(OutTVL->V, p, OutTVL->L);
		}
		else {
			temp[0] = *p++ & 0x7F;		//sum of length
			lTempLong.LongBuff = 0;
			if (temp[0] == 1)
				lTempLong.CharBuff[0] = *p++;
			else if (temp[0] == 2) {
				lTempLong.CharBuff[1] = *p++;
				lTempLong.CharBuff[0] = *p++;
			}
			else if (temp[0] == 3) {
				lTempLong.CharBuff[2] = *p++;
				lTempLong.CharBuff[1] = *p++;
				lTempLong.CharBuff[0] = *p++;
			}
			else if (temp[0] == 4) {
				lTempLong.CharBuff[3] = *p++;
				lTempLong.CharBuff[2] = *p++;
				lTempLong.CharBuff[1] = *p++;
				lTempLong.CharBuff[0] = *p++;
			}
			OutTVL->L = (lTempLong.LongBuff);
			memcpy(OutTVL->V, p, OutTVL->L);
		}
	}
	else {
		OutTVL->T = (INT16U)temp[0];
		if ((*p & 0x80) == 0) {
			OutTVL->L = (INT32U)*p++;
			memcpy(OutTVL->V, p, OutTVL->L);
		}
		else {
			temp[0] = *p++ & 0x7F;		//sum of length
			lTempLong.LongBuff = 0;
			if (temp[0] == 1)
				lTempLong.CharBuff[0] = *p++;
			else if (temp[0] == 2) {
				lTempLong.CharBuff[1] = *p++;
				lTempLong.CharBuff[0] = *p++;
			}
			else if (temp[0] == 3) {
				lTempLong.CharBuff[2] = *p++;
				lTempLong.CharBuff[1] = *p++;
				lTempLong.CharBuff[0] = *p++;
			}
			else if (temp[0] == 4) {
				lTempLong.CharBuff[3] = *p++;
				lTempLong.CharBuff[2] = *p++;
				lTempLong.CharBuff[1] = *p++;
				lTempLong.CharBuff[0] = *p++;
			}
			OutTVL->L = (lTempLong.LongBuff);
			memcpy(OutTVL->V, p, OutTVL->L);
		}
	}
	return;
}
