
#ifndef _GSZCT_HEAD_FILE_
#define _GSZCT_HEAD_FILE_

#include "StructDef.h"

//�ڿ⺯�����ã����ܸ���
//#define M_PRO_VER "20090912160901"//20091011160901 �����еİ汾��
//------------GPRS----------------
#define CMD_QURE_INFO	0xa0
#define CMD_RE_CONNECT	0xa2
#define CMD_DATA_GPRS	0xa1
#define CMD_DATA_GPS	0xa3
#define CMD_SET_PARA	0xa4
#define CMD_QURE_OTHER	0xa5
#define CMD_GPRS_CMD	0xa6
#define CMD_GPRS_OFF	0xa7
#define CMD_GPRS_ON		0xa8
#define CMD_RICEV_DATA	0xa9
#define CMD_RESTARD		0xaa
#define CMD_SEND_URT1	0xab//�����ݷ��͵�URT1
#define CMD_GET_ACK		0xac
#define CMD_24G_PROFILE	0xad

//#define BLACKLEN     16000//2                       0000//
//#define BLK_SNO_LEN	4	//һ������������

#define LEN_TCPIP_PACKET	500

#define CMD_GPRS_DERIVENO	0x02
#define CMD_GPRS_SEND_DATA  0x01
#define CMD_GPRS_RECORD		0x7e

#define CMD_GPRS_DEL_BLACK	0x8b

#define CMD_GPRS_TIME_BTM	0x04

#define CMD_GPRS_PRICE_BTM	0x09
#define CMD_BTM_AUDIO		0x08
#define CMD_BTM_PRICE_CARD	0x09
#define CMD_BTM_DREIVE		0x03


#define CMD_GCS_PING		0x8b
#define CMD_GPRS_PING		0x08	
#define CMD_SETUP_AUDIO		0x89
#define CMD_UPATE_DERIVE_NO	0x83
#define CMD_GPRS_TIME_GCS	0x84
#define CMD_GPRS_PRICE_GCS	0x8a

#define CMD_CHECK_BLACK_VER	0x85
#define CMD_UPDATE_BLACK_VER 0x86
#define CMD_GPRS_ADD_BLACK	0x87
#define CMD_DELE_BLACK		0x88
#define CMD_BLACK_ALL		0x8d	
#define CMD_BLACK_DEL_ALL	0x8c

#define CMD_DEAL_MODE		0x90
#define CMD_YCT_PRICETABLE	0x91
#define CMD_WRITE_CARD		0x92
#define CMD_SEND_WRITE		0x93
#define CMD_CHECK_MESSAGE	0x94

#define CMD_REACORD_ECHO	0x00
#define CMD_DERIVE_ECHO		0x1f
#define CMD_SETUP_TIME		0x23
#define CMD_AUDIO_REQUES	0x2a
#define CMD_PING_ECHO		0x2c		

#define MASK_RCV_BUF_INIT					0x00				//����֡��ʼ��״̬��
#define MASK_RCV_BUF_USED					0x80				//�ý���֡���岻�� ��������
#define MASK_RCV_BUF_PEEKED					0x40				//���֡�Ѿ�����·�㴦�� �Ž���Ӧ��ջ

#define TCP_ID_VER 0x01		//TCP���汾
#define DVICE_STYLE 0x01	//�豸����

#define GPRS_SOCKET_CHECK_CMD  0x3A

#define GPRS_LINK_CMD	0xF0//ģ��ر���
#define GPRS_TCPCLOSE_CMD 0xF1//TCPIP���ӹر���

//����MC55i,��Ҫ������ʹ��ƽΪ��
#define GPRS_LINK_UP 0xF3
//GPRS���ڵ�ģ��ע��
#define GPRS_AT_WAIT 0xF4
//����ر�ģ�飬AT^SMSO
#define GPRS_MC55_CLOSE 0xF5
//ģ����Ҫ����
#define GPRS_MODE_NEED_CLOSE 0xF6
//ģ���µ�
#define GPRS_MODE_POWER_DOWN 0xF7
//ģ���ϵ�
#define GPRS_MODE_POWER_ON 0xF8
//AT�����λָ��
#define GPRS_AT_RESET 0xFA
//ģ���Դ�ر�Ȼ��򿪣����¿�ʼGPRS����
#define GPRS_HWPOWER_RESET 0xFE


