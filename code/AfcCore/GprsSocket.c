
//Socket 通讯相关

#include "Macro_Proj.h"
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <linux/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <sys/socket.h>

#include "inputmisc/PsamCard.h"

#include "inputmisc/GPIOCtrl.h"


#include "LightColor.h"
#include "GPRS.h"

#include "DemoMain.h"

#include "inputmisc/IcCardCtrlApi.h"

#include "Main_City.h"
#include "SlzrTypeDef.h"
#include "MYDES.h"
#include "GPRSdatatype.h"
#include "ICCardLib.h"
#include "PSAMLib.h"
#include "SL8583.h"
#include "RecordFile.h"
#include "GprsSocket.h"
#include "UtilityProc.h"

#define _debug_gprs



stBusVerIfo gBusVerInfo;
stGPRSinfo gGprsinfo;

unsigned short gSendOverTime;//发送后收应答超时计数器

extern stsl8583filelist gsl8583filelist[sl8583fileNum];
extern st8583filedown gsl8583FileDownPara;

//通信帧结构定义
#define FRAME_SIZE							2048				//一帧数据最大字节长度
typedef struct tag_FRAME_BUF {
	unsigned char ucFrameType;									//帧类型字节
	unsigned char uiFrameLen[2];									//帧长度
	unsigned char FrameBuf[FRAME_SIZE];							//帧缓冲
} FRAME_BUF;

#define MAX_RCV_PACKET_NUM					2					//接收包缓冲大小
//应用包缓冲队列,用于存放数据链路层接收到的数据帧,包括应用层数据和控制帧数据
typedef struct tag_PACKET_BUF {
	FRAME_BUF LinkFrame;										//帧内容
	unsigned char ucItemStatus;									//帧状态标示 
} PACKET_RCV;


int ServerTIMEOUT = 400;				//1分钟没有收到数据重连,超时时间通过信息配置
static time_t ServerGetDataTime[_SOCKET_MAXNUM];	//从网络上接收到的时间延时。如果时间有SEVERTIMEOUT秒，则重新连接SOCKET
volatile int g_socketfd[_SOCKET_MAXNUM];
stNetSendList Netsendline[_SOCKET_MAXNUM];	//需要发送到网络上的数据，数据来源任务传入。
stNetRevList Netrecvline[_SOCKET_MAXNUM];	//收到的网络数据，需要处理

int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

int unblock_connect(const char* ip, int port, int time)
{
	int ret = 0, i;
	struct sockaddr_in address;
	bzero(&address, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons(port);
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int fdopt = setnonblocking(sockfd);
	ret = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
	printf("connect sockfd=%d, ret code = %d:IP=%s:%d\n", sockfd, ret, ip, port);
	if (ret == 0)
	{
		printf("connect with server immediately\n");
		fcntl( sockfd, F_SETFL, fdopt );   //set old optional back
		return sockfd;
	}
	//unblock mode --> connect return immediately! ret = -1 & errno=EINPROGRESS
	else if (errno != EINPROGRESS)
	{
		printf("ret = %d\n", ret);
		printf("unblock connect failed!\n");
		return -1;
	}
	else if (errno == EINPROGRESS)
	{
		printf("unblock mode socket is connecting...\n");
	}
	//use select to check write event, if the socket is writable, then
	//connect is complete successfully!
	fd_set readfds;
	fd_set writefds;
	struct timeval timeout;
	FD_ZERO(&readfds);
	FD_SET(sockfd, &writefds);
	timeout.tv_sec = time; //timeout is 10 minutes
	timeout.tv_usec = 0;
	for (i = 0; i < 5; i++) {
		usleep(10000);
		ret = select(sockfd + 1, NULL, &writefds, NULL, &timeout);
		if (ret <= 0)
		{
			printf("connection time out\n");
			// 			close( sockfd );
			// 			return -1;
			continue;
		}
		if (!FD_ISSET(sockfd, &writefds))
		{
			printf("no events on sockfd found\n");
			// 			close( sockfd );
			// 			return -1;
			continue;
		}
		int error = 0;
		socklen_t length = sizeof(error);
		if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &length) < 0)
		{
			printf("get socket option failed\n");
			// 			close( sockfd );
			// 			return -1;
			continue;
		}
		if (error != 0)
		{
			printf("connection failed after select with the error: %d \n", error);
			// 			close( sockfd );
			// 			return -1;
			continue;
		}
		break;
	}
	//connection successful!
	printf("connection ready after select with the socket: %d \n", sockfd);
	//fcntl( sockfd, F_SETFL, fdopt ); //set old optional back
	return sockfd;
}

