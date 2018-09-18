#include "Macro_Proj.h"
#include "EC20Lx_HTTP.h"
#include <stdio.h>//printf
#include <string.h>//�ַ�������
#include <sys/socket.h>//�׽���
#include <arpa/inet.h>//ip��ַ����
#include <fcntl.h>//openϵͳ����
#include <unistd.h>//writeϵͳ����
#include <netdb.h>//��ѯDNS
#include <stdlib.h>//exit����
#include <dirent.h>
#include <sys/stat.h>//statϵͳ���û�ȡ�ļ���С
#include <sys/time.h>//��ȡ����ʱ��
#include <errno.h>
#include <pthread.h>
#include <errno.h>

#include "GprsSocket.h"
#include "szct.h"


#ifndef FALSE
#define FALSE	0
#endif
#ifndef TRUE
#define TRUE	1
#endif


#define DownDir	"/mnt/appbak/"

struct HTTP_RES_HEADER//������Ӧͷ��Ϣ
{
	int status_code;//HTTP/1.1 '200' OK
	char content_type[128];//Content-Type: application/gzip
	long content_length;//Content-Length: 11683079
};

typedef struct
{
	int Dtype;	//��������
	long hasrecieve;		//�Ѿ����յ����ļ���С
	long allLen;			//�ļ��ܴ�С
	char FFlag[3];			//�ļ���ʶ
	char FVer[2];			//�ļ��汾
	char Filename[32];		//�ļ���
	char HttpAddr[2048];	//url
}stHttpDownInfo;


typedef struct
{
	unsigned char dflag;		//�����ֵΪEA����Ҫ�Ѱ汾��Ϣ���͸�����
	unsigned char busProName[16];//����������������汾
	unsigned char CSN_BUSName[16];//Ʊ�۰汾
	unsigned char busLineName[16];//վ��������Ϣ�汾2B
	unsigned char busVoiceName[16];//��Կ��Ϣ�汾2B   ����У���Ҫת��16������

	unsigned char White_Name[16];//��������Ϣ�汾  ��λ�ں�
	unsigned char White_JTB_Name[16];//��������Ϣ�汾  ��λ�ں�
	unsigned char BLKName[16];//�������汾
	unsigned char LineName[16];//��·��Ϣ�汾
	unsigned char EC20ProName[16];//����������������汾
}stEc20VerInfo;

stHttpDownInfo gHttpDinfo;
stEc20VerInfo gEc20VerInfo;	//


							//�����ļ���ʶ��ȡ���ļ���
void getFilesName(char *FFlag, char *obuf)
{
	if (memcmp(FFlag, SL8583FileFLAG_BLK, 3) == 0)
		strcpy(obuf, gEc20VerInfo.BLKName);
	else if (memcmp(FFlag, SL8583FileFLAG_PRO, 3) == 0)
		strcpy(obuf, gEc20VerInfo.busProName);
	else if (memcmp(FFlag, SL8583FileFLAG_GPS, 3) == 0)
		strcpy(obuf, gEc20VerInfo.busLineName);
	else if (memcmp(FFlag, SL8583FileFLAG_CSN, 3) == 0)
		strcpy(obuf, gEc20VerInfo.CSN_BUSName);
	else if (memcmp(FFlag, SL8583FileFLAG_WHT, 3) == 0)
		strcpy(obuf, gEc20VerInfo.White_JTB_Name);
	else if (memcmp(FFlag, SL8583FileFLAG_EC20, 3) == 0)
		strcpy(obuf, gEc20VerInfo.EC20ProName);
	else
		obuf[0] = 0;

}

