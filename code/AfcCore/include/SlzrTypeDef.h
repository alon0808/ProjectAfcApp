#ifndef SLZR_TYPE_DEF_H_
#define SLZR_TYPE_DEF_H_

//modify by lks

#define _debug_

#ifdef _debug_
#define MSG_LOG(...) printf(__VA_ARGS__)
#define BCD_LOG(b, l, m) debugdata(b, l, m)
#else
#define MSG_LOG(...)
#define BCD_LOG(...)
#endif



//��¼�ռ�2M
#define RECORD_SPACE_LENGH 0x200000


typedef signed char    SLZR_S8;
typedef unsigned char  SLZR_U8;
typedef signed short   SLZR_S16;
typedef unsigned short SLZR_U16;
typedef signed int     SLZR_S32;
typedef unsigned int   SLZR_U32;
typedef char           SLZR_CHAR;
typedef char*          SLZR_STRING;
typedef void           SLZR_VOID;
typedef void*          SLZR_LPVOID;
typedef unsigned long long int SLZR_U64;
typedef signed long long int   SLZR_S64;

typedef enum
{
	SLZR_FALSE = 0,
	SLZR_TRUE = 1
}SLZR_BOOL;

typedef struct slzr_buf_s
{
	SLZR_LPVOID pBuf;
	SLZR_U32 u32Size;

}slzr_buf_t;

typedef SLZR_S32 SLZR_CHN;




typedef struct {
	unsigned char year_h;
	unsigned char year_l;
	unsigned char month;
	unsigned char day;
	unsigned char hours;
	unsigned char minutes;
	unsigned char seconds;
}stcomtime;


#define SAM1 1
#define SAM2 2
#define SAM3 3
#define SAM4 4

#define PSAM_GJ SAM1
#define PSAM_YD SAM2

#define PSAM_TRUE  1
#define PSAM_FALSE 0

typedef struct {
	unsigned char SLot;			//�˿����ڵĿ�����
	unsigned char Flag;			//��ʶPSAM״̬���Ƿ�������0������������������
	unsigned char CardNO[6];	//PSAM����
	unsigned char PSAMLiuShou[4];//PSAM���ص���ˮ��
}stPsamInfo;




#define WHITELEN		 1000
#define WHITE_SNO_LEN		4 
#define FLASH_WHITE_END	  10000

// #define BLACKLEN     16000//20000//
// #define BLK_SNO_LEN	10	//һ������������
// #define FLASH_BLK_END 500000

#define BLK_SNO_LEN	10	//һ������������


#define shift_blk 10
#define NUM_blk  10      //�������ļ�ÿ����ռ���ֽ���

typedef int boolean;

#ifndef NULL
#define NULL (void *)0
#endif // !NULL

#ifndef TRUE
#define TRUE 1
#endif // !TRUE

#ifndef FALSE
#define FALSE 0
#endif // !FALSE

#define ST_OK 0
#define ST_ERROR 1
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


#define RET_MONEY_SN_EROR 20	//�ÿ���Ǯ��������ʽȫ��



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


#if 0
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
#define BIT_GPSDownLine_INFO	BIT_BLACK_2BLOK+32
//4 pboc ��¼��ˮ��
#define BIT_PBOC_NS	BIT_GPSDownLine_INFO+32
//160  ��ʷָ�����ڣ�ÿ�����һ����ʷָ���ͳ������ ����ʱ��7+[����+��ֵ�ۼƽ��4+...]+CRC32(4)   8�ſ�
#define BIS_PERDat_Info (BIT_PBOC_NS+4)
//4 ˾��Ա�����
#define BIT_STUFFER_NO (BIS_PERDat_Info+160)
//20 ��ʷ��¼�ϴ���ַ.����GPRS������ʷ��¼ָ��.(�׵�ַ4+����4+����ʱ��7+����1+CRC324)=20�ֽ�,�׵�ַ����Ե�ַ,Ҫ����FLASH2_START1
#define BIT_HISREC_SND (BIT_STUFFER_NO+4) //2536
//8 �ۼ�ˢ������ ����4+У��4
#define BIT_SWAPCARD_TIMES BIT_HISREC_SND+20
//8 ���õ�ˢ������ ����4+У��4
#define BIT_SWTIMES_MAX BIT_SWAPCARD_TIMES+8
//28 ��ͳ�ƣ�����ǰһ���º͵�ǰ�µ�ˢ��ͳ�� 2 * (����2+ˢ���ܴ�4+ˢ���ܽ��4+CRC32У��4) = 28�ֽ�
#define BIT_MOTH_TOJI_2 BIT_SWTIMES_MAX+8
//8 ���ػ����ñ�ʶ���ں�̨ǩ�����ؼ��������и��� Ŀǰֻ���������Ƿ����õġ�"��"+"0x00"+��ʶ1+CRC32У��4 = 8�ֽ�
#define BIT_Center_Ctrl_BU_Data (BIT_MOTH_TOJI_2+28)
//4 ��λ,���ű��
#define BIT_UNIT BIT_Center_Ctrl_BU_Data+8		
// 136�Ҽ�¼�洢������128�ֽڣ�����Ч��4 + 128 data+4CRC = 136
#define BIT_UNKNOW_RECORD	BIT_UNIT + 4

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
#endif
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
//�û��� 
#define CARD_NORMAL			0				//��ͨA��
#define CARD_STUDENT		1				//ѧ��A��(�Ž� ���������)

#define CARD_OLDMAN			2				//���꿨
#define CARD_STUFF			3				//Ա����
#define CARD_COUPON			4				//������ ������Ʊ��(����)��
#define CARD_RELATION_BUS	5				//������ ������Ʊ��(����)��
#define	CARD_HANDICAPPED	6				//�м��˿� (�Ž��ͱ���) (���� ��ͨB��)
#define CARD_NORMAL_B		7				// ���� ����ר�߿�

#define CARD_CAP_BUS		8				//[���� ��ͨ�ֻ�����

#define CARD_JILIAN_BUS		9				// (���� ���п�)
#define CARD_YOUFU_BUS		10				//������ �Ÿ�����
#define CARD_17RD_BUS		11				//(�Ž� 17·ר�ÿ�)
#define CARD_JTB			12				//����/��ͨ���Ŀ���
#define CARD_MOTH_CI		13				//�����ο�  ��������ˢ
#define CARD_LIANGTONG_STUF 14              //�ֻ�Ա����


#define CARD_QRC_LTY 15              //��̩Դ��ά��


#define  CARD_FENDUAN_Line	33		//�ֶ���·Ʊ�ۿ�





typedef enum
{
	CARDSTYLE_NULL = 0,
	//ͨ��M1��
	CARDSTYLE_NORM1,
	//CPU��
	CARDSTYLE_CPU,
	//2.4G CPU��
	CARDSTYLE_24CPU,
	//�մ�M1��
	CARDSTYLE_YIDAM1,
	//2.4G�ƶ���
	CARDSTYLE_24Mobile,	// 5
	   //2.4G���ſ�
	   CARDSTYLE_24CT,
	   //�п�Ѷ��2.4G ��ͨ
	   CARDSTYLE_24ZKXL,
	   //����ʢ��2.4G ��ͨ������
	   CARDSTYLE_24XMSH,
	   //�����Ͽ�
	   CARDSTYLE_ANLONG,

	   SEND_FILE_ERROR_,	// 10
	   CARDSTYLE_JTB,
	   QPBOC_JTB,   //��ͨ�����ֽ�
	   CARDQR_CODE,	//��ά��
	   CARDSTYLE_QR_PBOC,
	   CARDSTYLE_PBOC,
	   CARDSTYLE_QPBOC,
	   CARDSTYLE_UNPAY_ODA,	// 15
	   CARDSTYLE_QR_UNPAY_BUS,
}CardCand_enum;


#define ID_REC_QRC_GZT		0xA6		//����ͨ��ά���¼
#define CARDSTYLE_QR_TX			0xA4		//��Ѷ��ά���¼
#define ID_REC_QRC_TX		0xA4		//��Ѷ��ά���¼
#define ID_REC_QRC_ZFB		0xA5		//֧������ά���¼
#define ID_REC_QRC_PBOC		0xA7		//������ά��S
// A8�ǿɳ��ά��
#define ID_REC_QRC_UNPAY_BUS	0xA9		//�����˳���

//mifare

#define PICC_REQIDL        0x26         //!< request idle
#define PICC_REQALL        0x52         //!< request all
#define PICC_ANTICOLL1     0x93         //!< anticollision level 1 106 kBaud
#define PICC_ANTICOLL2     0x95         //!< anticollision level 2
#define PICC_ANTICOLL3     0x97         //!< anticollision level 3
#define PICC_AUTHENT1A     0x60         //!< authentication using key A
#define PICC_AUTHENT1B     0x61         //!< authentication using key B
#define PICC_READ          0x30         //!< read block
#define PICC_WRITE         0xA0         //!< write block
#define PICC_UL_WRITE      0xA2         //!< ultralight write block
#define PICC_DECREMENT     0xC0         //!< decrement value
#define PICC_INCREMENT     0xC1         //!< increment value
#define PICC_RESTORE       0xC2         //!< restore command code
#define PICC_TRANSFER      0xB0         //!< transfer command code
#define PICC_HALT          0x50         //!< halt


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

//***************************************************************************
//                       ���������ĸ�ʽ���� 
//***************************************************************************
#define CARD_PULICH_MESSAGE_LEN		8
#define Public_dealpo 0
#define Public_moneydealsum 1
#define Public_delaflag 3
#define Public_monthdealsum 4
#define Public_blackflag 6
#define Public_fileflag 7
typedef struct
{
	unsigned char DealPo;		//������ָ��	0
	unsigned char MoneyDealSum[2];	//Ǯ�����״���  1
	unsigned char DelaFlag;		//���׽��̱�־  3
	unsigned char MonthDealSum[2];	//��Ʊ���״���  4
	unsigned char BlackFlag;	//��������־    6
	unsigned char FileFlag;		//�ļ���ʾ      7
}Parameter8;

typedef struct
{
	unsigned char old_Purcs_Time[7];
	unsigned char old_Purcs_LineNum[3];
	unsigned char old_Purcs_Device[4];
	unsigned char resear[2];
}oldPursIfo;



#define ERROR_DETAIL_SUM			3
#define ERROR_DETAIL_LEN			32
typedef struct
{
	unsigned char effFlag;		//�������ͣ���Ʊ������Ǯ����
	unsigned char Cardno[4];	//����Ŀ���
	unsigned char ErrorStep;	//������ȣ���Ǯ���Ρ�������д��������
	unsigned char OpSector;		//û�ã�
	unsigned char OpBlock;		//û�ã�
	unsigned char TollValue[4];	//���׽��
	unsigned char AferToll[4];	//���׺����
	unsigned char ToChange[16];	//������Ϣ�����ݡ�д������Ϣ����ʱ�����á�
}Parameter6;