//SOCEKT 建立链接
int SocketLink(void)
{
	char servInetAddr[20];// = "119.137.73.90";
	//	struct sockaddr_in sockaddr;
	int i;
	time_t tt;
	unsigned char sendbuffer[50];
	unsigned int len = 0;
	
#ifdef _debug_
	//printf("SocketLink:%d", connected);
#endif

	
	for (i = _SOCKET_MAXNUM; i >= 0; --i) {
		if (gDeviceParaTab.gServerInfo[i].port == 0) {
			//isNetOK[i] = 0;
			continue;
		}
		if (gDeviceParaTab.gServerInfo[i].linkAttr.bits.bit0 != 0)
		{
			time(&tt);
// 			if ((tt - ServerGetDataTime[i]) > (ServerTIMEOUT)) {
// 				https_closeHandle();
// 			}
// #if 0
// 			if (https_getIsConnectOk() == FALSE) {
// 				// COM_sen_start();
// 				https_setServerInfor(gServerInfo + i);
// 				isNetOK[i] = https_OpenSsl(&fd);
// 				g_socketfd[i] = fd;
// 				if (isNetOK[i] == FALSE) {
// 					sleep(1);
// 				}
// 			}
// 			else {
// 				isNetOK[i] = TRUE;
// 			}
// #else
// 			isNetOK[i] = TRUE;
// #endif
		}
		else if (gGprsinfo.isNetOK[i] == FALSE) {

			strcpy(servInetAddr, gDeviceParaTab.gServerInfo[i].IPaddr);
			time(&ServerGetDataTime[i]);	//重新连接了，得置超时计数器
			time(&tt);
			if ((tt - gDeviceParaTab.gServerInfo[i].pcTime) < (CONNECT_DLYTIME)) {	//重新链接，如果第一次链接有问题，需要机延时，此链接没到连接时间
				continue;
			}
			gDeviceParaTab.gServerInfo[i].pcTime = tt;
			g_socketfd[i] = unblock_connect(servInetAddr, gDeviceParaTab.gServerInfo[i].port, 10);
			if (g_socketfd[i] < 0)
			{
				printf("connect error %s errno: %d\n", strerror(errno), errno);
				// sendbuffer[0]=(unsigned char*)&i; 
				memcpy(sendbuffer, (unsigned char*)&i, 4);
				len += 4;
				memcpy(sendbuffer, (unsigned char*)&g_socketfd[i], 4);
				len += 4;
				sleep(1);
			}
			else {
#ifdef _debug_
				printf("connect OK:%d\n", g_socketfd[i]);
#endif
				gGprsinfo.GPRSLinkProcess = TCPSTARTSTAT;
				gGprsinfo.isNetOK[i] = TRUE;
			}
		}
		else {
			time(&tt);
			if ((tt - ServerGetDataTime[i]) > (ServerTIMEOUT)) {
#ifdef _debug_
				printf("connect:%d timeout:%d\n", g_socketfd[i], (int)tt);
#endif
				gGprsinfo.isNetOK[i] = FALSE;
				close(g_socketfd[i]);
			}
		}
	}
	return 0;
}



extern int https_performSsl(unsigned char *pInput, int len, unsigned char *pOutput, int *pOLen);
extern int socket_performNSsl(unsigned char *pInput, int len, unsigned char *pOutput, int *pOLen);
int SendDataTOserver(void)	//发送相关的数据到服务器
{
	int i, ret;
//	int len;
//	unsigned char *obuf = NULL;
	
	for (i = 0; i < _SOCKET_MAXNUM; i++) {
		//printf("Net-Netsendline[%d].dataFlag:%d\n", i, Netsendline[i].dataFlag);
		if (Netsendline[i].dataFlag == 0xAA) {
			
			MSG_LOG("SendDataTOserver-Netsendline[i].dataFlag:%02X\n", Netsendline[i].dataFlag);
			if (gDeviceParaTab.gServerInfo[i].linkAttr.bits.bit0 != 0) {	// tls
#ifdef _debug_
				MSG_LOG("Net-SendData-tls:%d,sID:%d,Len:%d\n", i, g_socketfd[i], Netsendline[i].bufLen);
#endif
	/*			//https_setServerInfor(gDeviceParaTab.gServerInfo + i);
				len = MAXLINE;
				obuf = (unsigned char*)Netrecvline[i].Revbuf;
				if (gDeviceParaTab.gServerInfo[i].linkAttr.bits.bit3 == 0) {
					ret = https_performSsl(Netsendline[i].Sendbuf, Netsendline[i].bufLen, obuf, &len);
				}
				else {
					ret = socket_performNSsl(Netsendline[i].Sendbuf, Netsendline[i].bufLen, obuf, &len);
				}
				if (ret != Ret_Ok)
				{
					SET_INT16(obuf, PECFunction);
					len = 2;
				}
				
				time(&ServerGetDataTime[i]);
				Netrecvline[i].Revbuf[len] = 0;        // null terminate
				Netrecvline[i].bufLen = len;
				Netrecvline[i].dataFlag = 0xAA;
#ifdef _debug_
				MSG_LOG("Time:%d,socket Read:%d len:%d\r\n", ServerGetDataTime[i], i, len);
#endif
				Netsendline[i].dataFlag = 0;
				Netsendline[i].bufLen = 0;*/
				
			}
			else if (gGprsinfo.isNetOK[i] != FALSE) {
#ifdef _debug_
				MSG_LOG("\r\nNet-SendData:%d,sID:%d,Len:%d\n", i, g_socketfd[i], Netsendline[i].bufLen);
#endif
				
				ret = send(g_socketfd[i], Netsendline[i].Sendbuf, Netsendline[i].bufLen, 0);
				//			printf(" ret=%d ", ret);
				if (ret < 0)
				{
#ifdef _debug_
					MSG_LOG("Net-send id%d mes error: %s errno : %d\r\n", i, strerror(errno), errno);	//发送错误 
#endif
//					close(g_socketfd[i]);
//					gGprsinfo.isNetOK[i] = FALSE;
				}
				else {
					Netsendline[i].dataFlag = 0;
					Netsendline[i].bufLen = 0;
#ifdef _debug_
					MSG_LOG("Net-SendData OK ID:%d, len:%d\r\n", i, Netsendline[i].bufLen);
#endif
				}
			}
			else {
#ifdef _debug_
				MSG_LOG("Net-SendData NOS:%d,Len:%d\r\n", i, Netsendline[i].bufLen);
#endif 
				Netsendline[i].dataFlag = 0;
				Netsendline[i].bufLen = 0;
			}
		}
	}
	
	return 0;
}