//ģ����Ҫ�ر�TCP/IP
#define GPRS_NEED_CLOSEIP 0x2F

#define GPRS_Out_CMD 0xF2//GPRS����
#define GPRS_SENDING_CMD 0xA0//���ڷ������ݣ�����Ӧ����
#define GPRS_RING_CMD 0xA1//�е绰����
#define GPRS_CStatues 0xA2	//�ڹ����в�ѯWIFI�����Ƿ����������ڼ�������Ƿ��뿪(��û��ʹ��)

#define TCPSTARTSTAT	22	//7TCP/IP��ʼֵ��֮ǰ��ֵ��ģ���ʼ������


#define GPSDATA_LEN	34					//��¼����
#define GPRS_AT	0
#define GPRS_CLOSE_ATO	1
#define GPRS_SIM_STATE	2
#define GPRS_CIMI_STATE	3
#define GPRS_CREG_STATE	4
#define GPRS_CREG_C		5
#define GPRS_CGREG_STATE 6
#define GPRS_CGREG_C	7

#define GPRS_PDP_STATE	8
#define GPRS_QIACT		9
#define GPRS_QIACT_C	10
#define GPRS_QIDE		11 //B
#define GPRS_ICCIDC		12 //c
#define GPRS_QIOPEN		13 //D
#define GPRS_CLOSEIP	17
#define GPRS_FREE		22
#define GPRS_SEND_C		16
//#define LEN_BLACK_ADD 8
//#define LEN_BLACK_ADD 8
typedef struct 
{
	unsigned int ZLVer;//�����汾��
	unsigned int KLVer;//��汾��
}BLACK_ADD2;

typedef struct 
{
	unsigned char flag;
	unsigned short serial;
	unsigned short len;
	unsigned char cmd;
	unsigned char buffer[128];
}ECHO_1;

//ͨ��֡�ṹ����
#define FRAME_SIZE							1500				//һ֡��������ֽڳ���
typedef struct tag_FRAME_BUF {
	unsigned char ucFrameType;									//֡�����ֽ�
	unsigned char uiFrameLen[2];									//֡����
	unsigned char FrameBuf[FRAME_SIZE];							//֡����
} FRAME_BUF;

#define MAX_RCV_PACKET_NUM					5					//���հ������С
//Ӧ�ð��������,���ڴ��������·����յ�������֡,����Ӧ�ò����ݺͿ���֡����
typedef struct tag_PACKET_BUF {
	FRAME_BUF LinkFrame;										//֡����
	unsigned char ucItemStatus;									//֡״̬��ʾ 
} PACKET_RCV;

//--------------------------------
//00 00 00 04 00 12 34 56 78 1A 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 

typedef struct
{
	unsigned char firstFlag[4];//��һ�����б�־
	unsigned char deviceNo[9];//�豸��
	unsigned char lineNo[3];//��·��
	unsigned char severIP[4];//������IP��ַ��û�п�����ֵ��
	unsigned char severPort[2];//�������˿ںţ�û�п�����ֵ��
	unsigned char priceTab[512];//Ʊ�ۼ�����
}stSYSBUPARA;

#define RS_IN_SOUND 0x01	//��վ��վ
#define RS_OUT_SOUND 0x02	//��վ��վ
#define RS_IN_MAIN 0x04		//������վλ��
#define RS_IN_SEND 0x10		//��վ�����ϴ�
#define RS_OUT_SEND 0x20	//��վ�����ϴ�
typedef struct  {
	unsigned char ucStation;			//��ǰվ̨��
	unsigned char ucPeStation;			//ǰһվ̨��
	unsigned char inOrOut;				//��վ��վ��־(1��2��)
	unsigned char ucOverSpeed;			//�Ƿ��� ����00����0C
	unsigned char ucDirect;				//���� ��0 ��1
	unsigned char ucFlag;				//��־���Ƿ�վ��4�ֽڣ�������4�ֽ�
}stRunStation;
extern stRunStation RunStation;				//�����е�վ����Ϣ
typedef struct  {
	unsigned char ucStation[2];
	unsigned int ulDisxx[2];
	unsigned int ulDisnn[2];
	unsigned char ucFlag[3];
}stFindStation;
extern stFindStation FindS;					//��վ�����

typedef struct {
	unsigned char flag;					//��־
	unsigned char direct;				//����
	unsigned char buff[7];				//����ʱ��
}stGpsFED;
extern stGpsFED gGpsFeD;						//GPS�������������

#define ADDR_FLASH_LINE1 FLASH_GPS_LINE_START			//��һ����·��Ϣ����ĵ�ַ ʹ��ǰ32K�ռ�
#define ADDR_FLASH_LINE2 FLASH_GPS_LINE_START+0x8000	//�ڶ�����·��Ϣ����ĵ�ַ ʹ�ú�32K�ռ�

#define GPS_SEND_DLY 5				//GPS��վ���ϱ�ʱ��
#define GPS_RE_SEND_DLY	65000			//�ط�GPS��Ϣ
#define GPSVALID				0x41	//��Ч
#define GPSINVALID				0x56	//��Ч
#define GPSUES					0xaa	//ʹ����

extern stGpsPar gGPsPar;				//������Ϣ
extern GPS_INFO gGpsInfo;		//GPS��Ϣ,Ӧ�ó����жϵĻ���
//--------GPS------------------

#ifdef _Gps_demo_auto_
extern unsigned int autodly;
extern unsigned char autoEnable;
#endif

extern void SoundFreeManage(char *sstr);

#define IDSoundPowerOn 0xa1
#define IDSoundStartST 0xa2
#define IDSoundEndST 0xa3
#define IDSoundOUT 0xa4
#define IDSoundIN1 0xa5
#define IDSoundComm 0xa6
#define IDSoundDefin 0xa7
#define IDSoundRound 0xa8
#define IDSoundFree 0xa9
#define IDSoundOverSpeed 0xB1 //����
#define IDSoundOverStation 0xB2//Խվ

#define IDSoundOverSpeedAT 0xB3 //����
#define IDSoundOverStationAT 0xB4 //����

#define XFStartCompose	0x01		//start Compose sound
#define XFStopCompose	0x02		//stop compose sound
#define XFPauseCompose	0x03		//Pause compose soud
#define XFResumeCompose	0x04		//
#define XFQueryStatues	0x21		//Query XF-s4240 statues
#define XFPowerDown		0x88		//
////////////////////////////////////////////
#define GCMD_HEAD 0
#define GCMD_Address 3
#define GCMD_CmdCode 7
#define GCMD_GnCode 9
#define GCMD_CmdPara 10
#define GCMD_Ln 14
#define GCMD_Data 16
/////////�ƶ����ݽṹ����--------------------------
#define MobileAID "\xD1\x56\x00\x01\x01\x80\x03\x80\x00\x00\x00\x01\x00\x00\x00\x00"

