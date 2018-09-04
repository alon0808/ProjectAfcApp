

#ifndef _qppoc_8583__H_
#define _qppoc_8583__H_

#include "SL8583.h"

#define HTTP_HEAD	1

#define SHOP_ID "402641041310003"							//Ĭ���̻���		
#define DEVICE_ID "64630000"								//Ĭ�� �豸��
#define PHONE_NO_DEFAULT "\x00\x00\x02\x22\x22\x22\x22\x22"	//Ĭ�ϵ绰��



//#define		TPDU	"6005010000"	//���Ի����յ�����TPDU
//#define		TPDU	"6000070000"	//��ʽ������TPDU 
//#define		TPDU	"6006010000"	//��ʽ������TPDU  �Ϸ�������,,,��һ��TPDU
//#define		HEAD	"603100311312"
#ifdef PBOC_TEST1
#define		TPDU	"6000870000"	// �Ϻ�����TPDU
#define		HEAD	"613200324605"	
#define		BUSS_ID	"898520154110004"
#else
//#define		TPDU	"6000270000"	// �Ϻ�����TPDU
#define		TPDU	"6005370000"	// �Ϻ�����TPDU
#define		HEAD	"613200324605"	
#endif

#define MISS_PBOC_LOGIN_aut 0x29
#define MISS_PBOC_LOGIN	 0x20	//����ǩ��
#define MISS_PBOC_PURSE	 0x22	//�����ͽ��ױ���
#define MISS_PBOC_RE_PURSE	 0x23	//˫�����
#define MISS_PBOC_UPREC_ODA	 0x24	//ODA�ϴ���¼
#define MISS_PBOC_DOWN_ODA_BLK	 0x25	//ODA���غ�����
#define MISS_PBOC_UPREC_ODA_first	 0x27	//ODA�ϴ���¼
#define MISS_PBOC_UPREC_ca	 0x28	//
extern unsigned char mode_63[128];
extern unsigned int mode_len;
extern unsigned char mode_type;
extern unsigned char mode_para[10];
///ǩ����Ϣ����
typedef struct
{
	unsigned char msgType[2];		//��Ϣ����
	unsigned char bitMap[8];		//λԪ��
//	unsigned char dealCmd[3];		//������
}DEF8583MSG_BIP;


typedef struct {//ǩ��״̬ ���浽�����У��������������
	unsigned char SingInDate[7];//ǩ��ʱ��
	unsigned char ISOK;//�����Ƿ���Ч  �˲�����Ҫ����ʱ�жϸ�ֵ
	//------������Կ WK����------------------
	unsigned char PIN_KEY[16];//
	unsigned char PIN_KEY_CHECKVALUE[4];//
	unsigned char MAC_KEY[16];//
	unsigned char MAC_KEY_CHECKVALUE[4];
	unsigned char TDK_KEY[16];//  ˫�⽻����
	unsigned char TDK_KEY_CHECKVALUE[4];
	//---------------------------------------
//	unsigned char auto_settlement_date[2];			//�Զ�����ʱ��
	unsigned char LoadStyle;//״̬��0λ��KEK�����أ�1λ������������.
	unsigned char resum[15];//����
	unsigned int CrcCheck;//32λCRCУ��
}stMobilStyle;//ǩ��״̬����������������Ϣ��KEK ���� ��������״̬���������� 88�ֽ�

typedef struct {//��������Ϣ���� ������������
	unsigned char BthCount;//=0x55,��Ҫ��������
	unsigned char UpDataTime[7];//��ʼ����ʱ��
	unsigned char posPiCiNo[3];//POS���κ� BCD��
	unsigned char DLushuiNo[3];//��ǰ�Ѿ�������ˮ��
	unsigned char resum1[1];//����
	unsigned char RecordSum;	//��������
	unsigned int MoneySum;		//���ͽ��
	unsigned int CrcCheck;//32λCRCУ��
}stTradeBatchinfo;  //��24�ֽ�

typedef struct {
	unsigned char shopNo[16];//�̻���� ֻ��15�ֽ�
	unsigned char KEK[16];
	unsigned char AUTHKEY[16];  
	unsigned char ip[4];
	unsigned char port[4];
	unsigned char tpdu[5];	//ѹ��BCD	
	unsigned char device[8];// �豸��
	unsigned char functionQr;//����
	unsigned char KEK_1[16];  //����1
	unsigned char KEK_2[16];   //����2
	unsigned char ODA_FLAGE[2];//OK
	unsigned char ODA_NO[9]; 
	unsigned char domain_LEN_1;
	unsigned char domain_name_1[24];
	unsigned char domain_LEN_2;
	unsigned char domain_name_2[24];
	unsigned char domain_name_3[24];
	unsigned char Ip_P_BAK[4];      //��������
	unsigned char Special_IP[4];
	unsigned char Special_PORT[4];
	unsigned char Special_BAK[4];
	unsigned char shuangmian_OVER_TIME;
	unsigned char chongzheng_OVER_TIME;
	unsigned char SOCKET_OVER_TIME;  //��ѯip��ʱʱ��
	unsigned char switch_control_DNS; //
	unsigned char switch_control_SP;
	unsigned char TMS_START_FLAGE;
	unsigned char TMS_KEY_FLAGE;
	unsigned char TMS_FLAGE_FIRST;
	unsigned char domain_APN[18];
	unsigned char reserver[3+20];//����
	unsigned char CrcCheck[4];//32λCRCУ��
}stMobileParameter;//�����·��Ĳ�������ṹ  128     48+8+14+16+16+10=78+24+10=112+16=128 +128 =256
typedef struct
{
	unsigned char Plen[2];			//���ĳ���
	unsigned char ID;				//ID 60H
	unsigned char desAddr[2];		//Ŀ�ĵ�ַ
	unsigned char sourceAddr[2];	//Դ��ַ
	unsigned char appType;		//Ӧ�����
	unsigned char appVer;		//����汾��
	unsigned char posStatedealType;			//�ն�״̬//����Ҫ�󣨵�4λ��
	unsigned char appVer2[3];			//���ܱ�ʶ//��Կ����
// 	unsigned char pnull;			//����

	unsigned char MSGtype[2];		//��Ϣ����
	unsigned char MSGbitmap[8];	//λԪ��

}DEF_qpboc_8583Head;


#define FeRC_Write	1
#define FeRC_Dlelt	2
#define FeRC_Check	3
#define FeRC_READ	4
#define FeRC_Init	5

#define ODA_FeRC_Write	1
#define ODA_FeRC_Dlelt	2
#define ODA_FeRC_Check	3
#define ODA_FeRC_READ	4
#define ODA_FeRC_Init	5

#if 0
//buffΪ�䳤�ľ�Ϊlen+buff ���ݸ���ʽ�������Ѿ�����8583������֯���ˣ���ֵʱ��ֱ�ӿ�����ƴ�Ӽ���
typedef struct
{
	unsigned char flag[2];		//0x55 AA  ��ʾ������Ҫ��
	unsigned char gMCardCand;		//��ά��������п�����Ϊ���͵���һ������Ҫ���
	//ic�����ֶζ�ά��û��
	unsigned char msg2[1 + 11];	//11 1962146020120001439560 //0 Ϊbuff����ĳ��ȣ�LLV BCD ������2���� 19 62146020120001439560 ��ʾ 19�ֽ� �����ǿ���(19+1)/2 �ֽ�bcd
	unsigned char msg23[2];	//��Ƭ���к�	n3		BCD	C	C	C����POS�ܹ���ø�ֵʱ���ڣ�ͬԭʼ����
	unsigned char msg35[1 + 20];	//LLV BCD ���ŵ����� 376214602012000143956D221222000000867000  �����Ƕ��ŵ�(37+1)/2 �ֽ�bcd
	unsigned char msg53[8];
	unsigned char msg55[1 + 200];//IC �������� ��� 255 �ֽ�����LLLVAR�����������
	//��ά����ic��û���ֶ�
	unsigned char msg59[1 + 32];	//��ά����ɨ��ƾ֤  len 26 dat:ASCII:A30196222095947245984168    //dat:1A 0024413330313936323232303935393437323435393834313638
	unsigned char msg57[255];	// 
	//�����ֶ�
	unsigned char msg3[3];	////3�򣺽��״�����000000 Ĭ��
	unsigned char msg4[6];	//4  ���׽��  N12    BCD  M  M  �ն˿���������� 99999999.99
	unsigned char msg11[3];	//11  �ܿ���ϵͳ���ٺ�  n6    BCD  M  M  POS �ն˽�����ˮ
	unsigned char msg22[2];	//��������뷽ʽ�� ͬԭʼ����
	unsigned char msg25[1];//�����������	n2		BCD	M	M	ͬԭ����
	unsigned char msg41[8];//  ans8    ASCII  M  M  �ն˴��� 
	unsigned char msg42[15];	//�̻�
	unsigned char msg48[255];	//48	��ҵ�ض���Ϣ	ans...300	LLLVAR	ASCII	M	M	���48���÷�˵��
	unsigned char msg49[3];	//3 ���Ҵ��� "156"


	unsigned char msg60[1 + 15];//���κ� ��������Ϣan��011	LLLVAR
}st_qpoc_repurse;		//�������� �˸�ʽ���������ͱ���ʱ�������������˸������õ�8583��ʽ���������
#endif

