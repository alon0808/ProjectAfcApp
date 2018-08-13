
#ifndef _STPROLIB_HEAD_
#define _STPROLIB_HEAD_

//typedef int time_t;        /* time value */
//��ĸBCD��ת�ַ���
//ZBbuf-����ZBlen�ֽڵ�ѹ������豸��
//strin-����8�ֽ��豸�ţ�����8λǰ�油0
//ZBlen-��Ҫ�����ĳ���,���ܴ���16�ֽ�
//���أ�8������������˵�����Ȳ��ԣ�����ų�����8λ�ַ�
extern unsigned char ZBcod2ASCII(unsigned char *ZBbuf, unsigned char ZBlen, char *strin);
//�ַ�ת��ĸBCD�룺
//strin-����8�ֽ��豸�ţ�����8λǰ�油0
//ZBbuf-����DVlen�ֽڵ�ѹ������豸��
//ZBlen-��Ҫ�����ĳ���,���ܴ���16�ֽ�
//���أ�//���أ���С����Ч�ֽڣ�������ǰ���00��С��ZBlen������������˵�����Ȳ��ԣ�ZBlen�ֽڱ��治����ô����
extern unsigned char ASCII2ZBcod(char *strin, unsigned char *ZBbuf, unsigned char ZBlen);
/*****************************************
Ӧ�ÿ⺯����
��¼д��
DES
******************************************/
//�⺯���б�������
/*************DES*******************/

extern void DES_encrypt(unsigned char *plain_text,unsigned char *key_text,unsigned char *encrypt_text);
extern void DES_decrypt(unsigned char *encrypt_text,unsigned char *key_text,unsigned char *plain_text);
extern void DES3_decrypt(unsigned char *encrypt_text,unsigned char *key_text,unsigned char *plain_text);
extern void DES3_encrypt(unsigned char *plain_text,const unsigned char *key_text,unsigned char *encrypt_text);
extern void CountMac1(unsigned char *Key,unsigned char *random,unsigned char *_data,unsigned int len ,unsigned char *Result);
//��Ҫʱ��Ƚϳ�
//16�ֽ���Կ
/************************************************************************/
/* key : ��Կ 16�ֽ�
   inittext : ��ʼֵ  8�ֽ�
   Source : ����ܵ�����
   le : �������ݵĳ���
   destin : ���ܺ�Ľ�� 4�ֽ�
                                                                     */
/************************************************************************/
extern void CountMac(unsigned char *key,unsigned char *inittext,unsigned char *Source,unsigned int le ,unsigned char *destin);
//8�ֽ���Կ
/************************************************************************/
/* key : ��Կ 8�ֽ�
   inittext : ��ʼֵ 8�ֽ�
   Source : ����ܵ�����
   le : �������ݵĳ���
   destin : ���ܺ�Ľ�� 4�ֽ�
                                                                     */
/************************************************************************/
extern void SCountMac(unsigned char *key,unsigned char *inittext,unsigned char *Source,unsigned int le ,unsigned char *destin);

