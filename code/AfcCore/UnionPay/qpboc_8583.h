

#ifndef _qppoc_8583__H_
#define _qppoc_8583__H_

#include "SL8583.h"

#define HTTP_HEAD	1

#define SHOP_ID "402641041310003"							//默认商户号		
#define DEVICE_ID "64630000"								//默认 设备号
#define PHONE_NO_DEFAULT "\x00\x00\x02\x22\x22\x22\x22\x22"	//默认电话号



//#define		TPDU	"6005010000"	//测试环境收单公网TPDU
//#define		TPDU	"6000070000"	//正式环境下TPDU 
//#define		TPDU	"6006010000"	//正式环境下TPDU  南方单渠道,,,上一个TPDU
//#define		HEAD	"603100311312"
#ifdef PBOC_TEST1
#define		TPDU	"6000870000"	// 上海银联TPDU
#define		HEAD	"613200324605"	
#define		BUSS_ID	"898520154110004"
#else
//#define		TPDU	"6000270000"	// 上海银联TPDU
#define		TPDU	"6005370000"	// 上海银联TPDU
#define		HEAD	"613200324605"	
#endif

#define MISS_PBOC_LOGIN_aut 0x29
#define MISS_PBOC_LOGIN	 0x20	//银联签到
#define MISS_PBOC_PURSE	 0x22	//银联送交易报文
#define MISS_PBOC_RE_PURSE	 0x23	//双免冲正
#define MISS_PBOC_UPREC_ODA	 0x24	//ODA上传记录
#define MISS_PBOC_DOWN_ODA_BLK	 0x25	//ODA下载黑名单
#define MISS_PBOC_UPREC_ODA_first	 0x27	//ODA上传记录
#define MISS_PBOC_UPREC_ca	 0x28	//
extern unsigned char mode_63[128];
extern unsigned int mode_len;
extern unsigned char mode_type;
extern unsigned char mode_para[10];
///签到消息报文
typedef struct
{
	unsigned char msgType[2];		//消息类型
	unsigned char bitMap[8];		//位元素
//	unsigned char dealCmd[3];		//处理码
}DEF8583MSG_BIP;


typedef struct {//签到状态 保存到铁电中，重启后继续操作
	unsigned char SingInDate[7];//签到时间
	unsigned char ISOK;//参数是否有效  此参数需要开机时判断赋值
	//------工作密钥 WK密文------------------
	unsigned char PIN_KEY[16];//
	unsigned char PIN_KEY_CHECKVALUE[4];//
	unsigned char MAC_KEY[16];//
	unsigned char MAC_KEY_CHECKVALUE[4];
	unsigned char TDK_KEY[16];//  双免交易用
	unsigned char TDK_KEY_CHECKVALUE[4];
	//---------------------------------------
//	unsigned char auto_settlement_date[2];			//自动结算时间
	unsigned char LoadStyle;//状态，0位：KEK需下载，1位：参数需下载.
	unsigned char resum[15];//保留
	unsigned int CrcCheck;//32位CRC校验
}stMobilStyle;//签到状态，包括其它任务信息，KEK 程序 参数下载状态，开机需检测 88字节

typedef struct {//批上送信息保存 保存在铁电中
	unsigned char BthCount;//=0x55,需要进入批结
	unsigned char UpDataTime[7];//开始上送时间
	unsigned char posPiCiNo[3];//POS批次号 BCD码
	unsigned char DLushuiNo[3];//当前已经上送流水号
	unsigned char resum1[1];//保留
	unsigned char RecordSum;	//上送条数
	unsigned int MoneySum;		//上送金额
	unsigned int CrcCheck;//32位CRC校验
}stTradeBatchinfo;  //共24字节