// /*flag*/
// #define		FIX_LEN		0	/*(LENgth fix ��ָ��ISO_8583�еĳ��Ⱦ�������ĳ���)*/
// #define		LLVAR_LEN	1	/*(LENgth 00~99)*/
// #define		LLLVAR_LEN	2	/*(LENgth 00~999)*/
// /*type*/
// #define		L_BCD		0	/*�����BCD��*/
// #define		L_ASC		1	/*�����ASC��*/
// #define		R_BCD		2	/*�Ҷ���BCD��*/
// #define		R_ASC		3	/*�Ҷ���ASC��*/


//�����ڵ�����λ�ã�ע�������ֵ���ĵ����λ�����1



extern data_element_flag msgf[129];	//128����ķֽ����.
extern unsigned char pakege_8583[1024];
extern stMobilStyle Sign_Infor;
extern volatile  unsigned char ACK_flag;	//��̨Ӧ��39���ֵ  bcd
extern unsigned int POS_8583_COUNT;	//�ܿ���ϵͳ���ٺ� ��8583 11��


unsigned char QPBOC_DataDeal(unsigned char *pakege, int packLen);



//����
//PiCiNo:��ǰ���κ�3 + ��ˮ��3
//addRec:��ǰ��Ҫ���ӵ�����
//addMoney: ��ǰ������Ҫ���ӵĽ��
//��������
//-1 : ���κŲ���ȷ
// 0 : �ɹ�
int saveTradeBatchinfo(unsigned char *PiCiNoLS, unsigned int addRec, unsigned int addMoney);

//�����α�����Ϣ
void getTradeBatchinfo(unsigned char *oVal);
//-------���� stMobileParameter--------------
/*
mode=1: shopNo[15];//�̻����
mode=2: posNo[8];//pos�ն˺�
mode=3:char bphoneNo[8];//�󶨵绰BCD
mode=4: KEK_SN;//��Կ����,����δ��
mode=5: KEK[48];//KEK 16�ֽ�
mode=6:ͬʱ����1��2��3
*/
extern void saveMobileParameter(unsigned char mode, const void *pParam);
/*
mode=1: shopNo[15];//�̻����
mode=2: KEK[48];//KEK 16�ֽ�
*/
unsigned char getMobileParameter(unsigned char mode, unsigned char *obuf);
unsigned int Build_http_pack(char *outhttppack, char *ip, unsigned int port, unsigned int Senddatlen);
unsigned int Build_qpboc_8583Packge(unsigned char *revBuf, unsigned char mode);
extern void find_qpboc_new_mission(void);//������һ���һ��


//----ǩ����Ϣ---stMobilStyle--------------------
int saveSingInInfo(void);
//Cmod = 1,����ǩ��״̬��2���ع�����Կ����40�ֽڡ���3��������ֵ
//�������� >=0��������<0,У����ǩ�����ǵ����
int checkSingInInfo(unsigned char *obuf, unsigned char Cmod);
//�����̻���15λASC���ն˺�8λASC���绰��16λBCD
//mode 1 ����tpdu
void set_bussid(unsigned char mode);
void set_bussid_pboc(void);
void set_tpdu(void);
void getbussinessNO_qpboc(unsigned char *buffer);
void load_pboc_8583para(void);
extern int SQDataFromSVT(unsigned char SQmode, int mseconds);
//����Ǯ����ά��
//����Ǯ����ά��
int qpboc_qr_main(char *QRCdat, unsigned char *Rdata);


unsigned int get_deal_count(unsigned int addr);
unsigned int save_deal_count_add(unsigned int addr);

//U��ȡ��Կ��Ϣ������0������<0:ʧ�ܣ�����>0�ɹ�
int usb_load_pboc_infor(void);
//U��ȡ��·��Ϣ������0������<0:ʧ�ܣ�����>0�ɹ�
int usb_loadlineinfor(void);
//����<0:ʧ�ܣ�����>0�ɹ�
int cp_cacert(void);
void set_pboc_menu(void);

void Show_pboc_minu(void);
void dis_qboc_ack_code(unsigned char ack);
void PBOC_hand(unsigned char value);
void down_kek(void);

//mode==1  д�����磬mode==2ɾ�������¼  3 ����Ƿ��м�¼��Ҫת��      �������δ���  �ϵ�û��¼�����
void save_repurse_infor(unsigned char mode, unsigned char *data);
extern int save_ODA_infor(unsigned char mode, unsigned char *re_infor);

unsigned char read_re_infor(unsigned char *out_infor, int *pOlen);
//ST_OK �г�����¼Ҫ��
unsigned char get_repurse_num(void);
//ɾ���������flash��Ӧ�ĳ�����Ϣ
unsigned char del_repurse(unsigned char msg11[3]);
extern unsigned char repurse_infor[512];		//����ʱ����
#define PBOC_FREE_CNT 30
////ˢ��ά��������п�����Ϊһ���������Լ�Ϊ0 ʱ���Լ��ر����е����ӣ�����ʱ�������رպ���open���������Լ��ر�
extern unsigned char pboc_free_cnt;
extern void BLK_63_int_first(void);
extern void BLK_63_int(void);
extern int build8583_qpboc_Purse_0200(unsigned char *oDat);
extern void PAY_MODE_init(void);  //1�ֽڴ��־��1���ֽڴ濪��
extern void online_(unsigned char mode);
extern unsigned char switch_both;
extern void PAY_MODE_init_first(void);
extern void PAY_MODE_SWITCH(unsigned char shuangmian_T, unsigned char switch_both_T) ;
extern  unsigned char shuangmian;
extern unsigned char pay_time;
extern unsigned char s_isAuthOk ;
extern unsigned char s_isDownOdaBlkList;
extern QPBOC_TYPE_63 TYPE_63;
extern unsigned char ODA_time;
extern void SHANGHU_INIT_F(void);
extern void InitQpboc8583(void);

extern unsigned int gSendGLogin_qpoc;
extern  unsigned char deal_type;
extern unsigned char g_supportQR;
extern void EnableCREDIT_CARD(void);
extern void DisableCREDIT_CARD(void) ;
extern void qpoc_init_singe(void);
extern void add_qpoc_flag(void);
extern unsigned char qpoc_flag_or(void);
extern void show_err(void);
extern void set_oda_para(void);
extern void set_dns_para(void);
extern void oda_and_dns_first(void);
extern void set_para_w(void);
extern  void sercel(void);
extern int qpoc_nfc_(unsigned char mode );
extern unsigned char qrDisp_history(unsigned char mode, unsigned int Index);
extern void Q_QPBOC_para_INIT(void);
extern stMobileParameter Q_QPBOC_para;
extern void online_1(void);
extern void online_2(void);
extern void online_3(void);
extern void DisableAliPay(void); 
extern void DisableWeiXin(void);
extern void EnableWeiXin(void) ;
extern void EnableAliPay(void) ;
extern void Enableqpoc(void);
extern void Disableqpoc(void); 
#endif



