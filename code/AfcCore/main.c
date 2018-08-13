#include "Macro_Proj.h"
#include "libHandanCore.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <pthread.h>



#include "inputmisc/PsamCard.h"

#include "inputmisc/GPIOCtrl.h"


#include "LightColor.h"
#include "GPRS.h"

#include "DemoMain.h"

#include "inputmisc/IcCardCtrlApi.h"
#include "Main_City.h"
#include "SlzrTypeDef.h"
#include "ICCardLib.h"
#include "PSAMLib.h"

#include "libHandanCore.h"
#include "xSys_Lantaiyuan.h"
#include "GprsSocket.h"

typedef struct
{
	unsigned char packetStatus;			//包的状态
	unsigned short length;				//发送包长度
	unsigned char TTL;					//生存时间
	unsigned short Serial;				//发送序列号
	unsigned char tcpBuffer[250];		//最大可发送7个记录
}TCP_IP_PACKET1;

//
int  CreateDir(const   char   *sPathName)
{
	char   DirName[256];
	strcpy(DirName, sPathName);
	int   i, len = strlen(DirName);
	if (DirName[len - 1] != '/')
		strcat(DirName, "/");

	len = strlen(DirName);

	for (i = 1; i < len; i++)
	{
		if (DirName[i] == '/')
		{
			DirName[i] = 0;
			if (access(DirName, F_OK) != 0)
			{
				if (mkdir(DirName, 0755) == -1)
				{
					perror("mkdir   error");
					return   -1;
				}
			}
			DirName[i] = '/';
		}
	}

	return   0;
}

static stUIData s_UIData;

AFC_CORE__API stUIData* GetStatusData(void) {

	//Get_SerialNum(s_UIData.devId);
	memcpy(s_UIData.devId, gDeviceParaTab.DeviceNo, 8);
	s_UIData.devId[LEN_DEV_ID - 1] = '\0';
	//s_UIData.isDDOk = gGprsinfo.isNetOK[link_DD];
	s_UIData.isGJOk = gGprsinfo.isNetOK[LINK_GJ];
	memcpy(s_UIData.lineId, gDeviceParaTab.LineNo, LEN_LINE_ID);
	//s_UIData.devId = 
	s_UIData.linkStatus = gGprsinfo.GPRSLinkProcess;
	s_UIData.modVer = 0x001;
	s_UIData.task = gGprsinfo.gmissflag;
	s_UIData.uploadRec = 0;
	s_UIData.version = SOFT_VER_TIME_LOG;

	return &s_UIData;
}