unsigned long FindIndexFiles(char *fullName, char *obuf)
{
	int fd, ret;
	int l = 0;

#ifdef _debug_
	printf("Black index FileName:%s\r\n", fullName);
#endif

	if (access(fullName, F_OK) != -1) {
		fd = open(fullName, O_RDONLY);
		if (fd >= 0) {
			ret = read(fd, obuf, 16);
			if (ret >= 0) {
				printf("read file %s data%s:\n", fullName, obuf);
				l = 99;
			}
			else {
				printf("read %s fail!\n", fullName);
				perror("read");
			}
		}
		else
		{
			printf("open %s fail!\n", fullName);
			perror("open");
		}

	}
	// 	else{
	// 		printf("NO file:%s??\r\n", fullName);
	// 	}
	return l;
}
void getEc20FilesVer(void)
{
	unsigned long len = 0;
	char fileName[256];

	memset((unsigned char*)&gEc20VerInfo, 0, sizeof(gEc20VerInfo));

	//���Һ������ļ���Ϣ
	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, DownDir);
	strcat(fileName, "/BLKindex");
	FindIndexFiles(fileName, gEc20VerInfo.BLKName);

	//���ҳ��ػ�����
	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, DownDir);
	strcat(fileName, "/PROindex");
	len = FindIndexFiles(fileName, gEc20VerInfo.busProName);

	//����EC20����
	memset(fileName, 0, sizeof(fileName));
	strcpy(fileName, DownDir);
	strcat(fileName, "/EC2index");
	len = FindIndexFiles(fileName, gEc20VerInfo.EC20ProName);

	gEc20VerInfo.dflag = 0xEA;
}


void parse_url(const char *url, char *host, int *port, char *file_name)
{
	/*ͨ��url����������, �˿�, �Լ��ļ���*/
	int j = 0;
	int start = 0;
	*port = 80;
	char *patterns[] = { "http://", "https://", NULL };
	int i;
	for (i = 0; patterns[i]; i++)//�������ص�ַ�е�httpЭ��
		if (strncmp(url, patterns[i], strlen(patterns[i])) == 0)
			start = strlen(patterns[i]);

	//��������, ���ﴦ��ʱ��������Ķ˿ںŻᱣ��
	for (i = start; url[i] != '/' && url[i] != '\0'; i++, j++)
		host[j] = url[i];
	host[j] = '\0';

	//�����˿ں�, ���û��, ��ô���ö˿�Ϊ80
	char *pos = strstr(host, ":");
	if (pos)
		sscanf(pos, ":%d", port);

	//ɾ�������˿ں�
	for (i = 0; i < (int)strlen(host); i++)
	{
		if (host[i] == ':')
		{
			host[i] = '\0';
			break;
		}
	}

	//��ȡ�����ļ���
	j = 0;
	for (i = start; url[i] != '\0'; i++)
	{
		if (url[i] == '/')
		{
			if (i != strlen(url) - 1)
				j = 0;
			continue;
		}
		else
			file_name[j++] = url[i];
	}
	file_name[j] = '\0';
}

struct HTTP_RES_HEADER parse_header(const char *response)
{
	/*��ȡ��Ӧͷ����Ϣ*/
	struct HTTP_RES_HEADER resp;

	char *pos = strstr(response, "HTTP/");
	if (pos)//��ȡ���ش���
		sscanf(pos, "%*s %d", &resp.status_code);

	pos = strstr(response, "Content-Type:");
	if (pos)//��ȡ�����ĵ�����
		sscanf(pos, "%*s %s", resp.content_type);

	pos = strstr(response, "Content-Length:");
	if (pos)//��ȡ�����ĵ�����
		sscanf(pos, "%*s %ld", &resp.content_length);

	return resp;
}

void get_ip_addr(char *host_name, char *ip_addr)
{
	/*ͨ�������õ���Ӧ��ip��ַ*/
	struct hostent *host = gethostbyname(host_name);//�˺����������DNS������
	if (!host)
	{
		ip_addr = NULL;
		return;
	}
	int i;
	for (i = 0; host->h_addr_list[i]; i++)
	{
		strcpy(ip_addr, inet_ntoa(*(struct in_addr*) host->h_addr_list[i]));
		break;
	}
}