//00 00 00 04 00 12 34 56 78 1A 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 
typedef struct
{
	unsigned char FCcode[2];	//����������
	unsigned char citycode[2];	//���д���
	//	unsigned char algorithm;	//�㷨֧��
	unsigned char tradcoed[2];		//��ҵ����
	unsigned char recre[2];			//���� ��������ñ�ʶ
	unsigned char QYflag;			//Ӧ�����ͱ�ʶ�����ñ�־ 01���ã�1
	unsigned char YYVer;			//Ӧ�ð汾1
	unsigned char FYFlag[2];		//��ͨ��ʶ2
	unsigned char YYNO[8];			//Ӧ�����к�8
	unsigned char StartDate[4];		//Ӧ���������ڣ�YYYYMMDD�� 4
	unsigned char EndDate[4];		//Ӧ����Ч���ڣ�YYYYMMDD�� 4
	unsigned char rFu2[2];			//Ԥ��2
	unsigned char cardtype;		//������
	unsigned char Ymoney;		//Ѻ�� ��λ��Ԫ
	unsigned char rFu3[5];		//Ԥ��
}stJSBF15;
typedef struct
{
	unsigned char cPublisherSign[8];	/* ��������ʶ */
	unsigned char cAppType;				/* Ӧ�ð汾�� */
	unsigned char cCitycode[2];			/* ���д���	*/
	unsigned char cCardType;			/* ��Ӧ������ */
	unsigned char cApp_sn[10];			/* Ӧ�����к� 5��00H�ֽ� + 1����Ƭ�����ֽ�+ 4����ˮ�ֽ� ����Ϊ����5���ֽ� */
	unsigned char cIssueDate[4];		/* Ӧ���������� */
	unsigned char cValidDate[4];		/* ��Ч���� */
	unsigned char cSing;				/* �Ƿ���� */
	unsigned char cYaJin[2];			/* Ѻ�� */
	unsigned char cMinremain[2];		/* ���������� */
	unsigned char cUseFlah;				/* ʹ�ñ�� */
	unsigned char cMaxPur[4];			/* ������ѽ�� */
	unsigned char cLinezf;				/* �����˻���� */
	unsigned char cRFU[9];				/* ���� */
}typea_public_inf_zaozuang;				//��ׯCPU����Ϣ�ṹ

typedef struct
{
	unsigned char cTermSnq[4];			/* �ն˻�������� */
	unsigned char cDateTime[7];			/* ��������ʱ�� */
	unsigned char cMac[4];				/* ����MAC�� */
}pboc_debit_capp_purchase;

typedef struct
{
	unsigned char cBalance[4];			/* ����Ǯ����� */
	unsigned char cSnq[2];				/* ����Ǯ���������к� */
	unsigned char cOverdraftLimit[3];	/* ͸֧���� */
	unsigned char cKeyVer;				/* ��Կ�汾 */
	unsigned char cKeyEn;				/* ��Կ��ʶ */
	unsigned char cRand[4];				/* ����� */
}pboc_init_capp_purchase_res;
typedef struct
{
	unsigned char cRand[4];				/* ����� */
	unsigned char cSnq[2];				/* ����Ǯ���������к� */
	unsigned char cMoney[4];			/* ���׽�� */
	unsigned char cType;				/* �������� */
	unsigned char cDateTime[7];			/* ��������ʱ�� */
	unsigned char cKeyVer;				/* ��Կ�汾 */
	unsigned char cKeyEn;				/* ��Կ��ʶ */
	unsigned char cKeyFactor[8];		/* 8��Կ��ɢ���� */
	unsigned char cPublisherSign[8];		/* 8��Կ��ɢ���� */
	unsigned char cReaderMAC[12];//��ͷMAC,����PSAM��У���ͷ�Ƿ�Ϸ�
}pboc_sam_init_purchase;


#define LAST_LIST_LEN			50				//��󱣴濨����
#define POINT_MESSAGE_LEN		17
#define RECORD_DEAL				0xba
#define RECORD_NONE				0

struct pMonthTime_1	//���治����ˢ����Ϣ��
{
	unsigned char endptr;
	unsigned char startptr;
	unsigned char LastList[LAST_LIST_LEN][7];              //��Ʊ����ʱ���
};

#define LEN_CARD_RECORD 16

typedef struct {
	int headp;	//ͷָ��
	int endp;	//βָ��
	unsigned int crc;	//У��
}stFileRecordPoint;

typedef struct
{
	unsigned char DealTime[4];			//����ʱ��4
	unsigned char Balance[4];		    //����ǰ���
	unsigned char DealMoney[3];			//���׽��
	unsigned char DealType;				//��������
	unsigned char DeviceNo[4];			//�豸��
}cardRecord1;

#define  ON_BUS			3	//Ǯ���ϳ�
#define	 OFF_BUS		4	//Ǯ���³�
#define  COVER_BUS		8 //Ǯ����Ʊ
#define  ON_Nomal		6//��ͨ����
#define FeRC_Write	1
#define FeRC_Dlelt	2
#define FeRC_Check	3

#define NO_CITY_UNION	0x63
#define APP_LOCK		0x64
#define NO_JTB_UNTION	0x65

#define NO_WriteNUM		0x80//���ڰ�������
#define NO_START		0x81//δ����
#define NO_Ech			0x83//���ǻ�����
#define NO_APP			0x84//δ����
#define PursAgian		0x85//��Ʊ
#define LocakCard		0x86//���ؿ�
#define YiDiCard		0x87//��ؿ�

#define ID_REC_HJY_REC		100			//�ҽ��׼�¼����



#define RECORD_LEN 64
#define RECORD_JTB_LEN 128

#define PBOC_RECORD_LEN 150  //�ϴ������п�ʵ����Ч������150���ֽڣ�132+4+1+4��
#define QRC_RECORD_LEN 248  //�ϴ������п�ʵ����Ч������248���ֽڣ�132+4+1+4��

#define TXQRCRECORD_LEN 256


//�ڳ�վ�ϳ�������2 6  �����������ϳ�
#define BUS_Star			(1<<0) //1 3 5 7ˢ������ 1 0XFE����ˢ�˷���	��վҪ���
#define BUS_InStation		(1<<1) //�ڳ�վ�� 2 5 6�ڳ�վ  0XFD
#define BUS_ON				(1<<2) //�б�ʶΪ 4 �ϳ���¼ 0XFB
#define BUS_TES				(1<<7) //���Լ�¼