void* getNetData(void *arg)
{
	//#define fdINDE_ 0
	int n;
	int fdINDE_ = (int)arg;
	boolean f = FALSE;
	printf("getNetData: Time:%d,socket Read:%d\r\n", (int)ServerGetDataTime[fdINDE_], fdINDE_);
	while (1) {
		//usleep(1000);
		//sleep(1);
		if (gDeviceParaTab.gServerInfo[fdINDE_].linkAttr.bits.bit0 == 0 && gGprsinfo.isNetOK[fdINDE_]) {
			//usleep(100);
			f = FALSE;
			n = read(g_socketfd[fdINDE_], Netrecvline[fdINDE_].Revbuf, MAXLINE);
			int sockErr = errno;
			//printf(" Gclient:%d read ret:%d, %d\r\n", fdINDE_, n, sockErr);
			if (n > 0) {
				time(&ServerGetDataTime[fdINDE_]);
				Netrecvline[fdINDE_].Revbuf[n] = 0;        /* null terminate */
				Netrecvline[fdINDE_].bufLen = n;
				Netrecvline[fdINDE_].dataFlag = 0xAA;
#ifdef _debug_
				printf("Time:%d,socket Read:%d len:%d\r\n", (int)ServerGetDataTime[fdINDE_], fdINDE_, n);
#endif
				//debugdata(Netrecvline[fdINDE_].Revbuf, Netrecvline[fdINDE_].bufLen, 1);
			}
			else if ((n == -1) && (sockErr == EBADF)) { //主动关闭的SOCKET
				printf(" Gclient:%d Close1\r\n", fdINDE_);
				f = TRUE;
			}
			else if ((n == -1) && (sockErr == ECONNRESET)) {//主动关闭的SOCKET
				printf(" Gclient:%d Close2\r\n", fdINDE_);
				f = TRUE;
			}
			else if ((n == 0) && (sockErr == EWOULDBLOCK)) {//对于被动关闭的SOCKET,recv返回0，而且errno被置为11
				printf(" Gclient:%d Close3\r\n", fdINDE_);
				f = TRUE;
			}
			else if (!(/*n == -1 && */(sockErr == EWOULDBLOCK || sockErr == 113))) {
				printf(" Gclient:%d Close4\r\n", fdINDE_);
				f = TRUE;
			}
			if (f) {	//
				printf(" Gclient:%d read error:%d, %d\r\n", fdINDE_, n, sockErr);
				gGprsinfo.isNetOK[fdINDE_] = FALSE;	//断开链接，重新连
				close(g_socketfd[fdINDE_]);
			}
		}
		else
			sleep(1);
	}
	pthread_exit((void *)(fdINDE_ + 1));  // Explicitly exit the thread, and return (void *)2
	//#undef fdINDE_
}