void progress_bar(long cur_size, long total_size, double speed)
{
	/*������ʾ���ؽ�����*/
	float percent = (float)cur_size / total_size;
	const int numTotal = 50;
	int numShow = (int)(numTotal * percent);

	if (numShow == 0)
		numShow = 1;

	if (numShow > numTotal)
		numShow = numTotal;

	char sign[51] = { 0 };
	memset(sign, '=', numTotal);

	printf("\r%.2f%%[%-*.*s] %.2f/%.2fMB %4.0fkb/s", percent * 100, numTotal, numShow, sign, cur_size / 1024.0 / 1024.0, total_size / 1024.0 / 1024.0, speed);
	fflush(stdout);


	if (numShow == numTotal)
		printf("\n");
}

unsigned long get_file_size(const char *filename)
{
	//ͨ��ϵͳ����ֱ�ӵõ��ļ��Ĵ�С
	struct stat buf;
	if (stat(filename, &buf) < 0)
		return 0;
	return (unsigned long)buf.st_size;
}

//extern void com_send_data_OTHER_CMD(unsigned char socketindex, unsigned char cmd, unsigned int len, unsigned char *dat);
void download(int client_socket, char *file_name, long content_length, char *shortFilename)
{
	/*�����ļ�����*/
	long hasrecieve = 0;//��¼�Ѿ����صĳ���
	struct timeval t_start, t_end;//��¼һ�ζ�ȡ��ʱ�������յ�, �����ٶ�
	int mem_size = 8192;//��������С8K
	int buf_len = mem_size;//����״̬ÿ�ζ�ȡ8K��С���ֽ���
	int len;

	//�����ļ�������
	int fd = open(file_name, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);
	if (fd < 0)
	{
		printf("�ļ�����ʧ��!\n");
		return;
	}

	char *buf = (char *)malloc(mem_size * sizeof(char));

	//���׽������ж�ȡ�ļ���
	long diff = 0;
	int prelen = 0;
	double speed;

	int pos = 0;
	unsigned char sndbuf[128];

	while (hasrecieve < content_length)
	{
		gettimeofday(&t_start, NULL); //��ȡ��ʼʱ��
		len = read(client_socket, buf, buf_len);
		write(fd, buf, len);
		gettimeofday(&t_end, NULL); //��ȡ����ʱ��

		hasrecieve += len;//�����Ѿ����صĳ���

						  //�����ٶ�
		if (t_end.tv_usec - t_start.tv_usec >= 0 && t_end.tv_sec - t_start.tv_sec >= 0)
			diff += 1000000 * (t_end.tv_sec - t_start.tv_sec) + (t_end.tv_usec - t_start.tv_usec);//us

		if (diff >= 1000000)//��һ��ʱ��δ���1s=1000000usʱ, ����һ���ٶ�
		{
			speed = (double)(hasrecieve - prelen) / (double)diff * (1000000.0 / 1024.0);
			prelen = hasrecieve;//����������
			diff = 0;//����ʱ��γ���

			memset(sndbuf, 0, sizeof(sndbuf));	//���ͽ��ȵ�����,�ٶ�̫��ʱ��С��1��������������������
			sndbuf[pos++] = 2;
			strcpy((char *)sndbuf + pos, shortFilename);
			pos += 20;
			memcpy(sndbuf + pos, (unsigned char*)&content_length, 4);
			pos += 4;
			memcpy(sndbuf + pos, (unsigned char*)&hasrecieve, 4);
			pos += 4;
			//com_send_data_OTHER_CMD(0, 0x09, pos, sndbuf);//CMD��09��������Ӧ�����ҪӦ����Ҫ��29
		}



		//progress_bar(hasrecieve, content_length, speed);

		if (hasrecieve == content_length) {
			memset(sndbuf, 0, sizeof(sndbuf));	//���ͽ��ȵ�����,����ˣ�Ҫ����
			sndbuf[pos++] = 2;
			strcpy((char *)sndbuf + pos, shortFilename);
			pos += 20;
			memcpy(sndbuf + pos, (unsigned char*)&content_length, 4);
			pos += 4;
			memcpy(sndbuf + pos, (unsigned char*)&hasrecieve, 4);
			pos += 4;
			//com_send_data_OTHER_CMD(0, 0x09, pos, sndbuf);//CMD��09��������Ӧ�����ҪӦ����Ҫ��29
			break;
		}
	}
}

