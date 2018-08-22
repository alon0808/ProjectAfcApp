/*==========================================================================*
 *    Copyright(c) 2008-2009, SANLIAN., Ltd.
 *                     ALL RIGHTS RESERVED
 *
 *  PRODUCT  :
 *
 *  FILENAME : qPBOC.c
 *  CREATOR  :             DATE: 2009-08-05 10:11
 *  VERSION  : V1.00
 *  PURPOSE  :
 *
 *
 *  HISTORY  :
 *
 *==========================================================================*/
#ifndef _H_QPBOC_H
#define _H_QPBOC_H
#include "../include/include.h"
#include "xSys_Lantaiyuan.h"

 //��Ҫ��  _debug_
 //#define QPBOC_DEBUG
 //#define QPBOC_DEBUG_ER

 //���д����ռ䳤��
#define qPbocRECORD_LEN 256

#ifdef	_debug_ 
#define ERROR(...) printf(__VA_ARGS__)
#else
#define ERROR(...)
#endif
#if defined  _debug_stop
extern unsigned char stop;
#endif
typedef	unsigned char U8, INT8U;
typedef unsigned short U16, INT16U;
typedef unsigned int U32, INT32U;
typedef	signed char S8, INT8S;
typedef signed short S16, INT16S;
typedef signed int S32, INT32S;

#define EXTERN		extern

#ifndef IN
#define IN
#define OUT
#endif

#define SWAP16(x)	(((x & 0xff) << 8) | (x >> 8))
#define SWAP32(x)   (((x>>24)&0x000000ff) |((x>>8)&0x0000ff00)|((x<<8)&0x00ff0000)|((x<<24)&0xff000000))

//һ��ͨ����

#define INFO_81		"\x00\x00\x00\x00"	/*��Ȩ��������*/
#define INFO_95     "\x00\x00\x00\x00\x00"                       /* �ն���֤���(TVR) */
#define INFO_9A		"\x00\x00\x00"								/*��������YYMMDD*/
#define INFO_9B		"\x00\x00"			                        /*����״̬��ϢTSI ����Ӧ�ó�ʼ�����������Զ���Ĭ��Ϊ00*/
#define INFO_9C     "\x00"                                    	/* �������� */
#define INFO_9F01	"\x11\x22\x33\x44\x55\x66"					/*�յ��б�ʶ*/
#define INFO_9F02	"\x00\x00\x00\x00\x00\x00"					/*��Ȩ���*/
#define INFO_9F03   "\x00\x00\x00\x00\x00\x00"                   /* ������� */
#define INFO_9F04	"\x00\x00\x00\x00"	/*�������*/
#define INFO_9F09	"\x00\x20"	/*Ӧ�ð汾*/
#define INFO_9F15	"\x00\x01"	/*N4 �̻�������*/
#define INFO_9F16	"\xB4\xF3\xC3\xF7\xCE\xE5\xD6\xDE\xB3\xB5\xD4\xD8\x50\x4F\x53"	/*ans15 �̻���ʶ(�������޳���POS)*/
#define INFO_9F1A   "\x01\x56"                                  	/* �ն˹��Ҵ��� */
#define INFO_9F1B	"\x00\x00\x00\x00"	/*�ն�����޶�*/
#define INFO_9F1C	"\x31\x32\x33\x34\x35\x36\x37\x38"	/*an8 �ն˱�ʶASC*/
#define INFO_9F1D	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
// #define INFO_9F1E	"EFS3000L"		/*an8 �ӿ��豸���кţ����̷�����������к�DMWZ-A01*/
#define INFO_9F1E	"54004318"		/*an8 �ӿ��豸���кţ��ٸ�pos��������*/
#define INFO_9F21	"\x00\x00\x00"	/*����ʱ��HHMMSS*/
#define INFO_5F2A   "\x01\x56"                                 	/* ���׻��Ҵ��� */
#define INFO_5F2D   "\x63\x6ex65\x6ex00\x00x00\x00"           	/* ��ѡ����*/
//#define INFO_9F33	"\x00\x08\xC0"	/*b3 �ն�����*/
//#define INFO_9F33	"\x00\x00\x01"	/*b3 �ն�����*/
//#define INFO_9F33	"\x20\x89\xc8"	/*b3 �ն�����*/
#define INFO_9F33	"\xE0\xE1\x80"	/*b3 �ն����ܰٸ�pos��������*/
#define INFO_9F34	"\x00\x00\x00"	/*�ֿ�����֤����CVM���*/
#define INFO_9F35	"\x26"		/*�ն�����*/
#define INFO_5F36	"\x02"	/*���׻���ָ��*/
#define INFO_9F37	"\x00\x00\x00\x00"							/*����Ԥ֪��*/
#define INFO_9F39		"\x07"	/*���۵�POS���뷽ʽ*/
#define INFO_9F3A	"\x00\x00\x00\x00"	/*�ο����ҽ��*/
#define INFO_9F3C	"\x01\x56"	/*���ײο����Ҵ���*/
#define INFO_9F3D		"\x02"	/*���ײο�����ָ��*/
#define INFO_9F40	"\xA0\x00\x10\x10\x01"		/*�����ն�����*/
#define INFO_9F41	"\x00\x00\x10\x43"	/*������ż�����*/
#define INFO_9F4E	"\xB4\xF3\xC3\xF7\xCE\xE5\xD6\xDE\xB3\xB5\xD4\xD8\x30\x31\x32"	/*��Ʒ����*/
#define INFO_9F66   "\x28\x00\x00\x00"                           /* �ն˽������� */ //28���ѻ�QPBOC��֧��ֽ��ǩ��
#define INFO_9F66_1   "\x08\x00\x00\x00"                       /* �ն˽������� */ //28���ѻ�QPBOC��֧��ֽ��ǩ��
#define INFO_9F66_2   "\x28\x80\x00\x00"                       /* �ն˽������� */ //28���ѻ�QPBOC��֧��ֽ��ǩ��
#define INFR_8A			"Y1"						//д��"Y1"���ѻ�����


//add by zhgfan 2018-1-5
typedef enum {
	PREPAID_CARD,
	CREDIT_CARD
}EMV_CARDTYPE;

typedef enum {
	E_cash,
	PAY_Online,
	ODA,
	UICS
}PBOC_CHANNEL;
typedef union
{
	INT8U	CharBuff[2];
	INT16U	IntBuff;
} TYPE_CHAR_INT;

typedef union
{
	INT8U	CharBuff[4];
	INT16U	IntBuff[2];
	INT32U	LongBuff;
}TYPE_CHAR_LONG;

// PBOC ����Ĵ������
enum FUN_QPBOC_RETURN_ERRCODE {
	qPBOC_SUCESS_ECU = 0,			//4 ����ִ�гɹ�
	qPBOC_TRADE_ABORT = 1,			//4 ������ֹ
	qPBOC_APDU_CMD = 84,			//4 ������APDUʧ��
	qPBOC_CARD_ACTIVE = 85,			//4 ���ʧ��
	qPBOC_CARD_ANTICOLL = 86,			//4 ����ͻʧ��
	qPBOC_NO_CARD = 87,			//4 û�м�⵽��
	qPBOC_NOOK_PATH = 88,			//4 û��ƥ��Ľ���·��
	qPBOC_POS_NOFUNCTION = 89,			//4 ���ն˲�֧�ָù���
	qPBOC_NO_QPBOC = 90,			//4 ��֧��QPBOC����
	qPBOC_FCI_LOSTDATA = 91,			//4 FCIģ��ǿ������ȱʧ
	qPBOC_MUTL_CARD = 92,			//4 ��⵽���ſ�
	qPBOC_TRADEMEONY_ZERO = 93,			//4 ���׽��Ϊ0
	qPBOC_MONEYOVERLIMIT = 94,			//4 ������ֹ:���׽����ն˷ǽӴ��������޶�
	qPBOC_APPFILE_FAILE = 95,			//4 ѡ���ļ�ʧ��
	qPBOC_APPFILE_INVALID = 96,			//4 Ӧ���ļ���Ч
	qPBOC_APPFILE_NO_FIND = 97,			//4 Ӧ���ļ�û���ҵ�
	qPBOC_APPFILE_LOCK = 98,			//4 Ӧ���ļ��Ѿ�����
	qPBOC_SELECT_NO_APP = 99,			//4 Ӧ����û�з���Ӧ���ļ���������ֹ
	qPBOC_PUBLISHKEY_LEN = 100,		//4 �����й�Կ������CA KEY���Ȳ����
	qPBOC_RSA_ERROR = 101,		//4 ����RSA����
	qPBOC_PUBLISHC_FORMAT = 102,		//4 �����й�Կ֤���ʽ����
	qPBOC_PUBLISHC_SIGN = 103,		//4 �����й�Կ֤��SHAֵ��
	qPBOC_PUBLISHC_FLAG = 104,		//4 �����й�Կ֤�鷢���б�ʶ����
	qPBOC_READ_SYSTIME = 105,		//4 ��ϵͳʱ�����
	qPBOC_PUBLISHC_EXP = 106,		//4 �����й�Կ֤�����
	qPBOC_PUBLISHC_PKEYF = 107,		//4 �����й�Կ��ʶ
	qPBOC_ICC_NOEXIT = 108,		//4 IC����Կ֤��û���ҵ�
	qPBOC_IKEYSGLENNO = 109,		//4 ǩ����̬���ݳ��ȸ������й�Կ���Ȳ�һ�� 
	qPBOC_ICC_LEN = 110,		//4 IC���Ĺ�Կ֤�鳤�Ȳ���
	qPBOC_ICC_FORMAT = 111,		//4 �ָ�IC����Կ֤���ʽ����
	qPBOC_ICC_STAG_NO82 = 112,		//4 ��̬Ӧ�ñ�ǩ��û���ҵ�82
	qPBOC_OFFLINEAUTENDATA_INVALID = 113,	//4 �ѻ���֤���ݷǷ�
	qPBOC_ICC_SIGN = 114,		//4 IC����Կ֤��SHAֵ��
	qPBOC_ICC_PANFLAG = 115,		//4 IC�����˺������˺Ų�һ��
	qPBOC_ICC_EXP = 116,		//4 IC����Կ֤�����
	qPBOC_ICC_PKEYF = 117,		//4 IC����Կ��ʶ
	qPBOC_DDA_DATAFORMAT = 118,		//4 DDAĳЩ����û���Ҵ�
	qPBOC_DDA_DATAKEYLEN = 119,		//4 DDA������IC��PKEY���Ȳ�һ��
	qPBOC_DDA_SIGN = 120,		//4 IC����Կ֤��SHAֵ��
	qPBOC_CAINDEX_NOEXIT = 121,		//4 CA��Կ����������
	qPBOC_PUBLISHC_NOEXIT = 122,		//4 �����й�Կ֤�鲻����
	qPBOC_PUBLISHE_NOEXIT = 123,		//4 �����й�Կָ��������
	qPBOC_CAKEYRID_NOEXIT = 124,		//4 ����RIDû���ҵ�
	qPBOC_SIGNSTATIC_NOEXIT = 125,		//4 ǩ���ľ�̬����
	qPBOC_AFL_NOEXIT = 126,		//4 AFL ������
	qPBOC_STATICDATA_NOEXIT = 127,		//4 ��Ҫ��֤�ľ�̬���ݲ�����
	qPBOC_CAPKEY_NOEXIT = 128,		//4 CA��Կ������
	qPBOC_NOFIND_AUTEN = 129,		//4 û�к��ʵ���֤����
	qPBOC_ICCE_NOEXIT = 130,		//4 IC����Կָ��û���ҵ�
	qPBOC_ICSIGNDYDATA_NOEXIT = 131,		//4 ǩ���Ķ�̬����û��֪��
	qPBOC_NOUNDOWNDATA_NOEXIT = 132,		//4 ����Ԥ�����ݲ�����
	qPBOC_NO_CDA = 134,		//4 QPBOC ��֧��CDA
	qPBOC_RESUL_AAC = 135,		//4 �ѻ��ܾ�
	qPBOC_RESUL_TC = 136,		//4 �ѻ���׼
	qPBOC_RESUL_ARQC = 137,		//4 ������׼
	qPBOC_RESUL_RFU = 138,		//4 δ֪��Ϊ
	qPBOC_NOFIND_9F10 = 139,		//4 û���ҵ�9F10��Ƭ��������
	qPBOC_AACARQC_NOM = 140,		//4 �ѻ��ܾ�������ȱ�ٱ�������
	qPBOC_TC_NOM = 141,		//4 �ѻ���׼ȱ�ٱ�������
	qPBOC_9F38_NOEXIT = 142,		//4 PDOL������
	qPBOC_9F66_NOEXIT = 143,		//4 �ն˽������Բ�����
	qPBOC_SHA_FLAG = 144,		//4 ��ϣ�㷨��ʶ����
	qPBOC_AFL_SFI_ERROR = 145,		//4 AFL ��SFI����
	qPBOC_OFFMONEY0 = 146,		//4 �ѻ����׽��Ϊ0
	qPBOC_OFFNOLINE = 147,		//4 �ѻ��豸��������
	qPBOC_LINERESUL_AAC = 148,		//4 �豸���������ܾ�
	qPBOC_VER_NO_SAME = 149,		//4 �ն˺Ϳ�Ӧ�ð汾��һ��
	qPBOC_CONTRY_OUTTRADE = 150,		//4 ���Ҵ��벻ͬ�����ܽ��й��ʽ���
	qPBOC_CONTRY_INTRADE = 151,		//4 ���Ҵ�����ͬ,���ܽ��й��ڽ���
	qPBOC_NOATM_INVALID = 152,		//4 ��ATM�ն�����Ч
	qPBOC_ATM_INVALID = 153,		//4 ATM�ն�����Ч
	qPBOC_STARTTIME_INVLAID = 154,		//4 ��û����Ч
	qPBOC_ENDTIME_INVLIAD = 155,		//4 ����ʧЧ
	qPBOC_BLACK_CARD = 156,		//4 �������� 
	qPBOC_REPEAT_TAG = 157,		//4 �ظ���ǩ����
	qPBOC_TVL_INVALID = 158,		//4 TVL ��ʽ�Ƿ�
	qPBOC_DATA_NO70 = 159,		//4 ����ģ�����
	qPBOC_DATA_SIGAN_NO70 = 160,
	qPBOC_QpboC_SIGAN_NO = 161,	//����Ϣû�ҵ�
	qPBOC_QpboC_tryAgain = 162,	//��Ҫ��ˢ
	qPBOC_CARD_forbid = 163,	//����ֹˢ
	qPBOC_SDA_DATAFORMAT = 164,		//4 SDAĳЩ����û���Ҵ�
	qPBOC_SDA_DATAKEYLEN = 165,		//4 SDA������IC��PKEY���Ȳ�һ��
	qPBOC_SDA_SIGN = 166,		//4 SDA SHAֵ��
	qPBOC_lostData = 167,		//4 ����ȱʧ
	qPBOC_CARD_forbid_oda = 168,		//4 0da
	qPBOC_DEFAULT = 0xFFF
};