/***************************************
Key     ��16�ֽ���Կ,����SESLK��3DES����Կ��������Ȧ��ʱ����SESLKʱ��3DES����ԿΪ��Ȧ������Կ��(������˻���Ϣ)��
pInData1���μӼ���SESLK��Դ���ݣ�������Ȧ��ʱpInData1Ϊ  α�������ICC��||���Ӵ�������������Ż����Ǯ�������������||��8000��
pInData2���μӼ���MAC��Դ���ݣ�������Ȧ��ʱpInData2Ϊ  ���Ӵ���������ǰ�����ߵ���Ǯ��������ǰ��||���׽�� || �������ͱ�ʶ || �ն˻���š�
data2len��pInData2�ĳ��ȣ�
initData��8 bytes��0x00 ���飻
mac     ����Ҫ���������MACֵ�����
****************************************/
extern void Mac(unsigned char *Key,unsigned char *pInData1,unsigned char *pInData2,int data2len,unsigned char *initData, unsigned char *mac);
//-----------------------------------------
/**************RecordFile***********************/
extern int getsysfile(unsigned char Bsec, unsigned char *flname);
//Bsec-��ţ�0��31��,�ļ�������ʼ��ַ��ʼ������4K�ռ�.
extern int savesysfile(unsigned char Bsec, unsigned int startAddr, unsigned int endAddr);
//ȡ��ʼ���ڣ��������ڵĵ�ַ(1-31),ֻ��ȡ����������
extern int getHisAddr(unsigned char* sdat, unsigned char* edat, unsigned int *sadd, unsigned int *eadd);
extern void writeBackRec(unsigned char*dat, int len, unsigned char mode);//ȡ��¼ָ��
//������Ҫд���ָ��,��������ͷָ��
//mode == 1 ֻ�����Ƿ���
//extern int Get_Record_point(unsigned char *headp, unsigned char mode);
//�������ʽ��ǰ������
extern void lib_clr_Record_point(void);
extern unsigned char libWriteRecord(unsigned int cFeaddr, unsigned char *buffer, unsigned char mode);
//---------------------------------------------------------
/**************************KeyFile**************/
extern unsigned char lget_MiKey(unsigned char gPSAMin, unsigned char mode, unsigned char *rekey);
//mode=1 ��˳ʹ�� 0 Ĭ������  2 ��ƽ̨
extern unsigned char Lib3DESKey(unsigned char *data, unsigned char *outKey, unsigned char mode);
//��ƽ̨ �����û�����Կ
extern unsigned char Lib3DESCloudKey(unsigned char *data, unsigned char *outKey, unsigned char *desKey);
extern unsigned char LibGetMKey(unsigned char SamSlot, unsigned char *data, unsigned char mode);
//���㽻�׼�¼��MACУ�� ����KEY  ���Ȳ�����256�ֽ�
extern void CountRecMac(unsigned char *Source,unsigned int le ,unsigned char *destin);
//----------------------------------------------------------
/****************CRC**********************/
extern unsigned char CRC8_Tab(unsigned char *ucPtr, unsigned char ucLen);
extern unsigned int crc16_isr(unsigned char *Dat, unsigned int len);
extern unsigned int Getcrc16(unsigned char *r_data,unsigned int length, unsigned char cInhi, unsigned char cInlo);
extern unsigned int GetCrcWord(unsigned char* ptr,int length);
extern unsigned short cal_crc16(unsigned char *ptr, unsigned char len);
extern unsigned int GenerateCRC32(unsigned char *DataBuf,unsigned int len);
extern void crc_cu_16(unsigned char *SerBuffer, unsigned int n,unsigned int length);
extern unsigned char crc_ck_16(unsigned char *SerBuffer,unsigned int length);
extern unsigned char crc_c(unsigned char x,unsigned char temp);
extern unsigned char crc_ck(unsigned char *string_ck,unsigned char length);
extern void crc_cu(unsigned char *string_cu,unsigned char length);
//�Ž���CPU��������Կ
//extern unsigned getJiuJiang_CPUKey(unsigned char *Sdata, unsigned char *Rkey);
//-----------------------------------------------------------
/****************PUBLIC**********************/
extern unsigned char checkIP(char *IPb);
extern unsigned char bcdADD(unsigned char bcd1, unsigned char bcd2);
extern unsigned char bcdDEC(unsigned char bcd1, unsigned char bcd2);
extern unsigned int str2int(unsigned char *pstr, unsigned char len);
extern unsigned int BCD2int(unsigned char *pstr, unsigned char len);

extern void MessageBox(unsigned char ucIsError, char *pDispBuf);
extern void TimeAdd(unsigned char*outBCDTime, unsigned char*bcdTime, unsigned int addSec);

//����ʱ���������λΪ�롣ǰ��ʱ�������ʱ�䡣
extern int DataTimeDec(unsigned char *datatime1, unsigned char *datatime2);

//len-Ҫ�Ƚϵĸ�������uLΪ��λ��  dat-����  ul-ÿ����λ�ĳ��ȣ�intΪ4 shortΪ2��
extern unsigned int BinSCH(unsigned int len, unsigned char *dat, unsigned char *sdat, unsigned char uL);
//����ƽ����
extern unsigned int sqrtint(unsigned int a);
//Ŀ¼
//��ʾĿ¼����
//Ŀ¼����
//̧ͷ��ʾ 
//������
//��ʱʱ�� 50ms��λ
extern int MenuFrame(char menu[][20], void (*function[])(void), char *titlestr, unsigned char ucMENU_NUM, unsigned int dlywaite);//���Է���ҳ��ʾ,
//��ʾ������ʾ������,������ʾ8�У�������ʾ16��,Ĭ��8�У����Ҫ�Ķ���Ҫ���ó������
extern void setCUM_PER_SCREEN(unsigned int cnum);
extern void dispageInfo(unsigned char maxROW, char disbuf[][20]);
//���Է���ҳ��ʾ, ��ʾ���ȼӴ�30�ַ���
extern void dispageInfo_L(unsigned char maxROW, char disbuf[][30]);

extern void end_card(void);
extern unsigned char MifIsPurse(unsigned char *buffer);
extern unsigned char MiBlockInvalid(unsigned char mode,unsigned char count,unsigned char *ptr);
extern unsigned char profile(unsigned char mode);

