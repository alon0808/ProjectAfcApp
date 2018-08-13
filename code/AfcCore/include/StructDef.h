
#ifndef _Head_Struct_def_file_
#define _Head_Struct_def_file_
//����ST����Ŀ⺯���ݣ����Բ����������
///////////////////PSAM������Ϣ//////////
typedef struct
{
	unsigned char serial[10];
	unsigned char verNo;
	unsigned char KeyType;
	unsigned char DefineData[2];
	unsigned char DeriveNo[6];
}PSAM_MESSAGE;
#define CARDMESSAGE_LEN		69
#define P_FIRST_VARY_LEN	32
typedef struct 
{
	unsigned int BlackListNum;	
	unsigned char PostCode[4];			
	unsigned char DeviceNo[9];			
	unsigned char DeviceNo_1[9];		
	unsigned char LineNoOld[4];				
	unsigned char LineNo[3];
	unsigned char count;					//���һ�ſ�������Ĵ���
	unsigned char fileSno[2];				//�ɼ����к�	
}pFistVary_1;

#define LEN_F_FLAG_VARY		7	

typedef struct
{
	unsigned char listSector;			
	unsigned char publishSector;		
	unsigned char publicMoneySector;	
	unsigned char publicMessageSector;
	unsigned char dealRecordSector[3];	 //!@	
	unsigned char oddMonthSector;		//��Ʊ��������	
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
	unsigned char Zicard_catalog;
	unsigned char MoneySectorType;	
	
	
	unsigned char catalogPOST;//Ŀ¼������������ƽ̨��ʹ��
	
	unsigned char PbocTac[4];	
	unsigned char stuffNO[4];//�������ķ������ڣ�˾����������Ա�����
	
	unsigned char getdata_w;
	unsigned char getdata_flag;
	unsigned char WriteRecord_dis;
	unsigned char ProcessFlag;

	unsigned char FactorFlg[8];//��������ʶ
}Parameter5;
typedef struct 
{
	unsigned char packetStatus;			//����״̬
	unsigned short length;				//���Ͱ�����
	unsigned char TTL;					//����ʱ��
	unsigned short Serial;				//�������к�
	unsigned char tcpBuffer[250];		//���ɷ���7����¼
}TCP_IP_PACKET1;

#define BIT_PACK_SERIAL			0
#define BIT_MONEY_POINT			2
#define BIT_MONTH_POINT			6
#define BIT_CONTROL_POINT		10
typedef struct 
{
	unsigned short PackSerial;			//GPRS���Ͱ������к�
	unsigned int MoneyDealPointer;		  
	unsigned int MonthDealPointer;		
	unsigned int ControlDealPointer;	
	unsigned char crc;					//У��
}Parameter2;

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
/*typedef struct
{
	unsigned char DealPo;		//������ָ��	0
	unsigned char MoneyDealSum[2];	//Ǯ�����״���  1
	unsigned char DelaFlag;		//���׽��̱�־  3
	unsigned char MonthDealSum[2];	//��Ʊ���״���  4
	unsigned char BlackFlag;	//��������־    6
	unsigned char FileFlag;		//�ļ���ʾ      7
}Parameter8;
*/


#define BIT_SUM_MONEY		4
#define BIT_SUM_TI			0
#define BIT_SUM_MTI			2
#define BIT_CRC_BYTES		8
#define BIT_CUR_E2PROM		9
#define BIT_BAK_CRC			13
#define BIT_C_CUR_PO		14
//
#define BIT_HEAD_ADDRESS	20
#define BIT_HEAD_CRC		28
#define BIT_BAK_HEAD_ADDRESS	24
#define BIT_BAKC_HEADCRC		29

#define BIT_RECORD_COUNT	30