#define RSA_NO_PADDING		3

#define 	A1_MSD			0x80
#define	A1_NPBOC		0x40
#define	A1_QPBOC		0x20
#define	A1_YPBOC		0x10
#define	A1_OFFLINE		0x08
#define	A1_ONLINEPIN	0x04
#define	A1_ODA			0x01
#define	A1_SIGN		0x02
#define	A2_ONLINEKEY	0x80
#define	A2_CVM			0x40

#define	TYPE_AUTEN_NO				0x00
#define	TYPE_AUTEN_SDA				0x01
#define	TYPE_AUTEN_DDA				0x02
#define	TYPE_AUTEN_CDA				0x03

#define	AIP_SDA					0x40		//4 ֧��SDA
#define	AIP_DDA					0x20		//4 ֧��DDA		
#define	AIP_CVM					0x10		//4 ֧�ֳֿ�����֤
#define	AIP_POS_RISP				0x08		//4 ִ���ն˷��չ���
#define	AIP_PUBLISH_AUTEN			0x04		//4 ֧�ַ�������֤
#define	AIP_CDA					0x01		//4 ֧��CDA

//bytes1
#define	CA_MANINPUT				0x80		//4 �ֹ���������
#define	CA_MAG						0x40		//4 ����
#define	CA_CONTACTCARD			0x20		//4 �Ӵ�ʽIC��
//bytes2
#define	CA_ICPINAUTEN				0x80		//4 IC������PIN��֤
#define	CA_DESPINAUTEN			0x40		//4 IC������PIN��֤
#define 	CA_SIGN_PAGE				0x20		//4 ǩ��ֽ
#define	CA_NO_CVM					0x08		//4 ����CVM	
#define	CA_MAN_CVM				0x01		//4 �ֿ���֤����֤
//tytes3
#define	CA_SDA						0x80		//4 ��̬������֤
#define	CA_DDA						0x40		//4 ��̬������֤
#define	CA_GULP_CARD				0x20		//4 �̿�
#define	CA_CDA			

