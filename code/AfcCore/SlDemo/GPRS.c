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
	
	// �ж����������Ĳ����Ƿ���ȷ
	//��:./ 127.0.0.1
	// if( argc != 2)  	 {	printf("usage: ./client <ipaddress>\n"); exit(0);  } 
//printf("socket:\n");
	 // �����ͻ����׽���--IPv4Э�飬��������ͨ�ţ�TCPЭ��
	 if((client_sockfd=socket(AF_INET,SOCK_STREAM,0))<0)  
	  {  
		  perror("socket error!");  
		  exit(0);	
	  }  
	 // ��ʼ�� 
	memset(&servaddr,0,sizeof(servaddr)); // ���ݳ�ʼ��-����
	servaddr.sin_family = AF_INET; // ����IPv4ͨ�� 
	servaddr.sin_port = htons(serv_port);//htons(SERV_PORT) ���÷������˿ں� 
//printf("inet_pton:\n");
	
	// IP��ַת�������������ʮ����ת��Ϊ������ 
	 if( inet_pton(AF_INET, serv_ip, &servaddr.sin_addr) <= 0)	
	  {  
//		   printf("inet_pton error for %s\n",argv[1]);	
		   exit(0);  
	  }  
//printf("connect:\n");
	 // ���׽��ְ󶨵��������������ַ��
	if( connect(client_sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))<0)  
	  {  
		  perror("connected failed");  
		  exit(0);	
	  }  
//printf("send:\n");
	// ѭ�����ͽ������ݣ�send�������ݣ�recv��������
//	while(1)  
	{  
//	 printf("send msg to server: \n");	
	// fgets(sendbuf, 1024, stdin);  
	 // ��������˷�������
	 if( send(client_sockfd, sendbuf, strlen(sendbuf), 0) < 0)	
	  {  
		  printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
		  exit(0);	
	  }  
	 // ���ܷ������˴����������� 
	 if((rec_len= recv(client_sockfd,revbuf, MAXLINE_R,0)) == -1)  
	  {  
		   perror("recv error");  
		   exit(1);  
	  }  
	 revbuf[rec_len]='\0';	
	 printf("Response from server: %s\n",revbuf);  
	 *revlen = rec_len;
	}  
	// �ر��׽���  
	close(client_sockfd);  
	return 0;  
} 