// typedef struct{
// 	unsigned char LineNo[3];
// 	unsigned char buPrice[4];
// 	unsigned char rate[32];
// 	unsigned char deviceNO[8];
// 	unsigned char IPaddr[4];//IP��ַ POS
// 	unsigned char Port[2];//�˿ں� POS
// 	unsigned char stopflag;//�Ƿ�Ҫ��ͣˢ�� POS
// 	unsigned char ManagePassWord[3];//6λ���� 3 �ֽ� POS
// 	unsigned char reservee[3];		//--------60
// 	unsigned char UserKeyA[16];//����Կ������Կ
// 	unsigned char UserKeyACrc32[4];//kEY CRC
// 	unsigned char UserKeyB[16];//����Կ������Կ
// 	unsigned char UserKeyBCrc32[4];//kEY CRC  -----100
// 	
// 	unsigned char BussinessNo[6];//�̻����2�ֽ�+CRC32 4�ֽ� ��6�ֽ� 
// 	unsigned char ReseverAPN[17];//POS������APN
// 	unsigned char catalogPOST;//Ŀ¼������λ��
// 	unsigned char voice[32];//���ػ��������룬��ֵ������Ʊ��Ϣ��
// 	unsigned char plusMonthPrice[4];//��Ʊ���ӷ�
// 	unsigned char wifi_SSID[10];	//WIFI SSID
// 	unsigned char wifi_KEK[20];		//WIFI KEY
// 	unsigned char wifi_CrC32[4];	//WIFI KEY �� WIFI SSID ��У���� 
// 	unsigned char FDModeFlag;		//�ֶγ��������õı�־�������־=0x5EΪ���Ż�����־Ϊ0xA0Ϊǰ�Ż�����Ҫ��־����Ч��ͨ��Ӳ�����жϡ�	
// 	unsigned char linever[2];		//2017-5-8��·Ʊ�۰汾    ���ں�
// 	unsigned char CSN_BUSVer[2];	//Ʊ�۰汾
// 	unsigned char Reserver[15];//
// 	unsigned char DriverNO[4];
// 	unsigned char Crc32[4];
// }stCpuInfo;//����CPU FLASH�е����ݡ�

typedef struct{
	unsigned char LineNo[3];
	unsigned char buPrice[4];
	unsigned char rate[32];
	unsigned char deviceNO[8];
	unsigned char IPaddr[4];//IP��ַ POS
	unsigned char Port[2];//�˿ں� POS
	unsigned char stopflag;//�Ƿ�Ҫ��ͣˢ�� POS
	unsigned char ManagePassWord[3];//6λ���� 3 �ֽ� POS
	unsigned char PlusSta;			//���ӷ����ñ�־
	unsigned char reservee[2];		//--------60
	unsigned char UserKeyA[16];//����Կ������Կ
	unsigned char UserKeyACrc32[4];//kEY CRC
	unsigned char UserKeyB[16];//����Կ������Կ
	unsigned char UserKeyBCrc32[4];//kEY CRC  -----100
	
	unsigned char BussinessNo[6];//�̻����2�ֽ�+CRC32 4�ֽ� ��6�ֽ� 
	unsigned char disprice[3];	//���ø��ӷ�ʱ�����ʾ��Ʊ�ۣ�ֻ���ڽ�����ʾ��2+xor1
	unsigned char ReseverAPN[14];// û��
	unsigned char catalogPOST;//Ŀ¼������λ��
	unsigned char voice[32];//���ػ��������룬��ֵ������Ʊ��Ϣ��
	unsigned char plusMonthPrice[4];//��Ʊ���ӷ�
	unsigned char wifi_SSID[18];	//WIFI SSID
	unsigned char wifi_KEK[10];		//WIFI KEY
	unsigned char wifi_CrC32[4];	//WIFI KEY �� WIFI SSID ��У���� 
// 	unsigned char  buBalance[4];		//���
// 	unsigned char  buMoney[4];			//�ۿۺ�Ʊ��
	unsigned char linever[2];		//��·Ʊ�۰汾    priceVER 20140731 QUAN
	unsigned char CSN_BUSVer[2];
	unsigned char Time_price[16];
	unsigned char plusPrice[4];//	���ӷ�ʹ��
	unsigned char Crc32[4];
}stCpuInfo;//����CPU FLASH�е����ݡ�
extern unsigned char getbussinessNO(unsigned char *dat);
extern unsigned char getWifiInfo(unsigned char *SSID, unsigned char *KEYE);

extern void wifiTlh_main(void);
#endif