//保存文件版本号
int savBusVerInfo(void)
{
	char fullName[100];

	
	int fd=-1, ret=-1;
	
	strcpy(fullName, WorkDir);
	strcat(fullName, _File_BusVer);
	
	fd = open(fullName, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
	if (fd < 0)
	{
		printf("[%s] %s创建 文件误@! fd:%d\r\n", __FUNCTION__, fullName, fd);
		return -1;

	}

	lseek(fd, 0, SEEK_SET);
	ret = write(fd, (unsigned char*)&gBusVerInfo, sizeof(stBusVerIfo));//写入文件
	if(ret < 0){
		printf("[%s] %s写入文件误@!ret:%d\r\n", __FUNCTION__, fullName, ret);
	}
	close(fd);
	return ret;

}


//获取所有的保存的文件版本号
int getBusVerInfo(void)
{
	char fullName[100];
		
	int fd=-1, ret=-1;
	
	strcpy(fullName, WorkDir);
	strcat(fullName, _File_BusVer);
	
	fd = open(fullName, O_RDONLY, S_IRWXG | S_IRWXO | S_IRWXU);
	if (fd < 0)
	{
		printf("[%s] %s打开文件误@! fd:%d\r\n", __FUNCTION__, fullName, fd);
		return -1;
		
	}
	
	ret = read(fd, (unsigned char*)&gBusVerInfo, sizeof(stBusVerIfo));
	if(ret < 0){
		printf("[%s] %s读文件误@!ret:%d\r\n", __FUNCTION__, fullName, ret);
	}
	
	close(fd);

#ifdef _debug_gprs
	printf("[%s] get end:", __FUNCTION__);
	debugdata((unsigned char *)&gBusVerInfo, sizeof(gBusVerInfo), 1);
#endif



	return ret;
}

void GPRSSocketParaINIT(void)
{
	int i ;
	char filename[128];

	getBusVerInfo();

	memcpy(gGprsinfo.MAC_KEY, "sl8583testMACKEY", 16);
	gGprsinfo.crc_mackey = GenerateCRC32(gGprsinfo.MAC_KEY, 16);

	gGprsinfo.ISOK = 0;
	gGprsinfo.GPRSLinkProcess = 0;

	for(i=0;i<_SOCKET_MAXNUM; i++)
		gGprsinfo.isNetOK[i] = 0;
	

	strcpy(filename, WorkDir);
	strcat(filename, _File_FileDownPara);
	GetFileDatac(filename, 0, sizeof(gsl8583FileDownPara), (unsigned char *)&gsl8583FileDownPara); //读出下载的信息用于续传
	
	memset((unsigned char*)&gDeviceParaTab.gServerInfo, 0, sizeof(gDeviceParaTab.gServerInfo));
	strcpy(gDeviceParaTab.gServerInfo[0].APN, "CMNET");
	strcpy(gDeviceParaTab.gServerInfo[0].IPaddr, "139.199.213.63");			////139.199.213.63:2020 测试。
	gDeviceParaTab.gServerInfo[0].port = 2020;

}

int GJRec_Send(void)
{
	unsigned int sum;
	unsigned int curp, headp;
	
	stFAT_hisRec hismsg;
	sum = Check_Send_FAT_Rcord(&hismsg);
	if (sum == 1 )
	{
//		debugstring("找到后台要采集的信息\r\n");
		return 1;//找到后台要采集的信息		
	}
	else{
		curp = Get_Record_point((unsigned char*)&headp, 0);
		//如果记录缓冲中没有记录可发将发送E2PROM中没有发成功的记录
		if(curp>headp)
		{
			sum=(curp - headp)/RECORD_JTB_LEN;
#ifdef _debug_
			debugstring("有记录!\r\n");
#endif		
		}
		else{
			sum = 0;
		}
	}
	return sum;
}


void find_G_new_mission(void)//此任务一秒进一次
{
	unsigned int iver;
//	unsigned short usTemp=0;
	unsigned char i;//, flag;
	unsigned char btf[20];

// 	if(gErrorFlag&ERROR_BUS_CONNECT){
// #ifdef _debug_
// 		sprintf((char*)btf, "公:%u", gErrortimes[1]);
// 		debugstring((char*)btf);
// #endif
// 		if(gErrortimes[1] > 0){
// 			if((gmissflag & MISS_GJ)!=0)
// 				gmissflag = 0;
// 			return;//上次连接错误,时间没到不给任务.
// 		}
// 	}
// 	
// 	gErrorFlag &= (~ERROR_BUS_CONNECT);// 清除错误标识

	if(gGprsinfo.ISOK == 0){//还没有签过到
		if(gGprsinfo.gmissflag == MISS_G_LOGINGJ)
			return;	//已经是签到状态了。
#ifdef _debug_gprs
		debugstring("没有签到！:");
#endif
		gGprsinfo.gmissflag = MISS_G_LOGINGJ;
		if(gGprsinfo.GPRSLinkProcess == GPRS_SENDING_CMD)
			gGprsinfo.GPRSLinkProcess = TCPSTARTSTAT;
		return;
	}


	
	//先找参数文件
	for(i=0; i<sl8583fileNum; i++){
		if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_CSN, 3) == 0 )
			break;
		
	}