typedef struct {
	unsigned char shopNo[16];//商户编号 只用15字节
	unsigned char KEK[16];
	unsigned char AUTHKEY[16];  
	unsigned char ip[4];
	unsigned char port[4];
	unsigned char tpdu[5];	//压缩BCD	
	unsigned char device[8];// 设备号
	unsigned char functionQr;//保留
	unsigned char KEK_1[16];  //分量1
	unsigned char KEK_2[16];   //分量2
	unsigned char ODA_FLAGE[2];//OK
	unsigned char ODA_NO[9]; 
	unsigned char domain_LEN_1;
	unsigned char domain_name_1[24];
	unsigned char domain_LEN_2;
	unsigned char domain_name_2[24];
	unsigned char domain_name_3[24];
	unsigned char Ip_P_BAK[4];      //公网备用
	unsigned char Special_IP[4];
	unsigned char Special_PORT[4];
	unsigned char Special_BAK[4];
	unsigned char shuangmian_OVER_TIME;
	unsigned char chongzheng_OVER_TIME;
	unsigned char SOCKET_OVER_TIME;  //轮询ip超时时间
	unsigned char switch_control_DNS; //
	unsigned char switch_control_SP;
	unsigned char TMS_START_FLAGE;
	unsigned char TMS_KEY_FLAGE;
	unsigned char TMS_FLAGE_FIRST;
	unsigned char domain_APN[18];
	unsigned char reserver[3+20];//保留
	unsigned char CrcCheck[4];//32位CRC校验
}stMobileParameter;//中心下发的参数缓冲结构  128     48+8+14+16+16+10=78+24+10=112+16=128 +128 =256
typedef struct
{
	unsigned char Plen[2];			//报文长度
	unsigned char ID;				//ID 60H
	unsigned char desAddr[2];		//目的地址
	unsigned char sourceAddr[2];	//源地址
	unsigned char appType;		//应用类别
	unsigned char appVer;		//软件版本号
	unsigned char posStatedealType;			//终端状态//处理要求（低4位）
	unsigned char appVer2[3];			//加密标识//密钥索引
// 	unsigned char pnull;			//保留

	unsigned char MSGtype[2];		//消息类型
	unsigned char MSGbitmap[8];	//位元表

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
//buff为变长的均为len+buff 数据个格式，数据已经按照8583报文组织好了，充值时候直接拷贝，拼接即可
typedef struct
{
	unsigned char flag[2];		//0x55 AA  表示有数据要送
	unsigned char gMCardCand;		//二维码或者银行卡，因为上送的域不一样，需要标记
	//ic卡有字段二维码没有
	unsigned char msg2[1 + 11];	//11 1962146020120001439560 //0 为buff里面的长度，LLV BCD 类似域2域如 19 62146020120001439560 表示 19字节 后面是卡号(19+1)/2 字节bcd
	unsigned char msg23[2];	//卡片序列号	n3		BCD	C	C	C：当POS能够获得该值时存在；同原始交易
	unsigned char msg35[1 + 20];	//LLV BCD 二磁道数据 376214602012000143956D221222000000867000  后面是二磁道(37+1)/2 字节bcd
	unsigned char msg53[8];
	unsigned char msg55[1 + 200];//IC 卡数据域 最大 255 字节数据LLLVAR包含多个子域
	//二维码有ic卡没有字段
	unsigned char msg59[1 + 32];	//二维码用扫描凭证  len 26 dat:ASCII:A30196222095947245984168    //dat:1A 0024413330313936323232303935393437323435393834313638
	unsigned char msg57[255];	// 
	//共有字段
	unsigned char msg3[3];	////3域：交易处理码000000 默认
	unsigned char msg4[6];	//4  交易金额  N12    BCD  M  M  终端可输入最大金额 99999999.99
	unsigned char msg11[3];	//11  受卡方系统跟踪号  n6    BCD  M  M  POS 终端交易流水
	unsigned char msg22[2];	//服务点输入方式码 同原始交易
	unsigned char msg25[1];//服务点条件码	n2		BCD	M	M	同原交易
	unsigned char msg41[8];//  ans8    ASCII  M  M  终端代码 
	unsigned char msg42[15];	//商户
	unsigned char msg48[255];	//48	行业特定信息	ans...300	LLLVAR	ASCII	M	M	详见48域用法说明
	unsigned char msg49[3];	//3 货币代码 "156"


	unsigned char msg60[1 + 15];//批次号 等其他信息an…011	LLLVAR
}st_qpoc_repurse;		//冲正报文 此格式在消费上送报文时候生产，包含了各个域打好的8583格式，方便冲正
#endif

// /*flag*/
// #define		FIX_LEN		0	/*(LENgth fix 是指由ISO_8583中的长度决定该域的长度)*/
// #define		LLVAR_LEN	1	/*(LENgth 00~99)*/
// #define		LLLVAR_LEN	2	/*(LENgth 00~999)*/
// /*type*/
// #define		L_BCD		0	/*左对齐BCD码*/
// #define		L_ASC		1	/*左对齐ASC码*/
// #define		R_BCD		2	/*右对齐BCD码*/
// #define		R_ASC		3	/*右对齐ASC码*/


//域所在的数组位置，注意这里的值比文档域的位序号少1



extern data_element_flag msgf[129];	//128个域的分解情况.
extern unsigned char pakege_8583[1024];
extern stMobilStyle Sign_Infor;
extern volatile  unsigned char ACK_flag;	//后台应答39域的值  bcd
extern unsigned int POS_8583_COUNT;	//受卡方系统跟踪号 在8583 11域


unsigned char QPBOC_DataDeal(unsigned char *pakege, int packLen);



//存入
//PiCiNo:当前批次号3 + 流水号3
//addRec:当前需要增加的条数
//addMoney: 当前批次需要增加的金额
//函数返回
//-1 : 批次号不正确
// 0 : 成功
int saveTradeBatchinfo(unsigned char *PiCiNoLS, unsigned int addRec, unsigned int addMoney);

//读批次保存信息
void getTradeBatchinfo(unsigned char *oVal);
//-------参数 stMobileParameter--------------
/*
mode=1: shopNo[15];//商户编号
mode=2: posNo[8];//pos终端号
mode=3:char bphoneNo[8];//绑定电话BCD
mode=4: KEK_SN;//密钥索引,保留未用
mode=5: KEK[48];//KEK 16字节
mode=6:同时设置1、2、3
*/
extern void saveMobileParameter(unsigned char mode, const void *pParam);
/*
mode=1: shopNo[15];//商户编号
mode=2: KEK[48];//KEK 16字节
*/
unsigned char getMobileParameter(unsigned char mode, unsigned char *obuf);
unsigned int Build_http_pack(char *outhttppack, char *ip, unsigned int port, unsigned int Senddatlen);
unsigned int Build_qpboc_8583Packge(unsigned char *revBuf, unsigned char mode);
extern void find_qpboc_new_mission(void);//此任务一秒进一次


//----签到信息---stMobilStyle--------------------
int saveSingInInfo(void);
//Cmod = 1,返回签到状态，2返回工作密钥【共40字节】，3返回所有值
//函数返回 >=0，正常，<0,校验错或签到不是当天的
int checkSingInInfo(unsigned char *obuf, unsigned char Cmod);
//设置商户号15位ASC、终端号8位ASC、电话号16位BCD
//mode 1 设置tpdu
void set_bussid(unsigned char mode);
void set_bussid_pboc(void);
void set_tpdu(void);
void getbussinessNO_qpboc(unsigned char *buffer);
void load_pboc_8583para(void);
extern int SQDataFromSVT(unsigned char SQmode, int mseconds);
//银联钱包二维码
//银联钱包二维码
int qpboc_qr_main(char *QRCdat, unsigned char *Rdata);


unsigned int get_deal_count(unsigned int addr);
unsigned int save_deal_count_add(unsigned int addr);

//U盘取密钥信息，返回0：返回<0:失败，返回>0成功
int usb_load_pboc_infor(void);
//U盘取线路信息，返回0：返回<0:失败，返回>0成功
int usb_loadlineinfor(void);
//返回<0:失败，返回>0成功
int cp_cacert(void);
void set_pboc_menu(void);

void Show_pboc_minu(void);
void dis_qboc_ack_code(unsigned char ack);
void PBOC_hand(unsigned char value);
void down_kek(void);

//mode==1  写进铁电，mode==2删掉铁电记录  3 检查是否有记录需要转正      解决消费未完成  断电没记录的情况
void save_repurse_infor(unsigned char mode, unsigned char *data);
extern int save_ODA_infor(unsigned char mode, unsigned char *re_infor);

unsigned char read_re_infor(unsigned char *out_infor, int *pOlen);
//ST_OK 有冲正记录要用
unsigned char get_repurse_num(void);
//删除铁电或者flash对应的冲正信息
unsigned char del_repurse(unsigned char msg11[3]);
extern unsigned char repurse_infor[512];		//冲正时候用
#define PBOC_FREE_CNT 30
////刷二维码或者银行卡，置为一个数，再自减为0 时候，自己关闭银行的连接，怕有时候银联关闭后，再open出错，所以自己关闭
extern unsigned char pboc_free_cnt;
extern void BLK_63_int_first(void);
extern void BLK_63_int(void);
extern int build8583_qpboc_Purse_0200(unsigned char *oDat);
extern void PAY_MODE_init(void);  //1字节存标志，1个字节存开关
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