//byte1
#define	TVR_NO_OFFLINEAUTEN		0x80		//4 δ�����ѻ�������֤
#define	TVR_OFFLINEAUTEN_FAILE	0x40		//4 �ѻ�������֤ʧ��
#define	TVR_ICDATA_LOST			0x20		//4 IC������ȱʧ
#define	TVR_IC_ABORTFILE			0x10		//4 IC���������쳣�ļ���
#define	TVR_OFFDDAAUTENFAILE		0x08		//4 �ѻ���̬������֤ʧ��
#define	TVR_CDAAUTENFAILE			0x04		//4 ���϶�̬������֤ʧ��
//bytes2
#define	TVR_ICPOS_VERERR			0x80		//4 IC����POS�汾��һ��
#define	TVR_APP_EXP				0x40		//4 Ӧ���ѹ���
#define	TVR_APP_INVALID			0x20		//4 Ӧ��û����Ч
#define	TVR_ICNOREQSERVER			0x10		//4 ��Ƭ������������ķ���
#define	TVR_NEW_CARD				0x08		//4 �¿�
//bytes3
#define	TVR_CVMAUTEN_FAILE		0x80		//4 �ֿ�����֤ʧ��
#define	TVR_NOUNDOW_CVM			0x40		//4 δ֪��CVM
#define	TVR_PINTRY_SUMOVER		0x20		//4 PIN���Դ�������
#define	TVR_INPIN_KEYERROR		0x10		//4 Ҫ������PIN����������̲����ڻ���������
#define	TVR_INPIN_NOPIN			0x08		//4 Ҫ������PIN��������̴��ڣ���δ����PIN
#define	TVR_INONLINEPIN			0x04		//4 ��������PIN
//tyes4
#define	TVR_OVERLLIMITMONEY		0x80		//4 ���׳�������޶�
#define	TVR_OVER_OFFLINE_LIMITL	0x40		//4 ���������ѻ���������
#define	TVR_OVER_OFFLINE_LIMITH 	0x20		//4 ���������ѻ���������
#define	TVR_RAMSELONLINEAUTEN	0x10		//4 ���ױ����ѡ����������
#define	TVR_BRANDSELONLINEAUTEN	0x08		//4 �̻�ѡ����������
//bytes5
#define	TVR_USE_DEFAULT_PDOL		0x80		//4 ʹ��ȱʡTDOL
#define	TVR_PUBLISH_AUTEN_FAILE	0x40		//4 ��������֤ʧ��
#define	TVR_BEFGC_CRE_FAILE		0x20		//4 ���һ��GENERATE AC����֮ǰ�ű�����ʧ��
#define	TVR_AFGC_CRE_FAILE		0x10		//4 ���һ��GENERATE AC����֮��ű�����ʧ��

#define	TSI_OFFAUTEN_EXE			0x80		//4 �ѻ�������֤�ѽ���
#define	TSI_CVMAUTEN_EXE			0x40		//4 �ֿ�����֤�ѽ���
#define	TSI_CARDRIST_EXE			0x20		//4 ��Ƭ���չ����ѽ���
#define	TSI_PUBLISHAUTEN_EXE		0x10		//4 ��������֤�ѽ���
#define	TSI_POSRIST_EXE			0x08		//4 �ն˷��չ����ѽ���
#define	TSI_CRE_EXE				0x04		//4 �ű������ѽ���


//��ǩ����
#define 	T_FCI_MODULE			0x6F		//4 FCI ģ��
#define	T_SPECAI_FILE_DF		0x84		//4 ר���ļ���DF
#define	T_FCI_SPE_MOUDLE		0xA5		//4 FCI ר��ģ��
#define 	T_FCI_ISSIA_DATA		0xBF0C		//4 FCI �������Զ�������
#define	T_DIR_ENTER			0x61		//4 Ŀ¼���
#define	T_APP_FILE_AID			0x4F		//4 DF��(AID)
#define 	T_APP_TARGE			0x50		//4 Ӧ�ñ�ǩ
#define	T_APP_USER_PRIOR		0x87		//4 Ӧ������ָʾ��
#define	T_DEV_TRADE_AATR		0x9F66		//4 �ն˽�������
#define 	T_ACCREDIT_MONEY		0x9F02		//4 ��Ȩ���
#define	T_OTHER_MONEY			0x9F03		//4 �������
#define	T_DEV_COUNTY_CODE	0x9F1A		//4 �ն˹��Ҵ���
#define	T_DEV_TVR				0x95		//4 �ն���֤���
#define	T_TRADE_COIN_CODE	0x5F2A		//4 ���׻��Ҵ���
#define	T_TRADE_TIME			0x9A		//4 ��������
#define	T_TRADE_TYPE			0x9C		//4 ��������
#define	T_FORECAST_DATA		0x9F37		//4 ����Ԥ��������
#define 	T_AIP					0x82
#define	T_ATC					0x9F36
#define	T_AFL					0x94
#define	T_AAC					0x9F26		//Application Authentication Cryptogram
#define 	T_IAD					0x9F10		// Issuer Application Data
#define	T_MSD2					0x57
#define	T_PANS					0x5F34
#define	T_SDAD					0x9F4B
#define	T_CARDTRAT				0x9F6C		//4 ��Ƭ��������
#define	T_OFFMONEY			0x9F5D		//4 ���õ��ѻ����
#define	T_MONEYCODE			0x9F51		//4 ��ʹ�õĻ��Ҵ���
#define	T_ADDINGMSG			0x9F68		//4 ��Ƭ������Ϣ
#define	 T_LAST_INLINECNT		0x9F13		//4 �ϴ��������׼�����
#define	 T_PINTRYSUM			0x9F17		//4 PIN ���ԵĴ���
#define 	T_CVM_MONEY_OVER		0x9F6B		//4 CVM �޶�
#define	T_CVMNAME				0x5F20		//4 CVM ������

#define	T_AVLIDTIME			0x5F24		//4 ʧЧ����
#define	T_MAINPAN				0x5A		//4 ���˺�
#define	T_APP_CTRL				0x9F07		//4 Ӧ�ð汾��
#define	T_CVM_LIST				0x8E		//4 CVM �б�
#define	T_IAC_DEFAULT			0x9F0D		//4 �����е�Ĭ����Ϊ����
#define	T_IAC_REFUSE			0x9F0E		//4 �����еľܾ���Ϊ����
#define	T_IAC_ONLINE			0x9F0F		//4 �����е�������Ϊ����
#define	T_CONTRYCODE			0x5F28		//4 �����еĹ��Ҵ���


#define	T_ICPUBKEY_E			0x9F47		//4 IC���Ĺ�Կָ��
#define	T_ICPUBKEY_N_MOD		0x9F48	//4 	//IC����Կ������
#define	T_DDOL					0x9F49		//4 ��̬���ݶ����б�
#define	T_SDATAG				0x9F4A		//4 ��̬������֤��ǩ
#define	T_NAME					0x9F0B		//4 �ֿ�������
#define	T_SERVERCODE			0x5F30		//4 ������
#define	T_APP_VER				0x9F08		//4 Ӧ�ð汾��
#define	T_APP_VALID			0x5F25		//4 Ӧ����Ч����
#define	T_E_PUBISH				0x9F23		//4 �����й�Կָ��
#define	T_N_PUBLISH_MOD		0x92		//4 �����й�Կ����
#define	T_PULISHAUTENCODE	0x9F74		//4 ��������Ȩ��

#define	PT_MAN_ONLINE			1			//4 �з���Ա:��������
#define	PT_MAN_ONLINEOFF		2			//4 �з���Ա:�������������ѻ�
#define	PT_MAN_OFFLINE		3			//4 �з���Ա:�����ѻ�
#define	PT_NOMAN_ONLINE		4			//4 �����ģ���������
#define	PT_NOMAN_ONLINEOFF	5			//4 �����ģ��������������ѻ�
#define	PT_NOMAN_OFFLINE		6			//4 �����ģ������ѻ�

#define	PT_ORG_BANK			0x10		//4 ���ڻ���
#define	PT_ORG_SHOP			0x20		//4 �̻�����
#define	PT_ORG_CVMAN			0x30		//4 �ֿ���

//bytes1
#define	PTA_CASH					0x80		//4 �ֽ�
#define	PTA_COMMODITY				0x40		//4 ��Ʒ
#define	PTA_SERVER					0x20		//4 ����
#define	PTA_RETURNCASH			0x10		//4 ����
#define	PTA_CHECK					0x08		//4 ��ѯ
#define	PTA_TURNCASH				0x04		//4 ת��
#define	PTA_DEBIT					0x02		//4 ����
#define	PTA_MANAGE				0x01		//4 ����
//bytes2
#define	PTA_DEPOSIT				0x80		//4 ����
//bytes3
#define	PTA_NUMKEY				0x80		//4 ���ּ�
#define	PTA_ALPKEY					0x40		//4 ��ĸ�������ַ�
#define	PTA_COMMANDKEY			0x20		//4 �����
#define	PTA_FUNCTIONKEY			0x10		//4 ���ܼ�
//byte4
#define	PTA_PRN_SERVER			0x80		//4 ��ӡ������Ա
#define	PTA_PRN_CVM				0x40		//4 ��ӡ���ֿ���
#define	PTA_DIS_SERVER			0x20		//4 ��ʾ������Ա
#define	PTA_DIS_CVM				0x10		//4 ��ʾ���ֿ���
#define	PTA_TAB10					0x02		//4 �����10
#define	PTA_TAB9					0x01		//4 �����9
//byte5
#define	PTA_TAB8					0x80		//4 �����8
#define	PTA_TAB7					0x40		//4 �����7
#define	PTA_TAB6					0x20		//4 �����6
#define	PTA_TAB5					0x10		//4 �����5
#define	PTA_TAB4					0x08		//4 �����4
#define	PTA_TAB3					0x04		//4 �����3
#define	PTA_TAB2					0x02		//4 �����2
#define	PTA_TAB1					0x01		//4 �����1

#define	B_PUBLISHC_HEADER					0		//4 �ָ�����ͷ1
#define	B_PUBLISHC_CERF					1		//4 ֤���ʽ	1
#define	B_PUBLISHC_ISSID					2		//4 �����б�ʶ 4
#define	B_PUBLISHC_EXPTIME				6		//4 ʧЧ����	2
#define	B_PUBLISHC_CSERIAL				8		//4 ֤�����к�	3
#define	B_PUBLISHC_SHAFLAG				11		//4 SHA�㷨��ʶ	1
#define	B_PUBLISHC_PKEYFLAG				12		//4 ��Կ�㷨��ʶ	1
#define	B_PUBLISHC_PKEYLEN				13		//4 ��Կ����		1
#define	B_PUBLISHC_PKEYELEN				14		//4 ��Կָ������	1
#define	B_PUBLISHC_PKEY					15		//4 ��Կ������ֽ�


#define	B_ICC_HEADER						0		//4 �ָ�����ͷ1
#define	B_ICC_CERF							1		//4 ֤���ʽ	1
#define	B_ICC_APPPAN						2		//4 Ӧ�����˺�10
#define	B_ICC_EXPTIME						12		//4 ʧЧ����	2
#define	B_ICC_CSERIAL						14		//4 ֤�����к�	3
#define	B_ICC_SHAFLAG						17		//4 SHA�㷨��ʶ	1
#define	B_ICC_ICPKEYFLAG					18		//4 ��Կ�㷨��ʶ	1
#define	B_ICC_PKEYLEN						19		//4 ��Կ����		1
#define	B_ICC_PKEYELEN						20		//4 ��Կָ������	1
#define	B_ICC_ICPKEYLEFT					21		//4 ��Կ������ֽ�	

//4 ��������Ϣ
typedef struct
{
	INT8U 	Result[2];			//4 ���׽��	0
	INT8U 	TradeType;			//4 ��������	2
	INT8U 	TradeMoney[4];		//4 ���׽��	3
	INT8U	BefMoney[4];		//4 ���׺���	7
	INT8U 	D_9F4C[8];			//4 IC����̬����11
	INT8U 	TVR[5];				//4 ���׽��	19
}QPBOC_RESULT_TYPE;