//	MSG_LOG("先找CSN文件:i========%d\r\n",i);
	if (i<sl8583fileNum)
	{
		
		if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_CSN, 3) == 0){
			iver = 0;
			MSG_LOG("find:%s\r\n",SL8583FileFLAG_CSN);
			
			memcpy((unsigned char*)&iver, gBusVerInfo.CSN_BUSVer, 2);
			MSG_LOG("find:%s:本地ver:%d,服务器ver:%d\r\n",SL8583FileFLAG_CSN,iver,gsl8583filelist[i].fileVer);
			
			if(gsl8583filelist[i].fileVer == iver){//已经是最新版了
				memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
			}
			else{//不一致，需要下载
				goto load_cs;
			}
		}
		
	}
 

	//先找参数文件
	for(i=0; i<sl8583fileNum; i++){
		if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_PRI, 3) == 0 )
			break;
		
	}
//	MSG_LOG("先找PRI文件i=======%d\r\n",i);
	if (i<sl8583fileNum)
	{
		
		if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_PRI, 3) == 0){//有设备参数下载
			iver = 0;
			
			Ascii2BCD(gsl8583filelist[i].fileVer2,btf,8);
			if (memcmp(btf,gDeviceParaTab.LineNo,2) != 0)
			{
				MSG_LOG("下载线路不一致\r\n");
				memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
				
			}
			
			
			
			memcpy((unsigned char*)&iver, gBusVerInfo.busticketVer, 2);
			MSG_LOG("find:%s:本地ver:%d,服务器ver:%d\r\n",SL8583FileFLAG_PRI,iver,gsl8583filelist[i].fileVer);
			if(gsl8583filelist[i].fileVer == iver){//已经是最新版了
				memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
				MSG_LOG("票价为最新版本\r\n");
			}
			else{//不一致，需要下载
				MSG_LOG("票价需要下载\r\n");
				goto load_cs;
			}
		}
		
	}



	

	//如果已经签到，看看是否有参数要下发 
	for(i=0; i<sl8583fileNum; i++){


// 	 if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_CSN, 3) != 0 && memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_PRI, 3) != 0 ){//有黑名单下载
// 		 continue;
// 	 }


		if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_CSN, 3) == 0){//
			iver = 0;
			MSG_LOG("find:%s\r\n",SL8583FileFLAG_CSN);

			memcpy((unsigned char*)&iver, gBusVerInfo.CSN_BUSVer, 2);
			MSG_LOG("find:%s:本地ver:%d,服务器ver:%d\r\n",SL8583FileFLAG_CSN,iver,gsl8583filelist[i].fileVer);
			
			if(gsl8583filelist[i].fileVer == iver){//已经是最新版了
				memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
			}
			else{//不一致，需要下载
				break;
			}
			
		}
		else if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_PRI, 3) == 0){//有设备参数下载
			iver = 0;

			Ascii2BCD(gsl8583filelist[i].fileVer2,btf,8);
			if (memcmp(btf,gDeviceParaTab.LineNo,2) != 0)
			{
				MSG_LOG("下载线路不一致\r\n");
				memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
				continue;
			}



			memcpy((unsigned char*)&iver, gBusVerInfo.busticketVer, 2);
			MSG_LOG("find:%s:本地ver:%d,服务器ver:%d\r\n",SL8583FileFLAG_PRI,iver,gsl8583filelist[i].fileVer);
			if(gsl8583filelist[i].fileVer == iver){//已经是最新版了
				memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
				MSG_LOG("票价为最新版本\r\n");
			}
			else{//不一致，需要下载
				MSG_LOG("票价需要下载\r\n");
				break;
			}
		
		}
		else if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_BLK, 3) == 0){//有黑名单下载
			iver = 0;

			

			memcpy((unsigned char*)&iver, gBusVerInfo.busBLKVer, 2);
			MSG_LOG("find:%s:本地ver:%d,服务器ver:%d\r\n",SL8583FileFLAG_BLK,iver,gsl8583filelist[i].fileVer);
			if(gsl8583filelist[i].fileVer == iver){//已经是最新版了
				memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
			}
			else{//不一致，需要下载
				break;
			}
		}


		else if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_GPS, 3) == 0){//有定位限速下载
			iver = 0;
			memcpy((unsigned char*)&iver, gBusVerInfo.busLineVer, 2);
			if(gsl8583filelist[i].fileVer == iver){//已经是最新版了
				memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
			}
			else{//不一致，需要下载
				break;
			}
		}
		else if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_PKI, 3) == 0){//有公钥下载
			iver = 0;
			memcpy((unsigned char*)&iver, gBusVerInfo.busVoiceVer, 2);
			if(gsl8583filelist[i].fileVer == iver){//已经是最新版了
				memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
			}
			else{//不一致，需要下载
				break;
			}
		}
		else if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_WHT, 3) == 0){//有白名单下载
			
			MSG_LOG("find:%s\r\n",SL8583FileFLAG_WHT);
			
			if (memcmp(gsl8583filelist[i].fileVer2,WHT_BUS,3) == 0)
			{
				iver = 0;
				memcpy((unsigned char*)&iver, gBusVerInfo.White_Ver, 2);
				MSG_LOG("find:%s:本地ver:%d,服务器ver:%d\r\n",WHT_BUS,iver,gsl8583filelist[i].fileVer);

				if(gsl8583filelist[i].fileVer == iver){//已经是最新版了
					memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
				}
				else{//不一致，需要下载
					break;
				}
			}
			else if (memcmp(gsl8583filelist[i].fileVer2,WHT_BUS_JTB,3) == 0)
			{
				iver = 0;
				memcpy((unsigned char*)&iver, gBusVerInfo.White_Ver_JTB, 2);
				if(gsl8583filelist[i].fileVer == iver){//已经是最新版了
					memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
				}
				else{//不一致，需要下载
					break;
				}
			}
		
		
		}

		else if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_PRO, 3) == 0){//有设备程序下载
//50524F 5053540000000000 0121   PROPST  0121
			if (memcmp(gsl8583filelist[i].fileVer2,POS_Cand_FLAG,3) != 0)
			{
				memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
				continue;
			}

			iver = 0;
			memcpy((unsigned char*)&iver, gBusVerInfo.busProVer, 2);
			MSG_LOG("PROD:%04X,%04X\r\n", iver, gsl8583filelist[i].fileVer);
			if(gsl8583filelist[i].fileVer == iver){//已经是最新版了
				memset(gsl8583filelist[i].filename, 0, 4);//清除平台文件参数
			}
			else{//不一致，需要下载
				break;
			}
		}
	}