#define BUS_CL_Star			0xFE //���������
#define BUS_CL_InStation	0xFD//���ڳ�վ
#define BUS_CL_ON			0xFB//�³�
#define BUS_CL_TES			0x7F//���Լ�¼���


typedef struct
{
	unsigned char cRand[4];				/* ����� */
	unsigned char cSnq[2];				/* ����Ǯ���������к� */
	unsigned char cMoney[4];			/* ���׽�� */
	unsigned char cType;				/* �������� */
	unsigned char cDateTime[7];			/* ��������ʱ�� */
	unsigned char cKeyVer;				/* ��Կ�汾 */
	unsigned char cKeyEn;				/* ��Կ��ʶ */
	unsigned char cPublisherSign[8];	/* 8��Ӧ�����кſ��ź�8�ֽ�*/
	unsigned char cKeyFactor[8];		/* 8������������ */
	unsigned char cReaderMAC[12];//��ͷMAC,����PSAM��У���ͷ�Ƿ�Ϸ�
}pboc_sam_init_purchaseJTB;
typedef struct//����ֵ��С���ֵ� һ��ΪС��ģʽ
{
	//29�����ǵ���ˢ��������
	unsigned char rPublishNumb[10];		//BCD���п���ˮ�ſ���
	unsigned char rCardType;			//����
	unsigned char rCardCand;			//����:������  M11��  2 CPU�� 11 �ֽ�
	unsigned char rSAMxulie[4];			//PSAM�ն˽������к� (�������ƽ̨ �� �����豸�Զ����ɵ���ˮ)
	unsigned char rPSAMNO[6];			//PSAM����72  BCD
	unsigned char rDeviceNo[4];			//�豸������ �������豸�ţ����ƺţ�(HEX)
	unsigned char rDealType;			//��������	HEX
	unsigned char rPiaoJia[2];			//Ʊ�� ���
	//20
	unsigned char rMoneyDealSum[2];		//Ǯ���ۼƽ��״���    Ӧ�ý��׼����� 9F36HEX
	unsigned char rDealMoney[3];		//���׽�� HEX 9F02
	unsigned char rAfterMoney[4];		//���23���
	unsigned char rDealTime[7];			//����ʱ��YYYY/MM/DD/HH/MM/SS 22���ֽ� 
	unsigned char rCPUTAC[4];			//CPU������֤�� *����CPU�����ص���֤��
	//20//
	unsigned char rDriverNo[4];			//˾���� ���
	unsigned char rLineNo[3];			//��·�� ���
	unsigned char rGPSzhandian;			//վ����
	unsigned char rGPSfangxian;			//GPS���� ����/����
	unsigned char rFactorFlg[8];		//�³�������ʶ
	unsigned char rCityCode[2];			//���д���
	unsigned char rHUIJIAOYI;			//�ҽ��� 0���� 1������
	//43//��¼�ļ����ϱ����� (�ϳ���¼)
	unsigned char rDealTimeOLD[7];		//����ʱ��YYYY/MM/DD/HH/MM/SS* BCD
	unsigned char rPSAMNOOLD[6];		//�ϱʽ����ն� BCD ����PSAM����
	unsigned char rDriverNoOLD[4];		//˾���� 
	unsigned char rLineNoOLD[3];		//��·�� 
	unsigned char rGPSzhandianOld;		//�ϳ�վ����
	unsigned char rCityCodeOLD[2];		//�ϳ����д���
	unsigned char rFactorFlgOLD[8];		//�ϳ�������ʶ
	unsigned char rProviceOLD[3];		//�ϳ��ĳ��� һ������+һ����ĸ(��ʱ����)
	unsigned char rDeviceNoOLD[4];		//�ϳ����ƺ�(HEX)
	//......................................................
	unsigned char rProvice[3];			//�³��ĳ��� һ������+һ����ĸ(��ʱ����)
	unsigned char rZwyNum[4];			//�³�վ��Ա���
	unsigned char rBusTC[2];			//�˴�
	//����һ��104�ֽ�
	//
	//	QPBOC_RC	  rQpboc_Rc;			//QPBOCר������ 70���ֽ�
	unsigned char rBusFlag;				//��ͬ״̬��ˢ����¼ 
	unsigned char reasea[5];			//����
	unsigned char rProVer[2];			//����汾��
	unsigned char rTTTT[4];				//CRCУ���ĸ��ֽ�AA


}DEALRECODE_JTB;

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

typedef struct
{
	unsigned char rFlag[8];
	unsigned char rCardDealNumb[4];		//�豸������ˮ
	unsigned char rDeviceNo[4];			//�豸��(���к�)
	unsigned char rCardType;			//����
	unsigned char rDealType;			//��������
	unsigned char rPublishNumb_a;		//���п���ˮ�����λ  
	unsigned char rRemain;				//����   //˾������λ


//	unsigned char ruserTimes[2];		//ʹ�ô��� ˾���Ÿ�λ  [�ֶ� ȫ��ʱ��] ������пصļ�¼�����湲18�ֽ�Ϊ��Ƭ�����֤��
	unsigned char rAfterMoney[4];		//ԭ��
	unsigned char rDealMoney[3];		//���׽��
	unsigned char rDealTime[7];			//����ʱ��YYYY/MM/DD/HH/MM/SS
	unsigned char rDriverNo[4];			//˾����
	unsigned char rDeiceSleNo[4];		//�豸�Ա�� //У����
	unsigned char rLineNo[2];			//��·��
	unsigned char rProVer[2];			//����汾��
	unsigned char rPrice_NA[2];			//Ʊ��
	unsigned char QRC_record[204];		//*��ά���¼�����ģ� �ַ�����BASE64��ʽ����0x00Ϊ�������������ֶ�Ϊ��ά�뽻�׼�¼����Ҫֱ�Ӵ�����Ѷ����������
//	unsigned char QRC_record[212];		//*��ά���¼�����ģ� �ַ�����BASE64��ʽ����0x00Ϊ�������������ֶ�Ϊ��ά�뽻�׼�¼����Ҫֱ�Ӵ�����Ѷ����������
	unsigned char rTAC[4];				// У����
}DEALRECODE_OTHQRC;	//��ά���¼