typedef struct {
	INT16U    T;
	INT32U 	L;
	INT8U	V[248];
}BER_TVL;

typedef struct {
	INT32U 	TagValue;
	INT32U 	SizeOff;
}TAG_OFFSTU;

typedef struct
{
	TAG_OFFSTU 	t_4F;				//4 DF����
	TAG_OFFSTU	t_50;				//4 Ӧ�ñ�ǩ
	TAG_OFFSTU 	t_57;				//4 2�ŵ�����
	TAG_OFFSTU 	t_5A;				//4 Ӧ�����˺�(PAN)
	TAG_OFFSTU 	t_61;				//4 Ŀ¼���
	TAG_OFFSTU 	t_6F;				//4 FCIģ��
	TAG_OFFSTU  t_81;				//4 ���׽��HEX (4)
	TAG_OFFSTU	t_82;				//4 Ӧ�ý�������
	TAG_OFFSTU	t_84;				//4 ר���ļ�������DF
	TAG_OFFSTU	t_8C;				//CDOL1
	TAG_OFFSTU 	t_8E;				//4 �ֿ�����֤����(CVM)�б�
	TAG_OFFSTU 	t_8F;				//4 CA��Կ����
	TAG_OFFSTU 	t_90;				//4 �����й�Կ֤��
	TAG_OFFSTU 	t_93;				//4 ǩ���ľ�̬Ӧ������
	TAG_OFFSTU 	t_92;				//4 �����й�Կ����
	TAG_OFFSTU 	t_94;				//4 Ӧ���ļ���λ��
	TAG_OFFSTU  t_95;				//4 �ն���֤���
	TAG_OFFSTU  t_9A;				//4 ��������
	TAG_OFFSTU  t_9C;				//4 ��������
	TAG_OFFSTU  t_A5;				//4 FCIר��ģ��
	TAG_OFFSTU  t_5F20;				//4 �ֿ�������
	TAG_OFFSTU 	t_5F24;				//4 ʧЧ����
	TAG_OFFSTU 	t_5F25;				//4 Ӧ����Ч����
	TAG_OFFSTU 	t_5F28;				//4 �����й��Ҵ���
	TAG_OFFSTU 	t_5F2A;				//4 ���׻��Ҵ���
	TAG_OFFSTU 	t_5F2D;				//4 ��ѡ����
	TAG_OFFSTU 	t_5F30;				//4 ������
	TAG_OFFSTU 	t_5F34;				//4 Ӧ��PAN���к�
	TAG_OFFSTU 	t_9F02;				//4 ������Ȩ��������������
	TAG_OFFSTU 	t_9F03;				//4 �뽻����صĵڶ�����ʾ���ֽ��
	TAG_OFFSTU  t_9F06;				//4 //4  ѡ�ļ�AID
	TAG_OFFSTU 	t_9F07;				//4 Ӧ����;���� -AUC
	TAG_OFFSTU 	t_9F08;				//4 Ӧ�ð汾��
	TAG_OFFSTU 	t_9F0B;				//4 �ֿ���������չ
	TAG_OFFSTU 	t_9F0D;				//4 ��������Ϊ���루IAC��-ȱʡ
	TAG_OFFSTU 	t_9F0E;				//4 ��������Ϊ���루IAC��-�ܾ�
	TAG_OFFSTU 	t_9F0F;				//4 ��������Ϊ���루IAC��-����
	TAG_OFFSTU 	t_9F10;				//4 ������Ӧ������
	TAG_OFFSTU 	t_9F11;				//4 �����д��������
	TAG_OFFSTU 	t_9F12;				//4 Ӧ����ѡ����
	TAG_OFFSTU  t_9F1A;				//4 �ն˹��Ҵ���
	TAG_OFFSTU	t_9F26;				//4 Ӧ������
	TAG_OFFSTU	t_9F27;				//4 ������Ϣ����
	TAG_OFFSTU 	t_9F32;				//4 �����й�Կָ��
	TAG_OFFSTU 	t_9F36;				//4 ATC
	TAG_OFFSTU 	t_9F37;				//4 ����Ԥ��������
	TAG_OFFSTU	t_9F38;				//4 PDOL
	TAG_OFFSTU	t_9F42;				//4 Ӧ�û��Ҵ��� ���CVM��Ҫ�����飬��Ҫ������
	TAG_OFFSTU	t_9F45;				//4 ������֤��
	TAG_OFFSTU 	t_9F46;				//4 IC����Կ֤��
	TAG_OFFSTU 	t_9F47;				//4 IC����Կָ��
	TAG_OFFSTU 	t_9F48;				//4 IC����Կ������
	TAG_OFFSTU 	t_9F49;				//4 ��̬������֤���ݶ����б�DDOL��
	TAG_OFFSTU 	t_9F4A;				//4 ��̬������֤��ǩ�б�
	TAG_OFFSTU 	t_9F4B;				//4 ǩ����̬Ӧ������
	TAG_OFFSTU 	t_9F4C;				//4 IC��̬Ӧ������
	TAG_OFFSTU 	t_9F4D;				//4 ��־���	
	TAG_OFFSTU	t_9F51;				//0156Ϊ�����  ˫�⽻��ʹ��
	TAG_OFFSTU	t_DF71;				//0156Ϊ�����˫�⽻��ʹ��
	TAG_OFFSTU 	t_9F5D;				//4 �����ѻ����ѽ��
	TAG_OFFSTU	t_9F66;				//4 �ն˽�������
	TAG_OFFSTU 	t_9F6C;				//4 ��Ƭ��������
	TAG_OFFSTU 	t_9F74;				//4 ��������Ȩ��
	TAG_OFFSTU	t_9F79;				//4 �����ֽ����
	TAG_OFFSTU	t_BF0C;				//4 �ļ�������Ϣ��FCI���������Զ�������
	TAG_OFFSTU	t_DF61;				//4 �ļ�������Ϣ��FCI���������Զ�������

}BIT_TVL_TAG;