#define COMSUM_MESSAGE_FILE_LEN		32
typedef struct
{
	unsigned short SumTi;			//��ǰ���Ѵ���	0
	unsigned short SumMti;			//��ǰ��Ʊ��ˢ������ 2
	unsigned int SumMoney;			//��ǰ�����ܶ�  4
	
	unsigned char BR_Crcbyte;			//��ǰָ���У��λ 8
	unsigned char BR_BAK_crc;			//����E2PROM��ָ���У��λ 9
	unsigned char resever[2];
	unsigned int BR_curE2pointer;		//E2PROM�ĵ�ǰָ�� 12
	unsigned int BR_c_cur_po;			//����E2PROM��ָ�� 16
	
	unsigned int BR_HeadAddress;		//��ʼ��ַ 20
	unsigned int BR_BAK_HeadAddress;  //���ݿ�ʼ��ַ24
	unsigned char BR_HeadCRC;			//��ʼ��ַ��У����28
	unsigned char BR_BACK_HeadCRC;		//���ݿ�ʼ��ַ��У����29
	unsigned short recordCount;		//��¼E2PROM���ܹ��еļ�¼���� 30
}Parameter3;
#define LAST_LIST_LEN			50				//��󱣴濨����
#define POINT_MESSAGE_LEN		17
#define RECORD_DEAL				0xba
#define RECORD_NONE				0

/*typedef struct
{
	unsigned char stop_flag;			//ͣ��״̬	1(ͣ)0(����)
	unsigned char PlusSta;				//�Ƿ���ȡ���ӷ�		
	unsigned char set_device_status;	//�豸��ˢ����־
	unsigned char set_time_status;		//ʱ�����ÿ�ˢ����־
	unsigned char set_line_status;		//��·��ˢ����־
	unsigned char WriteCardFlag;
	unsigned char verrrr;
}fFlagVary_1;

typedef struct
{
	unsigned char busProVer[2];//����汾
	unsigned char busBLKVer[2];//�������汾
	unsigned char busLineVer[2];//վ��������Ϣ�汾2B
	unsigned char busVoiceVer[2];//��Կ��Ϣ�汾2B   ����У���Ҫת��16������
	
	unsigned char newbusProVer[2];//����汾
	unsigned char newbusBLKVer[2];//�������汾
	unsigned char newbusLineVer[2];//��·��Ϣ�汾
	unsigned char newbusVoiceVer[2];//�����ļ��汾
}stBusVerIfo;*/

//��������������������ֹ�����·������ʱ����λ����
#define GPRS_Login_dly  40
typedef struct{
	unsigned char MISS_flag;//��ǰ�Ƿ���Ч��־
	unsigned char Miss_style;//�������ͣ�GnCode
	unsigned char MISS_MAXPack[4];//������
	unsigned char Miss_ver[2];//����汾��
	unsigned char Miss_ALL_LEn[4];//�������ֽ���
	unsigned char Miss_StartData[7];//������������ʱ��
	unsigned char Miss_PPack[2];//���ڸ��µİ����
	unsigned char Miss_Time;//������ͨѶʱ��
	unsigned char Miss_rever[5];//����
}stbusdownpro;//һ�س������ 	������ 4B+����汾 2B +�������ֽ�����4B+������������4B+��������ʱ��3B

typedef struct{
	unsigned char Miss_cmdCode[2];//CMDCODE��˵�������鵱ǰ���ܣ�����������·
	unsigned char Miss_style;//�������ͣ�GnCode
	unsigned int MISS_MAXPack;//������
	unsigned char Miss_ver[2];//���ذ汾��
	unsigned int Miss_ALL_LEn;//���������ֽ���
	unsigned short Miss_PPack;//���ڸ��µİ����
	unsigned char Miss_time;
}stbusdownPara;//���ز��� 	

