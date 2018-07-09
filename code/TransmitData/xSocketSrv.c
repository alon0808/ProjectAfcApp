#include "Macro_Project.h"
#include "TypeDef.h"
#include "xSocketSrv.h"
#include "string.h"
//#include <time.h>
#include <errno.h>
#include <stdio.h>


#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#include "winsock.h"
#pragma comment(lib,"Ws2_32")
#define XSOCKET_SEND_PARAM4		0
typedef int socklen_t;  
typedef int ssize_t;  

#elif (WHICH_PLATFORM == _LUNIX_PLATFORM_V)
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
//#include <netinet/in.h>
#include <arpa/inet.h>   //inet_ntoa
#include <unistd.h>      //close
#include <pthread.h>
#include <sys/select.h>
#define XSOCKET_SEND_PARAM4		MSG_NOSIGNAL
#endif





#define WINSOCKET_MAJOR_VERSION		2
#define WINSOCKET_MINOR_VERSION		2


#define QUEUE_LINE  12
#define MAX_DATA_LEN 50000

typedef struct timeval TTimeVal;

typedef struct sockaddr_in TSockAddr_in;

static TINT32 s_Sock_fd;//文件描述符 (file descriptor)
static TSockAddr_in addr_serv;
static TSockAddr_in addr_client; //本地的地址信息
static fd_set sRfds;  
static TTimeVal sTimeV;
static TTimeVal* spTime = NULL;
//--------------different between WIN32 and Linux---------------------------
static TINT32 xSocketInitUnderWin32(void)
{
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
	UINT16 wVersionRequested;
	WSADATA wsaData;
	TINT32 err;

	wVersionRequested=MAKEWORD(WINSOCKET_MINOR_VERSION, WINSOCKET_MAJOR_VERSION);
	err=WSAStartup(wVersionRequested,&wsaData);
	if (err!=0)
	{
		return eSocketVersionErr;
	}
	if (LOBYTE(wsaData.wVersion)!=2||
		HIBYTE(wsaData.wVersion)!=2)
	{
		WSACleanup();
		return eSocketVersionErr;
	}
#endif
	return eSeccessful;
}

static void xSocketClose(TUINT32 connfd)
{
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
	closesocket(connfd);
#elif (WHICH_PLATFORM == _LUNIX_PLATFORM_V)
	close(connfd);
#endif
}

void SocketSvrExist(void)
{
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
	WSACleanup();
#endif
}

//------------------------------------------

TINT32 SocketSvrInit(TINT8 *pSourceIPAdds, TUINT32 nSourcePort, TUINT32 *pSock_fd)
{

	TUINT32 iMode = 0;
	TINT32 opt = 1;
	//memset(&rfds, 0, sizeof(fd_set));
	//memset(&timeV,0, sizeof(TTimeVal));
	spTime = NULL;

	if (xSocketInitUnderWin32() != eSeccessful)
	{
		return eSocketVersionErr;
	}

	s_Sock_fd = -1;

	//创建套接字
	s_Sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (s_Sock_fd < 0)
	{
		return eCreateSocketErr;
	}
	//设置为阻塞模式
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
	ioctlsocket(s_Sock_fd, FIONBIO, (u_long*)&iMode);
#elif (WHICH_PLATFORM == _LUNIX_PLATFORM_V)
	ioctl(s_Sock_fd, FIONBIO, &iMode);
#endif
	//reuse the port
	// Warning	15	warning C4133: 'function' : incompatible types - from 'TINT32 *' to 'const char *'	d:\智能卡操作系统\PAYPASS\ACR900\代码\ACR321_Paypass\xSocket\xSocketSrv.c	128	TerminalApp
	// this warning is normal, do not use char, it will not take effect when invoke setsockopt
    if( setsockopt(s_Sock_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)(&opt), sizeof(opt)) < 0)
	{
		return eListenErr;
	}
	//初始化服务器端地址
	memset(&addr_serv, 0, sizeof(addr_serv));
	addr_serv.sin_family = AF_INET;         //协议族
	addr_serv.sin_port = htons((u_short)nSourcePort);  //本地端口号
	addr_serv.sin_addr.s_addr = inet_addr(pSourceIPAdds);

	//绑定套接字
	if (bind(s_Sock_fd, (struct sockaddr *)&addr_serv, sizeof(TSockAddr_in) ) < 0)
	{
		return eBindSocketErr;
	}

	//监听
	if (listen(s_Sock_fd, QUEUE_LINE) < 0)
	{
		return eListenErr;
	}

	*pSock_fd = s_Sock_fd;

	return eSeccessful;
}