#define	 SUM_APP_LIST_AID			5
typedef struct {
	INT8U 	LenAID;
	INT8U 	DF_AID[20];
	INT8U 	LenTag;
	INT8U 	AppTag[12];
	INT8U 	LenPro;
	INT8U 	AppPrio[2];
}FCI_MODULE;
typedef struct
{
	unsigned char rpurSerial[6];//0-5��¼��ˮ BCD
	unsigned char rBankCardNo[18];//6--23 ��������� ASC
	unsigned char rcardMainNO[10];//24-33 �����ʻ� BCD
	unsigned char rcardlimitdata[2];//34-45 ����Ч���� YYMM
	unsigned char rcardseial[2];//36-37 ��Ƭ���к� BCD
	unsigned char rUseKeytext[8];//38-45 Ӧ������ HEX
	unsigned char rUsemsginfo;//46 Ӧ����Ϣ���� HEX
	unsigned char rpublisBanduse[32];//47-78 ������Ӧ������ HEX
	unsigned char rRand[4];//79-82 ����Ԥ֪�� HEX
	unsigned char rdealTimer[2];//83-84 Ӧ�ý��׼�����HEX
	unsigned char rchekend[5];//85-89 �ն���֤��� HEX
	unsigned char ryyjfType[2];//90-91 Ӧ�ý������� HEX
	unsigned char ruseVer[2];//92-93 Ӧ�ð汾�� HEX
	unsigned char rSrlTimer[2];//94-95 �������м����� HEX
	unsigned char rLicense[6];//96-101 �����ֽ𷢿�����Ȩ�� HEX
	unsigned char rLicenseACK[2];//102-103 ��Ȩ��Ӧ�� HEX
	unsigned char rBuPOSSerial[3];//104-106 �ӿ��豸���к� BCD  POS�����
	unsigned char rDealMoney[2];//107-108 ���׽�� HEX ��λ�ں�
	unsigned char rAferMoney[4];//109-112 ���׺���� HEX ��λ�ں�
	unsigned char rDatatiem[7];//113-116 �������� YYYYMMDD//117-119 ����ʱ�� HHMMSS
	unsigned char rdealType;//120 �������� HEX
	unsigned char rbusinessCode[2];//121-122 ��Ӫ�̻����� BCD
	unsigned char rBuLineDevNo[6];//123-128 ��·���+������� 3�ֽ���·���+3�ֽ���·��� BCD
	unsigned char rNull;//129 Ԥ��
	unsigned char rCrc16[2];//130-131 ��¼У���� ǰ130�ֽڵ�CRC16  HEX
	unsigned char rDriverNo[4];//132-135 ˾����ţ���λ��ǰ
	unsigned char rfDDAResult;	//136 ��֤���
	unsigned char rCardType;	//137 ����
	unsigned char rDeviceSerial[4];//138-141//�豸���к�

	unsigned char rZYADD[8];//142-149 ר���ļ�����
	unsigned char rCDdata[16];//150-165 2�ŵ�����

	unsigned char rBBB[18];	//166-183 ����
}stPbocRec;
typedef struct
{
	INT32U	e_Key;
	INT32U	n_KeyLen;
	INT8U	n_Key[256];
}KEY_PUBLICMSG;



extern BIT_TVL_TAG QpbocTVLFCI;
EXTERN unsigned char GetDateTime(void);
extern void get_BER_TVL(unsigned int  bitPos, BER_TVL *OutTVL);
extern void emv_set_pay_channel(PBOC_CHANNEL channel);
extern PBOC_CHANNEL emv_get_pay_channel(void);
extern void emv_set_pay_channel(PBOC_CHANNEL channel);
extern void getMobileParameter_test(void);
extern unsigned char qrDisp_history(unsigned char mode, unsigned int Index);
extern EMV_CARDTYPE emv_get_card_type(void);
extern void emv_set_card_type(EMV_CARDTYPE type);



#include "RamStorage.h"


#endif