void CheckDownloadFile(const char * file_name, char *findName)
{
	int mem_size = 8192;//��������С8K
	int buf_len = mem_size;//����״̬ÿ�ζ�ȡ8K��С���ֽ���
	int len;
	int i, t;
	char name[100], name2[100];
	char newName[64];//���µĺ������ļ�
	int newVer = 0;
	char strtokbuffer[100];
	char *token, *token2;
	char *buf = (char *)malloc(mem_size * sizeof(char));


	memset(newName, 0, sizeof(newName));

	printf("����ļ�%s\n", file_name);
	int fd = open(file_name, O_CREAT | O_RDONLY, S_IRWXG | S_IRWXO | S_IRWXU);
	if (fd < 0)
	{
		printf("�ļ�����ʧ��!\n");
		return;
	}
	len = read(fd, buf, buf_len);
	if (len < 0)
	{
		printf("��ȡ�ļ�ʧ��\r\n");
		return;
	}
	//	 for(i=0;i<len;i++)
	//	 {
	//		printf("%X",buf[i]);
	//	 }
	printf("\n���ص�����%d->\n%s\r\n***************************\r\n", len, buf);

	//	token=strtok(buf,"\n");
	//	while(token!=NULL)
	//	{
	//		printf("%s\n",token);
	//		token=strtok(NULL,"\n");
	//	}

	i = 0;
	token = strstr(buf, "a href=");
	while (token != NULL)
	{
		//	token2=strstr(token,"</a>");
		token2 = strstr(token, "\"");
		token2 += 1;
		token = strstr(token2, "\"");
		memcpy(name, token2, (token - token2));

		printf("firs name:%s\n", name);
		memset(strtokbuffer, 0, 100);
		token2 = strstr(token, "</a>");
		token += 1;
		memcpy(name2, token + 1, (token2 - token - 1));
		printf("last name:%s\n", strtokbuffer);

		if (strncmp(name, name2, strlen(name)) == 0)
		{
			printf("������������������-��:%s\n", name);
			if ((memcmp(name, "BLK_", 4) == 0) && (strlen(name) >= 8)) {//�������ļ�
				memset(name2, 0, sizeof(name2));
				memcpy(name2, name + strlen(name) - 4, 4);	//ȡ�汾��,ֻȡ���4�ֽ�
				t = atol(name2);
				printf("Blk Ver=%d\r\n", t);
				if (t > newVer) {//�ҵ����µİ汾
					newVer = t;
					strcpy(newName, name);
				}
			}
		}
		else
		{
			printf("������������:%s->%s\n", name, name2);
		}
		memset(name, 0, 100);
		memset(name2, 0, 100);
		memset(strtokbuffer, 0, 100);
		token += i;
		token = strstr(token2, "a href=");
	}

	printf("Find new ver File:%s\r\n", newName);
	strcpy(findName, newName);//�������µİ汾�ļ�

}
#define MODE (S_IRWXU | S_IRWXG | S_IRWXO)  
int mk_dir(char *dir)
{
	DIR *mydir = NULL;
	if ((mydir = opendir(dir)) == NULL)//�ж�Ŀ¼   
	{
		int ret = mkdir(dir, MODE);//����Ŀ¼  
		if (ret != 0)
		{
			return -1;
		}
		printf("%s created sucess!/n", dir);
	}
	else
	{
		printf("%s exist!/n", dir);
	}

	return 0;
}