typedef struct
{
	unsigned char flag;			//��ʶ��1�ֽڣ�
	unsigned char citycode[6];	// 6�ֽڳ��д��룩
	unsigned char ZCcode[9];	//ע��Ψһ�루9�ֽڣ�
	unsigned char datetime[10];	//����ʱ�䣨10�ֽڣ�
	unsigned char checkdata[10];	//��֤�루10�ֽ�У�飩��
	unsigned char longitude[4];		//���ȣ�4�ֽڣ�
	unsigned char latitude[4];		///ά�� 4�ֽ� 
	unsigned char direction[4];		//����4�ֽ�
	unsigned char speed[4];			//�ٶ�4�ֽ�

}stltyQRCandGPS;

typedef struct {
	unsigned char falg[4];//��Ч��ʶ
	unsigned char RCE[128];//��¼
	unsigned char CRC[4];	//CRC
}FeUnKnowREC;
typedef struct
{
	unsigned char cPublisherSign[8];	/* ����������ʶ */
	unsigned char cAppType;				/* Ӧ�����ͱ�ʶ 02 ���������� */
	unsigned char cStarFlag;			/* ��������Ӧ�ð汾 *���ñ�ʶ*/
	unsigned char cApp_sn[10];			/* Ӧ�����к�*/
	unsigned char cIssueDate[4];		/* Ӧ���������� */
	unsigned char cValidDate[4];		/* Ӧ����Ч���� */
	unsigned char cFCI[2];				/* ���������Զ���FCI���� */
}stJTB15;		//��ͨ��95�ļ��ṹ      30byte


typedef struct
{
	unsigned char cCardType;			/* �����ͱ�ʶ */
	unsigned char cStuffcode;		/*����ְ����ʶ*/
	unsigned char cName[20];			//�ֿ�������
	unsigned char cCredent[32];			/* ֤���� */
	unsigned char cCretype;				//֤������
}stJTB16;		//��ͨ��96�ļ��ṹ     55byte
typedef struct
{
	unsigned char intercode[4];			/* ���ʴ��� */
	unsigned char provcode[2];			/*ʡ������*/
	unsigned char Citycode[2];		/*���д���*/
	unsigned char car_type[2];			//��ͨ����  00�ǻ�ͨ�� 01 ��ͨ��
	unsigned char card_catalog;			//��������
	unsigned char reser[49];			//Ԥ��
}stJTB17;		//��ͨ��97�ļ�:������Ϣ�ļ�  60byte

typedef struct
{
	unsigned char rEPsearl[2];			/* EP���������ѻ��������к� */
	unsigned char rOverdraft[3];		/*͸֧�޶�*/
	unsigned char rPursPrice[4];		//���׽��
	unsigned char rPursType;			/*�������ͱ�ʶ*/
	unsigned char rPSAMnum[6];			//�ն˻����	
	unsigned char rPursTime[7];			//����ʱ��
}stJTB18;		//��ͨ��98�ļ�:  23

typedef struct//w�ļ��� 2702   128�ֽ�
{
	unsigned char rID[2];				//1-2���ױ�ʶ  2702  ��������Ӧ����Ϣ
	unsigned char rlen;					//3��¼����
	unsigned char rOk;					//4��Ч��ʶ  �̶�01
	unsigned char rpursFlag;			//5���ױ�ʶ1 ���÷��Ƿֶο۷�/��������    2 Ԥ��Ȩ����
	unsigned char rLock;				//6��Ӧ�ñ�ʶ 0û���� 1����
	unsigned char rPursSearl[8];		//7-14������ˮBCD
	unsigned char rPursState;			//15����״̬��ʶ1	00����ʼֵ 01���ϳ� 02���³�
	unsigned char rCityCodeON[2];		//16-17�ϳ����д���
	unsigned char rFactoryOn[8];		//18-25�ϳ���������
	unsigned char rYunYinCode[2];		//26-27-�ϳ����д���
	unsigned char rLineNum[2];			//28��29�ϳ���·��	2	HEX�����ֽ���ǰ��
	unsigned char rStationOn;			//30�ϳ�վ��
	unsigned char rBusNum[8];			//31-38�ϳ�������Ascii
	unsigned char rPsamOn[8];			//39-46�ϳ��ն˺�BCD
	unsigned char rPursTimeOn[7];		//47-53�ϳ�ʱ��
	unsigned char rPriceMax[4];			//54��57	��ע���4������Ʊ׷�� HEX�����ֽ���ǰ��
	unsigned char rDirection;			//58����
	unsigned char rCityCodeOff[2];		//59-60�³����д���
	unsigned char rFactoryOff[8];		//61-68�³���������
	unsigned char rYunYinCodeOff[2];	//69-70�³���Ӫ����HEX
	unsigned char rLineNumOff[2];		//71��72�³���·��	2	HEX�����ֽ���ǰ��
	unsigned char rStationOff;			//73�³�վ��
	unsigned char rBusNumOff[8];		//74-81�³�������Ascii
	unsigned char rPsamOff[8];			//82-89�³��ն�
	unsigned char rPursTimeOff[7];		//90-96�³�ʱ��
	unsigned char rPrice[4];			//97-100���׽��
	unsigned char rMileage[2];			//101-102�˳���� HEX
	//30								//103-128Ԥ��26

//	unsigned char rCiytCodeY[2];		//103-104Ԥ����Ϣ���д���
//	unsigned char rlenY;				//105Ԥ����Ϣ���� 
	//16
	unsigned char rDriverNum[4];		//106-19�ϳ�˾����
	unsigned char rCwuyuanNum[4];		//110-113 �ϳ�����Ա���
	unsigned char rTangCi[2];			//114-115�˴�
	unsigned char rTimeMax[2];			//116-117�ʱ��,��λ ����
	unsigned char rName[10];			//119-128վ������Ascii  �������
	unsigned char rCH[4];				//118 Ԥ��

//	unsigned char rResear[8];			//Ԥ����Ϣ����
}stJTB1A;		//��ͨ��1A�䳤��¼�ļ��ṹ     55byte
typedef struct//1E   48�ֽ�
{
	unsigned char eDealType;	//00��ʼֵ  01��EC��վ/�ϳ�/�����ȣ�02��EC��վ/�³�/�����ȣ�03 EP�� 04 EP��վ/�³�/������05��EC�������ѣ�   
								// 	06��EP�������ѣ�07��EC��Ʊ08��EP��Ʊ��09��EC��ֵ��0A��EP��ֵ������Ԥ����						
	unsigned char eDeviceNum[8];//�ն˱��BCD
	unsigned char eTracode;		//10��ҵ���� 01���� 02���� 03����ˮ�Ͽ��� 04�������� 05�������� 06�������г� 07ͣ���շ� 
								//08��;���� 09�ֶ� 0A�Ǽʸ�����· 0B�� 0C���ٹ�·�շ� 0E�н����������٣� 0F��12 ���淶Ԥ��
	unsigned char eLineNUm[4];	//11-14��·�� BCD
	unsigned char eRF[3];		//15-17RF
	unsigned char eDmoney[4];	//18-21���׽�� ���HEX
	unsigned char eYuE[4];		//22-25���
	unsigned char eDealTime[7]; //26-32��������ʱ��
	unsigned char eCityco[2];	//33-34�������д���
	unsigned char eFactoryco[8];//35-42��������
	unsigned char eErrPriceMax[4];//43-46���������������
	unsigned char eTimeMax[2];	//47-48�ʱ��,��λ ���Ӵ�



	unsigned char N[6];			//Ԥ��
}stJTB1E;		//1E 48�ֽ�

typedef struct
{
	unsigned char kid;
	unsigned char PublisherSign[8];
	unsigned char ReceiverSign[8];
	unsigned char startDate[4];
	unsigned char endDate[4];
}sam_pub;



typedef struct
{
	unsigned char busProVer[2];//����汾
	unsigned char busBLKVer[2];//�������汾
	unsigned char CSN_BUSVer[2];//��·��Ϣ�汾
	unsigned char busLineVer[2];//վ��������Ϣ�汾2B
	unsigned char busVoiceVer[2];//��Կ��Ϣ�汾2B   ����У���Ҫת��16������
	//	unsigned char WHT_BUSVer[2];
	unsigned char White_Ver[2];//ס�������а�������Ϣ�汾  ��λ�ں�
	unsigned char White_Ver_JTB[2];//��ͨ�����а�������Ϣ�汾  ��λ�ں�
	unsigned char Linux4GVer[2];//Linuxģ��汾��

	unsigned char busticketVer[2];//Ʊ�۰汾
	unsigned char busTXTPKVer[2];//��Ѷ��Կ�汾

	unsigned int BlackListNum;		//����������,����
	unsigned int TXPublicKeyLen;	//��Ѷ��Կ���� 
	unsigned int WHTBusListNum;		//ס�������������� 
	unsigned int WHTJTBListNum;		//��ͨ������������ 
}stBusVerIfo;


#define MAXList 3
#define MAXLINE 2048
#define _SOCKET_MAXNUM 4

typedef struct {
	unsigned char netwwwOk;	//�����Ѿ���ͨ
	unsigned char PPPtimes;	//�����κŴ���

	unsigned char gmissflag;				//�����ʶ��ָʾ��ǰ��ͨѶ����״̬
	unsigned char GPRSLinkProcess;			//����ͨѶ���ӽ���
	unsigned char isNetOK[_SOCKET_MAXNUM];	//�������Ƿ����ӳɹ�

	unsigned char ISOK;						//ƽ̨�Ƿ�ǩ���ɹ�
	//------������Կ WK����------------------
	unsigned char PIN_KEY[16];				//û��
	unsigned int crc_pinkey;
	unsigned char MAC_KEY[16];				//
	unsigned int crc_mackey;
	//---------------------------------------
	unsigned char gcmdcode[100];			//�������ƣ�LLVAR�����99���ַ�,ǰһ���ֽ��ǳ���

	unsigned int gSendGLogin;				//��������������
	unsigned char GprsCardList[20];			//SIM�����

	unsigned int gprsErrorTimes;			//���Ӵ������
}stGPRSinfo;


typedef struct
{
	unsigned int colStyle;			//�ɼ�����,0Ϊ�����ڣ�1����ˮ��,,,��ǰֻ���������ڲɼ�
	unsigned char startdate[4];		//����Ϊ0ʱ���·�����������4 YYYYMMDD ;����Ϊ1ʱΪ��ʼ��ˮ�ţ���λ��ǰ
	unsigned char enddate[4];		//����Ϊ0ʱ���·�����������4 YYYYMMDD ;����Ϊ1ʱΪ������ˮ�ţ���λ��ǰ
	char sfilename[100];			//���ڷ����ļ���
	unsigned int filelen;			//��ȡ�ļ����ļ��ܴ�С����ֹ���������
	unsigned int offset;			//��ǰ�ļ�����λ��
	unsigned int scrc32;

}stFAT_hisRec;		//��ʷ��¼�ļ� ��fat�ļ�ϵͳ�� 20�ֽ�
#define BIT_REORD_HIS_LEN (sizeof(stFAT_hisRec))