typedef struct{
	unsigned char MSG60_LLL[2];//BCD BM#60�Զ������ݳ��� = 0072
	unsigned char MSG60_1[16];//CN Ӧ�ñ�ʶAID �䳤��5��16���ֽڣ��󲹿ո�0X20��
	unsigned char MSG60_2[14];//CN Ӧ�ð汾�� ��ʽ��YYYYMMDDHHMMSS
	unsigned char MSG60_3[2];//N KEK��Կ������ 00~02
	unsigned char MSG60_4[14];//CN �����汾�� ��ʽ��YYYYMMDDHHMMSS
	unsigned char MSG60_5[2];//CN POS��Ϣ������ F0��FF(F0)
	unsigned char MSG60_6[2];//CN POS������Ϣ������ 00��FF 01:ǩ��	
	unsigned char MSG60_7[6];//CN ���κ� 
	unsigned char MSG60_8[8];//CN ����Ա�� �䳤��2��8���ֽڣ��󲹿ո�0X20��
	unsigned char MSG60_9[8];//B ���� 
}stP8583_60;//
typedef struct{
	unsigned char LLL[2];//BCD BM#60�Զ������ݳ��� = 0072
	unsigned char MSG1[3];//����ܱ��� CN
	unsigned char MSG2[12];//����ܽ�� CN
	unsigned char MSG3[3];//�����ܱ��� CN
	unsigned char MSG4[12];//�����ܽ�� CN
	unsigned char MSG5[3];//ת���ܱ��� CN
	unsigned char MSG6[12];//ת���ܽ�� CN
}stP8583_63;
typedef struct{
	unsigned char LLL[2];//BCD BM#60�Զ������ݳ��� = 0072
	unsigned char MSG1;//���ͺ�����־ 0x01����  0x00�����һ���� �޺����� CN
	unsigned char MSG2;//��ϸ���� 0x01--0x05 CN
	unsigned char MSG3[415];//��ϸ1-5 B
}stP8583_61;
#ifdef hb_c
typedef struct{//ǩ��״̬ ���浽�����У��������������
	unsigned char SingInDate[7];//ǩ��ʱ��
	unsigned char LoadStyle;//״̬��0λ��KEK�����أ�1λ�����������أ�2λ������������
	unsigned char ISOK;//�����Ƿ���Ч  �˲�����Ҫ����ʱ�жϸ�ֵ
	//------������Կ WK����------------------
	unsigned char PIN_KEY[16];//
	unsigned char PIN_KEY_CHECKVALUE[4];//
	unsigned char MAC_KEY[16];//
	unsigned char MAC_KEY_CHECKVALUE[4];
	//---------------------------------------
	unsigned char resum[11];//����
}stMobilStyle;//ǩ��״̬����������������Ϣ��KEK ���� ��������״̬����������

typedef struct{
	unsigned char posSerilNo[16];//POS�ն����к�
	unsigned char posNo[8];//pos�ն˺�
	unsigned char shopNo[15];//�̻����
	unsigned char shopName[20];//�̻�����
	unsigned char posPiCiNo[6];//POS���κ�
	unsigned char puchaceTimeOut;//pos���׳�ʱʱ�� ��λ��
	unsigned char tikectNumber;//��ӡƱ�ݵ���
	unsigned char prchaceType[8];//����������,8�ֽ�λͼ
	unsigned char posAcktime[8];//pos��������,��λ�루ǰ��0��BCD���ʾ��
	unsigned char oneMaxMoney[12];//�ն˵��ʽ������� ��λ�֣�ǰ��0��BCD���ʾ��
	unsigned char posMaxmoney[12];//�ն��ۼƽ��׽������ ��λ�֣�ǰ��0��BCD���ʾ��
	unsigned char congZenMactimes;//�������������
	unsigned char posMaxPurTimes[3];//�ն�ÿ������ױ��� 000-999��ǰ��0��BCD���ʾ��
	unsigned char posRunStyle;//�ն�����״̬ 
	unsigned char posOKGoTimes[8];//ƽ���޹�������ʱ�䵥λ����(ǰ��0��BCD���ʾ)
	unsigned char posOnlineTimes[8];//POS����ʱ�� ��λ����(ǰ��0��BCD���ʾ)
	unsigned char posProNewVer[14];//�ն�Ӧ�ó���汾��--���ĸ��İ汾��
	unsigned char Ver[14];//�ն˲����汾�� YYYYMMDDHHMMSS
	unsigned char purGate[15];//�������ص�ַ
	unsigned char pruAPN[20];//����APN
	unsigned char purIP[15];//����IP��ַ IP��ַ������
	unsigned char purPort[6];//���׶˿ں� 000000��999999��ǰ��0��BCD���ʾ��
	unsigned char manageGate[15];//�������ص�ַ
	unsigned char manageAPN[20];//����APN
	unsigned char manageIP[15];//����IP��ַ
	unsigned char managePort[6];//����˿ں�
	unsigned char organNo[10];//������� POS���������ı��
	unsigned char organName[20];//�������Ƽ�д POS�����������Ƽ�д

	unsigned char KEK_SN;//��Կ����
	unsigned char KEK[48];//KEK
	unsigned char addPICINO_limit;//���κżӴ���,�����������5��,�Ͳ��ټ�.
	unsigned char gMobil_Settlement_error;//ͬһ��������������
	unsigned char NUllbuf[6];//
}stMobileParameter;//�����·��Ĳ�������ṹ
#endif
typedef struct{
	unsigned char MSGtypeID[2];//BCD MSG-TYPE-ID 0800
	unsigned char BIT_MAP[8];//B BIT MAP
	unsigned char MSG11[6];//BCD �յ���ϵͳ���ٺ�
	unsigned char MSG12[6];//BCD �յ������ڵ�ʱ��
	unsigned char MSG13[8];//BCD �յ������ڵ�����
	unsigned char MSG40[16];//ASCII �ն����к�
	unsigned char MSG41[8];//ASCII �յ����ն˱�ʶ��=POS�ն˺�
	unsigned char MSG42[15];//ASCII �ܿ�����ʶ��=�̻���
	unsigned char MSG60_LLL[2];//BCD BM#60�Զ������ݳ��� = 0072
	unsigned char MSG60_1[16];//CN Ӧ�ñ�ʶAID �䳤��5��16���ֽڣ��󲹿ո�0X20��
	unsigned char MSG60_2[14];//CN Ӧ�ð汾�� ��ʽ��YYYYMMDDHHMMSS
	unsigned char MSG60_3[2];//N KEK��Կ������ 00~02
	unsigned char MSG60_4[14];//CN �����汾�� ��ʽ��YYYYMMDDHHMMSS
	unsigned char MSG60_5[2];//CN POS��Ϣ������ F0��FF(F0)
	unsigned char MSG60_6[2];//CN POS������Ϣ������ 00��FF 01:ǩ��	
	unsigned char MSG60_7[6];//CN ���κ� 
	unsigned char MSG60_8[8];//CN ����Ա�� �䳤��2��8���ֽڣ��󲹿ո�0X20��
	unsigned char MSG60_9[8];//B ���� 
//	unsigned char MSG64[8];//MAC 
}stSingIn;
typedef struct{
	unsigned char MSGtypeID[2];//BCD MSG-TYPE-ID 0800
	unsigned char BIT_MAP[8];//B BIT MAP
	unsigned char MSG11[6];//BCD �յ���ϵͳ���ٺ�
	unsigned char MSG12[6];//BCD �յ������ڵ�ʱ��
	unsigned char MSG13[8];//BCD �յ������ڵ�����
	unsigned char MSG15[4];//BCD �������� MMDD
	unsigned char MSG60_LLL[2];//BCD BM#60�Զ������ݳ��� = 0072
	unsigned char MSG60_1[16];//CN Ӧ�ñ�ʶAID �䳤��5��16���ֽڣ��󲹿ո�0X20��
	unsigned char MSG60_2[14];//CN Ӧ�ð汾�� ��ʽ��YYYYMMDDHHMMSS
	unsigned char MSG60_3[2];//N KEK��Կ������ 00~02
	unsigned char MSG60_4[14];//CN �����汾�� ��ʽ��YYYYMMDDHHMMSS
	unsigned char MSG60_5[2];//CN POS��Ϣ������ F0��FF(F0)
	unsigned char MSG60_6[2];//CN POS������Ϣ������ 00��FF 01:ǩ��	
	unsigned char MSG60_7[6];//CN ���κ� 
	unsigned char MSG60_8[8];//CN ����Ա�� �䳤��2��8���ֽڣ��󲹿ո�0X20��
	unsigned char MSG60_9[8];//B ���� 
	unsigned char MSG63_LLL[2];//BCD BM#63�Զ��峤�� 
	unsigned char MSG63_1[3];//BCD ����ܱ��� 
	unsigned char MSG63_2[12];//BCD ����ܽ��
	unsigned char MSG63_3[3];//BCD �����ܱ���
	unsigned char MSG63_4[12];//BCD �����ܽ��
	unsigned char MSG63_5[3];//BCD ת���ܱ���
	unsigned char MSG63_6[12];//BCD ת���ܽ��
	unsigned char MSG64[8];//MAC 
}stSendSettlement;//����
typedef struct{
	unsigned char MSGtypeID[2];//BCD MSG-TYPE-ID 0800
	unsigned char BIT_MAP[8];//B BIT MAP
	unsigned char MSG11[6];//BCD �յ���ϵͳ���ٺ�
	unsigned char MSG12[6];//BCD �յ������ڵ�ʱ��
	unsigned char MSG13[8];//BCD �յ������ڵ�����
	unsigned char MSG15[4];//BCD �������� MMDD
	unsigned char MSG60_LLL[2];//BCD BM#60�Զ������ݳ��� = 0072
	unsigned char MSG60_1[16];//CN Ӧ�ñ�ʶAID �䳤��5��16���ֽڣ��󲹿ո�0X20��
	unsigned char MSG60_2[14];//CN Ӧ�ð汾�� ��ʽ��YYYYMMDDHHMMSS
	unsigned char MSG60_3[2];//N KEK��Կ������ 00~02
	unsigned char MSG60_4[14];//CN �����汾�� ��ʽ��YYYYMMDDHHMMSS
	unsigned char MSG60_5[2];//CN POS��Ϣ������ F0��FF(F0)
	unsigned char MSG60_6[2];//CN POS������Ϣ������ 00��FF 01:ǩ��	
	unsigned char MSG60_7[6];//CN ���κ� 
	unsigned char MSG60_8[8];//CN ����Ա�� �䳤��2��8���ֽڣ��󲹿ո�0X20��
	unsigned char MSG60_9[8];//B ���� 
	unsigned char MSG64[8];//MAC 
}stSendTradeBatch;//����������
//--TLV
typedef struct  
{
	unsigned char DAP_FF[3];//0xff 0x01 + �������� 0x02: ��������
	unsigned char DAP_28[18];//0x28 16+POS�ն����к�(16)
	unsigned char DAP_29[10];//0x29 8+POS�ն˺�(8)
	unsigned char DAP_2A[17];//0x2a 15+�̻����(15)
	unsigned char DAP_90[18];//90 16+Ӧ�ñ�ʶ��AID(16)
	unsigned char DAP_aa[16];//0xaa 14+�ն˲����汾�� YYYYMMDDhhssmm
}stDownLoadAppParameter1;//������������ POS�ն��ڲ�����ʼ���������в���POS�ն˺ţ����ڲ�������ʱҪ��POS�ն˺ţ�
						//POS�ն��ڲ�����ʼ��ʱ���ն˲����汾��Ϊȫ0���ڲ�������ʱ��Ӧ���ϵ�ǰPOS�ն˵Ĳ����汾�š�
typedef struct  
{
	unsigned char DAP_FF[3];//0xff 0x01 + �������� 0x02: ��������
	unsigned char DAP_28[18];//0x28 16+POS�ն����к�(16)
	unsigned char DAP_90[18];//90 16+Ӧ�ñ�ʶ��AID(16)
	unsigned char DAP_aa[16];//0xaa 14+�ն˲����汾�� YYYYMMDDhhssmm
}stDownLoadAppParameter0;//��ʼ������,����POS�ն˺�

typedef struct{
	unsigned char DAP_FF[3];//0xff 0x01 + ���� 0x03:��������
	unsigned char DAP_FE[3];//0xfe 1 + ���� 0x01:��������
	unsigned char DAP_28[18];//0x28 16+POS�ն����к�(16)
	unsigned char DAP_90[18];//90 16+Ӧ�ñ�ʶ��AID(16)
	unsigned char DAP_FC_TAG;//0xfc ����� FC + 27 + ����
	unsigned char DAP_FC_LEN;//27
	unsigned char DAP_FC_1[2];//������ 0x0001-0xffff
	unsigned char DAP_FC_2;//�������� 0x00: Ӧ�ó���
	unsigned char DAP_FC_3[14];//�汾�� Ӧ�ó���汾��,��ʼ��ʱ��Ϊ0
	unsigned char DAP_FC_4;//���� 0x00:���ǣ�0x01�����ӣ�0x02��ɾ��
	unsigned char DAP_FC_5[4];//�����С Ӧ�ó����ļ���С��HEX��ʾ��
	unsigned char DAP_FC_6[5];//����������� 1B ��ɰٷֱȣ�0x00-0x64��+ 4B �������ļ���ƫ��
}stDownloadApplication;//Ӧ�ó����������ؽṹ
typedef struct{
	unsigned char DAP_FF[3];//0xff 0x01 + ���� 0x03:��������
	unsigned char DAP_FE[3];//0xfe 1 + ���� 0x02:��������
	unsigned char DAP_28[18];//0x28 16+POS�ն����к�(16)
	unsigned char DAP_90[18];//90 16+Ӧ�ñ�ʶ��AID(16)
	unsigned char DAP_FC_TAG;//0xfc ����� FC + 27 + ����
	unsigned char DAP_FC_LEN;//17
	unsigned char DAP_FC_1[2];//������ 0x0001-0xffff
	unsigned char DAP_FC_2;//�������� 0x00: Ӧ�ó���
	unsigned char DAP_FC_3[14];//�汾�� Ӧ�ó���汾��,��ʼ��ʱ��Ϊ0
}stDownloadAppResultNotification;//Ӧ�ó������ؽ��֪ͨ������
typedef struct{
	unsigned char DAP_FF[3];//0xff 0x01 + �������� 0x03: KEK����
	unsigned char DAP_28[18];//0x28 16+POS�ն����к�(16)
	unsigned char DAP_29[10];//0x29 8+POS�ն˺�(8)
	unsigned char DAP_2A[17];//0x2a 15+�̻����(15)
	unsigned char DAP_90[18];//90 16+Ӧ�ñ�ʶ��AID(16)
}stInitializeForKEK;//KEK����

typedef struct{
	unsigned char AID[16];//Ӧ�ñ�ʶAID CN
	unsigned char dealType;//��������	0x01:��ֵ 0x02:�ѻ����� 0x12���� 0x13�˻� CN
	unsigned char dealCardNo[10];//���׿���	Ǯ������ N
	unsigned char dealMoney[6];//���׽�� N
	unsigned char dealDate[4];//��������yyyymmdd N
	unsigned char dealtime[3];//�ն˽���ʱ�� hhmmss N
	unsigned char posSerial[3];//POS�ն˽�����ˮ�� N

	unsigned char Cardtag[8];//����Ψһ��ʶ��������һ������ CN
	unsigned char KEYflagNO;//��Կ������ CN
	unsigned char KEYVer;//��Կ�汾�� N
	unsigned char TAC[4];//���ѳ�ֵTAC B
	unsigned char purseMoney[4];//���ѽ��׽�� B
	unsigned char pursestyle;//�������ͱ�ʶ B
	unsigned char deviceNo[6];//POS�ն˺� CN
	unsigned char PSAMdealSerial[4];//�ն˽������	PSAM������ B
	unsigned char date[4];//���ѽ������ڣ��նˣ�CN
	unsigned char time[3];//���ѽ���ʱ�䣨�նˣ�CN
	unsigned char afterMoney[4];//Ǯ�������׺���B
}stMobileRecord;
#define MOBILE_REC_LEN 128//ÿ���ƶ����Ѽ�¼����

typedef struct{
	unsigned char MISS_Sreil[2];//������
	unsigned char Miss_style;//��������
	unsigned char Miss_ver[14];//�汾��
	unsigned char Miss_dongzuo;//���� 0x00:���ǣ�0x01�����ӣ�0x02��ɾ��
	unsigned char Miss_len[4];//�����С
	unsigned char Miss_pesent[5];//����������� 1B ��ɰٷֱȣ�0x00-0x64��+ 4B �������ļ���ƫ��
}stdownProgPara;//һ�س������

typedef struct{
	unsigned char STX;
	unsigned char alen[2];//��λ��ǰ,��λ�ں�
	unsigned char Data[512];//����data+LRC+EXT(0x03)
}stG24ReadP;

typedef struct{
	unsigned short T;
	unsigned short L;
	unsigned char link;
	unsigned char V[1500];
}stTcpIpSp;//MG323�������ݻ��档���������ȴ������������Ͷ���Ҫ���η��͡���Ӧ����¼��

#define TcpIPFree 0xA5
#define TcpIPBusy 0x55

//���ػ������־
#define MISS_IF1 0x10//��Ҫ��½��IF1
#define MISS_M_SINGIN 0x11		//0x01 ��½�ƶ�,ǩ��.
#define MISS_M_Settlement 0x12	//0x02 ������
#define MISS_M_TradeBatch 0x13	//0x03 �����ͽ���
#define MISS_IF2 0x20//��Ҫ��½��IF2
#define MISS_M_DPARAM 0x21		//0x01 ���ز���
#define MISS_M_DPGRAM 0x22		//0x02 ������������
#define MISS_M_DPGRAMOK 0x23	//0x03 �������ؽ��֪ͨ
#define MISS_M_KEK 0x24			//0x04 KEK����
#define MISS_M_ActiveTest 0x25	//0x05 ����

#define MISS_GJ 0x40	//��Ҫ��½������
#define MISS_G_LOGINGJ 0x40		//0xF0 ��½������̨
#define MISS_G_DBLKD 0x41		//0xF1 ���غ����� ��  ��������������һ���������Ҫ����
#define MISS_G_UREC 0x42		//2 �ϴ�����
#define MISS_G_FILES 0x43		//3 �����ļ� ����,���е��ļ����ض�ʹ�ô�����
#define MISS_G_DLine	0x44		//3 �����ļ� ��·����
#define MISS_G_DSound	0x45		//3 �����ļ� ��������
#define MISS_G_DPROOK	0x46	//���س�����
#define MISS_G_DBLKI 0x47		//0xF1 ���غ����� ��
#define MISS_G_DLineOK	0x48	//���س�����
#define MISS_G_DSoundOK	0x49	//���س�����
#define MISS_G_HART 0x4A		//������
#define MISS_G_GPS 0x4B			//GPS massage
#define MISS_G_TOO 0x4C			//�ն˻���ָ֪�� -2002: �ն˻����������ն˻�������Ϣ�����ջ���
#define MISS_G_ALAM 0x4D		//���ͱ�����GPS��Ϣ
#define MISS_G_PRICE 0x4E		//����Ʊ����·��Ϣ
#define MISS_G_FREE 0x4F		//���ӵ����������ǿ�����

#define MISS_G_TREC 0x80		//ʵʱ����Ķ�ά������

#define MISS_G_MSG1 0x60		//��Ҫ���Ͷ���
#define MISS_G_MSG2 0x61		//��Ҫ��������

#define MISS_G_TMS	0x90

#define MISS_HTTP 0x80			//80���ϵ�������HTTP���ص�
#define MISS_HTTP_BLK 0x81		//ͨ��LINUXģ�����غ������ļ� 
#define MISS_HTTP_EC20 0x82		//ͨ��LINUXģ������LInuxģ����� 
#define MISS_HTTP_PRO 0x83		//ͨ��LINUXģ�����س��ػ����� 

#define MISS_G_SINGOUT 0x70		//�ն�ǩ��

#define MISS_COM_PARA 0x80		//�������س�����
//ǩ��״̬��0λ��KEK�����أ�1λ�����������أ�2λ������������
#define LoadStyle_KEK (0x01<<0)
#define LoadStyle_para (0x01<<1)
#define LoadStyle_program (0x01<<2)
//--------------------------------------------------------------------------
extern stSoundInfo gSoundInfo;					//����
extern void GPRS_INIT(unsigned int band);
extern void GPS_PAR_CLR(void);
extern void gps_par_init(void);
extern void main_GpsRs(void);
extern void showGps(unsigned char mode);
extern void gprs_par_init(void);
//Rstr:���ص��ַ�
extern unsigned char GetInSound(char *Rstr);
extern void SetSevice(void);
extern void ShowGprsStatus(void);
//��GPRS�����ݣ�buf��Ҫ���͵����ݣ�len���������ݵĳ���
extern void GPRSSASK(unsigned char *dat, unsigned char cmd, unsigned int len);
extern void closeGPS(void);
#ifdef _GPS_DIAODU_
extern void Save_DIAODUCMD_info(void);
extern void Load_DIAODUCMD_info(void);
extern void save_DD_YinYun_info(void);
#endif
//extern void gps_config_hw(void);
extern unsigned char gpsTimeOK;

//extern volatile unsigned char gllstyle;//GLL����������������ζ�û�У�����Ϊ��G595������������Ϣ.

extern Parameter2 POINT_MESSAGE;
//extern stLcdCmd gLcdCmd;
extern fFlagVary_1 F_FLAG_VARY;

#define GPS_RCV_BUFFER_SIZE 100

extern volatile unsigned char gucGPSindex;
extern unsigned char gps_RcvBuf[GPS_RCV_BUFFER_SIZE+1];	//GPS��Ϣ���ջ���
#endif