int processHTTPDown(int mode, char *url, char *DFileName)
{
	/* �����в���: ������������, ��һ�������ص�ַ, �ڶ������ļ��ı���λ�ú�����, ���ص�ַ�Ǳ����, Ĭ�����ص���ǰĿ¼
	* ʾ��: ./download http://www.baidu.com baidu.html
	*/
	//    char url[2048] = "http://120.77.173.234/download/BLK.tar";//����Ĭ�ϵ�ַΪ����,
	char host[64] = { 0 };//Զ��������ַ
	char ip_addr[16] = { 0 };//Զ������IP��ַ
	int port = 80;//Զ�������˿�, httpĬ��80�˿�
	char file_name[256] = { 0 };//�����ļ���
	char downfiletemp[256];

	printf("************************down start****************\n");
	printf("argv[0] =%s\n", url);

	//  strcpy(url, (char*)desAddr);


	if (mk_dir(DownDir) < 0) {
		printf("Creat QR Dir Error\r\n");
		return -1;
	}

	printf("1: ���ڽ������ص�ַ...%s\n", url);
	parse_url(url, host, &port, file_name);//��url�з�����������, �˿ں�, �ļ���

	puts("2: ���ڻ�ȡԶ�̷�����IP��ַ...");
	get_ip_addr(host, ip_addr);//���ú���ͬ����DNS��������ȡԶ��������IP
	if (strlen(ip_addr) == 0)
	{
		printf("����: �޷���ȡ��Զ�̷�������IP��ַ, �������ص�ַ����Ч��\n");
		return -1;
	}

	strcpy(file_name, DownDir);
	strcat(file_name, "/");
	strcat(file_name, DFileName);	//ʹ�ô��������ļ���
	puts("\n>>>>���ص�ַ�����ɹ�<<<<");
	printf("\t���ص�ַ: %s\n", url);
	printf("\tԶ������: %s\n", host);
	printf("\tIP �� ַ: %s\n", ip_addr);
	printf("\t����PORT: %d\n", port);
	printf("\t �ļ��� : %s\n\n", file_name);


	strcpy(downfiletemp, DownDir);
	strcat(downfiletemp, "/downtemp.tar");

	//����http����ͷ��Ϣ
	char header[2048] = { 0 };
	sprintf(header, \
		"GET %s HTTP/1.1\r\n"\
		"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"\
		"User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537(KHTML, like Gecko) Chrome/47.0.2526Safari/537.36\r\n"\
		"Host: %s\r\n"\
		"Connection: keep-alive\r\n"\
		"\r\n"\
		, url, host);

	puts("3: ���������׽���...");
	int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket < 0)
	{
		printf("�׽��ִ���ʧ��: %d\n", client_socket);
		return -2;//exit(-1);
	}

	//����IP��ַ�ṹ��
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_addr);
	addr.sin_port = htons(port);

	//����Զ������
	puts("4: ��������Զ������...");//�������û�з��������1�ֶ��ӵĳ�ʱ���˳�
	int res = connect(client_socket, (struct sockaddr *) &addr, sizeof(addr));
	if (res == -1)
	{
		printf("����Զ������ʧ��, error: %d\n", res);
		return -3;//exit(-1);
	}

	puts("5: ���ڷ���http��������...");
	write(client_socket, header, strlen(header));//writeϵͳ����, ������header���͸�������


	int mem_size = 4096;
	int length = 0;
	int len;
	char *buf = (char *)malloc(mem_size * sizeof(char));
	char *response = (char *)malloc(mem_size * sizeof(char));

	//ÿ�ε����ַ���ȡ��Ӧͷ��Ϣ
	puts("6: ���ڽ���http��Ӧͷ...");
	while ((len = read(client_socket, buf, 1)) != 0)
	{
		if (length + len > mem_size)
		{
			//��̬�ڴ�����, ��Ϊ�޷�ȷ����Ӧͷ���ݳ���
			mem_size *= 2;
			char * temp = (char *)realloc(response, sizeof(char) * mem_size);
			if (temp == NULL)
			{
				printf("��̬�ڴ�����ʧ��\n");
				return -4;//exit(-1);
			}
			response = temp;
		}

		buf[len] = '\0';
		strcat(response, buf);

		//�ҵ���Ӧͷ��ͷ����Ϣ
		int flag = 0;
		int i;
		for (i = strlen(response) - 1; response[i] == '\n' || response[i] == '\r'; i--, flag++);
		if (flag == 4)//�����������кͻس���ʾ�Ѿ�������Ӧͷ��ͷβ, �������ֵľ�����Ҫ���ص�����
			break;

		length += len;
	}

	struct HTTP_RES_HEADER resp = parse_header(response);

	printf("\n>>>>http��Ӧͷ�����ɹ�:<<<<\n");

	printf("\tHTTP��Ӧ����: %d\n", resp.status_code);
	if (resp.status_code != 200)
	{
		printf("�ļ��޷�����, Զ����������: %d\n", resp.status_code);
		return -5;
	}
	printf("\tHTTP�ĵ�����: %s\n", resp.content_type);
	printf("\tHTTP���峤��: %ld�ֽ�\n\n", resp.content_length);



	printf("7: ��ʼ�ļ�����...\n");

	if (access(downfiletemp, F_OK) != -1)
	{

		printf("%s�ļ����ڣ�ɾ��\n", downfiletemp);
		remove(downfiletemp);
	}
	else
		printf("û�л����ļ�\n");
	download(client_socket, downfiletemp, resp.content_length, DFileName);
	printf("8: �ر��׽���\n");

	if (resp.content_length == get_file_size(downfiletemp))
	{

		if (rename(downfiletemp, file_name) == 0)
			printf("\n�ļ�%s���سɹ�! ^_^\n\n", file_name);
		else
		{
			remove(downfiletemp);
			remove(file_name);
			perror("rename");
		}
	}
	else
	{
		remove(file_name);
		printf("\n�ļ����������ֽ�ȱʧ, ����ʧ��, ������!\n\n");
	}

	shutdown(client_socket, 2);//�ر��׽��ֵĽ��պͷ���

	if (mode == 0)//���ص�Ŀ¼����
		CheckDownloadFile(file_name, DFileName);
	return 0;
}