//����Ϣ�ṹ
typedef struct {
	unsigned int c_serial;			//�����кš�
	CardCand_enum gMCardCand;		//������


	unsigned char listSector;
	unsigned char publishSector;
	unsigned char publicMoneySector;
	unsigned char publicMessageSector;
	unsigned char dealRecordSector[3];	 //!@	
	unsigned char oddMonthSector;
	unsigned char dodMonthSector;

	unsigned char PublishKEYA[6];
	unsigned char PublichKEYA[6];
	unsigned char MonthKEYA[6];
	unsigned char MoneyKEYA[6];
	unsigned char RecodeKEYA[6];

	unsigned char CityCode[2];
	unsigned char TradeCode[2];
	unsigned char PublishBicker[10];
	unsigned char PublishBickerH[4];
	unsigned char CardCheckCode[4];
	unsigned char card_catalog;
	unsigned char JTBcard_catalog;
	unsigned char Zicard_catalog;
	unsigned char MoneySectorType;

	unsigned char nnAudioflag;	//������ʾ��ʶ

	unsigned char catalogPOST;//Ŀ¼������������ƽ̨��ʹ��

	unsigned char PbocTac[4];
	unsigned char stuffNO[4];//�������ķ������ڣ�˾����������Ա�����

	unsigned char getdata_w;
	unsigned char getdata_flag;
	unsigned char WriteRecord_dis;
	unsigned char FactorFlg[8];//��������
	unsigned char ProcessFlag;

	unsigned char gucRestorFlag;	//���ڱ�ʶ���ڸ���Ϣ��״̬����Ǯ���Ƿ�����������Ǯ������Ϣ���Ƿ�һ�µ�
	unsigned char CardType; //�������� ����/���
	unsigned char gucEndTimeFlag;//�Ƿ���ʾ��Ҫ�����ʶ
	unsigned char gucSoudnorm;		//�Ƿ���ʾ���ֵ��ʶ

}stCardInfo;

#ifndef _BITS8_ATTR_
#define _BITS8_ATTR_
typedef struct {
	unsigned char bit0 : 1;	//111
	unsigned char bit1 : 1;
	unsigned char bit2 : 1;
	unsigned char bit3 : 1;
	unsigned char bit4 : 1;
	unsigned char bit5 : 1;
	unsigned char bit6 : 1;
	unsigned char bit7 : 1;
}stBits8;

typedef union {
	unsigned char dataCh;
	stBits8 bits;
}unBits8;
#endif


typedef struct {
	char dataFlag;
	int bufLen;
	unsigned char Sendbuf[MAXLINE];
}stNetSendList;
typedef struct {
	char dataFlag;
	int bufLen;
	unsigned char Revbuf[MAXLINE];
}stNetRevList;


//������63��˵��,������
typedef struct
{
	unsigned char 	Organization_[8];		//
									//	INT8U	up_time[8];		//
	unsigned char 	card_no[10];

}QPBOC_TYPE_63;


//ͬһ����������ʱ����ʱ30��
#define CONNECT_DLYTIME 30
typedef struct {
	char linkNo;
	char APN[40];
	char IPaddr[16];
	int port;
	long pcTime;	//�ϴ�����ʱ��
					// // HEX����8λ��ÿλΪ1������Ч��0λ��HTTPS��1λΪ0��APN1��1λΪ1��APN2.
	unBits8 linkAttr;	// ��������
}stServerInfo;

typedef struct {
	unsigned char citycode[2];
	unsigned char hanye[2];
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


	unsigned char DeviceNo[9];		//�豸��


	unsigned char LineNo[3];
	unsigned char stopflag;					//�Ƿ�Ҫ��ͣˢ�� POS
	unsigned char ManagePassWord[3];//6λ���� 3 �ֽ� POS
	unsigned char reservee[3];		//--------60
	unsigned char UserKeyA[16];//����Կ������Կ
	unsigned char UserKeyACrc32[4];//kEY CRC
	unsigned char UserKeyB[16];//����Կ������Կ
	unsigned char UserKeyBCrc32[4];//kEY CRC  -----100

	unsigned char BussinessNo[6];//�̻����2�ֽ�+CRC32 4�ֽ� ��6�ֽ� 
	unsigned char ReseverAPN[17];//POS������APN
	unsigned char catalogPOST;//Ŀ¼������λ��
	unsigned char wifi_SSID[10];	//WIFI SSID
	unsigned char wifi_KEK[20];		//WIFI KEY
	unsigned char wifi_CrC32[4];	//WIFI KEY �� WIFI SSID ��У���� 
	unsigned char Reserver[18];//

	unsigned char SN[4];		//�豸���к�

	stServerInfo gServerInfo[_SOCKET_MAXNUM];

	unsigned char Null[1024];

	unsigned char Crc32[4];

	QPBOC_TYPE_63 TYPE_63;
	int batchNo;

}stDeviceParatable;//�µ�Ʊ��

typedef struct
{
	unsigned char stop_flag;			//ͣ��״̬	1(ͣ)0(����)
	unsigned char PlusSta;				//�Ƿ���ȡ���ӷ�		
	unsigned char set_device_status;	//�豸��ˢ����־
	unsigned char set_time_status;		//ʱ�����ÿ�ˢ����־
	unsigned char set_line_status;		//��·��ˢ����־
	unsigned char WriteCardFlag;
	unsigned char verrrr;
	unsigned char restore_flag;		//��Ϊ0������Ҫˢ�½��浽������
	unsigned char g24GDisFlash;		//������ʱˢ�½���

	unsigned char DriverNO[4];		//˾����
	unsigned char StufferNO[4];		//����Ա��

	unsigned short SumTi;			//��ǰ���Ѵ���	0
	unsigned short SumMti;			//��ǰ��Ʊ��ˢ������ 2
	unsigned int SumMoney;			//��ǰ�����ܶ�  4


	unsigned short PackSerial;			//GPRS���Ͱ������к�
	unsigned int MoneyDealPointer;		//�豸��ˮ�ţ�Ǯ��  
	unsigned int MonthDealPointer;		//�豸��ˮ�ţ���Ʊ
	unsigned int ControlDealPointer;	//�豸��ˮ�ţ�����



}fFlagVary_1;	//����״̬��ʶ