typedef struct{
	unsigned char MISS_flag;//��ǰ�Ƿ���Ч��־
	unsigned char Miss_style;//�������ͣ�GnCode
	unsigned char MISS_MAXPack[4];//������
	unsigned char Miss_ver[2];//����汾��
	unsigned char Miss_ALL_LEn[4];//�������ֽ���
	unsigned char Miss_StartData[7];//������������ʱ��
	unsigned char Miss_PPack[2];//���ڸ��µİ����
	unsigned char Miss_Time;//������ͨѶʱ��
	unsigned char Miss_Line[2];//��·
	unsigned char Miss_rever[3];//����
}stGPSdownLine;//һ�س������ 	������ 4B+����汾 2B +�������ֽ�����4B+������������4B+��������ʱ��3B

// typedef struct{
// 	unsigned char MISS_Sreil[2];//������
// 	unsigned char Miss_style;//��������
// 	unsigned char Miss_ver[14];//������ɺ����յĺ����汾��
// 	unsigned char Miss_filever[14];//���������ڸ��µ��ļ�
// 	unsigned char Miss_len[4];//�����С
// 	unsigned char Miss_pesent[4];//����������� 1B ��ɰٷֱȣ�0x00-0x64��+ 4B �������ļ���ƫ��
// 	unsigned char MISS_UNIT;//�����ָ����ʲô����
// }stbusblkdownPara;//һ�غ����������

typedef struct{
	unsigned char Single[8];//8���ֽڵı�־˵��
	unsigned int StartAddr;//�׵�ַ
	unsigned int SendAddr;//�Ѿ����͵��ĵ�ַ
	unsigned int EndAddr;//��Ҫ���͵��Ľ�����ַ����������ʱʹ��,����������ʱʹ��gMobilREC_End
	unsigned int all_number;//�Ѿ����͵�����
	unsigned int all_money;//�Ѿ����ͳɹ���ͳ�ƽ��
	unsigned int SendPiCI;//�����κ�
	unsigned char BCC;//У��
}stMobilSend;//�ƶ����Ͳ����ṹ �������ֽ�

//*****************��������ʶ����(��32λ)******
#define ERROR_GPRS		(0x0001)	//GPRSģ�����
#define ERROR_SAM		(0x0002)	//SAM������
#define ERROR_PSAM1		(0x0004)	//PSAM1����    ʹ����
#define ERROR_PSAM2		(0x0008)	//PSAM2����    ʹ����
#define ERROR_DEVICE	(0x0010)	//�豸�Ŵ���
#define ERROR_LINENO	(0x0020)	//��·�Ŵ���
#define ERROR_DATETIME	(0x0040)	//ʱ�����
#define ERROR_24READER	(0x0080)	//ʱ�����    ʹ����

#define ERROR_CONNECT_MOBILE	(0x0100)	//�����ƶ�����
// #define ERROR_LOGIN_MOBILE		(0x0200)	//��½�ƶ�ƽ̨����
// #define ERROR_TRANSFER_MOBILE	(0x0400)//�����ƶ����ݴ���
// #define ERROR_DUIZANG_MOBILE	(0x0800)//�ƶ����ʲ�ƽ

#define ERROR_BUS_CONNECT		(0x1000)//���ӹ�������
// #define ERROR_BUS_LOGN			(0x2000)//��½��������
// #define ERROR_BUS_TRANSFER		(0x4000)//�ϴ��������ݴ���

#define Gmode_900C	0
#define Gmode_MG323 1
#define Gmode_MC323 2
#define Gmode_OTHER 3
#define Gmode_WIFI_TLH 4

#define Gmode_MODEM 0x80

//����
#define ALARM_YY			0x01	//������Ӫ
#define ALARM_TC			0x02	//�˳���Ӫ
#define ALARM_TH			0x03	//����ͨ��
#define ALARM_GZ			0x04	//��������
#define ALARM_DQ			0x05	//����
#define ALARM_YJ			0x06	//ӵ��
#define ALARM_SC			0x07	//����
#define ALARM_SG			0x08	//�¹�
#define ALARM_JF			0x09	//����
#define ALARM_XX			0x0A	//��Ϣ
#define ALARM_LY			0x0B	//��������

typedef struct 
{
	unsigned char centry; //BCD��
	unsigned char year;   //BCD��
	unsigned char month;  //BCD��
	unsigned char day;    //BCD��
	unsigned char hour;   //BCD��
	unsigned char min;    //BCD��
	unsigned char sec;    //BCD��
}TimeGPS;
typedef struct tag_GPS_INFO
{
	unsigned int Latitude;		//γ��	[��][��][��С��][��С��]		�������γ,��γ�����λ��1
	unsigned int Longitude;	 //����	[��][��][��][��С��][��С��]	���������,�򾭶����λ��1
	unsigned int velocity;		//�ٶ�
	TimeGPS UTCtime;			//UTC ʱ�� [ʱ][��][��][��С��] BCD��
	unsigned char SataletNum;	//��������
	unsigned int uiCog;		//����
	unsigned char ucValid;		//�����Ƿ���Ч 1=��Ч��0=��Ч
	unsigned char ucState;		//GPSװ̬
}GPS_INFO;
typedef struct
{
	unsigned char EnableGps;			//��ֹGPS��Ϣ 0��ֹ 1����
}stGpsPar;

typedef struct  {
	unsigned short uiVer;		//�汾��
	unsigned int uiGaddr;		//��ʼ��ַ
	unsigned char usNum;		//��������
	unsigned int uiSaddr[50];	//ÿ����������ʼ��ַ
	unsigned char index;        //���ڷŵڼ���
}stSoundInfo;

#define GPRS_SISW 0xE1
#define GPRS_SISR 0xE2
#define GPRS_SISRing 0xE3
typedef struct{
	unsigned char ucSendstyle; //GPRS����״̬ 0xE1=��Ҫ������������ 0xE2=��Ҫ����ȡ��������
	unsigned char ucReadStyle;
	unsigned int uiNum;	//��Ҫ���͵ĳ���
}stGPrsDataStyle;

#define LINEINFO_ROW_LEN	32	//��·��Ϣÿ������
#define STATION_MAX_COUNT	50
#define LINEINFO_HEAD_LEN	10	//��·ͷ����
typedef struct{
	unsigned char ucId;			//վ����
	unsigned int ulLongitudeIN;	//��վ�㾭��
	unsigned int ulLatitudeIN;	//��վ��γ��
	unsigned int ulHxiangIN;	//����
	unsigned int ulLongitude;	//վ�㾭��
	unsigned int ulLatitude;	//վ��γ��
	unsigned int ulHxiang;	//����
	unsigned int ulLongitudeOUT;//վ�㾭��
	unsigned int ulLatitudeOUT;	//վ��γ��
	unsigned int ulHxiangOUT;	//����
	char name[23];				//վ������
	unsigned char Sstyle;		//��վ����0 ��վ��1 ת��
}stStationInfo;					//վ����Ϣ�ṹ 32

#define SPEED_MAX_COUNT 50
typedef struct{
	unsigned char ID;//���
	unsigned char lineNo[3];
	unsigned char ucDirect;		//(��0��1)
	unsigned int ulMaxSpeed;	//�����
	unsigned char ucStationMax;
	unsigned int uiVer;		//��·��Ϣ�汾
	stStationInfo StationInfo[STATION_MAX_COUNT];//һ����·����50��վ����Ϣ
}stLineInfo;
/*
typedef struct
{
	unsigned char old_Purcs_Time[7];
	unsigned char old_Purcs_LineNum[3];
	unsigned char old_Purcs_Device[4];
	unsigned char resear[2];
}oldPursIfo;*/
typedef struct{
	unsigned char ucId;			//���
	unsigned short usRDNo;		//·�α��
	unsigned short usTIme1;		//ʱ��һ
	unsigned short usNSpeed1;	//����һ
	unsigned short usTIme2;		//ʱ�ζ�
	unsigned short usNSpeed2;	//���ٶ�
	unsigned short usTIme3;		//ʱ����
	unsigned short usNSpeed3;	//������
	unsigned int ulLongitude;//վ�㾭��
	unsigned int ulLatitude;	//վ��γ��
	unsigned int ulHxiang;	//����
}stNSpeedInfo_inf;				//���ٵ�ṹ 32