load_cs:
	if(i < sl8583fileNum){//找到了有需要下载的任务
		MSG_LOG("有文件任务:%s\r\n", (char*)gsl8583FileDownPara.Miss_Fileflag);
		MSG_LOG("版本:%02X%02X\r\n", gsl8583FileDownPara.Miss_ver[0], gsl8583FileDownPara.Miss_ver[1]);
		MSG_LOG("版本:0x%04X\r\n", gsl8583filelist[i].fileVer);

   

		if((memcmp(gsl8583FileDownPara.Miss_Fileflag, gsl8583filelist[i].filename, 3) == 0)&&
			(memcmp(gsl8583FileDownPara.Miss_ver, (unsigned char*)&gsl8583filelist[i].fileVer, 2) == 0)&&
			(gsl8583FileDownPara.Miss_ALL_LEn == gsl8583filelist[i].filelen) &&
			(gsl8583FileDownPara.filecrc == gsl8583filelist[i].crc32)
			)

		{//如果下在下载的版本和平台的内容一致，则续传
			;
		}
		else{//否则从头开始下载
			memcpy(gsl8583FileDownPara.Miss_Fileflag, gsl8583filelist[i].filename, 3);
			memcpy(gsl8583FileDownPara.Miss_FileVer2, gsl8583filelist[i].fileVer2, 8);
			memcpy(gsl8583FileDownPara.Miss_ver, (unsigned char*)&gsl8583filelist[i].fileVer, 2);
			gsl8583FileDownPara.Miss_ALL_LEn = gsl8583filelist[i].filelen;
	
			if(memcmp(gsl8583filelist[i].filename, SL8583FileFLAG_BLK, 3) == 0)
			{
				MSG_LOG("BLK注意:Miss_offset+=:%d\r\n",shift_blk);
				gsl8583FileDownPara.Miss_offset = shift_blk;		//跳过黑名单.blk文件的里面8字节版本和条数
				
			}else{
				gsl8583FileDownPara.Miss_offset = 0;
				
			}
			
			
			gsl8583FileDownPara.filecrc = gsl8583filelist[i].crc32;

		}

		MSG_LOG("downfile:%s,VER:0x%02X%02X\r\n",gsl8583FileDownPara.Miss_Fileflag,gsl8583FileDownPara.Miss_ver[0],gsl8583FileDownPara.Miss_ver[1]);

		gGprsinfo.gmissflag = MISS_G_FILES;//都使用同一个命令

		return;
	}
	
	//记录发送不再使用铁电。直接从存贮区打包，8583会把发送的记录起始地址和条数上发，后台收到后会发下来，收到回应后再把数据删除掉
	if(GJRec_Send() != 0)//处理GPRS发送数据
		gGprsinfo.gmissflag = MISS_G_UREC;
	else{
		gGprsinfo.gmissflag = MISS_G_FREE;//没有记录要传 无任务
// 		if(((GPRSLinkProcess >= 20)&&(GPRSLinkProcess < 0x30))||(GPRSLinkProcess == 0xA0)){
// 			MSG_LOG("没任务，关掉!\r\n");
// 			tcpipClose(0);
// 			GPRSLinkProcess = GPRS_STANDBY;
// 		}

		if(gGprsinfo.gSendGLogin > SEND_PIN_COUNT_){
			gGprsinfo.gSendGLogin = 0;
//			gsl8583Style.ISOK = 0;	//重签到
			gGprsinfo.gmissflag = MISS_G_TOO;
			gGprsinfo.GPRSLinkProcess =TCPSTARTSTAT;
		}
	}
}

