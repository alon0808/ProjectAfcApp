#include "Macro_Proj.h"
#include "GPRS.h"

#include <stdio.h>  
#include <string.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <arpa/inet.h>  
#include <unistd.h>  
#include <netinet/in.h>  
#include <errno.h>  
#include <stdlib.h>  
	  
#define MAXLINE_R		1024  
//#define SERV_PORT	60000  
	  
int GPRS_main(char *serv_ip, int serv_port, char *sendbuf, int sendlen, char *revbuf, int *revlen)  //(int argc, char *argv[])
{  
//	char receivebuf[MAXLINE],sendbuf[MAXLINE]="helloworld slzr!";	char serv_ip[20] = "139.199.213.63";
	struct sockaddr_in servaddr;  
	int client_sockfd;	
	int rec_len;
	
	system("pppd file etc/ppp/3gdial &");

	sleep(4);
	
	// 判断命令端输入的参数是否正确
	//如:./ 127.0.0.1
	// if( argc != 2)  	 {	printf("usage: ./client <ipaddress>\n"); exit(0);  } 
//printf("socket:\n");
	 // 创建客户端套接字--IPv4协议，面向连接通信，TCP协议
	 if((client_sockfd=socket(AF_INET,SOCK_STREAM,0))<0)  
	  {  
		  perror("socket error!");  
		  exit(0);	
	  }  
	 // 初始化 
	memset(&servaddr,0,sizeof(servaddr)); // 数据初始化-清零
	servaddr.sin_family = AF_INET; // 设置IPv4通信 
	servaddr.sin_port = htons(serv_port);//htons(SERV_PORT) 设置服务器端口号 
//printf("inet_pton:\n");
	
	// IP地址转换函数，将点分十进制转换为二进制 
	 if( inet_pton(AF_INET, serv_ip, &servaddr.sin_addr) <= 0)	
	  {  
//		   printf("inet_pton error for %s\n",argv[1]);	
		   exit(0);  
	  }  
//printf("connect:\n");
	 // 将套接字绑定到服务器的网络地址上
	if( connect(client_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))<0)  
	  {  
		  perror("connected failed");  
		  exit(0);	
	  }  
//printf("send:\n");
	// 循环发送接收数据，send发送数据，recv接收数据
//	while(1)  
	{  
//	 printf("send msg to server: \n");	
	// fgets(sendbuf, 1024, stdin);  
	 // 向服务器端发送数据
	 if( send(client_sockfd, sendbuf, strlen(sendbuf), 0) < 0)	
	  {  
		  printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
		  exit(0);	
	  }  
	 // 接受服务器端传过来的数据 
	 if((rec_len= recv(client_sockfd,revbuf, MAXLINE_R,0)) == -1)  
	  {  
		   perror("recv error");  
		   exit(1);  
	  }  
	 revbuf[rec_len]='\0';	
	 printf("Response from server: %s\n",revbuf);  
	 *revlen = rec_len;
	}  
	// 关闭套接字  
	close(client_sockfd);  
	return 0;  
} 