void* main_HTTPDataDown(void *arg)
{
	int n, ret;
	boolean f = FALSE;
	char fullName[256];
	char IndexName[256];
	char tarName[256];
	//	char HttpDes[] = "http://120.77.173.234/download/";		//���ﻹ��ʹ�õĹ̶�Ŀ¼,��ʽʹ��ʱҪ�͹�����ͨѶ��������ַһ��
	//	char HttpDes[] = "http://139.199.161.164:12020/File/Down?merchant=00000000&file=BLKBUS&ver=0012";

	while (1) {
		if (gGprsinfo.isNetOK[LINK_GJ] == TRUE) {
			if (gHttpDinfo.Dtype == HTTP_NEED_DOWN) {
#ifdef _debug_
				printf("HTTPData gHttpDinfo.Filename:%s\r\n", gHttpDinfo.Filename);
#endif
				if (strlen(gHttpDinfo.Filename) != 0) {	//�ļ�������Ӧ������

					strcpy(fullName, DownDir);
					strcat(fullName, "/");
					strcat(fullName, gHttpDinfo.Filename);

					getFilesName(gHttpDinfo.FFlag, IndexName);
#ifdef _debug_
					printf("main HTTP DOWN fullName:%s\r\n", gHttpDinfo.Filename);
					printf("main HTTP DOWN addr:%s\r\n", gHttpDinfo.HttpAddr);
					printf("main HTTP DOWN Afile:%s\r\n", IndexName);
#endif

					if (strcmp(gHttpDinfo.Filename, IndexName) == 0)
					{	//�ļ����ھͲ���������
						printf("%s File exist!\n", fullName);
						memset(gHttpDinfo.Filename, 0, sizeof(gHttpDinfo.Filename));
						gHttpDinfo.Dtype = HTTP_FINISH_DOWN;
					}
					else {
						ret = processHTTPDown(1, gHttpDinfo.HttpAddr, gHttpDinfo.Filename);
						if (ret == 0)
						{//���سɹ�
							if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_BLK, 3) == 0)
								strcpy(gEc20VerInfo.BLKName, gHttpDinfo.Filename);
							else if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_PRO, 3) == 0)
								strcpy(gEc20VerInfo.busProName, gHttpDinfo.Filename);
							else if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_EC20, 3) == 0)
								strcpy(gEc20VerInfo.EC20ProName, gHttpDinfo.Filename);


							memset(IndexName, 0, sizeof(IndexName));
							strcpy(IndexName, DownDir);
							strcat(IndexName, "/");
							memcpy(IndexName + strlen(IndexName), gHttpDinfo.FFlag, 3);
							strcat(IndexName, "index");
							if (access(IndexName, F_OK) != -1)
							{
								printf("%s file exist,delete\n", IndexName);
								remove(IndexName);
							}

							int fd = open(IndexName, O_CREAT | O_WRONLY, S_IRWXG | S_IRWXO | S_IRWXU);	//д�����������ļ�
							if (fd >= 0)
							{
								ret = write(fd, gHttpDinfo.Filename, strlen(gHttpDinfo.Filename));//д��ԭʼ�ļ������ļ���=3�ֽڱ�ʶ_4�ֽڰ汾��
							}
							close(fd);

							memset(tarName, 0, sizeof(tarName));
							if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_BLK, 3) == 0)
								strcpy(tarName, BLACK_LIST_FILE_NAME);
							else if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_PRO, 3) == 0)
								strcpy(tarName, BUSPRO_LIST_FILE_NAME);
							else if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_EC20, 3) == 0)
								strcpy(tarName, EC20PRO_LIST_FILE_NAME);

							remove(tarName);
							if (rename(fullName, tarName) == 0) {
								printf("\n%s complete! ^_^\n\n", tarName);
							}
							else
							{
								remove(fullName);
								perror("rename");
							}

							if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_EC20, 3) == 0) {
								//��ԭ�г��򱸷�
								if (access("/usrdata/qr/wxhj_bak/", F_OK) == -1) {
									mkdir("/usrdata/qr/wxhj_bak", 0777);
								}

								system("mv -f /usrdata/qr/ec20net /usrdata/qr/wxhj_bak/");

								strcpy(tarName, "tar -xvf ");
								strcat(tarName, EC20PRO_LIST_FILE_NAME);
								strcat(tarName, " -C /usrdata/qr/");

								ret = system(tarName);

								remove(IndexName);
								MSG_LOG("SYStem:ret:%d\r\n", ret);
								sleep(1);
								system("reboot");

							}
							else if (memcmp(gHttpDinfo.FFlag, SL8583FileFLAG_PRO, 3) == 0) {	//�����˳��ػ�����������ɺ���Ҫ�ѳ��򴫸����ػ�
																								//								SendPROtoBUS();	//�� OtherMission ��ʱ�������·�
							}
							else {
								getEc20FilesVer();	//���»�ȡ�ļ��汾���Ѱ汾��Ϣ������
							}

							memset(gHttpDinfo.Filename, 0, sizeof(gHttpDinfo.Filename));//����ļ�������ȥ���Ƿ����µ��ļ�
							gHttpDinfo.Dtype = HTTP_FINISH_DOWN;
						}
						else {//���ش��󣬻�Ҫ���أ�
							printf("main_HTTPDown 1 error:%d \n", ret);

							n = 0;
							memset(IndexName, 0, sizeof(IndexName));	//���ͽ��ȵ�����,����ˣ�Ҫ����
							IndexName[n++] = 2;
							strcpy((char *)IndexName + n, gHttpDinfo.Filename);
							n += 20;
							memcpy(IndexName + n, "\xFF\xFF\xFF\xFF", 4);
							n += 4;
							memcpy(IndexName + n, "\xFF\xFF\xFF\xFF", 4);
							n += 4;
							//com_send_data_OTHER_CMD(0, 0x09, n, IndexName);//CMD��09��������Ӧ�����ҪӦ����Ҫ��29

							memset(gHttpDinfo.Filename, 0, sizeof(gHttpDinfo.Filename));//������������
							gHttpDinfo.Dtype = HTTP_FINISH_DOWN;
						}
					}
				}
				else {
					gHttpDinfo.Dtype = HTTP_FINISH_DOWN;
				}
			}
		}
		sleep(1);
	}
	pthread_exit((void *)2);  // Explicitly exit the thread, and return (void *)2

}