//把数据 放到发送缓冲中，linkNum取值1-4
void gprs_send_data(unsigned char linkNum, unsigned int len, unsigned char *dat)
{
	int si;
	
	if ((linkNum == 0) || (linkNum > 4))//链接号只能从1到4,其它的默认0号链接
		return;

	si = linkNum - 1;
	
	Netsendline[si].bufLen = len;
	memcpy(Netsendline[si].Sendbuf, dat, Netsendline[si].bufLen);
	Netsendline[si].dataFlag = 0xAA;
}
extern unsigned int Buildsl8583Packge(unsigned char *revBuf, unsigned char mode);
void TaskRecWrite(void)
{
	unsigned char buffer[4096];//[2100];
	unsigned int uilen;


	if((gGprsinfo.GPRSLinkProcess != TCPSTARTSTAT)&&//还没连接TCPIP
		(gGprsinfo.GPRSLinkProcess != GPRS_SENDING_CMD)){
		return;
	}

	if(gGprsinfo.isNetOK[LINK_GJ] == 0){//GPRSLinkProcess = 21,但是已经断开了，需要重新连接
		gGprsinfo.GPRSLinkProcess = GPRS_NEED_CLOSEIP;//tcpipClose(0);
		return;
	}


	if(gGprsinfo.GPRSLinkProcess == GPRS_SENDING_CMD){//正在等待上个命令应答
		if(gSendOverTime == 0){//接收应签超时
			close(g_socketfd[LINK_GJ]);
			gGprsinfo.isNetOK[LINK_GJ] = FALSE;
			gGprsinfo.GPRSLinkProcess = GPRS_NEED_CLOSEIP;
		}
		return;
	}

	
	if(gGprsinfo.isNetOK[LINK_GJ] == 0){
		MSG_LOG("没有连接公交:%d\r\n", gGprsinfo.isNetOK[LINK_GJ]);
		return;
	}

	switch(gGprsinfo.gmissflag){
	case MISS_G_TOO:
	case MISS_G_LOGINGJ:
	case MISS_G_SINGOUT:
	case MISS_G_PRICE:
	case MISS_G_DBLKD:
	case MISS_G_DBLKI:
	case MISS_G_UREC:
	case MISS_G_FILES:
	case MISS_G_DLine:
	case MISS_G_DSound:
	case MISS_G_DPROOK:
	case MISS_G_DLineOK:
	case MISS_G_DSoundOK:
	case MISS_G_HART:
	case MISS_G_GPS:
	case MISS_G_ALAM:
	case MISS_G_TREC:
		uilen = Buildsl8583Packge(buffer, gGprsinfo.gmissflag);//BuildGJPackge(buffer, gmissflag);
		if(uilen == 0){
			gGprsinfo.gmissflag = MISS_G_FREE;
			break;
		}

#ifdef _debug_
		debugstring("TaskRecWrite_snd data:");
		debugdata(buffer, uilen, 1);
#endif
		gprs_send_data(1, uilen, buffer);
		if((gGprsinfo.gmissflag == MISS_G_HART)||(gGprsinfo.gmissflag == MISS_G_GPS)){
			gGprsinfo.gmissflag = MISS_G_FREE;
			return;//不需要等应答
		}
		gGprsinfo.GPRSLinkProcess = GPRS_SENDING_CMD;
		gSendOverTime = DE_SendOverTime;
	case MISS_HTTP_BLK:	//通过模块下载
	case MISS_HTTP_EC20:	
	case MISS_HTTP_PRO:
//		uilen = BuildHTTPPackge(buffer, gGprsinfo.gmissflag);
		break;
	case MISS_G_FREE:
		break;
	default:
		gGprsinfo.gmissflag = 0;
		break;
	}
	

	if((gGprsinfo.gmissflag == 0)||(gGprsinfo.gmissflag == MISS_G_FREE)||\
		(gGprsinfo.gmissflag == MISS_G_LOGINGJ)){
		find_G_new_mission();
	}


	return;
}


