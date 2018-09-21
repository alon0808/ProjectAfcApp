#ifndef _INCLUDE_EC20LX_HTTP_H
#define _INCLUDE_EC20LX_HTTP_H

//#include "SlzrTypeDef.h"

#define BLKindex_LIST_FILE_NAME WorkDir"BLKindex"
#define BLACK_LIST_FILE_NAME WorkDir"blacklist"
#define BUSPRO_LIST_FILE_NAME WorkDir"buspro"
#define PROindex_LIST_FILE_NAME WorkDir"PROindex"
#define EC20PRO_LIST_FILE_NAME WorkDir"ec20pro"


#define SL8583FileFLAG_PRO	"PRO"	//程序文件按照《三联众瑞POS终端接入规范v2.00.docx》 修订通讯格式 
#define SL8583FileFLAG_BLK "BLK"	//黑名单
#define SL8583FileFLAG_GPS "GPS"	//定位限速信息
#define SL8583FileFLAG_CSN "CSN"	//
#define SL8583FileFLAG_PRI "PRI"	//票价信息版本,暂时没用
#define SL8583FileFLAG_PKI "PKI"	//公钥信息
#define SL8583FileFLAG_WHT	"WHT"	//白名单 包括公交白名单或者银联白名单

#define SL8583FileFLAG_EC20	"EC2"	//Linux 模块的应用程序
#define SL8583FileFLAG_BUS	"BUS"	//Linux 模块的应用程序

#define HTTP_FINISH_DOWN 0
#define HTTP_NEED_DOWN 0x01
#define HTTP_START_DOWN 0x02


typedef struct
{
	int Dtype;	//下载类型
	long hasrecieve;		//已经接收到的文件大小
	long allLen;			//文件总大小
	char FFlag[3];			//文件标识
	char FVer[2];			//文件版本
	char Filename[32];		//文件名
	char HttpAddr[2048];	//url
}stHttpDownInfo;

extern stHttpDownInfo gHttpDinfo;
extern pthread_t tidmain_mainDownload;

#ifdef __cplusplus
extern "C" {
#endif
	extern void* main_HTTPDataDown(void *arg);
#ifdef __cplusplus
}
#endif

#endif

