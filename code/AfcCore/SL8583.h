#ifndef _sl8583__H_
#define _sl8583__H_

/*flag*/
#define		FIX_LEN		0	/*(LENgth fix 是指由ISO_8583中的长度决定该域的长度)*/
#define		LLVAR_LEN	1	/*(LENgth 00~99)*/
#define		LLLVAR_LEN	2	/*(LENgth 00~999)*/
/*type*/
#define		L_BCD		0	/*左对齐BCD码*/
#define		L_ASC		1	/*左对齐ASC码*/
#define		R_BCD		2	/*右对齐BCD码*/
#define		R_ASC		3	/*右对齐ASC码*/


//域所在的数组位置，注意这里的值比文档域的位序号少1
#define field_dealcode 3
#define field_errode 9
#define field_recAdd 11
#define field_time 12
#define field_date 13
#define field_recupnum 14
#define field_errmsg 38
#define field_ack 39
#define field_cmdcode 40
#define field_posserial 41
#define field_bussinessno 42
#define field_lineno 43
#define field_deviceno 44
#define field_posfilestyle 45
#define field_severfilelist 46
#define field_ackfile 47
#define field_filedata 48
#define field_posparaver 49
#define field_severparaver 50
#define field_POSpara 51
#define field_ackpospara 52
#define field_kek 53
#define field_posinfo 54
#define field_trec 57
#define field_trec_ack 58
#define field_MAC 64


struct  ISO_8583 /* ISO 8583 Message Structure Definitions */
{
    int				len;   /* data element max length */
    unsigned char   type;  /* bit0,bit1暂留 */
	/*bit2:为0时左对齐,为1时右对齐*/
	/* bit3:为0时BCD码,为1时ASC码*/
	/*type:0,1,2,3三种*/
    unsigned char   flag;  /* length field length: 0--FIX_LEN型 1--LLVAR_LEN型 2--LLLVAR_LEN型*/
};

typedef struct 
{
	short bitf;			/*field map if 1 true  0 false*/
    short len;			/*field length*/
    int   dbuf_addr;	/*field data's start address*/
}data_element_flag;


typedef struct  
{
	unsigned char Plen[2];			//报文长度
	unsigned char ID;				//ID 60H
	unsigned char desAddr[2];		//目的地址
	unsigned char sourceAddr[2];	//源地址
	unsigned char appType;		//应用类别
	unsigned char appVer;		//软件版本号
	unsigned char posStatedealType;			//终端状态//处理要求（后4字节）
	unsigned char keyFlagkeySN[2];			//加密标识//密钥索引
	unsigned char pnull;			//保留

	unsigned char MSGtype[2];		//消息类型
	unsigned char MSGbitmap[16];	//位元表
	unsigned char MSGdealCode[3];	//交易处理码

}DEF8583Head;

///签到消息报文
typedef struct 
{
	unsigned char msgType[2];		//消息类型
	unsigned char bitMap[16];		//位元素
	unsigned char dealCmd[3];		//处理码
}DEF8583MSG_6001A;



#define SL8583FileMode_PRO 1	//程序文件
#define SL8583FileMode_BLK 2	//黑名单
#define SL8583FileMode_GPS 3	//定位限速信息
#define SL8583FileMode_PRI 4	//票价信息版本
#define SL8583FileMode_PKI 5	//公钥信息

// #define SL8583FileFLAG_PRO7x "P7X"	//程序文件  POS_Cand_FLAG 代替
// #define SL8583FileFLAG_PRO7S "P7S"	//程序文件
// #define SL8583FileFLAG_PROST "PST"	//程序文件
// #define SL8583FileFLAG_PROET "PET"	//程序文件
#define SL8583FileFLAG_PRO	"PRO"	//程序文件按照《三联众瑞POS终端接入规范v2.00.docx》 修订通讯格式 
#define SL8583FileFLAG_BLK "BLK"	//黑名单
#define SL8583FileFLAG_GPS "GPS"	//定位限速信息
#define SL8583FileFLAG_CSN "CSN"	//
#define SL8583FileFLAG_PRI "PRI"	//票价信息版本,暂时没用
#define SL8583FileFLAG_PKI "PKI"	//公钥信息
#define SL8583FileFLAG_WHT	"WHT"	//白名单 包括公交白名单或者银联白名单
#define SL8583FileFLAG_TPK	"TPK"	//腾讯公钥 腾讯根密钥	腾讯二维码公钥文件

#define SL8583FileFLAG_EC20	"EC2"	//Linux 模块的应用程序

#define WHT_BUS "BUS"
#define WHT_BUS_JTB "JTB"
//#define SL8583FileFLAG_PPP "PPP"	//平台设备参数版本，这个是单独指令，但是处理统一模式。

#define slFileDonwLenPer 800		//下载文件时一个包的大小,黑名单要是10的整数倍
//#define slFileDonwLenPer 1000		//下载文件时一个包的大小,黑名单要是10的整数倍
typedef struct{
	unsigned char Miss_Fileflag[3];	//程序文件标识
	unsigned char Miss_Revert;//
   	unsigned char Miss_FileVer2[8];	//ASCII ，表示为子版本
	
	unsigned char Miss_ver[2];//下载版本号
	unsigned char Miss_REV[2];//保留，为了4字节对齐

	unsigned int Miss_ALL_LEn;//需下载总字节数
	unsigned int Miss_offset;//正在更新的包序号	//V120，改成int，可存黑名单文件偏移量。
	
// 	unsigned char Miss_time;
// 	unsigned char Miss_NULL1[3];//保留，为了4字节对齐
	unsigned int filecrc;		//正在下载的文件检验 用于下载完以后检验文件

	int tmpfilehand;			//临时文件名柄

	unsigned int CRC32;
}st8583filedown;//下载文件结构 

// typedef struct{//签到状态 每次启动都重新签到
// 	unsigned char ISOK;//参数是否有效  此参数需要开机时判断赋值
// 	//------工作密钥 WK密文------------------
// 	unsigned char PIN_KEY[16];//没用
// 	unsigned int crc_pinkey;
// 	unsigned char MAC_KEY[16];//
// 	unsigned int crc_mackey;
// 	//---------------------------------------
// 	unsigned char gcmdcode[100];//访问令牌，LLVAR，最多99个字符,前一个字节是长度
// }stsl8583Style;//签到状态，包括其它任务信息，KEK 程序 参数下载状态，开机需检测

#define sl8583fileNum 7
typedef struct{	//文件列表
	unsigned char filename[4];
	unsigned char fileVer2[8];		//文件子版本
	unsigned int fileVer;
	unsigned int filelen;
	unsigned int crc32;			//后来才加的，怕下载程序出问题

}stsl8583filelist;

typedef struct{
	unsigned char msg_len[2];		//整个域的长度。不含自身
	unsigned char msg_type[2];
	unsigned char msg_id[4];
	unsigned char msg_buff[32];
}stsl8583msg_down;



typedef struct{
	unsigned char msg_len[2];		//buff 的长度
	unsigned char msg_type[2];
	unsigned char msg_id[4];
	unsigned char msg_ack_code[2];
	unsigned char msg_buff[32];
}stsl8583msg_ack;

#endif