//每一秒钟进一次
void in_dispGprsProcess(void)
{
	char buff[32];
	
	sprintf(buff, "%02X", gGprsinfo.GPRSLinkProcess);
	miniDispstr(15, 19, buff, DIS_RIGHT|DIS_CONVERT);

	
	sprintf(buff, "%02X", gGprsinfo.gmissflag);
	miniDispstr(14, 19, buff, DIS_RIGHT|DIS_CONVERT);

}

extern unsigned char GJDataDeal(unsigned char *pakege);
void mainGprs(void)
{
	unsigned char ret;
	
	if(Netrecvline[0].dataFlag == 0xAA) 
	{
		//应用层数据帧
#ifdef	_debug_gprs 
		debugstring("TaskGprsRev-------:\r\n");
		debugdata(Netrecvline[0].Revbuf, Netrecvline[0].bufLen, 1);
#endif

		ret = GJDataDeal((unsigned char*)&Netrecvline[0].dataFlag);//公交数据处理			
		if(ret == 0){
			memset((unsigned char*)&Netrecvline[0].dataFlag,0,50);
		}
		else{
			Netrecvline[0].dataFlag = 0xAA;
#ifdef _debug_gprs
			debugstring("MAIN:");
			debugdata((unsigned char*)&Netrecvline[0].dataFlag, 20, 1);
#endif
		}
	}
}

//主要是用到1秒钟计数功能,相关计数器如果不是0就会减1.
void main_Onesec(void)
{
	unsigned int t;
//	static unsigned int ledflag=0;
	
	t = get_timer0(0);

	if (t == 0) {
		set_timer0(1000, 0);//1s钟进一次

#ifdef _debug_gprs
		printf("[%s]-%d\r\n", __FUNCTION__, gSendOverTime);
#endif
		in_dispGprsProcess();	//主要显示状态GPRS，1秒更新显示一下状态
		

		if(gSendOverTime > 0){
			gSendOverTime--;
		}
		
		gGprsinfo.gSendGLogin++;	//发送心跳计数

		if(gBuInfo.g24GDisFlash != 0){
			gBuInfo.g24GDisFlash--;
			if(gBuInfo.g24GDisFlash == 0){
				gBuInfo.restore_flag = 1;	//延迟刷新界面
			}
		}
		
		DisRetry();
// 		if(ledflag == 0){
// 			Light_main(QR_LIGHT, LIGHT_OPEN, QR_G, (char *)&t);
// 		}
// 		else if(ledflag == 1){
// 			Light_main(QR_LIGHT, LIGHT_OPEN, QR_B, (char *)&t);
// 		}
// 		else if(ledflag == 2){
// 			Light_main(QR_LIGHT, LIGHT_OPEN, QR_R, (char *)&t);
// 		}
// 		else{
// 			Light_main(QR_LIGHT, LIGHT_OPEN, QR_W, (char *)&t);
// 		}
// 		ledflag++;
// 		if(ledflag > 3)
// 			ledflag = 0;

//		in_sendGPs();//不为0，已经发送一条GPS信息
		
	}

}



//激活网络
void *main_NetConnect(void *arg)
{
	
	unsigned int t=0;
	
	sleep(1);
	
	//先卸载
	system("rmmod /mnt/app/ltyapp/ko/option.ko");  
				
	//加载驱动
	system("insmod /mnt/app/ltyapp/ko/option.ko");
	
	sleep(5);
	
	
	while (1) {
		
		if(gGprsinfo.isNetOK[LINK_GJ] == 0){
			if((t++ % 100) == 0){
				system("pppd file /mnt/app/ltyapp/etc/ppp/3gdial");			//开始执行会出错，如果网络不成功每10秒执行一次。测试时如果网络成功，执行这个也没有影响
				
				printf("pppd file /mnt/app/ltyapp/etc/ppp/3gdial\r\n");
			}
		}

		usleep(100000);		//pppd运行成功后，后面的代码就运行不到了。。所以下面没有加其它任务了
		

	}
	
	printf("[thread]:main_NetConnect end\n");
	
	return (void *)1;
}





void *main_GPRS(void *arg)
{	
	unsigned int t=0;
	
	Init_3Gmode();
	
	while(1){
		usleep(10000);//10ms

		main_Onesec();

		if ((gGprsinfo.gmissflag == 0) || (gGprsinfo.gmissflag == MISS_G_FREE) || (gGprsinfo.gmissflag == MISS_G_LOGINGJ)) {//查找GPRS的任务

			find_G_new_mission();
		}
		
		if((t++ % 100)==0)
			SocketLink();

		TaskRecWrite();//处理要发送的数据

		mainGprs();//处理缓冲接收到的中的GPRS数据
		
		SendDataTOserver();	//发送相关的数据到服务器
	}
	return (void *)0;
}





