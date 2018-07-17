#include "Macro_Proj.h"
#include "TypeDef.h"
#include "xSocketClient.h"
#include <string.h>

#if defined(__LINUX__) || defined(__linux)
#include<sys/socket.h>
#include <sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>   //inet_ntoa
#include<unistd.h>      //close
#define XSOCKET_SEND_PARAM4		MSG_NOSIGNAL
#endif

#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
#include "winsock.h"
#pragma comment(lib,"Ws2_32")

#define XSOCKET_SEND_PARAM4		0
#endif


#define WINSOCKET_MAJOR_VERSION		2
#define WINSOCKET_MINOR_VERSION		2


#define QUEUE_LINE  12
#define MAX_DATE_LEN 50000

enum eSocketErrInfo
{
	eSeccessful = 0,
	eCreateSocketErr = 1,
	eBindSocketErr = 2,
	eListenErr = 3,
	eAcceptErr = 4,
	eRecvErr = 5,
	eSendErr = 6,
	eSocketVersionErr = 7,
	eSocketConnect = 8,

};

static TINT32 s_Sock_fd;//文件描述符 (file descriptor) 
//static TINT32 s_Conn_fd;
struct sockaddr_in addr_serv;
struct sockaddr_in addr_client; //本地的地址信息


static TINT32 xSocketInitUnderWin32(void)
{
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
	TUINT16 wVersionRequested;
	WSADATA wsaData;
	TINT32 err;

	wVersionRequested = MAKEWORD(WINSOCKET_MINOR_VERSION, WINSOCKET_MAJOR_VERSION);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return eSocketVersionErr;
	}
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
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
#endif

#if (WHICH_PLATFORM == _LINUX_PLATFORM_V)
    close(connfd);
#endif
	connfd = 0;
}

void SocketClientExist(void)
{
#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
	WSACleanup();
#endif
}


TUINT32 SocketClientInit(char *pIPAdds, TUINT32 nSourcePort, TUINT32 *pSock_fd)
{
	if (xSocketInitUnderWin32() != eSeccessful)
	{
		return eSocketVersionErr;
	}

	s_Sock_fd = -1;

	//创建套接字
	s_Sock_fd = socket(AF_INET, SOCK_STREAM, 0);//IPPROTO_IP,IPPROTO_TCP
	if (s_Sock_fd < 0)
	{
		SocketClientExist();
		return eCreateSocketErr;
	}


	//初始化服务器端地址
	memset(&addr_serv, 0, sizeof(addr_serv));
	addr_serv.sin_family = AF_INET;         //协议族
	addr_serv.sin_port = htons(nSourcePort);  //本地端口号
	addr_serv.sin_addr.s_addr = inet_addr(pIPAdds);

#if (WHICH_PLATFORM == _WIN32_PLATFORM_V)
	if (connect(s_Sock_fd, (struct sockaddr *)&addr_serv, sizeof(struct sockaddr)) == SOCKET_ERROR)
#elif (WHICH_PLATFORM == _LUNIX_PLATFORM_V)
	if (connect(s_Sock_fd, (struct sockaddr *)&addr_serv, sizeof(struct sockaddr)) != 0)
#endif
	{
		SocketClientExist();
		return eSocketConnect;
	}



	*pSock_fd = s_Sock_fd;

	return eSeccessful;
}


TUINT32 SocketClientRecv(TUINT32 conn_fd, TUINT8 *pData, TINT32 *nDataLen)
{
	TINT32 recvNums = -1;


	*nDataLen = 0;
	recvNums = recv(conn_fd, (char*)pData, MAX_DATE_LEN, 0);

	*nDataLen = recvNums;

	if (recvNums <= 0 || recvNums >= MAX_DATE_LEN)
	{
		return eRecvErr;
	}

	return eSeccessful;
}

TUINT32 SocketClientSend(TUINT32 conn_fd, TUINT8 *pData, TUINT32 nDataLen)
{
	TUINT32 sendNums = 0;

	sendNums = send(conn_fd, (char*)pData, nDataLen, XSOCKET_SEND_PARAM4);
	if (sendNums != nDataLen)
	{
		return eSendErr;
	}

	return eSeccessful;
}


void SocketClientClose(TUINT32 connfd)
{
	xSocketClose(connfd);
}

