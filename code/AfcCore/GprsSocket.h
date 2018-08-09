

#ifndef _HEAD_GPRS_SOCKET_
#define _HEAD_GPRS_SOCKET_


#define LINK_GJ		0
#define LINK_PBOC	1
#define LINK_DD		2

extern stBusVerIfo gBusVerInfo;
extern stGPRSinfo gGprsinfo;
extern int savBusVerInfo(void);
//获取所有的保存的文件版本号
extern int getBusVerInfo(void);
extern void GPRSSocketParaINIT(void);














#endif