typedef struct{
	unsigned char lineNo[3];
	unsigned char ucDirect;		//(��0��1)
	unsigned char ucStationMax;
	unsigned int uiVer;		//��·��Ϣ�汾
	stNSpeedInfo_inf SpeedPInfo[SPEED_MAX_COUNT];//һ����·ÿ�����򱣴�50�����ٵ���Ϣ
}stNSpeedInfo;

typedef struct{
	unsigned char ucId;			//վ����
	unsigned char ulLongitude[4];	//����
	unsigned char ulLatitude[4];	//γ��
	char name[22];				//վ������
	unsigned char Sstyle;		//��վ����0 ��վ��1 ת��
}stStationInfoT;					//վ����Ϣ�ṹ 32
typedef struct{
	unsigned char ID;//���
	unsigned char lineNo[3];
	unsigned char ucDirect;		//(��0��1)
	unsigned char ulMaxSpeed[4];	//�����
	unsigned char ucStationMax;
	unsigned char uiVer[4];		//��·��Ϣ�汾
	stStationInfoT StationInfo[STATION_MAX_COUNT];//һ����·����50��վ����Ϣ
}stLineInfoT;

typedef struct{
	unsigned char ucCMD;//ָ�����
	unsigned char ZLseiral[2];//ָ����ˮ,���ȱ��
	unsigned char ucZiLine[4];//����·���
	unsigned char ucBUSNo[4];//�������
	unsigned char FCTime[2];//����ʱ��
	unsigned char StartSno;//��ʼվ��
	unsigned char EndSno;//�յ�վ��
	unsigned char Date[4];//�˵��ȵ�����
	unsigned char ucDDstyle;//�Ƿ���Ҫ��ʾ
	unsigned char ucStartYorN;//�Ƿ�ͣ��
	unsigned char ref[3];//���á�
}stDiaoDuCMD;

//����Ӫ����Ϣ����
//��ǰ���١��˴Ρ�
typedef struct{
	unsigned int uiALLLiCeng;	//���
	unsigned int licLON;	//��̾���
	unsigned int licLA;		//���γ��
	unsigned int licJD;		//�ϴ���̽Ƕȣ�����ǶȲ�̫��ͼ������
	unsigned char limitcounter;//����ʱ�����
	unsigned char nearS;//��������ٵ�
	unsigned char REFFF;//����
	unsigned char limitID;	//��ǰ���ٵ�վ���
	unsigned char limitRDNo[2];//����·��
	unsigned char licengTime[7];//�������ʱ��
	unsigned char licCounter;	//��̼��� �Ƶ�5�ۼ�һ��
	unsigned char RenCIUp;		//�˴�
	unsigned char RenCiDown;	//�˴�
	unsigned char overSpeedStart[7];//������ʼʱ��
	unsigned char overSpeedRDNO[2];//����·�α��2B
	unsigned char ONRun;		//�Ƿ�Ӫ�˴���1B
	unsigned char CRC16[2];		//У��
}stDiaoDuinfo;

typedef struct{
	unsigned char alamFlag;//������ʶ
	unsigned char style;//״̬
	unsigned char reSendTimes;//�ط�����
}stAlammessage;

//����ָ��ṹ
typedef struct{
	unsigned int startAddr;
	unsigned int endAddr;
	unsigned char dateTIme[7];
	unsigned char Reserv;
	unsigned int tCrc32;
}stBackRecPointer;
#define _FileNameNum (4096/sizeof(stBackRecPointer))
#endif

