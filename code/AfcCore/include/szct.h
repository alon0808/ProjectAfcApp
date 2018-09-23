#ifndef _INCLUDE_SCZT_HEAD_H_
#define _INCLUDE_SCZT_HEAD_H_

#include <stdio.h>

#include "Main_City.h"
#include "STProLib.h"
#include "StructDef.h"
#include "SlzrTypeDef.h"
#include "BuCtrlFlag.H"


#if 0
//#define _debug_sys_info_
#define _debug_
//#define debug_IF2_TLVDeal_
//#define debug_deal_8583_data
//#define debug_GJ_TLVDeal_
//#define _debug_gprs
//#define _debug_gps_
#define _debug_CPU_
#endif


#ifdef _debug_
//extern void dprintf (const char *fmt, ...);
#define MSG_LOG(...) printf(__VA_ARGS__)
#define BCD_LOG(b, l, m) debugdata(b, l, m)
#else
#define MSG_LOG(...)
#define BCD_LOG(...)
#endif

#define SWITCH_PBOC	1	// �Ƿ�֧����������,Ӱ�����п�,����������(�ǳ˳���),ͨ��������΢��֧����������

#if SWITCH_PBOC
//#define PBOC_TEST1	// �������Բ���,���Ϲ���������
//#define  PBOC_TEST		//������ʾ����ֻ����������̨
#endif

#ifdef PBOC_TEST1
#define DOMAIN_SHANGHAI_UNIONPAY	"ipos.chinaums.com"
#define DEV_TYPE	"EFS3000L"
#else
#define DOMAIN_SHANGHAI_UNIONPAY	"upos.chinaums.com"
#define DEV_TYPE	"ESF3000L"
#endif

#define SWITCH_ODA_BACKEND	1

//��PC���ڷ���GPS�켣��Ϣ��ģ�⳵������
//#define _GPS_Speed
//���ڹ���
//#define _card_guard_
//#define _TEST_YANPIAO_
//#define _phone_test_
//����ͨѶģ��ΪCDMA����MC323��������Ϊ900C �ھ�����ж�����

//����GPRSʹ�õ�COM�ں� COM2
#ifdef Modem_BUS_
#define COM_GPRS COM1
#else
#define COM_GPRS COM2
#endif

#define GPS_COM COM5

//����汾��
#define SOFT_VER_TIME_LOG 0x0103	// 1.03,��1.02һ��
#define SOFT_VER_TIME "2018.09.23 23:25 CHANGSHA"	//

#define PRAGRAMMER "ZZC"

//#define SOFT_VER_TIME_LOG "201009061359BB"

#define _backLight_on_ 0
#define _watchdog_open

//ˢ��������
//ͨ��M1��
#define CARDSTYLE_NORM1 1
//CPU��
#define CARDSTYLE_CPU 2
//2.4G CPU��
#define CARDSTYLE_24CPU 3
//�մ�M1��
#define CARDSTYLE_YIDAM1 4
//2.4G�ƶ���
#define CARDSTYLE_24Mobile 5
//2.4G���ſ�
#define CARDSTYLE_24CT 6
//�п�Ѷ��2.4G ��ͨ
#define CARDSTYLE_24ZKXL 7
//����ʢ��2.4G ��ͨ������
#define CARDSTYLE_24XMSH 8
//�����Ͽ�
#define CARDSTYLE_ANLONG 9

#define SEND_FILE_ERROR_ 10

#define  CARDSTYLE_JTB  11
#define	 QPBOC_JTB      12 //��ͨ�����ֽ�
//#define LED_DISPLAYNUM 
#define BLACKSUM 16000

#ifdef GPRS_SOUD_ENABLE
#define MC323_Ring_sta	0xA1
#define MC323_Ring_key	0xA2
#define MC323_Ring_ing	0xA3
#define MC323_Ring_END	0xA4
#endif

//������������
#define RUN_RESTART_TIME 125

//10
#define RECORD_TTL 20
#define RECORD24G_TTL 20

#define null 0

#define _TEST_GPS 1
#define _TEST_GPRS 2
#define _TEST_Irda 3

/* key 115 Pressed  UP
key 115 Released
key 114 Pressed		DOWN
key 114 Released
key 139 Pressed		ENTER
key 139 Released
key 28 Pressed		ESC
key 28 Released*/
#ifndef SLZRKEY_UP
#define SLZRKEY_UP		115	// Arrow Up
#define SLZRKEY_DOWN	114	// Arrow Down
#define SLZRKEY_ESC		28		// Esc
#define SLZRKEY_ENTER	139		// Enter

#define SLZRKEY_F1		KEY_F1	// Function
#define SLZRKEY_F2		KEY_F2
#endif

////////////////////////////////////////////////////////////////////////////////
////////					���ܿ���
////////////////////////////////////////////////////////////////////////////////
#define HONG_WAI_XIAN_GET_DATA_EN 	
////////////////////////////////�������淶////////////////////////////////////////
#define ST_OK			0			//��������
#define ST_ERROR		1			//����ʧ��
//-----------------------------��Ƭ����ش���--------------------------------------
#define ERR_NO_PSAM			10		//�Ҳ���PSAM��
#define ERR_SELECT_MW_ADF3	11		//ѡMW_ADF3�ļ�
#define ERR_PINAUTH				12	//PIN��֤����
#define ERR_READ_MW_ADF3_EF01	13	//��MW_ADF3_EF01�������ļ�����
#define ERR_READ_RECORD_FILE	14
#define ERR_R_MW_ADF3_EF04		15	//��MW_ADF3_EF04��¼�ļ�����
#define ERR_CMP_FILE			16 //�Ƚ��ļ�����
#define ERR_CMP_ADF3_EF04		17	//������MW_ADF3_EF04�Ƚϳ���
#define ERR_R_MW_ADF3_EF06		18	//���ɼ��ɼ����кų���
#define ERR_R_MW_ADF3_EF05		19	//����ȡ������������������
#define ERR_R_MW_ADF3_EF03		20	//����ǰ�ɼ��ļ�����
#define ERR_R_MW_ADF3_EF02		21	//����ʷ�ɼ��ļ�����
#define ERR_R_MW_ADF3_EF07		22	//����ֵ���׼�������
#define ERR_R_MW_ADF3_EF08	    23	//����Ʊ���׼�������
#define ERR_R_MW_ADF3_EF09		24	//���ƿ����׼�������
#define ERR_CHEK_E2PROM_PO		50	//�Ƚ�E2PROM��ָ�����!
#define ERR_GET_KEY				51	//�Ƶ���Կ
#define ERR_GET_RESPONSE		52	//ȡ��ԿӦ��	
#define ERR_INIT_PSAM			53	//PSAM ��ʼ��ʼ��
#define ERR_MW_ADF3_EF02		54	//����ʷ�ɼ��ļ�
#define ERR_UPDATE_BIN			55	//д�������ļ�

#define ERR_MIFARE_LOAD_KEY		80	//MIFARE��װ����Կ
#define ERR_MIFARE_AUTHEN		81	//��֤MIFARE����Կ
#define ERR_MIFARE_READ			82  //��MIFARE������
#define ERR_CARD_GESHI			83	//��������У�����
#define ERR_CARD_STOP_WORK		84	//�ÿ���ͣ��
#define ERR_CARD_NO_SYSTEM		85	//�ÿ����Ǳ�ϵͳ�����Ŀ�
#define ERR_CARD_NO_USE         86	//��û������	
#define ERR_MIFARE_WRITE		87	//д������	
#define ERR_CARD_PUBLIC			88	//������������
#define ERR_MONTH_BAD			89	//��Ʊ����Ǯ����ʽ����
#define ERR_CARD_WHITE_BUS		90	//�ǰ׿�����û�г���

#define ERR_READ_TIME			100	//����ʱ�����
#define ERR_MONEY_SN			101	//Ǯ����ʽ����
#define ERR_CHARGE_MEONY		102	//���ֵ

#define SOUND_DEAL				120		//����������ʾ
#define SOUND_LEV_BUS			121		//������������ʾ
#define SOUND_ARV_BUS			122		//ͣ����������ʾ
#define SOUND_CHARGE_MONEY		123		//���ֵ��ʾ
#define SOUND_FLING_MONEY		124		//��Ͷ����ʾ
#define DEAL_NO_FINISH			125		//����û�������ʾ
#define SOUND_DEAL_FREE			126		//����������ʾ(����Ǯ�ʹ�)
#define SOUND_DI_BUS			127		//��������
//*****************������ʾ��Ϣ************************
#define TASK_ERROR_MESSAGE_PRIO	2
#define TASK_PSAM_PRIO		4
#define TASK_FEROM_PRIO		3
#define TASK_EEROM_PRIO		5
#define TASK_MIFARE_PRIO	7
#define TASK_GPRS_PRIO		8
#define TASK_GPRS_SEND		9
#define TASK_TIME_PRIO		10
#define TASK_KEY_BOARD_PRIO	11
#define TASK_DISP_PRIO		12

#define TASK_CARD_MANAGE_PRIO	37
#define TASK_MASTER_PRIO		39
#define TASK_RECORD_QUERY_PRIO	40
#define TASK_AUDIO_PRIO		56
#define TASK_BEEP_PRIO		57
#define TASK_MAC_PRIO		58
#define TASK_RECORD_WRITE_PRIO	59
#define TASK_GPRS_CONNENT_PRIO	60


#define ERR_NO_CURRENT_RECORD 	1
#define ERR_SEND_CMDPACKAGE 		2
#define	ERR_FILENAME_PACKAGE 		3
#define	ERR_SEND_DATAPACKAGE		4
#define	ERR_RECIVE_ACKPACKAGE		5
#define	ERR_PACKAGE_LENGTH		6
#define	ERR_RECIVE_DATAPACKAGE		7
#define	ERR_READ_DATE				8
#define	ERR_RECIVE_CMDPACKAGE		10
#define	ERR_SERIALNO				11
#define	ERR_PROOF_TIME				12
#define ERR_LOAD_BLACKCARD		13

#define ERR_MIF_REQUEST		1		////M1��Ѱ������
#define ERR_MIF_ANTICOLL	2		//M1������ͻ����
#define ERR_MIF_AUTEN		3		//M1������֤����
#define ERR_MIF_SELECT		4		//M1��Ƭѡ������
#define ERR_MIF_READ		5		//M1��������
#define ERR_MIF_WRITE		6		//M1д������
#define ERR_MIF_INC			7		//M1��ֵ����
#define ERR_MIF_TYPE		8		//M1�����ʹ���
#define ERR_MIF_INBLACK		9		//M1�����ں�������
#define ERR_MIF_PURSE		10		//Ǯ��������
#define	ERR_MIF_FORMAT		11		//M1Ƭ���ݸ�ʽ����
//-----------------------------����ͨѶ����--------------------------------------
#define ERR_OPEN_PORT		20		//�򿪶˿ڴ�
#define ERR_CLOSE_PORT		21		//�رն˿ڴ�
#define ERR_COM_HARDWARE	22		//Ӳ������
#define ERR_COM_TIMEOUT		23		//��ʱ����
#define ERR_COM_COMMAND		24		//����ŷǷ�
#define ERR_COM_FORMAT		25		//�����ʽ����
#define ERR_COM_SEND		26		//�����������
#define ERR_COM_RECEIVE		27		//�����������
//-----------------------------�洢����ش���--------------------------------------
#define ERR_MEM_EXHAUST		40		//�洢���ռ�ľ�
#define ERR_MEM_WRITE		41		//д��¼����
#define ERR_MEM_READ		42		//����¼����
//-----------------------------���ݲɼ���ش���--------------------------------------
#define ERR_BOX_TYPE		50		//�ɼ������ʹ���
#define ERR_BOX_BLIST		51		//�ɼ��к��������ݸ�ʽ����
#define ERR_BOX_FCB			52		//�ɼ����ļ����ƿ����ݸ�ʽ����
#define ERR_BOX_NOTEXT		53		//û�з��ֲɼ���
#define ERR_BOX_READ		54		//�ɼ������ݶ�����
#define ERR_BOX_WRITE		55		//�ɼ�������д����
#define ERR_BOX_OVER		56		//�ɼ����������
//-----------------------------�豸����ش���--------------------------------------
#define ERR_CHG_NUM			60		//��ֵ���Ƿ�
#define ERR_CHG_DEVTYPE		61		//�豸���ʹ���
#define ERR_CHG_DEVATHEN	62		//�豸δ��Ȩ
#define ERR_CHG_NOINIT		63		//�豸û�г�ʼ��
//-----------------------------PSAM����ش���--------------------------------------
#define ERR_PSAM_NOREPEN	70		//û�и�λӦ��
#define ERR_PSAM_FILESEL	71		//�ļ�ѡ�����
#define ERR_PSAM_PINAUTH	72		//PIN��֤����
#define ERR_PSAM_PAGELEN	73		//���ȴ���
#define ERR_PSAM_NOINFO		74		//����Ϣ�ṩ
#define ERR_PSAM_FILEINVAL	75		//ѡ���ļ���Ч
#define ERR_PSAM_AUTHEN		76		//��֤����
#define ERR_PSAM_WREEPROM	77		//дEEPROMʧ��
#define ERR_PSAM_DATALEN	78		//���ݳ��ȴ���
#define ERR_PSAM_INVASTATE	79		//��Ч��״̬
#define ERR_PSAM_FILETYPE	80		//�ļ����Ͳ�ƥ��
#define ERR_PSAM_SSTATE		81		//��ȫ״̬������
#define ERR_PSAM_KEYLOCK	82		//��Կ�ѱ���ס
#define ERR_PSAM_DATAIN		83		//����������Ч
#define ERR_PSAM_TERM		84		//������������
#define ERR_PSAM_SCPAGKE	85		//��ȫ�����������ȷ
#define ERR_PSAM_DATAFLD	86		//�������������ȷ
#define ERR_PSAM_FUNC		87		//���ܲ�֧��
#define ERR_PSAM_NOFILE		88		//û���ҵ��ļ�
#define ERR_PSAM_RECORD		89		//û���ҵ���¼
#define ERR_PSAM_P1P2		90		//P1��P2��������ȷ
#define ERR_PSAM_INDATA		91		//δ�ҵ���������
#define ERR_PSAM_PARM		92		//��������
#define ERR_PSAM_ICINS		93		//����ȷ��INS
#define ERR_PSAM_ICCLA		94		//����ȷ��CLA
#define ERR_PSAM_UNKNOW		95		//δ����Ĵ���
#define ERR_PSAM_IVMAC		96		//MAC��Ч
#define ERR_PSAM_PLOCK		97		//Ӧ����������
#define ERR_PASM_BUDGET		98		//����
#define ERR_PSAM_SPKEY		99		//��Կ������֧��
#define ERR_PSAM_SPMAC		100		//����MAC��֧��

#define ERR_SELECT_MAIN		102		//ѡ���ļ�����
#define ERR_SELECT_ADF3		103		//ѡ��ADF3�ļ�����
#define ERR_SELECT_ADF3_EF01 104	//ѡ��ο��ļ�����
#define ERR_READ_ADF3_EF01	105		//���ο��ļ�����	
#define ERR_READ_READFILE	106		//�������ļ�����
#define ERR_ADD_READFILE	107		//׷��ѭ���ļ�����	
#define ERR_INIT_FOR_DESCRYPT	108	//MAC��ʼ������
#define ERR_DAOCHUKEY_1			109  //������һ������Կ
#define ERR_MAC					110		//MAC �����������

#define ERR_LOAD_KEY			112	
#define ERR_MIF_AUTHEN	    	113
#define ERR_GET_M1KEY			114		//�Ƶ�M1����Կ����	
//#define ERR_CARD_GESHI			115		//��������У�����
#define ERR_MIF_CHANGE			116		//��ֵ��������
#define ERR_MIF_TRANSFER		117	
//#define ERR_UPDATE_BIN			118		//д�������ļ�����
#define ERR_EEPROM_FULL			200
#define ERR_BUFFER_FULL			201
#define ERR_CARD_RESTORE		202
//------------�ļ���ʶ����-------------
#define KEYA				0
#define KEYB				1
#define PSAM_HEAD_FLAG		0x3b
#define MW_CITY_STD			0x86 	//����PSAM����־	
#define MW_EF15				0x15	//Ӧ�ù�����Ϣ��ʶ
#define MW_MF				0x3f00	//���ļ���ʶ	
#define MW_ADF3				0x1003	//�ǽӴ�ADF3Ŀ¼Ӧ�ñ�ʶ
#define MW_ADF2				0x1002	//һ��ͨר��Ŀ¼Ӧ�ñ�ʶ
#define MW_ADF3_EF01		0x01	//������Ϣ�ļ���ʶ
#define MW_ADF3_EF04		0x04	//������Ϣ�ļ�
#define RECORD_CAPTION		0x30	//���׼�¼MAC��Կ��ʶ
#define RECORD_KEY_TYPE		0x08	//���׼�¼MAC��Կ����	
#define MW_ADF3_EF05		0x05	//��������Ϣ�ļ�������	
#define MW_ADF3_EF02		0x02	//��ʷ�ɼ��ļ���Կ��ʶ		
#define MW_ADF3_EF03		0x03    //��ǰ�ɼ��ļ�	
#define MW_ADF3_EF06		0x06	//�ļ����к�
#define MW_ADF3_EF07		0x07	//��ֵ���׼���
#define MW_ADF3_EF08		0x08	//��Ʊ���׼���
#define MW_ADF3_EF09		0x09	//���ƿ����׼���
#define MW_KEY1_A			0x20	//��������A��Կ��ʶ	
#define MW_KEY2_A			0x60	//��¼�����͹�����Ϣ��A��Կ��ʶ
#define MW_KEY3_A			0xa0	//��Ʊ��A��Կ��ʶ	
#define MW_KEY4_A			0x40	//����Ǯ������A��Կ��ʶ	
#define MW_KEY5_RECORD		0x30	//�������ݵ���Կ��ʶ	

#define MF_CARD_MES			0x0015
#define MF_TREE_MES			0x0001
#define MF_ZHOUDUANG_MES	0x0016
#define ADF1				0x1001
#define ADF1_PUBLIC_MES		0x0017
#define ADF2				0x1002
#define ADF2_KEY			0x0
#define ADF3				0x1003
#define ADF2_PUBLIC_MES		0x0002
#define ADF2_DEAL_COUNT		0x0004

#ifdef _debug_
#define WAIT_TIME 300
#else
#ifdef BUS_SYBAOSHI_
#define WAIT_TIME 20
#elif defined ZongKong_CPU_ //�пص�CPU��������
#define WAIT_TIME 2
#elif defined _HD_XIAOER_ //����С�����ѵȴ�ʱ��2Сʱ
#define WAIT_TIME 20
#else
#define WAIT_TIME 300
#endif
#endif

///////////////////////////////�̶����ȶ���/////////////////////////////////
#define CARD_MESSAGE_LEN		8
#define CITY_CODE_LEN			2
#define TRADE_CODE_LEN			2
#define PUBLISH_BICKER_LEN		4
#define CARD_CHECH_CODE_LEN		4
#define PRIVATE_PIN_LEN			6

///////////////////////////////������������/////////////////////////////////
#define NOT_BLACK				0
#define IS_BLACK				1

//////////////////////���̱�־˵��/////////////////////////////////
#define PRO_RESET				0		//�����̱�־��λ	
#define PRO_WRITE_MAIN_RECORD	0x10		//д����¼��E2PROM
#define PRO_WRITE_BACK_RECORD	0x8A		//д���ݵ�E2PROM
#define PRO_WRITE_PSAM_POINTER1	0x77		//д����ָ�뵽PSAM��
#define PRO_WRITE_PSAM_POINTER2	0x78
#define PRO_COUNT_MAC			0x79

#define WR_MONEY				0x90
#define WR_MONTH				0x91
#define WR_CONTROL				0x92	
#define REQUEST_TWO				0x98
///////////////////////������������/////////////////////////////

//***************************************************************************
//                       �����״�������ĸ�ʽ���� 
//***************************************************************************
#define IS_EFFECT					0x55	//������Ϣ��Ч	
#define IS_EFFECT_MONEY				0x77	//����Ǯ����
#define IS_EFFECT_MONTH				0x88	//������Ʊ����

#define OPER_TRANSFER_NO			0xa0	//�ڽ�����ʱִ��mif_transfer���ɹ�
#define OPER_RECORD_PROFILE			0xa1	//��¼����ʧ��
#define OPER_PUBLIC_MESSAGE			0xa2	//д������Ϣʧ��		

#define OPER_BLACK					0xa3	//����������
#define OPER_PUBLIC_MESSAGE_W		0xa4	//д������Ϣ����
#define ERROR_MONTH					0x80
#define ERROR_MONEY					0x82
#define ERROR_BLACK					0x83
#define OPER_CHECK_MEONY			0x56	//��Ǯ	
#define ERROR_DETAIL_LEN			32

#define ERROR_DETAIL_SUM			3
//////////////////////////////���ñ�־/////////////////////////////////
#define STARTUP_OK		0x02	//����
#define STARTUP_NO		0x01	//û������		
#define STARTUP_STOP	0x03	//ͣ��		
#define STARTUP_BLACK	0x04	//������		

///////////////////////////////���ֿ������б�־/////////////////////////////////
							//��ֵ����
#define CARD_CHAGEERR	0x4F
#define CARD_BLACK		0x50			
#define CARD_STOPWORK	0x51			
#define CARD_NO_SYSTEM	0x52			
#define CARD_MONEY		0x53			
#define MONEY_CARD		0x53			
#define INVALID_CARD	0x54			
#define MONTH_CARD		0x55			
#define CONTROL_CARD	0x56
#define CARD_BLACK_PRO	0x57
#define CARD_NO_TIME	0x58	
#define CARD_HUIFU		0x59
#define CARD_NO_USE		0x5a			
#define CARD_PUBLIC_BAD	0x5b
#define CARD_NO_USER1	0x5c
#define CARD_DATA_BAD	0x5d
#define CARD_WRITE_CARD	0x5e
#define CARD_YEAR		0x5f
#define CARD_FREE		0x60
#define CARD_NO_Busins	0x61

#define CARD_forbid		0x62




