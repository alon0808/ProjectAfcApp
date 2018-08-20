

#ifndef _HEAD_GPRS_SOCKET_
#define _HEAD_GPRS_SOCKET_


#define LINK_GJ			0
#define LINK_PBOC		1
#define LINK_PBOC_TMS	2		//ssl
#define LINK_DD			2

extern stBusVerIfo gBusVerInfo;
extern stGPRSinfo gGprsinfo;

#ifdef __cplusplus
extern "C" {
#endif

	extern int savBusVerInfo(void);
	//获取所有的保存的文件版本号
	extern int getBusVerInfo(void);
	extern void GPRSSocketParaINIT(void);
	extern int unblock_connect(const char* ip, int port, int time);

	extern void *main_GPRS(void *arg);

#ifdef __cplusplus
}
#endif

#endif