TINT32 SocketSvrSetRecvTimeout(TUINT32 overtime)
{

	if (overtime <= 0)
	{
		spTime = NULL;
		return Ret_Error;
	}
	else if (overtime == 0xFFFFFFFF)
	{
		spTime = NULL;
	}
	else
	{
		sTimeV.tv_sec = overtime/1000;
		sTimeV.tv_usec = (overtime%1000)*1000;
		spTime = &sTimeV;
	}
	/*
	TINT32 ret;
	TINT32 timeOut = overtime;
	fd_set rfds;  

	struct timeval tv;  
	tv.tv_sec = overtime/1000;
	tv.tv_usec = overtime%1000*1000;

	
	FD_ZERO(&rfds);
	FD_SET(s_Sock_fd,&rfds);

	ret  = setsockopt(s_Sock_fd,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv,sizeof(struct timeval));
	if( ret !=0)
	{
		return eSocketVersionErr;
	}
	*/
	return Ret_OK;
}

TUINT32 SocketSvrAccept(TUINT32 sock_fd, TUINT32 *pConn_fd)
{
	socklen_t client_len = sizeof(TSockAddr_in);
	TINT32 ret = 0;


	if (*pConn_fd != 0)
	{
		SocketSvrSend(*pConn_fd, (UINT8 *)&ret, 2);
		*pConn_fd = 0;
	}

	ret = accept(sock_fd, (struct sockaddr *)&addr_client, &client_len);
	if(ret == -1)
	{
		return eAcceptErr;
	}
	//printf("Client ip: :\n");
	printf("Client ip: %s:%d\n", inet_ntoa(addr_client.sin_addr), (int)addr_client.sin_port);

	*pConn_fd = ret;

	return eSeccessful;
}
TINT32 SocketSvrRecv(TUINT32 conn_fd, TUINT8 *pData, TINT32 nExpDataLen)
{
	TINT32 recvNums = -1;
	TINT32 ret;


	if (conn_fd == 0)
	{
		return eRecvErr;
	}
	FD_ZERO(&sRfds);
	#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
	#pragma warning(disable: 4127)
	#endif
	FD_SET(conn_fd,&sRfds);
	#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
	#pragma warning(default: 4127)
	#endif
	ret = select(conn_fd+1,&sRfds,NULL,NULL,spTime);
	if(ret == -1)
	{
		return eRecvErr;
	}
	else if (ret == 0)
	{//time out
		return eTimeOut;
	}
	else
	{
		if(FD_ISSET(conn_fd,&sRfds))
		{
			while(nExpDataLen > 0)
			{
				recvNums = 0;
				recvNums = recv(conn_fd, (TINT8*)pData, nExpDataLen, 0);
				if(recvNums <= 0 || recvNums > nExpDataLen)
				{
					return eRecvErr;
				}
				/*if (recvNums == nExpDataLen)
				{
					return eSeccessful;
				}*/
				pData = pData + recvNums;
				nExpDataLen -= recvNums;
			}
			return eSeccessful;
		}
	}
	
	return eRecvErr;
}

TINT32 SocketSvrSend(TUINT32 conn_fd,const TUINT8 *pData, TINT32 nDataLen)
{
	TINT32 sendNums = 0;

	if (conn_fd == 0)
	{
		return eSendErr;
	}
	while(nDataLen > 0)
	{
		sendNums = send(conn_fd, (const TINT8*)pData, nDataLen, XSOCKET_SEND_PARAM4);
		if (sendNums <= 0 || sendNums > nDataLen)
		{
			return eSendErr;
		}

		/*if (sendNums == nDataLen)
		{
			break;
		}*/
		pData = pData + sendNums;
		nDataLen -= sendNums;
	}

	return eSeccessful;
}

void SocketSvrClose(TUINT32 connfd)
{
	xSocketClose(connfd);
}