///////////////////////////////���ֿ��Ķ���/////////////////////////////////			
#define CARD_DATA_GATHER	61				//���ݲɼ���
#define CARD_DEV_SET		62				//�豸���ÿ�
#define CARD_LINE_SET		63				//��·���ÿ�
#define CARD_NUMBER_SET		64				//�������ÿ�
#define CARD_DELAY_SET		65				//��ʱ��
#define CARD_TOLL_START		66				//�շ�������
#define CARD_TIME_SET		67				//ʱ�����ÿ�
#define CARD_LEV_BUS		68				//������
#define CARD_ARV_BUS		69				//��վ��
#define CARD_DRIVER_BUS		70				//˾����
#define CARD_CLERK_BUS		71				//վ��Ա��
#define CARD_AIR_BUS		72				//�յ���
#define CARD_MONTH_STA		73				//��Ʊ������

#define CARD_WHITE_BUS		50				//�׿�	
#define CARD_WHITE_MOBILE	51				//�ֻ��׿�
#define CARD_WHITE_CPU		52				//CPU�׿�

//////////////////////////�û����Ķ���///////////////////////////

///////////////////////////////�����˿ں�//////////////////////////////
#ifdef AUDIO_SOUND_OLD //�ϵ�16������
//��Ƭ8������
#define Audio_DONG			1		//��
#define Audio_CY			2		//�ŷ���
#define Audio_STUDENT		3		//ѧ����
#define Audio_BIRTHDAY		4		//���ǿ�
#define Audio_STUFF			5		//Ա����
#define Audio_OLDMAN		6		//���˿�
#define Audio_PLUS_MONEY	7		//���ֵ
#define Audio_FLING_MONEY	8		//��Ͷ��
#define Audio_YH			9		//�Żݿ�
#define Audio_LOVER			10		//���Ŀ�
#define Audio_CYman			11		//�Ÿ���(����)
#define Audio_STUFFman		12		//Ա����(����)
#define Audio_OLDMANman		13		//���꿨(����)
#define Audio_INVALID		14		//�Ƿ���
#define Audio_TRY_AGAIN		15		//����ˢ
#define Audio_STUDENTman	16		//ѧ����(����
#define Audio_MONTH			15		//��Ʊ��
//��������
#elif defined BUS_BAOJI_
#define Audio_DONG			1		//��
#define Audio_CY			2		//�ŷ���
#define Audio_STUDENT		3		//ѧ����
#define Audio_BIRTHDAY		4		//���ǿ�
#define Audio_STUFF			5		//Ա����
#define Audio_OLDMAN		6		//���꿨
#define Audio_PLUS_MONEY	7		//���ֵ
#define Audio_FLING_MONEY	8		//��Ͷ��
#define Audio_INVALID		9		//�Ƿ���
#define Audio_FAMILY		10		//������
#define Audio_Zilu			11		//��Ů��
#define Audio_YWJYu			12		//������
#define Audio_JC			13		//���鿨
#define Audio_LOVER			14		//���Ŀ�
#define Audio_TRY_AGAIN		15		//����ˢ
#define Audio_GSK			16		//��ʧ��
#define Audio_FACE			17		//������
#define Audio_DaoZan		18		//��վ��
#define Audio_CAIJI			19		//�ɼ���
#define Audio_Siji			20		//˾����
#define Audio_YH			21		//�Żݿ�
#define Audio_Wuxiao		22		//��Ч��
#define Audio_JianDu		23		//�ල��
#define Audio_XianLu		24		//��·��
#define Audio_DiaoDu		25		//���ȿ� վ��Ա��
#define Audio_GuanLi		26		//����
#define Audio_DaZe			27		//���ۿ�
#define Audio_MONTH			28		//��Ʊ��
#define Audio_QianDaoEnd	29		//ǩ�����
#define Audio_zuanxian		30		//ר�߿�
#define Audio_TuanTi		31		//���忨
#define Audio_YuerDi5		32		//������5Ԫ

//��������	#elif defined BUS_BAOJI_
#else//��32������
//һƬ32������
#define Audio_INVALID		2		//�Ƿ���
#define Audio_NOMONEY		3		//����//����
#define Audio_FAMILY		4		//������
#define Audio_Zilu			5		//��Ů��
#define Audio_YWJYu			6		//���鿨  ������  ������������Ž���
#define Audio_STUDENT		7		//ѧ����
#define Audio_STUFF			9		//Ա����
#define Audio_OLDMAN		10		//���˿�
#define Audio_LOVER			11		//���Ŀ�
#ifdef SOUND_New_B
#define Audio_Man_STUFF		12		//Ա����(��)
#else
#define Audio_TuanTi		12		//���忨
#endif
#define Audio_Wuxiao		13		//��Ч��
#define Audio_zuanxian		14		//ר�߿� ǩ�����, ���³�
#define Audio_MONTH			15		//��Ʊ��
#define Audio_PuTong		16		//��ͨ��
#define Audio_YH			17		//�Żݿ�
#define Audio_DONG			18		//��
#define Audio_DingDang		19		//���ˣ����ſ�
#define Audio_Di			20		//��
#ifdef SOUND_New_B
#define Audio_NODRen		21		//���ܴ��˳˳�
#else
#define Audio_YongJun		21		//ӵ����
#endif
#define Audio_SuaKaOK		22		//ˢ���ɹ�
#define Audio_YouDai		23		//�Ŵ���
#define Audio_Siji			24		//˾����
#define Audio_PLUS_MONEY	25		//���ֵ..B=�뼰ʱ��ֵ
#ifdef SOUND_New_B
#define Audio_MAN_STUDENT	26		//ѧ������
#else
#define Audio_BIRTHDAY		26		//���Ͽ�
#endif
#define Audio_CY			27		//�ŷ���
#define Audio_FULI			28		//������
#define Audio_GuaShi		29		//��ʧ��,�뽻��
#ifdef SOUND_New_B
#define Audio_MAN_Month		30		//��Ʊ����
#else
#define Audio_XiaoXueShen	30		//Сѧ����
#endif
#define Audio_XueShenYP		31		//ѧ����Ʊ��
#ifdef SOUND_New_B
#define Audio_Man_FULI		32		//��������
#else
#define Audio_YuerDi5		32		//������5Ԫ
#endif

#define Audio_no_di			0xee	//û������ֻ�Ƿ�������

#define Audio_TRY_AGAIN		1		//����ˢ 
#define Audio_FLING_MONEY	8		//��Ͷ��

#endif

///////////////////////////////Ŀ¼������/////////////////////////////////
#define LIST_AREA			0x00		//Ŀ¼��
#define PUBLISH_AREA		0x01		//������
#define DEALRECORD_AREA		0x03		//���׼�¼��3
#define PBULICMESSAGE_AREA	0x06		//������Ϣ��
#define ODDMONTH_AREA		0x07		//����Ʊ��
#define DODMONTH_AREA		0x08		//˫��Ʊ��
#define PUBLICMONEY_AREA	0x10		//����Ǯ����
#define NOUSE_AREA			0xff		//δʹ������

///////////////////////////////��������/////////////////////////////////
#define ID_REC_TOLL			2			//Ǯ������
#define ID_REC_MON			3			//��Ʊ����
#define ID_REC_MOBILE		4			//�ƶ���¼,(ԭ�����ݲɼ���¼����)
#define ID_REC_TIMESET		5			//ʱ������
#define ID_REC_TIMEDELY		6			//ʱ������
#define ID_REC_DEVNOSET		7			//���ƺ�����
#define ID_REC_PAKAGE		8			//���ݰ��ָ�
#define ID_REC_DATE			9			//���ڱ��
#define ID_REC_DRIVENO		10			//˾������¼
#define ID_REC_CLERK		11			//վ��Ա����¼
#define ID_REC_LINESET		12			//��·���ÿ�
#define ID_REC_BLKLIST		13			//��������ˢ��
#define ID_REC_AUTHOR		14			//�豸��Ȩˢ��
#define ID_REC_DTRAN2		15			//���ݲɼ�
#define ID_REC_DEVSET		16			//�豸������
#define ID_REC_AIRCON		17			//�յ�������
#define ID_REC_MONSTT		18			//��Ʊ������
#define ID_REC_LEVBUS		19			//������
#define ID_REC_ARVBUS		20			//��վ��
#define ID_REC_TOLLSTT		21			//�շ�������
#define ID_REC_DEVNOSET1	22			//���ƺź�����
#define ID_REC_NO			23			//��ͷ�ļ� 
#define ID_REC_FUEL1		24			//���ͼ�¼1
#define	ID_REC_FUEL2		25			//���ͼ�¼2		
#define ID_REC_LINENO		26			//������·��	
#define ID_REC_FDDown		27			//�ֶ��շ��е��³���¼
#define ID_REC_FDUp			28			//�ֶ��շ��е��ϳ���¼
#define ID_REC_PURSE		80			//Ǯ����ֵ
#define ID_REC_MONTH		81			//��Ʊ��ֵ
#define ID_REC_DXBAOSI		90			//������ʯˢ����¼
#define ID_REC_CARDLOCK		99			//��������

#define ID_REC_CS456_ASCii	101			//0x65 �������ڼ�¼���� #ifdef CS456_ASCii_

#define ID_REC_GPS			0xE0		//GPS��¼ վ̨��¼
#define ID_REC_GPS_OF		0xE3		//GPS��¼ �����վ
#define ID_REC_GPS_OZ		0xE4		//GPS��¼ �����վ
#define ID_REC_GPS_JZ		0xE5		//GPS��¼ �����վ
#define ID_REC_GPS_JF		0xE6		//GPS��¼ �����վ
#define ID_REC_START		50			//���ػ��Լ�

#define YCD_ID_TOLL			0x01
#define YCD_ID_MONTH        0x47
//******************PurseProcess()���ش���///////////////
#define RET_MONEY_SN_EROR 20	//�ÿ���Ǯ��������ʽȫ��

#ifdef MOBILE_XIANYANG_
typedef struct
{
	unsigned char Tyear;//���
	unsigned short Ttimes;//ʹ�ô���
}CardUserTimes;
#endif

//****************************************************************
//              ���������Ҫ��ŵ�����洢��
//****************************************************************
#define BIT_IS_FIRST_TIME	5					//4  ��һ�����г�ʼ��־
#define BIT_F_FLAG_VARY		BIT_IS_FIRST_TIME+4	//7 ������վ״̬
#define BIT_POINT_MESSAGE	BIT_F_FLAG_VARY+7	//17 ������ˮ��Ϣ
#define BIT_COMSUME_FILE	BIT_POINT_MESSAGE+17//32 ������Ϣ�ļ�
#define BIT_PRICETAB		BIT_COMSUME_FILE+32 //512 Ʊ�ۼ�������Ϣ�ļ� BIT_BASIC_MESSAGE

#define BIT_DEVICE_NO		BIT_PRICETAB+512	//9 �豸�ն˺�
#define BIT_DRICE_CARD	    BIT_DEVICE_NO+9		//4 ˾������
#define BIT_LINENO			BIT_DRICE_CARD+4	//3 ��·��
#define BIT_BLACK_SUM		BIT_LINENO+3		//4 ������
#define BIT_ERROR_DETAIL	BIT_BLACK_SUM+4		//32*3=96 ˢ�������־
#define BIT_ERROR_POINT		BIT_ERROR_DETAIL+96	//1 ˢ������ָ��
#define BIT_HISDATA_GATHER	BIT_ERROR_POINT+1	//210		��ʷ�ɼ��ļ�
#define BIT_GET_FILESNO		BIT_HISDATA_GATHER+210//2 �ļ���
#define BIT_ZCYH			BIT_GET_FILESNO+2	// 3 ת�˲���
//	//---��������ˢ��ʱ���
#define BIT_INTERVAL_STAR	BIT_ZCYH+3	//1			��ʼָ��
#define BIT_INTERVAL_END	BIT_INTERVAL_STAR+1		//1			����ָ��
#define BIT_INTERVAL_DATA	BIT_INTERVAL_END+1		//500		����ʱ�������

#define FE_PACKSERIAL		BIT_INTERVAL_DATA+500 //2 TCP/IP�����к�
#define FE_REC_START		FE_PACKSERIAL+2		//4 ��¼��ָ��****************************************************
#define FE_REC_CURRENT		FE_REC_START+4		//4 ��¼��ǰָ��****************************************************
#define FE_GPS_DIRECT		FE_REC_CURRENT+2		//9 GPS��λ���򣺱��+����+ʱ��
#define BIT_TCPIP_PACKET	FE_GPS_DIRECT+9    //500 Ҫ���͵�IP��


//#define BIT_SETUP_AUDIO		BIT_TCPIP_PACKET+500//32 ����
#define BIT_SETUP_AUDIO		BIT_TCPIP_PACKET+500//20 û��
#define BIT_BAK_ADDR		BIT_SETUP_AUDIO+20	//12,�׵�ַ+β��ַ+У��
#define BIT_BLACK_Ver		BIT_BAK_ADDR +12//32	�������汾��(CN) û�ã��汾���ں������ļ���,���ڴ�ź�����16���������ÿ�����ֽ�һ��

#define BIT_RE_SUM			BIT_BLACK_Ver+32	//4 �����ܰ�
#define BIT_TCP_REV_SUM		BIT_RE_SUM+4//4 �����ܰ�
#define BIT_GPRS_IP			BIT_TCP_REV_SUM+4//4 IP��ַ
#define BIT_GPRS_PORT		BIT_GPRS_IP+4//2 �˿ں�
#define BIT_SEND_SUM		BIT_GPRS_PORT+2//4 �ܷ��Ͱ�����

#define BIT_MobilStyle		BIT_SEND_SUM+4//60 ǩ��״̬
#define BIT_MobilPOSNS		BIT_MobilStyle+60//4 �յ����ն�ϵͳ���ٺ� POS��ˮ
#define BIT_MobilREC_Start	BIT_MobilPOSNS+4//4 �ƶ���¼��ʼָ��
#define BIT_MobilREC_End	BIT_MobilREC_Start+4//4 �ƶ���¼����ָ��

#define BIT_MobilePRO_INFO	BIT_MobilREC_End+4//27 �ƶ�GPRS�������ز���(����)��
#define BIT_BUSDownPRO_INFO	BIT_MobilePRO_INFO+27//27 ����GPRS�������ز���(����)��
#define BIT_BUSblkDownPRO_INFO	BIT_BUSDownPRO_INFO+27//39 ����GPRS�������ز���(����)��

#define BIT_SendInfo		BIT_BUSblkDownPRO_INFO+40//44 ���ͼ�¼��Ϣ��
#define BIT_PICINO_BAK		BIT_SendInfo+44			//7 ���α���
//����ʱ�����к� //5�ֽڣ���һ�ֽ���У���롣
#define BIT_SendSeril		BIT_PICINO_BAK+7
//6 + 24 + 7 =37�����еĲ��� + �ҵ���� + ʱ��
#define BIT_GPS_RUN			BIT_SendSeril+6
//��Ӣ�Ĵ��� 1�ֽ�
#define BIT_CHN_ENG			BIT_GPS_RUN+38
//4 �ƶ���¼��ʼָ�� ����
#define BIT_MobilREC_Start_BAK	BIT_CHN_ENG+1
//4 �ƶ���¼����ָ�� ����
#define BIT_MobilREC_End_BAK	BIT_MobilREC_Start_BAK+4
//1 �ƶ���¼��ʼָ�� ����У��
#define BIT_MobilREC_Start_BAK_CRC	BIT_MobilREC_End_BAK+4
//32 ������
#define BIT_BLACK_2BLOK BIT_MobilREC_Start_BAK_CRC+4
//27 ����GPRS�������ز���(����)��
#define BIT_GPSDownLine_INFO	(BIT_BLACK_2BLOK+32)
//4 pboc ��¼��ˮ��
#define BIT_PBOC_NS	BIT_GPSDownLine_INFO+32
//160  ��ʷָ�����ڣ�ÿ�����һ����ʷָ���ͳ������ ����ʱ��7+[����+��ֵ�ۼƽ��4+...]+CRC32(4)   8�ſ�
#define BIS_PERDat_Info (BIT_PBOC_NS+4)
//4 ˾��Ա�����
#define BIT_STUFFER_NO (BIS_PERDat_Info+160)
//20 ��ʷ��¼�ϴ���ַ.����GPRS������ʷ��¼ָ��.(�׵�ַ4+����4+����ʱ��7+����1+CRC324)=20�ֽ�,�׵�ַ����Ե�ַ,Ҫ����FLASH2_START1
#define BIT_HISREC_SND (BIT_STUFFER_NO+4) //2536
//8 �ۼ�ˢ������ ����4+У��4
#define BIT_SWAPCARD_TIMES (BIT_HISREC_SND+20)
//8 ���õ�ˢ������ ����4+У��4
#define BIT_SWTIMES_MAX (BIT_SWAPCARD_TIMES+8)
//28 ��ͳ�ƣ�����ǰһ���º͵�ǰ�µ�ˢ��ͳ�� 2 * (����2+ˢ���ܴ�4+ˢ���ܽ��4+CRC32У��4) = 28�ֽ�
#define BIT_MOTH_TOJI_2 BIT_SWTIMES_MAX+8
//8 ���ػ����ñ�ʶ���ں�̨ǩ�����ؼ��������и��� Ŀǰֻ���������Ƿ����õġ�"��"+"0x00"+��ʶ1+CRC32У��4 = 8�ֽ�
#define BIT_Center_Ctrl_BU_Data (BIT_MOTH_TOJI_2+28)
//4 ��λ,���ű��
#define BIT_UNIT (BIT_Center_Ctrl_BU_Data+8)
// 136�Ҽ�¼�洢������128�ֽڣ�����Ч��4 + 128 data+4CRC = 136
#define BIT_UNKNOW_RECORD	(BIT_UNIT + 4)

#define BIS_BLK_63          (BIT_UNKNOW_RECORD+136)     //  0x1A29,
#define BBIS_BLK_63_LEN      26


//��������Ϣsizeof(stTradeBatchinfo)) =24
#define BIT_TradeBatchinfo		BIS_BLK_63+BBIS_BLK_63_LEN
//stMobilStyle = 88�ֽ�
#define BIT_SingInStyle		(BIT_TradeBatchinfo+ 24) // ǩ��״̬
//pos �Ĳ�����Ϣ  stMobileParameter  64
#define BIT_qpbpc_para			BIT_SingInStyle +  88+30   //192

//4 pboc ��¼��ˮ�� 999999 ��+1
#define BIT_PBOC_NS_BIG		(BIT_qpbpc_para+64+64+128) 

//500�ֽڣ�st_qpoc_repurse �ṹ�� ���ڴ洢��������ʱ��¼������е�ʱ��Ҫ���͸���̨
#define BIT_repurse_infor	BIT_PBOC_NS_BIG+4
#define BIT_repurse_infor_LEN      850

#define BIT_PBOC_NS_8583           (BIT_repurse_infor + BIT_repurse_infor_LEN)     //  0x1019,�����õ�11������ˮ4�ֽ� BIT_PBOC_NS �����ʱ�򲻶ԣ��ᱻ���ǣ����߾���û������û�д洢�ϣ�
#define BIT_PBOC_NS_8583_LEN       4

#define BIS_PAY_MODE          (BIT_PBOC_NS_8583+BIT_PBOC_NS_8583_LEN)     //  0x1A29,
#define BIS_PAY_MODE_LEN      4
#define BIT_BAK_ADDR_add		(BIS_PAY_MODE+BIS_PAY_MODE_LEN)	//12,�׵�ַ+β��ַ+У��
#define BIT_BAK_ADDR_add_LEN 12

#define BIT_TMS_FILE_DOWNLOAD (BIT_BAK_ADDR_add+BIT_BAK_ADDR_add_LEN)     //  0x1A29,
#define BIT_TMS_FILE_DOWNLOAD_LEN      216

//192   ����������ʱ��¼
#define BIT_qPBOC_Rec_Temp (BIT_TMS_FILE_DOWNLOAD+BIT_TMS_FILE_DOWNLOAD_LEN)

#define BIT_END_ADDR		(BIT_qPBOC_Rec_Temp+256)

//----------------------------------
//28(24) ����������Ϣ  �������ݴ���7168��ʼ�ĵ�ַ��7K�����繲8K����1024�ֽ�
#define BIT_GPSDiaoDuCMD_INFO	7168
//65+7 ������������Ϣ����,�ɴ�64�ֽڣ�32������ +7�ֽ�����ʱ��,���и�������
#define BIT_GPS_MESSAGE		BIT_GPSDiaoDuCMD_INFO+28
//48 ����Ӫ����Ϣ
#define BIT_GPSDDYY_INFO	BIT_GPS_MESSAGE+72
//1 ��������
#define BIT_GPSSOUND_V		BIT_GPSDDYY_INFO+48
//2 �����õ�������·��
#define BIT_GPSChildLine	BIT_GPSSOUND_V+1
//4 �����������ʱƱ��
#define BIT_KeyPrice		BIT_GPSChildLine+2
//�Ѿ��õ����������ַ
#define BIT_FE_END_ADDR		BIT_KeyPrice+4	//7318
//******************������Ϣ�ṹ����************	
#define LEN_CITY_CODE		0
#define LEN_DEVICE_NO		4
#define LEN_DEVICE_No1		13
#define LEN_LINE_NO			17
#define LEN_PRICE			20

#define RES_PUBLIC_D 0x01
#define RES_PUBLIC_FLAG 0x02
#define RES_PUBLIC_Z 0x04
#define RES_PUBLIC_F 0x08
#define RES_MONEY_D 0x10
#define RES_MONEY_Z 0x20
#define RES_MONEY_F 0x40

// typedef struct  {
// 	unsigned char ucValid;				//�����Ƿ���Ч
// 	unsigned char Cmd;					//����
// 	unsigned short usLen;				//���� data+BCC
// 	unsigned short usIndex;
// 	unsigned char databuf[10];			//���� data+BCC
// }stLcdCmd;
//--------------��¼------------------

// #ifdef BUS_HANDAN_
// #define RECORD_LEN 32
// #else
#define RECORD_LEN 64
//#endif

#define MAX_REC_NUM	(LEN_EEPROM / RECORD_LEN)   //eeprom�д�ŵ�����¼��Ŀ
//**********************************************
//           ���׼�¼�ĸ�ʽ����				  //
//**********************************************
#if 0
#ifdef BUS_HANDAN_XX
typedef struct
{
	unsigned char rPublishNumb[4];		//���п���ˮ��
	unsigned char rCardDealNumb[4];		//��Ƭ������ˮ
	unsigned char rCardType;			//����
	unsigned char rDealType;			//��������

	unsigned char rMoneyDealSum[2];		//Ǯ���ۼƽ��״���	
	unsigned char rAfterMoney[4];		//ԭ��
	unsigned char rDealMoney[3];		//���׽��
	unsigned char rDealTime[7];			//����ʱ��YYYY/MM/DD/HH/MM/SS
	unsigned char TTL;					//��¼����ʱ��
	unsigned char rRecordType;			//��¼����
	unsigned char rMAC[4];				//��¼��MAC��
}DEALRECODE_32B;	//����

typedef struct 		//���ƿ�����
{
	unsigned char rPublishNumb[4];		//���п���ˮ��
	unsigned char rCardDealNumb[4];		//��Ƭ������ˮ
	unsigned char rCardType;			//����
	unsigned char rDealType;			//��������

	unsigned char rDealTime[7];			//�ɼ�ʱ��
	unsigned char rDriveNo[4];			//�豸��
	unsigned char rLineNo[4];			//��·���
	unsigned char rLineNO_1;			//��·�ֺ�
	unsigned char TTL;					//��¼����ʱ��
	unsigned char rRecordType;			//��¼����
	unsigned char rMAC[4];				//��¼��MAC��
}Get_data_2;
typedef struct
{
	unsigned char rPublishNumb[4];		//���п���ˮ��
	unsigned char rCardDealNumb[4];		//��Ƭ������ˮ
	unsigned char rCardType;			//����
	unsigned char rDealType;			//��������

	unsigned char rDealTime[7];			//�ɼ�ʱ��
	unsigned char MoneySum[4];				//���������ܽ��
	unsigned char MonthSum[2];				//������Ʊ�ܴ�
	unsigned char RecordSum[2];				//�����ܼ�¼��
	unsigned char rReserveByte;			//����
	unsigned char TTL;					//��¼����ʱ��
	unsigned char rRecordType;			//��¼����
	unsigned char rMAC[4];				//��¼��MAC��
}Get_data_1;

typedef struct 		//���ƿ�����
{
	unsigned char rPublishNumb[4];		//���п���ˮ��
	unsigned char rCardDealNumb[4];		//��Ƭ������ˮ
	unsigned char rCardType;			//����
	unsigned char rDealType;			//��������

	unsigned char rDelaTime[7];			//����ʱ��
	unsigned char rPublishNumb2[4];		//���п���ˮ��
	unsigned char rReserveByte[5];		//����
	unsigned char TTL;					//��¼����ʱ��
	unsigned char rRecordType;			//��¼����
	unsigned char rMAC[4];				//��¼��MAC��
}Record_Control_32B;	//����
#else  //#ifdef BUS_HANDAN_
typedef struct
{
	unsigned char rPublishNumb[4];		//���п���ˮ��
	unsigned char rDeviceNo[4];			//�豸��
	unsigned char rCardType;			//����
	unsigned char rDealType;			//��������
	unsigned char rMoneyDealSum[2];		//Ǯ���ۼƽ��״��� [�ֶ�(�ϳ���¼)-ȫ��Ʊ��]
	unsigned char rAfterMoney[4];		//ԭ��
	unsigned char rDealMoney[3];		//���׽��
	unsigned char rDealTime[7];			//����ʱ��YYYY/MM/DD/HH/MM/SS
	unsigned char rRemain;				//���� [�ֶ� վ����]
	unsigned char rPublishNumb_a;		//���п���ˮ�����λ 
//	unsigned char rTradCode[2];			//��ҵ���� ����������ͬ��Կ�Ĳ�ͬ���л�Ӧ��-------
	unsigned char rDriverNo[4];			//˾����

	unsigned char rCardDealNumb[4];		//�豸������ˮ
	unsigned char rLineNo[2];			//��·��
	unsigned char rResever1[4];			//�豸Ψһ���к� 42  ������ ���ű��
	unsigned char ruserTimes[2];		//ʹ�ô��� ˾���Ÿ�λ  [�ֶ� ȫ��ʱ��] ������пصļ�¼�����湲18�ֽ�Ϊ��Ƭ�����֤��
	unsigned char oldLineNO[2];			//����·�� [�ֶ� �ϳ�ˢ������]
	unsigned char PbocTac[4];			//pboc����TAC�롣
	unsigned char rBussinessNo[2];		//�̻����
	unsigned char rstuffernum[4];		//˾��Ա�����
	unsigned char rCardCand;			//����:�ֻ���\�մ￨\������\CPU��
	unsigned char rReseverX;			//���� 
	unsigned char rProVer[2];			//����汾��
	unsigned char rTAC[4];				//У����
}DEALRECODE;

#endif //#ifdef BUS_HANDAN_
#endif

typedef struct
{
	unsigned char rDealTime[7];			//����ʱ��YYYY/MM/DD/HH/MM/SS
	unsigned char rPublishNumb[2];		//û��
	unsigned char rDealType;			//�������� 10

	unsigned char rDeviceNo[4];			//�豸��Ψһ��
	unsigned char rCardType[2];			// û��
	unsigned char rName[16];			//������
	unsigned char rNameID[16];			//֤����
	unsigned char rCardID[16];			//����
}DEALRECODE_CS456_ASCii;

typedef struct
{
	unsigned char rDeviceNo[4];//�豸��
	unsigned char rLineNo[2];//��·��
	unsigned char AID1[3];//Ӧ�ñ�ʶAID1 CN ��������
	unsigned char dealType;//��������	0x04
	unsigned char AID2[3];//Ӧ�ñ�ʶAID2 CN ��������
	unsigned char rDriverNo[4];	//˾����
	unsigned char dealCardNo[10];//���׿���	Ǯ������ N
	unsigned char posSerial[3];//POS�ն˽�����ˮ�� N ѹ��BCD��

	unsigned char KEYflagNO;//��Կ������ CN
	unsigned char KEYVer;//��Կ�汾�� N
	unsigned char TAC[4];//���ѳ�ֵTAC B
	unsigned char purseMoney[4];//���ѽ��׽�� B
	unsigned char pursestyle;//�������ͱ�ʶ B
	unsigned char deviceNo[6];//POS�ն˺� CN
	unsigned char PSAMdealSerial[4];//�ն˽������	PSAM������ B ���ڱ��桡�ն˱�ʶ�롣
	unsigned char date[4];//���ѽ������ڣ��նˣ�CN
	unsigned char time[3];//���ѽ���ʱ�䣨�նˣ�CN
	unsigned char afterMoney[4];//Ǯ�������׺���B
	unsigned char reserver[2];//����������������������������������64�ֽ�

// 	unsigned char PSAMdealSerial[4];	//�ն˽������	PSAM������ B
// 	unsigned char rDeviceNo[4];			//�豸�� POS�ն˺�
// 	unsigned char rMobilCardType;		//�ƶ���������	0x01:��ֵ 0x02:�ѻ����� 0x12���� 0x13�˻� CN
// 	unsigned char rDealType;			//�������� 0x04 �ֻ�����
// 	unsigned char rResever0[2];		//
// 	unsigned char rAfterMoney[4];		//ԭ��
// 	unsigned char rDealMoney[3];		//���׽��
// 	unsigned char rDealTime[7];			//����ʱ��YYYY/MM/DD/HH/MM/SS
// 	unsigned char rResever1[2];			//
// 	unsigned char rDriverNo[4];			//˾����
// 	
// 	unsigned char rCardDealNumb[4];		//POS�ն˺� �ƶ�PSAM����
// 	unsigned char rLineNo[2];			//��·��
// 	unsigned char rMAC[10];				//���׿���	Ǯ������ N
// 	unsigned char rResever[14];			//����
// 	unsigned char rCrc[2];				//У����
}MobileDEALRECODE;