//激活网络
extern void *main_NetConnect(void *arg);
extern void* getNetData(void *arg);
extern unsigned char CPUPsamInitJTB(void);
extern void *main_GPRS(void *arg);
extern void *onemsSecondDly(void *arg);
AFC_CORE__API void* StartApp(void *argv)//int argc, 
{
	//	char c;
	SLZR_U8 RcvBuff[512];
	SLZR_U8 buf[5] = { 20,12,0,0 };
	//	struct ev_loop *loop;
	SLZR_U32 RcvBuff_len, u32Ret;


#ifdef KEYBOARD
	pthread_t tidmain_ExKeyBoard;
#endif
	pthread_t tidmain_NetConnect;
	pthread_t tidmain_GPRS;
	pthread_t tidonemsSecondDly;
	pthread_t tidgetNetData0;
	pthread_t tidgetNetData1;
	pthread_t tidgetNetData2;
	pthread_t tidgetNetData3;

	sleep(1);

	printf("-----------main start-------------\n");


	// 	CPsamCard();
	// 	

	printf("hello World\n");

	if (CreateDir(WorkDir) != 0) {	//建立工作目录，如果存在则不建，否则新建
		MessageBox(1, "工作目录建立失败");
		while (1)
		{
			printf("工作目录建立失败\r\n");
			sleep(2);
		}
	}

	Card_QRCodeInit();
	Card_Init();
	R485_Init();

	//	CPsamCard_Init();


	for (u32Ret = 0; u32Ret < 3; u32Ret++) {	//有时候会错，多试几次。
		if (PsamInitialize() == ST_OK)
			break;
		sleep(1);

	}
	CPUPsamInitJTB();

	ICCardInit();	//初始化IC卡消费时的变量

	if (pthread_create(&tidmain_NetConnect, NULL, main_NetConnect, NULL) != 0)
	{
		printf("Create thread main_NetConnect error!\n");
		//exit(1);
	}
	printf("Create thread main_NetConnect, TID: %lu\n", tidmain_NetConnect);

	if (pthread_create(&tidmain_GPRS, NULL, main_GPRS, NULL) != 0)
	{
		printf("Create thread tidmain_GPRS error!\n");
		//exit(1);
	}
	printf("Create thread tidmain_GPRS, TID: %lu\n", tidmain_GPRS);

	if (pthread_create(&tidonemsSecondDly, NULL, onemsSecondDly, NULL) != 0)	//主要是用于计时器.
	{
		printf("Create thread tidonemsSecondDly error!\n");
		//exit(1);
	}
	printf("Create thread tidonemsSecondDly, TID: %lu\n", tidonemsSecondDly);
	if (pthread_create(&tidgetNetData0, NULL, getNetData, (void *)0) != 0)
	{
		printf("Create thread getNetData0 error!\n");
		//exit(1);
	}
	//#else
	if (pthread_create(&tidgetNetData1, NULL, getNetData, (void *)1) != 0)
	{
		printf("Create thread getNetData1 error!\n");
		//exit(1);
	}
	if (pthread_create(&tidgetNetData2, NULL, getNetData, (void *)2) != 0)
	{
		printf("Create thread getNetData2 error!\n");
		//exit(1);
	}
	if (pthread_create(&tidgetNetData3, NULL, getNetData, (void *)3) != 0)
	{
		printf("Create thread getNetData3 error!\n");
		//exit(1);
	}
#ifdef KEYBOARD
	if (pthread_create(&tidmain_ExKeyBoard, NULL, main_ExKeyBoard, (void *)3) != 0)
	{
		printf("Create thread getNetData3 error!\n");
		//exit(1);
	}
#endif

	Light_main(QR_LIGHT, LIGHT_OPEN, QR_W, (char *)buf);

	beep(3, 10, 10);
	while (1) {

		main_card();

		restore_disp();

		usleep(100000);

		// 		if((u32Ret % 10) == 0){
		// 			R485WriteData(RcvBuff, 10);
		// 		}
		// 
		// 		R485ReadData(RcvBuff, &RcvBuff_len);

	}


	u32Ret = 0;
	memset(RcvBuff, '1', sizeof(RcvBuff));
	//	CmdPSAMbps(1);

		//1.运行语音
		//Voice_main(WelCome);


		//去掉看门狗
	system("wtd off");

	//屏幕上方灯打开蓝色

	/**/	Light_main(SCREEN_LIGHT, LIGHT_OPEN, SCREEN_B, (char *)buf);

	Voice_main(Thanks_Again);

	printf("LL:%2X\n", sizeof(TCP_IP_PACKET1));

	Light_main(SCREEN_LIGHT, LIGHT_OPEN, SCREEN_R, (char *)buf);

	//扫码灯打开绿色
	Light_main(QR_LIGHT, LIGHT_OPEN, QR_G, (char *)buf);

	Voice_main(Invaild_Code);

	Light_main(QR_LIGHT, LIGHT_OPEN, QR_W, (char *)buf);

	//关闭屏幕上方灯
	Light_main(SCREEN_LIGHT, LIGHT_CLOSE, SCREEN_R, (char *)buf);

	//后背LED显示
	buf[0] = '2'; buf[1] = '.'; buf[2] = '8'; buf[3] = '0'; buf[4] = 0x00;
	Light_main(LED_LIGHT, 0, 0, (char *)buf);

	Voice_main(WelCome);


	buf[0] = '1'; buf[1] = '.'; buf[2] = '6'; buf[3] = 0x00;
	Light_main(LED_LIGHT, 0, 0, (char *)buf);

	//6.获取按键的键值
	int key, value;
	key = KeyBoard_main(&value);
	printf("Key is %d %s\n", key, (value) ? "Down" : "Up");

	//10.GPRS

	char receivebuf[1000], sendbuf[100] = "helloworld slzr..!";
	char serv_ip[20] = "139.199.213.63";
	int serv_port = 60000, sendlen, revlen;
	sendlen = strlen(sendbuf);

	GPRS_main(serv_ip, serv_port, sendbuf, sendlen, receivebuf, &revlen);

	printf("The ReceiveBuf is %s", receivebuf);
	/**/
		//13.GPS
		//GPS_main();

	CPsamCard_Init();
	u32Ret = CPsamCard_QRCodeInit(RcvBuff, &RcvBuff_len);
	printf("QRCode:");
	if (u32Ret == SLZR_SUCCESS)
	{
		printf("RcvBuff:%s\n", RcvBuff);
	}


	return NULL;

}

/*
void main(void)
{
#ifdef _debug_
	unsigned int i;

	printf("TCP_IP_PACKET1:len:%X\n", sizeof(TCP_IP_PACKET1));

	printf("GPS_INFO:len:%X\n", sizeof(GPS_INFO));

	printf("stDiaoDuinfo:len:%X\n", sizeof(stDiaoDuinfo));

	printf("stStaeInfo:len:%X\n", sizeof(stStaeInfo));

	printf("铁电用量1:%X\n", sizeof(BIT_HISREC_SND+20));

	printf("铁电用量2:%X\n", sizeof(BIT_FE_END_ADDR));
#endif


	CIcCardCtrlApi();
	CPsamCard();

//	CmdPSAMbps(1);


}*/