#ifndef _ProtocolErrCode_
#define _ProtocolErrCode_
typedef enum {
	PECUnkown = 0x1000,
	PECParam,
	PECFunction,
	PECSucess = 0x9000
}ProtocolErrCode;
#endif // !_ProtocolErrCode_

// #ifndef _RetCode_
// #define _RetCode_
// typedef enum {
// 	Ret_Error = -32768,
// 		Ret_Err_Param,
// 		Ret_Err_OverFlow,
// 		Ret_Err_Function,
// 		Ret_Err_Format,
// 		Ret_Err_TimeOut,
// 		Ret_Ok = 0
// }RetCode;
// #endif // !_RetCode_



// function, little endian
// #define SET_INT16(buf, len)	(buf)[0] = ((len) >> 8) & 0x0FF; (buf)[1] = ((len)) & 0x0FF
#define GET_INT16_B(buf) (((buf)[0]<<8)|((buf)[1]&0x0FF))	// ��˴洢
//#define GET_INT16(buf) (((buf)[1]<<8)|((buf)[0]&0x0FF))	// С�˴洢


#define voice_invalid		"/mnt/qrdata/wav/invalidcard.wav"		//�Ƿ���
#define voice_studentcard	"/mnt/qrdata/wav/studentcard.wav"		//ѧ����
#define voice_jianhangcard	"/mnt/qrdata/wav/jianhangcard.wav"	//���п�
#define voice_stuffcard		"/mnt/qrdata/wav/stuffcard.wav"		//Ա����
#define voice_jiaotongcard	"/mnt/qrdata/wav/jiaotongcard.wav"	//��ͨ��
#define voice_jinglaocard	"/mnt/qrdata/wav/jinglaocard.wav"		//���Ͽ�
#define voice_liantongcard	"/mnt/qrdata/wav/liantongcard.wav"	//��ͨ��
#define voice_youfucard		"/mnt/qrdata/wav/youfucard.wav"		//�ŷ���
#define voice_monthcard		"/mnt/qrdata/wav/monthcard.wav"		//��Ʊ��
#define voice_youhuicard	"/mnt/qrdata/wav/youhuicard.wav"		//�Żݿ�
#define voice_normalcard	"/mnt/qrdata/wav/normalcard.wav"		//��ͨ��
#define voice_zhuanxiancard "/mnt/qrdata/wav/zhuanxiancard.wav"	//ר�߿�
#define voice_plsgeton		"/mnt/qrdata/wav/plsgeton.wav"		//���ϳ�
#define voice_drivercard	"/mnt/qrdata/wav/drivercard.wav"		//˾����
#define voice_showagain		"/mnt/qrdata/wav/showagain.wav"		//����ˢ
#define voice_shuamashangche "/mnt/qrdata/wav/shuamashangche.wav"	//ˢ��ɹ�,���ϳ�
#define voice_WelCome		"/mnt/qrdata/wav/success.wav"			//��ӭ�˳�
#define voice_chargemonty	"/mnt/qrdata/wav/chargemonty.wav"		//���ֵ
#define voice_flingmoney	"/mnt/qrdata/wav/flingmoney.wav"		//��Ͷ��
#define voice_refleshRQC	"/mnt/qrdata/wav/refleshRQC.wav"		//��ˢ�¶�ά��*
#define voice_invalidRQC	"/mnt/qrdata/wav/invalidRQC.wav"		//��Ч��ά��*

//�ļ���ض���
#define WorkDir "/mnt/posdata"

//�豸��Ϣ�ļ�,���治�Ǿ������ĵ��豸��Ϣ�������̻��š��豸�š�Ʊ�ۡ���·�ŵ�
#define _File_DevicePara "/DevicePara.inf"
#define _File_DevicePara_mode 0
//�豸״̬��ʶ���������Ϣ�仯��ԱȽ�Ƶ������Ϣ��
#define _File_BuInfo "/BusInfo.inf"
#define _File_BuInfo_mode 1

//��ǰ��¼�ļ�����ǰ��¼�ļ���ʼ��2M�ֽڡ�Ȼ����ָ���ļ������м�¼д�롣
#define _File_Record_Cur	"/currecord.rec"
#define _File_Record_Point	"/RecordPoint.inf"
#define _File_BusVer		"/BusVer.ver"

#define _File_MothDelayTime		"/MothDelay.inf"
#define _File_ErrorCard		"/ErrorCard.inf"
//9��������Ϣ�ļ�, ���ڱ������ؽ��ȣ��Ա�����������
#define _File_FileDownPara "/FileDownPara.inf"

//��Ҫ���ڱ�����ʱ���ص��ļ������ļ����������ᱻɾ���������µ��ļ�����ʱ�ᱻ����
#define _File_TempFile "/TempFile.tmp"
//�ɼ���Ϣ�ļ������ڱ�ʶ��Ҫ�ɼ�����Ϣ���ݡ�
#define _File_His_Collect "/ColHisinfo.inf"
//�����������ļ�
#define _File_BlackList_bus "/Blacklist.lst"
//��Ѷ��Կ
#define _File_TXPublicKey "/TXpublicKey.lst"
//ס������������Կ
#define _File_WiteListBUS "/WiteListBUS.lst"
//��ͨ����������Կ
#define _File_WiteListJTB "/WiteListJTB.lst"
//�����ļ�����ֱ��ʹ�ã�ֻ�����ڱ��ݣ�
#define _File_Bus_csn "/Buscsn"
//Ʊ���ļ�����ֱ��ʹ�ã�ֻ�����ڱ��ݣ�
#define _File_Bus_PRI "/BusPRI"




#endif