typedef struct
{
	unsigned char rDeviceNo[8];			//�豸��
	unsigned char posHardW_flag;		//���ػ�Ӳ����ʶ
	unsigned char rDealType;			//��������  50
	unsigned char busBLKVer[7];         //�������汾��(BCD��)
	unsigned char busProVer[7];			//����汾��(BCD��)
	unsigned char busLineVer[7];		//��·��Ϣ�汾��(BCD��)
	unsigned char CurRecordPointer[4];		//��ǰ��¼ָ��
	unsigned char CurPointer[4];			//��ʼ��ַ
	unsigned char BlackListNum[4];         //����������

	unsigned char rLineNo[2];			//��·��
	unsigned char rResever[19];			//����
}SD_DEALRECODE;//�ܶ��ļ�¼��ʽ_wxliu_V3.27_2010.12.15

typedef struct {
	unsigned char citycode[2];
	unsigned char hanye[2];
	unsigned char deviceNo[4];//�豸��
	unsigned char startDateTime[7];//����ʱ��
	unsigned char lineVer[2];//�汾
	unsigned char busPrice[4];//����Ʊ��
	unsigned char rate[32];//�ۿ�
	unsigned char voice[32];//��������
	unsigned char plusrate[32];//�Ƿ������۷�
	unsigned char cardControl[32];//������
	unsigned char plusPrice[4];//���ӷ�Ʊ��
	unsigned char plusMonthPrice[4];//���ӷ�Ʊ��
	//
	unsigned char ChangeType;	//���˱�ʶAA
	unsigned char ChangeRate;	//�����ۿ�
	unsigned char ChangeTime;	//����ʱ�� ��λ ����
	unsigned char Rever[96];//Ԥ��
}stPricetable;//�µ�Ʊ��

typedef struct {
	unsigned char flag[4];//��־�����ڵ���
	unsigned char index[3];// λ��
	unsigned char DorI;//���ӻ�ɾ����־
	unsigned char CardNo[5];//���5�ֽڳ���������
	unsigned char BCC;//У��
}stAddBLKTEMP;

typedef struct {
	unsigned char cardNo[4];//�˶β���Ա����
	unsigned int chargeA;//Ǯ�������ܽ��
	unsigned int RseverA;//���������ܽ��
	unsigned int SaleA;//ˢ���ܴ���
}stSatone;
typedef struct {
	unsigned char DataTime[7];
	stSatone infoBuf[8];
	unsigned char resever[8];
	unsigned int sCrc32;
}stStaeInfo;//ͳ�����ݼ�¼�ṹ 148

//extern void clr_wdt(void);

////////////////////////////////GPS//////////////////////////

typedef struct
{
	unsigned char rDeviceNO[4];			//0-3 �豸�ų������
	unsigned char rDeviceSl[4];			//4-7 ���к��ն˻����
	unsigned char rinRun;				//8 �Ƿ�Ӫ�˴���1B
	unsigned char rDealType;			//9 ��������  E0--E6

	unsigned char ZULine[4];			//10-13 ����·���
	unsigned char ZiLine[4];			//14-17 ����·���
	unsigned char ulCardNo[4];			//18-21 ˾������

	unsigned char stationNo;			//22 վ����
	unsigned char RunDirect;			//23 ����
	unsigned char LuduanNo[2];			//24-25 ·�α��2B

	unsigned char MTimesUP;				//26 �ϳ��˴�
	unsigned char MTimesDown;			//27 �³��˴�
	unsigned char LiCengALL[4];			//28-31 ���4B___32

	unsigned char DealTime[7];			//32-38 ʱ��(���ٽ���ʱ��)
	unsigned char STARTDealTime[7];		//39-45 ʱ��(���ٿ�ʼʱ��)

	unsigned char rCardDealNumb1[4];	//46-49 �豸������ˮ
	unsigned char RFU1[2];				//50-51 �����ֽ�__52

	unsigned char RFU[10];				//52-61 �����ֽ�
	unsigned char CRC16[2];				//62-63 ����__��64�ֽ�
}RECORD_GPS;

////////������GPS�ṹ����///////////////////////
////////����2.4G ������ʯ��ؽṹ//////////
#define SELPHONE_valid 0xA5
#define SELPHONE_SENDED 0xAA
typedef struct
{
	char Style;				//��ʶ��0xA5��Ч
	char SellPhoneNo[14];	//�ֻ���
	char SendMsg[70];		//Ҫ���Ͷ��ŵ����ݣ�20������UNICODE����
	unsigned char CrC[4];	//CrC32У��

	unsigned char len;		//�������ݵĳ���,����UNICODE���룬������str��غ���.
	unsigned char times;	//�Ѿ����ʹ������������3�β��ɹ�����ʱ��
	unsigned char TLL;		//��ʱ��������Ͷ��û�ɹ������ӳ�һ��ʱ��
	unsigned char index;	//�ڴ�������λ�ú�
}stSelMsg; //61�ֽ�

typedef struct
{
	char style;
	unsigned char datatime[7];
	char SellPhoneNo[14];	//�ֻ���
	unsigned char reseve[6];//����
	unsigned char CrC[4];	//CrC32У��
}stSaveMSG;	//32 ������Ҫ���͵���Ϣ���ֻ�������ʱ�� BIT_GPSDiaoDuCMD_INFO  1K �� 32��
//----------------------------------------------
//-------------�ֶ��շѽṹ����-----------------
typedef struct
{
	unsigned char getupFlag;	//0:�ϳ���־=AA
	unsigned char times;		//1:�ϳ�ˢ������ (HEX)
	unsigned char stationNo;	//2:�ϳ�ʱ��վ���� (HEX)
	unsigned char Allprice[2];	//3-4:��·ȫ��Ʊ�� ��HEX��
	unsigned char LineNo[3];	//5-7��·��� ����:5-6=��·���(BCD),7=���߱��(BCD)
	unsigned char getupTime[7];	//8-14�ϳ�ʱ�� (������ʱ����=YYYYMMDDHHMMSS)
	unsigned char Cxor;			//15��У��
}stFDCard1;
typedef struct
{
	unsigned char deviceNo[4];	//0-3:�ϳ�ʱ���豸��ţ�������ţ�(BCD)
	unsigned char DriverNo[3];	//4-6:�ϳ�ʱ��˾����� (BCD)
	unsigned char NstationNO;	//7:����վ���ţ�HEX��
	unsigned char BUSerial[4];	//8-11:�ϳ�ʱ���豸���к� (BCD)
	unsigned char getdownTimes;	//12:�³�ˢ������=00��HEX����
	unsigned char timeDly[2];	//13-14:�����ϳ������ʱ�䳤�� (HEX ��λ����)
	unsigned char Cxor;			//15��У��
}stFDCard2;
typedef struct {
	unsigned char LineNo[3];	//��·��ţ��͹��õ�һ��
	unsigned char CXulieNo;		//Ʊ�ۿ����
	unsigned char CPriceNo[2];	//Ʊ�۷����ĸ���
	unsigned char StationNum;	//��·��վ��
	unsigned char PlusPrice[2];	//���ӷ�
	unsigned char LianxuDly;	//����ˢ����ʱʱ��
	unsigned char NeedMaxTime[2];	//����·�����������ʱ��
	unsigned char TottalPrice[2];	//����·ȫ��Ʊ��
	unsigned char StartPrice;		//�𲽼ۣ�ͬվ�³��ļ۸�
	unsigned char Cxor;			//У��
}stFDLineHead;	//�ֶ���·ͷ�������ǵ�1����ÿ2�顣
//----------------------------------------------

#include "SlzrTypeDef.h"
#include "UtilityProc.h"

#endif
